/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_GEODetail.h ( GEO Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_GEODetail__
#define __GT_GEODetail__

#include "GT_API.h"
#include "GT_Primitive.h"
#include "GT_GEODetailList.h"
#include <GA/GA_Names.h>
#include <GA/GA_Range.h>

class GU_ConstDetailHandle;
class GU_Detail;
class UT_StringRef;

/// @brief Primitive representing entire detail.  Refine this primitive.
///
/// Refining this primitive will iterate over the primitives in the detail.
class GT_API GT_GEODetail : public GT_Primitive
{
public:
	     GT_GEODetail(const GT_GEODetailListHandle &list,
			  const GA_Range *prim_range);
	     GT_GEODetail(const GU_ConstDetailHandle &gdh,
			  const GA_Range *prim_range);
	     GT_GEODetail(const GT_GEODetail &src);
    virtual ~GT_GEODetail();

    virtual const char *className() const { return "GT_GEODetail"; }
    
    /// Compute the bounding box
    virtual void	enlargeBounds(UT_BoundingBox boxes[],
					int nsegments) const;
    virtual void	getVelocityRange(
			    UT_Vector3 &min, UT_Vector3 &max,
			    const UT_StringRef &attribute_name = GA_Names::v
			    ) const;

    /// Return the primitive type
    virtual int		getPrimitiveType() const;

    /// Harden the detail.
    virtual GT_PrimitiveHandle	doHarden() const;

    /// Do a soft-copy
    virtual GT_PrimitiveHandle	doSoftCopy() const
				    { return new GT_GEODetail(*this); }

    /// Parameters
    /// - bool renderpoints (false) @n Generate GT_PrimPointsMesh primitive
    /// - string rendergroup ("") @n Name of primitive group to refine
    /// - bool polygonmesh (true) @n Generate GT_PrimPolygonMesh primitive
    virtual bool	refine(GT_Refine &refine,
					const GT_RefineParms *parms) const;

    virtual int64	getMemoryUsage() const
			    { return sizeof(*this); }

    /// Query number of motion segments
    int			 getMotionSegments() const
			    { return myList->getMotionSegments(); }
    /// Get geometry for a given segment
    const GU_ConstDetailHandle	&getGeometry(int segment=0) const
			    { return myList->getGeometry(segment); }
    const UT_SharedPtr<const GA_Range>    &getRange() const
			    { return myRange; }

    /// @{
    /// Make a primitive for an entire detail
    static GT_PrimitiveHandle	makeDetail(const GU_ConstDetailHandle &gdh,
					const GA_Range *prim_range = NULL);

    static GT_PrimitiveHandle	makeDetail(
					const GT_GEODetailListHandle &geo,
					const GA_Range *prim_range
				);
    /// @}

    /// @{
    /// Make a polygon mesh primitive for all the polygons in the detail
    static GT_PrimitiveHandle	makePolygonMesh(
					const GU_ConstDetailHandle &gdp,
					const GA_Range *prim_range = NULL,
					const GT_RefineParms *rparms = NULL
				);
    static GT_PrimitiveHandle	makePolygonMesh(
					const GT_GEODetailListHandle &geo,
					const GA_Range &prim_range,
					const GT_RefineParms *rparms = NULL
				);
    /// @}

    /// @{
    /// Make a point mesh containing just the points from a detail.  Note that
    /// the attributes can be picked up from detail attributes as well.
    /// Vertex & primitive attributes are ignored.
    static GT_PrimitiveHandle	makePointMesh(
					const GU_ConstDetailHandle &gdp,
					const GA_Range *point_range = NULL
				);
    static GT_PrimitiveHandle	makePointMesh(
					const GT_GEODetailListHandle &geo,
					const GA_Range *point_range = NULL
				);
    /// @}
    virtual bool	save(UT_JSONWriter &w) const;

private:
    GT_GEODetailListHandle	myList;
    UT_SharedPtr<const GA_Range> myRange;
};

#endif
