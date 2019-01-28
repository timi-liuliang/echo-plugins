/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU Library (C++)
 *
 */

#ifndef __GU_QuadLayout_h__
#define __GU_QuadLayout_h__

#include "GU_API.h"
#include "GU_Detail.h"

#include <GEO/GEO_HedgeInterface.h>

#include <GA/GA_Edge.h>
#include <GA/GA_EdgeGroup.h>

#include <UT/UT_Map.h>
#include <UT/UT_ParallelUtil.h>
#include <UT/UT_UniquePtr.h>
#include <UT/UT_TriangleMesh.h>


class GU_API GU_QuadLayout
{
public:
    explicit		 GU_QuadLayout(const GU_Detail *gdp,
			    	const GA_PrimitiveGroup *group = nullptr,
			    	const GA_EdgeGroup *separator_edges = nullptr,
			    	const GA_Attribute *poly_island = nullptr,
			    	const GEO_DetachedHedgeInterface *hip
			    		= nullptr);


    struct Degree
    {
	Degree(int nq, int s) : num_quads(nq), separations(s) { }
	int		 num_quads;
	int 		 separations;
    };

    enum ArcOrientation { UNASSIGNED = 0, ROW, COL };
    using PolyFunctor = std::function<void(GA_Offset)>;

    SYS_FORCE_INLINE
    bool  		 isNode(GA_Offset vtx) const
			    { return getFlags(vtx, NODE_VTX) != 0; }

    SYS_FORCE_INLINE
    bool  		 isOnArc(GEO_Hedge h) const
			    { return getFlags(srcVertex(h), ARC_HDG) != 0; }

    SYS_FORCE_INLINE
    int		 	 hedgeBlock(GEO_Hedge h) const
    			    { return polyBlock(hedgePoly(h)); }

    SYS_FORCE_INLINE
    bool		 isValid(GEO_Hedge h) const
    			    { return h != GEO_INVALID_HEDGE; }

    SYS_FORCE_INLINE
    bool		 isValidArc(int a) const
    			    { return a >= 0 && isValid(arcStart(a)); }

    SYS_FORCE_INLINE
    bool		 isValidBlock(int b) const;

    // Test whether any of the four block arcs are identified with another.
    SYS_FORCE_INLINE
    bool		 isFreeBlock(int b) const;

    SYS_FORCE_INLINE
    ArcOrientation 	 arcOrientation(int a) const
    			    { return myArcOrientation(a); }

    SYS_FORCE_INLINE
    GA_Offset		 srcVertex(GEO_Hedge h) const
    			    { return myHip->srcVertex(h); }

    SYS_FORCE_INLINE
    GA_Offset		 srcPoint(GEO_Hedge h) const
    			    { return myHip->srcPoint(h); }

    SYS_FORCE_INLINE
    GA_Offset		 dstPoint(GEO_Hedge h) const
    			    { return myHip->dstPoint(h); }

    SYS_FORCE_INLINE
    GEO_Hedge		 lnext(GEO_Hedge h) const
    			    { return myHip->lnext(h); }

    SYS_FORCE_INLINE
    GEO_Hedge		 lprev(GEO_Hedge h) const
    			    { return myHip->lprev(h); }

    SYS_FORCE_INLINE
    GA_Offset		 vertexPoly(GA_Offset vtx) const
    			    { return myGdp->vertexPrimitive(vtx); }

    SYS_FORCE_INLINE
    GA_Offset		 hedgePoly(GEO_Hedge h) const
    			    { return vertexPoly(srcVertex(h)); }

    SYS_FORCE_INLINE
    int			 polyIsland(GA_Offset poly) const
    			    { return myPolyIsland.isValid()
				     ? myPolyIsland.get(poly) : 0; }

    SYS_FORCE_INLINE
    int			 hedgeIsland(GEO_Hedge h) const
    			    { return polyIsland(hedgePoly(h)); }

    // sym() restricted to the island of the hedge.
    // Note that here we override the usual half-edge interface sym behaviour
    // of returning the half-edge itself for boundary half-edges and instead
    // return GEO_INVALID_HEDGE. Note also that if the user supplies us with
    // a half-edge interface that doesn't cover the supplied group polygons,
    // the group is implicitly overridden by the half-edge interface.
    SYS_FORCE_INLINE
    GEO_Hedge 		 sym(GEO_Hedge h) const
    {
        GEO_Hedge h_sym = myHip->sym(h);
	if (h_sym == h || myHip->sym(h_sym) != h)
            return GEO_INVALID_HEDGE;

	if (getFlags(h, BOUNDARY_HDG) || !getFlags(h_sym, QUAD_VTX))
	    return GEO_INVALID_HEDGE;

	if (hedgeIsland(h) != hedgeIsland(h_sym))
	    return GEO_INVALID_HEDGE;

	return h_sym;
    };

    SYS_FORCE_INLINE
    GEO_Hedge 		 onext(GEO_Hedge h) const { return sym(lprev(h)); }

    SYS_FORCE_INLINE
    bool		 isQuad(GA_Offset poly) const
    {
	return myGdp->getPrimitiveTypeId(poly) == GEO_PRIMPOLY
	       && myGdp->getPrimitiveVertexCount(poly) == 4
	       && myGdp->getPrimitiveClosedFlag(poly);
    };

    int 		 numArcs() const { return int(myArcStart.size()); }

    int			 numBlocks() const
    			    { return int(myArcStart.size() / 4); }

