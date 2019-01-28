/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        GA_PolyCounts.h (GA Library, C++)
 *
 * COMMENTS:    Implementation of a simple run-length encoded array
 *              intended for storing polygon vertex counts.
 */

#ifndef __GA_PolyCounts__
#define __GA_PolyCounts__

#include "GA_API.h"

#include "GA_Types.h"
#include <UT/UT_Array.h>
#include <UT/UT_Assert.h>
#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Types.h>

#include <tbb/tbb_stddef.h>
typedef tbb::split UT_Split;

class GA_API GA_PolyCounts
{
public:

    class SizeCount
    {
    public:
        SizeCount() {}
        SizeCount(int sz, int ct)
            : mySize(sz)
            , myCount(ct)
        {}
        SizeCount(const SizeCount &that) = default;
        const int &size() const { return mySize; }
        const int &count() const { return myCount; }
        int &size() { return mySize; }
        int &count() { return myCount; }
        GA_Size product() const { return GA_Size(mySize)*GA_Size(myCount); }

        typedef int InternalType;
    private:
        int mySize;
        int myCount;
    };

    template<typename CLASS,typename ARRAYCLASS>
    class iteratorT
    {
    public:
        iteratorT() {}
        iteratorT(CLASS &list)
            : myArray(&list.array)
            , myPolygon(0)
            , myStart(0)
            , myIndex(0)
            , myCount(0)
        {}
        iteratorT(CLASS &list, GA_Size polygon)
            : myArray(&list.array)
            , myPolygon(polygon)
        {
            ARRAYCLASS &array = *myArray;
            UT_ASSERT_P(array.entries() != 0);
            GA_Size start = 0;
            GA_Size i;
            for (i = 0; i < array.entries()
                     && polygon >= array(i).count(); ++i)
            {
                start   += array(i).product();
                polygon -= array(i).count();
            }
            if (i < array.entries())
                start += polygon*array(i).size();
            myStart = start;
            myIndex = i;
            myCount = polygon;
        }
        iteratorT(const iteratorT<CLASS, ARRAYCLASS> &that)
            : myArray(that.myArray)
            , myPolygon(that.myPolygon)
            , myStart(that.myStart)
            , myIndex(that.myIndex)
            , myCount(that.myCount)
        {}
        iteratorT(ARRAYCLASS *array, GA_Size polygon, GA_Size start, GA_Size index, GA_Size count)
            : myArray(array)
            , myPolygon(polygon)
            , myStart(start)
            , myIndex(index)
            , myCount(count)
        {}
        SYS_FORCE_INLINE bool atEnd() const
        {
            return myIndex == (*myArray).entries();
        }
        iteratorT<CLASS, ARRAYCLASS> &operator++()
        {
            ++myPolygon;
            myStart += nvertices();
            ++myCount;
            if (myCount == (*myArray)(myIndex).count())
            {
                myCount = 0;
                ++myIndex;
            }
            return *this;
        }
        iteratorT<CLASS, ARRAYCLASS> &operator--()
        {
            if (myCount == 0)
            {
                --myIndex;
                myCount = (*myArray)(myIndex).count();
            }
            --myCount;
            myStart -= nvertices();
            --myPolygon;
            return *this;
        }
        iteratorT<CLASS, ARRAYCLASS> operator+(GA_Size count) const
        {
            if (count == 0)
                return *this;

            ARRAYCLASS &array = *myArray;

            GA_Size polygon = myPolygon + count;
            UT_ASSERT_P(polygon >= 0);
            GA_Size start = myStart;
            GA_Size i = myIndex;
            count += myCount;
            // If in the same block, just adjust start
            if (count >= 0 && count < array(i).count())
            {
                start += (count - myCount)*array(i).size();
            }
            // If past the end of the current block
            else if (count >= array(i).count())
            {
                start += (array(i).count() - myCount)*array(i).size();
                count -= array(i).count();
                for (++i; i < array.entries() && count >= array(i).count(); ++i)
                {
                    start += array(i).product();
                    count -= array(i).count();
                }
                if (i < array.entries())
                    start += count*array(i).size();
            }
            // If before the beginning of the current block
            else
            {
                if (i < array.entries())
                    start -= myCount*array(i).size();
                for (--i; -count >= array(i).count(); --i)
                {
                    start -= array(i).product();
                    count += array(i).count();
                }
                start -= (array(i).count() + count)*array(i).size();
                count += array(i).count();
            }
            return iteratorT<CLASS, ARRAYCLASS>(myArray, polygon, start, i, count);
        }
        iteratorT<CLASS, ARRAYCLASS> operator-(GA_Size count) const
        {
            return (*this)+(-count);
        }
        iteratorT<CLASS, ARRAYCLASS> &operator=(const iteratorT<CLASS, ARRAYCLASS> &that)
        {
            if (&that!=this)
            {
                // NOTE: Cannot assign from an iterator
                //       of a different GA_PolyCounts
                myArray   = that.myArray;
                myPolygon = that.myPolygon;
                myStart   = that.myStart;
                myIndex   = that.myIndex;
                myCount   = that.myCount;
            }
            return *this;
        }
        bool operator==(const iteratorT<CLASS, ARRAYCLASS> &that) const
        {
            // NOTE: For efficiency, cannot compare with an iterator
            //       of a different GA_PolyCounts
            return (myPolygon == that.myPolygon);
        }
        bool operator!=(const iteratorT<CLASS, ARRAYCLASS> &that) const
        {
            // NOTE: For efficiency, cannot compare with an iterator
            //       of a different GA_PolyCounts
            return (myPolygon != that.myPolygon);
        }
        SYS_FORCE_INLINE GA_Size nvertices() const
        {
            return (*myArray)(myIndex).size();
        }
        SYS_FORCE_INLINE GA_Size polygon() const
        {
            return myPolygon;
        }
        SYS_FORCE_INLINE GA_Size start() const
        {
            return myStart;
        }

