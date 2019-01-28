/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_ShelfDock_h__
#define __HOM_ShelfDock_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Module.h"
#include "HOM_Errors.h"
#include <string>
#include <vector>

SWIGOUT(%rename(ShelfDock) HOM_ShelfDock;)

class HOM_ShelfSet;

class HOM_API HOM_ShelfDock
{
public:
    HOM_ShelfDock()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_ShelfDock(const HOM_ShelfDock &element)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_ShelfDock()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_ShelfSet> > shelfSets() = 0;

    virtual std::vector<int> iconsize() = 0;

    virtual void show(bool on) = 0;
};

#endif

