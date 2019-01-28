/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_Query_h__
#define __SIM_Query_h__

#include "SIM_API.h"
#include <UT/UT_Options.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_Array.h>

class UT_StringArray;
class UT_InfoTree;
class SIM_Data;
class SIM_QuerySuffixInfo;

/// This class provides an interface between a SIM_Data and the Houdini
/// expression language. It also helps in building the tree view of a
/// simulation.
class SIM_API SIM_Query
{
public:
    /// Simple constructor.
    explicit			 SIM_Query();
    /// Constructor which takes a const SIM_Data as our owner.
    explicit			 SIM_Query(const SIM_Data *owner);
    /// Destructor for this class.
    virtual			~SIM_Query();

    /// This function returns true if this object needs to be deleted when
    /// the owner data changes. This may be the case if this class caches
    /// some information about the SIM_Data owner. This default implementation
    /// returns false.
    bool			 deleteIfDataModified() const;

    int				 getNumRecordTypes() const;
    const char			*getRecordTypeName(int recordtypenum) const;

    int				 getNumRecords(const char *recordtype) const;
    int				 getNumFields(const char *recordtype) const;
    const char			*getFieldName(const char *recordtype,
					int fieldnum) const;
    UT_OptionType		 getFieldType(const char *recordtype,
					int fieldnum) const;
    UT_OptionType		 getFieldTypeByName(const char *recordtype,
					const char *fieldname) const;

    /// Sets the result pointer to a UT_OptionEntry subclass that
    /// depends on the data type of the field. It may be any of the
    /// UT_OptionEntryImpl classes. It is up to the caller to free
    /// this result pointer.
    bool			 getFieldRaw(const char *recordtype,
					int recordnum,
					const char *fieldname,
					UT_OptionEntry *&result) const;
    /// These functions use getFieldRawand process the result to
    /// generate a nice string or single float value as required
    /// by the expression library.
    bool			 getFieldString(const char *recordtype,
					int recordnum,
					const char *fieldname,
					UT_String &result) const;
    bool			 getFieldString(const char *recordtype,
					int recordnum,
					const char *fieldname,
					UT_StringHolder &result) const;
    bool			 getFieldFloat(const char *recordtype,
					int recordnum,
					const char *fieldname,
					fpreal &result) const;

    /// Fills the provided array with the names of all valid values that
    /// can be passed to the getFieldFloat function. This includes all
    /// versions of the field names with appropriate suffices.
    void			 getAllFieldFloatNames(
					const char *recordtype,
					UT_StringArray &names,
					UT_Array<UT_OptionType> &fieldtypes,
					UT_IntArray &vectorindices) const;

    /// Fills a UT_InfoTree structure with all our data.
    void			 getInfoTree(UT_InfoTree &infotree,
					     const char *recordtype) const;

protected:
    const SIM_Data		*getOwner() const;

    virtual bool		 deleteIfDataModifiedSubclass() const;

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
    static const SIM_QuerySuffixInfo 	*getSuffixInfos(UT_OptionType otype);
    static bool				 isPossibleFieldSuffix(
					    char secondlastchar,
					    char lastchar);
    static int				 getFieldSuffixVectorIndex(
					    UT_OptionType fieldtype,
					    char secondlastchar,
					    char lastchar);

    const SIM_Data			*myOwner;

    /// Give SIM_QueryCombine special access to our protected methods.
    friend class			 SIM_QueryCombine;
};

#endif

