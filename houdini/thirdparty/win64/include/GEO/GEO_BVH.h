/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        GEO_BVH.h (GEO Library, C++)
 *
 * COMMENTS:    Bounding Volume Hierarchy (BVH) implementation for GEO_Detail.
 */

#pragma once

#include "GEO_API.h"
#include <GA/GA_Handle.h>
#include <GA/GA_Types.h>
#include <UT/UT_BVH.h>
#include <UT/UT_UniquePtr.h>
#include <UT/UT_Vector3.h>
#include <limits>

#define GEO_BVH_INTERLEAVED 1
#if GEO_BVH_INTERLEAVED
#include <VM/VM_SIMD.h>
#endif

class GEO_Detail;

namespace GEO {

template<uint NAXES,typename SUBCLASS>
class BVHBase
{
public:
    BVHBase() noexcept
        : myHasCurvesOrPoints(false)
    {}
    ~BVHBase() noexcept {}

    using VectorType = typename SYS_SelectType<UT_Vector2,UT_Vector3,NAXES==3>::type;
    using UintVectorType = typename SYS_SelectType<UT_FixedVector<uint,2>,UT_FixedVector<uint,3>,NAXES==3>::type;
    using SingleBoxType = UT::Box<float,NAXES>;
#if !GEO_BVH_INTERLEAVED
    using BoxType = UT::Box<float,NAXES>;
#else
    using BoxType = UT::Box<v4uf,NAXES>;
#endif

protected:
    void clear() noexcept;
public:

    SYS_FORCE_INLINE bool isEmpty() const noexcept
    {
        return myTree.getNumNodes() == 0;
    }

    struct ItemHitInfo
    {
        exint myItemIndex = -1;
        UT_Vector3 myUVW;
    };
    struct CommonHitInfo : public ItemHitInfo
    {
        using ItemHitInfo::myItemIndex;
        using ItemHitInfo::myUVW;

        /// This can be used for packed primitive hits.
        UT_Array<exint> *myNestedItemIndices = nullptr;

        SYS_FORCE_INLINE bool isHit() const noexcept
        {
            return (myItemIndex >= 0);
        }
    };
    struct HitInfo : public CommonHitInfo
    {
        float myT = -1;
    };
    struct HitInfoAndNormal : public HitInfo
    {
        VectorType myNml;
    };

    /// Sends a ray from the specified origin in the specified direction and
    /// finds the closest (or farthest) intersection.
    /// Potential hits less than tmin or greater than tmax from the origin are
    /// treated as not having been hit, regardless of whether farthest is true.
    /// If rm_backface is true, backface hits will be treated as no hit,
    /// and if reverse is also true, frontface hits will be treated as no hit.
    /// NOTE: If the hit_info.myNestedItemIndices is non-null,
    ///       it will be filled with the hit path,
    ///       ignoring the outermost index, which is in hit_info.myItemIndex.
    template<bool farthest=false,bool rm_backface=false,bool reverse=false,typename HitInfoType>
    void sendRay(const VectorType &origin, const VectorType &direction, HitInfoType &hit_info,
        float tmin = 0, float tmax = std::numeric_limits<float>::max()) const noexcept;

    /// Like sendRay, except with a radius (tolerance) for curve and point hits,
    /// in case myRadii is empty.
    template<bool farthest=false,bool rm_backface=false,bool reverse=false,typename HitInfoType>
    void sendRayRad(const VectorType &origin, const VectorType &direction, HitInfoType &hit_info,
        float default_radius,
        float tmin = 0, float tmax = std::numeric_limits<float>::max()) const noexcept;

    /// Sends a ray from the specified origin in the specified direction and
    /// finds all intersections, removing all but one that are within
    /// duplicate_tolerance of another hit.
    /// Potential hits less than tmin or greater than tmax from the origin are
    /// treated as not having been hit, regardless of whether farthest is true.
    /// If rm_backface is true, backface hits will be treated as no hit,
    /// and if reverse is also true, frontface hits will be treated as no hit.
    /// NOTE: If nesting_temp_array is non-null, this function will allocate
    ///       nesting arrays for any applicable entries in hit_info, so the
    ///       caller is responsible for deleting myNestedItemIndices arrays in
    ///       hit_info.  nesting_temp_array itself is just used for temporary
    ///       storage to reduce the number of allocations.
    template<bool rm_backface=false,bool reverse=false,typename HitInfoType>
    void sendRayAll(const VectorType &origin, const VectorType &direction, UT_Array<HitInfoType> &hit_info,
        UT_Array<exint> *nesting_temp_array=nullptr,
        float duplicate_tolerance = 0,float tmin = 0, float tmax = std::numeric_limits<float>::max()) const noexcept;

