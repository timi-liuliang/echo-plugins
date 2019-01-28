/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_DopRecord_h__
#define __HOM_DopRecord_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Module.h"
#include "HOM_EnumValue.h"
#include "HOM_Errors.h"
#include "HOM_PtrOrNull.h"
#include <UT/UT_Options.h>
#include <UT/UT_UniquePtr.h>
#include <string>
#include <vector>

class HOM_DopData;
class HOM_Vector2;
class HOM_Vector3;
class HOM_Vector4;
class HOM_Quaternion;
class HOM_Matrix3;
class HOM_Matrix4;

SWIGOUT(%rename(DopRecord) HOM_DopRecord;)

class HOM_API HOM_DopRecord
{
public:
    HOM_DopRecord()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_DopRecord(const HOM_DopRecord &record)
    { HOM_CONSTRUCT_OBJECT(this) }
    virtual ~HOM_DopRecord()
    { HOM_DESTRUCT_OBJECT(this) }

    // We compare against a pointer instead of a reference because we want
    // to be able to compare against None.
    virtual bool operator==(HOM_PtrOrNull<HOM_DopRecord> record) = 0;
    virtual bool operator!=(HOM_PtrOrNull<HOM_DopRecord> record) = 0;

    virtual int __hash__() = 0;
    virtual std::string __repr__() = 0;

    virtual std::string recordType() = 0;
    virtual int recordIndex() = 0;

    // Get the names of all fields in this record.
    virtual std::vector<std::string> fieldNames() = 0;

    // Get the data type of a particular field.
    virtual HOM_EnumValue &fieldType(const char *field_name) = 0;

    // This is the function that does the real work of fetching the field
    // value. It returns a UT_OptionEntry, which gets converted by the field
    // function into a python object. This function is never called directly,
    // only through field.
    SWIGOUT(%ignore fieldAsRaw;)
    virtual UT_OptionEntry *fieldAsRaw(const char *field_name) = 0;

#ifdef SWIG
%extend
{
    InterpreterObject field(const char *field_name)
    {
	UT_UniquePtr<UT_OptionEntry> raw_value(
	    self->fieldAsRaw(field_name));
	return raw_value.get()
	    ? HOMoptionEntryToInterpreterObject(*raw_value) : SWIG_Py_Void();
    }
}
#endif

    // Swig refuses to check for a bool before an int, even if you rearrange
    // the declarations, so we need a different method to set bool fields.
    virtual void setFieldBool(const char *field_name, bool value) = 0;
    virtual void setField(const char *field_name, int value) = 0;
    virtual void setField(const char *field_name, double value) = 0;
    virtual void setField(const char *field_name, const char *value) = 0;
    virtual void setField(const char *field_name, HOM_Vector2 &value) = 0;
    virtual void setField(const char *field_name, HOM_Vector3 &value) = 0;
    virtual void setField(const char *field_name, HOM_Vector4 &value) = 0;
    virtual void setField(const char *field_name, HOM_Quaternion &value) = 0;
    virtual void setField(const char *field_name, HOM_Matrix3 &value) = 0;
    virtual void setField(const char *field_name, HOM_Matrix4 &value) = 0;
};

#endif

