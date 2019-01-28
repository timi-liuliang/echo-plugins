/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PI_OpHandleLink.h (PI Library, C++)
 *
 * COMMENTS:
 *
 *	PI base class.
 */

#ifndef __PI_OpHandleLink__
#define __PI_OpHandleLink__

#include "PI_API.h"
#include <UT/UT_Color.h>
#include <UT/UT_String.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_SymbolTable.h>
#include <UT/UT_UndoManager.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_VectorTypes.h>
#include <PRM/PRM_ChanState.h>
#include <PRM/PRM_Value.h>
#include <OP/OP_Value.h>
#include <iosfwd>

#include <functional>

#include "PI_ResourceTemplate.h"
#include "PI_ResourceManager.h"

#define PI_SETTING_SHOWOPPATH	"showoppath"
#define PI_SETTING_COLOR	"color"
#define PI_SETTING_OWNEROP	"ownerop"
#define PI_SETTING_OWNEROPGROUP	"owneropgroup"

class OP_Node;
class OP_Operator;
class CMD_Args;
class PI_SettingList;

 class PI_API PI_OpHandleLink
{
public:
   struct LateBinding
    {
	// Interface to define the protocol for implementing handle late bindings
	LateBinding() = default;
	virtual ~LateBinding() = default;

	// read/write parm values
	virtual bool toStateValues(PI_OpHandleLink*) = 0;
	virtual bool toHandleValues(PI_OpHandleLink*) = 0;

	// data accessors
	virtual int getParmValue(PI_OpHandleLink const* pi, int pindex, int &val, int d=0) const = 0;
	virtual int getParmValue(PI_OpHandleLink const* pi, int pindex, fpreal &val, fpreal d) const = 0;
	virtual int getParmValue(PI_OpHandleLink const* pi, int pindex, UT_String &val, UT_String d=0) const = 0;
	virtual int setParmValue(PI_OpHandleLink const* pi, int pindex, fpreal value, bool setkey) = 0;
	virtual int setParmValue(PI_OpHandleLink const* pi, int pindex, int value, bool setkey) = 0;
	virtual int setParmValue(PI_OpHandleLink const* pi, int pindex, UT_StringRef const& value) = 0;
    };

 	     PI_OpHandleLink(const PI_PITemplate &templ);
    virtual ~PI_OpHandleLink();

    // Methods to link a handle's parameter with an op's parameter.
    // Return 1 on success, 0 if parm name is invalid. op_type_name tells the
    // type of operator. Do not call these methods from the class c-tor.
    int			 linkOpParmToHandleParm(const char *op_type,
				    const char *handle_parm_name,
				    const char *op_parm_name);

    // Activate the underlying handle parm, typically used when dynamic handle 
    // binding is in effect (see PI_OpHandleLink::HandleBinding)
    int activateHandleParm(int pindex);

    virtual int          attach(OP_Node &node);
    virtual int          detach(OP_Node &node);
    virtual void         detachAll();

    void		 attachForKeyframe(OP_Node &node);
    void		 detachForKeyframe(OP_Node &node);
    void		 detachAllForKeyframe();

    // Hide all handle parameter parts except for the ones from handle_parms
    // (starting from index `beg`).
    void		 isolateHandleParms(int beg,
					 const UT_StringArray& handle_parms);

    void		 setShowOpPath(bool onoff);
    virtual bool	 getShowOpPath() const;

    // The root description is the description for the PI that is set
    // in the binding. Then the name of the first attached operator may
    // be appended by the buildDecription function (which calls
    // the virtual descriptionChanged in case a subclass needs to do
    // something in responds to a description change).
    void 		 setRootDescription(const char *description);
    const char		*getRootDescription() const;
    void		 buildDescription();
    const char		*getDescription() const;
    virtual void	 descriptionChanged();

    // The time overrides are used by the Motion Path PI to manipulate other PIs
    // to modify parm values at a given time instead of at the current time
    // in the animation timeline.
    void                 setIsTimeOverride(bool is_time_override);
    void                 setTimeOverrideTime(fpreal t);

    bool                 getIsTimeOverride() const
                         { return myIsTimeOverride; }
    fpreal               getTimeOverrideTime() const
                         { return myTimeOverrideTime; }

