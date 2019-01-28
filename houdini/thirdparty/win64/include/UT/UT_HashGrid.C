/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Utility Library (C++)
 *
 * COMMENTS:
 *	This class implements a grid-based data structure for
 *	storing spacial data (ie. points) with a fixed radius of
 *	influence.  It can be used to quickly determine the set
 *	of elements affecting a given point in space.
 */

#ifndef __UT_HashGrid_C__
#define __UT_HashGrid_C__

#include "UT_HashGrid.h"
#include "UT_Map.h"
#include "UT_IntArray.h"
#include "UT_Vector3.h"
#include "UT_Vector3Array.h"

// We limit the number of cells in any one dimension to
// 21 bits.  This gives us 63 bits used in total, so
// fits in the size of an int64.
#define		ut_CELL_SHIFT 21
#define		ut_CELL_SHIFT2 42
#define		ut_CELL_MASK (( ((int64) 1) << ut_CELL_SHIFT)-1)

static const int    ut_NUM_CELL_NEIGHBOURS = 27;

template <typename utPtr>
UT_HashGrid<utPtr>::UT_HashGrid(fpreal cellsize, const UT_Vector3 &origin,
			bool fullinit, unsigned int sz)
    : myOrigin(origin),
      myCellWidth(cellsize),
      myMaxElements(0),
      myGridSet(),
      myCellArray(0),		// We may want to initialize to sz/factor.
      myFullInit(fullinit),
      myPositions(sz),
      myElements(sz)
{
    myCellWidth2 = myCellWidth * myCellWidth;
    myInvCellWidth = 1.0f / myCellWidth;

    // This is likely user error, but the result is catastropic so
    // we just bail.
    if (myOrigin.isNan())
	myOrigin = 0;
}

template <typename utPtr>
int64
UT_HashGrid<utPtr>::getMemoryUsage(bool inclusive) const
{
    int64 memuse = inclusive ? sizeof(*this) : 0;
    memuse += myPositions.getMemoryUsage(false);
    memuse += myElements.getMemoryUsage(false);
    memuse += myCellArray.getMemoryUsage(false);
    for (exint i = 0; i < myCellArray.entries(); i++)
    {
        memuse += myCellArray(i).myIndices.getMemoryUsage(false);
    }
    memuse += myGridSet.getMemoryUsage(false);
    return memuse;
}

// Insert a new element in to the grid set.
template <typename utPtr>
bool
UT_HashGrid<utPtr>::insert(const UT_Vector3 &p, utPtr t)
{
    // Start by getting the cell this point should
    // be in, creating it if it does not already
    // exist.
    UT_HashGridCell<utPtr>	*cell;
    UT_HashGridCell<utPtr>	*othercell;
    int				 i, j, k;
    int				 newindex;
    int				 pointindex = myPositions.entries();
    
    myPositions.append(p);
    myElements.append(t);

    // Refuse to pollute with nans.
    if (p.isNan())
	return true;

    i = SYSfastFloor((p.x() - myOrigin.x()) * myInvCellWidth);
    j = SYSfastFloor((p.y() - myOrigin.y()) * myInvCellWidth);
    k = SYSfastFloor((p.z() - myOrigin.z()) * myInvCellWidth);

    cell = getCell(i, j, k, true);

    // Append the element to the cell's list.
    newindex = cell->myIndices.append(pointindex) + 1;

    if (newindex > myMaxElements)
	myMaxElements = newindex;

    if (myFullInit)
    {
	for (int dz = -1; dz <= 1; dz++)
	{
	    for (int dy = -1; dy <= 1; dy++)
	    {
		for (int dx = -1; dx <= 1; dx++)
		{
		    // Already did center.
		    if (!dx && !dy && !dz)
			continue;

		    othercell = getCell(i+dx, j+dy, k+dz, true);
		    newindex = othercell->myIndices.append(pointindex) + 1;

		    if (newindex > myMaxElements)
			myMaxElements = newindex;
		}
	    }
	}
    }

    return true;
}

// Insert a new element in to the grid set.
template <typename utPtr>
void
UT_HashGrid<utPtr>::insertBlanks(int nblanks)
{
    utPtr		blankelem = utPtr();
    UT_Vector3		zero(0, 0, 0);
    
    myPositions.appendMultiple(zero, nblanks);
    myElements.appendMultiple(blankelem, nblanks);
}

// Generates a list of elements close to the given point.
template <typename utPtr>
void
UT_HashGrid<utPtr>::findCloseElements(const UT_Vector3 &p,
			UT_Array<utPtr> &list) const
{
    list.entries(0);

    // Trivial to look up nans.
    if (p.isNan())
    {
	return;
    }
    if (myFullInit)
    {
	const UT_HashGridCell<utPtr>	*cell = getCell(p);
	int			 i, n;

	if (cell)
	{
	    n = cell->myIndices.entries();

	    list.setCapacityIfNeeded(n);

	    for (i = 0; i < n; i++)
	    {
		int index = cell->myIndices(i);
		if ((p - myPositions(index)).length2() < myCellWidth2)
		    list.append(myElements(index));
	    }
	}
    }
    else
    {
	int			 i, j, k;
	int			 ptnum, npts;
	const UT_HashGridCell<utPtr>	*currcell;

	i = SYSfastFloor((p.x() - myOrigin.x()) * myInvCellWidth);
	j = SYSfastFloor((p.y() - myOrigin.y()) * myInvCellWidth);
	k = SYSfastFloor((p.z() - myOrigin.z()) * myInvCellWidth);

	// We rely on the caller not wiping out the list, thus
	// there is no real advantage in pre-sizing it.

	for (int dz = -1; dz <= 1; dz++)
	{
	    for (int dy = -1; dy <= 1; dy++)
	    {
		for (int dx = -1; dx <= 1; dx++)
		{
		    currcell = getCell(i+dx, j+dy, k+dz);
		    if (currcell)
		    {
			npts = currcell->myIndices.entries();
			for (ptnum = 0; ptnum < npts; ptnum++)
			{
			    int index = currcell->myIndices(ptnum);

			    if ( (p - myPositions(index)).length2() < myCellWidth2)
				list.append(myElements(index));
			}
		    }
		}
	    }
	}
    }
}

