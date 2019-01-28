/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __VOP_CodeGenerator_h__
#define __VOP_CodeGenerator_h__

#include <map>
#include <string>

#include <OP/OP_Network.h>
#include <OP/OP_Node.h>
#include <VEX/VEX_VexTypes.h>
#include <UT/UT_UniquePtr.h>
#include <UT/UT_SharedPtr.h>

#include "VOP_API.h"
#include "VOP_Error.h"
#include "VOP_ErrorMicroNode.h"
#include "VOP_Node.h"
#include "VOP_Types.h"

// The names of the parameters for setting the min and max inputs for the
// operator type defined by a VOP Network.
#define VOP_MININPUTS_NAME	"mininputs"
#define VOP_MAXINPUTS_NAME	"maxinputs"

// A node may be a shader node that defines a shader function code on its own
// (eg, a vopnet) but also represent an ad-hoc temporary shader code
// constructed from node and its inputs (siblings). The temporary auto shader 
// will contain a call to the node's own shader. To disambiguate which of
// these shaders we refer to, we prefix the node path for auto shaders.
#define VOP_AUTO_SHADER_PREFIX		"_auto_"
#define VOP_AUTO_SHADER_PREFIX_LEN	6
#define VOP_AUTO_SHADER_FN_PREFIX	"auto_"
#define VOP_AUTO_SHADER_FN_PREFIX_LEN	5

class UT_StringArray;
class CH_LocalVariable;
class OP_OTLDefinition;
class OP_Network;
class VOP_Collect;
class VOP_Language;
class VOP_CodeGenerator;
class VOP_CodeVarMapperContext;
class VOP_CodeCompilerArgs;
class VOP_ExportedParmsManager;
class VOP_LanguageContextTypeList;
class VOP_VopTypeDefiner;
class VOP_MemberMethodInfo;
class VOP_ObjectCodeArgs;
class VOP_ShaderSpaceInfo;
class VCC_Compiler;
class VCC_Diagnostics;
class VCC_DiagnosticInfo;

typedef UT_Array<VCC_DiagnosticInfo> VCC_DiagnosticList;


class vop_CodeCacheData;
class vop_CodeGenOutputContext;

typedef std::map < VOP_ContextType, std::string > TContextTypeStringMap;

class VOP_API VOP_SnippetMicroNode : public DEP_MicroNode
{
public:
    VOP_SnippetMicroNode(VOP_CodeGenerator &cg);
    virtual const char *className() const
		    { return "VOP_SnippetMicroNode"; }
    virtual void becameDirty(DEP_MicroNode &src, const DEP_PropagateData &data);
protected:
    VOP_CodeGenerator	&myCodeGenerator;
};

class VOP_API VOP_CodeOperatorFilter : public OP_OperatorFilter
{
public:
    VOP_CodeOperatorFilter(const char *optypename, VOP_CodeGenerator *owner);
    virtual ~VOP_CodeOperatorFilter();

    virtual bool	 allowOperatorAsChild(OP_Operator *op);

    /// Returns true if operator's vopnet maks is accepted by one of the given
    /// shader contexts.
    static bool		 isOpVopnetMaskAllowed(OP_Operator *op, 
				const UT_StringArray &accepted_contexts,
				bool is_vex );

private:
    UT_StringArray	 myVopnetTypes;
    VOP_CodeGenerator	*myOwner;
};


class VOP_API VOP_CodeGenerator
{
public:
    /// Constructor
    /// @param owner	The network based on which the code is generated (whose
    ///			outupt node is used during code generation, etc)
    /// @param context_type A list of context type for a specific language,
    ///			which the generator will generate code for. For
    ///			single-context shaders, the list will have a single
    ///			entry, while multi-context shaders will have several.
    ///			The code generator will take ownership of that 
    ///			context type list object and will delete it in the
    ///			destructor.
    /// @param mininputs    Minimum number of inputs to the owner
    /// @param maxinputs    Maximum number of input to the owner
			 VOP_CodeGenerator(OP_Network *owner,
				   VOP_LanguageContextTypeList * context_type,
				   int mininputs,
				   int maxinputs);
    virtual 		~VOP_CodeGenerator(void);

    OP_OperatorFilter	*getOperatorFilter();
    UT_ErrorManager	&getErrorManager();

    /// Return the exported parameters manager.
    VOP_ExportedParmsManager	*exportedParmsManager() const
				    { return myExportedParmsMgr; }
    virtual bool	 hasShaderParameter(const char *parm_name);

    void		 beforeAddNode(OP_Node *node);
    void		 afterAddNode(OP_Node *node);

