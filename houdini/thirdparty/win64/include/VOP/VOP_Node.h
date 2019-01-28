/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP Library (C++)
 *
 * COMMENTS:	The base class for all VOP operators
 *
 */

#ifndef __VOP_Node_h__
#define __VOP_Node_h__

#include <set>
#include <map>
#include <iosfwd>

#include "VOP_API.h"
#include "VOP_DataMicroNode.h"
#include "VOP_Error.h"
#include "VOP_Language.h"
#include "VOP_TypeInfo.h"
#include "VOP_OperatorInfo.h"
#include "VOP_OutputInfoManager.h"
#include <OP/OP_Director.h>
#include <OP/OP_Network.h>
#include <UT/UT_UniquePtr.h>
#include <UT/UT_BitArray.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_SymbolTable.h>
#include <UT/UT_Array.h>
#include <UT/UT_Map.h>
#include <SYS/SYS_Types.h>
#include <vector>

class UT_StringRef;

#define VOP_OPT_LOW_FLAG	'L'
#define VOP_OPT_MEDIUM_FLAG	'M'
#define VOP_OPT_HIGH_FLAG	'H'
#define VOP_DEBUG_FLAG		'D'
#define VOP_MATERIAL_FLAG	'E'

// The name of the parameter that contains the ordering of the
// VOPNET Parameters.
#define VOP_PARMORDER_NAME		"parmorder"
#define VOP_BIND_NAME			"bind"
#define VOP_GLOBAL_NODE_NAME		"global"
#define VOP_ILLUM_NODE_NAME		"illuminance"
#define VOP_PARM_NODE_NAME		"parameter"
#define VOP_CONST_NODE_NAME		"constant"
#define VOP_PARMRAMP_NODE_NAME		"rampparm"
#define VOP_INLINE_CODE_NODE_NAME	"inline"
#define VOP_SNIPPET_NODE_NAME		"snippet"
#define VOP_COLLECT_NODE_NAME		"collect"
#define VOP_MATERIAL_BUILDER_NODE_NAME  "materialbuilder"
#define VOP_RSL_PREFIX			"rsl_"
#define VOP_VARIABLE_INOUT_MAX		2048

/// Prefix given to the subnet input variables.
#define VOP_SUBNET_VARIABLE_PREFIX	"_"

enum VOP_UIChangeType
{
    VOP_UICHANGE_DEFINER = OP_UICHANGE_OPTYPE_VOP
};

class TIL_Thumbnail;
class VCC_DiagnosticInfo;
class VOP_Node;
class VOP_GlobalVarData;
class VOP_ParmGenerator;
class VOP_OutputVar;
class VOP_Bind;
class VOP_Block;
class VOP_AutoConvert;
class VOP_CodeGenContext;
class VOP_ExportedParmList;
class VOP_ExtraVarRequests;
class VOP_VariableTags;
class VOP_SubnetBase;
using VOP_VariableTagsHandle	= UT_SharedPtr<const VOP_VariableTags>;

extern "C" {
    SYS_VISIBILITY_EXPORT extern void       newVopOperator(OP_OperatorTable *table);
};

typedef UT_ValArray<VOP_Node *>		 VOP_NodeList;
typedef UT_ValArray<VOP_GlobalVarData *> VOP_GlobalVarList;
typedef UT_ValArray<VOP_ParmGenerator *> VOP_ParmGeneratorList;

typedef UT_SymbolMap<VOP_GlobalVarData *> VOP_GlobalVarMap;
typedef UT_SymbolMap<VOP_ParmGenerator *> VOP_ParmGeneratorMap;
typedef UT_SymbolMap<VOP_ParmGeneratorList *> VOP_LocalChannelMap;

struct VOP_AutoConvertInfo
{
    // A pointer to the autoconvert node and a value for the input index.
    VOP_AutoConvert* myACNode;
    int myIndex;
};
typedef std::vector < VOP_AutoConvertInfo > TAutoConvertInfos;

typedef std::set < int > TIntSet;
struct VOP_MergeNodeInfo
{
    VOP_Node* myNode;
    TIntSet myInputIndices;
};
typedef std::map <VOP_Node*, VOP_MergeNodeInfo> TMergeNodeInfos;

//============================================================================= 
class VOP_API VOP_VarInfo
{
public:
    UT_StringHolder	myName;		    // variable name
    UT_StringHolder	myDescription;	    // variable description
    VOP_ContextType	myContextType;	    // context type in which var exists
    int			myVarIndex;	    // index in the context's var table
    VOP_TypeInfo	myTypeInfo;	    // variable type (int, float, etc)
    bool		myReadable;	    // can read from variable?
    bool		myWritable;	    // can write to variable?
    bool		myIlluminanceVar;   // is illuminance?
};

//============================================================================= 
class VOP_API VOP_GlobalVarData
{
public:
     VOP_GlobalVarData( const char * name, const VOP_VarInfo & info );
    ~VOP_GlobalVarData();
    void			appendInfo( const VOP_VarInfo & info );

    const UT_StringHolder &	getName() const 
				{ return myName; }

    const UT_StringHolder &	getDescription( VOP_ContextType type ) const;
    const VOP_TypeInfo &	getTypeInfo( VOP_ContextType type ) const;
    int				getVarIndex( VOP_ContextType type ) const;
    bool			isReadable( VOP_ContextType type ) const;
    bool			isWriteable( VOP_ContextType type ) const;
    bool			isIlluminance( VOP_ContextType type ) const;
    bool			isValidInContext( VOP_ContextType type ) const;

    const VOP_TypeInfo &	getAnyTypeInfo() const
    {
	return myInfos(0).myTypeInfo;
    }

private:
    template<typename T> T	    getInfoMember( VOP_ContextType ctx_type, 
					       T VOP_VarInfo::*member, 
					       T default_val ) const;
    template<typename T> const T    &getInfoMemberRef( VOP_ContextType ctx_type,
					       T VOP_VarInfo::*member, 
					       const T & default_val ) const;
    int			    getIndexForContextType( VOP_ContextType type) const;

private:
    UT_StringHolder		myName;		// variable name
    UT_Array<VOP_VarInfo>	myInfos;	// infos for applicable contexts
};

//============================================================================= 
class VOP_API VOP_OutputNameEditorSource
{
public:
    virtual		~VOP_OutputNameEditorSource();
    virtual int		 getNumInputsFromParent() const = 0;
    virtual void	 NAMEFROMPARM(UT_String &str, int idx) const = 0;
    virtual void	 LABELFROMPARM(UT_String &str, int idx) const = 0;
    virtual void	 setNAMEFROMPARM(UT_String &str, int idx) = 0;
    virtual void	 setLABELFROMPARM(UT_String &str, int idx) = 0;
};

// ============================================================================ 
/// Abstracts information about shader function parameters/arguments.
/// It has two use cases: when declaring and defining a shader function, and
/// when calling a shader function. It always stores shader parameter type and 
/// name, but may not store some of the other information for some use cases.
class VOP_API VOP_FunctionArgInfo
{
public:
			    VOP_FunctionArgInfo();

    /// Argument type.
    void		    setTypeInfo( const VOP_TypeInfo &type_info );
    const VOP_TypeInfo &    getTypeInfo() const
				{ return myTypeInfo; }

    /// Argument name.
    void		    setName( const char *name );
    const UT_StringHolder & getName() const
				{ return myName; }

    /// Local variable name to use when calling the function.
    void		    setVarName( const char *var_name );
    const UT_StringHolder & getVarName() const;

    /// Expression (ie, RHS of the assignment) to use for initialization 
    /// of the argument when calling the shader function.
    void		    setInitCodeStr( const char *init_code );
    const UT_StringHolder & getInitCodeStr() const
				{ return myInitCodeStr; }

    /// Node input index, if argument corresponds to an input; otherwise -1.
    void		    setInputIndex( int input_index );
    int			    getInputIndex() const 
				{ return myInputIdx; }

    /// Node output index, if argument corresponds to an input; otherwise -1.
    void		    setOutputIndex( int output_index );
    int			    getOutputIndex() const
				{ return myOutputIdx; }

    /// True if it's an export (output) shader function parameter.
    void		    setExported( bool exported );
    bool		    isExported() const
				{ return myIsExported; }

private:
    VOP_TypeInfo    myTypeInfo;	    // Argument type
    UT_StringHolder myName;	    // Argument name
    UT_StringHolder myVarName;	    // Variable name to use in function call
    UT_StringHolder myInitCodeStr;  // Expression code to init an arg to.
    int		    myInputIdx;	    // Input index this variable corresponds to
    int		    myOutputIdx;    // Output index this var corresponds to
    bool	    myIsExported;   // Is an output argument
};

//============================================================================= 
class VOP_API vop_Connector
{
public:
    vop_Connector(int groups_above,
	    int index = -1)
	: myIndex(index),
	  myGroupsAbove(groups_above),
	  myIsExpanded(false),
	  myIsGroup(false)
    { }
    vop_Connector(int groups_above,
	    const UT_StringHolder &group_name,
	    int index,
	    bool expanded)
	: myGroupName(group_name),
	  myIndex(index),
	  myGroupsAbove(groups_above),
	  myIsExpanded(expanded),
	  myIsGroup(true)
    { }
    ~vop_Connector()
    { }

