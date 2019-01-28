/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_ThingList.h ( UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_ThingList__
#define __UT_ThingList__

#include "UT_API.h"
#include <stdio.h>
#include "UT_Defines.h"
#include "UT_Thing.h"
#include "UT_Hash.h"
#include "UT_SmallObject.h"

#include <SYS/SYS_Deprecated.h>

SYS_DEPRECATED_PUSH_DISABLE()

class UT_API SYS_DEPRECATED(16.0) UT_ThingEntry
    : public UT_SmallObject<UT_ThingEntry,
			    UT_SMALLOBJECT_CLEANPAGES_DEFAULT,
			    UT_SMALLOBJECT_PAGESIZE_DEFAULT,
			    UT_SMALLOBJECT_THREADSAFE_DEFAULT>
{
public:
    UT_Thing		 data;
    UT_Hash		*name;
    UT_ThingEntry	*next;
};

class SYS_DEPRECATED_REPLACE(16.0, "std::list") UT_API UT_ThingList {
public:
     UT_ThingList();
    ~UT_ThingList();

    // if 'ordered' is true, you must define UT_Hash::strcmp properly,
    // to return -1, 0 and 1 based on <, = or >; not just 0 or 1. 
    int 	 insertEntry(const UT_Hash &name, const UT_Thing & value,
			     bool ordered = false);
    int		   findEntry(const UT_Hash &name, UT_Thing *valuep,
			     bool ordered = false) const;
    int		   findEntry(const UT_Hash &name, UT_Thing *valuep,
			     const UT_Hash **namep, bool ordered = false) const;
    UT_Thing	   &getEntry(const UT_Hash &name, int *created = 0,
			     bool ordered = false);
    int 	 deleteEntry(const UT_Hash &name,
			     bool ordered = false);

    const UT_Hash & getHashReference(const UT_Hash &name, int *created = 0,
				     bool ordered = false);

    int		 count() const;
    int		 traverse(int (*function)(UT_Thing &, const UT_Hash &, void *),
			  void *data) const;

    unsigned	 entries(void) const;
    unsigned	 empty(void) const;	// Return true if empty

    int64	 getMemoryUsage(bool inclusive) const;


    // These methods are called by UT_HashTable when rebuilding the hash table.
    UT_ThingEntry	*removeFirstThingEntry();
    // This function removes a UT_ThingEntry from the list and returns it
    // so it can be modified and re-inserted to this list or another.
    UT_ThingEntry	*removeFirstThingEntry(const UT_Hash &name,
					       bool ordered);
    // Inserts an entry into this list.
    void		 insertThingEntry(UT_ThingEntry &thing_entry,
					  bool ordered);

    class UT_API traverser
    {
	public:
	    traverser()
		: myCurrEntry(0) { }
	    traverser(const traverser &src)
			 { *this = src; }
	    ~traverser() { }

	    UT_Thing	&thing() const { return myCurrEntry->data; }
	    UT_Hash	*hash() const { return myCurrEntry->name; }

	    int		 operator==(const traverser &cmp) const
			 {  return (myCurrEntry == cmp.myCurrEntry); }
	    int		 operator!=(const traverser &cmp) const
			 { return !(*this == cmp); }

	    traverser	&operator++()
			 {
			     myCurrEntry = myCurrEntry->next;
			     return *this;
			 }

	    const traverser 	&operator=(const traverser &src)
			 {
			     myCurrEntry = src.myCurrEntry;
			     return *this;
			 }

	private:
	    traverser(UT_ThingEntry *sym)
		: myCurrEntry(sym) { }

	    UT_ThingEntry	*myCurrEntry;

	    friend class UT_ThingList;
    };

    traverser	 begin() const { return traverser(symbols); }
    traverser	 end() const { return traverser(); }
 
private:
    UT_ThingEntry	*findOrCreate(const UT_Hash &name, int *created,
				      bool ordered);
    UT_ThingEntry	*symbols;
};

SYS_DEPRECATED_POP_DISABLE()

#endif
