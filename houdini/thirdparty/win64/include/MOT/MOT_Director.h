/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	MOT library (C++)
 *
 * COMMENTS:	This is the OP_Director class for all of MOT files
 *
 */

#ifndef __MOT_Director_h__
#define __MOT_Director_h__

#include "MOT_API.h"
#include <OP/OP_Director.h>
#include <UT/UT_UniquePtr.h>

class MGR_Node;
class LOP_Network;
class CHOP_Node;
class CMD_Args;
class MOT_ScriptNodeResolver;

class MOT_API MOT_Director : public OP_Director
{
public:
	     MOT_Director(const char *appname, bool alloptypes = true,
			  bool exec_pythonrc = true, 
			  bool exec_startup_scripts = false);
    virtual ~MOT_Director();

    virtual OP_Node	*getChild(const char *name, int *hint=0) const;
    virtual OP_Node	*getChild(int index) const;

    static const char	*theChildTableName;

    virtual void	 clearNetwork( int clear_aliases );

    MGR_Node		*getObjectManager()	{ return myObjects; }
    MGR_Node		*getCopManager()	{ return myCop; }
    MGR_Node		*getRenderManager()	{ return myRops; }
    MGR_Node		*getParticleManager()	{ return myParticles; }
    MGR_Node		*getChopNetManager()	{ return myChopNet; }
    MGR_Node		*getShopManager()	{ return myShops; }
    MGR_Node		*getVopManager()	{ return myVops; }
    MGR_Node		*getMatManager()	{ return myMats; }
    LOP_Network		*getLopManager()	{ return myLops; }

    const UT_String	&getFileName() const { return myFileName; }
    void		 setFileName(const char *nm);
    void		 bumpFileName();
    void		 setSaveTime();

    // A simple wrapper around OP_Director::autoIncrementRename() to avoid
    // doing anything when we don't have write access to our filename.  It
    // returns false in this case, and true if autoIncrementRename() was
    // called, whether or not it succeeded.
    bool		 moveFileNameToNumberedBackup(UT_String &errorstring,
						      bool autosave);

    // Clean up for the new file.  We don't call runPostNewScript automatically
    // to give the caller time  to perform additional cleanup first, so be sure
    // to call it later.
    void		 resetForNewFile();

    // Call after resetForNewFile() and any additional related cleanup.  The
    // script execution will not change our state to modified.
    void		 runPostNewScript();

    // Called prior to and after loading a hip file.
    virtual void	 beginLoading();
    virtual void	 endLoading();

    // This function does nothing at this level, but it is meant to be
    // overridden to create new OP_OperatorInfos for the given table based
    // on the existance of VOP networks.
    virtual void	 findInternalScriptOperators(const char *indexPath,
					    OP_OTLDefinitionArray &defs);
    virtual OP_Node	*linkInternalScriptOperator(const char *definitionOp,
					    OP_Operator *op);
    virtual void	 getInternalScriptDefinition(OP_Node *definitionOp,
					    UT_String &definition);
    virtual void	 updateExportedParms(OP_Node *definitionOp);

    // Here we actually implement the saving and loading of the spare parms
    // definition sections for a node.
    virtual OP_ERROR     saveNodeSpareParms(OP_Parameters *node,
					    bool compiled,
					    std::ostream &os) const;
    virtual bool	 loadNodeSpareParms(OP_Parameters *node,
					    UT_IStream &is,
					    UT_String &errors) const;
    // Here we implement the functions for adding and removing a single
    // spare parameter from a node. By implementing the functions here we
    // have access to the PI_EditScriptedParms class.
    virtual bool	 addNodeSpareParm(OP_Parameters *node,
					  PRM_Template *parmtemplate) const;
    virtual bool	 removeNodeSpareParm(OP_Parameters *node,
					     const char *parmname,
					     UT_StringArray *errs = NULL,
					     UT_StringArray *warn = NULL) const;
    // Implement the OP_Director convenience function for changing the
    // spare parameters of a node to match a PI_EditScriptedParms.
    virtual void	 changeNodeSpareParms(OP_Parameters *node,
					      PI_EditScriptedParms &parms,
					      UT_String &errors) const;
    // Implement saving out commands for replicating the spare parameters
    // of a node for use by the opscript command.
    virtual void	 saveNodeSpareParmCommands(OP_Parameters *node,
                	                           std::ostream &os,
						   const char *nodename,
						   bool saveempty) const;
    // Delete spare parameter/base parameter mixing layout. This puts the
    // spare parameters on their own page.
    virtual void	 deleteNodeSpareParmLayout(OP_Parameters *node) const;
    // Delete all spare parameters on a node.
    virtual void	 deleteAllNodeSpareParms(OP_Parameters *node) const;

    // Implement allocation of the PRM_ScriptPage.  This is done when the node
    // parses the spare parameters from a dialog script.  However, for
    // callbacks and other features, we need to parse a script page from PI.
    virtual PRM_ScriptPage	*allocateSpareScriptPage(OP_Operator *op);

