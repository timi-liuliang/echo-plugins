/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 *
 * NAME:	UT_VarScan.h (C++)
 *
 * COMMENTS:
 *
 */

#ifndef __UT_VarScan_h__
#define __UT_VarScan_h__

#include "UT_API.h"

class UT_WorkBuffer;

typedef const char *(*UTVarCallback)(const char *str, void *userdata);

/// UTVariableScan copies the input string instr to the work
/// string outstr searching for variable references of the form
/// $name or ${name} in the input string.  Each such variable
/// reference found in the input string is replaced by its value
/// which is obtained by passing the variable name to the func-
/// tion referenced by the given argument lookup. If the given
/// function fails by returning a null pointer then the variable
/// reference is replaced in the output string by a null string.
///
/// If a null function pointer is passed as the lookup argument,
/// then UTVariableScan calls the function getenv to find the
/// value of each variable.
/// 
/// UTVariableScan returns the integer number of variables which
/// have been expanded.
/// @note If 0 is returned, outstr might still be different from instr
/// due to backslash escaping.
UT_API extern int UTVariableScan(UT_WorkBuffer &outstr, const char *instr,
				 UTVarCallback lookup, void *userdata,
				 bool tildeexpand = true,
				 bool commentsexpand = true);

/// UTExprVariableScan writes the output to a fixed length char * buffer
/// in outstr of size n. The number of variables that were expanded is returned.
/// @note If 0 is returned, outstr might still be different from instr
/// due to backslash escaping.
UT_API extern int UTExprVariableScan(UT_WorkBuffer &outstr, const char *instr,
				     UTVarCallback lookup,
				     UTVarCallback expreval,
				     void *userdata,
				     bool varexpand = true,
				     bool tildeexpand = true,
				     bool commentsexpand = true);

/// Convenience method for expanding environment variables from instr to outstr
static inline void
UTexpandVariables(UT_WorkBuffer &outstr, const char *instr)
{
    UTVariableScan(outstr, instr ? instr : "", NULL, NULL);
}

#endif
