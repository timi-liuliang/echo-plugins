/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_ParallelUtil.h ( UT Library, C++)
 *
 * COMMENTS:	Simple wrappers on tbb interface
 */

#ifndef __UT_ParallelUtil__
#define __UT_ParallelUtil__

#include "UT_API.h"

#include "UT_Array.h"
#include "UT_PerformanceThread.h"
#include "UT_Task.h"
#include "UT_TaskScope.h"
#include "UT_Thread.h"

#include <tbb/blocked_range.h>
#include <tbb/blocked_range2d.h>
#include <tbb/task_arena.h>
#include <tbb/parallel_for.h>
#include <tbb/parallel_invoke.h>
#include <tbb/parallel_reduce.h>
#include <tbb/parallel_sort.h>

/// Typedef to denote the "split" constructor of a range
typedef tbb::split	UT_Split;

/// Declare prior to use.
template <typename T> 
class UT_BlockedRange;

template <typename T_ROW, typename T_COL=T_ROW> 
class UT_BlockedRange2D;

// Default implementation that calls range.size()
template< typename RANGE >
struct UT_EstimatorNumItems
{
    UT_EstimatorNumItems() {}

    size_t operator()(const RANGE& range) const
    {
	return range.size();
    }
};

// Partial specialization for UT_BlockedRange2D<T>
template< typename T >
struct UT_EstimatorNumItems< UT_BlockedRange2D<T> >
{
    UT_EstimatorNumItems() {}

    size_t operator()(const UT_BlockedRange2D<T>& range) const
    {
        return range.rows().size() * range.cols().size();
    }
};

/// This is needed by UT_CoarsenedRange
template <typename RANGE>
inline size_t UTestimatedNumItems(const RANGE& range)
{
    return UT_EstimatorNumItems<RANGE>()(range);
}

/// UT_CoarsenedRange: This should be used only inside 
/// UT_ParallelFor and UT_ParallelReduce
/// This class wraps an existing range with a new range.
/// This allows us to use simple_partitioner, rather than
/// auto_partitioner, which has disastrous performance with
/// the default grain size in ttb 4.
template< typename RANGE >
class UT_CoarsenedRange : public RANGE
{
public:
    // Compiler-generated versions are fine:
    // ~UT_CoarsenedRange();
    // UT_CoarsenedRange(const UT_CoarsenedRange&);

    // Split into two sub-ranges:
    UT_CoarsenedRange(UT_CoarsenedRange& range, tbb::split spl) :
        RANGE(range, spl),
        myGrainSize(range.myGrainSize)
    {        
    }

    // Inherited: bool empty() const

    bool is_divisible() const
    {
        return 
            RANGE::is_divisible() &&
            (UTestimatedNumItems(static_cast<const RANGE&>(*this)) > myGrainSize);
    }

private:
    size_t myGrainSize;

    UT_CoarsenedRange(const RANGE& base_range, const size_t grain_size) :
        RANGE(base_range),
        myGrainSize(grain_size)
    {        
    }

    template <typename Range, typename Body>
    friend void UTparallelFor(
        const Range &range, const Body &body,
        const int subscribe_ratio, const int min_grain_size
    );
    template <typename Range, typename Body>
    friend void UTparallelReduce(
        const Range &range, Body &body,
        const int subscribe_ratio, const int min_grain_size
    );
    template <typename Range, typename Body>
    friend void UTparallelDeterministicReduce(
        const Range &range, Body &body, const int grain_size
    );
};

/// Helper class for UTparallelFor().
/// Wraps the thread body in a task scope so that thread stats are collected
/// by the performance monitor, and child tasks can inherit task scope locks
/// from the parent task.
template<typename Range, typename Body>
class ut_TaskScopedBody
{
public:
    ut_TaskScopedBody(const Body *body)
	: myBody(body),
	  myParentTaskScope(UT_TaskScope::getCurrent())
    {
    }

    ut_TaskScopedBody(const ut_TaskScopedBody &src)
	: myBody(src.myBody),
	  myParentTaskScope(src.myParentTaskScope)
    {
    }

    void operator()(const Range &r) const
    {
	UT_TaskScope task_scope(myParentTaskScope);
	(*myBody)(r);
    }

private:
    const Body *myBody;
    const UT_TaskScope *myParentTaskScope;
};

/// Helper class for UTparallelForEachNumber()
/// This wraps the thread body to perform different load balancing based on
/// peeling off tasks using an atomic int to iterate over the range.
/// @C IntType must be an integer type supported by @c SYS_AtomicInt (currently
/// int32 or int64).
template <typename IntType, typename Body>
class ut_ForEachNumberBody
{
public:
    ut_ForEachNumberBody(const Body &body,
	    SYS_AtomicInt<IntType> &it, IntType end)
	: myBody(body)
	, myIt(it)
	, myEnd(end)
    {
    }
    void operator()(const UT_BlockedRange<IntType> &range) const
    {
	while (true)
	{
	    IntType	it = myIt.exchangeAdd(1);
	    if (it >= myEnd)
		break;
	    myBody(UT_BlockedRange<IntType>(it, it+1));
	}
    }
private:
    const Body			&myBody;
    SYS_AtomicInt<IntType>	&myIt;
    IntType			 myEnd;
};

