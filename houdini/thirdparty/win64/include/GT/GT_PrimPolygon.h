/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_PrimPolygon.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_PrimPolygon__
#define __GT_PrimPolygon__

#include "GT_API.h"
#include "GT_Primitive.h"
#include <UT/UT_Vector3.h>

/// @brief Single closed polygon
///
/// @section GT_Polygon_RParms Refine Parameters: PrimPolygon
/// The parameters inspected for refining a polygon are
/// - @c bool convex @n
///	Whether convexing is required
/// - @c int maxsides @n
///	The maximum sides allowed for a polygon
class GT_API GT_PrimPolygon : public GT_Primitive {
public:
    GT_PrimPolygon() {}
    GT_PrimPolygon(const GT_AttributeListHandle &vertex,
		const GT_AttributeListHandle &uniform)
    {
	init(vertex, uniform);
    }
    GT_PrimPolygon(const GT_PrimPolygon &src)
	: myVertex(src.myVertex),
	  myUniform(src.myUniform),
	  GT_Primitive(src)
    {
    }
    virtual ~GT_PrimPolygon();

    virtual const char *className() const { return "GT_PrimPolygon"; }
    virtual bool	save(UT_JSONWriter &w) const
			{
			    jsonWriter	j(w, "Polygon");
			    return saveAttributeLists(*j);
			}
    
    /// @param vertex
    ///   The length of this array determines the number of vertices.  There is
    ///   one entry per vertex.
    /// @param uniform
    ///   Data which is constant across the polygon
    bool	init(const GT_AttributeListHandle &vertex,
		     const GT_AttributeListHandle &uniform)
		 {
		     myVertex = vertex;
		     myUniform = uniform;
		     return true;
		 }

    GT_Size			getVertexCount() const;

    /// Get the varying data array.  There is one entry for each vertex
    const GT_AttributeListHandle	&getVertex() const
					    { return myVertex; }
    /// Get the unfirom data.  There should be one entry in the array.
    const GT_AttributeListHandle	&getUniform() const
					    { return myUniform; }
    /// @{
    /// Access attributes
    virtual const GT_AttributeListHandle	&getVertexAttributes() const
						    { return myVertex; }
    virtual const GT_AttributeListHandle	&getUniformAttributes() const
						    { return myUniform; }
    /// @}

    /// @{
    /// Methods defined on GT_Primitive
    virtual int			getPrimitiveType() const;
    virtual void		enlargeBounds(UT_BoundingBox boxes[],
					int nsegment) const;
    virtual int			getMotionSegments() const;
    virtual int64		getMemoryUsage() const;
    /// @}

    /// @{
    /// Methods for GEO/GU support.
    virtual fpreal	computePerimeter(int seg) const;
    virtual fpreal	computeSurfaceArea(int seg) const;
    virtual fpreal	computeVolume(const UT_Vector3 &ref_P, int seg) const;
    /// @}

    /// Convex the polygon.  Possible return primitives are
    ///	- NULL (degenerate polygon)
    /// - a handle to this polygon (this polygon is already convex)
    /// - a handle to a polygon mesh (multiple polygons)
    GT_PrimitiveHandle	convex(int max_sides=3) const;

    /// Utility method to compute the normal for an indexed face
    /// @note If dim(P) == 2, the normal will be (0,0,1)
    static void	computeNormal(UT_Vector3F &N, const GT_DataArray &vertexlist,
					const GT_DataArray &P)
			{
			    return computeNormal(N, vertexlist,
				    vertexlist.entries(), 0, P);
			}
    /// Utility method to compute the normal for an indexed face
    /// The face has @c nvtx vertices in the @c vertexlist and the first vertex
    /// is offset by @c vtxoff.
    /// @note If dim(P) == 2, the normal will be (0,0,1)
    static void		computeNormal(UT_Vector3F &N,
					const GT_DataArray &vertexlist,
					GT_Size nvtx, GT_Size vtxoff,
					const GT_DataArray &P);
    static void		computeNormal(UT_Vector3D &N,
					const GT_DataArray &vertexlist,
					GT_Size nvtx, GT_Size vtxoff,
					const GT_DataArray &P);
    /// @{
    /// Utility method to compute the normal for a face
    static void		computeNormal(UT_Vector3F &N, const GT_DataArray &P);
    static void		computeNormal(UT_Vector3D &N, const GT_DataArray &P);
    /// @}

    /// Compute a normal quickly.  The resulting normal will be unit length
    static void	computeFastNormal(UT_Vector3F &N,
					const GT_DataArray &vertexlist,
					GT_Size nvtx, GT_Size vtxoff,
					const GT_DataArray &P);
    static void	computeFastNormal(UT_Vector3D &N,
					const GT_DataArray &vertexlist,
					GT_Size nvtx, GT_Size vtxoff,
					const GT_DataArray &P);
    /// Compute a frame of reference for the polygon
    static void		computeFrameOfReference(UT_Vector3 &X, UT_Vector3 &Y,
					const GT_DataArray &vertexlist,
					GT_Size nvtx, GT_Size vtxoff,
					const GT_DataArray &P);

    /// @{
    /// Compute perimeter of a face or an indexed face
    static fpreal	facePerimeter(const GT_DataArray &P,
				const UT_Matrix4D *xform = NULL);
    static fpreal	facePerimeter(const GT_DataArray &vertexlist,
				GT_Size nvtx, GT_Size vtxoff,
				const GT_DataArray &P,
				const UT_Matrix4D *xform = NULL);
    /// @}
    /// @{
    /// Compute surface area of a face or an indexed face
    static fpreal	faceSurfaceArea(const GT_DataArray &P,
				const UT_Matrix4D *xform = NULL);
    static fpreal	faceSurfaceArea(const GT_DataArray &vertexlist,
				GT_Size nvtx, GT_Size vtxoff,
				const GT_DataArray &P,
				const UT_Matrix4D *xform = NULL);
    /// @}
    /// @{
    /// Compute signed volume of a face or an indexed face (given a reference
    /// point)
    static fpreal	faceVolume(const UT_Vector3 &ref_pt,
				const GT_DataArray &P,
				const UT_Matrix4D *xform = NULL);
    static fpreal	faceVolume(const UT_Vector3 &ref_pt,
				const GT_DataArray &vertexlist,
				GT_Size nvtx, GT_Size vtxoff,
				const GT_DataArray &P,
				const UT_Matrix4D *xform = NULL);
    /// @}

    /// Harden all attributes so there are no dangling dependencies
    virtual GT_PrimitiveHandle	doHarden() const;
    virtual GT_PrimitiveHandle	doSoftCopy() const
				    { return new GT_PrimPolygon(*this); }

    /// The virtual implementation of attribute merging
    virtual GT_PrimitiveHandle	doAttributeMerge(const GT_Primitive &src,
					const UT_StringMMPattern *vertex,
					const UT_StringMMPattern *point,
					const UT_StringMMPattern *uniform,
					const UT_StringMMPattern *detail) const;
private:
    GT_AttributeListHandle	myVertex;
    GT_AttributeListHandle	myUniform;
};

#endif
