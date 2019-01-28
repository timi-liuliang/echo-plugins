/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO_PolyInterface.h ( GEO Library, C++)
 *
 * COMMENTS:
 *
 * GEO_PolyInterface provides a way of navigating and manipulating polygon 
 * mesh topologies. It is meant to replace GEO_HedgeInterface which provides 
 * the same function but attempts at maintain half-edge information 
 * transparently through intercepting topological operations performed on the 
 * detail in various ways. This approach proved to provide little advantage for 
 * the cost it pays in efficiency, not to mention the difficulty of maintenance 
 * every time a new topology manipulation method is added somewhere. In 
 * contrast GEO_PolyInterface is explicit and expects the user to use the 
 * provided methods for all topology manipulation between the point where the 
 * interface is constructed until it is destroyed (or hardenPolys() is called).
 *
 * Note that other operations on the detail such as creation, destruction,
 * and manipulation of groups and attributes can be performed in the usual
 * manner without interference with GEO_PolyInterface. Standard topology 
 * attributes can also be read as usual. So, operations such as vertexPoint(),
 * vertexPrimitive(), pointVertex(), vertexToNextVertex() can still be called
 * through the detail although GEO_PolyInterface provides aliases for these 
 * as well. Note also that the recently added methods for accessing primitive
 * vertex lists or flags are also not compatible with GEO_PolyInterface and
 * cannot be used while a GEO_PolyInterface is active on a detail. 
 *
 * Furthermore, note that unlike GEO_HedgeInterface, GEO_PolyInterface 
 * abandons all aspirations of extending half-edges to primitive types other 
 * than polygons. In fact, even open polygons are not supported. 
 *
 */

#ifndef __GEO_PolyIface__
#define __GEO_PolyIface__

#include "GEO_Detail.h"
#include "GEO_Hedge.h"
#include "GEO_PrimPoly.h"

#include <GA/GA_ATITopology.h>
#include <GA/GA_AttributeRefMap.h>
#include <GA/GA_Range.h>

#include <UT/UT_Triangulate.h>

class GEO_PrimPoly;

#define USE_32BIT_LINKS         1
#define VERIFY_WITH_HARD_POLYS  0

#if USE_32BIT_LINKS

#define FLAG_MASK		0x40000000
typedef uint32                  RawLinkType;

#else // Use 64bit links

#define FLAG_MASK		0x4000000000000000
typedef uint64                  RawLinkType;

#endif

SYS_FORCE_INLINE bool
getLinkFlag(RawLinkType i)
{
    return (i & FLAG_MASK);
}

SYS_FORCE_INLINE RawLinkType
setLinkFlag(RawLinkType i)
{
    return (i | FLAG_MASK);
}

SYS_FORCE_INLINE RawLinkType
clearLinkFlag(RawLinkType i)
{
    return (i & (~FLAG_MASK));
}

/// Filtered Ranges are used to make range iterators over usual GA_Ranges
/// with additional filtering conditions.

template <typename FILTER, typename ITERTYPE>
struct FilteredRange
{
    FilteredRange(const FILTER &f, const GA_Range &r) :
	myFilter(f), myRange(r) { }

    struct Iterator
    {
	Iterator(const FILTER &f) : myFilter(f), myIter() { }
	Iterator(const FILTER &f, const GA_Range &r) :
	    myFilter(f), myIter(r)
	{ nextValid(); }

	ITERTYPE	 operator*() const { return ITERTYPE(*myIter); }
	bool		 operator!=(const Iterator &other)
			    { return !myIter.atEnd(); }
	Iterator	&operator++() { ++myIter; nextValid(); return *this; }

	void		 nextValid()
	{
	    while (!myIter.atEnd() && !myFilter(*myIter))
		++myIter;
	}

    private:
	GA_Iterator	 myIter;
	FILTER		 myFilter;
    };

    Iterator		 begin() const { return Iterator(myFilter, myRange); }
    Iterator		 end() const { return Iterator(myFilter); }
private:
    FILTER		 myFilter;
    GA_Range		 myRange;
};

template <typename CIRCULATOR, typename ITERTYPE>
struct Cycle
{
    Cycle(const CIRCULATOR &f, GA_Offset base) :
	myCirculator(f), myBase(base) { }

    struct Iterator
    {
	Iterator(const CIRCULATOR &f, GA_Offset iter) :
	    myCirculator(f), myIter(iter), myBase(GA_INVALID_OFFSET) { }

	ITERTYPE	 operator*() const { return ITERTYPE(myIter); }
	bool		 operator!=(const Iterator &other)
			    { return GAisValid(myIter) && myIter != myBase; }
	Iterator	&operator++()
	{
	    if (!GAisValid(myBase))
		myBase = myIter;
	    myIter = myCirculator(myIter);
	    return *this;
	}

    private:
	GA_Offset	 myIter, myBase;
	CIRCULATOR	 myCirculator;
    };

    Iterator		 begin() const
			    { return Iterator(myCirculator, myBase); }

    Iterator		 end() const
			    { return Iterator(myCirculator, myBase); }
private:
    CIRCULATOR		 myCirculator;
    GA_Offset		 myBase;
};

class GEO_API GEO_PolyInterface
{
public:

                         GEO_PolyInterface(GA_Detail *gdp,
                                const GA_PrimitiveGroup *polys = nullptr);

                        ~GEO_PolyInterface();

    /// hardenPolys converts the soft polygons used by GEO_PolyInterface
    /// into actual GEO_PrimPoly objects. You *MUST* harden your polygons
    /// before trying to access a GEO_PrimPoly object. Of course you can
    /// skip hardening the polygons, if you're working on a temporary
    /// detail which is going to be tossed out. In that case you would
    /// probably use copyPolys to write them into a different detail.
    /// The copied polys will be the usual polygons and will need no
    /// hardening.

    void                 hardenPolys();

    /// Copy current work set polygons into the given detail. The
    /// pt_offset_map_attr point attribute can determine the point offset
    /// in the target detail ino which each given point should be mapped.
    /// A value of -1 indicates that the point must be created. The
    /// attribute is non-const and is over-written at -1 entries with
    /// the offset of the created point in the given detail. If grp
    /// is given, only the polygons in (resp. out of) the given group are
    /// copied, when copy_excluded is false (resp. true).

    GA_Offset		 copyPolys(GA_Detail *gdp,
				GA_Attribute *pt_offset_map_attr = nullptr,
				const GA_PrimitiveGroup *grp = nullptr,
				bool copy_excluded = false);

    /// Add the given point/vertex/primitive attribute or those passing
    /// the given filter to the list of attributes that are automatically
    /// interpolated during operations.

    void 		 wranglePointAttribute(GA_Attribute *attrib);
    void 		 wranglePointAttribute(
	    			const GA_AttributeFilter &filter);

    void 		 wrangleVertexAttribute(GA_Attribute *attrib);
    void 		 wrangleVertexAttribute(
	    			const GA_AttributeFilter &filter);

