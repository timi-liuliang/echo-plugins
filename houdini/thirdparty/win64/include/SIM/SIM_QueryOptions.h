/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_QueryOptions_h__
#define __SIM_QueryOptions_h__

#include "SIM_API.h"
#include "SIM_Query.h"

class SIM_Options;

/// This class provides an interface between a SIM_Data and the Houdini
/// expression language. It also helps in building the tree view of a
/// simulation.
class SIM_API SIM_QueryOptions : public SIM_Query
{
public:
    /// Constructor which takes a const SIM_Data as our owner.
    explicit			 SIM_QueryOptions(const SIM_Data *owner,
						  const SIM_Options *options);
    /// Destructor for this class.
    virtual			~SIM_QueryOptions();

protected:
    const SIM_Options		&getOptions() const;

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
    void			 buildNameList();

    const SIM_Options		*myOptions;
    UT_ValArray<const char *>	 myNames;
    UT_Array<UT_OptionType>	 myTypes;
};

#endif

