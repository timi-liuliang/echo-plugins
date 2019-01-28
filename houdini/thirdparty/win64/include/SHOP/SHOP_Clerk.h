/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SHOP_Clerk.h ( SHOP Library, C++)
 *
 * COMMENTS:	Defines a mapping from SHOP parameters to a renderer.
 *
 */

#ifndef __SHOP_Clerk__
#define __SHOP_Clerk__

#include "SHOP_API.h"
#include "SHOP_NodeTypes.h"
#include <UT/UT_BoundingBox.h>
#include <UT/UT_String.h>

class UT_Options;
class OP_Context;
class OP_Node;
class VOP_Node;
class SHOP_Node;
class SHOP_ReData;

extern "C" {
    SYS_VISIBILITY_EXPORT extern void	newShopClerk();
}

class SHOP_API SHOP_Clerk {
public:
	     SHOP_Clerk();
    virtual ~SHOP_Clerk();

    /// Function that installs the clerks from DSOs.
    static void		installClerks();

    /// Explicitly registers a new clerk for the given shader type.
    static bool		addShaderClerk(SHOP_TYPE type, SHOP_Clerk *shop);

    /// Get a list of available clerks for the given shader type.  
    /// It is very good practice not to manipulate this list.
    static const UT_ValArray<SHOP_Clerk *>  &getClerkList(SHOP_TYPE type);

    /// @{ Fetching a clerk that matches the given shader and render types.
    static SHOP_Clerk	*getClerk( const UT_Options *options, 
			    const char *default_clerk, const char*rendermask,
			    SHOP_TYPE shader_type, SHOP_TYPE interpret_type,
			    bool only_string_data, bool accept_badmask = false);
    static SHOP_Clerk	*getClerk(const char *rendertype, const char*rendermask,
			    SHOP_TYPE shader_type, SHOP_TYPE interpret_type,
			    bool accept_badmask = false);
    static SHOP_Clerk	*getAnyClerk( const char *rendermask, 
			    SHOP_TYPE shader_type, SHOP_TYPE interpret_type,
			    bool only_string_generators);
    /// @}

    /// Utility function for matching render mask.
    static bool		 renderMatch(const char *rendertype, 
				const char *rendermask, 
				bool accept_badmask=false);

    // Applies a create scripts specified by registered clerks for this
    // render and shader type.
    static void		 runCreateScript(const char *node_path, 
				const char *render_mask, SHOP_TYPE shader_type);

    //
    // This render name should match the name in the IFD class.  That is, this
    // should be the type of renderer passed in for evaluation.
    // The label is a string which can be used to describe the renderer.
    // Typically, this string is used in menus in the UI.  For example:
    //
    //	  getRenderName()	{ return "RIB"; }
    //	  getRenderLabel()	{ return "RenderMan V3.7"; }
    //
    virtual const char		*getRenderName() const = 0;
    virtual const char		*getRenderLabel() const;
    virtual const char		*getDefaultKeywords() const;

    virtual int			 getDialogScriptEnum(SHOP_TYPE type) const;

    virtual bool		 generatesString() const;
    virtual bool		 requireAutoAdd(SHOP_TYPE type) const;

    /// @{
    /// Package the SHOP parameters into shader data.
    /// - @c shop: Build the shader string for this SHOP
    /// - @c data/shader/box: Storage for the result of building the shader
    /// - @c now: The evaluation time
    /// - @c options: An arbitrary dictionary of options
    /// - @c interpret_type:  Interpret the SHOP as this type of shader.  This
    ///      is used for SHOPs which can generate multiple shader types (i.e.
    ///      the material SHOP).
    /// 
    /// The OP_Node* methods are more general and allow wider selection
    /// of shader node representation (ie, VOPs too).
    /// Implementing and overriding them is preferrable to overriding
    /// the older versions that took SHOP_Node*. However, for better
    /// compatibility with older plugin sources, the new methods call 
    ///the old ones in this base class (for now).
    virtual bool	 buildOpShaderData(OP_Node *node,
				    SHOP_ReData &data, fpreal now,
				    const UT_Options *options,
				    SHOP_TYPE interpret_type);
    virtual bool	 buildOpShaderString(OP_Node *node,
				    UT_String &shader, fpreal now,
				    const UT_Options *options,
				    SHOP_TYPE interpret_type);
    virtual bool	 buildOpShaderBounds(OP_Node *node, 
				    UT_BoundingBox &box, fpreal now, 
				    const UT_Options *options,
				    SHOP_TYPE interpret_type);

    //SYS_DEPRECATED_REPLACE(14.5, buildOpShaderData)
    virtual bool	 buildShaderData(SHOP_Node *shop,
				    SHOP_ReData &data,
				    fpreal now,
				    const UT_Options *options,
				    SHOP_TYPE interpret_type);
    //SYS_DEPRECATED_REPLACE(14.5, buildOpShaderString)
    virtual bool	 buildShaderString(SHOP_Node *shop,
				    UT_String &shader,
				    fpreal now,
				    const UT_Options *options,
				    SHOP_TYPE interpret_type);
    //SYS_DEPRECATED_REPLACE(14.5, buildOpShaderBounds)
    virtual bool	 buildShaderBounds(SHOP_Node *shop,
				    UT_BoundingBox &box,
				    fpreal now,
				    const UT_Options *options,
				    SHOP_TYPE interpret_type);
    /// @}

    /// Build a shader string given a VOP node.  The VOP node will @b always be
    /// a code generator.
    virtual bool	 buildVopShaderString(OP_Node *material,
				    VOP_Node *vop,
				    UT_String &shader,
				    fpreal now,
				    const UT_Options *options,
				    SHOP_TYPE interpret_type);


    // When evaluating geometry shaders, the clerk may need to evaluate
    // bounding box information.  Particularly for RIB and VMantra.
    virtual bool	 getBoundingBox(UT_BoundingBox &box,
					 SHOP_Node &node, OP_Context &ctx);

    //
    // When attaching the shader information to a primitive, the shader
    // information must be able to be stored as a string.  In this case, we
    // need to define the attribute name as well as how to build the string.
    //
    // If the attribute cannot be attached to geometry, then the
    // getGeometryAttribute() function should return a null pointer.
    // By default, this is what the getGeometryAttribute() does.
    //
    // The GeometryIndirect returns the name of an indirect attribute which
    // causes the SHOP to be evaluated at render time.
    //
    virtual const char	*getGeometryAttribute(SHOP_TYPE shader_type) const;
    virtual const char	*getGeometryIndirect(SHOP_TYPE shader_type) const;

    // Return true if we are the VEX clerk.  Returns false by default.
    virtual bool	getIsVexClerk() const;

    // A creation script gets run when the SHOP has it's creation script run.
    // This allow the clerk to add spare properties to the SHOP if required.
    virtual const char	*getCreationScript(SHOP_TYPE shader_type) const;

protected:
    virtual void	buildShaderStart(UT_String &str,
					 const char *shader_name,
					 SHOP_Node *node);
    virtual void	buildShaderEnd(UT_String &str,
					 const char *shader_name,
					 SHOP_Node *node);

    // Evaluate the bounding box specified by the path to the OP_Node.
    bool		evalSopBoundingBox(UT_BoundingBox &box,
					   SHOP_Node &node,
					   const char *parmname,
					   fpreal now);
};

#endif
