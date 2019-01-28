/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SHOP_VopShaderAdapter.h ( SHOP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SHOP_VopShaderAdapter__
#define __SHOP_VopShaderAdapter__

#include "SHOP_API.h"
#include "SHOP_NodeTypes.h"
#include <OP/OP_DataTypes.h>
#include <UT/UT_BoundingBox.h>
#include <SYS/SYS_Types.h>


class SHOP_ReData;
class VOP_Node;
class OP_Node;
class UT_String;
class UT_StringArray;
class UT_Options;


/// A layer over VOP shaders that allows tapping into the SHOP clerks for 
/// shader parameter evaluation. This allows treating VOP shaders the same way
/// as SHOP node shaders, with respect to shader aspect evaluation calls.
class SHOP_API SHOP_VopShaderAdapter
{
public:
		SHOP_VopShaderAdapter( VOP_Node *vop );

    /// Registers some callback for use of VOP lib.
    static  void installCallbacks();

    /// Returns a shader node of a given type.
    // The options may contain:
    //	rendertype:  The the shader for a specific renderer
    OP_Node	*findShader(SHOP_TYPE shader_type, fpreal now,
			const UT_Options *options);

    /// Returns the actual shader type reported by the underlying VOP.
    SHOP_TYPE	 getShaderType();

    /// Builds shader call representation as a string 
    /// (ie, shader "command" name followed by the argument name/value pairs).
    bool buildShaderString(UT_String &result, fpreal now,
		const UT_Options *options, OP_Node *obj=0, OP_Node *sop=0,
		SHOP_TYPE interpret_type = SHOP_INVALID);

    /// Returns the shader handle by which the shader instance is known
    /// to the renderer or to other shaders wanting to reference this one.
    bool buildShaderHandle(UT_String &result, fpreal now,
		const UT_Options *options, OP_Node *obj=0, OP_Node *sop=0,
		SHOP_TYPE interpret_type = SHOP_INVALID);

    /// Returns the name of a language in which the shader is written.
    bool buildShaderLanguage(UT_String &result, fpreal now,
		const UT_Options *options, OP_Node *obj=0, OP_Node *sop=0,
		SHOP_TYPE interpret_type = SHOP_INVALID);

    /// Returns the shader data for the viewport rendering.
    bool buildShaderData(SHOP_ReData &data, fpreal now,
		const UT_Options *options, OP_Node *obj=0, OP_Node *sop=0,
		SHOP_TYPE interpret_type = SHOP_INVALID);

    /// Builds the shader bounds for the displacement shader.
    bool buildShaderBounds(UT_BoundingBox &box, fpreal now,
		const UT_Options *options, OP_Node *obj=0, OP_Node *sop=0,
		SHOP_TYPE interpret_type = SHOP_INVALID);

    /// Returns the list of co-shaders used by this shader.
    bool buildCoShaderStrings(UT_StringArray &result, 
		OP_NodeList &coshaders, fpreal now, 
		const UT_Options *options, OP_Node *obj=0, OP_Node *sop=0,
		SHOP_TYPE interpret_type = SHOP_INVALID);

    /// Returns the list of co-shader handles used to reference the co-shaders
    /// used by this shader.
    bool buildCoShaderHandles(UT_StringArray &result, fpreal now,
		const UT_Options *options, OP_Node *obj=0, OP_Node *sop=0,
		SHOP_TYPE interpret_type = SHOP_INVALID);

    /// Returns the list of language names in which co-shaders are written.
    bool buildCoShaderLanguages(UT_StringArray &result, fpreal now,
		const UT_Options *options, OP_Node *obj=0, OP_Node *sop=0,
		SHOP_TYPE interpret_type = SHOP_INVALID);

    /// Returns shader string for procedural node 'proc'.
    bool buildVopShaderString(VOP_Node *proc, UT_String &result, fpreal now,
		const UT_Options *options, OP_Node *obj, OP_Node *sop,
		SHOP_TYPE interpret_type = SHOP_INVALID);

    /// Returns ture if the given parameter has a corresponding input wired in.
    bool hasCoShaderInputNode(const char* parm_name);

private:
    /// Gets the input node corresponding to the given parameter.
    /// When shader input is wired, it means that output parameter of another
    /// shader (co-shader) is used as input value for this shader.
    VOP_Node *		 getCoShaderInputNode(const char* parm_name);

private:
    /// An underlying VOP shader node.
    VOP_Node *	myShaderVop;
};

#endif

