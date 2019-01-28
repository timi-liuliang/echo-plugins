/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_Names.h (GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_NAMES_H_INCLUDED__
#define __GT_NAMES_H_INCLUDED__

#include "GT_API.h"
#include <UT/UT_StringHolder.h>

/// Cached const UT_StringHolder's for common attribute names,
/// so that the hash and length only get computed once,
/// and equality can be checked using the pointers first.
namespace GT_Names
{
    /// These names all start with __ to indicate internal attributes
    /// @{
    extern GT_API const UT_StringHolder bboxmax;
    extern GT_API const UT_StringHolder bboxmin;
    extern GT_API const UT_StringHolder cache_name;
    extern GT_API const UT_StringHolder filename;
    extern GT_API const UT_StringHolder instancefile;
    extern GT_API const UT_StringHolder lop_pick_id;
    extern GT_API const UT_StringHolder map_blend;
    extern GT_API const UT_StringHolder map_cd;
    extern GT_API const UT_StringHolder map_disp;
    extern GT_API const UT_StringHolder map_nml;
    extern GT_API const UT_StringHolder nested_prim_id;
    extern GT_API const UT_StringHolder nested_vertex_id;
    extern GT_API const UT_StringHolder object_name;
    extern GT_API const UT_StringHolder point_id;
    extern GT_API const UT_StringHolder primitive_id;
    extern GT_API const UT_StringHolder primitivelist;
    extern GT_API const UT_StringHolder profile_id;
    extern GT_API const UT_StringHolder selection;
    extern GT_API const UT_StringHolder time;
    extern GT_API const UT_StringHolder topology;
    extern GT_API const UT_StringHolder version;
    extern GT_API const UT_StringHolder vertex_id;
    extern GT_API const UT_StringHolder view_lod;
    extern GT_API const UT_StringHolder view_lod_mask;
    /// @}

    /// These names are more specialized and don't begin with __
    /// @{
    extern GT_API const UT_StringHolder boneIndex;
    extern GT_API const UT_StringHolder boneIndex1;
    extern GT_API const UT_StringHolder boneIndex2;
    extern GT_API const UT_StringHolder boneWeight;
    extern GT_API const UT_StringHolder boneWeight1;
    extern GT_API const UT_StringHolder boneWeight2;
    /// @}

} // namespace GT_Names

#endif // __GT_NAMES_H_INCLUDED__
