/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Direct manipulation library (C++)
 *
 * COMMENTS:
 *	The base class of a BM state.
 *
 */
#ifndef __BM_State_H__
#define __BM_State_H__

#include "BM_API.h"
#include "BM_ParmState.h"
#include <UI/UI_Keyboard.h>
#include <UT/UT_Array.h>


class UT_Options;
class UT_StringHolder;
class UT_Undo;
class PI_StateTemplate;
class OP_Node;
class OP_Network;
class BM_OpHandleLink;

class BM_API BM_StateFlags
{
public:
    BM_StateFlags()   {
		sticky		= 1; // must be sticky by default (obsolete)
		entered		= 0;
		scratch		= 0;
		requestnew	= 0;
		preempted	= 0;
		busybuild	= 0;
		wantlocate	= 0;
		modifier	= 0;
		concealed	= 0;
    }
    unsigned	sticky:1,	// does not pop back after an operation
		entered:3,	// the way it was entered (BM_EntryType)
		scratch:3,	// entered from scratch
		requestnew:1,	// a new node is requested when generating
		preempted:1,	// have I been interrupted or not
		busybuild:1,	// I'm in the building stage (not editing)
		wantlocate:1,	// want to get locate events
		modifier:1,	// changes geometry, etc or not?
		concealed:1;	// are we concealed in a non-visible desktop?
};

class BM_API BM_State : public BM_ParmState
{
public:
    enum BM_GenerateMode {	BM_INLINE	= 0x01,
				BM_OUTLINE 	= 0x02,
				BM_REGENERATE 	= 0x04
			 };

    // Class constructor and destructor.
	     BM_State(	BM_SceneManager &app,
			PI_StateTemplate &templ,
			const char     		*cursor		= 0,
			const char	*const	*vnames		= 0,
			UI_EventMethod	 const	*vmethods	= 0);
    virtual ~BM_State(void);

    // Return the current status string for this state. This is used by the
    // viewport rendering code to display some extra information in the
    // viewport instead of in the blue status bar. If this string is
    // returned empty (the default), it is ignored.
    virtual void	 status(UT_String &s) const	{ s.harden(""); }

    // Render whatever extra things may be needed beside the status string.
    // You can assume that translations to that location have been performed
    // already (i.e., you can render without moving).
    virtual void	 renderStatusExtras(RE_Render *) {}

    virtual int		 isHandle() const	{ return 0; }

    // Enter or exit this state. There are 2 entry types:
    // 1. fulltime (BM_FULLTIME_ENTRY): the state stays on until another one
    //      is invoked; that other state replaces the current.
    // 2. overlay  (BM_OVERLAY_ENTRY): also known as "volatile", it gets
    //      triggered by pressing a key and stays on until the key is released.
    //      Upon exiting it returns control to the previous state. It is not
    //      allowed to overlay the current state with itself or to have
    //      nested overlay states. When starting the overlay state, the 
    //      previous state is _interrupted_, not exited; then, it is _resumed_.
    // enter() returns 0 if entered and -1 if not.
    virtual int 	 enter(BM_SimpleState::BM_EntryType how);
    virtual void	 exit (void);


    //
    // When the current desktop is changed, it's possible that the state might
    // have to clean up some stuff.  The concealed() method is called when the
    // desktop is changed, the revealed() method when the desktop is re-opened.
    // Note, it may be possible to get a revealed() called without having the
    // concealed called.
    //
    virtual void	concealState();
    virtual void	revealState();

    // Interrupt this state or resume its activity. These methods should
    // be called by overlay states when entering and exiting respectively. 
    // Interrupt will leave the state in limbo and will usually turn off
    // the feed from the app; resume will do the opposite. interrupt()
    // should not change the modeler's current state. If 'state' is given, it
    // is the state that has interrupted/resumed us. This may be particularly
    // useful if we are interrupted by a handle (possibly ours).
    virtual void	 interrupt(BM_SimpleState *state = 0);
    virtual void	 resume   (BM_SimpleState *state = 0);

    // Pre-process a mouse start event, in case we wish to push/position a
    // handle so the mouse event is processed by the handle.
    virtual int		 preprocessSelect(UI_Event *event);