    int			 myIndex;
    int			 myGroupsAbove;
    UT_StringHolder	 myGroupName;
    bool		 myIsExpanded;
    bool		 myIsGroup;
};
typedef UT_Array<vop_Connector>  vop_ConnectorArray;

//============================================================================= 
class VOP_API VOP_Node : public OP_Network
{
public:
    static const char	*theChildTableName;
    virtual const char	*getChildType() const;
    virtual const char	*getOpType() const;

    virtual OP_OpTypeId  getChildTypeID() const;
    virtual OP_OpTypeId  getOpTypeID() const;

    ///  Nobody should have to override this, but it's public for other people
    virtual OP_DataType	 getCookedDataType() const;

    /// Whenever a variable type parm changes, we have to send out an event
    /// to get our tile to update.
    virtual void	 opChanged(OP_EventType reason, void *data=0);
    virtual void	 referencedParmChanged(int pi);
    
    static void		 buildOperatorTable(OP_OperatorTable &table);

    /// Get the parm block information that will go in the dialog script
    /// for our VOPNET. The shader_contexts argument is a list of
    /// contexts, in which the parameter is a shader argument.
    virtual void	 getParameterBlock(UT_String &parmstr,
					   const char *shader_contexts);

    /// The parameter variable declaration using the syntax of a given language.
    virtual void	 getParameterDeclaration(UT_String &parmdecl,
					    const VOP_Language *language,
					    const VOP_CodeGenContext &context);

    /// Obtains the parameter declaration code comment, which is appended
    /// after the declaration in the generated source code.
    virtual void	 getParameterComment(UT_String &parm_comment,
					    const VOP_Language *language,
					    const VOP_CodeGenContext &context);

    /// If false, getShaderExtraParameters is not invoked as it is assumed
    /// to return an empty list
    virtual bool	 definesShaderExtraParameters() const;

    /// Returns a list of the attributes that we want to bind or export.  
    /// These are added to the vex functions parameter list, but not to the
    /// UI as they are only expected to be overridden.  If they match
    /// an existing value, they do not replace.
    /// The shader type is used only in a very specific cases of material
    /// builder, when extra parameters may depend on a shader type.
    virtual int 	 getShaderExtraParameters(UT_Array<VOP_Type> &types, 
				UT_IntArray &doexport, UT_StringArray &names,
				UT_StringArray &def,
				VOP_Type shader_type = VOP_TYPE_UNDEF ) const;

    /// Get compiler pragmas for thi snode.
    virtual void	 getPragmas(UT_String &pragmastr,
				const VOP_CodeGenContext &context);

    /// If the node generates a shader call, get the modules names to import.
    virtual void	 getModulesToImport(UT_StringArray &modules,
				const VOP_CodeGenContext &context);

    /// Get the code fragment to be included in the outer code,
    /// before the main shader code.
    virtual void	 getOuterCode(UT_String &codestr,
				const VOP_CodeGenContext &context);

    /// Get the code fragment to be included in the shader code.
    virtual void	 getCode(UT_String &codestr,
				const VOP_CodeGenContext &context);

    /// Get the constant code that encodes the value of the node parameter.
    virtual bool	 getParmConstantString(const char *parmname,
					       const VOP_TypeInfo &type_info,
					       UT_String &str,
					       bool expand_string,
					       const VOP_Language *l=0);
    /// Returns the code name of the type, such as "float" from VOP_TYPE_FLOAT
    /// Threadsafe but not re-entrant.
    static const char 	*getTypeNameFromType(const VOP_TypeInfo &type_info,
						const VOP_Language *lang);
    /// Returns a UI friendly version of the string, with NiceNumber
    /// type effects embedded.
    bool		 getParmPrintableString(const char *parmname,
					       VOP_Type type,
					       UT_String &str,
					       bool expand_string) const;



    /// Returns true if this VOP is a VOP_ParmGenerator or subclass.
    virtual bool	 getIsParmGenerator() const;

    /// @{
    /// Virtual methods to avoid dynamic_casts
#define VOP_IMPLEMENT_CASTS(NAME) \
	static const VOP_##NAME *castTo##NAME(const OP_Node *node) { \
	    const VOP_Node *vop = CAST_VOPNODE(node); \
	    return vop ? vop->castTo##NAME() : nullptr; } \
	static VOP_##NAME *castTo##NAME(OP_Node *node) { \
	    VOP_Node *vop = CAST_VOPNODE(node); \
	    return vop ? vop->castTo##NAME() : nullptr; } \
	virtual const VOP_##NAME *castTo##NAME() const { return nullptr; } \
	virtual VOP_##NAME *castTo##NAME() { return nullptr; }

    VOP_IMPLEMENT_CASTS(ParmGenerator)
    VOP_IMPLEMENT_CASTS(SubnetBase)
    VOP_IMPLEMENT_CASTS(OutputVar)
    VOP_IMPLEMENT_CASTS(Bind)
    VOP_IMPLEMENT_CASTS(Block)
#undef VOP_IMPLEMENT_CASTS
    /// @}

    /// Moves a visualizer into our output list.
    virtual bool	 addOrMoveVisualizerToOutput(int outputidx);

    /// Creates a code generator for the VOP, if it needs one.
    /// When a VOP is placed in a non-shaer container node that does not have
    /// an overarching code generator, this method may be called to create one 
    /// for a VOP to act as a stand-alone shader.
    void		 createAutoCodeGenerator();

    /// Utility method to create explicit children inside that node.
    /// The children correspond to the epilogue in auto-generated shader.
    void		 createAutoShaderChildren();

    /// @{ Obtains a code generator that provides shader code associated
    /// with a node. Usually, vopnet shader VOPs have their own dedicated
    /// code generator for shader constructed fom their children. 
    /// However, also, all VOP nodes inside a Shader Network container node
    /// have their own auto-generator for automatically constructing temporary 
    /// shader from their inputs (siblings).
    virtual VOP_CodeGenerator	*getVopCodeGenerator();
    virtual VOP_CodeGenerator	*getVopAutoCodeGenerator();
    /// @}

    /// Gives the node a chance to initialize itself when it is first created.
    /// This is only used for VOP_ParmGenerator nodes.
    virtual void	 initializeNode();

    /// Called by VOP_CodeGenerator::afterAddNode to ensure that nodes
    /// have been properly initialized. Some nodes weren't calling their
    /// base class' initializeNode() resulting in only partial initialization.
    bool		 isInitialized() const { return myInitialized; }

    /// Make sure that our parmname parameter (if we have one) doesn't
    /// conflict with any other parmname parameter values.
    virtual void	 ensureParametersAreValid();
    static void		 incrementVariableName(UT_String &varname);

    /// @{ Some variable names may contain characters that are illegal in 
    /// C-style variable names. Eg, Snippet VOP may allow colons to indicate 
    /// namespaced attributes which get bound to VEX parameters/variables.
    /// To ensure the variable name is valid, such characters are encoded
    /// using only alphanumeric (and underscore) characters.
    /// The decode function reverses such encoding.
    static UT_StringHolder	 ensureValidVarName(const UT_StringHolder &var);
    static UT_StringHolder	 decodeVarName(const UT_StringHolder &var);
    /// @}

    /// Functions to get information about our operator outputs.
    /// These just call the getOutputXXXSubclass functions.
    virtual void	 getOutputName(UT_String &out, int idx) const;
    void	 	 getOutputName(UT_StringHolder &out, int idx) const
    { UT_String tmp; getOutputName(tmp, idx); out.adoptFromString(tmp); }
    virtual int		 getOutputFromName(const UT_String &out) const;
    VOP_Type		 getOutputType(int idx);
    void		 getOutputTypeInfo(VOP_TypeInfo &type_info,
					   int idx) const;
    VOP_Type		 getNamedOutputType(const OP_ConnectorId& output_name);
    void		 getNamedOutputTypeInfo(VOP_TypeInfo &type_info,
					    const OP_ConnectorId& output_name);
    /// Obtains annotations (tags) for the variable value pushed out on
    /// the given output connector. The tags are arbitrary, but are primarily
    /// used to denote the contents of arrays (ie, export name for the value).
    VOP_VariableTagsHandle  getOutputVariableTags(int idx, 
				VOP_Type shader_type) const;
    VOP_VariableTagsHandle  getNamedOutputVariableTags(
				const OP_ConnectorId& output_name,
				VOP_Type shader_type) const;

    /// Returns variable tags provided by an input node on the given input idx.
    VOP_VariableTagsHandle  getVariableTagsFromInputNode( int idx, 
				VOP_Type shader_type) const;

    /// @{ Virtuals for obtaining inputs from which to copy shader layer
    /// exports variable tags and which exports to add, for a given output.
    virtual UT_IntArray	    getShaderLayerExportsInputsToCopy(int out_idx,
				VOP_Type shader_type);
    virtual UT_StringHolder getShaderLayerExportsToAdd(int out_idx,
				VOP_Type shader_type);
    /// @}


