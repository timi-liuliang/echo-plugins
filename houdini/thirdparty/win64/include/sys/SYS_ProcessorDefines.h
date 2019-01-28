/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * Authored by:
 *	Mark Alexander
 *	Side Effects Software Inc.
 *
 * NAME:	SYS_ProcessorDefines.h (Utility Library, C++)
 *
 * COMMENTS:
 *	Defines needed for the SYSgetProcessor call in SYS_Platform.h.
 */

#ifndef __SYS_ProcessorDefines_h__
#define __SYS_ProcessorDefines_h__

// Processor codes:
enum {
    SYS_PROC_UNKNOWN = -1,
    SYS_PROC_INTEL,	
    SYS_PROC_ALPHA,	
    SYS_PROC_MIPS,	
    SYS_PROC_IA32,		// Generic IA32 
    SYS_PROC_SPARC,	
    SYS_PROC_IA64,
    SYS_PROC_PPC64,		// PowerPC 64
    SYS_PROC_AMD64		// AMD 64
};

// Flags: (bitfield)
enum {
    SYS_PROCFLAG_NONE		= 0x0000,
    SYS_PROCFLAG_MMX 		= 0x0001, // Intel MMX  (PentiumMMX+, Athlon)
    SYS_PROCFLAG_XMMX		= 0x0002, // Intel XMMX (PentiumIII, Althon)
    SYS_PROCFLAG_SSE 		= 0x0004, // Intel SSE  (PentiumIII)
    SYS_PROCFLAG_SSE2		= 0x0008, // Intel SSE2 (Pentium4)
    SYS_PROCFLAG_CONDMOV	= 0x0010, // Conditional mov  (Pentium Pro+,Ath?)
    SYS_PROCFLAG_FPCONDMOV	= 0x0020, // FP Condition mov (Pentium Pro+,Ath?)
    SYS_PROCFLAG_HYPERTHREAD	= 0x0040, // P4 HyperThreading
    SYS_PROCFLAG_3DNOW		= 0x0080, // 3Dnow!
    SYS_PROCFLAG_ALTIVEC	= 0x0100, // PPC Altivec
    SYS_PROCFLAG_SSE3           = 0x0200, // Intel SSE3
    SYS_PROCFLAG_SSSE3          = 0x0400, // Intel SSSE3
    SYS_PROCFLAG_SSE41          = 0x0800, // Intel SSE41
    SYS_PROCFLAG_SSE42          = 0x1000, // Intel SSE42
    SYS_PROCFLAG_AVX            = 0x2000  // AVX
};

#endif
