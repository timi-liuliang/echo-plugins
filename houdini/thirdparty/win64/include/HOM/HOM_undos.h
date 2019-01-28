/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_undos_h__
#define __HOM_undos_h__

#include "HOM_API.h"
#include "HOM_Errors.h"

#include <string>
#include <vector>
#include <stddef.h>


class HOM_UndosDisabler;
class HOM_UndosGroup;


SWIGOUT(%rename(undos) HOM_undos;)
class HOM_API HOM_undos
{
public:
    virtual ~HOM_undos()
    {}

    virtual std::string __repr__() = 0;

    virtual bool areEnabled() = 0;

    SWIGOUT(%newobject disabler;)
    virtual HOM_UndosDisabler *disabler() = 0;

    SWIGOUT(%newobject group;)
    virtual HOM_UndosGroup *group(const char *label) = 0;

    virtual void clear() = 0;

    virtual size_t memoryUsage() = 0;

    virtual size_t memoryUsageLimit() = 0;

    virtual bool performUndo() = 0;

    virtual bool performRedo() = 0;

    virtual std::vector<std::string> undoLabels() = 0;

    virtual std::vector<std::string> redoLabels() = 0;
};

#endif
