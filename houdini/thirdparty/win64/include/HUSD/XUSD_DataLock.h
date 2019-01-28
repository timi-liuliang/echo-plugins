/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __XUSD_DataLock_h__
#define __XUSD_DataLock_h__

#include "HUSD_API.h"
#include <OP/OP_ItemId.h>
#include <UT/UT_IntrusivePtr.h>
#include <UT/UT_Lock.h>
#include <UT/UT_String.h>
#include <UT/UT_StringHolder.h>
#include <pxr/pxr.h>

PXR_NAMESPACE_OPEN_SCOPE

class XUSD_DataLock : public UT_IntrusiveRefCounter<XUSD_DataLock>
{
public:
			 XUSD_DataLock()
			     : myLockCount(0),
			       myLockedId(OP_INVALID_ITEM_ID),
			       myWriteLock(false),
			       myLayerLock(false),
			       myWriteLockedTagAddedLayer(false)
			 {
			 }
			~XUSD_DataLock()
			 { }

    bool		 isLocked() const
			 { return myLockCount > 0; }
    bool		 isReadLocked() const
			 { return myLockCount > 0 && !myWriteLock; }
    bool		 isWriteLocked() const
			 { return myLockCount > 0 && myWriteLock; }
    bool		 isLayerLocked() const
			 { return myLockCount > 0 && myLayerLock; }
    std::string		 getWriteLockedTag() const
			 { return myWriteLockedTag.toStdString(); }

    void		 addLayerToWriteLockedTag()
			 {
			     if (!myWriteLockedTagAddedLayer)
			     {
				 myWriteLockedTagAddedLayer = true;
				 myWriteLockedTag += "/addlayer1";
			     }
			     else
			     {
				 UT_String incrementer(myWriteLockedTag);
				 incrementer.incrementNumberedName();
				 myWriteLockedTag = incrementer;
			     }
			 }

private:
    UT_StringHolder	 myWriteLockedTag;
    UT_Lock		 myMutex;
    int			 myLockCount;
    int			 myLockedId;
    bool		 myWriteLock;
    bool		 myLayerLock;
    bool		 myWriteLockedTagAddedLayer;

    friend class ::HUSD_DataHandle;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif

