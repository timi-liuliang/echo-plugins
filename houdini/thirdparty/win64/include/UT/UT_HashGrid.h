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
 *
 */

#ifndef __UT_HashGrid_h__
#define __UT_HashGrid_h__

#ifdef WIN32
    #pragma warning(disable:4251)
    #pragma warning(disable:4275)
#endif

#include "UT_Map.h"
#include "UT_IntArray.h"
#include "UT_Array.h"
#include "UT_Vector3.h"
#include "UT_Vector3Array.h"

/// A class that defines a cubic grid cell.
template <typename utPtr>
class UT_HashGridCell {
    public:
	UT_HashGridCell() : myIndices(50)
	{
	}

	~UT_HashGridCell() {}

	UT_IntArray		 myIndices;
};

template <typename utPtr>
class UT_HashGrid {

public:
    /// Constructor takes a floating point value to indicate
    /// the size of each grid cell to be stored by the, and
    /// optionally takes a size parameter to determine the
    /// initial size of the grid cell array.  The constructor
    /// also requires an origin about which the point set will
    /// be roughly centred.  fullinit determines whether or
    /// not a point is associated with all surrounding grid
    /// cells immediately upon addition, or if a point should
    /// only ever be associated with one grid cell.  In the
    /// second case, each query requires that 27 grid cells be
    /// checked instead of 1.  This is essentially a trade off
    /// between the cost of building the data structure and the
    /// cost of making point queries.  If the number of point
    /// queries is likely to outnumber the number of points
    /// added to the tree, then fullinit = true is recommended.
    UT_HashGrid(fpreal cellsize, const UT_Vector3 &origin,
			bool fullinit, unsigned int sz = 0);

    /// Trivial destructor
    ~UT_HashGrid() {}

    int64			 getMemoryUsage(bool inclusive) const;

    int				 getNumCells() const { return myCellArray.entries(); }

    /// Inserts a new element in to the set using the given
    /// point to determine that element's location in the grid.
    /// Returns true if successful.
    bool			 insert(const UT_Vector3 &p, utPtr t);

    /// Inserts blank elements so our index list will match an external
    /// structure with holes.
    void			 insertBlanks(int nblanks);

    /// Finds all elements close to the given point and returns
    /// a list of those elements.
    inline void			 findCloseElements(const UT_Vector3 &p,
				    UT_Array<utPtr> &list) const;

    /// Finds all close indices, does no distance lookups!  This is
    /// every index within +/- one cell
    inline void			 findAllCloseIndices(const UT_Vector3 &p,
				    UT_IntArray &idxlist) const;

    /// Resets the structure by clearing all elements from it
    /// and optionally reinitializing the cell array to the
    /// given size.
    void			 reset(unsigned int sz = 0);

    /// Additional reset function that also changes the cell
    /// width and origin for the structure.
    void			 reset(fpreal cellsize,
				    const UT_Vector3 &origin,
				    bool fullinit,
				    unsigned int sz = 0);

    /// These lookup our element and positions using the hash grid's index
    /// which corresponds to the order in which you called insert.
    inline UT_Vector3		 getPos(int idx) const { return myPositions(idx); }
    inline utPtr		 getElement(int idx) const { return myElements(idx); }

    inline fpreal		 getCellWidth() const { return myCellWidth; }

private:
    typedef UT_Array<UT_HashGridCell<utPtr> > ut_CellArray;

    /// Helper functions used to get a grid cell from the
    /// hash table, or generate a new one if necessary.
    UT_HashGridCell<utPtr>	*getCell(const UT_Vector3 &p,
					bool create);
    UT_HashGridCell<utPtr>	*getCell(int i, int j, int k,
					bool create);
    const UT_HashGridCell<utPtr>*getCell(const UT_Vector3 &p) const;
    const UT_HashGridCell<utPtr>*getCell(int i, int j, int k) const;

private:
    UT_Vector3			 myOrigin;
    fpreal			 myCellWidth;
    fpreal			 myCellWidth2;
    fpreal			 myInvCellWidth;

    int				 myMaxElements;

    UT_Map<exint,exint>		 myGridSet;
    ut_CellArray		 myCellArray;

    bool			 myFullInit;

    UT_Vector3Array		 myPositions;
    UT_ValArray<utPtr>		 myElements;

};

#if defined( WIN32 ) || defined( LINUX ) || defined(MBSD) || defined(GAMEOS)
    #include "UT_HashGrid.C"
#endif

#endif
