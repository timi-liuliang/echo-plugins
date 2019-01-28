/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Base Direct manipulation library (C++)
 *
 * COMMENTS:
 *	The base class of a general state.
 *
 */

#ifndef __BM_SimpleState_H__
#define __BM_SimpleState_H__

#include "BM_API.h"
#include <UT/UT_UndoManager.h>
#include <UT/UT_UndoWorkerFinder.h>
#include <UI/UI_Keyboard.h>
#include <SI/AP_Interface.h>

class UT_String;
class RE_Render;
class RE_Cursor;
class UI_Feel;
class UI_Menu;
class BM_SceneManager;
class BM_Viewport;

#define BM_STATE	0x00000000
#define BM_HANDLE	0x10000000

class BM_API BM_SimpleState : public AP_Interface
{
public:
    enum BM_EntryType {	BM_FULLTIME_ENTRY	= 0x01,
			BM_OVERLAY_ENTRY 	= 0x02
		      };

    // Class constructor and destructor. 'name' is the name of the state.
    BM_SimpleState(BM_SceneManager &app, const char *cursor, 
		   const char		  *name		= 0,
		   const char      *const *vnames	= 0,
		   UI_EventMethod   const *vmethods	= 0);

    virtual ~BM_SimpleState(void);

    // Returns the icon name and label that should appear in the viewer
    // operation controls bar when we are in this state. The default
    // implementation extracts this information from the state template.
    virtual const char	*getOperationBarIconName() const;
    virtual const char	*getOperationBarLabel() const;
    virtual const char	*getHotkeyString() const;

    // Return the menu feel for others to use:
    virtual UI_Menu	*getStateMenu(int ortho = 0);

    // Return a selector menu (if applicable).
    virtual UI_Menu	*getSelectorMenu();

    // Return an extra RMB menu (if applicable).
    virtual UI_Menu	*getExtraStateMenu();
	
    // Enter or exit this state.
    virtual int 	 enter(BM_EntryType how =
			       BM_SimpleState::BM_OVERLAY_ENTRY) = 0;
    virtual void	 exit (void)			= 0;

    // Interrupt or resume this state:
    virtual void	 interrupt(BM_SimpleState * = 0);
    virtual void	 resume   (BM_SimpleState * = 0);

    // Deal with the events the modeler has sent me. Return 1 if consumed
    // and 0 otherwise.
    virtual int		 handleMouseEvent	(UI_Event *event) = 0;
    virtual int		 handleArrowEvent	(UI_Event *event) = 0;
    virtual int		 handleKeyEvent		(UI_Event *event);
    virtual int		 handleParameterEvent	(UI_Event *event);

    // The state might contain some geometry it wants to display, but may not
    // want it to be part of the main gdp. This method allows the state to
    // "sneak in" this special geometry that it might have (eg. a rubber band
    // when building curves). The feel of the workbench calls this method when
    // it draws itself.
    virtual void	 render(RE_Render *r, int x, int y);

    // Allows the state to draw things into the overlay plane, if it exists.
    virtual void	 overlayRender(RE_Render *r, int x, int y);

    // These two function are used to get the different toolboxes
    // that are defined by the different levels of subclassing.
    //
    // getToolboxCount returns the total number of toolboxes defined
    // by this class (including all the ones defined by it's base classes)
    // and getToolbox is used to query for each of them.
    virtual int		 getToolboxCount() const;
    virtual UI_Feel	*getToolbox(int index) const;

    // Volatile states can also have toolboxes, but these cannot be queried
    // by overriding BM_SimpleState::getToolboxCount()/getToolbox(int index)
    // as the state has not necessarily been entered yet and so we cannot
    // distinguish a volatile instance from a non-volatile one without the
    // caller doing so for us.  Note that it is possible to have both a
    // volatile and a nonvolatile instance of the same state simultaneously,
    // so getVolatileToolbox() should not return the same feel pointer as
    // getToolbox().
    //
    virtual bool	 isOverlay() const;
    virtual int		 getVolatileToolboxCount() const;
    virtual UI_Feel	*getVolatileToolbox(int index) const;

    // The name of this class:
    virtual const char	*className(void) const;

    // Is this simple state a handle?
    virtual int		 isHandle() const = 0;

    void		 setVolatileViewport(BM_Viewport *view)
			    { myVolatileViewport = view; }
    BM_Viewport		*getVolatileViewport() const
			    { return myVolatileViewport; }