    private:
        ARRAYCLASS *myArray;
        GA_Size myPolygon;
        GA_Size myStart;
        GA_Size myIndex;
        GA_Size myCount;
    };
    typedef iteratorT<GA_PolyCounts, UT_Array<SizeCount> > iterator;
    typedef iteratorT<const GA_PolyCounts, const UT_Array<SizeCount> > const_iterator;

    /// Range class for multi-threading with TBB
    class Range
    {
    public:
        Range(const GA_PolyCounts &list)
            : myIt(list)
            , myNPolygons(list.getNumPolygons())
            , myGrainSize(1)
        {}
        Range(const GA_PolyCounts &list, GA_Size begin, GA_Size end, GA_Size grainsize = 1)
            : myIt(list)
            , myNPolygons(list.getNumPolygons())
            , myGrainSize(grainsize)
        {}
        Range(Range &that, UT_Split)
            : myIt(that.myIt + that.myNPolygons/2)
            , myNPolygons(that.myNPolygons - (that.myNPolygons/2))
            , myGrainSize(that.myGrainSize)
        {
            that.myNPolygons = that.myNPolygons/2;
        }

        const_iterator begin() const { return myIt; }

        // NOTE: DO NOT use the return value from end() as a real iterator;
        //       It is intended for uses like: it != end()
        //       Constructing a full end iterator is expensive, and if
        //       necessary, can be done with: begin() + size()
        const_iterator end() const
        { return const_iterator(NULL, myIt.polygon() + myNPolygons, 0, 0, 0); }

        GA_Size size() const { return myNPolygons; }
        GA_Size grainsize() const { return myGrainSize; }
        bool empty() const { return myNPolygons == 0; }
        bool is_divisible() const { return myNPolygons >= 2*myGrainSize; }
    private:
        const_iterator myIt;
        GA_Size       myNPolygons;
        const GA_Size myGrainSize;
    };

    GA_PolyCounts()
    {}

