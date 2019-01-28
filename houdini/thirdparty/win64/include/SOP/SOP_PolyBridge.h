/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP_PolyBridge.h ( SOP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SOP_PolyBridge__
#define __SOP_PolyBridge__

#include "SOP_API.h"
#include "SOP_Node.h"
#include <UT/UT_StringStream.h>

#include <GU/GU_PolyBridge.h>

class GU_Decompose;

class SOP_API SOP_PolyBridge : public SOP_Node
{
public:

			 SOP_PolyBridge(OP_Network *net, const char *,
				 OP_Operator *entry);

    virtual 		~SOP_PolyBridge() {}

    virtual OP_ERROR	 cookInputGroups(OP_Context &context, int alone = 0);
    virtual bool	 updateParmsFlags();

    static OP_Node	*myConstructor(OP_Network  *net, const char *name,
				OP_Operator *entry);

    static PRM_Template	 myTemplateList[];

    static const char	*theSrcCentroidAttribName;
    static const char	*theDstCentroidAttribName;
    static const char	*theSrcNormalAttribName;
    static const char	*theDstNormalAttribName;

protected:
    virtual OP_ERROR	 cookMySop(OP_Context &context);
    virtual const char  *inputLabel(unsigned idx) const;

    void		 readPairingsFromParms();
    void		 addChainToDegreeMap(GA_OffsetArray &chain,
				int chain_id,
				bool is_src);

    int			 cacheSrcAndDstChains(GU_Decompose *src_comps,
				GU_Decompose *dst_comps,
				bool reverse_src,
				bool reverse_dst);

    void		 removeAmbiguousPairings();

    void		 findChainPairings(int src_chain_idx,
				int dst_chain_idx,
				UT_Array<std::pair<int, int> > &pairings);

    void	 	 updateAffectedNormals(const GA_PrimitiveGroup *prims);

private:
    void		 saveEdgeArrayToDetailAttrib(const GA_OffsetArray &offs,
				const char *attrib_name);

    void		 updateAffectedNormals(bool add_vertex_normals,
				GA_PrimitiveGroup *side_prims,
				fpreal cusp_angle_degrees,
				bool cusp_src, bool cusp_dst,
				GA_EdgeGroup *src_edges,
				GA_EdgeGroup *dst_edges);

    static int	 	 initExplicitSrcDirStatic(void *, int idx,
				fpreal t, const PRM_Template *);

    static int	 	 initExplicitDstDirStatic(void *, int idx,
				fpreal t, const PRM_Template *);

    int	 	 	 initExplicitSrcDir(fpreal t);
    int	 	 	 initExplicitDstDir(fpreal t);

    struct ChainPointInfo
    {
	ChainPointInfo() : chain_number(-1), index_in_chain(-1),
	    is_src_chain(false), degree(0)
	{}

	ChainPointInfo(int chain, int idx, bool is_src) :
	    chain_number(chain), index_in_chain(idx), is_src_chain(is_src),
	    degree(1)
	{}

	void		 bumpDegree() { degree++; }

	int 		 chain_number;
	int		 index_in_chain;
	int		 degree;
	bool		 is_src_chain;
    };

    typedef UT_Map<GA_Index, ChainPointInfo>	ChainPointMap;
    typedef UT_Map<GA_Index, int>		DegreeMap;
    typedef GA_OffsetArray			Chain;

    typedef UT_Array<Chain>			ChainArray;

    typedef		 std::pair<GA_Index, GA_Index> IndexPair;
    UT_Array<IndexPair>  myPairList;

    typedef GU_PolyBridge::SpinePositioning 	SpinePositioning;
    typedef GU_PolyBridge::SpineSampleMethod 	SpineSampleMethod;



    GU_Detail		*myGdp;
    const GA_EdgeGroup	*mySrcEdges;
    const GA_EdgeGroup 	*myDstEdges;

    const GA_EdgeGroup	*mySrcSplitEdges;
    const GA_EdgeGroup 	*myDstSplitEdges;
    const GA_PointGroup	*mySrcSplitPoints;
    const GA_PointGroup *myDstSplitPoints;


    const
    GA_PrimitiveGroup	*mySrcPrims;

    const
    GA_PrimitiveGroup	*myDstPrims;

    ChainPointMap	 myChainPointMap;

    ChainArray		 mySrcChains;
    ChainArray		 myDstChains;

    GA_OffsetArray	 mySrcRefVtxs;
    GA_OffsetArray	 myDstRefVtxs;

    UT_IntArray		 mySrcChainsClosed;
    UT_IntArray		 myDstChainsClosed;

    UT_IntArray		 mySrcChainsInnerBoundary;
    UT_IntArray		 myDstChainsInnerBoundary;

    UT_Vector3		 myLastSrcLoopNormal;
    UT_Vector3		 myLastDstLoopNormal;

    UT_StringStream	 myWarningMessages;

    GU_DetailGroupPair	 myDetailGroupPair;

    GEO_DetachedHedgeInterface
			*myHip;

};

#endif

