/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU Library.
 *
 * COMMENTS:	This file contains useful tetrahedra-connectivity and
 *		surface related methods.
 *
 */
#ifndef __GU_TetConnectivity_h__
#define __GU_TetConnectivity_h__

#include "GU_API.h"
#include "GU_Detail.h"
#include "GU_PrimTetrahedron.h"
#include <GA/GA_AttributeFilter.h>
#include <GA/GA_AttributeDict.h>
#include <GA/GA_AttributeSet.h>
#include <GA/GA_AIFCopyData.h>
#include <GA/GA_AIFMerge.h>
#include <GA/GA_MergeMap.h>
#include <GA/GA_MergeOptions.h>
#include <GA/GA_Types.h>
#include <GA/GA_ElementWrangler.h>

//
// GU_AdjacentTetrahedra represents a tetrahedron
// and the at most 4 tetrahedra that are adjacent to it
// 
class GU_API GU_AdjacentTetrahedra
{
public:
    GU_AdjacentTetrahedra()
    {
	myPrimitiveIndex = GA_INVALID_INDEX;

	for(int excluded_vertex = 0; excluded_vertex != 4; ++excluded_vertex)
	{
	    myNeighborForExcludedVertex[excluded_vertex] = GA_INVALID_INDEX;
	}
    }

    void setPrimitiveIndex(const GA_Index primitive_index)
    {
        UT_ASSERT( primitive_index >= 0 );

	myPrimitiveIndex = primitive_index;
    }

    // Assign the neighbor's primitive index for a facet
    // The facet is identified by the vertex that's excluded from it
    // "neighbor" must be a valid primitive index.
    void setNeighbor(
        const int excluded_vertex,
        const GA_Index neighbor
    )
    {
        UT_ASSERT( (0 <= excluded_vertex) && (excluded_vertex < 4) );
        UT_ASSERT( neighbor != GA_INVALID_INDEX );
	UT_ASSERT( neighbor != myPrimitiveIndex );

	myNeighborForExcludedVertex[excluded_vertex] = neighbor;
    }

    void removeNeighbor(const int excluded_vertex)
    {
        UT_ASSERT( (0 <= excluded_vertex) && (excluded_vertex < 4) );
	myNeighborForExcludedVertex[excluded_vertex] = GA_INVALID_INDEX;
    }

    GA_Index getPrimitiveIndex() const
    {
	return myPrimitiveIndex;
    }

    // Return the primitive index of the neighbor tetrahedron
    // along the facet that's opposite to "excluded_vertex"
    // If there is no neighbor at that facet, then return GA_INVALID_INDEX
    GA_Index neighbor(const int excluded_vertex) const
    {
        UT_ASSERT( (0 <= excluded_vertex) && (excluded_vertex < 4) );

	return myNeighborForExcludedVertex[excluded_vertex];
    }

    // Return whether this tetrahedron has at least one face with
    // no adjacent tetrahedron
    bool isBoundaryTet() const
    {
	for(int excluded_vertex = 0; excluded_vertex != 4; ++excluded_vertex)
	{
	    if( myNeighborForExcludedVertex[excluded_vertex] == GA_INVALID_INDEX )
		return true;
	}
	return false;
    }

private:
    // The index into the detail's list of primitives
    // NOTE: some of the detail's primitives may not be tetrahedrons
    GA_Index myPrimitiveIndex;

    // For each excluded vertex, store the neighbors for the
    // facet that opposes it.
    // Note that at most one valid adjacent tetrahedron is stored
    // for each facet.
    // If there is no neighbor, then GA_INVALID_INDEX is stored.
    // Any additional adjacent tetrahedrons are discarded.
    GA_Index myNeighborForExcludedVertex[4];
};

// This class is used when constructing the list of adjacent tetrahedra
//
// A structure containing 3 point indices and a primitive index
// representing the boundary triangle of a tetrahedron
//
// Note: the point indices must be in increasing order
//
struct GU_TetrahedronFacet
{
    // Sorted list of point indices for tetrahedron face
    GA_Index myPS[3];

    // Primitive index for the tetrahedron
    GA_Index myPrimitiveIndex;

    // The single tet vertex not included in the facet
    // A number between 0 and 3
    int myExcludedVertex;

    // PRE: pids is sorted
    GU_TetrahedronFacet(
        const GA_Index pids[3],
        const GA_Index primitive_index,
        const int excluded_vertex
    )
    {
	UT_ASSERT( pids[0] < pids[1] );
	UT_ASSERT( pids[1] < pids[2] );

        myPS[0] = pids[0];
        myPS[1] = pids[1];
        myPS[2] = pids[2];

	myPrimitiveIndex = primitive_index;
	myExcludedVertex = excluded_vertex;
    }

private:
    // Disallow
    GU_TetrahedronFacet();
};

// Order first by point triple, second by primitive index
// When you sort using this order, all tetrahedrons that
// share a designated facet will be contiguous
inline bool operator< (
    const GU_TetrahedronFacet& a,
    const GU_TetrahedronFacet& b
)
{
    for( int id = 0; id != 3; ++id )
    {
        if( a.myPS[id] < b.myPS[id] )
           return true;
	
        if( a.myPS[id] > b.myPS[id] )
            return false;
    }

    if( a.myPrimitiveIndex < b.myPrimitiveIndex )
        return true;

    if( a.myPrimitiveIndex > b.myPrimitiveIndex )
        return false;

    return false; // these are equal
}

// Determine whether two tetrahedron facets are equal
// NOTE: This does not mean that the tetrahedra themselves are equal
inline bool GUhaveSameFacet(
    const GU_TetrahedronFacet& a,
    const GU_TetrahedronFacet& b
)
{
    for( int id = 0; id != 3; ++id )
        if( a.myPS[id] != b.myPS[id] )
            return false;

    return true;
}

class GU_API GU_TetrahedronAdjacency
{
public:
    GU_TetrahedronAdjacency(const GU_Detail& detail);

    const GU_AdjacentTetrahedra &getAdjacentTetrahedraForPrimitive(
        GA_Index prim_index) const
    {
        UT_ASSERT_P(GAisValid(prim_index) && prim_index < GA_Index(adjacent_tetrahedra.size()));
        return adjacent_tetrahedra[prim_index];
    }

    int myNumTetConnectionsWithMoreThanTwoTets() const
    {
        return tetrahedra_connections_with_more_than_two_tetrahedra;
    }

    const std::vector<GU_AdjacentTetrahedra> &raw() const
    {
        return adjacent_tetrahedra;
    }

private:
    std::vector< GU_AdjacentTetrahedra > adjacent_tetrahedra;
    int tetrahedra_connections_with_more_than_two_tetrahedra;
};

////////////////////////////////////////////////////////////////////
// Useful tetrahedron-related functions


//
// Get the surface
//
GU_API bool GUgetTetSurface(GU_Detail& gdp, const GU_Detail& src_gdp, bool buildpolysoup=false);

#endif
