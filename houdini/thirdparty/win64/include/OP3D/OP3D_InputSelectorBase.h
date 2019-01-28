/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        OP3D_InputSelectorBase.h
 *
 * COMMENTS:
 *	This descendant of DM_InputSelector knows about the viewer.  It uses
 *	the viewer to find out display/current sop and to use the viewer's
 *	selection type buttons.
 */

#ifndef __OP3D_InputSelectorBase_h__
#define __OP3D_InputSelectorBase_h__

#include "OP3D_API.h"

#include <DM/DM_InputSelector.h>
#include <DM/DM_Defines.h>
#include <GR/GR_PickRecord.h>
#include <GU/GU_SelectType.h>
#include <UI/UI_Value.h>
#include <UT/UT_String.h>
#include <GA/GA_Types.h>

class SIM_Data;
class SIM_Object;
class GU_Detail;
class OP_Node;
class PI_SelectorTemplate;
class UI_Menu;
class UI_Feel;
class UI_Event;
class SI_RubberBox;
class SI_Lasso;
class SI_Brush;
class DM_Viewport;
class DM_Workbench;
class GUI_DetailLook;
class OP3D_View;

enum OP3D_SelectionAction
{
    OP3D_SELECT_ALL,
    OP3D_INVERT_SELECTION,
    OP3D_SELECT_NONE
};

class OP3D_PickId
{
public:
		 OP3D_PickId();
		 OP3D_PickId(int lookindex, int detailindex);
		~OP3D_PickId();

    bool	 operator==(const OP3D_PickId &cmp) const;
    bool	 operator!=(const OP3D_PickId &cmp) const;
    static int	 compare(const OP3D_PickId *id1, const OP3D_PickId *id2);

    int	 myLookIndex;
    int	 myDetailIndex;
};

typedef UT_Array<OP3D_PickId> OP3D_PickIdArray;
typedef UT_Array<GU_SelectionHandle> OP3D_SelectionHandles;

class OP3D_API OP3D_InputSelectorBase : public DM_InputSelector
{
public:
	     OP3D_InputSelectorBase(OP3D_View &viewer,
				    PI_SelectorTemplate &templ);
    virtual ~OP3D_InputSelectorBase();

    // This method allows the resource manager can set the workbench to
    // something different if a selector is reused.  It should not be used
    // otherwise.  A nil pointer is allowed so the select can remove any
    // of its interests.  However, methods of the selector should never be
    // called when its workbench is nil.  The method is virtual so
    // descendants can also have a chance to clean up.
    //
    // Note that setSelectionUIValues must be called after calling setViewer()
    virtual void	 setViewer(BM_View *viewer);

    // The selector needs to be notified when it starts/stops selecting
    // so it can add/remove interests (the selection type, etc.).  
    virtual void	 startSelecting();
    virtual void	 stopSelecting();

    // Use this opportunity to render prior object selections.
    virtual void	 doRender(RE_Render *r, int x, int y, int ghost);

    // This UI value changes when the selector has finished selecting.  The
    // state that owns the selector adds an interest in this event to know
    // when the selector is finished.
	  UI_Value	&finishedValue()
			 { return *myFinishedValue; }
    const UI_Value	&finishedValue() const
			 { return *myFinishedValue; }

    virtual UI_Feel	*getCustomToolbox() const;

    void		 setPreferredSelectMode(DM_SelectMode selmode)
			 { myPreferredSelectMode = selmode; }
    DM_SelectMode	 getPreferredSelectMode() const
			 { return myPreferredSelectMode; }

    // If the selector doesn't allow multiple selection, then at most one
    // object will appear in the pickedNodes() array.
    bool		 allowDragging() const
			 { return myAllowDragging; }
    void		 setAllowDragging(bool on)
			 { myAllowDragging = on; }

    // Some states (like the select state) don't ever want their selector
    // to finish.
    bool		 allowFinishing() const
			 { return myAllowFinishingFlag; }
    void		 setAllowFinishing(bool allow_finishing)
			 { myAllowFinishingFlag = allow_finishing;}

    // Some selector can finish with whatever selection exists already.
    bool		 allowFinishWithExistingSelection() const;
    void		 setAllowFinishWithExistingSelection(bool allow);

    // If quick select is on, then the selection is immediately finished
    // if we have secure selection off
    bool		 allowQuickSelect() const
			 { return myAllowQuickSelect; }
    void		 setAllowQuickSelect(bool on)
			 { myAllowQuickSelect = on; }

    /// Tells the selector about previous selection information that we still
    /// want to display in the viewport, but which is not part of the current
    /// selection operation.
    void		 setPriorSelections(const UT_StringArray &paths,
				const UT_StringArray &ids,
				const OP3D_SelectionHandles &selections);