/// Run the @c body function over a range in parallel.
/// UTparallelFor attempts to spread the range out over at most 
/// subscribe_ratio * num_processor tasks.
/// The factor subscribe_ratio can be used to help balance the load.
/// UTparallelFor() uses tbb for its implementation.
/// The used grain size is the maximum of min_grain_size and
/// if UTestimatedNumItems(range) / (subscribe_ratio * num_processor).
/// If subscribe_ratio == 0, then a grain size of min_grain_size will be used.
/// A range can be split only when UTestimatedNumItems(range) exceeds the
/// grain size the range is divisible. 

///
/// Requirements for the Range functor are:
///   - the requirements of the tbb Range Concept
///   - UT_estimatorNumItems<Range> must return the the estimated number of work items
///     for the range. When Range::size() is not the correct estimate, then a 
///     (partial) specialization of UT_estimatorNumItemsimatorRange must be provided
///     for the type Range.
///
/// Requirements for the Body function are:
///  - @code Body(const Body &); @endcode @n
///	Copy Constructor
///  - @code Body()::~Body(); @endcode @n
///	Destructor
///  - @code void Body::operator()(const Range &range) const; @endcode
///	Function call to perform operation on the range.  Note the operator is
///	@b const.
///
/// The requirements for a Range object are:
///  - @code Range::Range(const Range&); @endcode @n
///	Copy constructor
///  - @code Range::~Range(); @endcode @n
///	Destructor
///  - @code bool Range::is_divisible() const; @endcode @n
///	True if the range can be partitioned into two sub-ranges
///  - @code bool Range::empty() const; @endcode @n
///	True if the range is empty
///  - @code Range::Range(Range &r, UT_Split) const; @endcode @n
///	Split the range @c r into two sub-ranges (i.e. modify @c r and *this)
///
/// Example: @code
///     class Square {
///     public:
///          Square(fpreal *data) : myData(data) {}
///         ~Square();
///         void        operator()(const UT_BlockedRange<int64> &range) const
///                     {
///                         for (int64 i = range.begin(); i != range.end(); ++i)
///                             myData[i] *= myData[i];
///                     }
///         fpreal      *myData;
///     };
///     ...
///
///     void
///     parallel_square(fpreal *array, int64 length)
///     {
///         UTparallelFor(UT_BlockedRange<int64>(0, length), Square(array));
///     }
/// @endcode
///	
/// @see UTparallelReduce(), UT_BlockedRange()

template <typename Range, typename Body>
void UTparallelFor(
    const Range &range, const Body &body,
    const int subscribe_ratio = 2,
    const int min_grain_size = 1
)
{
    const size_t num_processors( UT_Thread::getNumProcessors() );

    UT_ASSERT( num_processors >= 1 );
    UT_ASSERT( min_grain_size >= 1 );
    UT_ASSERT( subscribe_ratio >= 0 );

    const size_t est_range_size( UTestimatedNumItems(range) );

    // Don't run on an empty range!
    if (est_range_size == 0)
        return;

    // Avoid tbb overhead if entire range needs to be single threaded
    if (num_processors == 1 || est_range_size <= min_grain_size ||
        !UT_Thread::isThreadingEnabled())
    {
        body(range);
        return;
    }

    size_t grain_size(min_grain_size);
    if( subscribe_ratio > 0 )
        grain_size = std::max(
                         grain_size, 
                         est_range_size / (subscribe_ratio * num_processors)
                     );

    UT_CoarsenedRange< Range > coarsened_range(range, grain_size);

    if (UTperformanceIsRecordingThreadStats())
    {
	tbb::parallel_for(
	    coarsened_range, ut_TaskScopedBody<Range, Body>(&body), 
	    tbb::simple_partitioner());
    }
    else
    {
	tbb::parallel_for(coarsened_range, body, tbb::simple_partitioner());
    }
}

/// Version of UTparallelFor that is tuned for the case where the range
/// consists of lightweight items, for example,
/// float additions or matrix-vector multiplications.
template <typename Range, typename Body>
void
UTparallelForLightItems(const Range &range, const Body &body)
{
    UTparallelFor(range, body, 2, 1024);
}

/// Version of UTparallelFor that is tuned for the case where the range
/// consists of heavy items, for example, defragmenting an entire attribute.
///
/// If possible, UTparallelForEachNumber() is preferred over use of
/// UTparallelForHeavyItems().
///
/// Note, when the range is guaranteed to be small, you might prefer to run
/// <tt>UTparallelFor(range, body, 0, 1)</tt>.  That form of the loop would
/// guarantee that a separate task is started for each iteration of the body.
/// However, that form can cause issues when the range gets large, in that a @b
/// large number of tasks may be created.
///
template <typename Range, typename Body>
SYS_DEPRECATED_REPLACE(16.5, "UTparallelForEachNumber||UTparallelFor(r,b,0,1)")
void
UTparallelForHeavyItems(const Range &range, const Body &body)
{
    // By oversubscribing by 32, small ranges will still be split into
    // individual tasks.  However, large ranges will be chunked, causing fewer
    // tasks, but potentially worse load balancing.
    //
    // Consider using UTparallelForEachNumber() instead.
    UTparallelFor(range, body, 32, 1);
}