    void 		 wranglePrimitiveAttribute(GA_Attribute *attrib);
    void 		 wranglePrimitiveAttribute(
	    			const GA_AttributeFilter &filter);

    /// Clear the list of wrangled point/vertex/primitive attributes.

    void		 resetPointWrangling()
    			    { deleteAttribRefMap(myPointAttrRefMap); }

    void		 resetVertexWrangling()
    			    { deleteAttribRefMap(myVertexAttrRefMap); }

    void		 resetPrimitiveWrangling()
    			    { deleteAttribRefMap(myPrimitiveAttrRefMap); }

    /// Temporarily enable or disable all wrangling of point/vertex/primitive
    /// attributes initiated by calling wranglePointAttributes(), etc.

    void		 enablePointWrangling(bool enable)
    			    { myPointWranglingEnabled = enable;}

    void		 enableVertexWrangling(bool enable)
    			    { myVertexWranglingEnabled = enable;}

    void		 enablePrimitiveWrangling(bool enable)
    			    { myPrimitiveWranglingEnabled = enable;}

    /// Return the polygon to which the hedge belongs.
    SYS_FORCE_INLINE
    GA_Primitive	*hedgePrimitive(GEO_Hedge h) const
                            { return geo_hedge::hedgePrimitive(myGdp, h); }

    // Hedge to poly and poly to first hedge
    GA_Offset            hedgePoly(GEO_Hedge h) const
                            { return vertexPrimitive(srcVertex(h)); }

    GEO_Hedge            polyHedge(GA_Offset poly) const
                            { return GEO_Hedge(polyFirst(poly)); }

    // Returns true if vtx is the source vertex of a valid half-edge.
    SYS_FORCE_INLINE
    bool          	 isValidHedgeSrcVertex(GA_Offset vtx) const
    			    { return GAisValid(vtx)
				     && GAisValid(symNext(vtx)); }

    /// Check whether a hedge is valid. A half-edge is valid if it is 
    /// belongs to a closed polygon in the group on which the interface
    /// is constructed or is the result of the manipulation of such hedges. 
    SYS_FORCE_INLINE
    bool 	 	 isValidHedge(GEO_Hedge h) const
    			    { return isValidHedgeSrcVertex(srcVertex(h)); }

    SYS_FORCE_INLINE
    bool 	 	 isValid(GEO_Hedge h) const
    			    { return isValidHedge(h); }

    /// returns true of h is the *primary* hedge in its equivalence class.
    /// Each equivalence class of hedges at any time has a unique primary
    /// hedge. This is used for example to enumerate over all edges in the
    /// detail (as opposed to over all hedges).
    SYS_FORCE_INLINE
    bool 	 	 isPrimary(GEO_Hedge h) const
    			    { return !isSecondary(srcVertex(h)); }

    /// Returns the source vertex of the hedge.
    SYS_FORCE_INLINE
    GA_Offset	 	 srcVertex(GEO_Hedge h) const
                            { return geo_hedge::srcVertex(h); }

    /// Returns the destination vertex of the hedge.
    SYS_FORCE_INLINE
    GA_Offset	 	 dstVertex(GEO_Hedge h) const
                            { return geo_hedge::dstVertex(*this, h); }

    /// Returns the vertex preceding the source of the hedge in its polygon.
    SYS_FORCE_INLINE
    GA_Offset	 	 preSrcVertex(GEO_Hedge h) const
                            { return geo_hedge::preSrcVertex(*this, h); }

    /// Returns the vertex succeeding the destination of the hedge.
    SYS_FORCE_INLINE
    GA_Offset	 	 postDstVertex(GEO_Hedge h) const
                            { return geo_hedge::postDstVertex(*this, h); }

    /// Returns the point to which the source vertex is wired.
    SYS_FORCE_INLINE
    GA_Offset	 	 srcPoint(GEO_Hedge h) const
                            {  return geo_hedge::srcPoint(myGdp, h); }

    /// Returns the point to which the destination vertex is wired.
    SYS_FORCE_INLINE
    GA_Offset	 	 dstPoint(GEO_Hedge h) const
                            { return geo_hedge::dstPoint(*this, h); }

    /// Returns the point to which the preSrcVertex(h) is wired.
    /// @see: preSrcVertex()
    SYS_FORCE_INLINE
    GA_Offset	 	 preSrcPoint(GEO_Hedge h) const
                            { return geo_hedge::preSrcPoint(*this, h); }

    /// Returns the point to which the postDstVertex(h) is wired.
    /// @see: postDstVertex()
    SYS_FORCE_INLINE
    GA_Offset	 	 postDstPoint(GEO_Hedge h) const
                            { return geo_hedge::postDstPoint(*this, h); }

    /// Returns the next hedge (hedge whose source is the destination of the
    /// parameter hedge) in its polygon.

    SYS_FORCE_INLINE
    GEO_Hedge	 	 nextPrimitiveHedge(GEO_Hedge h) const
                            { return geo_hedge::nextPrimitiveHedge(*this, h); }

    /// Quad-Edge/GQ alias for nextPrimitiveHedge().
    SYS_FORCE_INLINE
    GEO_Hedge	 	 lnext(GEO_Hedge h) const
                            { return nextPrimitiveHedge(h); }

    /// Returns the previous hedge (hedge whose destination is the source
    /// of the parameter hedge) in its polygon. 
    SYS_FORCE_INLINE
    GEO_Hedge	 	 prevPrimitiveHedge(GEO_Hedge h) const
                            { return geo_hedge::prevPrimitiveHedge(*this, h); }

    /// Quad-Edge/GQ alias for prevPrimitiveHedge().
    SYS_FORCE_INLINE
    GEO_Hedge	 	 lprev(GEO_Hedge h) const
                            { return prevPrimitiveHedge(h); }

    /// Returns the hedge other than the parameter hedge in its polygon that 
    //  has pt as an endpoint.
    SYS_FORCE_INLINE
    GEO_Hedge	 	 otherPrimitiveHedgeAtPoint(GEO_Hedge h,
                                GA_Offset pt) const;

    /// Returns the "next" equivalent hedge to h:
    /// two hedges are equivalent if either their source and destination points 
    /// are the same or the source point of each is the destination point of 
    /// the other. Calling nextEquivalentHedge() repeatedly returns back to the 
    /// original hedge. Thus to check if hedge h is manifold hedge one can 
    //  check that: 
    //	    nextEquivalentHedge(h) != h 
    //	    && nextEquivalentHedge(nextEquivalentHedge(h)) == h.
    //	Note that this only verifies that there are two distinct hedges in the
    //	class but does not verify that they are oriented oppositely. 
    SYS_FORCE_INLINE
    GEO_Hedge	 	 nextEquivalentHedge(GEO_Hedge h) const
    			    { return GEO_Hedge(symNext(srcVertex(h))); }

