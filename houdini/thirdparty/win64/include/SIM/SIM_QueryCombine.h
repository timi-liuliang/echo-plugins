/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_QueryCombine_h__
#define __SIM_QueryCombine_h__

#include "SIM_API.h"
#include "SIM_Query.h"

/// This specialization of SIM_Query allows two other SIM_Query classes to
/// be combined and treated as a single object. This is particularaly useful
/// for SIM_Data classes that use multiple inheritance and want to preserve
/// all the SIM_Query behavior of each base class.
class SIM_API SIM_QueryCombine : public SIM_Query
{
public:
    /// Constructor which takes pointers to the two SIM_Query objects we
    /// want to combine. This object becomes the owner of these other
    /// objects, and will take care of deleting them.
    explicit			 SIM_QueryCombine(SIM_Query *q1, SIM_Query *q2);
    /// Destructor for this class.
    virtual			~SIM_QueryCombine();

protected:
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
    SIM_Query			*myQuery1;
    SIM_Query			*myQuery2;
};

#endif