/// Version of UTparallelFor tuned for a range consists of heavy items, for
/// example, defragmenting an entire attribute.
///
/// This approach uses "ideal" load balancing across threads and doesn't rely
/// on the TBB task scheduler for splitting the range.  Instead, it iterates
/// from @c 0 to @c nitems, calling @c body with a UT_BlockedRange<IntType>
/// containing a list of tasks to execute.
///
/// @note The @c IntType must work with @c SYS_AtomicInt (currently int32 or
/// int64).  If you get a boost static assertion, please make sure the @c body
/// range takes the proper integer type.
template <typename IntType, typename Body>
void
UTparallelForEachNumber(IntType nitems, const Body &body)
{
    const size_t	num_processors(UT_Thread::getNumProcessors());

    UT_ASSERT(num_processors >= 1);
    if (nitems == 0)
	return;
    if (num_processors == 1)
    {
	body(UT_BlockedRange<IntType>(0, nitems));
	return;
    }
    if (nitems <= num_processors)
    {
	// When there are a small number of tasks, split into a single task per
	// thread.
	UTparallelFor(UT_BlockedRange<IntType>(0, nitems), body, 0, 1);
	return;
    }

    // Split across number of processors, with each thread using the atomic int
    // to query the next task to be run (similar to UT_ThreadedAlgorithm)
    SYS_AtomicInt<IntType>	it(0);
    UTparallelFor(UT_BlockedRange<IntType>(0, num_processors),
	    ut_ForEachNumberBody<IntType, Body>(body, it, nitems), 0, 1);
}

/// UTserialFor can be used as a debugging tool to quickly replace a parallel
/// for with a serial for.
template <typename Range, typename Body>
void UTserialFor(const Range &range, const Body &body)
	{ body(range); }

/// UTparallelInvoke() executes the given functions in parallel when the
/// parallel flag is true - otherwise it runs them serially.  F1 and F2
/// should be void functors.
template <typename F1, typename F2>
inline void UTparallelInvoke(bool parallel, F1 &&f1, F2 &&f2)
{
    if (parallel && UT_Thread::isThreadingEnabled())
    {
	tbb::parallel_invoke(std::forward<F1>(f1), std::forward<F2>(f2));
    }
    else
    {
	f1();
	f2();
    }
}

template <typename F1, typename F2, typename... Rest>
inline void UTparallelInvoke(bool parallel, F1 &&f1, F2 &&f2, Rest&&... rest)
{
    if (parallel && UT_Thread::isThreadingEnabled())
    {
        tbb::parallel_invoke(std::forward<F1>(f1), std::forward<F2>(f2),
                             std::forward<Rest>(rest)...);
    }
    else
    {
	f1();
        UTparallelInvoke(parallel, f2, std::forward<Rest>(rest)...);
    }
}

template <typename F1>
class UT_ParallelInvokePointers
{
public:
    UT_ParallelInvokePointers(const UT_Array<F1 *> &functions)
	: myFunctions(functions) {}
    void operator()(const tbb::blocked_range<int>& r ) const
    {
	for (int i = r.begin(); i != r.end(); ++i)
	    (*myFunctions(i))();
    }
private:
    const UT_Array<F1 *>	&myFunctions;
};

/// UTparallelInvoke() executes the array of functions in parallel when the
/// parallel flag is true - otherwise it runs them serially.  F1 should be
/// a void functor.
template <typename F1>
inline void UTparallelInvoke(bool parallel, const UT_Array<F1 *> &funs)
{
    if (parallel && funs.entries() > 1 && UT_Thread::isThreadingEnabled())
    {
	tbb::parallel_for(tbb::blocked_range<int>(0, funs.entries(), 1),
		UT_ParallelInvokePointers<F1>(funs));
    }
    else
    {
	for (int i = 0; i < funs.entries(); i++)
	    (*funs(i))();
    }
}

template <typename F1>
class UT_ParallelInvokeFunctors
{
public:
    UT_ParallelInvokeFunctors(const UT_Array<F1> &functions)
	: myFunctions(functions) {}
    void operator()(const tbb::blocked_range<int>& r ) const
    {
	for (int i = r.begin(); i != r.end(); ++i)
	    myFunctions(i)();
    }
private:
    const UT_Array<F1>	&myFunctions;
};

/// UTparallelInvoke() executes the array of functions in parallel when the
/// parallel flag is true - otherwise it runs them serially.  F1 should be
/// a void functor.
template <typename F1>
inline void UTparallelInvoke(bool parallel, const UT_Array<F1> &funs)
{
    if (parallel && funs.entries() > 1 && UT_Thread::isThreadingEnabled())
    {
	tbb::parallel_for(tbb::blocked_range<int>(0, funs.entries(), 1),
		UT_ParallelInvokeFunctors<F1>(funs));
    }
    else
    {
	for (int i = 0; i < funs.entries(); i++)
	    funs(i)();
    }
}

