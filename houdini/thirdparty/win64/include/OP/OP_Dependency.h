/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_Dependency.h (C++)
 *
 * COMMENTS:
 *		OP_Dependency is used to keep track of op dependencies
 */

#ifndef _OP_Depdency_h_
#define _OP_Depdency_h_

#include "OP_API.h"

#include "OP_DataTypes.h"
#include "OP_Output.h"

#include <PRM/PRM_RefId.h>
#include <UT/UT_Assert.h>
#include <UT/UT_Array.h>
#include <UT/UT_Map.h>
#include <UT/UT_ValArray.h>

class OP_API OP_Reference
{
public:
    OP_Reference()
    {
	myRefOpId = -1;
	myRefCount = 0;
    }
    explicit OP_Reference( int ref_op_id )
    {
	myRefOpId = ref_op_id;
	myRefCount = 1;
    }

    void addRef()
    {
	UT_ASSERT_P( myRefCount >= 0 );
	myRefCount++;
    }
    int removeRef(int count = 1)
    {
	UT_ASSERT_P( myRefCount >= count );
	if( myRefCount >= count )
	    myRefCount -= count;
	else
	    myRefCount = 0;
	return myRefCount;
    }

    void clearRefOpId()		{ myRefOpId = -1; }
    int getRefOpId() const	{ return myRefOpId; }
    void setRefOpId(int op_id)	{ myRefOpId = op_id; }

    int getRefCount() const	{ return myRefCount; }

    unsigned	hash() const
		{
		    return SYSwang_inthash(myRefOpId);
		}
    int operator==( const OP_Reference &other )
    {
	return myRefOpId == other.myRefOpId;
    }

private:
    int	myRefOpId;
    int	myRefCount;
};
typedef UT_Array<OP_Reference>	OP_ReferenceList;

class OP_API OP_Dependency
{
public:
    OP_Dependency()
    {
	myRefOpId = -1;
	myInterestType = OP_INTEREST_NONE;
    }
    
    // ref_op_id    - The ID of the op who is dependent on our owner
    // source_ref   - A reference to a parm/channel in our owner on which
    //		      ref_op_id is dependent
    // ref	    - The parm/chan in the dependent that is dependent on our
    //		      owner
    // interest	    - The kind of interest the dependent will have
    OP_Dependency(int ref_op_id, const PRM_RefId &source_ref,
		  const PRM_RefId &ref, OP_InterestType interest)
    {
	myRefOpId      = ref_op_id;
	myRef	       = ref;
	myInterestType = interest;
	mySourceRef    = source_ref;
    }

    OP_Dependency &operator=(const OP_Dependency &other) = default;

    void addInterest( OP_InterestType type )
    {
	myInterestType = (OP_InterestType) ((int) myInterestType | type);
    }
    OP_InterestType getInterest() const
    {
	return myInterestType;
    }

    int matches( int ref_op_id, const PRM_RefId &ref,
		 OP_InterestType mask = OP_INTEREST_ALL ) const
    {
	return (myRefOpId == ref_op_id && myRef.matches(ref)
		&& hasInterest(mask));
    }
    int matches( int ref_op_id, const PRM_RefId &source_ref,
		 const PRM_RefId &ref,
		 OP_InterestType mask = OP_INTEREST_ALL ) const
    {
	return myRefOpId == ref_op_id && mySourceRef == source_ref && 
	       myRef == ref && hasInterest(mask);
    }
    unsigned	hash() const
		{
		    uint	h = SYSwang_inthash(myRefOpId);
		    h = h * 37 + SYSwang_inthash(myInterestType);
		    h = h * 37 + mySourceRef.hash();
		    h = h * 37 + myRef.hash();
		    return h;
		}

    int operator==( const OP_Dependency &other ) const
    {
	return matches( other.myRefOpId, other.mySourceRef, other.myRef );
    }

