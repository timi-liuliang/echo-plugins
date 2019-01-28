/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        UT_MemoryCounter.h (UT Library, C++)
 *
 * COMMENTS:    Classes for simplifying counting of shared memory in
 *              different ways.  There are currently 4 main use cases
 *              that require different methods of counting shared memory:
 *              
 *              1) Counting the sum total of all used memory.
 *              2) Counting how much memory would be freed if an object,
 *                 such as a detail, is freed.
 *              3) Counting the total amount of memory referenced by an
 *                 object.
 *              4) Counting the amount of memory referenced by an object
 *                 that is not referenced by particular other objects.
 */

#ifndef __UT_MemoryCounter__
#define __UT_MemoryCounter__

#include "UT_Assert.h"
#include "UT_ArrayMap.h"
#include "UT_ArraySet.h"
#include <SYS/SYS_Floor.h>
#include <SYS/SYS_Types.h>

#define UT_MEMORY_DEBUG_LOGGING 0
#if !defined(UT_MEMORY_DEBUG_LOGGING) || UT_MEMORY_DEBUG_LOGGING==0
#define UT_MEMORY_DEBUG_LOG(m,s)
#define UT_MEMORY_DEBUG_LOG_SHARED(m,s,p,r)
#else
#define UT_MEMORY_DEBUG_LOG(m,s)            {printf(m ": %" SYS_PRId64 " B\n", s); fflush(stdout);}
#define UT_MEMORY_DEBUG_LOG_SHARED(m,s,p,r) {printf(m ": %" SYS_PRId64 " B, 0x%p, refcount=%" SYS_PRId64 "\n", s,p,r); fflush(stdout);}
#endif

/// This is the base class of a set of classes for simplifying counting
/// of shared memory for different use cases.  The base class counts
/// all shared memory as if it is unshared, thus computing the total
/// amount of memory used by something, assuming that the same block
/// of memory is never referenced more than once.
class UT_MemoryCounter
{
public:
    UT_MemoryCounter()
        : myUnsharedCount(0)
        , myMustCountShared(true)
        , myMustCountUnshared(true)
    {}

    /// NOTE: The virtual destructor is needed to destruct subclass
    ///       members, even if no special destruction is necessary
    ///       for the subclass itself.
    virtual ~UT_MemoryCounter() {}

    /// This resets any data structures used for counting to their
    /// freshly-constructed state.
    /// NOTE: Subclasses should call UT_MemoryCounter::reset().
    virtual void reset()
    {
        myUnsharedCount = 0;
    }

    /// This counts shared memory according to the requirements of the
    /// use case.  The default implementation counts the total amount
    /// of memory referenced by something, assuming the same block is
    /// never referenced more than once.
    ///
    /// This returns true if the memory counter has recorded this shared
    /// memory pointer at least once before.  If you have a shared object
    /// that itself owns separately shared data, if calling countShared
    /// for the outer level of shared data returns true, the inner
    /// level of shared data doesn't need to be counted again.
    /// For example, when counting the memory of packed primitives,
    /// multiple packed primitives may be referencing the same detail,
    /// and multiple details may be sharing attribute data, so if
    /// the countShared call for the detail handle returns true,
    /// the detail doesn't need to be traversed again.
    ///
    /// NOTE: Subclasses should *NOT* call UT_MemoryCounter::countShared().
    virtual bool countShared(size_t size, exint refcount, const void *p)
    {
        UT_ASSERT_P(refcount > 0);
        myUnsharedCount += size;
        return false;
    }

    /// This counts unshared memory, which doesn't require any special
    /// handling.
    void countUnshared(size_t size)
    {
        myUnsharedCount += size;
    }

    /// This returns the current count of memory used.
    /// NOTE: Subclasses should call UT_MemoryCounter::getCount() to get
    ///       the unshared memory contribution.
    virtual size_t getCount() const
    {
        return myUnsharedCount;
    }

    /// If this returns false, the code using the
    /// UT_MemoryCounter doesn't need to compute memory usage
    /// for a complicated object if its refcount > 1.
    ///
    /// NOTE: Subclasses must handle shared memory appropriately,
    ///       even if this is false, to avoid having to always check this.
    bool mustCountShared() const
    {
        return myMustCountShared;
    }

    /// If this returns false, the code using the
    /// UT_MemoryCounter doesn't need to compute memory usage
    /// for a complicated object if it is unshared or has
    /// refcount == 1.
    ///
    /// NOTE: Subclasses must handle memory with refcount==1 appropriately,
    ///       even if this is false, to avoid having to always check this.
    bool mustCountUnshared() const
    {
        return myMustCountUnshared;
    }

protected:
    UT_MemoryCounter(const bool countshared, const bool countunshared)
        : myUnsharedCount(0)
        , myMustCountShared(countshared)
        , myMustCountUnshared(countunshared)
    {}

private:
    /// The current count of unshared memory
    size_t myUnsharedCount;

