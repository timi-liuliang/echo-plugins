/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        UT_Tetrahedralize.h (UT Library, C++)
 *
 * COMMENTS:
 *
 * This calculates the 3D Delaunay triangulation (tetrahedralization) 
 * of the input points. Note that it does not support constrained 
 * triangulation or refinement.  This implementation closely follows:
 *     Ledoux, Hugo. "Computing the 3D Voronoi Diagram Robustly: 
 *		      An Easy Explanation."
 *     Proceedings 4th International Symposium on Voronoi Diagrams in 
 *     Science and Engineering.
 *     Pontypridd, Wales, UK. 2007. pp. 117-129
 *     http://www.gdmc.nl/ledoux/pdfs/_07isvd.pdf
 *
 * It uses Schewchuk's geometric predicates as provided in 
 * UT_GeometryPredicate.h to ensure robustness when the 
 * points are in degenerate positions.
 */


#ifndef __UT_Tetrahedralize_h
#define __UT_Tetrahedralize_h

#include "UT_API.h"
#include "UT_Array.h"
#include "UT_Vector3.h"
#include "UT_Interrupt.h"

class UT_TetTetrahedron;
class UT_TetVertex;

typedef UT_Array<UT_TetVertex *>        UT_TetVertexArray;
typedef UT_Array<UT_TetTetrahedron *>   UT_TetTetrahedronArray;

/// The UT_TetVertex class contains the position of the vertex, a data 
/// parameter for use by calling code, and a list of the Tetrahedra 
/// containing this vertex.
/// Note that this tetrahedron list will not be valid until after a 
/// call to tetrahedralization completes successfully.
class UT_API UT_TetVertex
{
public:
                        UT_TetVertex(const UT_Vector3 &p, void *data = NULL);

    void                addTet(UT_TetTetrahedron *t);
    /// Calculates the vertices that share an edge with this one 
    /// in one or more tetrahedra.
    void                incidentVertices(UT_TetVertexArray &incident) const;
    
    const UT_Vector3    &getPos() const     {return myPos;}

    /// A data parameter that can be used for arbitrary purposes by 
    /// calling code.
    void                setData(void *d)    {myData = d;}
    void                *data() const       {return myData;}
private:
    UT_Vector3          myPos;
    void                *myData;
    UT_TetTetrahedronArray myTets;
};

/// The UT_TetFace struct is a simple container class to hold UT_TetVertex 
/// objects, and represents the triangular faces of a tetrahedron. 
/// They are created on the fly during the tetrahedralization 
/// process, and are not stored by any other objects.
struct UT_API UT_TetFace
{
                        UT_TetFace(): 
                            a(NULL), b(NULL), c(NULL) {}
                        UT_TetFace(UT_TetVertex *a_, UT_TetVertex *b_,UT_TetVertex *c_):
                            a(a_), b(b_), c(c_) {}
    bool                hasVertex(const UT_TetVertex *v) const {return a == v || b == v || c == v;}
    UT_TetVertex           *a, *b, *c;
};

/// The UT_TetTetrahedron class is the primary structure in the 
/// tetrahedralization process.
/// It holds pointers to its four vertex objects, as well as to any 
/// adjacent tetrahedra.
/// The convention is that a particular adjacent tetrahedra is stored 
/// with the same index as the face to which it is adjacent and the 
/// vertex from which it is opposite.
/// In other words, adjacent(i) is adjacent to face(i), which is opposite 
/// vertex(i).
///
/// Several of the member functions are related to maintaining the 
/// adjacency relationships during the tetrahedralization process.  
/// They are called only from the various flip?? functions.
///
/// This class also holds a visited flag that is used to stop iterating 
/// during walks through adjacent tetrahedra in the walk function and 
/// the tetrahedralization function.  This is faster than using something 
/// like hboost::unordered_set to keep track of visited tetrahedra at the 
/// expense of a small amount of memory.
class UT_API UT_TetTetrahedron
{
public:
                        UT_TetTetrahedron(UT_TetVertex *a, UT_TetVertex *b, UT_TetVertex *c, UT_TetVertex *d);
    
    /// Calls UT_TetVertex::addTet on each of its vertices.
    void                addToVertices();
    
    /// Updates the adjacency information for the face opposite vertex v.
    void                setAdjacentOppositeVertex(const UT_TetVertex *v, 
						  UT_TetTetrahedron *tet);
    UT_TetTetrahedron      *adjacentOppositeVertex(const UT_TetVertex *v) const;
    
    /// Updates the adjacency information for the face opposite vertex v, and 
    /// (if non-NULL) also updates the provided tetrahedron's adjacency to 
    /// point to this tetrahedron.
    void                updateAdjacency(const UT_TetVertex *v, UT_TetTetrahedron *tet);
    
    /// Returns the tetrahedron adjacent to this one at the given face.
    UT_TetTetrahedron      *adjacentAtFace(const UT_TetFace &f) const;
    
    /// Returns whether a particular tetrahedron is adjacent to this one.
    bool                hasAdjacent(const UT_TetTetrahedron *tAdj) const;

    /// Clears out any adjaceny pointers set to provided tetrahedron.
    void                clearAdjacency(UT_TetTetrahedron* tAdj);
    
    /// Tests for the existence of a given face, vertex, any of a set of 
    /// vertices.
    bool                hasFace(const UT_TetFace &f) const;
    bool                hasVertex(UT_TetVertex* v) const;
    bool                hasAnyVertex(const UT_TetVertexArray &verts) const;
    
    bool                vertexInside(const UT_TetVertex *v) const;
    
    /// Diagnostic function - should always return true when tet is 
    /// locally delaunay with tetrahedralization.
    bool                delaunay() const;

    /// Returns the first vertex in this that is not in provided tetrahedron.
    UT_TetVertex           *distinctVertex(const UT_TetTetrahedron *tAdj) const;
    
    /// Returns the first vertex in this that is not in the provided face.
    UT_TetVertex           *distinctVertex(const UT_TetFace &f) const;

    /// Unchecked access to vertices,
    UT_TetVertex           *vertex(unsigned int i) const   {return myVerts[i];}
    /// Unchecked access to adjacent tetrahedra,
    UT_TetTetrahedron      *adjacent(unsigned int i) const {return myAdjacent[i];}
    /// Returns face such that face(i) is opposite from vertex(i) and 
    /// adjacent to adjacent(i).
    UT_TetFace             face(unsigned int i) const;
    
    /// Visited flag.
    void                setVisited(bool b)              {myVisited = b;}
    bool                visited() const                 {return myVisited;}
private:
    UT_TetVertex *myVerts[4];
    UT_TetTetrahedron *myAdjacent[4];
    bool myVisited;
};
    
/// The tetrahedralization function.  The input is an array of pointers to 
/// UT_TetVertex objects; the output is a list of pointers to UT_TetTetrahedron 
/// objects.  The caller is responsible for deleting the UT_TetTetrahedron 
/// objects (and probably the UT_TetVertex objects, depending on how they were 
/// created).
/// The boss parameter allows the caller to provide a UT_Interrupt object 
/// to poll for interrupts and update with progress.  The interruptInterval 
/// is the number of points processed between calls to 
/// UT_Interrupt::opinterrupt. 
///
/// This function returns false if the tetrahedralization fails, meaning the 
/// tetrahedralization is in an invalid state, although that really, really 
/// should not happen.  Note that duplicate points are degenerate and will 
/// not be inserted into the tetrahedralization, but this does not indicate 
/// a failure condition.
UT_API bool 
UTtetrahedralize( const UT_TetVertexArray &verts, 
		  UT_TetTetrahedronArray &tets);
#endif