    /// Returns the variable name associated with a given input number.
    /// These just call the getInputXXXSubclass functions.
    virtual void	 getInputName(UT_String &in, int idx) const;
    void	 	 getInputName(UT_StringHolder &in, int idx) const
    { UT_String tmp; getInputName(tmp, idx); in.adoptFromString(tmp); }
    virtual int		 getInputFromName(const UT_String &in) const;

    /// will_autoconvert_out, if not null, is set to true if autoconversion
    /// will be needed, to false otherwise.
    VOP_Type		 getInputType(int idx) const;
    void		 getInputTypeInfo(VOP_TypeInfo &type_info,
					  int idx) const;
    VOP_Type		 getNamedInputType(const OP_ConnectorId& input_name);
    void		 getNamedInputTypeInfo(VOP_TypeInfo &type_info,
					   const OP_ConnectorId& input_name);

    //  Whether the visibility flag has been set in the operator type property.
    //  This is not the same flag as the one in OP_Input.
    virtual bool	getIsInputVisibleDefault(int idx);

    /// Returns currently allowed input types for a given input, regardless 
    /// of what is connected to that input at the moment. 
    /// These do not include types to which autoconversion is possible. 
    virtual void	 getAllowedInputTypeInfos(unsigned idx,
					      VOP_VopTypeInfoArray &typeinfos);
    virtual void	 getAllowedInputTypes(unsigned idx,
					      VOP_VopTypeArray &voptypes);

    /// Returns all allowed types for a given input, regardless of what is 
    /// currently wired to that input *and* regardless of what subset of all 
    /// these types is currently legal due to pruning of invalid signatures
    /// (input types) on a script node (etc).
    /// This is similar to getAllowedInputTypeInfos(), but unlike it, this 
    /// method returns types belonging to invalid signatures.
    void		 getAllAllowedInputTypeInfos(unsigned idx,
					      VOP_VopTypeInfoArray &typeinfos);
    
    /// Get the nuber of signatures that the node has, and obtain the i-th
    /// signature name.
    /// The VOP signatures apply mostly to the VOP HDAs,
    /// where a user can define several signatures  for the operator (a
    /// signature is a set of input types, and the user provides a name
    /// associated with each such set). The build-in operators ususally have
    /// only a single signature, and thus don't bother to provide any
    /// distinguishing name for it (but they may).
    virtual int		 getSignatureCount() const;
    virtual void	 getSignatureName(UT_String &name, int index) const;

    /// Sets the current signature. The name must be a valid signature
    /// recognized by the node. Also it should be possible to switch to that
    /// signature, which may not be possible due to the types of connected
    /// inputs, which limit the aplicability of some signatures.
    virtual void	 setCurrentSignature(const char *name);
    /// Obtains the current signature name.
    virtual bool	 getCurrentSignatureName(UT_String &name);

    /// Some variables associated with vop inputs can be overriden or set
    /// without an explicit wire to that input. 
    /// The override comes from an output connector of another node,
    /// encoded as <node, output index> pair.
    using InputVariableOverride = UT_Pair<VOP_Node*, int>;
    void		 setInputVariableOverride(int input_index,
				const InputVariableOverride &var_override);
    InputVariableOverride getInputVariableOverride(int input_index) const;

    /// Returns true if the output variables of this node should not be
    /// renamed in the code (in an attempt to avoid name collisions);
    /// it usually applies to the shader output parameters in a given context.
    /// If so, the second function returns the name of this output variable.
    /// The version with VOP_CodeGenContext argument should be used.
    virtual bool	 areOutputVariablesFixed(const VOP_CodeGenContext &ctx);
    virtual bool	 areOutputVariablesFixed(VOP_ContextType context_type);
    virtual void	 getFixedOutputVariable(UT_String &var, int idx);

    /// Returns the variable name (in the generated code) that corresponds to 
    /// the node's idx-th output. Usually it's just the output name.
    virtual void	 getOutputVariableName(UT_String &var, int idx) const;

    /// If node explicitly declares variables by itself (eg, subnet), it should 
    /// report such types with this method, in case it uses some custom
    /// data type that needs to be defined in the shader code (eg, struct).
    /// Most nodes don't need to override this, since code generator knows
    /// what types it used for this node's (output connector) variables.
    virtual void	 getExtraTypesToDefine( UT_Array<VOP_TypeInfo> &types );

    /// Returns true if the methods below should consider only the inputs
    /// of this node that belong to the given shader context.
    /// Most nodes indiscriminately merge all their inputs, but some (like 
    /// Material Buider VOP) need pick inuts that belong to the given context.
    virtual bool	 shouldCheckInputContextForMergeInputNodeList() const;

    /// Recurse through our inputs and add them in order to the given list.
    /// Note this this might create new auto-conversion nodes in this list,
    /// and these will have to be deallocated by the caller (using
    /// the utility function deleteAutoConvertNodesIn())
    void		 mergeInputNodeList(VOP_NodeList &nodes, 
				TMergeNodeInfos &merge_infos,
				const VOP_CodeGenContext *context = nullptr);
    void		 mergeInputNodeListInternal(VOP_NodeList &nodes, 
				TMergeNodeInfos &merge_infos,
				VOP_NodeList &extraterminals,
				const VOP_CodeGenContext *context = nullptr);

    /// Scans the node list and deletes the auto convert nodes (and replaces
    /// them with NULLs in the list). Also, clears any auto-convert information
    /// for all other nodes in the list. Note, thet the node list is usually
    /// constructed with mergeInputNodeList() method.
    static void		 deleteAutoConvertNodesIn(VOP_NodeList& nodes);

    /// Determines if this is a terminal node. Ie, whether this node is a
    /// code sink that culminates (one of) the shader calculations. 
    /// The culmination may be in the form of setting the global variable,
    /// setting an export parameter, or a general reqest by a user to
    /// ensure the node is part of the shader (eg, print vop).
    /// The code code generator determines all the nodes that participate
    /// in the shader definition by by recursively traversing the inputs
    /// starting from this node.
    /// @param check_shader_context Is true when the node should consider
    ///	    the shader context name/type contained in the code gen context
    ///	    parameter; the node should return true only if it really
    ///	    is a terminal node for the given shader type.
    ///	    Otherwise, if false, the node can disregard the shader type
    ///	    most likely because it lives in a single-context vopnet.
    virtual bool	 forceCodeGenerationOfInputs(
					const VOP_CodeGenContext &context,
					bool check_shader_context) const;

    /// Returns true if the node can and should initiate generation of code 
    /// in the given context. If, if the node is a terminal node.
    bool		forceCodeGenerationInContext( 
					const VOP_CodeGenContext &context)const;

    /// Returns true if the given output represents an export variable for
    /// a shader of a type specified by context.
    bool		isInputForShaderType(int input_idx, 
					const VOP_CodeGenContext &context)const;
    bool		isOutputForShaderType(int output_idx, 
					const VOP_CodeGenContext &context)const;

    /// @{ Returns a node that implements or represents the given shader type.
    /// The returned node may be a stand-alone (self-contained) shader node,
    /// or a building-block node from which a shader of a given type can
    /// be generated.
    /// The first variant tests for a given single shader type.
    /// The second variant returns all applicable shader nodes and their
    /// corresponding shader types.
    virtual VOP_Node	*findShader(VOP_Type shader_type, fpreal now);
    virtual void	 findAllShaders( VOP_NodeList &shader_nodes,
					VOP_ShaderTypeList &shader_types );
    /// @}

    /// Returns true if the VOP can represent a shader. Ie,
    /// whether it can be converted into a shader.
    virtual bool	 canDefineShader() const;

    /// Returns true if the VOP is a stand-alone shader (and not
    /// a building-block for a shader).
    /// The VOP may generate the code from its children VOPs, point to 
    /// an external file, or store the code in the HDA section.
    /// But, it represents a self-contained shader code, which can be invoked
    /// for shading an object (surface), used as an encapsulated shader (CVEX
    /// shader for geoemtry procedurals, like fur), or called from another
    /// shader (as a shader function call).
    virtual bool	 isShader() const;

    /// Returns true if the VOP is a stand-alone shader (and not a 
    /// building-block for a shader), and can provide the shader code for 
    /// a shader of the given type. Note, material VOP may return true for 
    /// several shader types, such as both surface and displacement.
    virtual bool	 isShader(VOP_Type shader_type) const;

    /// Returns true if the node has pre-generated shader code and is not
    /// using network to generate it. This mainly applies to digital assets
    /// that store shader code in their sections (whether or not they also
    /// store the network contents).
    virtual bool	 isCachedShader() const;

    /// Returns true if the operator represents an externally defined shader
    /// such as a Geometry Shader, or a shader whose code is saved in a file
    /// on disk separately from the operator or HDA definition.
    bool		 isExternalShader() const;

