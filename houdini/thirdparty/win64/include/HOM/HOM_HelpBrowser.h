/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_HelpBrowser_h__
#define __HOM_HelpBrowser_h__

#include "HOM_PaneTab.h"

SWIGOUT(%rename(HelpBrowser) HOM_HelpBrowser;)

class HOM_API HOM_HelpBrowser : virtual public HOM_PaneTab
{
public:
    HOM_HelpBrowser()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_HelpBrowser(const HOM_HelpBrowser &pane)
    : HOM_PaneTab(pane)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_HelpBrowser()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0;)

    virtual void displayHelp(HOM_NodeType &node_type) = 0;
    virtual void displayHelpPath(const char *help_path) = 0;
    virtual void displayHelpPyPanel(const char *interface_name) = 0;

    virtual std::string homePage() = 0;
    virtual void setHomePage(const char *home_page) = 0;

    virtual std::string url() = 0;
    virtual void setUrl(const char *url) = 0;

    virtual void showUI(bool show) = 0;
};

#endif
