/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP Library (C++)
 *
 * COMMENTS:
 *	The OP_Director class is a collection (network) of managers.
 *	The Managers register themselves with an active OP_Director
 *	when they are created. The active OP_Director is obtained
 *	via the global function (yuch) OPgetDirector().
 *
 */

#ifndef __OP_Director_h__
#define __OP_Director_h__

#include "OP_API.h"
#include "OP_CommandManager.h"
#include "OP_Network.h"
#include "OP_Node.h"
#include "OP_OTLManager.h"
#include "OP_OperatorTable.h"
#include "OP_ConnectorId.h"
#include <CH/CH_Manager.h>
#include <UT/UT_ColorTable.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_NonCopyable.h>
#include <UT/UT_Thread.h>
#include <UT/UT_ThreadSpecificValue.h>
#include <UT/UT_Tuple.h>
#include <UT/UT_ValArray.h>
#include <SYS/SYS_Types.h>
#include <iosfwd>

#define OP_POSTLOAD_SCRIPT	"456.cmd"

class UT_WorkArgs;
class UT_WorkBuffer;
class UT_Args;
class OP_SaveCallback;
class OP_Input;
class OP_ExprFunc;
class op_ShopClerkInfo;
class OP_BundleList;
class OP_StyleManager;
class OP_Take;
class OP_FileResolver;
class OP_GlobContext;
class PI_EditScriptedParms;
class PRM_ScriptPage;
class UT_Obfuscator;
class OP_EventScriptPathCache;

typedef void (*OP_GlobalOpChangedCallback)(OP_Node *node, OP_EventType reason,
					   void *data, void *cbdata);
typedef UT_ValArray<OP_GlobalOpChangedCallback> OP_GlobalOpChangedCallbacks;

class OP_API OP_Director : public OP_Network, public OP_OTLManagerSink
{
public:    
    enum EventType
    {
	BEGIN_CLEAR_NETWORK,
	END_CLEAR_NETWORK,
	BEGIN_LOAD_NETWORK,
	END_LOAD_NETWORK,
	BEGIN_MERGE_NETWORK,
	END_MERGE_NETWORK,
	BEGIN_SAVE_NETWORK,
	END_SAVE_NETWORK,
	NUM_NETWORK_EVENT_TYPES,
    };
    typedef void (*EventCallback)(EventType type, void *);
    typedef OP_ERROR (*SaveCallback)(std::ostream &, void *);
    
public:
	     OP_Director(const char *name, const fpreal32 version);
    virtual ~OP_Director();

    virtual OP_OpTypeId	 getOpTypeID() const { return DIR_OPTYPE_ID; }
    virtual const char	*getOpType() const { return DIR_OPTYPE_NAME; }
    virtual OP_DataType  getCookedDataType() const { return OP_NO_DATA; }
    virtual void	*getCookedData(OP_Context &) { return 0; }
    virtual void         deleteCookedData() { }
    virtual int		 saveCookedData(std::ostream &, OP_Context &,
					int = 0) { return 1; }
    virtual bool	 loadCookedData(UT_IStream &, const char * /*data*/=0)
					{ return true; }
    virtual int		 saveCookedData(const char *,
					OP_Context &) { return 1; }
    virtual OP_OpTypeId	 getChildTypeID() const { return MGR_OPTYPE_ID; }
    virtual const char	*getChildType() const { return MGR_OPTYPE_NAME; }

    OP_Network		*getManager(const char *name) const;
    CH_Manager		*getChannelManager() { return &myCommandManager; }
    OP_CommandManager	*getCommandManager() { return &myCommandManager; }
    fpreal32		 getVersion() const { return myVersion; }

    // NB: clearNetwork() will always reload the channel manager options!
    //	   However, the user is responsible as to when we save them
    bool		 loadChannelManagerOptions();
    bool		 saveChannelManagerOptions();

    virtual void	 enable(int state);

