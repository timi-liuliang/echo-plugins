/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SYS_AtomicPtr.h ( UT Library, C++)
 *
 * COMMENTS:	Thread-safe operations on pointer types.
 */

#ifndef __SYS_AtomicPtr__
#define __SYS_AtomicPtr__

#include "SYS_Types.h"
#include "SYS_AtomicInt.h"

#include <cstddef>

template <typename T>
class SYS_AtomicPtr 
{
private:
public:
    typedef T   value_type;
    typedef T * pointer;


    explicit	     SYS_AtomicPtr(T *value = 0)
			 : myValue(reinterpret_cast<ptrdiff_t>(value))
		     {
		     }

    /// Atomically assigns val to myValue, returning the prior value of
    /// myValue.
    inline T 	    *exchange(T *val);

    /// Atomically compares (myValue == expected), and if true, replaces
    /// myValue with desired. Returns prior myValue.
    inline T 	    *compare_swap(T *expected, T *desired);

    /// Atomically compares (ptr_ref == expected), and if true, replaces
    /// ptr_ref with desired and returns true. Else, returns false.
    static bool	     compare_exchange_strong(
			    T * volatile * ptr_addr,
			    T *expected, T *desired);

    /// Atomically obtain myValue
    operator	     T *() const;

    /// Uses T *() for atomic indirection
    T *	    	     operator->() const { return (T *)*this; }

    inline T *relaxedLoad() const
    {
        return reinterpret_cast<T *>(myValue.relaxedLoad());
    }
    inline void relaxedStore(T *val)
    {
        myValue.relaxedStore(reinterpret_cast<ptrdiff_t>(val));
    }

private:
    SYS_AtomicInt<ptrdiff_t>	myValue;

private:
    // Prohibited operators
    SYS_AtomicPtr(const SYS_AtomicPtr &);
    SYS_AtomicPtr &operator=(const SYS_AtomicPtr &);
};

/// Atomically assigns val to myValue, returning the prior value of
/// myValue.
template <typename T>
T *
SYS_AtomicPtr<T>::exchange(T *val)
{
    return reinterpret_cast<T *>(
	    myValue.exchange(reinterpret_cast<ptrdiff_t>(val)));
}

template <typename T>
T *
SYS_AtomicPtr<T>::compare_swap(T *expected, T *desired)
{
    return reinterpret_cast<T *>(
	    myValue.compare_swap(
		reinterpret_cast<ptrdiff_t>(expected),
		reinterpret_cast<ptrdiff_t>(desired)));
}

template <typename T>
bool
SYS_AtomicPtr<T>::compare_exchange_strong(
	T * volatile * ptr_addr, T *expected, T *desired)
{
    T *old = reinterpret_cast<T *>(
	SYS_AtomicImpl::compare_and_swap(
	    reinterpret_cast<volatile ptrdiff_t *>(ptr_addr),
					   reinterpret_cast<ptrdiff_t>(expected),
					   reinterpret_cast<ptrdiff_t>(desired)));

    return (old == expected);
}

template <typename T>
SYS_AtomicPtr<T>::operator T *() const
{
    return reinterpret_cast<T *>(myValue.load());
}


#endif