    /// Quad-Edge/GQ alias for nextEquivalentHedge().
    SYS_FORCE_INLINE
    GEO_Hedge	 	 sym(GEO_Hedge h) const
                            { return nextEquivalentHedge(h); }

    /// Returns the primary hedge equivalent to the argument hedge.
    SYS_FORCE_INLINE
    GEO_Hedge	 	 primaryEquivalentHedge(GEO_Hedge h) const
    			    { return GEO_Hedge(findPrimary(srcVertex(h))); }

    /// Alias for primaryEquivalentHedge().
    SYS_FORCE_INLINE
    GEO_Hedge	 	 primary(GEO_Hedge h) const
                            { return primaryEquivalentHedge(h); }

    /// Arbitrarily-Ordered Scan Methods:
    /// These methods scan over all edge/hedges incident to a point in an
    /// arbitrary order. NB. This order may be different in different 
    /// executions of the code or across different platforms. 

    /// Returns a first incident hedge to a point. Together with
    /// nextIncidentHedge, one can enumerate circularly over all hedges
    /// incident to a point. A hedge is incident to a point if one of its 
    /// endpoints is that point.
    SYS_FORCE_INLINE
    GEO_Hedge     	 firstIncidentHedge(GA_Offset p) const
                            { return geo_hedge::firstIncidentHedge(*this, p); }

    SYS_FORCE_INLINE
    GEO_Hedge	 	 nextIncidentHedge(GEO_Hedge h, GA_Offset p) const
                            { return geo_hedge::nextIncidentHedge(*this, h, p); }

    /// Similar to first/nextIncidentHedge but stops only at the primary 
    /// hedge in each equivalence class of hedges involved. 
    SYS_FORCE_INLINE
    GEO_Hedge	 	 firstIncidentEdge(GA_Offset p) const
                            { return geo_hedge::firstIncidentEdge(*this, p); }

    SYS_FORCE_INLINE
    GEO_Hedge	 	 nextIncidentEdge(GEO_Hedge h, GA_Offset p) const
                            { return geo_hedge::nextIncidentEdge(*this, h, p); }

    /// Similar to first/nextIncidentHedge but using only outgoing hedges.
    /// Note that nextOutgoingHedge does not need the parameter p anymore.
    SYS_FORCE_INLINE
    GEO_Hedge	 	 firstOutgoingHedge(GA_Offset p) const
                            { return geo_hedge::firstOutgoingHedge(*this, p); }

    SYS_FORCE_INLINE
    GEO_Hedge	 	 nextOutgoingHedge(GEO_Hedge h) const
                            { return geo_hedge::nextOutgoingHedge(*this, h); }

    /// Similar to first/nextIncidentHedge but using only incoming hedges.
    /// Note that nextOutgoingHedge does not need the parameter p anymore.
    SYS_FORCE_INLINE
    GEO_Hedge	 	 firstIncomingHedge(GA_Offset p) const
                            { return geo_hedge::firstIncomingHedge(*this, p); }

    SYS_FORCE_INLINE
    GEO_Hedge	 	 nextIncomingHedge(GEO_Hedge h) const
                            { return geo_hedge::nextIncomingHedge(*this, h); }

    /// Manifold Scan Methods:

    /// A "manifold scan" around a point moves from one incoming (resp outgoing
    /// hedge incident to a point to the next in counterclockwise order as
    /// long as the hedge in question is a manifold hedge (i.h. it is
    /// equivalent to exactly one other hedge oriented oppositely).

    /// Returns the first hedge with the same destination point as the argument
    /// hedge that is reachable from it through a counterclockwise manifold 
    /// scan around the destination point.
    /// Returns the argument hedge itself if either:
    /// 	1) it is already the first such hedge, or
    /// 	2) its source is an interior point of a manifold,
    ///            ie. a counterclockwise scan does a full circle.
    SYS_FORCE_INLINE
    GEO_Hedge	 	 firstManifoldIncomingHedge(GEO_Hedge h) const;


    /// Quad-Edge/GQ alias:
    SYS_FORCE_INLINE
    GEO_Hedge 	 	 dfirst(GEO_Hedge h) const
                            { return firstManifoldIncomingHedge(h); }
    /// Returns the previous hedge with the same dst as h in a
    /// counterclockwise manifold scan, returns GEO_INVALID_HEDGE if no
    /// such hedge exists.
    ///
    /// NOTE: This is equivalent to dprev() operation in Quad-Edge
    /// terminology, which is also used in GQ.
    SYS_FORCE_INLINE
    GEO_Hedge	 	 prevManifoldIncomingHedge(GEO_Hedge h) const;

    /// Quad-Edge/GQ alias:
    SYS_FORCE_INLINE
    GEO_Hedge 	 	 dprev(GEO_Hedge h) const
                            { return prevManifoldIncomingHedge(h); }

    /// Returns the next hedge with the same dst as h in a counterclockwise
    /// manifold scan, returns GEO_INVALID_HEDGE if no such hedge exists.
    ///
    /// NOTE: This is equivalent to dnext() operation in Quad-Edge
    /// terminology, which is also used in GQ.
    SYS_FORCE_INLINE
    GEO_Hedge	 	 nextManifoldIncomingHedge(GEO_Hedge h) const;

    /// Quad-Edge/GQ alias:
    SYS_FORCE_INLINE
    GEO_Hedge 	 	 dnext(GEO_Hedge h) const
                            { return nextManifoldIncomingHedge(h); }

    /// Similar to firstManifoldIncomingHedge but finds the first hedge with
    /// the same src as h in a counterclockwise scan from h.
    SYS_FORCE_INLINE
    GEO_Hedge 	 	 firstManifoldOutgoingHedge(GEO_Hedge h) const;

    /// Quad-Edge/GQ alias:
    SYS_FORCE_INLINE
    GEO_Hedge	  	 ofirst(GEO_Hedge h) const
                            { return firstManifoldOutgoingHedge(h); }

    /// Returns the previous hedge with the same src as h in a counterclockwise
    // manifold scan, returns GEO_INVALID_HEDGE if no such hedge exists.
    ///
    /// NOTE: This is equivalent to oprev() operation in Quad-Edge
    /// terminology, which is also used in GQ or equivalently lnext(sym()).
    SYS_FORCE_INLINE
    GEO_Hedge 	 	 prevManifoldOutgoingHedge(GEO_Hedge h) const;

    /// Alias for prevManifoldOutgoingHedge, equivalent to lnext(sym()).
    SYS_FORCE_INLINE
    GEO_Hedge 	 	 oprev(GEO_Hedge h) const
                            { return prevManifoldOutgoingHedge(h); }

    /// Returns the next hedge with the same src as h in a counterclockwise
    /// manifold scan, returns GEO_INVALID_HEDGE if no such hedge exists.
    ///
    /// NOTE: This is equivalent to onext() operation in Quad-Edge
    /// terminology, which is also used in GQ or equivalently sym(lprev()).
    SYS_FORCE_INLINE
    GEO_Hedge 	 	 nextManifoldOutgoingHedge(GEO_Hedge h) const;

