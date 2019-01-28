/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OBJ_ReLightProxy.h ( OBJ Library, C++)
 *
 * COMMENTS:
 *	Stand-in proxy for OBJ_ReLight.  If OBJ_ReLight is not loaded (i.e.
 *	non-graphical apps), then the proxy does nothing.
 */

#ifndef __OBJ_ReLightProxy__
#define __OBJ_ReLightProxy__

#include "OBJ_API.h"

#include "OBJ_Light.h"
#include <UT/UT_Vector3.h>

class OBJ_Ambient;
class OBJ_ReLightProxy;
class RE_Light;
class UT_Ramp;

typedef OBJ_ReLightProxy *(*OBJ_ReLightProxyCreateFunc)
    (OBJ_Ambient *obj_light);

// ==========================================================================
// OBJ_ReLightProxy declaration.
// ==========================================================================
class OBJ_API OBJ_ReLightProxy
{
public:
    virtual ~OBJ_ReLightProxy();

    virtual RE_Light *getLight();

    static OBJ_ReLightProxy *create(OBJ_Ambient *obj_light);
    static void registerCreateFunc(OBJ_ReLightProxyCreateFunc create_func);

    // Proxies for methods on the OBJ_ReLight object.
    // In this base class, these proxies are just stubs.
    virtual void enable(bool e);
    virtual void setInfinite(int onoff);
    virtual void setAmbient(int onoff);
    virtual void setCone(int onoff);
    virtual void setName(const char *name);
    virtual void setPosition(const UT_Vector3 &p);
    virtual void setDirection(const UT_Vector3 &d);
    virtual void setOrientation(UT_Matrix3 &orient);
    virtual void setColor(const UT_Vector3 &c);
    virtual void setConeAttrib(float a, float d, float r);
    virtual void setProjectMap(const char *name, const char *relativeTo);
    virtual void setProjectMapClamp(int onoff);
    virtual void setShadowed(bool onoff);
    virtual void setShadowIntensity(float i);
    virtual void setShadowBias(float b);
    virtual void setShadowBlur(float b);
    virtual void setShadowQuality(float q);
    virtual void setShadowMask(const char *m);
    virtual void setTransform(const UT_Matrix4 &m);
    virtual void setFocalLength(float focal);
    virtual void setAperture(float aper);
    virtual void setNearDistance(float znear);
    virtual void setFarDistance(float zfar);
    virtual int isInfinite() const;
    virtual void setOrthoWidth(float width);
    virtual void setDiffuse(int onoff);
    virtual void setSpecular(int onoff);
    virtual void setArea(bool onoff);
    virtual void setGlobalIllumination(bool onoff);
    virtual void setPortal(bool onoff);
    virtual void setAreaSide(bool single, bool reverse);
    virtual void setAreaAxes(const UT_Vector3F &axes);
    virtual void setSurfaceArea(float area);
    virtual void setAreaShadowShape(OBJ_LightAreaShape shape);
    virtual void setAreaTransform(UT_Matrix4 t);
    virtual void setEnvironmentMap(const char *name, const char *relativeTo);
    virtual void setEnvironment(bool onoff);
    virtual void setEnvironmentMipmap(bool onoff);
    virtual void setEnvironmentCone(float angle);
    virtual bool isEnvironmentLight() const;
    virtual void setAttenuation(float constant, float linear, float quadratic);
    virtual void setAttenuationRange(float near_dist, float far_dist);
    virtual void setAttenuationRamp(
		    const UT_Ramp *ramp, float start_dist, float end_dist);
    virtual void setLightShadowMapSize(int s);

protected:
    OBJ_ReLightProxy();

private:
    void verifySubclassOverride() const;
};


// ==========================================================================
// OBJ_ReLightProxy stub implementation.
// ==========================================================================

inline RE_Light *OBJ_ReLightProxy::getLight()
{
    verifySubclassOverride();
    return NULL;
}

inline void OBJ_ReLightProxy::enable(bool e)
{ verifySubclassOverride(); }

inline void OBJ_ReLightProxy::setInfinite(int onoff)
{ verifySubclassOverride(); }

inline void OBJ_ReLightProxy::setAmbient(int onoff)
{ verifySubclassOverride(); }

inline void OBJ_ReLightProxy::setCone(int onoff)
{ verifySubclassOverride(); }

inline void OBJ_ReLightProxy::setName(const char *name)
{ verifySubclassOverride(); }

inline void OBJ_ReLightProxy::setPosition(const UT_Vector3 &p)
{ verifySubclassOverride(); }

