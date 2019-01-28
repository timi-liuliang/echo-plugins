/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Pair.h ( UT Library, C++)
 *
 * COMMENTS:	Simple templated pair of variables.
 */

#ifndef __UT_Pair__
#define __UT_Pair__

#ifdef WIN32
    #pragma warning(disable:4251)
    #pragma warning(disable:4275)
#endif

template <class T1, class T2>
class UT_Pair {
public:
    UT_Pair()
	: myFirst(T1()), mySecond(T2()) { }
    UT_Pair(T1 v1, T2 v2)
	: myFirst(v1), mySecond(v2) { }

    bool	operator==(const UT_Pair<T1, T2> &p) const
		{
		    return (myFirst == p.myFirst) && (mySecond == p.mySecond);
		}
    bool	operator!=(const UT_Pair<T1, T2> &p) const
		{
		    return (myFirst != p.myFirst) || (mySecond != p.mySecond);
		}

    T1		 myFirst;
    T2		 mySecond;
};

/// Common types
typedef UT_Pair<int,int> UT_IntPair;

template<typename OS, typename T1, typename T2>
inline OS &
operator<<(OS &os, const UT_Pair<T1, T2> &p)
{
    os << "UT_Pair(" << p.myFirst << ", " << p.mySecond << ")";
    return os;
}

#endif
