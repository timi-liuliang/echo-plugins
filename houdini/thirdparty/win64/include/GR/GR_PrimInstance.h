/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GR_PrimInstance.h (GR Library, C++)
 *
 * COMMENTS:
 */
#ifndef __GR_PrimInstance__
#define __GR_PrimInstance__

#include "GR_PrimPacked.h"

class GR_API GR_PrimInstance : public GR_PrimPacked
{
public:
	     GR_PrimInstance(const GR_RenderInfo *info,
			     const char *cache_name,
			     const GT_PrimitiveHandle &instance_prim,
			     const GT_PrimitiveHandle &instancer,
			     const UT_Matrix4DArray &instance_transforms,
			     const GU_ConstDetailHandle &instance_gdp,
			     int nest_level,
			     int prim_index = -1,
			     const UT_BoundingBox *bbox = nullptr);
    virtual ~GR_PrimInstance();

    virtual const char *className() const { return "GR_PrimInstance"; }

    virtual GR_BasePrimType getBasePrimType() const
			{ return GR_BASE_PRIM_INSTANCE; }

    void		setTransforms(const UT_Matrix4DArray &inst_transforms);

    void		setNestLevel(int nest_level)
			    {
				myNestLevel = nest_level;
				if(nest_level == 0)
				    myTopLevelTransforms.entries(0);
			    }
    
    // when nested, the top-level transforms are needed to position the point
    // and primitive decorations.
    void		setTopTransforms(const UT_Matrix4DArray &top_xforms)
			{ myTopLevelTransforms = top_xforms; }

    virtual bool	updateOnViewChange(const GR_DisplayOption &dopts) const
    { return ((getNumInstances() > 1) ||
	      (myInstance?myInstance->updateOnViewChange(dopts):false)); }
    
    virtual void	viewUpdate(RE_Render *r,
				   const GR_ViewUpdateParms &parms);
    
    virtual void	checkForDeferredDraw(const RE_Render *r,
					     GR_RenderMode mode,
					     GR_RenderFlags flags,
					     const GR_DrawParms &dp,
					     const UT_Matrix4D &proj_view,
					     const UT_Matrix4D &object,
					     bool &visible,
					     bool &defer);

     virtual void	update(RE_Render		 *r,
			       const GT_PrimitiveHandle  &primh,
			       const GR_UpdateParms	 &p);

    virtual void	render(RE_Render	      *r,
			       GR_RenderMode	       render_mode,
			       GR_RenderFlags	       flags,
			       GR_DrawParms	       dp);

    virtual bool	inViewFrustum(const UT_Matrix4D &objviewproj);
    
    virtual exint	getNumGLPrimitives(const GR_DisplayOption *);
 
    virtual bool	 getSingleTransform(UT_Matrix4D &mat) const
			    {
				if(getNumInstances() == 1)
				{
				    mat = myInstancedTransforms(0);
				    return true;
				}
				return false;
			    }

    int			getNumTopTransforms() const
			{
			    if(myTopLevelTransforms.entries())
				return myTopLevelTransforms.entries();
			    return myInstancedTransforms.entries();
			}
    int			getNumTransforms() const
			{
			    return myInstancedTransforms.entries();
			}

    void		setInstancer(const GT_PrimitiveHandle &ph);

private:
    virtual int		getNumInstances() const;
    virtual UT_Matrix4D getTransform(int instance_index) const
			    { return myInstancedTransforms(instance_index); }
    
    void		buildInstancedSelection(RE_Render *r,
						const GR_UpdateParms &p);
    void		buildInstancedColor(RE_Render *r,
					    const GR_UpdateParms &p);
    
    void		buildPoints(RE_Render *r,
				    const GR_UpdateParms &p,
				    bool check_lod_centroid);

    void		buildVisibilityInfo(RE_Render *r,
					    const GR_DisplayOption &opts,
					    const GR_FrustumList &frustums);
    void		buildInstanceIndexArrays(RE_Render *r,
						 bool has_partial_vis,
						 const UT_IntArray &vis_insts);

    void		renderInstances(RE_Render	*r, 
					GR_RenderMode	 render_mode,
					GR_RenderFlags	 render_flags,
					GR_DrawParms	 dp);
    void		renderCentroidsOrBoxes(RE_Render	  *r,
					       GR_RenderMode	   render_mode,
					       GR_RenderFlags	   render_flags,
					       GR_DrawParms	   dp);
    GR_SelectMode	buildSelectionFromAttrib(RE_Render *r,
						 RE_Geometry *geo,
						 const GR_UpdateParms &p,
						 const GT_DataArrayHandle &sel);

    // transforms from the GT prim itself. If nested, there will be more
    // instanced transforms than the top-level transforms (it'll be a multiple)
    UT_Matrix4DArray  myInstancedTransforms;

    // Decoration transforms, if nested (otherwise used instanced transforms)
    UT_Matrix4DArray  myTopLevelTransforms;

    // GT transforms convolved with object-level instancing
    UT_Matrix4DArray  myObjectInstancedTransforms;

    int		       myNestLevel;
    int		       myInstancesPerPrim;
    bool	       myHasObjectInstancing;
    bool	       myHasCentroidLOD;
    bool	       myHasBBoxLOD;
    bool	       myHasPointsLOD;
    bool	       myHasFullLOD;
    bool	       myIsInstanceFile;

    bool	       myHasPartialVisibility;
    bool	       myVisArraysDirty;
    UT_IntArray	       myVisibleInstanceIndex;
    UT_IntArray	       myVisibleInstances;
    UT_IntArray	       myDeferInstances;
    UT_IntArray	       myDeferPointInstances;
    UT_Array<uint8>    mySelected;
    UT_Vector3FArray   myInstanceColors;
    
    int64	       myNameDataId;
    int64	       myInstanceId;

    UT_IntArray	       myMaterials;
    UT_IntArray	       myMaterialGroupsVisible;
    UT_BoundingBox     myPrimBBox;
};

#endif
