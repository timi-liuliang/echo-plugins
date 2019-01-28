/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GSTY_BundleMap.h ( GSTY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GSTY_BundleMap__
#define __GSTY_BundleMap__

#include "GSTY_API.h"
#include <UT/UT_StringMap.h>
#include <UT/UT_StringSet.h>

class OP_Node;

class GSTY_API GSTY_BundleMap
{
public:
			 GSTY_BundleMap();
    virtual		~GSTY_BundleMap();

    /// Adds an array of names to the supplied bundle name.
    void		 add(const UT_StringHolder &bundle_name,
				const char * const *names,
				int num_names);
    /// This function is not implemented at this level. If you want to make
    /// use of real OP_Bundles, use the SOP_BundleMap subclass instead of
    /// a GSTY_BundleMap.
    virtual void	 add(OP_Node *node);

    /// Removes a bundle from the map.
    void		 remove(const UT_StringHolder &bundle);

    /// Checks if the specified bundle contains the specified name.
    bool		 hasName(const UT_StringRef &bundle_name,
				const UT_StringRef &name) const;

protected:
    UT_StringMap<UT_StringSet>	 myBundles;
};

#endif

