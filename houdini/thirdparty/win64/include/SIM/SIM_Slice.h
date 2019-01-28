/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_Slice_h__
#define __SIM_Slice_h__

#include "SIM_API.h"
#include "SIM_DataUtils.h"
#include <UT/UT_Vector3.h>
#include <UT/UT_Array.h>
#include <UT/UT_VectorTypes.h>

class GU_ConstDetailHandle;
class SIM_Geometry;
class SIM_Position;
class SIM_Motion;

class SIM_Slice;

class SIM_SliceSearchEntry
{
public:
    SIM_SliceSearchEntry()
    {
	mySlice = 0;
	myInside = false;
    }
    SIM_SliceSearchEntry(const SIM_Slice *slice, bool isinside)
    {
	mySlice = slice;
	myInside = isinside;
    }
    const SIM_Slice	*mySlice;
    bool		 myInside;
};

typedef UT_Array<SIM_SliceSearchEntry> SIM_SliceSearchList;
typedef UT_Array<SIM_SliceSearchList> SIM_SliceSearchTable;

/// This is the definition of the SIM_Slice interface. The only function that
/// needs to be overridden (besides any standard SIM_Data overrides) is the
/// getSliceBoundaryDistSubclass() functions.
class SIM_API SIM_Slice : public SIM_Data
{
public:
    /// Returns whether we are inside this slice or not.
    /// true is inside.  This corresponds to a negative slice
    /// boundary distance but is overrideable for efficiency.
    bool		 isInsideSlice(const UT_Vector3 &pos) const;

    /// Returns the distance to the slice's boundary.  If the point
    /// is inside the slice this is a negative distance, else positive.
    fpreal		 getSliceBoundaryDist(const UT_Vector3 &pos) const;

    /// This is used to determine which side of my parent I am on.
    virtual UT_Vector3	 getCenter() const;

    /// Count number of slices from myself and my children.
    int			 countSlices() const;
    static int		 countSlices(const SIM_ConstDataArray &slices);

    /// The nth entry of this list stores how to search for the nth
    /// slice.
    void		 buildSliceSearch(SIM_SliceSearchTable &table) const;
    static void		 buildSliceSearch(SIM_SliceSearchTable &table,
				    SIM_ConstDataArray &slices);

    static fpreal	 computeSliceDist(const SIM_SliceSearchList &list,
					const UT_Vector3 &pos);
    /// This is different than a computeSliceDist < 0 because
    /// we need to ensure that we are consistent with <= against
    /// a zero result...
    static bool		 isInsideSlice(const SIM_SliceSearchList &list,
					const UT_Vector3 &pos);

    /// This returns true only if the bounding box is fully contained
    /// in the slice.  Assumes convex slices.
    static bool		 isInsideSlice(const SIM_SliceSearchList &list,
					const UT_BoundingBox &bbox);

protected:
    explicit		 SIM_Slice(const SIM_DataFactory *factory);
    virtual		~SIM_Slice();

    /// Returns whether we are inside this slice or not.
    /// true is inside.  This corresponds to a negative slice
    /// boundary distance but is overrideable for efficiency.
    /// Default implementation just compares the distance with 0.
    virtual bool	 isInsideSliceSubclass(const UT_Vector3 &pos) const;

    /// Returns the distance to the slice's boundary.  If the point
    /// is inside the slice this is a negative distance, else positive.
    /// This is what must be overridden to make the slice do something
    /// interesting
    virtual fpreal	 getSliceBoundaryDistSubclass(const UT_Vector3 &pos) const = 0;

    /// This is used to determine which side of my parent I am on.
    virtual UT_Vector3	 getCenterSubclass() const = 0;

private:
    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_CLASSNAME(SIM_Slice, SIM_Data);
};

#endif

