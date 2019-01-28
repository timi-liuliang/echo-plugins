/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_PrimitiveTypeMask.h (GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_PrimitiveTypeMask_h__
#define __GA_PrimitiveTypeMask_h__

#include "GA_API.h"

#include "GA_PrimitiveTypeId.h"

#include <SYS/SYS_Types.h>
#include <iterator>

class UT_BitArray;
class GA_PrimitiveFactory;

class GA_API GA_PrimitiveTypeMask
{
public:
    struct GA_API FamilyBitMask
    {
	explicit FamilyBitMask(unsigned m) : myMask(m) {}
	unsigned getMask() const { return myMask; }
	private:
	    unsigned	 myMask;
    };

    struct GA_API FactoryTypeBitMask
    {
	explicit FactoryTypeBitMask(unsigned m) : myMask(m) {}
	unsigned getMask() const { return myMask; }
	private:
	    unsigned	 myMask;
    };

    GA_PrimitiveTypeMask()
	: myBaseMask(0), myExtendedMask(NULL)
    {
    }
    GA_PrimitiveTypeMask(const GA_PrimitiveFactory &factory,
			 const FamilyBitMask &mask)
	: myBaseMask(0), myExtendedMask(NULL)
    {
	init(factory, mask);
    }
    explicit GA_PrimitiveTypeMask(const FactoryTypeBitMask &mask)
	: myBaseMask(mask.getMask()), myExtendedMask(NULL)
    {
    }

    ~GA_PrimitiveTypeMask();

    bool	 includes(GA_PrimitiveTypeId type_id) const;
    bool	 excludes(GA_PrimitiveTypeId type_id) const
		 { return !includes(type_id); }

    void	 add(GA_PrimitiveTypeId type_id);
    void	 remove(GA_PrimitiveTypeId type_id);

    void	 operator|=(const GA_PrimitiveTypeMask &m);
    void	 operator&=(const GA_PrimitiveTypeMask &m);
    void	 operator-=(const GA_PrimitiveTypeMask &m);
    bool	 operator==(const GA_PrimitiveTypeMask &m);
    bool	 operator!=(const GA_PrimitiveTypeMask &m);

    class const_iterator :
	public std::iterator<std::forward_iterator_tag, const GA_PrimitiveTypeId>
    {
    public:
	const GA_PrimitiveTypeId &operator*() const { return myId; }

	bool operator==(const const_iterator &other) const
		{ return myId == other.myId; }
	bool operator!=(const const_iterator &other) const
		{ return myId != other.myId; }

	const_iterator &operator++()
	{
	    myId = myMask.getNextType(myId);
	    return *this;
	}

	// Delete the post-increment operator.
	const_iterator &operator++(int) = delete;

    protected:
	friend class GA_PrimitiveTypeMask;
	const_iterator(const GA_PrimitiveTypeMask &mask,
	               const GA_PrimitiveTypeId &id) : myMask(mask), myId(id) {}
    private:
	const GA_PrimitiveTypeMask &myMask;
	GA_PrimitiveTypeId myId;
    };

    /// Returns an iterator representing the beginning of the range for the
    /// type ids contained with this mask.
    const_iterator begin() const
    { return const_iterator(*this, getFirstType()); }

    /// Returns an iterator representing the end of the range for the
    /// type ids contained with this mask.
    const_iterator end() const
    { return const_iterator(*this, GA_PrimitiveTypeId()); }

    SYS_SAFE_BOOL operator bool() const;

private:
    friend class GA_PrimitiveTypeMask::const_iterator;

    /// Initialize primitive mask using a family mask.
    void	 init(const GA_PrimitiveFactory &factory,
		      const FamilyBitMask &mask);

    GA_PrimitiveTypeId getFirstType() const;
    GA_PrimitiveTypeId getNextType(GA_PrimitiveTypeId c) const;

    friend const GA_PrimitiveTypeMask operator|
	(const GA_PrimitiveTypeMask &m1, const GA_PrimitiveTypeMask &m2);
    friend const GA_PrimitiveTypeMask operator&
	(const GA_PrimitiveTypeMask &m1, const GA_PrimitiveTypeMask &m2);
#if 0
    friend const GA_PrimitiveTypeMask operator~
	(const GA_PrimitiveTypeMask &m1);
#endif

    uint64	 myBaseMask;
    UT_BitArray	*myExtendedMask;
};

// Binary Operations on GA_TypeMask
inline const GA_PrimitiveTypeMask
operator|(const GA_PrimitiveTypeMask &m1, const GA_PrimitiveTypeMask &m2)
{
    GA_PrimitiveTypeMask	m(m1);
    m |= m2;
    return m;
}
inline const GA_PrimitiveTypeMask
operator&(const GA_PrimitiveTypeMask &m1, const GA_PrimitiveTypeMask &m2)
{
    GA_PrimitiveTypeMask	m(m1);
    m &= m2;
    return m;
}

// TODO: Cannot invert without a GA_PrimitiveFactory reference...
#if 0
inline const GA_PrimitiveTypeMask
operator~(const GA_PrimitiveTypeMask &m1)
{
    GA_PrimitiveTypeMask	m(m1);
    m.invert();
    return m;
}
#endif

#endif
