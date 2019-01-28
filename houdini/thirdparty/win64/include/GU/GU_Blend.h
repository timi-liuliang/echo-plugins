/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_Blend.h (C++)
 *
 * COMMENTS:	Functions for blending between details.
 *
 */
 
 
 #ifndef __GU_BLEND_H__
#define __GU_BLEND_H__

#include "GU_API.h"
#include <GA/GA_Names.h>
#include <GA/GA_Types.h>

class UT_StringRef;
class GU_Detail;
class GA_Group;

enum class GU_VoxelBlend
{
    None,
    ByGridIndex,
    ByVoxelPos,
    Advected
};

namespace GU_Blend
{
struct PerAttribData
{
    UT_Array<UT_Array<float>> myArrayOfDeltas;
    UT_Array<UT_Array<GA_Offset>> myArrayOfOffsets;
    UT_Array<int64> myArrayOfDataId;
};

struct PerMappingData
{
    UT_Array<GA_Offset> myArrayOfOffsets;
    bool myHasIdAttribMapping = false;
    bool myIdAttribDataIdIsSame = false;
};

class CachedItems
{
public:
    UT_Array<PerAttribData> *myPtArrayOfPerAttribData = NULL;
    UT_Array<PerAttribData> *myPrimArrayOfPerAttribData = NULL;
    UT_Array<PerAttribData> *myVertexArrayOfPerAttribData = NULL;
    UT_Array<PerMappingData> *myPtArrayOfPerMappingData = NULL;
    UT_Array<PerMappingData> *myPrimArrayOfPerMappingData = NULL;
    UT_Array<int64> *myPtTopoDataIds = NULL;
    UT_Array<int64> *myPrimListDataIds = NULL;
    int64 *myPtIdDataId = NULL;
    int64 *myPrimIdDataId = NULL;
};
}

GU_API void GUblend(
    GU_Detail *dest, const GU_Detail *source, fpreal weight,
    const char *pattern = "*", const UT_StringRef &ptidattr = GA_Names::id, const UT_StringRef &primidattr = GA_Names::name,
    bool doslerp = true, bool usevforpinterp = false, float timestep = (1/24.0),
    GU_VoxelBlend dovoxel = GU_VoxelBlend::None, GU_Blend::CachedItems *cached_items = NULL, const char *velname = "vel", float advdt = 1);

/// NOTE: Advected volume interpolation is necessarily linear; in that case, volweight is needed to determine
/// which detail to blend with and by how much.
GU_API void GUblend(
    GU_Detail *dest, const GU_Detail *gdps[], const float *weights, int size, const GA_Group *group,
    const char *pattern = "*", const UT_StringRef &ptidattr = GA_Names::id, const UT_StringRef &primidattr = GA_Names::name,
    bool doslerp = true, GU_VoxelBlend dovoxel = GU_VoxelBlend::None, GU_Blend::CachedItems *cached_items = NULL,
    bool is_differencing = true, bool dosparse = false,
    float timestep = (1/24.0), const char *velname = "vel", float advdt = 1, fpreal volweight = 0);

#endif
