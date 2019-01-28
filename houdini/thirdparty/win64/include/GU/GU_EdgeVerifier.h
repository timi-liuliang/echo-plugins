/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_EdgeVerifier.h ( GU Library, C++)
 *
 */

#ifndef __GU_EdgeVerifier__
#define __GU_EdgeVerifier__

#include "GU_API.h"

#include <GA/GA_Types.h>

#include <UT/UT_Map.h>
#include <UT/UT_Set.h>

class GA_Detail;
template<typename T, bool B> class GA_EdgeT; 
using GA_Edge = GA_EdgeT<GA_Offset, false>;

/// This is a utility class to verify, in an iterative fashion, whether an edge
/// is topologically valid for the given detail. The lifetime of this class 
/// should not exceed the lifetime of the detail passed in.
/// Note also that if the topology of detail changes, the 
class GU_API GU_EdgeVerifier
{
public:
    GU_EdgeVerifier(const GA_Detail &gdp);
    
    /// Check if the given edge forms a valid topological edge and return
    /// \c true if so.
    bool isValidEdge(const GA_Edge &e);

    /// Returns the list of primitive that share this edge.
    GA_OffsetArray getEdgePrimitives(const GA_Edge &e);
    
    /// Reset the edge verifier internal state. This should be called in case 
    /// the topology changes, such as a point/primitive added, deleted, or
    /// re-ordered.
    void reset();
    
private:
    typedef UT_SortedSet<GA_Offset> PrimSet;
    const PrimSet &getPrimitiveSetForPoint(GA_Offset p);
    PrimSet getOverlappingPrimitives(const GA_Edge &e);
    bool verifyEdge(const GA_Edge &e) const;
    
    const GA_Detail &m_gdp;
    UT_Map<GA_Offset, PrimSet> m_pointPrimMap; 
};

#endif // __GU_EdgeVerifier__