    // Does this PI allow saving/loading from the per-op and/or last settings
    // tables?  By default, it allows using both tables.
    bool		 allowsUsingPerOpSettingsTable() const
			 { return myAllowUsingPerOpSettingsTable; }
    bool		 allowsUsingLastSettingsTable() const
			 { return myAllowUsingLastSettingsTable; }
    bool		 allowsUsingOpTypeSettings() const
			 { return myAllowUsingOpTypeSettings; }
    bool		 allowsUsingLastVisibilitySetting() const
			 { return myAllowUsingLastVisibilitySetting; }

    // When saving settings for this PI to the last settings table, we
    // often only want PI's of this type to use these settings when they're
    // attached to a specific type of operator.  For example, the "xform"
    // PI can be attached to many different types of sops.  However, we
    // don't want the last "xform" settings for the "copy" sop to be used
    // the next time we put down an "xform" sop.  This method returns that
    // type of operator, if there are any restrictions, or null if there are
    // no restrictions.  The PI must be attached to at least one op when
    // this method is called.  By default, it returns null if this PI is
    // attached to more than one op, or the type of the op if it is attached
    // to exactly one.  This method will also always return null if the
    // myAllowUsingOpTypeSettings flag is false.
    OP_Operator		*lastSettingsOperatorTypeRestriction() const;

    // Return the op to which we're attached, or nil if we're detached.
    OP_Node		*getOpNode(int opnum = 0) const;
    int			 getNumOps() const;
    OP_Node		*getKeyframeOpNode(int opnum = 0) const;
    int			 getKeyframeNumOps() const;

    // Find out if we contain this node:
    int			 hasOpNode(const OP_Node &node) const;
    int			 hasKeyframeOpNode(const OP_Node &node) const;
    OP_Node		*findOpNode(int op_type, const char *op_name) const;

    // Update ourselves (and our handles) based on our ops parm values.
    void		 doUpdateFromOpParms();

    // Respond to a parameter change in the op. Called from the above
    // function.
    virtual void	 handleOpNodeChange(OP_Node &node);
    virtual void	 handleOpUIChange(OP_Node & /*node*/)
			 { }
    virtual int		 wantsOpNodeChange(const OP_Node & /*node*/) const
			 { return 0; }
    virtual bool	 flagOpNodeChangeNeeded(OP_Node & /*node*/)
			 { return false; }
    
    // The user can dynamically change the bindings from the textport.
    // We need to refresh the current handle link in the viewports 
    // because it could be affected. Return 0 if this handle has no bindings
    // left.
    virtual int 	 refreshBindings(int op_type, const char *op_name);

    // These id values are used by the linking mechanism to determine if
    // the pi for a particular linked parm exists already or not.
    void		 id(int i);
    int			 id() const;

    // Returns a list of all parms bound to this handle.
    const UT_StringArray		&linkedParmNames(int opnum) const
			 { return *myParmNames(opnum); }
    // These virtuals are similar to the linked... functions below.
    // The difference is that these versions are used by the Key
    // functions above. By ovrriding these, subclasses can make the
    // above functions work for OP parms that are not linked to
    // PI parms.
    virtual const UT_StringArray	&allParmNames(int opnum) const
			 { return *myParmNames(opnum); }

    // This gets the index of the operator that is actually bound
    // to the specified index.  If none is bound, -1 is returned.
    int			 getOpnumForHandleParmIndex(int idx) const;

    // Query or set the active color of the PI:
    const UT_Color	&getActiveColor() const;
    virtual void	 setActiveColor(const UT_Color &c);

    // Set accessibility state of the binding
    void		 setBindingState(const bool active);

    virtual int		 isEqual(const PI_OpHandleLink &pi) const;
    virtual int		 setEqual(const PI_OpHandleLink &pi);

    // An ethereal handle link is one that is invisible by nature.
    bool		 ethereal(void) const;
    const PI_PITemplate	&getTemplate() const;

    int			 writeOptions(std::ostream &os, const char *baseCommand);
    int			 runBindCommand(CMD_Args &args);
    int			 runUnbindCommand(CMD_Args &args);