    /// Sometimes a VOP needs to change its variable tables (i.e. an
    /// encapsulated shader)
    void		 resetTables(VOP_Node *node);

    /// Functions for controlling when a code generator is updated.
    /// The code pattern is something like this:
    ///     int update_id = codegenerator->beginUpdate();
    ///	    ...
    ///	    if (code_needs_to_update)
    ///		codegenerator->setNeedsCodeUpdate();
    ///	    ...
    ///	    codegenerator->endUpdate(update_id);
    /// @{
    int			beginUpdate();
    void		endUpdate(int update_level);

    void		clearAllUpdateFlags()
			{
			    myNeedsCodeUpdate = false;
			}
    void		setNeedsCodeUpdate();
    /// @}

    void		 ownerChanged(OP_EventType reason, void *data);
    void		 ownerFinishedLoadingNetwork();
    void		 generateVopErrors();

    bool		 getVariableString(int index, UT_String &value);

    const char		*getEnglishName();

    /// @{
    /// The function name is mangled for some renderers (i.e. RSL) since basing
    /// the function name on the node name is not safe.  However, some places
    /// require the node name -- unmangled (i.e. dialog scripts).
    const char		*getFunctionName(UT_String &name) const;
    const char		*getInternalFunctionName(UT_String &name) const;
    /// @}
   
    /// The function path is the leading path to the shader
    const char		*getFunctionPath(UT_String &name) const;

    int			 getMinimumInputs() const;
    int			 getMaximumInputs() const;
    void		 getInternalOperatorInfo(OP_OTLDefinition &info);

    /// Returns ture if the generator can generate code for the given type.
    bool		 canGenerateCode( VOP_Type shader_type ) const;

    /// Returns a list of shader types that can be auto-generated for the owner.
    void		 getAutoShaderTypes( VOP_ShaderTypeList &types ) const;

    void		 setVexCodeDirty();

    /// Outputs vex code.
    /// @param[out] os	Stream to write the vex code to.
    /// @param[in]  shadername Specifies the name that should be used for the
    ///		    generated vex code.
    /// @param[in]  context_type Specifies the shading context for which 
    ///		     to generate vex code; relevant only for multi-context
    ///		     vopnets.
    /// @param[out]  is_pure_compiled Set to true if the vex code comes from
    ///		     a locked code source (i.e., a CPIO packet for the
    ///		     node saved in the hip file), which indicates the node
    ///		     has been compiled out.
    bool		 outputVexCode(std::ostream &os, const char *shadername,
			    VEX_CodeGenFlags flags = VEX_CG_DEFAULT,
			    VOP_ContextType ctx_type = VOP_CONTEXT_TYPE_INVALID,
			    bool *is_pure_compiled = nullptr,
			    VOP_ShaderSpaceInfo *shader_space = nullptr );

    /// Determines if the giving context has already been cached.
    bool		 isVexCodeCached(VOP_ContextType context_type) const
    {
	return findCachedCode(context_type) ? true : false;
    }

    /// Generates a VFL code and writes it to the os. 
    /// @param	shadername	The name of the shader function.
    /// @param  flags		The code generation flags.
    /// @param	context_type	For multi-context materials, this is the shader
    ///		    context for which to generate code.
    /// @param	skipheader	If true the file comment will be skpped;
    ///		    the file comment contains info about original
    ///		    node, file name, and generation time stamp.
    /// @param	output_node_name    Optional, this is a hint that specifies 
    ///		    the name of the output node for which to generate the code.
    ///		    If the node by this name is found and it supports the 
    ///		    context type (if given), then it is used for gathering
    ///		    nodes that participate in VFL code generation. 
    ///		    If the node by this name cannot be used, then a Collect VOP
    ///		    node will be used. And if there is no collect node
    ///		    a (arbitrary) output node supporting the context type
    ///		    will be used.
    virtual bool	 outputVflCode(
			    std::ostream &os, const char *shadername,
			    VEX_CodeGenFlags flags = VEX_CG_DEFAULT,
			    VOP_ContextType ctx_type = VOP_CONTEXT_TYPE_INVALID,
			    bool skipheader = false,
			    const char * output_node_name = nullptr,
			    VCC_Diagnostics *diag = nullptr,
			    VOP_ShaderSpaceInfo *shader_space = nullptr );

    /// Generates a VFL code for a vex function defined by a vop subnet node.
    // TODO: unify it with outputVflCode(); use temp code gen if needed
    bool		 outputEncapsulatedVflCode(
			    std::ostream &os,
        		    VOP_Node *subnet,
        		    const char *function_name,
        		    VEX_CodeGenFlags flags,
        		    VOP_ContextType context_type,
        		    bool generate_outer_code);

