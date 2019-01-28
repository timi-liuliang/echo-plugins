/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_Defragment.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_Defragment__
#define __GA_Defragment__

#include "GA_API.h"
#include "GA_IndexMap.h"
#include "GA_OffsetList.h"
#include "GA_Types.h"

#include <UT/UT_Array.h>
#include <SYS/SYS_Inline.h>

#include <stddef.h>


class UT_Options;


/// @brief Defragmentation of IndexMaps
///
/// Defragmentation consists of two stages
/// - Analyzing the index map and computing the swaps/exchanges
/// - Application of the swaps
/// When applying the swaps, there are two operations which need to be done
/// - When an offset is swapped, all attributes need to have their data
///   swapped.
/// - When an offset is swapped, any reference to the old offset needs to be
///   updated to the new offset.  For example, primitive store offsets into the
///   vertex index map.  When these offsets get moved, primitives need to
///   update their reference.  The detail's topology management automatically
///   updates references.
///
/// NOTE: Ranges being moved can *only* overlap if the move is from a higher
///       address to a lower address!
///
class GA_API GA_Defragment
{
public:
     GA_Defragment();
    ~GA_Defragment();

    enum STRATEGY
    {
	/// Defragment so that the offsets will match the ordered index
	/// This will also remove any holes in the index map.
	DEFRAG_TO_ORDER,

	/// Default strategy
	DEFRAG_DEFAULT = DEFRAG_TO_ORDER
    };

    enum OPERATION
    {
	MOVE_A_TO_B,	// Move data from A to B (can only be overlapping if A > B)
	SWAP_AB,	// Swap data at A and B  (can't be overlapping!)
    };

    const GA_IndexMap	&getIndexMap() const	{ return *myIndexMap; }
    GA_AttributeOwner	 getOwner() const	{ return myIndexMap->getOwner(); }

    /// Clear all information
    void	clear();

    /// Create a defragmentation operation using various strategies
    /// Options include
    /// - @c bool force [off] @n
    ///   Force defragmenting to occur.
    /// - @c occupancy [0.75] @n
    ///   Defragment when the ratio of occupied indices to index capacity is
    ///   beyond this threshold.
    /// - @c bool sortvertex [off] @n
    ///	  When defragmenting vertices, sort the vertices by primitive reference
    ///	  order.  This will optimize primitive vertex traversal, but add cost
    ///	  to defragmenting.
    void	create(const GA_IndexMap &map,
			STRATEGY strategy=DEFRAG_DEFAULT,
			const UT_Options *options=NULL);

    /// Check if there are any swaps to perform
    bool	isEmpty() const	
		    { return getSwapCount() == 0; }

    /// Test to see if an offset will move during the swapping
    SYS_FORCE_INLINE
    bool	hasOffsetChanged(GA_Offset old_offset) const
		    { return mapOffset(old_offset) != old_offset; }

    /// Given the old offset, return the new offset.
    SYS_FORCE_INLINE
    GA_Offset	mapOffset(GA_Offset old_offset) const
		    { return myOffsetMap(old_offset); }

    /// Return number of offsets stored in the map
    GA_Offset	getOffsetMapEntries() const { return myOffsetMap.entries(); }

    /// Return the maximum offset @b after the defragmentation is complete
    GA_Offset	getMaxOccupiedOffset() const	{ return myMaxOffset; }

    /// Class to hold information about a swap.
    /// Move entries from [a, a+n-1] to [b, b+n-1]
    /// Swap entries in the range [a, a+n-1] with [b, b+n-1]
    class swapInfo
    {
    public:
	swapInfo(GA_Size a, GA_Size b, GA_Size n, OPERATION op)
	    : myA(a)
	    , myB(b)
	    , myN(n)
	    , myOp(op)
	{ }
	swapInfo(const swapInfo &i)
	    : myA(i.myA)
	    , myB(i.myB)
	    , myN(i.myN)
	    , myOp(i.myOp)
	{ }
	~swapInfo() {}
	swapInfo	&operator=(const swapInfo &i)
			 {
			     myA = i.myA;
			     myB = i.myB;
			     myN = i.myN;
			     myOp = i.myOp;
			     return *this;
			 }
	bool		operator==(const swapInfo &i) const
			{
			    return myA == i.myA && myB == i.myB && myN == i.myN && myOp == i.myOp;
			}

	GA_Offset	getA() const	{ return myA; }
	GA_Offset	getB() const	{ return myB; }
	GA_Size		getN() const	{ return myN; }
	OPERATION	getOp() const	{ return myOp; }

