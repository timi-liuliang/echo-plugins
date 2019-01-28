/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_AttributeDict.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#pragma once

#ifndef __GA_AttributeDict__
#define __GA_AttributeDict__

#include "GA_API.h"
#include "GA_Attribute.h"
#include "GA_AttributeProxy.h"
#include "GA_ElementGroup.h"
#include "GA_Types.h"

#include <UT/UT_Assert.h>
#include <UT/UT_OrderedIterator.h>
#include <UT/UT_String.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_ArrayStringMap.h>

#include <SYS/SYS_Types.h>

#include <stddef.h>
#include <string.h>
#include <utility>

class GA_AttributeSet;
class GA_Group;
class UT_MemoryCounter;

/// @brief A string map of attributes to ease backward compatibility
/// In the GB/GEO/GU library code would often pass around a GB_AttributeDict
/// around.  The GA library really only requires a string map of attributes
/// since the attributes are managed by the GA_AttributeSet.
class GA_API GA_AttributeDict
{
public:
    typedef UT_ArrayStringMap<GA_AttributeProxy*> MapType;
    typedef MapType::const_iterator const_map_iterator;
    typedef MapType::iterator map_iterator;

     GA_AttributeDict() { myVersionParms = 0; }
    ~GA_AttributeDict() {}

    /// @{
    /// Get access to the containing attribute set.
    const GA_AttributeSet	&getSet() const	{ return *mySet; }
    GA_AttributeSet		&getSet()	{ return *mySet; }
    /// @}

