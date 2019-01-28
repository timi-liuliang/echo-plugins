/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __HUSD_Utils_h__
#define __HUSD_Utils_h__

#include "HUSD_API.h"
#include <UT/UT_StringHolder.h>

class UT_String;
class OP_Node;

enum HUSD_PrimTraversalDemands {
    HUSD_TRAVERSAL_ACTIVE_PRIMS		= 0x00000001,
    HUSD_TRAVERSAL_DEFINED_PRIMS	= 0x00000002,
    HUSD_TRAVERSAL_LOADED_PRIMS		= 0x00000004,
    HUSD_TRAVERSAL_NONABSTRACT_PRIMS	= 0x00000008,

    HUSD_TRAVERSAL_DEFAULT_DEMANDS	= HUSD_TRAVERSAL_ACTIVE_PRIMS |
					  HUSD_TRAVERSAL_DEFINED_PRIMS |
					  HUSD_TRAVERSAL_LOADED_PRIMS |
					  HUSD_TRAVERSAL_NONABSTRACT_PRIMS,
    HUSD_TRAVERSAL_NO_DEMANDS		= 0x00000000
};

// Configures the USD library for use within Houdini. The primary purpose is to
// set the prefered ArResolver to be the Houdini resolver. This should be
// called as soon as possible after loading the HUSD library.
HUSD_API void HUSDinitialize();

// Modifies the passed in string to make sure it conforms to USD primitive
// naming restrictions. Illegal characters are replaced by underscores.
HUSD_API bool HUSDmakeValidUsdName(UT_String &name);

// Returns the name of the node passed through the HUSDmakeValidUsdName method.
// This saves several lines of code every time we use this pattern.
HUSD_API UT_StringHolder HUSDgetValidUsdName(OP_Node &node);

// Modifies the passed in string to make sure it conforms to USD primitive
// naming restrictions. Illegal characters are replaced by underscores. Each
// path component is validated separately.
HUSD_API bool HUSDmakeValidUsdPath(UT_String &path);

// Returns the path of the node passed through the HUSDmakeValidUsdPath method.
// This saves several lines of code every time we use this pattern.
HUSD_API UT_StringHolder HUSDgetValidUsdPath(OP_Node &node);

#endif

