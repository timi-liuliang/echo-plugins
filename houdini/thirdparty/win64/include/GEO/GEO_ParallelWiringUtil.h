/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO_ParallelWiringUtil.h (GEO Library, C++)
 *
 * COMMENTS:    Parallel functors for wiring points to vertices are here.
 *              It's not *completely* impossible to follow, but it gets
 *              rather messy. See GEO_Face::buildBlock or
 *              GEO_PrimPolySoup::build for usage examples.
 */

#ifndef __GEO_ParallelWiringUtil__
#define __GEO_ParallelWiringUtil__

#include "GEO_SurfaceType.h"

#include <GA/GA_ATITopology.h>
#include <GA/GA_Edge.h>
#include <GA/GA_Iterator.h>
#include <GA/GA_OffsetList.h>
#include <GA/GA_SplittableRange.h>
#include <GA/GA_Types.h>

#if UT_ASSERT_LEVEL >= UT_ASSERT_LEVEL_PARANOID
#include <GA/GA_Detail.h>
#endif

#include <UT/UT_Array.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_Interrupt.h>
#include <UT/UT_Lock.h>
#include <UT/UT_ParallelUtil.h>

namespace {

/// This class sets the vertex-to-point topology attribute (the easy one)
/// in parallel.
class geo_SetTopoMappedParallel
{
public:
    geo_SetTopoMappedParallel(GA_ATITopology *topology, const GA_Offset startto,
            const GA_Offset startfrom, const int *map)
        : myTopology(topology)
        , myStartTo(startto)
        , myStartFrom(startfrom)
        , myMap(map)
    {}

    void operator()(const GA_SplittableRange &r) const
    {
        if (myMap)
        {
            GA_Offset start;
            GA_Offset end;
            for (GA_Iterator it(r); it.blockAdvance(start, end); )
            {
                for (GA_Offset off = start; off < end; ++off)
                {
                    GA_Offset to = myMap[off - myStartFrom] + myStartTo;
                    UT_ASSERT_P(GAisValid(to) && to < myTopology->getDetail().getIndexMap(myTopology->getLinkOwner()).offsetSize());
                    myTopology->setLink(off, to);
                }
            }
        }
        else
        {
            // If myMap is NULL, the points are contiguous starting from
            // myStartPt, e.g. for independent curves.
            GA_Offset start;
            GA_Offset end;
            for (GA_Iterator it(r); it.blockAdvance(start, end); )
            {
                for (GA_Offset off = start, to = (start - myStartFrom) + myStartTo; off < end; ++off, ++to)
                {
                    UT_ASSERT_P(GAisValid(to) && to < myTopology->getDetail().getIndexMap(myTopology->getLinkOwner()).offsetSize());
                    myTopology->setLink(off, to);
                }
            }
        }
    }

private:
    GA_ATITopology *myTopology;
    const GA_Offset myStartTo;
    const GA_Offset myStartFrom;
    const int *myMap;
};

/// This class, in parallel, initializes a UT_IntArray to 0, 1, 2, 3,...
/// It may not sound related, but the next step is to sort it by the
/// point offset of the vertex whose offset is in another array at this
/// array element's position.  See below.
class geo_TrivialArrayParallel
{
public:
    geo_TrivialArrayParallel(UT_IntArray &array) : myArray(array) {}

    void operator()(const UT_BlockedRange<GA_Size> &r) const
    {
        for (GA_Size i = r.begin(); i < r.end(); ++i)
            myArray(i) = i;
    }

private:
    UT_IntArray &myArray;
};

/// This class is a comparator for use with UTparallelSort, to sort
/// an array comprising indices into myArray based on the values in myArray.
/// This is used to create the mapping that would sort vertices by the points
/// to which they refer.
template <bool force_stable=true>
class geo_VerticesByPointCompare
{
public:
    geo_VerticesByPointCompare(const int *array) : myArray(array) {}

