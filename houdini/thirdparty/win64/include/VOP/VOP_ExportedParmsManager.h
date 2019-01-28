/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP_ExportedParmsManager.h (VOP Library, C++)
 *
 * COMMENTS:
 *
 *		Manages the exported parameters and layout for the owning code
 *		generator or subnet.
 */

#ifndef __VOP_ExportedParmsManager_h__
#define __VOP_ExportedParmsManager_h__

#include "VOP_API.h"

#include "VOP_CodeGenerator.h"
#include "VOP_ExportedParmLayoutMicroNode.h"
#include "VOP_ExportedParms.h"
#include "VOP_Node.h"
#include "VOP_ParmDSMicroNode.h"

#include <PI/PI_OldParms.h>
#include <UT/UT_Assert.h>
#include <UT/UT_UniquePtr.h>
#include <UT/UT_WorkBuffer.h>

class UT_StringArray;
class OP_Network;
class VOP_CodeGenContext;
class VOP_ExportedParmsManager;
class VOP_ParmContextInfo;
class VOP_ParmGenerator;

/// Helper function for returning the exported parms manager for a give node.
/// Returns NULL if the node does not have a manager.
VOP_API VOP_ExportedParmsManager *VOPgetExportedParmsManager(OP_Node *node);

/// Manager of parameters exported by parameter VOPs contained in the owner
/// network.
class VOP_API VOP_ExportedParmsManager
{
public:
    explicit VOP_ExportedParmsManager(VOP_CodeGenerator *code_gen);
    explicit VOP_ExportedParmsManager(VOP_Node *owner_subnet);
    virtual ~VOP_ExportedParmsManager();

    /// Return the network that owns the exported parameters.
    inline OP_Network *		ownerNetwork() const;

    /// Return true if the manager is for code parameters
    /// and false otherwise (i.e. for subnet-scoped parameters).
    bool			isForCodeParms() const
				    { return ownerCodeGen_() != NULL; }

    // Get a list of all Parameter type VOPs.
    void			getParmNodes(
				    VOP_ExportedParmList &list, 
				    bool includeInvisible);

    /// Obtains a list of all parameter nodes for the material (if it is 
    /// a material vopnet) or for the shader (if it is just a shader)
    void			getParticipatingParametersAndDeclarations( 
				    const VOP_NodeList &nodes, 
				    const VOP_CodeGenContext &codegen_ctx,
				    const char * scope,
				    UT_StringArray *declarations,
				    UT_StringArray *comments,
				    VOP_ExportedParmList *parms);

    /// Get a list of Parameter VOPs that define class member variables.
    void			getClassMemberVariableParameterNodes(
				    VOP_ExportedParmList &list,
				    UT_StringArray *declarations=NULL,
				    UT_StringArray *comments=NULL,
				    UT_StringArray *access_level=NULL);

    /// Returns parm nodes that define shader class parameters.
    void			getClassShaderParameterNodes(
				    VOP_NodeList &parms);

    /// Return the dialog script for the code generator's
    /// exported parameter layout.  The layout DS differs from the
    /// regular parm DS because it contains the owner node's complete
    /// parameter layout including spare parameters, folders and separators.
    const char *		getParmLayoutDialogScript() const;

    /// Return true if there is a VOP node which defines a parameter
    /// with the given name.
    bool			hasParmNode(
				    const char *parm_name,
				    bool use_cached_parms=false) const;

    /// Return a pointer to the Parameter VOP node that corresponds
    /// to the given parameter name.  Return NULL if no such node
    /// exists.
    VOP_ParmGenerator *		getParmNode(
				    const char *parm_name,
				    bool use_cached_parms=false) const;

    /// @{
    /// A flag to indicate if the cached parameters may have outdated parm
    /// tag that specifies parameter's shading contexts. Only shop clerk
    /// is interested in this tag and currently manages the updating itself.
    /// TODO: Remove tag_dirty argument.
    void			setContextParmTagDirty(bool tag_dirty=true)
				{
				    myContextParmTagDirty = tag_dirty;
				}
    bool			isContextParmTagDirty() const
				{
				    return myContextParmTagDirty;
				}
    /// @}

