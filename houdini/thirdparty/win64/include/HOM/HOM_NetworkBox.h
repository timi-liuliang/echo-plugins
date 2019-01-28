/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_NetworkBox_h__
#define __HOM_NetworkBox_h__

#include "HOM_API.h"
#include "HOM_NetworkMovableItem.h"
#include "HOM_EnumModules.h"
#include <string>

class HOM_Color;
class HOM_Vector2;
class HOM_BoundingRect;
class HOM_StickyNote;
class HOM_SubnetIndirectInput;

SWIGOUT(%rename(NetworkBox) HOM_NetworkBox;)

class HOM_API HOM_NetworkBox : virtual public HOM_NetworkMovableItem
{
public:
    HOM_NetworkBox()
    : HOM_NetworkMovableItem(HOM_networkItemType::NetworkBox)
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_NetworkBox(const HOM_NetworkBox &net_box)
    : HOM_NetworkMovableItem(net_box)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_NetworkBox()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;

    virtual bool operator==(HOM_PtrOrNull<HOM_NetworkBox> networkbox) = 0;
    virtual bool operator!=(HOM_PtrOrNull<HOM_NetworkBox> networkbox) = 0;

    virtual std::string comment() = 0;
    virtual void setComment(const char *comment) = 0;

    virtual bool autoFit() = 0;
    virtual void setAutoFit(bool auto_fit) = 0;

    virtual void setBounds(const HOM_BoundingRect &bounds) = 0;
    virtual void setSize(const std::vector<double> &size) = 0;
    virtual void resize(const std::vector<double> &amount) = 0;
    virtual void fitAroundContents() = 0;

    virtual HOM_Vector2 minimizedSize() = 0;
    virtual HOM_Vector2 restoredSize() = 0;

    virtual bool isMinimized() = 0;
    virtual void setMinimized(bool on) = 0;

    SWIGOUT(%newobject nodes;)
    SWIGOUT(%kwargs nodes;)
    virtual std::vector<HOM_ElemPtr<HOM_Node> > nodes(bool recurse=true) = 0;
    virtual void addNode(HOM_Node &node) = 0;
    virtual void removeNode(HOM_Node &node) = 0;
    virtual void removeAllNodes() = 0;

    SWIGOUT(%newobject networkBoxes;)
    SWIGOUT(%kwargs networkBoxes;)
    virtual std::vector<HOM_ElemPtr<HOM_NetworkBox> >
	networkBoxes(bool recurse=true) = 0;
    virtual void addNetworkBox(HOM_NetworkBox &networkbox) = 0;
    virtual void removeNetworkBox(HOM_NetworkBox &networkbox) = 0;

    SWIGOUT(%newobject stickyNotes;)
    SWIGOUT(%kwargs stickyNotes;)
    virtual std::vector<HOM_ElemPtr<HOM_StickyNote> >
	stickyNotes(bool recurse=true) = 0;
    virtual void addStickyNote(HOM_StickyNote &stickynote) = 0;
    virtual void removeStickyNote(HOM_StickyNote &stickynote) = 0;

    SWIGOUT(%newobject subnetIndirectInputs;)
    SWIGOUT(%kwargs subnetIndirectInputs;)
    virtual std::vector<HOM_ElemPtr<HOM_SubnetIndirectInput> >
	subnetIndirectInputs(bool recurse=true) = 0;
    virtual void addSubnetIndirectInput(HOM_SubnetIndirectInput &indirect) = 0;
    virtual void removeSubnetIndirectInput(HOM_SubnetIndirectInput &indirect) = 0;

    SWIGOUT(%newobject items;)
    SWIGOUT(%kwargs items;)
    virtual std::vector<HOM_ElemPtr<HOM_NetworkMovableItem> >
	    items(bool recurse=true) = 0;
    virtual void addItem(HOM_NetworkMovableItem &item) = 0;
    virtual void removeItem(HOM_NetworkMovableItem &item) = 0;
    virtual void removeAllItems() = 0;

    virtual void destroy(bool destroy_contents=false) = 0;

    SWIGOUT(%kwargs asCode;)
    virtual std::string asCode(bool brief=false,
			       bool recurse=false,
			       bool save_box_contents=false,
			       bool save_channels_only=false,
			       bool save_creation_commands=true,
			       bool save_keys_in_frames=false,
			       bool save_parm_values_only=false,
			       bool save_spare_parms=false,
			       const char *function_name=NULL) = 0;
};

#endif
