/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:    UT_PointGridImpl (UT Library, C++)
 *
 * COMMENTS:
 *
 */
#ifndef __UT_PointGridImpl_h__
#define __UT_PointGridImpl_h__

#include "UT_PointGrid.h"
#include "UT_ParallelUtil.h"
#include "UT_Interrupt.h"
// Named not to conflict with other DO_TIMING macros.
// #define UT_PointGridImpl_DO_TIMING

#ifdef UT_PointGridImpl_DO_TIMING
#include "UT_StopWatch.h"
#endif

template <typename T>
UT_PointGridIterator<T>::UT_PointGridIterator()
    : myGrid(0), mySequences(0), myCurIdx(-1), myCurSequence(-1)
{
}

template <typename T>
UT_PointGridIterator<T>::UT_PointGridIterator(const UT_PointGrid<T> *grid,
                                              queuetype &queue)
    : myGrid(grid), mySequences(&queue)
{
    rewind();
}

template <typename T>
void
UT_PointGridIterator<T>::rewind()
{
    myCurIdx = -1;
    myCurEnd = 0;
    myCurSequence = -1;
    advanceSequence();
}

template <typename T>
bool
UT_PointGridIterator<T>::atEnd() const
{
    return (myCurSequence < 0);
}

template <typename T>
void
UT_PointGridIterator<T>::advanceSequence()
{
    // Move to beginning of next sequence if possible.
    if (mySequences && ++myCurSequence < mySequences->entries())
    {
        myCurIdx = (*mySequences)(myCurSequence).begin;
        myCurEnd = (*mySequences)(myCurSequence).end;
    }
    else
    {
        myCurSequence = -1;
    }
}

template <typename T>
void
UT_PointGridIterator<T>::advance()
{
    if (++myCurIdx == myCurEnd)
        advanceSequence();
}

template <typename T>
typename UT_PointGridIterator<T>::keytype
UT_PointGridIterator<T>::getValue() const
{
    UT_ASSERT(!atEnd());
    return myGrid->getKeyAt(myCurIdx);
}

template <typename T>
exint
UT_PointGridIterator<T>::entries() const
{
    if (!mySequences)
        return 0;
    exint total = 0;
    for (exint i = 0; i < mySequences->entries(); i++)
        total += (*mySequences)(i).end - (*mySequences)(i).begin;
    return total;
}

template <typename INDEX, typename KEY>
const KEY
UT_PointGridVector3ArrayAccessor<INDEX, KEY>::INVALIDKEY = std::numeric_limits<KEY>::max();

template<typename T>
const typename UT_PointGrid<T>::indextype
UT_PointGrid<T>::INVALIDINDEX = std::numeric_limits<typename UT_PointGrid<T>::indextype>::max();

template<typename T>
const typename UT_PointGrid<T>::grididxtype
UT_PointGrid<T>::INVALIDGRIDIDX = std::numeric_limits<typename UT_PointGrid<T>::grididxtype>::max();

template <typename T>
bool
UT_PointGrid<T>::posToIndex(UT_Vector3 pos, int &x, int &y, int &z,
                            bool boundsCheck) const
{
    // Convert from world space to unit cube.
    // We go from the position in the unit cube into the index.
    // The center of cells must map to the exact integer indices.
    pos -= myOrig;
    pos *= myInvVoxelSize;

    // The centers of cells are now mapped .5 too high.  Ie, the true
    // center of cell index (0,0,0) would be (0.5,0.5,0.5)  This, however,
    // is exactly what we want for rounding.
    x = SYSfastFloor(pos.x());
    y = SYSfastFloor(pos.y());
    z = SYSfastFloor(pos.z());

    if (!boundsCheck)
        return true;
    // Determine if out of bounds.
    return isValidIndex(x, y, z);
}

// Convert an x, y, z index to a grid index.
template<typename T>
typename UT_PointGrid<T>::grididxtype
UT_PointGrid<T>::indexToGridIdx(int x, int y, int z)
{
    return (grididxtype(z) * myBegins.getYRes() + y) * myBegins.getXRes() + x;
}

// Convert a grid index to an x, y, z index.
template<typename T>
void
UT_PointGrid<T>::gridIdxToIndex(typename UT_PointGrid<T>::grididxtype idx,
                                int &x, int &y, int &z)
{
    x = idx % myBegins.getXRes();
    idx -= x;
    idx /= myBegins.getXRes();
    y = idx % myBegins.getYRes();
    idx -= y;
    idx /= myBegins.getYRes();
    z = idx;
}