inline void OBJ_ReLightProxy::setDirection(const UT_Vector3 &d)
{ verifySubclassOverride(); }

inline void OBJ_ReLightProxy::setOrientation(UT_Matrix3 &orient)
{ verifySubclassOverride(); }

inline void OBJ_ReLightProxy::setColor(const UT_Vector3 &c)
{ verifySubclassOverride(); }

inline void OBJ_ReLightProxy::setConeAttrib(float a, float d, float r)
{ verifySubclassOverride(); }

inline void OBJ_ReLightProxy::setProjectMap(
    const char *name, const char *relativeTo)
{ verifySubclassOverride(); }

inline void OBJ_ReLightProxy::setProjectMapClamp(int onoff)
{ verifySubclassOverride(); }

inline void OBJ_ReLightProxy::setShadowed(bool onoff)
{ verifySubclassOverride(); }

inline void OBJ_ReLightProxy::setShadowIntensity(float i)
{ verifySubclassOverride(); }

inline void OBJ_ReLightProxy::setShadowBias(float b)
{ verifySubclassOverride(); }

inline void OBJ_ReLightProxy::setShadowBlur(float b)
{ verifySubclassOverride(); }

inline void OBJ_ReLightProxy::setShadowQuality(float q)
{ verifySubclassOverride(); }

inline void OBJ_ReLightProxy::setShadowMask(const char *m)
{ verifySubclassOverride(); }

inline void OBJ_ReLightProxy::setTransform(const UT_Matrix4 &m)
{ verifySubclassOverride(); }

inline void OBJ_ReLightProxy::setFocalLength(float focal)
{ verifySubclassOverride(); }

inline void OBJ_ReLightProxy::setAperture(float aper)
{ verifySubclassOverride(); }

inline void OBJ_ReLightProxy::setNearDistance(float znear)
{ verifySubclassOverride(); }

inline void OBJ_ReLightProxy::setFarDistance(float zfar)
{ verifySubclassOverride(); }

inline int OBJ_ReLightProxy::isInfinite() const 
{ 
    verifySubclassOverride();
    return 0;
}

inline void OBJ_ReLightProxy::setOrthoWidth(float width)
{ verifySubclassOverride(); }

inline void OBJ_ReLightProxy::setDiffuse(int onoff)
{ verifySubclassOverride(); }

inline void OBJ_ReLightProxy::setSpecular(int onoff)
{ verifySubclassOverride(); }

inline void OBJ_ReLightProxy::setArea(bool onoff)
{ verifySubclassOverride(); }

inline void OBJ_ReLightProxy::setGlobalIllumination(bool onoff)
{ verifySubclassOverride(); }

inline void OBJ_ReLightProxy::setPortal(bool onoff)
{ verifySubclassOverride(); }

inline void OBJ_ReLightProxy::setAreaSide(bool single, bool reverse)
{ verifySubclassOverride(); }

inline void OBJ_ReLightProxy::setAreaAxes(const UT_Vector3F &axes)
{ verifySubclassOverride(); }

inline void OBJ_ReLightProxy::setSurfaceArea(float area)
{ verifySubclassOverride(); }

inline void OBJ_ReLightProxy::setAreaShadowShape(OBJ_LightAreaShape shape)
{ verifySubclassOverride(); }

inline void OBJ_ReLightProxy::setAreaTransform(UT_Matrix4 t)
{ verifySubclassOverride(); }

inline void OBJ_ReLightProxy::setEnvironmentMap(
    const char *name, const char *relativeTo)
{ verifySubclassOverride(); }

inline void OBJ_ReLightProxy::setEnvironment(bool onoff)
{ verifySubclassOverride(); }

inline void OBJ_ReLightProxy::setEnvironmentMipmap(bool onoff)
{ verifySubclassOverride(); }

inline void OBJ_ReLightProxy::setEnvironmentCone(float angle)
{ verifySubclassOverride(); }

inline bool OBJ_ReLightProxy::isEnvironmentLight() const
{
    verifySubclassOverride();
    return false;
}

inline void OBJ_ReLightProxy::setAttenuation(
    float constant, float linear, float quadratic)
{ verifySubclassOverride(); }

inline void OBJ_ReLightProxy::setAttenuationRange(
    float near_dist, float far_dist)
{ verifySubclassOverride(); }

inline void OBJ_ReLightProxy::setAttenuationRamp(
    const UT_Ramp *ramp, float start_dist, float end_dist)
{ verifySubclassOverride(); }

inline void OBJ_ReLightProxy::setLightShadowMapSize(int s)
{ verifySubclassOverride(); }

#endif