    /// Like sendRayAll, except with a radius (tolerance) for curve and point hits,
    /// in case myRadii is empty.
    template<bool rm_backface=false,bool reverse=false,typename HitInfoType>
    void sendRayAllRad(const VectorType &origin, const VectorType &direction, UT_Array<HitInfoType> &hit_info,
        float default_radius, UT_Array<exint> *nesting_temp_array=nullptr,
        float duplicate_tolerance = 0,float tmin = 0, float tmax = std::numeric_limits<float>::max()) const noexcept;

private:
    template<bool USE_TOLERANCE>
    struct SingleHitFunctor;
    template<bool USE_TOLERANCE>
    struct SingleHitAndNormalFunctor;
    template<bool USE_TOLERANCE>
    struct AllHitsFunctor;
    template<bool USE_TOLERANCE>
    struct AllHitsAndNormalsFunctor;
public:

    template<bool farthest,bool rm_backface,bool reverse,typename FUNCTOR>
    void sendRayGeneric(VectorType origin, VectorType direction, FUNCTOR &hit_info,
        float tmin = 0, float tmax = std::numeric_limits<float>::max()) const noexcept;

    struct MinInfo : public CommonHitInfo
    {
        float myDistSquared = -1;
        VectorType myP;
    };

    /// Finds the closest (or farthest) position to origin on any surface, as long as it's within max_dist_squared.
    /// NOTE: If farthest is true, max_dist_squared is actually the *minimum* distance squared from the origin,
    ///       so you will need to specify a value.
    template<bool farthest>
    void findClosest(VectorType origin, MinInfo &min_info, float max_dist_squared = std::numeric_limits<float>::max()) const noexcept;

    /// Fills box_indices array with the indices of all boxes intersecting query_box.
    /// NOTE: This does not clear out previous contents of indices, so that you
    ///       can easily query intersection with multiple boxes.
    /// WARNING: DO NOT depend on the order of box_indices.  If you need a consistent order, sort it.
    void getIntersectingBoxes(const SingleBoxType &query_box, UT_Array<exint> &box_indices) const noexcept;

    SYS_FORCE_INLINE exint numPoints() const noexcept
    {
        return myPoints.size();
    }
    SYS_FORCE_INLINE GA_Offset pointOffset(exint item_index) const noexcept
    {
        UT_ASSERT_P(item_index < numPoints());
        return myPoints(item_index);
    }

    /// Returns the geometric normal (not based on the N attribute) for the hit surface,
    /// optionally normalized, since apparently Mantra doesn't need it normalized.
    template<bool normalize=true>
    VectorType getGeometricNormal(const CommonHitInfo &hit_info) const noexcept;

    /// Fills in the values of dP/du and dP/dv for the hit surface.
    void getDerivs(const CommonHitInfo &hit_info, VectorType &dP_du, VectorType &dP_dv) const noexcept;

    /// For points, this converts the uvw from ItemHitInfo to polar form.
    /// The original is effectively a normal.
    static void pointUVWToPolar(VectorType &uvw) noexcept;

    /// Returns true on success, false on failure.  The function can fail if the hit is on a point
    /// and the attribute is a vertex or primitive attribute.
    template<GA_AttributeOwner owner,typename T,typename DEST_T>
    bool getAttribute(const CommonHitInfo &hit_info, const GA_ROHandleT<T> &attrib, const GEO_Detail &detail, DEST_T &value) const noexcept;

    SingleBoxType getBBox() const noexcept
    {
        SingleBoxType bbox;
        if (isEmpty())
        {
            bbox.initBounds();
            return bbox;
        }

        BoxType &root_boxes = myNodeBoxes[0];
        for (uint axis = 0; axis < NAXES; ++axis) {
            SYS_STATIC_ASSERT(sizeof(root_boxes[axis][0]) == 4*sizeof(float));
            float *vmin = (float*)&root_boxes[axis][0];
            bbox[axis][0] = SYSmin(vmin[0],vmin[1],vmin[2],vmin[3]);
            float *vmax = (float*)&root_boxes[axis][1];
            bbox[axis][1] = SYSmax(vmax[0],vmax[1],vmax[2],vmax[3]);
        }
        return bbox;
    }

protected:
    SYS_FORCE_INLINE SUBCLASS *subclass() noexcept { return static_cast<SUBCLASS*>(this); }
    SYS_FORCE_INLINE const SUBCLASS *subclass() const noexcept { return static_cast<const SUBCLASS*>(this); }

    using NodeData = BoxType;

#if !GEO_BVH_INTERLEAVED
    static constexpr uint BVH_N = 2;
#else
    static constexpr uint BVH_N = 4;
#endif
    UT_BVH<BVH_N> myTree;
    UT_UniquePtr<BoxType[]> myNodeBoxes;
    //UT_UniquePtr<uint[]> myIndices;
    UT_UniquePtr<UT_FixedVector<int32,BVH_N>[]> myNodeNItems;

