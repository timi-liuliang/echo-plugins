/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_QueryArrays_h__
#define __SIM_QueryArrays_h__

#include "SIM_API.h"
#include <UT/UT_IntArray.h>
#include <UT/UT_StringArray.h>
#include "SIM_Query.h"

class UT_BitArray;

/// This class provides an interface between a SIM_Data and the Houdini
/// expression language. It also helps in building the tree view of a
/// simulation.
class SIM_API SIM_QueryArrays : public SIM_Query
{
public:
    /// Constructor which takes a const SIM_Data as our owner.
    explicit			 SIM_QueryArrays(const SIM_Data *owner);
    /// Destructor for this class.
    virtual			~SIM_QueryArrays();

    void			 addArray(const char *recordtype,
					  const char *fieldname,
					  const UT_BitArray *array);
    void			 addArray(const char *recordtype,
					  const char *fieldname,
					  const UT_IntArray *array);
    void			 addArray(const char *recordtype,
					  const char *fieldname,
					  const UT_FloatArray *array);
    void			 addArray(const char *recordtype,
					  const char *fieldname,
					  const UT_StringArray *array);
    void			 addArray(const char *recordtype,
					  const char *fieldname,
					  const UT_Vector2Array *array);
    void			 addArray(const char *recordtype,
					  const char *fieldname,
					  const UT_Vector3Array *array);
    void			 addArray(const char *recordtype,
					  const char *fieldname,
					  const UT_Vector4Array *array);

protected:
    virtual int			 getNumRecordTypesSubclass() const;
    virtual const char		*getRecordTypeNameSubclass(
					int recordtypenum) const;

    virtual int			 getNumRecordsSubclass(
					const char *recordtype) const;
    virtual int			 getNumFieldsSubclass(
					const char *recordtype) const;
    virtual const char		*getFieldNameSubclass(
					const char *recordtype,
					int fieldnum) const;
    virtual UT_OptionType	 getFieldTypeSubclass(
					const char *recordtype,
					int fieldnum) const;
    virtual bool		 getFieldRawSubclass(
					const char *recordtype,
					int recordnum,
					const char *fieldname,
					UT_OptionEntry *&result) const;

private:
    class SIM_QueryArrayField
    {
    public:
					 SIM_QueryArrayField(
						const UT_BitArray *array);
					 SIM_QueryArrayField(
						const UT_IntArray *array);
					 SIM_QueryArrayField(
						const UT_FloatArray *array);
					 SIM_QueryArrayField(
						const UT_StringArray *array);
					 SIM_QueryArrayField(
						const UT_Vector2Array *array);
					 SIM_QueryArrayField(
						const UT_Vector3Array *array);
					 SIM_QueryArrayField(
						const UT_Vector4Array *array);
					~SIM_QueryArrayField();

	UT_OptionType			 getFieldType() const;
	int				 getNumEntries() const;
	bool				 getValueRaw(int entrynum,
						UT_OptionEntry *&result) const;

    private:
	void				 initialize();

	UT_OptionType				 myFieldType;
	const UT_BitArray			*myBitArray;
	const UT_IntArray			*myIntArray;
	const UT_FloatArray			*myFloatArray;
	const UT_StringArray			*myStringArray;
	const UT_Vector2Array			*myVector2Array;
	const UT_Vector3Array			*myVector3Array;
	const UT_Vector4Array			*myVector4Array;
    };

    class SIM_QueryArrayRecord
    {
    public:
					 SIM_QueryArrayRecord();
					~SIM_QueryArrayRecord();

	void				 addField(const char *fieldname,
						SIM_QueryArrayField *field);
	int				 getNumFields() const;
	const char			*getFieldName(int fieldnum) const;
	UT_OptionType			 getFieldType(int fieldnum) const;
	const SIM_QueryArrayField	*getField(int fieldnum) const;
	int				 getNumEntries() const;
	bool				 getValueRaw(const char *fieldname,
						int entrynum,
						UT_OptionEntry *&result) const;

    private:
	UT_StringArray				 myFieldNames;
	UT_ValArray<SIM_QueryArrayField *>	 myFields;
    };

    SIM_QueryArrayRecord		*getRecord(const char *recordtype);

    UT_StringArray				 myRecordNames;
    UT_ValArray<SIM_QueryArrayRecord *>		 myRecords;
};

#endif

