/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	MSS_SingleOpState.h
 *
 * COMMENTS:
 *	This is an "automated" state that links handles with op
 *	parameters.
 */

#ifndef __MSS_SingleOpState_H__
#define __MSS_SingleOpState_H__

#include "MSS_API.h"
#include <UT/UT_ValArray.h>
#include <UT/UT_String.h>
#include <GU/GU_Selection.h>
#include <UI/UI_Value.h>
#include <UI/UI_Event.h>
#include <BM/BM_ResourceManager.h>
#include "MSS_SingleOpBaseState.h"

class UT_String;
class PRM_Template;
class DM_Workbench;
class OP3D_View;
class JEDI_View;
class OP3D_InputSelector;
class DM_Viewport;

class mss_InputSelectorUndoWorker;
class mss_UndoClearHandles;

class MSS_API MSS_SingleOpState : public MSS_SingleOpBaseState
{
    friend class mss_UndoClearHandles;
    friend class mss_UndoCreateHandles;
    friend class mss_InputSelectorUndoWorker;

public:
    // Class constructor and destructor.
	     MSS_SingleOpState(JEDI_View &view, PI_StateTemplate &templ,
			       BM_SceneManager *scene, const char *cursor);
    virtual ~MSS_SingleOpState(void);

    // This constructor and parameter template list go into the
    // DM_StateTemplate for this state.
    static BM_State	*ourConstructor(BM_View &view, PI_StateTemplate &templ,
					BM_SceneManager *scene);
    static PRM_Template	*ourTemplateList;

    // The name and type of this class:
    virtual const char	*className() const;

    // Enter or exit this state.
    virtual int		 enter(BM_SimpleState::BM_EntryType how);
    virtual void	 exit();
    virtual void	 restart(void);
    virtual int		 generate(BM_SimpleState::BM_EntryType how,
				  bool insertmode = true,
				  bool requestnew = false);

    virtual int		 hasLocates(void) const;

    // Check if we have to do anything if this node is deleted.  Return -1 if
    // we can't handle it, 0 otherwise.
    virtual int		 handleNodeDeleted(OP_Node &node);

    // Called whenever the current detail look, or the geometry in it, changes.
    // See needsGeoChangedInterest() to control when this interest is added.
    virtual void	 handleGeoChangedEvent(UI_Event *);

    // If a state generates a sop and displays it (eg. a hide sop), it
    // will want the state controller to ignore the change so the state
    // doesn't exit.
    virtual bool	 ignoreDisplayFlagChange() const;

    void		 undoReselecting();

    // Return the feel for the RMB menu.
    virtual UI_Menu	*getSelectorMenu();

    // Return the menu feel for others to use:
    virtual UI_Menu	*getExtraStateMenu();
    
    // Methods to query whether or not this state is allowed to change the
    // current selection type to match the cook selection of our node when
    // the corresponding global user preference allows it.  Defaults to false
    // for all sub-classes and true for instances created by this class's
    // ourConstructor().
    bool		 getAllowExportingCookSelectionType() const
			    { return myAllowExportingCookSelectionType; }
    void		 setAllowExportingCookSelectionType(bool on)
			    { myAllowExportingCookSelectionType = on; }

    // Returns true when this state allows changing the current selection type
    // to match that of the cook selection and the corresponding global user
    // preference allows it to do so.
    bool		 canExportCookSelectionType() const;

    class PrefTokens;

protected:
    // Quickly generate a node, connect it to the network if needed,
    // and stop generating. This method is called from generate().
    virtual void	 doGenerate(void);

    // calls the selector render, if any
    virtual void	 doRender(RE_Render *r, int, int, int ghost);

    // Respond to mouse and keyboard events.
    virtual int		 handleMouseEvent(UI_Event *event);
    virtual int		 handleMouseWheelEvent(UI_Event *event);
    virtual bool	 handleDoubleClickEvent(UI_Event *event);
    virtual int		 handleKeyTypeEvent(UI_Event *event, BM_Viewport &);
    virtual bool	 handleTransitoryKey(const UI_Event &event);

    // This depends on the secure selection button and 
    // whether a selector is active.
    virtual bool	 isSelecting(void) const;

    // Interrupt this state or resume its activity. These methods should
    // be called by overlay states when entering and exiting respectively. 
    // Interrupt will leave the state in limbo and will usually turn off
    // the feed from the app; resume will do the opposite. interrupt()
    // should not change the modeler's current state. If 'state' is given, it
    // is the state that has interrupted/resumed us. This may be particularly
    // useful if we are interrupted by a handle (possibly ours).
    virtual void	 interrupt(BM_SimpleState *state = 0);
    virtual void	 resume(BM_SimpleState *state = 0);

    // Generate the necessary nodes and connect them
    virtual bool	 generateAllNodes();

    // We're done selecting and we've been successful. In this class we simply 
    // clear the display.
    virtual void	 doneSelecting(void);

    // If this state is reselecting and it tries to reselect within the same
    // node then that node's unique id is returned.  Otherwise, 0 is returned.
    virtual int		 nodeIdToReselectWithin() const { return 0; }

    // Access to the list of selectors
    OP3D_InputSelector	*selector(int idx)
			 {
			     UT_ASSERT(idx >= 0);
			     UT_ASSERT(idx < mySelectors.entries());
			     return mySelectors(idx).mySelector;
			 }
    int			 getNumSelectors() const
			 {
			     return mySelectors.entries();
			 }

    virtual bool	 isSelectorActive() const
			 { return (myCurrentSelector != 0); }

    virtual int		 getToolboxCount() const;
    virtual UI_Feel 	*getToolbox(int idx) const;

    virtual void	 handleSelectorDone(UI_Event *);

