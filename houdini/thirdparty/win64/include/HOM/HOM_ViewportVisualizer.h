/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_ViewportVisualizer_h__
#define __HOM_ViewportVisualizer_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"

#include <string>
#include <vector>

class HOM_GeometryViewport;
class HOM_Ramp;
class HOM_ViewportVisualizer;
class HOM_ViewportVisualizerType;

SWIGOUT(%rename(ViewportVisualizer) HOM_ViewportVisualizer;)

class HOM_API HOM_ViewportVisualizer
{
public:
    HOM_ViewportVisualizer()
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_ViewportVisualizer()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual bool operator==(HOM_PtrOrNull<HOM_ViewportVisualizer> visualizer) = 0;
    virtual bool operator!=(HOM_PtrOrNull<HOM_ViewportVisualizer> visualizer) = 0;

    virtual std::string __repr__() = 0;

    virtual std::string name() = 0;
    virtual void setName(const char *name) = 0;

    virtual std::string label() = 0;
    virtual void setLabel(const char *label) = 0;

    virtual std::string icon() = 0;
    virtual void setIcon(const char *icon) = 0;


    // We set the %newobject directive for isActive() and setIsActive()
    // because they internally call categoryNode().
    SWIGOUT(%kwargs isActive;)
    virtual bool isActive(HOM_GeometryViewport *viewport=NULL) = 0;
    SWIGOUT(%kwargs setIsActive;)
    virtual void setIsActive(bool on, HOM_GeometryViewport *viewport=NULL) = 0;

    SWIGOUT(%newobject type;)
    virtual HOM_ViewportVisualizerType *type() = 0;
    virtual void setType(HOM_ViewportVisualizerType *type) = 0;

    virtual HOM_EnumValue &category() = 0;
    SWIGOUT(%newobject categoryNode;)
    virtual HOM_Node *categoryNode() = 0;

    virtual HOM_EnumValue &scope() = 0;
    virtual void setScope(HOM_EnumValue &scope) = 0;

    virtual bool isShownInToolbar() = 0;
    virtual void showInToolbar(bool on) = 0;

    virtual bool isActiveWhenUnselected() = 0;
    virtual void setIsActiveWhenUnselected(bool on) = 0;
    virtual bool isActiveWhenSelected() = 0;
    virtual void setIsActiveWhenSelected(bool on) = 0;
    virtual bool isActiveWhenGhosted() = 0;
    virtual void setIsActiveWhenGhosted(bool on) = 0;
    virtual bool isActiveWhenDisplayed() = 0;
    virtual void setIsActiveWhenDisplayed(bool on) = 0;
    virtual bool isActiveWhenCurrent() = 0;
    virtual void setIsActiveWhenCurrent(bool on) = 0;
    virtual bool isActiveWhenTemplated() = 0;
    virtual void setIsActiveWhenTemplated(bool on) = 0;

    virtual std::vector<std::string> parmNames() = 0;
    virtual double evalParmAsFloat(const char *parm_name) = 0;
    virtual int evalParmAsInt(const char *parm_name) = 0;
    virtual std::string evalParmAsString(const char *parm_name) = 0;
    SWIGOUT(%newobject evalParmAsRamp;)
    virtual HOM_Ramp *evalParmAsRamp(const char *parm_name) = 0;
    
#ifdef SWIG
%extend
{
    InterpreterObject evalParm(const char *parm_name)
    {
	return HOMevalViewportVisualizerParm(*self, parm_name);
    }
}
#endif

    virtual void setParm(const char *parm_name, double float_value) = 0;
    virtual void setParm(const char *parm_name, int int_value) = 0;
    virtual void setParm(const char *parm_name, const char *string_value) = 0;
    virtual void setParm(const char *parm_name, HOM_Ramp *ramp_value) = 0;

    SWIGOUT(%ignore parmDataTypeEnumId;)
    virtual int parmDataTypeEnumId(const char *parm_name) = 0;

    virtual void destroy() = 0;

};

#endif

