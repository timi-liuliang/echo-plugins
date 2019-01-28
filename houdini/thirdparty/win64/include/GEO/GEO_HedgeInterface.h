/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO_HedgeInterface.h ( GEO Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GEO_HedgeInterface__
#define __GEO_HedgeInterface__

#include "GEO_Detail.h"
#include "GEO_Hedge.h"
#include <GA/GA_Primitive.h>

class GEO_PrimPoly;
class GA_ElementWranglerCache;

///
//  GEO_DetachedHedgeInterface is a READ-ONLY half-edge interface and is
//  built with the assumption that the detail on which it is based stays
//  constant while the interface is in use. It only features those methods
//  of GEO_HedgeInterface that do not modify the geometry. Compared to
//  GEO_HedgeInterface, it has the following advantages:
//
// 	1. It does not create additional topology attributes on the detail.
//	2. It stores primitive vertex order information internally and does
//	   not access primitives (after construction) and is therefore faster
//	   than GEO_HedgeInterface in about every operation it supports.
//	3. It can be constructed for a primitive group, as though other
//	   primitives do not exist.
//
//  It is therefore a much much better idea to use detached interface to
//  access a constant detail.

class GEO_API GEO_DetachedHedgeInterface
{
public:

    /// If group prims is specified, all primitives in the group are ignored,
    /// if exclude_prims is ture. Otherwise, all primitives outside the group
    /// are ignore. In both cases, half-edges defined by vertices of the
    /// ignored primitives are regarded as non-existent (invalid).
    GEO_DetachedHedgeInterface(const GA_Detail *gdp,
                               const GA_PrimitiveGroup *prims = NULL,
			       bool exclude_prims = true) :
	myGdp(gdp)
    {
	buildHedgeTopologyLinks(prims, exclude_prims);
    }

    ~GEO_DetachedHedgeInterface() {}

    /// Return the polygon to which the hedge belongs.
    /// NOTE: may need to modify this if other primitives support hedges.
    const GEO_Primitive	*hedgePrimitive(GEO_Hedge e) const
    { return static_cast<const GEO_Primitive *>(geo_hedge::hedgePrimitive(myGdp, e)); }

    GA_Offset		 hedgePrimitiveOffset(GEO_Hedge e) const
    { return geo_hedge::hedgePrimitiveOffset(myGdp, e); }

    /// Check whether a hedge is valid: a hedge is valid if
    ///	  1: its src vertex belongs to a primitive that supports hedges, and
    ///   2: the primitive returns a valid *next boundary vertex* for its
    ///      dst vertex.
    /// @see: GA_Topology.h
    bool	 isValidHedge(GEO_Hedge e) const;

    /// returns true of e is the *primary* hedge in its equivalence class.
    /// Each equivalnce class of hedges at any time has a unique primary hedge.
    /// This is used for example to enumerate over all edges in the detail (as
    /// opposed to over all hedges).
    bool	 isPrimary(GEO_Hedge e) const;

    /// Returns the src vertex of the hedge
    GA_Offset	 srcVertex(GEO_Hedge e) const
    { return geo_hedge::srcVertex(e); }

    /// Returns the src vertex of a signed hedge
    GA_Offset	 srcVertex(GEO_SHedge se) const
    { return se.isPositive() ? srcVertex(se.hedge()) : dstVertex(se.hedge()); }

    /// Returns the dst vertex of the hedge
    GA_Offset	 dstVertex(GEO_Hedge e) const
    { return myPrimNextRef(geo_hedge::srcVertex(e)); }

    /// Returns the src vertex of a signed hedge
    GA_Offset	 dstVertex(GEO_SHedge se) const
    { return se.isPositive() ? dstVertex(se.hedge()) : srcVertex(se.hedge()); }

    /// Returns the vertex before the src vertex of the hedge in the poly
    /// to which the hedge belongs
    GA_Offset	 preSrcVertex(GEO_Hedge e) const
    { return myPrimPrevRef(geo_hedge::srcVertex(e)); }

    /// Returns the vertex after the dst of the vertex of the hedge in the
    /// poly to which the hedge belongs
    GA_Offset	 postDstVertex(GEO_Hedge e) const
    {
	GA_Offset next = myPrimNextRef(geo_hedge::srcVertex(e));
	return GAisValid(next) ? myPrimNextRef(next) : GA_INVALID_OFFSET;
    }

    /// Returns the point to which the src vertex is wired
    GA_Offset	 srcPoint(GEO_Hedge e) const
    {  return myGdp->vertexPoint(srcVertex(e)); }

    /// Returns the src vertex of a signed hedge
    GA_Offset	 srcPoint(GEO_SHedge se) const
    { return se.isPositive() ? srcPoint(se.hedge()) : dstPoint(se.hedge()); }

    /// Returns the point to which the dst vertex is wired
    GA_Offset	 dstPoint(GEO_Hedge e) const
    { return myGdp->vertexPoint(dstVertex(e)); }

    /// Returns the src vertex of a signed hedge
    GA_Offset	 dstPoint(GEO_SHedge se) const
    { return se.isPositive() ? dstPoint(se.hedge()) : srcPoint(se.hedge()); }

    /// Returns the point to which the pre-src vertex is wired.
    /// @see: preSrcVertex()
    GA_Offset	 preSrcPoint(GEO_Hedge e) const
    { return myGdp->vertexPoint(preSrcVertex(e)); }

    /// Returns the point to which the post-dst vertex is wired
    /// @see: postDstVertex()
    GA_Offset	 postDstPoint(GEO_Hedge e) const
    { return myGdp->vertexPoint(postDstVertex(e)); }

    /// Returns the next hedge (hedge with src equal to the dst of the
    /// parameter hedge) in the polygon that contains the parameter hedge
    GEO_Hedge	 nextPrimitiveHedge(GEO_Hedge e) const
    { return GEO_Hedge(dstVertex(e)); }

    SYS_FORCE_INLINE
    GEO_Hedge	 lnext(GEO_Hedge e) const
    { return nextPrimitiveHedge(e); }

    /// Returns the previous hedge (hedge whose dst is the same as the src of
    /// the parameter hedge) in the polygon that contains the parameter hedge.
    GEO_Hedge	 prevPrimitiveHedge(GEO_Hedge e) const
    { return GEO_Hedge(preSrcVertex(e)); }

    SYS_FORCE_INLINE
    GEO_Hedge	 lprev(GEO_Hedge e) const
    { return prevPrimitiveHedge(e); }

    /// Returns the "other" (than e) hedge in the polygon that has the pt as an
    /// endpoint.
    GEO_Hedge	 otherPrimitiveHedgeAtPoint(GEO_Hedge e, GA_Offset pt) const
    {
	if (srcPoint(e) == pt)
	    return prevPrimitiveHedge(e);
	if (dstPoint(e) == pt)
	    return nextPrimitiveHedge(e);
	return GEO_INVALID_HEDGE;
    }

    /// Returns the "next" equivalent hedge to e:
    /// two hedges are equivalent if either their dsts and srcs are
    /// respectively wired to the same points or if the dst of each is wired
    /// to the same point as the src of the other. calling
    /// nextEquivalentHedge() repeatedly returns back to the original hedge.
    /// Thus to check if hedge e is manifold hedge one can check that:
    /// nextEquivalentHedge(e) != e &&
    /// 	nextEquivalentHedge(nextEquivalentHedge(e)) == e
    GEO_Hedge nextEquivalentHedge(GEO_Hedge e) const;

    SYS_FORCE_INLINE
    GEO_Hedge	 sym(GEO_Hedge e) const
    { return nextEquivalentHedge(e); }

    /// Returns a first incidentHedge to a point. Together with
    /// nextIncidentHedge, one can enumerate circularly over all hedges
    /// incident to a point.
    GEO_Hedge	 firstIncidentHedge(GA_Offset pt) const;
    GEO_Hedge	 nextIncidentHedge(GEO_Hedge e, GA_Offset pt) const;

    /// Similar to first/nextIncidentHedge but stops over only one of the
    /// hedges (the primary) in each equivalence class of hedges incident to
    /// the point.
    GEO_Hedge	 firstIncidentEdge(GA_Offset pt) const
    { return geo_hedge::firstIncidentEdge(*this, pt); }

    GEO_Hedge	 nextIncidentEdge(GEO_Hedge e, GA_Offset pt) const
    { return geo_hedge::nextIncidentEdge(*this, e, pt); }

    /// Similar to first/nextIncidentHedge but using only outgoing hedges.
    /// Note that nextOutgoingHedge does not need the parameter pt anymore.
    GEO_Hedge	 firstOutgoingHedge(GA_Offset pt) const
    { return geo_hedge::firstOutgoingHedge(*this, pt); }

    GEO_Hedge	 nextOutgoingHedge(GEO_Hedge e) const
    { return geo_hedge::nextOutgoingHedge(*this, e); }

    /// Similar to first/nextIncidentHedge but using only incoming hedges.
    /// Note that nextOutgoingHedge does not need the parameter pt anymore.
    GEO_Hedge	 firstIncomingHedge(GA_Offset pt) const;
    GEO_Hedge	 nextIncomingHedge(GEO_Hedge e) const;

    /// Manifold Scan Methods:

    /// A "manifold scan" around a point moves from one incoming (resp outgoing
    /// hedge incident to a point to the next in counterclockwise order as
    /// long as the hedge in question is a manifold hedge (i.e. it is
    /// equivalent to exactly one other hedge oriented oppositely).

    /// Returns the first hedge with the same dst as e that is reachable
    /// from e through a counterclockwise manifold scan around dst of e.
    /// Returns e itself if either:
    /// 	1) e is already the first such hedge, or
    /// 	2) the src of e is an interior point of a manifold,
    ///            ie the counterclockwise scan reaches back at e.
    GEO_Hedge	 firstManifoldIncomingHedge(GEO_Hedge e) const
    { return geo_hedge::firstManifoldIncomingHedge(*this, e); }

    SYS_FORCE_INLINE
    GEO_Hedge	 dfirst(GEO_Hedge e) const
    { return firstManifoldIncomingHedge(e); }
    /// Returns the previous hedge with the same dst as e in a
    /// counterclockwise manifold scan, returns GEO_INVALID_HEDGE if no
    /// such hedge exists.
    ///
    /// NOTE: This is equivalent to dprev() operation in Quad-Edge
    /// terminology, which is also used in GQ.
    GEO_Hedge	 prevManifoldIncomingHedge(GEO_Hedge e) const
    { return geo_hedge::prevManifoldIncomingHedge(*this, e); }

    SYS_FORCE_INLINE
    GEO_Hedge	 dprev(GEO_Hedge e) const
    { return prevManifoldIncomingHedge(e); }

    /// Returns the next hedge with the same dst as e in a counterclockwise
    /// manifold scan, returns GEO_INVALID_HEDGE if no such hedge exists.
    ///
    /// NOTE: This is equivalent to dnext() operation in Quad-Edge
    /// terminology, which is also used in GQ.
    GEO_Hedge	 nextManifoldIncomingHedge(GEO_Hedge e) const
    { return geo_hedge::nextManifoldIncomingHedge(*this, e); }

    SYS_FORCE_INLINE
    GEO_Hedge	 dnext(GEO_Hedge e) const
    { return nextManifoldIncomingHedge(e); }

    /// Similar to firstManifoldIncomingHedge but finds the first hedge with
    /// the same src as e in a counterclockwise scan from e.
    GEO_Hedge	 firstManifoldOutgoingHedge(GEO_Hedge e) const
    { return geo_hedge::firstManifoldOutgoingHedge(*this, e); }

    SYS_FORCE_INLINE
    GEO_Hedge	 ofirst(GEO_Hedge e) const
    { return firstManifoldOutgoingHedge(e); }

    /// Returns the previous hedge with the same src as e in a counterclockwise
    // manifold scan, returns GEO_INVALID_HEDGE if no such hedge exists.
    ///
    /// NOTE: This is equivalent to oprev() operation in Quad-Edge
    /// terminology, which is also used in GQ.
    GEO_Hedge	 prevManifoldOutgoingHedge(GEO_Hedge e) const
    { return geo_hedge::prevManifoldOutgoingHedge(*this, e); }

    SYS_FORCE_INLINE
    GEO_Hedge	 oprev(GEO_Hedge e) const
    { return prevManifoldOutgoingHedge(e); }

    /// Returns the next hedge with the same src as e in a counterclockwise
    /// manifold scan, returns GEO_INVALID_HEDGE if no such hedge exists.
    ///
    /// NOTE: This is equivalent to onext() operation in Quad-Edge
    /// terminology, which is also used in GQ.
    GEO_Hedge	 nextManifoldOutgoingHedge(GEO_Hedge e) const
    { return geo_hedge::nextManifoldOutgoingHedge(*this, e); }

    SYS_FORCE_INLINE
    GEO_Hedge	 onext(GEO_Hedge e) const
    { return nextManifoldOutgoingHedge(e); }

    /// Find a hedge with the given endpoints or return GEO_INVLAID_HEDGE
    GEO_Hedge	 findHedgeWithEndpoints(GA_Offset p0, GA_Offset p1) const
    { return geo_hedge::findHedgeWithEndpoints(*this, p0, p1); }

    /// Returns the primary hedge equivalent to the arguemtn hedge.
    GEO_Hedge primaryEquivalentHedge(GEO_Hedge e) const;
    SYS_FORCE_INLINE
    GEO_Hedge	 primary(GEO_Hedge e) const
    { return primaryEquivalentHedge(e); }

    GEO_SHedge	 primary(GEO_SHedge e) const
    {
	GEO_Hedge ep = primary(e.hedge());
	return GEO_SHedge(ep, srcPoint(ep) == srcPoint(e) ? 1 : -1);
    }

    /// Returns the length of the hedge e.
    /// Note that it requires the endpoints of e to be determiend.
    fpreal	 length(GEO_Hedge e) const
    { return geo_hedge::length(*this, e); }

    /// Returns the positions of the source point of e
    UT_Vector3	 srcPos3(GEO_Hedge e) const
    { return geo_hedge::srcPos3(myGdp, e); }

    /// Returns the positions of the destination point of e
    UT_Vector3	 dstPos3(GEO_Hedge e) const
    { return geo_hedge::dstPos3(*this, e); }

    /// Returns the vector defined by the end-points of e
    UT_Vector3 	 hedgeVector(GEO_Hedge e) const
    { return dstPos3(e) - srcPos3(e); }

    /// Returns the primitives angle at dst of the hedge.
    /// NB. If the angle in question is reflex, then its 2*pi complement is
    /// returned unless the normal of the primitive that contains e is
    /// passed using the optional nml parameter.
    fpreal	 dstPrimitiveAngle(GEO_Hedge e, UT_Vector3 *nml = 0) const
    { return geo_hedge::dstPrimitiveAngle(*this, e, nml); }

    /// Returns the primitives angle at src of the hedge
    /// NB. If the angle in question is reflex, then its 2*pi complement is
    /// returned unless the normal of the primitive that contains e is
    /// passed using the optional nml parameter.
    fpreal	 srcPrimitiveAngle(GEO_Hedge e, UT_Vector3 *nml = 0) const
    { return geo_hedge::srcPrimitiveAngle(*this, e, nml); }

    /// Returns the primitives angle at dst of the hedge
    fpreal	 dstPrimitiveAngleCos(GEO_Hedge e) const
    { return geo_hedge::dstPrimitiveAngleCos(*this, e); }

    /// Returns the primitives angle at src of the hedge
    fpreal	 srcPrimitiveAngleCos(GEO_Hedge e) const
    { return geo_hedge::srcPrimitiveAngleCos(*this, e); }

    /// Returns true if the hedge e is a manifold hedge, if accept_bd is
    /// false, this means that the equivalence class of e consists of
    /// exactly two hedges oriented oppositely. If accept_bd is true, then
    /// the equivalence class of e can also consist of a single (boundary)
    /// half-edge.
    bool	 isManifoldHedge(GEO_Hedge e, bool accept_bd = false) const
    { return geo_hedge::isManifoldHedge(*this, e, accept_bd); }

    /// Returns true if the hedge e is a boundary hedge, i.e. if its
    /// equivalence class is singleton.
    bool	 isBoundaryHedge(GEO_Hedge e) const
    { return geo_hedge::isBoundaryHedge(*this, e); }

    /// Returns true if the hedge e is a bridge hedge, i.e. if is a
    /// manifold hedge and its other equivalent hedge belongs to the
    /// same primitive as e does.
    bool	 isBridgeHedge(GEO_Hedge e) const
    { return geo_hedge::isBridgeHedge(*this, e); }

    /// Returns true if e1 and e2 are equivalent hedges with opposite
    /// orientation.
    bool	 areOpposite(GEO_Hedge e1, GEO_Hedge e2) const
    { return geo_hedge::areOpposite(*this, e1, e2); }

    bool	 areOpposite(GEO_SHedge e1, GEO_SHedge e2) const
    { return areEquivalent(e1, -e2); }

    /// Returns true if e1 and e2 are equivalent hedges.
    bool	 areEquivalent(GEO_Hedge e1, GEO_Hedge e2) const
    { return geo_hedge::areEquivalent(*this, e1, e2); }

    /// Returns true if e1 and e2 are equivalent signed hedges.
    bool	 areEquivalent(GEO_SHedge e1, GEO_SHedge e2) const
    { return primary(e1) == primary(e2); }

    /// Returns the number of hedges in the equivalence class of e.
    GA_Size	 numEquivalentHedges(GEO_Hedge e) const
    { return geo_hedge::numEquivalentHedges(*this, e); }

    /// Returns the number if *edges* incident to a point (equivalent hedges
    /// count as 1).
    GA_Size	 numIncidentEdges(GA_Offset pt) const
    { return geo_hedge::numIncidentEdges(*this, pt); }

    /// Returns the number of distinct *hedges* incident to pt
    GA_Size	 numIncidentHedges(GA_Offset pt) const
    { return geo_hedge::numIncidentHedges(*this, pt); }

    const
    GA_Detail	*getDetail() const { return myGdp; }

    SYS_FORCE_INLINE
    GA_Offset    prevPrimVertex(GA_Offset v) const;

    SYS_FORCE_INLINE
    GA_Offset    nextPrimVertex(GA_Offset v) const;

    SYS_FORCE_INLINE
    void  	 getAdjacentPrimVertices(GA_Offset v, GA_Offset &vprev,
                        GA_Offset &vnext) const;

private:
    void 	 buildHedgeTopologyLinks(const GA_PrimitiveGroup *group = NULL,
		 	bool exclude_prims = true);

    const GA_Detail		*myGdp;
    GA_OffsetArray		myHedgeNextRef;
    GA_OffsetArray		myPrimPrevRef;
    GA_OffsetArray		myPrimNextRef;
    UT_BitArray			myPrimaryMask;
};

/// GEO_HedgeInterface provides the interface to deal with a detail's
/// half-edges. Ultimately it's expected to replace GQ. By default the
/// constructor of the class adds two additional half-edge topology attributes
/// to the detail if they're not already present (unless the second parameter
/// is set to false, in which case all operations are handled without the
/// topology attributes and can therefore be much much slower). The
/// destructor of GEO_HedgeInterface removes these topology
/// attributes from detail if they were added by the constructor but leaves
/// them there if they already existed when the class was created.
///
/// NOTE: The merge code for topology attributes is not tested and is quite
/// like to fail. It is therefore highly advisable to remove the topology
/// attributes when you're done with them and thereby prevent them from
/// being passed to details downstream.

class GEO_API GEO_HedgeInterface
{
public:

    GEO_HedgeInterface(GA_Detail *gdp, bool use_hedge_topology = true);
    ~GEO_HedgeInterface();

    void	 destroyHedgeTopologyLinks();

    bool	 haveHedgeTopology() const
    { return myHaveHedgeTopology; }

    /// If a wrangler cache is set, the operations involving hedges use it
    /// to wrangle attributes. To stop wrangling, the method can be called
    /// with NULL as parameter.

    void 	 setWranglerCache(GA_ElementWranglerCache *wc)
    { myWranglerCache = wc; }

    /// Return the polygon to which the hedge belongs.
    /// NOTE: may need to modify this if other primitives support hedges.
    GEO_Primitive	*hedgePrimitive(GEO_Hedge e) const
    { return static_cast<GEO_Primitive *>(geo_hedge::hedgePrimitive(myGdp, e)); }

    GA_Offset		 hedgePrimitiveOffset(GEO_Hedge e) const
    { return geo_hedge::hedgePrimitiveOffset(myGdp, e); }

    /// Check whether a hedge is valid: a hedge is valid if
    ///  1: its src vertex belongs to a primitive that supports hedges, and
    ///	 2: the primitive returns a valid *next boundary vertex* for its dst
    ///     vertex.
    ///
    /// @see: GA_Topology.h
    bool 	 isValidHedge(GEO_Hedge &e) const;

    /// returns true of e is the *primary* hedge in its equivalence class.
    /// Each equivalence class of hedges at any time has a unique primary
    /// hedge. This is used for example to enumerate over all edges in the
    /// detail (as opposed to over all hedges).
    bool 	 isPrimary(GEO_Hedge e) const;

    /// Returns the src vertex of the hedge
    GA_Offset	 srcVertex(GEO_Hedge e) const
    { return geo_hedge::srcVertex(e); }

    /// Returns the dst vertex of the hedge
    GA_Offset	 dstVertex(GEO_Hedge e) const
    { return geo_hedge::dstVertex(*this, e); }

    /// Returns the vertex before the src vertex of the hedge in the poly
    /// to which the hedge belongs
    GA_Offset	 preSrcVertex(GEO_Hedge e) const
    { return geo_hedge::preSrcVertex(*this, e); }

    /// Returns the vertex after the dst of the vertex of the hedge in
    /// the poly to which the hedge belongs
    GA_Offset	 postDstVertex(GEO_Hedge e) const
    { return geo_hedge::postDstVertex(*this, e); }

    /// Returns the point to which the src vertex is wired
    GA_Offset	 srcPoint(GEO_Hedge e) const
    {  return geo_hedge::srcPoint(myGdp, e); }

    /// Returns the point to which the dst vertex is wired
    GA_Offset	 dstPoint(GEO_Hedge e) const
    { return geo_hedge::dstPoint(*this, e); }

    /// Returns the point to which the pre-src vertex is wired.
    /// @see: preSrcVertex()
    GA_Offset	 preSrcPoint(GEO_Hedge e) const
    { return geo_hedge::preSrcPoint(*this, e); }

    /// Returns the point to which the post-dst vertex is wired
    /// @see: postDstVertex()
    GA_Offset	 postDstPoint(GEO_Hedge e) const
    { return geo_hedge::postDstPoint(*this, e); }

    /// Returns the next hedge (hedge with src equal to the dst of the
    /// parameter hedge) in the polygon that contains the parameter hedge
    GEO_Hedge	 nextPrimitiveHedge(GEO_Hedge e) const
    { return geo_hedge::nextPrimitiveHedge(*this, e); }

    SYS_FORCE_INLINE
    GEO_Hedge	 lnext(GEO_Hedge e) const
    { return nextPrimitiveHedge(e); }

    /// Returns the previous hedge (hedge whose dst is the same as the src
    /// of the parameter hedge) in the polygon that contains the parameter
    /// hedge.
    GEO_Hedge	 prevPrimitiveHedge(GEO_Hedge e) const
    { return geo_hedge::prevPrimitiveHedge(*this, e); }

    SYS_FORCE_INLINE
    GEO_Hedge	 lprev(GEO_Hedge e) const
    { return prevPrimitiveHedge(e); }

    /// Returns the "other" (than e) hedge in the polygon that has the
    /// pt as an endpoint
    GEO_Hedge	 otherPrimitiveHedgeAtPoint(GEO_Hedge e, GA_Offset pt) const
    { return geo_hedge::otherPrimitiveHedgeAtPoint(*this, e, pt); }

    /// Returns the "next" equivalent hedge to e:
    /// two hedges are equivalent if either their dsts and srcs are
    /// respectively wired to the same points or if the dst of each is
    /// wired to the same point as the src of the other. calling
    /// nextEquivalentHedge() repeatedly returns back to the original hedge.
    /// Thus to check if hedge e is manifold hedge one can check that:
    /// nextEquivalentHedge(e) != e &&
    /// 	nextEquivalentHedge(nextEquivalentHedge(e)) == e
    GEO_Hedge	 nextEquivalentHedge(GEO_Hedge e) const;

    SYS_FORCE_INLINE
    GEO_Hedge	 sym(GEO_Hedge e) const
    { return nextEquivalentHedge(e); }

    /// Returns the primary hedge equivalent to the argument hedge.
    GEO_Hedge	 primaryEquivalentHedge(GEO_Hedge e) const;
    SYS_FORCE_INLINE
    GEO_Hedge	 primary(GEO_Hedge e) const
    { return primaryEquivalentHedge(e); }

    /// Returns a first incidentHedge to a point. Together with
    /// nextIncidentHedge, one can enumerate circularly over all hedges
    /// incident to a point.
    GEO_Hedge firstIncidentHedge(GA_Offset pt) const
    { return geo_hedge::firstIncidentHedge(*this, pt); }

    GEO_Hedge	 nextIncidentHedge(GEO_Hedge e, GA_Offset pt) const
    { return geo_hedge::nextIncidentHedge(*this, e, pt); }

    /// Similar to first/nextIncidentHedge but stops over only one of the
    /// hedges in each equivalence class of hedges incident to the point
    GEO_Hedge	 firstIncidentEdge(GA_Offset pt) const
    { return geo_hedge::firstIncidentEdge(*this, pt); }

    GEO_Hedge	 nextIncidentEdge(GEO_Hedge e, GA_Offset pt) const
    { return geo_hedge::nextIncidentEdge(*this, e, pt); }

    /// Similar to first/nextIncidentHedge but using only outgoing hedges.
    /// Note that nextOutgoingHedge does not need the parameter pt anymore.
    GEO_Hedge	 firstOutgoingHedge(GA_Offset pt) const
    { return geo_hedge::firstOutgoingHedge(*this, pt); }

    GEO_Hedge	 nextOutgoingHedge(GEO_Hedge e) const
    { return geo_hedge::nextOutgoingHedge(*this, e); }

    /// Similar to first/nextIncidentHedge but using only incoming hedges.
    /// Note that nextOutgoingHedge does not need the parameter pt anymore.
    GEO_Hedge	 firstIncomingHedge(GA_Offset pt) const
    { return geo_hedge::firstIncomingHedge(*this, pt); }

    GEO_Hedge	 nextIncomingHedge(GEO_Hedge e) const
    { return geo_hedge::nextIncomingHedge(*this, e); }

    /// Manifold Scan Methods:

    /// A "manifold scan" around a point moves from one incoming (resp outgoing
    /// hedge incident to a point to the next in counterclockwise order as
    /// long as the hedge in question is a manifold hedge (i.e. it is
    /// equivalent to exactly one other hedge oriented oppositely).

    /// Returns the first hedge with the same dst as e that is reachable
    /// from e through a counterclockwise manifold scan around dst of e.
    /// Returns e itself if either:
    /// 	1) e is already the first such hedge, or
    /// 	2) the src of e is an interior point of a manifold,
    ///            ie the counterclockwise scan reaches back at e.
    GEO_Hedge	 firstManifoldIncomingHedge(GEO_Hedge e) const
    { return geo_hedge::firstManifoldIncomingHedge(*this, e); }

    SYS_FORCE_INLINE
    GEO_Hedge 	 dfirst(GEO_Hedge e) const
    { return firstManifoldIncomingHedge(e); }
    /// Returns the previous hedge with the same dst as e in a
    /// counterclockwise manifold scan, returns GEO_INVALID_HEDGE if no
    /// such hedge exists.
    ///
    /// NOTE: This is equivalent to dprev() operation in Quad-Edge
    /// terminology, which is also used in GQ.
    GEO_Hedge	 prevManifoldIncomingHedge(GEO_Hedge e) const
    { return geo_hedge::prevManifoldIncomingHedge(*this, e); }

    SYS_FORCE_INLINE
    GEO_Hedge 	 dprev(GEO_Hedge e) const
    { return prevManifoldIncomingHedge(e); }

    /// Returns the next hedge with the same dst as e in a counterclockwise
    /// manifold scan, returns GEO_INVALID_HEDGE if no such hedge exists.
    ///
    /// NOTE: This is equivalent to dnext() operation in Quad-Edge
    /// terminology, which is also used in GQ.
    GEO_Hedge	 nextManifoldIncomingHedge(GEO_Hedge e) const
    { return geo_hedge::nextManifoldIncomingHedge(*this, e); }

    SYS_FORCE_INLINE
    GEO_Hedge 	 dnext(GEO_Hedge e) const
    { return nextManifoldIncomingHedge(e); }

    /// Similar to firstManifoldIncomingHedge but finds the first hedge with
    /// the same src as e in a counterclockwise scan from e.
    GEO_Hedge 	 firstManifoldOutgoingHedge(GEO_Hedge e) const
    { return geo_hedge::firstManifoldOutgoingHedge(*this, e); }

    SYS_FORCE_INLINE
    GEO_Hedge	 ofirst(GEO_Hedge e) const
    { return firstManifoldOutgoingHedge(e); }

    /// Returns the previous hedge with the same src as e in a counterclockwise
    // manifold scan, returns GEO_INVALID_HEDGE if no such hedge exists.
    ///
    /// NOTE: This is equivalent to oprev() operation in Quad-Edge
    /// terminology, which is also used in GQ.
    GEO_Hedge 	 prevManifoldOutgoingHedge(GEO_Hedge e) const
    { return geo_hedge::prevManifoldOutgoingHedge(*this, e); }

    SYS_FORCE_INLINE
    GEO_Hedge 	 oprev(GEO_Hedge e) const
    { return prevManifoldOutgoingHedge(e); }

    /// Returns the next hedge with the same src as e in a counterclockwise
    /// manifold scan, returns GEO_INVALID_HEDGE if no such hedge exists.
    ///
    /// NOTE: This is equivalent to onext() operation in Quad-Edge
    /// terminology, which is also used in GQ.
    GEO_Hedge 	 nextManifoldOutgoingHedge(GEO_Hedge e) const
    { return geo_hedge::nextManifoldOutgoingHedge(*this, e); }

    SYS_FORCE_INLINE
    GEO_Hedge	 onext(GEO_Hedge e) const
    { return nextManifoldOutgoingHedge(e); }

    /// Returns the length of the hedge e.
    /// Note that it requires the endpoints of e to be determined.
    fpreal	 length(GEO_Hedge e) const
    { return geo_hedge::length(*this, e); }

    /// Returns the positions of the source point of e
    UT_Vector3	 srcPos3(GEO_Hedge e) const
    { return geo_hedge::srcPos3(myGdp, e); }

    /// Returns the positions of the destination point of e
    UT_Vector3	 dstPos3(GEO_Hedge e) const
    { return geo_hedge::dstPos3(*this, e); }

    /// Returns the vector defined by the end-points of e
    UT_Vector3 	 hedgeVector(GEO_Hedge e) const
    { return dstPos3(e) - srcPos3(e); }

    /// Returns the primitives angle at dst of the hedge
    /// NB. If the angle in question is reflex, then its 2*pi complement is
    /// returned unless the normal of the primitive that contains e is
    /// passed using the optional nml parameter.
    fpreal	 dstPrimitiveAngle(GEO_Hedge e, UT_Vector3 *nml = 0) const
    { return geo_hedge::dstPrimitiveAngle(*this, e, nml); }

    /// Returns the primitives angle at src of the hedge
    /// NB. If the angle in question is reflex, then its 2*pi complement is
    /// returned unless the normal of the primitive that contains e is
    /// passed using the optional nml parameter.
    fpreal	 srcPrimitiveAngle(GEO_Hedge e, UT_Vector3 *nml = 0) const
    { return geo_hedge::srcPrimitiveAngle(*this, e, nml); }

    /// Returns the primitives angle at dst of the hedge
    fpreal dstPrimitiveAngleCos(GEO_Hedge e) const
    { return geo_hedge::dstPrimitiveAngleCos(*this, e); }

    /// Returns the primitives angle at src of the hedge
    fpreal	 srcPrimitiveAngleCos(GEO_Hedge e) const
    { return geo_hedge::srcPrimitiveAngleCos(*this, e); }

    /// Returns true if the hedge e is a manifold hedge, if accept_bd is
    /// false, this means that the equivalence class of e consists of
    /// exactly two hedges oriented oppositely. If accept_bd is true, then
    /// the equivalence class of e can also consist of a single (boundary)
    /// half-edge.
    bool	 isManifoldHedge(GEO_Hedge e, bool accept_bd = false) const
    { return geo_hedge::isManifoldHedge(*this, e, accept_bd); }

    /// Returns true if the hedge e is a boundary hedge, i.e. if its
    /// equivalence class is singleton.
    bool	 isBoundaryHedge(GEO_Hedge e) const
    { return geo_hedge::isBoundaryHedge(*this, e); }

    /// Returns true if the hedge e is a bridge hedge, i.e. if is a
    /// manifold hedge and its other equivalent hedge belongs to the
    /// same primitive as e does.
    bool	 isBridgeHedge(GEO_Hedge e) const
    { return geo_hedge::isBridgeHedge(*this, e); }

    /// Returns true if e1 and e2 are equivalent hedges with opposite
    /// orientation.
    bool	 areOpposite(GEO_Hedge e1, GEO_Hedge e2) const
    { return geo_hedge::areOpposite(*this, e1, e2); }

    /// Returns true if e1 and e2 are equivalent hedges.
    bool	 areEquivalent(GEO_Hedge e1, GEO_Hedge e2) const
    { return geo_hedge::areEquivalent(*this, e1, e2); }

    /// Returns the number of hedges in the equivalence class of e.
    GA_Size	 numEquivalentHedges(GEO_Hedge e) const
    { return geo_hedge::numEquivalentHedges(*this, e); }

    /// Returns the number if *edges* incident to a point (equivalent hedges
    /// count as 1).
    GA_Size	 numIncidentEdges(GA_Offset pt) const
    { return geo_hedge::numIncidentEdges(*this, pt); }

    /// Returns the number of distinct *hedges* incident to pt
    GA_Size	 numIncidentHedges(GA_Offset pt) const
    { return geo_hedge::numIncidentHedges(*this, pt); }

    /// Find a hedge with the given endpoints or return GEO_INVLAID_HEDGE
    GEO_Hedge	 findHedgeWithEndpoints(GA_Offset p0, GA_Offset p1) const
    { return geo_hedge::findHedgeWithEndpoints(*this, p0, p1); }

    /// Returns the number of primitives that have pt as a vertex and support
    /// hedges.
    GA_Size	 numIncidentHedgeSupportingPrims(GA_Offset pt) const;

    // operations involving half-edges

    /// Rotate e forward, provided that e is a manifold hedge (equivalent
    /// to exactly one other hedge f oppositely oriented. This is not checked
    /// and the topology may be corrupted if rotate is applied to non-manifold
    /// or boundary edges. Returns the resulting hedge which happens to be
    /// equal to e (The input hedge will be the hedge that rotates (same src
    /// vertex) and the same holds for the equivalent edge of e.
    ///
    ///    ------>------>          ------>------>
    ///    ^     |^     |          ^          /7|
    ///    |     ||     |          |        //  |
    ///    |   e ||     |   ===>   |   e  //    |
    ///    |     ||     |          |    //      |
    ///    |     v|     v          |  //        v
    ///    <------<------          |L/<---<------
    ///
    /// Note that this operation is precisely an edge flip if the two
    /// primitives involved are triangles.

    GEO_Hedge	 rotateForward(GEO_Hedge e);

    /// Complete reverse of rotateForward (all the point, vertex, and
    /// primitive offsets are restored). When the incident primitives are
    /// triangles, rotating forward or backward result in the same edge but
    /// they are not exactly identical. In particular, two consecutive flips
    /// on the same edge will reverse the orientation of the edge.

    GEO_Hedge	 rotateBackward(GEO_Hedge e);

    /// Returns true if e can be flipped without the result overlapping an
    /// existing hedge. In other words, e is flippable if its endpoints are
    /// not endpoints of an existing edge in the geometry before the flip that
    /// is not equivalent to e.

    bool	 isFlippable(GEO_Hedge e) const;

    /// Flip e (provided that e is shared between two triangles) and is
    /// flippable. Returns the resulting (flipped) hedge (equal to e) or
    /// GEO_INVALID_HEDGE if e is not flippable.

    GEO_Hedge	 flip(GEO_Hedge e);

    /// unflip is the reverse of a flip. Doing a flip followed by an unflip
    /// should result in the same mesh with the same vertex/point/primtive
    /// offsets and indices (but the vertices within primitives may shift).
    GEO_Hedge 	 unflip(GEO_Hedge e);

    /// Returns true if the hedge e is contractible, i.e. contracting e does
    /// not force a a non-trivial (separating) cycle to collapse. A separating
    /// cycle is a closed sequence of edges that if removed (together with
    /// their incident geometry elements) the number of connected components
    // increases. Contracting a non-contractible edge will split geometry into
    /// multiple components.
    bool 	 isContractible(GEO_Hedge e) const;

    /// Contracts e.
    /// @parameter on_dst, if true, causes the src point to move to the
    ///			position of the dst point. Otherwise, the dst point
    ///			is moved to the src point's position.
    /// @parameter ratio, if not equal to 1.0, places the point of contraction
    /// 		in a convex combination with biases ratio and
    ///			1.0 - ratio from src and dst (or the other way around
    /// 		if on_dst is false) respectively.
    GA_Offset 	 contract(GEO_Hedge e, bool on_dst = true, fpreal ratio = 1.0,
              	        bool check_contractible = true);

#if 0
    GA_Offset 	 contractNew(GEO_Hedge e, bool on_dst = true, fpreal ratio = 1.0);
#endif

    /// Splits the hedge e and its incident polygon (assumed to be a triangle,
    /// although it doesn't fail if it isn't).
    /// @parameter ratio determines the position of the new point at which the
    /// 		hedge is split.
    /// @parameter and_equivalent if true also splits all other hedges
    ///			equivalent to e at the same point.
    /// @parameter poff is an optional point offset assumed to be located in
    //			the correct position (as given by ratio). This allows
    //			the caller to reuse points already created.
    GEO_Hedge	 split(GEO_Hedge e, fpreal ratio, bool and_equivalent = true,
             	       GA_Offset poff = GA_INVALID_OFFSET);


    GEO_Hedge	 reversibleSplit(GEO_Hedge e, fpreal ratio,
             	        bool and_equivalent = true,
             	        GA_Offset poff = GA_INVALID_OFFSET);


    /// Inserts a point in the middle of a hedge and divides into two hedges.
    ///
    /// @parameter ratio determines the position of the new point at which the
    /// 		hedge is split.
    /// @parameter and_equivalent if true also divides all other hedges
    ///			equivalent to e at the same point.
    /// @parameter poff is an optional point offset assumed to be located in
    //			the correct position (as given by ratio). This allows
    //			the caller to reuse points already created.
    GEO_Hedge	 divideHedge(GEO_Hedge e, fpreal ratio,
             	        bool and_equivalent = true,
             	        GA_Offset poff = GA_INVALID_OFFSET);

    /// Splits the primitive of a hedge into two primitives by cutting it
    /// along a diagonal. The hedges starting from ekeep and ending
    /// before edrop are kept in the current primitive while the edges
    /// hedges starting at edrop and ending before ekeep end up in the
    /// new primitive.
    GEO_Hedge 	 divideHedgePrimitive(GEO_Hedge ekeep, GEO_Hedge edrop);

    /// Subdivides the polygon of e at its barycenter or the supplied point
    GA_Offset 	 splitHedgePrimitive(GEO_Hedge e,
              	        GA_Offset pt = GA_INVALID_OFFSET);

    // Deletes the *star* of a point. The star of a point in a polygonal
    // complex consists of all edges and polygons incident to that point.
    // So, deleting the star of an internal point on a manifold should leave
    // a hole behind. If the patch_link is set to true, the hole is patched
    // (if possible) with a new polygon. For this be possible the point *must*
    // be an internal manifold point. If patching is possible then one of
    // the polygons previously incident to pt can be specified as patch_prim
    // to extend and become the patching polygon. If this polygon is not
    // specified or is not actually incident to pt, then a new polygon will
    // be created.
    void	 deletePointStar(GA_Offset pt, bool patch_link = true,
        	        GA_Offset patch_prim = GA_INVALID_OFFSET);

    GA_Detail 	*getDetail() const { return myGdp; }

    SYS_FORCE_INLINE
    GA_Offset    prevPrimVertex(GA_Offset v) const;

    SYS_FORCE_INLINE
    GA_Offset    nextPrimVertex(GA_Offset v) const;

    SYS_FORCE_INLINE
    void  	 getAdjacentPrimVertices(GA_Offset v, GA_Offset &vprev,
                        GA_Offset &vnext) const;
private:

    GA_Detail 			*myGdp;
    GA_Topology 		&myTopology;
    bool 			 myKeepHedgeTopology;
    bool 			 myHaveHedgeTopology;
    GA_ElementWranglerCache	*myWranglerCache;
};


GA_Offset
GEO_HedgeInterface::prevPrimVertex(GA_Offset v) const
{
    GA_Offset vprev, vnext;
    myGdp->getTopology().getAdjacentBoundaryVertices(v, vprev, vnext);
    return vprev;
}


GA_Offset
GEO_HedgeInterface::nextPrimVertex(GA_Offset v) const
{
    GA_Offset vprev, vnext;
    myGdp->getTopology().getAdjacentBoundaryVertices(v, vprev, vnext);
    return vnext;
}

void
GEO_HedgeInterface::getAdjacentPrimVertices(GA_Offset v, GA_Offset &vprev,
                                            GA_Offset &vnext) const
{
    myGdp->getTopology().getAdjacentBoundaryVertices(v, vprev, vnext);
}

GA_Offset
GEO_DetachedHedgeInterface::prevPrimVertex(GA_Offset v) const
{
    return myPrimPrevRef(v);
}


GA_Offset
GEO_DetachedHedgeInterface::nextPrimVertex(GA_Offset v) const
{
    return myPrimNextRef(v);
}

void
GEO_DetachedHedgeInterface::getAdjacentPrimVertices(GA_Offset v,
                                                    GA_Offset &vprev,
                                                    GA_Offset &vnext) const
{
    vprev = myPrimPrevRef(v);
    vnext = myPrimNextRef(v);
}

#endif
