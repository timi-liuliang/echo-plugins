/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_GeometryViewport_h__
#define __HOM_GeometryViewport_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"
#include "HOM_ObjNode.h"
#include "HOM_EnumValue.h"

class HOM_GeometryViewportCamera;
class HOM_GeometryViewportSettings;
class HOM_Vector2;

SWIGOUT(%rename(GeometryViewport) HOM_GeometryViewport;)

class HOM_API HOM_GeometryViewport
{
public:
    HOM_GeometryViewport()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_GeometryViewport(const HOM_GeometryViewport &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_GeometryViewport()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;

    virtual HOM_EnumValue& type() = 0;
    virtual void changeType(HOM_EnumValue &type) = 0;

    SWIGOUT(%newobject settings;)
    virtual HOM_GeometryViewportSettings *settings() = 0;

    virtual std::string name() = 0;
    virtual void changeName(std::string &name) = 0;

    virtual void home() = 0;

    virtual void homeAll() = 0;
    virtual void homeSelected() = 0;
    virtual void homeGrid() = 0;
    virtual void homeNonTemplated() = 0;

    virtual void frameAll() = 0;
    virtual void frameBoundingBox(HOM_BoundingBox *bbox = NULL) = 0;
    virtual void frameSelected() = 0;
    virtual void frameGrid() = 0;
    virtual void frameNonTemplated() = 0;

    virtual void draw() = 0;

    SWIGOUT(%newobject viewTransform;)
    virtual HOM_Matrix4 *viewTransform() = 0;

    SWIGOUT(%newobject viewPivot;)
    virtual HOM_Vector3 *viewPivot() = 0;

    SWIGOUT(%newobject camera;)
    virtual HOM_ObjNode *camera() = 0;
    virtual void setCamera(HOM_ObjNode &camera_node) = 0;
    virtual void setCamera(const char *camera_path) = 0;
    virtual void saveViewToCamera(HOM_ObjNode &camera_node) = 0;

    SWIGOUT(%newobject defaultCamera;)
    virtual HOM_GeometryViewportCamera *defaultCamera() =0;
    virtual void setDefaultCamera(HOM_GeometryViewportCamera *cam_settings) =0;
    virtual void useDefaultCamera() =0;

    SWIGOUT(%newobject queryNodeAtPixel;)
    virtual HOM_Node	*queryNodeAtPixel(int x, int y) = 0;
    SWIGOUT(%newobject queryPrimAtPixel;)
    virtual HOM_Prim	*queryPrimAtPixel(HOM_Node *node, int x, int y) = 0;

    SWIGOUT(%newobject queryInspectedGeometry;)
    virtual HOM_Geometry	*queryInspectedGeometry() = 0;
    SWIGOUT(%newobject queryInspectedPrim;)
    virtual HOM_Prim		*queryInspectedPrim() = 0;

    virtual std::pair<HOM_ElemPtr<HOM_Vector3>, HOM_ElemPtr<HOM_Vector3> >
	mapToWorld(double x, double y) = 0;

    virtual HOM_Vector2 *mapToScreen(const HOM_Vector3 &pos) = 0;

    virtual std::pair<int, int> mapFromMouseChop(double x, double y) = 0;

    SWIGOUT(%ignore addOpaqueEventCallback;)
    virtual void addOpaqueEventCallback(void *callback) = 0;
    SWIGOUT(%ignore removeOpaqueEventCallback;)
    virtual void removeOpaqueEventCallback(void *callback) = 0;
    SWIGOUT(%ignore opaqueEventCallbacks;)
    virtual std::vector<void *> opaqueEventCallbacks() = 0;
    SWIGOUT(%ignore clearOpaqueEventCallbacks;)
    virtual void clearOpaqueEventCallbacks() = 0;

#ifdef SWIG
// expose the handling of the viewport callbacks to HOM
%extend
{
        void addEventCallback(InterpreterObject callback)
    {
        self->addOpaqueEventCallback(callback);
    }

    void removeEventCallback(InterpreterObject callback)
    {
        self->removeOpaqueEventCallback(callback);
    }

    void clearEventCallbacks()
    {
        self->clearOpaqueEventCallbacks();
    }

    std::vector<InterpreterObject> eventCallbacks()
    {
        std::vector<void *> opaque_callbacks(self->opaqueEventCallbacks());

        std::vector<InterpreterObject> callbacks;
        for (int i = 0; i< opaque_callbacks.size(); ++i)
            callbacks.push_back((InterpreterObject)opaque_callbacks[i]);
        return callbacks;
    }
}
#endif

};

#endif