    OP_ERROR		 saveNetwork(std::ostream &os, 
            		             const OP_SaveFlags &options);
    bool		 loadNetwork(UT_IStream &is, int merge=0,
				     const char *mergePattern=0,
				     int overwrite=0);
    void		 initializeManagerWireStyles();
    virtual void	 clearNetwork( int clear_aliases );

    // This will go through the entire hip file and cook all the CHOPs
    // that have their export flags set and cook them with CookOverride
    // turned on.
    virtual void	 opUpdateAllExports();

    void		 setOldCHOPOverrideFlag() 
			 { myOldCHOPOverrideFlag = true; }
    bool		 getOldCHOPOverrideFlag() 
			 { return myOldCHOPOverrideFlag; }
    void		 clearOldCHOPOverrideFlag() 
			 { myOldCHOPOverrideFlag = false; }

    /// Create an evaluation context scope with a new node
    class CwdScope;

    OP_Node		*getCwd()
			    { return getCwd(SYSgetSTID()); }
    void		 getCwd(UT_String &str)
			    { getCwd(SYSgetSTID(), str); }

    OP_Node		*getCwd(int thread);
    void		 getCwd(int thread, UT_String &str);
    void		 setCwd(int thread, const OP_Node *node,
				int eval_collection=-1);

    // pushCwd() pushes the current cwd onto a stack that can later be restored
    // using popCwd(). The optional new_cwd parameter sets the cwd to it at
    // the same time.
    void		 pushCwd(int thread, const OP_Node *new_cwd = NULL);
    void		 popCwd(int thread);

    void		 setEditNetworkUI(OP_Network *net);

    bool		 getPickedNodes(OP_NodeList &picked_nodes,
				bool include_hidden=false,
				bool append_to_list=false) const;
    bool		 getPickedItems(OP_NetworkBoxItemList &picked_items,
				bool include_hidden=false) const;
    bool		 getPickedInputs(
				UT_Array<OP_Input *> &picked_inputs) const;
    OP_Node		*getLastPickedNode(OP_Node *parent = NULL) const;
    const OP_ItemIdList	&getPickedItemIds() const;

    void		 clearPickedItems();

    // Pick the given item, clearing all other picked items first. This is here
    // so that we can prevent an extra undo if the current state already
    // matches what we want.
    void		 makeOnlyPickedCurrent(OP_NetworkBoxItem &item);

    // These next 3 methods should only be called from OP_Node::setPicked()
    // The myPickedItems array can't contain duplicates and maintains the
    // invariant that:
    // - node.getPicked() is TRUE iff it is in the list.
    // - node.getPicked() is FALSE implies that it is NOT in the list.
    void		 addPickedNode(const OP_Node &node, bool edit);
    void		 removePickedNode(const OP_Node &node, bool edit);
    void		 makePickedNodeLast(const OP_Node &node, bool edit);

    // These methods should only be called from the various
    // setPicked() functions in OP_NetworkBoxItem subclasses. The
    // myPickedXXXIds arrays can't contain duplicates and maintain the
    // invariant that:
    // - item.getPicked() is TRUE iff it is in the list.
    // - item.getPicked() is FALSE implies that it is NOT in the list.
    void		 addPickedItem(const OP_NetworkBoxItem &item);
    void		 removePickedItem(const OP_NetworkBoxItem &item);
    void		 itemBeingDeleted(const OP_NetworkBoxItem &item);

    // These methods should only be called from OP_Input::setPicked(). The
    // myPickedXXXIds arrays can't contain duplicates and maintain the
    // invariant that:
    // - input.getPicked() is TRUE iff it is in the list.
    // - input.getPicked() is FALSE implies that it is NOT in the list.
    void		 addPickedInput(const OP_Input &input);
    void		 removePickedInput(const OP_Input &input);

    // Control time.
    void		 setTime(fpreal time);
    fpreal		 getTime() const;
    void		 setFPS(fpreal fps);
    void		 setNFrames(fpreal nframes);
    void		 setTimeRange(fpreal tstart, fpreal tend);

