/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_DataFilter_h__
#define __SIM_DataFilter_h__

#include "SIM_API.h"
#include <UT/UT_Guid.h>
#include <UT/UT_String.h>
#include <UT/UT_WorkArgs.h>
#include <UT/UT_DMatrix4.h>

class SIM_Data;

/// This class is used to filter SIM_Data. This class or its subclasses are
/// used in the SIM_Data::filterSubData(), SIM_Data::filterConstSubData(),
/// SIM_Data::getFirstSubData(), SIM_Data::getFirstConstSubData(),
/// SIM_Data::forEachSubData(), and SIM_Data::forEachConstSubData()
/// functions.
class SIM_API SIM_DataFilter
{
public:
    explicit		 SIM_DataFilter()
			 { }
    virtual		~SIM_DataFilter()
			 { }
			 SIM_DataFilter(const SIM_DataFilter &)
			 { }

    /// Override this function to decide what data is accepted and what
    /// is rejected. The name parameter is the path of the data relative
    /// to the root of the search that is calling this filter.
    virtual bool	 acceptData(const SIM_Data *data,
				    const char *dataname) const = 0;

private:
    /// Disable the assignment operator so subclasses don't need to worry
    /// about implementing it.
    const SIM_DataFilter	&operator=(const SIM_DataFilter &)
				 { return *this; }
};

/// This filter accepts any data.
class SIM_API SIM_DataFilterAll : public SIM_DataFilter
{
public:
    explicit		 SIM_DataFilterAll()
			 { }
    virtual		~SIM_DataFilterAll()
			 { }
			 SIM_DataFilterAll(const SIM_DataFilterAll &)
			    : SIM_DataFilter()
			 { }

    virtual bool	 acceptData(const SIM_Data *,
				    const char *) const
			 { return true; }
};

/// This filter rejects all data.
class SIM_API SIM_DataFilterNone : public SIM_DataFilter
{
public:
    explicit		 SIM_DataFilterNone()
			 { }
    virtual		~SIM_DataFilterNone()
			 { }
			 SIM_DataFilterNone(const SIM_DataFilterNone &)
			    : SIM_DataFilter()
			 { }

    virtual bool	 acceptData(const SIM_Data *,
				    const char *) const
			 { return false; }
};

/// This implementation filters data based on its type. Any data that can
/// be cast to the data type specified in the constructor is accepted.
class SIM_API SIM_DataFilterByType : public SIM_DataFilter
{
public:
    explicit		 SIM_DataFilterByType(const char *datatype)
			    : myDataType(datatype, 1)
			 { }
    virtual		~SIM_DataFilterByType()
			 { }
			 SIM_DataFilterByType(const SIM_DataFilterByType &src)
			    : SIM_DataFilter(),
			      myDataType(src.myDataType, 1)
			 { }

    virtual bool	 acceptData(const SIM_Data *data,
				    const char *dataname) const;

private:
    const UT_String	 myDataType;
};

/// This implementation filters data based on its name. Any data that
/// matches the pattern specified in the constructor is accepted.
/// Pattern matching is case sensitive.
class SIM_API SIM_DataFilterByName : public SIM_DataFilter
{
public:
    explicit		 SIM_DataFilterByName(const char *pattern)
			    : myPattern(pattern, 1)
			 { }
    virtual		~SIM_DataFilterByName()
			 { }
			 SIM_DataFilterByName(const SIM_DataFilterByName &src)
			    : SIM_DataFilter(),
			      myPattern(src.myPattern, 1)
			 { }

    virtual bool	 acceptData(const SIM_Data *data,
				    const char *dataname) const;

private:
    const UT_String	 myPattern;
};

/// This implementation filters data based on its unique id. Only data with
/// the specified unique id will be accepted.
class SIM_API SIM_DataFilterByUniqueId : public SIM_DataFilter
{
public:
    explicit		 SIM_DataFilterByUniqueId(const UT_Guid &uniqueid)
			    : myUniqueId(uniqueid)
			 { }
    virtual		~SIM_DataFilterByUniqueId()
			 { }
			 SIM_DataFilterByUniqueId(
				 const SIM_DataFilterByUniqueId &src)
			    : SIM_DataFilter(),
			      myUniqueId(src.myUniqueId)
			 { }

    virtual bool	 acceptData(const SIM_Data *data,
				    const char *dataname) const;

private:
    const UT_Guid	 myUniqueId;
};

