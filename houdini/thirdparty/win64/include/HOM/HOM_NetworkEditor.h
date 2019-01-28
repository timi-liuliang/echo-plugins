/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_NetworkEditor_h__
#define __HOM_NetworkEditor_h__

#include "HOM_PathBasedPaneTab.h"
#include "HOM_NetworkAnimValue.h"
#include "HOM_NetworkFootprint.h"
#include "HOM_NetworkImage.h"
#include "HOM_NetworkShape.h"
#include <UT/UT_Tuple.h>
#include <vector>

class HOM_Node;
class HOM_VopNode;
class HOM_NetworkItem;
class HOM_NetworkMovableItem;
class HOM_StickyNote;
class HOM_SubnetIndirectInput;
class HOM_BoundingRect;
class HOM_Vector2;

SWIGOUT(%rename(NetworkEditor) HOM_NetworkEditor;)

class HOM_API HOM_NetworkEditor : virtual public HOM_PathBasedPaneTab
{
public:
    HOM_NetworkEditor()
    { HOM_CONSTRUCT_OBJECT(this) }
    // Because the lowermost base classes initialize the virtual bases
    // before any non-virtual bases, the correct thing to do here is
    // explicitly call the constructor for HOM_PaneTab.
    HOM_NetworkEditor(const HOM_NetworkEditor &pane)
    : HOM_PaneTab(pane), HOM_PathBasedPaneTab(pane)
    { HOM_CONSTRUCT_OBJECT(this) }

