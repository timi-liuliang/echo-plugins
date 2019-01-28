/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP_HDACodeCompiler.h ( VOP Library, C++)
 *
 * COMMENTS:	A class that compiles a node into a script-based HDA.
 *		There are two separate and distinct compilation types:
 *		- compiling whole network into a shader (or vex-based op, 
 *		    such as Vop SOP or Vop COP, etc)
 *		- compiling a vop subnet into an encapsulated Vop operator
 *		    that represents a function call inside a shader
 */


#ifndef __VOP_HDACodeCompiler__
#define __VOP_HDACodeCompiler__

#include "VOP_API.h"
#include <UT/UT_Error.h>
#include <UT/UT_SharedPtr.h>
#include <UT/UT_String.h>
#include <UT/UT_StringMap.h>
#include <iosfwd>
#include "VOP_Types.h"

class FS_IndexFile;
class OP_Node;
class OP_OperatorTable;
class VOP_Node;
class VOP_CodeGenerator;
class VCC_Compiler;
class opui_ScriptMaterialCodeMapper;
class OP_OTLLibrary;

// ===========================================================================
/// A class that compiles a node into a script-based HDA.
/// This is a base class that presents two interface functions, 
/// compileVexCodeToSections() and compileNewOperatorType() and the derived
/// class should implement at least one of them.
class VOP_API VOP_HDACodeCompiler
{
public:
			 VOP_HDACodeCompiler();
    virtual		~VOP_HDACodeCompiler();

    /// @{ Main shader name used in compilation.
    void		    setShaderName(const char *shader_name)
				{ myShaderName = shader_name; }
    const UT_StringHolder & getShaderName() const
				{ return myShaderName; }
    /// @}

    /// @{
    /// Returns an error or a warning message from the last compilation.
    UT_ErrorSeverity	getStatus(UT_String &message, UT_String *details = 0);
    /// @}

    /// @{
    /// Configures the flag to force creation of the new operator even
    /// if there is already one by that name.
    void		setForceCompilationOnOpNameConflict( bool flag )
			    { myForceOnOpNameConflict = flag; }
    bool		getForceCompilationOnOpNameConflict()
			    { return myForceOnOpNameConflict; }
    /// @}

    /// Tests if the node can be compiled into vex code sections.
    /// @return		True if node can be compiled; otherwise, false.
    static bool		canCompileNode( OP_Node * srcnode );

    /// Utility function for cleaning up HDA of any compiled sections.
    /// @parm node  The node defined by the HDA saved in file. It is used
    ///		    to determine the section name convention for the given
    ///		    type of HDA (shader vs. vop).
    static void		removeCodeSections( OP_Node *node, FS_IndexFile *file );

    /// Utility function for compiling node to code sections, but also
    /// to update function name section if needed.
    /// Returns true on success; false otherwise (along with error message)
    static bool		updateCodeSections( OP_Node *node, FS_IndexFile *file,
				UT_String *message);

    /// Compiles a given node to the sections of a given HDA (already existing).
    /// This is used for HDAs that have contents network, but also want to store
    /// cached vfl/vex code.
    /// @param	srcnode	The node from which to obtain vex code.
    /// @param	hda	The HDA definition to which save the vfl and vex code.
    virtual bool	compileVexCodeToSections( OP_Node * srcnode,
						  FS_IndexFile * hda,
						  const char *context_mask = 0);

    /// Compiles a given node to a new script-based HDA (ie, the content nodes
    /// are not saved; only the vex code that they generate is saved).
    /// This is used for HDAs that don't have contents network, and only use
    /// the compiled code for cooking.
    /// @param	srcnode	The node from which to create the new HDA.
    /// @param	name	The name of the new operator.
    /// @param	label	The label of the new operator.
    /// @param	path	The file path of the OTL to which the new HDA is saved.
    /// @param	metasrc	The meta source for the OTL containing new HDA.
    /// @param	types	The shader context types to save. If NULL, all
    ///			implemented context types are saved.
    /// @return		True if creation of new HDA succeeded. On failure,
    ///			false is returned and the member error message is set.
    virtual bool	compileNewOperatorType( OP_Node * srcnode,
				    const char *name,
				    const char *label,
				    const char *path,
				    const char *metasrc,
				    const VOP_ContextTypeList * types = NULL );
    