    /// Alias for nextManifoldOutgoingHedge, equivalent to sym(lprev()).
    SYS_FORCE_INLINE
    GEO_Hedge	 	 onext(GEO_Hedge h) const
                            { return nextManifoldOutgoingHedge(h); }

    /// Returns true if the hedge h is a manifold hedge, if accept_bd is
    /// false, this means that the equivalence class of h consists of
    /// exactly two hedges oriented oppositely. If accept_bd is true, then
    /// the equivalence class of h can also consist of a single (boundary)
    /// half-edge.
    SYS_FORCE_INLINE
    bool	         isManifoldHedge(GEO_Hedge h,
                                bool accept_bd = false) const;

    /// Returns true if the hedge h is a boundary hedge, i.e. if its
    /// equivalence class is singleton.
    SYS_FORCE_INLINE
    bool	         isBoundaryHedge(GEO_Hedge h) const
                            { return geo_hedge::isBoundaryHedge(*this, h); }

    /// Returns true if the hedge h is a bridge hedge, i.e. if is a
    /// manifold hedge and its other equivalent hedge belongs to the
    /// same primitive as h does.
    SYS_FORCE_INLINE
    bool	         isBridgeHedge(GEO_Hedge h) const
                            { return geo_hedge::isBridgeHedge(*this, h); }

    /// Returns true if h1 and h2 are equivalent hedges with opposite
    /// orientation.
    SYS_FORCE_INLINE
    bool	         areOpposite(GEO_Hedge h1, GEO_Hedge h2) const
                            { return geo_hedge::areOpposite(*this, h1, h2); }

    /// Returns true if h1 and h2 are equivalent hedges.
    SYS_FORCE_INLINE
    bool	         areEquivalent(GEO_Hedge h1, GEO_Hedge h2) const
                            { return geo_hedge::areEquivalent(*this, h1, h2); }

    /// Returns the number of hedges in the equivalence class of h.
    SYS_FORCE_INLINE
    GA_Size	 	 numEquivalentHedges(GEO_Hedge h) const
                            { return geo_hedge::numEquivalentHedges(*this, h); }

    /// Returns the number if *edges* incident to a point (equivalent hedges
    /// count as 1).
    SYS_FORCE_INLINE
    GA_Size	 	 numIncidentEdges(GA_Offset pt) const
                            { return geo_hedge::numIncidentEdges(*this, pt); }

    /// Returns the number of distinct *hedges* incident to pt
    SYS_FORCE_INLINE
    GA_Size	 	 numIncidentHedges(GA_Offset pt) const
                            { return geo_hedge::numIncidentHedges(*this, pt); }

    /// Find a hedge with the given endpoints or return GEO_INVLAID_HEDGE
    SYS_FORCE_INLINE
    GEO_Hedge	 	 findHedgeWithEndpoints(GA_Offset p0,
                                GA_Offset p1) const;

    /// Rotate h forward, assuming h is an interior manifold hedge (equivalent
    /// to exactly one other hedge f oppositely oriented. This is not checked
    /// and the topology may be corrupted if rotate is applied to non-manifold
    /// or boundary edges. Returns the resulting hedge which happens to be
    /// equal to h (The input hedge will be the hedge that rotates (same src
    /// vertex) and the same holds for the equivalent edge of h.
    ///
    ///    ------>------>          ------>------>
    ///    ^     |^     |          ^          /7|
    ///    |     ||     |          |        //  |
    ///    |   h ||     |   ===>   |   h  //    |
    ///    |     ||     |          |    //      |
    ///    |     v|     v          |  //        v
    ///    <------<------          |L/<---<------
    ///
    /// Note that this operation is precisely an edge flip if the two
    /// primitives involved are triangles.
    ///
    /// Note also that the half-edges associated with the flipped edge are
    /// *NOT* identified with those of any pre-existing edge with the same
    /// endpoints. This deviates from the convention taken by poly interface
    /// but is very useful when pinching of a surface is temporarily needed
    /// as is the case with many algorithms. Method isFlippable() can be used
    /// to determine such a scenario beforehand.
    GEO_Hedge	         rotateForward(GEO_Hedge h);

    /// Complete reverse of rotateForward (all the point, vertex, and
    /// primitive offsets are restored). When the incident primitives are
    /// triangles, rotating forward or backward result in the same edge but
    /// they are not exactly identical. In particular, two consecutive flips
    /// on the same edge will reverse the orientation of the edge.
    GEO_Hedge	         rotateBackward(GEO_Hedge h);

    /// Returns true if h can be flipped without the result overlapping an
    /// existing hedge. In other words, h is flippable if its endpoints are
    /// not endpoints of an existing edge in the geometry before the flip that
    /// is not equivalent to h.
    bool	         isFlippable(GEO_Hedge h) const;

    /// Flip h (provided that h is shared between two triangles) and is
    /// flippable. Returns the resulting (flipped) hedge (equal to h) or
    /// GEO_INVALID_HEDGE if h is not flippable.
    GEO_Hedge	         flip(GEO_Hedge h);

    /// unflip is the reverse of a flip. Doing a flip followed by an unflip
    /// should result in the same mesh with the same vertex/point/primtive
    /// offsets and indices (but the vertices within primitives may shift).
    GEO_Hedge 	         unflip(GEO_Hedge h);

    /// Returns true if the hedge h is contractible, i.e. contracting h does
    /// not force a a non-trivial (separating) cycle to collapse. A separating
    /// cycle is a closed sequence of edges that if removed (together with
    /// their incident geometry elements) the number of connected components
    // increases. Contracting a non-contractible edge will split geometry into
    /// multiple components.
    bool 	         isContractible(GEO_Hedge h) const;

    /// Contracts h.
    /// @parameter on_dst, if true, causes the src point to move to the
    ///			position of the dst point. Otherwise, the dst point
    ///			is moved to the src point's position.
    /// @parameter ratio, if in [0.0 .. 1.0], places the point of contraction
    /// 		in a convex combination with biases ratio and
    ///			1.0 - ratio from src and dst (or the other way around
    /// 		if on_dst is false) respectively.

    // The internal flag distinguishes two different policies on whether
    // the neighbouring triangles to the ones being destroyed as the result of
    // the contraction of the edge inherit any of the half-edges of the latter
    // triangles. If internal is set to true, the half-edge preceding or
    // following each collapsing half-edge (depending on whether on_dst is true
    // or not and whether the half-edge is oriented similarly to h or not) is
    // inherited by the neighbouring triangle at the other half-edge sharing
    // a vertex with the one being collapsed. If internal is false though,
    // all half-edges of the destroyed triangles are destroyed and the others
    // survive and are unified in the expected manner.

    GA_Offset 	         contract(GEO_Hedge h, bool on_dst = true,
                                fpreal ratio = -1.0,
                                bool check_contractible = true,
    				bool internal = true,
    				bool destroy_released_point = true);