    /// The save callback is called when saving .hip files. It is called with
    /// a stream reference to the .hip file so that the callback can insert
    /// hscript commands into the .hip file's .application section. When a
    /// .hip file is loaded, all the commands in the .application section are
    /// executed.
    // @{
    void		 setSaveCallback(SaveCallback cb, void *cb_data);
    void		 removeSaveCallback(SaveCallback cb, void *cb_data);

    /// Callbacks for certain global OP_Director events, such as new scene 
    /// and load scene.
    void		 addEventCallback(EventType type, EventCallback cb, 
        		                  void *data);
    void		 removeEventCallback(EventType type, EventCallback cb);
    // @}

    void		 setUserDisplayOptionSaveCallback(
				OP_ERROR (*cb)(std::ostream &, void *),
				void *callbackData);
    void		 setSceneFileVisualizersSaveCallback(
    				OP_ERROR (*cb)(std::ostream &, void *),
    				void *callbackData);
    void		 addBadLoadData(char *str, int len);

    int			 isLoading() const  { return (myInLoadCount > 0); }
    virtual void	 beginLoading();
    virtual void	 endLoading();
    bool		 getIsLoadingHip() const
			 { return myIsLoadingHip; }
    bool		 getIsQuitting() const
			 { return myIsQuitting; }
    void		 setIsDoingExplicitSave(bool value)
			 { myIsDoingExplicitSave = value; }
    bool		 getIsDoingExplicitSave() const
			 { return myIsDoingExplicitSave; }
    bool		 getIsAnyNodeCooking() const;

    bool		 getIsMantra() const
			 { return myIsMantra; }
    void		 setIsMantra(bool v)
			 { myIsMantra = v; }

    void		 destroyFileResolver();

    void		 beginTakeSwitch();
    void		 endTakeSwitch();
    bool		 isSwitchingTake() const
					    { return mySwitchingTake; }

    virtual int		 renameNode(OP_Node *, const char *, 
				    OP_RenameAction);

    // These methods are only called by the network load/save
    void		 saveChannelGroups(std::ostream &os, int binary);
    bool		 loadChannelGroups(UT_IStream &is, const char *path);
    void		 saveTimeGroups(std::ostream &os, int binary);
    bool		 loadTimeGroups(UT_IStream &is, const char *path);
    void		 saveScriptedOpLibrary(std::ostream &os);
    bool		 loadScriptedOpLibrary(UT_IStream &is, int modtime);
    bool		 loadScriptedOpDefs(UT_IStream &is);
    void		 saveInternalOpLibraries(std::ostream &os);
    bool		 loadInternalOpLibraries(UT_IStream &is);
    void		 saveLibraryPreferences(std::ostream &os);
    bool		 loadLibraryPreferences(UT_IStream &is);
    bool		 loadHOMSessionModuleSource(UT_IStream &is);
    void		 saveUnselectedFilePatterns(std::ostream &os);
    bool		 loadUnselectedFilePatterns(UT_IStream &is);

    void		 saveOTLBackupInformation(std::ostream &os);

    OP_ExprFunc		*getExprFunctions();
    void		 saveExprFunctions(std::ostream &os);
    bool		 loadExprFunctions(UT_IStream &is, const char *path);

    OP_BundleList	*getBundles()	{ return myBundles; }

    // These track the current update mode: Should an OP recook?
    int			 cookEnabled() const
			 { return myCookEnabled; }
    void		 setCookEnabled(int state)
			 { myCookEnabled = state; }

    // These track the simulation update mode: Should simulations cook?
    int			 simulationEnabled() const
			 { return mySimulationEnabled; }
    void		 setSimulationEnabled(int state)
			 { mySimulationEnabled = state; }