    /// Returns a list of dependencies, if the shader handled by this code
    /// generator is an encapsulated shader (cached code).
    void		 getDependencies(VOP_ContextType context_type,
        		                 UT_StringArray &shader_deps);

    /// On the given micronode, add an extra input to our code micro
    /// node so it gets reset everytime we are dirtied.
    void		 addCodeDependency(DEP_MicroNode *target)
    {
	if (target) target->addExplicitInput(myCodeUpdatedMicroNode);
    }

    /// Return the time stamp from when VEX code was last generated or -1
    /// if no cached code exists
    int			 getCachedCodeTimeStamp(
				VOP_ContextType context_type) const;
    /// Return true if this code generator has any cached code.
    bool		 hasCachedCode() const;

    void		 generateRslObject(UT_String &ofile,
					   const char *cache_directory,
					   const char *renderer,
					   int major_version,
					   int minor_version,
					   const char *shaderpath,
					   const char *shadername,
					   VOP_ContextType context_type,
					   bool ignore_dirty_flag,
					   UT_String *error_return);

    void		 generateOslObject(UT_String &ofile,
					   const char *cache_directory,
					   const char *shaderpath,
					   const char *shadername,
					   VOP_ContextType context_type,
					   bool ignore_dirty_flag,
					   UT_String *error_return);

    // Get a list of infos about the class member methods.
    void		 getClassMethodInfos( 
				    UT_Array< VOP_MemberMethodInfo > &infos );

    // Returns parm nodes that define shader class parameters.
    void		 getClassShaderParameterNodes(VOP_NodeList &parms);

    // Returns owner's output nodes and the shader type they're contributing.
    void		 getOutputNodes(VOP_NodeList &outputs,
				VOP_ShaderTypeList *types = nullptr) const;

    /// Return the collection VOP (or NULL if there is no collection)
    VOP_Collect		*getCollectNode() const;

    /// Return the code generating encapsulated shader for the given context
    /// (or NULL).
    VOP_Node		*getEncapsulatedShader(VOP_ContextType context) const;

    // Expand the compiler & options using local variables.
    const char		*getCompiler(UT_String &compiler, fpreal now,
				     const char *defcompiler);
    const char		*getCompiler(UT_WorkBuffer &wbuf, fpreal now,
				     const char *defcompiler);

    /// @{ Get a string containing the errors or warnings from our last vex 
    /// code generation (including source code generation and compilation).
    /// Returns true if there were any errors.
    bool		 getGeneratorErrors(UT_String &errors) const;
    bool		 getGeneratorErrors(UT_String &errors, 
				       VOP_ContextType context_type) const;
    bool		 getGeneratorErrorsAndWarnings(UT_String &msgs) const;
    bool		 getGeneratorErrorsAndWarnings(UT_String &msgs,
				       VOP_ContextType context_type) const;

    bool		 getDiagnosticsForNode(VOP_Node *node,
        		                       VCC_DiagnosticList &list) const;
    /// @}

    /// Get the VOP language generated by this code generator.
    const VOP_Language	*getLanguage() const
			 { return myLanguage; }

    /// Get the object representing the list of shader contexts (surface,
    /// displacement, etc) supported by this generator. The returned object
    /// also describes the language type supported by this generator.
    const VOP_LanguageContextTypeList *	getLanguageContextTypeList() const
			 { return myContextTypeList; }

    /// Returs a language for which this generator produces code.
    VOP_LanguageType	 getLanguageType() const;


    /// @{ An automatic code generator is created by default for a VOP, 
    /// when a VOP is created in a Shader Network container. 
    /// The automatic generator provides a code for a temporary shader 
    /// consisting of that VOP and any of its input chains.
    void		 setIsAutoGenerator(bool is_auto)
			 { myIsAutoGenerator = is_auto; }
    bool		 isAutoGenerator() const
			 { return myIsAutoGenerator; }
    /// @}
 
    /// Returns true if the generator should generate temporary, automatic,
    /// adhoc shader for the owner node, when that node is assigned as a shader.
    /// Such code is needed for VOPs directly inside a Shader Network container,
    /// when they are not true shaders themselves, or when a connected input
    /// implies a composite shder generation. 
    /// In other cases (eg an unconnected VOP that represents an external 
    /// shader, or a RIS shader), there is no need for temp shader code gen.
    bool		 needsToGenerateAutoShader() const;