    // Returns true if the source ref matches exactly the removed parm index
    bool fixRemovedSourceRefParmIndex(int removed_parm_idx)
    {
	if (!mySourceRef.isValid())
	    return false;

	if (mySourceRef.getParmRef() == removed_parm_idx)
	{
	    UT_ASSERT_P(false);
	    mySourceRef.setParmRef(-1, -1);
	    return true;
	}
	else if (mySourceRef.getParmRef() > removed_parm_idx)
	{
	    mySourceRef.setParmRef(mySourceRef.getParmRef() - 1,
				   mySourceRef.getParmSubIndex());
	}
	return false;
    }

    int			getRefOpId() const	    { return myRefOpId; }
    void		clearRefOpId()		    { myRefOpId = -1; }
    void		setRefOpId( int op_id )	    { myRefOpId = op_id; }
    const PRM_RefId     &getRefId() const	    { return myRef; }
    const PRM_RefId     &getSourceRefId() const	    { return mySourceRef; }
    OP_InterestType	getInterestType() const	    { return myInterestType; }
    int			hasInterest(OP_InterestType interest) const
			{ return (myInterestType & interest) != 0; }

private:
    int		    myRefOpId;
    PRM_RefId	    mySourceRef;
    PRM_RefId	    myRef;
    OP_InterestType myInterestType;
};

// Storage for dependency information for a single node's dependents
class OP_DependencyNode
{
public:
    OP_DependencyNode()
    {
    }
    OP_DependencyNode(const OP_DependencyNode &src)
	: myList(src.myList)
    {
    }
    ~OP_DependencyNode()
    {
    }

    bool		 isEmpty() const { return myList.entries() == 0; }
    void		 clear()	{ myList.entries(0); }
    void		 append(const OP_Dependency &v) { myList.append(v); }
    int64		 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += myList.getMemoryUsage(false);
        return mem;
    }

    typedef UT_Array<OP_Dependency>::iterator	iterator;
    typedef UT_Array<OP_Dependency>::const_iterator	const_iterator;
    typedef UT_Array<OP_Dependency>::reverse_iterator reverse_iterator;

    iterator		begin()		{ return myList.begin(); }
    iterator		end()		{ return myList.end(); }
    const_iterator	begin() const	{ return myList.begin(); }
    const_iterator	end() const	{ return myList.end(); }
    reverse_iterator	rbegin()	{ return myList.rbegin(); }
    reverse_iterator	rend()		{ return myList.rend(); }

    void	removeItem(const reverse_iterator &it)
		{
		    myList.removeItem(it);
		}
private:
    // Called by OP_DependencyList
    int		getRefOpId() const
		{
		    UT_ASSERT(myList.entries());
		    return myList(0).getRefOpId();
		}
    friend class OP_DependencyList;
    UT_Array<OP_Dependency>	myList;
};

class OP_DependencyList
{
    using DependencyMap = UT_Map<int, OP_DependencyNode *>;
public:
    OP_DependencyList()
	: myEntries(0)
    {
    }
    OP_DependencyList(const OP_DependencyList &src);
    ~OP_DependencyList()
    {
	clear();
    }
    OP_DependencyList	&operator=(const OP_DependencyList &src);
    bool		 isEmpty() const	{ return myEntries == 0; }
    void		 clear();
    void		 append(const OP_Dependency &v);
    int64		 getMemoryUsage(bool inclusive) const;

    /// Find a dependency for the given criteria
    OP_Dependency	*find(int ref_op_id, const PRM_RefId &source_ref,
				const PRM_RefId &ref_id) const;
    OP_Dependency	*find(int ref_op_id, const PRM_RefId &ref_id) const;