    // These control whether those simulations reset by output drivers should
    // reset based on the global or playback playbar range.
    bool		 skipPlaybarBasedSimulationReset() const
			    { return mySkipPlaybarBasedSimulationReset > 0; }
    void		 bumpSkipPlaybarBasedSimulationReset(int inc)
			    { mySkipPlaybarBasedSimulationReset += inc; }

    // These methods are used by SHOPs to keep track of visible clerks.
    bool		 addShopClerk(const char *type);
    int			 getNShopClerks() const
			 { return myShopInfo.entries(); }
    const char		*getShopClerkName(int clerk) const;
    int			 getShopClerkVisibility(int clerk) const;
    void		 setShopClerkVisibility(int clerk, int onoff);
    bool                 isCompiledLibrary (OP_OTLLibrary *lib);

    // This function does nothing at this level, but it is meant to be
    // overridden to create new OP_OperatorInfos for the given table based
    // on the existance of VOP networks.
    virtual void	 findInternalScriptOperators(
				const char * /* indexPath */,
				OP_OTLDefinitionArray & /* defs */)
			 { }
    virtual OP_Node	*linkInternalScriptOperator(
				const char * /* definitionOp */,
				OP_Operator * /* op */)
			 { return 0; }
    virtual void	 getInternalScriptDefinition(
				OP_Node * /* definitionOp */,
				UT_String & /* definition */)
			 { }
    virtual void	 updateExportedParms(
				OP_Node * /* definitionOp */)
			 { }
    // Save the spare parameters section for the specified node. We have
    // to do this through a virtual function that is actually implemented
    // in the MOT library because it requires the PI_EditScriptedParms
    // class.
    virtual OP_ERROR	 saveNodeSpareParms(OP_Parameters *,
					    bool,
					    std::ostream &) const
			 { return UT_ERROR_NONE; }
    // Load the spare parms section for a node. Although it doesn't require
    // the PI library, this function is also implemented in the MOT library
    // just to keep the save and load code paths in a similar location.
    virtual bool	 loadNodeSpareParms(OP_Parameters *,
					    UT_IStream &,
					    UT_String &) const
			 { return true; }
    // Adds a single spare parm to the specified node. This function exists
    // for backward compatibility with spare parms from H8. This function is
    // implemented in the MOT library where we have access to the PI classes
    // for editing parms (PI_EditScriptedParms).
    virtual bool	 addNodeSpareParm(OP_Parameters * /*node*/,
					  PRM_Template * /*parmtemplate*/) const
			 { return false; }
    // Removes a single spare parm to the specified node. This function exists
    // for backward compatibility with spare parms from H8. This function is
    // implemented in the MOT library where we have access to the PI classes
    // for editing parms (PI_EditScriptedParms).
    virtual bool	 removeNodeSpareParm(OP_Parameters * /*node*/,
					     const char * /*parmname*/,
					     UT_StringArray * /*errs*/ = NULL,
					     UT_StringArray * /*warn*/ = NULL
					     ) const
			 { return false; }
    // Updates the spare parms of a node to match the supplied
    // PI_EditScriptedParms. This class isn't defined until the PI
    // library, so this function is actually implemented in MOT_Director.
    virtual void	 changeNodeSpareParms(OP_Parameters *,
					      PI_EditScriptedParms &,
					      UT_String &) const
			 { }
    // Save a series of hscript commands to a stream to replicate the
    // spare parameters of the specified node.
    virtual void	 saveNodeSpareParmCommands(OP_Parameters *,
                	                           std::ostream &,
						   const char *,
						   bool) const
			 { }
    // Delete spare parameter/base parameter mixing layout. This puts the
    // spare parameters on their own page.
    virtual void	 deleteNodeSpareParmLayout(OP_Parameters *) const
			 { }
    // Delete all spare parameters on a node.
    virtual void	 deleteAllNodeSpareParms(OP_Parameters *) const
			 { }

    virtual PRM_ScriptPage	*allocateSpareScriptPage(OP_Operator *)
			 { return 0; }

