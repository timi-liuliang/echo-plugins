/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_NetworkDot_h__
#define __HOM_NetworkDot_h__

#include "HOM_API.h"
#include "HOM_IndirectInput.h"
#include "HOM_PtrOrNull.h"

class HOM_Node;
class HOM_NodeConnection;

SWIGOUT(%rename(NetworkDot) HOM_NetworkDot;)

class HOM_API HOM_NetworkDot : virtual public HOM_IndirectInput
{
public:
    HOM_NetworkDot()
    : HOM_IndirectInput(HOM_networkItemType::NetworkDot),
      HOM_NetworkMovableItem(HOM_networkItemType::NetworkDot)
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_NetworkDot(const HOM_NetworkDot &dot)
    : HOM_IndirectInput(dot),
      HOM_NetworkMovableItem(dot)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_NetworkDot()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;

    virtual bool operator==(HOM_PtrOrNull<HOM_NetworkDot> dot) = 0;
    virtual bool operator!=(HOM_PtrOrNull<HOM_NetworkDot> dot) = 0;

    virtual bool isPinned() = 0;
    virtual void setPinned(bool pinned) = 0;

    virtual void setInput(
	    HOM_NetworkMovableItem *item_to_become_input,
	    int output_index = 0) = 0;
    virtual void setInput(int input_index,
	    HOM_NetworkMovableItem *item_to_become_input,
	    int output_index = 0) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_NodeConnection> > inputConnections() = 0;

    virtual void destroy() = 0;
};

#endif