    /// Set the locked compiled dialog script. When set, the locked dialog
    /// script causes the manager ignore the VOP nodes in the owner network 
    /// and always return the locked value.
    void			setLockedCompiledDS(const char *dialog_script);

    /// This must be called when the owner network has finished loading
    /// (i.e. VOP_CodeGenerator::ownerFinishedLoadingNetwork()
    /// or VOP_SubnetBase::ownerFinishedLoadingNetwork()).
    void			ownerFinishedLoadingNetwork();

    /// Return the spare parameter layout that is exported to the owner network.
    /// The layout contains a mix of exported and regular parameters.
    void			getOwnerSpareParmLayout(
				    PI_EditScriptedParms &parm_layout);

    /// Return the parameter layout to be used for the owner's HDA's dialog
    /// script.
    ///
    /// You can optionally set `use_incoming_layout` to true if you want 
    /// to use the layout already defined by `parm_layout` instead of 
    /// the parameter layout stored in the owner network.  In that case, 
    /// the exported parameters are merged into the incoming layout with new
    /// parameters added to the end.
    void			getOwnerHDADialogScriptLayout(
				    PI_EditScriptedParms &parm_layout,
				    bool use_incoming_layout=false);

    /// Output the dialog script of the owner network's spare parameter layout.
    void			outputOwnerSpareParmLayout(std::ostream &os);

    /// Update the owner network's spare parameter layout.
    void			updateOwnerSpareParmLayout(
				    bool allow_update_while_loading=false);

    /// Update the owner network's spare parameter layout using the layout
    /// that is passed in (i.e. new_parm_layout). The new layout can contain a
    /// mix of exported parameters and regular parameters.  Use `old_parms` to
    /// supply a list of parameters that are to be renamed/deleted from the
    /// layout.  Optionally set `apply_changes_to_parm_vops` to true if you
    /// want to apply the changes from `old_parms` to the defining 
    /// Parameter VOPs.
    void			updateOwnerSpareParmLayout(
				    const PI_EditScriptedParms &new_parm_layout,
				    const PI_OldParms &old_parms,
				    bool apply_changes_to_parm_vops=false);

    /// Update the parameters on the owner network's internal operator 
    /// (VOPNETs only).
    void			updateInternalOpParms(
				    bool force_ds_generation=false,
				    bool allow_update_while_loading=false);

    /// Add the given parameter layout as a template for the next
    /// update to the owner VOP network's spare parameter layout.
    void			addParmLayoutTemplate(
				    const PI_EditScriptedParms &ref_parms);

    /// Micro node that represents cached the exported parameter layout.
    VOP_ExportedParmLayoutMicroNode &parmLayoutMicroNode()
				    { return myParmLayoutMicroNode; }

    /// Micro node that represents cached exported parameters and dialog
    /// script.
    VOP_ParmDSMicroNode	&	parmsMicroNode()
				    { return myParmsMicroNode; }

    /// Dirty the exported parameters.
    void			dirtyParms();

private:
    /// Initialize the manager. This is called from the constructor.
    void			initialize_();

    /// Return the code generator that owns this exported parameters manager.
    /// Return NULL if there is no owning code generator.
    inline VOP_CodeGenerator *	ownerCodeGen_() const;

    /// Return the owning subnet VOP.  Return NULL if there is no owning subnet.
    inline VOP_Node *		ownerSubnet_() const;

    /// Return true if the owner is an HDA node.
    inline bool			isOwnerHDANode_() const;

    /// Make sure that the exported parameter layout dialog script
    /// is up-to-date and update it if needed.
    void			ensureParmLayoutIsUpdated_();

    /// Make sure that the exported parameters list and dialog script
    /// are up-to-date and update them if needed.
    void			ensureParmsAreUpdated_();

    /// Generate a dialog script for the defining VOP network,
    /// the Parameter VOP nodes, and the parameter layout set by
    /// the user on the owning VOPNET node.  The dialog script also
    /// includes spare parameters created on the owning node.
    void			generateParmLayoutDialogScript_(
				    UT_String &definition);

    /// Generate the exported parameters list and dialog script
    /// and pass them back to the caller.
    void			generateParms_(
				    VOP_ExportedParmList &parms, 
				    UT_String &definition);