    /// true if this is interested in memory that is shared
    /// with refcount > 1.  If false, the code using the
    /// UT_MemoryCounter doesn't need to compute memory usage
    /// for a complicated object if its refcount > 1.
    const bool myMustCountShared;

    /// true if this is interested in memory that is unshared
    /// or has refcount == 1.  If false, the code using the
    /// UT_MemoryCounter doesn't need to compute memory usage
    /// for a complicated object if it is unshared or has
    /// refcount == 1.
    const bool myMustCountUnshared;
};

/// This subclass counts shared memory that is unique to something,
/// i.e. nothing else references the same memory blocks.
/// Since this class just uses the UT_MemoryCounter's unshared count,
/// it doesn't need to implement reset() or getCount().
class UT_MemoryCounterUnique : public UT_MemoryCounter
{
public:
    UT_MemoryCounterUnique()
        : UT_MemoryCounter(false, true)
    {}
    virtual ~UT_MemoryCounterUnique() {}

    /// This counts shared memory according to the requirements of the
    /// use case.  Since this use case only counts memory unique to
    /// something, it only adds to the count if refcount==1.
    virtual bool countShared(size_t size, exint refcount, const void *p)
    {
        UT_ASSERT_P(refcount > 0);
        if (refcount == 1)
            countUnshared(size);
        return false;
    }
};

/// This subclass behaves the same as UT_MemoryCounterNewSafe,
/// except without an input set of pointers to avoid
class UT_MemoryCounterUniqueFullSafe : public UT_MemoryCounter
{
public:
    UT_MemoryCounterUniqueFullSafe()
        : UT_MemoryCounter(true, true)
        , myVisitedCount()
        , myFullSharedCount(0)
        , myUniqueSharedCount(0)
    {}
    virtual ~UT_MemoryCounterUniqueFullSafe() {}

    /// This resets any data structures used for counting to their
    /// freshly-constructed state.
    virtual void reset()
    {
        UT_MemoryCounter::reset();
        myVisitedCount.clear();
        myFullSharedCount = 0;
        myUniqueSharedCount = 0;
    }

    /// This counts shared memory according to the requirements of the
    /// use case.
    virtual bool countShared(size_t size, exint refcount, const void *p)
    {
        UT_ASSERT_P(refcount > 0);
        UT_ASSERT_P(p);
        // If refcount==1, it's always unique, so we don't have to
        // add it to myVisitedCount
        if (refcount == 1)
        {
            UT_ASSERT_MSG_P(myVisitedCount.count(p) == 0, "Why is p with refcount==1 referred to here more than once?");
            UT_IF_ASSERT_P(myVisitedCount[p] = 1);
            UT_MemoryCounter::countUnshared(size);
        }
        // If we haven't encountered it before, insert it
        else if (myVisitedCount.count(p) == 0)
        {
            myVisitedCount[p] = 1;
            myFullSharedCount += size;
        }
        else
        {
            exint newvisitcount = ++myVisitedCount[p];
            // If all of the references to p are in what's being counted,
            // it's unique to what's being counted.
            if (newvisitcount == refcount)
                myUniqueSharedCount += size;
            UT_ASSERT_MSG_P(newvisitcount <= refcount, "Why is p referred to here more times than its refcount?");
            return true;
        }
        return false;
    }

    size_t getUniqueCount() const
    {
        return UT_MemoryCounter::getCount() + myUniqueSharedCount;
    }

    size_t getFullCount() const
    {
        return UT_MemoryCounter::getCount() + myFullSharedCount;
    }

private:
    UT::ArrayMap<const void *,exint> myVisitedCount;
    size_t myFullSharedCount;
    size_t myUniqueSharedCount;
};

/// This subclass behaves the same as UT_MemoryCounterUniqueFullSafe,
/// except not checking whether memory is unique to what's being counted.
class UT_MemoryCounterFullSafe : public UT_MemoryCounter
{
public:
    UT_MemoryCounterFullSafe()
        : UT_MemoryCounter(true, true)
        , myVisitedSet()
        , myFullSharedCount(0)
    {}
    virtual ~UT_MemoryCounterFullSafe() {}

    /// This resets any data structures used for counting to their
    /// freshly-constructed state.
    virtual void reset()
    {
        UT_MemoryCounter::reset();
        myVisitedSet.clear();
        myFullSharedCount = 0;
    }

