/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:     GA_Edge.h (GA Library, C++)
 *
 * COMMENTS: This class represents an edge as two point offsets.
 *           Comparisons treat the edge as undirected.
 */

#pragma once

#ifndef __GA_Edge__
#define __GA_Edge__

#include "GA_API.h"
#include "GA_Types.h"
#include <SYS/SYS_Hash.h>

template<typename T,bool DIRECTED>
class GA_EdgeT
{
public:
    typedef GA_EdgeT<T,DIRECTED> ThisType;

    GA_EdgeT() : myP0(T(-1)), myP1(T(-1)) {}
    GA_EdgeT(T pt0, T pt1) : myP0(pt0), myP1(pt1) {}
    ~GA_EdgeT() {}

    T  p0()  const { return myP0; }
    T &p0()        { return myP0; }
    T  p1()  const { return myP1; }
    T &p1()        { return myP1; }

    int64 getMemoryUsage() const
    {
        return sizeof(*this);
    }

    bool isValid() const
    { return GAisValid(myP0) && GAisValid(myP1); }

    /// Return whether this edge is the same undirected edge as the given edge.
    bool operator==(const ThisType & e) const
    {
        if (DIRECTED)
        {
            return (myP0 == e.myP0 && myP1 == e.myP1);
        }
        else
        {
            return (myP0 == e.myP0 && myP1 == e.myP1)
                || (myP0 == e.myP1 && myP1 == e.myP0);
        }
    }
    bool operator!=(const ThisType & e) const
    {
        return !(*this == e);
    }

    /// Hash function
    size_t hash() const
    {
	size_t hash_val;
	if (DIRECTED || myP0 < myP1)
	{
	    hash_val = GA_Size(myP0);
	    SYShashCombine(hash_val, GA_Size(myP1));
	}
	else
	{
	    hash_val = GA_Size(myP1);
	    SYShashCombine(hash_val, GA_Size(myP0));
	}
	return hash_val;
    }

private:
    T myP0;
    T myP1;
};

template<typename T,bool DIRECTED>
SYS_FORCE_INLINE size_t hash_value(const GA_EdgeT<T,DIRECTED> &edge)
{
    return edge.hash();
}

// Specialize the GA_EdgeT template for the known edge types
using GA_Edge = GA_EdgeT<GA_Offset, false>;
using GA_DirectedEdge = GA_EdgeT<GA_Offset, true>;

using GA_IndexEdge = GA_EdgeT<GA_Index, false>;
using GA_DirectedIndexEdge = GA_EdgeT<GA_Index, true>;

namespace std {
template<typename T,bool DIRECTED>
struct hash<GA_EdgeT<T,DIRECTED> >
{
    typedef GA_EdgeT<T,DIRECTED> argument_type;
    typedef size_t result_type;

    size_t operator()(const GA_EdgeT<T,DIRECTED> &edge) const
    {
        return edge.hash();
    }
};
}

#endif
