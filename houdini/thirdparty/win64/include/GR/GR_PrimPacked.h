/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GR_PrimPacked.h (GR Library, C++)
 *
 * COMMENTS:
 */
#ifndef __GR_PrimPacked__
#define __GR_PrimPacked__

#include "GR_Primitive.h"
#include "GR_MaterialGroups.h"

#include <RE/RE_VertexArray.h>

class GR_API GR_PrimPacked : public GR_Primitive
{
public:
	     GR_PrimPacked(const GR_RenderInfo *info,
			   const char *cache_name,
			   const GT_PrimitiveHandle &packed_prim,
			   const GT_PrimitiveHandle &instanced_prim,
			   const GU_ConstDetailHandle &instanced_gdp,
			   int prim_index = -1);

    virtual ~GR_PrimPacked();

    virtual const char *className() const { return "GR_PrimPacked"; }
	     
    virtual GR_BasePrimType	getBasePrimType() const
				{ return GR_BASE_PRIM_PACKED; }
    virtual GT_PrimitiveHandle  getInstancePrim() const
				{ return myInstancedPrim; }

    const GT_PrimitiveHandle &instancePrim() const { return myInstancedPrim; }

    void		setPackedPrim(const GT_PrimitiveHandle &packed_prim);
    void		setInstancedPrim(const GT_PrimitiveHandle &instance,
				 const GU_ConstDetailHandle &instance_gdp)
			    {
				myInstancedPrim = instance;
				myInstancedDetail = instance_gdp;
			    }
    void		setPrimIndex(int pindex)
			    { myPrimIndex = pindex; }
    
    virtual GR_PrimAcceptResult	acceptPrimitive(GT_PrimitiveType t,
						int geo_type,
						const GT_PrimitiveHandle &ph,
						const GEO_Primitive *prim);

    virtual void	update(RE_Render		 *r,
			       const GT_PrimitiveHandle  &primh,
			       const GR_UpdateParms	 &p);

    virtual void	checkForDeferredDraw(const RE_Render *r,
					     GR_RenderMode mode,
					     GR_RenderFlags flags,
					     const GR_DrawParms &dp,
					     const UT_Matrix4D &proj_view,
					     const UT_Matrix4D &object,
					     bool &visible,
					     bool &defer);

    virtual void	render(RE_Render	      *r,
			       GR_RenderMode	       render_mode,
			       GR_RenderFlags	       flags,
			       GR_DrawParms	       dp);

    virtual void	renderDecoration(RE_Render		  *r,
					 GR_Decoration		   decor,
					 const GR_DecorationParms &parms);
    virtual int		renderPick(RE_Render *r,
				   const GR_DisplayOption *opt,
				   unsigned int pick_type,
				   GR_PickStyle pick_style,
				   bool has_pick_map);
    
    bool		checkForTransformChange();
       
    // Delegate these to the child prim.
    virtual GR_DispOptChange displayOptionChange(const GR_DisplayOption &opts,
						 bool first_init);
    virtual bool	 updateOnViewChange(const GR_DisplayOption &) const;
    virtual void	 viewUpdate(RE_Render *r,
 				   const GR_ViewUpdateParms &parms);
    virtual bool	 needsGLStateCheck(const GR_DisplayOption &opts) const;
    virtual bool	 checkGLState(RE_Render *r,const GR_DisplayOption &opt);
    virtual bool	 setup(const GR_DisplayOption *);
    virtual void	 cleanup(RE_Render *r);

    virtual bool	 requiresAlphaPass() const;
    virtual const GA_Primitive	*getGAPrimitive(const GU_Detail *gdp,
						exint primitive_id) const;
    virtual exint	 getGAVertex(const GU_Detail *gdp,
				     exint vertex_id) const;
    virtual RE_Geometry *getDecorationGeometry();
    virtual bool	 renderInAlphaPass(GR_AlphaPass a);
    virtual bool	 inViewFrustum(const UT_Matrix4D &objviewproj);
    virtual bool	 getBoundingBox(UT_BoundingBoxD &bbox) const;
 
