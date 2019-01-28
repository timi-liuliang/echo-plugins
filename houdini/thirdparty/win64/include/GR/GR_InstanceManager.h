/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GR_InstanceManager (C++)
 *
 * COMMENTS:
 *   Global instance cache and global draw pass management
 * 
 */
#ifndef GR_InstanceManager_h
#define GR_InstanceManager_h


#include "GR_API.h"

#include <GT/GT_Primitive.h>
#include <UT/UT_BoundingBox.h>
#include <UT/UT_Map.h>
#include <UT/UT_StringMap.h>
#include <UT/UT_NonCopyable.h>
#include <UT/UT_StringHolder.h>

#include "GR_Defines.h"
#include "GR_CommonDispOption.h"
#include "GR_DrawParms.h"

class RE_Render;

class GR_API GR_InstanceManager : public UT_NonCopyable
{
public:
    static GR_InstanceManager &get() { return *theInstance; }

    // ------------------------------------------------------------------
    // Deferred Instance logging
    //
    // This allows you to register what you will be drawing, have everything
    // be globally processed to ensure a max poly count is respected, and get
    // the results of that
    // The bouding box, matrices, and arrays must not be deleted until the
    // request has been processed and you call getDrawRequestResult().

    bool isActive() const { return myIsActive; }

    bool canQueueRequests() const
			{ return myIsActive && myInteractiveMode; }

    /// For primitives that cannot be culled, this at least notifies the
    /// manager of the GL primitive count so it can be taken into account.
    void queueUncullableDraw(int num_gl_prims);

    // Submit a draw request of a single object with 'nprims' GL primitives
    // within bounding box 'box' and having a full world transform of
    // 'world_mat'. Returns an ID which is used to fetch the results.
    int	queueDrawRequest(const RE_Render *r,
			 int num_gl_prims,
			 const UT_BoundingBox &box,
			 const UT_Matrix4D &world_mat,
			 GR_RenderMode  rmode,
			 GR_RenderFlags rflags,
			 const GR_DrawParms   &dp,
			 bool wireframe,
			 int32 prim_id,
			 bool selected,
			 UT_Vector3F *color = nullptr);

    // queue a list of instances to draw with different transforms.
    // If only some of the draws should be performed, 'result_visible_indices'
    // will indicte which ones.
    int	queueInstanceDrawRequest(const RE_Render *r,
				 int num_gl_prims,
				 const UT_BoundingBox &box,
				 const UT_Matrix4D &obj_transform,
				 const UT_Matrix4DArray &instances,
				 UT_IntArray &result_visible_indices,
				 GR_RenderMode  rmode,
				 GR_RenderFlags rflags,
				 const GR_DrawParms   &dp,
				 bool wireframe,
				 const GT_DataArrayHandle &prim_ids,
				 const UT_Array<uint8> *selected,
				 const UT_Vector3FArray *colors,
				 bool all_selected = false);

    // queue a partial list of instances to draw from a full list.
    // 'visible_indices' must refer to entries in the 'instances' array.
    // If only some of the draws should be performed, 'result_visible_indices'
    // will indicte which ones.
    int	queuePartialInstanceDrawRequest(const RE_Render *r,
					int num_gl_prims,
					const UT_BoundingBox &box,
					const UT_Matrix4D &obj_transform,
					const UT_Matrix4DArray &instances,
					const UT_IntArray &visible_indices,
					UT_IntArray &result_visible_indices,
					GR_RenderMode  rmode,
					GR_RenderFlags rflags,
					const GR_DrawParms   &dp,
					bool wireframe,
					const GT_DataArrayHandle &prim_ids,
					const UT_Array<uint8> *selected,
					const UT_Vector3FArray *colors,
					bool all_selected = false);

    // Queue a single bounding box to be drawn.
    void queueBBoxDraw(const RE_Render *r,
		       const UT_BoundingBox &box, 
		       const UT_Matrix4D &obj_transform,
		       bool wireframe,
		       int32 prim_id,
		       bool selected,
		       UT_Vector3F *color = nullptr);
    
    // Queue a list of bounding boxes to be drawn (one bounding box transormed)
    void queueBBoxDraw(const RE_Render *r,
		       const UT_BoundingBox &box, 
		       const UT_Matrix4D &obj_transform,
		       const UT_Matrix4DArray &instances,
		       bool wireframe,
		       const GT_DataArrayHandle &prim_ids, //size of 'instances'
		       const UT_Array<uint8> *selected,	 //size of 'instances'
		       const UT_Vector3FArray *colors,	 //size of 'instances'
		       bool all_selected = false); // everything selected

    // Queue a partial list of bounding boxes to be drawn (one bbox transformed
    // by some of the transforms, indexed by visible_instances).
    void queuePartialBBoxDraw(
		       const RE_Render *r,
		       const UT_BoundingBox &box, 
		       const UT_Matrix4D &obj_transform,
		       const UT_Matrix4DArray &instances,
		       const UT_IntArray &visible_indices,
		       bool wireframe,
		       const GT_DataArrayHandle &prim_ids, //size of 'instances'
		       const UT_Array<uint8> *selected, //size of 'instances'
		       const UT_Vector3FArray *colors, // size of ^^
		       bool all_selected = false); // everything selected

