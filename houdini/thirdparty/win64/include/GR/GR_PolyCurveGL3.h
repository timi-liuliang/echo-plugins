/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        GR_PolyCurveGL3.h ( GR Library, C++)
 *
 * COMMENTS:
 *	Polygon curve rendering in GL3.
 */
#ifndef GR_PolyCurveGL3_h
#define GR_PolyCurveGL3_h

#include "GR_GeoRender.h"

#include <GT/GT_DataArray.h>

class GU_Detail;
class RE_VertexArray;
class GT_PrimCurveMesh;
class RE_Shader;

// The non-extruded line connectivity (GL_LINES). All others will
// have thickness when in shaded modes (TODO)
#define GR_BASIC_WIRE_IDX	1

class GR_API GR_PolyCurveGL3 : public GR_GeoRender
{
public:
	     GR_PolyCurveGL3(GR_Primitive *prim);
    virtual ~GR_PolyCurveGL3();

    void	setSubdivisionCurve(bool subdiv);

    virtual const char *className() const { return "GR_PolyCurveGL3"; }
    
    virtual void	update(RE_Render *r,
			       const GT_PrimitiveHandle &primh,
			       const GR_UpdateParms	&parms,
			       const char		*cache_name);

    virtual void	draw(RE_Render *r,
			     GR_RenderMode mode,
			     GR_RenderFlags flags,
			     GR_DrawParms dp);
    
    virtual bool	supportsInstances() const { return true; }
    virtual void	geometryUpdated(RE_Render *r);

    static GT_PrimitiveHandle	getCurveMesh(const GT_PrimitiveHandle &primh,
					     const GR_UpdateParms &parms);

    void			setSplineParms(const void *spline,
					       int nu, int nv, 
					       bool uwrap, bool vwrap,
					       int density)
				{   mySpline = spline;
				    myNumU = nu;
				    myNumV = nv;
				    myDensity = density;
				    myWrapU = uwrap;
				    myWrapV = vwrap;
				}

    virtual exint      getNumGLPrimitives() const;

    // fetch LOD parameters for SudD curves based on display options
    static void		getLODParms(const GR_DisplayOption *opt,
				    UT_Vector2F &lod_parms);
private:
    
    void		buildConnectivity(RE_Render *r,
					  const GT_PrimCurveMesh *cmesh,
					  const char *cache_name,
					  RE_CacheVersion geo_version,
					  const RE_MaterialAtlas &mats);
    
    void		addCurveToElement(RE_Render *r,
					  const GT_PrimCurveMesh *cmesh,
					  int curve_indexi,
					  RE_ElementArray *element,
					  const int curve_vertex_cutoff,
					  UT_IntArray &temp);

    inline int		getNumMaterials() const
				{ return myMaterials.entries(); }

    void		buildPrimInfo(RE_Render *r,
				      const GT_PrimCurveMesh *cmesh,
				      const GR_UpdateParms &parms,
				      RE_CacheVersion geo_version,
				      RE_Geometry *geo);

    void		buildLineConnectivity(RE_Render *r,
					      const GT_PrimCurveMesh *cmesh,
					      const char *cache_name,
					      RE_CacheVersion version);
    
    virtual int		setupSelection(RE_Render *r,
				       const GR_DisplayOption *opt,
				       GR_RenderFlags flags) const;

    virtual GR_SelectMode addToBreakpointSelection(RE_Render *r,
					   RE_VertexArray *select,
					   int bpnt_len,
					   const GA_BreakpointGroup *bpnt_sel,
					   bool &init);

    void		drawHairShadow(RE_Render *r,
				       const GR_DisplayOption *opt,
				       int num_materials,
				       int instance_group,
				       bool subdiv);
    
    RE_VertexArray	*myCd;
    RE_VertexArray	*myAlpha;
    bool		 myHasDetailCd;
    bool		 myHasDetailAlpha;
    bool		 myHasPrimAttribs;
    bool		 myHasWidthAttrib;
    bool		 myObjectSelectedFlag;
    RE_CacheVersion	 myLastTopVersion;
    GT_PrimitiveHandle	 myRefinedBezier;
    bool		 myUseFurShaders;
    RE_Texture		*myRandomTex;
    RE_Texture		*myRandomMaskTex;
    int			 myRandomMaskTexSamples;

    UT_IntArray		 myMaterials;
    
    // For splines, such as bezier or NURBS. Defines the isoparm detail for
    // breakpoint selections.
    const void		 *mySpline;	 
    int			  myNumU, myNumV;
    bool		  myWrapU, myWrapV;
    int			  myDensity;

    // subd curves
    bool		 myIsSubdivisionCurve;
    
    int			 myHairShadowPassID;
};
#endif
