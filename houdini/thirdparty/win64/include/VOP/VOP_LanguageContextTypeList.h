/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP_LanguageContextTypeList.h ( VOP Library, C++)
 *
 * COMMENTS:
 *
 */

#ifndef __VOP_LanguageContextTypeList__
#define __VOP_LanguageContextTypeList__

#include "VOP_API.h"
#include <SYS/SYS_Deprecated.h>
#include <UT/UT_StringHolder.h>
#include "VOP_Types.h"
class UT_StringArray;
class OP_Node;

/// A class that abstracts shader types and shader context types.
/// It encapsulates shader types that make up a given vopnet node,
/// and also shader contexts conavailable inside that vopnet.
/// 
/// This class is used to determine variables available in Global VOP 
/// (as a menu of available context types), constituent shaders in 
/// Vars Output VOP (as a menu of available shader type), and final form 
/// of a shader source code n View VEX Code (as a menu of shader types), etc.
///
/// A shader type represents a shader kind. It could be a surface, 
/// a displacement, but also a geometry or a class. It corresponds to VOP_Type.
///
/// A context type represents a language support for a given shader type. 
/// Context determines global variables and functions available to implement 
/// a given shader type in a given language. 
/// But not all shader types have corresponding contexts, if they can't be 
/// implemented in a given language, for example, like geometry shader type.
/// Context type is represented by VOP_ContextType.
class VOP_API VOP_LanguageContextTypeList
{
public:
    /// @{ Default c-tor de-tor.
			    VOP_LanguageContextTypeList();
			   ~VOP_LanguageContextTypeList();
    /// @}

    /// Constructor for representing a shader type in a given language.
    /// Some shader types (eg, a material or a class) may represent 
    /// a structured shader that consists of several sub-shader types.
    /// In such cases the list of sub-shaders can be provided explicitly.
    /// 
    /// @param language_type	Shader's language type. 
    ///		It determines the shader context types that are available, 
    ///		which in turn determine global variables and functions 
    ///		available to the shader. It is needed if (some) shader types
    ///		are implemented thru VOP networks. 
    ///
    /// @param shader_type	The shader type. 
    ///		For a single-context node, it represents its shader type.
    ///		For multi-context nodes, it represents the master shader type
    ///		(eg, material or a class).
    ///
    /// @param sub_types	For a multi-context node, this parameter
    ///		explicitly lists the constituent shader types that make up
    ///		the master shader. Eg, a material may allow surf and disp.
    ///
			    VOP_LanguageContextTypeList( 
				    VOP_LanguageType language_type,
				    VOP_Type shader_type,
				    const VOP_ShaderTypeList *sub_types = NULL);

    /// Deprecated. Uses old VOP_ContextType rather than VOP_Type shader type.
    /// Eg, use VOP_CVEX_SHADER instead of VEX_CVEX_CONTEXT.
			   SYS_DEPRECATED_HDK(16.0)
			   VOP_LanguageContextTypeList( 
				   VOP_LanguageType language_type,
				   VOP_ContextType  context_type);


    /// Sets the object data to reflect the node's shader types and contexts.
    void		setFromNode( OP_Node *node );

    /// Obtains the default context.
    VOP_ContextType	getDefaultContextType() const;

    /// Oringial overarching shader type (eg, class, material, surface).
    VOP_Type		getMasterShaderType() const
				{ return myMasterType; }

    /// Returns the type of the language in which the context types are valid.
    VOP_LanguageType	getLanguageType() const
				{ return myLanguageType; }

    /// Returns true if the list represetns a single-context list.
    bool	isSingleContextType() const
				{ return myShaderTypes.entries() == 1; }

    /// Returns true if the main shader is a class type.
    bool	isClassBasedShader() const;

    /// Returns a list of shader types that make up the master shader.
    void	getConstituentShaderTypes( VOP_ShaderTypeList &types ) const;

    /// Returns a list of shader context types which are valid in the list's
    /// language and for which there is a source code that can be generated.
    /// Some constituent shader types may not have a corresponding a shader 
    /// context (eg geometry shader type).
    void	getConstituentShaderContexts( VOP_ContextTypeList &ctxs ) const;

    /// Returns a list of shaders whose parameters can appear on the
    /// master shader (material) node. Such parameters are tagged 
    /// for shader context so they can be used in renderer's shader string.
    void	getParmTagShaderTypes( VOP_ContextTypeList &contexts ) const;

    /// Returns a list of shaders contexts (and labels) for which the master
    /// shader node provides source code. These usually correspond
    /// to language contexts (surface, displacement), but this can also be just
    /// a class. However, procedurals (geometry) usually is not in this list.
    void	getSourceCodeShaderTypes( UT_StringArray &labels,
			VOP_ContextTypeList &contexts ) const;

private:
    /// Convenience helper to obtain a context type.
    VOP_ContextType	asContextType( VOP_Type type ) const
			{ return VOPconvertToContextType(type, myLanguageType);}

private:
    /// The language type in which the context types should be interpreted.
    VOP_LanguageType		myLanguageType;

    /// The original shader type consisting of shading context types.
    VOP_Type			myMasterType; 

    /// The list of constituent shaders types. Some types (and in most
    /// cases all types) map to shader contexts valid in the language.
    /// These are the contexts for which a vopnet can generate source code for 
    /// a well-defined shader function. Other types, eg geometry, don't have 
    /// a corresponding context, but are part of the master shader nonetheless.
    VOP_ShaderTypeList		myShaderTypes;
};

#endif