    // This needs to be available at the BM_SimpleState level, because
    // when a state is resumed it may query whether it's coming back from
    // a modifier state.  A proper implementation is in BM_State.
    virtual int		 isModifier(void) const { return 0; }

    // Classes derived from us will need to access the sceneManager:
          BM_SceneManager &sceneManager(void)	    { return mySceneManager; }
    const BM_SceneManager &sceneManager(void) const { return mySceneManager; }

    // Query or set the previous state, if any
    void		  setSavedState(BM_SimpleState *state)
			    { myPrevState = state; }
    BM_SimpleState	 *getSavedState()
			    { return myPrevState; }
    const BM_SimpleState *getSavedState() const
			    { return myPrevState; }

    virtual const char	*replaceCursor(const char *newcursor);

    const char *	 getCurrentCursor() const
			    { return myCursor; }

    // Query the viewport mask to determine which viewports we can
    // act in.
    unsigned int	 getViewportMask() const { return myViewportMask; }

    const UT_UndoWorkerFinder<BM_SimpleState> &undoWorkerFinder() const
					    { return myUndoWorkerFinder; }

    // These two functions are provided for all the BM_SimpleState based
    // handles and states that need to manage an undo block that begins in one
    // function but ends in another function (or another instance of the same
    // function).
    //
    // However, they can only handle ONE block at a time. No nesting
    // of calls to beginDistributedUndo!
    void		 beginDistributedUndoBlock(const char *operation,
						   UT_UndoBlockType blocktype,
						   bool ignore_log=false);
    void		 endDistributedUndoBlock(bool ignore_log=false);
    bool                 inDistributedUndoBlock();

protected:
    // handle events coming from state popup menus
    virtual void	 handlePopupMenuEvent(UI_Event *event);

    // handle an event from either the popup menu or the keyboard - perform the
    // action on the specified viewport
    virtual int		 handleKeyTypeEvent(UI_Event *event,
					    BM_Viewport &viewport);

    // Push or pop the cursor if one is defined and if we're running the UI
    void		 pushCursor(void);
    void		 popCursor (void);
    void		 setCursor (void);
    void		 setCursor (const char *cursorname);

    // Allow derived classes to override the cursor set by base classes.
    // Don't confuse this method with setCursor.
    void		 setDefaultCursor(const char *cursorname);

    // Initialize the app, wire the interface, tie all the UI values, etc:
    void		 initializeUI();

    // Do the AP_Interface thing (called by initializeUI()):
    virtual void	 initApplication(UI_Manager *, int c, const char **v);

    // Return the name of the UI file containing the menu, toolbox, or any
    // other UI gadgets the SimpleState may have.
    virtual void	 getUIFileName(UT_String &full_path_and_name) const;

    // Set or get the mouse button. It's a stash we might find useful
    // occasionally.
    void		 mouseDown(UI_Event *event);
    int			 mouseDown(void) const	{ return myMouseDown; }

    // Set the viewport mask for the viewport types we will act in.
    void		 setViewportMask(unsigned mask)
				{ myViewportMask = mask; }

    // set/remove flags that cause handleMouseEvent to be called when mouse
    // buttons are pressed
    int			 getClickInterest();
    void		 addClickInterest(int buttons);
    void		 removeClickInterest(int buttons);

    // redraw the scene
    void		 redrawScene();

    // typically called from the derived class dtor to safely close an open undo block
    void		 closeOpenUndoBlock();

private:
    BM_SceneManager 	&mySceneManager;// The sceneManager I'm a state of.
    const char		*myCursor;	// the name of our cursor.
    BM_SimpleState	*myPrevState;	// The state we're preempting
    BM_Viewport		*myVolatileViewport; // the viewport that events
					     // dealing with menus and other
					     // volatile handles should go to
    UI_Feel		*myMenuGadget;	   // the feel that holds the menu
    UI_Feel		*myOrthoMenuGadget;// the feel that holds the menu to
					   // be used in ortho viewports

    int			myDistributedUndoBlockLevel;

    UI_Feel		*mySimpleToolbox;

    unsigned int	 myViewportMask; // bit mask for viewports in which
					 // we need to handle events.

    int			 myMouseDown; // mouse button down on START or PICKED

    UT_UndoWorkerFinder<BM_SimpleState> myUndoWorkerFinder;

    UT_Array<RE_Cursor*> myCursorStack;
};

#endif
