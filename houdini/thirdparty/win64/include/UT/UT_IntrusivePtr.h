/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_IntrusivePtr.h (UT Library, C++)
 *
 * COMMENTS:	A wrapper around hboost::intrusive_ptr.
 *
 */

#ifndef __UT_INTRUSIVEPTR_H_INCLUDED__
#define __UT_INTRUSIVEPTR_H_INCLUDED__

#include <SYS/SYS_AtomicInt.h>
#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Types.h>
#include <memory>		// For std::default_delete
#include <hboost/intrusive_ptr.hpp>

/// @brief Wrapper around hboost::intrusive_ptr
///
/// An intrusive pointer assumes that the referenced object implements
/// reference counting on itself.  The reference counting is implemented using
/// the methods @c intrusive_ptr_add_ref() and @c intrusive_ptr_release().
///
/// For simple usages, simply derive from UT_IntrusiveRefCounter.
///
/// Here's a roll your own example: @code
///     class Foo {
///     public:
///         Foo() : myCount(0) {}
///        ~Foo() {}
///         void        incref()
///                     {
///                         myCount.add(1);
///                     }
///         void        decref()
///                     {
///                         if (myCount.add(-1) == 0)
///                             delete this;
///                     }
///         SYS_AtomicInt32     myCount;
///     };
///     static inline void intrusive_ptr_add_ref(Foo *o) { o->incref(); }
///     static inline void intrusive_ptr_release(Foo *o) { o->decref(); }
///
///     typedef UT_IntrusivePtr<Foo>   FooHandle;
/// @endcode
/// @see UT_SharedPtr
template<class T>
class UT_IntrusivePtr : public hboost::intrusive_ptr<T>
{
public:
    SYS_FORCE_INLINE
    UT_IntrusivePtr()
        : hboost::intrusive_ptr<T>(nullptr, true) {}

    // NOTE: This should really be explicit, but there are so many
    // places currently implicitly casting to UT_IntrusivePtr that
    // it could take quite a while to fix.
    SYS_FORCE_INLINE
    /*explicit*/ UT_IntrusivePtr(T *p)
        : hboost::intrusive_ptr<T>(p, true) {}

    SYS_FORCE_INLINE
    UT_IntrusivePtr(T *p, bool add_ref)
        : hboost::intrusive_ptr<T>(p, add_ref) {}

    SYS_FORCE_INLINE
    UT_IntrusivePtr(const UT_IntrusivePtr<T> &p)
        : hboost::intrusive_ptr<T>(p) {}

    template <typename Y>
    SYS_FORCE_INLINE
    UT_IntrusivePtr(const UT_IntrusivePtr<Y> &p)
        : hboost::intrusive_ptr<T>(p) {}

    SYS_FORCE_INLINE
    UT_IntrusivePtr(UT_IntrusivePtr<T> &&p)
        : hboost::intrusive_ptr<T>(std::move(p)) {}

    SYS_FORCE_INLINE
    UT_IntrusivePtr<T> &operator=(const UT_IntrusivePtr<T> &that)
    {
        hboost::intrusive_ptr<T>::operator=(that);
        return *this;
    }

    template <typename Y>
    SYS_FORCE_INLINE
    UT_IntrusivePtr<T> &operator=(const UT_IntrusivePtr<Y> &that)
    {
        hboost::intrusive_ptr<T>::operator=(that);
        return *this;
    }

    SYS_FORCE_INLINE
    UT_IntrusivePtr<T> &operator=(UT_IntrusivePtr<T> &&that)
    {
        hboost::intrusive_ptr<T>::operator=(std::move(that));
        return *this;
    }
};

/// Thread-safe policy for UT_IntrusiveRefCounter
struct UT_IntrusiveThreadSafeCounterPolicy
{
    typedef SYS_AtomicInt32 type;

    SYS_FORCE_INLINE
    static uint32   load(const type& counter)   { return counter.relaxedLoad();}
    SYS_FORCE_INLINE
    static void     increment(type& counter)    { (void) counter.add(1); }
    SYS_FORCE_INLINE
    static uint32   decrement(type& counter)    { return counter.add(-1); }
};

/// NOT Thread-safe policy for UT_IntrusiveRefCounter
struct UT_IntrusiveNonThreadSafeCounterPolicy
{
    typedef uint32 type;

    SYS_FORCE_INLINE
    static uint32   load(const type& counter)   { return counter; }
    SYS_FORCE_INLINE
    static void     increment(type& counter)    { ++counter; }
    SYS_FORCE_INLINE
    static uint32   decrement(type& counter)    { return (--counter); }
};

