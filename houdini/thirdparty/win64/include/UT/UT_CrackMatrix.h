/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_CrackMatrix.h ( UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_CrackMatrix__
#define __UT_CrackMatrix__

#include "UT_API.h"
#include "UT_Quaternion.h"
#include "UT_XformOrder.h"
#include <SYS/SYS_Types.h>

/// Updates radian euler rotations @c rx, @c ry, and @c rz to have the closest
/// values to @c ox, @c oy, and @c oz radians while still describing the same
/// orientation.
/// Returns false if @c rx/ry/rz are gimbal locked, true otherwise.
/// @{
UT_API bool
UTcrackMatrixSmooth(const UT_XformOrder &order,
	fpreal32 &rx, fpreal32 &ry, fpreal32 &rz,
	fpreal32 ox, fpreal32 oy, fpreal32 oz);
UT_API bool
UTcrackMatrixSmooth(const UT_XformOrder &order,
	fpreal64 &rx, fpreal64 &ry, fpreal64 &rz,
	fpreal64 ox, fpreal64 oy, fpreal64 oz);
UT_API bool
UTcrackMatrixSmooth(const UT_XformOrder &order,
	const UT_Quaternion &q,
	fpreal &rx, fpreal &ry, fpreal &rz,
	fpreal ox, fpreal oy, fpreal oz);
/// @}

#endif