    // Functions for saving and loading compiled VEX and RSL code for nodes
    // with VOP_CodeGenerators in a compiled hip file.
    virtual void	 saveNodeCompiledCode(OP_Node *, std::ostream &, 
				bool is_source_code = false)
			 { }
    virtual bool	 loadNodeCompiledCode(OP_Node *, UT_IStream &,
				bool is_source_code = false)
			 { return true; }
    virtual void	 saveNodeCompiledDs(OP_Node *, std::ostream &)
			 { }
    virtual bool	 loadNodeCompiledDs(OP_Node *, UT_IStream &)
			 { return true; }

    OP_Take		*getTakeManager()	{ return myTakeManager; }
    OP_StyleManager	*getStyleManager()	{ return myStyleManager; }
    UT_ColorTable	&getGroupColorTable()	{ return myGroupColorTable; }

	// deprecated method: interest gets cleared out before every cook
    virtual void	 addExtraInput(OP_Node *op, OP_InterestType type);

    // A utility function to glob (pattern expand) all arguments on a command
    // line.
    static int		 globAllArguments(OP_Node *node, UT_Args &args,
					  UT_WorkBuffer &workbuf,
					  UT_WorkArgs &argv,
					  OP_ItemTypeMask item_type,
					  OP_GlobContext *context,
					  int startitem = 1);

    // This function is similar to the one above, except it takes a string
    // pattern (which may contain spaces) and returns an array of matching
    // nodes and/or netboxes.
    static void		 globAllArgumentsAsArray(OP_Node &relative_to_node,
				    const char *pattern,
				    OP_ItemTypeMask item_type,
				    OP_GlobContext &glob_context,
				    UT_ValArray<OP_NetworkBoxItem *> &result);

    // These functions are the instantiations of the virtual functions
    // from our OP_OTLManagerSink base class.
    virtual void	 definitionsAdded(int libindex,
					  UT_IntArray &defindex);
    virtual void	 definitionsRemoved(int libindex,
					    UT_IntArray &defindex,
					    OP_OTLLibrary *&preserve);

    // Given a path of the form "tablename/opname", return the table and
    // operator indicated by the path. The relativetonode parm is used when
    // the path is actualy a node path, and we want the table and operator
    // of the node specified by the path. If it is a relative path, the
    // relativetonode is the starting point. If it is null, the current
    // working node is used, or the director.
    void		 getTableAndOperator(const char *path,
					     OP_OperatorTable *&table,
					     OP_Operator *&op,
					     const OP_Node *relativetonode = 0);
    OP_OTLManager	&getOTLManager()
			 { return myOTLManager; }
    const OP_OTLManager	&getOTLManager() const
			 { return myOTLManager; }

    // This function should be called right before exiting. It will go
    // through all OP_Operators with active nodes and run the POSTLASTDELETE
    // event script (though it doesn't actually delete the nodes).
    void		 runDeleteScriptsOnQuit();

    // Add or remove callback functions that get executed whenever we call
    // OP_Node::opChanged.
    void		 addGlobalOpChangedCallback(
			    OP_GlobalOpChangedCallback cb, void *cbdata);
    void		 removeGlobalOpChangedCallback(
			    OP_GlobalOpChangedCallback cb, void *cbdata);
    // This function is called any time OP_Node::opChanged is called. It sends
    // the change information along to any global op change callback functions
    // that have been registered by addGlobalOpChangeCallback.
    void		 globalOpChanged(OP_Node *node,
					 OP_EventType reason,
					 void *data);

    // This function creates a backup of the specified file, if the option
    // to save backup files is turned on. The actual file name and file type
    // are irrelevent. It returns true if the backup was successful (or was
    // not required).
    static bool		 createBackupFile(const char *filename,
					  bool domove= false);
    // This function will rename the current file "filename" to the next
    // filename in the sequence to free up "filename" so that it can be
    // saved over.
    static void		 autoIncrementRename(const char *filename,
					     UT_String &errorstring,
					     bool movefile,
					     bool autosave);

