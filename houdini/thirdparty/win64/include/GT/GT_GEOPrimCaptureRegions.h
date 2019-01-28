/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_GEOPrimCaptureRegions.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_GEOPrimCaptureRegions__
#define __GT_GEOPrimCaptureRegions__

#include "GT_API.h"
#include "GT_PrimQuadricMesh.h"
#include "GT_TransformArray.h"

/// A virtual container used by sphere, circle and tube meshes
class GT_API GT_GEOPrimCaptureRegions : public GT_PrimQuadricMesh
{
public:
    /// Default c-tor
    GT_GEOPrimCaptureRegions()
	: myTaperX()
	, myTaperY()
	, GT_PrimQuadricMesh()
    { }

    GT_GEOPrimCaptureRegions(const GT_GEOPrimCaptureRegions &src)
	: GT_PrimQuadricMesh(src)
	, myTaperX(src.myTaperX)
	, myTaperY(src.myTaperY)
	, myBottomCap(src.myBottomCap)
	, myTopCap(src.myTopCap)
    {
    }

    /// Construct a full list of quadrics
    GT_GEOPrimCaptureRegions(
			const GT_DataArrayHandle &taperx,
			const GT_DataArrayHandle &tapery,
			const GT_DataArrayHandle &bcap,
			const GT_DataArrayHandle &tcap,
			const GT_AttributeListHandle &vertex,
			const GT_AttributeListHandle &detail,
			const GT_TransformArrayHandle &xforms)
	: myTaperX(taperx)
	, myTaperY(tapery)
	, myBottomCap(bcap)
	, myTopCap(tcap)
	, GT_PrimQuadricMesh(vertex, detail, xforms)
    {
    }
    virtual ~GT_GEOPrimCaptureRegions();

    virtual const char *className() const { return "GT_GEOPrimCaptureRegions"; }
    virtual int		getPrimitiveType() const
			    { return GT_PRIM_CAPTURE_REGIONS; }
    virtual GT_PrimitiveHandle	doSoftCopy() const
				{ return new GT_GEOPrimCaptureRegions(*this); }
    virtual GT_PrimitiveHandle	doHarden() const;

    virtual bool	refine(GT_Refine &refiner,
				const GT_RefineParms *parms) const;

    GT_Size			entries() const { return getQuadricCount(); }
    const GT_DataArrayHandle	&taperX() const	{ return myTaperX; }
    const GT_DataArrayHandle	&taperY() const	{ return myTaperY; }
    const GT_DataArrayHandle	&bottomCap() const { return myBottomCap; }
    const GT_DataArrayHandle	&topCap() const { return myTopCap; }

protected:
    virtual void	getQBounds(UT_BoundingBox &box) const;
    virtual GT_PrimitiveHandle	makeQuadric(GT_Size index,
					const GT_AttributeListHandle &h,
					const GT_TransformHandle &x,
					const GT_RefineParms *parms) const;
    /// Refine to a curve mesh.  Each region will be composed of 4 individual
    /// curves.
    GT_PrimitiveHandle	refineToCurves(const GT_RefineParms *parms) const;

    GT_DataArrayHandle	myTaperX;
    GT_DataArrayHandle	myTaperY;
    GT_DataArrayHandle	myBottomCap;
    GT_DataArrayHandle	myTopCap;
};

#endif
