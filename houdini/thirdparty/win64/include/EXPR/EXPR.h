/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	EXPR.h	(C)
 *
 * COMMENTS:	Include file for expression library.
 *
 */

#ifndef __LIBEXPR_H__
#define __LIBEXPR_H__

#include "EXPR_API.h"
#include "EX_Error.h"
#include "EXPR_Lock.h"
#include <UT/UT_SmallObject.h>
#include <UT/UT_String.h>
#include <SYS/SYS_Types.h>
#include <iosfwd>

class ev_Vector;
class ev_Matrix;
class EX_ExprFunc;
class UT_IStream;
class UT_WorkBuffer;

#define EV_OPTIMIZE0	0		/* No optimization */
#define EV_OPTIMIZE1	1		/* Constant folding */

#define MAX_RECURSION	20		/* Maximum depth of recursion */

#define EV_NHELPARGS	10		/* Maximum # of args in help */

typedef char	*(*ev_Expander)(const char *str, int thread);

/// Valid operator tokens - used by the operator typedef below
/// DON'T CHANGE THESE VALUES - They are saved as ints for compiled expressions.
/// @{
#define OP_PLUS		1		/* + */
#define OP_MINUS	2		/* - */
#define OP_TIMES	3		/* * */
#define OP_DIVIDE	4		/* / */
#define OP_POWER	5		/* ^ */
#define OP_MOD		6		/* % */
#define OP_NOT		7		/* ! */
#define OP_EQ		8		/* == */
#define OP_NE		9		/* != */
#define OP_LT		10		/* < */
#define OP_LE		11		/* <= */
#define OP_GT		12		/* > */
#define OP_GE		13		/* >= */
#define OP_LOGAND	14		/* && (logical and) */
#define OP_LOGOR	15		/* || (logical or) */
#define OP_COMMA	16		/* , almost a NOP */
#define OP_QUESTION	20		/* ? */
#define OP_COLON	21		/* : */
#define OP_AND		22		/* & */
#define OP_OR		23		/* | */
#define OP_DOT		24		/* . */
#define OP_TILDE	25		/* ~ */
#define OP_AT		26		/* @ */
#define OP_SBRACKET	30		/* [ end square bracket implied */
#define OP_BRACE	31		/* { end brace implied */
#define OP_EQUATE	32		/* = */
#define MAX_OPERATOR_TOKEN 32
/// @}

/// These flags are common to symbols, operators and functions
///
/// NOTE: The high 8 bits of the flag are reserved for internal use
///	  User flags should start at not go higher than 0x800000
///
/// Note: when changing this definition, update EX_USERFLAGMASK 
/// @{
#define EV_SYMTRANSIENT	0x04000000	///< Symbol is result of operation
#define EV_SYMCONSTANT	0x08000000	///< Symbol has a constant value
#define EV_FLAGS_SET	0x10000000	///< User flags have been set
#define EV_SYMEXPAND	0x20000000	///< String should be expanded
#define EV_EXFUNC_REF	0x40000000	///< Refers to an expression function
#define EV_FOR_EXFUNC	0x80000000	///< Symbol is to be used by expr func
/// @}

#define EX_USERFLAGMASK	(0x03ffffff | EV_EXFUNC_REF)


/// Implicit types known by expression language
///	- The type identifiers must be positive unique integers
///	- Eventhough Variables are handled as a special case, the
///		type identifier for each variable type must be unique
///	- It is suggested that applications start their typedefs identifiers
///		10 or greater.  The identifiers do not have to be sequential.
///
/// DON'T CHANGE THESE VALUES - They are saved as ints for compiled expressions.
/// @{
#define EV_TYPEFLOAT	 		0	///< Float type of symbol
#define EV_TYPESTRING	 		1	///< String type of symbol
#define EV_TYPEUNIXVAR	 		2	///< Unix variable type
#define EV_TYPEEXPANDVAR 		4	///< Variable of form ${foo$x}
#define EV_TYPEVECTOR	 		5	///< Vector
#define EV_TYPEMATRIX	 		6	///< Matrix
/// @}

