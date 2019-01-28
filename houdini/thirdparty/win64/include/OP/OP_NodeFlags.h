/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP Library (C++)
 *
 */

#ifndef __OP_NodeFlags_h__
#define __OP_NodeFlags_h__

#include "OP_API.h"
#include <SYS/SYS_Types.h>
#include <iosfwd>

class UT_IStream;
class UT_String;
class OP_Node;
class OP_Network;
class OP_Director;

#define OP_BYPASS_FLAG		'b'
#define OP_COLORDEFAULT_FLAG	'F'
#define OP_COMPRESS_FLAG	'c'
#define OP_CURRENT_FLAG		'C'
#define OP_DISPLAY_FLAG		'd'
#define OP_EXPOSE_FLAG		'e'
#define OP_FOOTPRINT_FLAG	'f'
#define OP_HIGHLIGHT_FLAG	'h'
#define OP_LOCK_FLAG		'l'
#define OP_SOFT_LOCK_FLAG	'K'
#define OP_PICK_FLAG		'p'
#define OP_DESCRIPTIVENAME_FLAG	'P'
#define OP_DISPLAYCOMMENT_FLAG	'Y'
#define OP_RENDER_FLAG		'r'
#define OP_SAVEDATA_FLAG	's'
#define OP_TEMPLATE_FLAG	't'
#define OP_UNLOAD_FLAG		'u'
#define OP_VISIBLE_FLAG		'V'
#define OP_XRAY_FLAG		'y'

#define OP_MODELED_FLAG		'm'
#define OP_END_FLAG		'E'
#define OP_ORIGIN_FLAG		'x'

#define OP_FLAG_ON		" on"
#define OP_FLAG_OFF		" off"

#define SET_FLAG_STRING(str_val, flag_val)	\
	    str_val = (flag_val) ? OP_FLAG_ON : OP_FLAG_OFF

//___________________________________________________________________________

class OP_API OP_NodeFlags
{
public:
    
    enum OP_LockTypes 
    { 
	OP_UNLOCKED	= 0x0, 
	OP_HARD_LOCKED	= 0x1, 
	OP_SOFT_LOCKED 	= 0x2
    };

    static const char  *getFlagName(uchar token);

    static const uchar *getTakeFlagTokens();
    static bool		isTakeFlag(uchar token);

    // Methods to check whether a particular flag is subject to permissions.
    // The set of flags subject to asset permissions is a superset of those
    // subject to standard permissions.
    static bool		isSubjectToStdPermissions(uchar token);
    static bool		isSubjectToAssetPermissions(uchar token);

    void		setRecursion(bool val) const
			{ myRecursion = val; }
    bool		getRecursion() const
			{ return myRecursion; }

    void		setUndoComment(int onOff)
			{ undoComment = onOff; }
    void		setUndoInput(int onOff)
			{ undoInput = onOff; }
    void		setUndoLayout(int onOff)
			{ undoLayout = onOff; }
    void		setUndoDelScript(int onOff)
			{ undoDelScript = onOff; }
    void		setUndoPreTransform(int onOff)
			{ undoPreTransform = onOff; }
    void		setUndoFlagChange(int onOff)
			{ undoFlagChange = onOff; }
    void		setColorDefault(int onOff)
			{ colorDefault = onOff; }
    void		setCompress(int onOff)
			{ compress = onOff; }
    void		setDisplayDescriptiveName(int onOff)
			{ displayDescriptiveName = onOff; }
    void		setDisplayComment(int onOff)
			{ displayComment = onOff; }
    void		setModelled(int onOff)
			{ modelled = onOff; }
    void		setAlwaysCook(int onOff)
			{ alwaysCook = onOff; }
    void		setUnload(int onOff)
			{ unload = onOff; }
    void		setForceCook(int onOff)
			{ forceCook = onOff; }
    void		setHasCookedData(int onOff)
			{ myHasCookedData = onOff; }
    void		setTimeDep(int onOff)
			{ timeDep = onOff; }
    void		setCooking(int onOff)
			{ cooking = onOff; }
    void		setDisplay(int onOff)
			{ display = onOff; }
    void		setRender(int onOff)
			{ render = onOff; }
    void		setHighlight(int onOff)
			{ highlight = onOff; }
    void		setTemplate(int onOff)
			{ templated = onOff; }
    void		setFootprint(int onOff)
			{ footprinted = onOff; }
    void		setXray(int onOff)
			{ xray = onOff; }
    void		setBypass(int onOff)
			{ bypass = onOff; }
    void		setPicked(int onOff)
			{ picked = onOff; }
    void		setEditPicked(int onOff)
			{ editPicked = onOff; }
    void		setHardLocked(int onOff)
			{ locked = onOff ? OP_HARD_LOCKED : OP_UNLOCKED; }
    void		setSoftLocked(int onOff)
			{ locked = onOff ? OP_SOFT_LOCKED : OP_UNLOCKED; }
    void		setLocked(int state)
			{ locked = state;   }
    void		setModified(int onOff)
			{ modified = onOff; }
    void		setExpose(int onOff)
			{ exposed = onOff; }
    void		setSaveBypass(int onOff)
			{ saveBypass = onOff; }
    void		setSaveData(int onOff)
			{ saveData = onOff; }

