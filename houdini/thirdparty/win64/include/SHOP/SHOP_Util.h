/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SHOP_Util.h ( SHOP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SHOP_Util__
#define __SHOP_Util__

#include "SHOP_API.h"
#include "SHOP_NodeTypes.h"
#include <SYS/SYS_Types.h>

class UT_Options;
class UT_PropertyTable;
class UT_StringHolder;
class OP_Node;
class PRM_Parm;

/// Contains shader utility functions.
class SHOP_API SHOP_Util
{
public:
    /// @{
    /// Get/set property table for defaults.  
    /// If a parameter exists in the property table, then that default value is
    /// used rather than the parameter's default value.
    static const UT_PropertyTable *getPropertyDefaults();
    static void		 setPropertyDefaults(const UT_PropertyTable *table);
    /// @}

    /// Returns shader type name token for the given shader type.
    static const char	*getShaderType(SHOP_TYPE type);

    /// Returns shader label for the given shader type.
    static const char	*getShaderLabel(SHOP_TYPE type);

    /// Return the icon name for the given shader type.
    static const char	*getIconName(SHOP_TYPE type);

    /// Return the name of internal bundle filter for shader of a given type.
    static const char	*getBundleFilter(SHOP_TYPE type);

    /// Return the parameter name that specifies the shader of a given type
    /// (i.e. "shop_materialpath")
    static const UT_StringHolder &getShaderParmPath(SHOP_TYPE type);

    /// Return the parameter name that specifies the space for the shader.
    static const char	*getShaderSpaceParm(SHOP_TYPE type);

    /// Return the shader type given the shader type name.
    static SHOP_TYPE	 getShaderType(const char *name);

    /// Return the shader type given the shader parameter name.
    static SHOP_TYPE	 getTypeFromParm(const char *parmname);

    /// Return the shader type from the space parameter name.
    static SHOP_TYPE	 getTypeFromSpaceParm(const char *parmname);

    /// Return true if the given integer representation of SHOP_TYPE is valid.
    static bool		 isValidShaderType(int i);

    /// Convert a string like "{'foo':'bar','fum':(1,2,3)}' to a UT_Options
    static bool		 convertStringToOptions(UT_Options &options,
						const char *str);

    /// @{ Returns true if the paramter is at a default value.
    /// If check_input is true, will also return true if parameter has
    /// a corresponding input that is wired in.
    static bool		 isParmDefaultValue(OP_Node *node, PRM_Parm *parm,
				fpreal now, const UT_Options *options,
				bool check_input = false);
    static bool		 isParmDefaultValue(OP_Node *node, const char *parmname,
				fpreal now, const UT_Options *options,
				bool check_input = false);
    /// @}

    /// Returns true if the parmeter of the node has a RiType tag.
    static bool          hasParmRiType(OP_Node *node, const char *parmname);

    /// Returns ture if the parm has a corresponding input on the node
    /// and that input is wired in.
    static bool		 hasConnectedInput(OP_Node *node, PRM_Parm *parm);

    /// Returns true if the parameter corresponds to a co-shader parameter.
    static bool		 isCoShaderParm(const PRM_Parm *parm );

    /// Returns true if the parameter corresponds to a coshader array parameter.
    static bool		 isCoShaderArray( const PRM_Parm *parm );
};

#endif