/// These types are used in Houdini outside of EXPR.
/// DON'T CHANGE THESE VALUES - They are saved as ints for compiled expressions.
/// @{
#define EV_TYPE_LOCAL_VAR		10	///< A node local variable
#define EV_TYPE_UNRESOLVED_VAR		11	///< An unknown variable
#define EV_TYPE_DEFINE			12
#define EV_TYPE_GEOATTRIBUTE		13	///< An attribute reference
#define EV_TYPE_GLOBAL_VAR		20	///< A global variable
#define EV_TYPE_CPPSYMBOL		45	///< A symbol inside a function
#define EV_TYPE_UNRESOLVED_CPPSYMBOL	46	///< An unknown function symbol
/// @}

/// Typedefs for structures in the expression library
///	SYMBOL	 - any data in the expression (i.e. fpreal, string, variable)
///	TYPEDEF  - a definition for a type (i.e. fpreal, string, etc.)
///			Defines how to handle this data type
///	OPERATOR - binary or unary operator which takes symbol(s) and
///			performs some action resulting in a new symbol
///	FUNCTION - function which takes arguments and performs some action
///			on them to produce a new symbol
///	EXPRESSION - the type seen to the outside world
/// @{
class EV_FUNCTION;
struct EV_SYMBOL;
typedef struct EV_TYPEDEF	EV_TYPEDEF;
typedef struct EV_OPERATOR	EV_OPERATOR;
typedef struct EV_FUNCHELP	EV_FUNCHELP;
/// @}

/// This callback is used to scan for op dependencies and add/remove
/// dependencies.  Unlike evaluation, this callback can have null arguments.
/// A null argument signifies a non-constant parameter to the function.
typedef void	(*EXPRopDependencyCallback)(EV_FUNCTION *me,
					    EV_SYMBOL **argv,
					    void *ref_id);
typedef void	(*EXPRopChangeRefCallback)(EV_FUNCTION *me,
					  EV_SYMBOL **argv,
					  char *new_args[],
					  const char *new_fullpath,
					  const char *old_fullpath,
					  const char *old_cwd,
					  const char *chan_name,
					  const char *old_chan_name);

/// union used to hold a token's value
typedef union {
	fpreal		fval;
	char		*sval;
	void		*data;
} EV_TOKENVALUE;

/// class representing a symbol operated upon by expressions
struct EXPR_API EV_SYMBOL
{
	bool		isForExprFunc() const
			    { return ((flag & EV_FOR_EXFUNC) != 0); }
	void		setForExprFunc()
			    { flag |= EV_FOR_EXFUNC; }
	bool		isConstant() const
			    { return ((flag & EV_SYMCONSTANT) != 0); }
	bool		isTransient() const
			    { return ((flag & EV_SYMTRANSIENT) != 0); }

	unsigned	flag;	///< Flag for symbols
	EV_TYPEDEF	*type;	///< Interpretation of data
	EV_TOKENVALUE	value;	///< Token data
	EV_SYMBOL	*next;	///< Used Internally
	int		pos;	///< String pos in expression
};

struct EV_TYPEDEF {
	int		type;
	const char	*name;			/* Name of type */
	void		(*allocValue)(EV_SYMBOL *);
	void		(*freeValue)(EV_SYMBOL *);
	EV_SYMBOL	*(*resolveSymbol)(const char *, int thread);
	const char	*(*getName)(EV_SYMBOL *);
	int		(*castValue)(EV_SYMBOL *to, EV_SYMBOL *me, int thread);
	int		(*castFrom)(EV_SYMBOL *me, EV_SYMBOL *from);
	void		(*copyValue)(EV_SYMBOL *dest, EV_SYMBOL *src, int thread);
	int		(*getCastType)(EV_SYMBOL *src);
	void		(*opDepend)(EV_SYMBOL *src, void *ref_id);

	EV_TYPEDEF	*next;			/* Used Internally */
	EV_TYPEDEF	*varnext;		/* Used Internally */
};

