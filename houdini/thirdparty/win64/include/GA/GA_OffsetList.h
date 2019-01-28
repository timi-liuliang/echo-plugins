/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_OffsetList.h ( GA Library, C++)
 *
 * COMMENTS:	Class to store a list of offsets/indices based on the
 *		GA_Offset/GA_Index types.
 */

#ifndef __GA_OffsetList__
#define __GA_OffsetList__

#include "GA_API.h"

#include "GA_Types.h"

#include <UT/UT_ArrayHelp.h>
#include <UT/UT_Array.h>
#include <UT/UT_VectorTypes.h>

#include <SYS/SYS_AtomicInt.h>
#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Math.h>
#include <SYS/SYS_Types.h>

class GA_Defragment;
class GA_LoadMap;
class GA_SaveMap;
class GA_IndexMap;
class UT_JSONParser;
class UT_JSONWriter;
class UT_MemoryCounter;

#define GA_OFFSETLIST_VERBOSE_DEBUG 0

// Forward declaration of subclass for use in superclass
template<typename FromType,typename ToType,typename INT_TYPE>
class GA_ListType;

/// GA_OffsetList implements an array of GA_Offsets.
/// Copy-on-write is used to reduce memory usage and make the copying of a
/// GA_OffsetList an inexpensive operation.
///
/// See also: @ref JSON-GA_OffsetList
template <typename FromType, typename ToType, typename INT_TYPE=exint>
class GA_API GA_ListTypeRef
{
protected:
    // These friend declarations are needed for accessing data in related types.
    template<typename FromType2,typename ToType2,typename INT_TYPE2>
    friend class GA_ListTypeRef;
    template<typename FromType2,typename ToType2,typename INT_TYPE2>
    friend class GA_ListType;
public:
    typedef INT_TYPE theIntType;
protected:

    using this_type = GA_ListTypeRef<FromType, ToType, INT_TYPE>;

    // The shareable data stored in a GA_OffsetList
    class GA_API ListTypeData
    {
    public:
        static ListTypeData *allocate(exint capacity)
        {
            // NOTE: We should really try to fix the cases where we're
            //       allocating this with capacity zero, but we have to
            //       validate that it's safe to switch it to trivial instead,
            //       given the comment in changeSize() about it needing to be
            //       safe to call GA_IndexMap::compactIndices() and
            //       GA_IndexMap::isTrivial() at the same time, and needing
            //       for isTrivial() to return false.
            //UT_ASSERT_MSG_P(capacity >= 1, "Why are we allocating something empty?");
            exint bytes = sizeof(ListTypeData) + sizeof(INT_TYPE)*capacity;
            ListTypeData *data = (ListTypeData *)malloc(bytes);
            data->myRefCount.relaxedStore(1);
            data->myCapacity = capacity;
#if GA_OFFSETLIST_VERBOSE_DEBUG
            printf("Allocating %p with ref count 1 and capacity %d\n", data, int(capacity));
            fflush(stdout);
#endif
            return data;
        }
        ListTypeData *reallocate(exint new_capacity)
        {
            // See NOTE above about fixing cases where we're allocating this
            // with capacity zero.
            //UT_ASSERT_MSG_P(new_capacity >= 1, "Why are we allocating something empty?");
            UT_ASSERT_P(myRefCount.relaxedLoad() == 1);

            ListTypeData *that = (ListTypeData *)realloc(this, sizeof(ListTypeData) + sizeof(INT_TYPE)*new_capacity);
            // NOTE: this may no longer be valid, so we can't use it after this point.

#if GA_OFFSETLIST_VERBOSE_DEBUG
            printf("Reallocating %p to %p with ref count %d from capacity %d to %d\n", this, that, int(that->myRefCount.relaxedLoad()), int(that->myCapacity), int(new_capacity));
            fflush(stdout);
#endif

            that->myCapacity = new_capacity;
            return that;
        }
        ListTypeData *bumpCapacity(exint mincapacity)
        {
            UT_ASSERT_P(myRefCount.relaxedLoad() == 1);
            if (myCapacity >= mincapacity)
                return this;

            mincapacity = SYSmax(mincapacity, UTbumpAlloc(myCapacity));
            ListTypeData *that = reallocate(mincapacity);
            // NOTE: this may no longer be valid, so we can't use it after this point.
            return that;
        }
        ListTypeData *setCapacity(exint new_capacity)
        {
            UT_ASSERT_P(myRefCount.relaxedLoad() == 1);
            if (myCapacity == new_capacity)
                return this;

            ListTypeData *that = reallocate(new_capacity);
            // NOTE: this may no longer be valid, so we can't use it after this point.
            return that;
        }
        ListTypeData *copy(exint size,exint new_capacity) const
        {
            ListTypeData *that = allocate(new_capacity);
            INT_TYPE *dest = that->getRawData();
            const INT_TYPE *src = getRawData();
            if (new_capacity < size)
                size = new_capacity;
            for (exint i = 0; i < size; ++i)
                dest[i] = src[i];
            return that;
        }

        // methods to manage sharing
        void ref() const
        {
#if GA_OFFSETLIST_VERBOSE_DEBUG
            exint new_count =
#endif
            myRefCount.add(1);
#if GA_OFFSETLIST_VERBOSE_DEBUG
            printf("Incrementing ref of %p with capacity %d from %d to %d\n", this, int(myCapacity), int(new_count-1), int(new_count));
            fflush(stdout);
            if (new_count < 0)
            {
                printf("!!! ERROR: NEGATIVE REF COUNT INCREMENTED on %p !!!", this);
                fflush(stdout);
            }
#endif
        }
        void unref()
        {
            exint new_count = myRefCount.add(-1);
#if GA_OFFSETLIST_VERBOSE_DEBUG
            printf("Decrementing ref of %p with capacity %d from %d to %d\n", this, int(myCapacity), int(new_count+1), int(new_count));
            fflush(stdout);
            if (new_count < 0)
            {
                printf("!!! ERROR: NEGATIVE REF COUNT DECREMENTED on %p !!!", this);
                fflush(stdout);
            }
#endif
            UT_ASSERT_P(new_count >= 0);
            if (new_count == 0)
            {
                free(this);
            }
        }
        bool isShared() const
        { return myRefCount.relaxedLoad() != 1; }

        SYS_FORCE_INLINE
        int64 getMemoryUsage(bool inclusive) const
        { return (inclusive ? sizeof(*this) : 0) + sizeof(INT_TYPE)*myCapacity; }

        void countMemory(UT_MemoryCounter &counter, bool inclusive) const;

        SYS_FORCE_INLINE
        GA_Size capacity() const
        { return myCapacity; }