    // Functions used to handle menu functionality
    void 		 editChannels(int append_channels, int opnum=-1);
    void 		 findKey(int direction, int opnum=-1);
    void 		 setKeys(int opnum = -1,
				 int pindex_start = -1,
				 int pined_stop = -1,
				 const char *undo_str = 0);
    void 		 removeKeys(int opnum=-1);
    void 		 deleteChannels(int opnum=-1);
    void 		 lockParms(int opnum=-1);
    void 		 revertToDefaults(int opnum=-1);
    void 		 revertToFactoryDefaults(int opnum=-1);
    void 		 overwriteDefaults(int opnum=-1);

    // Load or save the settings for this PI.  If specific_to_op is true
    // in saveSettings() then the PI's settings will be saved in the per-op
    // settings table instead of the last settings table.
    virtual void	 loadSettings(const PI_SettingList &settings);
    virtual void	 saveSettings(PI_SettingList &settings,
				      bool specific_to_op) const;

    // Save the settings for this PI.
    virtual void	 saveSettingsToAppropriateLocation();

    // Load the settings for this PI from the per-op or last setting table.
    // The PI _must_ be attached to its nodes when this method is called.
    void		 loadSettingsFromTables(bool okay_to_use_last_settings);

    // Remove any settings this PI may have stored from the per-op and
    // last setting tables.
    void		 removeSettingsFromTables();

    class PI_API PI_Interest
    {
	public:
	    int			 myIndex;
	    PRM_ValueCallback	 myCallback;
    };

    // This extracts the projection associated with this binding, if any,
    // from the parmname.  channel is set to the name of the channel
    // which is bound.
    static void		 extractProjection(UT_String &channel,
			    const char *parmname, fpreal &a, fpreal &b,
			    fpreal &c, fpreal &d);
    static void		 extractChannelName(UT_String &channel,
					    const char *parmname)
			    {
				fpreal	dummy;
				extractProjection(channel, parmname,
						  dummy, dummy,
						  dummy, dummy);
			    }

    void setLateBinding(LateBinding * binding);
    bool hasLateBinding(LateBinding const * binding) const;

protected:

    // Update ourselves (and our handles) based on our ops parm values.
    virtual void	 updateFromOpParms(void);

    // Override this function to help decide which handles should be
    // active when this PI is attached to an operator.  This will be
    // called if we successfully link to the given pindex.
    //
    // You should call the base class first and only proceed if it
    // returns 1.  I don't know the semantics of that - it's just how
    // the old linkOpParm worked.
    virtual int		 	 justLinked(int pindex);
    virtual void		 justUnlinked(int pindex);

    virtual PI_Interest *getInputGroupInterests() const;

    // Map a handle parm name to an index, or retrieve the index by the name.
    // Indices must be non-negative.  If the parm has not been mapped,
    // findHandleParmIndex returns -1.
    int			 findHandleParmIndex(const char *h_parm_name) const;

    // These methods add/remove interests in all of a PI's input group
    // parameters. These methods are called when a node is attached or
    // detached from the PI. They use the array of PI_Interests
    // returned by getInputGroupInterests().
    void		 addAllInputGroupInterests(int opnum);
    void		 removeAllInputGroupInterests(int opnum);

    // Convenience method for descendents to get the current time.
    virtual fpreal	 getTime() const;

    // Methods to get/set the op's parameter values.  If the parameter has
    // not been linked to an op parameter, these methods return 0.  Otherwise,
    // they return 1.
    int			 getParmValue(int pindex, int &val, int d=0) const;
    int			 getParmValue(int pindex, fpreal &val, fpreal d=0) const;
    int			 getParmValue(int pindex, UT_String &val,
				      UT_String d=0) const;
    // Get the [pindex, pindex+3) parm values into a UT_Vector3R
    void		 getParmVector3(int pindex, UT_Vector3R &v,
					fpreal def = 0.0) const;

