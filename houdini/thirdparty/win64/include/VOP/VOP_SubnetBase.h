/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP Library (C++)
 *
 * COMMENTS:	The base class for all subnet VOP operators
 *
 */

#ifndef __VOP_SubnetBase_h__
#define __VOP_SubnetBase_h__

#include "VOP_API.h"
#include "VOP_Node.h"
#include "VOP_ExportedParmsManager.h"
#include <UT/UT_StringArray.h>

class VOP_SubnetHelper;

class VOP_API VOP_SubnetBase : public VOP_Node
{
public:
    virtual OP_OperatorFilter	*getOperatorFilter();

    virtual void	 getCode(UT_String &codestr,
                	         const VOP_CodeGenContext &context);

    virtual const char	*inputLabel(unsigned idx) const;
    virtual const char	*outputLabel(unsigned idx) const;

    virtual unsigned	 getNumVisibleInputs() const;
    unsigned		 getNumRealInputs() const;
    virtual unsigned	 getNumVisibleOutputs() const;
    virtual unsigned	 orderedInputs() const;
    
    virtual void	 getNodeSpecificInfoText(OP_Context &context,
						 OP_NodeInfoParms &iparms);

    // When one of our inputs changes, we may want to rearrange things on
    // our subnet input and subnet output nodes.
    virtual OP_ERROR	 setInput(unsigned idx, OP_Node *op,
				  unsigned outputIdx = 0);
    virtual OP_ERROR	 setInputReference(unsigned idx, const char *label,
					   int keeppos, unsigned outputIdx = 0);
    virtual OP_ERROR	 setNamedInput(const OP_ConnectorId& input_name, OP_Node *op,
	const OP_ConnectorId* output_name = NULL);
    virtual OP_ERROR	 setNamedInputReference(const OP_ConnectorId&  input_name, 
	const char *label, int, const OP_ConnectorId* output_name = NULL);


    virtual bool	 isConnected(int inputidx, bool recursive);

    virtual const VOP_SubnetBase *castToSubnetBase() const { return this; }
    virtual VOP_SubnetBase *castToSubnetBase() { return this; }

    virtual bool	 forceCodeGenerationOfInputs(
					const VOP_CodeGenContext &context,
					bool check_shader_context) const;
    bool		 getIsAdjustForInputChanged(void);

    virtual VOP_ExportedParmsManager *getVopExportedParmsManager(
					bool create_if_needed=false);
				    
    static const char	*theChildTableName;

protected:
    VOP_SubnetBase(OP_Network *parent, const char *name, OP_Operator *entry);
    virtual ~VOP_SubnetBase();

    virtual bool		updateParmsFlags();
    virtual bool		shouldShowInputNext() const;
    virtual UT_StringArray *	getArrayForLoadedInputNames();
    virtual UT_StringArray *	getArrayForLoadedOutputNames();
    virtual VOP_ParmGeneratorMap *getSubnetVariables();
    virtual void	 beginLoadingInNetwork();
    virtual void	 endLoadingInNetwork();
    virtual void	 loadStart();
    virtual void	 finishedLoadingNetwork(bool is_child_call=false);
    virtual void	 setupConnectorsAfterLoad();
    virtual void	 addNode(OP_Node *node, int notify=1, int explicitly=1);

    /// Inherited from VOP_Node.
    /// Generate the node errors.
    /// Return true if an error was generated and false otherwise.
    virtual bool	 generateErrorsSubclass();

    virtual void	 preOpChanged(OP_EventType reason, void *data);
    virtual void	 postOpChanged(OP_EventType reason, void *data);

    /// @{ Methods for handling inputs defined by a parm vop children
    void		 handleChildAdd( OP_Node *child );
    void		 handleChildDelete( OP_Node *child );
    void		 handleDescendentAdd( VOP_Node *descendent );
    void		 handleDescendentDelete( VOP_Node *descendent );
    static void		 handleParmNodeChange(OP_Node *caller, void *callee, 
				OP_EventType type, void *);
    void		 updateConnectorDefiner( VOP_Node *node );
    virtual void	 shaderParmGrandChildAdded(VOP_Node *grand_child);
    virtual void	 shaderParmGrandChildDeleted(VOP_Node *grand_child);
    virtual void	 shaderParmGrandChildChanged(VOP_Node *grand_child);
    void		 setupConnectorDefinersAfterLoadIfNeeded();
    void		 setupConnectorDefinersAfterLoad();
    void		 addConnectorDefinerAfterLoad(VOP_ParmGenerator *pg);
    /// @}

