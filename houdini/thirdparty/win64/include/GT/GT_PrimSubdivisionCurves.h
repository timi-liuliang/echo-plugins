/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_PrimSubdivisionCurves.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_PrimSubdivisionCurves__
#define __GT_PrimSubdivisionCurves__

#include "GT_API.h"
#include "GT_PrimCurveMesh.h"

/// A bunch of subdivision curves
class GT_API GT_PrimSubdivisionCurves : public GT_PrimCurveMesh
{
public:
    GT_PrimSubdivisionCurves() : GT_PrimCurveMesh() {}
    
    GT_PrimSubdivisionCurves(const GT_PrimCurveMesh &src)
	: GT_PrimCurveMesh(src) {}

    GT_PrimSubdivisionCurves(const GT_PrimCurve &curve)
	: GT_PrimCurveMesh(curve) {}
  
    GT_PrimSubdivisionCurves(const GT_PrimCurveMesh &src,
			     const GT_AttributeListHandle &vattrib,
			     const GT_AttributeListHandle &uattrib,
			     const GT_AttributeListHandle &dattrib)
	: GT_PrimCurveMesh(src, vattrib, uattrib, dattrib) {}
    
    GT_PrimSubdivisionCurves(const GT_DataArrayHandle &vtx_counts,
			     const GT_AttributeListHandle &vertex,
			     const GT_AttributeListHandle &uniform,
			     const GT_AttributeListHandle &detail,
			     bool wrap)
	: GT_PrimCurveMesh(GT_BASIS_LINEAR, vtx_counts, vertex, uniform, detail,
			   wrap) {}
    
    virtual ~GT_PrimSubdivisionCurves();
    
    virtual int		getPrimitiveType() const
					  { return GT_PRIM_SUBDIVISION_CURVES; }
    virtual const char *className() const { return "GT_PrimCurveMesh"; }

    // Refine to a GT_PrimCurveMesh of tessellated curves
    virtual bool	refine(GT_Refine &refiner,
			       const GT_RefineParms *parms) const;

};

#endif
    