        bool isTrivial(exint size) const
        {
            if (size > 1)
            {
                const INT_TYPE *start = getRawData();
                const INT_TYPE offset = start[0];
                for (INT_TYPE i = 1; i < size; i++)
                {
                    if (start[i] != offset+i)
                        return false;
                }
            }
            return true;
        }

        bool isAscending(exint size) const
        {
            if (size > 1)
            {
                const INT_TYPE *data = getRawData();
                INT_TYPE prev = data[0];
                for (exint i = 1; i < size; i++)
                {
                    INT_TYPE cur = data[i];
                    if (cur < prev)
                        return false;
                    prev = cur;
                }
            }
            return true;
        }

	// Set the entries...
        // in a bizarre way whose motivations I don't really understand.
        // Maybe someday I'll clean up the changing of size/capacity for GA_ListType.
	ListTypeData *setEntries(GA_Size sz, exint old_size, bool doresize=true);

	// adding and removing elements
        FromType insert(FromType index, ToType value, FromType size);
        FromType multipleInsert(FromType index, GA_Size count, FromType size);
        FromType remove(FromType i, FromType size)
        {
            if (i < FromType(0) || i >= size)
                return FromType(-1);

            INT_TYPE *start = getRawData();
            for (FromType j = i+1; j < size; ++j)
            {
                start[j-1] = start[j];
            }
            return i;
        }
        FromType findAndRemove(ToType v, FromType size)
        {
            INT_TYPE *array = getRawData();
            INT_TYPE *p = array;
            const INT_TYPE *end = array + size;
            for (; p != end; ++p)
            {
                if (ToType(*p) == v)
                {
                    FromType idx(p - array);
                    for (FromType j = idx+1; j < size; ++j)
                    {
                        array[j-1] = array[j];
                    }
                    return idx;
                }
            }
            return FromType(-1);
        }
        GA_Size removeAll(ToType v, FromType size)
        {
            INT_TYPE *start = getRawData();
            const INT_TYPE *src = start;
            const INT_TYPE *end = start + size;
            INT_TYPE *dest = start;
            for (; src != end; ++src)
            {
                if (*src != INT_TYPE(v))
                {
                    if (dest != src)
                        *dest = *src;
                    ++dest;
                }
            }
            return dest-start;
        }
        FromType find(ToType v, FromType s, FromType size) const
        {
            const INT_TYPE *array = getRawData();
            const INT_TYPE *p = array + s;
            const INT_TYPE *end = array + size;
            for (; p != end; ++p)
            {
                if (ToType(*p) == v)
                    return FromType(p - array);
            }
            return FromType(-1);
        }
        FromType findSorted(ToType v, FromType s, FromType size) const;

        // basic accessors
        SYS_FORCE_INLINE
        INT_TYPE &operator()(exint index)
        {
            return ((INT_TYPE *)(this+1))[index];
        }
        SYS_FORCE_INLINE
        const INT_TYPE &operator()(exint index) const
        {
            return ((const INT_TYPE *)(this+1))[index];
        }
        SYS_FORCE_INLINE
        void set(FromType index, ToType value)
        { (*this)(index) = value; }
        SYS_FORCE_INLINE
        ToType get(FromType index) const
        { return ToType((*this)(index)); }

        void constant(ToType value, FromType size)
        {
            INT_TYPE *start = getRawData();
            for (INT_TYPE i = 0; i < INT_TYPE(size); ++i)
                start[i] = INT_TYPE(value);
        }

        template<typename S>
        void set(const S *data, exint size, ToType offset)
        {
            UT_ASSERT_P(myCapacity >= size);
            INT_TYPE *array = getRawData();
            if (offset == ToType(0))
            {
                for (exint i = 0; i < size; ++i)
                    array[i] = ToType(data[i]);
            }
            else
            {
                for (exint i = 0; i < size; ++i)
                    array[i] = ToType(data[i]) + offset;
            }
        }
        template<typename S>
        void copyAdd(FromType destindex, const S *values, GA_Size srcindex, GA_Size n, ToType offset)
        {
            INT_TYPE *array = getRawData();
            for (GA_Size i = 0; i < n; ++i)
                array[i + destindex] = values[i + srcindex] + offset;
        }
        template<typename S>
        void copyAdd(FromType destindex, const GA_ListTypeRef<FromType, ToType, S> &values, FromType srcindex, GA_Size n, ToType offset)
        {
            INT_TYPE *array = getRawData();
            if (values.isTrivial())
            {
                ToType combined = values.myTrivialOffset + GA_Size(srcindex) + offset;
                for (GA_Size i = 0; i < n; ++i)
                    array[i + destindex] = combined + i;
            }
            else
            {
                const S *src = values.myData->getRawData();
                for (GA_Size i = 0; i < n; ++i)
                    array[i + destindex] = src[i + srcindex] + offset;
            }
        }

        void cycle(GA_Size howMany, FromType size)
        {
            // This looks silly, I know, but I didn't want to have to
            // verify that UT_Array::cycle goes in the same direction
            // as std::rotate, and cycle has some edge case handling,
            // e.g. for negative values.
            UT_Array<INT_TYPE> array;
            array.unsafeShareData(getRawData(), size);
            array.cycle(howMany);
            array.unsafeClearData();
        }
        void reverse(FromType size)
        {
            INT_TYPE *array = getRawData();
            std::reverse(array, array+size);
        }

        void sortAscending(FromType size)
        {
            INT_TYPE *array = getRawData();
            std::sort(array, array+size, std::less<INT_TYPE>());
        }

        FromType sortAndRemoveDuplicates(FromType size)
        {
            if (size <= FromType(1))
                return size;

            INT_TYPE *array = getRawData();
            INT_TYPE *end = array + size;
            std::sort(array, end, std::less<INT_TYPE>());

            // Sorted remove duplicates
            const INT_TYPE *src = array+1;
            INT_TYPE *dest = array+1;
            INT_TYPE prev = array[0];
            for (; src != end; ++src)
            {
                INT_TYPE cur = *src;
                if (cur != prev)
                {
                    if (dest != src)
                        *dest = cur;
                    prev = cur;
                    ++dest;
                }
            }
            return FromType(dest-array);
        }

	FromType findInRange(FromType start, FromType end, ToType search) const
	{
            const INT_TYPE *array = getRawData();
            for ( ; start < end; start++)
	    {
                if (ToType(array[start]) == search)
                    return start;
	    }
	    return end;
	}
	FromType findInRangeNotEqual(FromType start, FromType end, ToType search) const
	{
            const INT_TYPE *array = getRawData();
            for ( ; start < end; start++)
	    {
                if (ToType(array[start]) != search)
                    return start;
	    }
	    return end;
	}

