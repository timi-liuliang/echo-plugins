/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OH_OpInfo.h (C++, OPUI library)
 *
 * COMMENTS:
 *		This structure is used to store info about nodes.
 */

#ifndef __OH_OpInfo__
#define __OH_OpInfo__

#include "OH_API.h"
class OP_Node;

#define OH_DEFINE_GET_FLAG( __FLAG__ ) { return myFlags & __FLAG__; }
#define OH_DEFINE_SET_FLAG( __FLAG__ ) \
{ \
    if (v) \
	myFlags |= __FLAG__; \
    else \
	myFlags &= ~__FLAG__; \
}

class OH_API OH_OpInfo
{
public:
	     OH_OpInfo(OP_Node *node);
    virtual ~OH_OpInfo();

    OP_Node		*getNode() { return myNode; }

    // The UpdatePending flag must be set by the OH_OpEventHandler derived
    // class before calling ohRequestDeferredUpdate

    bool		 getUpdatePendingFlag()
			    OH_DEFINE_GET_FLAG( OH_UPDATE_PENDING )
    void		 setUpdatePendingFlag(bool v)
			    OH_DEFINE_SET_FLAG( OH_UPDATE_PENDING )

    // This flag is provided for the use of derived classes. You
    // would typically use it to flag any nodes that contain other
    // nodes in which you are interested.
    bool		 getContainerNodeFlag()
			    OH_DEFINE_GET_FLAG( OH_CONTAINER_NODE )
    void		 setContainerNodeFlag(bool v)
			    OH_DEFINE_SET_FLAG( OH_CONTAINER_NODE )

    // This flag is provided for the use of derived classes as well.
    // You could use it to flag the one node that is the top level
    // node in which you are interested.
    bool		 getMasterNodeFlag()
			    OH_DEFINE_GET_FLAG( OH_MASTER_NODE )
    void		 setMasterNodeFlag(bool v)
			    OH_DEFINE_SET_FLAG( OH_MASTER_NODE )
protected:
    enum
    {
	OH_UPDATE_PENDING	= 0x1,
	OH_MASTER_NODE		= 0x2,
	OH_CONTAINER_NODE	= 0x4,
	OH_OPINFO_BITLIMIT	= 0x8	// This should always be last & largest
	    // subclasses can use the rest of myFlags by using
	    // OH_OPINFO_BITLIMIT, OH_OPINFO_BITLIMIT << 1, etc.
    };

    unsigned int	 myFlags;

private:
    OP_Node		*myNode;
};


class OH_API OH_RefCountOpInfo : public OH_OpInfo
{
public:
	    OH_RefCountOpInfo(OP_Node *node);
   virtual ~OH_RefCountOpInfo();

   void	    bumpRefCount(int dir) { myRefCount += dir; }
   int	    getRefCount() const { return myRefCount; }

private:
   int	    myRefCount;
};


#endif