// Generates a list of elements close to the given point.
template <typename utPtr>
void
UT_HashGrid<utPtr>::findAllCloseIndices(const UT_Vector3 &p,
			UT_IntArray &list) const
{
    list.entries(0);

    // Trivial to look up nans.
    if (p.isNan())
    {
	return;
    }
    if (myFullInit)
    {
	const UT_HashGridCell<utPtr>	*cell = getCell(p);

	if (cell)
	{
	    list.concat(cell->myIndices);
	}
    }
    else
    {
	int			 i, j, k;
	const UT_HashGridCell<utPtr>	*currcell;

	i = SYSfastFloor((p.x() - myOrigin.x()) * myInvCellWidth);
	j = SYSfastFloor((p.y() - myOrigin.y()) * myInvCellWidth);
	k = SYSfastFloor((p.z() - myOrigin.z()) * myInvCellWidth);

	// We rely on the caller not wiping out the list, thus
	// there is no real advantage in pre-sizing it.
	list.entries(0);

	for (int dz = -1; dz <= 1; dz++)
	{
	    for (int dy = -1; dy <= 1; dy++)
	    {
		for (int dx = -1; dx <= 1; dx++)
		{
		    currcell = getCell(i+dx, j+dy, k+dz);
		    if (currcell)
		    {
			list.concat(currcell->myIndices);
		    }
		}
	    }
	}
    }
}
template <typename utPtr>
void
UT_HashGrid<utPtr>::reset(unsigned int sz)
{
    myMaxElements = 0;

    myGridSet.clear();

    myCellArray.entries(0);
    myCellArray.setCapacity(0);

    myPositions.entries(0);
    myPositions.setCapacity(sz);

    myElements.entries(0);
    myElements.setCapacity(sz);
}

template <typename utPtr>
void
UT_HashGrid<utPtr>::reset(fpreal cellsize, const UT_Vector3 &origin,
			bool fullinit, unsigned int sz)
{
    reset(sz);

    myCellWidth = cellsize;
    myOrigin = origin;
    myFullInit = fullinit;
}

template <typename utPtr>
UT_HashGridCell<utPtr> *
UT_HashGrid<utPtr>::getCell(const UT_Vector3 &p, bool create)
{
    int				 xi, yi, zi; // x, y and z indices

    xi = SYSfastFloor((p.x() - myOrigin.x()) * myInvCellWidth);
    yi = SYSfastFloor((p.y() - myOrigin.y()) * myInvCellWidth);
    zi = SYSfastFloor((p.z() - myOrigin.z()) * myInvCellWidth);

    return getCell(xi, yi, zi, create);
}

template <typename utPtr>
const UT_HashGridCell<utPtr> *
UT_HashGrid<utPtr>::getCell(const UT_Vector3 &p) const
{
    int				 xi, yi, zi; // x, y and z indices

    xi = SYSfastFloor((p.x() - myOrigin.x()) * myInvCellWidth);
    yi = SYSfastFloor((p.y() - myOrigin.y()) * myInvCellWidth);
    zi = SYSfastFloor((p.z() - myOrigin.z()) * myInvCellWidth);

    return getCell(xi, yi, zi);
}

template <typename utPtr>
UT_HashGridCell<utPtr> *
UT_HashGrid<utPtr>::getCell(int xi, int yi, int zi, bool create)
{
    exint index;
    index = zi & ut_CELL_MASK;
    index <<= ut_CELL_SHIFT;
    index += yi & ut_CELL_MASK;
    index <<= ut_CELL_SHIFT;
    index += xi & ut_CELL_MASK;

    // Locate the cell number from the hash table.
    exint arrayindex = -1;
    auto &&it = myGridSet.find(index);
    if (it != myGridSet.end())
    {
	arrayindex = it->second;
    }

    if (arrayindex >= 0)
	return &myCellArray(arrayindex);
    else if (create)
    {
        exint cellindex = myCellArray.append();

	// Add this cell to the hash table.
        myGridSet[index] = cellindex;

	return &myCellArray(cellindex);
    }
    else
	return NULL;
}

template <typename utPtr>
const UT_HashGridCell<utPtr> *
UT_HashGrid<utPtr>::getCell(int xi, int yi, int zi) const
{
    exint index;
    index = zi & ut_CELL_MASK;
    index <<= ut_CELL_SHIFT;
    index += yi & ut_CELL_MASK;
    index <<= ut_CELL_SHIFT;
    index += xi & ut_CELL_MASK;

    // Locate the cell number from the hash table.
    exint arrayindex = -1;
    auto &&it = myGridSet.find(index);
    if (it != myGridSet.end())
    {
	arrayindex = it->second;
    }

    if (arrayindex >= 0)
	return &myCellArray(arrayindex);
    else
	return NULL;
}

#endif // __UT_HashGrid.C
