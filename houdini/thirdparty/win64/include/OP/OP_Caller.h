/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_Caller.h ( OP Library, C++)
 *
 * COMMENTS:	Sub-class of the UT_OpCaller which is used to handle op
 *		references.
 */

#ifndef __OP_Caller__
#define __OP_Caller__

#include "OP_API.h"
#include <UT/UT_OpCaller.h>
#include <UT/UT_ConcurrentVector.h>
#include <DEP/DEP_ContextOptions.h>

class OP_Node;
class DEP_MicroNode;

class OP_API OP_Caller : public UT_OpCaller
{
public:
	     OP_Caller(OP_Node *node,
		     const DEP_ContextOptionsStack *context_options_stack,
		     DEP_ContextOptionsReadHandle context_options);
	     OP_Caller(OP_Node *node, DEP_MicroNode *micronode,
		     const DEP_ContextOptionsStack *context_options_stack,
		     DEP_ContextOptionsReadHandle context_options);
    virtual ~OP_Caller();

    /// @{
    /// Get the calling node (i.e. the node being cooked)
    virtual int		 getOpId();
    OP_Node		*getNode()
			 { return myNode; }
    /// @}

    /// Return the options associated with the calling context.
    const DEP_ContextOptionsStack *getContextOptionsStack() const
			 { return myContextOptionsStack; }

    /// Return the options associated with the calling context.
    DEP_ContextOptionsReadHandle getContextOptions() const
			 { return myContextOptions; }

    /// Tell the calling node that they refer to the data for the other op
    /// If the @c callee is not NULL, it's doneWithOp() method will be called
    /// after the caller is finished with the node.
    virtual void	referenceOtherOp(int opid, UT_OpCallee *cache,
				bool flaginterest);

    /// Reference a specific parameter
    virtual void	referenceOtherParameter(int opid, int pi, int vi);

    /// Clear all references (notifying callee's about completion
    void		 clearReferences();

    /// Add operator dependencies -- the user should ensure that this is called
    /// in a thread-safe fashion.
    /// @note This is called automatically from the destructor.
    void		 addDependencies();

    /// Flag the caller to ignore time dependencies in addDependencies.
    /// Used when cooking a channel time range in VEXCHOP
    bool ignoreTimeDep() const;
    void setIgnoreTimeDep();

protected:

    class NodeInterest
    {
    public:
	NodeInterest(int id, UT_OpCallee *cache, bool flag = false)
	    : myOpId(id)
	    , myCallee(cache)
	    , myFlagInterest(flag)
	{
	}
	NodeInterest(const NodeInterest &src)
	    : myOpId(src.myOpId)
	    , myCallee(src.myCallee)
	    , myFlagInterest(src.myFlagInterest)
	{
	}
	NodeInterest	&operator=(const NodeInterest &src)
			 {
			     myOpId = src.myOpId;
			     myCallee = src.myCallee;
			     myFlagInterest = src.myFlagInterest;
			     return *this;
			 }
	bool		 operator==(const NodeInterest &src) const
			 {
			     return myOpId == src.myOpId &&
				    myCallee == src.myCallee &&
				    myFlagInterest == src.myFlagInterest;
			 }

	int		 getOpId() const	{ return myOpId; }
	UT_OpCallee	*getCallee() const	{ return myCallee; }
	bool		 getFlagInterest() const{ return myFlagInterest; }

    private:
	int		 myOpId;
	bool		 myFlagInterest;
	UT_OpCallee	*myCallee;
    };

    typedef UT_ConcurrentVector<NodeInterest>	ListType;

    ListType				 myInterests;
    OP_Node				*myNode;
    DEP_MicroNode			*myMicroNode;
    const DEP_ContextOptionsStack	*myContextOptionsStack;
    DEP_ContextOptionsReadHandle	 myContextOptions;
    bool				 myIgnoreTimeDep;
};

#endif