    bool operator()(const int &a, const int &b) const
    {
        // NOTE: For platform consistency, we must force the sort to be stable.
        //       tbb::parallel_sort is normally unstable and thus so is
        //       UTparallelSort.
        // a and b are the original positions in the array being sorted,
        // so we can do this by enforcing that a < b when the values are equal.
        if (force_stable)
            return (myArray[a] < myArray[b])
                || ((myArray[a] == myArray[b]) && (a < b));
        else
            return (myArray[a] < myArray[b]);
    }
private:
    const int *myArray;
};


/// This class creates arrays for the next vertex and prev vertex in parallel.
/// If we wanted to do this in geo_LinkToposParallel, we would first have to
/// create an inverse map anyway to find out where vertices are in map, so
/// this saves re-traversing things.
class geo_NextPrevParallel
{
public:
    geo_NextPrevParallel(const UT_IntArray &order,
                        const int *pointnumbers,
                        UT_IntArray &nextarray,
                        UT_IntArray &prevarray,
                        const GA_Offset startvtx,
                        const GA_Offset startpt,
                        const GA_ATITopology *pt2vtx,
                        GA_ATITopology *prevvtx,
                        UT_Lock &lock)
        : myOrder(order)
        , myPointNumbers(pointnumbers)
        , myNextArray(nextarray)
        , myPrevArray(prevarray)
        , myStartVtx(startvtx)
        , myStartPt(startpt)
        , myPt2Vtx(pt2vtx)
        , myPrevVtx(prevvtx)
        , myUpdateExistingLock(lock)
    {}

    void operator()(const UT_BlockedRange<GA_Size> &r) const
    {
        // NOTE: Even though GA_Edge is supposed to be for point offset pairs,
        //       these are vertex offset pairs.
        UT_Array<GA_Edge> prevlinks;

        GA_Size i = r.begin();
        int vtx = myOrder(i);
        int pt = myPointNumbers[vtx];
        // NOTE: Use INT_MIN instead of -1, because myNextArray needs to be able
        //       to refer to existing vertices, which would have negative
        //       relative offsets.
        int prev = (i-1 >= 0 && myPointNumbers[myOrder(i - 1)] == pt) ? myOrder(i - 1) : INT_MIN;
        for (; i < r.end(); ++i)
        {
            myPrevArray(vtx) = prev;

            int next = (i+1 < myOrder.entries() && myPointNumbers[myOrder(i + 1)] == pt) ? myOrder(i + 1) : INT_MIN;
            if (next >= 0)
            {
                myNextArray(vtx) = next;
                prev = vtx;
                vtx = next;
            }
            else
            {
                GA_Offset existingvtx = myPt2Vtx->getLink(pt + myStartPt);
                // If not sharing points with existing geometry, things are easy
                if (!GAisValid(existingvtx))
                {
                    myNextArray(vtx) = INT_MIN;
                }
                // Otherwise, we need to update the existing vertex's prev link
                // very carefully.
                else
                {
                    myNextArray(vtx) = existingvtx - myStartVtx;
                    prevlinks.append(GA_Edge(existingvtx, vtx + myStartVtx));
                }
                if (i+1 < myOrder.entries())
                {
                    prev = INT_MIN;
                    vtx = myOrder(i + 1);
                    pt = myPointNumbers[vtx];
                }
            }
        }

        // Add any prev links from existing geometry to the new geometry
        if (prevlinks.entries())
        {
            UT_AutoLock lock(myUpdateExistingLock);
            for (GA_Size i = 0; i < prevlinks.entries(); ++i)
                myPrevVtx->setLink(prevlinks(i).p0(), prevlinks(i).p1());
        }
    }

private:
    const UT_IntArray &myOrder;
    const int *myPointNumbers;
    UT_IntArray &myNextArray;
    UT_IntArray &myPrevArray;

    // The following are only needed for handling the case when existing
    // geometry has used the same points
    const GA_Offset myStartPt;
    const GA_Offset myStartVtx;
    const GA_ATITopology *myPt2Vtx;
    GA_ATITopology *myPrevVtx;
    UT_Lock &myUpdateExistingLock;
};

/// This class is a comparator used in geo_Pt2VtxTopoParallel to find the
/// start of 
class geo_VertexVsPointCompare
{
public:
    geo_VertexVsPointCompare(const int *pointnumbers)
        : myPointNumbers(pointnumbers)
    {}