    /// Splits the hedge h and its incident polygon (assumed to be a triangle,
    /// although it doesn't fail if it isn't).
    /// @parameter ratio determines the position of the new point at which the
    /// 		hedge is split.
    /// @parameter and_equivalent if true also splits all other hedges
    ///			equivalent to h at the same point.
    /// @parameter pt is an optional point offset assumed to be located in
    //			the correct position (as given by ratio). This allows
    //			the caller to reuse points already created.
    GEO_Hedge	         split(GEO_Hedge h, fpreal ratio,
                                bool and_equivalent = true,
             	                GA_Offset pt = GA_INVALID_OFFSET);

    /// Inserts a point in the middle of a hedge and divides into two hedges.
    ///
    /// @parameter ratio determines the position of the new point at which the
    /// 		hedge is split.
    /// @parameter and_equivalent if true also divides all other hedges
    ///			equivalent to h at the same point.
    /// @parameter pt is an optional point offset assumed to be located in
    //			the correct position (as given by ratio). This allows
    //			the caller to reuse points already created.
    GEO_Hedge	         divide(GEO_Hedge h, fpreal ratio,
             	                bool and_equivalent = true,
             	                GA_Offset pt = GA_INVALID_OFFSET);

    /// Break a polygon into two polygons by dividing it along a diagonal.
    /// The hedges starting from h_keep and ending before h_drop are kept in
    /// the current polygon while the hedges starting at h_drop and ending
    /// before h_keep end up in the new polygon. The returned hedge will be
    /// one added to the original polygon corresponding to the cut diagonal.
    /// NB. h_keep and h_drop *must* belong to the same polygon and cannot
    /// be the same or consecutive hedges.
    GEO_Hedge 	         divide(GA_Offset poly, GEO_Hedge h_keep,
				GEO_Hedge h_drop);

    /// Split a polygon at its barycenter, or a given point. If the point
    /// is not given, then the position of the created point is set to the
    /// barycenter of the polygon.
    GA_Offset 	         split(GA_Offset poly,
			    	GA_Offset pt = GA_INVALID_OFFSET);

    /// Triangualtes a polygon.

    template <typename T = fpreal32>
    exint 		 triangulate(GA_Offset poly,
				UT_Vector3T<T> *sym_ctr = nullptr,
				GA_ROHandleT< UT_Vector3T<T> > pos_handle
					= GA_ROHandleT< UT_Vector3T<T> >(),
				GEO_Hedge start_hedge = GEO_INVALID_HEDGE,
				UT_Array< UT_Vector3T<T> > *pos3 = nullptr,
				UT_Array< UT_Vector2T<T> > *pos2 = nullptr,
    				UT_Triangulate::WorkBuffer *wb = nullptr,
				UT_IntArray *tris = nullptr);

    template <typename S, typename T = fpreal32>
    exint		 triangulate(const S &&polys,
				UT_Vector3T<T> *sym_ctr = nullptr,
				GA_ROHandleT< UT_Vector3T<T> > pos_handle
					= GA_ROHandleT< UT_Vector3T<T> >());

    // Deletes the *star* of a point. The star of a point in a polygonal
    // complex consists of all edges and polygons incident to that point.
    // So, deleting the star of an internal point on a manifold should leave
    // a hole behind. If the cap_link is true, the hole is capped
    // (if possible) with a new polygon. For this be possible the point *must*
    // be an internal manifold point. If capping is possible then one of
    // the polygons previously incident to pt can be specified as cap_poly
    // to extend and become the patching polygon. If this polygon is not
    // specified or is not actually incident to pt, then a new polygon will
    // be created. The point pt itself is destroyed if destroy_pt is true.

    void	         deletePointStar(GA_Offset pt, bool cap_hole = true,
        	                GA_Offset cap_poly = GA_INVALID_OFFSET,
				bool destroy_pt = true);


    // Dissolve the edge represented by h. The edge to be dissolved must be
    // an interior manifold edge with consistent winding in the two polygons
    // sharing it. Return value is true if h is dissovled and false if
    // the dissolving has failed.
    bool		 dissolve(GEO_Hedge h);

    void	         wireVertexPoint(GA_Offset vtx, GA_Offset pt,
				bool rewire_hedges = true);

    /// Basic polygon manipulation methods. You should use these instead of
    /// those offered by GEO_PrimPoly while dealing with softened polys.

    GA_Offset            appendPoly();

    SYS_FORCE_INLINE
    bool          	 isValidPoly(GA_Offset poly) const
    			    { return GAisValid(poly)
				     && GAisValid(polyFirst(poly)); }

    void		 destroyPoly(GA_Offset poly, bool and_pts = false);

    GA_Offset            appendVertex(GA_Offset poly, GA_Offset pt,
                                bool relink_hedges = true);

    GA_Offset            insertVertex(GA_Offset poly, GA_Offset pt,
                                GA_Offset before = GA_INVALID_OFFSET,
                                bool relink_hedges = true);

    GA_Offset            stealVertex(GA_Offset poly, GA_Offset vtx,
                                GA_Offset insert_before = GA_INVALID_OFFSET,
                                bool relink_hedges = true);

    int                  deleteVertex(GA_Offset vtx, bool relink_hedges = true);

    int			 vertexCount(GA_Offset poly) const;

    GA_Detail 	        *getDetail() const { return myGdp; }

    bool		 softValidate() const;
    bool                 validate() const;

    SYS_FORCE_INLINE
    GA_Offset     	 prevPrimVertex(GA_Offset v) const;

    SYS_FORCE_INLINE
    GA_Offset     	 nextPrimVertex(GA_Offset v) const;

    SYS_FORCE_INLINE
    void          	 getAdjacentPrimVertices(GA_Offset v, GA_Offset &vprev,
                                GA_Offset &vnext) const;

    void                 dumpHedge(GA_Offset vtx);
    void                 dumpHedge(GEO_Hedge h)
                            { dumpHedge(srcVertex(h)); }

    // For a given polygon offset controlled by this interface, polyFirst()
    // returns the first vertex in the circular list of vertices of that
    // polygon. If a polygon is not modified by the interface this will be
    // the first vertex of the input polygon. Also, when the polygon is
    // hardened, this will be its first vertex.

    SYS_FORCE_INLINE
    GA_Offset	 	 polyFirst(GA_Offset poly) const;

    // Given a vertex of a polygon, returns the next vertex in its
    // circular list of vertices.
    SYS_FORCE_INLINE
    GA_Offset	 	 polyNext(GA_Offset v) const
    			    { return GA_Offset(myNextPolyHedge->getLink(v)); }

    // Given a vertex of a polygon, returns the previous vertex in its
    // circular list of vertices.
    SYS_FORCE_INLINE
    GA_Offset	 	 polyPrev(GA_Offset v) const
    			    { return GA_Offset(myPrevPolyHedge->getLink(v)); }

    UT_Vector3		 calcBarycenter(GA_Offset poly) const;