    /// Returns true if this is a subnetwork which generates an encapsulated
    /// shader.  For example the SubnetCVEX VOP.  Parent material passes 
    /// encapsulated shaders as shader string to other shaders (usually,
    /// geometry procedurals) to calculate certain properties dynamically.
    /// And to construct shader string there are some restrictions on
    /// encapsulated shaders (eg, their inputs can be only wired to constants 
    /// and parameter VOPs).
    // TODO: But there is little distinction between them and regular shaders,
    //	     so remove this method. The difference is really how they are used
    //	     (ie, whether the shader string of the main shader like fur
    //	     procedural contains the shader string for this cvex 'encapsulated'
    //	     shader. But, to distinguis that, the clerk calls
    //	     hou.VopNode.shaderString(as_encapsulated=True)
    virtual bool	 isEncapsulatedShader() const;

    /// Material VOP may be able to return a procedural node.
    virtual VOP_Node	*getProcedural(VOP_Type type);

    /// @{ Returns the name, type, and render mask of the VOP shader.
    virtual UT_StringHolder 
			getShaderName(bool forvex, VOP_Type shader_type) const;
    virtual VOP_Type	getShaderType() const;
    const char *	getRenderMask() const;
    /// @}

    /// @{ For expanding compiler string used in auto code generator.
    virtual bool	 evalVariableValue(UT_String &v, int index, int thread);
    virtual bool	 evalVariableValue(fpreal &v, int i, int thr)
			 { return OP_Network::evalVariableValue(v,i,thr); }
    /// @}

    /// @{
    /// Obtains a coshader node (or array of nodes) associated with the parm.
    /// Usually, the co-shader computes the value for the parameter, and
    /// is represented as a wired input to the main shader node.
    /// If no coshaders are wired in, the output list is empty.
    void		 getCoShaderNodes(UT_ValArray<VOP_Node *> &coshaders,
				    const char* parm_name);
    void		 getCoShaderNodesOutputNames(UT_StringArray &outputs,
				    const char* parm_name);
    /// @}

    /// Writes out the shader code from the cache to the given output stream.
    /// Returns true if successful, otherwise, returns false, if there's no 
    /// cached shader code for this context.
    virtual bool	 getCachedShaderCode(std::ostream &os,
                	            VOP_ContextType context_type) const;

    /// @{
    /// To be able to use Cvex VOP in Attribute SOP, etc.
    using VexBuildFn1 = void (*)(VOP_Node*, UT_String&, fpreal, OP_Node*, 
	    UT_Map<int,bool>*);
    using VexBuildFn2 = void (*)(VOP_Node*, UT_String&, fpreal, DEP_MicroNode*, 
	    UT_Map<int,bool>*);
    static void		 setBuildVexScriptCB(VexBuildFn1 fn1, VexBuildFn2 fn2);
    virtual void	 buildVexScript(UT_String &result, fpreal t,
				OP_Node *parent, UT_Map<int, bool> *visited);
    virtual void	 buildVexScript(UT_String &result, fpreal t,
				DEP_MicroNode *depnode, UT_Map<int, bool> *v);
    /// @}


    /// @{ Returns true if the VOP represents a method inside a class.
    virtual bool	 isVopMethod() const;
    virtual bool	 isVopMethodPublic() const;
    /// @}

    /// @{ If the node represents a function, a method, or a shader,
    /// then these functions return the info (name, return type, args) about 
    /// the function or a shader function as it appears in the source code.
    virtual void	 getVopFunctionName(UT_String &function_name) const;
    virtual void	 getVopFunctionReturnInfo(VOP_FunctionArgInfo &i) const;
    virtual void	 getVopFunctionArgInfos(
				UT_Array<VOP_FunctionArgInfo> &arg_infos);
    /// @}

    /// Helper method for getVopFunctionArgInfos() to obtain shader function 
    /// parameter infos based on the node inputs and outputs.
    /// Optionally, the method can prefix function argument names with an 
    /// underscore, which is customary for shaders based on subnets. If so,
    /// the variable name will be also implicitly prefixed (otherwise,
    /// it would most likely be resolved to an input value).
    /// If argument name is not prefixed, the local variable used in a call
    /// may still be optionally prefixed (when needed to avoid var replacement).
    void		getVopFunctionArgInfosFromInputsAndOutputs( 
				UT_Array<VOP_FunctionArgInfo> &arg_infos, 
				int first_input, int last_input,
				int first_output, int last_output,
				bool prefix_input_arg_and_var_names,
				bool prefix_input_var_names = false,
				const VOP_CodeGenContext *context = nullptr);

    /// Helper method to obtain shader function parameter infos based on
    /// the node parameters.
    void		getVopFunctionArgInfosFromNodeParameters( 
				UT_Array<VOP_FunctionArgInfo> &arg_infos,
				const VOP_CodeGenContext *context = nullptr);
    void		getVopFunctionArgInfosFromNodeParameters( 
				UT_Array<VOP_FunctionArgInfo> &arg_infos,
				VOP_Type shader_type);

    /// Helper method to obtain shader function parameter infos based on
    /// the node inputs, outputs, and parameters.
    void		getVopFunctionArgInfosFromInputsOutputsAndParms( 
				UT_Array<VOP_FunctionArgInfo> &arg_infos, 
				int first_input, int last_input,
				int first_output, int last_output,
				bool prefix_input_arg_and_var_names,
				bool prefix_input_var_names = false,
				const VOP_CodeGenContext *context = nullptr);

    /// Helper method to obtain shader function parameter infos based
    /// on writable global variables in the shader's context.
    /// Used for shader call, that explicitly binds shader parameters
    /// to callee's globals, by listing them as arguments.
    void		getVopFunctionArgInfosFromGlobals( 
				UT_Array<VOP_FunctionArgInfo> &arg_infos, 
				VOP_Type shader_type );

    /// Helper method to obtain all known export parameters from a shader node.
    virtual void	getVopFunctionArgInfosFromExports( 
				UT_Array<VOP_FunctionArgInfo> &arg_infos, 
				VOP_Type shader_type );

    /// Gives this node a chance to register an interest in a varialbe that
    /// the upstream vop computes, but only optionally when explicitly asked.
    virtual void	 registerExtraVarRequests(
				VOP_ExtraVarRequests &extra_var_requests,
				const VOP_CodeGenContext &context);

    /// Removes entries that correspond to a shader function parameters
    /// at a default value (or unconnected).
    /// If @p keep_export_args is true, all exports args are kept, even
    /// if they are at default. Otherwise, at-default exports are pruned.
    void		pruneVopFunctionArgInfos(
				UT_Array<VOP_FunctionArgInfo> &arg_infos,
				bool keep_all_export_args = false );

    /// Returns true if the parameter of a given name is at default value.
    /// If the parameter template is also given, it is used for default value,
    /// otherwise the parameter's template is used.
    bool		isParmAtDefaultValue( const char *parm, 
			    const PRM_Template *tplate = nullptr ) const;

    /// Writes declarations of variables used as argument to a function call.
    /// @param[out] buffer
    ///		Outgoing argument that is filled with variable declarations.
    /// @parm[in] arg_infos 
    ///		Information about inputs/output variables to write out.
    /// @parm[in] use_defined_input_defaults	
    ///		Flags whether the input variables should be initialized to
    ///		special defaults defined by the owner subnet.  If true, any
    ///		unconnected inputs will be initialized to such specially defined
    ///		defaults (connected inputs will be initialized to incoming
    ///		variables corresponding to the incoming wire).  If false, the
    ///		input defaults defined by owner subnet are not used, and all
    ///		inputs are initialized to incoming variables, which resolve to
    ///		the value if input is connected or to the empty constant or node
    ///		parm value.
    void	    outputVopFunctionVariableDeclarations(UT_WorkBuffer &buffer,
				const UT_Array<VOP_FunctionArgInfo> & arg_infos,
				bool use_defined_input_defaults);

    /// @{ Constructs a shader call, assuming this node represents a shader.
    UT_StringHolder getVopFunctionCallName(const VOP_CodeGenContext &ctx);
    void	    getVopFunctionCallCode(UT_String &codestr, 
				const VOP_CodeGenContext &ctx);
    void	    getVopFunctionCallModules(UT_StringArray &modules, 
				const VOP_CodeGenContext &ctx);
    void	    appendVopFunctionArgument(UT_StringArray &arg_names, 
				UT_StringArray &arg_vals, 
				const VOP_FunctionArgInfo &arg_info);
    /// @}

    /// Returns true if the code generator should declare local variables
    /// for this node's outputs. Returns false, if node declares them itself.
    virtual bool    shouldOutputNodeVariableDeclarations() const;

