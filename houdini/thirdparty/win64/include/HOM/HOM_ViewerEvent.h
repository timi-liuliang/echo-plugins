/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef HOM_ViewerEvent_h
#define HOM_ViewerEvent_h

#include "HOM_API.h"
#include "HOM_ElemPtr.h"
#include "HOM_UIEvent.h"
#include <UT/UT_Tuple.h>
class HOM_Vector3;

SWIGOUT(%feature("notabstract") HOM_ViewerEvent;)
SWIGOUT(%rename(ViewerEvent) HOM_ViewerEvent;)

class HOM_API HOM_ViewerEvent : virtual public HOM_UIEvent
{
public:
#ifdef SWIG
%extend
{
    HOM_ViewerEvent(HOM_SceneViewer& sceneViewer, int64 val)
    { 
	return HOM().newViewerEvent(sceneViewer, val); 
    }
}
#else
    HOM_ViewerEvent()
    { HOM_CONSTRUCT_OBJECT(this) }
#endif

    virtual ~HOM_ViewerEvent()
    { HOM_DESTRUCT_OBJECT(this) }

    SWIGOUT(virtual std::string __repr__() = 0;)

    virtual UT_Tuple<HOM_ElemPtr<HOM_Vector3>, HOM_ElemPtr<HOM_Vector3> > ray() = 0;    
    virtual UT_Tuple<HOM_ElemPtr<HOM_Vector3>, HOM_ElemPtr<HOM_Vector3>, bool> snappingRay() = 0;
};

#endif // HOM_ViewerEvent_h