    struct PolyFilter
    {
	explicit 	 PolyFilter(const GEO_PolyInterface &pi) : myPI(pi) { }
	bool		 operator()(GA_Offset off) const
			    { return myPI.isValidPoly(off); }

    private:
	const GEO_PolyInterface	&myPI;
    };
    typedef FilteredRange<PolyFilter, GA_Offset> PolyRange;

    struct HedgeFilter
    {
	explicit 	 HedgeFilter(const GEO_PolyInterface &pi) :
				myPI(pi) { }

	bool		 operator()(GA_Offset off) const
			    { return myPI.isValidHedge(GEO_Hedge(off)); }

    private:
	const GEO_PolyInterface	&myPI;
    };
    typedef FilteredRange<HedgeFilter, GEO_Hedge> HedgeRange;

    struct PrimaryHedgeFilter
    {
	explicit 	 PrimaryHedgeFilter(const GEO_PolyInterface &pi) :
			    	myPI(pi) { }

	bool		 operator()(GA_Offset off) const
			    { return myPI.isValidHedge(GEO_Hedge(off))
				&& myPI.isPrimary(GEO_Hedge(off)); }
    private:
	const GEO_PolyInterface	&myPI;
    };
    typedef FilteredRange<PrimaryHedgeFilter, GEO_Hedge> PrimaryHedgeRange;

    PolyRange		 polys(const GA_PrimitiveGroup *g = nullptr,
			    	bool complement = false) const;
    HedgeRange		 hedges(const GA_VertexGroup *g = nullptr) const;
    PrimaryHedgeRange	 primaryHedges(const GA_VertexGroup *g = nullptr) const;

    struct PolyCirculator
    {
	explicit 	 PolyCirculator(const GEO_PolyInterface &pi) :
				myPI(pi) { }

	GA_Offset	 operator()(GA_Offset off) const
			    { return myPI.polyNext(off); }
    private:
	const GEO_PolyInterface	&myPI;
    };

    typedef Cycle<PolyCirculator, GEO_Hedge> PolyCycle;

    /// Returns the hedges around a polygon as a PolyCycle.
    PolyCycle		 polyHedges(GA_Offset poly) const
			    { return PolyCycle(PolyCirculator(*this),
					       polyFirst(poly)); }

private:

    /// Get or set previous or next hedge in the circular list of hedges
    /// equivalent (sym) to the one with source v.
    ///
    /// The equivalence class of a hedge consists of all hedges whose two
    /// endpoints coincide with those of the hedge in question (although they
    /// may be reversely paired). By *endpoints* of a hedge we mean the two
    /// points into which the tail and head vertex of a hedge are respectively
    /// wired.
    /// @note Starting with any valid hedge, repeated calls to prevSymHedge
    /// or enxtSym traverses all hedges to the original one and
    /// ultimately wraps around.

    SYS_FORCE_INLINE
    void	         symPrev(GA_Offset v, GA_Offset w);

    SYS_FORCE_INLINE
    GA_Offset	 	 symPrev(GA_Offset v) const
    			    { return myPrevSymHedge->getLink(v); }

    SYS_FORCE_INLINE
    void	         symNext(GA_Offset v, GA_Offset w);

    SYS_FORCE_INLINE
    GA_Offset	 	 symNext(GA_Offset v) const;


    /// Form a singleton half-edge equivalence class over src_vtx. Clearly,
    /// the formed half-edge will be primary.
    /// NB. This should be the ONLY way of creating a valid half-edge outside
    /// of the buildHedgeTopology().

    SYS_FORCE_INLINE
    void          	 symLoop(GA_Offset src_vtx);


    SYS_FORCE_INLINE
    bool          	 isPrimary(GA_Offset v) const
                            { return !isSecondary(v); }
    SYS_FORCE_INLINE
    bool          	 isSecondary(GA_Offset v) const;

    SYS_FORCE_INLINE
    void          	 makePrimary(GA_Offset v);

    SYS_FORCE_INLINE
    void          	 makeSecondary(GA_Offset v);

    /// returns the primary hedge in the equivalence class of a given hedge
    GA_Offset 	         findPrimary(GA_Offset vtx) const;

    /// Methods for finding hedges along the boundary of a given polygon.
    SYS_FORCE_INLINE
    void	         polyFirst(GA_Offset poly, GA_Offset v);


    SYS_FORCE_INLINE
    void	         polyPrev(GA_Offset v, GA_Offset w)
                            { myPrevPolyHedge->setLink(v, w); }

    SYS_FORCE_INLINE
    void	         polyNext(GA_Offset v, GA_Offset w)
                            { myNextPolyHedge->setLink(v, w); }

    SYS_FORCE_INLINE
    void          	 polyLoop(GA_Offset src_vtx);

    SYS_FORCE_INLINE
    void          	 getAdjacentBoundaryVertices(GA_Offset vtx,
                                GA_Offset &prev_vtx,
                                GA_Offset &next_vtx) const;

    SYS_FORCE_INLINE
    bool          	 isHard(GA_Offset v) const
                            { return !isSoft(v); }
    SYS_FORCE_INLINE
    bool          	 isSoft(GA_Offset poly) const;

    SYS_FORCE_INLINE
    void          	 makeHard(GA_Offset poly);

    SYS_FORCE_INLINE
    void          	 makeSoft(GA_Offset poly);

    SYS_FORCE_INLINE
    GA_Offset     	 vertexPoint(GA_Offset vtx) const
                            { return myGdp->vertexPoint(vtx); }

    SYS_FORCE_INLINE
    void          	 vertexPoint(GA_Offset vtx, GA_Offset pt)
                            { myTopology.wireVertexPoint(vtx, pt); }

    SYS_FORCE_INLINE
    GA_Offset	 	 vertexToNextVertex(GA_Offset vtx) const
			    { return myGdp->vertexToNextVertex(vtx); }

    SYS_FORCE_INLINE
    GA_Offset     	 pointVertex(GA_Offset vtx) const
                            { return myGdp->pointVertex(vtx); }

    SYS_FORCE_INLINE
    GA_Offset     	 vertexPrimitive(GA_Offset vtx) const
                            { return myGdp->vertexPrimitive(vtx); }

    SYS_FORCE_INLINE
    void          	 vertexPrimitive(GA_Offset vtx, GA_Offset p)
                            { myTopology.wireVertexPrimitive(vtx, p); }

    void	         symUnlink(GA_Offset tailv);

    void	         symLink(GA_Offset src_vtx,
                                 GA_Offset into_vtx = GA_INVALID_OFFSET);

    GA_Offset            symFind(GA_Offset src_vtx, GA_Offset dst_vtx);

    /// This should remain the only possible way of assigning GA_INVALID_OFFSET
    /// to a specific hedge sym next, effectively rendering it invalid. That's
    /// aside from the default attrbute value being the same.

    SYS_FORCE_INLINE
    void	         invalidateHedge(GA_Offset src_vtx)
    			    {
				myNextSymHedge->setLink(src_vtx,
							GA_INVALID_OFFSET); }