    /// Obtains info about imported shaders that needed to be also compiled
    /// and saved in HDA sections. Eg, imported shaders that are implemented
    /// as nested Material Builders (or HDAs based on them).
    /// The keys are the shader function names and mapped values are 
    /// the section names where they are stored.
    const UT_StringMap<UT_StringHolder> &getFunctionsSectionsMap() const
			{ return myFunctionsSectionsMap; }

protected:
    /// Saves the code to section.
    void		saveVflCodeToSection(FS_IndexFile *hda,
				    OP_Node *shader_node,
				    VOP_CodeGenerator * code_generator,
				    VOP_ContextType context_type,
				    const char * section_name,
				    OP_Node * output_node );

    /// @{
    /// Allow subclasses to set the error and warning messages.
    void 		setStatus(UT_ErrorSeverity sev, const char *msg,
         		          const char *details = 0);
    
    void		clearStatus();
    /// @}
    
    /// Checks if there was an error during compilation up till now.
    bool		isOK() const
			    { return mySeverity < UT_ERROR_ABORT; }

private:
    /// Shader name used for compiled shader.
    UT_StringHolder	myShaderName;

    /// The string containing an error message from the last compile attempt.
    UT_ErrorSeverity	mySeverity;
    UT_String		myStatusMessage;
    UT_String		myStatusDetails;

    /// Continue with creation of the new operator even if there is already 
    /// one by this name.
    bool		myForceOnOpNameConflict;

    /// A map from shader function name to code section name that contains
    /// the shader code.
    UT_StringMap<UT_StringHolder>	myFunctionsSectionsMap;
};

// ===========================================================================
/// A class that compiles the encapsulated function vop code into HDA section.
class VOP_API VOP_VopHDACompiler : public VOP_HDACodeCompiler
{
public:
    /// Constructor.
			VOP_VopHDACompiler();

    /// Compiles a given node to the sections of a given HDA (already existing).
    /// This is used for HDAs that have contents network, but also want to store
    /// cached vfl/vex code.
    /// @param	srcnode	The node from which to obtain vex code.
    /// @param	hda	The HDA definition to which save the vfl and vex code.
    virtual bool	compileVexCodeToSections( OP_Node * srcnode,
						  FS_IndexFile * hda,
						  const char *context_mask);

    /// Utility function for cleaning up HDA of any compiled sections.
    static void		removeVopCodeSections( FS_IndexFile * hda );


protected:
    /// Sets up class members for compilation an encapsulated vop.
    bool		initForVopCompilation( OP_Node * srcnode );

    /// @{ 
    /// Accessors for the member data, available to derived classes.
    FS_IndexFile *	getHDAFile() const
			    { return myHDAFile;	}
    VOP_Node *		getVopNode() const
			    { return myVopNode; }
    VOP_CodeGenerator *	getCodeGenerator() const
			    { return myCodeGenerator; }
    /// @}
  
private:
    // Obtains vfl, compiles it into vex, and adds both to HDA.
    bool		compileVopCodeAndAddToHDA(VOP_ContextType vop_context);

    bool		getVexCode(VOP_ContextType vop_context,
        		           std::ostream &vex_stream);

    bool		getContextsFromMask(const char *context_mask,
					    VOP_ContextTypeList &context_types);

private:
    // Member variables used during compilation
    FS_IndexFile *		myHDAFile;	    // HDA definition file
    VOP_Node *			myVopNode;	    // src node
    VOP_CodeGenerator *		myCodeGenerator;    // code gen to use
};

// ===========================================================================
/// A class that compiles a shader (or vop-based operator like Vop SOP),
/// into a script-based HDA (with or without the contents network).
class VOP_API VOP_ShaderHDACompiler : public VOP_HDACodeCompiler
{
public:
    /// Constructor
			VOP_ShaderHDACompiler();

