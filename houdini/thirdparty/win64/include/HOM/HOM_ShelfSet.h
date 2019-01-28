/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_ShelfSet_h__
#define __HOM_ShelfSet_h__

#include "HOM_API.h"
#include "HOM_ShelfElement.h"

class HOM_Shelf;

SWIGOUT(%rename(ShelfSet) HOM_ShelfSet;)

class HOM_API HOM_ShelfSet : virtual public HOM_ShelfElement
{
public:
    HOM_ShelfSet()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_ShelfSet(const HOM_ShelfSet &shelfset)
    : HOM_ShelfElement(shelfset)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_ShelfSet()
    { HOM_DESTRUCT_OBJECT(this) }

    // Let swig know we're overriding __repr__ for this class so it doesn't
    // provide its own __repr__.
    SWIGOUT(virtual std::string __repr__() = 0;)

    virtual std::vector<HOM_ElemPtr<HOM_Shelf> > shelves() = 0;

    // We need to receive the vector as a reference to a const, otherwise swig
    // won't convert from python sequences.
    virtual void setShelves(const std::vector<HOM_Shelf *> &shelves) = 0;

    virtual void destroy() = 0;
};

#endif

