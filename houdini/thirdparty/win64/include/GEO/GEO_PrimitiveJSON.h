/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO_PrimitiveJSON.h ( GEO Library, C++)
 *
 * COMMENTS:	Methods to load/save various GEO level primitives
 */

#ifndef __GEO_PrimitiveJSON__
#define __GEO_PrimitiveJSON__

#include "GEO_API.h"

class GA_PrimitiveJSON;

/// @brief JSON interfaces for standard GEO library primitives
///
/// This class provides readers/writers for standard GEO library primitives.
/// For details, please see GA_PrimitiveJSON.
class GEO_API GEO_PrimitiveJSON
{
public:
    /// @{
    /// JSON Schema for face types
    static const GA_PrimitiveJSON	*getPrimPoly();
    static const GA_PrimitiveJSON	*getPrimRBezCurve();
    static const GA_PrimitiveJSON	*getPrimNURBCurve();
    /// @}

    /// @{
    /// JSON Schema for hull types
    static const GA_PrimitiveJSON	*getPrimMesh();
    static const GA_PrimitiveJSON	*getPrimRBezSurf();
    static const GA_PrimitiveJSON	*getPrimNURBSurf();
    /// @}
 
    /// @{
    /// JSON Schema for pasted surface
    static const GA_PrimitiveJSON	*getPrimPasteDAG();
    /// @}

    /// @{
    /// JSON Schema for quadrics
    static const GA_PrimitiveJSON	*getPrimCircle();
    static const GA_PrimitiveJSON	*getPrimSphere();
    static const GA_PrimitiveJSON	*getPrimTube();
    /// @}

    /// @{
    /// JSON Schema for meta types
    static const GA_PrimitiveJSON	*getPrimMetaBall();
    static const GA_PrimitiveJSON	*getPrimMetaSQuad();
    /// @}

    /// @{
    /// JSON Schema for particle type
    static const GA_PrimitiveJSON	*getPrimParticle();
    /// @}

    /// @{
    static const GA_PrimitiveJSON	*getPrimTriBezier();
    static const GA_PrimitiveJSON	*getTriMesh();
    static const GA_PrimitiveJSON	*getPrimTriFan();
    static const GA_PrimitiveJSON	*getPrimTriStrip();
    static const GA_PrimitiveJSON	*getPrimVolume();
    static const GA_PrimitiveJSON	*getPrimPolySoup();
    /// @}

};

#endif