    /// @{
    /// Configures the file path where PRMan shaders are located.
    void		setRManPath( const char * path )
			    { myRManPath.harden( path ); }
    const UT_String &	getRManPath() const
			    { return myRManPath; }
    /// @}

    /// Utility function for cleaning up HDA of any compiled sections.
    static void		removeShaderCodeSections( FS_IndexFile * file );

protected:
    /// @{ 
    /// Accessors for the member data, available to derived classes.
    OP_Node *		getNode() const
			    { return myNode; }
    VOP_CodeGenerator *	getCodeGenerator() const
			    { return myCodeGenerator; }
    OP_OperatorTable *	getTable() const
			    { return myTable; }
    /// @}

    /// Sets up member variables to be ready for compilation.
    bool		initForShaderCompilation( OP_Node * srcnode );

    /// Obtains the context types for which to compile the code.
    /// The subclass needs to implement this virtual to provide that
    /// information.
    virtual void	getContextTypesForCompilation( 
				    VOP_ContextTypeList & contexts ) const = 0;

    /// Compiles the given context type of the source node to the HDA in OTL.
    /// @param	code_generator	The code generator to use for compilation.
    /// @param	context_type	The context for which to compile the code.
    ///	@param	section_name	The name of the HDA section in which to save
    ///				the compiled code.
    /// @param	output_node	The node to use for generating the code,
    ///				in case there are several output nodes
    ///				for the same context.
    virtual void	doCompile(  FS_IndexFile *hda,
                	            VOP_CodeGenerator * code_generator,
				    VOP_ContextType context_type,
				    const char * section_name,
				    OP_Node * output_node ) = 0;

protected:
    /// Compiles the vex code for the Output VOP nodes. If there are a few
    /// oputput nodes for the same context (connecting to collect vop via
    /// a switch), then the currently selected one is used.
    /// @return	    Returns true if the HDA definition was actually created
    ///		    (it may not be if there are no primary output nodes).
    bool		compilePrimaryOutputNodes(
				    FS_IndexFile *hda,
				    opui_ScriptMaterialCodeMapper & mapper );

    /// Inspects the mapper for vop nodes it has mapped and compiles vex code 
    /// associated with each mapped node. The code is saved to HDA section.
    void		compileMappedNodes( 
				    FS_IndexFile *hda,
				    opui_ScriptMaterialCodeMapper & mapper );

private:
    /// Directory where PRMan shaders are saved
    UT_String		myRManPath;

    // Member variables used during compilation
    OP_Node *			myNode;		    // src node
    VOP_CodeGenerator *		myCodeGenerator;    // code gen to use
    OP_OperatorTable *		myTable;	    // HDA table
};

//============================================================================= 
/// A class that compiles a shader into a new script-based shader HDA (ie, u
/// no contents network) and saves it in OTL.
class VOP_API VOP_ShaderHDACompilerOTL : public VOP_ShaderHDACompiler
{
public:
    /// Constructor
			VOP_ShaderHDACompilerOTL();

    /// Compiles a given node to a new script-based HDA (ie, the content nodes
    /// are not saved; only the vex code that they generate is saved)
    /// This is used for HDAs that don't have contents network, and only use
    /// the compiled code for cooking.
    /// @param	srcnode	The node from which to create the new HDA.
    /// @param	name	The name of the new operator.
    /// @param	label	The label of the new operator.
    /// @param	path	The file path of the OTL to which the new HDA is saved.
    /// @param	metasrc	The meta source for the OTL containing new HDA.
    /// @param	types	The shader context types to save. If NULL, all
    ///			implemented context types are saved.
    /// @return		True if creation of new HDA succeeded. On failure,
    ///			false is returned and the member error message is set.
    virtual bool	compileNewOperatorType( OP_Node * srcnode,
				    const char *name,
				    const char *label,
				    const char *path,
				    const char *metasrc,
				    const VOP_ContextTypeList * types = NULL );

protected:
    /// Obtains the context types for which to compile the code.
    virtual void	getContextTypesForCompilation( 
				VOP_ContextTypeList & contexts ) const;