	FromType findValidInRange(FromType start, FromType end) const
	{
            const INT_TYPE *array = getRawData();
            for ( ; start < end; start++)
	    {
                if (GAisValid(GA_Size(array[start])))
                    return start;
	    }
	    return end;
	}
	FromType findInvalidInRange(FromType start, FromType end) const
	{
            const INT_TYPE *array = getRawData();
            for ( ; start < end; start++)
	    {
                if (!GAisValid(GA_Size(array[start])))
		    return start;
	    }
	    return end;
	}

        SYS_FORCE_INLINE
        const INT_TYPE *getRawData() const
        {
            return (const INT_TYPE *)(this+1);
        }
        SYS_FORCE_INLINE
        INT_TYPE *getRawData()
        {
            return (INT_TYPE *)(this+1);
        }

    private:
	// We use a SYS_AtomicCounter to avoid losing references when multiple
	// threads add and remove references to the shared values
	mutable SYS_AtomicCounter myRefCount;
        exint myCapacity;
    };

public:
    /// Default constructor
    SYS_FORCE_INLINE
    explicit GA_ListTypeRef()
    {
        // Unlike in subclass, leave the data uninitialized until assigned,
        // because this class doesn't need to own its data.
    }

    /// Copy constructor
    SYS_FORCE_INLINE
    GA_ListTypeRef(const GA_ListTypeRef &src)
#if GA_OFFSETLIST_VERBOSE_DEBUG
    {
        if (!src.isTrivial())
        {
            printf("%p listref copy constructor with data %p from %p\n", this, src.myData, &src);
            fflush(stdout);
        }
        // NOTE: I'm assuming that it's expected that myIsFlagSet will be copied.
        memcpy(this, &src, sizeof(*this));
    }
#else
    = default;
#endif

    /// Move constructor
    SYS_FORCE_INLINE
    GA_ListTypeRef(GA_ListTypeRef &&src)
#if GA_OFFSETLIST_VERBOSE_DEBUG
    {
        if (!src.isTrivial())
        {
            printf("%p listref move constructor with data %p from %p\n", this, src.myData, &src);
            fflush(stdout);
        }
        memcpy(this, &src, sizeof(*this));
    }
#else
    = default;
#endif
private:
    /// Move constructor from subclass owning myData is forbidden
    GA_ListTypeRef(GA_ListType<FromType,ToType,INT_TYPE> &&src)
    {
        UT_ASSERT_MSG(0,"GA_ListTypeRef cannot be move-constructed from GA_ListType, because the data will be invalid momentarily.");
    }
public:

    /// Trivial list constructor
    SYS_FORCE_INLINE
    GA_ListTypeRef(ToType startvalue, GA_Size size, bool flag_set=false)
    {
        myIsTrivial = true;
        myTrivialOffset = startvalue;
        myIsFlagSet = flag_set;
        mySize = size;
    }

    /// Destructor
    SYS_FORCE_INLINE
    ~GA_ListTypeRef() = default;

    /// Copy assignment operator
    SYS_FORCE_INLINE
    GA_ListTypeRef &operator=(const GA_ListTypeRef &src)
#if GA_OFFSETLIST_VERBOSE_DEBUG
    {
        if (!src.isTrivial())
        {
            printf("%p listref copy constructor with data %p from %p\n", this, src.myData, &src);
            fflush(stdout);
        }
        // NOTE: I'm assuming that it's expected that myIsFlagSet will be copied.
        memcpy(this, &src, sizeof(*this));
        return *this;
    }
#else
    = default;
#endif

    /// Move assignment operator
    SYS_FORCE_INLINE
    GA_ListTypeRef &operator=(GA_ListTypeRef &&src)
#if GA_OFFSETLIST_VERBOSE_DEBUG
    {
        if (!src.isTrivial())
        {
            printf("%p listref move constructor with data %p from %p\n", this, src.myData, &src);
            fflush(stdout);
        }
        // NOTE: I'm assuming that it's expected that myIsFlagSet will be copied.
        memcpy(this, &src, sizeof(*this));
        return *this;
    }
#else
    = default;
#endif
private:
    /// Move constructor from subclass owning myData is forbidden
    GA_ListTypeRef &operator=(GA_ListType<FromType,ToType,INT_TYPE> &&src)
    {
        UT_ASSERT_MSG(0,"GA_ListTypeRef cannot be move-assigned from GA_ListType, because the data will be invalid momentarily.");
        return *this;
    }
public:

    /// clear removes all of the entries
    SYS_FORCE_INLINE
    void clear()
    {
        // NOTE: Subclass clear will unref myData if non-trivial.
        myIsTrivial = true;
        // NOTE: DON'T set myIsFlagSet, since it's independent of whether
        //       this list is clear.
        myTrivialOffset = 0;
        mySize = 0;
    }

    /// Makes the list a trivial list with the specified start value and size
    SYS_FORCE_INLINE
    void setTrivial(ToType startvalue, GA_Size size)
    {
        UT_ASSERT(size >= 0);
        myIsTrivial = true;
        myTrivialOffset = startvalue;
        mySize = size;
    }

    /// Makes the list a trivial list with the specified start value and size,
    /// and also sets the extra flag.
    SYS_FORCE_INLINE
    void setTrivial(ToType startvalue, GA_Size size, bool flag)
    {
        UT_ASSERT(size >= 0);
        myIsTrivial = true;
        myTrivialOffset = startvalue;
        mySize = size;
        myIsFlagSet = flag;
    }

    /// Returns the allocated capacity of the list
    SYS_FORCE_INLINE
    GA_Size capacity() const
    {
        return isTrivial() ? 0 : myData->capacity();
    }
    /// Returns the number of used elements in the list (always <= capacity())
    SYS_FORCE_INLINE
    FromType entries() const
    { return size(); }
    /// Returns the number of used elements in the list (always <= capacity())
    SYS_FORCE_INLINE
    FromType size() const
    {
        return FromType(mySize);
    }
    /// Check whether this offset list is stored in a compact
    /// (non-allocated) form.
    /// NOTE: It *must* be threadsafe to call this while hardening
    ///       a non-trivial list and get false as a return value.
    ///       GA_IndexMap::compactIndices() may be called at the same
    ///       time as GA_IndexMap::isTrivial(), and this *must*
    ///       return false, no matter what the race.
    SYS_FORCE_INLINE
    bool	isTrivial() const	{ return myIsTrivial != 0; }
    SYS_FORCE_INLINE
    bool	getExtraFlag() const	{ return myIsFlagSet != 0; }
    SYS_FORCE_INLINE
    void	setExtraFlag(bool v)	{ myIsFlagSet = v; }