    void	         symRelink(GA_Offset src_vtx,
                                GA_Offset into_src_vtx = GA_INVALID_OFFSET);

    void	         symRelinkJoint(GA_Offset vtx, GA_Offset prev_into,
                                        GA_Offset vtx_into);

    void	         symRelinkJoint(GA_Offset vtx);
    void 	         wireVertexPointAndRelinkHedges(GA_Offset vtx,
                                GA_Offset pt, GA_Offset prev_into_src_vtx,
                                GA_Offset next_into_src_vtx);

    // Deprecated, kept for a couple of unported methods.
    SYS_FORCE_INLINE
    GEO_PrimPoly	*hedgePrimPoly(GEO_Hedge h) const;

    const GEO_PrimPoly  *getPrimPoly(GA_Offset primoff) const;
    GEO_PrimPoly	*getPrimPoly(GA_Offset primoff);

    typedef std::pair<GA_Offset, GA_Offset>     OffsetPair;
    typedef UT_Array<OffsetPair>                OffsetPairArray;

    void		 buildHedgeTopology(const GA_PrimitiveGroup *polys);

    void                 linkPointIncidentHedges(GA_Offset pt,
                                OffsetPairArray &pairs_local,
				const GA_PrimitiveGroup *polys,
				UT_IntArray &index_cache);

    void                 polyLink(GA_Offset vtx, GA_Offset prev,
                                GA_Offset next);

    void                 polyUnlink(GA_Offset vtx,
                                bool release_from_poly = true);

    bool                 verifyPoly(GA_Offset poly) const;

    SYS_FORCE_INLINE
    void		 copyVtxAttribs(GA_Offset dst, GA_Offset src);

    SYS_FORCE_INLINE
    void		 lerpVtxAttribs(GA_Offset dst, GA_Offset src0,
				GA_Offset src1, fpreal bias);
    SYS_FORCE_INLINE
    void		 copyPtAttribs(GA_Offset dst, GA_Offset src);

    SYS_FORCE_INLINE
    void		 lerpPtAttribs(GA_Offset dst, GA_Offset src0,
				GA_Offset src1, fpreal bias);
    SYS_FORCE_INLINE
    void		 copyPrimAttribs(GA_Offset dst, GA_Offset src);

    SYS_FORCE_INLINE
    void		 lerpPrimAttribs(GA_Offset dst, GA_Offset src0,
				GA_Offset src1, fpreal bias);

    GEO_Hedge		 splitHedgeSrc(GEO_Hedge h, GA_Offset pt,
				bool local_split_edge_link,
				GA_Offset link0 = GA_INVALID_OFFSET,
				GA_Offset link1 = GA_INVALID_OFFSET,
    				fpreal lerp_ratio = 0.5);

    GEO_Hedge		 splitHedgeDst(GEO_Hedge h, GA_Offset pt,
				bool local_split_edge_link,
				GA_Offset link0 = GA_INVALID_OFFSET,
				GA_Offset link1 = GA_INVALID_OFFSET,
    				fpreal lerp_ratio = 0.5);


    void		 deleteAttribRefMap(GA_AttributeRefMap *&r)
    			    { delete r; r = nullptr; }

    bool		 contractHedgeSrc(GEO_Hedge h);
    bool		 contractHedgeDst(GEO_Hedge h);


    SYS_FORCE_INLINE
    bool		 compareOrderInPoly(GA_Offset v0, GA_Offset v1,
				UT_IntArray &index_cache);

    GA_Detail 		*myGdp;

    GA_ATITopology      *myPrevSymHedge;
    GA_ATITopology      *myNextSymHedge;
    GA_ATITopology      *myPrevPolyHedge;
    GA_ATITopology      *myNextPolyHedge;
    GA_ATITopology      *myFirstPolyHedge;

    GA_Topology 	&myTopology;

    GA_AttributeRefMap	*myPointAttrRefMap;
    GA_AttributeRefMap	*myVertexAttrRefMap;
    GA_AttributeRefMap	*myPrimitiveAttrRefMap;

    GA_OffsetArray	 myTempOffsetArray1;
    GA_OffsetArray	 myTempOffsetArray2;

    bool		 myPointWranglingEnabled;
    bool		 myVertexWranglingEnabled;
    bool		 myPrimitiveWranglingEnabled;
};

void
GEO_PolyInterface::symPrev(GA_Offset v, GA_Offset w)
{
    if (!GAisValid(w))
        myPrevSymHedge->setLink(v, GA_INVALID_OFFSET);
    else
        myPrevSymHedge->setLink(v, w);
}

void
GEO_PolyInterface::symNext(GA_Offset v, GA_Offset w)
{
    UT_ASSERT_P(isValidHedgeSrcVertex(v));
    UT_ASSERT_P(isValidHedgeSrcVertex(w));

    bool secondary = isSecondary(v);
    auto raw = RawLinkType(w);
    if (secondary)
	raw = setLinkFlag(raw);
    myNextSymHedge->setLink(v, GA_Offset(raw));
}

GA_Offset
GEO_PolyInterface::symNext(GA_Offset v) const
{
    GA_Offset off = myNextSymHedge->getLink(v);
    if (!GAisValid(off))
	return GA_INVALID_OFFSET;

    // Clear the secondary bit if set!
    return GA_Offset(clearLinkFlag(RawLinkType(off)));
}

void
GEO_PolyInterface::polyFirst(GA_Offset poly, GA_Offset v)
{
    if (!GAisValid(v))
        myFirstPolyHedge->setLink(poly, GA_INVALID_OFFSET);
    else
    {
        // Preserve the soft bit!
        bool soft = isSoft(poly);
        auto raw = RawLinkType(v);
        if (soft)
            raw = setLinkFlag(raw);
        myFirstPolyHedge->setLink(poly, GA_Offset(raw));
    }
}

GA_Offset
GEO_PolyInterface::polyFirst(GA_Offset poly) const
{
    GA_Offset v = myFirstPolyHedge->getLink(poly);
    if (!GAisValid(v))
	return v;

    // Clear the soft bit if set!
    return GA_Offset(clearLinkFlag(RawLinkType(v)));
}

bool
GEO_PolyInterface::isSecondary(GA_Offset v) const
{
    return getLinkFlag(RawLinkType(myNextSymHedge->getLink(v)));
}

void
GEO_PolyInterface::makeSecondary(GA_Offset v)
{
    auto raw = RawLinkType(myNextSymHedge->getLink(v));
    myNextSymHedge->setLink(v, GA_Offset(setLinkFlag(raw)));
}

void
GEO_PolyInterface::makePrimary(GA_Offset v)
{
    auto raw = RawLinkType(myNextSymHedge->getLink(v));
    myNextSymHedge->setLink(v, GA_Offset(clearLinkFlag(raw)));
}

