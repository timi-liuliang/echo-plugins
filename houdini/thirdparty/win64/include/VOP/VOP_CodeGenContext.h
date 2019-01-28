/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP_CodeGenContext.h ( VEX Library, C++ )
 *
 * COMMENTS:	A container class for useful data to be passed down to the
 * 		VOP_Node::getCode and getOuterCode functions.
 */

#ifndef __VOP_CodeGenContext__
#define __VOP_CodeGenContext__

#include "VOP_API.h"
#include <stdlib.h>
#include <UT/UT_StringHolder.h>
#include <VEX/VEX_VexTypes.h>
#include "VOP_Types.h"

class VOP_CodeVarMapperContext;
class VOP_ShaderSpaceInfo;
class VOP_ExtraVarRequests;


class VOP_API VOP_CodeGenContext
{
public:
    VOP_CodeGenContext(VOP_ContextType context_type = VOP_CONTEXT_TYPE_INVALID,
		       const char *context_name = NULL,
                       VEX_CodeGenFlags cg_flags = VEX_CG_DEFAULT,
                       VOP_CodeVarMapperContext *var_mapper_ctx = NULL,
		       bool should_output_parm_defaults = true) :
	myShaderContextType(context_type),
	myShaderContextName(context_name),
	myCodeGenFlags(cg_flags),
	myVarMapperCtx(var_mapper_ctx),
	myShouldOutputParmDefaults(should_output_parm_defaults),
	myForceParmExport(false),
	mySetParentShaderGlobals(false),
	myShaderSpace(nullptr),
	myExtraVarRequests(nullptr)
    {}

    VOP_ContextType		shaderContextType() const
				    { return myShaderContextType; }
    const UT_StringHolder &	shaderContextName() const	    
				    { return myShaderContextName; }
    VEX_CodeGenFlags		codeGenFlags() const
				    { return myCodeGenFlags; }
    VOP_CodeVarMapperContext *	varMapperCtx() const
				    { return myVarMapperCtx; }
    bool			shouldOutputParmDefaults() const
				    { return myShouldOutputParmDefaults; }

    void			setForceParmExport(bool force) 
				    { myForceParmExport = force; }
    bool			isForceParmExport() const
				    { return myForceParmExport; }

    void			setSetParentShaderGlobals(bool set) 
				    { mySetParentShaderGlobals = set; }
    bool			shouldSetParentShaderGlobals() const
				    { return mySetParentShaderGlobals; }

    void			setShaderSpace(const VOP_ShaderSpaceInfo *space)
				    { myShaderSpace = space; }
    const VOP_ShaderSpaceInfo *	getShaderSpace() const
				    { return myShaderSpace; }

    void			setExtraVarRequests(
					const VOP_ExtraVarRequests *requests)
				    { myExtraVarRequests = requests; }
    const VOP_ExtraVarRequests *getExtraVarRequests() const
				    { return myExtraVarRequests; }

    /// @{ Utility functions for manipulating shader context name that 
    /// involves shader node path along with the traditional context name.
    static bool			isOpPrefixContext( const char *opprefixpath );
    static bool			isRelativeOpPrefixContext( 
					const char *opprefixpath );
    static UT_StringHolder 	buildOpPrefixContext( const char *node_path,
					const char *context_name );
    static void			parseOpPrefixContext( const char *opprefixpath,
					UT_StringHolder &node_path, 
					UT_StringHolder &context_name );
    /// @}
    
    /// Returns true if the context accepts any of the given names.
    bool	matchesAnyShaderContextName( 
			const UT_StringArray &context_names) const;

private:
    // Shading context type.
    VOP_ContextType		 myShaderContextType;

    // Snading contesxt name. Usually corresponds to context type above,
    // but for some shaders (like "class") it will contain a method name, etc.
    UT_StringHolder		 myShaderContextName;

    // Flags for code generation.
    VEX_CodeGenFlags 		 myCodeGenFlags;

    // Variable mapper (and variable renaming) cotnext.
    VOP_CodeVarMapperContext 	*myVarMapperCtx;

    // Info about space in which the shader calls occur.
    const VOP_ShaderSpaceInfo	*myShaderSpace; 

    // Requests for var that are optional and not computed by default.
    const VOP_ExtraVarRequests	*myExtraVarRequests;

    // If true, the parm declaration should include initialization to defaults.
    bool			 myShouldOutputParmDefaults;

    // If true, the parm declaration should use export keyword.
    bool			 myForceParmExport;

    // If true, the shader node should generate code to set caller's globals
    // after it generates the shader call code.
    bool			 mySetParentShaderGlobals;
};


#endif // __VOP_CodeGenContext__