/// This implementation accepts the Nth data accepted by some other filter.
/// The nth time data is accepted by this secondary filter, this filter
/// accepts the data. All subsequent data is immediately rejected.
///
/// Note: in typical C++ fashion, n starts at zero.
class SIM_API SIM_DataFilterNth : public SIM_DataFilter
{
public:
    explicit		 SIM_DataFilterNth(const SIM_DataFilter &filter, int n)
			    : myFilter(filter),
			      myN(n),
			      myCount(0)
			 { }
    virtual		~SIM_DataFilterNth()
			 { }
			 SIM_DataFilterNth(const SIM_DataFilterNth &src)
			    : SIM_DataFilter(),
			      myFilter(src.myFilter),
			      myCount(src.myCount),
			      myN(src.myN)
			 { }

    virtual bool	 acceptData(const SIM_Data *data,
				    const char *dataname) const
			 {
			     if( myCount <= myN &&
				 myFilter.acceptData(data, dataname) )
			     {
				 ++myCount;
				 return myCount == myN + 1;
			     }
			     return false;
			 }

private:
    const SIM_DataFilter    &myFilter;
    mutable int		     myCount;
    const int		     myN;
};

/// This implementation returns the union of two other filters.
/// If either one accepts the data, then this filter accepts the data.
class SIM_API SIM_DataFilterOr : public SIM_DataFilter
{
public:
    explicit		 SIM_DataFilterOr(const SIM_DataFilter &filter1,
					  const SIM_DataFilter &filter2)
			    : myFilter1(filter1),
			      myFilter2(filter2)
			 { }
    virtual		~SIM_DataFilterOr()
			 { }
			 SIM_DataFilterOr(const SIM_DataFilterOr &src)
			    : SIM_DataFilter(),
			      myFilter1(src.myFilter1),
			      myFilter2(src.myFilter2)
			 { }

    virtual bool	 acceptData(const SIM_Data *data,
				    const char *dataname) const
			 {
			     if( myFilter1.acceptData(data, dataname) ||
				 myFilter2.acceptData(data, dataname) )
				 return true;
			     return false;
			 }

private:
    const SIM_DataFilter	&myFilter1;
    const SIM_DataFilter	&myFilter2;
};

/// This implementation returns the intersection of two other filters.
/// If both accept the data, then this filter accepts the data.
class SIM_API SIM_DataFilterAnd : public SIM_DataFilter
{
public:
    explicit		 SIM_DataFilterAnd(const SIM_DataFilter &filter1,
					   const SIM_DataFilter &filter2)
			    : myFilter1(filter1),
			      myFilter2(filter2)
			 { }
    virtual		~SIM_DataFilterAnd()
			 { }
			 SIM_DataFilterAnd(const SIM_DataFilterAnd &src)
			    : SIM_DataFilter(),
			      myFilter1(src.myFilter1),
			      myFilter2(src.myFilter2)
			 { }

    virtual bool	 acceptData(const SIM_Data *data,
				    const char *dataname) const
			 {
			     if( myFilter1.acceptData(data, dataname) &&
				 myFilter2.acceptData(data, dataname) )
				 return true;
			     return false;
			 }

private:
    const SIM_DataFilter	&myFilter1;
    const SIM_DataFilter	&myFilter2;
};

/// This implementation returns the opposite value of some other filter.
/// If the secondary filter rejects the data, this filter accepts it.
class SIM_API SIM_DataFilterNot : public SIM_DataFilter
{
public:
    explicit		 SIM_DataFilterNot(const SIM_DataFilter &filter)
			    : myFilter(filter)
			 { }
    virtual		~SIM_DataFilterNot()
			 { }
			 SIM_DataFilterNot(const SIM_DataFilterNot &src)
			    : SIM_DataFilter(),
			      myFilter(src.myFilter)
			 { }

    virtual bool	 acceptData(const SIM_Data *data,
				    const char *dataname) const
			 {
			     if( !myFilter.acceptData(data, dataname) )
				 return true;
			     return false;
			 }

private:
    const SIM_DataFilter	&myFilter;
};

/// This class is specific to filtering SIM_RootData and its subclasses,
/// SIM_Relationship and SIM_Object.
class SIM_API SIM_DataFilterRootData : public SIM_DataFilter
{
public:
    explicit		 SIM_DataFilterRootData(const char *filterstr)
			    : myFilterStr(filterstr, 1)
			 {
			     myFilterStr.tokenize(myFilterArgs);
			 }
    virtual		~SIM_DataFilterRootData()
			 { }
			 SIM_DataFilterRootData(
				const SIM_DataFilterRootData &src)
			    : SIM_DataFilter(),
			      myFilterStr(src.myFilterStr),
			      myFilterArgs(src.myFilterArgs)
			 { }

    /// Compares the rootdata to the string passed to the constructor. An
    /// rootdata is accepted if its name or id appears in the string, or if
    /// the string contains the name of a group to which the rootdata belongs.
    virtual bool         acceptData(const SIM_Data *data,
				    const char *dataname) const;

private:
    UT_String		 myFilterStr;
    UT_WorkArgs		 myFilterArgs;
};

#endif

