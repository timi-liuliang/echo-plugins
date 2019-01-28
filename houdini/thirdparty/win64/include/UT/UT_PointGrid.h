/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:    UT_PointGrid (UT Library, C++)
 *
 * COMMENTS: Point-in-grid acceleration structure
 */

#ifndef __UT_PointGrid_h__
#define __UT_PointGrid_h__

#include "UT_ValArray.h"
#include "UT_Vector3.h"
#include "UT_VectorTypes.h"
#include "UT_VoxelArray.h"
#include "UT_BitArray.h"

template <typename T>
class UT_PointGrid;

/// @brief Iteration over a range of keys returned from a UT_PointGrid query.
///
/// An iterator is used to iterate over the results of a query into
/// UT_PointGrid. of a given range.  The UT_PointGrid query functions return
/// iterators already rewound, so in most cases there is no need for
/// an explicit rewind() call.
template <typename T>
class UT_PointGridIterator
{
public:
    typedef typename T::indextype indextype;
    typedef typename T::keytype keytype;

    UT_PointGridIterator();

    // Use default copy constructor.
    // UT_PointGridIterator(const UT_PointGridIterator &src);

    /// @{
    /// Standard iterator methods.
    void rewind();
    bool atEnd() const;
    void advance();
    /// @}

    /// @{
    /// Query state of the iterator
    keytype getValue() const;
    keytype operator *() const { return getValue(); }
    /// @}

    /// The total number of keys over which this iterator will iterate.
    exint entries() const;

    /// The total number of occupied point grid voxels over which this
    /// iterator will iterate.
    exint numSequences() const { return mySequences->entries(); }

    /// Test to see whether the iterator is valid.
    bool isValid() const { return myGrid != 0; }

protected:

    struct UT_PointGridSequence
    {
        indextype begin;
        indextype end;
    };

    typedef UT_ValArray<UT_PointGridSequence> queuetype;

    UT_PointGridIterator(const UT_PointGrid<T> *grid,
                         queuetype &queue);

    void advanceSequence();

    queuetype                           *mySequences;
    indextype                            myCurIdx;
    indextype                            myCurEnd;
    exint                                myCurSequence;
    const UT_PointGrid<T>               *myGrid;
    friend class UT_PointGrid<T>;
};

/// @brief Iteration over a range of elements
/// This class implements a point-in-grid acceleration structure
/// that does a multithreaded build to handle large particle counts
/// and provides linear time lookups into the grid structure.
/// It's similar in spirit to particle acceleration techniques
/// used on GPUs:
/// Green, Simon. Particle Simulation using CUDA. CUDA 4.0 SDK
template <typename T>
class UT_PointGrid
{
public:

    /// Create a point grid with the specified accessor.
    /// @note The point grid is not valid until build() is called.
    UT_PointGrid(const T &accessor): myAccessor(accessor)
    {
        // The accessor's indextype must be signed so we can
        // store it in a UT_VoxelArray.
	SYS_STATIC_ASSERT(std::numeric_limits<indextype>::is_signed);
    }

    /// The index and key type for the point grid, derived from the accessor
    /// type.
    typedef typename T::indextype indextype;
    typedef typename T::keytype keytype;

    typedef exint grididxtype;

    typedef T accessortype;
    typedef UT_PointGridIterator<T> iterator;
    typedef typename UT_PointGridIterator<T>::queuetype queuetype;

    /// Constructs the point grid over the specified world-space coordinates
    /// and resolution.  The points will be read from the accessor provided
    /// when the object was created.
    /// @note This function asserts on canBuild(), to catch the case that
    /// the indextype or keytype types can not represent the number of points
    /// or the size of the grid.
    bool build(const UT_Vector3 &orig, const UT_Vector3 &size,
               int xres, int yres, int zres);

    /// Returns true if all point elements will fit into the specified
    /// indextype and keytpe data types without overflow, given the provided
    /// resolution.
    bool canBuild(int xres, int yres, int zres);

    /// Returns an iterator over the points occupying the grid voxel at the
    /// specified index.
    /// @note The iterator will be created rewound.
    /// @note The queue object should be created with createQueue(), and
    /// only one iterator at a time can be created with a particular queue.
    UT_PointGridIterator<T> getKeysAt(int x, int y, int z,
                                      queuetype &queue) const;

