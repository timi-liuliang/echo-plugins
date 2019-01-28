/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT library math support
 *
 * COMMENTS:
 *	void UT_fastRandomSeed(int seed)
 *	Seed the fast random number generator
 *	
 *	fpreal UT_fastRandom(void)
 *	Really, really fast random number generation - pretty good
 *	distribution of sequential random numbers
 *
 *	Ref.  Numerical Recipes (pp 284)
 *
 *	Currently, only the fast random number generation is included
 *	here, however, in the future, there may be other utilities.
 *
 * CAVEATS:
 *	By including this header file, static data will be allocated.
 */

#ifndef __UT_Math_h__
#define __UT_Math_h__

#include <SYS/SYS_Math.h>

#if defined(_MSC_VER) 
    #pragma message ("Deprecated in Houdini 13.0. Use SYS_Math.h instead.")
#else
    #warning "Deprecated in Houdini 13.0. Use SYS_Math.h instead."
#endif

#define UTwang_inthash		SYSwang_inthash
#define UTwang_inthash64	SYSwang_inthash64
#define UTfastRandom		SYSfastRandom
#define UTrandom		SYSrandom
#define UTfastRandomZero	SYSfastRandomZero
#define UTrandomZero		SYSrandomZero

#define UTatan2(y,x)		SYSatan(y,x)
#define UTacos			SYSacos
#define UTasin			SYSasin
#define UTdegToRad		SYSdegToRad
#define UTradToDeg		SYSradToDeg

#define UTmax			SYSmax
#define UTmin			SYSmin
#define UTabs			SYSabs

#define UTequalZero		SYSequalZero
#define UTisEqual		SYSisEqual
#define UTisGreaterOrEqual	SYSisGreaterOrEqual
#define UTisGreater		SYSisGreater
#define UTisLessOrEqual		SYSisLessOrEqual
#define UTisLess		SYSisLess

#define UTclamp			SYSclamp
#define UTroundDownToMultipleOf	SYSroundDownToMultipleOf
#define UTroundUpToMultipleOf	SYSroundUpToMultipleOf
#define UTfit			SYSfit
#define UTsmooth		SYSsmooth

#define UTisInt			SYSisInt
#define UTisFloat		SYSisFloat

#define UTroundAngle		SYSroundAngle
#define UTisPrime		SYSisPrime
#define UTmakePrime		SYSmakePrime
#define UTnextPrime		SYSnextPrime
#define UTisPow2		SYSisPow2
#define UTmakePow2		SYSmakePow2

#define UTgetSinCosFromSlope	SYSgetSinCosFromSlope

#endif
