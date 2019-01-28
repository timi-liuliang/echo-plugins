/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_Edge_h__
#define __HOM_Edge_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Vector3.h"
#include "HOM_PtrOrNull.h"
#include <vector>

class HOM_Geometry;
class HOM_Attrib;
class HOM_Point;
class HOM_Prim;

SWIGOUT(%rename(Edge) HOM_Edge;)

class HOM_API HOM_Edge
{
public:
    HOM_Edge()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_Edge(const HOM_Edge &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_Edge()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual bool operator==(HOM_PtrOrNull<HOM_Edge> edge) = 0;

    virtual bool operator!=(HOM_PtrOrNull<HOM_Edge> edge) = 0;    

    virtual int __hash__() = 0;
    virtual std::string __repr__() = 0;

    SWIGOUT(%newobject geometry;)
    virtual HOM_Geometry *geometry() = 0;    

    virtual std::vector<HOM_ElemPtr<HOM_Point> > points() = 0;
    
    virtual std::string edgeId() = 0;

    virtual float length() = 0;
    
    virtual std::vector<HOM_ElemPtr<HOM_Prim> > prims() = 0;

};

#endif