    /// Returns an iterator over all points that have invalid keys or did not
    /// fit in the bounding box of the grid.
    /// @note The iterator will be created rewound.
    /// @note The queue object should be created with createQueue(), and
    /// only one iterator at a time can be created with a particular queue.
    UT_PointGridIterator<T> getInvalidKeys(queuetype &queue) const;

    /// Queries the grid voxels within radius of position.
    /// Returns an iterator over the keys corresponding to the points.
    /// @note The iterator will be created rewound.  It may return keys
    /// from points that are in the voxels but outside the provided radius.
    /// The caller must cull these out manually if so desired.
    /// @note The queue object should be created with createQueue(), and
    /// only one iterator at a time can be created with a particular queue.
    UT_PointGridIterator<T> findCloseKeys(const UT_Vector3 &position,
                                          queuetype &queuetype,
                                          fpreal radius) const;

    /// Returns true if any voxels within radius of the position contain
    /// points.
    bool hasCloseKeys(const UT_Vector3 &position, fpreal radius) const;

    /// Returns the total number of points.
    exint entries() const { return myIdxKeys.entries(); }

    /// Returns number of points contained in a given voxel.
    exint entriesAt(int x, int y, int z) const;

    /// Convert a world space position to a grid index.
    /// Always returns true if boundsCheck is false, else returns true only
    /// if the index is valid for the grid
    bool posToIndex(UT_Vector3 position, int &x, int &y, int &z,
                    bool boundsCheck = true) const;

    /// Returns true if the specified index is valid for the underlying grid.
    bool isValidIndex(int x, int y, int z) const
    {
        return myBegins.isValidIndex(x, y, z);
    }

    /// Returns the total memory usage (in bytes) of the grid.
    int64 getMemoryUsage() const;

    /// Returns the key at the specified index.
    keytype getKeyAt(indextype idx) const { return myIdxKeys(idx).key;}

    /// X-resolution of the underlying grid.
    int      getXRes() const { return myBegins.getXRes(); }
    /// Y-resolution of the underlying grid.
    int      getYRes() const { return myBegins.getYRes(); }
    /// Z-resolution of the underlying grid.
    int      getZRes() const { return myBegins.getZRes(); }
    /// Resolution of the underlying grid along the specified axis.
    int      getRes(int axis) const { return myBegins.getRes(axis); }

    /// Returns the voxel size of the underlying grid.
    const UT_Vector3 &getVoxelSize() const { return myVoxelSize; }

    /// Creates a queue that can be used for querying this point grid.  Any
    /// number of queueus can be created for a point grid, but only one
    /// iterator can use a particular queue at a time.  Typically each thread
    /// in a multithreaded operation will have its own queue.
    static queuetype *createQueue() { return new queuetype; }

    /// Destroys a queue created with createQueue().
    static void destroyQueue(queuetype *queue) { delete queue; }

protected:
    static const indextype INVALIDINDEX;
    static const grididxtype INVALIDGRIDIDX;

    // Gets the key begin/end indices for the given voxel.
    // Returns false is none exist or the indices are invalid.
    bool getKeysAt(int x, int y, int z,
                   indextype &begin, indextype &end) const;


    // The most commons use of this struct has an exint grididxtype
    // and a uint32 key; rather than have the compiler extend this to
    // 16 bytes, we try to pack to 12.  Any slower access times are more
    // than made up for by much faster sorting, which is memory bound.
    SYS_PACKED_STRUCT_HINT_BEGIN(UT_GridIdxKey, 4)
    {
        grididxtype grididx;
        keytype key;
    };
    SYS_PACKED_STRUCT_HINT_END

    grididxtype indexToGridIdx(int x, int y, int z);

    void gridIdxToIndex(grididxtype idx, int &x, int &y, int &z);

    bool calcBounds(const UT_Vector3 &pos, fpreal radius,
                    int &xmin, int &ymin, int &zmin,
                    int &xmax, int &ymax, int &zmax) const;

    // STL-style comparator for sorting by grididx.
    class ut_isGridIdxLess
    {
    public:
        inline bool operator()(const UT_GridIdxKey &a,
                               const UT_GridIdxKey &b) const
        {
            return a.grididx < b.grididx;
        }
    };