    /// @{
    /// Remove all matching dependencies.  Returns number of items removed
    int		removeMatching(int ref_op_id, OP_InterestType mask);
    int		removeMatching(int ref_op_id, const PRM_RefId &ref_id,
				OP_InterestType mask);
    /// @}
    /// Access to the hash table
    class reverse_iterator
    {
	typedef UT_ValArray<OP_DependencyNode *> NodeCache;
    public:
	reverse_iterator()
	    : myOwner(NULL)
	    , myNodeCache()
	    , myNodeIterator()
	    , myListIterator()
	{
	}
	reverse_iterator(const reverse_iterator &it)
	    : myOwner(it.myOwner)
	    , myNodeCache(it.myNodeCache)
	    , myNodeIterator(it.myNodeIterator)
	    , myListIterator(it.myListIterator)
	{
	}
	~reverse_iterator() {}

	OP_Dependency	&getDependency() const
	{
	    return *myListIterator;
	}
	OP_Dependency	&operator*() const
	{
	    return *myListIterator;
	}
	OP_Dependency	*operator->() const
	{
	    return &getDependency();
	}

	reverse_iterator	&operator=(const reverse_iterator &cmp)
				{
				    myOwner = cmp.myOwner;
				    myNodeCache = cmp.myNodeCache;
				    myNodeIterator = cmp.myNodeIterator;
				    myListIterator = cmp.myListIterator;
				    return *this;
				}
	reverse_iterator	&operator++()
				{
				    advance();
				    return *this;
				}
	reverse_iterator	&operator++(int)
				{
				    advance();
				    return *this;
				}
	bool	operator==(const reverse_iterator &cmp) const
	{
	    return myOwner == cmp.myOwner &&
		    myNodeIterator == cmp.myNodeIterator &&
		    myListIterator == cmp.myListIterator &&
		    myNodeCache == cmp.myNodeCache;
	}
	bool	operator!=(const reverse_iterator &cmp) const
	{
	    return !(*this == cmp);
	}
	void	advance()
	{
	    myListIterator.advance();
	    while (myListIterator.atEnd())
	    {
		++myNodeIterator;
		if (myNodeIterator.atEnd())
		    break;
		myListIterator = getDependencyNode()->rbegin();
	    }
	}

	void	rewind()
		{
		    const DependencyMap &dep_map = myOwner->myDependencyMap;
		    myNodeCache.entries(0);
		    myNodeCache.setCapacityIfNeeded(dep_map.size());
		    for (OP_DependencyNode *node : dep_map.mapped_range())
		    {
			if (!node->isEmpty())
			    myNodeCache.append(node);
		    }
		    myNodeIterator = myNodeCache.begin();
		    if (!myNodeIterator.atEnd())
		    {
			myListIterator = getDependencyNode()->rbegin();
		    }
		    else
		    {
			myListIterator = OP_DependencyNode::reverse_iterator();
		    }
		}

	bool	atEnd() const
		{
		    return myNodeIterator.atEnd() && myListIterator.atEnd();
		}
    private:
	reverse_iterator(const OP_DependencyList *owner)
	    : myOwner(owner)
	    , myNodeIterator()
	    , myListIterator()
	{
	    rewind();
	}

	// Use in private methods to OP_DependencyList
	OP_DependencyNode	*getDependencyNode() const
	{
	    return myNodeIterator.item();
	}
	const OP_DependencyNode::reverse_iterator	&getListIterator() const
	{
	    return myListIterator;
	}


	const OP_DependencyList			*myOwner;
	NodeCache			 	 myNodeCache;
	NodeCache::iterator	 	 	 myNodeIterator;
	OP_DependencyNode::reverse_iterator	 myListIterator;
	friend class OP_DependencyList;
    };
    typedef reverse_iterator	const_iterator;

    const_iterator	begin() const	{ return const_iterator(this); }
    const_iterator	end() const	{ return const_iterator(); }
    reverse_iterator	rbegin() const	{ return reverse_iterator(this); }
    reverse_iterator	rend() const	{ return reverse_iterator(); }

    void	removeItem(const reverse_iterator &it);

private:
    friend class reverse_iterator;
    OP_DependencyNode		*findNode(int op_id) const;

    UT_Map<int, OP_DependencyNode *> myDependencyMap;
    exint		myEntries;
};

#endif // _OP_Depdency_h_