    virtual exint	 getNumGLPrimitives(const GR_DisplayOption *);
    virtual void	 retirePrimitive();

    GR_SelectMode	 getSelectionMode(const GU_Detail &gdp,
                 	                  GU_SelectionHandle &selection) const;

    void		 setPickID(GA_Index prim_id, GA_Index point_id);

    void		 setInstanceHookPrim(GR_Primitive *prim);
protected:
    virtual int		 getNumInstances() const { return 1; }
    virtual UT_Matrix4D  getTransform(int ) const
				{ return myInstancePrimTransform; }
    virtual bool	 isFragment() const { return false; }
    virtual bool	 getSingleTransform(UT_Matrix4D &mat) const
			    { mat = myInstancePrimTransform; return true; }

    bool		 updateCacheName(const GT_PrimitiveHandle &iph,
					 const GR_UpdateParms &p,
					 bool fallback);
    void		 updateInstanceGeometry(RE_Render *r,
				      const GT_PrimitiveHandle &container_prim,
						const GR_UpdateParms &p,
						bool build_anchors);
    
    bool		 fetchBoundingBox(UT_BoundingBoxD &bbox) const;
    virtual void	 buildAnchorsGL3(RE_Render *r,
					 const GR_UpdateParms &p,
					 const GT_PrimitiveHandle &xforms);
    void		 buildPickID(const GT_PrimitiveHandle &pid);
    RE_Geometry		*buildPickBBox(RE_Render *r) const;
    void		 buildInstPrimTransform();

    // Given the packed geometry handle and the update reason for the packed
    // primitive, determine the update reason for the packed geometry. 
    GR_UpdateReason	 checkForPackedGeoChanges(const GT_PrimitiveHandle &ph,
						  GR_UpdateReason reason);
    GR_UpdateParms	checkForPackedMaterialAssignment(const GR_UpdateParms &p);
  

    // Packed primitive
    GT_PrimitiveHandle	 myPackedPrim;
    RE_Geometry		*myCenterPoints;
    RE_Geometry		*myCenterPrims;
    RE_CacheVersion	 myCenterPointCacheVersion;

    // Instanced (contained) primitive
    GR_Primitive	*myInstance;
    GU_ConstDetailHandle myInstancedDetail;
    GT_PrimitiveHandle	 myInstancedPrim;
    GT_PrimitiveType	 myInstancedPrimType;
    int64		 myInstancedDetailID;
    int64		 myInstancedDetailCacheCount;
    UT_Vector3F		 myInstancePoint;
    UT_Matrix4D		 myInstancePrimTransform;
    UT_BoundingBox	*myAssignedBBox;
    UT_Matrix4D		 myBBoxTransform;
    RE_VertexArray      *myColorVA;

    GR_SelectMode        mySelectMode;
    GR_SelectMode	 myPointSelectMode;
    int64		 myTopVersion;
    int64		 myMatVersion;
    int64		 myPrimVersion;
    int64		 myPackedPVersion;
    int64		 myAlembicVersion;

    GT_DataArrayHandle	 myPickIDs;
    GT_DataArrayHandle	 myPrimIDs;
    GT_DataArrayHandle	 myPointIDs;
    GT_DataArrayHandle	 myLODMask;
    UT_IntArray	         myBBoxInstances;
    UT_IntArray	         myPointInstances;

    RE_VertexArrayRef    myCenterPointP;
    RE_VertexArrayRef    myCenterPointInstMat;
    RE_VertexArrayRef    myCenterPrimP;
    RE_VertexArrayRef    myCenterPrimInstMat;
    bool		 myPrimSwappedFlag;
    int64		 myPrimUniqueID;
    
    GR_MaterialGroups	 myMatGroups;
    UT_Map<int, UT_IntArray> myMaterialGroups;

    bool		 myIsVisible;
    bool		 myPrimVisible;
    
