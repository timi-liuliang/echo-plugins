/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        UT_Functor.C
 *
 *	This file contains template specializations for UT_Functor.  They
 *	cannot go into UT_Functor.h because compilers that don't use the
 *	Borland template compilation model will fail because of multiply
 *	defined symbols.
 */

#ifndef __UT_Functor_C__
#define __UT_Functor_C__

#include "UT_Functor.h"

// It's okay for a functor with a void return type to not do anything if
// it was never initialized.  This can happen if you just declare a
// functor with the default constructor but never initialize it.
template <>
inline void
UT_Functor<void>::operator()() const
{
    if (myFunctorImpl.myFunctorImpl)
	(*myFunctorImpl.myFunctorImpl)();
}

// ---------------------------------------------------------------------------

// Irix doesn't let you use "return <expression>" when <expression>'s type is
// void.  We can't partially specialize member functions, so instead we have
// to partially specialize the classes to when a void return type is used.
template <typename PointerToObj, typename PointerToMemFn>
class UT_MemFunHandler<UT_Functor<void>, PointerToObj, PointerToMemFn>
    : public UT_FunctorImpl<void>
{
public:
    typedef void ReturnType;

    UT_MemFunHandler(const PointerToObj &pointer_to_obj,
		     const PointerToMemFn pointer_to_mem_fn)
	: myPointerToObj(pointer_to_obj), myPointerToMemFn(pointer_to_mem_fn)
    {}

    virtual UT_FunctorImpl<void> *clone() const
    {  return new UT_MemFunHandler<UT_Functor<void>, PointerToObj,
				   PointerToMemFn>(*this); }

    // There is no "return" here, because Irix doesn't support it.
    virtual void operator()() const
    { ((*myPointerToObj).*myPointerToMemFn)(); }

private:
    PointerToObj	 myPointerToObj;
    PointerToMemFn	 myPointerToMemFn;
};

// ---------------------------------------------------------------------------

template <typename P1, typename PointerToObj, typename PointerToMemFn>
class UT_MemFunHandler1<UT_Functor1<void, P1>, PointerToObj, PointerToMemFn>
    : public UT_FunctorImpl1<void, P1>
{
public:
    typedef void ReturnType;
    typedef P1 Parm1Type;

    UT_MemFunHandler1(const PointerToObj &pointer_to_obj,
		      const PointerToMemFn pointer_to_mem_fn)
	: myPointerToObj(pointer_to_obj), myPointerToMemFn(pointer_to_mem_fn)
    {}

    virtual UT_FunctorImpl1<void, P1> *clone() const
    {  return new UT_MemFunHandler1<UT_Functor1<void, P1>, PointerToObj,
				    PointerToMemFn>(*this); }

    // There is no "return" here, because Irix doesn't support it.
    virtual void operator()(Parm1Type parm1) const
    { ((*myPointerToObj).*myPointerToMemFn)(parm1); }

    virtual int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        return mem;
    }

private:
    PointerToObj	 myPointerToObj;
    PointerToMemFn	 myPointerToMemFn;
};

// ---------------------------------------------------------------------------

template <typename P1, typename P2,
	  typename PointerToObj, typename PointerToMemFn>
class UT_MemFunHandler2<UT_Functor2<void, P1, P2>, PointerToObj, PointerToMemFn>
    : public UT_FunctorImpl2<void, P1, P2>
{
public:
    typedef void ReturnType;
    typedef P1 Parm1Type;
    typedef P2 Parm2Type;

    UT_MemFunHandler2(const PointerToObj &pointer_to_obj,
		      const PointerToMemFn pointer_to_mem_fn)
	: myPointerToObj(pointer_to_obj), myPointerToMemFn(pointer_to_mem_fn)
    {}

    virtual UT_FunctorImpl2<void, Parm1Type, Parm2Type> *clone() const
    {  return new UT_MemFunHandler2<UT_Functor2<void, P1, P2>, PointerToObj,
				    PointerToMemFn>(*this); }

    // There is no "return" here, because Irix doesn't support it.
    virtual void operator()(Parm1Type parm1, Parm2Type parm2) const
    { ((*myPointerToObj).*myPointerToMemFn)(parm1, parm2); }

private:
    PointerToObj	 myPointerToObj;
    PointerToMemFn	 myPointerToMemFn;
};

template <typename P1>
class UT_Functor1<void, P1>
{
public:
    typedef void ReturnType;
    typedef P1 Parm1Type;

    UT_Functor1() {}

    explicit UT_Functor1(
		UT_FunctorImpl1<void, Parm1Type> *functor_implementation)
    : myFunctorImpl(functor_implementation)
    {}

