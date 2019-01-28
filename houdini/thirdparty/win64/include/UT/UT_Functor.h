/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        UT_Functor.h
 *
 * COMMENTS:
 *	This functor implementation is adapted from "Modern C++ Design"
 *	by Andrei Alexandrescu.  You needn't be concerned with how it
 *	works if you just want to use it.
 *
 *	Note that these functors use value semantics.  So, you can safely
 *	assign from one functor to another and it is safe to pass
 *	functors (without a reference or pointer) to a function.
 *
 *	Using functors with functions:
 *	    int function();
 *	    UT_Functor<int> functor1(function);
 *	    result = functor1();
 *
 *	Using functors with methods:
 * 	    class TestClass has method getValue() that returns an int.
 * 	    TestClass object;
 * 	    UT_Functor<int> functor2(&object, &TestClass::getValue);
 * 	    result = functor2();
 *
 *	Using functors with other functors:
 *	    UT_Functor<int> functor3;
 *	    functor3 = functor1;
 *	    result = functor3();
 *
 *	Using functors with arguments:
 *	    int function2(int);
 *	    UT_Functor1<int, int> functor4(function);
 *	    result = functor4(5);
 *
 *	Using functors as callbacks;
 *	    UT_Functor<int, int> myCallback;
 *	    void setCallback(UT_Functor<int, int> functor)
 *	    	{ myCallback = functor; }
 *	    void executeCallback()
 *	    {
 *	    	if (myCallback.isset())
 *	    	    result = myCallback(myValue);
 *	    }
 *
 *	Encapsulating data in functors by binding values to arguments:
 *	    UT_Functor<int, int> functor5(function2);
 *	    UT_Functor<int>	 functor6;
 *	    functor6 = UT_BindFirst(functor5, 7);
 *	    result = functor6();
 */

#ifndef __UT_Functor_h__
#define __UT_Functor_h__

#include "UT_Assert.h"
#include <SYS/SYS_Types.h>

// These are pure virtual interfaces for the classes that do the real work
// for the UT_Functor wrapper class.  There are different classes for
// functors that take different number of parameters.
template <typename R>
class UT_FunctorImpl
{
public:
    typedef R ReturnType;
    virtual ReturnType operator()() const = 0;

    // Return a copy of this functor implementation.
    virtual UT_FunctorImpl<ReturnType> *clone() const = 0;

    // We need to make the destructor virtual in this base class, even
    // though it doesn't need to do anything here.
    virtual ~UT_FunctorImpl() {}
};

// ---------------------------------------------------------------------------

template <typename R, typename P1>
class UT_FunctorImpl1
{
public:
    typedef R ReturnType;
    typedef P1 Parm1Type;
    virtual ReturnType operator()(Parm1Type parm1) const = 0;
    virtual UT_FunctorImpl1<ReturnType, Parm1Type> *clone() const = 0;
    virtual int64 getMemoryUsage(bool inclusive) const = 0;
    virtual ~UT_FunctorImpl1() {}
};

// ---------------------------------------------------------------------------

template <typename R, typename P1, typename P2>
class UT_FunctorImpl2
{
public:
    typedef R ReturnType;
    typedef P1 Parm1Type;
    typedef P2 Parm2Type;
    virtual ReturnType operator()(Parm1Type parm1, Parm2Type parm2) const = 0;
    virtual UT_FunctorImpl2<ReturnType, Parm1Type, Parm2Type> *clone() const =0;
    virtual ~UT_FunctorImpl2() {}
};

// ===========================================================================

// UT_FunctorHandlers are UT_FunctorImpl's that can automatically handle
// function pointers and other functors, thanks to the nature of template
// instantiation.
template <typename ParentFunctor, typename FunctorType>
class UT_FunctorHandler
    : public UT_FunctorImpl<typename ParentFunctor::ReturnType>
{
public:
    typedef typename ParentFunctor::ReturnType ReturnType;

    UT_FunctorHandler(const FunctorType &functor)
	: myFunctor(functor)
    {}

    // Our default copy constructor will call the copy constructor of our
    // member variables.  So, if our member variable is a UT_Functor, it
    // will properly duplicate its UT_FunctorImpl.
    virtual UT_FunctorImpl<ReturnType> *clone() const
    { return new UT_FunctorHandler<ParentFunctor, FunctorType>(*this); }

    virtual ReturnType operator()() const
    { return myFunctor(); }

private:
    FunctorType	 myFunctor;
};

// ---------------------------------------------------------------------------

