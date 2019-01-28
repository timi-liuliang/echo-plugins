/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_viewportVisualizers_h__
#define __HOM_viewportVisualizers_h__

#include "HOM_API.h"
#include "HOM_Errors.h"
#include "HOM_ElemPtr.h"
#include "HOM_EnumModules.h"

#include <string>
#include <vector>

class HOM_GeometryViewport;
class HOM_Node;
class HOM_ViewportVisualizer;
class HOM_ViewportVisualizerType;

SWIGOUT(%rename(viewportVisualizers) HOM_viewportVisualizers;)

class HOM_API HOM_viewportVisualizers
{
public:
    virtual ~HOM_viewportVisualizers()
    {}

    virtual std::string __repr__() = 0;

    SWIGOUT(%kwargs visualizers;)
    virtual std::vector<HOM_ElemPtr<HOM_ViewportVisualizer> > visualizers(
	HOM_EnumValue &category=HOM_viewportVisualizerCategory::Common, 
	HOM_Node *node=NULL) = 0;

    SWIGOUT(%newobject createVisualizer;)
    SWIGOUT(%kwargs createVisualizer;)
    virtual HOM_ViewportVisualizer *createVisualizer(
	HOM_ViewportVisualizerType *type,
	HOM_EnumValue &category=HOM_viewportVisualizerCategory::Common, 
	HOM_Node *node=NULL) = 0;

    SWIGOUT(%newobject copyVisualizer;)
    SWIGOUT(%kwargs copyVisualizer;)
    virtual HOM_ViewportVisualizer *copyVisualizer(
	HOM_ViewportVisualizer *source) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_ViewportVisualizerType> > types() = 0;
    SWIGOUT(%newobject type;)
    virtual HOM_ViewportVisualizerType *type(const char *name) = 0;

    SWIGOUT(%kwargs isCategoryActive;)
    virtual bool isCategoryActive(
	HOM_EnumValue &category, HOM_Node *node=NULL, 
	HOM_GeometryViewport *viewport=NULL) = 0;
    SWIGOUT(%kwargs setIsCategoryActive;)
    virtual void setIsCategoryActive(
	bool on, HOM_EnumValue &category, HOM_Node *node=NULL,
	HOM_GeometryViewport *viewport=NULL) = 0;
};

#endif

