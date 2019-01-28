/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __HUSD_Data_h__
#define __HUSD_Data_h__

#include "HUSD_API.h"
#include "HUSD_DataHandle.h"
#include "HUSD_Overrides.h"
#include "XUSD_DataLock.h"
#include "XUSD_Ticket.h"
#include <UT/UT_StringArray.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_IntrusivePtr.h>
#include <UT/UT_SharedPtr.h>
#include <UT/UT_UniquePtr.h>
#include <pxr/usd/usd/stage.h>

PXR_NAMESPACE_OPEN_SCOPE

typedef UT_Array<SdfLayerRefPtr>	 XUSD_LayerArray;
typedef UT_Array<SdfLayerOffset>	 XUSD_LayerOffsetArray;

enum XUSD_AddLayerOp {
    XUSD_ADD_LAYER_LOCKED,
    XUSD_ADD_LAYER_EDITABLE,
    XUSD_ADD_LAYER_SEQUENCE,
};

class HUSD_API XUSD_Data : public UT_IntrusiveRefCounter<XUSD_Data>,
			   public UT_NonCopyable
{
public:
				 XUSD_Data(HUSD_MirroringType mirroring);
				~XUSD_Data();

    bool			 isStageValid() const;
    const UsdStageRefPtr	&stage() const;
    const SdfLayerRefPtr	&activeLayer() const;
    const SdfLayerRefPtr	&activeSourceLayer() const;
    const XUSD_LayerArray	&sourceLayers() const;
    const XUSD_LayerOffsetArray	&sourceLayerOffsets() const;
    const HUSD_ConstOverridesPtr&overrides() const;

    // These functions should only be called when you have a
    // "manage layers lock" since they affect the edit target.
    bool			 addLayer(const std::string &filepath,
					const SdfLayerOffset &offset,
					XUSD_AddLayerOp add_layer_op);
    bool			 addLayer(const SdfLayerRefPtr &layer,
					const SdfLayerOffset &offset,
					XUSD_AddLayerOp add_layer_op);
    bool			 addLayer();

    // Store a ticket in with this data to keep alive cooked node data in the
    // XUSD_TicketRegistry as long as it might be referenced by our stage.
    void			 addTicket(const XUSD_TicketPtr &ticket);
    void			 addTickets(const XUSD_TicketArray &tickets);
    const XUSD_TicketArray	&tickets() const;

private:
    void			 reset();
    void			 createNewData();
    void			 createSoftCopy(const XUSD_Data &src,
					bool make_new_implicit_layer);
    void			 flattenStage(const XUSD_Data &src);
    void			 mirror(const XUSD_Data &src);

    void			 afterLock(bool for_write,
					const HUSD_ConstOverridesPtr
					    &read_overrides =
					    HUSD_ConstOverridesPtr(),
					const HUSD_OverridesPtr
					    &write_overrides =
					    HUSD_OverridesPtr(),
					int write_overrides_layer_idx = 0);
    XUSD_LayerPtr		 editActiveSourceLayer();
    void			 afterRelease();

    static void			 exitCallback(void *);

    UsdStageRefPtr			 myStage;
    UT_SharedPtr<UT_StringArray>	 myStageLayerAssignments;
    UT_SharedPtr<XUSD_LayerArray>	 myStageLayers;
    XUSD_LayerArray			 mySourceLayers;
    XUSD_LayerOffsetArray		 mySourceLayerOffsets;
    HUSD_ConstOverridesPtr		 myReadOverrides;
    HUSD_OverridesPtr			 myWriteOverrides;
    XUSD_DataLockPtr			 myDataLock;
    XUSD_TicketArray			 myTicketArray;
    HUSD_MirroringType			 myMirroring;
    int					 myWriteOverridesLayerIdx;
    int					 myActiveLayerIndex;
    bool				 myActiveLayerChanged;

    friend class ::HUSD_DataHandle;
};

class HUSD_API XUSD_Layer : public UT_IntrusiveRefCounter<XUSD_Layer>
{
public:
				 XUSD_Layer(const SdfLayerRefPtr &layer,
					 bool create_change_block)
				     : myLayer(layer),
				       myChangeBlock(nullptr)
				 {
				     if (create_change_block)
					 myChangeBlock.reset(
					     new SdfChangeBlock());
				 }
				~XUSD_Layer()
				 { }

    const SdfLayerRefPtr	&layer() const
				 { return myLayer; }

private:
    SdfLayerRefPtr		 myLayer;
    UT_UniquePtr<SdfChangeBlock> myChangeBlock;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif

