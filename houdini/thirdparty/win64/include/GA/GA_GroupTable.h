/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_GroupTable.h (GA Library, C++)
 *
 * COMMENTS:	A table for managing GA_Group objects.
 *
 */

#pragma once

#ifndef __GA_GroupTable_h__
#define __GA_GroupTable_h__

#include "GA_API.h"

#include "GA_Group.h"
#include "GA_Types.h"

#include <UT/UT_String.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_ArrayStringMap.h>
#include <UT/UT_OrderedIterator.h>

#include <SYS/SYS_Types.h>


class GA_Detail;
class GA_LoadMap;
class GA_SaveMap;
class GA_Stat;

class UT_JSONParser;
class UT_JSONWriter;
class UT_MemoryCounter;
class UT_StringArray;
class UT_WorkBuffer;
template <typename T> class UT_Array;

class GA_API GA_GroupTable
{
public:
    typedef UT_ArrayStringMap<GA_Group*> MapType;

    GA_GroupTable(GA_Detail &detail);
    virtual ~GA_GroupTable();

    /// Report memory usage (includes all shared memory)
    virtual int64	getMemoryUsage(bool inclusive) const;

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    /// If inclusive is true, the size of this object is counted,
    /// else only memory owned by this object is counted.
    /// If this is pointed to by the calling object, inclusive should be true.
    /// If this is contained in the calling object, inclusive should be false.
    /// (Its memory was already counted in the size of the calling object.)
    virtual void countMemory(UT_MemoryCounter &counter, bool inclusive) const;

    bool	 isEmpty() const	{ return myEntryMap.empty(); }
    // Number of groups, not including internal groups.
    exint	 getPersistentCount() const;
    // Raw number of groups, including internal groups.
    exint	 entries() const { return myEntryMap.size(); }

    GA_Group	*find(const UT_StringRef &name) const;

    GA_Group	*newGroup(const UT_StringHolder &name);
    GA_Group	*newInternalGroup();

    /// TODO: Deprecate internal groups with user-specified names.
    GA_Group	*newGroup(const UT_StringHolder &name, bool internal);

    // A detached group is not owned by anyone but the caller, so
    // will not track new points and must be deleted by the caller.
    GA_Group	*newDetachedGroup() const;

    bool destroy(GA_Group *group);
    bool destroy(const UT_StringRef &name);

    class GA_API Filter
    {
    public:
	virtual ~Filter() {}

	virtual bool	 match(const GA_Group *group) const = 0;
    };

    GA_Size	 destroyGroups(const Filter &filter);

    void	 clear();

    bool	 renameGroup(const UT_StringRef &fromname, const UT_StringHolder &toname);

    /// Rename a group to an unused name.  Typically used to make fromName
    /// available for another group.
    ///
    /// Returns new name on success, UT_StringHolder() on failure.
    UT_StringHolder renameGroupUnspecified(const UT_StringRef &fromName);

    /// Save array of groups to a JSON stream
    /// @section JSON-GA_GroupTable JSON Schema: GA_GroupTable
    /// Stores an array of groups.
    /// @code
    /// {
    ///     "name"        : "GA_GroupTable",
    ///     "description" : "A list of groups. All items should be same type",
    ///     "type"  : "array",
    ///     "items" : { "$ref" : "GA_Group" },
    /// }
    /// @endcode
    /// @see @ref JSON_FileFormat, GA_Group
    bool	jsonSave(UT_JSONWriter &w, const GA_SaveMap &map,
			    const char *type) const;

    /// Load the groups from the JSON stream
    bool	jsonLoad(UT_JSONParser &p, const GA_LoadMap &map,
			    const char *type_default, GA_AttributeOwner owner);

    /// Returns @c true if there are any groups to be saved out. 
    bool	jsonShouldSave(const GA_SaveMap &map) const;
    
    /// Stat the group from the JSON stream
    static bool	jsonStat(UT_JSONParser &p, GA_Stat &sbuf, GA_GroupType owner);

    /// @{
    /// Get information about groups
    void	stat(UT_StringArray &info, uint level);
    void	stat(GA_Stat &stat, uint level) const;
    /// @}

    /// @note Derived classes are responsible for ensuring that this iterator
    /// template is only instantiated for the appropriate GROUP_TYPE, if any.
    template <typename GROUP_TYPE>
    class iterator
    {
        public:
            SYS_FORCE_INLINE
            iterator() : myIt(), myEnd() {}
            SYS_FORCE_INLINE
            explicit iterator(const MapType::const_iterator &it,const MapType::const_iterator &end)
                : myIt(it)
                , myEnd(end)
            {}