    // Used to detect if we need to call OP_Node::handleTimeInterest() on
    // a time change
    void		setTimeInterest(int onOff)
			{ timeInterest = onOff; }
    void		setTimeInterestCook(int onOff)
			{ timeInterestCook = onOff; }
    void		setClearDependency(int onOff)
			{clearDependency = onOff;}

    int			getUndoComment() const
			{ return undoComment; }
    int			getUndoInput() const
			{ return undoInput; }
    int			getUndoLayout() const
			{ return undoLayout; }
    int			getUndoDelScript() const
			{ return undoDelScript; }
    int			getUndoPreTransform() const
			{ return undoPreTransform; }
    int			getUndoFlagChange() const
			{ return undoFlagChange; }
    int			getColorDefault() const
			{ return colorDefault; }
    int			getCompress() const
			{ return compress; }
    int			getDisplayDescriptiveName() const
			{ return displayDescriptiveName; }
    int			getDisplayComment() const
			{ return displayComment; }
    int			getModelled() const
			{ return modelled; }
    int			getAlwaysCook() const
			{ return alwaysCook; }
    int			getUnload() const
			{ return unload; }
    int			getForceCook() const
			{ return forceCook; }
    int			getHasCookedData() const
			{ return myHasCookedData; }
    int			getTimeDep() const
			{ return timeDep; }
    int			getCooking() const
			{ return cooking; }
    int			getDisplay() const
			{ return display; }
    int			getRender() const
			{ return render; }
    int			getHighlight() const
			{ return highlight; }
    int			getTemplate() const
			{ return templated; }
    int			getFootprint() const
			{ return footprinted; }
    int			getXray() const
			{ return xray; }
    int			getBypass() const
			{ return bypass; }
    int			getPicked() const
			{ return picked; }
    int			getEditPicked() const
			{ return editPicked; }
    int			getHardLocked() const	
			{ return locked == OP_HARD_LOCKED; }
    int			getSoftLocked() const	
			{ return locked == OP_SOFT_LOCKED; }
    int			getLocked() const
			{ return locked; }
    int			getModified() const
			{ return modified; }
    int			getExpose() const
			{ return exposed; }
    int			getSaveBypass() const
			{ return saveBypass; }
    int			getSaveData() const
			{ return saveData; }
    int			getTimeInterest() const
			{ return timeInterest; }
    int			getTimeInterestCook() const
			{ return timeInterestCook; }
    int			getClearDependency() const
			{ return clearDependency; }

    void		clearAllTakeFlags();
    // Set the activation flag
    void		setTakeFlag(uchar which, int v);
    int			getTakeFlag(uchar which) const;
    // Set the actual flag associated with the take token
    void		setTakeValue(uchar which, int v);
    int			getTakeValue(uchar which) const;

    void		setHasTakeData(int onoff)
			{ myHasTakeData = onoff; }
    int			getHasTakeData() const
			{ return myHasTakeData; }

    int			getTakeDisplay() const
			{ return myTakeDisplay; }
    int			getTakeRender() const
			{ return myTakeRender; }
    int			getTakeTemplate() const
			{ return myTakeTemplate; }
    int			getTakeExpose() const
			{ return myTakeExpose; }
    int			getTakeBypass() const
			{ return myTakeBypass; }
    int			getTakePick() const
			{ return myTakePick; }
    int			getTakeCurrent() const
			{ return myTakeCurrent; }

    bool		getInterrupted() const
			{ return myInterrupted; }
    void		setInterrupted(bool f)
			{ myInterrupted = f; }
    bool		getDescendantInterrupted() const
			{ return myDescendantInterrupted; }
    void		setDescendantInterrupted(bool f)
			{ myDescendantInterrupted = f; }
    bool		getClearingInterrupt() const
			{ return myClearingInterrupt; }
    void		setClearingInterrupt(bool f)
			{ myClearingInterrupt = f; }

    const char		*getExtra() const
    			 { return myExtra; }