/// Helper class for UTparallelInvoke().
/// Wraps the thread body in a task scope so that thread stats are collected
/// by the performance monitor, and child tasks can inherit task scope locks
/// from the parent task.
template<typename Body>
class ut_TaskScopedInvokeBody
{
public:
    ut_TaskScopedInvokeBody(const Body &body)
	: myBody(body),
	  myParentTaskScope(UT_TaskScope::getCurrent())
    {
    }

    ut_TaskScopedInvokeBody(const ut_TaskScopedInvokeBody &src)
	: myBody(src.myBody),
	  myParentTaskScope(src.myParentTaskScope)
    {
    }

    void operator()() const
    {
	UT_TaskScope task_scope(myParentTaskScope);
	myBody();
    }

private:
    const Body &myBody;
    const UT_TaskScope *myParentTaskScope;
};

/// Takes a functor for passing to UTparallelInvoke, and wraps it in a
/// ut_TaskScopeInvokeBody object so the functor will be invoked wrapped in
/// a UT_TaskScope that makes it safe to use UT_TaskLock objects that are
/// currently locked by the parent scope.
template <typename Body>
const ut_TaskScopedInvokeBody<Body>
UTmakeTaskScopedInvokeBody(const Body &body)
{
    return ut_TaskScopedInvokeBody<Body>(body);
}

template <typename RANGE, typename BODY>
class UT_ParallelForTaskImpl : public UT_Task
{
public:
    UT_ParallelForTaskImpl(const RANGE &range, const BODY &body)
	: myRange(range)
	, myBody(body)
    {
    }

    static UT_Task *
    create(const RANGE &range, const BODY &body)
    {
	return new (UT_Task::allocate_root())
			UT_ParallelForTaskImpl(range, body);
    }

private:
    virtual UT_Task *run()
    {
        UT_ASSERT(UT_Thread::isThreadingEnabled());
	tbb::parallel_for(myRange, myBody, tbb::simple_partitioner());
	return NULL;
    }

    RANGE		myRange;
    BODY		myBody;
};

/// Append a UTparallelFor() task to a UT_TaskList for later spawning en masse.
/// @see UTparallelFor(), UT_TaskList
template <typename RANGE, typename BODY>
void
UTparallelForAppendToTaskList(
	UT_TaskList &task_list,
	const RANGE &range,
	const BODY &body)
{
    task_list.append(
	*UT_ParallelForTaskImpl<RANGE, BODY>::create(range, body));
}

/// UTparallelReduce() is a simple wrapper that uses tbb for its implementation.
/// Run the @c body function over a range in parallel.
///
/// WARNING: The @c operator()() and @c join() functions MUST @b NOT initialize
///          data!  @b Both of these functions MUST ONLY accumulate data!  This
///          is because TBB may re-use body objects for multiple ranges.
///          Effectively, operator()() must act as an in-place join operation
///          for data as it comes in.  Initialization must be kept to the
///          constructors of Body.
///
/// Requirements for the Body function are:
///  - @code Body()::~Body(); @endcode @n
///	Destructor
///  - @code Body::Body(Body &r, UT_Split) const; @endcode @n
///	The splitting constructor.
///	WARNING: This must be able to run concurrently with calls to
///	         @c r.operator()() and @c r.join(), so this should not copy
///	         values accumulating in r.
///  - @code void Body::operator()(const Range &range); @endcode
///	Function call to perform operation on the range.  Note the operator is
///	@b not const.
///  - @code void Body::join(const Body &other); @endcode
///	Join the results from another operation with this operation.
///	@b not const.
///
/// The requirements for a Range object are:
///  - @code Range::Range(const Range&); @endcode @n
///	Copy constructor
///  - @code Range::~Range(); @endcode @n
///	Destructor
///  - @code bool Range::is_divisible() const; @endcode @n
///	True if the range can be partitioned into two sub-ranges
///  - @code bool Range::empty() const; @endcode @n
///	True if the range is empty
///  - @code Range::Range(Range &r, UT_Split) const; @endcode @n
///	Split the range @c r into two sub-ranges (i.e. modify @c r and *this)
///
/// Example: @code
///    class Dot {
///    public:
///     Dot(const fpreal *a, const fpreal *b)
///         : myA(a)
///         , myB(b)
///         , mySum(0)
///     {}
///     Dot(Dot &src, UT_Split)
///         : myA(src.myA)
///         , myB(src.myB)
///         , mySum(0)
///     {}
///     void    operator()(const UT_BlockedRange<int64> &range)
///             {
///                  for (int64 i = range.begin(); i != range.end(); ++i)
///                      mySum += myA[i] * myB[i];
///             }
///     void    join(const Dot &other)
///             {
///                 mySum += other.mySum;
///             }
///     fpreal          mySum;
///     const fpreal    *myA, *myB;
///    };
///
///    fpreal
///    parallel_dot(const fpreal *a, const fpreal *b, int64 length)
///    {
///     Dot     body(a, b);
///     UTparallelReduce(UT_BlockedRange<int64>(0, length), body);
///     return body.mySum;
///    }
/// @endcode
/// @see UTparallelFor(), UT_BlockedRange()
template <typename Range, typename Body>
void UTparallelReduce(
	const Range &range,
	Body &body,
	const int subscribe_ratio = 2,
	const int min_grain_size = 1
)
{
    const size_t num_processors( UT_Thread::getNumProcessors() );

    UT_ASSERT( num_processors >= 1 );
    UT_ASSERT( min_grain_size >= 1 );
    UT_ASSERT( subscribe_ratio >= 0 );

    const size_t est_range_size( UTestimatedNumItems(range) );

    // Don't run on an empty range!
    if (est_range_size == 0)
        return;

    // Avoid tbb overhead if entire range needs to be single threaded
    if (num_processors == 1 || est_range_size <= min_grain_size ||
        !UT_Thread::isThreadingEnabled())
    {
        body(range);
        return;
    }

    size_t grain_size(min_grain_size);
    if( subscribe_ratio > 0 )
        grain_size = std::max(
                         grain_size, 
                         est_range_size / (subscribe_ratio * num_processors)
                     );

    UT_CoarsenedRange< Range > coarsened_range(range, grain_size);

    tbb::parallel_reduce(coarsened_range, body, tbb::simple_partitioner());
}

