/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_Csup.h (GU Library, C++)
 *
 * COMMENTS:
 *
 */


#ifndef __GU_Decompose_h__
#define __GU_Decompose_h__

#include "GU_API.h"
#include <UT/UT_Classifier.h>
#include <UT/UT_Map.h>
#include <UT/UT_Set.h>
#include <GA/GA_Handle.h>
#include <SYS/SYS_Types.h>

class GU_Detail;
class GEO_DetachedHedgeInterface;
class GA_Group;
class GEO_Hedge;

class GA_VertexGroup;
class GA_EdgeGroup;
class GA_PrimitiveGroup;
class GA_PointGroup;

class GU_API GU_Decompose
{
public:

    enum DecompositionType
    {
	MAXIMAL_MANIFOLD = 0,
	EULER_TRAVERSAL
    };

			 GU_Decompose(const GU_Detail *gdp,
				const GA_Group *grp,
				const GA_Group *split_grp = NULL,
				bool split_all = false,
				const GEO_DetachedHedgeInterface *hip = NULL,
				DecompositionType dectype = MAXIMAL_MANIFOLD);

			~GU_Decompose();


    int			 fillComponentIDAttribute(GA_RWHandleI &attrib);

    /// Returns the number of maximal manifold components.
    int			 getNumComponents();

    /// Returns the total number of boundary chains (cycles) in all the
    /// components.
    /// Note that some components may have no boundary chains and other may
    /// have multiple. For a 2-manifold maximal patch, i.e. one made of
    /// primitives, each boundary chain is a closed cycle of edges. For
    /// a 1-manifold maximal patch, i.e. oen made of edges, the boundary chain
    /// (when non-empty) consists of the two endpoints.
    int			 getNumBoundaries();

    /// Returns the number of boundary components of a given component.
    int			 getComponentNumBoundaries(int comp);

    /// Returns index of the primitive component to which the idx-th
    /// boundary cycle belongs.
    int			 getBoundaryComponentID(int idx);

    bool		 isComponentClosed(int idx);

    GA_Offset		*getComponent(int idx, int &length);
    int			 getComponentSize(int idx);
    int			 getComponentSizesSum();

    int			 getBoundarySize(int idx);
    /// Returns a pointer to an array of half-edges which form the
    /// boundary cycle of numbered idx among all boundary cycles.
    GA_Offset		*getBoundary(int idx, int &length);

    int			 getComponentBoundarySize(int comp, int i);
    /// Returns a pointer to an array of half-edges which form the
    /// idx-th boundary cycle among all boundary chains for the patch comp.
    /// (first index is zero).
    GA_Offset		*getComponentBoundary(int comp, int idx, int &length);

    int			 getBoundarySizesSum();

    GA_GroupType	 getGroupType()
			    { return myGroupType; }

    struct Iterator
    {
			 Iterator(GU_Decompose *dec, int k, int b) :
			     myIdx(0)
			 {
			     myOffsets = b < 0 ?
				 dec->getComponent(k, mySize) :
				 k < 0 ?
				     dec->getBoundary(b, mySize):
				     dec->getComponentBoundary(k, b, mySize);

			     myClosed = b < 0 ?
				 dec->isComponentClosed(k) : true;
			 }

			 Iterator(const Iterator &it) :
			     myIdx(it.myIdx), mySize(it.mySize),
			     myOffsets(it.myOffsets), myClosed(it.myClosed) {}

			 Iterator(int i, int s, GA_Offset *offs, bool closed) :
			     myIdx(i), mySize(s), myOffsets(offs),
			     myClosed(closed) {}

	inline bool	 operator==(const Iterator &it) const
			    { return myIdx == it.myIdx;  }

	inline bool	 operator!=(const Iterator &it) const
			    { return myIdx != it.myIdx;}

	/// Pre-increment operator
	Iterator 	&operator++() { advance(); return *this; }
	Iterator 	&operator--() { retreat(); return *this; }

	inline bool	 atStart() const { return (myIdx == 0); }
	inline bool	 atEnd() const { return (myIdx == mySize); }