			 // Override in subclasses to disable selector starting
			 // by this class.
    virtual bool	 canStartSelectors() const	    { return true; }

    virtual const PI_BindingSelectorInfo *findNextParmWithSelector(int idx) const;

    virtual void	 addAnyReselectionUndos();

    virtual void	 updateCurrentSelector(OP3D_InputSelector * /*cur_sel*/,
			    const PI_BindingSelectorInfo * /*selector_info*/)
			 { }

    virtual bool	 getSelectionBoundingBox( DM_Viewport &vport,
						  UT_BoundingBox &bbox );

    // Override this method to return false if you don't want this base class
    // to start a non-secure selector on you.  Typically a subclass will do
    // this when it wants to inherit the bound selector behavior from this
    // base class, but wants to handle its own selections after that.
    virtual bool	 canStartNonSecureSelector() const { return true; }

    virtual bool	 getAllowQuickSelect() const	{ return false; }

    // Override this method to return false if you don't want this base class
    // to copy the cooked selection when starting the first selector.
    virtual bool	 copyCookedSelectionForFirstSelector() const
			 { return true; }

    // Override this method to return false if you don't want this base class
    // to create myHotkeySelector and myHotkeySelectorMenu.
    virtual bool	 usesHotkeySelector() const { return true; }

    virtual bool	 usesSloppySopSelectorToolbox() const { return false; }

    virtual const char	*getTemplateName() const
			    { return (const char *)getTemplate().name(); }

    virtual const char	*getOperatorKey() const
    { 
	return getTemplateName(); 
    }

    bool		 getFirstSelectionFlag() const
			    { return myFirstSelectionFlag; }
    void		 setFirstSelectionFlag(bool first)
			    { myFirstSelectionFlag = first; }

    virtual void	 handleSelectionTypeChange(UI_Event *event);

    void		 updateStashedSelectorActiveCoords(UI_Event *event);

    void		 createHotkeySelector();
    void		 destroyHotkeySelectorIfNeeded();

    // This base class will add an interest in DM_SceneManager::newGeoValue()
    // on enter()/generate() if this method returns true.  The virtual method
    // handleGeoChangedEvent() handles events from this value.
    virtual bool	 needsGeoChangedInterest() const;

private:
    // Append the specified node to the currently displayed sop.
    void		 appendNodeToDisplay(OP_Node &newnode);

    bool		 usesSelectors(); // sets mySelectableFlag

    void		 deleteSelectors();
    void		 setCurrentSelector(const PI_BindingSelectorInfo *
								selector_info);
    void		 clearReselectLists();

    // When reselecting, store the selections on the input nodes.
    void		 storeSelectionsOfInputNodes();
    // Restore the selections on our inputs from the information stored above.
    void		 restoreCurrentSelectorSelections();

    GA_GroupType	 getSelectionTypeForCurNode();
    
    /// Given a selection string, attempt to figure out which of the selector-
    /// assigned group types best apply, in order of declaration.
    GA_GroupType	 getSelectionTypeFromString(const char *sel_str) const;

    void		 startNonSecureSelector();
    void		 undoStartNonSecureSelector();

    void		 finishSelector();

    bool		 isRapidFire() const;

    void		 handleFinishUndoEvent(UI_Event *event = 0);

    // This class contains information about the input field to which a
    // selector is bound.
    class MSS_API MSS_SelectorBind
    {
    public:
	MSS_SelectorBind()
	    : mySelector(0), mySelectorInfo(0)
	    { }
	~MSS_SelectorBind()
	    {
		// It's possible that the resource manager doesn't exist
		// if we're being deleted because of an exit callback.
		if( mySelector && PIresourceManagerExists() )
		    BMgetResourceManager()->deleteSelector(
			    (BM_InputSelector *)mySelector);
	    }
	OP3D_InputSelector		*mySelector;
	const PI_BindingSelectorInfo	*mySelectorInfo;
    };

    // info about selectors
    UT_Array<MSS_SelectorBind>		 mySelectors;
    OP3D_InputSelector			*myCurrentSelector;
    const PI_BindingSelectorInfo	*myCurrentSelectorInfo;

    // Selector used for selector hotkeys when in quickselect mode and 
    // secure selection is turned off.
    OP3D_InputSelector			*myHotkeySelector;
    UI_Menu *				 myHotkeySelectorMenu;

    // The selector index is the index of the current selector in the list of
    // selectors provided for this state.  It coincides with the index into
    // the gdp's set of temporary selections.
    int					 mySelectorIndex;

    UT_ValArray<UT_StringArray *>	 myReselectPathLists;
    UT_StringArray			 myReselectSelectionStrings;
    UT_Array<GA_GroupType>	 	 myReselectSelectionTypes;

    UI_Value				 mySelFinishedValue;

    float				 mySelectorActiveCoords[2];
    UI_Event				 myRapidFireActiveEvent;
    UI_Value				 myRapidFireActiveEventValue;

    mss_InputSelectorUndoWorker *	 mySelectorUndoWorker;

    int					 mySelectableFlag; // uses selectors?
    unsigned				 myMouseTakenFlag:1; // can rapid-fire?
    bool				 myFirstSelectionFlag;
    bool				 myInNonSecureUndo;
    bool				 myInNonSecureSelector;
    bool				 myAllowExportingCookSelectionType;
    bool				 myHasGeoChangedInterest;

    int					 myDoubleClickUndoLevel;
};

/// Essentially a namespace containing static methods for querying various
/// global preference tokens.
class MSS_API MSS_SingleOpState::PrefTokens
{
public:
    // T global user preference for allowing states to change the current
    // selection type to match that of the cook selection.
    static const char	*exportCookSelectionType();
};

#endif
