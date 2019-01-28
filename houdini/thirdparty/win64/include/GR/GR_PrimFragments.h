/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GR_PrimFragments.h (GR Library, C++)
 *
 * COMMENTS:
 */
#ifndef __GR_PrimFragments__
#define __GR_PrimFragments__

#include "GR_PrimPacked.h"

class GR_API GR_PrimFragments : public GR_PrimPacked
{
public:
	     GR_PrimFragments(const GR_RenderInfo *info,
			      const char *cache_name,
			      const GT_PrimitiveHandle &instance_prim,
			      const GT_PrimitiveHandle &instancer,
			      const GU_ConstDetailHandle &instanced_gdp);
    virtual ~GR_PrimFragments();

    virtual const char *className() const { return "GR_PrimFragments"; }

    virtual void	 update(RE_Render *r,
			       const GT_PrimitiveHandle &primh,
			       const GR_UpdateParms &p);
    
    virtual GR_BasePrimType getBasePrimType() const
			{ return GR_BASE_PRIM_FRAGMENT; }

private:
    virtual bool	 isFragment() const { return true; }
    virtual bool	 getSingleTransform(UT_Matrix4D &mat) const
			    { return false; }
    virtual void	 buildAnchorsGL3(RE_Render *r,
					 const GR_UpdateParms &p,
					 const GT_PrimitiveHandle &xforms);
    
    // Members
    UT_Matrix4DArray myPointPos;
    UT_Matrix4DArray myPrimPos;
};

#endif