    // Returns true iff this and that are either both trivial and equal
    // or if both are not trivial and share the same ListTypeData pointer.
    // This does not fully check for equality!
    bool isSame(const GA_ListTypeRef &that) const
    {
        if (isTrivial())
        {
            return (that.isTrivial() &&
                    (mySize == that.mySize) &&
                    (myTrivialOffset == that.myTrivialOffset));
        }
        return (myData == that.myData);
    }

    /// Identifies whether the data is in ascending order
    bool	isAscending() const;

    /// Linearly search for the specified entry.  Returns the index of first
    /// matching element found, -1 otherwise.  An optional starting index can
    /// be specified in s.
    FromType	find(ToType value, FromType s = FromType(0)) const;

    /// Find the target in a sorted list.
    FromType	findSorted(ToType value, FromType s=FromType(0)) const;

    /// Get the the value at the index
    SYS_FORCE_INLINE
    ToType	get(FromType index) const
    {
        UT_ASSERT_P(index >= FromType(0) && index < size());
        return isTrivial()
                ? ToType(GA_Size(index)+myTrivialOffset)
                : myData->get(index);
    }

    /// Returns the start, assuming this list is trivial.
    SYS_FORCE_INLINE
    ToType trivialStart() const
    {
        UT_ASSERT_P(isTrivial());
        return ToType(myTrivialOffset);
    }

    /// Test a sub-block for equality with another list
    bool	isEqual(const GA_ListTypeRef &other,
			FromType start, FromType end) const;

    /// Return the value of the last element
    ToType	last() const
		{
                    exint last_index = mySize-1;
		    return isTrivial()
			    ? ToType(myTrivialOffset+last_index)
			    : myData->get(FromType(last_index));
		}

    /// Convenience () operator to access the list entries
    SYS_FORCE_INLINE
    ToType	operator()(FromType i) const { return get(i); }

    SYS_FORCE_INLINE
    ToType	operator[](FromType i) const { return get(i); }

    /// Finds the first instance of the search pattern in the given
    /// range.  Returns end if not found.
    FromType	findInRange(FromType start, FromType end, ToType search) const
    {
	if (isTrivial())
	{
	    FromType matchingindex = FromType(GA_Size(search) - GA_Size(myTrivialOffset));
	    if (matchingindex >= start && matchingindex < end)
		return matchingindex;
	    // No match.
	    return end;
	}
	return myData->findInRange(start, end, search);
    }
    FromType	findInRangeNotEqual(FromType start, FromType end, ToType search) const
    {
	if (isTrivial())
	{
	    FromType matchingindex = FromType(GA_Size(search) - GA_Size(myTrivialOffset));
	    // start is always a match. If not, start+1 is.
	    if (matchingindex != start)
		return start;
	    start += 1;
	    if (start > end)
		return end;
	    return start;
	}
	return myData->findInRangeNotEqual(start, end, search);
    }
    FromType	findValidInRange(FromType start, FromType end) const
    {
	if (isTrivial())
	{
	    // NOTE: We can have trivial lists with negative offsets!
            // If we have a valid trivial offset, everything matches.
            if (GAisValid(myTrivialOffset))
                return start;
            if (GAisValid(ToType(GA_Size(start)) + ToType(myTrivialOffset)))
                return start;
	    return SYSmin(end, FromType(-GA_Size(myTrivialOffset)));
	}
	return myData->findValidInRange(start, end);
    }
    FromType	findInvalidInRange(FromType start, FromType end) const
    {
	if (isTrivial())
	{
	    // NOTE: We can have trivial lists with negative offsets!
            // If we have a valid trivial offset or start, nothing matches.
            if (GAisValid(myTrivialOffset))
                return end;
            if (GAisValid(ToType(GA_Size(start)) + ToType(myTrivialOffset)))
                return end;
            return start;
	}
	return myData->findInvalidInRange(start, end);
    }

    /// Calls a functor (e.g. a lambda) for each entry, only checking
    /// for triviality once, to reduce overhead.
    template<typename FUNCTOR>
    SYS_FORCE_INLINE
    void forEach(FUNCTOR &&functor) const
    {
        if (myIsTrivial)
        {
            ToType value(myTrivialOffset);
            const ToType end(value + mySize);
            for (; value != end; ++value)
            {
                functor(value);
            }
        }
        else
        {
            const INT_TYPE *data = myData->getRawData();
            const INT_TYPE *const end = data + mySize;
            for (; data != end; ++data)
            {
                functor(ToType(*data));
            }
        }
    }

    /// @note It's likely more efficient to use @c forEach() than iterators
    template <typename LIST_T>
    class base_iterator:
	public std::iterator<std::random_access_iterator_tag,
	       ToType, FromType>
    {
	public:
	    using reference = ToType;

	    base_iterator()
		: myList(nullptr)
		, myCurrent(-1)
	    {
	    }

	    template <typename EIT>
	    base_iterator(const base_iterator<EIT> &src)
		: myList(src.myList)
		, myCurrent(src.myCurrent)
	    {
	    }
	    reference	operator*() const
	    {
		return myList->get(myCurrent);
	    }
	    reference	item() const
	    {
		return myList->get(myCurrent);
	    }
	    reference	operator[](FromType n) const
	    {
		return myList->get(myCurrent+n);
	    }
	    base_iterator	&operator++()
	    {
		++myCurrent;
		return *this;
	    }
	    base_iterator	operator++(int)
	    {
		base_iterator	tmp = *this;
		++myCurrent;
		return tmp;
	    }
	    base_iterator	&operator--()
	    {
		--myCurrent;
		return *this;
	    }
	    base_iterator	operator--(int)
	    {
		base_iterator	tmp = *this;
		--myCurrent;
		return tmp;
	    }
	    base_iterator	&operator+=(FromType n)
	    {
		myCurrent += n;
		return *this;
	    }
	    base_iterator	operator+(FromType n) const
	    {
		return base_iterator(myList, myCurrent+n);
	    }
	    base_iterator	&operator-=(FromType n)
				    { return (*this) += (-n); }
	    base_iterator	 operator-(FromType n) const
				    { return (*this) + (-n); }
	    template <typename LT>
	    bool	operator==(const base_iterator<LT> &r) const
			    { return r.myCurrent == myCurrent; }
	    template <typename LT>
	    bool	operator!=(const base_iterator<LT> &r) const
			    { return r.myCurrent != myCurrent; }

	    #define CMP_OP(OP) \
		template <typename LT> \
		bool	operator OP(const base_iterator<LT> &r) const { \
		    return myCurrent OP r.myCurrent; \
		}
	    CMP_OP(<)
	    CMP_OP(>)
	    CMP_OP(<=)
	    CMP_OP(>=)
	    #undef CMP_OP
	    template <typename LT>
            FromType	operator-(const base_iterator<LT> &r) const
			    { return (myCurrent - r.myCurrent); }
	protected:
	    friend class GA_ListTypeRef<FromType, ToType, INT_TYPE>;
	    base_iterator(LIST_T *list, FromType c)
		: myList(list)
		, myCurrent(c)
	    {
	    }
	private:
	    LIST_T	*myList;
            FromType	 myCurrent;
    };
    using iterator = base_iterator<this_type>;
    using const_iterator = base_iterator<const this_type>;