    void		 runScript(CMD_Manager *cman, const char *filename);
    UT_Tuple<bool, int>	 runPython(CMD_Manager *cman, const char *filename);
    UT_Tuple<bool, int>	 runPython(CMD_Manager *cman, int argc,
				      char *argv[]);
    UT_Tuple<bool, int>	 runDefaultStartupScripts(CMD_Manager *cman, 
					const char *startup_script,
					const char *postload_script);

    // Get the global (i.e. non-operator specific) script path cache.
    OP_EventScriptPathCache	*getGlobalEventScriptPathCache()
				    { return myGlobalEventScriptPathCache; }

    // Clear all cached paths to global event scripts.
    void			 clearGlobalEventScriptPathCache();

    // Query all cached global event script paths.
    void			 getCachedGlobalEventScriptPaths(
						UT_StringArray &events,
						UT_StringArray &paths,
						bool only_nonempty) const;

    // Because of code complexity issues, this method must be used to destruct
    // an OP_Director.
    static void	destroyDirector(OP_Director *opdir);

    bool			 getSaveSceneAsText() const;
    void			 setSaveSceneAsText(bool onOff);
    bool			 getNewFileSaveSceneAsText() const;
    void			 setNewFileSaveSceneAsText(bool onOff);

    /// Run callbacks for before saving a scene,
    /// 'autosave' indicates if the save was triggered by the autosave timer.
    void			 notifyBeforeSceneSave(
					const UT_StringHolder &filename,
					bool autosave);
    /// Run callbacks for before saving a scene,
    /// 'autosave' indicates if the save was triggered by the autosave timer.
    /// 'did_succeed' indicates if the save was successful.
    void			 notifyAfterSceneSave(
					const UT_StringHolder &filename,
					bool autosave,
					bool did_succeed);

    static UT_Obfuscator	*getObfuscator();

protected:
    virtual OP_ERROR	 cookMe(OP_Context &) { return UT_ERROR_NONE; }
    virtual OP_ERROR	 bypassMe(OP_Context &, int &) { return UT_ERROR_NONE; }
    virtual const char	*getFileExtension(int binary) const
			 { return binary ? ".bhip" : ".hip"; }
    void		 clearUnsupportedCommands() const;

    // This function is called when we update our OTL for an operator. It
    // does nothing at this level, but in MOT it will clear out the PI
    // settings that have been saved for the specified operator type.
    virtual void	 clearHandleSettings(OP_Operator *) { }

    // We don't allow spare parms on the director, so just ignore this call.
    virtual bool	 changeSpareParms(UT_IStream &, UT_String &errors)
			 {
			     errors += "Spare parameters are not alowed on ";
			     errors += "the root node.";
			     return false;
			 }

    /// Runs the user-defined post load script when loading
    /// a new network into Houdini.
    void		 runPostLoadScript();

private:
    // Disallow copying of OP_Director, these methods are not implemented
    // on purpose
    OP_Director(const OP_Director &copy);
    OP_Director &operator =(const OP_Director &copy);

    void		 installCommands();
    void		 installMoreCommands();
    void		 installCHCommands();
    void		 installOTLCommands();
    void		 installOpRampResolver();

    void		 savePreCommands(std::ostream &os);
    void		 savePostCommands(std::ostream &os);

    void		 notifyEventCallbacks(EventType type);

    /// Helper function to update to a new operator definition.  May delete
    /// the old operator and return a different one.
    void		 updateNewDefinition(
			    OP_Operator *&op,
			    OP_OTLLibrary *newlib,
			    int newdefindex);

    /// Helper function for runDefaultStartupScripts().
    /// Pass back the full path of the first startup script file
    /// found in the Houdini path.
    static void		 getFirstFoundStartupScriptFile(
			    UT_StringHolder &script_path,
			    const char *script_file_name);

