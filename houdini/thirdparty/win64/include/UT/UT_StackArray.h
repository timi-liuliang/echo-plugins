/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_StackArray.h ( UT Library, C++ )
 *
 * COMMENTS:	A class to allocate small, dynamic arrays on the stack from
 *		a static memory pool
 */

#ifndef __UT_StackArray__
#define __UT_StackArray__

#include "UT_API.h"
#include "UT_ValArray.h"
#include "UT_Array.h"
#include "UT_IntArray.h"
#include "UT_FloatArray.h"
#include "UT_ThreadSpecificValue.h"

template <typename Array>
class UT_StackArrayData {
public:
    int alloc()
    {
	int	idx;

	if (myFreeList.entries())
	{
	    idx = myFreeList.last();
	    myFreeList.removeLast();
	    myValue(idx)->entries(0);
	}
	else
	{
	    idx = myValue.entries();
	    myValue.append(new Array);
	}
	return idx;
    }
    void release(int idx)
    {
	myFreeList.append(idx);
    }
    Array &get(int idx)
    {
	return *myValue(idx);
    }
private:
    UT_ValArray<Array *>	myValue;
    UT_IntArray			myFreeList;
};

#if !defined(WIN32) && !defined(MBSD)

class UT_StackPtrArray {
public:
    UT_StackPtrArray()
    {
	myIdx = myData.get().alloc();
    }
    ~UT_StackPtrArray()
    {
	myData.get().release(myIdx);
    }
    template <typename T>
    UT_ValArray<T> &get()
    {
	return *(UT_ValArray<T> *)&myData.get().get(myIdx);
    }

private:
    static UT_API UT_ThreadSpecificValue<
	UT_StackArrayData<UT_ValArray<void *> > >	myData;
    int							myIdx;
};

template <typename Array>
class UT_StackArray {
public:
    UT_StackArray()
    {
	myIdx = myData.get().alloc();
    }
    ~UT_StackArray()
    {
	myData.get().release(myIdx);
    }
    Array &get()
    {
	return myData.get().get(myIdx);
    }

private:
    static UT_API UT_ThreadSpecificValue<
	UT_StackArrayData<Array> >			myData;
    int							myIdx;
};

#else

class UT_StackPtrArray {
public:
    template <typename T>
    UT_ValArray<T> &get()
    {
	return *(UT_ValArray<T> *)&myData;
    }

private:
    UT_ValArray<void *>		myData;
};

template <typename Array>
class UT_StackArray {
public:
    Array &get()
    {
	return myData;
    }

private:
    Array			myData;
};

#endif

// Macros to simplify creation of stack arrays

#define UT_STACK_PTRARRAY(type, name) \
    UT_StackPtrArray	 __##name##_stack__; \
    UT_ValArray<type>	&name = __##name##_stack__.get<type>()

#define UT_STACK_INTARRAY(name) \
    UT_StackArray<UT_IntArray>	 __##name##_stack__; \
    UT_IntArray			&name = __##name##_stack__.get()

#define UT_STACK_FLTARRAY(name) \
    UT_StackArray<UT_FloatArray>	 __##name##_stack__; \
    UT_FloatArray			&name = __##name##_stack__.get()

#endif
