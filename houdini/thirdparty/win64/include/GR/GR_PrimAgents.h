/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        GR_PrimAgents ( GR Library, C++)
 *
 * COMMENTS:
 *	Rendering class for agent decorations, bounding boxes
 */
#ifndef GR_PrimAgents_h
#define GR_PrimAgents_h

#include "GR_Primitive.h"

#include <UT/UT_ValArray.h>
#include <UT/UT_BoundingBox.h>
#include <UT/UT_VectorTypes.h>
#include <GT/GT_GEOPackedAgent.h>

class GR_PrimAgents : public GR_Primitive
{
public:
			GR_PrimAgents(const GR_RenderInfo *info,
				      const char	  *cache_name);
    virtual	       ~GR_PrimAgents();


    virtual const char *className() const { return "GR_PrimAgents"; }

    virtual GR_PrimAcceptResult	acceptPrimitive(GT_PrimitiveType t, int,
						const GT_PrimitiveHandle &,
						const GEO_Primitive *);
    
    virtual GR_DispOptChange displayOptionChange(const GR_DisplayOption &opts,
						 bool first_init);
    virtual void	update(RE_Render			*r,
			       const GT_PrimitiveHandle		&primh,
			       const GR_UpdateParms		&p);

    // update the visibility info.
    virtual bool	updateOnViewChange(const GR_DisplayOption &) const
				{ return true; }
    virtual void	viewUpdate(RE_Render *r,
				   const GR_ViewUpdateParms &parms);
     
    virtual void	render(RE_Render			*r,
			       GR_RenderMode			 render_mode,
			       GR_RenderFlags			 render_flags,
			       GR_DrawParms			 dp);
    
    virtual int		renderPick(RE_Render *r,
				   const GR_DisplayOption *opts,
				   unsigned int pickmask,
				   GR_PickStyle pickstyle,
				   bool has_pick_map);
    
    virtual void	renderDecoration(RE_Render		*r,
					 GR_Decoration		 decor,
					 const GR_DecorationParms &parms);
private:

    void	updateVisibility(RE_Render *r,
				 const GR_FrustumList &view_frustums);
    void	buildSkeleton(RE_Render *r, const GT_GEOPackedAgent *agents);
    void	updateBones(RE_Render *r, const GT_GEOPackedAgent *agent,
			    bool geo_changed);
    void	updateAgentColor(RE_Render *r, const GT_GEOPackedAgent *agent,
				 const RE_CacheVersion &version);
    void	updateSelection(RE_Render *r,
				const GT_GEOPackedAgent *agent,
				const GR_UpdateParms &p);

    GT_PrimitiveHandle	     myAgent;
    GT_AgentVisibilityHandle myVisibility;
    UT_BoundingBox	     myWorstCaseBounds;
    UT_Array<UT_BoundingBox> myBounds;
    UT_Vector3FArray	     myPos;
    UT_IntArray		     myPrimID;
    
    UT_Matrix4FArray		myMats;
    UT_IntArray			myBoneTransformIndices;
    UT_ValArray<UT_QuaternionF>	myBoneQuats;
    
    RE_Geometry *myPoints;
    RE_Geometry *myPrimAnchors;
    RE_Geometry *myBBoxes;
    RE_Geometry *myBaseDisc;
    RE_Geometry *myBones;
    
    RE_VertexArray *myBoneTransformsVA;
    int		    myLastTransformSerial;
    fpreal	    myWorstCaseOverestimate;

    unsigned	 myLastHullDisplay : 1,
		 myLastWireOver:1,
		 myLastWire:1,
		 myBoneRigDirty:1,
		 myBonesVisible:1,
		 myObjectSelected:1,
		 myDiscsDirty:1,
		 myBBoxDirty:1;
    RE_VertexArray     *myColorVA;
    RE_VertexArray     *mySelectionVA;
    RE_VertexArray     *myPointSelectionVA;
    GR_SelectMode	mySelectMode;
    GR_AgentWireMode	myAgentWireMode;
    UT_IntArray		myBoneSerialCounts;
    int			myBoneSerial;
    int64		myBoneVisibleHash;
    UT_Matrix4FArray	myBoneMatrixBuffer;
};

#endif
