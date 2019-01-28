/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 *     This class defines a thread-specific pointer that can be used to
 *     implement thread-local storage.  Each thread that accesses the
 *     pointer will have its own copy of the pointer's value, and the
 *     initial value for all threads is null.
 *
 *     To replace a global variable like
 *
 *         static int theGlobalValue = 0;
 *         // use theGlobalValue
 *
 *     to use thread-local storage, you would write something like
 *
 *         static UT_ThreadSpecificValue<int> theGlobalValue;
 *         // use theGlobalValue.get()
 *
 *     Note that the default constructor for the values will be called.
 *     (For native data types like ints, floats, pointers, etc., they will
 *     be initialized to zero/null.)
 *
 *     This class has been tuned for speed.  Speed of TLS is surprisingly
 *     sensitive to changes to this class.
 */

#ifndef __UT_ThreadSpecificValue_h__
#define __UT_ThreadSpecificValue_h__

#include "UT_SpinLock.h"
#include "UT_ConcurrentHashMap.h"
#include <SYS/SYS_Align.h>
#include <SYS/SYS_TypeTraits.h>
#include <SYS/SYS_Platform.h>
#include <SYS/SYS_SequentialThreadIndex.h>
#include <limits>
#include <SYS/SYS_Math.h>
#include <SYS/SYS_StaticAssert.h>

#ifdef HAS_NO_TLS
static const size_t	theNbStaticThreadValues = 1;
#else
static const size_t	theNbStaticThreadValues = 32;
#endif

template <typename T>
class UT_ThreadSpecificValue
{
private:
    template <typename U>
    class SYS_ALIGN(64) AlignType
    {
    public:
	AlignType()
	{
	    // If T is an object, the default constructor will have been
	    // called.  Otherwise, initialize the memory to 0.
	    if (SYSisPOD<U>())
		memset((void *)&myValue, 0, sizeof(U));
	}

	operator const U &() const
	{
	    return myValue;
	}
	operator U &()
	{
	    return myValue;
	}

	U &operator=(const U &v)
	{
	    if (&v != &myValue)
		myValue = v;
	    return myValue;
	}

    private:
	U	myValue;
    };

    typedef UT_ConcurrentHashMap<int, T *>	ut_DynamicValueMap;

public:

    typedef T value_type;

    UT_ThreadSpecificValue()
    {
	// Hello.
	
	// You're probably here because your build failed with a weird error.
	// Please try to keep your TLS data as small as possible.
	// UT_ThreadSpecificValue initializes a large block of pre-allocated 
	// values and this adds up over the large number of instances of it
	// there are in the system.
	// If large data structures are required, initialize them on-demand.
	// See prm_ThreadData in $PRM/PRM_ChoiceList.C for an example.
	SYS_STATIC_ASSERT(sizeof(T) <= 1024);

	// Ensure cache-line alignment.
	SYS_STATIC_ASSERT(sizeof(myStaticThreadValues[0]) >= 64);
    }

    ~UT_ThreadSpecificValue()
    {
	typename ut_DynamicValueMap::iterator	it;
	for (it = myDynamicThreadValues.begin();
	     it != myDynamicThreadValues.end(); ++it)
	    delete it->second;
    }

    // SYSgetSTID() will "allocate" an index for this thread
    // if it has never been called from this thread.  The thread indices it
    // returns start at 0.  See $SYS/SYS_SequentialThreadIndex.h for more
    // information.
    //
    /// Access the value for a particular thread index.
    /// Note that the threads you care about may not have
    /// been assigned sequentially!  Always loop up to the maxthreads
    /// value and be able to handle zero-initialized empty data.
    T &getValueForThread(int thread_index)
    {
	// We want to catch cases where people are passing in the default 0
	// thread value. However, for HDK users, we will fall back to computing
	// it automatically for correctness.
#ifdef HAS_NO_TLS
	return myStaticThreadValues[0];
#else
	UT_ASSERT_P(thread_index > 0);
	if (thread_index == 0)
	    thread_index = SYSgetSTID();

	thread_index -= SYS_SequentialThreadIndex::FIRST_INDEX;
	if (thread_index < theNbStaticThreadValues)
	    return myStaticThreadValues[thread_index];

	return getDynamicValue(thread_index);
#endif
    }
    const T &getValueForThread(int thread_index) const
    {
#ifdef HAS_NO_TLS
	return myStaticThreadValues[0];
#else
	UT_ASSERT_P(thread_index > 0);
	if (thread_index == 0)
	    thread_index = SYSgetSTID();

	thread_index -= SYS_SequentialThreadIndex::FIRST_INDEX;
	if (thread_index < theNbStaticThreadValues)
	    return myStaticThreadValues[thread_index];

	return getDynamicValue(thread_index);
#endif
    }
    
    T &get()			{ return getValueForThread(SYSgetSTID()); }
    T &local()			{ return getValueForThread(SYSgetSTID()); }

    const T &get() const	{ return getValueForThread(SYSgetSTID()); }
    const T &local() const	{ return getValueForThread(SYSgetSTID()); }

