/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * Authored by:
 *	Mark Alexander
 *	Side Effects Software Inc.
 *
 * NAME:	UT_ProcessorDefines.h (Utility Library, C++)
 *
 * COMMENTS:
 *	Defines needed for the UTgetProcessor call in UT_SysSpecific.h.
 */

#ifndef __UT_ProcessorDefines_h__
#define __UT_ProcessorDefines_h__

#include <SYS/SYS_ProcessorDefines.h>

// Processor codes:
enum {
    UT_PROC_UNKNOWN	= SYS_PROC_UNKNOWN,
    UT_PROC_INTEL	= SYS_PROC_INTEL,
    UT_PROC_ALPHA	= SYS_PROC_ALPHA,
    UT_PROC_MIPS	= SYS_PROC_MIPS,
    UT_PROC_IA32	= SYS_PROC_IA32,
    UT_PROC_SPARC	= SYS_PROC_SPARC,
    UT_PROC_IA64	= SYS_PROC_IA64,
    UT_PROC_PPC64	= SYS_PROC_PPC64,
    UT_PROC_AMD64	= SYS_PROC_AMD64
};

// Flags: (bitfield)
enum {
    UT_PROCFLAG_NONE		= SYS_PROCFLAG_NONE,
    UT_PROCFLAG_MMX		= SYS_PROCFLAG_MMX,
    UT_PROCFLAG_XMMX		= SYS_PROCFLAG_XMMX,
    UT_PROCFLAG_SSE		= SYS_PROCFLAG_SSE,
    UT_PROCFLAG_SSE2		= SYS_PROCFLAG_SSE2,
    UT_PROCFLAG_CONDMOV		= SYS_PROCFLAG_CONDMOV,
    UT_PROCFLAG_FPCONDMOV	= SYS_PROCFLAG_FPCONDMOV,
    UT_PROCFLAG_HYPERTHREAD	= SYS_PROCFLAG_HYPERTHREAD,
    UT_PROCFLAG_3DNOW		= SYS_PROCFLAG_3DNOW
};

#endif