bool
GEO_PolyInterface::isSoft(GA_Offset poly) const
{
    return getLinkFlag(RawLinkType(myFirstPolyHedge->getLink(poly)));
}

void
GEO_PolyInterface::makeSoft(GA_Offset poly)
{
    auto raw = RawLinkType(myFirstPolyHedge->getLink(poly));
    myFirstPolyHedge->setLink(poly, GA_Offset(setLinkFlag(raw)));
}

void
GEO_PolyInterface::makeHard(GA_Offset poly)
{
    auto raw = RawLinkType(myFirstPolyHedge->getLink(poly));
    myFirstPolyHedge->setLink(poly, GA_Offset(clearLinkFlag(raw)));
}

void
GEO_PolyInterface::symLoop(GA_Offset src_vtx)
{
    myNextSymHedge->setLink(src_vtx, src_vtx);
    myPrevSymHedge->setLink(src_vtx, src_vtx);

    // NB. Semantically, symLoop() must make src_vtx primary. Since our
    // implementation of making primary works by removing the secondary
    // bit which in this case has never been set anyway, we skip this.
    //
    // makePrimary(src_vtx);
}

void
GEO_PolyInterface::polyLoop(GA_Offset src_vtx)
{
    polyPrev(src_vtx, src_vtx);
    polyNext(src_vtx, src_vtx);
}

GA_Offset
GEO_PolyInterface::prevPrimVertex(GA_Offset v) const
{
    return polyPrev(v);
}

GA_Offset
GEO_PolyInterface::nextPrimVertex(GA_Offset v) const
{
    return polyNext(v);
}

void
GEO_PolyInterface::getAdjacentPrimVertices(GA_Offset v, GA_Offset &vprev,
                                       GA_Offset &vnext) const
{
    // TODO: we probably should get rid of this one.
    vprev = polyPrev(v);
    vnext = polyNext(v);
}

GEO_Hedge
GEO_PolyInterface::firstManifoldIncomingHedge(GEO_Hedge h) const
{
    return geo_hedge::firstManifoldIncomingHedge(*this, h);
}

GEO_Hedge
GEO_PolyInterface::prevManifoldIncomingHedge(GEO_Hedge h) const
{
    return geo_hedge::prevManifoldIncomingHedge(*this, h);
}

GEO_Hedge
GEO_PolyInterface::nextManifoldIncomingHedge(GEO_Hedge h) const
{
    return geo_hedge::nextManifoldIncomingHedge(*this, h);
}

GEO_Hedge
GEO_PolyInterface::firstManifoldOutgoingHedge(GEO_Hedge h) const
{
    return geo_hedge::firstManifoldOutgoingHedge(*this, h);
}

GEO_Hedge
GEO_PolyInterface::nextManifoldOutgoingHedge(GEO_Hedge h) const
{
    return geo_hedge::nextManifoldOutgoingHedge(*this, h);
}

GEO_Hedge
GEO_PolyInterface::prevManifoldOutgoingHedge(GEO_Hedge h) const
{
    return geo_hedge::prevManifoldOutgoingHedge(*this, h);
}

GEO_Hedge
GEO_PolyInterface::findHedgeWithEndpoints(GA_Offset p0, GA_Offset p1) const
{
    return geo_hedge::findHedgeWithEndpoints(*this, p0, p1);
}

bool
GEO_PolyInterface::isManifoldHedge(GEO_Hedge h, bool accept_bd) const
{
    return geo_hedge::isManifoldHedge(*this, h, accept_bd);
}

GEO_Hedge
GEO_PolyInterface::otherPrimitiveHedgeAtPoint(GEO_Hedge h, GA_Offset pt) const
{
    return geo_hedge::otherPrimitiveHedgeAtPoint(*this, h, pt);
}

GEO_PrimPoly *
GEO_PolyInterface::hedgePrimPoly(GEO_Hedge h) const
{
    GA_Offset primoff = myGdp->vertexPrimitive(srcVertex(h));
    return static_cast<GEO_PrimPoly *>(myGdp->getPrimitive(primoff));
}

void
GEO_PolyInterface::copyVtxAttribs(GA_Offset dst, GA_Offset src)
{
    if (myVertexAttrRefMap && myVertexWranglingEnabled)
	myVertexAttrRefMap->copyValue(GA_ATTRIB_VERTEX, dst,
				      GA_ATTRIB_VERTEX, src);
}

void
GEO_PolyInterface::lerpVtxAttribs(GA_Offset dst, GA_Offset src0,
				  GA_Offset src1, fpreal b)
{
    if (myVertexAttrRefMap && myVertexWranglingEnabled)
	myVertexAttrRefMap->lerpValue(GA_ATTRIB_VERTEX, dst,
				      GA_ATTRIB_VERTEX, src0, src1, b);
}

void
GEO_PolyInterface::copyPtAttribs(GA_Offset dst, GA_Offset src)
{
    if (myPointAttrRefMap && myPointWranglingEnabled)
	myPointAttrRefMap->copyValue(GA_ATTRIB_POINT, dst,
				     GA_ATTRIB_POINT, src);
}

void
GEO_PolyInterface::lerpPtAttribs(GA_Offset dst, GA_Offset src0,
				 GA_Offset src1, fpreal b)
{
    if (myPointAttrRefMap && myPointWranglingEnabled)
	myPointAttrRefMap->lerpValue(GA_ATTRIB_POINT, dst,
				     GA_ATTRIB_POINT, src0, src1, b);
}

void
GEO_PolyInterface::copyPrimAttribs(GA_Offset dst, GA_Offset src)
{
    if (myPrimitiveAttrRefMap && myPrimitiveWranglingEnabled)
	myPrimitiveAttrRefMap->copyValue(GA_ATTRIB_PRIMITIVE, dst,
					 GA_ATTRIB_PRIMITIVE, src);
}

void
GEO_PolyInterface::lerpPrimAttribs(GA_Offset dst, GA_Offset src0,
				   GA_Offset src1, fpreal b)
{
    if (myPrimitiveAttrRefMap && myPrimitiveWranglingEnabled)
	myPrimitiveAttrRefMap->lerpValue(GA_ATTRIB_PRIMITIVE, dst,
					 GA_ATTRIB_PRIMITIVE, src0, src1, b);
}

bool
GEO_PolyInterface::compareOrderInPoly(GA_Offset v0, GA_Offset v1,
				      UT_IntArray &index_cache)
{
    if (v0 == v1)
	return false;

    if (index_cache(v0) < 0 || index_cache(v1) < 0)
    {
	GA_Offset poly = vertexPrimitive(v0);
	GA_Offset v_first = polyFirst(poly);
	if (!GAisValid(v_first))
	    return false;

	int i = 0;
	GA_Offset v = v_first;
	do
	{
	    index_cache(v) = i++;
	    v = polyNext(v);
	} while (v != v_first);
    }

    UT_ASSERT_P(index_cache(v0) >= 0 && index_cache(v1) >= 0);
    return index_cache(v0) < index_cache(v1);
}


#endif