/// struct describing an operator's characteristics
struct EV_OPERATOR {
	int		flag;		///< Flag for operator
	int		token;		///< OP_ token to identify
	unsigned char	binary;		///< Binary or unary operator
	int		leftType;	///< Data for left side of op
	int		rightType;	///< Data for right side of op
	int		resultType;	///< Resultant type of op

	void		(*evaluate)(EV_SYMBOL *, EV_SYMBOL *, EV_SYMBOL *);

	EV_OPERATOR	*next;		///< Used internally
};

/// Handy macros for defining expression functions.
/// They only differ by which function parameters are supplied to avoid
/// unused parameter warnings.
/// @{
#define EV_START_FN(name)	 \
	    static void name(EV_FUNCTION *, EV_SYMBOL *result,\
			     EV_SYMBOL **argv, int thread, unsigned &func_flags)
#define EV_START_FNNA(name) \
	    static void name(EV_FUNCTION *, EV_SYMBOL *result, \
			     EV_SYMBOL **, int thread, unsigned &func_flags)
#define EV_START_FUNC(name) \
	    static void name(EV_FUNCTION *func, EV_SYMBOL *result,\
			     EV_SYMBOL **argv, int thread, unsigned &func_flags)
#define EV_END_FN(answer)	 \
	    result->value.fval = (answer)
#define EV_END_FNS(answer)	 \
	    result->value.sval = (answer)
/// @}

typedef void (*EXPRfuncCallback)(EV_FUNCTION *me,
				 EV_SYMBOL *result,
				 EV_SYMBOL **argv,
				 int thread,
				 unsigned &func_flags);

///
class EXPR_API EV_FUNCTION {
public:
    EV_FUNCTION(unsigned	 flag = 0,
		const char	*name = 0,
		int		 num_args = 0,
		int		 result_type = EV_TYPEFLOAT,
		const int	*argTypes = 0,
		EXPRfuncCallback callback = 0,
		EXPRopDependencyCallback	dependCB = 0,
		EXPRopChangeRefCallback		changeOpRefCB = 0,
		int		 is_safe = 1,
		bool		 is_threadsafe = false);
    virtual ~EV_FUNCTION();

    virtual int64 getMemoryUsage(bool inclusive) const;

    unsigned	 getUserFlags() const		{ return myUserFlags; }
    const char	*getName() const	{ return myName; }
    int		 getNArgs() const	{ return myNArgs; }
    int		 getResultType() const	{ return myResultType; }
    const int	*getArgTypes() const	{ return myArgTypes; }
    int		 getArgType(int i) const	{ return myArgTypes[i]; }
    const char	*getDSOLocation() const	{ return myDSOLocation; }

    /// getInstanceData() returns the instance data for the current evaluating
    /// expression function
    static void	*getInstanceData(int thread);
    static void	 setInstanceAllocator(const char *function,
				      void *(*alloc)(), void (*freer)(void *));
    void	 setInstanceAllocator(void *(*alloc)(), void (*freer)(void *))
		 {
		     myAllocator = alloc;
		     myFreer = freer;
		 }
    void	*allocInstanceData() { return myAllocator ? myAllocator() : 0; }
    void	 freeInstanceData(void *d) { myFreer(d); }

    void	 setOpDependencyCallback(EXPRopDependencyCallback cb)
		 {
		     myDependCallback = cb;
		 }
    void	 setChangeOpRefCallback(EXPRopChangeRefCallback cb)
		 {
		     myChangeOpRefCallback = cb;
		 }

    // isSafe() tells us if we can call this expression function when the
    // expression evaluator is in "safe" mode.
    int		 isSafe() const { return myIsSafe; }

    void	 setIsThreadSafe(bool flag) { myIsThreadSafe = flag; }
    bool	 isThreadSafe() const	    { return myIsThreadSafe; }

public:
    EXPRfuncCallback		myCallback;
    EXPRopDependencyCallback	myDependCallback;
    EXPRopChangeRefCallback	myChangeOpRefCallback;

private:
    const char		*myName;
    char		*myDSOLocation;
    int			 myNArgs;
    int			 myResultType;
    const int		*myArgTypes;
    void		*(*myAllocator)();
    void		 (*myFreer)(void *);
    unsigned		 myUserFlags;
    unsigned		 myIsSafe:1,
			 myIsThreadSafe:1;
};