    /// @{
    /// @note It's likely more efficient to use @c forEach() than iterators
    iterator		begin() { return iterator(this, FromType(0)); }
    iterator		end() { return iterator(this, size()); }
    const_iterator	begin() const { return const_iterator(this, FromType(0)); }
    const_iterator	end() const { return const_iterator(this, size()); }
    /// @}

    /// Report memory usage (includes all shared memory)
    SYS_FORCE_INLINE
    int64 getMemoryUsage(bool inclusive) const
    {
        return (inclusive ? sizeof(*this) : 0) + (!isTrivial() ? myData->getMemoryUsage(true) : 0);
    }

    SYS_FORCE_INLINE
    const INT_TYPE *getArray() const
    {
        return !isTrivial() ? myData->getRawData() : nullptr;
    }
    SYS_FORCE_INLINE
    INT_TYPE *getArray()
    {
        return !isTrivial() ? myData->getRawData() : nullptr;
    }

protected:
    static const intptr_t POINTER_MASK = ~0x1;
    static const intptr_t TRIVIAL_MASK = 0x1;
    static const intptr_t FLAG_MASK = 0x1;
#ifndef SESI_LITTLE_ENDIAN
#error "Make sure the bitfields in the union work on big endian platforms!"
#endif
    union {
        ListTypeData	*myData;
        struct {
            // NOTE: myTrivialOffset must be signed to support
            //       GA_INVALID_OFFSET and GA_INVALID_INDEX, but
            //       mySize can be unsigned.
            int64       myIsTrivial:1;
            int64       myTrivialOffset:63;
            // Make sure that this flag doesn't overlap with the
            // pointer, so that it doesn't need to be considered
            // when reading/writing myData.  myIsTrivial
            // can overlap with myData, because it's
            // mutually exclusive with using myData.
            uint64      myIsFlagSet:1;
            uint64      mySize:63;
        };
    };
};

template <typename FromType, typename ToType, typename INT_TYPE=exint>
class GA_API GA_ListType : public GA_ListTypeRef<FromType, ToType, INT_TYPE>
{
private:
    // These friend declarations are needed for accessing data in related types.
    template<typename FromType2,typename ToType2,typename INT_TYPE2>
    friend class GA_ListTypeRef;
    template<typename FromType2,typename ToType2,typename INT_TYPE2>
    friend class GA_ListType;

    typedef GA_ListTypeRef<FromType, ToType, INT_TYPE> Base;
protected:
    using Base::myData;
    using Base::myIsTrivial;
    using Base::myTrivialOffset;
    using Base::myIsFlagSet;
    using Base::mySize;
    using Base::POINTER_MASK;
    using Base::TRIVIAL_MASK;
    using Base::FLAG_MASK;
    // I don't know why one build machine running GCC 4.8 needed
    // "ListTypeData =" and the other didn't, but I've put it here
    // anyway, because it should keep everyone happy.
    using ListTypeData = typename Base::ListTypeData;
public:
    using Base::get;
    using Base::isTrivial;
    using Base::size;
    using Base::capacity;
    using typename Base::theIntType;
public:
    /// Default constructor
    SYS_FORCE_INLINE
    explicit GA_ListType()
        : Base()
    {
        myIsTrivial = true;
        myTrivialOffset = 0;
        myIsFlagSet = false;
        mySize = 0;
    }

    /// Copy constructor
    SYS_FORCE_INLINE
    GA_ListType(const GA_ListType &src)
        : Base()
    {
        if (!src.isTrivial())
        {
#if GA_OFFSETLIST_VERBOSE_DEBUG
            printf("%p adding reference to %p (orig reference by list %p) in copy constructor\n", this, src.myData, &src);
            fflush(stdout);
#endif
            src.myData->ref();
        }
        // NOTE: I'm assuming that it's expected that myIsFlagSet will be copied.
        Base::operator=(src);
    }

    /// Move constructor
    SYS_FORCE_INLINE
    GA_ListType(GA_ListType &&src)
        : Base()
    {
#if GA_OFFSETLIST_VERBOSE_DEBUG
        if (!src.isTrivial())
        {
            printf("%p stealing reference to %p (orig reference by list %p) in move constructor\n", this, src.myData, &src);
            fflush(stdout);
        }
#endif
        Base::operator=(src);
        src.myIsTrivial = true;
    }

    /// Copy constructor from GA_ListTypeRef.
    /// Although it may seem strange to have this at all, it should be
    /// safe, since the destination does take (shared) ownership of any
    /// non-trivial data.  There should be a GA_ListType somewhere else
    /// that already owns this.
    SYS_FORCE_INLINE
    explicit GA_ListType(const Base &src)
        : Base()
    {
        if (!src.isTrivial())
        {
#if GA_OFFSETLIST_VERBOSE_DEBUG
            printf("%p adding reference to %p (orig reference by listref %p) in list(listref) type conversion constructor\n", this, src.myData, &src);
            fflush(stdout);
#endif
            src.myData->ref();
            UT_ASSERT_MSG_P(src.myData->isShared(), "Something should have already owned this data.");
        }
        // NOTE: I'm assuming that it's expected that myIsFlagSet will be copied.
        Base::operator=(src);
    }

    /// Trivial list constructor
    SYS_FORCE_INLINE
    GA_ListType(ToType startvalue, GA_Size size, bool flag_set=false)
        : Base(startvalue, size, flag_set)
    {}

    // This will construct a GA_ListType by copying a portion of the array.
    GA_ListType(const UT_Array<ToType> &src, FromType start=FromType(0), FromType end=FromType(-1));

    /// Destructor
    SYS_FORCE_INLINE
    ~GA_ListType()
    {
        if (!isTrivial())
        {
#if GA_OFFSETLIST_VERBOSE_DEBUG
            printf("%p removing reference to %p in desctructor\n", this, myData);
            fflush(stdout);
#endif
            myData->unref();
        }
    }

