/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        GR_UpdateParms.h ( GR Library, C++)
 *
 * COMMENTS:
 *	Parameter class containing all parameters for GR_Primitive::update()
 *	and GR_GeoRender::update().
 *	GR_ViewUpdateParms is a subset of these parameters sent to viewUpdate().
 *	Don't cache these structures. 
 */
#ifndef GR_UpdateParms_h
#define GR_UpdateParms_h

#include "GR_API.h"
#include "GR_Defines.h"
#include "GR_DisplayOption.h"

#include <GU/GU_SelectType.h>
#include <GT/GT_RefineParms.h>
#include <GU/GU_DetailHandle.h>
#include <RE/RE_Material.h>
#include <RE/RE_CachedObject.h>

#include <UT/UT_Matrix4.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_Map.h>

class OP_Node;
class UT_BitArray;

/// set of parameters sent to GR_Primitive::update()
class GR_API GR_UpdateParms
{
public:
    GR_UpdateReason	    reason;	      // reason(s) for update
    OP_Node		   *op_node;	      // node detail belongs to
    GU_ConstDetailHandle    geometry;	      // detail prim is processed for
    const UT_IntArray	   &geo_indices;      // DOP geometry indices selection
    const GT_RefineParms   &refine_parms;     // GT refinement parms 
    const GR_DisplayOption &dopts;	      // current display options
    const RE_MaterialAtlas &material_atlas;   // new material list
    const char		   *view_name;        // full pathname of the viewer
    int			    poly_mesh_limit;  // max #polys in poly mesh
    GU_SelectionHandle	    sel_override;     // override selection
	
    GR_RenderVersion	    gl_version;	      // GL Renderer version (GL1,2,3)
    RE_CacheVersion	    geo_version;      // geometry serial version
    RE_CacheVersion	    select_version;   // selection serial version
    RE_CacheVersion	    instance_version; // instanced geo serial verison
    RE_CacheVersion	    view_version;     // view frustum serial version
    RE_CacheVersion	    inst_select_version; // Instance selection version
    int			    instance_group;   // Instancing group
    const UT_Matrix4DArray *instances;	      // per-point instance transforms
					      //   May be NULL if no instancing
    const UT_BitArray	   *inst_select;      // instance selection
    const GR_FrustumList   &visible_frustums; // frustums for visible viewports
    bool		    object_selected;  // true if selected at object lvl
    GR_DecorationOverride   required_dec;     // decor required above user
					      //   dispopt setting
    unsigned		    force_wire:1,     // must be wireframe
			    force_shaded:1,   // must be smooth shaded
			    force_smooth:1,   // ignore flat shading
			    refine_for_lops:1,// LOPs viewport
			    build_bone_weights:2, // bone weight arrays for (0,1,2,3) -> (0,4,8,12) bones
			    build_selection:1,// allow selection to be built
			    build_prim_selection:1,  // finer control of
			    build_point_selection:1, // selections to build
			    build_vertex_selection:1,// by type; build_selection
			    build_edge_selection:1,  // must be true as well
			    build_bpoint_selection:1,
			    build_ids:1;      // allow prim/vert ids to be built
    
    GR_UpdateParms(GR_UpdateReason	     upd_rsn,
		   OP_Node		    *node,
		   const GU_ConstDetailHandle &dtl,
		   const UT_IntArray	    &geoi,
		   const RE_MaterialAtlas   &material_atlas,
		   UT_Map<UT_StringRef,int> *attribs,
		   const char		    *view_name,
		   int			     max_poly_mesh_size,
		   const GT_RefineParms	    &ref,
		   const GR_DisplayOption   &d,
		   GR_RenderVersion	     gr,
		   bool			     objselect,
		   GR_DecorationOverride     req_dec,
		   RE_CacheVersion	     viewv,
		   const GR_FrustumList     &view_frustums,
		   const UT_Matrix4DArray   *insts,
		   int			     inst_grp,
		   RE_CacheVersion	     instv,
		   const UT_BitArray	    *inst_select,
		   RE_CacheVersion	     ints_select_version);

    GR_UpdateParms(const GR_UpdateParms	    &u);

    GR_UpdateParms(const GR_UpdateParms	    &u,
		   const GU_ConstDetailHandle &dtl);

    GR_UpdateParms(const GR_UpdateParms	    &u,
		   const RE_MaterialAtlas   &atlas);
    ~GR_UpdateParms();

    const UT_Map<UT_StringRef,int> *getConstNeededAttribs() const
			  { return myNeededAttribs; }
    UT_Map<UT_StringRef,int> *getNeededAttribs()
		  {
		      UT_ASSERT(myAttribsCopied);
		      return myAttribsCopied ? myNeededAttribs : nullptr;
		  }
    void	  copyNeededAttribs(bool only_if_shallow_ref);

    // NOTE: The attrib name must exist for the lifetime of the GR_UpdateParms
    //       object. This only keeps a reference to the string data.
    void	  addNeededAttrib(const UT_StringRef &attrib);
    void	  removeNeededAttrib(const UT_StringRef &attrib);

    void	  printNeededAttribs(std::ostream *os = nullptr) const;
private:
    bool		    myAttribsCopied;
    UT_Map<UT_StringRef,int> *myNeededAttribs;// attribs needed for all shaders
};

/// Set of parameters sent to GR_Primitive::viewUpdate().
class GR_ViewUpdateParms
{
public:
    GR_ViewUpdateParms(const GR_DisplayOption &opt,
		       const GU_ConstDetailHandle &geo,
		       RE_CacheVersion	       geov,
		       RE_CacheVersion	       selv,
		       RE_CacheVersion	       viewv,
		       const GR_FrustumList   &frustums)
	: dopts(opt),
	  geometry(geo),
	  geo_version(geov),
	  select_version(selv),
	  view_version(viewv),
	  visible_frustums(frustums) {}
	  

    const GR_DisplayOption &dopts;	      // current display options
    GU_ConstDetailHandle    geometry;	      // detail prim is processed for
    RE_CacheVersion	    geo_version;      // geometry serial version
    RE_CacheVersion	    select_version;   // selection serial version
    RE_CacheVersion	    view_version;     // view frustum serial version
    const GR_FrustumList   &visible_frustums; // frustums of all open viewports
					      // stereo cams have 2/viewport
};

#endif
