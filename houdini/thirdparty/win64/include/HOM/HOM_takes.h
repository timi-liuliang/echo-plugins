/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_takes_h__
#define __HOM_takes_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Take.h"

#include <string>
#include <vector>

SWIGOUT(%rename(takes) HOM_takes;)
class HOM_API HOM_takes
{
public:
    virtual ~HOM_takes()
    {}

    virtual std::string __repr__() = 0;

    virtual std::vector<HOM_ElemPtr<HOM_Take> > takes() = 0;

    SWIGOUT(%newobject currentTake;)
    virtual HOM_Take *currentTake() = 0;
    virtual void setCurrentTake(HOM_Take *take) = 0;

    SWIGOUT(%newobject rootTake;)
    virtual HOM_Take *rootTake() = 0;

    SWIGOUT(%newobject findTake;)
    virtual HOM_Take *findTake(const char *take_name) = 0;

    virtual std::string defaultTakeName() = 0;
    virtual void setDefaultTakeName(const char *default_name) = 0;

};

#endif
