/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_Surface_h__
#define __HOM_Surface_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Prim.h"
#include "HOM_Vertex.h"
#include <vector>
#include <stdexcept>
class HOM_Vector3;
class HOM_Vector4;

SWIGOUT(%rename(Surface) HOM_Surface;)

class HOM_API HOM_Surface : virtual public HOM_Prim
{
public:
    HOM_Surface()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_Surface(const HOM_Surface &surface)
    : HOM_Prim(surface)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_Surface()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0;)


    virtual int numRows() = 0;
    virtual int numCols() = 0;

    SWIGOUT(%newobject vertex;)
    virtual HOM_Vertex *vertex(int u_index, int v_index) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Vertex> > verticesInRow(int v_index) = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Vertex> > verticesInCol(int u_index) = 0;


    virtual bool isClosedInU() = 0;
    virtual bool isClosedInV() = 0;

    SWIGOUT(%newobject positionAt;)
    virtual HOM_Vector3 *positionAt(double u, double v) = 0;

    SWIGOUT(%newobject normalAt;)
    virtual HOM_Vector3 *normalAt(double u, double v) = 0;


    // These methods return hboost::any objects and are not wrapped directly
    // by swig.
    SWIGOUT(%ignore boostAnyAttribValueAt;)
    virtual hboost::any boostAnyAttribValueAt(
	    const char *name, double u, double v, int du=0, int dv=0) = 0;

    SWIGOUT(%ignore boostAnyAttribValueAt;)
    virtual hboost::any boostAnyAttribValueAt(
	    HOM_Attrib &attrib, double u, double v, int du=0, int dv=0) = 0;

#ifdef SWIG
%extend
{
    InterpreterObject attribValueAt(
	    const char *name, double u, double v, int du=0, int dv=0)
    {
	return HOMboostAnyToInterpreterObject(
	    self->boostAnyAttribValueAt(name, u, v, du, dv));
    }

    InterpreterObject attribValueAt(
	    HOM_Attrib &attrib, double u, double v, int du=0, int dv=0)
    {
	return HOMboostAnyToInterpreterObject(
	    self->boostAnyAttribValueAt(attrib, u, v, du, dv));
    }
}
#endif


    SWIGOUT(%kwargs addRow;)
    virtual void addRow(int after=-1) = 0;

    SWIGOUT(%kwargs addCol;)
    virtual void addCol(int after=-1) = 0;
};

#endif
