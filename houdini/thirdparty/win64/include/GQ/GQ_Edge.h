/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Quad Edge Library (C++)
 *
 * COMMENTS:
 *
 * The Quad Edge data structure was invented by Guibas & Stolfi in their
 * 1985 paper, "Primitives for the Manipulation of General Subdivisions and
 * the Computation of Voronoi Diagrams", ACM Transactions on Graphics,
 * 4(2):74-123, April 1985.
 *
 * That paper has a good description of the capabilities of the data
 * structure, and the definition of its basic operators, makeedge and splice().
 *
 */

#ifndef _GQ_Edge_h_
#define _GQ_Edge_h_

#include "GQ_API.h"
#include "GQ_Point.h"
#include <UT/UT_VectorTypes.h>
#include <UT/UT_SmallObject.h>
#include <GA/GA_Edge.h>
#include <iosfwd>

// This does not do the real diagram justice. Read Documentation.ps.gz in
// the current directory for a better idea of how the quad data structure
// is set up.
//
// lnext \       | / dnext     dprev \ |       / rprev        .
//        \---   |/                   \|   ---/               .
//        |\     /---               ---\     /|               .
//        | \   /                       \   / |               .
//           \ /                         \ /                  .
//            |                           |                   .
//            |                           |                   .
//            | e                         | e                 .
//            |                           |                   .
//            |                           |                   .
//           / \                         / \                  .
//        | /   \                       /   \ |               .
//        |/     \---               ---/     \|               .
//        /---   |\                   /|   ---\               .
// onext /       | \ rnext     lprev / |       \ oprev        .

class GQ_Point;
class GQ_Face;
class GQ_Detail;

//  Common flag definitions

#define GQ_BRIDGE       0x00010000      // Edge is a bridge edge 
#define GQ_INTERSECT    0x00020000      // Point is intersect on a plane/surface 
#define GQ_INSIDE    	0x00040000      // Point is inside a close geometry 
#define GQ_OUTSIDE      0x00080000      // Point is outside a close geometry 
#define GQ_VISIT	0x00100000      // Visited this edge
#define GQ_DELETE	0x00200000      // Mark as delete edge 
#define GQ_NEW	        0x00400000      // Mark as new edge 
#define GQ_GLUE		0x00800000      // Share edge resulted from unique point op 
#define GQ_SELECTED     0x01000000      // Need to process
#define GQ_SPAREPTR	0x02000000	// Spare pointer points to ptr array
#define GQ_VTXBOUNDARY	0x04000000	// Point has differing vertex
#define	GQ_EDGE		0x08000000	// Face used to be edge
#define GQ_CORNER	0x10000000	// Face used to be corner
					// attributes.

/// Traverse all the edges which access point 'elem'. 'edge' will be set
/// to each edge in turn. 'nedges' will be set to the number of edges accessed.
/// We set our current edge to one of the edges that falls off the point,
/// then we can go from each edge to the onext edge (see notes at onext,
/// oprev, etc in diagram above and comments below). Eventually, you will
/// get back to the original edge.
/// You can use the same macro for faces, but for clarity you might want to
/// change the name of the next pointer to be lnext (next edge on left face).
#define FOR_QUAD_EDGES(elem, edge, nedges, dir) \
                            for (edge=elem->getEdge(), nedges=0; \
                                 edge && (edge != elem->getEdge() || !nedges); \
                                 edge=edge->dir(), nedges++)

/// Exactly like FOR_QUAD_EDGES, but using a local instance of an unnamed
/// struct to hold the iteration variables.
///
/// NB: Because of a Visual Studio bug, we cannot use an unnamed struct as
///     intended.
///
/// Example:
///     FOR_QUAD_EDGES_STRUCT(face, scan, lnext)
///         fprintf(stderr, "  edge[%d] = %p\n", scan.i, scan.edge);
///
#define FOR_QUAD_EDGES_STRUCT(elem, localname, dir) \
    for (GQ_Edge::MacroIterStruct localname(elem->getEdge(), 0); \
	 localname.edge && (localname.edge != elem->getEdge()||!localname.i); \
	 localname.edge=localname.edge->dir(), localname.i++)

/// A single quad edge is stored as a GQ_Edge[4] block.
#define Q_RotN(e,n)		(e + (((e)->myIndex + n) & 0x03) - \
				    ((e)->myIndex & 0x03))