    template <typename FunctorType>
    UT_Functor1(FunctorType functor)
    : myFunctorImpl(new UT_FunctorHandler1<UT_Functor1, FunctorType>(functor))
    {}

    template <typename PointerToObj, typename PointerToMemFn>
    UT_Functor1(const PointerToObj &pointer, PointerToMemFn method)
    : myFunctorImpl(new UT_MemFunHandler1<UT_Functor1, PointerToObj,
					  PointerToMemFn>(pointer, method))
    {}

    ~UT_Functor1()
    {
	delete myFunctorImpl.myFunctorImpl;
	myFunctorImpl.myFunctorImpl = 0;
    }

    UT_Functor1 &operator=(const UT_Functor1 &functor)
    {
	if (&functor != this)
	{
	    delete myFunctorImpl.myFunctorImpl;
	    myFunctorImpl.myFunctorImpl = functor.myFunctorImpl.myFunctorImpl
			? functor.myFunctorImpl.myFunctorImpl->clone() : 0;
	}
	return *this;
    }

    // There is no "return" here, because Irix doesn't support it.
    void operator()(Parm1Type parm1) const
    {
	UT_ASSERT(myFunctorImpl.myFunctorImpl);
	(*myFunctorImpl.myFunctorImpl)(parm1);
    }

    bool isSet() const
    { return myFunctorImpl.myFunctorImpl != 0; }

    int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        if (myFunctorImpl.myFunctorImpl)
            mem += myFunctorImpl.myFunctorImpl->getMemoryUsage(true);
        return mem;
    }

private:
    class Helper
    {
    public:
	Helper() : myFunctorImpl(0) {}

	// Clone the other functor's implementation in the copy constructor.
	Helper(const Helper &helper)
	{ myFunctorImpl = helper.myFunctorImpl
			? helper.myFunctorImpl->clone() : 0; }

	explicit Helper(
		UT_FunctorImpl1<void, Parm1Type> *functor_implementation)
	    : myFunctorImpl(functor_implementation) {}

	template<typename U>
	explicit Helper(U *ptr) : myFunctorImpl(ptr) {}

	UT_FunctorImpl1<void, Parm1Type> *myFunctorImpl;
    };

    Helper myFunctorImpl;
};

// ---------------------------------------------------------------------------

template <typename P1, typename P2>
class UT_Functor2<void, P1, P2>
{
public:
    typedef void ReturnType;
    typedef P1 Parm1Type;
    typedef P2 Parm2Type;

    UT_Functor2() {}

    explicit UT_Functor2(
	    UT_FunctorImpl2<void, Parm1Type, Parm2Type> *functor_impl)
    : myFunctorImpl(functor_impl)
    {}

    template <typename FunctorType>
    UT_Functor2(FunctorType functor)
    : myFunctorImpl(new UT_FunctorHandler2<UT_Functor2, FunctorType>(functor))
    {}

    template <typename PointerToObj, typename PointerToMemFn>
    UT_Functor2(const PointerToObj &pointer, PointerToMemFn method)
    : myFunctorImpl(new UT_MemFunHandler2<UT_Functor2, PointerToObj,
					  PointerToMemFn>(pointer, method))
    {}

    ~UT_Functor2()
    {
	delete myFunctorImpl.myFunctorImpl;
	myFunctorImpl.myFunctorImpl = 0;
    }

    UT_Functor2 &operator=(const UT_Functor2 &functor)
    {
	if (&functor != this)
	{
	    delete myFunctorImpl.myFunctorImpl;
	    myFunctorImpl.myFunctorImpl = functor.myFunctorImpl.myFunctorImpl
			? functor.myFunctorImpl.myFunctorImpl->clone() : 0;
	}
	return *this;
    }

    // There is no "return" here, because Irix doesn't support it.
    void operator()(Parm1Type parm1, Parm2Type parm2) const
    {
	UT_ASSERT(myFunctorImpl.myFunctorImpl);
	(*myFunctorImpl.myFunctorImpl)(parm1, parm2);
    }

    bool isSet() const
    { return myFunctorImpl.myFunctorImpl != 0; }

private:
    class Helper
    {
    public:
	Helper() : myFunctorImpl(0) {}

	// Clone the other functor's implementation in the copy constructor.
	Helper(const Helper &helper)
	{ myFunctorImpl = helper.myFunctorImpl
			? helper.myFunctorImpl->clone() : 0; }

	explicit Helper(
		UT_FunctorImpl2<void, Parm1Type, Parm2Type> *functor_impl)
	    : myFunctorImpl(functor_impl) {}

	template<typename U>
	explicit Helper(U *ptr) : myFunctorImpl(ptr) {}

	UT_FunctorImpl2<void, Parm1Type, Parm2Type> *myFunctorImpl;
    };

    Helper myFunctorImpl;
};

#endif