    /// Positions for points
    UT_Array<VectorType> myPositions;
    //GA_ROHandleT<VectorType> myPositions;

    /// Radii for disconnected points
    UT_Array<float> myRadii;
    //GA_ROHandleF myRadii;

    /// Disconnected points
    GA_OffsetList myPoints;

    /// Data IDs for myPositions and myRadii
    /// @{
    GA_DataId myPositionsDataId;
    GA_DataId myRadiiDataId;
    /// @}

    bool myHasCurvesOrPoints;
};

#if 0
template<uint NAXES>
struct QueryPtWrapper {
    template<bool INSTANTIATED>
    SYS_FORCE_INLINE QueryPtWrapper(const UT_FixedVector<float,NAXES,INSTANTIATED> &p)
        : q(p), vq(p)
    {}

    /// This is used for findClosest.
    /// It can optionally return an underestimate, but NOT an overestimate.
    /// @{
    SYS_FORCE_INLINE float boxMinDist2(const UT::Box<float,NAXES> &box) const
    { return box.minDistance2(q); }
    SYS_FORCE_INLINE v4uf boxMinDist2(const UT::Box<v4uf,NAXES> &box) const
    { return box.minDistance2(vq); }
    /// @}

    /// This is used for findFarthest.
    /// It can optionally return an overestimate, but NOT an underestimate.
    /// @{
    SYS_FORCE_INLINE float boxMaxDist2(const UT::Box<float,NAXES> &box) const
    { return box.maxDistance2(q); }
    SYS_FORCE_INLINE v4uf boxMaxDist2(const UT::Box<v4uf,NAXES> &box) const
    { return box.maxDistance2(vq); }
    /// @}

    /// This should be exact
    template<bool INSTANTIATED>
    SYS_FORCE_INLINE float distance2(const UT_FixedVector<float,NAXES,INSTANTIATED> &p) const
    { return q.distance2(p); }

    /// These are for culling the query space
    /// @{
    SYS_FORCE_INLINE constexpr bool isValid(const UT::Box<float,NAXES> &box) const
    { return true; }
    static constexpr bool theHasBoxValidCheck = false;
    SYS_FORCE_INLINE constexpr v4uu isValid(const UT::Box<v4uf,NAXES> &box) const
    { return v4uu(1); }
    template<typename INT_TYPE>
    SYS_FORCE_INLINE constexpr bool isValid(INT_TYPE index) const
    { return true; }
    template<bool INSTANTIATED>
    SYS_FORCE_INLINE constexpr bool isValid(const UT_FixedVector<float,NAXES,INSTANTIATED> &p) const
    { return true; }
    /// @}

private:
    const UT_FixedVector<float,NAXES> q;
    const UT_FixedVector<v4uf,NAXES> vq;
};

template<uint NAXES,bool INSTANTIATED>
SYS_FORCE_INLINE QueryPtWrapper<NAXES> QueryPoint(const UT_FixedVector<float,NAXES,INSTANTIATED> &p)
{ return QueryPtWrapper<NAXES>(p); }
#endif

template<uint NAXES>
class GEO_API PointBVHT : public BVHBase<NAXES,PointBVHT<NAXES>>
{
public:
    using Parent = BVHBase<NAXES,PointBVHT<NAXES>>;
    using typename Parent::VectorType;
    using typename Parent::UintVectorType;
    using typename Parent::SingleBoxType;
    using typename Parent::BoxType;

    SYS_FORCE_INLINE PointBVHT() noexcept : Parent() {}
    SYS_FORCE_INLINE ~PointBVHT() noexcept {}

    /// NOTE: With this signature, radius is the point radius.
    SYS_FORCE_INLINE
    void init(const GA_Detail &detail,
        const GA_ROHandleT<VectorType> &P,
        const float radius = 0.0f,
        const GA_Range *point_range=nullptr,
        const bool force_rebalance=false) noexcept
    {
        init(detail, P, GA_ROHandleF(), radius, point_range, force_rebalance);
    }

    /// NOTE: With this signature, radscale scales the pscale attribute
    ///       if it's a valid attribute, else it's the point radius.
    void init(const GA_Detail &detail,
        const GA_ROHandleT<VectorType> &P,
        const GA_ROHandleF &pscale,
        const float radscale = 1.0f,
        const GA_Range *point_range=nullptr,
        const bool force_rebalance=false) noexcept;

#if 0
    /// Find the closest point to the position specified.  This method
    /// returns -1 if no point is found.
    /// NOTE THAT max_distance_squared IS SQUARED DISTANCE.
    template<bool farthest,typename QueryPointType>
    exint findClosest(
        const QueryPointType &pt,
        QueueType &queue,
        exint max_points = std::numeric_limits<exint>::max(),
        IntArrayType *list = nullptr,
        float max_distance_squared = std::numeric_limits<float>::max()) const noexcept;
#endif

