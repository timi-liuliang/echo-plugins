/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        PI_ResourceManager.h ( Direct Manipulation Library, C++)
 *
 * COMMENTS:
 *	This class maintains a table of all states, handles, op-handle links,
 *	and input selectors.  All state, handle, and op-handle link objects
 *	are created and deleted through a PI_ResourceManager.  The active one
 *	is obtained via the global function BMgetResourceManager().
 */

#ifndef __PI_ResourceManager_H__
#define __PI_ResourceManager_H__

#include "PI_API.h"
#include "PI_ResourceTemplate.h"
#include "PI_SettingList.h"
#include "PI_PythonState.h"

#include <OP/OP_Error.h>
#include <OP/OP_OperatorTable.h>
#include <UT/UT_Array.h>
#include <UT/UT_Pair.h>
#include <UT/UT_String.h>
#include <UT/UT_StringMap.h>
#include <UT/UT_SymbolTable.h>
#include <UT/UT_ValArray.h>
#include <SYS/SYS_Hash.h>

class CPP_Manager;
class PRM_Template;
class OP_Operator;
class OP_OperatorTable;
class OP_Node;
class CMD_Args;
class PI_OpHandleLink;
class pi_OpHandleBinding;
class pi_OpSelectorBinding;


/// Error codes retured by PI_ResourceManager bind methods
/// @{
#define PI_BIND_OK			 1
#define PI_NO_TEMPLATE_FOR_HANDLE 	-2
#define PI_INVALID_OP_PARM		-3 
#define PI_INVALID_PI_PARM		-4 
#define PI_BINDING_EXISTS		-5
#define PI_INVALID_OP_FOR_PI		-6
/// @}

typedef void (*PI_SaveCallback)(void *data);

class PI_API PI_BindingParmInfo
{
public:
    PI_BindingParmInfo() 	{ }

    // Note this is the channel name: the receiver should do a
    // PRM_ParmList::getParmPtrFromChannel to get the parm.
    const UT_String		&getOpParm() const
				 { return myOpParm; }
    const UT_String		&getPIParm() const
				 { return myPIParm; }
    int				 operator==(const PI_BindingParmInfo &i) const
				 { return (this == &i); }

private:
    UT_String		 myOpParm;
    UT_String		 myPIParm;
    friend class	 pi_OpHandleBinding;
};

using PI_BindingParmArray = UT_Array<PI_BindingParmInfo>;
using PI_OpHandleLinkMap = UT_SymbolMap<PI_PITemplate *>;
using PI_HandleTemplateMap = UT_SymbolMap<PI_HandleTemplate *>;
using PI_SelectorTemplateMap = UT_SymbolMap<PI_SelectorTemplate *>;

class PI_API PI_BindingHandleInfo
{
public:
    PI_BindingHandleInfo()	{ }

    const UT_String		&getPIName() const
				 { return myPIName; }
    const UT_String		&getPIDescription() const
				 { return myPIDescription; }
    const PI_SettingList	&getDefaultSettings() const
				 { return myDefaultSettings; }
    int				 getId() const
				 { return myId; }
    const PI_BindingParmArray	&getBindings() const
				 { return myBindings; }
    int				 operator==(const PI_BindingHandleInfo &i) const
				 { return (this == &i); }

private:
    UT_String		 myPIName;
    UT_String		 myPIDescription;
    PI_SettingList	 myDefaultSettings;
    int			 myId;
    PI_BindingParmArray	 myBindings;
    friend class	 pi_OpHandleBinding;
};
using PI_BindingHandleArray = UT_Array<PI_BindingHandleInfo>;

// See the bindSelector function for a description of all the data
// members of this class. They map directly to the bindSelector parameters.
class PI_API PI_BindingSelectorInfo
{
public:
    PI_BindingSelectorInfo() { }

