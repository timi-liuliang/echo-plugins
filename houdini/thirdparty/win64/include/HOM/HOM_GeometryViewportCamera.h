/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */
#ifndef __HOM_GeometryViewportCamera_h
#define __HOM_GeometryViewportCamera_h

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_EnumValue.h"
#include "HOM_Matrix3.h"

SWIGOUT(%rename(GeometryViewportCamera) HOM_GeometryViewportCamera;)

class HOM_API HOM_GeometryViewportCamera
{
public:
    HOM_GeometryViewportCamera()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_GeometryViewportCamera(const HOM_GeometryViewportCamera &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_GeometryViewportCamera()
    { HOM_DESTRUCT_OBJECT(this) }
    
    virtual std::string __repr__() = 0;

    SWIGOUT(%newobject stash;)
    virtual HOM_GeometryViewportCamera *stash() = 0;
    
    virtual void   setPerspective(bool perspective) = 0;
    virtual bool   isPerspective() = 0;
    virtual bool   isOrthographic() = 0;

    virtual void   setAperture(double ap) = 0;
    virtual double aperture() = 0;

    virtual void   setFocalLength(double fl) = 0;
    virtual double focalLength() = 0;

    virtual void   setAspectRatio(double ar) = 0;
    virtual double aspectRatio() = 0;
    
    virtual void   setOrthoWidth(double ow) = 0;
    virtual double orthoWidth() = 0;

    virtual void   setRotation(const HOM_Matrix3 &mat) = 0;
    virtual HOM_Matrix3 rotation() = 0;
    
    virtual void   setTranslation(std::vector<double> xyz) = 0;
    virtual std::vector<double> translation() = 0;

    virtual void   setPivot(std::vector<double> xyz) = 0;
    virtual std::vector<double> pivot() = 0;

    virtual void   setClipPlanes(std::vector<double> near_far) = 0;
    virtual std::vector<double> clipPlanes() = 0;

    virtual void   setWindowOffset(std::vector<double> xy) = 0;
    virtual std::vector<double> windowOffset() = 0;

    virtual void   setWindowSize(std::vector<double> size) = 0;
    virtual std::vector<double> windowSize() = 0;
};

#endif

