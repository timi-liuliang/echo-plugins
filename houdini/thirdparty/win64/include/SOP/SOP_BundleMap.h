/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP_BundleMap.h ( SOP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SOP_BundleMap__
#define __SOP_BundleMap__

#include "SOP_API.h"
#include <GSTY/GSTY_BundleMap.h>

class SOP_API SOP_BundleMap : public GSTY_BundleMap
{
public:
			 SOP_BundleMap();
    virtual		~SOP_BundleMap();

    /// Looks at the actual OP_Bundles in the scene, finds all bundles that
    /// the supplied node belongs to, and adds that node to our internal
    /// representation of those bundles.
    virtual void	 add(OP_Node *node);
};

#endif