/// This is an enumeration of the type of return values that an inline
/// function may be expected to return.  Setting this value to 
/// EV_EXPR_RETURN_NONE will disallow an expression from supporting inline
/// functions.
enum EV_InlineFuncReturnType
{
    EV_EXPR_RETURN_NONE,
    EV_EXPR_RETURN_FLOAT,
    EV_EXPR_RETURN_STRING,
    EV_EXPR_RETURN_VECTOR,
    EV_EXPR_RETURN_MATRIX
};


/// Structure used as the parameter to ev_ChangeOpReference
/// (Currently only used internally)
struct EV_ChangeOpReferenceParms
{
    const char    *new_fullpath;
    const char    *old_fullpath;
    const char	  *old_cwd;
    const char	  *chan_name;
    const char	  *old_chan_name;
};

class in_INSTRUCTION;

class EXPR_API EV_EXPRESSION
		    : public UT_SmallObject<EV_EXPRESSION,
					    UT_SMALLOBJECT_CLEANPAGES_DEFAULT,
					    UT_SMALLOBJECT_PAGESIZE_DEFAULT,
					    UT_SMALLOBJECT_THREADSAFE_ON>
{
public:
    EV_EXPRESSION(EV_InlineFuncReturnType ret_type)
    {
	myFlags = 0;
	mySource = 0;
	myCompiled = 0;
	myRecurs = 0;
	myFunc = 0;
	myFuncRetType = ret_type;
	myInExprFunc = false;
	myNeedResolveVars = false;
	myIsPureCompiled = false;
    }
    ~EV_EXPRESSION()
    {
	freeExprSpace();
    }

    int64	    getMemoryUsage(bool inclusive) const;

    /// Common evaluation functions.
    /// NOTES:
    ///   - These functions will cast to the desired return type.
    ///   - The thread parameter should be equivalent to SYSgetSTID()
    /// @{
    bool	    evaluate(int thread, fpreal &result);
    bool	    evaluate(int thread, UT_String &result);
    bool	    evaluate(int thread, ev_Vector &result);
    bool	    evaluate(int thread, ev_Matrix &result);
    /// @}

    /// Raw evaluation function that does no casting.
    /// @{
    EV_SYMBOL *	    evaluate(int thread)
			{ return evaluate(myFuncRetType, thread); }
    EV_SYMBOL *	    evaluate(EV_InlineFuncReturnType func_ret_type, int thread);
    /// @}

    bool	    changeExpr(const char *source);
    void	    unresolveVars(int thread);
    void	    dirtyExpr();

    /// Dependencies
    /// @{
    void	    updateOpDependency(void *ref_id, int thread);
    int		    changeOpReference(const char *new_fullpath,
				      const char *old_fullpath,
				      const char *old_cwd,
				      const char *chan_name,
				      const char *old_chan_name,
				      int thread);
    /// @}

    /// Save and load compiled expressions
    /// @{
    bool	    saveCompiledCode(std::ostream &os, int thread);
    bool	    loadCompiledCode(UT_IStream &is, int thread);
    /// @}

    /// Flags
    /// @{
    unsigned int    getFlags() const
			{ return myFlags; }
    bool	    hasFlag(unsigned int bit) const
			{ return ((myFlags & bit) != 0); }
    void	    clearAllFlags()
			{ myFlags = 0; }
    void	    setFlags(unsigned int flags)
			{ myFlags = flags; }
    void	    appendFlags(unsigned int flag)
			{ myFlags |= flag; }
    void	    setEvaluated()
			{ myFlags |= EV_FLAGS_SET; }
    bool	    isEvaluated() const
			{ return ((myFlags & EV_FLAGS_SET) != 0); }

    /// This function is called before evaluating EXPR expressions and python
    /// expressions to clear the appropriate flags on the expression.
    void	    clearFlagsBeforeEvaluation()
			{ myFlags &= ~(EX_USERFLAGMASK | EV_FLAGS_SET); }
    /// @}

    /// Other accesors
    /// @{
    const char *    getSource() const
			{ return mySource; }
    bool	    isCompiled() const
			{ return (myCompiled != 0); }

    bool	    isPureCompiled() const
			{ return myIsPureCompiled; }

    /// getSingleFunctionInstanceData() returns the instance data for this
    /// expression's function which must be of form func("literal").
    /// Returns NULL if this expression doesn't have the right form.
    void *	    getSingleFunctionInstanceData(
			EV_FUNCTION *func, int thread, UT_String &argument);

    /// isInExprFunc() says whether we are an expression inside a custom expr
    /// function. It is turned on in EX_ExprFunc.
    bool	    isInExprFunc() const
			{ return myInExprFunc; }
    void	    setInExprFunc(bool is_inside)
			{ myInExprFunc = is_inside; }
    /// @}

    void	    setInlineFuncRetType(EV_InlineFuncReturnType ret_type)
			{ myFuncRetType = ret_type; }

    /// Return a reference for thread-safe access
    EXPR_Lock &	    getLock() const
			{ return myLock; }

private:    // methods
    void	    freeCompiledExpr(EV_SYMBOL *&free_list);
    bool	    parseInlineFunc(UT_String &func_call, int thread);
    int		    compile(const char *tstring, int thread,
			    bool ignore_bad_vars);
    int		    parseExpr(EV_InlineFuncReturnType func_ret_type,
			      int thread, bool ignore_bad_vars);
    bool	    freeExprSpace();
    void	    doUpdateOpDependency(void *ref_id, int thread);
    int		    doChangeOpReference(UT_String &new_source,
					EV_ChangeOpReferenceParms &parms,
					int thread);

private:    // data
    unsigned			 myFlags;	/* Expression flag */
    char			*mySource;	/* Readable expression */
    in_INSTRUCTION		*myCompiled;	/* Compiled expression */
    int				 myRecurs;	/* Recursion count */
    EX_ExprFunc			*myFunc;	/* Inline function */
    EV_InlineFuncReturnType	 myFuncRetType;	/* Return type for inline func*/
    mutable EXPR_Lock		 myLock;
    bool			 myInExprFunc:1;/* Inside a custom EXPR func*/
    bool			 myNeedResolveVars:1;
    // True if the expression was loaded as a compiled 
    // expression, false otherwise.
    bool			 myIsPureCompiled:1;
};