    using typename Parent::ItemHitInfo;
    using typename Parent::CommonHitInfo;
    using typename Parent::HitInfo;
    using typename Parent::MinInfo;
    using Parent::numPoints;
    using Parent::isEmpty;
    using Parent::clear;

    /// Finds the closest points (up to max_points of them)
    /// within sqrt(max_distance_squared) of query_pt
    SYS_FORCE_INLINE
    void findClosestPoints(
        const VectorType &query_pt,
        UT::BVHOrderedStack &stack,
        UT::BVHOrderedStack &output_queue,
        exint max_points = std::numeric_limits<uint>::max(),
        float max_distance_squared = std::numeric_limits<float>::max()) const noexcept
    {
        findMaximalPoints<false>(query_pt, stack,
            output_queue, max_points, max_distance_squared);
    }

    /// Finds the farthest points (up to max_points of them)
    /// at least sqrt(min_distance_squared) away from query_pt
    SYS_FORCE_INLINE
    void findFarthestPoints(
        const VectorType &query_pt,
        UT::BVHOrderedStack &stack,
        UT::BVHOrderedStack &output_queue,
        exint max_points = std::numeric_limits<uint>::max(),
        float min_distance_squared = 0.0f) const noexcept
    {
        findMaximalPoints<true>(query_pt, stack,
            output_queue, max_points, min_distance_squared);
    }

    /// Sorts results from findClosestPoints or findFarthestPoints by distance
    /// squared, and, in the case of ties, by the point offsets.
    /// If farthest is true, it puts farther distances first, and breaks
    /// ties with larger offsets first.
    template<bool farthest>
    void sortResults(UT::BVHOrderedStack &closepts) const noexcept;

    template<bool farthest>
    void findMaximalPoints(
        const VectorType &query_pt,
        UT::BVHOrderedStack &stack,
        UT::BVHOrderedStack &output_queue,
        exint max_points,
        float max_distance_squared) const noexcept;

    /// Don't call this unless you really need to.  It might have to change in the future.
    /// The array is owned by myPoints.
    const exint *getPointsArray() const noexcept;

protected:
    static constexpr bool theHasPrimitives = false;

    using Parent::myTree;
    using Parent::myNodeBoxes;
    using Parent::myNodeNItems;
    using Parent::myPositions;
    using Parent::myRadii;
    using Parent::myPoints;
    using Parent::myPositionsDataId;
    using Parent::myRadiiDataId;
    using Parent::myHasCurvesOrPoints;
    using Parent::BVH_N;

    template<bool farthest,bool rm_backface,bool reverse,typename FUNCTOR>
    SYS_FORCE_INLINE bool intersectPrim(
        uint index, const VectorType &origin, const VectorType &direction,
        const VectorType &inverse_direction,
        int &max_dir, VectorType &N0, VectorType &N1,
        float &outer_tmax, float &outer_tmin, FUNCTOR &hit_info) const noexcept
    {
        UT_ASSERT_MSG(0, "Base class should deal with intersecting points");
        return false;
    }
    template<bool farthest>
    SYS_FORCE_INLINE void closestPrim(
        uint index, const VectorType &origin, float &max_dist_squared,
        exint &hit_index, UT_Vector3 &hit_uvw, VectorType &hit_position,
        const UT_FixedVector<v4uf,NAXES> &vorigin,
        UT_Array<exint> *nesting_array,
        exint nesting_array_base) const noexcept
    {
        UT_ASSERT_MSG(0, "Base class should deal with findign closest points");
    }
    template<bool normalize>
    SYS_FORCE_INLINE VectorType primGeometricNormal(const CommonHitInfo &hit_info) const noexcept
    {
        UT_ASSERT_MSG(0, "Base class should deal with normals for points");
        return VectorType(float(0));
    }
    SYS_FORCE_INLINE void primDerivs(const CommonHitInfo &hit_info, VectorType &dP_du, VectorType &dP_dv) const noexcept
    {
        UT_ASSERT_MSG(0, "Base class should deal with derivatives for points");
    }
    template<GA_AttributeOwner owner,typename T,typename DEST_T>
    SYS_FORCE_INLINE bool primAttribute(const CommonHitInfo &hit_info, const GA_ROHandleT<T> &attrib, const GEO_Detail &detail, DEST_T &value) const noexcept
    {
        UT_ASSERT_MSG(0, "Base class should deal with attributes for points");
        return false;
    }

private:
    friend Parent;
};

using PointBVH = PointBVHT<3>;
using PointBVH_2D = PointBVHT<2>;

} // GEO namespace

#undef GEO_BVH_INTERLEAVED
