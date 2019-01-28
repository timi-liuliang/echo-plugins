/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP_Compiled.h ( SOP Library, C++)
 *
 * COMMENTS:
 *	Compiles chains of nodes into a single object
 *	that can be evaluated.
 */

#ifndef __SOP_Compiled_h__
#define __SOP_Compiled_h__

#include "SOP_API.h"
#include "SOP_Node.h"
#include "SOP_NodeVerb.h"
#include <DEP/DEP_TimedMicroNode.h>
#include <CE/CE_Error.h>

class SOP_API SOP_Compiled
{
public:
	     SOP_Compiled();
    virtual ~SOP_Compiled();

    enum UnloadMode
    {
	UNLOAD_NEVER,
	UNLOAD_FLAG,
	UNLOAD_ALWAYS
    };

    class DelayedCook
    {
    public:
	DelayedCook(SOP_Node *cooknode, const OP_Context &context)
	    : myCookNode(cooknode)
	    , myContext(context)
	{
	}

	SOP_Node		*myCookNode;
	OP_Context		 myContext;
    };

    class TaskState
    {
    public:
	TaskState()
	{
	    myDirty = true;
	    myCooking = false;
	    myUseCount = 0;
	    myLocked = false;
	    myCECache = false;
	    myTimeDependent = false;
	    myNodeCache = 0;
	    myNodeParms = 0;
	    myNodeParmsOrig = 0;
	    myParmsDirty = true;
	    myDependsOnExprInputs = true;
	    myDelayedCook = 0;
	}
	~TaskState()
	{
	    delete myNodeCache;
	    delete myNodeParms;
	    delete myNodeParmsOrig;
	    delete myDelayedCook;
	}

	GU_DetailHandle	 myGdh;
	bool		 myDirty;
	bool		 myCECache;
	bool		 myTimeDependent;
	bool		 myCooking;		// recursion flag.
	int		 myUseCount;
	bool		 myLocked;
	bool		 myParmsDirty;	// Whether node parms current
	bool		 myDependsOnExprInputs;	// If expr inputs used.
	SOP_NodeCache	*myNodeCache;
	SOP_NodeParms	*myNodeParms;
	SOP_NodeParms	*myNodeParmsOrig;	// Pre-vexpression cache
	DelayedCook	*myDelayedCook;
    };

    typedef	UT_Array<TaskState>	TaskStateArray;

    class ExecuteParms
    {
	bool		 executeInputs(TaskStateArray &state, const OP_Context &context, UT_ErrorManager &errors, DEP_MicroNode *depnode);

    public:
	ExecuteParms(TaskStateArray &state, UnloadMode unloadmode, 
		     const OP_Context &context,
		     UT_ErrorManager &errors, DEP_MicroNode *depnode,
		     SOP_NodeVerb::ForbiddenNodeMap &forbiddennodes,
		     UT_Array<GU_DetailHandle> *recyclingbin)
	: myState(state)
	, myUnloadMode(unloadmode)
	, myContext(context)
	, myErrors(errors)
	, myDepNode(depnode)
	, myForbiddenNodes(forbiddennodes)
	, myRecyclingBin(recyclingbin)
	{ }

	~ExecuteParms()
	{}

	TaskStateArray	&state() const { return myState; }
	UnloadMode	 unloadMode() const { return myUnloadMode; }
	const OP_Context &context() const { return myContext; }
	UT_ErrorManager &errors() const { return myErrors; }
	DEP_MicroNode  *depnode() const { return myDepNode; }

	SOP_NodeVerb::ForbiddenNodeMap	&forbiddenNodes() const { return myForbiddenNodes; }

	bool		hasRecycleableGdp() const 
	{ 
	    if (!myRecyclingBin)
		return false;
	    return myRecyclingBin->entries() > 0; 
	}
	GU_DetailHandle	allocGdp(bool clear = true) const
	{
	    GU_DetailHandle	result;
	    if (!hasRecycleableGdp())
	    {
		result.allocateAndSet(SOP_Node::prepNewDetail(new GU_Detail()));
	    }
	    else
	    {
		result = myRecyclingBin->last();
		myRecyclingBin->removeLast();
		if (clear)
		    result.gdpNC()->stashAll();
		myRecycledGdpId.append(result.gdp()->getUniqueId());
		myRecycledGdpShareCount.append(result.getRefCount());
	    }
	    return result;
	}