    /// const_iterator
    ///
    /// @note The iterator iterates over ALL possible thread values, thus
    /// you must be aware that the get() method will return the default value
    /// for cases where this variable was never used in a thread.
    class const_iterator
    {
    public:
	const_iterator()
	    : myConstVal(0)
	    , myI(0)
	{
	}
	const_iterator(const const_iterator &copy)
	    : myConstVal(copy.myConstVal)
	    , myI(copy.myI)
	{
	}
	const_iterator &operator=(const const_iterator &copy)
	{
	    if (this != &copy)
	    {
		myConstVal = copy.myConstVal;
		myI = copy.myI;
	    }
	    return *this;
	}

	const T &get() const
	{
	    if (myI < theNbStaticThreadValues)
		return myConstVal->myStaticThreadValues[myI];
	    return *myDynamicIt->second;
	}

	int thread() const
	{
	    int	val;
	    if (myI < theNbStaticThreadValues)
		val = myI;
	    else
		val = myDynamicIt->first;
	    return SYS_SequentialThreadIndex::FIRST_INDEX + val;
	}

	const_iterator &operator++()
	{
	    ++myI;
	    if (myI == theNbStaticThreadValues)
		myDynamicIt = myConstVal->myDynamicThreadValues.begin();
	    else if (myI > theNbStaticThreadValues)
		++myDynamicIt;
	    return *this;
	}

	bool operator==(const const_iterator &right)
	{
	    return (myConstVal == right.myConstVal
		    && myI == right.myI);
	}
	bool operator!=(const const_iterator &right)
	{
	    return !(*this == right);
	}

    protected:
	const_iterator(const UT_ThreadSpecificValue<T> *value, int start)
	    : myConstVal(value)
	    , myI(start)
	{
	}

	const UT_ThreadSpecificValue<T>			*myConstVal;
	int						 myI;
	typename ut_DynamicValueMap::const_iterator	 myDynamicIt;

	template <typename TY> friend class UT_ThreadSpecificValue;
    };

    /// iterator
    ///
    /// @note The iterator iterates over ALL possible thread values, thus
    /// you must be aware that the get() method will return the default value
    /// for cases where this variable was never used in a thread.
    class iterator : public const_iterator
    {
    public:
	iterator()
	    : const_iterator()
	{
	}
	iterator(const iterator &copy)
	    : const_iterator(copy)
	{
	}
	iterator &operator=(const iterator &copy)
	{
	    this->const_iterator::operator=(copy);
	    return *this;
	}

	T &get()
	{
	    return const_cast<T &>(const_iterator::get());
	}

	iterator &operator++()
	{
	    const_iterator::operator++();
	    return *this;
	}

    private:
	iterator(UT_ThreadSpecificValue<T> *value, int start)
	    : const_iterator((const UT_ThreadSpecificValue<T> *)value, start)
	{
	}

	template <typename TY> friend class UT_ThreadSpecificValue;
    };

    /// begin() const iterator
    const_iterator begin() const
	{ return const_iterator(this, 0); }
    /// end() const iterator
    const_iterator end() const
	{ return const_iterator(this, maxThreadsSeen()); }

    /// begin() iterator
    iterator begin()
	{ return iterator(this, 0); }
    /// end() iterator
    iterator end()
	{ return iterator(this, maxThreadsSeen()); }

    /// The number of values that require iteration.  Don't use this for
    /// iteration - use iterators instead.
    int maxThreadsSeen() const
    {
	// This may be less than the actual number if someone
	// is currently writing to myDynamicValues, but then one
	// is doing something very questionable.
	return theNbStaticThreadValues + myDynamicThreadValues.size();
    }

    /// Clear values for all threads, resetting to the initial state
    void clear()
    {
	*this = UT_ThreadSpecificValue<T>();
    }

    exint getMemoryUsage(bool inclusive) const
    {
        exint mem = inclusive ? sizeof(*this) : 0;

        mem += UTgetMemoryUsage(myDynamicThreadValues, false);

        // NOTE: UT_ThreadSpecificValue owns the content, not just the map.
        mem += myDynamicThreadValues.size() * sizeof(T);

        return mem;
    }

private:
    UT_ThreadSpecificValue(const UT_ThreadSpecificValue<T> &); // unimplemented

    T &getDynamicValue(int thread_index) const
    {
	// Check if the value is in the map. If not, we'll hold the
	// read-write accessor to that entry so that a new one can be
	// constructed.
	typename ut_DynamicValueMap::accessor	a;
	if (myDynamicThreadValues.insert(a, thread_index))
	{
	    a->second = new T;
	    if (SYSisPOD<T>())
		::memset((void *)a->second, 0, sizeof(T));
	}
	return *a->second;
    }

private:
    AlignType<T>		 myStaticThreadValues[theNbStaticThreadValues];
    mutable ut_DynamicValueMap	 myDynamicThreadValues;

    friend class const_iterator;
};

#endif