    // Deal with the events the modeler has sent me. Scene events imply use
    // of the mouse. Return 1 if consumed and 0 otherwise. 
    virtual int		 handleMouseEvent (UI_Event *event);

    // This is where mouse wheel events get sent.
    // Integer deltas are stored into event->state.values[Z] where 1 step is a
    // multiple of 100. Positive values indicates that the wheel rotated
    // forwards away from the use, while negative values are the opposite.
    // Return 1 if consumed and 0 otherwise. 
    virtual int		 handleMouseWheelEvent(UI_Event *event);

    // Return false if un-handled.
    virtual bool	 handleDoubleClickEvent(UI_Event *);

    virtual int		 handleArrowEvent (UI_Event *event);

    // Called whenever the geometry changed (but not when the change was
    // caused by modelling in this state)
    virtual void	 handleGeoChangedEvent(UI_Event *);

    // Called by the viewport to allow the state to render its own bits.
    // It calls doRender(), which is what derived classes should implement.
    virtual void	 render(RE_Render *r, int x, int y);

    // The state might contain something that it wants to display in a
    // non-destructive manner (eg. by xor'ing).  Note that this method is not
    // called during the normal rendering of the workbench.
    virtual void	 renderPartialOverlay(RE_Render *r, int x, int y);

    // Similar to enter, but totally from scratch. At this level, it's the
    // same as doing an enter. It cannot be entered in volatile mode.
    // We can start generating either in inline (insert) mode
    // or in branching-off mode. The generation mode is relevant only when
    // generating ops in a network. Some states reuse existing ops when
    // possible instead of generating new ones.  A request can be made for
    // new ops here, but it is up to the individual state to interpret it.
    virtual int 	 generate(BM_SimpleState::BM_EntryType how,
				  bool insertmode = true,
				  bool requestnew = false);

    // Start or stop the generation process. At this level, generating doesn't
    // mean a thing. We can start generating either in inline (insert) mode
    // or in branching-off mode. The inline mode is the default. The mode
    // is relevant only when generating ops in a network.
            void	 startGenerating(BM_State::BM_GenerateMode how =
							BM_State::BM_INLINE,
					 bool requestnew = false)
			    {
				myFlags.scratch = how;
				myFlags.requestnew = requestnew;
			    }
    virtual void	 stopGenerating (void)
			    {
				myFlags.scratch = 0;
				myFlags.requestnew = 0;
			    }

    // Restart cancels what is currently being done and starts generating anew.
    virtual void	 restart(void);

    // Check if the state will allow a restart to be triggered by a given key
    // event.  Override this to return false if you do not want this key to
    // trigger a restart.
    virtual bool	 allowRestartWithKey(const UI_Event &event) const;

    // Also, find out whether you meet the conditions to be entered, or simply 
    // if the given key matches (one of) yours:
            int		 meetsEntryConditions (const UI_Event &event) const;
    virtual int		 isEntryCondition     (UI_Keyboard key) const; 

    // Override this to handle transitory key presses, default implementation
    // does nothing. Return true if key was handled. The key is in
    // event.state.id and the keypress state is in event.state.data.
    virtual bool	 handleTransitoryKey(const UI_Event & /*event*/)
			    { return false; }

    // Called if the state needs to handle changes in op parameters
    virtual int		 hasOpNode(const OP_Node &/*node*/) const;
    virtual void	 handleOpNodeChange(OP_Node &/*node*/);
    virtual void	 handleOpUIChange(OP_Node &/*node*/);
    virtual void	 handleOpNetChange(OP_Network &/*net*/);
    virtual void	 handleOpNetClear();

    // See how you can handle a node being deleted.  If it's an implicit
    // node we might be OK.  Return 0 if we can handle it, -1 if we must be
    // exited or restarted from scratch.
    virtual int		 handleNodeDeleted(OP_Node &/*node*/);

    // Set or get the "locate" flag, which determines whether we're interested
    // in locate events or not.
    void		 wantsLocates(int yesNo) { myFlags.wantlocate=yesNo; }
    virtual int		 hasLocates()   const{ return myFlags.wantlocate;}
    virtual bool	 doesHandleLocates() const { return false; }

    // Query some of the flags:
    int		 	 isPreempted() const	{ return myFlags.preempted;  }
    int		 	 isBuilding () const	{ return myFlags.busybuild;  }
    int		 	 isEntered(void)  const	{ return myFlags.entered;    }
    bool		 isConcealed(void) const{ return myFlags.concealed;  }