    /// Get or set the table of global or local variables for this context.
    /// The global variables are defined solely by the VEX context. The
    /// data associated with each entry is the VEX_Type of the variable.
    /// The local variables are those defined by Parameter ops. This
    /// table provides a common place to look for variable name conflicts.
    const VOP_GlobalVarMap *getGlobalVariables() const;
    const VOP_ParmGeneratorMap *getLocalVariables() const;
    const VOP_LocalChannelMap *getLocalChannels() const;
    void		 setTables(const VOP_GlobalVarMap *globals,
        		           VOP_ParmGeneratorMap *locals,
        		           VOP_LocalChannelMap *channels);
    void		 setTablesFromVop(VOP_Node *vop);
    void		 addLocalVariable(const UT_StringRef &varname,
					  VOP_ParmGenerator *definer);
    void		 deleteLocalVariable(const UT_StringRef &varname);
    void		 deleteLocalChannelDefiner( VOP_ParmGenerator *definer);
    void		 getSortedGlobalVariableList(VOP_GlobalVarList &vars,
						     VOP_ContextType type,
						     bool mustBeReadable,
						     bool mustBeWritable);

    /// Similar to getLocalVariables() but it returns a table of Parameter ops
    /// that define subnet inputs rather than shader parameters.
    virtual VOP_ParmGeneratorMap* getSubnetVariables();
    VOP_ParmGeneratorMap*   getParentSubnetVariables() const;
    void		    addParentSubnetVariable(const char *varname,
					  VOP_ParmGenerator *definer);
    void		    deleteParentSubnetVariable(const char *varname);

    /// Returns true if input variables should be prefixed inside the subnet.
    virtual bool	 shouldPrefixInputVariablesInsideSubnet() const;

    /// Returns true if parm nodes that define subnet connectors are treated
    /// as shader parms. This is true for subnet material (or shader) nodes.
    virtual bool	 usesSubnetConnectorsAsShaderParms() const;

    /// Returns true if shader parameters should be displayed as node inputs
    /// and outputs. This is true for subnet shader nodes.
    virtual bool	 showsShaderParmsAsSubnetConnectors() const;

    /// Informs this node that some descendent (but not immediate child)
    /// was added and it represents a shader input or output.
    virtual void	 shaderParmGrandChildAdded(VOP_Node *grand_child);
    virtual void	 shaderParmGrandChildChanged(VOP_Node *grand_child);
    virtual void	 shaderParmGrandChildDeleted(VOP_Node *grand_child);

    /// Functions for getting and setting the in/out LOD value, using the
    /// OP_NodeFlag mechanism.
    virtual int		 setFlag(char flag, int onoff);
    int			 setInOutLOD(int level, int inoff);
    int			 getInOutLOD(int level) const;
    static int		&getLODPref() { return theLODPref; }
    static void		 setLODPref(int value);

    /// Override the setInput functions so that we can send notification to
    /// the nodes that are our inputs when we disconnect from them.
    virtual OP_ERROR	 setInput(unsigned idx, OP_Node *op,
				  unsigned outputIdx = 0);
    virtual OP_ERROR	 setInputReference(unsigned idx, const char *label,
					   int keeppos, unsigned outputIdx = 0);

    virtual OP_ERROR	 setNamedInput(const OP_ConnectorId &input_name,
				OP_Node *op,
				const OP_ConnectorId* output_name = NULL);
    virtual OP_ERROR	 setNamedInputReference(
				const OP_ConnectorId &input_name, 
				const char *label, int,
				const OP_ConnectorId* output_name = NULL);

    /// For subnet VOPs, when moving an input, we want to go in and
    /// reorganize the connections to our subinput and suboutput nodes
    /// so that the internal connections remain the same.
    virtual void	 moveInput(int srcidx, int dstidx,
				   bool forcesubnet = false);

    /// Return our input node for the give index. If recursive is true and we
    /// are in a subnet and the direct input for inputidx is a subnet input
    /// node, travel up the parent chain until no longer in a subnet or the
    /// input node is not a subnet input node. Basically, we want to know if we
    /// are connected to anything meaningful.
    virtual bool	 isConnected(int inputidx, bool recursive);

    /// Returns a pointer used by OPUI_OutputNameEditor to get and set data.
    virtual VOP_OutputNameEditorSource	*getOutputNameEditorSource()
			 { return 0; }


    /// Some material vops cache a list of shader that needs to be dirtied.
    virtual void	 dirtyShaderList();

    /// Used by subnet type operators which must always have an output node.
    /// However, some specialty subnets, such as volume sampler, may return
    /// NULL, since its output node is not a regular subnet output node that
    /// determines subnet output connectors (which is for what the subnet base 
    /// class uses the output node returned by this method).
    virtual VOP_Node	*getSubnetOutputNode() const;

    /// Returns true if this node is immediatly inside a non-shader subnet.
    bool		 isInNonShaderSubNetwork() const;

    /// Returns true if the node is considered a subnet's input node.
    virtual bool	 isSubnetInput() const
			    { return false; }

    /// Returns the node from which to transfer information about local 
    /// variables used in source code generated by this node.
    virtual VOP_Node *	getPreResolveVariableSrc() const
			    { return nullptr; }

    /// Returns true if code generator should attempt to copy info about
    /// a variable corresponding to the i-th output connector.
    virtual bool	shouldPreResolveOutput(int output_index) const
			    { return true; }

    /// Returns true if the node is consdered an output node that represents
    /// a shader or a function that connects to a Collect node.
    virtual bool	 isOutputVopNode() const
			    { return false; }

    /// Retruns true if the node is an internal auto-convert node.
    virtual bool	 isAutoConvertNode() const
			    { return false; }

    /// Defines the different nodes that can be created and automatically
    /// connected to a VOP node's input.
    enum InputParmGenType
    {
	// Create a const VOP to connect to the input.
	INPUT_TYPE_CONST,
	// Create a parm VOP to connect to the input.
	INPUT_TYPE_PARM,
	// Create a subnet-scoped parm VOP to connect to the input.
	INPUT_TYPE_SUBNET_INPUT,
    };

    /// Create a Parameter or Constant VOP that matches the type, name and
    /// value of one of our inputs, and connect it to that input. 'parm'
    /// specifies whether to build a Parameter or a Constant.
    VOP_Node		*insertParmGenerator(int inputidx,
				InputParmGenType type);
    VOP_Node		*insertNode(int inputidx, const char* nodetype,
				bool connect_to_input,
				const char* undo_string = NULL);

    int			 inputParmTypeIndex(int inputidx,
				const PRM_Parm* parm) const;
    const char		*inputParmTypeString(int inputidx,
				const PRM_Parm *parm) const;

    /// When inserting a parameter (i.e. Promote Parameter), the source VOP is
    /// allowed to give us a source VOP_ParmGenerator if there's no
    /// corresponding parameter found.  The source parm generator is used to
    /// initialize settings on the new parameter node.
    virtual VOP_ParmGenerator	*getInsertParmGeneratorSource(int inputidx);
    
    /// Create Parameter or Constant VOPs for all inputs of this node. We
    /// don't create nodes for inputs that are already connected.
    /// If `created_vops` is not NULL, then the method will populate the table
    /// with the input names for the keys and the created VOP nodes for the
    /// values.
    void		 insertParmGeneratorsForAllInputs(
			    InputParmGenType type,
			    UT_SymbolMap<VOP_Node *> *created_vops=NULL);

    /// Create Bind Export VOPs for all outputs of this node.
    /// Set `subnet` to true to create subnet-scoped Bind Export VOPs that
    /// produce output connectors on the owner subnetwork.
    void		 insertBindExportsForAllOutputs(bool subnet);
    VOP_Node		*insertBindExport(int outputidx, bool subnet);

    /// Updates the output connections based on the correspondence
    /// between old and new output names.
    void		 rewireOutputConnections( 
				const UT_StringArray &old_names,
				const UT_StringArray &new_names );

    /// The following method is needed to obtain the parameter name to be
    /// associated with an input.  It is possible that the parameter name
    /// differs from the input name.
    virtual void	 getParmNameFromInput(UT_String &parmname,
					      int inputidx) const;
    virtual void	 getParmNameFromOutput(UT_String &parmname,
					      int outputidx) const;

    /// Find the parameter associated with an input, if any
    const PRM_Parm	*getParmFromInput(int inputidx) const;
    const PRM_Parm	*getParmFromOutput(int outputidx) const;

    /// Based on the specified input index, output a parm definition block
    /// appropriate for a dialog script. The parm definition provides a
    /// default value for the input if it is not connected. If the input is
    /// VEX_TYPE_UNDEF, we output nothing and return false. Otherwise we return
    /// true.
    bool		 outputDefaultParmDefinition(std::ostream &os, int idx);

    /// Writes out the input and output connector definitions.
    void		 saveNodeInputAndOutputDefinitions(std::ostream &os);

    virtual void	 saveDialogScriptExtraInfo(std::ostream &os);

    /// Saves the parameter values to the output stream.
    bool		 saveParmValues(std::ostream &os);
    /// Loads the parameter values to the output stream.
    bool		 loadParmValues(UT_IStream &is);

    /// This virtual is overridden by VOP_ParmGenerator so we can find out
    /// which VOP defines the actual parameter attributes.
    virtual VOP_Node	*getRealDefinition()
			 { return 0; }

    const VOP_Language	*getLanguage() const
			 { return myLanguage; }

