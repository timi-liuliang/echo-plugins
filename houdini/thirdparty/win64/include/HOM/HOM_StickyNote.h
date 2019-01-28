/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_StickyNote_h__
#define __HOM_StickyNote_h__

#include "HOM_API.h"
#include "HOM_NetworkMovableItem.h"
#include "HOM_EnumModules.h"
#include "HOM_PtrOrNull.h"
#include <string>

class HOM_Color;
class HOM_BoundingRect;

SWIGOUT(%rename(StickyNote) HOM_StickyNote;)

class HOM_API HOM_StickyNote : virtual public HOM_NetworkMovableItem
{
public:
    HOM_StickyNote()
    : HOM_NetworkMovableItem(HOM_networkItemType::StickyNote)
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_StickyNote(const HOM_StickyNote &sticky_note)
    : HOM_NetworkMovableItem(sticky_note)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_StickyNote()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;

    virtual bool operator==(HOM_PtrOrNull<HOM_StickyNote> stickynote) = 0;
    virtual bool operator!=(HOM_PtrOrNull<HOM_StickyNote> stickynote) = 0;

    virtual std::string text() = 0;
    virtual void setText(const char* new_text) = 0;

    virtual float textSize() = 0;
    virtual void setTextSize(float size) = 0;

    virtual HOM_Color textColor() = 0;
    virtual void setTextColor(HOM_Color &color) = 0;

    virtual bool drawBackground() = 0;
    virtual void setDrawBackground(bool on) = 0;

    virtual void setBounds(const HOM_BoundingRect &bounds) = 0;
    virtual void setSize(const std::vector<double> &size) = 0;
    virtual void resize(const std::vector<double> &amount) = 0;

    virtual HOM_Vector2 minimizedSize() = 0;
    virtual HOM_Vector2 restoredSize() = 0;

    virtual bool isMinimized() = 0;
    virtual void setMinimized(bool on) = 0;

    virtual void destroy(void) = 0;

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
