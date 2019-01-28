/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PRM_KeyValueDictDataItem.h (PRM Library, C++)
 *
 * COMMENTS:    Represents a key-value dictionary (string keys and values) in a
 *		parameter.
 */

#ifndef __PRM_KeyValueDictDataItem__
#define __PRM_KeyValueDictDataItem__

#include "PRM_API.h"
#include "PRM_Parm.h"
#include "PRM_Type.h"

class UT_JSONValueMap;

class PRM_API PRM_KeyValueDictDataItem : public PRM_DataItem
{
public:
    PRM_KeyValueDictDataItem();
    explicit PRM_KeyValueDictDataItem(UT_JSONValueMap *key_value_dict);
    virtual ~PRM_KeyValueDictDataItem();

    virtual PRM_Type::PRM_DataType getDataType() const 
	{ return PRM_Type::PRM_DATA_KEY_VALUE_DICT; }
    virtual const char *getDataTypeToken() const { return "key_value_dict"; }

    virtual bool saveAscii(std::ostream &os) const;
    virtual bool saveBinary(std::ostream &os) const;

    virtual int64 getMemoryUsage() const
    {
	return sizeof(this);
    }

    // Data accessor.
    virtual UT_JSONValueMap *getKeyValueDict() const;

private:
    UT_JSONValueMap *myKeyValueDict;
};

#endif
