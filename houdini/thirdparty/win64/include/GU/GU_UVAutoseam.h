/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP library (C++)
 *
 * COMMENTS:	UVFlatten SOP
 */

#ifndef __GU_AutoSeam_h__
#define __GU_AutoSeam_h__

#include "GU_API.h"

#include "GU_Detail.h"
#include <UT/UT_UniquePtr.h>

class UT_Classifier;
class GU_QuadLayout;

class GU_API GU_UVAutoseam
{
    using HedgeInterface = GEO_DetachedHedgeInterface;
public:
    explicit 		 GU_UVAutoseam(const GU_Detail *gdp,
			 	const GA_PrimitiveGroup *grp);

    			~GU_UVAutoseam();


    void 		 generateSeams(fpreal l21_threshold,
    				fpreal cov_threshold,
			 	fpreal boundary_weight,
			 	fpreal avoidance_weight,
			 	const GA_EdgeGroup *precuts,
		     	    	const GA_EdgeGroup *noncuts,
    				const GA_Attribute *avoid_attr,
    				GA_EdgeGroup *seams);


    void 		 getSymmetryLineEdges(GA_EdgeGroup *grp);

    const
    HedgeInterface 	*getHedgeInterface() const { return myHip; }

    const
    GU_Detail		*getDetail() const { return myGdp; }

    void		 getIslandNumbers(GA_RWHandleI &island_num);

private:
    using HedgeArray = UT_Array<GEO_Hedge>;

    SYS_FORCE_INLINE
    GEO_Hedge 		 sym(GEO_Hedge h) const { return myHip->sym(h); }

    SYS_FORCE_INLINE
    GEO_Hedge 		 lnext(GEO_Hedge h) const { return myHip->lnext(h); }

    SYS_FORCE_INLINE
    GEO_Hedge 		 lprev(GEO_Hedge h) const { return myHip->lprev(h); }


    SYS_FORCE_INLINE
    GA_Offset 		 srcVertex(GEO_Hedge h) const
    			    { return myHip->srcVertex(h); }

    SYS_FORCE_INLINE
    GA_Offset 		 dstVertex(GEO_Hedge h) const
    			    { return myHip->dstVertex(h); }

    SYS_FORCE_INLINE
    GA_Offset 		 srcPoint(GEO_Hedge h) const
    			    { return myHip->srcPoint(h); }

    SYS_FORCE_INLINE
    GA_Offset 		 dstPoint(GEO_Hedge h) const
    			    { return myHip->dstPoint(h); }

    SYS_FORCE_INLINE
    GA_Offset 		 hedgePoly(GEO_Hedge h) const
    			    { return myGdp->vertexPrimitive(srcVertex(h)); }

    SYS_FORCE_INLINE
    UT_Vector3R 	 getPos3(GA_Offset pt) const
    			    { return myGdp->getPos3(pt); }

    SYS_FORCE_INLINE
    fpreal 		 length(GEO_Hedge h) const
			    {
				return (getPos3(srcPoint(h))
					- getPos3(dstPoint(h))).length(); }


    struct IslandInfo;
    class Partition;

    void 		 extractIslandBoundaries(GA_EdgeGroup *seams);

    //////////////////////
    void 		 findSymmetryLines();

    template <typename T>
    void 		 growIslands(const T &merge_cost_func,
		     	    	fpreal cost_threshold,
		     	    	fpreal boundary_weight,
		     	    	fpreal avoidance_weight,
		     	    	const GA_EdgeGroup *precuts,
		     	    	const GA_Attribute *avoid_attr);

    template <typename T>
    void 		 smoothIslands(const T &merge_cost_func,
		     	    	fpreal cost_threshold,
		     	    	fpreal boundary_weight,
		     	    	fpreal avoidance_weight,
		     	    	const GA_EdgeGroup *precuts,
		     	    	const GA_Attribute *avoid_attr);

    void		 premergeNonSeams(const GA_EdgeGroup *noncuts);
    void 		 premergeQuadBlocks(const GA_EdgeGroup *cuts);


    fpreal 		 calcPolyArea(GA_Offset poly,
    				UT_Vector3R *normal) const;

    // Returns true when all the primitives in the give group are polygons.
    bool 		 indexGroupPolys(const GA_PrimitiveGroup *grp);

    GA_Range 		 polys() const
    {
	return myPolyIndex.isValid()
	       ? GA_Range(myGdp->getIndexMap(GA_ATTRIB_PRIMITIVE),
			  GA_OffsetList(myIndexPoly))
	       : myGdp->getPrimitiveRange();
    }

    SYS_FORCE_INLINE
    bool 		 isPoly(GA_Offset prim) const
    {
	return myGdp->getPrimitiveTypeId(prim) == GEO_PRIMPOLY
	       && myGdp->getPrimitiveClosedFlag(prim);;
    }

    SYS_FORCE_INLINE
    bool 		 isQuad(GA_Offset prim) const
    {
	return isPoly(prim) && myGdp->getPrimitiveVertexCount(prim) == 4;
    }

    SYS_FORCE_INLINE
    GEO_Hedge 		 twin(GEO_Hedge h) const
    {
        if (myTwinMap.isInvalid())
            return GEO_INVALID_HEDGE;

        GA_Offset tvtx(myTwinMap.get(srcVertex(h)));
	return GEO_Hedge(tvtx);
    }

    SYS_FORCE_INLINE
    bool 		 isOnSymmetryLine(GEO_Hedge h) const
    			    { return twin(h) == sym(h); }

    SYS_FORCE_INLINE
    int			 numPolys() const
    			    { return myPolyIndex.isValid()
				     ? int(myIndexPoly.size())
				     : int(myGdp->getNumPrimitives()); }

    SYS_FORCE_INLINE
    int			 polyIndex(GA_Offset poly) const
			    { return myPolyIndex.isValid()
				     ? myPolyIndex.get(poly)
				     : int(myGdp->primitiveIndex(poly)); }

    SYS_FORCE_INLINE
    GA_Offset		 polyAtIndex(exint i) const
    			    { return myPolyIndex.isValid()
				     ? myIndexPoly(i)
    			    	     : myGdp->primitiveOffset(GA_Index(i)); }

    static fpreal	 calcL21MergeCost(const IslandInfo &i,
    				const IslandInfo &j);

    static fpreal	 calcCovarianceMergeCost(const IslandInfo &i,
    				const IslandInfo &j);

    using PartitionUptr = UT_UniquePtr<Partition>;



    const GU_Detail 	*myGdp;
    HedgeInterface 	*myHip					= nullptr;

    GA_RWHandleI	 myTwinMap;
    PartitionUptr	 myPartition;

    UT_Array<fpreal>	 myGaussianCurvature;
    GU_QuadLayout	*myQuadLayout;

    // Group two-way reindexing.
    GA_RWHandleI	 myPolyIndex;
    GA_OffsetArray	 myIndexPoly;
};
#endif