    /// Copy assignment operator
    SYS_FORCE_INLINE
    GA_ListType &operator=(const GA_ListType &src)
    {
        if (!isTrivial())
        {
#if GA_OFFSETLIST_VERBOSE_DEBUG
            printf("%p removing reference to %p in copy assignment operator\n", this, myData);
            fflush(stdout);
#endif
            myData->unref();
        }
        if (!src.isTrivial())
        {
#if GA_OFFSETLIST_VERBOSE_DEBUG
            printf("%p adding reference to %p (orig reference by list %p) in copy assignment operator\n", this, src.myData, &src);
            fflush(stdout);
#endif
            src.myData->ref();
        }
        // NOTE: I'm assuming that it's expected that myIsFlagSet will be copied.
        Base::operator=(src);
        return *this;
    }

    /// Move assignment operator
    SYS_FORCE_INLINE
    GA_ListType &operator=(GA_ListType &&src)
    {
        if (!isTrivial())
        {
#if GA_OFFSETLIST_VERBOSE_DEBUG
            printf("%p removing reference to %p in move assignment operator\n", this, myData);
            fflush(stdout);
#endif
            myData->unref();
        }
#if GA_OFFSETLIST_VERBOSE_DEBUG
        if (!src.isTrivial())
        {
            printf("%p stealing reference to %p (orig reference by list %p) in move assignment operator\n", this, src.myData, &src);
            fflush(stdout);
        }
#endif
        // NOTE: I'm assuming that it's expected that myIsFlagSet will be copied.
        Base::operator=(src);
        src.myIsTrivial = true;
        return *this;
    }

    /// Copy assignment operator from GA_ListTypeRef.
    /// Although it may seem strange to have this at all, it should be
    /// safe, since the destination does take (shared) ownership of any
    /// non-trivial data.  There should be a GA_ListType somewhere else
    /// that already owns this.
    SYS_FORCE_INLINE
    GA_ListType &operator=(const Base &src)
    {
        if (!isTrivial())
        {
#if GA_OFFSETLIST_VERBOSE_DEBUG
            printf("%p removing reference to %p in copy-from-listref assignment operator\n", this, myData);
            fflush(stdout);
#endif
            myData->unref();
        }
        if (!src.isTrivial())
        {
#if GA_OFFSETLIST_VERBOSE_DEBUG
            printf("%p adding reference to %p (orig reference by listref %p) in copy-from-listref assignment operator\n", this, src.myData, &src);
            fflush(stdout);
#endif
            src.myData->ref();
            UT_ASSERT_MSG_P(src.myData->isShared(), "Something should have already owned this data.");
        }
        // NOTE: I'm assuming that it's expected that myIsFlagSet will be copied.
        Base::operator=(src);
        return *this;
    }

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    /// If inclusive is true, the size of this object is counted,
    /// else only memory owned by this object is counted.
    /// If this is pointed to by the calling object, inclusive should be true.
    /// If this is contained in the calling object, inclusive should be false.
    /// (Its memory was already counted in the size of the calling object.)
    void	countMemory(UT_MemoryCounter &counter, bool inclusive) const;

    /// clear removes all of the entries
    SYS_FORCE_INLINE
    void clear()
    {
        if (!isTrivial())
        {
#if GA_OFFSETLIST_VERBOSE_DEBUG
            printf("%p removing reference to %p in clear function\n", this, myData);
            fflush(stdout);
#endif
            myData->unref();
        }
        myIsTrivial = true;
        // NOTE: DON'T set myIsFlagSet, since it's independent of whether
        //       this list is clear.
        myTrivialOffset = 0;
        mySize = 0;
    }

    /// Identifies whether the data is a trivial mapping and if so,
    /// eliminates storage for the map.
    void computeTrivial();

    /// Makes the list a trivial list with the specified start value and size
    void setTrivial(ToType startvalue, GA_Size size)
    {
        UT_ASSERT(size >= 0);
        if (!isTrivial())
        {
#if GA_OFFSETLIST_VERBOSE_DEBUG
            printf("%p removing reference to %p in setTrivial(start,size)\n", this, myData);
            fflush(stdout);
#endif
            myData->unref();
            myIsTrivial = true;
        }
        myTrivialOffset = startvalue;
        mySize = size;
    }

    /// Makes the list a trivial list with the specified start value and size,
    /// and also sets the extra flag.
    void setTrivial(ToType startvalue, GA_Size size, bool flag)
    {
        UT_ASSERT(size >= 0);
        if (!isTrivial())
        {
#if GA_OFFSETLIST_VERBOSE_DEBUG
            printf("%p removing reference to %p in setTrivial(start,size,flag)\n", this, myData);
            fflush(stdout);
#endif
            myData->unref();
            myIsTrivial = true;
        }
        myTrivialOffset = startvalue;
        mySize = size;
        myIsFlagSet = flag;
    }

    /// Set the number of entries - if the number is smaller than the current
    /// entries, the capacity may be decreased. If doresize is false,
    /// only the number of entries is changed, not the size.
    void	setEntries(FromType sz, bool doresize=true);
    /// Reserve capacity for a number of entries (prevents reallocation as
    /// elements are appended).
    /// This does nothing if the list is currently trivial.
    void	reserve(FromType mincapacity); // Reserve capacity

                                               /// If trivial, this sets the size to the minimum of the current size and new_capacity.
                                               /// If not trivial, this sets the capacity to new_capacity, (also correctly decreasing
                                               /// the size if currently greater than new_capacity.)
    void	changeSize(FromType new_capacity);

    /// Add a single entry (may grow array)
    FromType append(ToType value)
    {
        if (isTrivial())
        {
            if (!mySize)
            {
                myTrivialOffset = value;
                mySize = 1;
                return FromType(0);
            }
            if (value == ToType(myTrivialOffset) + mySize)
            {
                ++mySize;
                return FromType(mySize-1);
            }
        }
        harden(mySize+1);
        myData->set(FromType(mySize), value);
        ++mySize;
        return FromType(mySize-1);
    }

    /// Append all the entries from the source list.  The offset will be added
    /// to each value in the source list.
    void append(const GA_ListType &src);

    /// Insert a single entry (may grow array)
    FromType	insert(FromType i, ToType value);

    /// Insert count entries at the given index (may grow array).  The new
    /// entries will be uninitialized.
    FromType	multipleInsert(FromType i, GA_Size count);

    /// Remove the entry at the given offset
    FromType	remove(FromType	i);
    /// Find an entry and remove it from the list
    FromType	findAndRemove(ToType i);
    /// Alias for remove to match UT array types
    FromType	removeIndex(FromType i)	{ return remove(i); }
    /// Remove all matching entries from the list
    GA_Size	removeAll(ToType i);
    /// Remove the last entry
    void        removeLast()
    {
        UT_ASSERT_P(mySize > 0);
        --mySize;
    }

    /// Sort entries into ascending order
    void	sortAscending();

    /// Sort entries into ascending order and remove duplicates
    void	sortAndRemoveDuplicates();

