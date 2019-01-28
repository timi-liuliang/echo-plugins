/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	BM_State.h
 *
 * COMMENTS:
 *	This is an "automated" state that links handles with op
 *	parameters.
 */

#ifndef __BM_OpState_H__
#define __BM_OpState_H__

#include "BM_API.h"
#include <UT/UT_SymbolTable.h>
#include <UI/UI_LookType.h>
#include <PRM/PRM_Template.h>
#include "BM_MoveTool.h"
#include "BM_State.h"

class OP_Node;
class BM_OpView;
class BM_Handle;
class UI_Feel;
class SI_Folders;
class BM_SceneManager;
class OPUI_Dialog;
class PI_OpHandleLink;

class opbm_DialogInfo;		// private to implementation
class opbm_PIContext;		// private to implementation

// NOTE: If you change this default cursor, make sure you update
// RE/RE_Cursor to reflect it.
#define BM_DEFAULT_CURSOR	"iris"

class BM_API BM_OpState : public BM_State
{
public:
    // Class constructor and destructor.
	     BM_OpState(BM_OpView &view, PI_StateTemplate &templ,
			BM_SceneManager *sceneManager,
			const char *cursor = BM_DEFAULT_CURSOR);
    virtual ~BM_OpState(void);

    // Mouse events are passed on to all visible PI's.
    virtual int		 handleMouseEvent (UI_Event *event);
    virtual int		 handleMouseWheelEvent (UI_Event *event);

    // The name and type of this class:
    virtual const char	*className() const;

    virtual void	 prepareDialogForDisplay() const;
    virtual int		 hasDialog() const;
    virtual UI_Feel	*dialogFeel() const;

    virtual int		 findPI(BM_OpHandleLink *pi) const;

    // Called to give any PIs a chance to update (and optionally reveal) an
    // invisible handle rather than have the viewer switch to a move tool
    // state.  This method returns true if a handle was updated (i.e. the
    // viewer should not launch a move tool state).
    virtual bool	 updateInvisiblePIHandleForMoveTool(
			    bool (*func)(BM_Handle *handle,
					 BM_MoveTool::Type tool,
					 unsigned vport_mask),
			    BM_MoveTool::Type tool,
			    unsigned vport_mask,
			    bool make_visible);

    // These set the flag as to whether or not we highlight ops
    // as we drop them.
    static int		 getAutoHighlightFlag() { return theAutoHighlightFlag;}
    static void		 setAutoHighlightFlag(int o) 
						{ theAutoHighlightFlag = o; }
    /// Obtains the current global time.
    fpreal		 getTime() const;

    virtual bool getStateParmNames(UT_StringArray &ret, const char* prefix=nullptr );
    virtual bool evalStateParm(const char *name, UT_StringHolder &ret);
    virtual bool setStateParm(const char *name, const UT_StringHolder &val);
    virtual bool pressStateButton(const char *name);

    BM_OpHandleLink*	    piByRootDescription(const char *description);
    BM_OpHandleLink const*  piByRootDescription(const char *description) const;

protected:
	  BM_OpView	&baseViewer()		{ return myViewer; }
    const BM_OpView	&baseViewer() const	{ return myViewer; }


    // Interrupt this state or resume its activity.
    virtual void	 interrupt(BM_SimpleState * = 0);
    virtual void	 resume   (BM_SimpleState * = 0);

    // deal with events coming from either the popup menus or the keyboard
    virtual int		 handleKeyTypeEvent(UI_Event *event,
					    BM_Viewport &viewport);
    // Handle transitory key presses.
    virtual bool	 handleTransitoryKey(const UI_Event &event);

    // Create a PI and add it to the list of PI's.  If add_button is set,
    // a button is added to turn the PI on and off, and the makePIVisible
    // method is called when the button is pressed.  This button is added
    // to the specified folder.  On success, the index of the newly added
    // PI is returned.  -1 is returned on failure.
    int			 createPI(const char *pi_name, int opnum = -1,
				  const char *description = 0);