    bool operator()(const int &relativevtx, const GA_Size &relativept) const
    { return (myPointNumbers[relativevtx] < relativept); }

private:
    const int *myPointNumbers;
};

/// This class fills in the point-to-vertex topology attribute, selecting
/// a first vertex for each point.
class geo_Pt2VtxTopoParallel
{
public:
    geo_Pt2VtxTopoParallel(GA_ATITopology *pt2vtx, const UT_IntArray &order,
                          const int *pointnumbers,
                          const GA_Offset startvtx, const GA_Offset startpt)
        : myPt2Vtx(pt2vtx)
        , myOrder(order)
        , myPointNumbers(pointnumbers)
        , myStartVtx(startvtx)
        , myStartPt(startpt)
    {}

    void operator()(const GA_SplittableRange &r) const
    {
        if (!myOrder.entries())
            return;

        GA_Offset start;
        GA_Offset end;
        for (GA_Iterator it = r.begin(); it.blockAdvance(start, end); )
        {
            int *orderend = myOrder.getArray() + myOrder.entries();
            // If we don't have any points as large as start, we're done
            if (myPointNumbers[orderend[-1]] < GA_Size(start - myStartPt))
                break;
            // If we don't have any points smaller than end, this iteration
            // won't find anything
            if (myPointNumbers[myOrder(0)] >= GA_Size(end - myStartPt))
                continue;

            // Find the start of where the points in question appear
            int *pvtx = std::lower_bound(myOrder.getArray(), orderend, GA_Size(start - myStartPt), geo_VertexVsPointCompare(myPointNumbers));

            // For each point offset
            for (GA_Offset i = start; i < end; ++i)
            {
                if (i == myPointNumbers[pvtx[0]] + myStartPt)
                {
                    UT_ASSERT_P(GAisValid(pvtx[0] + myStartVtx) && pvtx[0] + myStartVtx < myPt2Vtx->getDetail().getNumVertexOffsets());
                    myPt2Vtx->setLink(i, pvtx[0] + myStartVtx);
                    // Skip to the next point
                    while (pvtx + 1 < orderend
                                && myPointNumbers[pvtx[0]] == myPointNumbers[pvtx[1]])
                        ++pvtx;
                    ++pvtx;
                    if (pvtx == orderend)
                        break;
                }
            }
        }
    }

private:
    GA_ATITopology *myPt2Vtx;
    const UT_IntArray &myOrder;
    const int *myPointNumbers;
    const GA_Offset myStartVtx;
    const GA_Offset myStartPt;
};

/// This class (finally) writes out the vertex-to-next-vertex and
/// vertex-to-previous-vertex linked-list topology attributes.
class geo_LinkToposParallel
{
public:
    geo_LinkToposParallel(GA_ATITopology *nextvtx, GA_ATITopology *prevvtx,
                        const UT_IntArray &nextarray,
                        const UT_IntArray &prevarray,
                        const GA_Offset startvtx)
        : myNextVtx(nextvtx)
        , myPrevVtx(prevvtx)
        , myNextArray(nextarray)
        , myPrevArray(prevarray)
        , myStartVtx(startvtx)
    {}

    void operator()(const GA_SplittableRange &r) const
    {
        GA_Offset start;
        GA_Offset end;
        for (GA_Iterator it = r.begin(); it.blockAdvance(start, end); )
        {
            // Separate loops for better caching
            for (GA_Offset off = start; off < end; ++off)
            {
                int next = myNextArray(off - myStartVtx);
                myNextVtx->setLink(off, (next != INT_MIN) ? next + myStartVtx : GA_INVALID_OFFSET);
            }

            for (GA_Offset off = start; off < end; ++off)
            {
                int prev = myPrevArray(off - myStartVtx);
                myPrevVtx->setLink(off, (prev != INT_MIN) ? prev + myStartVtx : GA_INVALID_OFFSET);
            }
        }
    }

private:
    GA_ATITopology *myNextVtx;
    GA_ATITopology *myPrevVtx;
    const UT_IntArray &myNextArray;
    const UT_IntArray &myPrevArray;
    const GA_Offset myStartVtx;
};




/// This class unlinks vertices that are the only vertex linked to their point
class geo_UnlinkSingletonsParallel
{
public:
    geo_UnlinkSingletonsParallel(const GA_ATITopology *nextvtx, const GA_ATITopology *prevvtx,
                        GA_ATITopology *vtx2pt, GA_ATITopology *pt2vtx)
        : myNextVtx(nextvtx)
        , myPrevVtx(prevvtx)
        , myVtx2Pt(vtx2pt)
        , myPt2Vtx(pt2vtx)
        , myNonSingletons()
    {
        UT_ASSERT(nextvtx && prevvtx && vtx2pt && pt2vtx);
    }