template <typename ParentFunctor, typename FunctorType>
class UT_FunctorHandler1
    : public UT_FunctorImpl1<typename ParentFunctor::ReturnType,
			     typename ParentFunctor::Parm1Type>
{
public:
    typedef typename ParentFunctor::ReturnType ReturnType;
    typedef typename ParentFunctor::Parm1Type Parm1Type;
    UT_FunctorHandler1(const FunctorType &functor)
	: myFunctor(functor) {}

    virtual UT_FunctorImpl1<ReturnType, Parm1Type> *clone() const
    { return new UT_FunctorHandler1<ParentFunctor, FunctorType>(*this); }

    virtual ReturnType operator()(Parm1Type parm1) const
    { return myFunctor(parm1); }

    virtual int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        return mem;
    }

private:
    FunctorType	 myFunctor;
};

// ---------------------------------------------------------------------------

template <typename ParentFunctor, typename FunctorType>
class UT_FunctorHandler2
    : public UT_FunctorImpl2<typename ParentFunctor::ReturnType,
			     typename ParentFunctor::Parm1Type,
			     typename ParentFunctor::Parm2Type>
{
public:
    typedef typename ParentFunctor::ReturnType ReturnType;
    typedef typename ParentFunctor::Parm1Type Parm1Type;
    typedef typename ParentFunctor::Parm2Type Parm2Type;
    UT_FunctorHandler2(const FunctorType &functor)
	: myFunctor(functor) {}

    virtual UT_FunctorImpl2<ReturnType, Parm1Type, Parm2Type> *clone() const
    { return new UT_FunctorHandler2<ParentFunctor, FunctorType>(*this); }

    virtual ReturnType operator()(Parm1Type parm1, Parm2Type parm2) const
    { return myFunctor(parm1, parm2); }

private:
    FunctorType	 myFunctor;
};

// ===========================================================================

// These classes handle member function functors.
template <typename ParentFunctor, typename PointerToObj,
	  typename PointerToMemFn>
class UT_MemFunHandler
    : public UT_FunctorImpl<typename ParentFunctor::ReturnType>
{
public:
    typedef typename ParentFunctor::ReturnType ReturnType;

    UT_MemFunHandler(const PointerToObj &pointer_to_obj,
		     const PointerToMemFn pointer_to_mem_fn)
	: myPointerToObj(pointer_to_obj), myPointerToMemFn(pointer_to_mem_fn)
    {}

    virtual UT_FunctorImpl<ReturnType> *clone() const
    {  return new UT_MemFunHandler<ParentFunctor, PointerToObj,
				   PointerToMemFn>(*this); }

    virtual ReturnType operator()() const
    { return ((*myPointerToObj).*myPointerToMemFn)(); }

private:
    PointerToObj	 myPointerToObj;
    PointerToMemFn	 myPointerToMemFn;
};

// ---------------------------------------------------------------------------

template <typename ParentFunctor, typename PointerToObj,
	  typename PointerToMemFn>