    /// Returns true if it is a single-context vopnet that this generator
    /// produces code for. Otherwise (ie, multi-context) returns false.
    bool		 isSingleContextType() const;

    /// Returns true if this generator produces a class-based shader.
    bool		 isClassBasedShader() const;

    /// Returns true if the given shader type is class-based.
    bool		 isClassBasedShader(
			    const VOP_CodeGenContext &codegen_ctx) const;

    /// Returns a list of vopnet type masks for a given context list.
    void		 getVopnetTypes(const VOP_ContextTypeList &ctxts,
        		                UT_StringArray &vopnet_types);

    /// Returns true if the owner of the code generator displays parameters.
    /// This is currently true for VOPNET_Nodes.
    bool		 getOwnerHasParameters() const
				{ return myOwnerHasParameters; }

    /// Determine the VOP context type for single-context type vopnet generator.
    /// For generator of a multi-context vopnet returns
    /// VOP_CONTEXT_TYPE_INVALID, because the specific VOP context cannot be 
    /// reliably and unambiguously determined.
    VOP_ContextType	 getVopContextType() const;

    /// Determine the VEX context type for single-context type vopnet generator.
    /// For generator of a multi-context vopnet (or for RSL vopnet) returns
    /// VEX_INVALID_CONTEXT, because the specific VEX context cannot be 
    /// reliably and unambiguously determined.
    VEX_ContextType	 getVexContextType() const;
    RSL_ContextType	 getRslContextType() const;
    OSL_ContextType	 getOslContextType() const;

    /// Returns the compiler arguments.
    VOP_CodeCompilerArgs*getCompilerArgs()
			 { return myCompilerArgs; }

    /// Set the locked compiled code. When set, the locked code causes the
    /// code generator to ignore the VOP nodes in the owner network and always
    /// return the locked value.
    void		 setLockedCompiledCode(
			    const char *code, VOP_ContextType context_type,
			    bool is_source_code);

    /// Utility function to get a code generator corresponding to the node.
    /// Returns own code generator if it exists, or creator's code generator.
    static VOP_CodeGenerator	*findCodeGenerator( const OP_Node *node );

    // The local variables that must be defined by any code generator node.
    // These are the variables understood by the getVariableString function
    static CH_LocalVariable	 theLocalVariables[];

    // Install commands specific to VOP code generators.
    static void			 installCommands();

    // This function converts a local VOP code generator to a global VOP code
    // generator that defines an operator type.
    static bool			 convertVopOpToVopOpType(OP_Node *node,
							 UT_String &err);

    /// Returns the default VOP mask for VEX.
    static const char		*getDefaultVEXMask();

    // These variables define the parameter descriptions for the VOP
    // compiler parm used to specify how to compile VOP generated code.
    static PRM_Name		 theVopCompilerName;
    static PRM_Default		 theVopCompilerVexDefault;
    static PRM_Default		 theVopCompilerRslDefault;
    static PRM_Default		 theVopCompilerOslDefault;
    static PRM_ChoiceList	 theVopCompilerChoices;
    static PRM_Name		 theVopForceCompileName;

    static bool			 forceCompile(OP_Node *node);
    static int			 forceCompile(void *data, int, fpreal,
					      const PRM_Template *);

    /// Determines the vex context. In single-context vop, returns a vex type
    /// intrinsic to that vop. In multi-context vop, returns the vex type based
    /// on context_type passed as an argument.
    VEX_ContextType	 pickVexContextType(VOP_ContextType context_type) const;
    RSL_ContextType	 pickRslContextType(VOP_ContextType context_type) const;
    OSL_ContextType	 pickOslContextType(VOP_ContextType context_type) const;

    /// Determines the vop context (ie encode vex or rsl context type). 
    /// In single-context vop, returns a vex/rsl context type (encoded as vop
    /// type) intrinsic to that vop. In multi-context vop, returns the vop 
    /// type based passed as an argument.
    VOP_ContextType	 pickContextType(VOP_ContextType context_type) const;

    // Locates a procedural shader by searching the VOP network an output node
    // of type "interpret_type" and tracing back to the first non-simple node.
    VOP_Node		*getProcedural(VOP_Type interpret_type) const;

    /// Obtains the export variables that don't have explicit output connectors.
    void		 getVopFunctionArgInfosFromExports( 
				UT_Array<VOP_FunctionArgInfo> &arg_infos, 
				VOP_Type shader_type );

    /// Gathers all the rendering properties nodes.
    void		 getPropertiesNodes(VOP_NodeList &properties);

    OP_Network *	 ownerNetwork() const
			    { return myOwner; }