    int			 isGenerating() const   { return myFlags.scratch;    }
    int			 isGeneratingInline() const
			 { return myFlags.scratch==BM_State::BM_INLINE;      }
    bool		 isRequestingNew() const
			 { return myFlags.requestnew; }
    int			 isRegenerating() const
			 { return myFlags.scratch==BM_State::BM_REGENERATE;  }

    virtual bool 	 isOverlay() const
			 { 
			     return (myFlags.entered & 
				     BM_SimpleState::BM_OVERLAY_ENTRY) != 0;
			 }
    int			 isFullTime(void) const
			 {
			     return myFlags.entered &
				    BM_SimpleState::BM_FULLTIME_ENTRY;
			 }
    virtual int		 isModifier(void) const	{ return myFlags.modifier; }

    // If a state generates a sop and displays it (eg. a hide sop), it
    // will want the state controller to ignore the change so the state
    // doesn't exit.
    virtual bool	 ignoreDisplayFlagChange() const { return false; }

    // Return a pointer to the feel containing the extra buttons of the state.
    // If a state has no extra buttons, 0 will be returned.
    virtual int		 findPI(BM_OpHandleLink *pi) const;

    // Find out if the state is inherently sticky whether entered FULLTIME or
    // not. Non-sticky (ie oneTime) states pop back to the view state after
    // they've completed an operation. A state is sticky by default.
    int		 	 isSticky(void) const { return myFlags.sticky; }

    // Check if MMB can be used for indirect handle drags.  Some states use
    // the MMB themselves.
    virtual bool	 getAllowIndirectHandleDrag() const { return true; }

    // Checks if and how an RMB menu can be popped up. Usually, when the state 
    // is in building mode, the RMB menus are not allowed (since RMB is used
    // to complete building/drawing/selection).
    enum BM_RMBMenuPopupMode
    {
	BM_RMB_MENU_ALLOWED,	// show the menu right away
	BM_RMB_MENU_DELAYED,	// wait and show menu if mouse was not dragged
	BM_RMB_MENU_DENIED	// don't show the menu at all
    };
    virtual BM_RMBMenuPopupMode	 getRMBMenuPopupMode(short altflags) const;

    // Return this state's index in the list of states:
    int			 index(void) const	{ return myIndex; }
    void		 index(int idx)		{ myIndex =  idx; }

    // The name of this class:
    virtual const char	*className(void) const;
    
    const PI_StateTemplate &getTemplate() const { return myTemplate; }
	  PI_StateTemplate &getTemplate()	{ return myTemplate; }

    // For most states, op dependency is determined entirely by the state
    // template.  It is possible, however, for the state instance itself
    // to dynamically change whether or not it is op dependent on the fly.
    // Any state changing its op dependency in this fashion must notify the
    // appropriate BM_OpStateControl by calling its updateOpDependence()
    // method.  In general, getTemplate().opIndependent() should be false
    // for such states to allow use of switchToOpDependent().
    virtual int		 isOpIndependent() const;

    // Normally, when a user wants to switch to a node's default state, say
    // by hitting BM_KEY_ACCEPT, when already in that state, we have to use
    // a new instance whenever the current instance is not op dependent.
    //
    // These methods make it possible to override that behavior and change
    // the current instance to be op dependent.
    //
    // NB: Only used when !getTemplate().opIndependent() && isOpIndependent(),
    //     i.e., it is registered as an op dependent state that is currently
    //     not op dependent, and hasOpNode() returns true for the target node.
    virtual bool	 canSwitchToOpDependent() const { return false; }
    virtual void	 switchToOpDependent();

    // Returns the icon name and label that should appear in the viewer
    // operation controls bar when we are in this state. The default
    // implementation extracts this information from the state template.
    virtual const char	*getOperationBarIconName() const;
    virtual const char	*getOperationBarLabel() const;
    virtual const char	*getHotkeyString() const;

    virtual void	 afterUndo();

    // The user can dynamically change the bindings from the textport
    // We need to refresh the current handles in the viewport because
    // they could be affected.
    virtual void 	 refreshBindings(int id, const char *op_type);
    // This method is called reload the stored settings of any attached
    // PIs when these settings may have been changed.
    virtual void 	 refreshSettings(int id, const char *op_type);

