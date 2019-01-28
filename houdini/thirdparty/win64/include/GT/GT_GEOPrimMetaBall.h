/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_GEOPrimMetaBall.h (GU Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_GEOPrimMetaBall__
#define __GT_GEOPrimMetaBall__

#include "GT_API.h"
#include "GT_Primitive.h"
#include <UT/UT_BoundingBox.h>
#include <GU/GU_DetailHandle.h>

class GA_Range;
class GU_Detail;
class GEO_MetaExpression;

/// Metaball primitive for GT
///
/// Currently only one motion segment is supported.
class GT_API GT_GEOPrimMetaBall : public GT_Primitive
{
public:
     GT_GEOPrimMetaBall();
     GT_GEOPrimMetaBall(const GU_ConstDetailHandle *detail_list,
			int nsegments,
			const GA_Range &prim_range,
			const GT_AttributeListHandle &prims,
			const GT_AttributeListHandle &points);
     GT_GEOPrimMetaBall(const GT_GEOPrimMetaBall &src);
    ~GT_GEOPrimMetaBall();

    virtual const char *className() const { return "GT_GEOPrimMetaBall"; }
    
    /// @{
    /// Members on GT_Primitive
    virtual int		getPrimitiveType() const;
    virtual void	enlargeBounds(UT_BoundingBox boxes[],
				    int nsegments) const;
    virtual bool	refine(GT_Refine &refiner,
				    const GT_RefineParms *parms=NULL) const;
    virtual int		getMotionSegments() const;
    virtual int64	getMemoryUsage() const;
    virtual GT_PrimitiveHandle	doSoftCopy() const
				    { return new GT_GEOPrimMetaBall(*this); }
    /// @}

    /// Viewport refinement options
    virtual const GT_ViewportRefineOptions	&viewportRefineOptions() const;

    int			getNumMetaballs() const;
    UT_Matrix4D		getTransform(int metaball_index) const;
    
    /// Split into disjoint clusters
    bool		cluster(GT_Refine &refiner) const;

    /// Polygonize into an iso-surface
    bool		polygonize(GT_Refine &refiner,
				const GT_RefineParms *parms) const;

    virtual const GT_AttributeListHandle	&getPointAttributes() const
			    { return myPoints; }
    
    virtual const GT_AttributeListHandle	&getUniformAttributes() const
			    { return myUniforms; }
    
    /// Return a data array containing the primitive offsets of the
    /// corresponding GA primitives.
    GT_DataArrayHandle	getPrimitiveIds() const;
    GT_DataArrayHandle  getVertexIds() const;
    
    virtual bool	updateGeoPrim(const GU_ConstDetailHandle &dtl,
				      const GT_RefineParms &refine);
private:
    void	computeBox();
    GEO_MetaExpression	*getExpr() const
			{
			    return myExpression;
			}
    GU_ConstDetailHandle	 myHandle;
    const GU_Detail		*myDetail;
    GEO_MetaExpression		*myExpression;
    UT_BoundingBox		 myBounds;
    GT_AttributeListHandle	 myPoints;
    GT_AttributeListHandle	 myUniforms;
    GA_Range			*myRange;
};

#endif
