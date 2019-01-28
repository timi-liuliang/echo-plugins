/*
* PROPRIETARY INFORMATION.  This software is proprietary to
* Side Effects Software Inc., and is not to be reproduced,
* transmitted, or disclosed in any way without written permission.
*
* NAME:        UT_NameManager.h
*
* COMMENTS:
*
*	This class implements a templated name manager, which aims to
*	provide fast unique name management functions. Note that it
*	is templated, and thus can be used with any data type.
*
*/

#ifndef __UT_NameManager_h__
#define __UT_NameManager_h__

#include "UT_API.h"
#include "UT_ArrayStringMap.h"
#include "UT_String.h"
#include "UT_StringHolder.h"
#include "UT_WorkBuffer.h"

/*****************************************************************************/
// Faster version and simplier of the previous name manager.
// Based on profiling run the other version had a few problems.
//  Doing lookups was slow because of some operator new calls during the lookups.
//  Doing a lookup always had to split the string to get the base string.
//  The multi map behavior was really only required for generating a unique name in a fast way.
// Now the hash() function isn't be called everytime because UT_StringHolder keeps a hash.
// It means if the string is kept in memory and doesn't change, only the cached hash will be used.
template < class DATA_TYPE >
class UT_NameManager
{
public:

    /// suffix_start_number determines the starting numerical suffix when unique names are generated.
    /// For example:
    /// If set to 0, generating unique names for "base" will result in  "base0", "base1", "base2", etc.
    /// If set to 1, generating unique names for "base" will result in  "base", "base1", "base2", etc.
    /// If set to 2, generating unique names for "base" will result in  "base", "base2", "base3", etc.
    /// And so on.
    UT_NameManager(int suffix_start_number = 2)
    {
	mySuffixStartNumber = suffix_start_number;
	myNumEntries = 0;
    }
    ~UT_NameManager() {}

    /// Returns the amount of memory owned by this UT_NameManger
    int64	 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;

        mem += myMap.getMemoryUsage(false);

        return mem;
    }

    /// Returns the total number of unique names in the manager.
    unsigned	 entries() const { return (unsigned) myNumEntries; }

    /// Returns true if empty, false otherwise.
    bool	 empty() const { return myNumEntries == 0; }

    /// Removes a name, such as "base123", from the manager. If the removed name 
    /// was the last one in a category and the category is now empty, deletes 
    /// the category as well.
    /// Returns true if removal was successfull, false otherwise.
    bool	 deleteSymbol(const UT_StringRef &symbol, bool set_to_null )
    {
	if(!symbol.isstring())
	    return false;

	auto it = myMap.find(symbol);
	if( it==myMap.end() )
	    return false;

	if( set_to_null )
	    it->second = nullptr;
	else
	    myMap.erase(it);

	myNumEntries--;
	UT_ASSERT(myNumEntries>=0);

	return true;
    }

    /// Clears all the entries from this manager.
    ///  if set_to_null is true the map entries values will be set to null instead
    ///  of erasing all the entries.
    void clear(bool set_to_null)
    {
	if( set_to_null )
	{
	    for( auto it = myMap.begin(); !it.atEnd(); ++it )
		it->second = nullptr;
	}
	else
	    myMap.clear();

	myNumEntries = 0;
    }

    /// Generates a new name unique relative to the items alraedy in this collection, 
    ///	optionally starting with a given name.
    bool getUniqueName(const UT_StringRef &orig_name_in, UT_WorkBuffer& unique_name_out) const
    {
	unsigned i=mySuffixStartNumber;
	char number[UT_NUMBUF];

	UT_String temp_name(orig_name_in.isstring() ? orig_name_in : "name", false);
	UT_String base;
	temp_name.base(base);

	unique_name_out.strcpy(base);
	int base_len = unique_name_out.length();

	// Get the first available entry, we could add a multimap later on if this is too slow
	// The old name manager was really slow on the findSymbol which is called more often than this.
	for(;;)
	{
	    UT_String::itoa(number, i++);
	    unique_name_out.append(number);
	    UT_StringRef unique_name_ref(unique_name_out.buffer() );
	    if( !findSymbol( unique_name_ref ) )
		return true;

	    // get back to previous string keeping the same buffer
	    unique_name_out.truncate(base_len);
	}

	UT_ASSERT(false);// Shouldn't get here.
	return false;
    }

    /// Adds a new name to this collection. If the name already exists, 
    /// the addition will silently fail, so it is the caller's responsibility
    /// to assure that the name is unique by calling findSymbol() if
    /// needed. This is done to avoid extra uniqueness checks that may
    /// be unnecessary in certain situations.
    ///
    /// Returns true if symbol added successfully, false otherwise.
    bool	 addSymbol(const UT_StringRef &symbol, DATA_TYPE data)
    {
	if(!symbol.isstring())
	    return false;

	auto it = myMap.find(symbol);
	if( it!=myMap.end() )
	{
	    if( it->second == nullptr )
	    {
		it->second = data;
		myNumEntries++;
		return true;
	    }
	}
	else
	{
            myMap.emplace(symbol, data);
	    myNumEntries++;
	    return true;
	}

	return false;
    }

    /// Finds the data item associated with a given name. Returns a pointer
    /// to the item if found, nullptr if the name is not in the collection.
    DATA_TYPE	 findSymbol(const UT_StringRef &symbol) const
    {
	if(!symbol.isstring())
	    return nullptr;

	auto it = myMap.find(symbol);
	if( it==myMap.end() )
	    return nullptr;

	return it->second;

    }

private:

    UT_ArrayStringMap<DATA_TYPE> myMap;
    int mySuffixStartNumber;
    unsigned myNumEntries;

};
/*****************************************************************************/
#endif