/// This is a simple wrapper for deterministic reduce that uses tbb. It
/// works in the same manner as UTparallelReduce, with the following
/// differences:
///  - reduction and join order is deterministic (devoid of threading
///    uncertainty;
///  - a fixed grain size must be provided by the caller; grain size is
///    not adjusted based on the available resources (this is required to
///    satisfy determinism).
/// This version should be used when task joining is not associative (such
/// as accumulation of a floating point residual).
template <typename Range, typename Body>
void UTparallelDeterministicReduce(
    const Range &range,
    Body &body,
    const int grain_size
)
{
    UT_ASSERT( grain_size >= 1 );

    const size_t est_range_size( UTestimatedNumItems(range) );

    // Don't run on an empty range!
    if (est_range_size == 0)
        return;

    UT_ASSERT_MSG(UT_Thread::isThreadingEnabled(),
        "FIXME: There needs to be a way to do identical splits and joins when single-threading,"
        " to avoid having different roundoff error from when multi-threading. "
        " Something using simple_partitioner() might work.");

    UT_CoarsenedRange< Range > coarsened_range(range, grain_size);
    tbb::parallel_deterministic_reduce(coarsened_range, body);
}

/// Version of UTparallelReduce that is tuned for the case where the range
/// consists of lightweight items, for example, finding the min/max in a set of
/// integers.
template <typename Range, typename Body>
void UTparallelReduceLightItems(const Range &range, Body &body)
{
    UTparallelReduce(range, body, 2, 1024);
}

/// Version of UTparallelReduce that is tuned for the case where the range
/// consists of heavy items, for example, computing the bounding box of a list
/// of geometry objects.
template <typename Range, typename Body>
void UTparallelReduceHeavyItems(const Range &range, Body &body)
{
    UTparallelReduce(range, body, 0, 1);
}

/// UTserialReduce can be used as a debugging tool to quickly replace a
/// parallel reduce with a serial for.
template <typename Range, typename Body>
void UTserialReduce(const Range &range, Body &body)
	{ body(range); }

/// UTparallelSort() is a simple wrapper that uses tbb for its implementation.
///
/// WARNING: UTparallelSort is UNSTABLE!  You must explicitly force stability
///          if needed.
template <typename RandomAccessIterator, typename Compare>
void UTparallelSort(RandomAccessIterator begin, RandomAccessIterator end, const Compare &compare)
{
    if (UT_Thread::isThreadingEnabled())
        tbb::parallel_sort(begin, end, compare);
    else
        std::sort(begin, end, compare);
}

/// UTparallelSort() is a simple wrapper that uses tbb for its implementation.
///
/// WARNING: UTparallelSort is UNSTABLE!  You must explicitly force stability
///          if needed.
template <typename RandomAccessIterator>
void UTparallelSort(RandomAccessIterator begin, RandomAccessIterator end)
{
    if (UT_Thread::isThreadingEnabled())
        tbb::parallel_sort(begin, end);
    else
        std::sort(begin, end);
}

/// UTparallelSort() is a simple wrapper that uses tbb for its implementation.
///
/// WARNING: UTparallelSort is UNSTABLE!  You must explicitly force stability
///          if needed.
template <typename T>
void UTparallelSort(T *begin, T *end)
{
    if (UT_Thread::isThreadingEnabled())
        tbb::parallel_sort(begin, end);
    else
        std::sort(begin, end);
}

// Forward declaration of parallel_stable_sort; implementation at end of file.
namespace pss
{
template<typename RandomAccessIterator, typename Compare>
void parallel_stable_sort( RandomAccessIterator xs, RandomAccessIterator xe,
			   Compare comp );

//! Wrapper for sorting with default comparator.
template<class RandomAccessIterator>
void parallel_stable_sort( RandomAccessIterator xs, RandomAccessIterator xe )
{
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type T;
    parallel_stable_sort( xs, xe, std::less<T>() );
}
}