class UT_MemFunHandler1
    : public UT_FunctorImpl1<typename ParentFunctor::ReturnType,
			     typename ParentFunctor::Parm1Type>
{
public:
    typedef typename ParentFunctor::ReturnType ReturnType;
    typedef typename ParentFunctor::Parm1Type Parm1Type;

    UT_MemFunHandler1(const PointerToObj &pointer_to_obj,
		      const PointerToMemFn pointer_to_mem_fn)
	: myPointerToObj(pointer_to_obj), myPointerToMemFn(pointer_to_mem_fn)
    {}

    virtual UT_FunctorImpl1<ReturnType, Parm1Type> *clone() const
    {  return new UT_MemFunHandler1<ParentFunctor, PointerToObj,
				    PointerToMemFn>(*this); }

    virtual ReturnType operator()(Parm1Type parm1) const
    { return ((*myPointerToObj).*myPointerToMemFn)(parm1); }

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

template <typename ParentFunctor, typename PointerToObj,
	  typename PointerToMemFn>
class UT_MemFunHandler2
    : public UT_FunctorImpl2<typename ParentFunctor::ReturnType,
			     typename ParentFunctor::Parm1Type,
			     typename ParentFunctor::Parm2Type>
{
public:
    typedef typename ParentFunctor::ReturnType ReturnType;
    typedef typename ParentFunctor::Parm1Type Parm1Type;
    typedef typename ParentFunctor::Parm2Type Parm2Type;

    UT_MemFunHandler2(const PointerToObj &pointer_to_obj,
		      const PointerToMemFn pointer_to_mem_fn)
	: myPointerToObj(pointer_to_obj), myPointerToMemFn(pointer_to_mem_fn)
    {}

    virtual UT_FunctorImpl2<ReturnType, Parm1Type, Parm2Type> *clone() const
    {  return new UT_MemFunHandler2<ParentFunctor, PointerToObj,
				    PointerToMemFn>(*this); }

    virtual ReturnType operator()(Parm1Type parm1, Parm2Type parm2) const
    { return ((*myPointerToObj).*myPointerToMemFn)(parm1, parm2); }

private:
    PointerToObj	 myPointerToObj;
    PointerToMemFn	 myPointerToMemFn;
};

// ===========================================================================

// These classes are the ones you actually use directly.
template <typename R>
class UT_Functor
{
public:
    typedef R ReturnType;

    UT_Functor() {}

    // The functor takes ownership of the functor implementation.
    explicit UT_Functor(UT_FunctorImpl<ReturnType> *functor_implementation)
    : myFunctorImpl(functor_implementation)
    {}

    // This constructor lets us avoid using UT_FunctorHandler directly.
    // This lets us do nice things like declare functors that take
    // function pointers.  The method template type does not need to
    // be explicitly specified.  When the constructor sees a UT_Functor
    // created from a function pointer, it has no choice but to try the
    // templated constructor.
    template <typename FunctorType>
    UT_Functor(FunctorType functor)
    : myFunctorImpl(new UT_FunctorHandler<UT_Functor, FunctorType>(functor))
    {}

    // Like the contructor that creates a UT_FunctorHandler, you don't
    // need to explicitly specify the template parameters for this
    // constuctor.  The complier will figure it out.
    template <typename PointerToObj, typename PointerToMemFn>
    UT_Functor(const PointerToObj &pointer, PointerToMemFn method)
    : myFunctorImpl(new UT_MemFunHandler<UT_Functor, PointerToObj,
				 PointerToMemFn>(pointer, method))
    {}

    // The functor always owns the implementation.
    ~UT_Functor()
    {
	delete myFunctorImpl.myFunctorImpl;
	myFunctorImpl.myFunctorImpl = 0;
    }

    // Clone the other functor's implementation on assignment.
    UT_Functor &operator=(const UT_Functor &functor)
    {
	if (&functor != this)
	{
	    delete myFunctorImpl.myFunctorImpl;
	    myFunctorImpl.myFunctorImpl = functor.myFunctorImpl.myFunctorImpl
			? functor.myFunctorImpl.myFunctorImpl->clone() : 0;
	}
	return *this;
    }

    // Because we don't know what to return if the functor implementation
    // hasn't been set, it's an error to evaluate the functor without
    // an implementation.  void return types are specialized in
    // UT_Functor.C, though.
    ReturnType operator()() const
    {
	UT_ASSERT(myFunctorImpl.myFunctorImpl);
	return (*myFunctorImpl.myFunctorImpl)();
    }

    // This method is useful to tell if a callback has ever been set.
    bool isSet() const
    { return myFunctorImpl.myFunctorImpl != 0; }

private:
    // This class is to work around a bug in MSVC where you can't
    // override the copy constructor because of the templated constructor.
    // This workaround is copied from the Loki port to MSVC.
    class Helper
    {
    public:
	Helper() : myFunctorImpl(0) {}

	// Clone the other functor's implementation in the copy constructor.
	Helper(const Helper &helper)
	{ myFunctorImpl = helper.myFunctorImpl
			? helper.myFunctorImpl->clone() : 0; }

	explicit Helper(UT_FunctorImpl<ReturnType> *functor_implementation)
	    : myFunctorImpl(functor_implementation) {}

	template<typename U>
	explicit Helper(U *ptr) : myFunctorImpl(ptr) {}

	UT_FunctorImpl<ReturnType> *myFunctorImpl;
    };

    Helper myFunctorImpl;
};

// ---------------------------------------------------------------------------

template <typename R, typename P1>
class UT_Functor1
{
public:
    typedef R ReturnType;
    typedef P1 Parm1Type;

    UT_Functor1() {}

    explicit UT_Functor1(
		UT_FunctorImpl1<ReturnType, Parm1Type> *functor_implementation)
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

    ReturnType operator()(Parm1Type parm1) const
    {
	UT_ASSERT(myFunctorImpl.myFunctorImpl);
	return (*myFunctorImpl.myFunctorImpl)(parm1);
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
		UT_FunctorImpl1<ReturnType, Parm1Type> *functor_implementation)
	    : myFunctorImpl(functor_implementation) {}

	template<typename U>
	explicit Helper(U *ptr) : myFunctorImpl(ptr) {}

	UT_FunctorImpl1<ReturnType, Parm1Type> *myFunctorImpl;
    };

    Helper myFunctorImpl;
};

