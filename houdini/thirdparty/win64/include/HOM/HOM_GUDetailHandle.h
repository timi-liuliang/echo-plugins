/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 *     This class is not documented in the user documentation.  See
 *     HOMF_GUDetailHandle for why it exists.
 */

#ifndef __HOM_GUDetailHandle_h__
#define __HOM_GUDetailHandle_h__

#include "HOM_API.h"
#include "HOM_Module.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include <string>
class HOM_Geometry;

SWIGOUT(%rename(_GUDetailHandle) HOM_GUDetailHandle;)
SWIGOUT(%ignore HOM_GUDetailHandle::_asHandle();)

class HOM_API HOM_GUDetailHandle
{
public:
    HOM_GUDetailHandle()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_GUDetailHandle(const HOM_GUDetailHandle &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_GUDetailHandle()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;

    virtual void *_asVoidPointer() = 0;

    virtual bool isFrozen() = 0;

    virtual bool isReadOnly() = 0;

    virtual void destroy() = 0;

    virtual void *_asHandle() = 0;
};

#endif
