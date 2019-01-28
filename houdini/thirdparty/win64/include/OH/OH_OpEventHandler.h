/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OH_OpEventHandler.h ( Op Handler Library, C++)
 *
 * COMMENTS:	Read the documentation in DOCS/index.html
 */

#ifndef __OH_OpEventHandler__
#define __OH_OpEventHandler__

#include "OH_API.h"
#include "OH_EventHandler.h"
#include <UT/UT_ValArray.h>
#include <OP/OP_Node.h>

class OH_OpInfo;

class OH_API OH_OpEventHandler : public OH_EventHandler
{
public:
		 	 OH_OpEventHandler(OH_TriggerObject &);
    virtual	 	~OH_OpEventHandler();
    virtual const char 	*ohClassName() const { return "OH_OpEventHandler"; }

    // These methods are used to add interests to the nodes that you
    // want this object to watch. When you add an interest in a node,
    // an OH_OpInfo corresponding to your node will be added to this
    // OH gadget to track that node.
    OH_OpInfo		*ohAddOpInterest(OP_Node *node);
    void		 ohRemoveOpInterest(OP_Node *node);
    void		 ohRemoveOpInterest(long index);
    void		 ohRemoveAllOpInterests();

    // You override this method so that you can respond to the events
    // that are sent out by the nodes in which you are interested.
    //
    // Typically, your response is to use ohFindOpInfo to get the OH_OpInfo
    // for the specified node, and then set the OpInfo's UpdatePendingFlag,
    // and call ohRequestDeferredUpdate() on this object.
    virtual void	 ohHandleOpEvent(OP_Node *node, OP_EventType type,
					 void *data) = 0;
    
    // OH_OpEventHandler overrides the base-class ohHandleDeferredUpdate
    // so that it splits the "Update" task into a per Node operation and
    // calls ohHandleDeferredNodeUpdate on each node. You will typically
    // only override the latter.
    virtual void	 ohHandleDeferredUpdate();
    virtual void	 ohHandleDeferredNodeUpdate(OH_OpInfo *nodeinfo);

    // These are the low-level OpInfo Management methods. You override
    // ohCreateOpInfo to create a subclass of OH_OpInfo if you want to
    // extend the OH_OpInfo structure.
    virtual OH_OpInfo	*ohCreateOpInfo(OP_Node *node);
    int			 ohGetOpInfoCount() 
				{ return myOpInfos.entries(); }
    int			 ohFindOpInfoIndex(OP_Node *node);
    OH_OpInfo		*ohFindOpInfo(OP_Node *node);
    OH_OpInfo		*ohGetOpInfo(long index)
				{ return myOpInfos(index); }


private:
    static void		 ohHandleOpEvent(OP_Node *caller, void *callee,
					 OP_EventType type, void *data);

    UT_ValArray<OH_OpInfo *>	myOpInfos;
};

#endif
