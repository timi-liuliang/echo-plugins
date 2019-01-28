/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_PrimPointMesh.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_PrimPointMesh__
#define __GT_PrimPointMesh__

#include "GT_API.h"
#include "GT_Primitive.h"

/// @brief A mesh of unconnected points
class GT_API GT_PrimPointMesh : public GT_Primitive
{
public:
    GT_PrimPointMesh() {}
    GT_PrimPointMesh(const GT_AttributeListHandle &points,
		    const GT_AttributeListHandle &uniform)
    {
	init(points, uniform);
    }
    GT_PrimPointMesh(const GT_PrimPointMesh &src)
	: myPoints(src.myPoints)
	, myUniform(src.myUniform)
	, GT_Primitive(src)
    {
    }
    virtual ~GT_PrimPointMesh();

    virtual const char *className() const { return "GT_PrimPointMesh"; }
    virtual bool	save(UT_JSONWriter &w) const
			{
			    jsonWriter	j(w, "Points");
			    return saveAttributeLists(*j);
			}
    
    /// - @c attribs
    ///   The length of this array determines the number of points.  There is
    ///   one entry per point.
    bool	init(const GT_AttributeListHandle &points,
			const GT_AttributeListHandle &uniform)
		 {
		     myPoints = points;
		     myUniform = uniform;
		     return true;
		 }

    GT_Size				getPointCount() const;
    SYS_DEPRECATED(12.5) GT_Size	getVertexCount() const
					    { return getPointCount(); }

    /// Get the varying data array.  There is one entry for each vertex
    const GT_AttributeListHandle	&getPoints() const
					    { return myPoints; }
    const GT_AttributeListHandle	&getUniform() const
					    { return myUniform; }

    /// @{
    /// Access attributes
    virtual const GT_AttributeListHandle 	&getPointAttributes() const
						    { return myPoints; }
    // Uniforms are mapped to detail attributes for points, as they behave as
    // detail attributes to GL.
    virtual const GT_AttributeListHandle	&getDetailAttributes() const
						    { return myUniform; }
    /// @}

    /// @{
    /// Methods defined on GT_Primitive
    virtual void	enlargeBounds(UT_BoundingBox boxes[],
				int nsegment) const;
    virtual void	enlargeRenderBounds(UT_BoundingBox b[], int n) const;
    virtual int		getPrimitiveType() const;
    virtual int		getMotionSegments() const;
    virtual int64	getMemoryUsage() const;
    /// @}

    /// Harden all attributes so there are no dangling dependencies
    virtual GT_PrimitiveHandle	doHarden() const;
    virtual GT_PrimitiveHandle	doSoftCopy() const
				    { return new GT_PrimPointMesh(*this); }

    /// The virtual implementation of attribute merging
    virtual GT_PrimitiveHandle	doAttributeMerge(const GT_Primitive &src,
					const UT_StringMMPattern *vertex,
					const UT_StringMMPattern *point,
					const UT_StringMMPattern *uniform,
					const UT_StringMMPattern *detail) const;
private:
    GT_AttributeListHandle	myPoints;
    GT_AttributeListHandle	myUniform;
};

#endif
