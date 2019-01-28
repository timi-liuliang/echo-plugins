/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME: 	BitArray.h	
 *
 * COMMENTS:
 *
 */

#ifndef	_UT_BitArray_
#define	_UT_BitArray_

#include "UT_API.h"
#include "UT_Assert.h"
#include <SYS/SYS_Deprecated.h>
#include <SYS/SYS_Types.h>
#include <SYS/SYS_BitUtil.h>
#include <iosfwd>
#include <iterator>

class UT_IStream;


//
//  This class implements a one dimensional array of bits.
//
class UT_API UT_BitArray {
    typedef uint64	BlockType;
    
public:
     UT_BitArray();
     UT_BitArray(const UT_BitArray &src);
     UT_BitArray(UT_BitArray &&src) SYS_NOEXCEPT;
     explicit UT_BitArray(exint size);
    ~UT_BitArray();

    void	insert(exint index, bool value);
    void	append(bool value) { insert(myBitCount, value); }
    void	remove(exint index);

    // Cyclically shift the entire array by shift_offset.
    void	cycle(exint shift_offset);

    exint	size() const { return myBitCount; }
    void	setSize(exint new_size);

    void        clear() { setSize(0); }

    SYS_DEPRECATED(14.0)
    exint	getSize() const	    { return size(); }

    SYS_DEPRECATED(14.0)
    void	resize(exint size)  { setSize(size); }

    const BlockType	*data() const { return myBits; }

    exint	numBitsSet() const;
    exint	numBitsClear() const;

    bool	allBitsSet() const;
    bool	allBitsClear() const;

    inline bool	getBit(exint index) const;

    inline bool	setBit(exint index, bool value);// Returns previous bit value.
    inline bool	toggleBit(exint index);		  // Returns previous bit value.

    inline bool	getBitFast(exint index) const;
    inline void	setBitFast(exint index, bool value);
    inline void	toggleBitFast(exint index);

    void	   setBits(exint index, exint nbits, bool value);
    void	toggleBits(exint index, exint nbits);

    void	   setAllBits(bool value);
    void	toggleAllBits();

    // I/O methods:
    int          save(std::ostream &os, bool binary = false) const;
    bool	 load(UT_IStream &is);

    /// Iterate over set bits
    ///
    /// eg. for (i = bits.iterateInit(); i >= 0; i = bits.iterateNext(i))
    ///		... do something with bits(i)
    ///
    // @{
    exint	iterateInit() const;	// Initialize traversal
    exint	iterateNext(exint currentBit) const;	// Find next set bit
    
    // Deprecated. Don't use.
    // SYS_DEPRECATED(15.0)
    void	iterateInit(exint &i) const { i = iterateInit(); }
    // @}
    

    /// Find first or last bit in the array.  The functions will return -1 if
    /// no bits are set (i.e. equivalent to allBitsClear())
    exint	findFirstBit() const;
    exint	findLastBit() const;

    //		operators

    bool	 operator()(exint index) const { return getBit(index); }
    bool	 operator[](exint index) const { return getBit(index); }
    UT_BitArray	&operator&=(const UT_BitArray &inMap);	// and | intersection
    UT_BitArray	&operator|=(const UT_BitArray &inMap);	// or | union
    UT_BitArray	&operator^=(const UT_BitArray &inMap);	// exclusive or
    UT_BitArray	&operator-=(const UT_BitArray &inMap);	// minus
    UT_BitArray	&operator=(const UT_BitArray &inMap);	// assignment
    UT_BitArray	&operator=(UT_BitArray &&src);	// move assignment
    bool	 operator==(const UT_BitArray &inMap) const;	// compare
    bool	 operator!=(const UT_BitArray &inMap) const
    { return !(*this == inMap); }

    void		swap(UT_BitArray &other);

    /// Returns whether there are any intersections between the two arrays
    bool		intersects(const UT_BitArray &inMap) const;

    int64		getMemoryUsage(bool inclusive=true) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += myWordCount*sizeof(BlockType);
        return mem;
    }

    uint		computeHash() const;

    /// Iterate over bits that are turned on
    class iterator : 
	public std::iterator<std::forward_iterator_tag, exint>
    
    {
    public:
	typedef exint type;

	iterator()
	    : myArray(NULL)
	    , myIndex(0)
	    , mySize(0)
	{}
	~iterator()
	{}

	/// Get the current iteration state
	exint	getBit() const	{ return myIndex; }
	exint	operator*() const	{ return myIndex; }

	/// @{
	/// Standard iterator interface
	bool		operator==(const iterator &i) const
			{
			    return myArray == i.myArray
				&& myIndex == i.myIndex
				&& mySize == i.mySize;
			}
	bool		operator!=(const iterator &i) const
			{
			    return !(*this == i);
			}
	bool		atEnd() const
			{
			    return myIndex >= mySize;
			}
	iterator	&operator++()		{ advance(); return *this; }
	/// @}

	void		 rewind()
			 {
			     if (myArray)
			     {
				 myIndex = myArray->iterateInit();
				 if (myIndex < 0)
				     myIndex = mySize;
			     }
			 }
	void		 advance()
			 {
			     myIndex = myArray->iterateNext(myIndex);
			     if (myIndex < 0)
				 myIndex = mySize;
			 }
    private:
	iterator(const UT_BitArray *array, bool end)
	    : myArray(array)
	    , mySize(array->size())
	{
	    if (end)
		myIndex = mySize;
	    else
	    {
		rewind();
	    }
	}
	const UT_BitArray	*myArray;
	exint			 myIndex;
	exint			 mySize;
	friend class UT_BitArray;
    };
    typedef iterator const_iterator;

    iterator	begin() const	{ return iterator(this, false); }
    iterator	end() const	{ return iterator(this, true); }

