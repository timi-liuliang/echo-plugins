/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	DEP_MicroNode.h (DEP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __DEP_MICRONODE_H_INCLUDED__
#define __DEP_MICRONODE_H_INCLUDED__

#include "DEP_API.h"
#include "DEP_ContextOptions.h"
#include <UT/UT_Array.h>
#include <UT/UT_ConcurrentVector.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_NonCopyable.h>
#include <UT/UT_SmallObject.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_TBBSpinLock.h>
#include <UT/UT_ValArray.h>
#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Math.h>
#include <SYS/SYS_Types.h>
#include <functional>
#include <iosfwd>

class DEP_MicroNode;

typedef UT_Array<DEP_MicroNode *>		DEP_MicroNodeList;

/// Propagation info for a dep micro node
class DEP_API DEP_PropagateData : UT_NonCopyable
{
public:
    DEP_PropagateData() { myExprChange = false; }
    void		setExprChange(bool expr_change)
			{ myExprChange = expr_change; }
    bool		isExprChange() const
			{ return myExprChange; }

protected:
    bool		myExprChange:1;	// a parm expr modification?
};

/// Represent an atomic unit of computation for dependency tracking.
///
/// DEP_MicroNode's form a dependency graph that are connected both implicitly
/// and explicitly. The base class maintains the explicit edges while
/// subclasses maintain the implicit edges by overriding the
/// getInputs()/getOutputs() methods.
///
class DEP_API DEP_MicroNode : public UT_SmallObject<UT_NonCopyable>
{
public:
				DEP_MicroNode();
    virtual			~DEP_MicroNode();

    virtual const char *	className() const
				{ return "DEP_MicroNode"; }

    /// Get list of all our non-null inputs, both implicit and explicit.
    /// The base class returns the list of explicit inputs.
    virtual void		getInputs(DEP_MicroNodeList &inputs) const;

    /// Clear all inputs, including any state data related to them. Default
    /// implementation just calls clearExplicitInputs() and
    /// setTimeDependent(false).
    virtual void		clearInputs()
				{
				    clearExplicitInputs();
				    setTimeDependent(false);
				    clearContextOptionDeps();
				}

    /// Get list of all our non-null outputs, both implicit and explicit.
    /// The base class returns the list of explicit outputs.
    virtual void		getOutputs(DEP_MicroNodeList &outputs) const;

    /// update() is used by users of micro-nodes to mark this node as clean at
    /// the given time t. By default, it simply marks it as no longer dirty but
    /// subclasses override this to do more work.
    virtual void		update(fpreal t)
				{ setDirty(false); }

    /// requiresUpdate() specifies if the micro-node was dirtied, possibly
    /// based on the time t. By default, it returns isDirty() but subclasses
    /// override this to handle other conditions that make this micro-node
    /// requiring an update.
    virtual bool		requiresUpdate(fpreal t) const
				{ return isDirty(); }

    /// Methods for manipulating explicit edges
    // @{

    /// Add an input that this micronode depends on, with control on whether to
    /// check for duplicates.
    void			addExplicitInput(
				    DEP_MicroNode &inp,
				    bool check_dup);

    /// Add an input that this micronode depends on
    void			addExplicitInput(DEP_MicroNode &inp)
				{ addExplicitInput(inp, /*check_dup*/true); }

    /// Get list of all non-null explicit inputs
    void			getExplicitInputs(
				    DEP_MicroNodeList &inputs) const;

    /// Get the number of non-null inputs
    int				getNumExplicitInputs() const
				{ return countEdges(myExplicitInputs); }

    /// Get list of all non-null explicit outputs
    void			getExplicitOutputs(
				    DEP_MicroNodeList &outputs) const;

    /// Get the number of non-null outputs
    int				getNumExplicitOutputs() const
				{ return countEdges(myExplicitOutputs); }

    // @}

    typedef std::function<
	void (DEP_MicroNode & /*micronode*/,
	      DEP_MicroNode & /*src*/)
	>			Visitor;
    typedef std::function<
	bool (const DEP_MicroNode & /*micronode*/,
	      const DEP_MicroNode & /*src*/)
	>			TraverseVisitor;

    /// Mark this micronode and all its dependents as dirty. The global dirty
    /// counter will be incremented once for each call to propagateDirty().
    ///
    /// @param client_visit	Called for each micronode that is dirtied
    /// @param only_outputs	If true, then *this is ommitted from being
    ///				dirtied
    ///
    /// @note This method is NOT thread-safe with *any* operation on *any*
    /// micronode. Only call this when no other micronode operations are
    /// running.
    void			propagateDirty(
				    Visitor client_visit,
				    bool only_outputs = false);

    /// Callbacks for what to do when the micronode becomes dirty.
    /// Derived micronodes can override this.  Note that it requires
    /// knowledge of who made it dirty as they may behave differently.
    /// Currently used by OP_Node::propagateDirtyMicroNode
    virtual void		becameDirty(DEP_MicroNode &src,
				    const DEP_PropagateData &propdata)
				{ }


    /// Traverse all ancestor inputs of this micronode
    void			traverseAncestorInputs(
				    const TraverseVisitor &visitor) const;

    /// Mark this micronode as having seen the given global dirty counter.
    /// Returns false if we've already seen it.
    bool			markVisitPass(int pass)
				{
				    if (myLastVisitPass == pass)
					return false;
				    myLastVisitPass = pass;
				    return true;
				}

    int				lastVisitPass() const
				    { return myLastVisitPass; }

    /// Obtain approximate memory usage
    virtual int64		getMemoryUsage(bool inclusive) const;

    /// Dump contents to output stream, strictly for debugging purposes
    virtual void		dump(
				    std::ostream &os,
				    const char *prefix = 0,
				    int indent_level = 0) const;
    /// Dump the contents is a manner more suitable for DOT.
    void			dumpAsDOT(
				    std::ostream &os,
				    const char *prefix = 0,
				    int indent_level = 0) const;

    typedef bool		(DEP_MicroNode::*GetFlag)() const;
    typedef void		(DEP_MicroNode::*SetFlag)(bool flag);

    /// Set time dependent flag if any explicit inputs are set.
    /// Returns true if any explicit inputs are true.
    bool			inheritTimeDependentFromExplicitInputs();

    /// Set time interested flag if any explicit inputs are set.
    /// Returns true if any explicit inputs are true.
    bool			inheritTimeInterestedFromExplicitInputs();

    /// Inherit extra dependencies from all explicit inputs.
    /// The ignore_deps string array MUST BE SORTED.
    /// Returns true if any extra dependencies are found.
    bool			inheritContextOptionDepsFromExplicitInputs(
					const UT_StringArray &ignore_deps);

    /// Flag accessors
    // @{
    bool			isDirty() const
				    { return myDirty; }
    void			setDirty(bool flag, bool allow_clear=true)
				    {
					myDirty = flag;
					if (allow_clear
					    && myDirty
					    && canClearDependencies())
					{
					    clearInputs();
					}
				    }

    bool			isTimeDependent() const
				    { return myTimeDependent; }
    void			setTimeDependent(bool time_dependent)
				    { myTimeDependent = time_dependent; }

    bool			isTimeInterested() const
				    { return myTimeInterested; }
    void			setTimeInterested(bool time_interested)
				    { myTimeInterested = time_interested; }

    bool			 hasContextOptionDeps() const
				    { return myContextOptionDeps &&
					!myContextOptionDeps->empty(); }
    const DEP_ContextOptionDeps	&getContextOptionDeps() const
				    { return DEPgetContextOptionDepsFromPtr(
					myContextOptionDeps); }
    void			addContextOptionDeps(
				    const DEP_ContextOptionDeps &opts)
				    { if (opts.empty()) return;
					DEPcreateContextOptionDepsFromPtr(
					    myContextOptionDeps,
					    &myContextOptionDepsLock).insert(
						opts.begin(), opts.end()); }
    void			addContextOptionDep(const UT_StringHolder &opt)
				    { if (!opt.isstring()) return;
					DEPcreateContextOptionDepsFromPtr(
					    myContextOptionDeps,
					    &myContextOptionDepsLock).
						insert(opt); }
    void			clearContextOptionDeps()
				    { DEP_ContextOptionDepsLockScope scope(
					myContextOptionDepsLock);
					myContextOptionDeps.reset(); }

    bool			isOpDataMicroNode() const
				    { return myIsOPDataMicroNode; }
    // @}

    /// Global control whether dependencies can be cleared.
    static bool			canClearDependencies()
				    { return (theKeepDependenciesCount == 0); }

    /// Global control whether we can evaluate exports
    static bool			canEvaluateExports()
				    { return (theBlockEvaluateExports == 0); }

    /// One true method to compare equality of time values for caching.
    /// This particular form originates from what OP_Node::cook() did from the
    /// dawn of time.
    static SYS_FORCE_INLINE bool isSameTime(fpreal a, fpreal b)
				    { return SYSequalZero(a - b); }

protected:
    /// These methods are overriden by subclasses to be used by the
    /// dirty propagation algorithm.
    /// @{

    /// Used to determine if a micronode is currently evaluating. If this
    /// returns true, during dirty propagation, it will prevent dependencies
    /// from being removed from it.
    virtual bool		isEvaluating() const
				    { return false; }

    /// evaluateExports() is called after isExporting() is noted to be true
    /// during a dirty propagation pass initiated by dirtySubtree().
    virtual void		evaluateExports() { }

    /// Used to determine if a micronode requires its exports to be evaluated
    /// during the propagation pass.
    virtual bool		isExporting() const
				    { return false; }

    /// @}

    void setIsOpDataMicroNode( bool b )
				    { myIsOPDataMicroNode = b; }

private:
    typedef UT_TBBSpinLock	    EdgeLock;
    typedef UT_TBBSpinLock::Scope   EdgeLockScope;

    typedef UT_ConcurrentVector<DEP_MicroNode *>
				MicroNodePtrVector;

    void			clearExplicitInputs();
    void			clearExplicitOutputs();

    void			compactExplicitInputsAndOutputs();

    template <GetFlag GET_FLAG, SetFlag SET_FLAG>
    bool			inheritFlag(const MicroNodePtrVector &inputs);

    static MicroNodePtrVector &	getRawExplicitInputs(DEP_MicroNode &n)
				{ return n.myExplicitInputs; }
    static MicroNodePtrVector &	getRawExplicitOutputs(DEP_MicroNode &n)
				{ return n.myExplicitOutputs; }

    template <typename ADJACENT_FUNC>
    static void			clearEdges(
				    DEP_MicroNode *self,
				    MicroNodePtrVector &vec,
				    ADJACENT_FUNC get_adjacent);

    static void			compactVector(
				    MicroNodePtrVector &vec);

    static void			getEdgeList(
				    const MicroNodePtrVector &edges,
				    DEP_MicroNodeList &micronodes);
    static int			countEdges(const MicroNodePtrVector &edges);

private:
    /// The list of explicit edges for micronodes
    // @{
    EdgeLock			myExplicitInputsLock;
    MicroNodePtrVector		myExplicitInputs;
    MicroNodePtrVector		myExplicitOutputs;
    // @}

    DEP_ContextOptionDepsLock	myContextOptionDepsLock;
    DEP_ContextOptionDepsPtr	myContextOptionDeps;
    int				myLastVisitPass;

    bool			myDirty:1;
    bool			myTimeDependent:1;
    bool			myTimeInterested:1;
    bool			myIsOPDataMicroNode:1;

    static int			theKeepDependenciesCount;
    static int			theBlockEvaluateExports;

    friend class		dep_Visitor;
    friend class		DEP_KeepDependenciesScope;
    friend class		DEP_BlockEvaluateExportsScope;
};

/// Global control whether dependencies can be cleared.
class DEP_API DEP_KeepDependenciesScope
{
public:
    DEP_KeepDependenciesScope()
	{ ++DEP_MicroNode::theKeepDependenciesCount; }
    ~DEP_KeepDependenciesScope()
	{ --DEP_MicroNode::theKeepDependenciesCount; }
};

/// Global control whether we can evaluate exports
class DEP_API DEP_BlockEvaluateExportsScope
{
public:
    DEP_BlockEvaluateExportsScope()
	{ ++DEP_MicroNode::theBlockEvaluateExports; }
    ~DEP_BlockEvaluateExportsScope()
	{ --DEP_MicroNode::theBlockEvaluateExports; }
};

#endif // __DEP_MICRONODE_H_INCLUDED__
