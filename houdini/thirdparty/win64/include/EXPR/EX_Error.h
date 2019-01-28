/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Error handling for EXPR (EXPR Library, C++)
 *
 * COMMENTS:	Functions in this file are implemented in evparse.C
 */

#ifndef __EX_ERROR_H__
#define __EX_ERROR_H__

#include "EXPR_API.h"
#include <UT/UT_ThreadSpecificValue.h>


#define EV_NOERROR	0		/* No error */
#define EV_ERRSYNTAX	1		/* Syntax error */
#define EV_ERRBRACING	2		/* Bracing error */
#define EV_ERRNOFUNC	3		/* Unknown function */
#define EV_ERRNOOP	4		/* Unknown operator */
#define EV_ERRBADTYPE	5		/* Unknown operator */
#define EV_ERRBADARGS	6		/* Invalid number of arguments */
#define EV_ERRUNKNOWN	7		/* Unknown error */
#define EV_ERRRECURSION	8		/* Recursion too deep */
#define EV_ERRSTACK	9		/* Stack error */
#define EV_ERRBADVAR	10		/* Undefined variable */
#define EV_NAN		11		// Divide by zero or something like that
#define EV_ERREXTRAJUNK	12		// Extra junk at end of expression
#define EV_ERRUNSAFEFUNC 13		// Unsafe function
#define EV_ERRINLINEFUNC 14		// Error in inline function
#define EV_ERRDEADLOCK	 15		// Deadlock
#define EV_ERRCUSTOM	 16		// Custom error text


// Simply create a namespace to hide the global TLS variable. We're doing
// things like this in order to allow the compiler to inline.
namespace EXPR
{
    EXPR_API extern UT_ThreadSpecificValue<int>    theErrNo;
};

/// Set the current errno, returning the old value
static inline void
ev_SetErrNo(int err, int thread)
{
    EXPR::theErrNo.getValueForThread(thread) = err;
}

/// Retrieve the errno for the given thread
static inline int
ev_GetErrNo(int thread)
{
    return EXPR::theErrNo.getValueForThread(thread);
}

/// Retrieve a string representing the error
EXPR_API extern const char      *ev_GetErrorText(int error_number);
/// Sets an error, EV_ERRCUSTOM, using the given custom message as the text
EXPR_API extern void		 ev_SetCustomErrorMessage(const char *message,
							  int thread);
/// Clears the cycle detected flag for a particular thread. This will force
/// a more expensive check for cycles next time, but allows us to clear away
/// recursion errors that we dont care about.
EXPR_API extern void		 ev_ClearCycles(int thread);

#endif // __EX_ERROR_H__
