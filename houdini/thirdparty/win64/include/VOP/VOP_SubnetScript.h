/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP Library (C++)
 *
 */

#ifndef __VOP_SubnetScript_h__
#define __VOP_SubnetScript_h__

#include "VOP_API.h"
#include "VOP_ExportedParmsManager.h"
#include "VOP_ScriptNode.h"
class VOP_SubnetHelper;
			
class VOP_API VOP_SubnetScript : public VOP_ScriptNode
{
public:
    static OP_Node		*myConstructor(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);
    static PRM_Template		 myTemplateList[];

    virtual OP_OperatorFilter	*getOperatorFilter();

    virtual void	 getModulesToImport(UT_StringArray &modules,
				const VOP_CodeGenContext &context);
    virtual void	 getOuterCode(UT_String &codestr,
				const VOP_CodeGenContext &context);
    virtual void	 getCode(UT_String &codestr,
				const VOP_CodeGenContext &context);

    virtual bool	 forceCodeGenerationOfInputs(
					    const VOP_CodeGenContext &context,
					    bool check_shader_context) const;

    virtual bool	 canDefineShader() const;
    virtual void	 getVopFunctionArgInfos(
				UT_Array<VOP_FunctionArgInfo> &arg_infos);

    virtual bool	 getParmConstantString(const char *parmname,
					       const VOP_TypeInfo &type_info,
					       UT_String &str,
					       bool expand_string,
					       const VOP_Language *l=0);

    virtual const char	*inputLabel(unsigned idx) const;
    virtual const char	*outputLabel(unsigned idx) const;

    virtual unsigned	 maxInputs() const;
    virtual unsigned	 getNumVisibleInputs() const;
    virtual unsigned	 getNumVisibleOutputs() const;
    virtual int		 getNumNodeDefinedOutputs() const;
    virtual int		 getNumSubnetInputsForChildren() const;
    virtual int		 getSubnetInputIndexForChildren(int index) const;

    virtual bool	 isConnected(int inputidx, bool recursive);

    virtual VOP_ExportedParmsManager *getVopExportedParmsManager(
					bool create_if_needed=false);

    /// Updates the node so new inuts and outputs are taken into account.
    /// If op_info argument is NULL, the current operator op info is queried.
    void		 updateSubnetWithNewOp(const VOP_OperatorInfo *op_info);

    static const char	*theChildTableName;

protected:
    VOP_SubnetScript(OP_Network *net, const char *name, OP_Operator *entry);
    virtual ~VOP_SubnetScript();

    virtual UT_StringArray *	getArrayForLoadedInputNames();
    virtual UT_StringArray *	getArrayForLoadedOutputNames();
    void		        deleteArraysForLoadedInputAndOutputNames();
    virtual VOP_ParmGeneratorMap *getSubnetVariables();

    virtual void	 beginLoadingInNetwork();
    virtual void	 endLoadingInNetwork();
    virtual void	 loadStart();
    virtual void	 finishedLoadingNetwork(bool is_child_call=false);
    virtual void	 setupConnectorsAfterLoad();
    virtual void	 changeParmTemplate(PRM_Template *new_template);
    virtual void	 addNode(OP_Node *node, int notify=1, int explicitly=1);

    /// Invalidates the cached list of contained shaders. 
    virtual void	 dirtyShaderList();

    /// Overriding these virtuals to control how we create contents network
    /// @{
    virtual void	 matchOTLDefinition();
    virtual void	 unmatchOTLDefinition();
    virtual void	 handleOTLIndexFileCleared();
    /// @}

    /// Inherited from VOP_Node.
    /// Generate the node errors.
    /// Return true if an error was generated and false otherwise.
    virtual bool	 generateErrorsSubclass();

    /// Subnet declares own variables, so some types may need to be defined.
    virtual void	 getExtraTypesToDefine( UT_Array<VOP_TypeInfo> &types );

    virtual void	 opChanged(OP_EventType reason, void *data);
    virtual void	 postOpChanged(OP_EventType reason, void *data);