    /// Return the micro-node representing the error state. The micro-node will
    /// signal if we transition from clean -> error, error -> error, and
    /// error -> clean.
    VOP_ErrorMicroNode	&errorMicroNode()
			    { return myErrorMicroNode; }

    static void		formatDiagnostic(const VCC_DiagnosticInfo &info,
        		                 UT_WorkBuffer &str);

    /// Get a list of all vop nodes participating in the shader. The list
    /// is sorted according to order in which the vfl code should be generated.
    /// If 'check_errors' is true, node error checking is performed and if
    /// any error is encountered, the method will return false (and the node
    /// list may be incomplete). If this argument is false, the method
    /// does not check errors and always returns true and the list
    /// of nodes is complete.
    /// NB: when nodes a no longer needed, use cleanupNodesForShaderOutput() 
    ///	    to clean up the list of temporary nodes this method may create.
    bool		 getNodesForContextOrNodeFunctionVflCode(
				    VOP_NodeList &nodes, 
				    VOP_Node *&output_node,
				    const VOP_CodeGenContext &codegen_ctx,
				    VOP_CodeVarMapperContext &varmap_context,
				    const char *output_node_name, 
				    bool check_errors,
				    VCC_Diagnostics *diagnostics);

    /// Get a list of all vop nodes participating in the function code
    /// defined by a vop subnet. The list is sorted according to order in
    /// which the vfl code should be generated.
    /// NB: when nodes a no longer needed, use cleanupNodesForShaderOutput() 
    ///	    to clean up the list of temporary nodes this method may create.
    bool		 getNodesForSubnetFunctionVflCode(VOP_NodeList &nodes, 
				    const VOP_CodeGenContext &codegen_ctx,
				    VOP_CodeVarMapperContext &varmap_context,
				    VOP_Node *vop_subnet,
				    bool require_context_check);

    /// Clean up the list obtained with getNodesForShaderVflCode() or
    /// getNodesForSubnetFunctionVflCode()
    void		 cleanupNodesForVflCode(VOP_NodeList &nodes);

    bool		 needsAutoConvertCleanup() const
			    { return myNeedAutoConvertCleanup; }

    /// Controls whether the source code is laced with #ifdef __vex.
    void		 setUseIfdefVex(bool flag)
			    { myUseIfdefVex = flag; }

    /// Controls whether the source code contains pragmas.
    void		 setOmitPragmas(bool flag)
			    { myOmitPragmas = flag; }

    /// Obtains the name of the shading context as is used in the language.
    UT_StringHolder	    getShaderContextName(VOP_ContextType ctx_type)const;
    static UT_StringHolder  getShaderContextName(VOP_ContextType ctx_type,
				VOP_LanguageType lang_type); 

    /// Obtains the name of the shading context, which includes node path 
    /// prefix if it's an auto shader.
    UT_StringHolder	 getShaderContextFullName(VOP_ContextType ctx_t) const;

    /// @{ Methods for marking code path related to adding and deleting
    /// temporary nodes in auto-shader wrappers.
    void		 setIsChangingAutoShader(bool flag)
			 { myIsChangingAutoShader = flag; }
    bool		 getIsChangingAutoShader() const
			 { return myIsChangingAutoShader; }
    /// @}

private:
    /// Helper for compiling code.
    void		 generateObjectCode(VOP_ObjectCodeArgs *args);

    /// Returns true if there is a precompiled VEX set on this generator.
    bool    isCompiled(VOP_ContextType context_type) 
		{ return myLockedCompiledCode[context_type].length() > 0; }