    geo_UnlinkSingletonsParallel(const geo_UnlinkSingletonsParallel &that, UT_Split)
        : myNextVtx(that.myNextVtx)
        , myPrevVtx(that.myPrevVtx)
        , myVtx2Pt(that.myVtx2Pt)
        , myPt2Vtx(that.myPt2Vtx)
        , myNonSingletons()
    {}
    
    // NOTE: This is GA_Range instead of GA_SplittableRange *ONLY* so that
    //       this can be used serially, since it is only threadsafe with
    //       GA_SplittableRange.
    void operator()(const GA_Range &r)
    {
        GA_Offset start;
        GA_Offset end;
        for (GA_Iterator it = r.begin(); it.blockAdvance(start, end); )
        {
            for (GA_Offset off = start; off < end; ++off)
            {
                if (!GAisValid(myNextVtx->getLink(off)) && !GAisValid(myPrevVtx->getLink(off)))
                {
                    GA_Offset ptoff = myVtx2Pt->getLink(off);
                    myVtx2Pt->setLink(off, GA_INVALID_OFFSET);
                    // NOTE: We can't have multiple points mapping to the same
                    //       vertex, and ptoff should be pointing to off, so
                    //       the page should be hardened already.  Otherwise,
                    //       this wouldn't be threadsafe.
                    myPt2Vtx->setLink(ptoff, GA_INVALID_OFFSET);
                }
                else
                {
                    myNonSingletons.append(off);
                }
            }
        }
    }

    void join(const geo_UnlinkSingletonsParallel &that)
    {
        if (!that.myNonSingletons.entries())
            return;
        if (!myNonSingletons.entries())
            myNonSingletons = that.myNonSingletons;
        else
            myNonSingletons.append(that.myNonSingletons);
    }

    GA_OffsetList &getNonSingletons()
    { return myNonSingletons; }

private:
    const GA_ATITopology *const myNextVtx;
    const GA_ATITopology *const myPrevVtx;
    GA_ATITopology *const myVtx2Pt;
    GA_ATITopology *const myPt2Vtx;
    GA_OffsetList myNonSingletons;
};


/// This class finds the minimum and maximum point offset used by
/// a GA_OffsetList of vertex offsets.
class geo_PointRangeParallel
{
public:
    geo_PointRangeParallel(const GA_OffsetList &vertices,
                           const GA_ATITopology *vtx2pt)
        : myVtx2Pt(vtx2pt)
        , myVertices(vertices)
        , myMinPoint(GA_INVALID_OFFSET)
        , myMaxPoint(GA_INVALID_OFFSET)
    {
        UT_ASSERT(vtx2pt);
    }

    geo_PointRangeParallel(const geo_PointRangeParallel &that, UT_Split)
        : myVtx2Pt(that.myVtx2Pt)
        , myVertices(that.myVertices)
        , myMinPoint(GA_INVALID_OFFSET)
        , myMaxPoint(GA_INVALID_OFFSET)
    {}

    void operator()(const UT_BlockedRange<GA_Size> &r)
    {
        GA_Size i = r.begin();
        // NOTE: Can't assign myMinPoint or myMaxPoint here,
        //       because UTparallelReduce can re-use this object
        //       for multiple ranges.  myMinPoint and myMaxPoint
        //       are accumulated below.
        GA_Offset minpoint = myVtx2Pt->getLink(myVertices(i));
        GA_Offset maxpoint = minpoint;
        for (++i; i < r.end(); ++i)
        {
            GA_Offset ptoff = myVtx2Pt->getLink(myVertices(i));
            minpoint = SYSmin(minpoint, ptoff);
            maxpoint = SYSmax(maxpoint, ptoff);
        }
        if (!GAisValid(myMinPoint))
        {
            myMinPoint = minpoint;
            myMaxPoint = maxpoint;
        }
        else
        {
            myMinPoint = SYSmin(myMinPoint, minpoint);
            myMaxPoint = SYSmax(myMaxPoint, maxpoint);
        }
    }