    /// This counts shared memory according to the requirements of the
    /// use case.
    virtual bool countShared(size_t size, exint refcount, const void *p)
    {
        UT_ASSERT_P(refcount > 0);
        UT_ASSERT_P(p);
        // If refcount==1, it's always unique, so we don't have to
        // add it to myVisitedSet
        if (refcount == 1)
        {
            UT_ASSERT_MSG_P(myVisitedSet.count(p) == 0, "Why is p with refcount==1 referred to here more than once?");
            UT_IF_ASSERT_P(myVisitedSet.insert(p));
            UT_MemoryCounter::countUnshared(size);
        }
        // If we haven't encountered it before, insert it
        else if (myVisitedSet.count(p) == 0)
        {
            myVisitedSet.insert(p);
            myFullSharedCount += size;
        }
        else
            return true;
        return false;
    }

    size_t getFullCount() const
    {
        return UT_MemoryCounter::getCount() + myFullSharedCount;
    }

private:
    UT::ArraySet<const void *> myVisitedSet;
    size_t myFullSharedCount;
};

/// This subclass counts shared memory under the assumption that
/// all memory is being counted, so on a block of memory referenced
/// refcount times, countShared() will be called refcount times.
/// To avoid double-counting of this shared memory, countShared()
/// divides by refcount.
class UT_MemoryCounterTotal : public UT_MemoryCounter
{
public:
    UT_MemoryCounterTotal()
        : UT_MemoryCounter(true, true)
        , mySharedCount(0)
    {}
    virtual ~UT_MemoryCounterTotal() {}

    /// This resets any data structures used for counting to their
    /// freshly-constructed state.
    virtual void reset()
    {
        UT_MemoryCounter::reset();
        mySharedCount = 0;
    }

    /// This counts shared memory according to the requirements of the
    /// use case.  This implementation avoids double-counting of
    /// referenced memory by dividing size by the reference count.
    virtual bool countShared(size_t size, exint refcount, const void *p)
    {
        UT_ASSERT_P(refcount > 0);
        mySharedCount += fpreal(size)/refcount;
        return false;
    }

    /// This returns the current count of memory used.
    virtual size_t getCount() const
    {
        return UT_MemoryCounter::getCount() + size_t(SYSrint(mySharedCount));
    }

private:
    /// The current count of shared memory, each scaled by the reference
    /// count to avoid double-counting.
    fpreal mySharedCount;
};


/// This subclass counts shared memory excluding any shared memory
/// blocks in the specified set, so that one can determine how much
/// memory a node's output detail uses that its input details do not.
/// NOTE: This assumes that countShared() is called at most once for
///       each shared memory block, because unlike UT_MemoryCounterNewSafe,
///       it doesn't add each counted block to a set of blocks to avoid.
class UT_MemoryCounterNew : public UT_MemoryCounter
{
public:
    UT_MemoryCounterNew(const UT::ArraySet<const void *> &avoidset)
        : UT_MemoryCounter(true, true)
        , myAvoidSet(avoidset)
        , myNewSharedCount(0)
        , myFullSharedCount(0)
        , myUniqueCount(0)
    {}
    virtual ~UT_MemoryCounterNew() {}

    /// This resets any data structures used for counting to their
    /// freshly-constructed state.
    virtual void reset()
    {
        UT_MemoryCounter::reset();
        myNewSharedCount = 0;
        myFullSharedCount = 0;
        myUniqueCount = 0;
    }

    /// This counts shared memory according to the requirements of the
    /// use case.  This implementation avoids counting any shared memory
    /// in the specified set.
    virtual bool countShared(size_t size, exint refcount, const void *p)
    {
        UT_ASSERT_P(refcount > 0);
        UT_ASSERT_P(p);
        myFullSharedCount += size;
        if (myAvoidSet.count(p) == 0)
        {
            myNewSharedCount += size;
            if (refcount == 1)
                myUniqueCount += size;
        }
        else
        {
            UT_ASSERT_MSG_P(refcount > 1, "If we're the only one referring to this block, why is it in myAvoidSet?");
        }
        return false;
    }

    /// This returns the current count of "new" memory used.
    virtual size_t getCount() const
    {
        return UT_MemoryCounter::getCount() + myNewSharedCount;
    }

    size_t getFullCount() const
    {
        return UT_MemoryCounter::getCount() + myFullSharedCount;
    }

    size_t getUniqueCount() const
    {
        return UT_MemoryCounter::getCount() + myUniqueCount;
    }

private:
    const UT::ArraySet<const void *> &myAvoidSet;
    size_t myNewSharedCount;
    size_t myUniqueCount;
    size_t myFullSharedCount;
};

