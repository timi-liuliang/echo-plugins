/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_DataArrayPageTableImpl.h (GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_DATAARRAYPAGETABLEIMPL_H_INCLUDED__
#define __GA_DATAARRAYPAGETABLEIMPL_H_INCLUDED__

#include "GA_Types.h"

#include <UT/UT_Array.h>
#include <UT/UT_Assert.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_VectorTypes.h>

#include <SYS/SYS_AtomicInt.h>
#include <SYS/SYS_Math.h>
#include <SYS/SYS_Memory.h>
#include <SYS/SYS_Types.h>

#include <string.h>

class UT_MemoryCounter;

//#define VERBOSE_DATAARRAY_DEBUG_OUTPUT

/// Internal namespace for GA classes.
/// SUBJECT TO CHANGE! NOT FOR USE BY HDK USERS!
namespace GA_Private {

// class representing a page of numeric data
template <typename T>
class ga_DataArrayPage
{
public:
    ga_DataArrayPage(GA_PageOff capacity)
	: myRefCount(1)
        , myCapacity((int)GA_Size(capacity))
    {
        UT_ASSERT_P(capacity > 0);
        UT_ASSERT_P(capacity <= GA_PAGE_SIZE);
	myData = (T *)malloc(capacity * sizeof(T));
    }
    ~ga_DataArrayPage()
    {
	free(myData);
    }

    /// Report memory usage (includes all shared memory)
    int64
    getMemoryUsage(bool inclusive) const
    {
	return (inclusive ? sizeof(*this) : 0) + myCapacity*sizeof(T);
    }

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    /// If inclusive is true, the size of this object is counted,
    /// else only memory owned by this object is counted.
    /// If this is pointed to by the calling object, inclusive should be true.
    /// If this is contained in the calling object, inclusive should be false.
    /// (Its memory was already counted in the size of the calling object.)
    void countMemory(UT_MemoryCounter &counter, bool inclusive) const;

    // thread-safe reference counting methods
    void ref() const		{ myRefCount.add(1); }
    void unref()		{ if(myRefCount.add(-1) == 0) delete this; }
    SYS_FORCE_INLINE bool isShared() const	{ return myRefCount.relaxedLoad() != 1; }

    ga_DataArrayPage<T> *
    copy(GA_PageOff newcapacity = GA_PageOff(GA_PAGE_SIZE)) const
    {
#ifdef VERBOSE_DATAARRAY_DEBUG_OUTPUT
        printf("Copying page %p capacity %d into capacity %d\n", this, (int)myCapacity, (int)newcapacity);
        fflush(stdout);
#endif
        UT_ASSERT_P(myCapacity > 0 && myCapacity <= GA_PAGE_SIZE);
        UT_ASSERT_P(newcapacity > 0 && newcapacity <= GA_PAGE_SIZE);

	ga_DataArrayPage<T> *d = new ga_DataArrayPage<T>(newcapacity);
	if (d && d->myData)
        {
            GA_Size sizetocopy = SYSmin(myCapacity, newcapacity);
	    memcpy(d->myData, myData, sizetocopy * sizeof(T));
        }
	return d;
    }

    const T *castToData() const	{ return myData; }
    T *castToData()		{ return myData; }

    int capacity() const	{ return myCapacity; }

private:
    T *				myData;
    mutable SYS_AtomicInt32	myRefCount;
    int                         myCapacity;
};


template<typename T> static inline bool
ga_IsNotZero(T a)	    { return (bool) a; }
template<> inline bool
ga_IsNotZero(fpreal16 a)    { return !a.isZero(); }
template<> inline bool
ga_IsNotZero(UT_Vector3H a) { return a.x() || a.y() || a.z(); }
template<> inline bool
ga_IsNotZero(UT_Vector3F a) { return a.x() || a.y() || a.z(); }
template<> inline bool
ga_IsNotZero(UT_Vector3D a) { return a.x() || a.y() || a.z(); }
template<> inline bool
ga_IsNotZero(UT_Vector4H a) { return a.x() || a.y() || a.z() || a.w(); }
template<> inline bool
ga_IsNotZero(UT_Vector4F a) { return a.x() || a.y() || a.z() || a.w(); }
template<> inline bool
ga_IsNotZero(UT_Vector4D a) { return a.x() || a.y() || a.z() || a.w(); }


class ga_PageTableBaseImpl
{
};

// Internal implementation extracted from ga_DataArrayPageTable so that
// GA_Detail::getPos3()/setPos3() can perform fast direct lookups.
// @c T is the storage type, either a scalar type or a POD type
// @c TB is the underlying scalar type (same as T if T is a scalar type)
// @c SLICE is the component of a POD type represented by this table
// @c VECTORSIZE is the number of @c TB components making up @c T
template <typename T, typename TB, int SLICE, int VECTORSIZE>
class ga_DataArrayPageTableImpl : public ga_PageTableBaseImpl
{
protected:

    struct ga_DataArrayPagePtr
    {
	union
	{
	    ga_DataArrayPage<T> *   ptr;
	    char		    rawvalue[sizeof(T)];

	    // We can't just have T value because fpreal16 has
	    // a constructor.
	    const T &	value() const	    { return *((const T*)rawvalue); }
	    T &		value()		    { return *((T*)rawvalue); }
	    const TB &	scalarValue() const { return ((const TB*)rawvalue)[SLICE]; }
	    TB &	scalarValue()       { return ((TB*)rawvalue)[SLICE]; }
	} myData;

	T *		myRawData;

	// Note the usage of this is handled by the caller that thinks
	// this is a pointer.
	int64
	getMemoryUsage(bool inclusive) const
	{
            int64 mem = inclusive ? sizeof(*this) : 0;
	    if (!myRawData)
		return mem;
	    else
		return mem + myData.ptr->getMemoryUsage(true);
	}
        void countMemory(UT_MemoryCounter &counter, bool inclusive) const;
    };

    // The internal array of myPages is shared among slices.
    // It is only allocated by slice 0 and cannot be resized, once shared.
    mutable UT_Array<ga_DataArrayPagePtr> 	myPages;

    // In order to share myCountInLast among slices,
    // it can't be a directly-stored integer.  Its value
    // is kept in element 0 of the UT_Array, but is
    // only allocated by slice 0; the rest share the same
    // internal array of UT_Array.
    mutable UT_Array<GA_PageOff>                myCountInLast;
    // Same is true of myCapacityInLast
    mutable UT_Array<GA_PageOff>                myCapacityInLast;
    // The default value with which to fill new pages and allocations
    T 					 	myDefault;

public:

    ga_DataArrayPageTableImpl(T def)
        : myDefault(def)
        , myCountInLast((SLICE == 0) ? 1 : 0)
        , myCapacityInLast((SLICE == 0) ? 1 : 0)
    {
        if (SLICE == 0)
        {
            myCountInLast.append(GA_PageOff(0));
            myCapacityInLast.append(GA_PageOff(0));
        }
    }

    SYS_FORCE_INLINE GA_PageOff
    countInLast() const    { return myCountInLast(0);    }

    SYS_FORCE_INLINE GA_PageOff &
    countInLast()          { return myCountInLast(0);    }

    SYS_FORCE_INLINE GA_PageOff
    capacityInLast() const { return myCapacityInLast(0); }

    SYS_FORCE_INLINE GA_PageOff &
    capacityInLast()       { return myCapacityInLast(0); }

    SYS_FORCE_INLINE TB
    get(GA_Offset offset) const
    {
	return get(GAgetPageNum(offset), GAgetPageOff(offset));
    }
    SYS_FORCE_INLINE void
    set(GA_Offset offset, TB v)
    {
	set(GAgetPageNum(offset), GAgetPageOff(offset), v);
    }
    SYS_FORCE_INLINE TB
    get(GA_PageNum pageid, GA_PageOff offset) const
    {
        UT_ASSERT_P(pageid >= 0 && pageid < myPages.entries());
        UT_ASSERT_P(offset >= 0 && offset < GA_PAGE_SIZE);
        UT_ASSERT_P(pageid != myPages.entries() - 1 || offset < countInLast());
	const ga_DataArrayPagePtr &page = myPages(pageid);
	if (!page.myRawData)
	    return page.myData.scalarValue();
	else
	    return ((TB*)&(page.myRawData[offset]))[SLICE];
    }
    SYS_FORCE_INLINE void
    set(GA_PageNum pageid, GA_PageOff offset, TB v)
    {
        UT_ASSERT_P(pageid >= 0 && pageid < myPages.entries());
        UT_ASSERT_P(offset >= 0 && offset < GA_PAGE_SIZE);
        UT_ASSERT_P(pageid != myPages.entries() - 1 || offset < countInLast());
#ifdef VERBOSE_DATAARRAY_DEBUG_OUTPUT
        printf("(table %p) Setting slice %d of page %d offset %d value %d (pages %d; countinlast %d; capacityinlast %d)\n", this, (int)SLICE, (int)pageid, (int)offset, (int)v, (int)myPages.entries(), (int)countInLast(), (int)capacityInLast());
        fflush(stdout);
#endif
        UT_ASSERT_P(pageid != myPages.entries() - 1 || (!myPages(pageid).myRawData && !capacityInLast()) || (myPages(pageid).myRawData && capacityInLast()));
	ga_DataArrayPagePtr &page = myPages(pageid);
	if (page.myRawData)
	{
#ifdef VERBOSE_DATAARRAY_DEBUG_OUTPUT
        printf("              &page = %p; page.myRawData = %p\n", &page, page.myRawData);
        fflush(stdout);
#endif
	    if (page.myData.ptr->isShared())
	    {
                bool islast = pageid == myPages.entries() - 1;
                GA_PageOff capacity = islast ? capacityInLast() : GA_PageOff(GA_PAGE_SIZE);
		((TB*)&forceHardenToData(pageid, capacity)[offset])[SLICE] = v;
	    }
	    else
		((TB*)&page.myRawData[offset])[SLICE] = v;
	}
	else
	{
	    // Setting a constant page.
	    if (page.myData.scalarValue() != v)
	    {
#ifdef VERBOSE_DATAARRAY_DEBUG_OUTPUT
                printf("           non-default value\n");
                fflush(stdout);
#endif
                if (countInLast() == 1 && pageid == myPages.entries() - 1)
                {
#ifdef VERBOSE_DATAARRAY_DEBUG_OUTPUT
                    printf("           no need to harden (countInLast = %d; entries = %d)\n", countInLast(), myPages.entries());
                    fflush(stdout);
#endif
                    // Only one item, so just change its value
                    page.myData.scalarValue() = v;
                }
                else
                {
#ifdef VERBOSE_DATAARRAY_DEBUG_OUTPUT
                    printf("           need to harden (countInLast = %d; entries = %d)\n", countInLast(), myPages.entries());
                    fflush(stdout);
#endif
		    // Need to actually expand.
                    bool islast = pageid == myPages.entries() - 1;
                    GA_PageOff capacity = islast ? countInLast() : GA_PageOff(GA_PAGE_SIZE);
                    ((TB*)&forceHardenToData(pageid, capacity)[offset])[SLICE] = v;
                }
	    }
	}
    }

    SYS_FORCE_INLINE void
    add(GA_PageNum pageid, GA_PageOff offset, TB v)
    {
        UT_ASSERT_P(pageid >= 0 && pageid < myPages.entries());
        UT_ASSERT_P(offset >= 0 && offset < GA_PAGE_SIZE);
        UT_ASSERT_P(pageid != myPages.entries() - 1 || offset < countInLast());
#ifdef VERBOSE_DATAARRAY_DEBUG_OUTPUT
        printf("(table %p) Adding to slice %d of page %d offset %d\n", this, (int)SLICE, (int)pageid, (int)offset);
        fflush(stdout);
#endif
	ga_DataArrayPagePtr &page = myPages(pageid);
	if (page.myRawData)
        {
	    if (page.myData.ptr->isShared())
	    {
                bool islast = pageid == myPages.entries() - 1;
                GA_PageOff capacity = islast ? capacityInLast() : GA_PageOff(GA_PAGE_SIZE);
                ((TB*)&forceHardenToData(pageid, capacity)[offset])[SLICE] += v;
            }
            else
	        ((TB*)&page.myRawData[offset])[SLICE] += v;
        }
        else if (countInLast() == 1 && pageid == myPages.entries() - 1)
        {
            page.myData.scalarValue() += v;
        }
        else
        {
            bool islast = pageid == myPages.entries() - 1;
            GA_PageOff capacity = islast ? countInLast() : GA_PageOff(GA_PAGE_SIZE);
            ((TB*)&forceHardenToData(pageid, capacity)[offset])[SLICE] += v;
        }
    }

    SYS_FORCE_INLINE T
    getVector(GA_Offset offset) const
    {
	return getVector(GAgetPageNum(offset), GAgetPageOff(offset));
    }
    SYS_FORCE_INLINE void
    setVector(GA_Offset offset, const T &v)
    {
	setVector(GAgetPageNum(offset), GAgetPageOff(offset), v);
    }
    SYS_FORCE_INLINE T
    getVector(GA_PageNum pageid, GA_PageOff offset) const
    {
        UT_ASSERT_P(pageid >= 0 && pageid < myPages.entries());
        UT_ASSERT_P(offset >= 0 && offset < GA_PAGE_SIZE);
        UT_ASSERT_P(pageid != myPages.entries() - 1 || offset < countInLast());
#ifdef VERBOSE_DATAARRAY_DEBUG_OUTPUT
        printf("(table %p) Getting page %d offset %d (slice = %d; vectorsize = %d; value[%d] = %e)\n", this, (int)pageid, (int)offset, (int)SLICE, (int)VECTORSIZE, (int)SLICE, get(pageid, offset));
        fflush(stdout);
#endif
	const ga_DataArrayPagePtr &page = myPages(pageid);
	if (!page.myRawData)
	    return page.myData.value();
	else
	    return page.myRawData[offset];
    }
    SYS_FORCE_INLINE void
    setVector(GA_PageNum pageid, GA_PageOff offset, const T &v)
    {
        UT_ASSERT_P(pageid >= 0 && pageid < myPages.entries());
        UT_ASSERT_P(offset >= 0 && offset < GA_PAGE_SIZE);
        UT_ASSERT_P(pageid != myPages.entries() - 1 || offset < countInLast());
#ifdef VERBOSE_DATAARRAY_DEBUG_OUTPUT
        printf("(table %p) Setting page %d offset %d (slice = %d; vectorsize = %d; v[0] = %e; previousvalue[%d] = %e)\n", this, (int)pageid, (int)offset, (int)SLICE, (int)VECTORSIZE, ((TB*)&v)[0], (int)SLICE, get(pageid, offset));
        fflush(stdout);
#endif
	ga_DataArrayPagePtr &page = myPages(pageid);
	if (page.myRawData)
	{
	    if (page.myData.ptr->isShared())
	    {
                bool islast = pageid == myPages.entries() - 1;
                GA_PageOff capacity = islast ? capacityInLast() : GA_PageOff(GA_PAGE_SIZE);
		forceHardenToData(pageid, capacity)[offset] = v;
	    }
	    else
		page.myRawData[offset] = v;
	}
	else
	{
	    // Setting a constant page.
	    if (page.myData.value() != v)
	    {
#ifdef VERBOSE_DATAARRAY_DEBUG_OUTPUT
                printf("           non-default value\n");
                fflush(stdout);
#endif
                if (countInLast() == 1 && pageid == myPages.entries() - 1)
                {
#ifdef VERBOSE_DATAARRAY_DEBUG_OUTPUT
                    printf("           no need to harden (countInLast = %d; entries = %d)\n", countInLast(), myPages.entries());
                    fflush(stdout);
#endif
                    // Only one item, so just change its value
                    page.myData.value() = v;
                }
                else
                {
#ifdef VERBOSE_DATAARRAY_DEBUG_OUTPUT
                    printf("           need to harden (countInLast = %d; entries = %d)\n", countInLast(), myPages.entries());
                    fflush(stdout);
#endif
		    // Need to actually expand.
                    bool islast = pageid == myPages.entries() - 1;
                    GA_PageOff capacity = islast ? countInLast() : GA_PageOff(GA_PAGE_SIZE);
		    forceHardenToData(pageid, capacity)[offset] = v;
                }
	    }
	}
#ifdef VERBOSE_DATAARRAY_DEBUG_OUTPUT
        printf("            newvalue[%d] = %e)\n", (int)SLICE, get(pageid, offset));
        fflush(stdout);
#endif
    }
    SYS_FORCE_INLINE void
    addVector(GA_Offset offset, T v)
    {
	addVector(GAgetPageNum(offset), GAgetPageOff(offset), v);
    }
    SYS_FORCE_INLINE void
    addVector(GA_PageNum pageid, GA_PageOff offset, T v)
    {
        UT_ASSERT_P(pageid >= 0 && pageid < myPages.entries());
        UT_ASSERT_P(offset >= 0 && offset < GA_PAGE_SIZE);
        UT_ASSERT_P(pageid != myPages.entries() - 1 || offset < countInLast());
#ifdef VERBOSE_DATAARRAY_DEBUG_OUTPUT
        printf("(table %p) Adding to page %d offset %d\n", this, (int)pageid, (int)offset);
        fflush(stdout);
#endif
	ga_DataArrayPagePtr &page = myPages(pageid);
	if (page.myRawData)
        {
	    if (page.myData.ptr->isShared())
	    {
                bool islast = pageid == myPages.entries() - 1;
                GA_PageOff capacity = islast ? capacityInLast() : GA_PageOff(GA_PAGE_SIZE);
	        forceHardenToData(pageid, capacity)[offset] += v;
            }
            else
                page.myRawData[offset] += v;
        }
        else if (countInLast() == 1 && pageid == myPages.entries() - 1)
        {
            page.myData.value() += v;
        }
        else
        {
            bool islast = pageid == myPages.entries() - 1;
            GA_PageOff capacity = islast ? countInLast() : GA_PageOff(GA_PAGE_SIZE);
            forceHardenToData(pageid, capacity)[offset] += v;
        }
    }

    ga_DataArrayPage<T> *
    createConstantPage(T value, GA_PageOff startdef = GA_PageOff(GA_PAGE_SIZE), GA_PageOff capacity = GA_PageOff(GA_PAGE_SIZE)) const
    {
	ga_DataArrayPage<T> *page = new ga_DataArrayPage<T>(capacity);
	T *data = page->castToData();

	if (ga_IsNotZero(value))
	{
            for (GA_Size i = 0; i < startdef; ++i)
                data[i] = value;
	}
	else
	{
	    memset(data, 0, startdef * sizeof(T));
	}
        if (startdef < capacity)
        {
	    if (ga_IsNotZero(myDefault))
	    {
                for (GA_Size i = startdef; i < capacity; ++i)
                    data[i] = myDefault;
	    }
	    else
	    {
	        memset(data + startdef, 0, (capacity - startdef) * sizeof(T));
	    }
        }
	return page;
    }

    // Doesn't test, just hardens.
    T *
    forceHardenToData(GA_PageNum pageid, GA_PageOff capacity = GA_PageOff(GA_PAGE_SIZE))
    {
	ga_DataArrayPagePtr &	page = myPages(pageid);
	ga_DataArrayPage<T> *	pageptr;
	T *			result;

        bool islast = (pageid == myPages.entries() - 1);
        UT_ASSERT_P(islast || capacity == GA_PageOff(GA_PAGE_SIZE));
	if (!page.myRawData)
	{
#ifdef VERBOSE_DATAARRAY_DEBUG_OUTPUT
            printf("(table %p) Hardening page %d, pages = %d, countInLast = %d\n", this, (int)pageid, (int)myPages.entries(), (int)countInLast());
            fflush(stdout);
#endif
            if (islast)
            {
#ifdef VERBOSE_DATAARRAY_DEBUG_OUTPUT
                printf("            beforevalue[0][%d] = %e)\n", (int)SLICE, get(pageid, 0));
                fflush(stdout);
#endif
                UT_ASSERT_P(countInLast() <= capacity);
                pageptr = createConstantPage(page.myData.value(), countInLast(), capacity);
#ifdef VERBOSE_DATAARRAY_DEBUG_OUTPUT
                printf("            aftervalue[0][%d]  = %e)\n", (int)SLICE, get(pageid, 0));
                fflush(stdout);
#endif
            }
            else
	        pageptr = createConstantPage(page.myData.value(), GA_PageOff(GA_PAGE_SIZE), capacity);
	    page.myData.ptr = pageptr;
	    page.myRawData = result = pageptr->castToData();
	}
	else
	{
#ifdef VERBOSE_DATAARRAY_DEBUG_OUTPUT
            printf("(table %p) Re-hardening page %d, pages = %d, countInLast = %d capacityInLast = %d newcapacity = %d\n", this, (int)pageid, (int)myPages.entries(), (int)countInLast(), (int)capacityInLast(), (int)capacity);
            fflush(stdout);
#endif
	    pageptr = page.myData.ptr;
	    ga_DataArrayPage<T> *oldpage = pageptr;
            GA_PageOff oldcapacity = islast ? capacityInLast() : GA_PageOff(GA_PAGE_SIZE);
	    pageptr = oldpage->copy(capacity);
	    page.myData.ptr = pageptr;
	    page.myRawData = result = pageptr->castToData();
            if (oldcapacity < capacity)
            {
                if (ga_IsNotZero(myDefault))
                {
                    for (GA_Size i = oldcapacity; i < capacity; ++i)
                        result[i] = myDefault;
                }
                else
                {
                    memset(result + oldcapacity, 0, (capacity - oldcapacity) * sizeof(T));
                }
            }
	    oldpage->unref();
	}

        if (islast)
            capacityInLast() = capacity;

	UT_ASSERT_P(islast || page.myData.ptr->capacity() == GA_PAGE_SIZE);
	return result;
    }

    SYS_FORCE_INLINE T *
    hardenToData(GA_PageNum pageid)
    {
        const ga_DataArrayPagePtr &page = myPages(pageid);
        if (!page.myRawData || page.myData.ptr->isShared())
        {
            GA_PageOff capacity = (pageid != myPages.entries() - 1)
                                ? GA_PageOff(GA_PAGE_SIZE)
                                : countInLast();
            return forceHardenToData(pageid, capacity);
        }
        return page.myRawData;
    }

    // Does not unshare shared pages.
    SYS_FORCE_INLINE T *
    softenToData(GA_PageNum pageid)
    {
	const ga_DataArrayPagePtr &page = myPages(pageid);

	if (!page.myRawData)
	{
	    return forceHardenToData(pageid);
	}
	else
	{
	    return page.myRawData;
	}
    }

    void
    setConstant(GA_Offset start_offset, GA_Offset end_offset, T value)
    {
        GA_PageNum startid = GAgetPageNum(start_offset);
        GA_PageOff startoff = GAgetPageOff(start_offset);
        GA_PageNum endid = GAisValid(end_offset) ? GAgetPageNum(end_offset + GA_PAGE_SIZE - 1) : GA_PageNum(myPages.entries());
        GA_PageOff endoff = GAisValid(end_offset) ? GAgetPageOff(end_offset) : countInLast();
        if (startoff != 0)
        {
            GA_PageNum pageid = startid;
            ga_DataArrayPagePtr &page = myPages(pageid);
            GA_PageOff endstart = (startid != endid-1 || endoff == 0) ? GA_PageOff(GA_PAGE_SIZE) : endoff;
            if (page.myRawData)
            {
                if (page.myData.ptr->isShared())
                {
                    bool islast = pageid == myPages.entries() - 1;
                    GA_PageOff capacity = islast ? capacityInLast() : GA_PageOff(GA_PAGE_SIZE);
                    forceHardenToData(pageid, capacity);
                }
                for (GA_PageOff i = startoff; i < endstart; ++i)
                    page.myRawData[i] = value;
            }
            else
            {
                // Setting a constant page.
                if (page.myData.value() != value)
                {
                    // NOTE: We're never at the start, so we need to expand
                    bool islast = pageid == myPages.entries() - 1;
                    GA_PageOff capacity = islast ? countInLast() : GA_PageOff(GA_PAGE_SIZE);
                    forceHardenToData(pageid, capacity);
                    for (GA_PageOff i = startoff; i < endstart; ++i)
                        page.myRawData[i] = value;
                }
            }
            ++startid;
            startoff = GA_PageOff(0);
        }
        for (GA_PageNum pageid = startid; pageid < endid; ++pageid)
        {
            ga_DataArrayPagePtr &page = myPages(pageid);
            if (pageid != endid-1 || endoff == 0 || (pageid == myPages.entries() - 1 && endoff == countInLast()))
            {
                // NOTE: This is a copy-paste from makePageConstant, which isn't accessible here.
                if (page.myRawData)
                    page.myData.ptr->unref();
                page.myData.value() = value;
                page.myRawData = NULL;
                if (pageid == myPages.entries() - 1)
                    capacityInLast() = GA_PageOff(0);
            }
            else
            {
                if (page.myRawData)
                {
                    if (page.myData.ptr->isShared())
                    {
                        bool islast = pageid == myPages.entries() - 1;
                        GA_PageOff capacity = islast ? capacityInLast() : GA_PageOff(GA_PAGE_SIZE);
                        forceHardenToData(pageid, capacity);
                    }
                    for (GA_PageOff i(0); i < endoff; ++i)
                        page.myRawData[i] = value;
                }
                else
                {
                    // Setting a constant page.
                    if (page.myData.value() != value)
                    {
                        // NOTE: We're never all the way to the end in this path, so we need to expand
                        bool islast = pageid == myPages.entries() - 1;
                        GA_PageOff capacity = islast ? countInLast() : GA_PageOff(GA_PAGE_SIZE);
                        forceHardenToData(pageid, capacity);
                        for (GA_PageOff i(0); i < endoff; ++i)
                            page.myRawData[i] = value;
                    }
                }
            }
        }
    }
};

// Convenience types
typedef ga_DataArrayPageTableImpl<UT_Vector3F,fpreal32,0,3>
	    ga_DataArrayPageTableImplV3;
typedef ga_DataArrayPageTableImpl<int16,int16,0,1>
	    ga_DataArrayPageTableImplI16;
typedef ga_DataArrayPageTableImpl<int32,int32,0,1>
	    ga_DataArrayPageTableImplI32;
typedef ga_DataArrayPageTableImpl<int64,int64,0,1>
	    ga_DataArrayPageTableImplI64;

// Interface used in GA_Handle for direct access to page data

template <typename T>
static SYS_SAFE_FORCE_INLINE bool
ga_pageTableGet(GA_Private::ga_DataArrayPageTableImplV3 *, GA_Offset, T &)
{
    return false;
}

template <>
SYS_SAFE_FORCE_INLINE bool
ga_pageTableGet<UT_Vector3>(GA_Private::ga_DataArrayPageTableImplV3 *pagetable,
	GA_Offset off, UT_Vector3 &result)
{
    if (pagetable)
    {
	result = pagetable->getVector(off);
	return true;
    }
    return false;
}

template <typename T>
static SYS_SAFE_FORCE_INLINE bool
ga_pageTableSet(GA_Private::ga_DataArrayPageTableImplV3 *, GA_Offset, const T &)
{
    return false;
}

template <>
SYS_SAFE_FORCE_INLINE bool
ga_pageTableSet<UT_Vector3>(GA_Private::ga_DataArrayPageTableImplV3 *pagetable,
	GA_Offset off, const UT_Vector3 &val)
{
    if (pagetable)
    {
	pagetable->setVector(off, val);
	return true;
    }
    return false;
}

template <typename T, typename S>
static SYS_SAFE_FORCE_INLINE bool
ga_pageTableAdd(
	GA_Private::ga_DataArrayPageTableImpl<T,fpreal32,0,3> *,
	GA_Offset, const S &)
{
    return false;
}


template <>
SYS_SAFE_FORCE_INLINE bool
ga_pageTableAdd<UT_Vector3F,UT_Vector3F>(
	GA_Private::ga_DataArrayPageTableImplV3 *pagetable,
	GA_Offset off, const UT_Vector3F &val)
{
    if (pagetable)
    {
	pagetable->addVector(off, val);
	return true;
    }
    return false;
}

template <>
SYS_SAFE_FORCE_INLINE bool
ga_pageTableAdd<UT_Vector3F,UT_Vector3D>(
	GA_Private::ga_DataArrayPageTableImplV3 *pagetable,
	GA_Offset off, const UT_Vector3D &val)
{
    if (pagetable)
    {
	pagetable->addVector(off, UT_Vector3F(val));
	return true;
    }
    return false;
}

} // namespace GA_Private
#endif // __GA_DATAARRAYPAGETABLEIMPL_H_INCLUDED__
