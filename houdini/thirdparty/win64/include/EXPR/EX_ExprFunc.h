/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Expr library (C++)
 *
 * COMMENTS:	This is a real C++ class which provides an interface
 *		to the expression functions for Houdini
 *
 */

#ifndef __EX_Func_h__
#define __EX_Func_h__

#include "EXPR_API.h"
#include "EXPR.h"
#include <UT/UT_IntArray.h>
#include <UT/UT_Set.h>
#include <iosfwd>

class EX_Stream;		// Internal classes
class EX_Token;
class EX_Function;

class EXPR_API EX_Symbol {
public:
     //EX_Symbol(const char *name);
     EX_Symbol(const char *name, int type, int thread);
    ~EX_Symbol();

    int64 getMemoryUsage(bool inclusive) const;

    int		 myFlag;
    UT_String	 myName;
    EV_SYMBOL	*mySym;
    EX_Symbol	*myNext;
};

class EXPR_API EX_ExprLine {
public:
     EX_ExprLine();
    ~EX_ExprLine();

    int64 getMemoryUsage(bool inclusive) const;

    bool	saveCompiled(std::ostream &os);
    bool	loadCompiled(UT_IStream &is);

    void	setExpression(const char *source);

    int			 myFlag;
    int			 myLineNum;
    int			 myOperand;
    EX_Symbol		*mySymbols;
    EV_EXPRESSION	*myExpr;
    EX_ExprLine		*myBranch;
    EX_ExprLine		*myNext;
    int			 myBranchNum;
};

class EXPR_API EX_ExprFunc {
public:
     EX_ExprFunc();
    ~EX_ExprFunc();

    int64	getMemoryUsage(bool inclusive) const;

    // These return 0 on failure, 1 on success.  When loading, the
    //	stream will be positioned just after the last line of the function
    //	read.
    int		save(std::ostream &os);
    bool	saveCompiled(std::ostream &os, int thread);
    bool	load(UT_IStream &is, const char *filename, std::ostream *err,
		     int thread);
    bool	loadCompiled(UT_IStream &is, int thread);

    // This function will wire in the function to the expression library
    void	 wire();

    const char	*getName() const { return myName; }
    int		 getNArgs() const { return myArgTypes.entries(); }
    const char	*getReturnType() const;
    const char	*getArgName(int i);
    const char	*getArgType(int i);

    void	 touchMyReferences();
    void	 functionDeleted(EV_EXPRESSION *expr);

    void	 updateDependency(void *ref_id, int thread);
    int		 changeOpReference(UT_String &new_source, 
				   const EV_ChangeOpReferenceParms &parms,
				   int thread);

    void	 setInlineFlag(bool f) { myInlineFlag = f; }
    bool	 getInlineFlag() const { return myInlineFlag; }

    int		 getRunningCount() const { return myRunningCount; }

    void	 unresolveLocalVars(int thread);

private:
    void	 unwire();

    void	 errorMessage(const char *fmt, int linenum = -1,
			    const char *a1=0, const char *a2=0);


    int		 getLastLineNum() const;
    int		 checkValidName(const char *name);
    int		 checkType(const char *name);
    EX_Symbol	*symbolExists(const char *name);
    EX_Symbol	*addToSymbolTable(const char *name, int type, bool exists,
				  int thread);
    void	 addLineToExpr(EX_ExprLine *line);
    int		 addLoopBranch(int linenum);
    void	 resolveBreakCont(EX_ExprLine *line, int brk, int cont);
    int		 getAssignment(EX_ExprLine *, EX_Stream &, EX_Token *, int eol,
			       int thread);
    int		 doForLine(EX_ExprLine *line, EX_Stream &is, int thread);
    int		 doWhileLine(EX_ExprLine *line, EX_Stream &is, int thread);
    int		 doIfLine(EX_ExprLine *line, EX_Stream &is, int thread);
    int		 doBreakLine(EX_ExprLine *line, EX_Stream &is, int);
    int		 doReturnLine(EX_ExprLine *line, EX_Stream &is);
    int		 addReturn0();
    int		 parseLine(EX_Stream &is, int thread);
    int		 resolveBranches();
    int		 parseFunction(EX_Stream &is, int thread);

    int		 getScopeNest(EX_Stream &is, int thread);

    void	 internalEvaluate(EV_SYMBOL *result, EV_SYMBOL **argv,
				  int thread, unsigned &func_flags);
    static void	 evaluate(EV_FUNCTION *, EV_SYMBOL *result, EV_SYMBOL **argv,
			  int thread, unsigned int &func_flags);

    // Load helper methods.
    int		 load(EX_Stream &xis, const char *filename, std::ostream *err,
		      int thread);

    UT_String	 myName;
    UT_String	 mySourceFile;
    UT_IntArray	 myArgTypes;
    int		 myResultType;
    UT_String	 mySource;
    std::ostream*myError;

    bool	 myInlineFlag;
    int		 myRunningCount;


    EX_Symbol			*mySymbols;
    EX_ExprLine			*myLines;
    EX_ExprLine			*myLastLine;
    EX_Function			*myTableEntry;
    UT_Set<EV_EXPRESSION *>	 myReferences;
};

#endif