    // Functions for saving and loading compiled VEX and RSL code for nodes
    // with VOP_CodeGenerators in a compiled hip file.
    virtual void	 saveNodeCompiledCode(OP_Node *node, std::ostream &os,
				bool is_source_code = false);
    virtual bool	 loadNodeCompiledCode(OP_Node *node, UT_IStream &is,
				bool is_source_code = false);
    virtual void	 saveNodeCompiledDs(OP_Node *node, std::ostream &os);
    virtual bool	 loadNodeCompiledDs(OP_Node *node, UT_IStream &is);

    // createNewOp does NOT call runCreateScript for the Op
    static OP_Node	*createNewOp(const OP_Network *net, const char *opname,
	    			const char *node_name=NULL);
    //  If the current network has a sub-network operator the method
    //  hasSubNetworks returns the name of that operator. This is used
    //  to determine if the createSubNetwork method can be used in order
    //  to build sub-nets from selected nodes.
    static const char	*hasSubNetworks(OP_Network *net);

    //  The buildSubNetwork callback does all the cutting and pasting
    //  required to convert the selected nodes into a sub-network.
    //  OPUI_App::clearDialogPointers() should be called before buildSubNetwork.
    //  This returns the created subnetwork, if any. It has been made
    //  current, so the caller will likely want to do pathNodeChanged
    //  on it.
    //  If from_nodes is provided, those nodes are used instead of the selected
    //  nodes.  The from_nodes array may be modified to remove illegal nodes.
    //  If subnet_type_name is provided, that't the subnet type that will
    //  be created, instead of the primary subnet operator type.
    static OP_Network 	*buildSubNetwork(OP_Network *net, UT_String &err,
	    			const OP_NodeList *from_nodes = nullptr,
	    			const OP_NetworkBoxItemList *from_items=nullptr,
				const char *preferred_subnet_name = nullptr,
				const char *subnet_type_name = nullptr);

    // The extractSubNetwork callback does all the cutting and pasting
    // required to convert the selected sub-network into a set of nodes.
    static void		 extractSubNetwork(OP_Network *subnet,
				OP_NetworkBoxItemList &extracted_items,
				UT_String &err);

    // This will go through the entire hip file and cook all the CHOPs
    // that have their export flags set and cook them with CookOverride
    // turned on.
    virtual void	 opUpdateAllExports();

    // Called when we become (or stop being) the main director.
    virtual void	 enable(int state);
    
    // Returns false if an error has occurred and writes error messages to 
    // warnings_or_errors.
    // Returns true if file loaded or merged successfully, but
    // warnings_or_errors may contain warning messages.
    bool		 loadOrMergeHipFile(const char *file_name, 
				bool merge, const char *pattern, 
				bool overwrite, 
				UT_String &warnings_or_errors);

    // Returns an error message if an error has occurred
    // Returns an empty string if file opened/checked successfully
    void		 getCollisionsIfMerged(const char *file_name,
				OP_NodeList &node_collisions, 
				UT_ValArray<OP_NetworkBox *> 
				&netbox_collisions,
				UT_ValArray<OP_PostIt *> 
				&pnote_collisions,
				const char *pattern,
				UT_String &errors);
    
    // Saves the current hip file to the given file name
    void		 saveHipFile(const char *file_name,
				bool channels_only, UT_String &errors);

    // Returns the status of MOT_Director: true if MOT_Director is
    // loading hip file, doing explicit save, or shutting down.
    // This method should be used for verifying MOT_Director is not busy 
    // before calling loadOrMergeHipFile, getCollisionsIfMerged, 
    // and saveHipFile methods.
    bool		 isBusy(UT_String *msgs = nullptr);

protected:
    // Clears out the saved PI settings when we update our definition.
    virtual void	 clearHandleSettings(OP_Operator *op);

private:
    void	 findInternalScriptOperators(const char *indexPath,
					     OP_OTLDefinitionArray &defs,
					     OP_Network *net);
    void	 layoutManagers();
    void	 execPythonRCFiles();
    void	 installCommands();
    void	 loadOpCustomizeFile();
    void	 cacheChopnetOperators();
    bool	 forceChopnetToExport(OP_Node &chop_network);

    MGR_Node					*myObjects;
    MGR_Node					*myCop;
    MGR_Node					*myRops;
    MGR_Node					*myParticles;
    MGR_Node					*myChopNet;
    MGR_Node					*myShops;
    MGR_Node					*myVops;
    MGR_Node					*myMats;
    LOP_Network					*myLops;

    UT_String					 myFileName;

    // We cache the chopnet operators to speed up the process of forcing
    // chop overrides to update when a node is added/renamed.
    OP_OperatorList				 myChopnetOperators;

    UT_UniquePtr<MOT_ScriptNodeResolver>	 myScriptNodeResolver;

    mutable UT_Map<uint32, UT_UniquePtr<PI_EditScriptedParms> >
						 mySpareParmsCache;
};

#endif