// ---------------------------------------------------------------------------

template <typename R, typename P1, typename P2>
class UT_Functor2
{
public:
    typedef R ReturnType;
    typedef P1 Parm1Type;
    typedef P2 Parm2Type;

    UT_Functor2() {}

    explicit UT_Functor2(
	    UT_FunctorImpl2<ReturnType, Parm1Type, Parm2Type> *functor_impl)
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

    ReturnType operator()(Parm1Type parm1, Parm2Type parm2) const
    {
	UT_ASSERT(myFunctorImpl.myFunctorImpl);
	return (*myFunctorImpl.myFunctorImpl)(parm1, parm2);
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
		UT_FunctorImpl2<ReturnType, Parm1Type, Parm2Type> *functor_impl)
	    : myFunctorImpl(functor_impl) {}

	template<typename U>
	explicit Helper(U *ptr) : myFunctorImpl(ptr) {}

	UT_FunctorImpl2<ReturnType, Parm1Type, Parm2Type> *myFunctorImpl;
    };

    Helper myFunctorImpl;
};

// ===========================================================================

// This is a functor implementation class that stores an argument that
// passes that argument to a different functor.
template <typename Incoming>
class UT_BindFirstImpl : public UT_FunctorImpl<typename Incoming::ReturnType>
{
public:
    typedef typename Incoming::ReturnType ReturnType;
    typedef typename Incoming::Parm1Type BoundType;

    UT_BindFirstImpl(const Incoming &functor, BoundType bound_argument)
    : myFunctor(functor), myBoundArgument(bound_argument)
    {}

    UT_FunctorImpl<ReturnType> *clone() const
    { return new UT_BindFirstImpl(*this); }

    ReturnType operator()() const
    { return myFunctor(myBoundArgument); }

private:
    Incoming myFunctor;
    BoundType myBoundArgument;
};

// This function is used to convert one functor to another by binding
// the first argument to a fixed value.
template <typename Functor>
UT_Functor<typename Functor::ReturnType>
UT_BindFirst(const Functor &functor, typename Functor::Parm1Type bound_argument)
{
    // For some reason, g++ wants to call the wrong UT_Functor constructor
    // if we don't explicitly cast to the UT_FunctorImpl base class pointer.
    return UT_Functor<typename Functor::ReturnType>(
		(UT_FunctorImpl<typename Functor::ReturnType> *)
		new UT_BindFirstImpl<Functor>(functor, bound_argument));
}

// ---------------------------------------------------------------------------

template <typename Incoming>
class UT_BindFirstImpl1 : public UT_FunctorImpl1<typename Incoming::ReturnType,
						 typename Incoming::Parm2Type>
{
public:
    typedef typename Incoming::ReturnType ReturnType;
    typedef typename Incoming::Parm2Type Parm1Type;
    typedef typename Incoming::Parm1Type BoundType;

    UT_BindFirstImpl1(const Incoming &functor, BoundType bound_argument)
    : myFunctor(functor), myBoundArgument(bound_argument)
    {}

    UT_FunctorImpl1<ReturnType, Parm1Type> *clone() const
    { return new UT_BindFirstImpl1(*this); }

    ReturnType operator()(Parm1Type parm1) const
    { return myFunctor(myBoundArgument, parm1); }

    virtual int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        return mem;
    }

private:
    Incoming myFunctor;
    BoundType myBoundArgument;
};

template <typename Functor>
UT_Functor1<typename Functor::ReturnType, typename Functor::Parm2Type>
UT_BindFirst1(const Functor &functor,
	      typename Functor::Parm1Type bound_argument)
{
    typedef typename Functor::ReturnType ReturnType;
    typedef typename Functor::Parm2Type Parm1Type;

    // For some reason, g++ wants to call the wrong UT_Functor constructor
    // if we don't explicitly cast to the UT_FunctorImpl base class pointer.
    return UT_Functor1<ReturnType, Parm1Type>(
		(UT_FunctorImpl1<ReturnType, Parm1Type> *)
		new UT_BindFirstImpl1<Functor>(functor, bound_argument));
}

// We need to include UT_Functor.C.  Otherwise, it
// doesn't find the template specializations.
#if defined(WIN32) || defined(LINUX) || defined(MBSD)
    #include "UT_Functor.C"
#endif

#endif
