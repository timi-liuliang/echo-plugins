/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_VoxelCompress.h ( UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_VoxelCompress__
#define __UT_VoxelCompress__

#include "UT_API.h"
#include <SYS/SYS_Types.h>
#include "UT_ValArray.h"
#include "UT_VoxelArray.h"

/// We provide these functions so we can create file statics
/// for the corresponding lists.
UT_API UT_ValArray<UT_VoxelTileCompress<fpreal16> *> &UTvoxelTileGetCompressionEngines(fpreal16 *dummy);
UT_API UT_ValArray<UT_VoxelTileCompress<fpreal32> *> &UTvoxelTileGetCompressionEngines(fpreal32 *dummy);
UT_API UT_ValArray<UT_VoxelTileCompress<fpreal64> *> &UTvoxelTileGetCompressionEngines(fpreal64 *dummy);
UT_API UT_ValArray<UT_VoxelTileCompress<uint8> *> &UTvoxelTileGetCompressionEngines(uint8 *dummy);
UT_API UT_ValArray<UT_VoxelTileCompress<int8> *> &UTvoxelTileGetCompressionEngines(int8 *dummy);
UT_API UT_ValArray<UT_VoxelTileCompress<int16> *> &UTvoxelTileGetCompressionEngines(int16 *dummy);
UT_API UT_ValArray<UT_VoxelTileCompress<int32> *> &UTvoxelTileGetCompressionEngines(int32 *dummy);
UT_API UT_ValArray<UT_VoxelTileCompress<int64> *> &UTvoxelTileGetCompressionEngines(int64 *dummy);
UT_API UT_ValArray<UT_VoxelTileCompress<UT_Vector2> *> &UTvoxelTileGetCompressionEngines(UT_Vector2 *dummy);
UT_API UT_ValArray<UT_VoxelTileCompress<UT_Vector3> *> &UTvoxelTileGetCompressionEngines(UT_Vector3 *dummy);
UT_API UT_ValArray<UT_VoxelTileCompress<UT_Vector4> *> &UTvoxelTileGetCompressionEngines(UT_Vector4 *dummy);

UT_API void UT_RegisterVoxelCompressionMethods();

UT_API extern fpreal32 *theDitherMatrix;

inline fpreal32 UTgetVoxelDither(int x, int y, int z)
{
    const int		DITHERMASK = 15;
    const int		DITHERSIZE = 16;

    x &= DITHERMASK;
    y &= DITHERMASK;
    z &= DITHERMASK;

    return theDitherMatrix[ ((z*DITHERSIZE) + y)*DITHERSIZE + x ];
}


#endif