    virtual void	 getInternalInputName(UT_String &in, int idx) const;

    virtual int		 getNumDefinedInputs() const;
    virtual void	 getDefinedInputDefault(UT_String &def, int idx) const;
    virtual void	 getInputDefaultValue(UT_String &def, int idx) const;

    virtual void	 getInputNameSubclass(UT_String &in, int idx) const;
    virtual int		 getInputFromNameSubclass(const UT_String &in) const;
    virtual void	 getInputTypeInfoSubclass(
					    VOP_TypeInfo &type_info, int idx);
    virtual void	 getAllowedInputTypesSubclass(unsigned idx,
					    VOP_VopTypeArray &voptypes);
    virtual bool	 allowConsolidatingInput(int idx);

    /// Invalidates the cached list of contained shaders. 
    virtual void	 dirtyShaderList();

    /// Number of extra outputs a subnet subclass may want to provide, in 
    /// addition to outputs defined by this base class (and sub output child).
    virtual int		 getNumCustomOutputs() const;

    /// Number of extra outputs on the subnet node in addition to the
    /// outputs defined by the Subnet Output child vop inside.
    virtual int		 getNumNodeDefinedOutputs() const;
    virtual int		 getNumSubnetInputsForChildren() const;
    virtual int		 getSubnetInputIndexForChildren(int index) const;
    virtual void	 getOutputNameSubclass(UT_String &out, int idx) const;
    virtual void	 getOutputTypeInfoSubclass(VOP_TypeInfo &type_info, 
						   int idx);
    virtual UT_IntArray		getShaderLayerExportsInputsToCopy(int out_idx,
				    VOP_Type shader_type);
    virtual UT_StringHolder	getShaderLayerExportsToAdd(int out_idx,
				    VOP_Type shader_type);

    /// Returns true if output of a given index has an input by the same name.
    bool		 hasCorrespondingInput(int output_idx) const;

    virtual bool	 isInputConnectedInsideSubnet(int input_idx) const;
    virtual bool	 isNamedInputConnectedInsideSubnet(const OP_ConnectorId& input_name) const;

    /// Subnet declares own variables, so some types may need to be defined.
    virtual void	 getExtraTypesToDefine( UT_Array<VOP_TypeInfo> &types );

    /// Virtuals for handling input order editing.
    virtual int		 getInputEditorInputs();
    virtual bool	 getInputEditorInputLabel( UT_String &label, int idx );
    virtual void	 moveInput(int srcidx, int dstidx,
				   bool forcesubnet = false);
    virtual void	 doDeleteInput(int idx);


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

private:
    /// inputnull is true if the input is about to become null, and false if it
    /// is about to be connected.
    void		 adjustChildrenForInputChange(int idx, 
				    int child_input_idx,
				    bool inputnull, bool force, 
				    bool allow_disconnection, 
				    bool allow_connection);
    int			 preSetSubnetInput(int idx, bool inputnull);
    void		 postSetSubnetInput(int idx, bool inputnull, 
				    int child_input_idx);
    void		 findSafeBase(UT_String &in) const;
    bool		 isConnectedInside(int idx,
					   bool include_outputs,
					   bool include_parm_definers) const;
    // Note that this expects a subnet input name, i.e. the name not yet 
    // prefixed with VOP_SUBNET_VARIABLE_PREFIX.
    bool		 isConnectedInsideNamed(
				const OP_ConnectorId& input_name,
				bool include_parm_definers) const;
    void		 deleteArraysForLoadedInputAndOutputNames();

private:
    /// Forward declaration of our embedded VOP_SubnetHelper::SubnetAPI
    /// subclass.
    class SubnetAPI;

    friend class SubnetAPI;

    UT_StringArray		 myInputNames;
    unsigned			 myIsAdjustingForInputChange : 1,
				 myIsAppendingDefinedInput : 1,
				 myIsSettingUpConnectorDefinersAfterLoad : 1,
				 myIsLoadingNetwork : 1,
				 myAreConnectorsSetUp : 1;

    /// Helper that factors out methods shared with regular subnetwork.
    VOP_SubnetHelper		*mySubnetHelper;

    /// Used during loading to find out input and output names.
    UT_StringArray		*myLoadedInputNames;
    UT_StringArray		*myLoadedOutputNames;
};

#endif