    // For GR_PrimPacked only
    GA_Index		 myPrimID;

    GA_Index		 myOverridePointID;
    GA_Index		 myOverridePrimID;

    int			 myPrimIndex;
    // --------
};

class GR_API GR_PrimPackedMesh : public GR_PrimPacked
{
public:
    GR_PrimPackedMesh(const GR_RenderInfo *info,
		      const char *cache_name,
		      const GT_PrimitiveHandle &packed_prim,
		      const GT_PrimitiveHandle &instanced_prim,
		      const GU_ConstDetailHandle &instanced_gdp)
    : GR_PrimPacked(info, cache_name,packed_prim,instanced_prim,instanced_gdp)
	{}


    virtual const char *className() const { return "GR_PrimPackedMesh"; }
	     
    virtual GR_BasePrimType	getBasePrimType() const
				{ return GR_BASE_PRIM_PACKED_MESH; }
};


inline GR_Primitive::GR_DispOptChange
GR_PrimPacked::displayOptionChange(const GR_DisplayOption &opts,
				   bool first_init)
{
    if(myInstance)
	return myInstance->checkDisplayOptionChange(opts);
    return DISPLAY_UNCHANGED;
}

inline bool
GR_PrimPacked::updateOnViewChange(const GR_DisplayOption &opt) const
{
    if(myInstance)
	return myInstance->updateOnViewChange(opt);
    return false;
}

inline void	
GR_PrimPacked::viewUpdate(RE_Render *r, const GR_ViewUpdateParms &parms)
{
    if(myInstance)
	myInstance->viewUpdate(r, parms);
}

inline bool	
GR_PrimPacked::needsGLStateCheck(const GR_DisplayOption &opts) const
{
    if(myInstance)
	return myInstance->needsGLStateCheck(opts);
    return false;
}

inline bool	
GR_PrimPacked::checkGLState(RE_Render *r,const GR_DisplayOption &opts)
{
    if(myInstance)
	return myInstance->checkGLState(r,opts);
    return false;
}

inline bool	
GR_PrimPacked::setup(const GR_DisplayOption *d)
{
    if(myInstance)
	return myInstance->setup(d);
    return false;
}

inline void	
GR_PrimPacked::cleanup(RE_Render *r)
{
    if(myInstance)
	myInstance->cleanup(r);
}


inline bool	
GR_PrimPacked::requiresAlphaPass() const
{
    if(myInstance)
	return myInstance->requiresAlphaPass();
    return false;
}

inline const GA_Primitive *
GR_PrimPacked::getGAPrimitive(const GU_Detail *gdp,
			      exint primitive_id) const
{
    if(myInstance)
	return myInstance->getGAPrimitive(gdp, primitive_id);
    return NULL;
}

inline exint	 
GR_PrimPacked::getGAVertex(const GU_Detail *gdp,
			   exint vertex_id) const
{
    if(myInstance)
	return myInstance->getGAVertex(gdp, vertex_id);
    return exint(-1);
}

inline RE_Geometry *
GR_PrimPacked::getDecorationGeometry()
{
    return NULL;
}

inline bool	
GR_PrimPacked::renderInAlphaPass(GR_AlphaPass a)
{
    if(myInstance)
	return myInstance->renderInAlphaPass(a);
    return true;
}

inline bool
GR_PrimPacked::inViewFrustum(const UT_Matrix4D &objviewproj)
{
    if(myInstance)
	return myInstance->inViewFrustum(objviewproj);
    // if no instance, no point in rendering nothing, so cull
    return false;
}

inline bool
GR_PrimPacked::getBoundingBox(UT_BoundingBoxD &bbox) const
{
    return myInstance ? myInstance->getBoundingBox(bbox) : false;
}

inline exint
GR_PrimPacked::getNumGLPrimitives(const GR_DisplayOption *opt)
{
    return myInstance ? myInstance->getNumGLPrimitives(opt) : 0;
}

#endif