// Forward declarations for friends.
template <
    typename DerivedT,
    typename Deleter = std::default_delete<DerivedT>,
    typename CounterPolicyT = UT_IntrusiveThreadSafeCounterPolicy
>
class UT_IntrusiveRefCounter; 

template <typename DerivedT, typename Deleter, typename CounterPolicyT>
void intrusive_ptr_add_ref(
	const UT_IntrusiveRefCounter<DerivedT,Deleter,CounterPolicyT>* p);

template <typename DerivedT, typename Deleter, typename CounterPolicyT>
void intrusive_ptr_release(
	const UT_IntrusiveRefCounter<DerivedT,Deleter,CounterPolicyT>* p);

/// @brief A reference counter base class for use with UT_IntrusivePtr
///
/// This base class can be used with user-defined classes to add support for
/// @c UT_IntrusivePtr. The class contains a reference counter defined by the
/// @c CounterPolicyT.  Upon releasing the last @c UT_IntrusivePtr referencing
/// the object derived from the @c UT_IntrusiveRefCounter class, operator
/// @c delete is automatically called on the pointer to the object.
/// 
/// The other template parameter, @c DerivedT, is the user's class that
/// derives from @c UT_IntrusiveRefCounter.
///
/// Example: @code
///	class MyClass : public UT_IntrusiveRefCounter<MyClass>
///	    { ... };
///	typedef UT_IntrusivePtr<MyClass> MyClassPtr;
/// @endcode
template <
    typename DerivedT,
    typename Deleter,
    typename CounterPolicyT
>
class UT_IntrusiveRefCounter
{
public:
    /// Default constructor: Sets counter to 0.
    SYS_FORCE_INLINE
    UT_IntrusiveRefCounter()
	: myRefCount(0)
    {
    }
    /// Copy constructor: Sets counter to 0.
    SYS_FORCE_INLINE
    UT_IntrusiveRefCounter(const UT_IntrusiveRefCounter&)
	: myRefCount(0)
    {
    }
    /// Assignment operator: Does not modify counter.
    UT_IntrusiveRefCounter& operator=(const UT_IntrusiveRefCounter&)
    {
	return *this;
    }
    /// Return current counter
    SYS_FORCE_INLINE
    uint32 use_count() const
    {
	return CounterPolicyT::load(myRefCount);
    }

protected:
    /// Destructor: Only derived classes can destruct this.
    SYS_FORCE_INLINE
    ~UT_IntrusiveRefCounter()
    {
    }

private:
    mutable typename CounterPolicyT::type    myRefCount;

    friend void intrusive_ptr_add_ref<DerivedT,Deleter,CounterPolicyT>(
	    const UT_IntrusiveRefCounter<DerivedT,Deleter,CounterPolicyT>* p);
    friend void intrusive_ptr_release<DerivedT,Deleter,CounterPolicyT>(
	    const UT_IntrusiveRefCounter<DerivedT,Deleter,CounterPolicyT>* p);
};

template <typename DerivedT, typename Deleter, typename CounterPolicyT>
SYS_FORCE_INLINE void
intrusive_ptr_add_ref(
	const UT_IntrusiveRefCounter<DerivedT,Deleter,CounterPolicyT>* p)
{
    CounterPolicyT::increment(p->myRefCount);
}
template <typename DerivedT, typename Deleter, typename CounterPolicyT>
SYS_FORCE_INLINE void
intrusive_ptr_release(const UT_IntrusiveRefCounter<DerivedT,Deleter,CounterPolicyT>* p)
{
    if (CounterPolicyT::decrement(p->myRefCount) == 0)
    {
	Deleter	d;
	d(SYSconst_cast(static_cast<const DerivedT *>(p)));
    }
}

// For UT::ArraySet.
namespace UT
{
template <typename T>
struct DefaultClearer;

template <typename T>
struct DefaultClearer<UT_IntrusivePtr<T>>
{
    static void clear(UT_IntrusivePtr<T> &v)
		    { v = UT_IntrusivePtr<T>(); }
    static bool isClear(const UT_IntrusivePtr<T> &v)
		    { return v.get() == nullptr; }
    static void clearConstruct(UT_IntrusivePtr<T> *p)
		    { new ((void *)p) UT_IntrusivePtr<T>(nullptr); }
    static const bool clearNeedsDestruction = false;
};
} // namespace UT

#endif // __UT_INTRUSIVEPTR_H_INCLUDED__