    /// Absolute width
    virtual fpreal	 getW() const;
    /// Absolute height
    virtual fpreal	 getH() const;

    /// Override clearInterrupted so that we don't recook the whole DOP Network
    /// when the user interrupts a cook. The clearInterrupted on the containing
    /// VOP Network willb be enough.
    virtual void	 clearInterrupted();

    /// Calls cacheParmName on all VOP_ParmGenerator nodes under root.
    static void		 cacheAllParmNames(OP_Node *root);
    /// Calls recalculateParmDefiner on all VOP_ParmGenerator nodes under root.
    static void		 recalculateAllParmDefiners(OP_Node *root,
				bool dive_into_subnets = true,
				const UT_StringRef &target_parm_name
							    = UT_StringRef());

    /// Returns true if the input will be auto-converetd, false otherwise.
    virtual bool	 willAutoconvertInputType(int input_idx);

    /// Returns the type to which the input will be auto-convert. This is
    /// necessary because some inputs can take in a range of various types,
    /// and we need to know which one will be converting to. This function
    /// assumes that autoconversion is necessary. If it is not possible, or
    /// not needed, it returns VOP_TYPE_UNDEF.
    VOP_TypeInfo	 getAutoConvertTargetTypeInfo(int input_idx);
    VOP_Type		 getAutoConvertTargetType(int input_idx);

    // Similar to the getAutoConvertTargetType(), but takes in an input type
    // instead of trying to read it from the connected node. This is useful
    // when one needs to know what the incoming type will be converted to
    // before it is actually connected.
    VOP_TypeInfo	 getAutoConvertTypeInfoFromType(int input_idx,
				const VOP_TypeInfo &source_type);
    VOP_Type		 getAutoConvertTypeFromType(int input_idx,
				VOP_Type source_type);

    /// Do not call this method directly.
    void		 addAutoConvertNodePointer(VOP_AutoConvert* ac_node,
				int input_index);
    void		 clearAutoConvertInfos(void);
    VOP_AutoConvert*	 getAutoConvertNode(int input_idx);
    const VOP_AutoConvert*getAutoConvertNode(int input_idx) const;
    int			 getNumAutoConvertNodes(void);

    // Returns the output information manager. The manager is responsible for
    // manipulating VOP_OutputInfo objects for each output. These objects
    // dictate how an output behaves when the node is in debug or bypass mode.
    // See VOP_OutputInfo for more information.  Please note that by default,
    // an output info does not exist for any outputs, and is only created when
    // a debug/bypass value is changed.
    VOP_OutputInfoManager* getOutputInfoManager(void);

    // Appends code which overrides output values if necessary (uses
    // myOutputInfos below to determine this).
    void appendOutputOverrideCode(UT_String& str_code);

    // If this returns false, getCode() will not be called during code
    // generation on this node (appendOutputOverrideCode() still will). This is
    // useful when all outputs of this node are overriden, and no execution is
    // necessary.
    bool needToExecute(void);

    /// Do not call this method directly.
    void onNodeChange(OP_EventType type);

    virtual OP_ERROR	 save(std::ostream &os, const OP_SaveFlags &flags,
			      const char *path_prefix = "",
			      const UT_String &name_override = UT_String());
    virtual bool	 load(UT_IStream &is, const char *ext="",
					      const char *path=0);

    /// Gets/sets the value of the debug flag on this node.
    virtual int		 getDebug() const;
    virtual int		 setDebug(int on_off);

    /// Gets/sets the value of the material flag on this node.
    int			 getMaterialFlag() const;
    int			 setMaterialFlag(int on_off);
    virtual void	 initMaterialFlag();

    void		 getSubOutputAndSubInputs(OP_Node *&outputnode,
				      OP_NodeList &inputnodes) const;

    void		hideInput(int input_index, bool hide); 
    void		setReferenceTargetDefaults(int input_index,bool do_set);
    bool		getAllowEditingParmDefaults(int input_index);

    /// Determine the VEX context type if the node is single-context type.
    /// For nodes in a multi-context vopnet returns
    /// VEX_INVALID_CONTEXT, because the specific VEX context cannot be 
    /// reliably and unambiguously determined.
    // TODO: remove these methods
    VEX_ContextType	 getVexContextType() const;
    RSL_ContextType	 getRslContextType() const;
    OSL_ContextType	 getOslContextType() const;

    /// Determines if the vop can be interpreted as a single or several RSL/VEX
    /// context types (surface, disp, ..), by looking at code generator.
    /// Returns true if the vop node is multi-type and false if single-type.
    bool		 isSingleContextType() const;

    /// Gets the context type (VEX_ContextType or RSL_ContextType) to which
    /// this node subscribes. 
    /// In single-type vopnets, it is equal to the node's VEX/RSL context type.
    /// In multi-type vopnets, the exact type is variable and usually 
    /// depends on the node's parameter value.
    VOP_ContextType	 getShaderContextType() const;

    /// Returns a parmmap string array where alternate entries contain key and
    /// value, where key is the vop parameter name and the value is the
    /// parameter name specified by the Parm VOP connected to the input
    /// corresponding to that vop parameter.
    virtual void	 getFixedParameterMap(UT_StringArray &parmmap);

    /// Returns an input map string array where alternate entries contain
    /// key and value, where key is the vop parameter name and the value is the
    /// node path specified for that parameter (usually connected into an
    /// input corresponding to that vop parameter).
    virtual void	 getShaderInputMap(UT_StringArray &inputmap);

    /// Utility function to check if a parm by a given name is a cvex shader.
    bool		 isCVEXPathParm(const char *parm_name) const;

    /// Obtains the template array for shader parameters.
    virtual const PRM_Template	*getShaderParmTemplates();
    static void			 refreshShaderParmTemplatesIfNeeded(OP_Node *n);

    /// Deletes the input nodes for a given input (recursively), but
    /// only if none of them depend on anything other than this node.
    /// Returns true if any nodes were deleted, false otherwise.
    bool		 deleteIndependentInputNodes(int input_index);

    void		 getInputsRecursive(OP_NodeList& nodes_out);
    bool		 doesDependOnlyOn(const VOP_Node* other_node, 
				    int max_connections, bool recurse);

    /// Search through this node's inputs and return a list of parameter VOPs
    /// that appear in the input network.  Recursively search through
    /// subnetworks as well.  If this node is a parameter VOP, then add
    /// it to the list.
    void		 getParmInputs(VOP_ParmGeneratorList &parm_vops);

    /// Returns the node wired to this VOP's 'index' input tracing past
    /// simple VOPs like the Switch or Null VOP.
    VOP_Node		*findSimpleInput(int index);
    /// Virtual helper to findSimpleInput() which returns the simple input
    /// node (ie the final node we look for) based on the request coming
    /// from the node connected to on the output of a given index.
    virtual VOP_Node	*findSimpleInputFromOutput(int output_index) 
			 { return this; }
    /// A method that reports which immediate input connectors are considered
    /// by findSimpleInputFromOutput() method. This is useful when analyzing
    /// the simple input decision tree.
    virtual void	 findSimpleInputCandidatesFromOutput(int output_index,
					    UT_IntArray & input_indices )
			 { input_indices.clear(); }

    /// Gets a default value of an explicitly defined input (applies to subnets)
    virtual void	 getInputDefaultValue(UT_String &def, int idx) const;

    /// Gets number of outputs provided by a subnet, in addition to the outputs
    /// defined by wires connected to sub output.
    virtual int		 getNumNodeDefinedOutputs() const;

    /// @{ VOP subnets may restrict which inputs should show up in the
    /// Subnet Input and Subnet Output VOPs. These two methods control that.
    /// Gets number of subnet inputs available to subnet input/output children.
    /// Gets the subnet input index corresponding to the given index of
    /// (limited number of) input available to subnet input/output children.
    virtual int	    getNumSubnetInputsForChildren() const;
    virtual int	    getSubnetInputIndexForChildren(int child_input) const;
    int		    getChildConnectorIndexFromParent(int parent_input) const;
    /// @}

    /// Obtains additional nodes that should be copied or deleted when this
    /// node is copied or deleted. 
    virtual void	 getExtraNodesForCopyOrDelete(OP_NodeList &extras)const;

    /// Some clerks add own stuff on creation. Any such action can be
    /// registered with this method.
    static void		 setRunCreateScriptCB( void (*)(const char *node_path, 
				const char*render_mask, VOP_Type shader_type ));

    // Runs the creation script (if there is one). The return value is false
    // if the node was deleted while running the script. In this case
    // obviously the node pointer becomes invalid and should not be used
    // any more. It returns true if the node still exists.
    virtual bool	 runCreateScript();

    void		 updateInputFlagsFromOperator(
				VOP_OperatorInfo *type_info, 
				bool keep_visible);

    virtual void	onCreated(void);
    void		hideNamedInput(const OP_ConnectorId& connector_name,
					bool hide); 