    /// Set the index to the value
    void set(FromType index, ToType value)
    {
        UT_ASSERT_P(index >= FromType(0) && index < FromType(mySize));
        if (isTrivial())
        {
            if (myTrivialOffset+GA_Size(index) == GA_Size(value))
                return;
            if (mySize==1)
            {
                myTrivialOffset = GA_Size(value);
                return;
            }
        }

        harden();
        myData->set(index, value);
    }

    template<typename S>
    void set(const S *data, exint size, ToType offset)
    {
        clear();

        if (size == 0)
            return;

        // Check for triviality first
        ToType first = ToType(data[0]);
        bool istrivial = true;
        for (int64 i = 1; i < size; ++i)
        {
            if (ToType(data[i]) != first + ToType(i))
            {
                istrivial = false;
                break;
            }
        }

        if (istrivial)
            setTrivial(first+offset, FromType(size));
        else
        {
            UT_ASSERT_COMPILETIME(sizeof(GA_ListType<FromType,ToType>)==16);
            ListTypeData *newdata = ListTypeData::allocate(size);
            UT_ASSERT_P((intptr_t(newdata) & TRIVIAL_MASK) == 0);
            newdata->set(data, size, offset);
            mySize = size;
            myData = newdata;
        }
    }

    void        copyAdd(FromType destindex, const int *values, GA_Size srcindex, GA_Size n, ToType offset)
    {
        if (isTrivial())
        {
            bool ismatch = true;
            for (GA_Size i = 0; i < n; ++i)
            {
                if (myTrivialOffset + GA_Size(destindex) + i != values[srcindex + i] + GA_Size(offset))
                {
                    ismatch = false;
                    break;
                }
            }
            if (ismatch)
                return;
        }
        harden();
        myData->copyAdd(destindex, values, srcindex, n, offset);
    }
    void        copyAdd(FromType destindex, const GA_ListTypeRef<FromType, ToType> &values, FromType srcindex, GA_Size n, ToType offset)
    {
        if (isTrivial())
        {
            if (values.isTrivial() && myTrivialOffset + destindex == values.myTrivialOffset + srcindex)
                return;

            bool ismatch = true;
            for (GA_Size i = 0; i < n; ++i)
            {
                if (myTrivialOffset + GA_Size(destindex) + i != GA_Size(values(srcindex + i)) + GA_Size(offset))
                {
                    ismatch = false;
                    break;
                }
            }
            if (ismatch)
                return;
        }
        harden();
        myData->copyAdd(destindex, values, srcindex, n, offset);
    }
    void        setTrivialRange(FromType startindex, ToType startvalue, GA_Size nelements)
    {
        FromType endindex = startindex + nelements;
        if (isTrivial())
        {
            if (startindex == FromType(0) && nelements >= mySize)
            {
                myTrivialOffset = startvalue;
                mySize = nelements;
                return;
            }
            if (myTrivialOffset+GA_Size(startindex) == GA_Size(startvalue))
            {
                if (endindex > FromType(mySize))
                    mySize = endindex;
                return;
            }
        }
        else if (startindex == FromType(0) && nelements >= mySize)
        {
            setTrivial(startvalue, nelements);
            return;
        }

        exint new_size = SYSmax(exint(endindex), exint(mySize));
        harden(new_size);
        mySize = new_size;
        ListTypeData *data = myData;
        for (GA_Size i = 0; i < nelements; ++i)
            data->set(startindex + i, startvalue + i);
    }

    /// Set all entries to a constant value
    void	constant(ToType value);

    /// Cyclically shift the entire array by howMany
    void	cycle(GA_Size howMany);

    /// Reverse the entire array
    void	reverse();

    void harden()
    {
        if (isTrivial())
        {
            UT_ASSERT_COMPILETIME(sizeof(GA_ListType<FromType,ToType>)==16);
            ListTypeData *data = ListTypeData::allocate(mySize);
            UT_ASSERT_P((intptr_t(data) & TRIVIAL_MASK) == 0);
            INT_TYPE *array = data->getRawData();
            for (exint i = 0; i < exint(mySize); i++)
                array[i] = ToType(myTrivialOffset+i);
#if GA_OFFSETLIST_VERBOSE_DEBUG
            printf("%p taking ownership of new %p in harden()\n", this, data);
            fflush(stdout);
#endif
            myData = data;
        }
        else if (myData->isShared())
        {
            // ensure we have a ListTypeData and that isn't referenced by
            // any other GA_ListType
            ListTypeData *data = myData->copy(mySize, mySize);
#if GA_OFFSETLIST_VERBOSE_DEBUG
            printf("%p removing reference to %p in harden(), and taking ownership of new %p\n", this, myData, data);
            fflush(stdout);
#endif
            myData->unref();

            // NOTE: DO NOT set myData to NULL, even temporarily, because it
            //       *must* be threadsafe to call isTrivial() while hardening
            //       a non-trivial list and get false as a return value.
            //       GA_IndexMap::compactIndices() may be called at the same
            //       time as GA_IndexMap::isTrivial(), and isTrivial() *must*
            //       return false, no matter what the race.
            myData = data;
        }
        UT_ASSERT_P(!isTrivial() && !myData->isShared());
    }

    void harden(exint mincapacity)
    {
        if (isTrivial())
        {
            UT_ASSERT_COMPILETIME(sizeof(GA_ListType<FromType,ToType>)==16);
            ListTypeData *data = ListTypeData::allocate(mincapacity);
            UT_ASSERT_P((intptr_t(data) & TRIVIAL_MASK) == 0);
            INT_TYPE *array = data->getRawData();
            if (mincapacity < mySize)
                mySize = mincapacity;
            for (exint i = 0; i < exint(mySize); i++)
                array[i] = ToType(myTrivialOffset+i);
#if GA_OFFSETLIST_VERBOSE_DEBUG
            printf("%p taking ownership of new %p in harden(mincapacity)\n", this, data);
            fflush(stdout);
#endif
            myData = data;
        }
        else if (myData->isShared())
        {
            // ensure we have a ListTypeData and that isn't referenced by
            // any other GA_ListType
            ListTypeData *data = myData->copy(mySize, mincapacity);
#if GA_OFFSETLIST_VERBOSE_DEBUG
            printf("%p removing reference to %p in harden(mincapacity), and taking ownership of new %p\n", this, myData, data);
            fflush(stdout);
#endif
            myData->unref();

            // NOTE: DO NOT set myData to NULL, even temporarily, because it
            //       *must* be threadsafe to call isTrivial() while hardening
            //       a non-trivial list and get false as a return value.
            //       GA_IndexMap::compactIndices() may be called at the same
            //       time as GA_IndexMap::isTrivial(), and isTrivial() *must*
            //       return false, no matter what the race.
            myData = data;

            if (mincapacity < mySize)
                mySize = mincapacity;
        }
        else if (mincapacity > myData->capacity())
        {
#if GA_OFFSETLIST_VERBOSE_DEBUG
            printf("%p bumping capacity of %p, current capacity = %d in harden(mincapacity=%d)\n", this, myData, int(mincapacity), int(myData->capacity()));
#endif
            myData = myData->bumpCapacity(mincapacity);
#if GA_OFFSETLIST_VERBOSE_DEBUG
            printf("                   now %p      new capacity = %d\n", myData, int(myData->capacity()));
            fflush(stdout);
#endif
        }
        UT_ASSERT_P(!isTrivial() && !myData->isShared());
    }