template <typename T>
int64
UT_PointGrid<T>::getMemoryUsage() const
{
    return sizeof(UT_PointGrid<T>) +
       myIdxKeys.getMemoryUsage() +
       myBegins.getMemoryUsage() +
       myEnds.getMemoryUsage() +
       myAccessor.getMemoryUsage();
}

// Create grid indices for each point provided by the accessor.
template <typename T>
void
UT_PointGrid<T>::computeGridIdxPartial(UT_ValArray<bool> &tileoccupied,
                                       const UT_JobInfo &info)
{
    UT_Interrupt    *boss = UTgetInterrupt();
    int x, y, z;
    int tileidx;
    UT_ValArray<bool> occupied;
    indextype i, end, npts = myAccessor.entries();

    occupied.entries(tileoccupied.entries());
    occupied.zero();

    for(info.divideWork(npts, i, end); i < end; i++)
    {
        if (!(i & 127))
            if (boss->opInterrupt())
                break;
        // Always store key so we can access it even for invalid grid indices,
        // i.e. out-of-bounds points.
        myIdxKeys(i).key = myAccessor.getKey(i);
        // Store the grid index for each valid key, else invalid.
        if (myIdxKeys(i).key != myAccessor.INVALIDKEY &&
            posToIndex(myAccessor.getPos(i), x, y, z))
        {
            myIdxKeys(i).grididx = indexToGridIdx(x, y, z);
            // Keep track of which tiles will be occupied for later
            // uncompression.
            tileidx = myBegins.indexToLinearTile(x, y, z);
            occupied(tileidx) = true;
        }
        else
        {
            myIdxKeys(i).grididx = INVALIDGRIDIDX;
        }
    }

    // Copy from local occupied array to global.  This is faster than writing
    // directly to global array during above loop.
    for (int i=0; i < occupied.entries(); i++)
        if (occupied(i))
            tileoccupied(i) = true;

}

// Uncompress the occupied tiles in the myBegins, myEnds arrays.
template <typename T>
void
UT_PointGrid<T>::uncompressTilesPartial(const UT_ValArray<bool> &tileoccupied,
                                        const UT_JobInfo& info)
{
    UT_Interrupt    *boss = UTgetInterrupt();
    int i, end;
    for(info.divideWork(tileoccupied.entries(), i, end); i < end; i++)
    {
        if (!(i & 127))
            if (boss->opInterrupt())
                break;
        if (tileoccupied(i))
        {
            myBegins.getLinearTile(i)->uncompress();
            myEnds.getLinearTile(i)->uncompress();
        }
    }
}

