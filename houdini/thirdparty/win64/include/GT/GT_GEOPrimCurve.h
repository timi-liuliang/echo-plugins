/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_GEOPrimitive.h ( GEO Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_GEOPrimCurve__
#define __GT_GEOPrimCurve__

#include "GT_API.h"
#include "GT_GEOPrimitive.h"

class GT_API GT_GEOPrimCurve : public GT_GEOPrimitive
{
public:
    GT_GEOPrimCurve(const GU_ConstDetailHandle *dlist,
		    const GEO_Primitive *const* primlist,
		    int nsegments)
	: GT_GEOPrimitive(dlist, primlist, nsegments)
    { }
    GT_GEOPrimCurve(const GT_GEOPrimitive &src)
	: GT_GEOPrimitive(src)
    { }
    virtual ~GT_GEOPrimCurve();

    virtual const char *className() const { return "GT_GEOPrimCurve"; }
    /// @{
    /// Interface from GT_Primitive required by subclass
    virtual bool	refine(GT_Refine &refiner,
				const GT_RefineParms *parms=NULL) const;
    virtual GT_PrimitiveHandle	doSoftCopy() const
				    { return new GT_GEOPrimCurve(*this); }
    /// @}

    /// Refine for the GT_PrimNURBSCurveMesh
    bool	refineForCurveMesh() const;

    /// Viewport refinement options
    virtual const GT_ViewportRefineOptions	&viewportRefineOptions() const;

    /// Test if curve is a cubic Bezier
    bool	isOpenCubicBezier() const;

    /// Test if curve is a cubic Bspline (witha a uniform basis)
    bool	isOpenCubicBspline() const;

    /// @{
    /// Get a list of the break point indices for a refined curve given the
    /// refinement parameters.
    GT_DataArrayHandle	getBreakPointIndices(const GT_DataArrayHandle &P,
				const GT_RefineParms *parms=NULL) const
			{
			    return getBreakPointIndices(P->entries(), parms);
			}
    GT_DataArrayHandle	getBreakPointIndices(exint npoints,
				const GT_RefineParms *parms=NULL) const;
    /// @}
};

#endif
