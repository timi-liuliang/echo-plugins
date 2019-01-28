/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __HUSD_DataHandle_h__
#define __HUSD_DataHandle_h__

#include "HUSD_API.h"
#include <UT/UT_Assert.h>
#include <UT/UT_IntrusivePtr.h>
#include <UT/UT_NonCopyable.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_StringArray.h>
#include <pxr/pxr.h>

class UT_StringArray;
class HUSD_FindPrims;
class HUSD_Overrides;
typedef UT_IntrusivePtr<HUSD_Overrides>		 HUSD_OverridesPtr;
typedef UT_IntrusivePtr<const HUSD_Overrides>	 HUSD_ConstOverridesPtr;

PXR_NAMESPACE_OPEN_SCOPE

class XUSD_Data;
typedef UT_IntrusivePtr<XUSD_Data>		 XUSD_DataPtr;
typedef UT_IntrusivePtr<const XUSD_Data>	 XUSD_ConstDataPtr;
class XUSD_Layer;
typedef UT_IntrusivePtr<XUSD_Layer>		 XUSD_LayerPtr;
typedef UT_IntrusivePtr<const XUSD_Layer>	 XUSD_ConstLayerPtr;
class XUSD_DataLock;
typedef UT_IntrusivePtr<XUSD_DataLock>		 XUSD_DataLockPtr;
class XUSD_Ticket;
typedef UT_IntrusivePtr<XUSD_Ticket>		 XUSD_TicketPtr;
typedef UT_Array<XUSD_TicketPtr>		 XUSD_TicketArray;

PXR_NAMESPACE_CLOSE_SCOPE

enum HUSD_MirroringType {
    HUSD_NOT_FOR_MIRRORING,
    HUSD_FOR_MIRRORING
};

class HUSD_API HUSD_DataHandle
{
public:
				 HUSD_DataHandle(HUSD_MirroringType
					mirroring = HUSD_NOT_FOR_MIRRORING);
				 HUSD_DataHandle(const HUSD_DataHandle &src);
				~HUSD_DataHandle();

    void			 reset(int id, const UT_StringHolder &tag);
    const HUSD_DataHandle	&operator=(const HUSD_DataHandle &src);
    void			 createNewData();
    bool			 createSoftCopy(const HUSD_DataHandle &src,
					bool make_new_implicit_layer);
    bool			 flattenStage();
    bool			 mirror(const HUSD_DataHandle &src);

    const UT_StringArray	&lastModifiedPrims() const
				 { return myLastModifiedPrims; }
    void			 setLastModifiedPrims(
					const HUSD_FindPrims &prims);
    void			 setLastModifiedPrims(
					const UT_StringArray &prims)
				 { myLastModifiedPrims = prims; }
    void			 clearLastModifiedPrims()
				 { myLastModifiedPrims.clear(); }

    const UT_StringHolder	&tag() const
				 { return myTag; }
    int				 id() const
				 { return myId; }

    PXR_NS::XUSD_ConstDataPtr	 readLock(const HUSD_ConstOverridesPtr
					&overrides) const;
    PXR_NS::XUSD_DataPtr	 writeOverridesLock(const HUSD_OverridesPtr
					&overrides,
					int layer_idx) const;
    PXR_NS::XUSD_DataPtr	 writeLock(bool edit_active_layer) const;
    PXR_NS::XUSD_LayerPtr	 layerLock(PXR_NS::XUSD_DataPtr &data) const;
    void			 release() const;

private:
    HUSD_ConstOverridesPtr	 currentOverrides() const;

    friend class HUSD_AutoReadLock;

    PXR_NS::XUSD_DataPtr	 myData;
    PXR_NS::XUSD_DataLockPtr	 myDataLock;
    HUSD_MirroringType		 myMirroring;
    UT_StringArray		 myLastModifiedPrims;
    UT_StringHolder		 myTag;
    int				 myId;
};

// Parent class for read and write locks that permits reading.
class HUSD_API HUSD_AutoAnyLock : UT_NonCopyable
{
public:
					 HUSD_AutoAnyLock()
					 { }
    virtual				~HUSD_AutoAnyLock()
					 { }

    virtual PXR_NS::XUSD_ConstDataPtr	 constData() const = 0;
};

// Locks an HUSD_DataHandle for read-only operations.
class HUSD_API HUSD_AutoReadLock : public HUSD_AutoAnyLock
{
public:
    enum HUSD_OverridesUnchangedType { OVERRIDES_UNCHANGED };

					 HUSD_AutoReadLock(
						const HUSD_DataHandle &handle);
					 HUSD_AutoReadLock(
						const HUSD_DataHandle &handle,
						HUSD_OverridesUnchangedType);
					 HUSD_AutoReadLock(
						const HUSD_DataHandle &handle,
						const HUSD_ConstOverridesPtr
						    &overrides);
    virtual				~HUSD_AutoReadLock();

    const PXR_NS::XUSD_ConstDataPtr	&data() const
					 { return myData; }
    virtual PXR_NS::XUSD_ConstDataPtr	 constData() const;

private:
    const HUSD_DataHandle		&myHandle;
    PXR_NS::XUSD_ConstDataPtr		 myData;
};

// Locks an HUSD_DataHandle in a way that allows read-only operations on the
// data, but write access to the supplied overrides layer.
class HUSD_API HUSD_AutoWriteOverridesLock : public HUSD_AutoAnyLock
{
public:
					 HUSD_AutoWriteOverridesLock(
						const HUSD_DataHandle &handle,
						const HUSD_OverridesPtr
						    &overrides,
						int layer_idx);
    virtual				~HUSD_AutoWriteOverridesLock();

    const PXR_NS::XUSD_LayerPtr		&layer() const;
    const PXR_NS::XUSD_DataPtr		&data() const
					 { return myData; }
    virtual PXR_NS::XUSD_ConstDataPtr	 constData() const;

private:
    const HUSD_DataHandle		&myHandle;
    PXR_NS::XUSD_DataPtr		 myData;
    HUSD_OverridesPtr			 myOverrides;
    int					 myLayerIdx;
};

// Locks an HUSD_DataHandle for writing to the active layer in the context
// of the current stage.
class HUSD_API HUSD_AutoWriteLock : public HUSD_AutoAnyLock
{
public:
					 HUSD_AutoWriteLock(
						const HUSD_DataHandle &handle,
						bool edit_active_layer = true);
    virtual				~HUSD_AutoWriteLock();

    const PXR_NS::XUSD_DataPtr		&data() const
					 { return myData; }
    virtual PXR_NS::XUSD_ConstDataPtr	 constData() const;

private:
    const HUSD_DataHandle		&myHandle;
    PXR_NS::XUSD_DataPtr		 myData;
};

// Locks an HUSD_DataHandle for writing to the active layer outside the
// context of the current stage (which is accessible read-only while editing
// this off-stage layer).
class HUSD_API HUSD_AutoLayerLock : public HUSD_AutoAnyLock
{
public:
					 HUSD_AutoLayerLock(
						const HUSD_DataHandle &handle);
					~HUSD_AutoLayerLock();

    const PXR_NS::XUSD_LayerPtr		&layer() const
					 { return myLayer; }
    void				 addTickets(const PXR_NS::
					    XUSD_TicketArray &tickets) const;
    virtual PXR_NS::XUSD_ConstDataPtr	 constData() const;

private:
    const HUSD_DataHandle		&myHandle;
    PXR_NS::XUSD_DataPtr		 myData;
    PXR_NS::XUSD_LayerPtr		 myLayer;
};

#endif

