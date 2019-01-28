/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        UT_Singleton.h (UT Library, C++)
 *
 * COMMENTS:    Classes for on-demand construction of singletons or
 *              pointer member variables.
 */

#ifndef __UT_Singleton__
#define __UT_Singleton__

#include "UT_NonCopyable.h"
#include "UT_LockUtil.h"
#include <SYS/SYS_MemoryOrder.h>

class UT_Lock;

namespace {

/// This is a helper function for UT_Singleton and UT_SingletonWithLock.
/// Do NOT use it directly unless you know what you're doing.
template <typename T, class LOCK>
T &
UTacquireSingleton(T *volatile &p, LOCK &lock)
{
    // Read into temp to avoid re-reading p with "return *p;",
    // since p is only written once.
    T *temp = p;
    if (!temp)
    {
	typename LOCK::Scope autolock(lock);

        // Read into temp to avoid re-reading p with "return *p;",
        // since p is only written once.
        temp = p;
        if (!temp)
        {
            temp = new T();

            // Ensure that the data that temp refers to is written
            // out to main memory before setting p.
            // Without this store fence, the CPU may re-order the
            // storing of the data in the cache, and another
            // thread could receive p as non-NULL before
            // the data pointed-to by p is initialized
            // in main memory, so the thread could see garbage data.
            SYSstoreFence();

            p = temp;
        }
    }

    // NOTE: This load fence is only necessary for preventing speculative
    //       reads of any side effects of new T() producing values from
    //       before the allocation.
    //
    //       Even if there aren't explicit side effects, there's
    //       the side effect of a new memory block being allocated
    //       on the heap, so just in case, it's probably better to
    //       leave this here.  For (a poor) example, if the heap tracks
    //       how much memory it uses, and if the CPU does a following read
    //       of that count speculatively *before* the read of p, the value
    //       read could be the value from *before* p was allocated.
    SYSloadFence();

    return *temp;
}

/// This is a helper function for UT_Singleton and UT_SingletonWithLock.
/// This is the same as UTacquireSingleton(T *volatile &p, LOCK &lock),
/// except that it passes s, of type S, to the T constructor.
/// Do NOT use it directly unless you know what you're doing.
template <typename T, class LOCK, typename S>
T &
UTacquireSingleton(T *volatile &p, LOCK &lock, S s)
{
    // Read into temp to avoid re-reading p with "return *p;",
    // since p is only written once.
    T *temp = p;
    if (!temp)
    {
	typename LOCK::Scope autolock(lock);

        // Read into temp to avoid re-reading p with "return *p;",
        // since p is only written once.
        temp = p;
        if (!temp)
        {
            temp = new T(s);

            // Ensure that the data that temp refers to is written
            // out to main memory before setting p.
            // Without this store fence, the CPU may re-order the
            // storing of the data in the cache, and another
            // thread could receive p as non-NULL before
            // the data pointed-to by p is initialized
            // in main memory, so the thread could see garbage data.
            SYSstoreFence();

            p = temp;
        }
    }

    // NOTE: This load fence is only necessary for preventing speculative
    //       reads of any side effects of allocfunc(param) producing values
    //       from before the allocation.
    //
    //       Even if there aren't explicit side effects, there's
    //       the side effect of a new memory block being allocated
    //       on the heap, so just in case, it's probably better to
    //       leave this here.  For (a poor) example, if the heap tracks
    //       how much memory it uses, and if the CPU does a following read
    //       of that count speculatively *before* the read of p, the value
    //       read could be the value from *before* p was allocated.
    SYSloadFence();

    return *temp;
}

} // end of anonymous namespace

/// This is the same as UT_SingletonWithLock, except referencing an
/// existing lock in get(), instead of holding its own.  This can be useful
/// for avoiding having many locks as member variables when using
/// UT_Singleton for pointer member variables that are allocated
/// on-demand, or if another lock would need to be acquired in order
/// to run new T().
template <typename T, bool DESTRUCTONEXIT=false>
class UT_Singleton : public UT_NonCopyable
{
public:
    UT_Singleton() : myPointer(0) {}
    ~UT_Singleton()
    {
        if (DESTRUCTONEXIT)
            delete myPointer;
    }

    template <class LOCK>
    T &get(LOCK &lock)
    {
        return UTacquireSingleton(myPointer, lock);
    }

    template <class LOCK, typename S>
    T &get(LOCK &lock, S s)
    {
        return UTacquireSingleton(myPointer, lock, s);
    }

    /// NOTE: Even though unsafeSet locks, it is still unsafe, because
    ///       other threads may have called get(), gotten a valid pointer,
    ///       and be using it when unsafeSet destructs it.
    template <class LOCK>
    void unsafeSet(LOCK &lock, T *newp)
    {
        T *oldp = myPointer;
        if (oldp != newp)
        {
	    typename LOCK::Scope autolock(lock);

            oldp = myPointer;
            if (oldp != newp)
            {
                // Make sure to save out the data pointed to by newp to main
                // memory before setting myPointer to it.
                SYSstoreFence();

                myPointer = newp;

                if (oldp)
                    delete oldp;
            }
        }
    }

private:
    T *volatile myPointer;
};

///
/// This is a singleton constructed on-demand with a double-checked lock.
/// The lock is only locked if get() is called when myPointer is 0.
/// 
/// This is normally simpler to use than a UT_DoubleLock, and this should
/// be used for all on-demand singleton construction, as well as for
/// pointer member variables that are allocated on-demand, if applicable.
/// If it is preferable in such a case to use an existing lock instead of
/// using the lock member in this class, please use UT_Singleton.
///
/// To use this class in the simplest case, where OBJECT is to be
/// default-constructed:
///
/// static UT_SingletonWithLock<OBJECT> theSingleton;
///
/// void useSingleton()
/// {
///     OBJECT &obj = theSingleton.get();
///     ...
/// }
///
/// When special construction or destruction are necessary, subclass
/// OBJECT:
///
/// class OBJECTWrapper : public OBJECT
/// {
/// public:
///     OBJECTWrapper()
///         : OBJECT(12345)
///     { doSpecialInit(67890); }
///     ~OBJECTWrapper()
///     { doBeforeDestruction(7654); }
/// };
///
/// static UT_SingletonWithLock<OBJECTWrapper> theSingleton;
///
/// void useSingleton()
/// {
///     OBJECT &obj = theSingleton.get();
///     ...
/// }
///
/// NOTE: Do not roll your own on-demand singleton construction!
///       This class should be threadsafe, and it's very easy to
///       miss a store memory fence without noticing for a very
///       long time.
///
template <typename T, bool DESTRUCTONEXIT=false, class LOCK=UT_Lock>
class UT_SingletonWithLock : public UT_Singleton<T, DESTRUCTONEXIT>
{
public:
    typedef UT_Singleton<T, DESTRUCTONEXIT> Base;

    T &get()
    {
        return Base::get(myLock);
    }

    template <typename S>
    T &get(S s)
    {
        return Base::get(myLock, s);
    }

    /// NOTE: Even though unsafeSet locks, it is still unsafe, because
    ///       other threads may have called get(), gotten a valid pointer,
    ///       and be using it when unsafeSet destructs it.
    void unsafeSet(T *newp)
    {
        Base::unsafeSet(myLock, newp);
    }

    T &operator*()  { return  get(); }
    T *operator->() { return &get(); }
private:
    LOCK myLock;
};




#endif

