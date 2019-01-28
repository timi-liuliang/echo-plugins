/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_EdgeGroup_h__
#define __HOM_EdgeGroup_h__

#include "HOM_API.h"
#include "HOM_Module.h"
#include "HOM_EnumModules.h"
#include "HOM_Edge.h"
#include "HOM_PtrOrNull.h"
#include <vector>
class HOM_Geometry;

SWIGOUT(%rename(EdgeGroup) HOM_EdgeGroup;)

class HOM_API HOM_EdgeGroup
{
public:
    HOM_EdgeGroup()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_EdgeGroup(const HOM_EdgeGroup &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_EdgeGroup()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual bool operator==(HOM_PtrOrNull<HOM_EdgeGroup> edge_group) = 0;
    virtual bool operator!=(HOM_PtrOrNull<HOM_EdgeGroup> edge_group) = 0;

    virtual int __hash__() = 0;
    virtual std::string __repr__() = 0;

    SWIGOUT(%newobject geometry;)
    virtual HOM_Geometry *geometry() = 0;

    virtual std::string name() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Edge> > edges() = 0;

    virtual bool contains(HOM_Edge &edge) = 0;    

    virtual void add(HOM_Edge &edge) = 0;

    virtual void add(const std::vector<HOM_Edge *> &edges) = 0;

    virtual void add(HOM_EdgeGroup &edge_group) = 0;

    virtual void remove(HOM_Edge &edge) = 0;

    virtual void remove(const std::vector<HOM_Edge *> &edges) = 0;

    virtual void remove(HOM_EdgeGroup &edge_group) = 0;

    virtual void clear() = 0;

    virtual void destroy() = 0;

    
};

#endif