    void		 outputVflHeaderComment(std::ostream &os);
    void		 outputShaderDefines(std::ostream &os,
				VOP_ContextType context_type);
    bool		 outputShaderVflCodeBlock(std::ostream &os,
				const char *shadername,
				VEX_CodeGenFlags flags,
                                const VOP_CodeGenContext &codegen_ctx,
				const char *output_node_name,
				VCC_Diagnostics *diagnostics);
    bool		 outputStandardShaderVflCodeBlock(std::ostream &os,
				const char *shadername,
				VEX_CodeGenFlags flags,
				const VOP_CodeGenContext &codegen_ctx,
				const char *output_node_name,
				VCC_Diagnostics *diagnostics);
    bool		 outputFunctionVflCodeBlock(std::ostream &os,
				const char *function_name,
				VEX_CodeGenFlags flags,
				const VOP_CodeGenContext &codegen_ctx,
				VOP_Node *subnet,
				bool generate_outer_code);
    bool		 outputMethodVflCodeBlock(
				const vop_CodeGenOutputContext &output_ctx,
				bool for_struct,
				VEX_CodeGenFlags flags,
				VOP_Node *method_node,
				VCC_Diagnostics *diagnostics);
    bool		 outputClassVflCodeBlock(std::ostream &os,
				const char *shadername,
				VEX_CodeGenFlags flags,
				const VOP_CodeGenContext &codegen_ctx,
				VCC_Diagnostics *diagnostics);
    void		 outputVflPragmas(std::ostream &os,
				const VOP_NodeList & nodes, 
				const VOP_CodeGenContext &codegen_ctx);
    void		 outputVflFunctionBlock(
				const vop_CodeGenOutputContext &output_ctx,
				const VOP_NodeList & nodes, 
				VOP_Node *output_node,
				VOP_CodeVarMapperContext & root_varmap_context,
				const char *return_type,
				const char *function_name,
				VEX_CodeGenFlags flags,
				const VOP_CodeGenContext &codegen_ctx,
				VOP_Node *function_subnet,
				bool generate_outer_code);
    void		 outputIsConnectedDeclarations(std::ostream &os,
        		        const char *indent,
        		        const UT_StringArray &is_connected_list,
        		        bool needs_default);
    void		 outputShaderVflDeclaration(
				const vop_CodeGenOutputContext &output_ctx,
				const VOP_NodeList & nodes, 
				const char *return_type,
				const char *function_name,
				const VOP_CodeGenContext &codegen_ctx);
    void		 outputClassVflDeclaration(
				const vop_CodeGenOutputContext &output_ctx,
				const char *shadername,
				const VOP_CodeGenContext &codegen_ctx);
    void		 outputStructVflDeclaration(
				const vop_CodeGenOutputContext &output_ctx,
				const char *shadername);
    void		 outputFunctionVflDeclaration(
				const vop_CodeGenOutputContext &output_ctx,
				VOP_Node *func_node,
				const VOP_NodeList & nodes, 
				const char *return_type,
				const char *function_name,
				const VOP_CodeGenContext &codegen_ctx,
				const UT_StringArray &is_connected_list);
    void		 outputVflEmptyShaderFunction(std::ostream &os,
				const char *shadername,
                                const VOP_CodeGenContext &codegen_ctx);
    void		 outputShaderParameterDeclarations(std::ostream &os,
				const VOP_NodeList & nodes, 
				const char *indent,
                                const VOP_CodeGenContext &codegen_ctx);
    void		 outputMemberVariableDeclarations(
				const vop_CodeGenOutputContext &output_ctx);
    void		 outputFunctionParameterDeclarations(std::ostream &os,
				VOP_Node *function_subnet,
				const VOP_NodeList & nodes, 
				const char *indent,
                                const VOP_CodeGenContext &codegen_ctx);

    /// Obtains the final shader or function name.
    void		 getFunctionNameOrOverride(UT_String &function_name,
						const char *override);

    void		 clearFileCache();

    /// Adds prologue and epilogue nodes to the list to complete shader code.
    void		 wrapInAutoShaderIfNeeded( VOP_NodeList &nodes,
				    VOP_Node *output_node,
				    const VOP_CodeGenContext &codegen_ctx );

    /// Checks if there are any siblings subscribing to the owner.
    bool		 hasSubscribingSiblings( VOP_Type shader_type ) const;

    /// Expand subnets and Fetch VOPs in the given list.
    /// Nodes contained in the subnets and nodes pulled in from the
    /// Fetch VOPs are inserted in the list.
    /// @parm only_essential When expanding subnets, this method needs to
    ///		    determine the 'culminating' nodes that calculate final
    ///		    value of output variables. These are subnet output vop,
    ///		    and exporting parameter vops and they are called essential
    ///		    nodes. In the past, other nodes such as inline, print, etc
    ///		    would also be considered as "culminating", but with the
    ///		    advent of multi-contetxt it was nolonger clear which context
    ///		    they would generate final code in. The remaining
    ///		    (non-culminating) vops feed into the culminating ones, thus
    ///		    providing variables necessary to calculate final variables.
    ///		    This parameter serves as a switch between new
    ///		    (multi-context) vops, when 'true', and backwards
    ///		    compatibility (old) vops, when 'false'.
    void		expandSubnetsAndFetchVOPs(VOP_NodeList &nodes,
			    const VOP_CodeGenContext &codegen_ctx,
			    VOP_CodeVarMapperContext &root_context,
			    bool only_essential) const;