    /// Returns all allowed input types on a given input, regardless of what
    /// is currently connected to that input. These do not include types
    /// to which autoconversion is possible.
    virtual void	 getAllowedNamedInputTypeInfos(
					const OP_ConnectorId& input_name,
					VOP_VopTypeInfoArray &typeinfos);
    virtual void	 getAllowedNamedInputTypes(
					const OP_ConnectorId& input_name,
					VOP_VopTypeArray &voptypes);

    VOP_TypeInfo	 getNamedAutoConvertTargetTypeInfo(
					const OP_ConnectorId& input_name);
    VOP_Type		 getNamedAutoConvertTargetType(
					const OP_ConnectorId& input_name);
    VOP_TypeInfo	 getNamedAutoConvertTypeInfoFromType(
					const OP_ConnectorId& input_name,
					const VOP_TypeInfo &source_type);
    VOP_Type		 getNamedAutoConvertTypeFromType(
					const OP_ConnectorId& input_name,
					VOP_Type source_type);

    virtual bool	 willAutoconvertNamedInputType(
					const OP_ConnectorId& input_name);
    VOP_Node		*insertNamedNode(const OP_ConnectorId& input_name,
					const char* nodetype,
					bool connect_to_input,
					const char* undo_string = NULL);

    virtual const OP_DataMicroNode &	dataMicroNodeConst() const
				{ return myVopDataMicroNode; }

    /// Exposes protected method for taking over the errors from global manager.
    /// Should be used only by cerain helpers.
    void		 stealGlobalErrorsForHelper()
				{ stealGlobalErrors(); }

    /// @{ Manipulate cook error dirty state
    void		 setErrorsDirty();
    void		 setErrorsClean(const OP_Context &context);
    bool		 areErrorsDirty(const OP_Context &context);
    /// @}
		
    /// Obtains the idx-th input type info from the connected input node. 
    /// If not connected, type info is set to undefined.
    /// @param	type_info   The type info to fill out.
    /// @parm	idx	    The index of the input from where to take the type.
    /// @param	grow_inputs_to_idx 
    ///			    If true and the the requested idx is larger than
    ///			    the input list size, then the list is expanded
    ///			    to make the requested index legal. Otherwise,
    ///			    list is kept constant and input is treated as NULL,
    ///			    resulting in an info for an undefined type.
    /// @return		    True if the info was obtained from the vop,
    ///			    otherwise false (info is set to undef, by default).
    bool		 getInputTypeInfoFromInputNode( 
				    VOP_TypeInfo &type_info, int idx, 
				    bool grow_inputs_to_idx = false) const;

    /// Obtains the idx-th input name based on a connected input node.
    /// If not connected, the name is "next".
    /// It's ensured that the name is unique even if many input nodes
    /// provide the same name.
    /// If check_output_names is true, the input name is ensured to be unique
    /// and different from the all the output names too.
    void		 getInputNameFromInputNode(UT_String &in, int idx,
				    bool check_output_names)const;

    /// @{ Virtual overrides for evaluating parameters as render properties,
    /// in case the VOP node is a shader or a material.
    virtual bool	 getParameterOrProperty(
				const UT_StringRef &name, fpreal now,
				OP_Node *&op, PRM_Parm *&parm,
				bool create_missing_multiparms,
				PRM_ParmList *obsolete = 0);
    virtual bool	 getParameterOrPropertyByChannel(
				const UT_StringRef &chname,
				fpreal now, OP_Node *&op,
				PRM_Parm *&parm, int &vector_index,
				PRM_ParmList *obsolete = 0);
    virtual int		 findParametersOrProperties(fpreal now,
				OP_PropertyLookupList &list);
    /// @}

    /// Queries information about visible connectors. This takes into account
    /// grouping of inputs, the current LOD flag, etc.
    int				 getInputVisibleIndex(int idx);
    const vop_ConnectorArray	&getVisibleInputConnectors();
    int				 getOutputVisibleIndex(int idx);
    const vop_ConnectorArray	&getVisibleOutputConnectors();

    /// Get or set the expansion state of an input group for this node.
    /// The information is stored in the user data.
    void		 setInputGroupExpanded(const UT_StringHolder &group,
				bool expanded);
    void		 setAllInputGroupsExpanded(bool expanded);
    bool		 getInputGroupExpanded(const UT_StringHolder &group);

    /// Fetches grouping information for all inputs whether they
    /// are visible or not. All inputs not in a group will be
    /// returned in the map with an empty string as the key.
    void		 getAllInputsGrouped(UT_StringArray &groups,
				UT_StringMap<UT_IntArray> &mapping);

    /// Give this VOP ownership of a TIL_Thumbnail object. Deletes any
    /// thumbnail that was already here.
    void		 setThumbnail(TIL_Thumbnail *thumbnail);
    /// Returns a pointer to our thumbnail if we have one.
    TIL_Thumbnail	*getThumbnail() const;
    /// Handle changes to our user data. This may require a redraw.
    virtual void	 userDataChanged(const UT_StringHolder &key);

    // Global preferences controlled by the pref dialog.
    static bool		 getShowPreviewPref();
    static void		 setShowPreviewPref(bool show);
    
protected:
    VOP_Node(OP_Network *parent, const char *name, OP_Operator *entry);
    virtual ~VOP_Node();

    virtual void	 getNodeSpecificInfoText(OP_Context &context,
				OP_NodeInfoParms &iparms);
    virtual void	 fillInfoTreeNodeSpecific(UT_InfoTree &tree, 
				const OP_NodeInfoTreeParms &parms);

    virtual void	 deleteCookedData();
    virtual int		 saveCookedData(std::ostream &, OP_Context &,
					int binary = 0);
    virtual int		 saveCookedData(const char *, OP_Context &);
    virtual const char	*getFileExtension(int) const;

    virtual OP_ERROR	 cookMe(OP_Context &);
    virtual OP_ERROR	 bypassMe(OP_Context &, int &);

    virtual void	 preOpChanged(OP_EventType, void *)
			 { }
    virtual void	 postOpChanged(OP_EventType, void *)
			 { }

    /// @{ Helpers for getParameterOrProperty() and 
    /// getParameterOrPropertyByChannel()
    bool		getPropertyFromNode(
				const UT_StringRef &name, fpreal now,
				OP_Node *&op, PRM_Parm *&parm,
				bool add_missing_mparms, PRM_ParmList *obsolet);
    bool		getPropertyFromNode(
				const UT_StringRef &chname, fpreal now, 
				OP_Node *&op, PRM_Parm *&parm, int &vec_index,
				PRM_ParmList *obsolete);
    bool		getPropertyFromCodeGen(
				const UT_StringRef &name, fpreal now,
				OP_Node *&op, PRM_Parm *&parm,
				bool add_missing_mparms, PRM_ParmList *obsolet);
    bool		getPropertyFromCodeGen(
				const UT_StringRef &chname, fpreal now, 
				OP_Node *&op, PRM_Parm *&parm, int &vec_index,
				PRM_ParmList *obsolete);
    bool		getPropertyFromInputs(
				const UT_StringRef &name, fpreal now,
				OP_Node *&op, PRM_Parm *&parm,
				bool add_missing_mparms, PRM_ParmList *obsolet);
    bool		getPropertyFromInputs(
				const UT_StringRef &chname, fpreal now, 
				OP_Node *&op, PRM_Parm *&parm, int &vec_index,
				PRM_ParmList *obsolete);
    /// @}

    /// Gets the shader context type (VEX_ContextType
    /// or RSL_ContextType) to which a node belongs to. 
    ///
    /// This method is meant to be overriden by nodes that can specify a single
    /// concrete context type when they are created in a multi-context vopnet.
    /// This method is invoked by getShaderContextType() to obtain the concrete
    /// type, if possible
    ///
    /// In multi-context vopnets, for nodes that explicitly specify a single
    /// context (such as an output node, usually determined from the node's
    /// parameter), this method returns such explicltly selected context
    /// type. For other nodes that don't explicltly choose a context, this
    /// method returns an invalid type, meaning such nodes don't have control
    /// over a specific type, and they belong to a type specified during code
    /// generation.
    virtual VOP_ContextType	getSpecificShaderContextType() const;

    /// Obtains a list of shading contexts in which this node wants to initiate
    /// the generation of code.
    virtual void		getContextsForCodeGeneration(
					UT_StringArray & context_names ) const;

    /// Functions to get information about our operator outputs.
    /// These functions are used for variable name replacement and for
    /// creating output labels on the op tiles.
    virtual void	 getOutputNameSubclass(UT_String &out, int idx) const;
    virtual VOP_Type	 getOutputTypeSubclass(int idx);
    virtual void	 getOutputTypeInfoSubclass(VOP_TypeInfo &type_info, 
						   int idx);
    virtual VOP_VariableTagsHandle  getOutputVariableTagsSubclass(int idx,
					VOP_Type shader_type);

    /// Clears the cached info (name and type) about output connectors.
    void		 dirtyCachedOutputData();