    void join(const geo_PointRangeParallel &that)
    {
        if (!GAisValid(myMinPoint))
        {
            myMinPoint = that.myMinPoint;
            myMaxPoint = that.myMaxPoint;
        }
        else
        {
            myMinPoint = SYSmin(myMinPoint, that.myMinPoint);
            myMaxPoint = SYSmax(myMaxPoint, that.myMaxPoint);
        }
    }

    GA_Offset getStart() const
    { return myMinPoint; }

    GA_Offset getEnd() const
    { return myMaxPoint + 1; }

private:
    const GA_ATITopology *myVtx2Pt;
    const GA_OffsetList &myVertices;
    GA_Offset myMinPoint;
    GA_Offset myMaxPoint;
};

/// This just subtracts start from every element in input, and puts the result
/// as an int in output.
class geo_OffsetListToRelative
{
public:
    geo_OffsetListToRelative(const GA_OffsetList &input, int *output, GA_Offset start)
        : myInput(input)
        , myOutput(output)
        , myStart(start)
    {}

    void operator()(const UT_BlockedRange<GA_Size> &r) const
    {
        for (GA_Size i = r.begin(); i < r.end(); ++i)
        {
            myOutput[i] = int(GA_Size(myInput(i) - myStart));
        }
    }

private:
    const GA_OffsetList &myInput;
    int *const myOutput;
    const GA_Offset myStart;
};

template<typename INT_TYPE=int>
class geo_QuadGridVertsParallel
{
public:
    geo_QuadGridVertsParallel(GA_Size rows, GA_Size cols, INT_TYPE *pointnums,
        bool wrappedu=false, bool wrappedv=false,
        INT_TYPE startptnum=0, UT_Interrupt *interrupt=nullptr,
        bool start_pole_row=false, bool end_pole_row=false)
        : myRows(rows)
        , myCols(cols)
        , myInterrupt(interrupt)
        , myPointNums(pointnums)
        , myStartPtNum(startptnum)
        , myWrappedU(wrappedu)
        , myWrappedV(wrappedv)
        , myStartPole(start_pole_row)
        , myEndPole(end_pole_row)
        , myTriangularPoles(false)
    {
    }
    void setupSphere(bool triangular_poles) const
    {
        myWrappedU = true;
        myWrappedV = false;
        myStartPole = true;
        myEndPole = true;
        myTriangularPoles = triangular_poles;
    }
    void setupTube() const
    {
        myWrappedU = true;
        myWrappedV = false;
        myStartPole = false;
        myEndPole = false;
    }
    void setupGrid() const
    {
        myWrappedU = false;
        myWrappedV = false;
        myStartPole = false;
        myEndPole = false;
    }
    void setupTorus() const
    {
        myWrappedU = true;
        myWrappedV = true;
        myStartPole = false;
        myEndPole = false;
    }

