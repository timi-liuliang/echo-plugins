/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_BreakpointGroup.h (GA Library, C++)
 *
 * COMMENTS: 	GA_BreakpointGroup is a group container for GA_Breakpoint
 *          	entities.
 */

#pragma once

#ifndef __GA_BreakpointGroup_h__
#define __GA_BreakpointGroup_h__

#include "GA_API.h"
#include <UT/UT_LinkList.h>

#include "GA_Breakpoint.h"
#include "GA_Group.h"

#include <iterator>

class GA_Detail;
class GA_Primitive;
class UT_MemoryCounter;

class GA_API GA_BreakpointGroup : public GA_Group
{
public:
     GA_BreakpointGroup(const GA_Detail &gdp,
                        const char *name="");
    virtual ~GA_BreakpointGroup();

    /// Return the owning detail
    virtual const GA_Detail &getDetail() const { return myDetail; }

    /// Report memory usage
    virtual int64 getMemoryUsage(bool inclusive) const;

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    /// If inclusive is true, the size of this object is counted,
    /// else only memory owned by this object is counted.
    /// If this is pointed to by the calling object, inclusive should be true.
    /// If this is contained in the calling object, inclusive should be false.
    /// (Its memory was already counted in the size of the calling object.)
    virtual void countMemory(UT_MemoryCounter &counter, bool inclusive) const;

    class GA_API Entry : public UT_LinkNode
    {
	public:
	    explicit Entry(const GA_Breakpoint &b)
		: myBkpt(b) {}

	    GA_Breakpoint	 myBkpt;
    };

    Entry	*find(const GA_Breakpoint &it) const;
    bool 	 contains(const GA_Breakpoint &it) const;


    // Adds return 1 if successful and 0 otherwise
    bool	 add(const GA_Breakpoint &it);

    bool	 toggle(const GA_Breakpoint &it);

    bool	 removeEntry(Entry *it);
    bool	 remove(const GA_Breakpoint &it);

    UT_LinkList       &list() 		{ return myBreakpointList; }
    const UT_LinkList &list() const	{ return myBreakpointList; } 
    
    /// Query the number of breakpoints from primary primitives in the group.
    virtual GA_Size 	 entries() const
			 { return myBreakpointList.length() - myNumSecondary; }
    /// Query whether the group does not contain any breakpoints from primary
    /// primitives.
    bool isEmpty() const
    { return myBreakpointList.length() <= myNumSecondary; }
    /// Query the total number of breakpoints (from both primary and secondary
    /// primitives) in the group.
    GA_Size 	 	 entriesMix() const {return myBreakpointList.length();}
    /// Query whether the group does not contain any breakpoints.
    virtual bool	 isEmptyMix() const { return myBreakpointList.isEmpty(); }

    /// Query if we contain any breakpoints from secondary primitives.
    virtual bool	 isMixed() const { return myNumSecondary > 0; }

    virtual bool	 isOrdered() const { return false; }

    virtual void	 clear();
    virtual void	 addAll();

    /// Save data to a JSON stream.
    /// @section JSON-GA_BreakpointGroup JSON Schema: GA_BreakpointGroup
    /// @code
    /// {
    ///     "name"          : "GA_BreakpointGroup",
    ///     "description"   : "A list of breakpoints",
    ///     "type"          : "orderedmap",
    ///     "properties": {
    ///         "breakpoints": {
    ///             "type"   : "array",
    ///             "items"  : { "$ref", "GA_Breakpoint" },
    ///             "description" : "Array of breakpoints in the group",
    ///         }
    ///     },
    /// }
    /// @endcode
    /// @see @ref JSON_FileFormat, GA_Group
    virtual bool	jsonSaveData(UT_JSONWriter &w,
					const GA_SaveMap &map) const;

    /// Load breakpoint group from a JSON stream.
    virtual bool	jsonLoadData(UT_JSONParser &w, const GA_LoadMap &map);

    /// Iterator to iterate over all breakpoints in the group.  It is safe to
    /// call advance() after deleting the current entry.
    
    GA_BreakpointGroup	&operator|=(const GA_BreakpointGroup &inputGroup);
    GA_BreakpointGroup	&operator&=(const GA_BreakpointGroup &inputGroup);
    GA_BreakpointGroup	&operator-=(const GA_BreakpointGroup &inputGroup);
    GA_BreakpointGroup	&operator+=(const GA_BreakpointGroup &inputGroup);
    GA_BreakpointGroup	&operator^=(const GA_BreakpointGroup &inputGroup);
    GA_BreakpointGroup	&operator =(const GA_BreakpointGroup &inputGroup);
    
protected:
    template<typename T>
    class base_iterator :
	public std::iterator<std::input_iterator_tag, T>
    {
    public:
	typedef T &reference;

	/// Default constructor
	base_iterator() :
	    myGroup(NULL),
	    myCurr(NULL),
	    myNext(NULL)
	{
	}
	/// Copy constructor. Use a separate template type to allow copying
	/// from a const_iterator to non-const iterator.
	template<typename ET>
	base_iterator(const base_iterator<ET> &src)
	{
	    myGroup = src.myGroup;
	    myCurr = src.myCurr;
	    myNext = src.myNext;
	}
	/// Assignment operator
	const base_iterator &operator=(const base_iterator &src)
		{
		    myGroup = src.myGroup;
		    myCurr = src.myCurr;
		    myNext = src.myNext;
		    return *this;
		}
	bool	 operator==(const base_iterator &cmp) const
		     { return myCurr == cmp.myCurr; }

	bool	 operator!=(const base_iterator &cmp) const
		     { return myCurr != cmp.myCurr; }

	/// ++iterator
	base_iterator &operator++()	{ advance(); return *this; }
	// No post inc as it is harmful.

	bool	atEnd() const	{ return !myCurr; }
	void	advance()
		    {
			myCurr = myNext;
			if (myNext)
			    myNext = static_cast<Entry *>(myNext->next());
		    }
	void	rewind()
		    {
			myNext = static_cast<Entry *>(
					myGroup->myBreakpointList.head());
			advance();
		    }

	reference 	 operator*() const { return myCurr->myBkpt; }
	reference	 getBreakpoint() const { return myCurr->myBkpt; }
	Entry		*getEntry()	{ return myCurr; }
    private:
	base_iterator(const GA_BreakpointGroup *group)
	    : myGroup(group),
	      myCurr(static_cast<Entry *>(group->myBreakpointList.head()))
	{
	    myNext = myCurr ? static_cast<Entry *>(myCurr->next()) : 0;
	}
	const GA_BreakpointGroup	*myGroup;
	Entry			*myCurr, *myNext;

	friend class	GA_BreakpointGroup;
    };
public:
    typedef base_iterator<GA_Breakpoint> iterator;
    typedef base_iterator<const GA_Breakpoint> const_iterator;

    iterator	 	 begin() 	{ return iterator(this); }
    iterator	 	 end() 		{ return iterator(); }
    const_iterator	 begin() const	{ return const_iterator(this); }
    const_iterator	 end() const	{ return const_iterator(); }

private:
    bool	 addEntry(Entry *it);

    const GA_Detail		&myDetail;
    UT_LinkList			 myBreakpointList;
    unsigned			 myNumSecondary;	// number of secondary
							// breakpoints
};


#endif
