/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_DeformUtil.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_DeformUtils__
#define __GT_DeformUtils__

#include "GT_API.h"

#include <UT/UT_Vector4.h>

class GT_DataArray;

namespace GT_DeformUtils
{
GT_API void buildBoneIndexAndWeights(const GT_DataArray &bone_capture,
                                     UT_Vector4iArray *bone_index,
                                     UT_Vector4FArray *bone_weight,
                                     int bone_limit,
                                     int *max_bones_per_point = 0);
}

#endif