    /// Output the owner node's information in dialog script format.
    void			outputOwnerNodeInfoAsDS_(std::ostream &os);

    /// Return the name to use for the owner network's parameter dialog
    /// script.
    const char *		ownerParmDSName_() const;

    /// Return the label to use for the owner network's parameter dialog
    /// script.
    const char *		ownerParmDSLabel_() const;

    /// Return the list of exported parameters.
    const VOP_ExportedParmList &getParms_(bool use_cached_value=false) const;

    /// Return the dialog script for the exported parameters.
    const char *		getParmsDialogScript_() const;

    /// Compare the old and new list of parameters.
    /// Record any parameters that have been renamed or deleted
    /// and pass them back in `old_parms`.
    void			getRenamedAndDeletedParms_(
				    const VOP_ExportedParmList &old_code_parms,
				    const VOP_ExportedParmList &new_code_parms,
				    PI_OldParms &old_parms) const;

    /// Return the list of old parameters that have been renamed or deleted
    /// since the last exported parameter layout update.
    const PI_OldParms &		getOldParms_() const;

    /// Return the list of old HDA parameters that have been renamed or deleted
    /// since the last HDA dialog script update.
    const PI_OldParms &		getOldHDAParms_() const;

    /// Clear out the list of old parameters.
    /// This should only be called after the exported parameter layout
    /// has been updated.
    void			clearOldParms_();

    /// Clear out the list of old HDA parameters.
    /// This should only be called after the exported parameter layout
    /// has been pushed to the owner's HDA's dialog script section.
    void			clearOldHDAParms_();

    /// Obtains a list of all parameter nodes for the material (if it is 
    /// a material vopnet) or for the shader (if it is just a shader)
    void			getParmNodes_(
				    OP_Network *net, 
				    VOP_ExportedParmList &list,
				    std::ostream *os, 
				    bool includeInvisible,
				    const VOP_ParmContextInfo &info);
    void			getParmNodes_(
				    VOP_ExportedParmList &list,
				    std::ostream *os,
				    bool includeInvisible);

    /// Add the Parameter VOP to the given list and output its string to the
    /// given stream if it applies and is not empty, that is, if it contains
    /// information that should be added or outputted.
    void			addParmVOPIfApplicable_(
				    VOP_ParmGenerator *vop, 
				    const VOP_ParmContextInfo &info,
				    bool include_invisible,
				    VOP_ExportedParmList &parms, 
				    std::ostream *os) const;

    /// Builds an information about which parameters belong to which contexts.
    void			buildParmContextInfo_(
				    VOP_ParmContextInfo &info);
    void			buildParmClassContextInfo_(
				    VOP_ParmContextInfo &info,
				    const VOP_CodeGenContext &codegen_ctx);
    void			buildParmShaderContextInfo_(
				    VOP_ParmContextInfo &info,
				    const VOP_CodeGenContext &codegen_ctx);

    /// Propagate parameter changes down to the Parameter VOPs.
    void			propagateChangesToParmVOPs_(
				    const PI_OldParms &old_parms) const;

    /// Merge two sets of spare parameters into one.
    /// The merged set is passed back as `merge_to`.
    void			mergeSpareParms_(
				    PI_EditScriptedParms &merge_to,
				    PI_EditScriptedParms &merge_from) const;

    /// Merge the group start parameter into the specified layout, `merge_to`.
    void			mergeSpareGroupStartParm_(
				    PI_EditScriptedParms &merge_to, 
				    const PI_EditScriptedParm *parm,
				    UT_StringArray &folder_labels,
				    UT_IntArray &group_end_indices) const;

    /// Merge the group end parameter into the specified layout, `merge_to`.
    void			mergeSpareGroupEndParm_(
				    PI_EditScriptedParms &merge_to, 
				    const PI_EditScriptedParm *parm,
				    UT_StringArray &folder_labels,
				    UT_IntArray &group_end_indices) const;

    /// Merge the regular parameter into the specified layout, `merge_to`.
    void			mergeSpareRegularParm_(
				    PI_EditScriptedParms &merge_to, 
				    const PI_EditScriptedParm *parm,
				    UT_IntArray &group_end_indices) const;