#define Q_ROT(e)                (Q_RotN(e, 1))
#define Q_SYM(e)                (Q_RotN(e, 2))
#define Q_IROT(e)               (Q_RotN(e, 3))
#define Q_ONEXT(e)              ((e)->myNext)
#define Q_OPREV(e)              (Q_ROT( Q_ONEXT( Q_ROT(e))))
#define Q_DNEXT(e)              (Q_SYM( Q_ONEXT( Q_SYM(e))))
#define Q_DPREV(e)              (Q_IROT( Q_ONEXT( Q_IROT(e))))
#define Q_LNEXT(e)              (Q_ROT( Q_ONEXT( Q_IROT(e))))
#define Q_LPREV(e)              (Q_SYM( Q_ONEXT(e)))
#define Q_RNEXT(e)              (Q_IROT( Q_ONEXT( Q_ROT(e))))
#define Q_RPREV(e)              (Q_ONEXT( Q_SYM(e)))
#define Q_ORG(e)                ((e)->myData)
#define Q_DEST(e)               (Q_ORG( Q_SYM(e)))
#define Q_LEFT(e)               (Q_ORG( Q_IROT(e)))
#define Q_RIGHT(e)              (Q_ORG( Q_ROT(e)))

class GQ_Edge;

class GQ_API GQ_Edge : public UT_SmallObject<GQ_Edge>
{
public:
    /// Creates a data structure representing a subdivision of the sphere
    /// by a single edge with a single face.
    void		 init(int i)
    {
        myIndex = i;
        myData = 0;
        clearFlags();
        if ((i & 0x03) == 0)
        {
            this->onext() = this;
            this->sym()->onext() = this->sym();
            this->rot()->onext() = this->irot();
            this->irot()->onext() = this->rot();
        }
    }
    GQ_Edge		*root()         { return this-(myIndex & 0x03); }
    const GQ_Edge	*root() const   { return this-(myIndex & 0x03); }
    void                 setData(void *org, void *dest, void *left, void *right);

    GQ_Edge		*rotN(int n)    { return Q_RotN(this,n); }
    const GQ_Edge	*rotN(int n) const { return Q_RotN(this,n); }
    GQ_Edge		*rot()          { return Q_ROT(this); }
    const GQ_Edge	*rot() const    { return Q_ROT(this); }

    /// Get opposite orientation of this edge (symmetry). Returns an edge
    /// pointing in the opposite direction.
    GQ_Edge		*sym()          { return Q_SYM(this); }
    const GQ_Edge	*sym() const    { return Q_SYM(this); }
    GQ_Edge		*irot()         { return Q_IROT(this); }
    const GQ_Edge	*irot() const   { return Q_IROT(this); }

    /// Each edge is able to get at its previous and next edges in either
    /// direction (_L_eft, _R_ight, _O_rigin and _Destination)
    GQ_Edge	       *&onext()        { return myNext; }
    const GQ_Edge	*onext() const  { return myNext; }
    GQ_Edge		*oprev()        { return Q_OPREV(this); }
    const GQ_Edge	*oprev() const  { return Q_OPREV(this); }
    GQ_Edge		*dnext()        { return Q_DNEXT(this); }
    const GQ_Edge	*dnext() const  { return Q_DNEXT(this); }
    GQ_Edge		*dprev()        { return Q_DPREV(this); }
    const GQ_Edge	*dprev() const  { return Q_DPREV(this); }
    GQ_Edge		*lnext()        { return Q_LNEXT(this); }
    const GQ_Edge	*lnext() const  { return Q_LNEXT(this); }
    GQ_Edge		*lprev()        { return Q_LPREV(this); }
    const GQ_Edge	*lprev() const  { return Q_LPREV(this); }
    GQ_Edge		*rnext()        { return Q_RNEXT(this); }
    const GQ_Edge	*rnext() const  { return Q_RNEXT(this); }
    GQ_Edge	       *&rprev()        { return Q_RPREV(this); }
    const GQ_Edge	*rprev() const  { return Q_RPREV(this); }

    /// Get the origin of the edge. Needs to be cast to a GQ_Point.
    void	       *&org()          { return myData; }
    const void		*org() const    { return myData; }


    GQ_Point		*orgPoint() const { return static_cast<GQ_Point *>(myData); }

    /// Get the destination of the edge. Needs to be cast to a GQ_Point.
    void	       *&dest()         { return Q_DEST(this); } 
    const void		*dest() const   { return Q_DEST(this); } 

    GQ_Point		*destPoint() const { return static_cast<GQ_Point *>(Q_DEST(this)); }
    
    void		*safeDest() {if (dest()) return dest();
				     if (lnext()->org()) return lnext()->org();
				     return rnext()->org();
				    }

    /// Get the face on the left of an edge. Needs to be cast to a GQ_Face.
    void	       *&left()         { return Q_LEFT(this); }
    const void		*left() const   { return Q_LEFT(this); }

    GQ_Face		*leftFace() const { return static_cast<GQ_Face *>(Q_LEFT(this)); }

    /// Get the face on the right of an edge. Needs to be cast to a GQ_Face.
    void	       *&right()        { return Q_RIGHT(this); }
    const void		*right() const  { return Q_RIGHT(this); }

    GQ_Face		*rightFace() const { return static_cast<GQ_Face *>(Q_RIGHT(this)); }