    /// WARNING: PLEASE DO NOT CALL THIS UNLESS YOU KNOW WHAT YOU'RE DOING!
    ///          IF YOU'RE UNSURE, YOU DON'T NEED TO CALL THIS!
    /// Only call this if it's necessary to copy a GA_ListType and share
    /// its ownership in some way that doesn't correctly update the
    /// ref count for non-trivial lists.
    /// (This was added for GA_PrimitiveList to represent a GA_OffsetList
    /// with a UT_FixedVector<int64,2>.)
    void incDataRef()
    {
        if (!isTrivial())
        {
#if GA_OFFSETLIST_VERBOSE_DEBUG
            printf("%p adding reference to %p in incDataRef()\n", this, myData);
            fflush(stdout);
#endif
            myData->ref();
        }
    }
};


// A list of offsets.  The index type is still specified as a template
// parameter.
template <typename FromType, typename INT_TYPE=exint>
class GA_API GA_OffsetListType : public GA_ListType<FromType, GA_Offset, INT_TYPE>
{
private:
    // Shorthand for the base class
    typedef GA_ListType<FromType, GA_Offset, INT_TYPE> Base;
    using Base::myData;
    using Base::myIsFlagSet;
    using Base::myIsTrivial;
    using Base::myTrivialOffset;
    using Base::mySize;
public:
    using Base::clear;
    using Base::get;
    using Base::isTrivial;
    using Base::set;
    using Base::size;
    using typename Base::theIntType;

    // Default c-tor
    explicit GA_OffsetListType() : Base() {}
    // Copy c-tor
    GA_OffsetListType(const GA_OffsetListType &src)
	: Base(src) {}
    explicit GA_OffsetListType(const GA_ListTypeRef<FromType, GA_Offset, INT_TYPE> &src)
        : Base(src) {}
    // A GA_OffsetArray is a UT_Array of offsets.  The UT_Array doesn't have
    // paged storage, nor COW semantics, it's just a flat list of offsets.
    // This will construct a GA_OffsetListType by copying a portion of the array.
    GA_OffsetListType(const UT_ValArray<GA_Offset> &src, FromType start=FromType(0), FromType end=FromType(-1))
	: Base(src, start, end) {}

    /// Copy assignment operator
    SYS_FORCE_INLINE
    GA_OffsetListType &operator=(const GA_OffsetListType &src) = default;

    /// Move assignment operator
    SYS_FORCE_INLINE
    GA_OffsetListType &operator=(GA_OffsetListType &&src) = default;

    /// Copy assignment operator from GA_ListTypeRef.
    /// Although it may seem strange to have this at all, it should be
    /// safe, since the destination does take (shared) ownership of any
    /// non-trivial data.  There should be a GA_OffsetListType somewhere else
    /// that already owns this.
    SYS_FORCE_INLINE
    GA_OffsetListType &operator=(const GA_ListTypeRef<FromType, GA_Offset, INT_TYPE> &src)
    {
        Base::operator=(src);
        return *this;
    }

    /// @{
    /// Swap offset values for defragmentation process.  When defragmenting,
    /// offsets will move.  This method will update all references to offsets
    /// to their new values.  This returns the number of values updated.
    GA_Size	swapOffsetValues(const GA_Defragment &defrag);
    /// @}

    /// @section JSON-GA_OffsetList JSON Schema: GA_OffsetList
    /// @code
    /// {
    ///     "name"              : "GA_OffsetList",
    ///     "description"       : "An array of offsets/indicies",
    ///     "type"              : "array",
    ///     "items"             : "integer",
    /// }
    /// @endcode
    /// @see @ref JSON_FileFormat

    /// Save the offsets by doing the mapping to the points in the save map
    bool	jsonPointArray(UT_JSONWriter &w, const GA_SaveMap &save) const;
    /// Save the offsets by doing the mapping to the vertices in the save map
    bool	jsonVertexArray(UT_JSONWriter &w, const GA_SaveMap &save) const;
    /// Save the offsets by doing the mapping to the primitives in the save map
    bool	jsonPrimitiveArray(UT_JSONWriter &w, const GA_SaveMap &save) const;

    /// Load a point list from a JSON stream
    bool	jsonPointArray(UT_JSONParser &p, const GA_LoadMap &load);
    /// Load a vertex list from a JSON stream
    bool	jsonVertexArray(UT_JSONParser &p, const GA_LoadMap &load);
    /// Load a primitive list from a JSON stream
    bool	jsonPrimitiveArray(UT_JSONParser &p, const GA_LoadMap &load);

    /// Generic load given a load offset
    bool	jsonLoad(UT_JSONParser &p, GA_Offset load_offset);
    /// Load, appending to the current offset list without initial clear
    bool	jsonLoadAppend(UT_JSONParser &p, GA_Offset load_offset);
    /// Generic load from a JSON stream saved by index that maps the ordered
    /// index to an offset.
    bool	jsonLoadByIndex(UT_JSONParser &p, const GA_IndexMap &index_map,
				GA_Index load_offset);
    /// Load from a JSON stream saved by index, mapping the ordered index to 
    /// an offset and appending to the current offset list without an initial
    /// clear.
    bool	jsonLoadAppendByIndex(UT_JSONParser &p,
				      const GA_IndexMap &index_map, 
				      GA_Index load_offset);

private:
    bool jsonSaveTranslatedArray(
        UT_JSONWriter &w,
        const GA_SaveMap &save,
        GA_AttributeOwner xlate) const;
};

/// GA_OffsetList is a map from index to offset
typedef GA_OffsetListType<GA_Size> GA_OffsetList;

/// GA_OffsetListRef is like GA_OffsetList, except that it doesn't own
/// its data.  It's useful for making a temporary copy of a GA_OffsetList
/// that isn't going to be changing during the lifetime of the copy,
/// to avoid having to ref and unref myData, among a few related speedups.
typedef GA_ListTypeRef<GA_Size,GA_Offset> GA_OffsetListRef;

#endif

