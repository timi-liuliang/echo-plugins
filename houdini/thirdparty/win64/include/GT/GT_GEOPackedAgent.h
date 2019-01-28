/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_GEOPackedAgent.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_GEOPackedAgent__
#define __GT_GEOPackedAgent__

#include "GT_GEOPrimCollect.h"
#include "GT_GEOSupport.h"
#include "GT_Primitive.h"
#include "GT_PrimitiveTypes.h"
#include "GT_AgentSupport.h"

#include <GU/GU_AgentClip.h>

#include <UT/UT_ValArray.h>
#include <UT/UT_IntrusivePtr.h>
#include <UT/UT_NonCopyable.h>
#include <UT/UT_VectorTypes.h>

#include <SYS/SYS_AtomicInt.h>
#include <SYS/SYS_Types.h>


class GU_PrimPacked;
class UT_StringRef;


/// Contains a list of agents.
class GT_API GT_GEOPackedAgent : public GT_Primitive
{
public:
		GT_GEOPackedAgent(const GU_ConstDetailHandle &detail,
				  const char *rigname);
		GT_GEOPackedAgent(const GT_GEOPackedAgent &src);
    virtual    ~GT_GEOPackedAgent();

    /// Main interface

    /// Number of agents in this agent list
    int		getNumAgents() const { return myPrimOffsets.entries(); }

    /// Rig this agent list represents.
    const char *getRigName() const { return myRigName; }

    class RigElement
    {
    public:
	RigElement(int idx, int pidx)
	    : bone_length(0.0), trans_idx(idx), parent_idx(pidx) {}
	~RigElement()
	    {
		for(int i=0; i<children.entries(); i++)
		    delete children(i);
	    }
	
	fpreal bone_length;
	int    trans_idx;
	int    parent_idx;
	UT_Array<RigElement *> children;
    };

    /// Return a list of bones corresponding to all the ones used by deforming
    /// shapes. bone_transform_indices correspond to transforms in the rig and
    /// bone_quats are the directions for the bone (assuming a source
    /// orientation pointing in the -Z axis), and scaled to indicate the bone
    /// length.
    bool	buildRig(UT_IntArray &bone_transform_indices,
			 UT_ValArray<UT_QuaternionF> &bone_quats,
			 UT_BoundingBox &rig_worst_case_bounds) const;

    /// Fetch bounding box of an agent, return false if bbox couldn't be
    /// computed.
    bool	getBoundingBox(int agent_idx, UT_BoundingBox &box) const;

    /// Primitive IDs (GA_Index) of all agents in the agent list.
    GT_DataArrayHandle getPrimIDs() const;
    
    /// Point IDs (GA_Index) of all agents in the agent list.
    GT_DataArrayHandle getPointIDs() const;

    /// Point positions of all agents' points.
    GT_DataArrayHandle getPointPos() const;

    /// Fetch a data array for the attribute named 'attrib_name'.
    GT_DataArrayHandle fetchAttributeData(const UT_StringRef &attrib_name,
					  GT_Owner &attrib_list_owner) const;

    virtual bool	hasDataArray(const UT_StringRef &name,
				     GT_Owner owner_scope[],
				     int num_owners,
				     GT_Storage *storage = NULL,
				     GT_Size	*tuple_size = NULL) const;
					  
    /// local transform for agent at idx. Returns false if no transform present
    bool	       getTransform(int idx, UT_Matrix4D &mat) const;
   
    /// local transform for agent at idx.
    bool	       getTransform(int idx, UT_Matrix4F &mat) const;

    /// computed main transforms for all visible agents
    GT_AgentTransformsHandle  agentTransforms() const
				{ return myAgentTransforms; }

    /// computed rig transforms for all visible agents
    GT_AgentRigTransformsHandle  rigTransforms() const
				{ return myAgentRigTransforms; }

    /// map GA index locations of agent prims to a flat list used by this rig
    GT_AgentIndexMapHandle  indexMap() const
				{ return myAgentIndexMap; }

    /// list of selected agent primitives
    GT_AgentSelectionHandle selection() const { return myAgentSelection; }

    /// list of agent primitive ids.
    GT_AgentIDHandle	    primIDs() const { return myAgentPrimIDs; }

    /// List of agent colors;
    GT_AgentColorsHandle    colors() const { return myAgentColors; }
    
    /// Visibility information for agents.
    GT_AgentVisibilityHandle  visibility() const { return myVisibility; }

    const GT_GEOOffsetList   &pointOffsets() const { return myPointOffsets; }
    const GT_GEOOffsetList   &primOffsets() const { return myPrimOffsets; }
    const GT_GEOOffsetList   &vtxOffsets() const { return myVertexOffsets; }
    
    void		      init();
    
    // GT Interface
    virtual const char *className() const { return "GT_GEOPackedAgent"; }
    virtual int getPrimitiveType() const  { return GT_PRIM_AGENTS; }

    virtual int	getMotionSegments() const { return 1; }
    virtual int64 getMemoryUsage() const { return 0; }
    virtual GT_PrimitiveHandle doSoftCopy() const {return GT_PrimitiveHandle();}
   
    virtual void	enlargeBounds(UT_BoundingBox boxes[],
				      int nsegments) const {}

    void	append(const GU_PrimPacked *prim);

    const GU_PrimPacked	      *getPackedAgent(int agent_idx) const;

    /// compute transform information for those agents that are visible.
    /// 'initial_pass' indicates a topology change where incremental updates
    /// cannot be done.
    void	computeTransforms(bool initial_pass) const;
    
    void	bumpTransformSerial() const { myTransformSerial++; }
    int		getTransformSerial() const { return myTransformSerial; }

    
private:

    GU_ConstDetailHandle        myParentDetail;
    const GU_Detail		*myParentGdp;
    UT_String			myRigName;
    GT_GEOOffsetList		myPrimOffsets;
    GT_GEOOffsetList		myPointOffsets;
    GT_GEOOffsetList		myVertexOffsets;
    UT_BoundingBox		myBoundingBox;
    
    mutable int			myTransformSerial;

    GT_AgentVisibilityHandle	myVisibility;
    GT_AgentTransformsHandle	myAgentTransforms;
    GT_AgentRigTransformsHandle	myAgentRigTransforms;
    GT_AgentIndexMapHandle	myAgentIndexMap;
    GT_AgentSelectionHandle	myAgentSelection;
    GT_AgentIDHandle		myAgentPrimIDs;
    GT_AgentColorsHandle        myAgentColors;
};

#endif