    GA_PolyCounts(const GA_PolyCounts &that)
        : array(that.array)
    {}

    GA_PolyCounts &operator=(const GA_PolyCounts &that)
    {
        if (this == &that)
            return *this;

        array = that.array;
        return *this;
    }

    iterator begin() { return iterator(*this); }
    const_iterator begin() const { return const_iterator(*this); }

    // NOTE: DO NOT use the return value from end() as a real iterator;
    //       It is intended for uses like: it != end()
    //       Constructing a full end iterator is expensive, and if
    //       necessary, can be done with: begin() + size()
    const_iterator end() const { return const_iterator(NULL, getNumPolygons(), 0, 0, 0); }

    const_iterator findVertex(GA_Size relativevertex) const
    {
        GA_Size start = relativevertex;
        GA_Size polygon = 0;
        GA_Size i;
        for (i = 0; i < array.entries()
                    && relativevertex >= array(i).product(); ++i)
        {
            relativevertex -= array(i).product();
            polygon += array(i).count();
        }
        GA_Size count = 0;
        if (relativevertex)
        {
            GA_Size remainder;
            SYSdivMod(relativevertex, GA_Size(array(i).size()), count, remainder);
            polygon += count;
            start -= remainder;
        }
        return const_iterator(&array, polygon, start, i, count);
    }

    Range range() const { return Range(*this); }

    UT_Array<SizeCount> &getArray() { return array; }
    const UT_Array<SizeCount> &getArray() const { return array; }

    void clear() { array.clear(); }

    /// Adds "count" polygons with "size" vertices each to the end of the list.
    /// For example, to add 5000000 triangles, call append(3,5000000).
    /// Since this is run-length encoded, if the last entry was already
    /// for polygons with "size" vertices, "count" will just be added to
    /// the last entry.
    void append(GA_Size size, GA_Size count = 1)
    {
        if (count == 0)
            return;
        if (array.entries() && array.last().size() == size)
            array.last().count() += count;
        else
            array.append(SizeCount(size, count));
    }

    void append(const GA_PolyCounts &that)
    {
        if (!that.array.entries())
            return;

        if (array.entries() && array.last().size() == that.array(0).size())
            array.last().count() += that.array(0).count();
        else
            array.append(that.array(0));

        for (GA_Size i = 1; i < that.array.entries(); ++i)
            array.append(that.array(i));
    }

    GA_Size getNumPolygons() const {
        GA_Size sum = 0;
        for (GA_Size i = 0; i < array.entries(); ++i)
            sum += array(i).count();
        return sum;
    }

    GA_Size getNumVertices() const {
        GA_Size sum = 0;
        for (GA_Size i = 0; i < array.entries(); ++i)
            sum += array(i).product();
        return sum;
    }

    /// Simpler than getVertexRange if you just want the polygon size
    GA_Size getSize(GA_Size polygon) const
    {
        UT_ASSERT_P(array.entries() != 0);
        GA_Size i;
        for (i = 0; polygon >= array(i).count(); ++i)
            polygon -= array(i).count();
        return array(i).size();
    }

    void getVertexRange(GA_Size polygon, GA_Size &start, GA_Size &size) const
    {
        UT_ASSERT_P(array.entries() != 0);
        GA_Size index = 0;
        GA_Size i;
        for (i = 0; polygon >= array(i).count(); ++i)
        {
            index   += array(i).product();
            polygon -= array(i).count();
        }
        index += polygon*array(i).size();
        start = index;
        size = array(i).size();
    }

    int64 getMemoryUsage(bool inclusive) const
    { return (inclusive ? sizeof(*this) : 0) + array.getMemoryUsage(false); }

private:
    UT_Array<SizeCount> array;
};

/// This is needed by the tbb wrapper in UT_ParallelUtil
/// It returns an upper bound for the actual number of work items
/// in the range.
inline GA_API size_t UTestimatedNumItems(const GA_PolyCounts::Range& range)
{
    return range.size();
}

#endif
