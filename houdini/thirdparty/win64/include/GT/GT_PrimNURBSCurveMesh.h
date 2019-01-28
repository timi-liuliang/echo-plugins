/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_PrimNURBSCurveMesh.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_PrimNURBSCurveMesh__
#define __GT_PrimNURBSCurveMesh__

#include "GT_API.h"
#include "GT_GEOPrimitive.h"

#include <UT/UT_ValArray.h>
#include <GU/GU_DetailHandle.h>

class GEO_Curve;
class GT_DataArray;
class GT_CountArray;
class GT_AttributeList;
class GT_AttributeMerge;
class GT_PrimCurve;
class GT_GEODetailList;

/// @brief A mesh of polygons
class GT_API GT_PrimNURBSCurveMesh : public GT_Primitive
{
public:
    typedef UT_Array<GT_GEOPrimitive::GEOPrimitivePair>	PrimitiveArrayType;
    /// Default constructor
    GT_PrimNURBSCurveMesh();

    GT_PrimNURBSCurveMesh(const GT_PrimNURBSCurveMesh &src)
	: GT_Primitive(src)
	, myDetailHandle(src.myDetailHandle)
	, myCurves(src.myCurves)
	, myHullCounts(src.myHullCounts)
	, myHullVertex(src.myHullVertex)
	, myUniform(src.myUniform)
	, myDetail(src.myDetail)
	, myWrap(src.myWrap)
	, myCubicBezier(src.myCubicBezier)
    {
    }

    /// Useful constructor
    GT_PrimNURBSCurveMesh(const GU_ConstDetailHandle &gdh,
		const PrimitiveArrayType &curves,
		const GT_AttributeListHandle &uniform,
		const GT_AttributeListHandle &detail,
		bool wrap,
		bool cubic_bezier=false)
	: myDetailHandle(gdh)
	, myCurves(curves)
	, myUniform(uniform)
	, myDetail(detail)
	, myWrap(wrap)
	, myCubicBezier(cubic_bezier)

    {
    }
    
    /// Destructor
    virtual ~GT_PrimNURBSCurveMesh();

    virtual const char *className() const { return "GT_PrimNURBSCurveMesh"; }
    
    /// Initialize the mesh
    void		init(const PrimitiveArrayType &curvePrims,
				const GT_GEODetailList &geo);

    /// Set the vertex attributes for the hull
    void		setHullVertices(const GT_DataArrayHandle &counts,
				const GT_AttributeListHandle &vertex)
			{
			    myHullCounts = counts;
			    myHullVertex = vertex;
			}

    /// @{
    /// Methods defined on GT_Primitive
    virtual int			getPrimitiveType() const;
    virtual void		enlargeBounds(UT_BoundingBox boxes[],
						int nseg) const;
    virtual int			getMotionSegments() const;
    virtual int64		getMemoryUsage() const;

    /// Refine into eithe a GT_PrimCurveMesh or a GT_PrimPolygonMesh depending
    /// on @c getWrap()
    virtual bool		refine(GT_Refine &refiner,
					const GT_RefineParms *parms) const;

    virtual GT_PrimitiveHandle	doSoftCopy() const
				    { return new GT_PrimNURBSCurveMesh(*this); }
    /// @}

    /// Viewport refinement options
    virtual const GT_ViewportRefineOptions	&viewportRefineOptions() const;

    /// Refine to either a GT_PrimPolygonMesh or a GT_PrimCurveMesh depending
    /// on the state of myWrap.
    ///
    /// May return an empty pointer when there are no curves.
    GT_PrimitiveHandle		refineToMesh(const GT_RefineParms *parms) const
				    { return refineToMesh(parms, NULL); }

    /// Refine to the hull mesh
    GT_PrimitiveHandle		refineToHull(const GT_RefineParms *parms) const;

    /// Refine to a GT_PrimCurveMesh or a GT_PrimPolygonMesh dependeing on @c
    /// getWrap().  When refining to GT_PrimPolygonMesh, the @c outline
    /// primitive will be set to a GT_PrimCurveMesh containing the outlines of
    /// all the faces.
    GT_PrimitiveHandle		refineToMesh(const GT_RefineParms *parms,
					GT_PrimitiveHandle *outlines) const;

    /// Fill the break points for all curves.  You must pass in an array of the
    /// number of vertices for each curve.  For example, this could be the
    /// vertex count array of the polygon mesh created by the refine process.
    GT_DataArrayHandle	getBreakPointIndices(const GT_RefineParms *parms,
					GT_CountArray *curvecounts=NULL) const;

    /// @{
    /// Accessor
    GT_Size		 getCurveCount() const	{ return myCurves.entries(); }
    bool		 getWrap() const		{ return myWrap; }
    bool		 isOpenCubicBezier() const	{ return myCubicBezier;}
    const GEO_Curve	*getCurve(exint i) const;
    const GT_AttributeListHandle	&getUniform() const
					    { return myUniform; }
    const GT_AttributeListHandle	&getConstant() const
					    { return myDetail; }
    /// @}

    /// @{
    /// Access attributes
    virtual const GT_AttributeListHandle	&getVertexAttributes() const
						    { return myHullVertex; }
    virtual const GT_AttributeListHandle	&getUniformAttributes() const
						    { return myUniform; }
    virtual const GT_AttributeListHandle	&getDetailAttributes() const
						    { return myDetail; }
    /// @}

    bool	refineToCurveMesh(GT_Refine &refiner,
				    const GT_RefineParms *parms) const;
    
    /// Update the primitive pointers
    virtual bool	updateGeoPrim(const GU_ConstDetailHandle &dtl,
				      const GT_RefineParms &refine);

private:
    GU_ConstDetailHandle	myDetailHandle;
    PrimitiveArrayType		myCurves;
    GT_DataArrayHandle		myHullCounts;
    GT_AttributeListHandle	myHullVertex;
    GT_AttributeListHandle	myUniform;
    GT_AttributeListHandle	myDetail;
    bool			myWrap;
    bool			myCubicBezier; // Cubic Bezier
};

#endif
