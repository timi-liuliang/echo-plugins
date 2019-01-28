/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_RingBuffer.h ( UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_RingBuffer__
#define __UT_RingBuffer__

#include "UT_API.h"
#include "UT_ValArray.h"

/// A UT_RingBuffer is an indexable queue.  The buffer uses indices
/// that represent the insertion order of items in the buffer.  The
/// buffer may contain "holes", which have no associated data but 
/// still have an order number.
template <typename T>
class UT_RingBuffer {
public:
     UT_RingBuffer();
     UT_RingBuffer(int capacity);
    ~UT_RingBuffer();

    /// Null-padded insertion.  If the id of the incoming object is 
    /// greater than the next order number that would be assigned 
    /// by push(), holes are created in the buffer.
    void	 insert(int id, const T &data);
    void	 remove(int id); 

    /// Resets the buffer to an empty buffer with a 0 index
    void	 clear();

    /// Pop the first element off the head of the list, moving the first entry
    /// to the next element.
    T		 popFirst()
		 {
		     T	data = front();
		     pop();
		     return data;
		 }
    
    /// Insert the data into the buffer, and return the order number that
    /// it is assigned.
    int		 push(const T &data);

    /// Pop a single element from the start of the buffer
    void	 pop();

    /// Pop a single element from the back of the buffer
    void	 popBack();

    /// Pop the next "number" elements from the buffer
    void	 pop(int number);
   
    int          size() const { return myLastId - myFirstId + 1; }
    int		 entries() const { return size(); }
    int		 peakUsage() const	{ return myPeak; }
   
    /// operator [] is the bounds checked version of the operator.  The
    /// id is the order number of the object being indexed.
    T		 operator[](int id) const;
    const T	&operator()(int id) const
		 {
		     UT_ASSERT_P(id >= myFirstId && id <= myLastId);
		     return myData(idIndex(id));
		 }

    /// In order to iterate over the contents of the ring buffer
    ///		for (i = rbuf.frontId(); i <= rbuf.backId(); i++)
    ///			element = rbuf(i);
    /// The backId is *inclusive*!!!  There shouldn't be holes in the array.
    const T	&front() const { return myData(myFirst); }
    int		 frontId() const { return myFirstId; }
    const T	&back() const { return myData(myLast); }
    int		 backId() const { return myLastId; }

    void	 display() const;
   
private:
    void	 growIfNeeded(int number)
		 {
		     if (entries() + number > myData.entries())
			 growData(number);
		 }
    // Grows the size of the array by the specified amount
    void	 growData(int number);
    int		 idIndex(int id) const
		 { return (id - myFirstId + myFirst) % myData.entries(); }

private:
    UT_Array<T>	  myData;
    int		  myFirst;
    int		  myLast;

    // Order numbers increase monotonically as data is added to 
    // the buffer.
    int		  myFirstId;
    int		  myLastId;

    int		  myPeak;
};

template <typename T>
inline int
UT_RingBuffer<T>::push(const T &data)
{
    growIfNeeded(1);
    myLast++;
    if (myLast >= myData.entries())
	myLast = 0;
    myLastId++;
    myData(myLast) = data;
    return myLastId;
}

template <typename T>
inline void
UT_RingBuffer<T>::pop()
{
    UT_ASSERT_P(1 <= myLastId - myFirstId + 1);
    myFirst++;
    if (myFirst >= myData.entries())
	myFirst = 0;
    myFirstId++;
}

template <typename T>
inline void
UT_RingBuffer<T>::popBack()
{
    UT_ASSERT_P(1 <= myLastId - myFirstId + 1);
    myLast--;
    if (myLast < 0)
	myLast = myData.entries()-1;
    myLastId--;
}

#if defined( WIN32 ) || defined( LINUX ) || defined(MBSD) || defined(GAMEOS)
    #include "UT_RingBuffer.C"
#endif

#endif