    const UT_String	&getName() const
			 { return myName; }
    const UT_String	&getDescription() const
			 { return myDescription; }
    const UT_String	&getPrompt() const
			 { return myPrompt; }
    const UT_String	&getOpParm() const
			 { return myOpParm; }
    const UT_String	&getMenu() const
			 { return myMenu; }
    const UT_String	&getPrimMask() const
			 { return myPrimMask; }
    int			 getOpInput() const
			 { return myOpInput; }
    bool		 getOpInputReq() const
			 { return myOpInputReq; }
    bool		 getAllowDrag() const
			 { return myAllowDrag; }
    bool		 getAstSelAll() const
			 { return myAstSelAll; }
    const UT_String	&getExtraInfo() const
			 { return myExtraInfo; }
    int			 operator==(const PI_BindingSelectorInfo &i) const
			 { return (this == &i); }

private:
    UT_String		 myName;
    UT_String		 myDescription;
    UT_String		 myPrompt;
    UT_String		 myOpParm;
    UT_String		 myMenu;
    UT_String		 myPrimMask;
    int			 myOpInput;
    bool		 myOpInputReq;
    bool		 myAllowDrag;
    bool		 myAstSelAll;
    UT_String		 myExtraInfo;
    friend class	 pi_OpSelectorBinding;
};
using PI_BindingSelectorArray = UT_Array<PI_BindingSelectorInfo>;

