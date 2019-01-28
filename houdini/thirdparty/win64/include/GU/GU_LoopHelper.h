/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        GU_LoopHelper.h
 *
 * COMMENTS:
 *	This is a helper class for creating nice edge/prim/point loops.
 */

#ifndef __GU_LoopHelper_h__
#define __GU_LoopHelper_h__

#include "GU_API.h"
#include "GU_SelectType.h"
#include "GU_PathFinder.h"
#include "GU_Detail.h"
#include <GEO/GEO_HedgeInterface.h>

enum GU_LoopType
{
    GU_LOOP_PARTIAL = 0,    	// Running from start to end element
    GU_LOOP_EXTENDED,		// Partial, maximally extended quad walks at ends
    GU_LOOP_CLOSED		// Closed loop
};

class GU_API GU_LoopHelper
{
public:
     GU_LoopHelper();
    ~GU_LoopHelper();

    // New edge loop methods to later fully replace the old ones
    bool		 primLoop(GA_Index start, GA_Index end,
    				GU_LoopType loop_type,
    				UT_Array<GEO_SHedge> &loop);

    bool		 edgeLoop(GEO_Hedge start, GEO_Hedge end,
    				GU_LoopType loop_type,
				bool allow_ring_instead,
    				UT_Array<GEO_SHedge> &loop);

    bool		 edgeRing(GEO_Hedge start, GEO_Hedge end,
    				GU_LoopType loop_type,
    				UT_Array<GEO_SHedge> &loop);

    bool		 pointLoop(GA_Index start, GA_Index end,
				GU_LoopType loop_type,
				UT_Array<GEO_SHedge> &loop);

    bool		 vertexLoop(GA_Index start, int svtx, GA_Index end,
				int evtx, GU_LoopType loop_type,
				UT_Array<GEO_SHedge> &loop);

    // Sets the active detail, should only be called by setActiveNode.
    void		 setActiveDetail(const GU_Detail *gdp,
    				const GA_Attribute *uv_attrib = nullptr);

    // Returns our active detail.  May be NULL.
    const GU_Detail	*getActiveDetail() const;

    // Access our cached ring zero.  The first time this is called with a new
    // detail it will create a new ring zero.
    const UT_Array<GA_OffsetArray>
			*getActiveRingZero();

    // Access our cached hedge interface.  The first time this is called with
    // a new detail it will create a new hedge interface.
    const GEO_DetachedHedgeInterface
			*getActiveHedgeInterface();

    const GA_Attribute	*getActiveUVAttrib();

private:

    using LoopFinderUptr = UT_UniquePtr<GU_EdgeLoopFinder>;
    using RingFinderUptr = UT_UniquePtr<GU_EdgeRingFinder>;
    using HedgeInterfaceUptr = UT_UniquePtr<const GEO_DetachedHedgeInterface>;
    using OffsetArrayUptr = UT_UniquePtr<const UT_Array<GA_OffsetArray> >;
    using SHedgeArray =  UT_Array<GEO_SHedge>;

    GU_EdgeRingFinder  	*getLeftRingFinder();
    GU_EdgeRingFinder 	*getRightRingFinder();

    GU_EdgeLoopFinder  	*getLeftLoopFinder();
    GU_EdgeLoopFinder 	*getRightLoopFinder();

    void		 addPathToPointGroup(const SHedgeArray &path,
				GA_PointGroup *grp);

    void		 addPathToVertexGroup(const SHedgeArray &path,
				GA_VertexGroup *grp);

    void		 addPathToPrimitiveGroup(const SHedgeArray &path,
				GA_PrimitiveGroup *grp);

    // Returns true (and returns the shortest partial edge ring in "path") if
    // the the start and end edges given by ssh and esh suggest an edge ring
    // selection more than they do an edge loop one. This is the case exactly
    // when ssh and esh are distinct and there is a perfect partial edge
    // from start and end but while no perfect edge loops can be found between
    // them. If false is returned the shortest perfect edge loop between start
    // and end is returned as evidence.
    bool		 shouldFindEdgeRingInstead(GEO_SHedge ssh,
        		        GEO_SHedge esh, UT_Array<GEO_SHedge> &path);

    bool		 findPartialEdgeLoop(UT_Array<GEO_SHedge> &loop);
    bool		 findPartialEdgeRing(UT_Array<GEO_SHedge> &loop);

    bool		 findEdgeLoop(UT_Array<GEO_SHedge> &loop,
        		        GU_LoopType loop_type,
        		        bool same_start_and_end = false);

    bool		 findEdgeRing(UT_Array<GEO_SHedge> &loop,
        		        GU_LoopType loop_type,
        		        bool is_for_prim_loop = false);

    exint		 myActiveDetailId;
    exint		 myActiveDetailMetaCount;
    exint 		 myUVAttribDataId;


    LoopFinderUptr	 myLeftLoopFinder;
    LoopFinderUptr	 myRightLoopFinder;
    RingFinderUptr	 myLeftRingFinder;
    RingFinderUptr	 myRightRingFinder;

    const GU_Detail	*myActiveDetail;
    const GA_Attribute	*myUVAttrib;

    HedgeInterfaceUptr	 myActiveHedgeInterface;
    OffsetArrayUptr	 myActiveRingZero;
};

#endif