/// UTparalleStableSort() is a stable parallel merge sort.
///
/// NOTE: UTparallelStableSort requires a temporary buffer of size end-begin.
///       On allocation failure it falls back to calling @c std::stable_sort.
/// NOTE: Element initialization is done via @c std::move, so non-POD element
///       types should implement c++11 move semantics.
template <typename RandomAccessIterator, typename Compare>
void UTparallelStableSort(RandomAccessIterator begin, RandomAccessIterator end,
			  const Compare &compare)
{
    pss::parallel_stable_sort(begin, end, compare);
}

/// UTparalleStableSort() is a stable parallel merge sort.
///
/// NOTE: UTparallelStableSort requires a temporary buffer of size end-begin.
///       On allocation failure it falls back to calling @c std::stable_sort.
/// NOTE: Element initialization is done via @c std::move, so non-POD element
///       types should implement c++11 move semantics.
template <typename RandomAccessIterator>
void UTparallelStableSort(RandomAccessIterator begin, RandomAccessIterator end)
{
    pss::parallel_stable_sort(begin, end);
}

/// UTparalleStableSort() is a stable parallel merge sort.
///
/// NOTE: UTparallelStableSort requires a temporary buffer of size end-begin.
///       On allocation failure it falls back to calling @c std::stable_sort.
/// NOTE: Element initialization is done via @c std::move, so non-POD element
///       types should implement c++11 move semantics.
template <typename T>
void UTparallelStableSort(T *begin, T *end)
{
    pss::parallel_stable_sort(begin, end);
}

/// UTparalleStableSort() is a stable parallel merge sort.
///
/// NOTE: UTparallelStableSort requires a temporary buffer of size end-begin.
///       On allocation failure it falls back to calling @c std::stable_sort.
/// NOTE: Element initialization is done via @c std::move, so non-POD element
///       types should implement c++11 move semantics.
template <typename T, typename Compare>
void UTparallelStableSort(T *begin, T *end, const Compare &compare)
{
    pss::parallel_stable_sort(begin, end, compare);
}


/// UTparalleStableSort() is a stable parallel merge sort.
/// This form works with UT_Array and other containers with begin/end members.
///
/// NOTE: UTparallelStableSort requires a temporary buffer of size end-begin.
///       On allocation failure it falls back to calling @c std::stable_sort.
/// NOTE: Element initialization is done via @c std::move, so non-POD element
///       types should implement c++11 move semantics.
template <typename T>
void
UTparallelStableSort(T &a)
{
    pss::parallel_stable_sort(a.begin(), a.end());
}


/// UTparalleStableSort() is a stable parallel merge sort.
/// This form works with UT_Array and other containers with begin/end members.
///
/// NOTE: UTparallelStableSort requires a temporary buffer of size end-begin.
///       On allocation failure it falls back to calling @c std::stable_sort.
/// NOTE: Element initialization is done via @c std::move, so non-POD element
///       types should implement c++11 move semantics.
template <typename T, typename Compare>
void
UTparallelStableSort(T &a, const Compare &compare)
{
    pss::parallel_stable_sort(a.begin(), a.end(), compare);
}

/// UT_BlockedRange() is a simple wrapper using tbb for its implementation
/// This meets the requirements for a Range object, which are:
///  - @code Range::Range(const Range&); @endcode @n
///	Copy constructor
///  - @code Range::~Range(); @endcode @n
///	Destructor
///  - @code bool Range::is_divisible() const; @endcode @n
///	True if the range can be partitioned into two sub-ranges
///  - @code bool Range::empty() const; @endcode @n
///	True if the range is empty
///  - @code Range::Range(Range &r, UT_Split) const; @endcode @n
///	Split the range @c r into two sub-ranges (i.e. modify @c r and *this)
template <typename T>
class UT_BlockedRange : public tbb::blocked_range<T>
{
public:
    // TBB 2018 U3 no longer supports default blocked_range constructors
    UT_BlockedRange() = delete;

    UT_BlockedRange(T begin_value, T end_value, size_t grainsize=1)
	: tbb::blocked_range<T>(begin_value, end_value, grainsize)
    {}
    UT_BlockedRange(UT_BlockedRange &R, UT_Split split)
	: tbb::blocked_range<T>(R, split)
    {}
};

/// UT_BlockedRange2D() is a simple wrapper using tbb for its implementation
/// This meets the requirements for a Range object, which are:
///  - @code Range::Range(const Range&); @endcode @n
///	Copy constructor
///  - @code Range::~Range(); @endcode @n
///	Destructor
///  - @code bool Range::is_divisible() const; @endcode @n
///	True if the range can be partitioned into two sub-ranges
///  - @code bool Range::empty() const; @endcode @n
///	True if the range is empty
///  - @code Range::Range(Range &r, UT_Split) const; @endcode @n
///	Split the range @c r into two sub-ranges (i.e. modify @c r and *this)
template <typename T_ROW, typename T_COL>
class UT_BlockedRange2D : public tbb::blocked_range2d<T_ROW, T_COL>
{
public:
    // TBB 2018 U3 no longer supports default blocked_range constructors
    UT_BlockedRange2D() = delete;