    /// Report memory usage
    int64		getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += myTable.getMemoryUsage(false);
        for (auto it = myTable.begin(); !it.atEnd(); ++it)
            mem += it->second->getMemoryUsage(true);
        mem += myPrivateTable.getMemoryUsage(false);
        for (auto it = myPrivateTable.begin(); !it.atEnd(); ++it)
            mem += it->second->getMemoryUsage(true);
        return mem;
    }

    /// Report a cache-count.  This is bumped when attributes are
    /// created/destroyed (and hence renamed)
    /// Only unique to the GA_Detail that owns it, so combine
    /// with the GA_Detail::getUniqueId()
    /// (Name comes from OP_Node::getVersionParms which has a similar
    /// meaning)
    exint		getVersionParms() const { return myVersionParms; }

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    /// If inclusive is true, the size of this object is counted,
    /// else only memory owned by this object is counted.
    /// If this is pointed to by the calling object, inclusive should be true.
    /// If this is contained in the calling object, inclusive should be false.
    /// (Its memory was already counted in the size of the calling object.)
    void		countMemory(UT_MemoryCounter &counter, bool inclusive) const;

    GA_Attribute *find(GA_AttributeScope scope, const UT_StringRef &name) const;
    GA_AttributeProxy *findProxy(GA_AttributeScope scope, const UT_StringRef &name) const;

    SYS_FORCE_INLINE
    void clear()
    {
        bumpVersionParms();
        myTable.clear();
        myPrivateTable.clear();
        // Group table is cleared elsewhere.
    }

    SYS_FORCE_INLINE
    exint entries() const
    {
        return myTable.size() + myPrivateTable.size() + numGroups();
    }
    exint entries(GA_AttributeScope scope) const
    {
        if (scope == GA_SCOPE_PUBLIC)
            return myTable.size();
        if (scope == GA_SCOPE_PRIVATE)
            return myPrivateTable.size();
        if (scope == GA_SCOPE_GROUP)
            return numGroups();
        return entries();
    }
    bool	empty() const
    {
        return myTable.empty() && myPrivateTable.empty() && (numGroups()==0);
    }
    bool	empty(GA_AttributeScope scope) const
		    { return entries(scope) == 0; }

    class GA_API iterator
    {
    public:
	 iterator()
	     : myIterator()
	     , myDict(NULL)
	     , myScope(GA_SCOPE_INVALID)
             , myCurrentScope(GA_SCOPE_INVALID)
	 {}

	 iterator(const iterator &src) { *this = src; }
	~iterator() {}

	const char *name() const
			{
			     const GA_Attribute *atr =
				 proxy()->getAttribute();
			     return atr ? atr->getName().c_str() : nullptr;
			}
	GA_AttributeProxy *proxy() const
			{
                            if (myCurrentScope != GA_SCOPE_GROUP)
                            {
                                return myIterator->second;
                            }
                            return UTverify_cast<GA_ElementGroup*>(myGroupIterator->second)->getProxy().get();
			}
	GA_Attribute	*attrib() const
			{
			    return proxy()->getAttribute();
			}
	GA_Attribute	*operator*() const
			{
			    return attrib();
			}
	bool operator==(const iterator &cmp) const
        {
            UT_ASSERT_P(myDict == cmp.myDict);
            UT_ASSERT_P(myScope == cmp.myScope);
            return myIterator.getCurrent() == cmp.myIterator.getCurrent() || (myIterator.atEnd() && cmp.myIterator.atEnd());
        }
	bool operator!=(const iterator &cmp) const
        {
            UT_ASSERT_P(myDict == cmp.myDict);
            UT_ASSERT_P(myScope == cmp.myScope);
            return myIterator.getCurrent() != cmp.myIterator.getCurrent() && !(myIterator.atEnd() && cmp.myIterator.atEnd());
        }
	bool		atEnd() const
			{
                            return myIterator.atEnd();
			}
	iterator	&operator++()
			{
                            UT_ASSERT_P(!myIterator.atEnd());
			    ++myIterator;
                            // NOTE: This makes use of that myIterator and myGroupIterator
                            //       will have identical memory layouts.
                            if (myScope == GA_SCOPE_INVALID && myIterator.atEnd())
                            {
                                if (myCurrentScope == GA_SCOPE_PUBLIC)
                                {
                                    myIterator = myDict->myPrivateTable.begin();
                                    myCurrentScope = GA_SCOPE_PRIVATE;
                                    if (!myIterator.atEnd())
                                        return *this;
                                }
                                if (myCurrentScope == GA_SCOPE_PRIVATE)
                                {
                                    const UT_ArrayStringMap<GA_Group *> *grouptable = myDict->getGroupTable();
                                    if (grouptable)
                                    {
                                        myGroupIterator = grouptable->begin();
                                        myCurrentScope = GA_SCOPE_GROUP;
                                    }
                                }
                            }
                            return *this;
			}
	const iterator	&operator=(const iterator &src)
			{
			    myIterator = src.myIterator;
			    myDict = src.myDict;
			    myScope = src.myScope;
                            myCurrentScope = src.myCurrentScope;
			    return *this;
			}
    private:
	iterator(const GA_AttributeDict *dict,
		GA_AttributeScope scope,
		bool end)
            : myDict(dict)
            , myScope(scope)
            , myCurrentScope(scope)
        {
            if (scope == GA_SCOPE_PUBLIC)
            {
                myIterator = end ? dict->myTable.end() : dict->myTable.begin();
            }
            else if (scope == GA_SCOPE_INVALID)
            {
                if (!end)
                {
                    myIterator = dict->myTable.begin();
                    if (!myIterator.atEnd())
                    {
                        myCurrentScope = GA_SCOPE_PUBLIC;
                        return;
                    }
                    myIterator = dict->myPrivateTable.begin();
                    if (!myIterator.atEnd())
                    {
                        myCurrentScope = GA_SCOPE_PRIVATE;
                        return;
                    }
                    const UT_ArrayStringMap<GA_Group *> *grouptable = dict->getGroupTable();
                    if (grouptable)
                    {
                        myGroupIterator = grouptable->begin();
                        myCurrentScope = GA_SCOPE_GROUP;
                    }
                    else
                        myCurrentScope = GA_SCOPE_PRIVATE;
                    return;
                }
                const UT_ArrayStringMap<GA_Group *> *grouptable = dict->getGroupTable();
                if (grouptable)
                {
                    myGroupIterator = grouptable->end();
                    myCurrentScope = GA_SCOPE_GROUP;
                }
                else
                {
                    myIterator = dict->myPrivateTable.end();
                    myCurrentScope = GA_SCOPE_PRIVATE;
                }
            }
            else if (scope == GA_SCOPE_GROUP)
            {
                const UT_ArrayStringMap<GA_Group *> *grouptable = dict->getGroupTable();
                if (grouptable)
                    myGroupIterator = end ? grouptable->end() : grouptable->begin();
                else
                    myIterator = dict->myPrivateTable.end();
            }
            else
            {
                UT_ASSERT_P(scope == GA_SCOPE_PRIVATE);
                myIterator = end ? dict->myPrivateTable.end() : dict->myPrivateTable.begin();
            }
        }
        union {
            GA_AttributeDict::const_map_iterator myIterator;
            UT_ArrayStringMap<GA_Group *>::const_iterator myGroupIterator;
        };
	const GA_AttributeDict        *myDict;
	GA_AttributeScope              myScope;
        GA_AttributeScope              myCurrentScope;
	friend class GA_AttributeDict;
    };

    class GA_API Range
    {
    public:
	Range()
	    : myBegin()
	    , myEnd()
	{
	}

	Range(const GA_AttributeDict& dict, GA_AttributeScope scope)
	    : myBegin(dict.begin(scope))
	    , myEnd(dict.end(scope))
	{
	}

	typedef GA_AttributeDict::iterator iterator;
	typedef GA_AttributeDict::iterator const_iterator;

	iterator    begin() const   { return myBegin; }
	iterator    end() const	    { return myEnd; }

    private:
	iterator    myBegin;
	iterator    myEnd;
    };

    Range	range(GA_AttributeScope scope) const
		    { return Range(*this, scope); }

    static int	compareAlpha(GA_Attribute *const*a, GA_Attribute *const*b)
		{
		    const char *aname = (*a)->getName();
		    const char *bname = (*b)->getName();
		    if (!strcmp(aname, "P"))
			return -1;
		    if (!strcmp(bname, "P"))
			return 1;

		    // Not case-sensitive
		    return UT_String::compareNumberedString(aname,bname,false);
		}

    /// Iterator which traverses attributes in given order
    typedef UT_OrderedIterator< GA_Attribute *, iterator > ordered_iterator;

    /// @{
    /// Unordered (fastest) iteration
    iterator		begin(GA_AttributeScope scope = GA_SCOPE_INVALID) const
					{ return iterator(this, scope, false); }
    iterator		end(GA_AttributeScope scope = GA_SCOPE_INVALID) const
					{ return iterator(this, scope, true); }
    /// @}

    /// @{
    /// Ordered iteration.  This traverses attributes in a fixed order
    ordered_iterator	obegin(GA_AttributeScope scope = GA_SCOPE_INVALID) const
			{
			    iterator	it(this, scope, false);
			    return ordered_iterator(it, compareAlpha);
			}
    ordered_iterator	oend(GA_AttributeScope scope = GA_SCOPE_INVALID) const
			{
			    return ordered_iterator();
			}
    /// @}

    template<typename FUNCTOR>
    SYS_FORCE_INLINE
    void forEachProxy(FUNCTOR &&functor) const
    {
        if (!myTable.empty())
            myTable.forEachValue(functor);
        if (!myPrivateTable.empty())
            myPrivateTable.forEachValue(functor);
        const UT_ArrayStringMap<GA_Group *> *grouptable = getGroupTable();
        if (grouptable && !grouptable->empty())
        {
            grouptable->forEachValue([&functor](GA_Group *group){
                functor(UTverify_cast<GA_ElementGroup*>(group)->getProxy().get());
            });
        }
    }

    template<typename FUNCTOR>
    SYS_FORCE_INLINE
    void forEachProxy(GA_AttributeScope scope, FUNCTOR &&functor) const
    {
        if ((scope == GA_SCOPE_PUBLIC || scope == GA_SCOPE_INVALID) && !myTable.empty())
            myTable.forEachValue(functor);
        if ((scope == GA_SCOPE_PRIVATE || scope == GA_SCOPE_INVALID) && !myPrivateTable.empty())
            myPrivateTable.forEachValue(functor);
        if (scope == GA_SCOPE_GROUP || scope == GA_SCOPE_INVALID)
        {
            const UT_ArrayStringMap<GA_Group *> *grouptable = getGroupTable();
            if (grouptable && !grouptable->empty())
            {
                grouptable->forEachValue([&functor](GA_Group *group){
                    functor(UTverify_cast<GA_ElementGroup*>(group)->getProxy().get());
                });
            }
        }
    }

    template<typename FUNCTOR>
    SYS_FORCE_INLINE
    void forEachAttribute(FUNCTOR &&functor) const
    {
        if (!myTable.empty())
        {
            myTable.forEachValue([&functor](GA_AttributeProxy *proxy){
                functor(proxy->getAttribute());
            });
        }
        if (!myPrivateTable.empty())
        {
            myPrivateTable.forEachValue([&functor](GA_AttributeProxy *proxy){
                functor(proxy->getAttribute());
            });
        }
        const UT_ArrayStringMap<GA_Group *> *grouptable = getGroupTable();
        if (grouptable && !grouptable->empty())
        {
            grouptable->forEachValue([&functor](GA_Group *group){
                functor(UTverify_cast<GA_ElementGroup*>(group));
            });
        }
    }

    template<typename FUNCTOR>
    SYS_FORCE_INLINE
    void forEachAttribute(GA_AttributeScope scope, FUNCTOR &&functor) const
    {
        if ((scope == GA_SCOPE_PUBLIC || scope == GA_SCOPE_INVALID) && !myTable.empty())
        {
            myTable.forEachValue([&functor](GA_AttributeProxy *proxy){
                functor(proxy->getAttribute());
            });
        }
        if ((scope == GA_SCOPE_PRIVATE || scope == GA_SCOPE_INVALID) && !myPrivateTable.empty())
        {
            myPrivateTable.forEachValue([&functor](GA_AttributeProxy *proxy){
                functor(proxy->getAttribute());
            });
        }
        if (scope == GA_SCOPE_GROUP || scope == GA_SCOPE_INVALID)
        {
            const UT_ArrayStringMap<GA_Group *> *grouptable = getGroupTable();
            if (grouptable && !grouptable->empty())
            {
                grouptable->forEachValue([&functor](GA_Group *group){
                    functor(UTverify_cast<GA_ElementGroup*>(group));
                });
            }
        }
    }

    template<typename FUNCTOR>
    SYS_FORCE_INLINE
    void forEachName(GA_AttributeScope scope, FUNCTOR &&functor) const
    {
        if ((scope == GA_SCOPE_PUBLIC || scope == GA_SCOPE_INVALID) && !myTable.empty())
            myTable.forEachKey(functor);
        if ((scope == GA_SCOPE_PRIVATE || scope == GA_SCOPE_INVALID) && !myPrivateTable.empty())
            myPrivateTable.forEachKey(functor);
        if (scope == GA_SCOPE_GROUP || scope == GA_SCOPE_INVALID)
        {
            const UT_ArrayStringMap<GA_Group *> *grouptable = getGroupTable();
            if (grouptable && !grouptable->empty())
                grouptable->forEachKey(functor);
        }
    }


