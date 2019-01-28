/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * Authored by:
 *	Mark Alexander
 *	Side Effects Software Inc.
 *
 * NAME:	UT_CommonMMXFuncs.h (Utility Library, C++)
 *
 * COMMENTS:
 *	Common MMX functions. They do not check for whether MMX/SSE are
 *      actually present - it is up to the programmer to check beforehand.
 */

#ifndef __UT_CommonMMXFuncs_h__
#define __UT_CommonMMXFuncs_h__

#include "UT_API.h"
#include <SYS/SYS_Types.h>
#include <SYS/SYS_Floor.h>

#ifdef LINUX
#if !defined(IA64) && !defined(AMD64) && !defined(PPC)
#define UT_USE_ASM_ACCEL
#endif
#endif

// You must ensure that the processor supports SSE before calling these
// functions by calling UTgetProcessor() and checking for UT_PROCFLAG_SSE or
// UT_PROCFLAG_3DNOW. This is not done in these functions as it would be
// pretty inefficent.

// The FP -> int versions are much faster because clamping does not have
// any branches (about 10x). The int->FP versions are slightly faster (1.5x).
// Performance tested on Linux RH7, P4.

// Float to byte conversions; (0-1) -> (0-255) and (0,1) -> (b,w)
UT_API void UTconvertFPto8(unsigned char	*dest,
			   const fpreal32	*source,
			   int			 length,
			   int			 useprocext = 0);

UT_API void UTconvertFPto8BW(unsigned char	*dest,
			     const fpreal32	*source,
			     int		 length,
			     unsigned int	 black,
			     unsigned int	 white,
			     int		 useprocext = 0);

// byte to fpreal32 conversions; (0-65535) -> (0-1) and (b,w) -> (0,1)
UT_API void UTconvert8toFP(fpreal32		*dest,
			   const unsigned char	*source,
			   int			 length,
			   bool			 usesse = true);

UT_API void UTconvert8BWtoFP(fpreal32		*dest,
			     const unsigned char*source,
			     int		 length,
			     unsigned int	 black,
			     unsigned int	 white,
			     bool		 usesse = true);

// Float to short conversions; (0-1) -> (0-65535) and (0,1) -> (b,w)
UT_API void UTconvertFPto16(unsigned short	*dest,
			    const fpreal32	*source,
			    int			 length,
			    int			 procext = 0);

UT_API void UTconvertFPto16BW(unsigned short	*dest,
			      const fpreal32	*source,
			      int		 length,
			      unsigned int	 black,
			      unsigned int	 white,
			      bool		 usesse = true);

// short to fpreal32 conversions; (0-65535) -> (0-1) and (b,w) -> (0,1)
UT_API void UTconvert16toFP(fpreal32		*dest,
			    const unsigned short*source,
			    int			 length,
			    bool		 usesse = true);

UT_API void UTconvert16BWtoFP(fpreal32		*dest,
			      const unsigned short *source,
			      int		 length,
			      unsigned int	 black,
			      unsigned int	 white,
			      bool		 usesse = true);

// The next 2 functions convert FP numbers to 8 bit and 16 bit values.
UT_API inline unsigned char UTconvertFPto8(fpreal32 fval)
{
    unsigned char val;
    
#ifdef UT_USE_ASM_ACCEL
    const fpreal32 f255[1] = { 255.0f };
    const fpreal32 *fptr = &fval;
    int ival = 0;

    __asm__ __volatile (
	"movss	(%%edi), %%xmm0		\n"
	"movss	(%%ebx), %%xmm1		\n"
	"mov	$255, %%ecx		\n" // ecx = 255
	"mulss	%%xmm1, %%xmm0		\n"
	"mov	$0,   %%ebx		\n" // ebx = 0
	"cvtss2si	%%xmm0, %%eax		\n"
	"cmp	%%eax,  %%ecx		\n" // compare to 255
	"cmovl	%%ecx,  %%eax		\n"
	"cmp	%%eax, %%ebx		\n" // compare to 0
	"cmovg	%%ebx, %%eax		\n"
	: "=a" (ival)
	: "D" (fptr),
	  "b" (f255) : "%ecx");

    val = (unsigned char) ival;
#else
    int t = (int) SYSfloorIL(fval * 255.0f +0.5F);
    
    if(t < 0)
	val = 0;
    else if(t > 255)
	val = 255;
    else
	val = (unsigned char) t;
#endif    
    return val;
}

UT_API inline unsigned short UTconvertFPto16(fpreal32 fval)
{
    unsigned short val;
    
#ifdef UT_USE_ASM_ACCEL
    const fpreal32 f65535[1] = { 65535.0f };
    const fpreal32 *fptr = &fval;
    int ival = 0;
	
    __asm__ __volatile (
	"movss	(%%edi), %%xmm0		\n"
	"movss	(%%ebx), %%xmm1		\n"
	"mov	$65535, %%ecx		\n" // ecx = 65535
	"mulss	%%xmm1, %%xmm0		\n"
	"mov	$0,   %%ebx		\n" // ebx = 0
	"cvtss2si	%%xmm0, %%eax		\n"
	"cmp	%%eax,  %%ecx		\n" // compare to 65535
	"cmovl	%%ecx,  %%eax		\n"
	"cmp	%%eax, %%ebx		\n" // compare to 0
	"cmovg	%%ebx, %%eax		\n"
	: "=a" (ival)
	: "D" (fptr),
	  "b" (f65535) : "%ecx");
    
    val = (unsigned short) ival;
#else
    int t = (int) SYSfloorIL(fval * 65535.0f +0.5F);
    if(t < 0)
	val = 0;
    else if(t > 65535)
	val = 65535;
    else
	val = (unsigned short) t;
#endif
    return val;
}

#endif