    /// Return the index of the parameter that best matches the specified
    /// folder group parameter.  Return -1 if no such parameter exists.
    int				findMatchingGroupStartParm_(
				    PI_EditScriptedParms &parms, 
				    const UT_StringArray &folder_labels,
				    const PI_EditScriptedParm &group_parm) 
				    const;

    /// Update the given parameter layout with the changes stored in the old
    /// parameters (i.e. deleted parms, renamed parms).
    void			updateLayoutWithOldParms_(
				    PI_EditScriptedParms &parm_layout, 
				    const PI_OldParms &old_parms) const;

    /// Return the spare parameter layout of the exported parameters.
    /// The caller takes ownership of the returned PI_EditScriptedParms and is
    /// responsible for its deletion.
    UT_UniquePtr<PI_EditScriptedParms>
				createSpareParms_(bool skip_reserved);
   
    /// Set dependency links from dependent micro nodes
    /// to the exported parameter layout micro node.
    void			setParmLayoutMicroNodeDependents_();

    /// Generate and pass back the parameter layout either for the
    /// owner's spare parameters or the owner's HDA's dialog script.
    void			getOwnerParmLayout_(
				    PI_EditScriptedParms &parm_layout,
				    bool for_hda_ds,
				    bool use_incoming_layout);

    /// The exported parameter layout dialog script, the micro node
    /// that represents it and the sets of parameters to use as layout
    /// templates for the next update to the exported parameter layout.
    UT_String			myParmLayoutDialogScript;
    VOP_ExportedParmLayoutMicroNode myParmLayoutMicroNode;
    UT_ValArray<PI_EditScriptedParms *> myParmLayoutTemplates;

    /// The exported parameters, its dialog script and the micro node that
    /// represents them.
    VOP_ExportedParmList	myParms;
    UT_String			myParmsDialogScript;
    VOP_ParmDSMicroNode		myParmsMicroNode;

    /// Stores renamed and deleted parameters for the owner's spare parameter
    /// layout and the owner's HDA definition's dialog script section.
    PI_OldParms			myOldParms;
    PI_OldParms			myOldHDAParms;

    // Locked compiled dialog scripts for compiled owner nodes.
    UT_WorkBuffer		myLockedCompiledDialogScript;

    /// Cached data used to reduce the amount of updates propagated to the
    /// internal operator's parameter templates.
    UT_String			myCachedDialogScript;
    int				myCachedMinInputs;
    int				myCachedMaxInputs;

    /// Flag indicating whether the parm tag should be updated or not.
    bool			myContextParmTagDirty;

    /// Flag indicating whether we are currently updating the owner network's
    /// spare parameter layout.
    bool			myIsChangingOwnerSpareParms;


    /// Flag indicating whether we are currently updating the parameters.
    bool			myIsUpdatingParms;

    /// Code generator that owns this manager.
    VOP_CodeGenerator *		myOwnerCodeGen;

    /// Subnet VOP that owns this manager.
    VOP_Node *			myOwnerSubnet;
    
    /// Set to true if the owner node's spare parm layout needs to be updated
    /// when the node is finished loading.
    bool			myLoadPendingSpareParmLayoutUpdate = false;
};


// ===========================================================================
// Inline method implementations.
// ===========================================================================

OP_Network *
VOP_ExportedParmsManager::ownerNetwork() const
{  
    OP_Network *owner = NULL;
    if (ownerCodeGen_())
	owner = ownerCodeGen_()->ownerNetwork();
    else
	owner = (OP_Network *)ownerSubnet_();

    UT_ASSERT(owner);
    return owner;
}

VOP_CodeGenerator *
VOP_ExportedParmsManager::ownerCodeGen_() const
{ 
    return myOwnerCodeGen;
}

VOP_Node *
VOP_ExportedParmsManager::ownerSubnet_() const
{ 
    return myOwnerSubnet;
}

bool
VOP_ExportedParmsManager::isOwnerHDANode_() const
{
    OP_Network *owner = ownerNetwork();
    if (owner && owner->getOperator() 
	&& ownerNetwork()->getOperator()->getOTLIndexFile())
	return true;

    return false;
}

#endif