	bool		operator<(const swapInfo &cmp) const
			{
			    if (myOp != cmp.myOp)
			    {
				return myOp == MOVE_A_TO_B;
			    }
			    return myB < cmp.myB;
			}
	int		compare(const swapInfo &cmp) const
			{
			    if (*this == cmp)
				return 0;
			    return *this < cmp ? -1 : 1;
			}

	static int	qsortCompare(const swapInfo *a, const swapInfo *b)
			{
			    return a->compare(*b);
			}

    private:
	GA_Offset	myA, myB;
	GA_Size		myN;
	OPERATION	myOp;
    };
    /// Class to iterate over all defrag operations.  For example: @code
    /// for (const_iterator it = frag.begin(); !it.atEnd(); ++it) {
    ///     switch (it.getOp()) {
    ///         case MOVE_A_TO_B:
    ///	           moveRange(it.getA(), it.getB(), it.getN());
    ///         case SWAP_AB:
    ///	           swapRange(it.getA(), it.getB(), it.getN());
    ///     }
    /// }
    /// @endcode
    class const_iterator
    {
    public:
	const_iterator()
	    : myFrag(NULL)
	    , myCurr(0)
	    , myA(GA_INVALID_OFFSET)
	    , myB(GA_INVALID_OFFSET)
	    , myN(0)
	    , myOp(MOVE_A_TO_B)
	{ }
	const_iterator(const const_iterator &src)
	    : myFrag(src.myFrag)
	    , myCurr(src.myCurr)
	    , myA(src.myA)
	    , myB(src.myB)
	    , myN(src.myN)
	    , myOp(src.myOp)
	{ }
	~const_iterator()
	{ }

	/// Get the swap ranges
	GA_Offset	getA() const	{ return myA; }
	GA_Offset	getB() const	{ return myB; }
	GA_Size		getN() const	{ return myN; }
	OPERATION	getOp() const	{ return myOp; }

	const_iterator	&operator=(const const_iterator &src)
			 {
			     myFrag = src.myFrag;
			     myCurr = src.myCurr;
			     myA = src.myA;
			     myB = src.myB;
			     myN = src.myN;
			     myOp = src.myOp;
			     return *this;
			 }
	bool		operator==(const const_iterator &src) const
			{
			    if (atEnd() && src.atEnd())
				return true;
			    return myFrag == src.myFrag && myCurr == src.myCurr;
			}
	bool		atEnd() const
			{
			    return (!myFrag) || (myCurr >= myFrag->getSwapCount());
			}
	void		advance()
			{
			    myCurr++;
			    loadFromSwap();
			}
	void		rewind()
			{
			    myCurr = 0;
			    myA = GA_Offset(0);
			    myB = GA_Offset(0);
			    myN = 0;
			    loadFromSwap();
			}
	const_iterator	&operator++()		{ advance(); return *this; }
	const_iterator	&operator++(int)	{ advance(); return *this; }

    private:
	void	loadFromSwap()
		{
		    if (myCurr < myFrag->getSwapCount())
		    {
			const GA_Defragment::swapInfo
			    &s = myFrag->getSwap(myCurr);
			myA = s.getA();
			myB = s.getB();
			myN = s.getN();
			myOp = s.getOp();
		    }
		}
	/// Get the current swap information
	const GA_Defragment::swapInfo	&getSwap() const
					{ return myFrag->getSwap(myCurr); }
	const_iterator(const GA_Defragment &frag)
	    : myFrag(&frag)
	{
	    rewind();
	}
	const GA_Defragment	*myFrag;
	GA_Size			 myCurr;
	GA_Offset		 myA, myB;
	GA_Size			 myN;
	OPERATION		 myOp;
	friend class		 GA_Defragment;
    };

    const_iterator	begin() const	{ return const_iterator(*this); }
    const_iterator	end() const	{ return const_iterator(); }

    static bool		shouldIndexMapBeDefragmented(
				const GA_IndexMap &map,
				const UT_Options *options);
private:
    GA_Size		 getSwapCount() const	{ return mySwaps.entries(); }
    const swapInfo	&getSwap(GA_Size i) const	{ return mySwaps(i); }
    /// Build the list of moves and swaps to make (mySwaps)
    void		 packToIndexOrder(const UT_Options *options);

    /// Map, from offsets to indices, that describes the arrangement of data to be defragmented
    const GA_IndexMap		*myIndexMap;
    /// List of move and swap operations to be made in order to defragment the data
    UT_Array<swapInfo>	 mySwaps;
    GA_OffsetListType<GA_Offset> myOffsetMap;	// Map from old to new offset
    GA_Offset			 myMaxOffset;	// Maximum offset after defrag
};

#endif