/// initialize floating point operations
/// Float initialization will automatically initialize vector/matrix
EXPR_API extern void		ev_InitFloat(void);
/// initialize string operations
EXPR_API extern void		ev_InitString(void);
EXPR_API extern void		ev_InitVariable(void);
/// initialize vector operations
EXPR_API extern void 		ev_InitVector(void);
/// initialize matrix operations
EXPR_API extern void 		ev_InitMatrix(void);
EXPR_API extern void 		ev_InitUserFunc(void);

///  Functions for expansion of library
///	Add operators, type definitions and functions.
/// @{
EXPR_API extern void		ev_AddType(EV_TYPEDEF *type);
EXPR_API extern void		ev_AddVariableType(EV_TYPEDEF *type, int atEnd);
EXPR_API extern void		ev_DeleteVariableType(EV_TYPEDEF *type);
EXPR_API extern void		ev_AddOperator(EV_OPERATOR *op, int level);
EXPR_API extern void		ev_AddFunction(EV_FUNCTION *func);
EXPR_API extern void		ev_DeleteFunction(EV_FUNCTION *func);
EXPR_API extern void		ev_SetOptimization(int level);
/// @}

EXPR_API extern void		ev_PrintHelp(std::ostream &os, const char *match,
					     const char *keyword, 
					     bool full_match_only = false);

EXPR_API extern int		 ev_GetNFunctions();
EXPR_API extern EV_FUNCTION	*ev_GetFunction(int i);
EXPR_API extern EV_TYPEDEF	*ev_GetTypeDef(int key);
EXPR_API extern int		 ev_FindFunction(const char *name);

/// Functions for supplying state data for custom expressions.
// @{