    void operator()(const UT_BlockedRange<GA_Size> &r) const
    {
        char bcnt = 0;
        UT_Interrupt *const interrupt = myInterrupt;

        GA_Size nquadcols = myCols - !myWrappedU;

        int startrow; int startcol;
        SYSdivMod(r.begin(), nquadcols, startrow, startcol);
        GA_Size i = 4*r.begin();
        GA_Size end = 4*r.end();

        INT_TYPE pt = myStartPtNum;
        if (myStartPole && startrow == 0)
        {
            // Handle starting pole row
            for (int col = startcol; col < myCols-1 && i < end; ++col)
            {
                if (interrupt && !bcnt++ && interrupt->opInterrupt())
                {
                    i = end;
                    break;
                }

                myPointNums[i++] = pt;
                if (!myTriangularPoles)
                    myPointNums[i++] = pt;
                myPointNums[i++] = pt+1 + col + 1;
                myPointNums[i++] = pt+1 + col;
            }
            if (myWrappedU && i < end)
            {
                myPointNums[i++] = pt;
                if (!myTriangularPoles)
                    myPointNums[i++] = pt;
                myPointNums[i++] = pt + 1;
                myPointNums[i++] = pt + myCols;
            }
            startcol = 0;
            startrow = 1;
            ++pt;
        }
        else
        {
            pt += startrow*myCols + startcol;
        }

        for (int row = startrow; i < end; ++row)
        {
            if ((myWrappedV && row == myRows-1) || (!myWrappedV && myEndPole && row == myRows-2))
                break;
            for (int col = startcol; col < myCols-1 && i < end; ++col)
            {
                if (interrupt && !bcnt++ && interrupt->opInterrupt())
                {
                    i = end;
                    break;
                }

                myPointNums[i++] = pt;
                myPointNums[i++] = pt + 1;
                myPointNums[i++] = pt + myCols + 1;
                myPointNums[i++] = pt + myCols;

                pt++;
            }
            if (myWrappedU && i < end)
            {
                myPointNums[i++] = pt;
                myPointNums[i++] = pt - myCols + 1;
                myPointNums[i++] = pt + 1;
                myPointNums[i++] = pt + myCols;
            }
            // The last point of the row gets skipped if not wrapping U
            pt++;
            startcol = 0;
        }

        // If not wrapping V or handling end pole, we're done
        if (i == end)
            return;

        if (myEndPole)
        {
            // End pole point is starting point if wrapping.
            INT_TYPE endpolept = myWrappedV ? myStartPtNum : (pt+myCols);
            for (int col = startcol; col < myCols-1 && i < end; ++col)
            {
                if (interrupt && !bcnt++ && interrupt->opInterrupt())
                    break;

                myPointNums[i++] = pt;
                myPointNums[i++] = pt + 1;
                myPointNums[i++] = endpolept;
                if (!myTriangularPoles)
                    myPointNums[i++] = endpolept;

                pt++;
            }
            if (myWrappedU && i < end)
            {
                myPointNums[i++] = pt;
                myPointNums[i++] = pt - myCols + 1;
                myPointNums[i++] = endpolept;
                if (!myTriangularPoles)
                    myPointNums[i++] = endpolept;
            }
            return;
        }

        // If at last row and wrapping V
        for (int col = startcol; col < myCols-1 && i < end; ++col)
        {
            if (interrupt && !bcnt++ && interrupt->opInterrupt())
                break;

            myPointNums[i++] = pt;
            myPointNums[i++] = pt + 1;
            myPointNums[i++] = myStartPtNum + col + 1;
            myPointNums[i++] = myStartPtNum + col;

            pt++;
        }
        if (myWrappedU && i < end)
        {
            myPointNums[i++] = pt;
            myPointNums[i++] = pt - myCols + 1;
            myPointNums[i++] = myStartPtNum;
            myPointNums[i++] = myStartPtNum + myCols - 1;
        }
    }

private:
    const GA_Size myRows;
    const GA_Size myCols;
    UT_Interrupt *const myInterrupt;
    INT_TYPE *const myPointNums;
    const INT_TYPE myStartPtNum;
    const bool myWrappedU;
    const bool myWrappedV;
    const bool myStartPole;
    const bool myEndPole;
    const bool myTriangularPoles;
};

template<typename INT_TYPE=int>
class geo_TriGridVertsParallel
{
public:
    geo_TriGridVertsParallel(
        GA_Size rows, GA_Size cols, INT_TYPE *pointnums,
        bool wrappedu, bool wrappedv,
        GEO_SurfaceType type, INT_TYPE startptnum=0,
        UT_Interrupt *interrupt=nullptr,
        bool start_pole_row=false, bool end_pole_row=false)
        : myRows(rows)
        , myCols(cols)
        , myPointNums(pointnums)
        , myStartPtNum(startptnum)
        , myType(type)
        , myWrappedU(wrappedu)
        , myWrappedV(wrappedv)
        , myInterrupt(interrupt)
        , myStartPole(start_pole_row)
        , myEndPole(end_pole_row)
        , myTriangularPoles(false)
    {}

