/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_IteratorRange.h ( UT Library, C++)
 *
 * COMMENTS:	A simple helper class to wrap an iterator range
 * 		into a single object, suitable for use with range-based
 * 		for loops.
 */

#ifndef __UT_IteratorRange__
#define __UT_IteratorRange__

#include <utility>

template<typename IterT>
class UT_IteratorRange
{
public:
    UT_IteratorRange(IterT &&begin_it, IterT &&end_it) :
	myBegin(std::move(begin_it)), myEnd(std::move(end_it))
    {}

    IterT begin() const { return myBegin; }
    IterT end() const { return myEnd; }

    bool isEmpty() const { return myBegin == myEnd; }
private:
    IterT myBegin, myEnd;
};

template<typename IterT>
UT_IteratorRange<IterT> UTmakeRange(IterT &&b, IterT &&e)
{
    return UT_IteratorRange<IterT>(std::move(b), std::move(e));
}

#endif // __UT_IteratorRange__