// Find the sequential begin / end runs of grid indices within the sorted
// grididx list, storing the results in the myBegins, myEnds arrays.
template <typename T>
void
UT_PointGrid<T>::findIdxRangesPartial(UT_ValArray<UT_Vector3T<int> > &bboxmins,
                                      UT_ValArray<UT_Vector3T<int> > &bboxmaxs,
                                      indextype numKeys,
                                      const UT_JobInfo &info)
{
    UT_Interrupt    *boss = UTgetInterrupt();
    grididxtype curidx, nextidx;
    int x, y, z;
    indextype i, end;

    // Keep track of the (voxel-space) bounding box for this thread.
    UT_Vector3T<int> bboxmin, bboxmax;
    // NOTE: Make sure not to return early so these are properly stored
    // into the return value lists at the bottom of the function,
    bboxmin = std::numeric_limits<int>::max();
    bboxmax = std::numeric_limits<int>::min();

    info.divideWork(numKeys, i, end);
    // Make sure we have work to do.
    if (i < end)
    {
        curidx = myIdxKeys(i).grididx;
        UT_ASSERT(curidx != INVALIDGRIDIDX);
        // If first index is part of a partial run that will be handled by the
        // previous thread, just move past it.
        if (i > 0 && curidx == myIdxKeys(i-1).grididx)
        {
            while(i < end && curidx == myIdxKeys(i).grididx)
                i++;
        }
    }
    // Make sure we (still) have work to do.
    if (i < end)
        curidx = myIdxKeys(i).grididx;
    else
        curidx = INVALIDGRIDIDX;

    bool startingrun = true;
    // Loop until numkeys to follow partial runs into next thread's work,
    // as long as we don't hit an invalid id, in which case we're done
    while(i < numKeys && curidx != INVALIDGRIDIDX)
    {
        if (!(i & 127))
            if (boss->opInterrupt())
                break;
        // If starting new run, store the current start value.
        if (startingrun)
        {
            // Store start value in destination voxel.
            gridIdxToIndex(curidx, x, y, z);
            myBegins.setValue(x, y, z, i);
            startingrun = false;
        }
        // Look ahead to next index.
        nextidx = i < numKeys - 1 ? myIdxKeys(i + 1).grididx : INVALIDGRIDIDX;
        // If the provided numKeys is correct, we should never encounter
        // INVALIDGRIDIDX except when we explcitly use it as end-of-run signal.
        UT_ASSERT(i >= numKeys - 1 || nextidx != INVALIDGRIDIDX);
        // Check if we hit a change in grid index, signalling end of run.
        if (curidx != nextidx)
        {
            // Store end value into the same destination voxel index we
            // found for start.
            myEnds.setValue(x, y, z, i + 1);
            // Expand bounding box
            bboxmin.x() = SYSmin(bboxmin.x(), x);
            bboxmax.x() = SYSmax(bboxmax.x(), x);
            bboxmin.y() = SYSmin(bboxmin.y(), y);
            bboxmax.y() = SYSmax(bboxmax.y(), y);
            bboxmin.z() = SYSmin(bboxmin.z(), z);
            bboxmax.z() = SYSmax(bboxmax.z(), z);
            // If this is last element of thread's work, or if we followed this
            // run into next thread's work then we're done.
            if (i >= end - 1)
                break;
            // Start new run.
            curidx = nextidx;
            startingrun = true;
        }
        i++;
    }

    // Resize list of bboxes if needed.  Do this at the end to reduce the
    // chance of threads waiting.
    {
        UT_AutoJobInfoLock  autolock(info);

        if (bboxmins.entries() < info.numJobs())
        {
            bboxmins.entries(info.numJobs());
            bboxmaxs.entries(info.numJobs());
        }
    }
    bboxmins(info.job()) = bboxmin;
    bboxmaxs(info.job()) = bboxmax;
}

// Build the grid with the specified size. resolution.
template <typename T>
bool
UT_PointGrid<T>::build(const UT_Vector3 &orig, const UT_Vector3 &size,
                       int xres, int yres, int zres)
{
    UT_Interrupt    *boss = UTgetInterrupt();

#ifdef UT_PointGridImpl_DO_TIMING
    UT_Timer t("UT_PointGrid::build");
#endif
    {
#ifdef UT_PointGridImpl_DO_TIMING
    UT_Timer t("build accessor");
#endif
    myAccessor.build();
    }

    UT_ASSERT(canBuild(xres, yres, zres));
    indextype npts;
    UT_ValArray<bool> tileoccupied;

    myOrig = orig;
    mySize = size;
    myVoxelSize = mySize / UT_Vector3(xres, yres, zres);
    myInvVoxelSize = 1.0 / myVoxelSize;

    // Initialize underlying begin / end grids.
    myBegins.size(xres, yres, zres);
    myEnds.size(xres, yres, zres);
    myBegins.constant(INVALIDINDEX);
    myEnds.constant(INVALIDINDEX);

    // Assume no tiles occupied to start.
    tileoccupied.entries(myBegins.numTiles());
    tileoccupied.zero();

    // Compute grid idx for each entry provided by myAccesor.
    npts = myAccessor.entries();
    myIdxKeys.entries(npts);
    {
#ifdef UT_PointGridImpl_DO_TIMING
    UT_Timer t("computeGridIdxOffsets");
#endif
    computeGridIdx(tileoccupied);
    }
    UT_ASSERT(myIdxKeys.entries() == npts);

    if (boss->opInterrupt())
	return false;

    // Sort by grid index.
    {
#ifdef UT_PointGridImpl_DO_TIMING
    UT_Timer t("UTparallelStableSort");
#endif
    UTparallelStableSort(myIdxKeys.array(), myIdxKeys.array() + npts,
			 ut_isGridIdxLess());
    }

    if (boss->opInterrupt())
	return false;

    // Uncompress any occupied tiles before writing to them from multiple
    // threads.
    {
#ifdef UT_PointGridImpl_DO_TIMING
    UT_Timer t("uncompressTiles");
#endif
    uncompressTiles(tileoccupied);
    }

    if (boss->opInterrupt())
	return false;

    // Find the actual number of keys with a valid grididx.
    UT_GridIdxKey invalidkey;
    invalidkey.grididx = INVALIDGRIDIDX;
    UT_GridIdxKey *lastValid = std::lower_bound(myIdxKeys.array(),
                                                myIdxKeys.array() + npts,
                                                invalidkey,
                                                ut_isGridIdxLess());
    indextype numValidKeys = std::distance(myIdxKeys.array(), lastValid);
    UT_ASSERT((numValidKeys == npts) ||
              (numValidKeys < npts &&
               (!numValidKeys ||
                myIdxKeys(numValidKeys-1).grididx != INVALIDGRIDIDX)));

    // Look for begin / end ranges and store in the begin / end arrays.
    UT_ValArray<UT_Vector3T<int> > bboxmins, bboxmaxs;
    {
#ifdef UT_PointGridImpl_DO_TIMING
    UT_Timer t("findIdxRanges");
#endif
    findIdxRanges(bboxmins, bboxmaxs, numValidKeys);
    }

    if (boss->opInterrupt())
	return false;

    // Expand the bounding box to encompass those from all threads.
    myBBoxMin = bboxmins(0);
    myBBoxMax = bboxmaxs(0);
    for (int i=1; i < bboxmins.entries(); i++)
    {
        UT_Vector3T<int> &bboxmin = bboxmins(i);
        UT_Vector3T<int> &bboxmax = bboxmaxs(i);
        myBBoxMin.x() = SYSmin(myBBoxMin.x(), bboxmin.x());
        myBBoxMax.x() = SYSmax(myBBoxMax.x(), bboxmax.x());
        myBBoxMin.y() = SYSmin(myBBoxMin.y(), bboxmin.y());
        myBBoxMax.y() = SYSmax(myBBoxMax.y(), bboxmax.y());
        myBBoxMin.z() = SYSmin(myBBoxMin.z(), bboxmin.z());
        myBBoxMax.z() = SYSmax(myBBoxMax.z(), bboxmax.z());
    }

#ifdef UT_PointGridImpl_DO_TIMING
    std::cerr << "UT_PointGrid Memory Use: ";
    std::cerr << getMemoryUsage() / (1024.0 * 1024) << " MB" << std::endl;
#endif

    return true;
}