class PI_API PI_ResourceManager : public OP_UpdateTableSink,
			   public OP_UpdateTemplatesSink
{
public:
	     PI_ResourceManager();
    virtual ~PI_ResourceManager();

    // Return 1 on success and 0 if the state/handle/op-handle link
    // is already registered.
    virtual int		 registerState(PI_StateTemplate *type);
    virtual int		 registerHandle(PI_HandleTemplate *type);
    virtual int		 registerOpHandleLink(PI_PITemplate *type);
    virtual int		 registerSelector(PI_SelectorTemplate *type);

    bool		 containsState(const char *name,
				       PI_ViewerType viewer_type,
				       unsigned netmask) const;
    bool		 containsHandle(const char *name) const;
    bool		 containsOpHandleLink(const char *name) const;
    bool		 containsSelector(const char *name) const;

    // Get all the bound handles and parms for a given op type.
    const PI_BindingHandleArray
			*getBindingHandles(OP_Operator *op) const;
    // Get all the bound selectors for a given op type.
    const PI_BindingSelectorArray
			*getBindingSelectors(OP_Operator *op) const;

    // Return the list of states that can be used in a viewer for the
    // specified network type.
    const UT_ValArray<PI_StateTemplate *>
			&stateList(PI_ViewerType viewer_type,
				   const char *network_type) const;
    const UT_ValArray<PI_StateTemplate *>
			&stateList(PI_ViewerType viewer_type,
				   PI_NetworkType net_type) const;
    PI_StateTemplate	*getStateTemplate(const char *name,
					  PI_ViewerType viewer_type,
					  const char *type) const;
    PI_StateTemplate	*getStateTemplate(OP_Operator *op,
					  PI_ViewerType viewer_type,
					  const char *type) const;

    // Return the registration status of a state template.
    // 
    virtual PI_PythonState::RegisterStatus getViewerStateRegisterStatus(
				const char *state_name, 
				PI_ViewerType viewer_type,
				const char *type) const;

    // return true if the state name is valid 
    bool validStateTemplateName(const char* name) const;

    // These are functions which are called from commands that allow
    // for dynamic binding of handles to parameters
    int 		 unbind(OP_Operator *op, const char *pidescription,
				const UT_String &opparm);
    int 		 bind(OP_Operator *op, const char *piname,
				const char *pidescription,
				const UT_String &opparm,
				const UT_String &piparm,
				const char *default_settings);
    // this API is typically used for handle late-binding e.g. python states
    // would provide functions for binding handle parms
    int 		 bind(OP_Operator *op, const char *piname,
				const char *pidescription,
				const char *default_settings);
    int 		 bindScriptOp(OP_Operator *op, const char *piname,
				const char *pidescription,
				const UT_String &opparm,
				const UT_String &piparm,
				const char *default_settings);

    // Used for binding node operators to registered viewer states (i..e. python states)
    // returns true if the operation succeeded
    virtual bool bindOperatorToViewerState(const char* state_name, OP_Operator* op);

    /// Unbind the given selector from an operator. Uses the short description
    /// when bound using bindSelector()
    int 		 unbindSelector(OP_Operator *op,
				const char *seldescription);

    /// Dynamic binding of a selector to an operator
    ///
    /// @param op		Operator type that we want to bind a selector to
    /// @param sname		Selector type
    /// @param seldescription	Short description that identifies this selector
    ///				in the context of a one operator (used to
    ///				unbind the selector).
    /// @param prompt		Text in the status area when the selector is
    ///				active.
    /// @param opparm		Operator parameter where the selection string
    ///				is put.
    /// @param opinput		Wire input index where the op owning the
    ///				selected geometry is attached to our op.
    /// @param opinputreq	Is an input is required?
    /// @param primmask		Mask of which primitive types can be in the
    ///				selection. This uses the *_FILEID enum in
    ///				GEO_PrimTypeCompat.h. A regular
    ///				GEO_PrimTypeCompat mask can be used if you
    ///				first convert it using
    ///				GEO_PrimTypeCompat::convertToFileFromMask()
    /// @param allowdrag	Determines if click-and-drag operations are
    ///				allowed.
    /// @param menu		Operator menu parameter where we choose the
    ///				selection type.
    /// @param astselall	If an asterisk is required in the selection
    ///				string to indicate selecting all the input.
    ///				Otherwise we assume an empty string indicates
    ///				that the whole geometry is selected.
    /// @param extrainfo	String that is passed to the selector,
    ///				particular selectors may need specific
    ///				parameters that can be passed through this
    ///				mechanism.
    /// @param fromScriptOp	Used by our dialog script parser to avoid
    ///				infinite recursion. Setting this flag decreases
    ///				the error checking we do.
    ///
    /// @return	PI_BIND_OK upon success, else see of one of the other potential
    ///		error codes at the top of PI_ResourceManager.h
    int			 bindSelector(OP_Operator *op, const char *sname,
				const char *seldescription,
				const char *prompt, const char *opparm,
				int opinput, int opinputreq,
				const char *primmask, int allowdrag,
				const char *menu, int astselall,
				const char *extrainfo, bool fromScriptOp);

    //Helper functions for the above
    PRM_Template	*getTemplateAndVecId(const UT_String &,
					     OP_Operator *, 
					     int &) const;

    // This is for converting old style "t 2" into new style "tz".  It
    // only works if the operator has the template already, else it
    // assumes channel name == parmname.  Thus, it won't work on 
    // multiparms that only exist with OP_Node, not OP_Operator.
    void		 getChannelNameFromParmAndVec(UT_String &chan,
				    OP_Operator *op, const char *parmname,
				    int vecidx);

    int 		checkForPIName( const char *pi_name, 
					const char *pi_parm ) const;

    OP_Operator		*findOperator(PI_NetworkType net_type,
				      const char *op_name,
				      bool display_warning = true) const;
    OP_Operator 	*findOperator(const char *opbindname,
				      UT_String *optype = 0) const;

    virtual void	 findVolatileKeys(const char *volatile_hotkey_name,
					  UT_Array<int> &volatile_keys,
					  UT_Array<int> &keys_modifiers) 
    					  const;
    virtual void	 updateViewports(int id, const char *opname,
					 bool open_only = false) const;

    //These methods are used in PI_ResourceManagerCommand.C.
    const PI_OpHandleLinkMap &getOpHandleLinks() const { return myOpHandleLinks; }
    PI_PITemplate	*getOpHandleLinkTemplate(const char *pi_name) const;
    const PI_SelectorTemplateMap &getSelectors() const { return mySelectors; }
    PI_SelectorTemplate	*getSelectorTemplate(const char *selector_name) const;

    // The following method is used by builddialogs
    const PI_HandleTemplateMap &getHandles() const { return myHandles; }

    // Tell us if the pi with the given name is an invisible PI by nature.
    // Optionally return the type of the pi in isfloat (int or float assumed)
    bool		 isPIEthereal(const char *piname,
				      bool *isfloat = 0) const;

    // Because the order of callbacks for OP_UpdateTableSink is
    // undefined, some callbacks rely on the resource manager's table of states
    // to be accurate.  Since the resource manager's callback can happen
    // after the other callbacks, they can call these methods to ensure the
    // resource manager's table is accurate.
    void ensureStatesExistForAllOperators(const char *net_type_name);
    virtual void ensureStatesExistForAllOperators(PI_NetworkType net_type);

    // Map a viewer type token to a PI_ViewerType, or vice-versa.
    static bool			 getViewerType(const char *viewer_type_token,
					       PI_ViewerType &type);
    static void			 getViewerTypeToken(PI_ViewerType type,
						UT_String &viewer_type_token);
    
    // Map the network name returned by OP_Node::getOpType() to a
    // PI_NetworkType, or vice-versa.
    static bool		 	 getNetworkType(const char *network_type_name,
						PI_NetworkType &type);
    static const UT_StringHolder &getNetworkTypeName(PI_NetworkType type);
    static OP_OperatorTable	*getOpTableFromOperator(const OP_Operator *op);

    // Choose the first network type from a mask that contains multiple ones.
    static PI_NetworkType	 getFirstNetworkType(unsigned int network_mask);

    static PRM_Template		*findParmTemplate(OP_Operator &op,
						  const char *op_Parm_name,
						  int vec_idx, int *parm_idx);

    // Write out our bindings information to a file.
    int			 writeBindingsFile(const char *fname) const;

    // Load/save/remove the settings for a particular PI and a node that's
    // linked to it.
    void		 loadPISettingsFromTables(PI_OpHandleLink &pi,
				      bool okay_to_use_last_settings) const;
    void		 savePISettingsToTables(PI_OpHandleLink &pi);
    virtual void	 removePISettingsFromTables(OP_Operator &op_type);
    void		 removePISettingsFromTables(const PI_OpHandleLink &pi);

    // Record settings for a particular PI.  This method should only be
    // called by the omparm command.
    void		 recordPISettings(const char *pi_description,
					  const char *pi_type,
					  const OP_Node &op_node,
					  const char *settings);

    // Objects may add callbacks that are called just before the resource
    // manager saves its data to the hip file.  These callbacks update
    // the resource manager to make sure the data it saves is current.
    void		 addSaveCallback(PI_SaveCallback callback, void *data);
    void		 removeSaveCallback(PI_SaveCallback callback,
					    void *data);

protected:
    static void		 startParmsCache();
    static void		 stopParmsCache();
    static void		 getParmsCache(UT_ValArray<OP_Node *> *&ops,
				       UT_ValArray<PRM_Parm *> *&parms,
				       UT_IntArray *&vec_indices);
    static void		 getParmsCacheBehaviour( bool &set_keys,
						 bool &do_scope,
						 bool &append_scope );
public:
    static int		 cacheParms();
    static void		 appendToParmsCache(OP_Node *op, PRM_Parm *parm,
					    int vi);
    static void		 setParmsCacheBehaviour( bool set_keys, bool do_scope,
						 bool append_scope );

    static bool		 getHideLockedHandleParts()
			    { return theHideLockedHandleParts; }
    static void		 setHideLockedHandleParts( bool hide )
			    { theHideLockedHandleParts = hide; }
    static bool		 getScopeChannelsOnKey()
			    { return theScopeChannelsOnKey; }
    static void		 setScopeChannelsOnKey( bool scope )
			    { theScopeChannelsOnKey = scope; }
    static bool		 getAddScopedChannels()
			    { return theAddScopedChannels; }
    static void		 setAddScopedChannels( bool add )
			    { theAddScopedChannels = add; }
    static bool		 getOpenChannelEditorOnScope()
			    { return theOpenChannelEditorOnScope; }
    static void		 setOpenChannelEditorOnScope( bool do_open )
			    { theOpenChannelEditorOnScope = do_open; }
    static void		 getChannelScopeCommandPrefix( UT_String &str );


protected:
    // Classes:

    class PI_API PI_OpPIKey
    {
    public:
	PI_OpPIKey(const char *pi_name_in_setting_table, const char *pi_type,
		   int op_id) :
	   myPINameInSettingTable(pi_name_in_setting_table),
	   myPIType(pi_type),
	   myOpId(op_id) {}

	bool operator==(const PI_OpPIKey &o) const
	{
	    // myPIType is not used.
	    return myPINameInSettingTable == o.myPINameInSettingTable &&
		   myOpId == o.myOpId;
	}
	
	SYS_HashType hash() const
	{
	    // myPIType is not used.
	    SYS_HashType hash = myPINameInSettingTable.hash();
	    SYShashCombine(hash, myOpId);
	    return hash;
	}

	UT_StringHolder	piNameInSettingTable() const { return myPINameInSettingTable; }
	UT_StringHolder	piType() const { return myPIType; }
	int opId() const { return myOpId; }
	
	struct Hasher
	{
	    size_t operator()(const PI_OpPIKey &pk) const { return size_t(pk.hash()); }
	};
	
    private:
	UT_StringHolder	 myPINameInSettingTable;
	UT_StringHolder	 myPIType;
	int		 myOpId;
    };

    // Methods:

    void		 installCommands();
    void		 registerOpHandleLinks();
    void		 registerCopHandleLinks();
    void		 registerOpHandleBindings();
    void		 registerOpHandleConstructors();
    void		 registerSelectors();
    void		 registerSelectorConstructors();

    // Functions used by registerOpHandleLinks for loading the OPbindings file.
    bool		 isValidPiParmName( const UT_String &pi_name,
					    const UT_String &pi_parm);
    void 		 loadHandle(CPP_Manager &cppman, OP_Operator *op,
				    pi_OpHandleBinding *binding);
    void 		 loadSelector(CPP_Manager &cppman,
				      const UT_String &selname,
				      const UT_String &seldescription,
				      pi_OpSelectorBinding *binding);
    void		 loadOperator(CPP_Manager &cppman,
				      const UT_String &oppath);
    void 		 loadOperator(CPP_Manager &cppman,
				      const UT_String &optype,
				      OP_Operator *op,
				      pi_OpHandleBinding *hbinding,
				      pi_OpSelectorBinding *sbinding);

    virtual void	 addAutomaticStates(PI_NetworkType net_type);
    virtual void	 tableUpdated(OP_OperatorTable *table);
    virtual void	 tableDeleted(OP_OperatorTable *table);
    virtual void	 templatesUpdated(OP_Operator *op);
    virtual void	 operatorDeleted(OP_Operator *op);
    void		 ensureTableInterestExists(PI_NetworkType net_type,
						   OP_OperatorTable *table);

    PI_HandleTemplate	*getHandleTemplate(const char *handle_name) const;

    // Save all the PI settings to a stream.
    static OP_ERROR	 savePIResourceManagerCallback(std::ostream &os, 
                   	                               void *data);
    OP_ERROR		 saveSettings(std::ostream &os) const;

    // We need the non-const version of getDefaultPISettings() because we
    // might need to sort the default settings.
    const PI_SettingList *getDefaultPISettings(OP_Operator &op_type,
					       const char *type_name,
					       const char *description) const;
	  PI_SettingList *getDefaultPISettings(OP_Operator &op_type,
					       const char *type_name,
					       const char *description);


    // Data:
    using PI_SaveCallbackInfo = UT_Pair<PI_SaveCallback, void *>;

    UT_StringSet			 myStates;
    PI_HandleTemplateMap	 	 myHandles;
    PI_OpHandleLinkMap	 		 myOpHandleLinks;
    PI_SelectorTemplateMap	 	 mySelectors;

    UT_SymbolMap<pi_OpHandleBinding *>	 myOpHandleBindings;
    UT_SymbolMap<pi_OpSelectorBinding *> myOpSelectorBindings;

    UT_Map<PI_OpPIKey, PI_SettingList *, PI_OpPIKey::Hasher> myPISettingsForOps;
    UT_StringMap<PI_SettingList *>	 myLastPISettings;

    UT_ValArray<PI_StateTemplate *>	 myStateLists[PI_NUM_VIEWER_TYPES]
							[PI_NUM_NETWORK_TYPES];
    UT_SymbolMap<PI_NetworkType>	 myNetworkTypeTable;

    UT_Array<PI_SaveCallbackInfo>	 mySaveCallbacks;

    static bool				 ourSetKeyOnParms;
    static bool				 ourScopeParmChannels;
    static bool				 ourAppendToScope;
    static int				 ourCacheParms;
    static UT_ValArray<OP_Node *>	 ourParmNodes;
    static UT_ValArray<PRM_Parm *>	 ourParms;
    static UT_IntArray			 ourParmVecIndices;

    static bool				 theHideLockedHandleParts;
    static bool				 theScopeChannelsOnKey;
    static bool				 theAddScopedChannels;
    static bool				 theOpenChannelEditorOnScope;
};

PI_API extern PI_ResourceManager 	*PIgetResourceManager();
PI_API extern void	PIcreateResourceManager(bool verbose=true);
PI_API extern bool	PIresourceManagerExists();

#endif
