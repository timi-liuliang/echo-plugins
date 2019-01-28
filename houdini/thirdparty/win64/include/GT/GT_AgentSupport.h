/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_AgentSupport.h ( GT Library, C++)
 *
 * COMMENTS:
 *	Support classes for crowd agents:
 *
 *	GT_AgentIndexMap - mapping of agent primitive GA index to per-rig index
 *	GT_AgentVisibility - list of LOD/visible per agent
 *	GT_AgentTransforms - agent packed primitive transform list
 *	GT_AgentRigTransforms - lists of rig transforms, one per agent
 *	GT_ShapeLODGroup - data common to LOD levels of a shape 
 */
#ifndef _GT_AgentSupport_h_
#define _GT_AgentSupport_h_

#include <UT/UT_IntArray.h>
#include <UT/UT_IntrusivePtr.h>
#include <UT/UT_Matrix4.h>
#include <UT/UT_NonCopyable.h>
#include <UT/UT_SharedPtr.h>
#include <UT/UT_VectorTypes.h>

#include <SYS/SYS_AtomicInt.h>
#include <SYS/SYS_Types.h>

#include "GT_API.h"
#include "GT_DataArray.h"

class RE_VertexArray;
class GU_PrimPacked;

class GT_AgentIndexMap;
class GT_AgentVisibility;
class GT_AgentRigTransforms;
class GT_AgentTransforms;
class GT_AgentSelection;
class GT_AgentID;
class GT_AgentColors;
class GT_ShapeLODGroup;

typedef UT_IntrusivePtr<GT_AgentIndexMap>	GT_AgentIndexMapHandle;
typedef UT_IntrusivePtr<GT_AgentVisibility>	GT_AgentVisibilityHandle;
typedef UT_IntrusivePtr<GT_AgentRigTransforms>	GT_AgentRigTransformsHandle;
typedef UT_IntrusivePtr<GT_AgentTransforms>	GT_AgentTransformsHandle;
typedef UT_IntrusivePtr<GT_AgentSelection>	GT_AgentSelectionHandle;
typedef UT_IntrusivePtr<GT_AgentID>		GT_AgentIDHandle;
typedef UT_IntrusivePtr<GT_AgentColors>		GT_AgentColorsHandle;
typedef UT_IntrusivePtr<GT_ShapeLODGroup>	GT_ShapeLODGroupHandle;

/// Mapping from GA_Index to per-rig agent index 
class GT_API GT_AgentIndexMap
    : public UT_IntrusiveRefCounter<GT_AgentIndexMap>
{
public:
    void	clear()		{ myIndexMap.entries(0); }
    exint	entries() const	{ return myIndexMap.entries(); }

    // returns the index within the rig given the agent prim's GA index.
    int		getAgentIndex(exint ga_index) const
		    { return myIndexMap(ga_index); }

    void	setAgentIndex(int ga_index, int agent_index);

private:
    UT_IntArray		myIndexMap;
};


/// Visibility and LOD class for agents. 
class GT_API GT_AgentVisibility 
    : public UT_IntrusiveRefCounter<GT_AgentVisibility>
{
public:
    void	setEntries(exint entries) { myVisibility.entries(entries); }
    exint	entries() const		  { return myVisibility.entries(); }
    
    void	setVisibility(exint idx, bool visible, fpreal lod = 1.0)
		    { myVisibility(idx) = visible ? lod : 0.0; }
    
    bool	getVisibility(exint idx) const
		    { return myVisibility(idx) != 0.0f; }

    bool	getLOD(exint idx, fpreal32 &lod) const
		    {
			lod = myVisibility(idx);
			return (lod < 0.0f);
		    }

    void	allVisible() { myVisibility.constant(1.0); }
private:
    UT_FloatArray     myVisibility;
};

/// Transform lists for rigs, one per agent
class GT_API GT_AgentRigTransforms
    : public UT_IntrusiveRefCounter<GT_AgentRigTransforms>
{
public:
    typedef UT_SharedPtr<const UT_Array<UT_Matrix4F>> Matrix4ArrayConstPtr;

    void	setEntries(exint entries)
		{
		    myTransforms.entries(entries);
		    mySerial.entries(entries); 
		}

    Matrix4ArrayConstPtr &transformsPtr(exint agent_idx)
			    { return myTransforms(agent_idx); }

    const UT_Array<UT_Matrix4F> &transforms(exint agent_idx) const
			    { return *myTransforms(agent_idx); }

    int		getSerial(exint agent_idx) { return mySerial(agent_idx); }
    void	setSerial(exint agent_idx, int serial)
					 { mySerial(agent_idx) = serial; }
private:
    UT_Array<Matrix4ArrayConstPtr>  myTransforms;
    UT_IntArray			    mySerial;
};


/// Transforms for each entire agent
class GT_API GT_AgentTransforms 
    : public UT_IntrusiveRefCounter<GT_AgentTransforms>
{
public:
    void	setEntries(exint entries) { myTransforms.entries(entries); }

    UT_Matrix4F	      &agentTransform(exint agent_idx)
			    { return myTransforms(agent_idx); }

    const UT_Matrix4F &agentTransform(exint agent_idx) const
			    { return myTransforms(agent_idx); }
private:
    UT_Matrix4FArray	myTransforms;
};