    virtual ~HOM_NetworkEditor()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0);

    virtual bool isShowingConnectors() = 0;
    virtual bool isPosInside(const HOM_Vector2 &pos) = 0;
    virtual void setCursorPosition(const HOM_Vector2 &pos) = 0;
    virtual HOM_Vector2 cursorPosition(bool confine_to_view = true) = 0;
    virtual HOM_BoundingRect screenBounds() = 0;
    virtual HOM_BoundingRect visibleBounds() = 0;
    SWIGOUT(%kwargs setVisibleBounds;)
    virtual void setVisibleBounds(const HOM_BoundingRect &bounds,
	    float transition_time = 0.0,
	    float max_scale = 0.0,
	    bool set_center_when_scale_rejected = false) = 0;

    virtual void setLocatingEnabled(bool enabled) = 0;
    virtual bool locatingEnabled() = 0;

    virtual float lengthToScreen(float len) = 0;
    virtual float lengthFromScreen(float len) = 0;

    virtual HOM_Vector2 sizeToScreen(const HOM_Vector2 &size) = 0;
    virtual HOM_Vector2 sizeFromScreen(const HOM_Vector2 &size) = 0;

    virtual HOM_Vector2 posToScreen(const HOM_Vector2 &pos) = 0;
    virtual HOM_Vector2 posFromScreen(const HOM_Vector2 &pos) = 0;

    virtual HOM_Vector2 overviewPosToScreen(const HOM_Vector2 &pos) = 0;
    virtual HOM_Vector2 overviewPosFromScreen(const HOM_Vector2 &pos) = 0;
    virtual bool overviewVisible() = 0;
    virtual bool overviewVisibleIfAutomatic() = 0;

    SWIGOUT(%kwargs networkItemsInBox;)
    virtual std::vector<UT_Tuple<HOM_ElemPtr<HOM_NetworkItem>,
				 std::string,
				 int> >
	networkItemsInBox(const HOM_Vector2 &pos1, const HOM_Vector2 &pos2,
		bool for_drop = false, bool for_select = false) = 0;
    virtual void setDropTargetItem(
	    HOM_NetworkItem *item, const char *name, int index) = 0;
    virtual UT_Tuple<HOM_ElemPtr<HOM_NetworkItem>, std::string, int>
	dropTargetItem() = 0;
    virtual void setDecoratedItem(HOM_NetworkItem *item, bool interactive) = 0;
    SWIGOUT(%newobject decoratedItem;)
    virtual HOM_NetworkItem *decoratedItem() = 0;
    virtual bool decorationInteractive() = 0;

    virtual void setPreSelectedItems(
	    const std::vector<HOM_NetworkItem *> &items) = 0;
    virtual std::vector<HOM_ElemPtr<HOM_NetworkItem> > preSelectedItems() = 0;

    virtual void setNetworkBoxPendingRemovals(
	    const std::vector<HOM_NetworkMovableItem *> &items) = 0;
    virtual std::vector<HOM_ElemPtr<HOM_NetworkMovableItem> >
	    networkBoxPendingRemovals() = 0;

    virtual void setFootprints(
	    const std::vector<HOM_NetworkFootprint> &footprints) = 0;
    virtual std::vector<HOM_NetworkFootprint> footprints() = 0;

    virtual void setCursorMap(
	    std::map<std::pair<std::string, int>, std::string> cursors) = 0;
    virtual std::map<std::pair<std::string, int>, std::string> cursorMap() = 0;
    virtual void setDefaultCursor(const char *cursor_name) = 0;
    virtual std::string defaultCursor() = 0;

    virtual void setBackgroundImages(
	    const std::vector<HOM_NetworkImage> &images) = 0;
    virtual std::vector<HOM_NetworkImage> backgroundImages() = 0;

    virtual HOM_BoundingRect itemRect(
	    HOM_NetworkMovableItem *item, bool adjusted=true) = 0;
    virtual HOM_Vector2 itemInputPos(
	    HOM_NetworkMovableItem *item, int input_index, bool adjusted=true) = 0;
    virtual HOM_Vector2 itemInputDir(
	    HOM_NetworkMovableItem *item, int input_index) = 0;
    virtual HOM_Vector2 itemOutputPos(
	    HOM_NetworkMovableItem *item, int output_index, bool adjusted=true) = 0;
    virtual HOM_Vector2 itemOutputDir(
	    HOM_NetworkMovableItem *item, int output_index) = 0;
    virtual std::vector<std::pair<HOM_ElemPtr<HOM_NetworkMovableItem>,
				  HOM_BoundingRect> > allVisibleRects(
	    const std::vector<HOM_NetworkMovableItem *> &ignore_items,
	    bool adjusted = true) = 0;

    SWIGOUT(%kwargs setAdjustments;)
    virtual void setAdjustments(
	    const std::vector<HOM_NetworkItem *> &items,
	    const std::vector<HOM_NetworkAnimValue> &adjustments,
	    bool auto_remove = false) = 0;
    virtual void setShapes(
	    const std::vector<HOM_NetworkShape *> &shapes) = 0;
    virtual void setOverlayShapes(
	    const std::vector<HOM_NetworkShape *> &shapes) = 0;

    virtual void setTooltip(const char *tooltip) = 0;
    virtual std::string tooltip() = 0;
    virtual void setPrompt(const char *prompt) = 0;
    virtual std::string prompt() = 0;
    virtual void flashMessage(const char *image,
	    const char *message, float duration) = 0;

    SWIGOUT(%kwargs openTabMenu;)
    virtual void openTabMenu(const char *key = NULL,
	    bool auto_place = false,
	    bool branch = false,
	    HOM_NetworkMovableItem *src_item = NULL,
	    int src_connector_index = -1,
	    HOM_NetworkMovableItem *dest_item = NULL,
	    int dest_connector_index = -1,
	    HOM_Vector2 *node_position = NULL,
	    const std::vector<HOM_NetworkMovableItem *> &src_items =
		std::vector<HOM_NetworkMovableItem *>(),
	    const std::vector<int> &src_connector_indexes =
		std::vector<int>(),
	    const std::vector<HOM_NetworkMovableItem *> &dest_items =
		std::vector<HOM_NetworkMovableItem *>(),
	    const std::vector<int> &dest_connector_indexes =
		std::vector<int>()) = 0;
    SWIGOUT(%kwargs openNodeMenu;)
    virtual void openNodeMenu(HOM_Node *node = NULL,
	    const std::vector<HOM_NetworkMovableItem *> &items =
		std::vector<HOM_NetworkMovableItem *>()) = 0;
    virtual void openVopEffectsMenu(HOM_VopNode *node, int input_index) = 0;
    virtual void openVopOutputInfoMenu(HOM_VopNode *node, int output_index) = 0;

    virtual int openNameEditor(HOM_NetworkMovableItem *item,
	    bool select_all = false) = 0;
    virtual int openCommentEditor(HOM_NetworkMovableItem *item,
	    bool select_all = false) = 0;
    virtual int openNoteEditor(HOM_StickyNote *stickynote,
	    bool select_all = false) = 0;
    SWIGOUT(%kwargs closeTextEditor;)
    virtual void closeTextEditor(int id, bool apply_changes = true) = 0;

    virtual int scheduleTimerEvent(float seconds) = 0;
    virtual void handleCurrentKeyboardEvent(bool resend = false) = 0;
    virtual void setVolatileKeys(const std::vector<std::string> &keys) = 0;
    virtual bool isVolatileKeyDown(const char *key) = 0;

    virtual void setPref(const char *pref, const char *value) = 0;
    virtual std::string getPref(const char *pref) = 0;
    virtual void setPrefs(const std::map<std::string, std::string> &prefs) = 0;
    virtual std::map<std::string, std::string> getPrefs() = 0;
    virtual void registerPref(const char *pref, const char *value, bool global) = 0;
    virtual std::vector<std::vector<std::string> > badges() = 0;
    virtual std::vector<std::vector<std::string> > textBadges() = 0;

    virtual std::vector<std::string> nodeShapes() = 0;
    virtual std::vector<std::string> reloadNodeShapes() = 0;
    virtual void redraw() = 0;

    virtual void setDragSourceData(
	    const std::vector<HOM_NetworkMovableItem *> &items) = 0;

    SWIGOUT(%ignore pushOpaqueEventContext);
    virtual void pushOpaqueEventContext(const char *module, void *data) = 0;
    virtual void popEventContext() = 0;
    SWIGOUT(%ignore opaqueEventContextData);
    virtual void *opaqueEventContextData() = 0;
#ifdef SWIG
%extend
{
    virtual void pushEventContext(const char *module, InterpreterObject data)
    { return self->pushOpaqueEventContext(module, data); }
    virtual InterpreterObject eventContextData()
    { return HOMincRef((InterpreterObject)self->opaqueEventContextData()); }
}
#endif
};

#endif