	inline void	 advance() { myIdx += (myIdx < mySize); }
	inline void	 retreat() { myIdx -= (myIdx > 0); }

	inline void	 rewind() { myIdx = 0; }
	inline void	 forward() { myIdx = mySize; }

	Iterator	 first()
			    { return Iterator(0, mySize, myOffsets, myClosed); }
	Iterator	 last()
			    { return Iterator(mySize - 1, mySize, myOffsets,
					      myClosed); }

	Iterator	 next()
			    { return ((myIdx < mySize - 1 || !myClosed) ?
					Iterator(myIdx + 1, mySize, myOffsets,
						 myClosed) : first());
			    }

	Iterator	 prev()
			    { return ((myIdx > 0 || !myClosed) ?
					Iterator(myIdx - 1, mySize, myOffsets,
						 myClosed) : last()); }

	GA_Offset 	 operator*() const
			    { return ((myIdx >= 0 && myIdx <= mySize) ?
				myOffsets[myIdx] : GA_INVALID_OFFSET); }

	bool		 isClosed() const { return myClosed; }


	int		 myIdx, mySize;
	GA_Offset	*myOffsets;
	bool		 myClosed;
    };

    Iterator		 getComponentIterator(int comp)
			    { return Iterator(this, comp, -1); }

    Iterator		 getBoundaryIterator(int bd)
			    { return Iterator(this, -1, bd); }

    Iterator		 getComponentBoundaryIterator(int comp, int bd)
			    { return Iterator(this, comp, bd); }

private:
    void		 setRanksInGroup();
    void		 buildPrimComponents();

    int			 rankInGroup(GA_Offset prim_off,
				bool check_in_group = false) const;

    bool		 isPrimComponentClosed(int idx) { return false; }
    int			 extractPatches();
    int			 extractPrimBoundaries();
    int			 componentRank(GEO_Hedge h);

    bool		 isPathClosed(int idx);
    GA_Offset		*getPath(int idx, int &length);
    GA_Offset		*getPrimComponent(int idx, int &length);
    GA_Offset		*getPrimComponentBoundary(int idx, int &length);

    int			 extractPaths();
    int			 extractMaximalPaths();
    int			 extractEulerPaths();

    int			 getPrimComponentSizesSum();
    int			 getEdgeComponentSizesSum();

    void		 reversePath(int i);

    const GU_Detail 	*myGdp;
    GA_GroupType	 myGroupType;
    bool		 mySplitAll;

    bool		 myOwnHedgeInterface;

    UT_Map<GA_Index, exint>
			 myPrimRankInGroup;

    UT_Classifier 	 myPrimClasses;
    UT_IntArray 	 myPatchRank;
    UT_Array<GEO_Hedge>	 myBoundaryHedges;
    UT_Set<GA_Offset>	 myBoundarySrcVertices;

    int			 myNumPatches;

    UT_IntArray 	 myPatchBoundaryStart;
    UT_IntArray 	 myPatchBoundaryEnd;
    UT_IntArray 	 myBoundaryPatchID;
    UT_IntArray 	 myPatchFirstBoundary;
    UT_IntArray 	 myPatchLastBoundary;

    GA_OffsetArray	 myPatchPrims;
    UT_IntArray		 myPatchStart;
    UT_IntArray		 myPatchEnd;
    UT_Array<GEO_Hedge>  myCycleHedges;

    const GA_EdgeGroup	*mySplitEdges;
    const GA_PointGroup	*mySplitPoints;

    int			 myNumPaths;
    UT_IntArray 	 myPathStarts;
    UT_IntArray 	 myPathEnds;
    UT_Array<bool>	 myPathClosed;
    GA_OffsetArray	 myPathPoints;

    DecompositionType	 myDecompositionType;

    const GA_EdgeGroup 	*myEdges;

    const GA_VertexGroup		*myHedges;
    const GA_PrimitiveGroup 		*myPrims;
    const GEO_DetachedHedgeInterface 	*myHip;


};
#endif