    bool shouldMultithread() const
    {
        return myAccessor.entries() > 1000;
    }

    THREADED_METHOD1(UT_PointGrid<T>, shouldMultithread(),
                    computeGridIdx,
                    UT_ValArray<bool> &, tileoccupied);

    void computeGridIdxPartial(UT_ValArray<bool> &tileoccupied,
                               const UT_JobInfo &info);

    THREADED_METHOD3(UT_PointGrid<T>, shouldMultithread(),
                    findIdxRanges,
                    UT_ValArray<UT_Vector3T<int> > &, bboxmins,
                    UT_ValArray<UT_Vector3T<int> > &, bboxmaxs,
                    indextype, numValidKeys);

    void findIdxRangesPartial(UT_ValArray<UT_Vector3T<int> > &bboxmins,
                              UT_ValArray<UT_Vector3T<int> > &bboxmaxs,
                              indextype numValidKeys,
                              const UT_JobInfo &info);

    THREADED_METHOD1(UT_PointGrid<T>, tileoccupied.entries() > 20,
                     uncompressTiles,
                     const UT_ValArray<bool> &, tileoccupied);

    void uncompressTilesPartial(const UT_ValArray<bool> &tileoccupied,
                                const UT_JobInfo &info);

    UT_ValArray<UT_GridIdxKey>      myIdxKeys;
    UT_VoxelArray<indextype>        myBegins, myEnds;
    T                               myAccessor;
    UT_Vector3                      myOrig;
    UT_Vector3                      mySize;
    UT_Vector3                      myVoxelSize;
    UT_Vector3                      myInvVoxelSize;
    UT_Vector3T<int>                myBBoxMin, myBBoxMax;

};

/// @brief Generic UT_Vector3Array based accessor for UT_PointGrid.
///
/// This class provides the required functions to serve as an accessor for
/// building a UT_PointGrid over the points provided in the UT_Vector3Array
/// passed to the constructor.
/// See GEO_PointGrid for an accessor over the points in a GEO_Detail.
template <typename INDEX, typename KEY>
class UT_PointGridVector3ArrayAccessor
{
public:
    // indextype must be signed.
    typedef INDEX indextype;
    typedef KEY keytype;

    /// Invalid key value.
    static const keytype INVALIDKEY;

    /// Create an accessor for the provided array of positions and keys.
    /// @note This class will not take ownership of the provided arrays;
    /// it merely provides an interface for UT_PointGrid::build().
    UT_PointGridVector3ArrayAccessor(const UT_Vector3Array &positions,
                                     const UT_ValArray<keytype> &keys,
                                     const UT_BitArray *valid = NULL):
                                     myV3Array(positions),
                                     myKeyArray(keys),
                                     myValidArray(valid)
    {
        UT_ASSERT(myV3Array.entries() == myKeyArray.entries());
        UT_ASSERT(!myValidArray ||
                   myValidArray->size() == myKeyArray.entries());
    }

    /// Returns the number of points.
    indextype   entries() const
    {
        return myV3Array.entries();
    }

    /// Returns the position for a given index.
    UT_Vector3  getPos(indextype idx) const
    {
        return myV3Array(idx);
    }

    /// Gets the key for a given index.
    keytype     getKey(indextype idx) const
    {
        if (!myValidArray)
            return myKeyArray(idx);
        return myValidArray->getBit(idx) ? myKeyArray(idx) : INVALIDKEY;
    }

    /// Returns the maximum possible key value, used by
    /// UT_PointGrid::canBuild().
    exint       maxKeyValue() const
    {
        return std::numeric_limits<keytype>::max();
    }

    int64 getMemoryUsage() const
    {
        // The accessor doesn't own any data.
        return sizeof(UT_PointGridVector3ArrayAccessor<INDEX, KEY>);
    }

    void        build() {}

protected:
    const UT_Vector3Array       &myV3Array;
    const UT_ValArray<keytype>  &myKeyArray;
    const UT_BitArray           *myValidArray;
};

typedef UT_PointGrid<UT_PointGridVector3ArrayAccessor<int32, exint> > UT_Vector3Grid;
typedef UT_Vector3Grid::queuetype UT_Vector3GridQueue;

#include "UT_PointGridImpl.h"

#endif