    /// Checks if the temporary files have been already created for this
    /// context, and returns them if so. If not, it creates the files, and 
    /// then returns them. The temporary files will store the vfl code that is
    /// passed to vcc compiler and any errors that occured during compilation.
    /// They are means of passing the source code to the compiler.
    void		 ensureTempFilesCreated(VOP_ContextType context_type,
        		                        UT_StringHolder &vfl_file, 
        		                        UT_StringHolder &err_file);
    void		 ensureDstDirCreated(UT_String &output_file_no_ext,
			    const char *path, const char *shaderpath, 
			    const char *shadername, const char *context_suffix);

    void		 compileOutofProcess(VOP_ContextType context_type,
        		                     const char *shadername,
                                             UT_WorkBuffer &code_to_cache);

    bool		 usesStandardVCCOptions();

    // Gets a suffix that disambiguates the files for the same function name 
    // but different context for a multi-context VOP. 
    void		 getContextFileSuffix( UT_String & suffix,
					   VOP_ContextType context_type );

    // Utility functions for code cache manipulation.
    vop_CodeCacheData *	findCachedCode( VOP_ContextType context_type ) const;
    vop_CodeCacheData *	addCachedCode( VOP_ContextType context_type,
				       const char * code,
				       const VCC_DiagnosticList &diagnostics);
    void		deleteCachedCode( VOP_ContextType context_type );
    vop_CodeCacheData *	removeCachedCode( VOP_ContextType context_type );
    void		addCachedCode( VOP_ContextType context_type,
				       vop_CodeCacheData * cache_data );
    void		copyCachedErrorToDiagnostic(
				       const vop_CodeCacheData * cache_data );
    void		clearCachedCode();

    /// Obtains any cached messages.
    bool		getCachedMessages(
			    UT_String &msg, bool errors_only,
			    VOP_ContextType context_type =
				    VOP_CONTEXT_TYPE_INVALID) const;

    /// Helper to test if a node parameter of a given index affects the
    /// generated shader VFL code, by switching between different
    /// Output VOP nodes. 
    bool		isMetaSwitchParm(int parm_index) const;

    /// Returns true if the given VOP context is a mantra shading context.
    bool		isMantraShadingContext(
			    const VOP_CodeGenContext &codegen_ctx) const;
    
    void		 checkForErrorsOnChildren(OP_Context &cxt, 
        		                          OP_Node *node,
        		                          bool force_cook);

    /// Generate VOP errors and propagate them to the owner node.
    ///
    /// This method is intended to generate/collect errors outside of cooking
    /// (i.e. compilation errors) so it recreates the dirty-update-notify
    /// process that is typically done for cooking.
    void		 generateErrorsAndPropagateToOwner();

    /// Keeps code and other data relevant to the code generation and reuse.
    OP_Network			*myOwner;
    VOP_LanguageContextTypeList *myContextTypeList;
    VOP_Language		*myLanguage;
    UT_SharedPtr<VCC_Diagnostics> myDiagnostics;
    UT_UniquePtr<VCC_Compiler>	 myVflCompiler;
    TContextTypeStringMap	 myLockedCompiledCode;
    TContextTypeStringMap	 myLockedSourceCode;
    VOP_GlobalVarMap		 myGlobalVariables;
    VOP_ParmGeneratorMap	 myLocalVariables;
    VOP_LocalChannelMap		 myLocalChannels;
    VOP_CodeOperatorFilter	 myOperatorFilter;
    VOP_CodeCompilerArgs	*myCompilerArgs;
    UT_ErrorManager		 myErrorManager;

    mutable UT_Lock		 myCachedCodeLock;
    UT_ValArray<vop_CodeCacheData*> myCachedCode;
    UT_SymbolMap<UT_StringHolder> myTempVflFiles;
    UT_SymbolMap<UT_StringHolder> myTempErrFiles;
    UT_String			 myHipName;
    UT_String			 myCachedEnglishName;
    int				 myMinimumNumberOfInputs;
    int				 myMaximumNumberOfInputs;

    mutable int			 myCollectNodeId;
    int				 myUpdateLevel;
    bool			 myNeedsCodeUpdate;

    bool			 myOwnerHasParameters;
    bool			 myIsReCookingOwner;
    bool			 myNeedAutoConvertCleanup;

    bool			 myUseIfdefVex;
    bool			 myOmitPragmas;
    bool			 myIsAutoGenerator;
    bool			 myIsChangingAutoShader;

    /// Micro node representing VOP errors.
    VOP_ErrorMicroNode		 myErrorMicroNode;

