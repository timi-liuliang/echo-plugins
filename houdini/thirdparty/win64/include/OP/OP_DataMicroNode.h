/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_DataMicroNode.h (OP Library, C++)
 *
 * COMMENTS:	DEP_MicroNode subclass representing an OP_Node's output data.
 */

#ifndef __OP_DATAMICRONODE_H_INCLUDED__
#define __OP_DATAMICRONODE_H_INCLUDED__

#include "OP_API.h"
#include "OP_Version.h"
#include <DEP/DEP_MicroNode.h>
#include <SYS/SYS_Math.h>
#include <SYS/SYS_Types.h>

class OP_Node;

class OP_API OP_DataMicroNode : public DEP_MicroNode
{
public:
			    OP_DataMicroNode(OP_Node &node);

    virtual const char *    className() const
				{ return "OP_DataMicroNode"; }

    void		    getParmInputs(DEP_MicroNodeList &inputs) const;
    void		    getUsedDataInputs(DEP_MicroNodeList &inputs) const;

    virtual void	    getInputs(DEP_MicroNodeList &inputs) const;
    virtual void	    clearInputs();
    virtual void	    getOutputs(DEP_MicroNodeList &outputs) const;

    /// Like getInputs() but excludes wired OP_Node inputs
    void		    getExtraInputs(DEP_MicroNodeList &inputs) const;

    /// Like getOutputs() but excludes wired OP_Node outputs. For parmlist
    /// micronodes, this will exclude the its owner OP_Node.
    void		    getExtraOutputs(DEP_MicroNodeList &outputs) const;

    /// update() is used by users of micro-nodes to mark this node as clean at
    /// the given time t.
    virtual void	    update(fpreal t);
    void		    update(DEP_ContextOptionsReadHandle options,
				const DEP_ContextOptionsStack *optstack);

    /// requiresUpdate() specifies if the micro-node was dirtied, possibly
    /// based on the time t.
    virtual bool	    requiresUpdate(fpreal t) const;
    bool		    requiresUpdate(
				DEP_ContextOptionsReadHandle options,
				const DEP_ContextOptionsStack *optstack) const;

    /// Returns the options and values _used_ in the last cook of this data.
    DEP_ContextOptionsReadHandle getLastUsedContextOptions() const
			    { return myLastUsedContextOptions; }
    /// Returns all options and values provided to the last cook of this data.
    const DEP_ContextOptionsReadHandle &getLastAllContextOptions() const
			    { return myLastAllContextOptions; }

    /// Used to determine if a micronode is currently evaluating. If this
    /// returns true, during dirty propagation, it will prevent dependencies
    /// from being removed from it.
    virtual bool	    isEvaluating() const;

    /// evaluateExports() is called after isExporting() is noted to be true
    /// during a dirty propagation pass initiated by dirtySubtree().
    virtual void	    evaluateExports();

    /// Used to determine if a micronode requires its exports to be evaluated
    /// during the propagation pass.
    virtual bool	    isExporting() const;

    virtual int64	    getMemoryUsage(bool inclusive) const;

    virtual void	    dump(
				std::ostream &os,
				const char *prefix = 0,
				int indent_level = 0) const;

    OP_Node &		    ownerNode() const
				{ return myNode; }

    void		    bumpModVersion()
				{ myModVersion++; }
    bool		    isModVersionOutdated() const
				{ return myModVersion != myLastModVersion; }
    OP_VERSION		    modVersion() const
				{ return myModVersion; }
    OP_VERSION		    lastModVersion() const
				{ return myLastModVersion; }

    fpreal		    lastUpdateTime() const
				{ return myLastUpdateTime; }
				
    bool		    isEqualToLastUpdateTime(fpreal t) const
				{ return isSameTime(t, myLastUpdateTime); }

    virtual void	    becameDirty(DEP_MicroNode &src,
				const DEP_PropagateData &data);

    void		    setModVersion(OP_VERSION v)
				{ myModVersion = v; }

private:
    OP_Node					&myNode;
    DEP_ContextOptionsHandle			 myLastUsedContextOptions;
    UT_UniquePtr<DEP_ContextOptionsStack>	 myLastUsedContextStack;
    DEP_ContextOptionsReadHandle		 myLastAllContextOptions;
    // Bumped everytime the node gets dirty.
    OP_VERSION					 myModVersion;
    // Current version of the entire cache
    OP_VERSION					 myLastModVersion;
    fpreal					 myLastUpdateTime;
};

#endif // __OP_DATAMICRONODE_H_INCLUDED__