    /// Returns the variable name associated with a given input number.
    /// Implementers of this function should also implement the reverse
    /// function.
    virtual void	 getInputNameSubclass(UT_String &in, int idx) const;
    virtual int		 getInputFromNameSubclass(const UT_String &in) const;
    int			 findInputFromInputName(const UT_String &in) const;
    virtual void	 getInputTypeInfoSubclass(VOP_TypeInfo &type_info, 
						  int idx);
    // Depreciated in favour of getInputTypeInfoSubclass():
    virtual VOP_Type	 getInputTypeSubclass(int idx);
    virtual bool	 getIsInputVisibleDefaultSubclass(int idx);

    /// @{ One of these two methods should be overriden in most VOP nodes. 
    /// It is preferable to override the one that gets type infos, since
    /// they can encode complex types. 
    /// They should fill in voptypes vector with vop types that are allowed to 
    /// be connected to this node at the input at index idx. 
    /// Note that this method should return valid vop types even when nothing 
    /// is connected to the corresponding input.
    virtual void	 getAllowedInputTypeInfosSubclass(unsigned idx, 
					      VOP_VopTypeInfoArray &infos);
    virtual void	 getAllowedInputTypesSubclass(unsigned idx,
					      VOP_VopTypeArray &voptypes);
    /// @}

    /// If the can only accept parameters as inputs, this method should return
    /// true.  By default, this method checks the operator to see whether the
    /// VOP relies on external code (i.e. geometry procedurals)
    virtual bool	 getRequiresInputParameters() const;

    /// Disables parameters whose corresponding inputs are connected.
    bool		 disableConnectedParameters();

    void		 addError(VOP_ErrorCodes code, const char *msg=0,
        		          const UT_SourceLocation *loc=0)
			 { UTaddError("VOP", code, msg, loc); }
    void		 addMessage(VOP_ErrorCodes code, const char *msg=0,
        		            const UT_SourceLocation *loc=0)
			 { UTaddMessage("VOP", code, msg, loc); }
    void		 addWarning(VOP_ErrorCodes code, const char *msg=0,
        		            const UT_SourceLocation *loc=0)
			 { UTaddWarning("VOP", code, msg, loc); }
    void		 addFatal(VOP_ErrorCodes code, const char *msg=0,
        		          const UT_SourceLocation *loc=0)
			 { UTaddFatal("VOP", code, msg, loc); }

    virtual void	 addDiagnosticInfo(const VCC_DiagnosticInfo &diag);

    /// This function is used to rewire nodes when we do a moveInput.
    static void		 rewireInputs(OP_Network *parent, OP_NodeList &inputs,
				      int srcidx, int dstidx);

    const VOP_Language	*myLanguage;

    virtual void	getAdditionalUndoNodes(const OP_NodeList& orig_list,
				OP_NodeList& nodes_for_input_undo);

    /// Save and load VOP version numbers
    virtual OP_ERROR	 saveIntrinsic(std::ostream &os,
				const OP_SaveFlags &flags);
    virtual bool	 loadPacket(UT_IStream &is, short class_id,
				    short sig, const char *path = 0);
    virtual bool	 loadPacket(UT_IStream &is,
				    const char *token, const char *path = 0);

    /// If a subclass has VOP versioning, it should return a value greater
    /// than 0 for the latest version.
    virtual int		 getLatestVopVersion() const
			 { return 0; }

    void		 getInputsRecursiveHelper(OP_NodeList& nodes_out,
				UT_Array<int> &array);
    
    bool		 doesDependOnlyOnHelper(const VOP_Node* other_node,
				int max_connections, 
				bool recurse,
				UT_Array<int> &array);

    virtual void	 onInputAllocated(OP_Input* new_input, int index);

    virtual void	 finishedLoadingNetwork(bool is_child_call=false);
			
    virtual void	 ensureErrorsAreUpdatedSubclass();

    /// Generate the node errors.
    /// Return true if an error was generated and false otherwise.
    /// This method can be overridden to generate errors 
    /// specific to the subclass.
    virtual bool	 generateErrorsSubclass();
    
    /// Returns true if the node should attempt testing its parameters
    /// for time dependence during the error generation pass.
    virtual bool	 shouldCheckTimeDependence() const;

    int			 myVopVersion;

    /// This returns true if it is okay to modify parms based on input changing
    /// and false otherwise.  The decision is based on things like whether or 
    /// not we are being loaded or running in a creation script.
    bool		 allowedToChangeParms();

    /// @{ Represents this node as a co-shader. Usually it just adds itself
    /// to the list, but some VOPs may represent a co-shader array, in which 
    /// chase, they add a bunch of other VOPs that make up the array.
    virtual void	 representAsCoShader(UT_ValArray<VOP_Node*> &coshader);
    virtual void	 representAsCoShaderOutputName(UT_StringArray &name,
				int output_index);
    /// @}

    /// Ensures the given connector name does not have any conflicts
    /// with other connectors and is safe to use.
    /// If check_outputs' is true, the output names are checked for conflicts
    /// too, otherwise only inputs are tested.
    void		 findSafeConnectorNameBase(UT_String &in, 
						bool check_outputs ) const;

    /// Returns the name the input connector would prefer to have,
    /// if we did not need to worry about multiple inputs having the same name.
    virtual void	 getTentativeInputName(UT_String &in, int idx) const;

    /// Returns the name the output connector would prefer to have,
    /// if we did not need to worry about multiple outputs having the same name.
    virtual void	 getTentativeOutputName(UT_String &out, int idx) const;

    /// Flag our visible connector info as dirty so it will be rebuilt the
    /// next time anyone asks for it.
    void		 setVisibleConnectorsDirty();

private:
    /// Generate the node errors.
    void		 generateErrors(const OP_Context &context);

    /// Returns true if this node can access illuminance global variables.
    bool		 getIncludeIllumVars() const;

    /// Helper method for opChanged().
    /// Propagates the node change to the VOP code generator
    /// so that the code is re-generated and the exported parameters
    /// are updated.
    void		 propagateOpChangeToCodeGenerator(
				OP_EventType reason) const;

    /// Report a recursive loop error.
    void		 reportRecursiveLoopError();
    
    /// Get node type that should be used for promoting input at given index.
    const char		*getParameterOpName(int input_index);
    const char		*getConstantOpName(int input_index);
    bool		 isInputRampRBG(int input_index);

    /// Get the output index that should be used as parameter value.
    virtual int		 getParameterOpOutputIndex();

    /// Create Parameter VOPs for all inputs of this node. We
    /// don't create nodes for inputs that are already connected.
    /// If `created_vops` is not NULL, then the method will populate the table
    /// with the input names for the keys and the created VOP nodes for the
    /// values.
    void		 insertParmVOPsForAllInputs(
				InputParmGenType type,
				UT_SymbolMap<VOP_Node *> *created_vops);
    
    /// Create Constant VOPs for all inputs of this node. We
    /// don't create nodes for inputs that are already connected.
    /// If `created_vops` is not NULL, then the method will populate the table
    /// with the input names for the keys and the created VOP nodes for the
    /// values.
    void		 insertConstantVOPsForAllInputs(
				UT_SymbolMap<VOP_Node *> *created_vops);


    /// Add entries to the string array for each struct member. These entries
    /// values come from the given parameter. The values use correct syntax
    /// for the given language.
    void		 getParmConstantStringForStruct(UT_StringArray &values, 
				const VOP_TypeInfo &type_info,
				PRM_Parm &parm,
				const VOP_Language *language);

    /// Update our internal mapping of visible connectors to group names, input
    /// indices, etc.
    void		 updateVisibleConnectorInfo();
    void		 getParameterGroups(UT_StringMap<int> &group_map,
				UT_StringArray &group_names);

    const VOP_GlobalVarMap	*myGlobalVariables;
    VOP_ParmGeneratorMap	*myLocalVariables;
    VOP_LocalChannelMap		*myLocalChannels;
    UT_UniquePtr<TIL_Thumbnail>	 myThumbnail;
    exint			 myLatestOpChangedId;
    unsigned int		 myMakingInputList : 1,
				 myIsSearchingForParmInputs : 1,
				 myInitialized : 1;

    static VOP_Node		*theOriginalOpChanged;
    static exint		 theLatestOpChangedId;
    static int			 theLODPref;
    static bool			 theShowPreviewPref;

    TAutoConvertInfos		 myAutoConvertInfos;
    VOP_OutputInfoManager	 myOutputInfos;
    UT_Array<UT_IntPair>	 myInputVariableOverrides;

    mutable UT_Lock			    myCachedDataLock;
    mutable int				    myCachedOutputNodeId;
    mutable UT_SymbolMap<int>		    myCachedOutputNames;
    mutable UT_ValArray< VOP_TypeInfo* >    myCachedOutputTypeInfos;

    VOP_DataMicroNode		 myVopDataMicroNode;
    VOP_CodeGenerator		*myAutoCodeGenerator;
    UT_Map<int, int>		 myVisibleInputIndexMap;
    vop_ConnectorArray		 myVisibleInputConnectors;
    UT_Map<int, int>		 myVisibleOutputIndexMap;
    vop_ConnectorArray		 myVisibleOutputConnectors;
    bool			 myVisibleConnectorsDirty;
};

#endif
