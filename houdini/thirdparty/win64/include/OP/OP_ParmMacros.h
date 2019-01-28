/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP Library (C++)
 *
 * COMMENTS:	Convenient macros for evaluating parms.
 *
 */
#ifndef OP_PARM_MACROS_H
#define OP_PARM_MACROS_H

//
// macros to use parmIndirect[] array which optimizes UI parameter access
//
#define EVAL_STR(name, idx, vi, t) \
	    evalString(str, name, &parmIndirect[idx], vi, t);
#define EVAL_INT(name, idx, vi, t) \
	    return evalInt(name, &parmIndirect[idx], vi, t);
#define EVAL_FLT(name, idx, vi, t)    \
                return evalFloat(name, &parmIndirect[idx], vi, t);

#define SET_STRING(val, meaning, name, idx, vi, t) \
            setString(val, meaning, name, parmIndirect[idx], vi, t)
#define SET_INT(name, idx, vi, t, val) \
	    setInt(name, parmIndirect[idx], vi, t, val);
#define SET_FLT(name, idx, vi, t, val) \
	    setFloat(name, parmIndirect[idx], vi, t, val);

#endif
