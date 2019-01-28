/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Utility Library (C++)
 *
 * COMMENTS:
 *      Functions for logging all files that Houdini opens, attempts
 *      to open, or tests for existance.  Can be extended to handle
 *      any Windows SDK calls.
 *
 *      This is useful for debugging--in particular, figuring out
 *      why Houdini won't run or takes a long time to load on a
 *      given system.
 */

#ifndef __UT_NTSysTracer_H__
#define __UT_NTSysTracer_H__

#include "UT_API.h"
#include "UT_Defines.h"
#include "UT_SysClone.h"
#include <stdio.h>


enum UT_SysTraceTargets {
    UT_SYSTRACE_NONE		     = 0,

    UT_SYSTRACE_CREATEFILE	     = 1 << 0,
    UT_SYSTRACE_OPENFILE	     = 1 << 1,
    UT_SYSTRACE_FINDFIRSTFILE        = 1 << 2,
    UT_SYSTRACE_FINDFIRSTFILEEX      = 1 << 3,
    UT_SYSTRACE_GETDISKFREESPACE     = 1 << 4,
    UT_SYSTRACE_GETDISKFREESPACEEX   = 1 << 5,
    UT_SYSTRACE_GETFILEATTRIBUTES    = 1 << 6,
    UT_SYSTRACE_GETFILEATTRIBUTESEX  = 1 << 7,
    UT_SYSTRACE_GETFILESECURITY      = 1 << 8,
    UT_SYSTRACE_GETVOLUMEINFORMATION = 1 << 9,

    UT_SYSTRACE_ALL		     = 0xfffffff
};


class UT_API UT_SysTracer
{
public:
    // Checks for the environment variable HOUDINI_SYSTRACE, and
    // if set, calls installSysTracer().  Output goes by default
    // to stdout (the Houdini console), but can be redirected to
    // a file by setting the environment variable HOUDINI_SYSTRACE_FILENAME.
    static UT_Bool  initTracer( int targets );

    // This should only be called once (when Houdini terminates).
    // (It's registered as a UT_Exit callback, so you don't have to
    // call it explicitly.)
    static void	    terminateTracer();

    // Installs tracing functions for the specified system functions.
    //
    // 'targets' can be a combination of one or more of the flags
    // in UT_SysTraceTargets.
    //
    // Returns UT_TRUE on success, UT_FALSE otherwise. 
    //
    // There is currently no way to uninstall these tracing functions,
    // however something along those lines would also be easy to
    // write (the code in UT_NTHooking.h already does this).
    static UT_Bool installSysTracer( int targets, FILE *toFile );

private:
    static UT_Bool  theTracerInitialized;
};


#endif // __UT_NTSysTracer_H__
