/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_Prim_h__
#define __HOM_Prim_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_BoundingBox.h"
#include "HOM_Vertex.h"
#include "HOM_IterableList.h"
#include "HOM_PtrOrNull.h"
#include <UT/UT_Tuple.h>
#include <vector>

class HOM_EnumValue;
class HOM_Geometry;
class HOM_PrimGroup;
class HOM_Edge;
class HOM_Vector3;

SWIGOUT(%rename(Prim) HOM_Prim;)

class HOM_API HOM_Prim
{
public:
    HOM_Prim()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_Prim(const HOM_Prim &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_Prim()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual bool operator==(HOM_PtrOrNull<HOM_Prim> prim) = 0;
    virtual bool operator!=(HOM_PtrOrNull<HOM_Prim> prim) = 0;

    virtual int __hash__() = 0;
    virtual std::string __repr__() = 0;

    SWIGOUT(%newobject geometry;)
    virtual HOM_Geometry *geometry() = 0;

    virtual int number() = 0;

    virtual HOM_EnumValue &type() = 0;
    virtual HOM_EnumValue &attribType() = 0;


    SWIGOUT(%newobject vertices;)
    virtual HOM_IterableList<HOM_Vertex> *vertices() = 0;

    virtual int numVertices() = 0;


    virtual std::vector<HOM_ElemPtr<HOM_Edge> > edges() = 0;

    virtual int numEdges() = 0;

    virtual double floatAttribValue(const char *name) = 0;

    virtual double floatAttribValue(HOM_Attrib &attrib) = 0;

    virtual std::vector<double> floatListAttribValue(const char *name) = 0;

    virtual std::vector<double> floatListAttribValue(HOM_Attrib &attrib) = 0;

    virtual int64 intAttribValue(const char *name) = 0;

    virtual int64 intAttribValue(HOM_Attrib &attrib) = 0;

    virtual std::vector<int64> intListAttribValue(const char *name) = 0;

    virtual std::vector<int64> intListAttribValue(HOM_Attrib &attrib) = 0;

    virtual std::string stringAttribValue(const char *name) = 0;

    virtual std::string stringAttribValue(HOM_Attrib &attrib) = 0;

    virtual std::vector<std::string> stringListAttribValue(const char *name) = 0;

    virtual std::vector<std::string> stringListAttribValue(HOM_Attrib &attrib) = 0;


    virtual void setAttribValue(
	    HOM_Attrib &attrib, int64 attrib_value) = 0;
    virtual void setAttribValue(
	    HOM_Attrib &attrib, double attrib_value) = 0;
    virtual void setAttribValue(
	    HOM_Attrib &attrib, const char *attrib_value) = 0;
    virtual void setAttribValue(
	    HOM_Attrib &attrib, const std::vector<int64> &attrib_value) = 0;
    virtual void setAttribValue(
	    HOM_Attrib &attrib, const std::vector<double> &attrib_value) = 0;
    virtual void setAttribValue(
	    HOM_Attrib &attrib, const std::vector<std::string> &attrib_value) = 0;

    virtual void setAttribValue(
	    const char *attrib_name, int64 attrib_value) = 0;
    virtual void setAttribValue(
	    const char *attrib_name, double attrib_value) = 0;
    virtual void setAttribValue(
	    const char *attrib_name, const char *attrib_value) = 0;
    // Note that, because of strangess the with way swig works when deciding
    // which overloaded method to call, it's very important that we list the
    // vector of ints before the vector of doubles.  Otherwise, swig will call
    // the double version when you pass in a vector of ints.
    virtual void setAttribValue(
	    const char *attrib_name, const std::vector<int64> &attrib_value) = 0;
    virtual void setAttribValue(
	    const char *attrib_name, const std::vector<double> &attrib_value) = 0;
    virtual void setAttribValue(
	    const char *attrib_name,
	    const std::vector<std::string> &attrib_value) = 0;


    virtual std::vector<std::string> intrinsicNames() = 0;

    virtual hboost::any intrinsicValue(
	    const char *intrinsic_name) = 0;
    virtual size_t intrinsicSize(const char *intrinsic_name) = 0;
    virtual bool intrinsicReadOnly(const char *intrinsic_name) = 0;

    virtual void setIntrinsicValue(const char *intrinsic_name, double value) = 0;
    virtual void setIntrinsicValue(const char *intrinsic_name, int value) = 0;
    virtual void setIntrinsicValue(const char *intrinsic_name, int64 value) = 0;
    virtual void setIntrinsicValue(
	    const char *intrinsic_name, const char *value) = 0;
    virtual void setIntrinsicValue(
	    const char *intrinsic_name, const std::vector<double> &value) = 0;
    virtual void setIntrinsicValue(
	    const char *intrinsic_name, const std::vector<int> &value) = 0;
    virtual void setIntrinsicValue(
	    const char *intrinsic_name, const std::vector<int64> &value) = 0;
    virtual void setIntrinsicValue(
	    const char *intrinsic_name, const std::vector<std::string> &value) = 0;

#ifdef SWIG
%extend
{
    InterpreterObject attribValue(const char *name)
    { return HOMattribValue(*self, name); }

    InterpreterObject attribValue(HOM_Attrib &attrib)
    { return HOMattribValue(*self, attrib); }
}
#endif

    SWIGOUT(%newobject positionAtInterior;)
    virtual HOM_Vector3 *positionAtInterior(double u, double v, double w = 0.0) = 0;

    SWIGOUT(%ignore boostAnyAttribValueAtInterior;)
    virtual hboost::any boostAnyAttribValueAtInterior(
	    const char *name, double u, double v, double w) = 0;

    SWIGOUT(%ignore boostAnyAttribValueAtInterior;)
    virtual hboost::any boostAnyAttribValueAtInterior(
	    HOM_Attrib &attrib, double u, double v, double w) = 0;

#ifdef SWIG
%extend
{
    InterpreterObject attribValueAtInterior(
	    const char *name, double u, double v, double w = 0.0)
    {
	return HOMboostAnyToInterpreterObject(
	    self->boostAnyAttribValueAtInterior(name, u, v, w));
    }

    InterpreterObject attribValueAtInterior(
	    HOM_Attrib &attrib, double u, double v, double w = 0.0)
    {
	return HOMboostAnyToInterpreterObject(
	    self->boostAnyAttribValueAtInterior(attrib, u, v, w));
    }
}
#endif

    virtual HOM_BoundingBox boundingBox() = 0;

    virtual UT_Tuple<double, double, double> nearestToPosition(
	    const std::vector<double> &position) = 0;

    virtual std::vector< HOM_ElemPtr<HOM_PrimGroup> > groups() = 0;

};

#endif