    // Did the selector just change the display flag?  The selector sets
    // this flag when it creates a sop (eg. a hide or a group sop) and
    // sets its display flag.  The state will check this flag when the state
    // controller asks it if it just changed the display flag.  This way,
    // the selector can change the display flag without exiting the current
    // state.
    bool		 justDisplayedOp() const
			    { return myJustDisplayedOpFlag; }
    void		 setJustDisplayedOp(bool on)
			    { myJustDisplayedOpFlag = on; }

    // Selectors resize the brush radius with the mouse wheel when appropriate.
    virtual int		 handleMouseWheelEvent(UI_Event *event);

    // Handle transitory keys to change the mouse cursor according to our
    // selection mode.
    virtual bool	 handleTransitoryKey(const UI_Event &event);

    virtual const char	*cursor() const;

    void		 handleSelectionStyleChange(UI_Event *event);

    // Methods for setting user preferences
    static bool		 getPickingMenuOn()	    { return thePickingMenuOn; }
    static void		 setPickingMenuOn(bool val) { thePickingMenuOn = val;  }
    static bool		 getAllowUseExistingSelection()
			    { return theAllowUseExistingSelection; }
    static void		 setAllowUseExistingSelection(bool val)
			    { theAllowUseExistingSelection = val; }
    static bool		 getSelectFullLoops()
			    { return theSelectFullLoops; }
    static void		 setSelectFullLoops(bool val)
			    { theSelectFullLoops = val; }


    // Converts a string to a GU_Detail pointer. The string will either be
    // an object or sop path, or a path to a specific DOP simulation data.
    static GU_Detail		*getGeometryFromPath(const char *path);
    static OP_Node		*getNodeFromPath(const char *path);
    static const SIM_Object	*getSimObjectFromPath(const char *path);
    static const SIM_Data	*getSimModifierFromPath(const char *path);

    // A utility method to get the string representation of the loop selection
    // hotkey, if any is set.
    static const char		*getLoopSelectionHotkeyStringRepr();

    // A utility method to determine if a particular transitory key is bound
    // as a loop selection hotkey.
    static bool			 isTransitoryLoopSelectionHotkey(
						const UI_DeviceEvent &state);

    // A utility method to determine if the loop selection hotkey is pressed
    // in the specified viewport.
    static bool			isTransitoryLoopSelectionHotkeyPressed(
						DM_Viewport *viewport);
    
    // A utility method to show the loop selection hotkey message in the given
    // workbench.
    static void			 showLoopSelectionMessage(
						    DM_Workbench &workbench);

    virtual bool getStateParmNames(UT_StringArray &ret, const char* prefix=nullptr );
    virtual bool evalStateParm(const char *name, UT_StringHolder &ret);
    virtual bool setStateParm(const char *name, const UT_StringHolder &val);
    virtual bool pressStateButton(const char *name);

protected:
	  OP3D_View	&viewer()	   { return (OP3D_View &)baseViewer(); }
    const OP3D_View	&viewer() const	   { return (OP3D_View &)baseViewer(); }

	  DM_Workbench  &workbench()	   { return *myWorkbench; }
    const DM_Workbench	&workbench() const { return *myWorkbench; }

    void		 setFinishedValuePtr(UI_Value *new_value)
			    { myFinishedValue = new_value; }
    UI_Value *		 getFinishedValuePtr() const
			    { return myFinishedValue; }

    void		 setSelectionStylePtr(UI_Value *new_value)
			    { mySelectionStyle = new_value; }
    UI_Value *		 getSelectionStylePtr() const
			    { return mySelectionStyle; }
    DM_SelectionStyle	 selectionStyle() const
			    {
				if (mySelectionStyle)
				    return (DM_SelectionStyle)
							(int)*mySelectionStyle;
				else
				    return DM_STYLE_UNKNOWN;
			    }
    void		 setVisiblePickPtr(UI_Value *new_value)
			    { myVisiblePickValue = new_value; }
    UI_Value		*getVisiblePickPtr() const
			    { return myVisiblePickValue; }
    bool		 getVisiblePick() const
			    {
				if(myVisiblePickValue)
				    return (bool) *myVisiblePickValue;
				return true;
			    }
    
    void		 setContainedPickPtr(UI_Value *new_value)
			    { myContainedPickValue = new_value; }
    UI_Value		*getContainedPickPtr() const
			    { return myContainedPickValue; }
    bool		 getContainedPick() const
			    {
				if(myContainedPickValue)
				    return (bool) *myContainedPickValue;
				return false;
			    }

