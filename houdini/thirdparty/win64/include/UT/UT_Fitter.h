/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Utilities for fitting lines/planes (C++)
 *
 * COMMENTS:
 *
 */

#ifndef	__UT_Fitter_H__
#define	__UT_Fitter_H__

#include "UT_API.h"
#include "UT_Assert.h"
#include "UT_VectorTypes.h"
#include <SYS/SYS_Types.h>

template <typename T>
class UT_API UT_FitterT
{
public:
    /// Compute the least squares best fitting line using the perpendicular
    /// distance.
    bool 	fitLineLS(const UT_ValArray< UT_Vector2T<T> > &points,
			  UT_Vector3T<T> &line_eqn);
};

typedef UT_FitterT<fpreal>    UT_FitterR;
typedef UT_FitterT<fpreal32>  UT_FitterF;
typedef UT_FitterT<fpreal64>  UT_FitterD;
typedef UT_FitterT<fpreal64>  UT_Fitter;

#endif
