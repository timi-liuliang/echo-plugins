/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_Sort.h (GU Library, C++)
 *
 * COMMENTS:	Declarations of helper classes for sorting geometry
 */

#include <UT/UT_KDTree.h>

// A KD-Tree to sort the model's point or primitive positions
class GU_SortKDTree : public UT_KDTree {
public:
    GU_SortKDTree(size_t size)
	: UT_KDTree(3, size)
	, myPos(size)
    {
	myPos.entries(size);
	setMaxLeafNodes(4);
	setBalancer(UT_KD_CENTROID);
    }

    void	 setPos(int idx, const UT_Vector3 &pos) { myPos(idx) = pos; }

    const int	*getIndices() const	{ return myList; }

    virtual int		 comparePosition(int idx0, int idx1, int dim) const
    {
	return myPos(idx0)[dim] < myPos(idx1)[dim] ? -1 : 1;
    }
    virtual const float	*getP(int idx) const { return myPos(idx).vec; }

private:
    UT_Vector3Array	myPos;
};