private: // Methods

    /// @private
    /// Used only by GA_AttributeSet
    void setNumBuckets(GA_AttributeScope scope, size_t nbuckets)
    {
        if (scope == GA_SCOPE_PUBLIC)
            myTable.setNumBuckets(nbuckets);
        else if (scope == GA_SCOPE_PRIVATE)
            myPrivateTable.setNumBuckets(nbuckets);
    }

    /// @private
    /// Used only by GA_AttributeSet
    void setSet(GA_AttributeSet *s) { mySet = s; }
    void reserveNewSymbolTableSpace(GA_AttributeScope scope, exint n)
    {
        if (scope == GA_SCOPE_PUBLIC)
        {
            if (myTable.size()+n > myTable.bucket_count())
                myTable.rehash(myTable.size()+n);
        }
        else if (scope == GA_SCOPE_PRIVATE)
        {
            if (myPrivateTable.size()+n > myPrivateTable.bucket_count())
                myPrivateTable.rehash(myPrivateTable.size()+n);
        }
    }


    SYS_FORCE_INLINE
    void add(GA_AttributeScope scope, const UT_StringHolder &name, GA_AttributeProxy *proxy)
    {
        UT_ASSERT_P(proxy != NULL);
        bumpVersionParms();
        if (scope == GA_SCOPE_PUBLIC)
            myTable.insert(std::make_pair(name, proxy));
        else if (scope == GA_SCOPE_PRIVATE)
            myPrivateTable.insert(std::make_pair(name, proxy));
        // NOTE: We don't add the proxy to a table if scope is GA_SCOPE_GROUP,
        //       because it's already in the group table.
    }
    SYS_FORCE_INLINE
    void remove(GA_AttributeScope scope, const UT_StringRef &name)
    {
        bumpVersionParms();
        if (scope == GA_SCOPE_PUBLIC)
            myTable.erase(name);
        else if (scope == GA_SCOPE_PRIVATE)
            myPrivateTable.erase(name);
        // NOTE: We don't remove the proxy from a table if scope is GA_SCOPE_GROUP,
        //       because it's managed by the group table.
    }
    friend class GA_AttributeSet;   // Only GA_AttributeSet can add/remove
    friend class GA_GroupTable;	    // So group table can bump our parms.

    SYS_FORCE_INLINE
    void	bumpVersionParms() { myVersionParms++; }

    exint numGroups() const;
    const UT_ArrayStringMap<GA_Group*> *getGroupTable() const;

private: // Data

    MapType myTable;
    MapType myPrivateTable;

    GA_AttributeSet *mySet;
    exint myVersionParms;
};

#endif