    int			 getParmInstValue(int pindex, const int *inst,
					  int &val, int d=0,
					  int nestlevel=1) const;
    int			 getParmInstValue(int pindex, const int *inst,
					  fpreal &val, fpreal d=0,
					  int nestlevel=1) const;
    int			 getParmInstValue(int pindex, const int *inst,
					  UT_String &val, UT_String d=0,
					  int nestlevel=1) const;
    // Get the [pindex, pindex+3) parm values into a UT_Vector3R
    void		 getParmInstVector3(int pindex, const int *inst,
                                            UT_Vector3R &v,
					    fpreal def = 0.0) const;

    int			 setParmValue(int pindex, int val, bool addkey=false);
    int			 setParmValue(int pindex, fpreal val, bool addkey=false);
    int			 setParmValue(int pindex, const UT_String &val,
				      CH_StringMeaning meaning);
    // Get the [pindex, pindex+3) parm values into a UT_Vector3R
    void		 setParmVector3(int pindex, const UT_Vector3R &v);

    int			 setParmInstValue(int pindex, const int *inst,
					  int val, bool addkey=false,
					  int nestlevel=1);
    int			 setParmInstValue(int pindex, const int *inst,
					  fpreal val, bool addkey=false,
					  int nestlevel=1);
    int			 setParmInstValue(int pindex, const int *inst,
					  const UT_String &val,
					  CH_StringMeaning meaning,
					  int nestlevel=1);
    void		 setParmInstVector3(int pindex, const int *inst,
					    const UT_Vector3R &v);

    int			 performParmCallback(int pindex, int theitem);

    int			 setOpParmValue(OP_Node *op, PRM_Parm *parm, int vecidx,
					int val, bool addkey=false);
    int			 setOpParmValue(OP_Node *op, PRM_Parm *parm, int vecidx,
					fpreal val, bool addkey=false);
    int			 setOpParmValue(OP_Node *op, PRM_Parm *parm, int vecidx,
					const UT_String &val,
					CH_StringMeaning meaning);
    
    // Methods to read or modify parm values given a pindex without adding keys
    // Returns true if Parim is read/modified, otherwise false
    bool                 readParm(int pindex, std::function<void(const PRM_Parm &parm,
                                  int vecidx)> f);
    
    bool                 modifyParm(int pindex, std::function<void(PRM_Parm &parm,
                                    int vecidx)> f); 


    // copies the keyframe structure of pindex_from to pindex_dest, while
    // leaving the value of pindex_dest constant.
    void		 copyKeyStructure(int pindex_dest, int pindex_from);

    // returns true if the parameter is enabled (a la disableParms())
    bool		 isParmEnabled(int pindex, int opnum = 0) const;

    // returns the parameter animation state.
    PRM_ChanState	 getParmState(int pindex, int opnum = 0) const;

    // Functions to access the parameter index arrays.
    int			 getLinkedParmCount() const
			 { return myParmNames.entries(); }

    // This function determines whether or not we should do an
    // updateFromOpParms when the time changes.
    // Returns true unless we have an input group interest, in which
    // case the input group interest is enough to force the update,
    // so we return false.
    virtual int		 hasTimeInterest() const;
    virtual void	 handleTimeChange();

    // These two functions are provided for all the PI based operations
    // that need to manage an undo block that begins in one function
    // but ends in another function (or another instance of the same
    // function).
    //
    // However, they can only handle ONE block at a time. No nesting
    // of calls to beginDistributedUndo!
    void		 beginDistributedUndoBlock(const char *operation,
						   UT_UndoBlockType blocktype);
    void		 endDistributedUndoBlock();

    // Returns true if we have called beginDistributedUndoBlock but have
    // not yet called endDistributedUndoBlock; returns false otherwise.
    bool		 inDistributedUndoBlock();

    // This virtual callback is called whenever one of our attached
    // or attached-for-keyframe nodes is changed.
    virtual void	 handleOpChange(OP_Node *op,
					OP_EventType etype,
					void *data);

    // Subclasses may call these methods to customize how their settings
    // are saved.
    void		 setAllowUsingPerOpSettingsTable(bool onoff)
			 { myAllowUsingPerOpSettingsTable = onoff; }
    void		 setAllowUsingLastSettingsTable(bool onoff)
			 { myAllowUsingLastSettingsTable = onoff; }
    void		 setAllowUsingOpTypeSettings(bool onoff)
			 { myAllowUsingOpTypeSettings = onoff; }
    void		 setAllowUsingLastVisibilitySetting(bool onoff)
			 { myAllowUsingLastVisibilitySetting = onoff; }

