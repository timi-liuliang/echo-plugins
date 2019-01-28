/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_AssetBrowser_h__
#define __HOM_AssetBrowser_h__

#include "HOM_PathBasedPaneTab.h"
#include "HOM_EnumModules.h"

SWIGOUT(%rename(AssetBrowser) HOM_AssetBrowser;)

class HOM_API HOM_AssetBrowser : virtual public HOM_PathBasedPaneTab
{
public:
    HOM_AssetBrowser()
    { HOM_CONSTRUCT_OBJECT(this) }

    // Because the lowermost base classes initialize the virtual bases
    // before any non-virtual bases, the correct thing to do here is
    // explicitly call the constructor for HOM_PaneTab.
    HOM_AssetBrowser(const HOM_AssetBrowser &pane)
    : HOM_PaneTab(pane), HOM_PathBasedPaneTab(pane)
    { HOM_CONSTRUCT_OBJECT(this) }

    virtual ~HOM_AssetBrowser()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0;)

    virtual void reload() = 0;

    virtual void installRequiredDefinitionsForNodeTypeName(
            const char *node_type_name) = 0;

    virtual void storeUpdateSessionKey(
	    const char *session_key) = 0;

    virtual void updateAssetDownloadFraction(
	    const char *node_type_name, int downloaded_bytes, int total_bytes) = 0;

    virtual void storeSyncSessionKey(
	    const char *session_key) = 0;

};

#endif
