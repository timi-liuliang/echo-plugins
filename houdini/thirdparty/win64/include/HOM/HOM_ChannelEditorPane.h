/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_ChannelEditorPane_h__
#define __HOM_ChannelEditorPane_h__

#include "HOM_API.h"
#include "HOM_BaseKeyframe.h"
#include "HOM_ElemPtr.h"
#include "HOM_PaneTab.h"

#include <vector>


class HOM_ChannelGraph;


SWIGOUT(%rename(ChannelEditorPane) HOM_ChannelEditorPane;)

class HOM_API HOM_ChannelEditorPane : virtual public HOM_PaneTab
{
public:
    HOM_ChannelEditorPane()
    { HOM_CONSTRUCT_OBJECT(this) }
    // Because the lowermost base classes initialize the virtual bases
    // before any non-virtual bases, the correct thing to do here is
    // explicitly call the constructor for HOM_PaneTab.
    HOM_ChannelEditorPane(const HOM_ChannelEditorPane &pane)
    : HOM_PaneTab(pane)
    { HOM_CONSTRUCT_OBJECT(this) }

    virtual ~HOM_ChannelEditorPane()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0;)

    SWIGOUT(%newobject graph;)
    virtual HOM_ChannelGraph *graph() = 0;

    virtual double channelListSplitFraction() = 0;
    virtual void setChannelListSplitFraction(double split_fraction) = 0;

    virtual std::string displayFilter() = 0;
    virtual void setDisplayFilter(const std::string &filter) = 0;

    virtual HOM_EnumValue &editorMode() = 0;
    virtual void setEditorMode(HOM_EnumValue &mode) = 0;

    virtual std::string templateFilter() = 0;
    virtual void setTemplateFilter(const std::string &filter) = 0;

    virtual std::string colorsCallback() = 0;
    virtual bool setColorsCallback(const std::string &cb) = 0;

    SWIGOUT(%ignore registerColorsOpaqueCallback;)
    virtual bool registerColorsOpaqueCallback(const std::string &cb_class, void* ptr ) = 0;
    virtual bool unregisterColorsCallback(const std::string &cb_class ) = 0;
    virtual std::vector< std::string > colorsCallbacks() = 0;

#ifdef SWIG
%extend
{
    virtual bool registerColorsCallback(const std::string &cb_class, InterpreterObject callback)
    { return self->registerColorsOpaqueCallback(cb_class, callback); }
}
#endif
};

#endif // __HOM_ChannelEditorPane_h__
