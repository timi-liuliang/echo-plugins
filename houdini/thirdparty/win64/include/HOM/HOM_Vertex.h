/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_Vertex_h__
#define __HOM_Vertex_h__

#include "HOM_API.h"
#include "HOM_Module.h"
#include "HOM_PtrOrNull.h"
#include "HOM_EnumValue.h"
#include <vector>
class HOM_Attrib;
class HOM_Geometry;
class HOM_Prim;
class HOM_Point;

SWIGOUT(%rename(Vertex) HOM_Vertex;)

class HOM_API HOM_Vertex
{
public:
    HOM_Vertex()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_Vertex(const HOM_Vertex &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_Vertex()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual bool operator==(HOM_PtrOrNull<HOM_Vertex> vertex) = 0;
    virtual bool operator!=(HOM_PtrOrNull<HOM_Vertex> vertex) = 0;

    virtual int __hash__() = 0;
    virtual std::string __repr__() = 0;

    SWIGOUT(%newobject geometry;)
    virtual HOM_Geometry *geometry() = 0;

    SWIGOUT(%newobject prim;)
    virtual HOM_Prim *prim() = 0;

    SWIGOUT(%newobject point;)
    virtual HOM_Point *point() = 0;

    virtual int number() = 0;

    virtual int linearNumber() = 0;

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
    // vector of ints before the vector of doubles.  Otherwise, swig will call
    // the double version when you pass in a vector of ints.
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
};

#endif