    /// NB:  The arguments are in a different order than tbb
    UT_BlockedRange2D(T_ROW row_begin, T_ROW row_end,
		      T_COL col_begin, T_COL col_end,
		      size_t row_grainsize=1, size_t col_grainsize=1)
	: tbb::blocked_range2d<T_ROW, T_COL>(row_begin, row_end, row_grainsize,
				col_begin, col_end, col_grainsize)
    {}
    UT_BlockedRange2D(UT_BlockedRange2D &R, UT_Split split)
	: tbb::blocked_range2d<T_ROW, T_COL>(R, split)
    {}
};

/// @{
/// Wrapper around TBB's task isolation.  In versions of TBB that don't support
/// isolate, this uses a task arena.
#if TBB_VERSION_MAJOR >= 2018
template <typename F> static inline void
UTisolate(F &f) { tbb::this_task_arena::isolate(f); }

template <typename F> static inline void
UTisolate(const F &f) { tbb::this_task_arena::isolate(f); }
#else
template <typename F> static inline void
UTisolate(F &f)
{
    tbb::task_arena __nested;
    __nested.execute(f);
}
template <typename F> static inline void
UTisolate(const F &f)
{
    tbb::task_arena __nested;
    __nested.execute(f);
}
#endif
/// @}

// The code below is originally from:
// https://software.intel.com/en-us/articles/a-parallel-stable-sort-using-c11-for-tbb-cilk-plus-and-openmp
// and is covered by the following copyright:
/*
  Copyright (C) 2014 Intel Corporation
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

  * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in
    the documentation and/or other materials provided with the
    distribution.
  * Neither the name of Intel Corporation nor the names of its
    contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
  WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
*/
#include <utility>
#include <iterator>
#include <algorithm>

namespace pss {

namespace internal {

//! Destroy sequence [xs,xe)
template<class RandomAccessIterator>
void serial_destroy( RandomAccessIterator zs, RandomAccessIterator ze ) {
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type T;
    while( zs!=ze ) {
        --ze;
        (*ze).~T();
    }
}

//! Merge sequences [xs,xe) and [ys,ye) to output sequence [zs,(xe-xs)+(ye-ys)), using std::move
template<class RandomAccessIterator1, class RandomAccessIterator2, class RandomAccessIterator3, class Compare>
void serial_move_merge( RandomAccessIterator1 xs, RandomAccessIterator1 xe, RandomAccessIterator2 ys, RandomAccessIterator2 ye, RandomAccessIterator3 zs, Compare comp ) {
    if( xs!=xe ) {
        if( ys!=ye )
        {
            for(;;)
            {
                if( comp(*ys,*xs) ) {
                    *zs = std::move(*ys);
                    ++zs;
                    if( ++ys==ye ) break;
                } else {
                    *zs = std::move(*xs);
                    ++zs;
                    if( ++xs==xe ) goto movey;
                }
            }
        }
        ys = xs;
        ye = xe;
    }
movey:
    std::move( ys, ye, zs );
}

template<typename RandomAccessIterator1, typename RandomAccessIterator2, typename Compare>
void stable_sort_base_case(  RandomAccessIterator1 xs, RandomAccessIterator1 xe, RandomAccessIterator2 zs, int inplace, Compare comp) {
    std::stable_sort( xs, xe, comp );
    if( inplace!=2 ) {
        RandomAccessIterator2 ze = zs + (xe-xs);
        typedef typename std::iterator_traits<RandomAccessIterator2>::value_type T;
        if( inplace )
            // Initialize the temporary buffer
            for( ; zs<ze; ++zs )
                new(&*zs) T;
        else
            // Initialize the temporary buffer and move keys to it.
            for( ; zs<ze; ++xs, ++zs )
                new(&*zs) T(std::move(*xs));
    }
}

//! Raw memory buffer with automatic cleanup.
class raw_buffer {
    void* ptr;
public:
    //! Try to obtain buffer of given size.
    raw_buffer( size_t bytes ) : ptr( operator new(bytes,std::nothrow) ) {}
    //! True if buffer was successfully obtained, zero otherwise.
    operator bool() const {return ptr;}
    //! Return pointer to buffer, or  NULL if buffer could not be obtained.
    void* get() const {return ptr;}
    //! Destroy buffer
    ~raw_buffer() {operator delete(ptr);}
};

template<typename RandomAccessIterator1, typename RandomAccessIterator2, typename RandomAccessIterator3, typename Compare>
void parallel_merge( RandomAccessIterator1 xs, RandomAccessIterator1 xe, RandomAccessIterator2 ys,
                     RandomAccessIterator2 ye, RandomAccessIterator3 zs, bool destroy, Compare comp );

template<typename RandomAccessIterator1, typename RandomAccessIterator2, typename RandomAccessIterator3, typename Compare>
struct parallel_merge_invoke
{
  RandomAccessIterator1 _xs, _xe;
  RandomAccessIterator2 _ys, _ye;
  RandomAccessIterator3 _zs;
  bool _destroy;
  Compare _comp;
  parallel_merge_invoke( RandomAccessIterator1 xs, RandomAccessIterator1 xe, RandomAccessIterator2 ys, RandomAccessIterator2 ye,
                          RandomAccessIterator3 zs, bool destroy, Compare comp):
                          _xs(xs), _xe(xe), _ys(ys), _ye(ye), _zs(zs), _destroy(destroy), _comp(comp) {}

