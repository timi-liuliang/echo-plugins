/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_OffsetMatrix.h ( GA Library, C++)
 *
 * COMMENTS:	Class to store a matrix of offsets/indices based on the
 *		GA_Offset/GA_Index types.
 */

#ifndef __GA_OffsetMatrix__
#define __GA_OffsetMatrix__

#include "GA_API.h"
#include "GA_OffsetList.h"
#include "GA_Types.h"

#include <SYS/SYS_Types.h>


class GA_Defragment;
class GA_LoadMap;
class GA_SaveMap;

class UT_JSONParser;
class UT_JSONWriter;
class UT_MemoryCounter;

/// GA_OffsetMatrix implements an array of GA_Offsets.
/// Copy-on-write is used to reduce memory usage and make the copying of a
/// GA_OffsetMatrix an inexpensive operation.
///
/// See also: @ref JSON-GA_OffsetMatrix, GA_OffsetList
class GA_API GA_OffsetMatrix 
{
public:
     explicit GA_OffsetMatrix() : myRows(0), myCols(0) {}
	     ~GA_OffsetMatrix()	{ clear(); }

    /// Report memory usage
    int64	getMemoryUsage(bool inclusive) const
    { return (inclusive ? sizeof(*this) : 0) + myList.getMemoryUsage(false); }

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    /// If inclusive is true, the size of this object is counted,
    /// else only memory owned by this object is counted.
    /// If this is pointed to by the calling object, inclusive should be true.
    /// If this is contained in the calling object, inclusive should be false.
    /// (Its memory was already counted in the size of the calling object.)
    void countMemory(UT_MemoryCounter &counter, bool inclusive) const;

    /// clear removes all of the entries
    void	clear()
		{
		    myList.clear();
		    myRows = 0;
		    myCols = 0;
		}

    /// Returns the used rows of the matrix (always <= to getRowCapacity())
    GA_Size	rows() const	{ return myRows; }
    GA_Size	getRows() const	{ return myRows; }
    /// Returns the used columns of the matrix (always <= to getRowCapacity())
    GA_Size	cols() const	{ return myCols; }
    GA_Size	getCols() const	{ return myCols; }

    /// Reserve capacity for a number of rows/columns.  The capacity may not be
    /// decreased.
    void	reserve(GA_Size rows, GA_Size cols)
		{ myList.reserve(rows*cols); }

    /// Set the array size
    void	setEntries(GA_Size rows, GA_Size cols);

    /// Add a single entry (may grow array)
    GA_Size	appendRow()	{ return insertRow(myRows); }
    /// Add a single entry (may grow array)
    GA_Size	insertRow(GA_Size index);
    /// Remove the entry at the given offset
    GA_Size	removeRow(GA_Size i);

    /// Add a single entry (may grow array)
    GA_Size	appendCol()	{ return insertCol(myCols); }
    /// Add a single entry (may grow array)
    GA_Size	insertCol(GA_Size index);
    /// Remove the entry at the given offset
    GA_Size	removeCol(GA_Size i);


    /// Set the index to the value
    void	set(GA_Size row, GA_Size col, GA_Offset value)
		{ myList.set(row*myCols+col, value); }
    /// The the value at the index
    GA_Offset	get(GA_Size row, GA_Size col) const
		{ return myList.get(row*myCols+col); }

    /// Convenience () operator to access the list entries
    GA_Offset	operator()(GA_Size row, GA_Size col) const
		{ return get(row, col); }

    /// Cyclically shift the rows and/or columns
    void	cycle(GA_Size rowshift, GA_Size colshift);

    /// Returns the first linear index in the matrix whose value
    /// is equal to value, or -1 if not present
    GA_Size     find(GA_Offset value) const
    { return myList.find(value); }

    /// @{
    /// Swap offset values for defragmentation process.  When defragmenting,
    /// offsets will move.  This method will update all references to offsets
    /// to their new values.  This returns the number of values updated.
    GA_Size	swapOffsetValues(const GA_Defragment &defrag)
		{ return myList.swapOffsetValues(defrag); }
    /// @}

    /// @section JSON-GA_OffsetMatrix JSON Schema: GA_OffsetMatrix
    /// @code
    /// {
    ///     "name"              : "GA_OffsetMatrix",
    ///     "description"       :
    ///         "A 2D matrix of offsets/indicies.
    ///          Each item in the array represents a row of the matrix."
    ///     "type"              : "array",
    ///     "items"             : { "$ref" : "GA_OffsetList" }
    /// }
    /// @endcode
    /// @see @ref JSON_FileFormat

    /// Save the offsets by doing the mapping to the points in the save map
    bool	jsonPointMatrix(UT_JSONWriter &w, const GA_SaveMap &save) const;
    /// Save the offsets by doing the mapping to the vertices in the save map
    bool	jsonVertexMatrix(UT_JSONWriter &w, const GA_SaveMap &save) const;
    /// Save the offsets by doing the mapping to the primitives in the save map
    bool	jsonPrimitiveMatrix(UT_JSONWriter &w, const GA_SaveMap &save) const;

    /// Load a point list from a JSON stream
    bool	jsonPointMatrix(UT_JSONParser &p, const GA_LoadMap &load);
    /// Load a vertex list from a JSON stream
    bool	jsonVertexMatrix(UT_JSONParser &p, const GA_LoadMap &load);
    /// Load a primitive list from a JSON stream
    bool	jsonPrimitiveMatrix(UT_JSONParser &p, const GA_LoadMap &load);

    /// Generic load given a load offset
    bool	jsonLoad(UT_JSONParser &p, GA_Offset load_offset);

private:
    GA_OffsetList	myList;
    GA_Size		myRows;
    GA_Size		myCols;
};

#endif