    void operator()(const UT_BlockedRange<GA_Size> &r) const
    {
        char bcnt = 0;
        UT_Interrupt *const interrupt = myInterrupt;

        bool alt = (myType == GEO_PATCH_ALTTRIANGLE);
        bool rev = (myType == GEO_PATCH_REVTRIANGLE);

        GA_Size nquadcols = myCols - !myWrappedU;

        int startrow; int startcol;
        SYSdivMod(r.begin(), nquadcols, startrow, startcol);
        GA_Size i = 6*r.begin();
        GA_Size end = 6*r.end();
        INT_TYPE pt = myStartPtNum + startrow*myCols + startcol;
        for (int row = startrow; i < end; ++row)
        {
            if (myWrappedV && row == myRows-1)
                break;
            for (int col = startcol; col < myCols-1 && i < end; ++col)
            {
                if (interrupt && !bcnt++ && interrupt->opInterrupt())
                {
                    i = end;
                    break;
                }

                // Switch diagonal with checkerboard pattern
                bool switched = (rev || (alt && ((row^col)&1)));

                myPointNums[i++] = pt;
                myPointNums[i++] = pt + 1;
                myPointNums[i++] = pt + myCols + int(!switched);

                myPointNums[i++] = pt + int(switched);
                myPointNums[i++] = pt + myCols + 1;
                myPointNums[i++] = pt + myCols;

                pt++;
            }
            if (myWrappedU && i < end)
            {
                // Switch diagonal with checkerboard pattern
                bool switched = (rev || (alt && ((row^(myCols-1))&1)));

                myPointNums[i++] = pt;
                myPointNums[i++] = pt - myCols + 1;
                myPointNums[i++] = pt + (switched ? myCols : 1);

                myPointNums[i++] = pt + (switched ? (-myCols + 1) : 0);
                myPointNums[i++] = pt + 1;
                myPointNums[i++] = pt + myCols;
            }
            // The last point of the row gets skipped if not wrapping U
            pt++;
            startcol = 0;
        }

        // If not wrapping V, we're done
        if (i == end)
            return;

        // If at last row and wrapping V
        for (int col = startcol; col < myCols-1 && i < end; ++col)
        {
            if (interrupt && !bcnt++ && interrupt->opInterrupt())
                break;

            // Switch diagonal with checkerboard pattern
            bool switched = (rev || (alt && (((myRows-1)^col)&1)));

            myPointNums[i++] = pt;
            myPointNums[i++] = pt + 1;
            myPointNums[i++] = myStartPtNum + col + int(!switched);

            myPointNums[i++] = pt + int(switched);
            myPointNums[i++] = myStartPtNum + col + 1;
            myPointNums[i++] = myStartPtNum + col;

            pt++;
        }
        if (myWrappedU && i < end)
        {
            // Switch diagonal with checkerboard pattern
            bool switched = (rev || (alt && (((myRows-1)^(myCols-1))&1)));

            myPointNums[i++] = pt;
            myPointNums[i++] = pt - myCols + 1;
            myPointNums[i++] = myStartPtNum + (switched ? (myCols-1) : 0);

            myPointNums[i++] = pt + (switched ? (-myCols + 1) : 0);
            myPointNums[i++] = myStartPtNum + 0;
            myPointNums[i++] = myStartPtNum + myCols-1;
        }
    }

private:
    const GA_Size myRows;
    const GA_Size myCols;
    UT_Interrupt *const myInterrupt;
    INT_TYPE *const myPointNums;
    const INT_TYPE myStartPtNum;
    const GEO_SurfaceType myType;
    const bool myWrappedU;
    const bool myWrappedV;
    const bool myStartPole;
    const bool myEndPole;
    const bool myTriangularPoles;
};

template<typename INT_TYPE=int>
class geo_CurveGridVertsParallel
{
public:
    geo_CurveGridVertsParallel(GA_Size rows, GA_Size cols, INT_TYPE *pointnums,
        bool unrollcurves, GEO_SurfaceType type,
        INT_TYPE startptnum=0, UT_Interrupt *interrupt=nullptr,
        bool start_pole_row=false, bool end_pole_row=false)
        : myRows(rows)
        , myCols(cols)
        , myInterrupt(interrupt)
        , myPointNums(pointnums)
        , myStartPtNum(startptnum)
        , myType(type)
        , myUnrollCurves(unrollcurves)
        , myStartPole(start_pole_row)
        , myEndPole(end_pole_row)
    {
        UT_ASSERT(type == GEO_PATCH_ROWS || type == GEO_PATCH_COLS);
    }

