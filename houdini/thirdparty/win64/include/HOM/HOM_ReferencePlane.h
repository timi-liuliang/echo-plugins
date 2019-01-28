/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_ReferencePlane_h__
#define __HOM_ReferencePlane_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"
#include <vector>

class HOM_SceneViewer;
class HOM_Matrix4;

SWIGOUT(%rename(ReferencePlane) HOM_ReferencePlane;)

class HOM_API HOM_ReferencePlane
{
public:
    HOM_ReferencePlane()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_ReferencePlane(const HOM_ReferencePlane &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_ReferencePlane()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;

    SWIGOUT(%newobject sceneViewer;)
    virtual HOM_SceneViewer *sceneViewer() = 0;

    virtual bool isVisible() = 0;

    virtual void setIsVisible(bool on) = 0;

    SWIGOUT(%newobject transform;)
    virtual HOM_Matrix4 *transform() = 0;

    virtual void setTransform(HOM_Matrix4 &matrix) = 0;

    virtual std::vector<double> cellSize() = 0;

    virtual void setCellSize(const std::vector<double> &size) = 0;

    virtual std::vector<int> numberOfCellsPerRulerLine() = 0;

    virtual void setNumberOfCellsPerRulerLine(const std::vector<int> &number) = 0;
};

#endif
