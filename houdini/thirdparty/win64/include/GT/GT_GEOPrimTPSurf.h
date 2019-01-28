/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_GEOPrimTPSurf.h ( GT Library, C++ )
 *
 * COMMENTS:
 */

#ifndef __GT_GEOPrimTPSurf__
#define __GT_GEOPrimTPSurf__

#include "GT_API.h"
#include "GT_GEOPrimitive.h"
#include "GT_Handles.h"
#include <GU/GU_TPSurfCache.h>

class GEO_TPSurf;
class GU_TrimPatch;

class GT_API GT_GEOPrimTPSurf : public GT_GEOPrimitive
{
public:
    GT_GEOPrimTPSurf(const GU_ConstDetailHandle *details,
			const GEO_Primitive *const* primlist,
			int nsegments,
			const GT_AttributeListHandle &prim);
    
    GT_GEOPrimTPSurf(const GT_GEOPrimitive &src);
    virtual ~GT_GEOPrimTPSurf();

    virtual const char *className() const { return "GT_GEOPrimTPSurf"; }

    /// Interface from GT_Primitive required by subclass
    virtual bool	refine(GT_Refine &refiner,
				const GT_RefineParms *parms=NULL) const;

    /// Viewport refinement options
    virtual const GT_ViewportRefineOptions	&viewportRefineOptions() const;

    /// Interface from GT_Primitive implemented using methods on GEO_Primitive
    virtual int		getPrimitiveType() const
			    { return GT_GEO_PRIMTPSURF; }
    virtual GT_PrimitiveHandle	doSoftCopy() const
				    { return new GT_GEOPrimTPSurf(*this); }

    GT_PrimitiveHandle	buildIsoparms(const GT_RefineParms *parms,
				      GT_DataArrayHandle &breakpnt_list,
				      int &nu, int &nv, int &density,
				      bool &uwrap, bool &vwrap) const;
    GT_PrimitiveHandle	buildHull() const;

    // Build from cache
    GT_PrimitiveHandle	buildSurface(const GT_RefineParms *parms) const;

    // Build profile curves (may return an empty primitive)
    GT_PrimitiveHandle	buildProfileCurves(const GT_RefineParms *parms) const;

    /// Build a GT_PrimNuPatch
    GT_PrimitiveHandle	buildNuPatch() const;

    virtual const GT_AttributeListHandle	&getUniformAttributes() const
						    { return myUniformList; }
    virtual const GT_AttributeListHandle	&getPointAttributes() const
						    { return myPointList; }
    virtual const GT_AttributeListHandle	&getDetailAttributes() const
						    { return myDetailList; }
    virtual bool	updateGeoPrim(const GU_ConstDetailHandle &dtl,
				      const GT_RefineParms &refine);
private:
    GT_AttributeListHandle myUniformList;
    GT_AttributeListHandle myPointList;
    GT_AttributeListHandle myDetailList;
};

#endif