template <typename T>
bool
UT_PointGrid<T>::canBuild(int xres, int yres, int zres)
{
    // Grid index is absolute voxel number, so ensure number of voxels
    // fits into indextype.
    exint nvox = exint(xres) * yres * zres;
    if (nvox >= INVALIDGRIDIDX)
        return false;

    // Use exint in case accessor returns full 64-bit answer for
    // entries, like GEO_PointGridGDPAccessor.
    exint nentries = myAccessor.entries();
    if (nentries >= INVALIDINDEX)
        return false;

    // Ensure maximum returned key value fits into keytype (for
    // GEO_PointGridGDPAccessor this is a runtime property of the
    // underlying GEO_Detail, not a static type-specific value).
    exint maxkey = myAccessor.maxKeyValue();
    if (maxkey > std::numeric_limits<keytype>::max())
        return false;

    return true;
}


template <typename T>
exint
UT_PointGrid<T>::entriesAt(int x, int y, int z) const
{
    indextype begin, end;
    if (!myBegins.isValidIndex(x, y, z))
        return 0;
    begin = end = myBegins(x, y, z);
    if (begin != INVALIDINDEX)
    {
        end = myEnds(x, y, z);
    }
    return end - begin;
}

template <typename T>
bool
UT_PointGrid<T>::getKeysAt(int x, int y, int z,
                           typename UT_PointGrid<T>::indextype& begin,
                           typename UT_PointGrid<T>::indextype& end) const
{
    if (!isValidIndex(x, y, z))
        return false;
    begin = end = myBegins(x, y, z);
    if (begin == INVALIDINDEX)
        return false;
    end = myEnds(x, y, z);
    return true;
}

template <typename T>
UT_PointGridIterator<T>
UT_PointGrid<T>::getKeysAt(int x, int y, int z, queuetype &queue) const
{
    // Set up iterator as initially empty.
    indextype begin, end;
    if (getKeysAt(x, y, z, begin, end))
    {
        queue.entries(1);
        queue(0).begin = begin;
        queue(0).end = end;
        // Rewind here so iterator is ready to use.
        return UT_PointGridIterator<T>(this, queue);
    }
    return UT_PointGridIterator<T>();
}

