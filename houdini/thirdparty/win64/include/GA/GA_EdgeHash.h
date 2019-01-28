/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_EdgeHash.h ( GU Library, C++)
 *
 * COMMENTS:	This has a hash object useful for holding unordered
 *		and directed edges.  It is designed to be used with the
 *		UT_HashTable class.
 */

#ifndef __GA_EdgeHash__
#define __GA_EdgeHash__

#include "GA_API.h"
#include <UT/UT_Hash.h>

#include "GA_Types.h"

SYS_DEPRECATED_PUSH_DISABLE()

// The hash key for unordered edges:
template <typename T>
class SYS_DEPRECATED_REPLACE(16.0, "SYShash") GA_EdgeHashT : public UT_Hash
{
public:
    GA_EdgeHashT(T a, T b) : myA(a), myB(b) { orderKey(); }
    GA_EdgeHashT(GA_EdgeHashT const &h) : UT_Hash(), myA(h.myA), myB(h.myB)
			{ orderKey(); }
    virtual ~GA_EdgeHashT() {}

    T			getA() const { return myA; }
    T			getB() const { return myB; }

    virtual int		compare(const UT_Hash &a) const
			{
			    return !(myA==((const GA_EdgeHashT &)a).myA &&
				     myB==((const GA_EdgeHashT &)a).myB);
			}
    virtual void	copy(const UT_Hash &a)
			{
			    myA = static_cast<const GA_EdgeHashT &>(a).myA;
			    myB = static_cast<const GA_EdgeHashT &>(a).myB;
			}
    virtual unsigned	hash() const
			{
			    return myA * 47 + myB * 53;
			}

    virtual UT_Hash	*copy() const
			{
			    return new GA_EdgeHashT(myA, myB);
			}

    virtual int64 getMemoryUsage(bool inclusive) const
    { return inclusive ? sizeof(*this) : 0; }

private:
    void		orderKey()
			{
			    if (myA > myB)
			    {
				T t = myA;
				myA = myB;
				myB = t;
			    }
			}

    T			myA;
    T			myB;
};

typedef GA_EdgeHashT<GA_Offset>	GA_EdgeHash;
typedef GA_EdgeHashT<GA_Index>	GA_IndexEdgeHash;

// The hash key for directed edges:
template <typename T>
class GA_DirectedEdgeHashT : public UT_Hash
{
public:
    GA_DirectedEdgeHashT(T a, T b) : myA(a), myB(b) { }
    GA_DirectedEdgeHashT(GA_DirectedEdgeHashT const &h) : UT_Hash(), myA(h.myA), myB(h.myB) { }
    virtual ~GA_DirectedEdgeHashT() {}


    T			getA() const { return myA; }
    T			getB() const { return myB; }

    virtual int		compare(const UT_Hash &a) const
			{
			    return !(myA==((const GA_DirectedEdgeHashT &)a).myA &&
				     myB==((const GA_DirectedEdgeHashT &)a).myB);
			}
    virtual void	copy(const UT_Hash &a)
			{
			    myA = static_cast<const GA_DirectedEdgeHashT &>(a).myA;
			    myB = static_cast<const GA_DirectedEdgeHashT &>(a).myB;
			}
    virtual unsigned	hash() const
			{
			    return myA * 47 + myB * 53;
			}

    virtual UT_Hash	*copy() const
			{
			    return new GA_DirectedEdgeHashT(myA, myB);
			}

    virtual int64 getMemoryUsage(bool inclusive) const
    { return inclusive ? sizeof(*this) : 0; }

private:
    T			myA;
    T			myB;
};

typedef SYS_DEPRECATED_REPLACE(16.0, "SYShash") 
    GA_DirectedEdgeHashT<GA_Offset>	GA_DirectedEdgeHash;
typedef SYS_DEPRECATED_REPLACE(16.0, "SYShash") 
    GA_DirectedEdgeHashT<GA_Index>	GA_DirectedIndexEdgeHash;

SYS_DEPRECATED_POP_DISABLE()

#endif