    /// @{ Methods for handling inputs defined by a parm vop children in
    ///    unlocked assets.
    void		 handleChildAdd( OP_Node *child );
    void		 handleChildDelete( OP_Node *child );
    void		 handleDescendentAdd( VOP_Node *descendent );
    void		 handleDescendentDelete( VOP_Node *descendent );
    static void		 handleParmNodeChange(OP_Node *caller, void *callee, 
				OP_EventType type, void *);
    void		 updateConnectorDefiner( VOP_Node *node);
    virtual void	 shaderParmGrandChildAdded(VOP_Node *grand_child);
    virtual void	 shaderParmGrandChildDeleted(VOP_Node *grand_child);
    virtual void	 shaderParmGrandChildChanged(VOP_Node *grand_child);
    void		 setupConnectorDefinersAfterLoadIfNeeded(
				const VOP_OperatorInfo *op_info);
    void		 setupConnectorDefinersAfterLoad(
				const VOP_OperatorInfo *op_info);
    void		 addConnectorDefinerAfterLoad(VOP_ParmGenerator *pg,
				const VOP_OperatorInfo *op_info,
				UT_Array< VOP_ParmGenerator* > &set_in,
				UT_Array< VOP_ParmGenerator* > &append_in,
				UT_Array< VOP_ParmGenerator* > &set_out,
				UT_Array< VOP_ParmGenerator* > &append_out );
    int			 verifyOpInputIndexForNode(int idx) const;
    /// @}

    virtual void	 getInputNameSubclass(UT_String &in, int idx) const;
    virtual int		 getInputFromNameSubclass(const UT_String &in) const;
    virtual void	 getInputTypeInfoSubclass(
					    VOP_TypeInfo &type_info, int idx);
    virtual bool	 getIsInputDefinedByParmChild(int idx) const;
    virtual bool	 getIsOutputDefinedByParmChild(int idx) const;

    virtual void	 getOutputNameSubclass(UT_String &out, int idx) const;
    virtual void	 getOutputTypeInfoSubclass(VOP_TypeInfo &type_info, 
						   int idx);
    virtual UT_IntArray		getShaderLayerExportsInputsToCopy(int out_idx,
				    VOP_Type shader_type);
    virtual UT_StringHolder	getShaderLayerExportsToAdd(int out_idx,
				    VOP_Type shader_type);

    /// Returns true if output of a given index has an input by the same name.
    virtual bool	 hasCorrespondingInput(int output_idx) const;

    virtual void	 ensureSpareParmsAreUpdatedSubclass()
			 {
			    if (getVopExportedParmsManager()
				&& eventMicroNode(OP_SPAREPARM_MODIFIED)
				    .requiresUpdate(0.0))
			    {
				getVopExportedParmsManager()
				    ->updateOwnerSpareParmLayout();
			    }
			 }

    /// Returns true if the subnet script (HDA) is using cached code
    /// rather than generating it from the children.
    bool		 isUsingCachedCode() const
			 { return myIsUsingCachedCode; }

    /// @{ Helpers for managing script info object
    virtual void	loadMaterialInfo();
    virtual void	unloadMaterialInfo();
    /// @}

private:
    struct vop_EncapsulatedMetaData
    {
	vop_EncapsulatedMetaData() : myIsValid(false) {}

	bool				myIsValid;
	UT_SortedStringMap<UT_String>	myIsConnectedMap;
    };

    bool		 parseMetaDataString(UT_String &str,
        		                     vop_EncapsulatedMetaData &md);
    void		 parseEncapsulatedMetaData(const UT_WorkBuffer &code,
        		                           vop_EncapsulatedMetaData &md);

    void		 adjustChildrenForInputRemoval(int idx);
    void		 adjustChildrenForInputInsertion(int idx);

private:
    /// Forward declaration of our embedded VOP_SubnetHelper::SubnetAPI
    /// subclass.
    class SubnetAPI;

    friend class SubnetAPI;

    /// Helper that factors out methods shared with regular subnetwork.
    VOP_SubnetHelper		*mySubnetHelper;

    /// Used during loading to find out input and output names just in case
    /// we need to order Parameter VOP defined inputs/outputs in assets not
    /// matching their definition.
    UT_StringArray		*myLoadedInputNames;
    UT_StringArray		*myLoadedOutputNames;

    /// A flag indicating if node is exclusively using cached vex/vfl code.
    bool			 myIsUsingCachedCode;

    /// We're deleting a mass of nodes. We don't care about knowing whether
    /// we just lost one.
    bool			myIsDeletingSubnetNodes;

    /// We're in setupConnectorDefinersAfterLoad().
    unsigned			myIsSettingUpConnectorDefinersAfterLoad : 1,
				myIsLoadingNetwork : 1,
				myAreConnectorsSetUp : 1;
};

#endif