    /// Helper function for runDefaultStartupScripts().
    /// Passes back the full path of the first finding of inputpath
    /// in the script search directories.  The file name of outputpath
    /// may end in any of the acceptable script file extensions
    /// (i.e. .py or .cmd).
    static void		 getFirstFoundScriptFile(
			    UT_StringHolder &outputpath,
			    const char *inputpath);

    static void		 sceneSaveEvent(EventType event, UT_Args &args);

private:
    char				*myBadLoadData;
    int					 myBadLoadDataLen;
    OP_CommandManager			 myCommandManager;
    fpreal32				 myVersion;
    int					 myInLoadCount;

    struct CwdContext
    {
	CwdContext() : myCwdId(-1)
	{
	}
	int				 myCwdId;
	UT_IntArray			 myCwdStack;
	UT_IntArray			 myEvalCollectionStack;
    };
    UT_ThreadSpecificValue<CwdContext>	 myCwdContext;

    OP_ItemIdList					 myPickedItems;
    UT_Array<std::pair<OP_ItemId, OP_ConnectorId> >	 myPickedInputs;

    OP_SaveCallback			*myCallbacks;
    OP_SaveCallback			*myUserDisplayOptionCallback;
    OP_SaveCallback			*mySceneFileVisualizersCallback;
    OP_BundleList			*myBundles;
    OP_Take				*myTakeManager;
    OP_StyleManager			*myStyleManager;
    OP_FileResolver			*myFileResolver;
    UT_ColorTable			 myGroupColorTable;

    int					 myCookEnabled;
    int					 mySimulationEnabled;
    int					 mySkipPlaybarBasedSimulationReset;
    UT_ValArray<op_ShopClerkInfo *>	 myShopInfo;

    OP_OTLManager			 myOTLManager;
    OP_OperatorTableList		 myTablesToReload;
    UT_StringSet			*myDummyDefinitionActivations;

    OP_GlobalOpChangedCallbacks		 myGlobalOpChangedCallbacks;
    UT_ValArray<void *>			 myGlobalOpChangedCallbackData;

    struct EventCallbackInfo
    {
	EventCallback		 cb;
	void			*data;
    };
    typedef UT_Map<EventCallback, void *> EventCallbackInfoMap;
    typedef UT_Map<EventType, EventCallbackInfoMap> EventCallbackMap;
    EventCallbackMap			 myEventCallbacks;

    // Cache of global (i.e. non-operator specific) event script paths.
    OP_EventScriptPathCache		*myGlobalEventScriptPathCache;

    // This flag tracks whether any OPs that got loaded had the old
    // style override flag.  If so, we know we will have to do a complete
    // update.
    bool				 myOldCHOPOverrideFlag;
    // This flag tracs whether or not we are inside a call to loadNetwork.
    // This is different from the isLoading flag, which gets set during any
    // network load.
    bool				 myIsLoadingHip;
    bool				 mySwitchingTake;
    // This value is temporarily set to true when the user explicitly asks
    // for a hip file save either by executing an mwrite, or choosing Save
    // or Save As from the File menu.
    bool				 myIsDoingExplicitSave;
    // This flag is set to true while Houdini is exiting.
    bool				 myIsQuitting;
    
    // This flag is set when the OP director is created for mantra.  There are
    // some OP features which are expensive (i.e. importing the hou python
    // module).  This option disables those features.
    bool				 myIsMantra;

    bool				 mySaveSceneAsText;
    bool				 myNewFileSaveSceneAsText;

    static UT_Obfuscator		 *theObfuscator;
};

OP_API extern OP_Director	*OPgetDirector();
OP_API extern OP_Director	*OPsetDirector(OP_Director *boss);

/// Create an evaluation context scope with a new node
class OP_API OP_Director::CwdScope : UT_NonCopyable
{
public:
    CwdScope(int thread, const OP_Node &new_cwd);
    ~CwdScope();
private:
    int			    myThread;
    CH_EvalContext::Scope   myEvalScope;
    int			    myOldCwdId;
};

#endif
