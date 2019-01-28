/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef HOM_Drawable_h
#define HOM_Drawable_h

#include "HOM_API.h"
#include "HOM_Module.h"
#include "HOM_EnumValue.h"
#include "HOM_Defines.h"

SWIGOUT(%rename(Drawable) HOM_Drawable;)
SWIGOUT(%feature("notabstract") HOM_Drawable;)

class HOM_Matrix4;

class HOM_API HOM_Drawable
{
public:
#ifdef SWIG
%extend
{   
    HOM_Drawable(
	HOM_SceneViewer& scene_viewer,
	HOM_EnumValue const &prim_type,
	std::string const &name )
    {
	return HOM().newDrawable(scene_viewer, prim_type, name);
    }

    HOM_Drawable(
	HOM_SceneViewer& scene_viewer,
	HOM_Geometry const &geometry,
	std::string const &name )
    { 
	return HOM().newDrawable(scene_viewer, geometry, name);
    }
}
#else
    HOM_Drawable()
    { HOM_CONSTRUCT_OBJECT(this) }
#endif

    virtual ~HOM_Drawable()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;
    virtual std::string name() = 0;

    virtual void enable(bool value) = 0;
    virtual void show(bool value) = 0;

    virtual void setDisplayMode(HOM_EnumValue const & value) = 0;
    virtual HOM_EnumValue& displayMode() const = 0;

    virtual void setWireframeColor(HOM_Color const &color) = 0;

    virtual void setTransform(HOM_Matrix4 const & xform) = 0;
    virtual HOM_Matrix4 transform() = 0;
};

#endif // HOM_Drawable_h