    // allows base class to react when a new PI is created or removed
    virtual void	 onCreatePI(PI_OpHandleLink *pi);
    virtual void	 onRemovePI(PI_OpHandleLink *pi);

    // Append an already existing PI and reflect it it our UI:
    int			 appendPI(BM_OpHandleLink *pi, int opnum = -1);
    virtual void	 removePI(int pi_idx);
    void		 removeAllLinkedPIs();
	  BM_OpHandleLink *pi(int index);
    const BM_OpHandleLink *pi(int index) const;
	  BM_OpHandleLink *pi(const char *name);
    const BM_OpHandleLink *pi(const char *name) const;
    
    const char		*piDescription(int index) const;
    int			 piOpnum(int index) const;
    int			 numPIs() const { return myPIs.entries(); }

    // This method is called whenever the button is pressed to turn a PI
    // on or off.  It does not need to redraw the workbench.  It does nothing
    // in this base class.
    virtual void	 makePIVisible(int pi_idx, int onoff);

    // Methods to combine different dialogs into the dialog for this state.
    int			 appendFolder(const char *folder_name);
    void		 appendFeelToFolder(const char *folder_name, 
					    UI_Feel *child_feel);
    int			 appendDialogToFolder(const char *folder_name,
					      BM_ParmState &state_or_handle);
    int			 appendDialogToFolder(const char *folder_name,
				      OP_Node &op_node,
				      PRM_Template::PRM_Export export_level);
    int			 appendThisStateDialogToFolder(const char *folder_name);
    int			 appendDividerToFolder(const char *folder_name);
    int			 appendTextToFolder(
			    const char *folder_name, const char *text,
			    UI_LookType look_type = UI_LOOK_HEADING_TEXT);
    int			 removeDialog(BM_ParmState &state);
    int			 removeDialog(OP_Node &op_node,
				      PRM_Template::PRM_Export export_level);
    int			 removeFolder(const char *name);

    // Switches the UI to look at the folder given by name (if it exists)
    void		 switchToFolder(const char *name);

    // callback that is called when a dialog is deleted. It clears
    // any pointers that may become stale.
    virtual void	 handleDialogDeleted( OPUI_Dialog *deleted_dialog );

    // This will remove all the dialogs which this state has created.
    // It is virtual as some derived states may have extra dialogs.
    virtual void	 removeAllDialogs();
    void		 handlePIVisibilityChange(UI_Event *event);

    // Make the new, not-yet-connected node an output of the inputnode.
    // If not branching off, move inputnode's outputs to newnode's
    // output list.
    void		 connectNode(OP_Node &newnode, OP_Node &inputnode,
				     int input_idx = 0, int branch_off = 0);

    // Stamp the node with the name of this state:
    void		 stampNode(OP_Node &newnode);

    // Return 1 if the node is generated by a state that matches our name,
    // or if the node's name matches our name:
    virtual int		 matchesNode(OP_Node &node) const = 0;

    BM_OpView				&myViewer;

    UT_ValArray<opbm_DialogInfo *>	 myDialogs;
    UT_ValArray<opbm_PIContext *>	 myPIs;
    UT_ValArray<UI_Feel *>		 myMiscFeels;
    SI_Folders				*myFolders;

    static const char			*STATE_DIALOG_FOLDER;
    static const char			*HANDLES_FOLDER;
    static const char			*OP_DIALOG_FOLDER;
    static int				 theAutoHighlightFlag;

private:
    // callback that is called when a dialog is deleted. It clears
    // any pointers that may become stale.
    void		 dialogDeletedCallback( UI_Event* event );

    void		 buildStateParmPrefix(UT_WorkBuffer &buf, BM_OpHandleLink *pii, BM_Handle *h);
    BM_ParmState*	 findStateParm( UT_String &name );

};

#endif