    void		 setSelectionRulePtr(UI_Value *new_value)
			    { mySelectionRule = new_value; }
    UI_Value *		 getSelectionRulePtr() const
			    { return mySelectionRule; }
    GU_SelectionRule	 selectionRule() const
			    {
				if (mySelectionRule)
				    return (GU_SelectionRule)
							(int)*mySelectionRule;
				else
				    return GU_ReplaceSelect;
			    }
    void		 selectionRule(GU_SelectionRule srule)
			    {
				if (mySelectionRule)
				    *mySelectionRule = (int)srule;
			    }

    SI_Brush *		 getPickBrushPtr() const    { return myPickBrush; }
    SI_Brush *		 getPickLaserPtr() const    { return myPickLaser; }
    SI_Lasso *		 getPickLassoPtr() const    { return myPickLasso; }


    // returns true if box, lasso or paint selection is active.
    bool		 isDragSelectActive() const;
    
    // Returns true if the selector needs to make an object selection because
    // the viewer isn't currently pointed at the right network level.
    virtual bool	 needObjectPick() const;

    // Checks whether our selector knows how to handle selections in the
    // context of the current chosen op.
    bool		 selectModeMatchesTemplateType();

    // Given a pick buffer (where the first entry of each pick is an op
    // node id), set the viewer chosen op to the first picked node.
    bool		 pickChosenOp(UT_Array<GR_PickRecord> &pick_records,
				      bool *changed = nullptr);

    // Given a pick record, return the GU_Detail that it refers to, and
    // (optionally), the corresponding detail look.
    const GU_Detail	*getGeometryFromPickRecord(const GR_PickRecord &pick,
					   GUI_DetailLook **detail_look = nullptr);

   // Make the specified node into the chosen op for the viewer. Used by
    // pickChosenOp. Made the function virtual so subclasses can do extra
    // work once the switch is complete. Returns the new chosen op.
    // Sets @c changed to @c true if the operator chosen is different from
    // the one that was selected going in.
    virtual OP_Node	*switchToChosenOp(OP_Node *parent, 
					  bool *changed = nullptr);

    // Save our state for undo.
    virtual void	 saveForUndo();

    // Given a pick number (the first number in a pick buffer), return
    // the name in name. Returns true if the pick number was able to
    // be associated with a name, false otherwise.
    virtual bool	 getPickName(const OP3D_PickId &pickid,
				     UT_String &name,
				     bool descriptivename) const = 0;

    // Enable box picking or lasso picking, depending on what's currently
    // selected.
    void		 enablePicker(UI_Event *event);
    void		 disablePicker();

    void		 disableActivePicking() { myActivePicking = false; }
    bool		 isActivePicking() const { return myActivePicking; }

    virtual void	 handleBoxPick(UI_Event *event);
    virtual void	 activeBoxPick(UI_Event *event);
    virtual void	 handleLassoPick(UI_Event *event);
    virtual void	 activeLassoPick(UI_Event *event);
    virtual void	 handleBrushPick(UI_Event *event);
    virtual void	 activeBrushPick(UI_Event *event);
    virtual void	 handleLaserPick(UI_Event *event);
    virtual void	 activeLaserPick(UI_Event *event);
    bool		 visibleSelectToggle(UI_Event *event,
					     const char *echo_hotkey);
    bool		 containedSelectToggle(UI_Event *event,
					       const char *echo_hotkey);

    // Gets the current time from the OP_Director.
    static fpreal	 getTime();

private:
    DM_Workbench		*myWorkbench;

    UI_Value			*myFinishedValue;
    UI_Value			*mySelectionStyle;
    UI_Value			*myVisiblePickValue;
    UI_Value			*myContainedPickValue;
    UI_Value			*mySelectionRule;

    SI_RubberBox		*myPickBox;	// for box selection
    SI_Lasso			*myPickLasso;	// for lasso selection
    SI_Brush			*myPickBrush;	// for brush selection
    SI_Brush			*myPickLaser;	// for laser selection

    DM_SelectMode		 myPreferredSelectMode;
    bool			 myAllowDragging;
    bool			 myAllowFinishingFlag;
    bool			 myAllowFinishWithExistingSelection;
    bool			 myAllowQuickSelect;
    bool			 myJustDisplayedOpFlag;
    bool			 myActivePicking;

    UT_StringMap<UT_IntArray>	 myPriorObjSelections;
    bool			 myCreatedPriorSelections;
 
    static bool			 thePickingMenuOn;
    static bool			 theAllowUseExistingSelection;
    static bool			 theSelectFullLoops;
};

#endif