    virtual const char	*replaceCursor(const char *newcursor);

    // Return the help for this state in the string that's passed in. 
    // (Note that custom states may override getHelp() to
    // provide help, even though there is no actual help file.)  is_html
    // will be set to indicate if the help is in html or not.
    // NOTE: The help file may contain unexpanded hotkey variables.
    virtual void	 getHelp(UT_String &help_text, bool &is_html);

    // Find all help file information, see FUSE:openHdoxURL().
    virtual void	 getHelpDirAndNameOrUrl(UT_StringHolder &dir, 
						UT_StringHolder &name,
						UT_StringHolder &url);

    // Show persistent handles in this state?
    virtual bool	 showPersistent() const { return true; }

    // Return true if we would like to receive an event intended for the
    // click-orienter but was not consumed by it.  All normal states should
    // only want this event if they are overlay states.
    virtual bool	 wantFailedOrienterEvent() const { return isOverlay(); }

    // Show the geometry of the selected operator in this state?
    virtual bool	 getShowSelectedOp() const  { return true; }

    // Reset and clear any remembered state data. Used mainly for states such
    // as view, which would like to home on world origin again, etc.
    virtual void	 resetStateForNew()		{ };

    // A state can override what appears in the select mode side bar of the
    // viewer.
    virtual UI_Feel	*getCustomSelectModeSideBar() const { return 0; }

    // Returns true if the state respects the secure selection setting.
    virtual bool	 useSecureSelection() const { return true; }

    // Get the global state preferences.  Preferences for individual states
    // should be prefixed with "<getTemplate().name()>.".
    static UT_Options	&getGlobalPrefs();

    // returns true if this instance is executing code in a 'busy' mode
    bool isBusyProcessing() const
    {
	return myBusyCounter > 0;
    }

protected:
    friend class Busy;

    // The state might contain some geometry it wants to display, but may not
    // want it to be part of the main gdp. This method allows the state to
    // "sneak in" this special geometry that it might have (eg. a rubber band
    // when building curves). This method is called by DM_State::render().
    virtual void	 doRender(RE_Render *r, int x, int y, int ghost);

    // Similar to doRender() but called on a saved (interrupted) state for
    // states which may want to handle it.
    virtual void	 doRenderSaved(RE_Render *r, int x, int y, int ghost);

   // Make this a state that changes data or not (0 by default):
    void		 setModifier(int yn)	{ myFlags.modifier = yn; }

    // Find out whether the general entry conditions for overlay states
    // are met (before you test for your specific key conditions).
    // The basic conditions are that:
    // - the event type is key up/down
    // - no mouse button is down
    // - we're not the current state
    // - the current state is not and overlay itself
    int			 meetsBasicEntryConditions(const UI_Event &e) const;

    // Set the build flag, ie. whether we're actually building something now
    // or making changes to geometry that is linked to this state.
    virtual void	 setBuild(int onOff);

    void		 handleMouseEventVoidRet(UI_Event *event);

    // Call doRenderSaved on saved states recursively in case they want to
    // handle it.
    void		 renderSaved(RE_Render *r, int x, int y, int ghost);

    // This class is intended for classes deriving from BM_State to execute
    // their code in a 'busy' mode. While the code executes, calling 
    // BM_State::isBusyProcessing() will return true. The 'busy' mode gets reset 
    // when the derived class method using Busy goes out of scope.
    struct Busy
    {
	Busy() = delete;
	
	Busy( BM_State & state ) : myState(state)
	{
	    myState.myBusyCounter++;
	}
	
	~Busy()
	{
	    if (myState.myBusyCounter > 0)
	    {
		myState.myBusyCounter--;
	    }
	}
	
	BM_State& myState;
    };

    BM_StateFlags	 myFlags;	// useful flags

    static bool		 theDelayedSelectionEventIsValid;
    static UI_Event	 theDelayedSelectionEvent;
    
private:
    PI_StateTemplate	&myTemplate;

    UT_Array<int>	myVolatileKeys;// the key used for entry condition
    int			myIndex;	// index in the list of states

    bool		myCursorPushed;
    int			myBusyCounter;
};

#endif
