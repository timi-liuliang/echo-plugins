/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_Point_h__
#define __HOM_Point_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Vector3.h"
#include "HOM_PtrOrNull.h"
#include <vector>

class HOM_Geometry;
class HOM_Attrib;

SWIGOUT(%rename(Point) HOM_Point;)

class HOM_API HOM_Point
{
public:
    HOM_Point()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_Point(const HOM_Point &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_Point()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual bool operator==(HOM_PtrOrNull<HOM_Point> point) = 0;

    virtual bool operator!=(HOM_PtrOrNull<HOM_Point> point) = 0;


    virtual int __hash__() = 0;
    virtual std::string __repr__() = 0;

    SWIGOUT(%newobject geometry;)
    virtual HOM_Geometry *geometry() = 0;

    virtual int number() = 0;


    SWIGOUT(%newobject position;)
    virtual HOM_Vector3 *position() = 0;
    virtual void setPosition(const std::vector<double> &position) = 0;

    virtual double weight() = 0;
    virtual void setWeight(double weight) = 0;


    virtual HOM_EnumValue& attribType() = 0;


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
    // vector of ints before the vector of floats.  Otherwise, swig will call
    // the float version when you pass in a vector of ints.
    virtual void setAttribValue(
	    const char *attrib_name, const std::vector<int64> &attrib_value) = 0;
    virtual void setAttribValue(
	    const char *attrib_name, const std::vector<double> &attrib_value) = 0;
    virtual void setAttribValue(
	    const char *attrib_name,
	    const std::vector<std::string> &attrib_value) = 0;

#ifdef SWIG
%extend
{
    InterpreterObject attribValue(const char *name)
    { return HOMattribValue(*self, name); }

    InterpreterObject attribValue(HOM_Attrib &attrib)
    { return HOMattribValue(*self, attrib); }
}
#endif

    virtual std::vector<HOM_ElemPtr<HOM_Vertex> > vertices() = 0;
    virtual std::vector<HOM_ElemPtr<HOM_Prim> > prims() = 0;
};

#endif