    void		 setExtra(const char *opts, int val,
	    			  const char **labels)
			 {
			     myExtra = opts;
			     myExtraVal = val;
			     myExtraLabel = labels;
			 }

    int			 getExtraVal(char a) const;
    void		 setExtraVal(char a, int onoff);


    void		 appendCommandString(std::ostream &os,
					     const char *flags,
					     bool save_to_hip) const;
    void		 getFlagOptions(UT_String &options) const;

    //  Data for the flags
    //
    // These flags are public because I don't feel like breaking everything
    // right now. Instead, I've just made sure that all the flags have
    // accessors. USING THESE DIRECTLY IS DEPRECATED! USE THE ACCESSORS!

    // the most common bits are promoted to full characters for
    // maximum on/off testing efficiency
    char		modified;	// Changed since load/startup.
    char		 timeDep;	// Needs to recook at different time
    char		 cooking;	// OP is currently cooking
    char        	  locked;	// OP is hard or soft locked
    char               forceCook;	// Force op to recook
    char              alwaysCook;	// Always cook this op
    char	          bypass;	// OP is bypassed when cooking
    char		  render;	// OP is rendered
    char		 display;	// OP is displayed
    char	       templated;	// Data on for display only

    // USING THESE DIRECTLY IS DEPRECATED! USE THE ACCESSORS!
    unsigned    modelled:1,	// Cooked data has been edited.
	      saveBypass:1,	// OP is bypassed when saving.
		saveData:1,	// Save cooked data in file.
		  picked:1,	// Picked by user.
	      editPicked:1,	// Picked by user from viewport.
		  unload:1,	// Unload data after usage.
	    colorDefault:1,	// Use the default color for this node type.
		compress:1,	// Display OP node in a compressed UI.
  displayDescriptiveName:1,	// Display the descriptive name, if any.
	  displayComment:1,	// Display the comment, if any.

		// These flags are used to save whether a particular
	        // type of change for the node has been saved.
	  undoParmChange:1, // NB: This flag seems to be no longer used!
	  undoFlagChange:1,
	      undoLayout:1,
	       undoInput:1,
	     undoComment:1,
	         exposed:1;


private:
    // the most common bits are promoted to full characters for
    // maximum on/off testing efficiency
    mutable bool myRecursion;
    bool	myHasCookedData;	// Node has some concept of cooked data
    char	highlight;		// OP's contents are highlighted

    // Which flags are active in the current take
    unsigned	myHasTakeData:1,	// Node has take data of some kind
		myTakeDisplay:1,
		myTakeRender:1,
		myTakeTemplate:1,
		myTakeExpose:1,
		myTakeBypass:1,
		myTakePick:1,
		myTakeCurrent:1;

		// When an OP's cooking is interrupted, we set this flag
    bool	myInterrupted:1;
		// Parent networks of any interrupted node has this flag set
    bool	myDescendantInterrupted:1;
		// This one says we are in the clearInterrupt function.
    bool	myClearingInterrupt:1;

    unsigned	footprinted:1,
		xray:1,
		// Used to save whether a particular type of change has been
		// saved
		undoDelScript:1,
		undoPreTransform:1,
	        // Used to detect if we need to call
		// OP_Node::handleTimeInterest() on a time change
		timeInterest:1,
		timeInterestCook:1, // if we've already checked timeInterest
		clearDependency:1;  // if we should clear cook dependencies

private:
    friend class	OP_Node;
    friend class	OP_UndoFlag;

			OP_NodeFlags()
			    : myExtra(nullptr),
			      myExtraLabel(nullptr)
			{ setDefaults(); }
			OP_NodeFlags(const OP_NodeFlags &source)
			{ shallowCopy(source); }

    OP_NodeFlags &	operator=(const OP_NodeFlags &) = delete;

    void		shallowCopy(const OP_NodeFlags &source);


    // Methods to load & save for streams
    void		save(std::ostream &os, int binary = 0, int forundo = 0);
    bool		load(UT_IStream &os, const char *path=0);

    bool		loadAsciiFlag(const char *name, const char *val);
    bool		loadBinaryFlag(int tag, int val);

    // This method initializes all the flags to their default values
    void		setDefaults(void);

    const char		 *myExtra;
    const char		**myExtraLabel;
    unsigned int	  myExtraVal;

};

//
// When OP_FLAG_CHANGED events are sent a pointer to this structure
// is given.
struct OP_API OP_NodeFlagData
{
    OP_NodeFlagData(int f, int v) { flag = f; value = v; }
    OP_NodeFlagData() { flag = -1; value = 0; }

    int		flag;		// OP_FOO_FLAG or -1 for all
    int		value;		// New value where appropriate.
};

#endif