    /// Given the current quad edge a=this and b, splice affects the topology
    /// of the 2 quad edge rings a->org() and b->org() as follows:
    ///
    ///  1. if 2 rings are distinct, they will be combined.
    ///
    ///  2. if 2 rings are exactly the same, it will be broken  into 2 separate
    ///     pieces. The cuts will occur immediately after a and b in
    ///     counterclockwise order.  (Envision a wagon wheel with the spokes as
    ///     the edges and the centre of the wheel as their common origin.
    ///     For spokes (edges) a and b on the wheel, the divisions will occur 
    ///     between spoke a and the edge to its left, and spoke b and the edge
    ///     to its left.  Each of the two resulting pieces now forms a ring.)
    ///
    ///  Note that a->org() and b->org() should be equal for a splice to make
    ///  sense.  This method does not alter a->org() and b->org().
    void		 splice(GQ_Edge *b)
    {
        GQ_Edge *_a	= onext()->rot();
        GQ_Edge *_b	= b->onext()->rot();
        GQ_Edge *aOnext	= b->onext();
        GQ_Edge *bOnext	= onext();
        GQ_Edge *_aOnext= _b->onext();
        GQ_Edge *_bOnext= _a->onext();

        onext()     = aOnext;
        b->onext()  = bOnext;
        _a->onext() = _aOnext;
        _b->onext() = _bOnext;
    }

    /// Make this edge to connect between edge a and b
    /// this->left() == a->left() == b->left()
    void                 connect(GQ_Edge *a, GQ_Edge *b)
    {
        splice(a->lnext());
        sym()->splice(b);
        left() = a->left();
        right() = rnext()->right();
    }

    /// Connect this edge as the next edge of b in polygon
    /// this->left() == b->left()
    void                 connect(GQ_Point *org1, GQ_Edge *b)
    {
        org() = org1;
        splice(b->lnext());
        left() = b->left();
        right() = rnext()->right();
    }
    void		 connect(GQ_Edge *b) { splice(b->lnext()); }
    void                 disconnectOrg(); 

    /// Disconnect completely
    void		 disconnect();

    ///  Swap the connecting edge
    ///
    ///           ---          ---     |
    ///  From      /     TO     \      |
    ///           ---          ---     |
    ///
    ///  For example, if we have two triangles:
    ///        b
    ///     +------+           +------+
    ///     |     /|          b|\     |
    ///     |   _/ |          L| \_   |
    ///     |  e/| |          n| |\e  |aLnext
    ///     |  /   |  -> swap e|   \  |
    ///     | /    |          x|    \ |
    ///     |/ a   |          t|     \|
    ///     +------+           +------+
    ///
    void                 swap(); 
    int                  isBridge() const { return getFlags(GQ_BRIDGE); }
    int        		 isShare()  const { return org() == oprev()->org() &&
					    dest() == lnext()->org(); }
    int 		 isBoundary() const { return !isShare() || (!isBridge() && (!left() || !right())); }
    int 		 intersectPlane(UT_Vector3 &nml, float d, float &t, int donml=1);
    int			 intersect(GQ_Point *pt);
    int			 intersect(GQ_Edge *edge);
    int			 intersect(GQ_Point *org, GQ_Point *dest);
    int			 intersect(GQ_Face *face, float &t);

    void                 setFlags(unsigned mask)        { myFlags = mask; }
    unsigned             getFlags(unsigned mask=~0) const{ return myFlags & mask; }
    void                 addFlags(unsigned mask)        { myFlags |= mask; }
    void                 clearFlags(unsigned mask=~0)   { myFlags &= ~mask; }

    /// Our index into GQ_Detail::myEdges array, also used for cross references
    /// to other data structures which we don't want to duplicate
    int			 index() const			{ return myIndex >> 2; }

    /// Cast operator into GA_Edge
    operator GA_Edge () { return GA_Edge(orgPoint()->ptOff(), destPoint()->ptOff()); }
    operator GA_Edge () const { return GA_Edge(orgPoint()->ptOff(), destPoint()->ptOff()); }

    std::ostream	&save(std::ostream &os) const;
    friend std::ostream	&operator<<(std::ostream &os, const GQ_Edge &e)
			 { e.save(os); return os; }

    // Struct to replace the unnamed struct that the FOR_QUAD_EDGES_STRUCT()
    // should be using.  Unfortunately, Visual Studio does not allow the use
    // of an unnamed struct in the declaration part of the for statement.
    struct GQ_API MacroIterStruct
    {
	MacroIterStruct(GQ_Edge *input_edge, int input_i)
	    : edge(input_edge), i(input_i) {}

	GQ_Edge		*edge;
	int		 i;
    };

private:

    void 		*myData;
    GQ_Edge		*myNext;
    unsigned             myFlags;
    /// Index encodes two things: the low two bits encode which quarter of
    /// the quad-edge this is, and the upper bits define which quad-edge
    /// within the GQ_Detail this is.
    int			 myIndex;
};

#endif