	bool		isGdpIdRecycled(const GU_DetailHandle &gdh) const
	{
	    UT_ASSERT(gdh.isValid());
	    exint		idx = myRecycledGdpId.find(gdh.gdp()->getUniqueId());
	    if (idx < 0)
		return false;
	    // Verify we haven't had any extra copies made of us during
	    // task execution.  We compare against our stashed use
	    // count when first built.
	    if (gdh.getRefCount() != myRecycledGdpShareCount(idx))
	    {
		return false;
	    }
	    return true;
	}

    protected:
	TaskStateArray		&myState;
	UnloadMode		 myUnloadMode;
	const OP_Context	&myContext;
	UT_ErrorManager		&myErrors;
	DEP_MicroNode		*myDepNode;
	SOP_NodeVerb::ForbiddenNodeMap &myForbiddenNodes;
	UT_Array<GU_DetailHandle> *myRecyclingBin;
	mutable UT_ExintArray	 myRecycledGdpId;
	mutable UT_ExintArray	 myRecycledGdpShareCount;
    };

    class Task
    {
    public:
	Task(SOP_Node *node, int stateid);
	virtual ~Task();

	void		setInput(int inputidx, Task *input);
	void		setExprInput(int inputidx, Task *input);
	void		appendOutput(Task *output);

	// Verify we are still wired up the way we used to be.  Only the
	// node id is valid at this point.
	virtual bool	 validate();
	virtual void	 addNodes(SOP_NodeVerb::ForbiddenNodeMap &nodeset);

	// Unloads any cached gdps, but not necessarily things like
	// VEX bindings.
	virtual void	 forceUnload(TaskStateArray &state);

	// Resets the time dependencies of its own node's parm lists,
	// and any child states node's parm lists.
	virtual void	 clearNodeTimeDependencies();

	SOP_Node		*getNode() const { return myNode; }
	int			 getNodeId() const { return myNodeId; }
	GU_DetailHandle	 getDetailHandle(const TaskStateArray &state) { return state(myStateId).myGdh; }
	bool		 hasCECache(const TaskStateArray &state) const { return state(myStateId).myCECache; }
	bool		 getTimeDependent(const TaskStateArray &state) const { return state(myStateId).myTimeDependent; }
	void		 setTimeDependent(TaskStateArray &state, bool timedep) { state(myStateId).myTimeDependent = timedep; }
	GU_DetailHandle	 unloadDetailHandle(TaskStateArray &state, bool flushce, UnloadMode unloadmode, UT_ErrorManager &errors);

	void		 setDetailHandle(TaskStateArray &state, GU_DetailHandle gdh, bool cecache) 
	{ 
	    markClean(state);
	    state(myStateId).myUseCount = 0;
	    state(myStateId).myGdh = gdh; 
	    state(myStateId).myCECache = cecache;
	}

	void		 setDelayedCook(TaskStateArray &state, SOP_Node *cooknode, const OP_Context &context)
	{
	    delete state(myStateId).myDelayedCook;
	    state(myStateId).myDelayedCook = new DelayedCook(cooknode, context);
	}

	void		 lock(TaskStateArray &state)
	{
	    state(myStateId).myLocked = true;
	}
	bool		 isLocked(TaskStateArray &state) const
	{
	    return state(myStateId).myLocked;
	}

	bool		 isDirty(const TaskStateArray &state) const { return state(myStateId).myDirty; }
	void		 dirty(TaskStateArray &state) { state(myStateId).myDirty = true; }
	void		 markClean(TaskStateArray &state) { state(myStateId).myDirty = false; }

	bool		 isParmsDirty(const TaskStateArray &state) const { return state(myStateId).myParmsDirty; }
	void		 dirtyParms(TaskStateArray &state) { state(myStateId).myParmsDirty = true; }
	void		 markParmsClean(TaskStateArray &state) { state(myStateId).myParmsDirty = false; }
	bool		 dependsOnExprInputs(const TaskStateArray &state) const { return state(myStateId).myDependsOnExprInputs; }
	void		 clearDependsOnExprInputs(TaskStateArray &state) { state(myStateId).myDependsOnExprInputs = false; }
	void		 setDependsOnExprInputs(TaskStateArray &state) { state(myStateId).myDependsOnExprInputs = true; }

