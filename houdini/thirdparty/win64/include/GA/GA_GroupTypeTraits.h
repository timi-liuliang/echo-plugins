/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_GroupTypeTraits.h ( GA Library, C++)
 *
 * COMMENTS:	Type traits class for groups.
 */

#ifndef __GA_GroupTypeTraits__
#define __GA_GroupTypeTraits__

#include "GA_Types.h"

class GA_BreakpointGroup;
class GA_Breakpoint;
class GA_EdgeGroup;
class GA_PointGroup;
class GA_PrimitiveGroup;
class GA_VertexGroup;

template<typename T, bool B> class GA_EdgeT; 
using GA_Edge = GA_EdgeT<GA_Offset, false>;


/// Type traits base class for group type traits.
template<typename T>
struct GA_GroupTypeTraits
{
    /// Returns the base element type that the group expects. 
    typedef void elementType; 
    static inline GA_GroupType groupType() { return GA_GROUP_INVALID; }
};

template<>
struct GA_GroupTypeTraits<GA_BreakpointGroup>
{
    typedef GA_Breakpoint elementType; 
    static inline GA_GroupType groupType() { return GA_GROUP_BREAKPOINT; }
};

template<>
struct GA_GroupTypeTraits<GA_EdgeGroup>
{
    typedef GA_Edge elementType; 
    static inline GA_GroupType groupType() { return GA_GROUP_EDGE; }
};

template<>
struct GA_GroupTypeTraits<GA_PointGroup>
{
    typedef GA_Offset elementType; 
    static inline GA_GroupType groupType() { return GA_GROUP_POINT; }
};

template<>
struct GA_GroupTypeTraits<GA_PrimitiveGroup>
{
    typedef GA_Offset elementType; 
    static inline GA_GroupType groupType() { return GA_GROUP_PRIMITIVE; }
};

template<>
struct GA_GroupTypeTraits<GA_VertexGroup>
{
    typedef GA_Offset elementType; 
    static inline GA_GroupType groupType() { return GA_GROUP_VERTEX; }
};

#endif // __GA_GroupTypeTraits__
