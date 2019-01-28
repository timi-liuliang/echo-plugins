/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:     GU_CurveFrame.h (GU Library, C++)
 *
 * COMMENTS: Declarations of functions for computing reference frames
 *           for curve vertices.
 */

#include "GU_API.h"
#include <GEO/GEO_Detail.h>
#include <GA/GA_OffsetList.h>
#include <GA/GA_Types.h>
#include <UT/UT_Vector3.h>
#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Types.h>

class GA_PrimitiveGroup;

namespace GU_CurveFrame {

static constexpr int theCurveUVComponent = 0;           // u.  FIXME: Should this be v?
static constexpr int theCrossSectionUVComponent = 1;    // v.  FIXME: Should this be u?

enum TangentType
{
    CIRCULAR,
    SUBD,
    PREV,
    NEXT,
    NONE
};

enum TwistPer
{
    EDGE,
    DISTANCE,
    UV,
    FULLEDGES,
    FULLDISTANCE
};

GU_API bool
getPolyProperties(
    const GEO_Detail *geometry,
    const GA_OffsetListRef &vertices,
    exint &nedges,
    bool &closed,
    bool &unrolled);

SYS_FORCE_INLINE bool
getPolyProperties(
    const GEO_Detail *geometry,
    const GA_Offset primoff,
    exint &nedges,
    bool &closed,
    bool &unrolled)
{
    const GA_OffsetListRef vertices = geometry->getPrimitiveVertexList(primoff);
    return getPolyProperties(geometry, vertices, nedges, closed, unrolled);
}


GU_API void
computeCurveTransforms(
    const GEO_Detail *const geo,
    const GA_PrimitiveGroup *curve_group,
    const GA_RWHandleM4 &transform_attrib,
    const TangentType tangent_type,
    const bool extrapolate_end_tangents,
    bool transform_by_instance_attribs,
    bool stretch_using_backbone,
    float max_stretch_scale,
    float scale,
    UT_Vector3 angles,
    UT_Vector3 dangles,
    TwistPer twist_per,
    UT_Vector3 up_vector,
    bool use_normal_vector_up,
    const GA_ROHandleV3 &curve_uv_attrib);

/// This is instantiated for AXIS == 0 (x), 1 (y), and 2 (z).
template<int AXIS>
GU_API void
extractAxisAttrib(
    GEO_Detail *geo,
    const GA_PrimitiveGroup *curve_group,
    const GA_ROHandleM4 &transform_attrib,
    const GA_RWHandleV3 &axis_attrib);

}