/// Contains a bool array of selected agents.
class GT_API GT_AgentSelection
    : public UT_IntrusiveRefCounter<GT_AgentSelection>
{
public:
		GT_AgentSelection() : mySelectState(0) {}
    
    void	setEntries(exint entries) { mySelection.entries(entries); }
    
    void	clear()	      { mySelection.zero(); mySelectState = 0; }
    void	allSelected() { mySelection.constant(1); mySelectState = 1; }
    void	partiallySelected() { mySelectState = -1; }

    void	setSelected(exint idx, bool selected)
		    { mySelection(idx) = selected?1:0; }
    
    bool	isSelected(exint idx) const
		    { return mySelection(idx)==1; }

    // Usage of these methods depends upon clear, allSelected, or
    // partiallySelection() being called appropriately.
    bool	isFullySelected() const { return (mySelectState == 1); }
    bool	isNothingSelected() const { return (mySelectState == 0); }
    bool	isPartiallySelected() const { return (mySelectState == -1); }
    
private:
    UT_IntArray mySelection;
    int		mySelectState;
};

/// Contains IDs for each agent
class GT_API GT_AgentID
    : public UT_IntrusiveRefCounter<GT_AgentID>
{
public:
    void	setEntries(exint entries) { myIDs.entries(entries); }
    void	setID(exint idx, int id) { myIDs(idx) = id; }
    int		getID(exint idx) const { return myIDs(idx); }

    const UT_IntArray &getIDs() const  { return myIDs; }
    void	fetchIDs(int *id_array) const
		{ memcpy(id_array, myIDs.array(), myIDs.entries()*sizeof(int));}
private:
    UT_IntArray myIDs;
};

/// Contains colors for each agent
class GT_API GT_AgentColors
    : public UT_IntrusiveRefCounter<GT_AgentColors>
{
public:
    void setColors(const GT_DataArrayHandle &colors, GT_Owner owner,
                   exint num_agents)
    {
        if (colors)
        {
            // Note the color is vec3 but padded to vec4 because TBOs
            // only support 1,2 or 4 components with non-32b components.
            // The fill below compensates for this.
            myColors.setSizeNoInit(num_agents * 4);

            if (owner == GT_OWNER_DETAIL)
            {
                // For now, just fill an array with the constant color instead
                // of adding extra optimizations for detail Cd.
                colors->extendedFillArray(myColors.data(), 0, 1, 3, num_agents,
                                          4);
            }
            else
                colors->fillArray(myColors.data(), 0, num_agents, 3, 4);
        }
        else
            myColors.clear();
    }

    bool hasColor() const { return !myColors.isEmpty(); }

    /// Fetch the (4 uint8) color for an agent.
    void fetchColor(exint agent_idx, uint8 *data) const
    {
        memcpy(data, myColors.data() + 4 * agent_idx, 4 * sizeof(uint8));
    }

private:
    UT_Array<uint8> myColors;
};

/// Data common to all levels-of-detail for a series of GT_PrimAgentShapes
class GT_API GT_ShapeLODGroup
    : public UT_IntrusiveRefCounter<GT_ShapeLODGroup>
{
public:
    GT_ShapeLODGroup(const GT_AgentTransformsHandle &th,
		     const GT_AgentRigTransformsHandle &rh,
		     const GT_AgentVisibilityHandle &vis,
		     const GT_AgentIndexMapHandle &map,
		     const GT_AgentSelectionHandle &select,
		     const GT_AgentIDHandle &ids,
                     const GT_AgentColorsHandle &colors,
		     int shape_id);

    ~GT_ShapeLODGroup();

public:
    GT_AgentIndexMapHandle	myAgentIndexMap;
    GT_AgentTransformsHandle	myAgentTransforms;
    GT_AgentRigTransformsHandle	myAgentRigTransforms;
    GT_AgentVisibilityHandle	myVisibility;
    GT_AgentSelectionHandle	myAgentSelection;
    GT_AgentIDHandle		myAgentPrimID;
    GT_AgentColorsHandle        myAgentColors;
    UT_IntArray		       *myDeformMap;
    UT_Matrix4FArray	       *myInvRestTransform;

    UT_Array<RE_VertexArray *> *myTransformVA;
    UT_Array<RE_VertexArray *> *myRigTransformVA;
    UT_Array<RE_VertexArray *> *mySelectionVA;
    UT_Array<RE_VertexArray *> *myPrimIDVA;
    UT_Array<RE_VertexArray *> *myColorVA;
    int				myMaxLODLevel;
    int				myNumBoneTransforms;
    
    int				myTransformSerial;
    int				myRigSerial;
    int				mySelectionSerial;
    bool			myDeforming;
    int				myRigSetupSerial;
    int				myRigSetupDetailID;
    int				myShapeID;

    // this data is temporary, only valid during agent collection/refinement.
    UT_IntArray			myTransforms;
    UT_Array<const GU_PrimPacked *> myAgents;
    UT_IntArray			myBindings;
};


#endif
