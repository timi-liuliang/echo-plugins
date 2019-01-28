/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_ViewportVisualizerType_h__
#define __HOM_ViewportVisualizerType_h__

#include "HOM_API.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"

SWIGOUT(%rename(ViewportVisualizerType) HOM_ViewportVisualizerType;)

class HOM_API HOM_ViewportVisualizerType
{
public:
    HOM_ViewportVisualizerType()
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_ViewportVisualizerType()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual bool operator==(HOM_PtrOrNull<HOM_ViewportVisualizerType> type) = 0;
    virtual bool operator!=(HOM_PtrOrNull<HOM_ViewportVisualizerType> type) = 0;

    virtual std::string __repr__() = 0;

    virtual std::string name() = 0;
    virtual std::string description() = 0;
    virtual std::string icon() = 0;
};

#endif

