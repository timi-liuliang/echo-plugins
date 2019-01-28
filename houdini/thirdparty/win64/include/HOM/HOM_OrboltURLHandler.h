/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_OrboltURLHandler_h__
#define __HOM_OrboltURLHandler_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"

SWIGOUT(%rename(OrboltURLHandler) HOM_OrboltURLHandler;)

class HOM_API HOM_OrboltURLHandler
{
public:
    HOM_OrboltURLHandler()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_OrboltURLHandler(const HOM_OrboltURLHandler &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_OrboltURLHandler()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;

    virtual void updateProgressBar(double percentage) = 0;
    virtual void closeSplashScreen(bool immediate = false) = 0;

};

#endif
