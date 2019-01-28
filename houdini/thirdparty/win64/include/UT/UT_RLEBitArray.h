/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_RLEBitArray.h ( UT Library, C++)
 *
 * COMMENTS:
 *
 *	run length encoded bit array
 *
 */

#ifndef __UT_RLEBitArray__
#define __UT_RLEBitArray__

#include "UT_API.h"
#include "UT_Assert.h"
#include "UT_LinkList.h"
#include "UT_Swap.h"

#include <SYS/SYS_Types.h>

#include <limits.h>


class UT_API UT_RLEBitArray
{
public:
    enum { LOWEST_INDEX = INT_MIN };

    UT_RLEBitArray()
    {
	mySearchNode = 0;
	myLastIndex = 0;
    }
    ~UT_RLEBitArray()
    {
    }

    UT_RLEBitArray(const UT_RLEBitArray &v);

    void		 swap( UT_RLEBitArray &other );

    // For most efficient use, call the following increasing values of idx

    int			 numBitsSet() const;

    int			 getBit(int idx) const;
    bool		 getBitRun(int idx, int &start, int &end) const;

    void		 setBitTrue(int idx);
    void		 setBitFalse(int idx);
    void		 setBit( int idx, int value)
			 {
			     if( value )
				 setBitTrue( idx );
			     else
				 setBitFalse( idx );
			 }
    void		 setBitRunTrue( int start, int end );

    int			 toggleBit(int idx);

    void		 clear();
    bool		 isEmpty() const;
    int			 getNextRun(int &start, int &end, bool first=false);

    // returns true and sets currentBit to the next set bit
    // otherwise returns false leaving currentBit unchanged
    // to start off your search, set currentBit to LOWEST_INDEX (or below the
    // first set bit)
    // you could write, for example:
    // int	i = UT_RLEBitArray::LOWEST_INDEX; // (or -1 if applicable)
    // while( iterate( i ) ) { blah; }
    bool		 iterate( int &currentBit );

    int64		 getMemoryUsage() const;

    void		 display() const;

    // operators
    int		 operator()(int index) const { return getBit(index); }
    int		 operator[](int index) const { return getBit(index); }
    // A and B
    const UT_RLEBitArray&operator&=(const UT_RLEBitArray &v);
    // A or B
    const UT_RLEBitArray&operator|=(const UT_RLEBitArray &v);
    // A and not B or not A and B
    const UT_RLEBitArray&operator^=(const UT_RLEBitArray &v);
    // A and not B
    const UT_RLEBitArray&operator-=(const UT_RLEBitArray &v);
    const UT_RLEBitArray&operator= (const UT_RLEBitArray &v);
    int		 operator==(const UT_RLEBitArray &v) const;
    int		 operator!=(const UT_RLEBitArray &v) const;

protected:
    class Node;

public:
    class const_iterator
    {
    public:
	const_iterator()
	    : myNode(NULL)
	    , myI(LOWEST_INDEX)
	{
	}

	int operator*() const
	{
	    return myI;
	}
	int bitIndex() const
	{
	    return myI;
	}

	const_iterator &operator++()
	{
	    if (myNode)
	    {
		++myI;
		if (myI > myNode->myEnd)
		{
		    myNode = static_cast<const Node *>(myNode->next());
		    if (myNode)
			myI = myNode->myStart;
		}
	    }
	    return *this;
	}

	bool operator==(const const_iterator &rhs) const
	{
	    if (myNode != rhs.myNode)
		return false;
	    return (myNode ? (myI == rhs.myI) : true);
	}
	bool operator!=(const const_iterator &lhs) const
	{
	    return !(*this == lhs);
	}

	int begin() const
	{
	    return myNode->myStart;
	}
	int end() const	// uses STL convention to return exclusive index
	{
	    return myNode->myEnd + 1;
	}

    protected:
	const_iterator(const UT_LinkList &list)
	    : myNode(static_cast<const Node *>(list.head()))
	    , myI(myNode ? myNode->myStart : LOWEST_INDEX)
	{
	}

	const Node *	    myNode;
	int		    myI;

	friend class UT_RLEBitArray;
    };
    typedef const_iterator iterator;	// no mutable iterator

    const_iterator  begin() const   { return const_iterator(myList); }
    iterator	    begin()	    { return iterator(myList); }
    const_iterator  end() const	    { return const_iterator(); }
    iterator	    end()	    { return iterator(); }

protected:
    void		 restartSearch() const;
    void		 restartSearchForIndex( int idx ) const;
    void		 advanceSearchNode() const;

    // Invariants:
    // if mySearchNode points to a node (case 1)
    // the range from [myLastIndex, mySearchNode->myStart-1] is false
    // the range from [mySearchNode->myStart, mySearchNode->myEnd] is true
    //
    // if mySearchNode is NULL (case 2)
    // the range from [myLastIndex, inf) is false
    //
    // restartSearchForIndex( idx ) guarantees that
    // idx is in the range [myLastIndex, mySearchNode->myEnd] (case 1)
    // or else idx is in [myLastIndex, inf) (case 2)
    class Node : public UT_LinkNode
    {
    public:
	Node(int s, int e)
	    : myStart(s)
	    , myEnd(e)
	{
	}

	int		myStart;
	int		myEnd;
    };

    UT_LinkList		 myList;
    mutable Node	*mySearchNode;
    mutable int		 myLastIndex;
};

UT_SWAPPER_CLASS( UT_RLEBitArray )


inline int
UT_RLEBitArray::numBitsSet() const
{
    Node	*me;
    int		 n = 0;

    me = (Node *)myList.head();

    while( me )
    {
	n += me->myEnd - me->myStart + 1;
	me = (Node *)myList.next(me);
    }

    return n;
}

inline void
UT_RLEBitArray::restartSearch() const
{
    mySearchNode = (Node *)myList.head();
    myLastIndex = LOWEST_INDEX;
}

inline void
UT_RLEBitArray::restartSearchForIndex( int idx ) const
{
    if( !mySearchNode || idx < myLastIndex )
	restartSearch();
    while( mySearchNode && idx > mySearchNode->myEnd )
	advanceSearchNode();
}

inline void
UT_RLEBitArray::advanceSearchNode() const
{
    myLastIndex = mySearchNode->myEnd + 1;
    mySearchNode = (Node *)myList.next(mySearchNode);
}

inline int
UT_RLEBitArray::getBit(int idx) const
{
    restartSearchForIndex( idx );

    // Somewhere in the middle?

    if( mySearchNode &&
	(idx >= mySearchNode->myStart) && (idx <= mySearchNode->myEnd) )
	return 1;

    UT_ASSERT( !mySearchNode || idx < mySearchNode->myEnd );
    return 0;
}

#endif