            SYS_FORCE_INLINE
            const UT_StringHolder &name()
            { return myIt->second->getName(); }
            SYS_FORCE_INLINE
            GROUP_TYPE *group()
            { return static_cast<GROUP_TYPE *>(myIt->second); }
            SYS_FORCE_INLINE
            GROUP_TYPE *operator*() { return group(); }

            SYS_FORCE_INLINE
            bool operator==(const iterator &cmp) const
            { return (myIt == cmp.myIt); }
            SYS_FORCE_INLINE
            bool operator!=(const iterator &cmp) const
            { return !(*this == cmp); }
            SYS_FORCE_INLINE
            bool atEnd() const
            { return myIt == myEnd; }

            SYS_FORCE_INLINE
            iterator &operator++()
            {
                ++myIt;
                return *this;
            }
        private:
            MapType::const_iterator myIt;
            MapType::const_iterator myEnd;
    };

    /// Traverse the group table in creation order.
    ///
    /// @note Derived classes will add their own specialized versions
    /// of begin() and end() and call them beginTraverse() and endTraverse().
    /// These were named begin/endTraverse() to avoid any method hiding as
    /// the return types are different.
    GA_GroupTable::iterator<GA_Group> beginTraverse() const
			{ return begin<GA_Group>(); }
    GA_GroupTable::iterator<GA_Group> endTraverse() const
			{ return end<GA_Group>(); }

    /// @{
    /// Traverse the group table in alpabetic order.
    typedef UT_OrderedIterator<GA_Group *, GA_GroupTable::iterator<GA_Group> >
            ordered_iterator;
    static int compareAlpha(GA_Group *const*a, GA_Group *const*b)
    { return compareGroupName(**a, **b); }
    ordered_iterator obegin() const
    {
        GA_GroupTable::iterator<GA_Group> it = beginTraverse();
        return ordered_iterator(it, compareAlpha);
    }
    ordered_iterator oend() const
    { return ordered_iterator(); }
    /// @}

protected:
    /// Convenience function for ordered iteration (in subclasses)
    /// @{
    static int compareGroupName(const GA_Group &a, const GA_Group &b)
    {
	return UT_String::compareNumberedString(a.getName().c_str(), b.getName().c_str());
    }
    /// @}

    template <typename T> iterator<T> begin() const
	    { return iterator<T>(myEntryMap.begin(),myEntryMap.end()); }
    template <typename T> iterator<T> end() const
	    { return iterator<T>(myEntryMap.end(),myEntryMap.end()); }

    /// @note Derived classes are responsible for ensuring that this template
    /// method is only instantiated for the appropriate GROUP_TYPE, if any.
    template <typename GROUP_TYPE>
	void	 getListT(UT_Array<GROUP_TYPE *> &list) const
	{
	    iterator<GROUP_TYPE>		iter;

	    for (iter = begin<GROUP_TYPE>(); !iter.atEnd(); ++iter)
	    {
		GROUP_TYPE *group = iter.group();
		if (group)
		    list.append(group);
	    }
	}

    GA_Detail		&getDetail()		{ return myDetail; }
    const GA_Detail	&getDetail() const	{ return myDetail; }

    /// Method called to create a group during loading.  The sub-class should
    /// verify that the owner matches the type information.
    virtual GA_Group	*jsonCreateGroup(UT_JSONParser &p,
					const char *type,
					const char *name,
					bool internal,
					GA_AttributeOwner owner) = 0;

    virtual GA_Group *virtualCreateGroup(
        GA_Detail &detail,
        const UT_StringHolder &name,
        bool internal) const = 0;
    virtual GA_Group *virtualCreateDetachedGroup(
        const GA_Detail &detail) const = 0;

    // This variation of newGroup does not check to see if the
    // name is already used.
    GA_Group	*newGroupWithNoConflict(const UT_StringHolder &name, bool internal);

    void	 reserveNewSymbolTableSpace(exint n)
		 {
		    if (myEntryMap.size() + n > myEntryMap.bucket_count())
			myEntryMap.reserve(myEntryMap.size() + n);
		 }

    friend class GA_AttributeDict;

    /// Used only by GA_AttributeDict
    const MapType &getMap() const
    {
        return myEntryMap;
    }
private:
    UT_StringRef makeUniqueName(UT_WorkBuffer &name);

    /// Bumps the corresponding attribute set, if it exists, version
    /// parms.  Requires the group as that is the only way to locate
    /// the group type.
    void	 bumpVersionParms(const GA_Group *group);

    GA_Detail		&myDetail;
    MapType		 myEntryMap;
};

#endif
