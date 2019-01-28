/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_Interpolate.h (GU Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GU_Interpolate__
#define __GU_Interpolate__

#include "GU_API.h"

#include "GU_DetailHandle.h"
#include <UT/UT_SmallArray.h>

/// Interpolate between multiple pieces of geometry.
class GU_API GU_Interpolate
{
public:
    using DetailList = UT_Array<GU_ConstDetailHandle>;

    /// Linear interpolate between two pieces of geometry.  @c t should be
    /// between 0 and 1.
    ///
    /// This method assumes the topology matches.  Attributes don't have to
    /// match, but the primitive types and topology do.
    static GU_ConstDetailHandle	fastLerp(const GU_ConstDetailHandle &g0,
				         const GU_ConstDetailHandle &g1,
                                         fpreal t);

    /// Linear interpolate between two pieces of geometry.  @c t should be
    /// between 0 and 1.
    ///
    /// This method verifies the topology of the two details matches before
    /// performing interpolation.  If the topology doesn't match, the method
    /// returns the geometry with the largest contribution.
    static GU_ConstDetailHandle	lerp(const GU_ConstDetailHandle &g0,
				     const GU_ConstDetailHandle &g1,
                                     fpreal t);


    /// Perform a weighted sum of the geometry.  @c sum(weights) should be 1.
    ///
    /// This method verifies the topology of the geometry matches before
    /// performing interpolation.  All geometry which matches topology with the
    /// input with the largest weight will be considered.  Any geometry which
    /// doesn't match will have its weight distributed amongst the other
    /// geometry.
    static GU_ConstDetailHandle	interpolate(const DetailList &geo,
					const fpreal *weights);

    /// Perform a weighted sum of the geometry.  @c sum(weights) should be 1.
    ///
    /// This method assumes topology on all the geometry matches.  Attributes
    /// don't have to match, but the primitive types and topology do.
    static GU_ConstDetailHandle	fastInterpolate(
					const DetailList &geo,
					const fpreal *weights);

    /// @{
    /// Check to see whether geometry matches topology and can be interpolated.
    /// This doesn't verify that the attributes for geometries match, only the
    /// topology.
    static bool		matchTopology(const GU_ConstDetailHandle &g0,
					const GU_ConstDetailHandle &g1);
    static bool		matchTopology(const GU_Detail &g0, const GU_Detail &g1);
    /// @}
private:
};

#endif