template <typename T>
UT_PointGridIterator<T>
UT_PointGrid<T>::getInvalidKeys(queuetype &queue) const
{
    const UT_GridIdxKey *begin = myIdxKeys.getRawArray();
    const UT_GridIdxKey *end = begin + myIdxKeys.entries();

    UT_GridIdxKey invalid;
    invalid.grididx = INVALIDGRIDIDX;

    // We use lower_bound because INVALIDIDX is numeric_limits<T>::max().
    const UT_GridIdxKey *lb = std::lower_bound(begin, end, invalid,
                                               ut_isGridIdxLess());

    // If no invalid keys, return empty iterator.
    if (lb == end)
        return UT_PointGridIterator<T>();

    queue.entries(1);
    queue(0).begin = lb - begin;
    queue(0).end = myIdxKeys.entries();
    // Rewind here so iterator is ready to use.
    return UT_PointGridIterator<T>(this, queue);
}


template <typename T>
bool
UT_PointGrid<T>::calcBounds(const UT_Vector3 &pos, fpreal radius,
                            int &xmin, int &ymin, int &zmin,
                            int &xmax, int &ymax, int &zmax) const
{
    int mintile, maxtile;
    // Calc lower and upper points of bounding box.
    posToIndex(pos - radius, xmin, ymin, zmin, false);
    posToIndex(pos + radius, xmax, ymax, zmax, false);

    // Intersect with voxel-space bounding box of points.
    xmin = SYSmax(xmin, myBBoxMin.x());
    ymin = SYSmax(ymin, myBBoxMin.y());
    zmin = SYSmax(zmin, myBBoxMin.z());
    xmax = SYSmin(xmax, myBBoxMax.x());
    ymax = SYSmin(ymax, myBBoxMax.y());
    zmax = SYSmin(zmax, myBBoxMax.z());

    // If inverted bbox, return false.
    if (xmin > xmax || ymin > ymax || zmin > zmax)
        return false;

    // If bbox is fully inside invalid tile, return false.
    mintile = myBegins.indexToLinearTile(xmin, ymin, zmin);
    maxtile = myBegins.indexToLinearTile(xmax, ymax, zmax);
    if (mintile == maxtile &&
        myBegins.getLinearTile(mintile)->isConstant() &&
        ((*myBegins.getLinearTile(mintile))(0,0,0) == INVALIDINDEX))
            return false;

    return true;
}

template <typename T>
UT_PointGridIterator<T>
UT_PointGrid<T>::findCloseKeys(const UT_Vector3 &pos,
                               queuetype &queue,
                               fpreal radius) const
{
    int x, y, z, xmin, ymin, zmin, xmax, ymax, zmax, nvox;
    indextype begin;

    // If out-of-bounds, return empty iterator.
    if (!calcBounds(pos, radius, xmin, ymin, zmin, xmax, ymax, zmax))
        return UT_PointGridIterator<T>();

    nvox = (1 + xmax - xmin) * (1 + ymax - ymin) * (1 + zmax - zmin);
    // Special case single voxel case.
    if (nvox == 1)
        return getKeysAt(xmin, ymin, zmin, queue);

    // Set number of sequences to max number of voxels to avoid
    // multiple memory allocations when appending sequences.
    queue.entries(nvox);
    int idx = 0;
    for(z = zmin; z <= zmax; z++)
    for(y = ymin; y <= ymax; y++)
    for(x = xmin; x <= xmax; x++)
    {
        begin = myBegins(x, y, z);
        if (begin != INVALIDINDEX)
        {
            queue(idx).begin = begin;
            queue(idx).end = myEnds(x, y, z);
            idx++;
        }
    }
    // Set number of sequences to actual number of non-empty voxels.
    queue.entries(idx);

    // Constructor rewinds, so iterator is ready to use.
    return UT_PointGridIterator<T>(this, queue);
}

template <typename T>
bool
UT_PointGrid<T>::hasCloseKeys(const UT_Vector3 &pos, fpreal radius) const
{
    int x, y, z, xmin, ymin, zmin, xmax, ymax, zmax;

    if (!calcBounds(pos, radius, xmin, ymin, zmin, xmax, ymax, zmax))
        return false;

    for(z = zmin; z <= zmax; z++)
    for(y = ymin; y <= ymax; y++)
    for(x = xmin; x <= xmax; x++)
    {
        if (myBegins(x, y, z) != INVALIDINDEX)
            return true;
    }
    return false;
}

#endif