    void operator()(const UT_BlockedRange<GA_Size> &r) const
    {
        const bool unrollcurves = myUnrollCurves;
        const GA_Size nrows = myRows;
        const GA_Size ncols = myCols;
        INT_TYPE *pointnums = myPointNums + r.begin();

        if (myType == GEO_PATCH_ROWS)
        {
            GA_Size startrow = r.begin()/ncols;
            const GA_Size startcol = r.begin()%ncols;
            const GA_Size endrow = r.end()/ncols;
            const GA_Size endcol = r.end()%ncols;
            INT_TYPE v = myStartPtNum + r.begin();
            if (startrow == endrow)
            {
                for (GA_Size col = startcol; col < endcol; ++col, ++v, ++pointnums)
                    *pointnums = v;
                return;
            }
            if (startcol != 0)
            {
                // Partial first row
                for (GA_Size col = startcol; col < ncols; ++col, ++v, ++pointnums)
                    *pointnums = v;

                if (unrollcurves)
                {
                    *pointnums = v-ncols;
                    ++pointnums;
                }
                ++startrow;
            }
            // Middle rows
            for (GA_Size row = startrow; row < endrow; ++row)
            {
                for (GA_Size col = 0; col < ncols; ++col, ++v, ++pointnums)
                    *pointnums = v;

                if (unrollcurves)
                {
                    *pointnums = v-ncols;
                    ++pointnums;
                }
            }
            // Partial final row
            for (GA_Size col = 0; col < endcol; ++col, ++v, ++pointnums)
                *pointnums = v;
        }
        else
        {
            // GEO_PATCH_COLS

            const GA_Size nmeshpoints = nrows*ncols;
            GA_Size startcol = r.begin()/nrows;
            const GA_Size startrow = r.begin()%nrows;
            const GA_Size endcol = r.end()/nrows;
            const GA_Size endrow = r.end()%nrows;
            // NOTE: v uses ncols as normal; it's the output & iteration order
            //       that's the opposite of the above.
            INT_TYPE v = myStartPtNum + startrow*ncols + startcol;
            if (startcol == endcol)
            {
                for (GA_Size row = startrow; row < endrow; ++row, v += ncols, ++pointnums)
                    *pointnums = v;
                return;
            }
            if (startrow != 0)
            {
                // Partial first col
                for (GA_Size row = startrow; row < nrows; ++row, v += ncols, ++pointnums)
                    *pointnums = v;

                if (unrollcurves)
                {
                    *pointnums = v-nmeshpoints;
                    ++pointnums;
                }
                ++startcol;
                v -= (nmeshpoints-1);
            }
            // Middle cols
            for (GA_Size col = startcol; col < endcol; ++col, v -= (nmeshpoints-1))
            {
                for (GA_Size row = 0; row < nrows; ++row, v += ncols, ++pointnums)
                    *pointnums = v;

                if (unrollcurves)
                {
                    *pointnums = v-nmeshpoints;
                    ++pointnums;
                }
            }
            // Partial final col
            for (GA_Size row = 0; row < endrow; ++row, v += ncols, ++pointnums)
                *pointnums = v;
        }
    }

private:
    const GA_Size myRows;
    const GA_Size myCols;
    UT_Interrupt *const myInterrupt;
    INT_TYPE *const myPointNums;
    const INT_TYPE myStartPtNum;
    const GEO_SurfaceType myType;
    const bool myUnrollCurves;
    const bool myStartPole;
    const bool myEndPole;
};

} // end of anonymous namespace

#endif

