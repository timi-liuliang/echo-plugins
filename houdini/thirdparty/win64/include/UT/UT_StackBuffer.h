/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_StackBuffer.h (C++)
 *
 * COMMENTS:	A special buffer class that allocates from the stack if
 *		below the given threshold.
 *
 */

#ifndef __UT_StackBuffer_h__
#define __UT_StackBuffer_h__

#include <SYS/SYS_Types.h>

template <typename T, size_t MAXBYTES = 256>
class UT_StackBuffer
{
public:
    UT_StackBuffer(size_t num_elems)
	: mySize(num_elems)
    {
	if (num_elems <= MAXELEMS)
	    myData = myBuffer;
	else
	    myData = new T[num_elems];
    }
    ~UT_StackBuffer()
    {
	if (myData != myBuffer)
	    delete [] myData;
    }

    operator T *()
    {
	return myData;
    }
    operator const T *() const
    {
	return (const T *)myData;
    }

    T		*array() { return myData; }
    const T	*array() const { return myData; }

    // Some compilers get confused when there are both the array subscript
    // operators below and the implicit cast to pointer above,
    // even though it seems obvious that the operator[] should take
    // precedence when doing buf[integer].
#if 0
    T		&operator[](exint i) { return myData[i]; }
    const T	&operator[](exint i) const { return myData[i]; }
#endif

    T		&operator()(exint i) { return myData[i]; }
    const T	&operator()(exint i) const { return myData[i]; }

    exint	size() const	{ return mySize; }

    typedef T		*iterator;
    typedef const T	*const_iterator;
    iterator		 begin() { return myData; }
    iterator		 end() { return myData+mySize; }
    const_iterator	 begin() const { return myData; }
    const_iterator	 end() const { return myData+mySize; }

private:
    static const int	MAXELEMS = (MAXBYTES + sizeof(T) - 1)/sizeof(T);

    T		*myData;
    exint	 mySize;
    T		 myBuffer[MAXELEMS];
};

#endif // __UT_StackBuffer_h__
