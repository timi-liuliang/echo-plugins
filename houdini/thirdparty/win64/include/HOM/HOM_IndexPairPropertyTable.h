/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_IndexPairPropertyTable_h__
#define __HOM_IndexPairPropertyTable_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_PtrOrNull.h"
#include <string>
class HOM_Attrib;

SWIGOUT(%rename(IndexPairPropertyTable) HOM_IndexPairPropertyTable;)

class HOM_API HOM_IndexPairPropertyTable
{
public:
    HOM_IndexPairPropertyTable()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_IndexPairPropertyTable(const HOM_IndexPairPropertyTable &)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_IndexPairPropertyTable()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual bool operator==(HOM_PtrOrNull<HOM_IndexPairPropertyTable> proptable) = 0;
    virtual bool operator!=(HOM_PtrOrNull<HOM_IndexPairPropertyTable> proptable) = 0;

    virtual int __hash__() = 0;
    virtual std::string __repr__() = 0;

    SWIGOUT(%newobject attrib;)
    virtual HOM_Attrib *attrib() = 0;

    virtual int numIndices() = 0;

    virtual std::vector<std::string> propertyNames() = 0;
    virtual HOM_EnumValue propertyDataType(const char *prop_name) = 0;
    virtual int propertySize(const char *prop_name) = 0;

    virtual double floatPropertyValueAtIndex(const char *prop_name, int idx) = 0;
    virtual std::vector<double> floatListPropertyValueAtIndex(const char *prop_name, int idx) = 0;

    virtual int intPropertyValueAtIndex(const char *prop_name, int idx) = 0;
    virtual std::vector<int> intListPropertyValueAtIndex(const char *prop_name, int idx) = 0;

    virtual std::string stringPropertyValueAtIndex(const char *prop_name, int idx) = 0;
    virtual std::vector<std::string> stringListPropertyValueAtIndex(const char *prop_name, int idx) = 0;
};

#endif
