/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_IPRViewer_h__
#define __HOM_IPRViewer_h__

#include "HOM_PaneTab.h"
#include "HOM_EnumModules.h"
#include <vector>
#include <utility>

class HOM_Node;
class HOM_Prim;

SWIGOUT(%rename(IPRViewer) HOM_IPRViewer;)

class HOM_API HOM_IPRViewer : virtual public HOM_PaneTab
{
public:
    HOM_IPRViewer()
    { HOM_CONSTRUCT_OBJECT(this) }

    HOM_IPRViewer(const HOM_IPRViewer &pane)
    : HOM_PaneTab(pane)
    { HOM_CONSTRUCT_OBJECT(this) }

    virtual ~HOM_IPRViewer()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0;)

    virtual bool isPaused() = 0;

    virtual bool isRendering() = 0;

    virtual bool isActive() = 0;

    virtual void killRender() = 0;

    virtual void pauseRender() = 0;

    virtual void resumeRender() = 0;

    virtual void startRender() = 0;

    virtual bool isPreviewOn() = 0;

    virtual void setPreview(bool on) = 0;

    virtual bool isAutoSaveOn() = 0;

    virtual void setAutoSave(bool on) = 0;

    virtual std::string autoSavePath() = 0;

    virtual void setAutoSavePath(const char *path) = 0;

    virtual bool isAutoUpdateOn() = 0;

    virtual void setAutoUpdate(bool on) = 0;

    virtual double autoSaveTime() = 0;

    virtual void setAutoSaveTime(double time) = 0;

    virtual double delay() = 0;

    virtual void setDelay(double time) = 0;

    virtual double updateTime() = 0;

    virtual void setUpdateTime(double time) = 0;

    virtual std::vector<int> lastClickLocation() = 0;

    SWIGOUT(%newobject ropNode;)
    virtual HOM_Node *ropNode() = 0;

    virtual void setRopNode(HOM_Node *rop_node) = 0;

    virtual std::vector<int> imageResolution() = 0;

    virtual std::vector<double> cropRegion() = 0;

    virtual std::vector<std::string> planes() = 0;

    virtual std::string displayedPlane() = 0;

    virtual std::vector<double> pixel(const char *plane_name, int x, int y) = 0;

    virtual std::vector<std::vector<double> > pixels(const char *plane_name) = 0;

    virtual std::string evaluatedStyleSheetJSON(int x, int y) = 0;

    virtual std::vector<std::pair<std::string, int> >
	evaluatedStyleSheetPaths(int  x, int y) = 0;

    virtual bool saveFrame(const char *file_path, int source=0, int xres=-1,
			   int yres=-1, const char *color="C",
			   const char *alpha="C", const char *scope="*",
			   const char *lut="", float gamma=1.0,
			   bool convert=true) = 0;

    SWIGOUT(%newobject objectNode;)
    virtual HOM_Node *objectNode(int x, int y) = 0;

    SWIGOUT(%newobject prim;)
    virtual HOM_Prim *prim(int x, int y) = 0;

    SWIGOUT(%newobject materialNode;)
    virtual HOM_Node *materialNode(int x, int y) = 0;
};

#endif