/// This function specifies the constructor/destructor functions for the 
/// state data.
/// @param function	Name of the custom expression function
/// @param alloc	This allocates and initializes the state data
/// @param free		This deallocates the state data
EXPR_API extern void		 ev_SetFunctionInstanceAllocator(
						const char *function,
						void *(*alloc)(),
						void (*free)(void *));

/// From within an expression function, use this to retrieve the data created
/// by the alloc function specified in ev_SetFunctionInstanceAllocator().
/// @param thread	Thread id supplied to the expression function callback
EXPR_API extern void		*ev_GetFunctionData(int thread);

// @}

EXPR_API extern void		 ev_SetFunctionDependencyCallbacks(
					const char *func,
					EXPRopDependencyCallback depend_cb,
					EXPRopChangeRefCallback changeref_cb);

// Basic level of operation as seen by application
EXPR_API extern EV_EXPRESSION	*ev_AllocExpr(EV_InlineFuncReturnType rettype);
EXPR_API extern int		 ev_ChangeExpr(EV_EXPRESSION *expr,
						const char *source);
EXPR_API extern void		 ev_UnresolveVars(EV_EXPRESSION *expr,
						  int thread);
EXPR_API extern fpreal		 ev_EvaluateFloat(EV_EXPRESSION *expr,
						int thread);
EXPR_API extern void		 ev_EvaluateString(UT_String &result,
						   EV_EXPRESSION *expr,
						   int thread);
EXPR_API extern int		 ev_EvaluateVector(EV_EXPRESSION *expr,
						ev_Vector &result,
						int thread);
EXPR_API extern int		 ev_EvaluateMatrix(EV_EXPRESSION *expr,
						ev_Matrix &result,
						int thread);
EXPR_API extern void		 ev_FreeExpr(EV_EXPRESSION *expr);

EXPR_API extern void		 ev_UpdateOpDependency(EV_EXPRESSION *expr,
						void *ref_id, int thread);
EXPR_API extern int		 ev_ChangeOpReference(EV_EXPRESSION *expr,
						const char *new_fullpath,
						const char *old_fullpath,
						const char *old_cwd,
						const char *chan_name,
						const char *old_chan_name,
						int thread);
EXPR_API extern EV_SYMBOL	*ev_Evaluate(
				    EV_EXPRESSION *expr,
				    EV_InlineFuncReturnType func_ret_type,
				    int thread);

/// These functions save and load compiled expressions.
/// @{
EXPR_API extern bool		 ev_SaveCompiledCode(EV_EXPRESSION *expr,
						std::ostream &os, int thread);
EXPR_API extern bool		 ev_LoadCompiledCode(EV_EXPRESSION *expr,
						UT_IStream &is, int thread);
/// @}

/// Functions used internally by type declarations
/// @{
EXPR_API extern EV_SYMBOL	*ev_AllocSymbol(int type, int thread);
EXPR_API extern void		 ev_FreeSymbol(EV_SYMBOL *symbol, int thread);
/// @}

/// Nice conversion of float to string data.  This will do simple checks to try
/// to convert integer values more correctly.
EXPR_API extern void	EXPRftoa(UT_WorkBuffer &buf, fpreal v);

/// Following are the defines/typedefs for the language handler
/// @{
typedef struct EV_SYMTABLE	EV_SYMTABLE;

struct EV_SYMTABLE {			/* Used internally */
	int		flag;
	char		*name;
	fpreal		*value;		/* Value of symbol */
	EV_SYMBOL	*sym;		/* symbol	*/
	EV_SYMTABLE	*next;
};

struct EV_FUNCHELP {
	char		*name;			/* Name of the function */
	char		*argname[EV_NHELPARGS];	/* Arg names */
	char		*description;		/* A brief description */
};
/// @}

EXPR_API extern void		ev_SetExpander(ev_Expander expander);

/// These functions put the expression evaluator into/out of "safe" mode.
/// In safe mode, certain functions will not be executed.
/// @{
EXPR_API extern void		 ev_setSafeMode(bool safe_mode);
EXPR_API extern bool		 ev_isInSafeMode();
EXPR_API extern bool		 ev_isKeyword(const char *string);
/// @}

#endif