  void operator ()() const { parallel_merge(_xs, _xe, _ys, _ye, _zs, _destroy, _comp);}

};

// Merge sequences [xs,xe) and [ys,ye) to output sequence [zs,zs+(xe-xs)+(ye-ys))
template<typename RandomAccessIterator1, typename RandomAccessIterator2, typename RandomAccessIterator3, typename Compare>
void parallel_merge( RandomAccessIterator1 xs, RandomAccessIterator1 xe, RandomAccessIterator2 ys,
                     RandomAccessIterator2 ye, RandomAccessIterator3 zs, bool destroy, Compare comp ) {
    const size_t MERGE_CUT_OFF = 2000;
    if( (xe-xs) + (ye-ys) <= MERGE_CUT_OFF ) {
        serial_move_merge( xs, xe, ys, ye, zs, comp );
        if( destroy ) {
            serial_destroy( xs, xe );
            serial_destroy( ys, ye );
        }
    } else {
        RandomAccessIterator1 xm;
        RandomAccessIterator2 ym;
        if( xe-xs < ye-ys  ) {
            ym = ys+(ye-ys)/2;
            xm = std::upper_bound(xs,xe,*ym,comp);
        } else {
            xm = xs+(xe-xs)/2;
            ym = std::lower_bound(ys,ye,*xm,comp);
        }
        RandomAccessIterator3 zm = zs + ((xm-xs) + (ym-ys));
        tbb::parallel_invoke( parallel_merge_invoke<RandomAccessIterator1, RandomAccessIterator2, RandomAccessIterator3, Compare>( xs, xm, ys, ym, zs, destroy, comp ),
                              parallel_merge_invoke<RandomAccessIterator1, RandomAccessIterator2, RandomAccessIterator3, Compare>( xm, xe, ym, ye, zm, destroy, comp ));
    }
}

template<typename RandomAccessIterator1, typename RandomAccessIterator2, typename Compare>
void parallel_stable_sort_aux( RandomAccessIterator1 xs, RandomAccessIterator1 xe, RandomAccessIterator2 zs, int inplace, Compare comp );

template<typename RandomAccessIterator1, typename RandomAccessIterator2, typename Compare>
struct parallel_stable_sort_aux_invoke
{
  RandomAccessIterator1 _xs, _xe;
  RandomAccessIterator2 _zs;
  bool _inplace;
  Compare _comp;
  parallel_stable_sort_aux_invoke( RandomAccessIterator1 xs, RandomAccessIterator1 xe, RandomAccessIterator2 zs, int inplace, Compare comp ):
    _xs(xs), _xe(xe), _zs(zs), _inplace(inplace), _comp(comp) {}

  void operator ()() const { parallel_stable_sort_aux(_xs, _xe, _zs, _inplace, _comp);}

};

// Sorts [xs,xe), where zs[0:xe-xs) is temporary buffer supplied by caller.
// Result is in [xs,xe) if inplace==true, otherwise in [zs,zs+(xe-xs))
template<typename RandomAccessIterator1, typename RandomAccessIterator2, typename Compare>
void parallel_stable_sort_aux( RandomAccessIterator1 xs, RandomAccessIterator1 xe, RandomAccessIterator2 zs, int inplace, Compare comp ) {
    const size_t SORT_CUT_OFF = 500;
    if( xe-xs<=SORT_CUT_OFF ) {
        stable_sort_base_case(xs, xe, zs, inplace, comp);
    } else {
       RandomAccessIterator1 xm = xs + (xe-xs)/2;
       RandomAccessIterator2 zm = zs + (xm-xs);
       RandomAccessIterator2 ze = zs + (xe-xs);
       tbb::parallel_invoke( parallel_stable_sort_aux_invoke<RandomAccessIterator1, RandomAccessIterator2, Compare>( xs, xm, zs, !inplace, comp ),
                             parallel_stable_sort_aux_invoke<RandomAccessIterator1, RandomAccessIterator2, Compare>( xm, xe, zm, !inplace, comp ));
       if( inplace )
           parallel_merge( zs, zm, zm, ze, xs, inplace==2, comp );
       else
           parallel_merge( xs, xm, xm, xe, zs, false, comp );
   }
}
} // namespace internal

template<typename RandomAccessIterator, typename Compare>
void parallel_stable_sort( RandomAccessIterator xs, RandomAccessIterator xe, Compare comp ) {
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type T;
    internal::raw_buffer z = internal::raw_buffer( sizeof(T)*(xe-xs) );
    if( z && UT_Thread::isThreadingEnabled() )
        internal::parallel_stable_sort_aux( xs, xe, (T*)z.get(), 2, comp );
    else
        // Not enough memory available - fall back on serial sort
        std::stable_sort( xs, xe, comp );
}

} // namespace pss


#endif
