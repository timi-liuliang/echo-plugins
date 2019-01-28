/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Counter.h ( UT Library, C++)
 *
 * COMMENTS:	Class to keep track of records and print out the data at the
 *		end of a render.  You simply have to declare a static printer
 *		in the file you want the printing to occur in.  The destructor
 *		will cause the stat to be printed.
 */

#ifndef __UT_Counter__
#define __UT_Counter__

#include "UT_API.h"

#ifdef UT_DEBUG

#include <stdio.h>
#include <SYS/SYS_AtomicInt.h>
#include "UT_String.h"

class UT_API UT_Counter {
public:
    explicit UT_Counter(const char *msg) 
    {
	myMessage = msg;
    }
    UT_Counter(const UT_Counter &m)
    {
	myMessage = m.myMessage;
	myCount.relaxedStore(m.myCount.relaxedLoad());
	myPeak.relaxedStore(m.myPeak.relaxedLoad());
	myIncrements.relaxedStore(m.myIncrements.relaxedLoad());
    }
    
    ~UT_Counter()
     {
	if (myPeak.load() > 0)
	{
	    UT_String	count, peak, incs;
	    count.itoa_pretty(myCount.load());
	    peak.itoa_pretty(myPeak.load());
	    incs.itoa_pretty(myIncrements.load());
	    printf("%s: %s [%s peak, %s increments]\n",
		    myMessage, count.buffer(), peak.buffer(), incs.buffer());
            fflush(stdout);
	}
     }

    UT_Counter	&max(exint v)
		{
		    myPeak.maximum(v);
		    myCount.maximum(v);
		    return *this;
		}
    UT_Counter	&operator+=(int v)
		 {
		     myPeak.maximum(myCount.add(v));
		     if (v > 0)
			 myIncrements.add(v);
		     return *this;
		 }
    UT_Counter	&operator-=(int v)	{ return operator+=(-v); }
    UT_Counter	&operator++(int)	{ return operator+=(1); }
    UT_Counter	&operator--(int)	{ return operator+=(-1); }
    void	 reset()		{ myCount.store(0,
						SYS_MEMORY_ORDER_RELAXED); }
    exint	 peak() const		{ return myPeak.load(); }
    exint	 count() const		{ return myCount.load(); }
    exint	 increments() const	{ return myIncrements.load(); }

private:
    SYS_AtomicCounter		 myCount;
    SYS_AtomicCounter		 myPeak;
    SYS_AtomicCounter		 myIncrements;
    const char			*myMessage;
};

#else		// UT_DEBUG

#include <SYS/SYS_Types.h>
// When no debugging is on, we'll make a dummy class.  Hopefully, the compiler
// will optimize this right out.
class UT_API UT_Counter {
public:
     UT_Counter(const char *)	{}
    ~UT_Counter()		{}

    UT_Counter	&max(exint)		{ return *this; }
    UT_Counter	&operator+=(int)	{ return *this; }
    UT_Counter	&operator++(int)	{ return *this; }
    UT_Counter	&operator-=(int)	{ return *this; }
    UT_Counter	&operator--(int)	{ return *this; }
    void	 reset()		{ }
    exint	 peak() const		{ return 0; }
    exint	 count() const		{ return 0; }
    exint	 increments() const	{ return 0; }
};

#endif		// UT_DEBUG
#endif