    /// Tracks if our snippet has changed.
    VOP_SnippetMicroNode	 mySnippetMicroNode;

    /// Micro node which we invoke propagateDirty() on every
    /// time we are invalidated.
    DEP_MicroNode		 myCodeUpdatedMicroNode;

    /// If the owner network defines a VOP type (eg, a vop struct), 
    /// this member will channel such a definition to the VOP type manager.
    VOP_VopTypeDefiner *	myVopTypeDefiner;

    /// Manager of the exported parameters.
    VOP_ExportedParmsManager	*myExportedParmsMgr;
};

/// ===========================================================================
/// This class VOP errors in the code generator and stores them 
/// in the owner node.
/// The class should only be instantiated when the owner node is cooked
/// since its destructor can be expensive (i.e. generateVopErrors() is called).
class VOP_CodeGeneratorErrorThief
{
public:
    VOP_CodeGeneratorErrorThief(OP_Node &node)
	: myNode(node)
    {
    }

    ~VOP_CodeGeneratorErrorThief()
    {
	// Get the code generator.
	VOP_CodeGenerator *code_gen = myNode.getVopCodeGenerator();
	if (!code_gen)
	    return;

	// Generate VOP errors in the code generator.
	code_gen->generateVopErrors();

	// Now steal the errors from the code generator
	// and transfer them to the owner node.
	myNode.stealErrors(code_gen->getErrorManager(),	true);
     }

private:
    OP_Node	&myNode;
};

/// ===========================================================================
/// Class containing info about a scope level at which to generate/compile 
/// the nested shaders. Usually they use global level, implying that they use
/// full node path as shader function name. But when compiling HDAs, the
/// shaders nested inside them are anchored to the HDA and should use
/// relative path as shader function name. 
/// Also, this class can store all the nested shaders that need own compilation.
class VOP_API VOP_ShaderSpaceInfo
{
public:
		VOP_ShaderSpaceInfo( VOP_Node *shader_node, 
			const char *shader_name );

    const UT_Map<VOP_Node*, UT_StringHolder> & getNestedSubnetShaders() const
		{ return myNestedSubnetShaders; }
    void	addSubShader(VOP_Node *sub_shader, const char *name) const
		{ myNestedSubnetShaders[sub_shader] = name; }

    /// Returns true if the node generates code that should be isolated
    /// to the space.
    bool	shouldAddSubShader(VOP_Node *shader_node) const;
    
    /// Replaces the scene-specific prefix with an hda-specific one.
    void	replaceSubShaderNamePrefix( UT_String &sub_shader_name, 
			VOP_Node *sub_shader ) const;

    /// Removes the hda-specific prefix.
    void	removeShaderNamePrefix( UT_String &sub_shader_name ) const;

private:
    // Main parent shader HDA that contains the space for sub-shaders.
    VOP_Node *		myShaderNode;
    UT_StringHolder	myShaderName;

    // A list of nested shader nodes that are subnets and that generate code 
    // from children. They need to be explicitly saved into own sections when 
    // the main (outer parent) shader HDA has "save cached code" turned on.
    // The map is from node to the shader function name it used
    // when generating a shader call in the main shader
    mutable UT_Map<VOP_Node*, UT_StringHolder>	myNestedSubnetShaders;
};

/// ===========================================================================
/// A container for requesting generation of code that computes optional
/// variables. Some nodes downstream may express an interest in a variable
/// value that is not computed by default, and this is a mechanism to pass
/// requests for such values to the nodes upstream (which generate code first).
/// Eg, shader nodes may not include hidden exports in the shader function call,
/// unless explictly requested.
class VOP_API VOP_ExtraVarRequests
{
public:
    /// Registers a request for a given vop to generate code for the
    /// otherwise optional variable var. 
    void	requestVarFrom(VOP_Node &vop, const VOP_FunctionArgInfo &var);

    /// Obtains a list of variables requested from given vop. 
    const UT_Array<VOP_FunctionArgInfo> *getRequestedVars(VOP_Node &vop) const;

private:
    UT_Map< VOP_Node*, UT_Array<VOP_FunctionArgInfo> >	myRequestedVars;
};

/// ===========================================================================
/// Utility class for automatically adding auto-code-generator to a node.
/// Adding generator is a two-step process, requiring calling generator's
/// two methods, one before node is added and one after it is added to parent.
class VOP_API VOP_AutoCodeGeneratorAdder
{
public:
     VOP_AutoCodeGeneratorAdder( VOP_Node *vop );
    ~VOP_AutoCodeGeneratorAdder();

private:
    VOP_Node *	myVop;
};


#endif