private:
    static const int	 myBitsPerWord = sizeof(BlockType)*8;
    static const int	 myWordShift = SYS_LOG2F(myBitsPerWord);
    static const int	 myWordMask = (1 << myWordShift) - 1;
    
    BlockType		*myBits;
    exint		 myBitCount;
    exint		 myWordCount;

private:
    exint		iterateFromWord(uint64 wordIndex) const;
    void		swapBlocks32(BlockType *blocks, exint nb_blocks) const;
    void		clearTopWord();
    
    // returns the word number in which the given bit would be
    static inline exint
    indexWord(exint bit)
    {
	return (bit >> myWordShift);
    }

    // returns the position in word 'indexWord(bit)' where the given bit is
    static inline BlockType
    indexBit(exint bit)
    {
	return (BlockType(1) << (bit & myWordMask));
    }

    // returns the number of words required for the given number of bits
    static inline exint
    numWords(exint size_in_bits)
    {
	return (size_in_bits + (myBitsPerWord - 1)) / myBitsPerWord;
    }

    void		 outTo(std::ostream &os) const;
    friend std::ostream      &operator<<(std::ostream &os, const UT_BitArray &map)
			{
			    map.outTo(os);
			    return os;
			}
};

/// Overload for custom formatting of a UT_BitArray. with UTformat.
UT_API size_t
format(char *buffer, size_t bufsize, const UT_BitArray &b);

// Inline methods
inline bool
UT_BitArray::getBit(exint index) const
{
    BlockType	*word;

    if( index < 0 || index >= myBitCount )
    {
	UT_ASSERT_P( index >= 0 && index < myBitCount );
	return 0;
    }

    word = myBits + indexWord(index);
    return ((*word & indexBit(index)) != 0);
}

inline bool
UT_BitArray::setBit(exint index, bool value)
{
    BlockType	*word;
    BlockType	 mask;
    BlockType	 previous;

    if( index < 0  ||  index >= myBitCount )
    {
	UT_ASSERT_P( index >= 0 && index < myBitCount );
	return 0;
    }

    word = myBits + indexWord(index);
    mask = indexBit(index);
    previous = *word & mask;

    if( value ) *word |=  mask;
    else        *word &= ~mask;

    return (previous != 0);
}

inline bool
UT_BitArray::toggleBit(exint index)
{
    BlockType	*word;
    BlockType	 mask;
    BlockType	 previous;

    if( index < 0  ||  index >= myBitCount )
    {
	UT_ASSERT_P( index >= 0 && index < myBitCount );
	return 0;
    }

    word = myBits + indexWord(index);
    mask = indexBit(index);
    previous = *word & mask;

    *word ^= mask;

    return (previous != 0);
}

inline bool
UT_BitArray::getBitFast(exint index) const
{
    UT_ASSERT_P( index >= 0 && index < myBitCount );
    BlockType word = myBits[indexWord(index)];
    int bit = index & myWordMask;
    return (word >> bit) & BlockType(1);
}

inline void
UT_BitArray::setBitFast(exint index, bool value)
{
    UT_ASSERT_P( index >= 0 && index < myBitCount );
    BlockType &word = myBits[indexWord(index)];
    int bit = index & myWordMask;
    word = (BlockType(value) << bit) | (word & ~(BlockType(1) << bit));
}

inline void
UT_BitArray::toggleBitFast(exint index)
{
    UT_ASSERT_P( index >= 0 && index < myBitCount );
    BlockType &word = myBits[indexWord(index)];
    int bit = index & myWordMask;
    word ^= (BlockType(1) << bit);
}

SYS_FORCE_INLINE exint
UT_BitArray::iterateFromWord(uint64 wordIndex) const
{
    for (; wordIndex < myWordCount; wordIndex++)
    {
	if (myBits[wordIndex])
	{
	    exint bitIndex = SYSfirstBitSet(myBits[wordIndex]);
	    UT_ASSERT_P(bitIndex + (wordIndex<<myWordShift) < size());
	    return bitIndex + (wordIndex << myWordShift); 
	}
    }
    
    return -1;
}


inline exint
UT_BitArray::iterateInit() const
{
    return iterateFromWord(0);
}

inline exint
UT_BitArray::iterateNext(exint currentBit) const
{
    // Negative values imply search from the beginning.
    if (currentBit < 0)
	return iterateFromWord(0);
    
    // finish searching the current word
    if (currentBit >= (myBitCount - 1))
	return -1;
    
    int	       bitIndex = currentBit & myWordMask;
    exint      wordIndex = currentBit >> myWordShift;
    BlockType *word = &myBits[wordIndex];
    
    bitIndex = SYSnextBitSet(*word, bitIndex);
    if (bitIndex >= 0)
    {
	UT_ASSERT_P(bitIndex + (wordIndex<<myWordShift) < size());
	return bitIndex + (wordIndex << myWordShift);
    }
    wordIndex++;
    
    return iterateFromWord(wordIndex);
}


#endif
