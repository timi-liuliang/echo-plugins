/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VEX_VexOp.h ( VEX Library, C++)
 *
 * COMMENTS:	External VEX function definition
 *
 * The signature of a VEX OP function determines the parameters the function
 * expects.  The signature is composed as follows:
 *	name@parms
 * where name is the user function name and the parms are the single character
 * representations of the parameters expected.  The single character mnemonics
 * are:
 *	F	= float
 *	I	= integer
 *	S	= string
 *	V	= vector
 *	P	= vector4 (originally "point")
 *	3	= matrix3
 *	4	= matrix4
 *
 * Arrays of primitive types use the single character mnemonic for that
 * type preceeded by the open square bracket ([) character.  The array
 * types are then represented as:
 *	[F	= float array
 *	[I	= integer array
 *	[S	= string array
 *	[V	= vector array
 *	[P	= vector4 array
 *	[3	= matrix3 array
 *	[4	= matrix4 array
 *
 * If a parameter is written to by the VexOp function, the parameter mnemonic
 * should be preceded by an ampersand (&) or an asterix (*).  The ampersand is
 * used it the value of the variable is not read by the function, while the
 * asterix is used if the variable is read and then modified.  The signature
 * specification is very important if the compiler is to work correctly.
 *
 * The compiler is "smart" in that if only one argument is written to (and not
 * read from), that argument will be flagged as the return code for the
 * function.
 *
 * Examples:
 *	Signature:	test@&IF
 *	Decoded:	int test(float)
 *
 *	Signature:	test@*IF
 *	Decoded:	void test(int &, float)
 *	Details:	The integer parameter must be set since this function
 *			reads the argument before modifying it.
 *
 *	Signature:	test@S&VFF
 *	Decoded:	vector test(string; float, float)
 *
 *	Signature:	test@&IF+
 *	Decoded:	int test(float, ...)
 *
 */

#ifndef __VEX_VexOp__
#define __VEX_VexOp__

#include "VEX_API.h"
#include "VEX_PodTypes.h"
#include "VEX_VexTypes.h"
#include "VEX_Error.h"

/// A structure used to provide arguments with type information to the
/// callback function.
struct VEX_VexOpArg {
    void	*myArg;
    VEX_Type	 myType;
    bool	 myArray;
};

/// VEX operator initialization callback. When called, it should allocated
/// and initialize custom state data.
typedef void	*(*VEX_VexOpInit)();
/// VEX operator evaluation callback. When called, the data returned by the
/// VEX_VexOpInit function will be passed as the "data" argument.
typedef void	 (*VEX_VexOpCallback)(int argc, void *argv[], void *data);
/// VEX operator evaluation callback with argument type information. This
/// version of the operator callback must be used for variadic functions.
/// When called, the data returned by the VEX_VexOpInit function will be
/// passed as the "data" argument.
typedef void	 (*VEX_VexOpTypedCallback)(
			int argc, VEX_VexOpArg argv[], void *data);
/// VEX operator cleanup callback. When called, it should deallocate the data.
typedef void	 (*VEX_VexOpCleanup)(void *data);
/// VEX operator completed callback.  Called when a shader that evaluated a
/// custom operator completes.
typedef void	 (*VEX_VexOpCompleted)(void *data);

//
// It's possible to give hints to the compiler as to what optimizations may be
// performed on the function.  The different levels of optimizations are:
//	0 = No optimizations may be performed on the custom function.
//	1 = If the result of the function is unused, the function can
//	    be eliminated.
//	2 = If all parameters to the function are constant, then the result may
//	    be pre-computed and cached.
// The only time you would want to turn off optimization is to guarantee that
// your custom function is evaluated in all cases (i.e. something like printf).
// Typically, functions used in computation can be optimized by VEX.
//
// Turning off optimization may result in an overall performance hit.
//

/// @def VEX_OPTIMIZE_0
///	Perform no optimization on the VEX function
///	@warning this may impact performance severely.
/// @def VEX_OPTIMIZE_1
///	If the result of the function is unused, the function can be
///	eliminated.
/// @def VEX_OPTIMIZE_2
//	If all parameters to the function are constant, then the result may
//	be pre-computed and cached. (preferred mode)
#define VEX_OPTIMIZE_0		0
#define VEX_OPTIMIZE_1		1
#define VEX_OPTIMIZE_2		2

//
// When a function modifies only one parameter (without reading from it), the
// compiler will automatically interpret the first argument as a return code
// for the function.  However, if the function modifies multiple arguments, the
// compiler assumes that all arguments should be passed by reference.
// Sometimes, it's useful to have the first modified argument treated as a
// return code rather than an actual parameter to the function.  This behaviour
// can be forced (if all conditions are met) by passing in a true on the
// "force_return_code" parameter.
// For example, something like the function:
//	import@&IS&F
// would decode to
//	import(int &success, string, float &result)
// But, with the force_return_code argument set to true, the function would be
// interpreted as:
//	int import(string, float &result)
// which is most likely more friendly to the user.
//

/// Use this class to extend VEX by adding custom VEX functions.
class VEX_API VEX_VexOp {
public:
    /// Function declaration
    /// @param signature @n The function name and parameters expected
    /// @param evaluator32 @n The evaluation callback (32 bit)
    /// @param evaluator64 @n The evaluation callback (64 bit)
    /// @param ctx_mask @n Which contexts this function is available in
    /// @param init32 @n Initialization callback (called for every 32 bit instance)
    /// @param init64 @n Initialization callback (called for every 64 bit instance)
    /// @param cleanup32 @n Cleanup callback (32 bit)
    /// @param cleanup64 @n Cleanup callback (64 bit)
    /// @param optimize_level @n Optimization level
    /// @param force_return_code @n
    ///		For signatures which have multiple write-only variables, this
    ///		@c bool forces the first write-only variable to be interpreted
    ///		as a return code.
    /// @{
    VEX_VexOp(const UT_StringHolder &signature,
	      VEX_VexOpCallback	 evaluator32,
	      int		 ctx_mask=(VEX_ALL_CONTEXT),
	      VEX_VexOpInit	 init32 = 0,
	      VEX_VexOpCleanup	 cleanup32 = 0,
	      int		 optimize_level = VEX_OPTIMIZE_2,
	      bool		 force_return_code=false);
    VEX_VexOp(const UT_StringHolder &signature,
	      VEX_VexOpCallback	 evaluator32,
	      VEX_VexOpCallback	 evaluator64,
	      int		 ctx_mask=(VEX_ALL_CONTEXT),
	      VEX_VexOpInit	 init32 = 0,
	      VEX_VexOpInit	 init64 = 0,
	      VEX_VexOpCleanup	 cleanup32 = 0,
	      VEX_VexOpCleanup	 cleanup64 = 0,
	      int		 optimize_level = VEX_OPTIMIZE_2,
	      bool		 force_return_code=false);
    VEX_VexOp(const UT_StringHolder &signature,
	      VEX_VexOpTypedCallback	 evaluator32,
	      int			 ctx_mask=(VEX_ALL_CONTEXT),
	      VEX_VexOpInit		 init32 = 0,
	      VEX_VexOpCleanup		 cleanup32 = 0,
	      int			 optimize_level = VEX_OPTIMIZE_2,
	      bool			 force_return_code=false);
    VEX_VexOp(const UT_StringHolder &signature,
	      VEX_VexOpTypedCallback	 evaluator32,
	      VEX_VexOpTypedCallback	 evaluator64,
	      int			 ctx_mask=(VEX_ALL_CONTEXT),
	      VEX_VexOpInit		 init32 = 0,
	      VEX_VexOpInit		 init64 = 0,
	      VEX_VexOpCleanup		 cleanup32 = 0,
	      VEX_VexOpCleanup		 cleanup64 = 0,
	      int			 optimize_level = VEX_OPTIMIZE_2,
	      bool			 force_return_code=false);
    /// @}

    /// Standard destructor
    ~VEX_VexOp();

    /// Set the completed callback, called when all evaluate calls for a
    /// given shader invokation are finished.
    
    template <VEX_Precision PREC>
    void		 setCompleted(VEX_VexOpCompleted cback);

    /// Query the signature
    const UT_StringHolder	&getSignature() const	{ return mySignature; }
    /// Query the context mask
    int			 getContextMask() const	{ return myContextMask; }
    /// Query the initialization function
    template <VEX_Precision PREC>
    VEX_VexOpInit	 getInit() const;
    /// Query the evaluation function
    template <VEX_Precision PREC>
    VEX_VexOpCallback	 getEvaluator() const;
    /// Query the typed evaluation function
    template <VEX_Precision PREC>
    VEX_VexOpTypedCallback	 getTypedEvaluator() const;
    /// Query the cleanup function
    template <VEX_Precision PREC>
    VEX_VexOpCleanup	 getCleanup() const;
    /// Query the completed function
    template <VEX_Precision PREC>
    VEX_VexOpCompleted	 getCompleted() const;
    /// Query the optimization level
    int			 getOptimizer() const	{ return myOptimizeLevel; }

    /// @private: Internal used to query state
    unsigned int	 getFlag(unsigned int f) const	{ return myFlags&f; }
    /// @private: Internal used to store state
    void		 setFlag(unsigned int f)	{ myFlags |= f; }
    /// @private: Internal used to store state
    void		 clearFlag(unsigned int f)	{ myFlags &= ~f; }

    /// When dealing assigning to string arguments, you must "free" the previous
    /// string value by calling "stringFree".  You must set the resulting string
    /// to the return code from "stringAlloc".  If you do your own memory
    /// management (i.e. calling strdup() or setting strings to constant
    /// values), you will most likely crash VEX.
    /// For example:
    /// @code
    ///		void
    ///		vexStrCat(int argc, void *argv[], void *)
    ///		{
    ///		    // arg[0] = result, arg[1..2] == strings to concat
    ///		    char	work_buf[256];
    ///		    VEX_VexOp::stringFree((char *)argv[0]);
    ///		    strcpy(work_buf, (char *)argv[1]);
    ///		    strcat(work_buf, (char *)argv[2]);
    ///		    argv[0] = VEX_VexOp::stringAlloc(work_buf);
    ///		}
    /// @endcode
    static const char	*stringAlloc(const char *str);
    /// Function to free an allocated string
    static void		 stringFree(const char *str);

    /// During execution of a callback, this method can be used to retrieve
    /// the current VEX error log for error reporting.  This method will
    /// only operate correctly in the thread that executed the callback.
    static VEX_ErrorLog	&getErrorLog();

    /// These functions can be used to query the number of DSO's loaded by VEX
    /// and the locations of the shared objects loaded.
    static int			 getNumVexOps();
    /// Query the Nth function
    static const VEX_VexOp	*getVexOp(int idx);
    /// Get the path to the DSO associated with the VEX plug-in
    static const char		*getVexOpLocation(int idx);

protected:
    // The signature of the function call.  This is documented in the header
    // above.  The constructor will duplicate the string passed in.
    UT_StringHolder	mySignature;

    // Initialization, Evaluation and Cleanup callbacks
    VEX_VexOpInit		myInit32;
    VEX_VexOpCallback		myEvaluator32;
    VEX_VexOpTypedCallback	myTypedEvaluator32;
    VEX_VexOpCleanup		myCleanup32;
    VEX_VexOpCompleted		myCompleted32;

    VEX_VexOpInit		myInit64;
    VEX_VexOpCallback		myEvaluator64;
    VEX_VexOpTypedCallback	myTypedEvaluator64;
    VEX_VexOpCleanup		myCleanup64;
    VEX_VexOpCompleted		myCompleted64;

    // The contexts which this VEXOp is valid for.  The mask is a bit-field
    // represented by or'ing the context defines above together.
    int			 myContextMask;
    int			 myOptimizeLevel;
    unsigned int	 myFlags;

private:
    void		construct(const UT_StringHolder &signature,
				  VEX_VexOpCallback evaluator32,
				  VEX_VexOpCallback evaluator64,
				  VEX_VexOpTypedCallback typed_evaluator32,
				  VEX_VexOpTypedCallback typed_evaluator64,
				  int ctx_mask,
				  VEX_VexOpInit init32,
				  VEX_VexOpInit init64,
				  VEX_VexOpCleanup cleanup32,
				  VEX_VexOpCleanup cleanup64,
				  int optimize,
				  bool rcode);
};

extern "C" {
    /// Function called in DSO to install new functions
    /// All files found in $HOUDINI_DSO_PATH/vex are automatically scanned for
    /// this function.
    SYS_VISIBILITY_EXPORT extern void	newVEXOp(void *);
}

template <> inline void
VEX_VexOp::setCompleted<VEX_32>(VEX_VexOpCompleted callback)
{ myCompleted32 = callback; }
template <> inline VEX_VexOpInit
VEX_VexOp::getInit<VEX_32>() const
{ return myInit32; }
template <> inline VEX_VexOpCallback
VEX_VexOp::getEvaluator<VEX_32>() const
{ return myEvaluator32; }
template <> inline VEX_VexOpTypedCallback
VEX_VexOp::getTypedEvaluator<VEX_32>() const
{ return myTypedEvaluator32; }
template <> inline VEX_VexOpCleanup
VEX_VexOp::getCleanup<VEX_32>() const
{ return myCleanup32; }
template <> inline VEX_VexOpCleanup
VEX_VexOp::getCompleted<VEX_32>() const
{ return myCompleted32; }

template <> inline void
VEX_VexOp::setCompleted<VEX_64>(VEX_VexOpCompleted callback)
{ myCompleted64 = callback; }
template <> inline VEX_VexOpInit
VEX_VexOp::getInit<VEX_64>() const
{ return myInit64; }
template <> inline VEX_VexOpCallback
VEX_VexOp::getEvaluator<VEX_64>() const
{ return myEvaluator64; }
template <> inline VEX_VexOpTypedCallback
VEX_VexOp::getTypedEvaluator<VEX_64>() const
{ return myTypedEvaluator64; }
template <> inline VEX_VexOpCleanup
VEX_VexOp::getCleanup<VEX_64>() const
{ return myCleanup64; }
template <> inline VEX_VexOpCleanup
VEX_VexOp::getCompleted<VEX_64>() const
{ return myCompleted64; }

#endif
