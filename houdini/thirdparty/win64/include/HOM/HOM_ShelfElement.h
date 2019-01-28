/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_ShelfElement_h__
#define __HOM_ShelfElement_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Module.h"
#include "HOM_Errors.h"
#include "HOM_PtrOrNull.h"
#include <string>

SWIGOUT(%rename(ShelfElement) HOM_ShelfElement;)

class HOM_API HOM_ShelfElement
{
public:
    HOM_ShelfElement()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_ShelfElement(const HOM_ShelfElement &element)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_ShelfElement()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__() = 0;

    virtual bool operator==(HOM_PtrOrNull<HOM_ShelfElement> element) = 0;
    virtual bool operator!=(HOM_PtrOrNull<HOM_ShelfElement> element) = 0;

    virtual std::string filePath() = 0;
    virtual void setFilePath(const char *file_path) = 0;
    virtual std::string name() = 0;
    virtual void setName(const char *name) = 0;
    virtual std::string label() = 0;
    virtual void setLabel(const char *label) = 0;

    virtual bool isReadOnly() = 0;
    virtual void setReadOnly(bool on) = 0;
};

#endif

