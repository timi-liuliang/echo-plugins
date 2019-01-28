/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_ContainerPrinter.h ( UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_ContainerPrinter__
#define __UT_ContainerPrinter__

#include <SYS/SYS_Types.h>

template<typename T>
class UT_ContainerPrinter
{
public:
    UT_ContainerPrinter(const T &c, exint limit = 100) :
	myContainer(c), myLimit(limit) {}
    
private:
    const T 	&myContainer;
    exint	 myLimit;
    
    template<typename OS, typename S>
    friend OS &operator<<(OS &os, const UT_ContainerPrinter<S> &d);
};

template<typename OS, typename S>
inline OS &
operator<<(OS &os, const UT_ContainerPrinter<S> &cp)
{
    os << "{";
    exint	c = 0;
    for (typename S::const_iterator it = cp.myContainer.begin(); 
	it != cp.myContainer.end(); ++it, ++c)
    {
	if (c > 0) os << ", ";
	if (cp.myLimit > 0 && c > cp.myLimit)
	{
	    os << "...";
	    break;
	}
	os << *it;
    }
    os << "}[" << cp.myContainer.size() << "]";
    return os;
}

#endif // __UT_ContainerPrinter__