    // Some functions for getting and setting our owner operator information.
    // The owner operator is the operator that defines what space our handles
    // should appear in and what SOP/group field we should be interested in.
    virtual void	 setOwnerOp(const char *owner_op);
    const UT_String	&getOwnerOp() const;
    OP_Node		*getOwnerOpRelativeTo(OP_Node *node) const;
    virtual void	 setOwnerOpGroup(const char *owner_op_group);
    const UT_String	&getOwnerOpGroup() const;

    // Return the channel name for our first input group interest.
    const UT_StringHolder	&getFirstInputGroupInterestName() const;

    // This helper function returns true if the parm channel_name is locked and
    // false otherwise.  This function will follow any channel references that
    // this parm might have to determine the "true" locked status.
    static bool isOpParmLocked(
			    const char *channel_name, OP_Node &op, fpreal time,
			    const PRM_Parm **ret_parm = NULL,
			    const OP_Node **ret_node = NULL);

    // called by derived class to update the state values bind to a handle
    void updateStateValues();

private:
    int			 linkOpParm(const char *op_type,
				    int pindex, const char *parmname);
    void		 updateParmLockStatus( OP_Node &op );						
    UT_StringArray	&opParmNames(const char *op_type);
    void		 addParmNames(int opnum);
    void		 removeParmNames(int opnum);

    void 		 setKeysSingleOp(int opnum, int pindex = -1,
					 UT_String *names = NULL,
					 const char *name_prefix = NULL);
    void 		 removeKeysSingleOp(int opnum);
    void 		 deleteChannelsSingleOp(int opnum);
    void 		 lockParmsSingleOp(int opnum);
    void 		 revertToDefaultsSingleOp(int opnum);
    void 		 revertToFactoryDefaultsSingleOp(int opnum);
    void 		 overwriteDefaultsSingleOp(int opnum);

    // For a given pindex, add/remove interest's in the corresponding
    // parameter's PRM_Value.
    int			 addInputGroupInterest(int pindex,
					       PRM_ValueCallback callback,
					       int opnum);
    int			 removeInputGroupInterest(int pindex,
						  PRM_ValueCallback callback,
						  int opnum);

    // Callback function to make sure we always know when one of our
    // nodes is being deleted.
    static void		 handleOpChangeCallback(OP_Node *op, void *callee,
						OP_EventType etype, void *data);

    void clearOpTypeParmNamesMap();
    
    // used for updating handle values from a dynamic handle binding
    void updateHandleValues();
    
    const PI_PITemplate		&myTemplate;
    // This symbol table has as it's index the optype name, and as it's
    // contents the cached UT_StringArrays which would correspond to that
    // op's ParmNames.
    UT_SymbolMap<UT_StringArray *> myOpTypeParmNames;
    
    // This is a translation from our PI parm index into the opnum.
    UT_IntArray			 myOpnumForParmIndex;

    // This is indexed by the opnum and has the string array of op parms
    // that correspond to that opnum.  The index on the string array is
    // the index into our PI parms.
    UT_ValArray<UT_StringArray *>	 myParmNames;
    UT_ValArray<OP_Node *>	 myOpNodes;
    UT_ValArray<OP_Node *>	 myKeyframeOpNodes;
    UT_StringArray		 myInputGroupInterests;
    int				 myId;
    const PI_Interest		*myInterests;
    UT_String			 myRootDescription;
    UT_String			 myDescription;
    UT_String			 myOwnerOp;
    UT_String			 myOwnerOpGroup;

    UT_Color			 myActiveColor;

    int				 myDistributedUndoBlockLevel;
    bool			 myAllowUsingPerOpSettingsTable;
    bool			 myAllowUsingLastSettingsTable;
    bool			 myAllowUsingOpTypeSettings;
    bool			 myAllowUsingLastVisibilitySetting;
    bool			 myShowOpPath;

    bool                         myIsTimeOverride;
    fpreal                       myTimeOverrideTime;
    LateBinding*		 myLateBinding;
};

#endif

