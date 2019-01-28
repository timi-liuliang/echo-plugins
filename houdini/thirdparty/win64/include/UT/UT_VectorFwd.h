/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_VectorFwd.h (UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_VECTORFWD_H_INCLUDED__
#define __UT_VECTORFWD_H_INCLUDED__

#include "UT_API.h"
#include <SYS/SYS_Types.h>

template <typename T>
class UT_VectorT;

template <typename T>
class UT_PermutationT;

typedef UT_PermutationT<int>		UT_Permutation;
typedef UT_VectorT<fpreal>		UT_VectorR;
typedef UT_VectorT<fpreal32>		UT_VectorF;
typedef UT_VectorT<fpreal64>		UT_VectorD;
typedef UT_VectorT<fpreal64>		UT_Vector;

#endif // __UT_VECTORFWD_H_INCLUDED__