/// This subclass counts shared memory excluding any shared memory
/// blocks in the specified set, so that one can determine how much
/// memory a node's output detail uses that its input details do not.
/// Unlike UT_MemoryCounterNew, this does *NOT* assume that the
/// countShared() is only called once for each shared memory block,
/// by adding all blocks to a dynamic set of blocks to avoid.
/// This incurs extra cost on every countShared().
class UT_MemoryCounterNewSafe : public UT_MemoryCounter
{
public:
    UT_MemoryCounterNewSafe(const UT::ArraySet<const void *> &avoidset)
        : UT_MemoryCounter(true, true)
        , myAvoidSet(avoidset)
        , myVisitedCount()
        , myNewSharedCount(0)
        , myFullSharedCount(0)
        , myUniqueSharedCount(0)
    {}
    virtual ~UT_MemoryCounterNewSafe() {}

    /// This resets any data structures used for counting to their
    /// freshly-constructed state.
    virtual void reset()
    {
        UT_MemoryCounter::reset();
        myVisitedCount.clear();
        myNewSharedCount = 0;
        myFullSharedCount = 0;
        myUniqueSharedCount = 0;
    }

    /// This counts shared memory according to the requirements of the
    /// use case.  This implementation avoids counting any shared memory
    /// in the specified set.
    virtual bool countShared(size_t size, exint refcount, const void *p)
    {
        UT_ASSERT_P(refcount > 0);
        UT_ASSERT_P(p);
        // If refcount==1, it's always unique, so we don't have to
        // add it to myVisitedCount
        if (refcount == 1)
        {
            UT_ASSERT_MSG_P(myVisitedCount.count(p) == 0, "Why is p with refcount==1 referred to here more than once?");
            UT_ASSERT_MSG_P(myAvoidSet.count(p) == 0, "Why is p with refcount==1 referred to here and in the input set?");
            UT_IF_ASSERT_P(myVisitedCount[p] = 1);
            UT_MemoryCounter::countUnshared(size);
        }
        // If we haven't encountered it before, insert it
        else if (myVisitedCount.count(p) == 0)
        {
            myVisitedCount[p] = 1;
            myFullSharedCount += size;
            if (myAvoidSet.count(p) == 0)
                myNewSharedCount += size;
        }
        else
        {
            exint newvisitcount = ++myVisitedCount[p];
            // If all of the references to p are in what's being counted,
            // it's unique to what's being counted.
            if (newvisitcount == refcount)
            {
                UT_ASSERT_MSG_P(myAvoidSet.count(p) == 0, "Why does p have all refcount references here and also appear in the input set?");
                myUniqueSharedCount += size;
            }
            UT_ASSERT_MSG_P(newvisitcount <= refcount, "Why is p referred to here more times than its refcount?");
            return true;
        }
        return false;
    }

    /// This returns the current count of memory used.
    virtual size_t getCount() const
    {
        return UT_MemoryCounter::getCount() + myNewSharedCount;
    }

    size_t getFullCount() const
    {
        return UT_MemoryCounter::getCount() + myFullSharedCount;
    }

    size_t getUniqueCount() const
    {
        return UT_MemoryCounter::getCount() + myUniqueSharedCount;
    }

private:
    const UT::ArraySet<const void *> &myAvoidSet;
    UT::ArrayMap<const void *,exint> myVisitedCount;
    size_t myNewSharedCount;
    size_t myUniqueSharedCount;
    size_t myFullSharedCount;
};

/// This class is for gathering a set of shared memory blocks
/// using the same interface as memory counting, to avoid needing
/// two separate interfaces propagated to everything.  This is intended
/// to be used before counting memory with UT_MemoryCounterNew or
/// UT_MemoryCounterNewSafe.
class UT_MemoryCounterGather : public UT_MemoryCounter
{
public:
    UT_MemoryCounterGather(UT::ArraySet<const void *> &gatherset)
        : UT_MemoryCounter(true, false)
        , myGatherSet(gatherset)
    {}
    virtual ~UT_MemoryCounterGather() {}

    /// This resets any data structures used for counting to their
    /// freshly-constructed state.
    /// NOTE: This particular implementation does not necessarily
    ///       restore the gathered set to its initial state, if
    ///       it had elements to begin with.
    virtual void reset()
    {
        UT_MemoryCounter::reset();
        myGatherSet.clear();
    }

    /// This inserts the pointer p into the gathered set.
    /// NOTE: If p is already there, it's not inserted again.
    virtual bool countShared(size_t size, exint refcount, const void *p)
    {
        UT_ASSERT_P(refcount > 0);
        UT_ASSERT_P(p);
        // NOTE: If refcount==1, the block isn't shared with anything else,
        //       so we don't need to gather it.
        if (refcount > 1)
            myGatherSet.insert(p);
        return false;
    }

private:
    UT::ArraySet<const void *> &myGatherSet;
};


#endif