    /// Compiles the given context type of the source node to the HDA in OTL.
    virtual void	doCompile(  FS_IndexFile *hda,
                	            VOP_CodeGenerator * code_generator,
				    VOP_ContextType context_type,
				    const char * section_name,
				    OP_Node * output_node );

private:
    FS_IndexFile	*createLibrarySection(OP_OTLLibrary &otl,
                	                      UT_String &otl_section);

    /// Compiles new HDA based on the gathered info.
    void		 compileAddToLibraryAndReload();

    /// Compiles the HDA and saves it to the library
    void		 compileAndAddToLibrary();
    void		 addShaderInfoSection(FS_IndexFile &hda);

    /// Returns the render mask string for the HDA.
    const char 		*getRenderMask() const;

private:
    UT_String			 myOpName;	    // HDA name
    UT_String			 myLabel;	    // HDA label
    UT_String			 myPath;	    // OTL file path
    bool			 myIsEmbedded;	    // OTL is embedded library
    UT_String			 myMetaSrc;	    // OTL meta source
    const VOP_ContextTypeList   *myContextTypes;    // requested context types
};

//============================================================================= 
/// A class that compiles a shader and saves it into an existing shader HDA.
class VOP_API VOP_ShaderHDACompilerHDA : public VOP_ShaderHDACompiler
{
public:
    /// Constructor
			VOP_ShaderHDACompilerHDA();

    /// Compiles a given node to the sections of a given HDA (already existing).
    /// This is used for HDAs that have contents network, but also want to store
    /// cached vfl/vex code.
    /// @param	srcnode	The node from which to obtain vex code.
    /// @param	hda	The HDA definition to which save the vfl and vex code.
    virtual bool		compileVexCodeToSections( OP_Node * srcnode,
						   FS_IndexFile * hda,
						   const char *context_mask);

protected:
    /// Obtains the context types for which to compile the code.
    virtual void	getContextTypesForCompilation( 
				VOP_ContextTypeList & contexts ) const;

    /// Compiles the given context type of the source node to the HDA in OTL.
    virtual void	doCompile(  FS_IndexFile *hda,
                	            VOP_CodeGenerator * code_generator,
				    VOP_ContextType context_type,
				    const char * section_name,
				    OP_Node * output_node );

private:
    /// Compiles the vex to HDA sections.
    void		compileAndAddToHDA();

    /// Saves the vop network meta-data to the HDA sectin. 
    void		addInfoToHDA( const char * mat_info );

private:
    FS_IndexFile *		myHDADefFile;	    // HDA definition
};

// ===========================================================================
/// A convenience class that automatically chooses a suitable compiler object
/// given the node type and the kind of HDA (network-based or code-only).
class VOP_API VOP_AutoHDACodeCompiler
{
public:
    /// Constructor that initializes the object to use an instance of 
    /// an HDA code compiler object that is suitable for compiling vfl/vex code.
    /// @parm srcnode	The HDA node whose network is going to be compiled 
    ///			into the vex code.
    /// @parm for_network_hda    If true, the compiler will be able to 
    ///			compileVexCodeToSections(), ie it is suitable for
    ///			network-based HDAs with additional cached code sections.
    ///			Otherwise, it will be able to compileNewOperatorType(),
    ///			ie it is suitable for code-based HDAs (with no network).
			     VOP_AutoHDACodeCompiler( const OP_Node *srcnode,
				     bool for_network_hda,
				     const char *shader_name = nullptr);
			    ~VOP_AutoHDACodeCompiler();

    /// Obtain the underlying code compiler.
    VOP_HDACodeCompiler *   getCC() const
				{ return myCodeCompiler; }

private:
    VOP_HDACodeCompiler *   myCodeCompiler; // instance of HDA code compiler
};

#endif