	/// Dirties ourself and all children until we hit something
	/// already dirty.
	void		 dirtyChildren(TaskStateArray &state);

	/// Computes myself, and chains to any inputs, provided not
	/// dirty.
	bool	 	 execute(const ExecuteParms &parms);

	virtual bool	 doExecute(const ExecuteParms &parms) = 0;

	bool		 executeInputs(const ExecuteParms &parms);
	bool		 executeExprInputs(const ExecuteParms &parms);

	void	 	 dump(std::ostream &os) const;
	UT_StringHolder	 buildName() const;

    protected:
	virtual void	 dumpSubclass(std::ostream &os) const {}
	SOP_Node		*myNode;
	int			 myNodeId;
	int 			 myNodeUnload;
	int 		 	 myNodeBypassed;
	int 		 	 myNodeLocked;
	UT_IntArray		 myNodeInputIds;
	int			 myStateId;
	UT_Array<Task *>	 myInputs;
	UT_Array<Task *>	 myExprInputs;
	UT_Array<Task *>	 myOutputs;
    };

    /// Determine if the compiled task list still matches what
    /// the SOP network has.
    bool		isTopologyDirty(const OP_Context &context);

    bool		isTopologyTimeDependent() const;

    void		dump(std::ostream &os) const;

    /// Compile a node block, only rebuilding if Topo dependency is
    /// invalid
    bool		compile(SOP_Node *root, const OP_Context &context, UT_ErrorManager &errors);
    bool		compileMulti(const UT_Array<SOP_Node *> &roots, const OP_Context &context, UT_ErrorManager &errors);

    /// Determines what node is at the root of the compile block
    SOP_Node		*getRoot(exint idx = 0) const;
    exint		 getNumRoots() const;

    /// Is the task list compiled & valid?
    bool		isCompiled() const;

    /// Donate the provided detail to be recycled if needed.
    /// The passed in handle will be cleared.
    void		donateForRecycling(GU_DetailHandle &gdh);

    /// Execute our cached block
    /// The inputgdhs will be cleared as they are used, allowing
    /// them to be stolen & written to if they are unique.
    GU_DetailHandle	execute(bool &hascedata,
				TaskStateArray &states, 
				bool flushce,
				UnloadMode unload, 
				const OP_Context &context, 
				UT_ErrorManager &errors, 
				DEP_MicroNode *depnode,
				const UT_StringArray &inputnames,
				UT_Array<GU_DetailHandle> &inputgdhs,
				const UT_Array<bool> &inputhascedata);

    /// Execute a compiled block that has multiple roots.
    void		executeMulti(
				UT_Array<GU_DetailHandle> &outputgdhs,
				UT_Array<bool> &hascedata,
				TaskStateArray &states, 
				bool flushce,
				UnloadMode unload, 
				const OP_Context &context, 
				UT_ErrorManager &errors, 
				DEP_MicroNode *depnode,
				const UT_StringArray &inputnames,
				UT_Array<GU_DetailHandle> &inputgdhs,
				const UT_Array<bool> &inputhascedata);

    void		clear();

    exint		taskCount() const { return myTasks.size(); }
    exint		taskInputCount() const { return myTaskInputs.size(); }
    exint		nodeCount() const { return myNodeSet ? myNodeSet->size() : 0; }

    /// Determines somewhat conservatively if the node is eligbile for
    /// compilation.  Verbs always are, but things like subnets are only
    /// if their SOP contents are as well.
    static bool		canBeCompiled(SOP_Node *node);


protected:
    DEP_TimedMicroNode	 myTopoMicroNode;

    UT_Array<Task *>	 myRoots;
    UT_Array<Task *>	 myTasks;
    UT_Array<Task *>	 myTaskInputs;

    SOP_NodeVerb::ForbiddenNodeMap	*myNodeSet;
    UT_Array<GU_DetailHandle>	myRecyclingBin;
};

#endif

