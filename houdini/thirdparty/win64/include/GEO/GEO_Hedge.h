/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO_Hedge.h ( GEO Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GEO_Hedge__
#define __GEO_Hedge__

#include "GEO_API.h"
#include <GA/GA_Detail.h>
#include <GA/GA_Types.h>
#include <UT/UT_Assert.h>
#include <UT/UT_VectorTypes.h>
#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Math.h>
#include <SYS/SYS_Types.h>

#include <math.h>

class GEO_Detail;
class GEO_Hedge;
class GEO_PrimPoly;

class GA_ElementWranglerCache;

/// An invalid hedge is sometimes returned if an operation is unsuccessful
#define GEO_INVALID_HEDGE	GEO_Hedge(GA_INVALID_OFFSET)

namespace geo_hedge
{

GA_Offset srcVertex(GEO_Hedge);

}; // namespace geo_hedge

/// GEO_Hedge encapsulates a half-edge (hedge) which is the *restriction* of
//  an edge to one primitive incident to that edge, or equivalently the
//  contribution to an edge of one primitive incident to it. It essentially
//  stores only one GA_Offset which is the source (src) vertex for the
//  half-edge it represents. It can therefore be freely copied and assigned.

class GEO_API GEO_Hedge
{
public:
    explicit 		 GEO_Hedge()
			    { mySrcVertex = GA_INVALID_OFFSET; }

    explicit 		 GEO_Hedge(GA_Offset voffset)
			    { mySrcVertex = voffset; }

    SYS_FORCE_INLINE
    bool 	 	 operator==(GEO_Hedge e) const
			    { return mySrcVertex == e.mySrcVertex; }

    SYS_FORCE_INLINE
    bool 	 	 operator!=(GEO_Hedge e) const
			    { return mySrcVertex != e.mySrcVertex; }

    friend class 	 GEO_SHedge;
    friend GA_Offset 	 geo_hedge::srcVertex(GEO_Hedge);

private:
    GA_Offset 		 mySrcVertex;
};


/// GEO_SHedge encapsulates a *signed* half-edge. It is a half-edge together
//  with a sign in {+1, -1}. It is implemented by using the second most
//  significant bit of a half-edge's source vertex offset for the sign bit.
//  This means that a signed half-edge cannot have a source vertex offset
//  between 2^62 and 2^63 - 1.

#define GEO_INVALID_SHEDGE 	GEO_SHedge(GEO_INVALID_HEDGE, 1)
#define NEG_MASK    		(1ll << 62)

class GEO_API GEO_SHedge
{
public:
			 GEO_SHedge() :
			    myBits(GA_INVALID_OFFSET) { }

    explicit		 GEO_SHedge(GEO_Hedge hedge, int sign = 1) :
			    myBits(hedge.mySrcVertex) { setSign(sign); }

			 GEO_SHedge(const GEO_SHedge &other) :
			     myBits(other.myBits) { }

    int			 sign() const { return isPositive() ? 1 : -1; }
    bool 		 isPositive() const { return !isNegative(); }
    bool 		 isNegative() const { return (myBits & NEG_MASK); }
    bool 		 isValid() const { return myBits >= 0; }

    SYS_FORCE_INLINE
    void 	 	 setSign(int sign);

    SYS_FORCE_INLINE
    GEO_Hedge 	 	 hedge() const;

    SYS_FORCE_INLINE
    GEO_SHedge 		 &operator=(const GEO_SHedge &other);

    SYS_FORCE_INLINE
    bool 	 	 operator==(const GEO_SHedge& other) const
			    { return myBits == other.myBits; }

    SYS_FORCE_INLINE
    bool 	 	 operator!=(const GEO_SHedge& other) const
			    { return myBits != other.myBits; }

    SYS_FORCE_INLINE
    GEO_SHedge 	 	 operator-()
			    { return GEO_SHedge(hedge(), -sign()); }

private:
    GA_Size		 myBits;

#ifndef SESI_LITTLE_ENDIAN
#error "Make sure the bitfields in the union work on big endian platforms!"
#endif
};

void
GEO_SHedge::setSign(int sign)
{
    if (sign >= 0)
	myBits &= (~NEG_MASK);
    else
	myBits |= NEG_MASK;
}

SYS_FORCE_INLINE
GEO_SHedge &
GEO_SHedge::operator=(const GEO_SHedge &other)
{
    myBits = other.myBits;
    return *this;
}

GEO_Hedge
GEO_SHedge::hedge() const
{
    if (myBits < 0)
	return GEO_INVALID_HEDGE;

    return GEO_Hedge(GA_Offset(myBits & (~NEG_MASK)));
}

namespace geo_hedge
{
// static methods for common use with a given detail, or a templated
// interface parameter that supplies nextEquivalentHedge() or other
// half-edge supporting interface methods.

// NOTE: You probably shouldn't be calling these directly! Instead use
// an interface class that binds to your detail and provides a wrapper
// for all or most of these.


SYS_FORCE_INLINE
GA_Offset
srcVertex(GEO_Hedge h)
{
    return h.mySrcVertex;
}

SYS_FORCE_INLINE
GA_Offset
srcPoint(const GA_Detail *gdp, GEO_Hedge e)
{
    return gdp->vertexPoint(geo_hedge::srcVertex(e));
}

SYS_FORCE_INLINE
GA_Offset
hedgePrimitiveOffset(const GA_Detail *gdp, GEO_Hedge e)
{
    return gdp->vertexPrimitive(geo_hedge::srcVertex(e));
}

SYS_FORCE_INLINE
const GA_Primitive*
hedgePrimitive(const GA_Detail *gdp, GEO_Hedge e)
{
    return gdp->getPrimitive(hedgePrimitiveOffset(gdp, e));
}

SYS_FORCE_INLINE
GA_Primitive*
hedgePrimitive(GA_Detail *gdp, GEO_Hedge e)
{
    return gdp->getPrimitive(hedgePrimitiveOffset(gdp, e));
}

template <typename T>
SYS_FORCE_INLINE
GA_Offset
dstVertex(T &iface, GEO_Hedge e)
{
    return iface.nextPrimVertex(srcVertex(e));
}

template <typename T>
SYS_FORCE_INLINE
GA_Offset
preSrcVertex(T &iface, GEO_Hedge e)
{
    return iface.prevPrimVertex(srcVertex(e));
}

template <typename T>
SYS_FORCE_INLINE
GA_Offset
postDstVertex(T &iface, GEO_Hedge e)
{
    GA_Offset next_v;
    next_v = iface.nextPrimVertex(geo_hedge::srcVertex(e));
    if (!GAisValid(next_v))
	return GA_INVALID_OFFSET;

    return iface.nextPrimVertex(next_v);
}

template <typename T>
SYS_FORCE_INLINE
GA_Offset
dstPoint(T &iface, GEO_Hedge e)
{
    return iface.getDetail()->vertexPoint(dstVertex(iface, e));
}

template <typename T>
SYS_FORCE_INLINE
GA_Offset
preSrcPoint(T &iface, GEO_Hedge e)
{
    return iface.getDetail()->vertexPoint(preSrcVertex(iface, e));
}

template <typename T>
SYS_FORCE_INLINE
GA_Offset
postDstPoint(T &iface, GEO_Hedge e)
{
    return iface.getDetail()->vertexPoint(postDstVertex(iface, e));
}

template <typename T>
SYS_FORCE_INLINE
GEO_Hedge
nextPrimitiveHedge(T &iface, GEO_Hedge e)
{
    return GEO_Hedge(dstVertex(iface, e));
}

template <typename T>
SYS_FORCE_INLINE
GEO_Hedge
prevPrimitiveHedge(T &iface, GEO_Hedge e)
{
    return GEO_Hedge(preSrcVertex(iface, e));
}

template <typename T>
SYS_FORCE_INLINE
GEO_Hedge
otherPrimitiveHedgeAtPoint(T &iface, GEO_Hedge e, GA_Offset point)
{
    GA_Offset prev, next;

    iface.getAdjacentPrimVertices(geo_hedge::srcVertex(e), prev, next);
    if (srcPoint(iface.getDetail(), e) == point)
	return GEO_Hedge(prev);
    if (iface.getDetail()->vertexPoint(next) == point)
	return GEO_Hedge(next);
    else
	return GEO_INVALID_HEDGE;
}

template <typename T>
SYS_FORCE_INLINE
bool
areEquivalent(T &iface, GEO_Hedge e1, GEO_Hedge e2)
{
    GA_Offset dst1 = dstPoint(iface, e1);
    GA_Offset src1 = srcPoint(iface.getDetail(), e1);
    GA_Offset dst2 = dstPoint(iface, e2);
    GA_Offset src2 = srcPoint(iface.getDetail(), e2);
    return ((dst1 == dst2 && src1 == src2) || (dst1 == src2 && src1 == dst2));
}

template<typename T>
SYS_FORCE_INLINE bool
isBoundaryHedge(T &iface, GEO_Hedge e)
{
    return iface.nextEquivalentHedge(e) == e;
}

template<typename T>
bool
isBridgeHedge(T &iface, GEO_Hedge e)
{
    const GA_Detail *gdp = iface.getDetail();

    GEO_Hedge e0 = iface.nextEquivalentHedge(e);
    if (e0 == e)
	return false;

    GA_Offset esrcv = iface.srcVertex(e);
    GA_Offset eprim = gdp->vertexPrimitive(esrcv);
    GA_Offset esrcp = gdp->vertexPoint(esrcv);

    while (e0 != e)
    {
	GA_Offset e0srcv = iface.srcVertex(e);
	if (gdp->vertexPrimitive(e0srcv) == eprim &&
	    gdp->vertexPoint(e0srcv) != esrcp)
	    return true;
	e0 = iface.nextEquivalentHedge(e0);
    }

    return false;
}

template<typename T>
SYS_FORCE_INLINE
bool
isManifoldHedge(T &iface, GEO_Hedge e, bool accept_bd)
{
    GEO_Hedge ne = iface.nextEquivalentHedge(e);

    if (ne == e)
	return accept_bd;

    if (iface.nextEquivalentHedge(ne) != e)
	return false;

    return (iface.srcPoint(e) != iface.srcPoint(ne));
}

template<typename T>
SYS_FORCE_INLINE
GA_Size
numEquivalentHedges(T &iface, GEO_Hedge e)
{
    int res = 0;
    GEO_Hedge e0 = e;
    do
    {
	res++;
	e0 = iface.nextEquivalentHedge(e0);
    } while (e0 != e);

    return res;
}

template <typename T>
SYS_FORCE_INLINE
bool
areOpposite(T &iface, GEO_Hedge e1, GEO_Hedge e2)
{
    GA_Offset dst1 = dstPoint(iface, e1);
    GA_Offset src1 = srcPoint(iface.getDetail(), e1);
    GA_Offset dst2 = dstPoint(iface, e2);
    GA_Offset src2 = srcPoint(iface.getDetail(), e2);
    return (src1 == dst2 && dst1 == src2);
}

/// [first/next]IncidentHedge run over all half-edges incident
/// at src or dst to the given point in a specific order as
/// follows: all candiate vertices for incident half-edges are
/// traversed, by going over the vertices wired to the given point
/// in the order determined by GA_Topology and interleaving these
/// with (potential) half-edges that precede the one determined
/// by each vertex on its respective primitive.

template <typename T>
GEO_Hedge
firstIncidentHedge(T &iface, GA_Offset pt)
{
    const GA_Detail *gdp = iface.getDetail();

    for (GA_Offset vtx = gdp->pointVertex(pt); GAisValid(vtx);
	vtx = gdp->vertexToNextVertex(vtx))
    {
        // try outoing hedge at vtx, its dst must differ from pt
	GEO_Hedge e(vtx);
	if (iface.isValidHedge(e) && iface.dstPoint(e) != pt)
	    return e;

        // now try the previous vertex on primitive, its src shouldn't be pt
	GEO_Hedge eprev(iface.prevPrimVertex(vtx));
        if (iface.isValidHedge(eprev) && iface.srcPoint(eprev) != pt)
            return eprev;
    }

    return GEO_INVALID_HEDGE;
}

template <typename T>
GEO_Hedge
nextIncidentHedge(T &iface, GEO_Hedge e, GA_Offset pt)
{
    UT_ASSERT_P(iface.isValidHedge(e));

    const GA_Detail *gdp = iface.getDetail();

    GA_Offset vtx = srcVertex(e), vprev, vnext, v0;
    iface.getAdjacentPrimVertices(vtx, vprev, vnext);

    // figure out whether to return the previous edge on the same poly
    // or continue with next vertex wired to poin.

    if (gdp->vertexPoint(vtx) == pt && gdp->vertexPoint(vnext) != pt)
    {
	// pt is src of e: return previous hedge unless invalid
	GEO_Hedge eprev(vprev);
	if (iface.isValidHedge(eprev) && gdp->vertexPoint(vprev) != pt)
	    return eprev;
	v0 = vtx;
    }
    else
    {
	// pt must be dst of e: move to the next wired vertex
        UT_ASSERT_P(gdp->vertexPoint(vnext) == pt);
	v0 = vnext;
    }

    UT_ASSERT_P(gdp->vertexPoint(v0) == pt);

    GA_Offset vtmp = GA_INVALID_OFFSET;
    for (GA_Offset v = GAisValid(vtmp = gdp->vertexToNextVertex(v0)) ?
                       vtmp : gdp->pointVertex(pt); 1;
         v = GAisValid(vtmp = gdp->vertexToNextVertex(v)) ? vtmp :
             gdp->pointVertex(pt))
    {
	UT_ASSERT_P(gdp->vertexPoint(v) == pt);
	GEO_Hedge e0(v);
	if (iface.isValidHedge(e0) && iface.dstPoint(e0) != pt)
	    return e0;

        GEO_Hedge eprev(iface.prevPrimVertex(v));
        if (iface.isValidHedge(eprev) && iface.srcPoint(eprev) != pt)
            return eprev;

	// shouldn't be getting to here since by this time we
	// should have returned e itself
	UT_ASSERT_P(v != v0);
	if (v == v0)
	    break;
    }

    UT_ASSERT_MSG(0, "Control should not reach this pt!");
    return GEO_INVALID_HEDGE;
}

/// firstIncidentEdge and nextIncidentEdge, simply filter out
/// non-primary half-edges out of the results of firstIncidentHedge
/// and nextIncidentHedge

template <typename T>
GEO_Hedge
firstIncidentEdge(T &iface, GA_Offset point)
{
    GEO_Hedge e = iface.firstIncidentHedge(point);
    if (!iface.isValidHedge(e))
	return GEO_INVALID_HEDGE;

    while (!iface.isPrimary(e))
    {
	e = iface.nextIncidentHedge(e, point);

        UT_ASSERT_P(iface.isValidHedge(e));

	if (!iface.isValidHedge(e))
	    return GEO_INVALID_HEDGE;
    }
    return e;
}

template <typename T>
GEO_Hedge
nextIncidentEdge(T &iface, GEO_Hedge e, GA_Offset point)
{
    GEO_Hedge e0 = iface.nextIncidentHedge(e, point);
    if (!iface.isValidHedge(e0))
	return GEO_INVALID_HEDGE;

    while (!iface.isPrimary(e0))
    {
	e0 = iface.nextIncidentHedge(e0, point);
	if (!iface.isValidHedge(e0))
	    return GEO_INVALID_HEDGE;
    }
    return e0;
}

template <typename T>
GEO_Hedge
firstOutgoingHedge(T &iface, GA_Offset point)
{
    const GA_Detail *gdp = iface.getDetail();
    for (GA_Offset vtx = gdp->pointVertex(point); GAisValid(vtx);
	vtx = gdp->vertexToNextVertex(vtx))
    {
	GEO_Hedge e(vtx);
	if (iface.isValidHedge(e))
	    return e;
    }
    return GEO_INVALID_HEDGE;
}

template <typename T>
GEO_Hedge
nextOutgoingHedge(T &iface, GEO_Hedge e)
{
    const GA_Detail *gdp = iface.getDetail();
    GA_Offset vtx = iface.srcVertex(e);
    if (!GAisValid(vtx))
	return GEO_INVALID_HEDGE;

    GA_Offset point = gdp->vertexPoint(vtx);

    if (!GAisValid(point))
	return e;

    GA_Offset v0 = vtx;

    GA_Offset vtmp = GA_INVALID_OFFSET;
    for (GA_Offset v = GAisValid(vtmp = gdp->vertexToNextVertex(v0)) ? vtmp :
	    gdp->pointVertex(point); 1;
	v = GAisValid(vtmp = gdp->vertexToNextVertex(v)) ? vtmp :
	    gdp->pointVertex(point))
    {
	GEO_Hedge e0(v);
	if (iface.isValidHedge(e0))
	    return e0;

	if (v == v0)
	    break;
    }

    UT_ASSERT(0 && "Cotnrol should not reach this point!");
    return GEO_INVALID_HEDGE;
}

template <typename T>
GEO_Hedge
firstIncomingHedge(T &iface, GA_Offset point)
{
    const GA_Detail *gdp = iface.getDetail();
    for (GA_Offset vtx = gdp->pointVertex(point); GAisValid(vtx);
	vtx = gdp->vertexToNextVertex(vtx))
    {
	GA_Offset vprev =  iface.prevPrimVertex(vtx);
	if (GAisValid(vprev))
	{
	    GEO_Hedge eprev(vprev);
	    if (iface.isValidHedge(eprev))
		return eprev;
	}
    }
    return GEO_INVALID_HEDGE;
}

template <typename T>
GEO_Hedge
nextIncomingHedge(T &iface, GEO_Hedge e)
{
    const GA_Detail *gdp = iface.getDetail();
    GA_Offset vtx = iface.dstVertex(e);
    if (!GAisValid(vtx))
	return GEO_INVALID_HEDGE;

    GA_Offset point = gdp->vertexPoint(vtx);
    if (!GAisValid(point))
	return e;

    GA_Offset v0 = vtx;
    GA_Offset vtmp = GA_INVALID_OFFSET;
    for (GA_Offset v = GAisValid(vtmp = gdp->vertexToNextVertex(v0)) ? vtmp :
	    gdp->pointVertex(point); 1;
	v = GAisValid(vtmp = gdp->vertexToNextVertex(v)) ? vtmp :
	    gdp->pointVertex(point))
    {

	GA_Offset vprev = iface.prevPrimVertex(v);
	if (GAisValid(vprev))
	{
	    GEO_Hedge eprev(vprev);
	    if (iface.isValidHedge(eprev))
		return eprev;
	}

	if (v == v0)
	    break;
    }

    UT_ASSERT(0 && "Cotnrol should not reach this point!");
    return GEO_INVALID_HEDGE;

}

template<typename T>
GEO_Hedge
nextManifoldOutgoingHedge(T &iface, GEO_Hedge e)
{
    GEO_Hedge eprev = iface.prevPrimitiveHedge(e);
    GEO_Hedge eprevmate = iface.nextEquivalentHedge(eprev);
    if (eprevmate == eprev ||
	iface.nextEquivalentHedge(eprevmate) != eprev)
	return GEO_INVALID_HEDGE;

    if (iface.srcPoint(eprev) != iface.dstPoint(eprevmate))
	return GEO_INVALID_HEDGE;

    return eprevmate;
}

template<typename T>
GEO_Hedge
prevManifoldOutgoingHedge(T &iface, GEO_Hedge e)
{
    GEO_Hedge emate = iface.nextEquivalentHedge(e);
    if (emate == e || iface.nextEquivalentHedge(emate) != e)
	return GEO_INVALID_HEDGE;

    if (iface.srcPoint(e) != iface.dstPoint(emate))
	return GEO_INVALID_HEDGE;

    GEO_Hedge ematenext = iface.nextPrimitiveHedge(emate);
    if (!iface.isValidHedge(ematenext))
	return GEO_INVALID_HEDGE;
    return ematenext;
}

template<typename T>
GEO_Hedge
prevManifoldIncomingHedge(T &iface, GEO_Hedge e)
{
    GEO_Hedge enext = iface.nextPrimitiveHedge(e);
    GEO_Hedge enextmate = iface.nextEquivalentHedge(enext);
    if (enextmate == enext ||
	iface.nextEquivalentHedge(enextmate) != enext)
	return GEO_INVALID_HEDGE;
    if (iface.srcPoint(enext) != iface.dstPoint(enextmate))
    	return GEO_INVALID_HEDGE;
    return enextmate;
}

template<typename T>
GEO_Hedge
nextManifoldIncomingHedge(T &iface, GEO_Hedge e)
{
    GEO_Hedge emate = iface.nextEquivalentHedge(e);
    if (emate == e || iface.nextEquivalentHedge(emate) != e)
	return GEO_INVALID_HEDGE;
    if (iface.srcPoint(e) != iface.dstPoint(emate))
        	return GEO_INVALID_HEDGE;
    GEO_Hedge emateprev = iface.prevPrimitiveHedge(emate);
    if (!iface.isValidHedge(emateprev))
	return GEO_INVALID_HEDGE;
    return emateprev;
}

template<typename T>
GEO_Hedge
firstManifoldOutgoingHedge(T &iface, GEO_Hedge e)
{
    GEO_Hedge e0 = e;
    do
    {
	GEO_Hedge eprev = prevManifoldOutgoingHedge(iface, e);
	if (!iface.isValidHedge(eprev))
	    return e;
	e = eprev;
    } while (e0 != e);
    return e0;
}

template<typename T>
GEO_Hedge
firstManifoldIncomingHedge(T &iface, GEO_Hedge e)
{
    GEO_Hedge e0 = e;
    do
    {
	GEO_Hedge eprev = prevManifoldIncomingHedge(iface, e);
	if (!iface.isValidHedge(eprev))
	    return e;
	e = eprev;
    } while (e0 != e);
    return e0;
}

template <typename T>
SYS_FORCE_INLINE
fpreal
length(T &iface, GEO_Hedge e)
{
    auto gdp = iface.getDetail();
    return distance3d(gdp->getPos3(srcPoint(gdp, e)),
                      gdp->getPos3(dstPoint(iface, e)));
}


template <typename T>
SYS_FORCE_INLINE
fpreal vertexAngle(T &iface, GA_Offset v, UT_Vector3 *nml)
{
    const GA_Detail *gdp = iface.getDetail();
    GA_Offset v_prev, v_next;
    iface.getAdjacentPrimVertices(v, v_prev, v_next);
    if (!GAisValid(v_prev) || !GAisValid(v_next))
	return M_PI / 2.0;

    UT_Vector3 v1 = gdp->getPos3(gdp->vertexPoint(v_next));
    UT_Vector3 v0 = gdp->getPos3(gdp->vertexPoint(v));
    v1 -= v0;
    v0 = gdp->getPos3(gdp->vertexPoint(v_prev)) - v0;

    fpreal angle = UTangleBetween(v0, v1);
    if (!nml || dot(v1, cross(*nml, v0)) >= 0.0)
	return angle;

    return (2.0 * M_PI - angle);
}

SYS_FORCE_INLINE
UT_Vector3
srcPos3(const GA_Detail *gdp , GEO_Hedge h)
{
    return gdp->getPos3(srcPoint(gdp, h));
}

template <typename T>
SYS_FORCE_INLINE
UT_Vector3
dstPos3(T &iface, GEO_Hedge h)
{
    return iface.getDetail()->getPos3(dstPoint(iface, h));
}

template <typename T>
SYS_FORCE_INLINE
fpreal srcPrimitiveAngle(T &iface, GEO_Hedge e, UT_Vector3 *nml)
{
    return vertexAngle(iface, srcVertex(e), nml);
}

template <typename T>
SYS_FORCE_INLINE
fpreal dstPrimitiveAngle(T &iface, GEO_Hedge e, UT_Vector3 *nml)
{
    return vertexAngle(iface, dstVertex(iface, e), nml);
}

SYS_FORCE_INLINE
fpreal angleCos(const UT_Vector3 &u, const UT_Vector3 &v)
{
    fpreal udotv = dot(u, v);

    fpreal ulen = u.length();
    if (SYSequalZero(ulen))
	return udotv >= 0.0 ? 1.0 : -1.0;

    fpreal vlen = v.length();
    if (SYSequalZero(vlen))
	return udotv >= 0.0 ? 1.0 : -1.0;

    return udotv/(ulen * vlen);
}

template <typename T>
SYS_FORCE_INLINE
fpreal srcPrimitiveAngleCos(T &iface, GEO_Hedge e)
{
    auto gdp = iface.getDetail();
    UT_Vector3 pos = gdp->getPos3(srcPoint(gdp, e));
    return angleCos(gdp->getPos3(dstPoint(iface, e)) - pos,
                    gdp->getPos3(preSrcPoint(iface, e)) - pos);
}

template <typename T>
SYS_FORCE_INLINE
fpreal dstPrimitiveAngleCos(T &iface, GEO_Hedge e)
{
    auto gdp = iface.getDetail();
    UT_Vector3 pos = gdp->getPos3(dstPoint(iface, e));
    return angleCos(gdp->getPos3(postDstPoint(iface, e)) - pos,
		    gdp->getPos3(srcPoint(gdp, e)) - pos);
}

template <typename T>
GEO_Hedge
findHedgeWithEndpoints(T &iface, GA_Offset p0, GA_Offset p1)
{
    auto gdp = iface.getDetail();
    for (GA_Offset v = gdp->pointVertex(p0); GAisValid(v);
	v = gdp->vertexToNextVertex(v))
    {
	GA_Offset v_prev, v_next;
	iface.getAdjacentPrimVertices(v, v_prev, v_next);
	if (GAisValid(v_prev))
	    if (gdp->vertexPoint(v_prev) == p1)
		return GEO_Hedge(v_prev);
	if (GAisValid(v_next))
	    if (gdp->vertexPoint(v_next) == p1)
		return GEO_Hedge(v);
    }
    return GEO_INVALID_HEDGE;
}

template <typename T>
GA_Size
numIncidentEdges(T &iface, GA_Offset point)
{
    int res;
    GEO_Hedge e0 = iface.firstIncidentEdge(point);
    if (!iface.isValidHedge(e0))
	return 0;

    res = 1;
    GEO_Hedge e1 = iface.nextIncidentEdge(e0, point);
    while (e0 != e1)
    {
	res++;
	e1 = iface.nextIncidentEdge(e1, point);
    }
    return res;
}

template <typename T>
GA_Size
numIncidentHedges(T &iface, GA_Offset point)
{
    int res;
    GEO_Hedge e0 = iface.firstIncidentHedge(point);
    if (!iface.isValidHedge(e0))
	return 0;

    res = 1;
    GEO_Hedge e1 = iface.nextIncidentHedge(e0, point);
    while (e1 != e0)
    {
	res++;
	e1 = iface.nextIncidentHedge(e1, point);
    }
    return res;
}

}; // namespace geo_hedge_private


#endif