    int 		 polyBlock(GA_Offset poly) const
    			    { return myPolyBlock.get(poly); }

    void		 orientArcs();

    int 		 foreachBlockPoly(int b, const PolyFunctor &func) const;

    GEO_Hedge		 hedgeSucc(GEO_Hedge h) const
    			    { return quadSucc(h, ARC_HDG); }

    SYS_FORCE_INLINE
    int 		 blockArc(int b, int a = 0) const { return 4 * b + a; }

    SYS_FORCE_INLINE
    int 		 arcBlock(int a) const { return a / 4; }

    SYS_FORCE_INLINE
    int			 arcSym(int a) const { return myArcSym(a); }

    SYS_FORCE_INLINE
    int			 arcLnext(int a) const
    			    { return (a % 4 == 3) ? a - 3 : a + 1; }

    SYS_FORCE_INLINE
    int			 arcLprev(int a) const
    			    { return (a % 4 == 0) ? a + 3 : a - 1; }

    SYS_FORCE_INLINE
    GEO_Hedge		 arcStart(int a) const { return myArcStart(a); }

    SYS_FORCE_INLINE
    GEO_Hedge		 arcEnd(int a) const;

    SYS_FORCE_INLINE
    int			 blockComponent(int b) const { return myBlockComp(b); }

    SYS_FORCE_INLINE
    int			 numBlockComponents() const
    			    { return myNumBlockComponents; }

    using HedgeArray = UT_Array<GEO_Hedge>;
    // Traces the quad line from h. Returns the last hedge before hitting
    // a boundary, or GEO_INVALID_HEDGE if we loop back to h. If hedges
    // passed, the traversed edges are appended to hedges (not cleared).
    GEO_Hedge		 trace(GEO_Hedge h, HedgeArray *hedges = nullptr) const;

private:

    using HedgeInterface = GEO_DetachedHedgeInterface;
    using HedgeInterfaceUptr = UT_UniquePtr<HedgeInterface>;
    using PrimitiveGroupUptr = UT_UniquePtr<GA_PrimitiveGroup>;
    using ArcOrientationArray = UT_Array<ArcOrientation>;

    // Flags indicate for each vertex properties relative to the restriction
    // of the geometry to the quads in the given group. Some flags treat the
    // vertex as a vertex in restricted geometry (VTX) and others view it as
    // it's outgoing half-edge (HDG).

    enum VertexFlag
    {
        NONE		 = 0,
        CATEGORIZED	 = 1,		// determined singular or regular
        QUAD_VTX	 = 1 << 1,	// vertex of a quad
        INTERIOR_VTX	 = 1 << 2, 	// interior vertex
        SINGULAR_VTX	 = 1 << 3,	// singular (equiv. not regular)
        NODE_VTX	 = 1 << 4,	// layout node (block corner)
        BOUNDARY_HDG	 = 1 << 5,	// mesh boundary hedge
        ARC_HDG		 = 1 << 6	// layout arc (separatrix) hedge
    };

    void		 buildArcTopology(const GA_OffsetArray &block_corners);

    // Given a tri hedge i, return the tri hedge j that succeeds it
    // in a path, or -1 if none.
    GEO_Hedge 		 quadSucc(GEO_Hedge h, uint32 stop_mask) const;

    SYS_FORCE_INLINE
    uint32		 getFlags(GA_Offset vtx) const
    			    { return uint32(myVertexFlags.get(vtx)); }

    SYS_FORCE_INLINE
    uint32		 getFlags(GA_Offset vtx, uint32 mask) const
    			    { return (uint32(myVertexFlags.get(vtx)) & mask); }

    SYS_FORCE_INLINE
    uint32		 getFlags(GEO_Hedge h, uint32 mask) const
    			    { return getFlags(srcVertex(h), mask); }

    SYS_FORCE_INLINE
    void		 setFlags(GA_Offset vtx, uint32 mask) const
    			    { myVertexFlags.set(vtx, getFlags(vtx) | mask); }

    SYS_FORCE_INLINE
    void		 setFlags(GEO_Hedge h, uint32 mask) const
    			    { setFlags(srcVertex(h), mask); }

    SYS_FORCE_INLINE
    void		 clearFlags(GA_Offset vtx, uint32 mask) const
    			    { myVertexFlags.set(vtx, getFlags(vtx) & (!mask)); }

    GA_RWHandleI	 myVertexFlags;
    GA_RWHandleI	 myPolyBlock;
    GA_ROHandleI	 myPolyIsland;

    const GU_Detail	*myGdp					= nullptr;

    const
    HedgeInterface	*myHip					= nullptr;
    HedgeInterfaceUptr	 myOwnHip;

    int			 myNumBlockComponents			= 0;

    ArcOrientationArray	 myArcOrientation;
    HedgeArray 		 myArcStart;
    UT_IntArray 	 myArcSym;
    UT_IntArray		 myBlockComp;
};


bool
GU_QuadLayout::isValidBlock(int b) const
{
    return isValidArc(blockArc(b, 0)) && isValidArc(blockArc(b, 1))
	   && isValidArc(blockArc(b, 2)) && isValidArc(blockArc(b, 3));
}

bool
GU_QuadLayout::isFreeBlock(int b) const
{
    if (!isValidBlock(b))
        return false;

    for (int i = 0; i < 4; i++)
    {
        int a_i_sym = arcSym(blockArc(b, i));
	for (int j = 0; j < 4; j++)
	    if (a_i_sym == blockArc(b, j))
	        return false;
    }
    return true;
}


#endif