    enum gr_DrawRequestResult
    {
	NO_DRAW,		// no instances to draw
	PARTIAL_DRAW,		// some instances, flagged ones drawn
	POINTS_ONLY_DRAW,	// draw all as points only
	FULL_DRAW,		// draw all instances as full geometry

	ERROR_NO_SUCH_ID,	// the draw_id passed is invalid.
	ERROR_NOT_PROCESSED	// called too early, before all requests were
    };				// processed.

    // Get the result of the global processing. 'visible instances' from the
    // queued draw requests will only be set if the result is PARTIAL_DRAW.
    // If it is somehow visible, the render mode, flags and draw parms will be
    // set.
    gr_DrawRequestResult getDrawRequestResult(int draw_id,
					      GR_RenderMode  &mode,
					      GR_RenderFlags &flags,
					      GR_DrawParms   &parms) const;
					 
private:
    void	beginPass(const GR_CommonDispOption &opts);
    void	processDrawRequests(RE_Render *r,
				    const GR_CommonDispOption &opts);
    void	processQueuedBBoxes(RE_Render *r,
				    const GR_CommonDispOption &opts);
    void	endPass(RE_Render *r, const GR_CommonDispOption &opts);
    void	updateBBoxes(RE_Render *r);
    void	drawBBoxes(RE_Render *r, const GR_CommonDispOption &opts);
    void	generatePickID(const RE_Render *r, UT_Vector4i &pick_id);
    bool	determineBBoxMode(bool wire_req) const;
	
    
    static void	initialize();

		GR_InstanceManager();
    virtual    ~GR_InstanceManager();
    
    class gr_Instance
    {
    public:
	GT_PrimitiveHandle myInstance;
	UT_BoundingBoxF	   myBBox;
	int64		   myLRU;
	int64		   myVersion;
	int64		   mySize;
	int		   myUsage;
    };

    UT_StringMap<gr_Instance> myCache;
    int64		      myLRU;

public:
    class gr_DrawRequest
    {
    public:
	gr_DrawRequest(int np,
		       const UT_Matrix4D *si,
		       const UT_Matrix4DArray *insts,
		       const UT_BoundingBox   &bb,
		       const UT_IntArray      *vi,
		       UT_IntArray	      *vr,
		       GR_RenderMode	       rm,
		       GR_RenderFlags          rf,
		       GR_DrawParms	       dp)
	    : num_prims(np), single_inst(si), instances(insts), bbox(&bb),
	      visible_instances(vi), visible_result(vr), selected(false),
	      result(ERROR_NOT_PROCESSED), rmode(rm), rflags(rf), dparms(dp),
	      colors(nullptr), color(nullptr), selected_prims(nullptr),
	      wireframe(false) {}

	int			num_prims;
	const UT_Matrix4D      *single_inst;
	const UT_Matrix4DArray *instances;
	const UT_BoundingBox   *bbox;
	const UT_IntArray      *visible_instances;
	UT_IntArray	       *visible_result;
	gr_DrawRequestResult    result;
	GR_RenderMode		rmode;
	GR_RenderFlags		rflags;
	GR_DrawParms		dparms;
	const UT_Array<uint8>  *selected_prims;
	const UT_Vector3FArray *colors;
	GT_DataArrayHandle      prim_ids;
	UT_Vector4i		pick_id;
	UT_Vector3F	       *color;
	bool			selected;
	bool			wireframe;
    };

    class gr_BBoxRequest
    {
    public:
	gr_BBoxRequest(const UT_Matrix4D      *si,
		       const UT_Matrix4DArray *insts,
		       const UT_BoundingBox   &bb,
		       const UT_IntArray      *vi)
	    : single_inst(si),instances(insts),bbox(&bb),visible_instances(vi),
	      offset(0), bucket(0), selected_prims(nullptr),
	      colors(nullptr), selected(false), wireframe(false), color(nullptr)
	    {}
	const UT_Matrix4D      *single_inst;
	const UT_Matrix4DArray *instances;
	const UT_IntArray      *visible_instances;
	GT_DataArrayHandle      prim_ids;
	const UT_Array<uint8>  *selected_prims;
	const UT_Vector3FArray *colors;
	const UT_BoundingBox   *bbox;
	UT_Vector4i		pick_id;
	UT_Vector3F	       *color;
	bool			selected;
	bool			wireframe;

	// the list to write to.
	int			offset;
	int			bucket;
    };

private:
    UT_Array<gr_DrawRequest> myDrawRequests;
    UT_Array<gr_BBoxRequest> myBBoxRequests;
    UT_Matrix4FArray	     myBBoxTransforms;
    UT_Array<int8>	     myBBoxCategories;
    UT_Array<const UT_Vector3F *>  myBBoxColors;
    int64		     myGLPrimCount;
    fpreal		     myLastZ;
    fpreal		     myLastMinZ;
    bool		     myReducedDraws;
    bool		     myIsActive;
    bool		     myInteractiveMode;
    UT_Vector3F		     mySelectionColor;
    GR_CommonDispOption::gr_BBoxMode myBBoxMode;

    static GR_InstanceManager *theInstance;

    // These two are friends for the pass management and initialization.
    friend class GUI_SceneFeel;
    friend class DM_VPortAgent;
};

#endif
