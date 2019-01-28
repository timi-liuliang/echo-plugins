/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_Face_h__
#define __HOM_Face_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Prim.h"
#include "HOM_IterableList.h"
#include <vector>
#include <stdexcept>
class HOM_Vector3;
class HOM_Point;

SWIGOUT(%rename(Face) HOM_Face;)

class HOM_API HOM_Face : virtual public HOM_Prim
{
public:
    HOM_Face()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_Face(const HOM_Face &face)
    : HOM_Prim(face)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_Face()
    { HOM_DESTRUCT_OBJECT(this) }

    // TODO: Do we need to explicitly add operator== and operator!=, or
    //       does swig pick up the base class ones?

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0;)

    virtual bool isClosed() = 0;

    // This method is deprecated in favour of isClosed().
    bool closed()
    { return isClosed(); }

    virtual void setIsClosed(bool on) = 0;

    SWIGOUT(%newobject normal;)
    virtual HOM_Vector3 *normal() = 0;

    SWIGOUT(%newobject positionAt;)
    virtual HOM_Vector3 *positionAt(double u) = 0;

    // These methods return hboost::any objects and are not wrapped directly
    // by swig.
    SWIGOUT(%ignore boostAnyAttribValueAt;)
    virtual hboost::any boostAnyAttribValueAt(
	    const char *name, double u, int du=0) = 0;

    SWIGOUT(%ignore boostAnyAttribValueAt;)
    virtual hboost::any boostAnyAttribValueAt(
	    HOM_Attrib &attrib, double u, int du=0) = 0;

#ifdef SWIG
%extend
{
    InterpreterObject attribValueAt(
	    const char *name, double u, int du=0)
    {
	return HOMboostAnyToInterpreterObject(
	    self->boostAnyAttribValueAt(name, u, du));
    }

    InterpreterObject attribValueAt(
	    HOM_Attrib &attrib, double u, int du=0)
    {
	return HOMboostAnyToInterpreterObject(
	    self->boostAnyAttribValueAt(attrib, u, du));
    }
}
#endif

    SWIGOUT(%newobject vertex;)
    virtual HOM_Vertex *vertex(int index) = 0;

    SWIGOUT(%newobject addVertex;)
    virtual HOM_Vertex *addVertex(HOM_Point &point) = 0;

    virtual double arcLength(double u_start, double u_stop, int divs=10) = 0;
};

#endif
