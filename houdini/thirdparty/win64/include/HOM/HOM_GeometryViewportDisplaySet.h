/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_GeometryViewportDisplaySet_h__
#define __HOM_GeometryViewportDisplaySet_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"
#include "HOM_EnumValue.h"

SWIGOUT(%rename(GeometryViewportDisplaySet) HOM_GeometryViewportDisplaySet;)

#define TOGGLE_MARKER(name)		\
    virtual void show##name(bool b)=0;	\
    virtual bool isShowing##name()=0

#define TOGGLE_LOOK(name)		\
    virtual void use##name(bool b)=0;	\
    virtual bool isUsing##name()=0

#define TOGGLE_ITEM(name)		\
    virtual void set##name(bool b)=0;	\
    virtual bool is##name()=0

#define MARKER_VISIBILITY(getname, setname)			\
    virtual void set##setname##Visibility(HOM_EnumValue &v)=0; \
    virtual HOM_EnumValue &getname##Visibility()=0

class HOM_API HOM_GeometryViewportDisplaySet
{
public:
    HOM_GeometryViewportDisplaySet()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_GeometryViewportDisplaySet(const HOM_GeometryViewportDisplaySet &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_GeometryViewportDisplaySet()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;

    virtual HOM_EnumValue &displaySetType() = 0;

    // show/isShowing
    TOGGLE_MARKER(PointMarkers);
    TOGGLE_MARKER(PointNumbers);
    TOGGLE_MARKER(PointNormals);
    TOGGLE_MARKER(PointPositions);
    TOGGLE_MARKER(PointUVs);
    TOGGLE_MARKER(PointTrails);
    TOGGLE_MARKER(CoincidentPoints);

    TOGGLE_MARKER(PrimHulls);
    TOGGLE_MARKER(PrimNumbers);
    TOGGLE_MARKER(PrimNormals);
    TOGGLE_MARKER(PrimProfiles);
    TOGGLE_MARKER(PrimBreakpoints);
    TOGGLE_MARKER(PrimProfileNumbers);
    TOGGLE_MARKER(PrimBackfaces);
    
    TOGGLE_MARKER(VertexMarkers);
    TOGGLE_MARKER(VertexNumbers);
    TOGGLE_MARKER(VertexNormals);
    TOGGLE_MARKER(VertexUVs);
    TOGGLE_MARKER(UVBackfaces);
    TOGGLE_MARKER(UVOverlap);

    // *Visibility, set*Visibility
    MARKER_VISIBILITY(pointMarker, PointMarker);
    MARKER_VISIBILITY(pointNumber, PointNumber);
    MARKER_VISIBILITY(pointNormal, PointNormal);
    MARKER_VISIBILITY(pointPosition, PointPosition);
    MARKER_VISIBILITY(pointUV, PointUV);
    MARKER_VISIBILITY(pointTrail, PointTrail);

    MARKER_VISIBILITY(primNumber, PrimNumber);
    MARKER_VISIBILITY(primNormal, PrimNormal);
    MARKER_VISIBILITY(primBreakpoint, PrimBreakpoint);
    
    MARKER_VISIBILITY(vertexMarker, VertexMarker);
    MARKER_VISIBILITY(vertexNormal, VertexNormal);
    MARKER_VISIBILITY(vertexNumber, VertexNumber);
    MARKER_VISIBILITY(vertexUV, VertexUV);

    // set/isUsing
    TOGGLE_LOOK(GhostedLook);
    TOGGLE_LOOK(FadedLook);
    TOGGLE_LOOK(XRay);
    TOGGLE_LOOK(Lighting);

    // set/is
    TOGGLE_ITEM(ShadingModeLocked);
    TOGGLE_ITEM(ToolbarLinked);
    TOGGLE_ITEM(UniqueDisplaySet);

    virtual void setShadedMode(HOM_EnumValue &)=0;
    virtual HOM_EnumValue &shadedMode()=0;

    virtual void setBoundaryMode(HOM_EnumValue &) = 0;
    virtual HOM_EnumValue &boundaryMode() = 0;

    virtual void setUVBoundaryMode(HOM_EnumValue &) = 0;
    virtual HOM_EnumValue &uvBoundaryMode() = 0;

    virtual void setLinkToDisplaySet(HOM_EnumValue &)=0;
    virtual HOM_EnumValue & linkedToDisplaySet()=0;
    
};

#undef TOGGLE_MARKER
#undef TOGGLE_LOOK
#undef TOGGLE_ITEM
#undef MARKER_VISIBILITY
#endif
