/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GR_PrimAgentShape.h (GR Library, C++)
 *
 * COMMENTS:
 */
#ifndef GR_PrimAgentShape_h
#define GR_PrimAgentShape_h

#include "GR_PrimPacked.h"
#include <GT/GT_PrimAgentShape.h>

class GR_API GR_PrimAgentShape : public GR_PrimPacked
{
public:
	     GR_PrimAgentShape(const GR_RenderInfo *info,
			       const char *cache_name,
			       const GT_PrimitiveHandle &pack_prim,
			       const GT_PrimitiveHandle &shape_prim);
    virtual ~GR_PrimAgentShape();

    virtual const char *className() const { return "GR_PrimAgentShape"; }
	     
    virtual GR_BasePrimType	getBasePrimType() const
				    { return GR_BASE_PRIM_AGENT_SHAPE; }

    virtual GR_PrimAcceptResult	acceptPrimitive(GT_PrimitiveType t,
						int geo_type,
						const GT_PrimitiveHandle &ph,
						const GEO_Primitive *prim);
    
    virtual GR_DispOptChange	displayOptionChange(const GR_DisplayOption &opt,
						    bool first_init);
    
    virtual void	update(RE_Render		 *r,
			       const GT_PrimitiveHandle  &primh,
			       const GR_UpdateParms	 &p);

    virtual void	render(RE_Render	      *r,
			       GR_RenderMode	       render_mode,
			       GR_RenderFlags	       flags,
			       GR_DrawParms	       dp);

    virtual int		renderPick(RE_Render *r,
				   const GR_DisplayOption *opt,
				   unsigned int pick_type,
				   GR_PickStyle pick_style,
				   bool has_pick_map);

    virtual bool	updateOnViewChange(const GR_DisplayOption &opt) const
			    { return opt.shadingMode() >= GR_FLAT_SHADED; }
    virtual void	viewUpdate(RE_Render *r,
				   const GR_ViewUpdateParms &parms);

    virtual bool	inViewFrustum(const UT_Matrix4D &objviewproj)
			    { return myVisibilityFlag;  }
    
    virtual bool	requiresAlphaPass() const { return false; }
    virtual bool	getSingleTransform(UT_Matrix4D &mat) const
			    { return false; }

private:
    void		 processVisibility(RE_Render *r,
					const GR_DisplayOption &dopts);
    void		 updateTransforms(RE_Render *r,
					const GU_ConstDetailHandle &geo,
					bool geo_changed);
    void		 updateInstanceTransforms(RE_Render *r,
					const GU_ConstDetailHandle &geo,
					bool geo_changed);
    void		 updateRigTransforms(RE_Render *r,
					const GU_ConstDetailHandle &geo,
					bool geo_changed);
    void		 processSelection(RE_Render *r,
					  const RE_CacheVersion &select_ver);
    void		 freeVertexArrays();
    void		 updatePrimID(RE_Render *r, bool geo_changed);
    void		 updateColor(RE_Render *r, bool geo_changed);

    int			 myLODLevel;
    int			 myTotalAgents;
    int64		 myLastInstVersion;
    int			 myTransformSerial;
    int			 mySelectionSerial;
    int			 myRigSerial;
    GT_ShapeLODGroupHandle myShapeGroup;
    UT_IntArray		 myTransformIndexMap;
    GR_UpdateReason	 myDeferredReason;
    bool		 myDeformingShape;
    bool		 myVisibilityFlag;
    bool		 myLastWire;
    bool		 myLastLODEnable;
    fpreal		 myLastLODBias;
    int			 myLastLODBase;

    // Only the primary LOD holds any of these.
    UT_Array<RE_VertexArray *>  myTransformVA;
    UT_Array<RE_VertexArray *>  myRigTransformVA;
    UT_Array<RE_VertexArray *>  mySelectionVA;
    UT_Array<RE_VertexArray *>  myPrimIDVA;
    UT_Array<RE_VertexArray *>  myColorVA;
};



#endif
