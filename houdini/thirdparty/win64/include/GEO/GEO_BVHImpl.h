/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        GEO_BVHImpl.h (GEO Library, C++)
 *
 * COMMENTS:    Bounding Volume Hierarchy (BVH) implementation for GEO_Detail.
 */

#pragma once

#include "GEO_BVH.h"
#include <GA/GA_Detail.h>
#include <GA/GA_Handle.h>
#include <GA/GA_Iterator.h>
#include <GA/GA_SplittableRange.h>
#include <GA/GA_Types.h>
#include <UT/UT_Array.h>
#include <UT/UT_BVH.h>
#include <UT/UT_BVHImpl.h>
#include <UT/UT_ParallelUtil.h>
#include <UT/UT_RootFinder.h>
#include <UT/UT_SmallArray.h>
#include <UT/UT_StopWatch.h>
#include <SYS/SYS_Math.h>
#include <limits>

#define GEO_BVH_INTERLEAVED 1

namespace GEO {

template<uint NAXES,typename SUBCLASS>
void BVHBase<NAXES,SUBCLASS>::clear() noexcept
{
    myTree.clear();
    myNodeBoxes.reset();
    //myIndices.reset();
    myPositions.setCapacity(0);
    myRadii.setCapacity(0);
    //myPositions.clear();
    //myRadii.clear();
    myPoints.clear();
    myHasCurvesOrPoints = false;
}

template<typename VectorType>
static bool
geoIntersectSphere(
    VectorType rel_origin, const VectorType &direction,
    float radius, float &t0, float &t1)
{
    // NOTE: direction should already be normalized!

    // To stabilize the quadratic equation, we move closer to the centre of the
    // sphere...
    float rayt = SYSsqrt(rel_origin.length2()); // SYSsqrt(rel_origin.length2() / direction.length2());
    VectorType p = rel_origin + rayt*direction;

    //float a = direction.length2();
    float b = 2.0F * dot(p, direction);
    float c = p.length2() - radius*radius;

    float d = b*b - 4.0F*c; // b*b - 4.0F*a*c
    if (d < 0)
        return false;

    //a = 0.5F/a;
    d = SYSsqrt(d);

    t0 = rayt-(b+d); //rayt + a*(-b-d);

    // TODO: Exclude t1 if rmbackface
    t1 = rayt-(b-d); //rayt + a*(-b+d);

    return true;
}

template<uint NAXES,typename SUBCLASS>
template<bool USE_TOLERANCE>
struct BVHBase<NAXES,SUBCLASS>::SingleHitFunctor
{
    SYS_FORCE_INLINE SingleHitFunctor(
        HitInfo &hit_info) noexcept
        : myHitInfo(hit_info)
        , myNestingArrayBase(hit_info.myNestedItemIndices ? hit_info.myNestedItemIndices->size() : 0)
    {}
    SYS_FORCE_INLINE void noHit() noexcept
    {
        myHitInfo.myItemIndex = -1;
        myHitInfo.myUVW.assign(0,0,0);
        myHitInfo.myT = -1.0f;
    }
    SYS_FORCE_INLINE void insertHit(
        const UT_Vector3 &hit_uvw,
        const float t,
        const uint index,
        float &limit_t) noexcept
    {
        myHitInfo.myItemIndex = index;
        myHitInfo.myUVW = hit_uvw;
        // Clear local portion of nesting array
        if (myHitInfo.myNestedItemIndices)
            myHitInfo.myNestedItemIndices->setSize(myNestingArrayBase);
        myHitInfo.myT = t;
        limit_t = t;
    }
    SYS_FORCE_INLINE void setNestingArrayInto(HitInfo &hit_info) noexcept
    {
        hit_info.myNestedItemIndices = myHitInfo.myNestedItemIndices;
    }
    SYS_FORCE_INLINE UT_Array<exint> *nestingArray() noexcept
    {
        return myHitInfo.myNestedItemIndices;
    }
    SYS_FORCE_INLINE exint nestingArrayBase() noexcept
    {
        return myNestingArrayBase;
    }
    SYS_FORCE_INLINE VectorType getNormal(const HitInfo &hit_info) const noexcept
    { return VectorType(); }
    SYS_FORCE_INLINE void setNormal(const VectorType &nml) noexcept {}
    SYS_FORCE_INLINE void setNormal(HitInfo &hit_info, const VectorType &nml) const noexcept {}
    SYS_FORCE_INLINE UT_Array<HitInfo> *getHitArray() noexcept { return nullptr; }
    SYS_FORCE_INLINE exint nestingArrayBase() const noexcept { return myNestingArrayBase; }

    constexpr static bool theAllHits = false;
    constexpr static bool theNeedsNormal = false;
    constexpr static bool theUseTolerance = USE_TOLERANCE;

    using InfoType = HitInfo;

    HitInfo &myHitInfo;
    exint myNestingArrayBase;
    float myTolerance;
};

template<uint NAXES,typename SUBCLASS>
template<bool USE_TOLERANCE>
struct BVHBase<NAXES,SUBCLASS>::SingleHitAndNormalFunctor
{
    SYS_FORCE_INLINE SingleHitAndNormalFunctor(
        HitInfoAndNormal &hit_info) noexcept
        : myHitInfo(hit_info)
        , myNestingArrayBase(hit_info.myNestedItemIndices ? hit_info.myNestedItemIndices->size() : 0)
    {}
    SYS_FORCE_INLINE void noHit() noexcept
    {
        myHitInfo.myItemIndex = -1;
        myHitInfo.myUVW.assign(0,0,0);
        myHitInfo.myT = -1.0f;
    }
    SYS_FORCE_INLINE void insertHit(
        const UT_Vector3 &hit_uvw,
        const float t,
        const uint index,
        float &limit_t) noexcept
    {
        myHitInfo.myItemIndex = index;
        myHitInfo.myUVW = hit_uvw;
        // Clear local portion of nesting array
        if (myHitInfo.myNestedItemIndices)
            myHitInfo.myNestedItemIndices->setSize(myNestingArrayBase);
        myHitInfo.myT = t;
        limit_t = t;
    }
    SYS_FORCE_INLINE void setNestingArrayInto(HitInfoAndNormal &hit_info) noexcept
    {
        hit_info.myNestedItemIndices = myHitInfo.myNestedItemIndices;
    }
    SYS_FORCE_INLINE UT_Array<exint> *nestingArray() noexcept
    {
        return myHitInfo.myNestedItemIndices;
    }
    SYS_FORCE_INLINE const VectorType &getNormal(const HitInfoAndNormal &hit_info) const noexcept
    { return hit_info.myNml; }
    SYS_FORCE_INLINE void setNormal(const VectorType &nml) noexcept
    {
        myHitInfo.myNml = nml;
    }
    SYS_FORCE_INLINE void setNormal(HitInfoAndNormal &hit_info, const VectorType &nml) const noexcept
    {
        hit_info.myNml = nml;
    }
    SYS_FORCE_INLINE UT_Array<HitInfoAndNormal> *getHitArray() noexcept { return nullptr; }
    SYS_FORCE_INLINE exint nestingArrayBase() const noexcept { return myNestingArrayBase; }

    constexpr static bool theAllHits = false;
    constexpr static bool theNeedsNormal = true;
    constexpr static bool theUseTolerance = USE_TOLERANCE;

    using InfoType = HitInfoAndNormal;

    HitInfoAndNormal &myHitInfo;
    exint myNestingArrayBase;
    float myTolerance;
};

template<uint NAXES,typename SUBCLASS>
template<bool USE_TOLERANCE>
struct BVHBase<NAXES,SUBCLASS>::AllHitsFunctor
{
    SYS_FORCE_INLINE AllHitsFunctor(
        UT_Array<HitInfo> &hit_info,
        UT_Array<exint> *nesting_temp_array) noexcept
        : myHitInfo(hit_info)
        , myNestingTempArray(nesting_temp_array)
    {}
    SYS_FORCE_INLINE void noHit() noexcept
    {
        // Just don't add a hit.
    }
    SYS_FORCE_INLINE void insertHit(
        const UT_Vector3 &hit_uvw,
        const float t,
        const uint index,
        float &limit_t) noexcept
    {
        HitInfo hit_info;
        if (!myNestingTempArray || myNestingTempArray->isEmpty())
        {
            // Not nested
            hit_info.myItemIndex = index;
        }
        else
        {
            // myItemIndex is the top-most index.
            hit_info.myItemIndex = (*myNestingTempArray)[0];
            UT_Array<exint> *new_array = new UT_Array<exint>();
            hit_info.myNestedItemIndices = new_array;
            exint n = myNestingTempArray->size();
            new_array->setSizeNoInit(n);
            for (exint i = 1; i < n; ++i)
            {
                (*new_array)[i-1] = (*myNestingTempArray)[i];
            }
            (*new_array)[n-1] = index;
        }
        hit_info.myUVW = hit_uvw;
        hit_info.myT = t;
        myHitInfo.append(hit_info);
    }
    SYS_FORCE_INLINE void setNestingArrayInto(HitInfo &hit_info) noexcept {}
    SYS_FORCE_INLINE UT_Array<exint> *nestingArray() noexcept
    {
        return myNestingTempArray;
    }
    SYS_FORCE_INLINE VectorType getNormal(const HitInfo &hit_info) const noexcept
    { return VectorType(); }
    SYS_FORCE_INLINE void setNormal(const VectorType &nml) noexcept {}
    SYS_FORCE_INLINE void setNormal(HitInfo &hit_info, const VectorType &nml) const noexcept {}
    SYS_FORCE_INLINE UT_Array<HitInfo> *getHitArray() noexcept { return &myHitInfo; }
    SYS_FORCE_INLINE exint nestingArrayBase() const noexcept { return 0; }

    constexpr static bool theAllHits = true;
    constexpr static bool theNeedsNormal = false;
    constexpr static bool theUseTolerance = USE_TOLERANCE;

    /// NOTE: Intentionally not the array type.  See use in GU_BVH::intersectPrim
    using InfoType = HitInfo;

    UT_Array<HitInfo> &myHitInfo;
    UT_Array<exint> *const myNestingTempArray;
    float myTolerance;
};

template<uint NAXES,typename SUBCLASS>
template<bool USE_TOLERANCE>
struct BVHBase<NAXES,SUBCLASS>::AllHitsAndNormalsFunctor
{
    SYS_FORCE_INLINE AllHitsAndNormalsFunctor(
        UT_Array<HitInfoAndNormal> &hit_info,
        UT_Array<exint> *nesting_temp_array) noexcept
        : myHitInfo(hit_info)
        , myNestingTempArray(nesting_temp_array)
    {}
    SYS_FORCE_INLINE void noHit() noexcept
    {
        // Just don't add a hit.
    }
    SYS_FORCE_INLINE void insertHit(
        const UT_Vector3 &hit_uvw,
        const float t,
        const uint index,
        float &limit_t) noexcept
    {
        HitInfoAndNormal hit_info;
        if (!myNestingTempArray || myNestingTempArray->isEmpty())
        {
            // Not nested
            hit_info.myItemIndex = index;
        }
        else
        {
            // myItemIndex is the top-most index.
            hit_info.myItemIndex = (*myNestingTempArray)[0];
            UT_Array<exint> *new_array = new UT_Array<exint>();
            hit_info.myNestedItemIndices = new_array;
            exint n = myNestingTempArray->size();
            new_array->setSizeNoInit(n);
            for (exint i = 1; i < n; ++i)
            {
                (*new_array)[i-1] = (*myNestingTempArray)[i];
            }
            (*new_array)[n-1] = index;
        }
        hit_info.myUVW = hit_uvw;
        hit_info.myT = t;
        myHitInfo.append(hit_info);
    }
    SYS_FORCE_INLINE void setNestingArrayInto(HitInfoAndNormal &hit_info) noexcept {}
    SYS_FORCE_INLINE UT_Array<exint> *nestingArray() noexcept
    {
        return myNestingTempArray;
    }
    SYS_FORCE_INLINE VectorType getNormal(const HitInfoAndNormal &hit_info) const noexcept
    { return hit_info.myNml; }
    SYS_FORCE_INLINE void setNormal(const VectorType &nml) noexcept
    {
        myHitInfo.last().myNml = nml;
    }
    SYS_FORCE_INLINE void setNormal(HitInfoAndNormal &hit_info, const VectorType &nml) const noexcept
    {
        hit_info.myNml = nml;
    }
    SYS_FORCE_INLINE UT_Array<HitInfoAndNormal> *getHitArray() noexcept { return &myHitInfo; }
    SYS_FORCE_INLINE exint nestingArrayBase() const noexcept { return 0; }

    constexpr static bool theAllHits = true;
    constexpr static bool theNeedsNormal = true;
    constexpr static bool theUseTolerance = USE_TOLERANCE;

    /// NOTE: Intentionally not the array type.  See use in GU_BVH::intersectPrim
    using InfoType = HitInfoAndNormal;

    UT_Array<HitInfoAndNormal> &myHitInfo;
    UT_Array<exint> *const myNestingTempArray;
    float myTolerance;
};

template<uint NAXES,typename SUBCLASS>
template<bool farthest,bool rm_backface,bool reverse,typename HitInfoType>
void BVHBase<NAXES,SUBCLASS>::sendRay(
    const VectorType &origin,
    const VectorType &direction,
    HitInfoType &hit_info,
    float outer_tmin,
    float outer_tmax) const noexcept
{
    typename SYS_SelectType<SingleHitFunctor<false>, SingleHitAndNormalFunctor<false>, SYS_IsSame<HitInfoType, HitInfoAndNormal>::value>::type functor(hit_info);
    sendRayGeneric<farthest,rm_backface,reverse>(origin, direction, functor, outer_tmin, outer_tmax);
}

template<uint NAXES,typename SUBCLASS>
template<bool farthest,bool rm_backface,bool reverse,typename HitInfoType>
void BVHBase<NAXES,SUBCLASS>::sendRayRad(
    const VectorType &origin,
    const VectorType &direction,
    HitInfoType &hit_info,
    float default_radius,
    float outer_tmin,
    float outer_tmax) const noexcept
{
    if (!myHasCurvesOrPoints || !myRadii.isEmpty() || default_radius == 0)
    {
        // Either there are no points/curves, or we have explicit radii.
        sendRay<farthest,rm_backface,reverse>(origin, direction, hit_info, outer_tmin, outer_tmax);
        return;
    }
    typename SYS_SelectType<SingleHitFunctor<true>, SingleHitAndNormalFunctor<true>, SYS_IsSame<HitInfoType, HitInfoAndNormal>::value>::type functor(hit_info);
    functor.myTolerance = default_radius;
    sendRayGeneric<farthest,rm_backface,reverse>(origin, direction, functor, outer_tmin, outer_tmax);
}

template<uint NAXES,typename SUBCLASS>
template<bool rm_backface,bool reverse,typename HitInfoType>
void BVHBase<NAXES,SUBCLASS>::sendRayAll(
    const VectorType &origin,
    const VectorType &direction,
    UT_Array<HitInfoType> &hit_info,
    UT_Array<exint> *nesting_temp_array,
    float duplicate_tolerance,
    float outer_tmin,
    float outer_tmax) const noexcept
{
    typename SYS_SelectType<AllHitsFunctor<false>, AllHitsAndNormalsFunctor<false>, SYS_IsSame<HitInfoType, HitInfoAndNormal>::value>::type functor(hit_info, nesting_temp_array);
    sendRayGeneric<false,rm_backface,reverse>(origin, direction, functor, outer_tmin, outer_tmax);

    if (hit_info.size() <= 1)
        return;

    // Sort by t, breaking ties by index
    hit_info.stdsort([](const HitInfo &a, const HitInfo &b) -> bool {
        if (a.myT < b.myT)
            return true;
        if (a.myT > b.myT)
            return false;
        // FIXME: Check nesting array, if non-null!!!
        return (a.myItemIndex < b.myItemIndex);
    });

    if (duplicate_tolerance <= 0)
        return;

    float prev_t = hit_info[0].myT;
    exint desti = 1;
    for (exint i = 1, n = hit_info.size(); i < n; ++i)
    {
        float t = hit_info[i].myT;
        if (t-prev_t < duplicate_tolerance)
            continue;
        if (desti != i)
            hit_info[desti] = hit_info[i];
        ++desti;
    }
    hit_info.setSize(desti);
}

template<uint NAXES,typename SUBCLASS>
template<bool rm_backface,bool reverse,typename HitInfoType>
void BVHBase<NAXES,SUBCLASS>::sendRayAllRad(
    const VectorType &origin,
    const VectorType &direction,
    UT_Array<HitInfoType> &hit_info,
    float default_radius,
    UT_Array<exint> *nesting_temp_array,
    float duplicate_tolerance,
    float outer_tmin,
    float outer_tmax) const noexcept
{
    if (!myHasCurvesOrPoints || !myRadii.isEmpty() || default_radius == 0)
    {
        // Either there are no points/curves, or we have explicit radii.
        sendRayAll<rm_backface,reverse>(origin, direction, hit_info, nesting_temp_array, duplicate_tolerance, outer_tmin, outer_tmax);
        return;
    }
    typename SYS_SelectType<AllHitsFunctor<true>, AllHitsAndNormalsFunctor<true>, SYS_IsSame<HitInfoType, HitInfoAndNormal>::value>::type functor(hit_info, nesting_temp_array);
    functor.myTolerance = default_radius;
    sendRayGeneric<false,rm_backface,reverse>(origin, direction, functor, outer_tmin, outer_tmax);

    if (hit_info.size() <= 1)
        return;

    // Sort by t, breaking ties by index
    hit_info.stdsort([](const HitInfo &a, const HitInfo &b) -> bool {
        if (a.myT < b.myT)
            return true;
        if (a.myT > b.myT)
            return false;
        // FIXME: Check nesting array, if non-null!!!
        return (a.myItemIndex < b.myItemIndex);
    });

    if (duplicate_tolerance <= 0)
        return;

    float prev_t = hit_info[0].myT;
    exint desti = 1;
    for (exint i = 1, n = hit_info.size(); i < n; ++i)
    {
        float t = hit_info[i].myT;
        if (t-prev_t < duplicate_tolerance)
            continue;
        if (desti != i)
            hit_info[desti] = hit_info[i];
        ++desti;
    }
    hit_info.setSize(desti);
}

template<uint NAXES,typename SUBCLASS>
template<bool farthest,bool rm_backface,bool reverse,typename FUNCTOR>
void BVHBase<NAXES,SUBCLASS>::sendRayGeneric(VectorType origin, VectorType direction, FUNCTOR &hit_info, float outer_tmin, float outer_tmax) const noexcept
{
    using NodeType = typename UT_BVH<BVH_N>::Node;
    const NodeType *nodes = myTree.getNodes();
    const uint nnodes = myTree.getNumNodes();
    float length = direction.normalize();

    hit_info.noHit();
    if (nnodes == 0 || length == 0)
    {
        // Nothing to hit
        return;
    }

    const uint num_points = numPoints();

    UintVectorType sign;
    for (int i = 0; i < NAXES; ++i)
        sign[i] = (direction[i] < 0);

    VectorType inverse_direction;
    for (int i = 0; i < NAXES; ++i)
    {
        // This is deliberately different from SYSsafediv - since for plane
        // intersection, we need to generate a large result when dividing by 0.
        inverse_direction[i] = (direction[i] == 0) ? std::numeric_limits<float>::max() : 1/direction[i];
    }

    // For intersecting quads, we'll cache a coordinate frame.
    int max_dir = -1;
    VectorType N0;
    VectorType N1;

    struct StackEntry
    {
        uint myNode;
        float myTMin;

        SYS_FORCE_INLINE StackEntry() noexcept {}
        SYS_FORCE_INLINE StackEntry(uint node, float tmin) noexcept
            : myNode(node)
            , myTMin(tmin)
        {}
    };

#if GEO_BVH_INTERLEAVED
    UT_FixedVector<v4uf,NAXES> vorigin;
    for (uint axis = 0; axis < NAXES; ++axis)
    {
        vorigin[axis] = v4uf(origin[axis]);
    }
    UT_FixedVector<v4uf,NAXES> vinverse_direction;
    for (uint axis = 0; axis < NAXES; ++axis)
    {
        vinverse_direction[axis] = v4uf(inverse_direction[axis]);
    }
    v4uf vtolerance;
    if (hit_info.theUseTolerance)
        vtolerance = v4uf(hit_info.myTolerance);
#else
    const BoxType &box = myNodeBoxes[0];
    box.intersect(outer_tmin, outer_tmax, sign, origin, inverse_direction);

    if (outer_tmin > outer_tmax)
    {
        // No hits
        hit_info.noHit();
        return;
    }
#endif

    //UT_Vector3 hit_uvw;
    //exint hit_index = -1;

    UT_SmallArray<StackEntry,256> stack;
#if GEO_BVH_INTERLEAVED
    stack.append(StackEntry(NodeType::markInternal(0),(!farthest) ? outer_tmin : outer_tmax));
#else
    stack.append(StackEntry(0,outer_tmin));
#endif

    do
    {
        StackEntry entry = stack.last();
        stack.removeLast();

        uint next_node = entry.myNode;
        // When farthest is true, entry.myTMin is actually a tmax.
        if ((!farthest) ? (entry.myTMin >= outer_tmax) : (entry.myTMin <= outer_tmin))
            continue;

        while (true)
        {
#if GEO_BVH_INTERLEAVED
            if (NodeType::isInternal(next_node))
            {
                UT_ASSERT_MSG_P(next_node != NodeType::EMPTY, "How did a ray hit an empty box?");

                next_node = NodeType::getInternalNum(next_node);
                const BoxType &box = myNodeBoxes[next_node];
                v4uf tmin(outer_tmin); v4uf tmax(outer_tmax);
                if (!hit_info.theUseTolerance)
                    box.intersect(tmin, tmax, sign, vorigin, vinverse_direction);
                else
                    box.intersectTol(tmin, tmax, sign, vorigin, vinverse_direction, vtolerance);
#if 0
                {
                    uint axis_sign = sign[0];
                    v4uf t1 = (box.vals[0][axis_sign]   - vorigin[0]) * vinverse_direction[0];
                    tmin = SYSmax(t1, tmin);
                    v4uf t2 = (box.vals[0][axis_sign^1] - vorigin[0]) * vinverse_direction[0];
                    tmax = SYSmin(t2, tmax);
                }
                {
                    uint axis_sign = sign[1];
                    v4uf t1 = (box.vals[1][axis_sign]   - vorigin[1]) * vinverse_direction[1];
                    tmin = SYSmax(t1, tmin);
                    v4uf t2 = (box.vals[1][axis_sign^1] - vorigin[1]) * vinverse_direction[1];
                    tmax = SYSmin(t2, tmax);
                }
                {
                    uint axis_sign = sign[2];
                    v4uf t1 = (box.vals[2][axis_sign]   - vorigin[2]) * vinverse_direction[2];
                    tmin = SYSmax(t1, tmin);
                    v4uf t2 = (box.vals[2][axis_sign^1] - vorigin[2]) * vinverse_direction[2];
                    tmax = SYSmin(t2, tmax);
                }
#endif
                // NOTE: DO NOT change this to <, else axis-aligned polygons could be missed.
                v4uu hit_mask = (tmin <= tmax);
                const uint hit_bits = _mm_movemask_ps(V4SF(hit_mask.vector));
                if (hit_bits == 0)
                    break;
                const NodeType &node = nodes[next_node];
                if (!(hit_bits & (hit_bits-1)))
                {
                    // Only 1 bit set
                    uint local_index = SYSfirstBitSet(hit_bits);
#if 1
                    next_node = node.child[local_index];
#else
                    float local_tmin = tmins[local_index];
                    uint child_index = node.child[local_index];
                    if (!stack_size || ((!farthest) ? (stack_last->myTMin >= local_tmin) : (stack_last->myTMin <= local_tmin)))
                        next_node = child_index;
                    else
                    {
                        next_node = stack_last->myNode;
                        stack_last->myNode = child_index;
                        stack_last->myTMin = local_tmin;
                    }
#endif
                    continue;
                }

                exint stack_size = stack.size();
                StackEntry *stack_last = stack.data()+stack_size-1;

                // If we're going to be adding to the stack, we might as
                // well prune anything from the end of the stack that's
                // too far, to possibly reduce reallocations.
                // That said, this probably won't happen often, since
                // it requires hitting 2 child boxes in the current node
                // when the next node in the stack must be farther than an
                // existing hit.
                // When farthest is true, stack_last->myTMin is actually a tmax.
                while (stack_size != 0 && ((!farthest) ? (stack_last->myTMin >= outer_tmax) : (stack_last->myTMin <= outer_tmin)))
                {
                    --stack_size;
                    --stack_last;
                    stack.removeLast();
                }
                static constexpr uint two_bit_indices[16][2] = {
                    {4, 4}, {4, 4}, {4, 4}, {0, 1},
                    {4, 4}, {0, 2}, {1, 2}, {0, 1},
                    {4, 4}, {0, 3}, {1, 3}, {0, 1},
                    {2, 3}, {0, 2}, {1, 2}, {0, 1}
                };
                static constexpr uint third_bit_index[16] = {
                    4, 4, 4, 4,
                    4, 4, 4, 2,
                    4, 4, 4, 3,
                    4, 3, 3, 2
                };
                // When farthest is true, these are tmax's.
                union { v4sf tminv; float tmins[4]; };
                tminv = (!farthest) ? tmin.vector : tmax.vector;
                uint local_index0 = two_bit_indices[hit_bits][0];
                uint local_index1 = two_bit_indices[hit_bits][1];
                if (third_bit_index[hit_bits] == 4)
                {
                    // Only 2 bits set
                    float t0 = tmins[local_index0];
                    float t1 = tmins[local_index1];
                    uint child0 = node.child[local_index0];
                    uint child1 = node.child[local_index1];
                    if ((!farthest) ? (t0 > t1) : (t0 < t1))
                    {
                        uint childtmp = child0;
                        child0 = child1;
                        child1 = childtmp;
                        float tmp = t0;
                        t0 = t1;
                        t1 = tmp;
                    }

                    // When farthest is true, stack_last->myTMin is actually a tmax.
                    if (!stack_size || ((!farthest) ? (stack_last->myTMin >= t0) : (stack_last->myTMin <= t0)))
                    {
                        next_node = child0;

                        // Inserting t1
                        // Check end of stack first, since it's most likely that the box will go near the end.
                        if (!stack_size || ((!farthest) ? (stack_last->myTMin >= t1) : (stack_last->myTMin <= t1)))
                        {
                            stack.append(StackEntry(child1, t1));
                        }
                        else
                        {
                            // Insert in sorted order, but go back at most a constant
                            // number of entries, to avoid n^2 behaviour.
                            exint i;
                            exint limiti = SYSmax(0, stack_size-64);
                            for (i = stack_size-2; i >= limiti; --i)
                            {
                                if ((!farthest) ? (stack[i].myTMin >= t1) : (stack[i].myTMin <= t1))
                                {
                                    stack.insert(StackEntry(child1, t1), i+1);
                                    break;
                                }
                            }
                            if (i < limiti)
                            {
                                stack.insert(StackEntry(child1, t1), i+1);
                            }
                        }
                    }
                    else
                    {
                        next_node = stack_last->myNode;
                        stack_last->myNode = child1;
                        stack_last->myTMin = t1;
                        stack.append(StackEntry(child0, t0));
                    }
                    continue;
                }

                // 3-4 bits set
                uint nhit = (hit_bits == 0xF) ? 4 : 3;
                uint local_index2 = third_bit_index[hit_bits];
                uint children[BVH_N] = {
                    node.child[local_index0],
                    node.child[local_index1],
                    node.child[local_index2],
                    node.child[3]
                };
                tmins[0] = tmins[local_index0];
                tmins[1] = tmins[local_index1];
                tmins[2] = tmins[local_index2];
                //tmins[3] = tmins[3];

                float new_tmin;
                if (!stack_size || ((!farthest) ? (stack_last->myTMin >= tmins[0]) : (stack_last->myTMin <= tmins[0])))
                {
                    new_tmin = tmins[0];
                    next_node = children[0];
                }
                else
                {
                    new_tmin = stack_last->myTMin;
                    next_node = stack_last->myNode;
                    stack_last->myTMin = tmins[0];
                    stack_last->myNode = children[0];
                }
                for (uint hit = 1; hit < nhit; ++hit)
                {
                    float t = tmins[hit];
                    uint child = children[hit];
                    if ((!farthest) ? (t < new_tmin) : (t > new_tmin))
                    {
                        float tmpt = new_tmin;
                        uint tmpchild = next_node;
                        new_tmin = t;
                        next_node = child;
                        t = tmpt;
                        child = tmpchild;
                    }

                    // Check end of stack first, since it's most likely that the box will go near the end.
                    if (!stack_size || ((!farthest) ? (stack_last->myTMin >= t) : (stack_last->myTMin <= t)))
                    {
                        stack.append(StackEntry(child, t));
                    }
                    else
                    {
                        // Insert in sorted order, but go back at most a constant
                        // number of entries, to avoid n^2 behaviour.
                        exint i;
                        exint limiti = SYSmax(0, stack_size-64);
                        for (i = stack_size-2; i >= limiti; --i)
                        {
                            if ((!farthest) ? (stack[i].myTMin >= t) : (stack[i].myTMin <= t))
                            {
                                stack.insert(StackEntry(child, t), i+1);
                                break;
                            }
                        }
                        if (i < limiti)
                        {
                            stack.insert(StackEntry(child, t), i+1);
                        }
                    }
                    stack_last = stack.data()+stack_size;
                    ++stack_size;
                }
                continue;
            }
            uint index = next_node;//myIndices[next_node];
#else
            const NodeType &node = nodes[next_node];
            uint children[BVH_N];
            float tmins[BVH_N];
            uint nnodehits = 0;
            for (uint i = 0; i < BVH_N; ++i)
            {
                const uint itemi = node.child[i];
                if (NodeType::isInternal(itemi))
                {
                    if (itemi == NodeType::EMPTY)
                        continue;

                    // Internal node: intersect against box
                    const uint childi = NodeType::getInternalNum(itemi);
                    const BoxType &box = myNodeBoxes[childi];
                    float tmin = outer_tmin; float tmax = outer_tmax;
                    box.intersect(tmin, tmax, sign, origin, inverse_direction);
                    // NOTE: DO NOT change this to >=, else axis-aligned polygons could be missed.
                    if (tmin > tmax)
                        continue;

                    children[nnodehits] = childi;
                    tmins[nnodehits] = tmin;
                    ++nnodehits;
                }
                else
                {
                    // Leaf item: intersect against item
                    uint index = itemi;//myIndices[itemi];
#endif
                    if (!SUBCLASS::theHasPrimitives || index < num_points)
                    {
                        // Points as spheres
                        //if (myRadii.isValid())
                        if (!myRadii.isEmpty() || hit_info.theUseTolerance)
                        {
                            GA_Offset ptoff = myPoints(index);
                            const VectorType pos = myPositions(ptoff);
                            float radius = hit_info.theUseTolerance ?
                                hit_info.myTolerance :
                                ((myRadii.size() == 1) ? myRadii[0] : myRadii[ptoff]);
                            // We already checked for zero radius if theUseTolerance is true
                            if (hit_info.theUseTolerance || radius != 0)
                            {
                                float t0; float t1;
                                const VectorType rel_origin = origin - pos;
                                bool ishit = geoIntersectSphere(rel_origin, direction, radius, t0, t1);
                                if (!ishit)
#if GEO_BVH_INTERLEAVED
                                    break;
#else
                                    continue;
#endif

                                const float invradius = 1.0f/radius;
                                if ((t0 <= outer_tmax) && (t0 >= outer_tmin))
                                {
                                    VectorType p = rel_origin + t0*direction;
                                    p *= invradius;
                                    // NOTE: p is the normal
                                    if (!rm_backface || (dot(p,direction) <= 0) != reverse)
                                    {
                                        hit_info.insertHit(
                                            UT_Vector3(p[0], p[1], (NAXES==3) ? p[2] : 0),
                                            t0, index,
                                            (!farthest) ? outer_tmax : outer_tmin);
                                        if (hit_info.theNeedsNormal)
                                            hit_info.setNormal(p);
                                    }
                                }

                                if ((t1 <= outer_tmax) && (t1 >= outer_tmin))
                                {
                                    VectorType p = rel_origin + t1*direction;
                                    p *= invradius;
                                    // NOTE: p is the normal
                                    if (!rm_backface || (dot(p,direction) <= 0) != reverse)
                                    {
                                        hit_info.insertHit(
                                            UT_Vector3(p[0], p[1], (NAXES==3) ? p[2] : 0),
                                            t1, index,
                                            (!farthest) ? outer_tmax : outer_tmin);
                                        if (hit_info.theNeedsNormal)
                                            hit_info.setNormal(p);
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
#if !GEO_BVH_INTERLEAVED
                        bool ishit =
#endif
                            subclass()->template intersectPrim<farthest,rm_backface,reverse>(
                                index, origin, direction, inverse_direction, max_dir, N0, N1,
                                outer_tmax, outer_tmin, hit_info);
#if !GEO_BVH_INTERLEAVED
                        if (!ishit)
                            continue;
#endif
                    }
#if GEO_BVH_INTERLEAVED
                    break;
#endif
#if !GEO_BVH_INTERLEAVED
                }
            }
            if (nnodehits == 0)
                break;
            exint stack_size = stack.size();
            StackEntry *stack_last = stack.data()+stack_size-1;
            if (nnodehits == 1)
            {
                uint child_index = children[0];
                if (!stack_size || stack_last->myTMin >= tmins[0])
                    next_node = child_index;
                else
                {
                    next_node = stack_last->myNode;
                    stack_last->myNode = child_index;
                    stack_last->myTMin = tmins[0];
                }
            }
            else if (BVH_N == 2 || nnodehits == 2)
            {
                const uint local_index = (tmins[0] >= tmins[1]);
                next_node = children[local_index];
                const uint insert_node = children[!local_index];
                const float insert_tmin = tmins[!local_index];
                // Check end of stack first, since it's most likely that the box will go near the end.
                if (!stack_size || stack_last->myTMin <= insert_tmin)
                {
                    stack.append(StackEntry(insert_node, insert_tmin));
                }
                else
                {
                    // Insert in sorted order, but go back at most a constant
                    // number of entries, to avoid n^2 behaviour.
                    exint i;
                    exint limiti = SYSmax(0, stack_size-64);
                    for (i = stack_size-2; i >= limiti; --i)
                    {
                        if (stack[i].myTMin <= insert_tmin)
                        {
                            stack.insert(StackEntry(insert_node, insert_tmin), i+1);
                            break;
                        }
                    }
                    if (i < limiti)
                    {
                        stack.insert(StackEntry(insert_node, insert_tmin), i+1);
                    }
                }
            }
            else
            {
                // Sort/insert and keep closest
                SYS_STATIC_ASSERT_MSG(BVH_N==2, "FIXME: Implement case of BVH_N>2!!!");

            }
#endif
        }
    } while (!stack.isEmpty());

#if 0
    if (hit_index >= 0)
    {
        hit_info.myItemIndex = hit_index;
        hit_info.myUVW = hit_uvw;
        hit_info.myT = (!farthest) ? outer_tmax : outer_tmin;
    }
    else
    {
        hit_info.myItemIndex = -1;
        hit_info.myUVW.assign(0,0,0);
        hit_info.myT = -1.0f;
    }
#endif
}

template<uint NAXES,typename SUBCLASS>
template<bool farthest>
void BVHBase<NAXES,SUBCLASS>::findClosest(VectorType origin, MinInfo &hit_info, float max_dist_squared) const noexcept
{
    using NodeType = typename UT_BVH<BVH_N>::Node;
    const NodeType *nodes = myTree.getNodes();
    const uint nnodes = myTree.getNumNodes();

    if (nnodes == 0)
    {
        // Nothing to hit
        hit_info.myItemIndex = -1;
        hit_info.myUVW.assign(0,0,0);
        hit_info.myDistSquared = -1.0f;
        hit_info.myP = VectorType(typename VectorType::value_type(0));
        return;
    }

    UT_Array<exint> *nesting_array = hit_info.myNestedItemIndices;
    exint nesting_array_base = nesting_array ? nesting_array->size() : 0;

    const uint num_points = numPoints();

    struct StackEntry
    {
        uint myNode;
        float myDistSquared;

        SYS_FORCE_INLINE StackEntry() noexcept {}
        SYS_FORCE_INLINE StackEntry(uint node, float d2) noexcept
            : myNode(node)
            , myDistSquared(d2)
        {}
    };

#if GEO_BVH_INTERLEAVED
    UT_FixedVector<v4uf,NAXES> vorigin;
    for (uint axis = 0; axis < NAXES; ++axis)
    {
        vorigin[axis] = v4uf(origin[axis]);
    }
#else
    const BoxType &box = myNodeBoxes[0];
    float dist2 = (!farthest) ? box.minDistance2(origin) : box.maxDistance2(origin);

    // NOTE: When farthest is true, max_dist_squared is actually a minimum distance squared.
    if ((!farthest) ? (dist2 > max_dist_squared) : (dist2 < max_dist_squared))
    {
        // No hits
        hit_info.myItemIndex = -1;
        hit_info.myUVW.assign(0,0,0);
        hit_info.myDistSquared = -1.0f;
        hit_info.myP = VectorType(typename VectorType::value_type(0));
        return;
    }
#endif

    UT_Vector3 hit_uvw;
    exint hit_index = -1;
    VectorType hit_position;

    // Be careful, because this is a fairly big buffer on the stack,
    // and there's the potential to recurse into packed primitives.
    // If we have deeply nested packed primitives, that could be an issue.
    UT_SmallArray<StackEntry,1024> stack;
#if GEO_BVH_INTERLEAVED
    stack.append(StackEntry(NodeType::markInternal(0),(!farthest) ? 0 : std::numeric_limits<float>::max()));
#else
    stack.append(StackEntry(0,dist2));
#endif

    do
    {
        StackEntry entry = stack.last();
        stack.removeLast();

        uint next_node = entry.myNode;
        // When farthest is true, max_dist_squared is actually a minimum distance squared.
        if ((!farthest) ? (entry.myDistSquared > max_dist_squared) : (entry.myDistSquared < max_dist_squared))
            continue;

        while (true)
        {
#if GEO_BVH_INTERLEAVED
            if (NodeType::isInternal(next_node))
            {
                UT_ASSERT_MSG_P(next_node != NodeType::EMPTY, "How did a ray hit an empty box?");

                next_node = NodeType::getInternalNum(next_node);
                const BoxType &box = myNodeBoxes[next_node];
                v4uf dist2 = (!farthest) ? box.minDistance2(vorigin) : box.maxDistance2(vorigin);
                v4uu hit_mask = (!farthest) ? (dist2 <= max_dist_squared) : (dist2 >= max_dist_squared);
                const uint hit_bits = _mm_movemask_ps(V4SF(hit_mask.vector));
                if (hit_bits == 0)
                    break;
                const NodeType &node = nodes[next_node];
                if (!(hit_bits & (hit_bits-1)))
                {
                    // Only 1 bit set
                    uint local_index = SYSfirstBitSet(hit_bits);
#if 1
                    next_node = node.child[local_index];
#else
                    float local_d2 = d2s[local_index];
                    uint child_index = node.child[local_index];
                    if (!stack_size || ((!farthest) ? (stack_last->myDistSquared >= local_d2) : (stack_last->myDistSquared <= local_d2)))
                        next_node = child_index;
                    else
                    {
                        next_node = stack_last->myNode;
                        stack_last->myNode = child_index;
                        stack_last->myDistSquared = local_d2;
                    }
#endif
                    continue;
                }

                exint stack_size = stack.size();
                StackEntry *stack_last = stack.data()+stack_size-1;

                // If we're going to be adding to the stack, we might as
                // well prune anything from the end of the stack that's
                // too far, to possibly reduce reallocations.
                // That said, this probably won't happen often, since
                // it requires hitting 2 child boxes in the current node
                // when the next node in the stack must be farther than an
                // existing hit.
                // When farthest is true, max_dist_squared is actually a minimum distance squared.
                while (stack_size != 0 && ((!farthest) ? (stack_last->myDistSquared > max_dist_squared) : (stack_last->myDistSquared < max_dist_squared)))
                {
                    --stack_size;
                    --stack_last;
                    stack.removeLast();
                }
                static constexpr uint two_bit_indices[16][2] = {
                    {4, 4}, {4, 4}, {4, 4}, {0, 1},
                    {4, 4}, {0, 2}, {1, 2}, {0, 1},
                    {4, 4}, {0, 3}, {1, 3}, {0, 1},
                    {2, 3}, {0, 2}, {1, 2}, {0, 1}
                };
                static constexpr uint third_bit_index[16] = {
                    4, 4, 4, 4,
                    4, 4, 4, 2,
                    4, 4, 4, 3,
                    4, 3, 3, 2
                };
                // When farthest is true, these are tmax's.
                union { v4sf d2v; float d2s[4]; };
                d2v = dist2.vector;
                uint local_index0 = two_bit_indices[hit_bits][0];
                uint local_index1 = two_bit_indices[hit_bits][1];
                if (third_bit_index[hit_bits] == 4)
                {
                    // Only 2 bits set
                    float d20 = d2s[local_index0];
                    float d21 = d2s[local_index1];
                    uint child0 = node.child[local_index0];
                    uint child1 = node.child[local_index1];
                    if ((!farthest) ? (d20 > d21) : (d20 < d21))
                    {
                        uint childtmp = child0;
                        child0 = child1;
                        child1 = childtmp;
                        float tmp = d20;
                        d20 = d21;
                        d21 = tmp;
                    }

                    if (!stack_size || ((!farthest) ? (stack_last->myDistSquared >= d20) : (stack_last->myDistSquared <= d20)))
                    {
                        next_node = child0;

                        // Inserting d21
                        // Check end of stack first, since it's most likely that the box will go near the end.
                        if (!stack_size || ((!farthest) ? (stack_last->myDistSquared >= d21) : (stack_last->myDistSquared <= d21)))
                        {
                            stack.append(StackEntry(child1, d21));
                        }
                        else
                        {
                            // Insert in sorted order, but go back at most a constant
                            // number of entries, to avoid n^2 behaviour.
                            exint i;
                            exint limiti = SYSmax(0, stack_size-64);
                            for (i = stack_size-2; i >= limiti; --i)
                            {
                                if ((!farthest) ? (stack[i].myDistSquared >= d21) : (stack[i].myDistSquared <= d21))
                                {
                                    stack.insert(StackEntry(child1, d21), i+1);
                                    break;
                                }
                            }
                            if (i < limiti)
                            {
                                stack.insert(StackEntry(child1, d21), i+1);
                            }
                        }
                    }
                    else
                    {
                        next_node = stack_last->myNode;
                        stack_last->myNode = child1;
                        stack_last->myDistSquared = d21;
                        stack.append(StackEntry(child0, d20));
                    }
                    continue;
                }

                // 3-4 bits set
                uint nhit = (hit_bits == 0xF) ? 4 : 3;
                uint local_index2 = third_bit_index[hit_bits];
                uint children[BVH_N] = {
                    node.child[local_index0],
                    node.child[local_index1],
                    node.child[local_index2],
                    node.child[3]
                };
                d2s[0] = d2s[local_index0];
                d2s[1] = d2s[local_index1];
                d2s[2] = d2s[local_index2];
                //d2s[3] = d2s[3];

                float new_d2;
                if (!stack_size || ((!farthest) ? (stack_last->myDistSquared >= d2s[0]) : (stack_last->myDistSquared <= d2s[0])))
                {
                    new_d2 = d2s[0];
                    next_node = children[0];
                }
                else
                {
                    new_d2 = stack_last->myDistSquared;
                    next_node = stack_last->myNode;
                    stack_last->myDistSquared = d2s[0];
                    stack_last->myNode = children[0];
                }
                for (uint hit = 1; hit < nhit; ++hit)
                {
                    float d2 = d2s[hit];
                    uint child = children[hit];
                    if ((!farthest) ? (d2 < new_d2) : (d2 > new_d2))
                    {
                        float tmpd2 = new_d2;
                        uint tmpchild = next_node;
                        new_d2 = d2;
                        next_node = child;
                        d2 = tmpd2;
                        child = tmpchild;
                    }

                    // Check end of stack first, since it's most likely that the box will go near the end.
                    if (!stack_size || ((!farthest) ? (stack_last->myDistSquared >= d2) : (stack_last->myDistSquared <= d2)))
                    {
                        stack.append(StackEntry(child, d2));
                    }
                    else
                    {
                        // Insert in sorted order, but go back at most a constant
                        // number of entries, to avoid n^2 behaviour.
                        exint i;
                        exint limiti = SYSmax(0, stack_size-64);
                        for (i = stack_size-2; i >= limiti; --i)
                        {
                            if ((!farthest) ? (stack[i].myDistSquared >= d2) : (stack[i].myDistSquared <= d2))
                            {
                                stack.insert(StackEntry(child, d2), i+1);
                                break;
                            }
                        }
                        if (i < limiti)
                        {
                            stack.insert(StackEntry(child, d2), i+1);
                        }
                    }
                    stack_last = stack.data()+stack_size;
                    ++stack_size;
                }
                continue;
            }
            uint index = next_node;//myIndices[next_node];
#else
            const NodeType &node = nodes[next_node];
            uint children[BVH_N];
            float d2s[BVH_N];
            uint nnodehits = 0;
            for (uint i = 0; i < BVH_N; ++i)
            {
                const uint itemi = node.child[i];
                if (NodeType::isInternal(itemi))
                {
                    if (itemi == NodeType::EMPTY)
                        continue;

                    // Internal node: intersect against box
                    const uint childi = NodeType::getInternalNum(itemi);
                    const BoxType &box = myNodeBoxes[childi];
                    float dist2 = (!farthest) ? box.minDistance2(origin) : box.maxDistance2(origin);
                    if ((!farthest) ? (dist2 > min_dist_squared) : (dist2 < min_dist_squared))
                        continue;

                    children[nnodehits] = childi;
                    d2s[nnodehits] = dist2;
                    ++nnodehits;
                }
                else
                {
                    // Leaf item: intersect against item
                    uint index = itemi;//myIndices[itemi];
#endif
                    if (!SUBCLASS::theHasPrimitives || index < num_points)
                    {
                        // Points as spheres
                        GA_Offset ptoff = myPoints(index);
                        const VectorType pos = myPositions(ptoff);
                        float radius = 0;
                        //if (myRadii.isValid())
                        if (!myRadii.isEmpty())
                            radius = (myRadii.size() == 1) ? myRadii[0] : myRadii[ptoff];
                        VectorType displacement = origin-pos;
                        float d2 = displacement.length2();
                        if (radius == 0)
                        {
                            // Zero radius
                            // When farthest is true, max_dist_squared is actually a minimum distance squared.
                            if ((!farthest) ? (d2 < max_dist_squared) : (d2 > max_dist_squared))
                            {
                                max_dist_squared = d2;
                                hit_index = index;
                                hit_uvw.assign(0,0,0);
                                hit_position = pos;
                            }
                        }
                        else
                        {
                            float distance = SYSsqrt(d2) - SYSabs(radius);
                            d2 = distance*distance;
                            if ((!farthest) ? (d2 < max_dist_squared) : (d2 > max_dist_squared))
                            {
                                max_dist_squared = d2;
                                hit_index = index;
                                VectorType normal = displacement;
                                normal.normalize();
                                // Real normal is opposite the displacement if radius is negative.
                                VectorType real_normal = (((radius > 0) != farthest) ? normal : -normal);
                                hit_uvw[0] = real_normal[0];
                                hit_uvw[1] = real_normal[1];
                                hit_uvw[2] = (NAXES==3) ? real_normal[2] : 0;

                                // hit_position needs to use normal in the direction of displacement.
                                hit_position = pos;
                                if (!farthest)
                                    hit_position -= radius*normal;
                                else
                                    hit_position += radius*normal;
                            }
                        }
                    }
                    else
                    {
                        subclass()->template closestPrim<farthest>(
                            index, origin, max_dist_squared,
                            hit_index, hit_uvw, hit_position,
                            vorigin, nesting_array, nesting_array_base);
                    }
#if GEO_BVH_INTERLEAVED
                    break;
#else
                }
            }
            if (nnodehits == 0)
                break;
            exint stack_size = stack.size();
            StackEntry *stack_last = stack.data()+stack_size-1;
            if (nnodehits == 1)
            {
                uint child_index = children[0];
                if (!stack_size || stack_last->myTMin >= tmins[0])
                    next_node = child_index;
                else
                {
                    next_node = stack_last->myNode;
                    stack_last->myNode = child_index;
                    stack_last->myTMin = tmins[0];
                }
            }
            else if (BVH_N == 2 || nnodehits == 2)
            {
                const uint local_index = (tmins[0] >= tmins[1]);
                next_node = children[local_index];
                const uint insert_node = children[!local_index];
                const float insert_tmin = tmins[!local_index];
                // Check end of stack first, since it's most likely that the box will go near the end.
                if (!stack_size || stack_last->myTMin <= insert_tmin)
                {
                    stack.append(StackEntry(insert_node, insert_tmin));
                }
                else
                {
                    // Insert in sorted order, but go back at most a constant
                    // number of entries, to avoid n^2 behaviour.
                    exint i;
                    exint limiti = SYSmax(0, stack_size-64);
                    for (i = stack_size-2; i >= limiti; --i)
                    {
                        if (stack[i].myTMin <= insert_tmin)
                        {
                            stack.insert(StackEntry(insert_node, insert_tmin), i+1);
                            break;
                        }
                    }
                    if (i < limiti)
                    {
                        stack.insert(StackEntry(insert_node, insert_tmin), i+1);
                    }
                }
            }
            else
            {
                // Sort/insert and keep closest
                SYS_STATIC_ASSERT_MSG(BVH_N==2, "FIXME: Implement case of BVH_N>2!!!");

            }
#endif
        }
    } while (!stack.isEmpty());

    if (hit_index >= 0)
    {
        hit_info.myItemIndex = hit_index;
        hit_info.myUVW = hit_uvw;
        // When farthest is true, max_dist_squared is actually a minimum distance squared.
        hit_info.myDistSquared = max_dist_squared;
        hit_info.myP = hit_position;
    }
    else
    {
        hit_info.myItemIndex = -1;
        hit_info.myUVW.assign(0,0,0);
        hit_info.myDistSquared = -1.0f;
        hit_info.myP = VectorType(typename VectorType::value_type(0));
    }
}

template<uint NAXES,typename SUBCLASS>
void BVHBase<NAXES,SUBCLASS>::getIntersectingBoxes(const SingleBoxType &query_box, UT_Array<exint> &box_indices) const noexcept
{
    UT_SmallArray<uint,256> stack;
    UT::getIntersectingBoxes(myTree, myNodeBoxes.get(), query_box, box_indices, stack);
}

template<uint NAXES,typename SUBCLASS>
template<bool normalize>
typename BVHBase<NAXES,SUBCLASS>::VectorType BVHBase<NAXES,SUBCLASS>::getGeometricNormal(const CommonHitInfo &hit_info) const noexcept
{
    if (!SUBCLASS::theHasPrimitives || hit_info.myItemIndex < numPoints())
    {
        // NOTE: We don't need to negate if the radius is negative, because
        //       that was automatically done in sendRay by multiplying by invradius.
        return VectorType(hit_info.myUVW);
    }
    return subclass()->template primGeometricNormal<normalize>(hit_info);
}

template<uint NAXES,typename SUBCLASS>
void BVHBase<NAXES,SUBCLASS>::getDerivs(const CommonHitInfo &hit_info, VectorType &dP_du, VectorType &dP_dv) const noexcept
{
    if (!SUBCLASS::theHasPrimitives || hit_info.myItemIndex < numPoints())
    {
        // NOTE: We don't need to negate nml if the radius is negative, because
        //       that was automatically done in sendRay by multiplying by invradius.
        // TODO: However, do we need to negate dP_du or dP_dv?
        VectorType nml;
        nml[0] = hit_info.myUVW[0];
        nml[1] = hit_info.myUVW[1];
        if (NAXES == 3)
        {
            nml[2] = hit_info.myUVW[1];
            UT_Vector2 nmlxy(nml[0], nml[1]);
            float lengthxy = nmlxy.normalize();

            dP_dv[0] = -nml[2]*nmlxy[0];
            dP_dv[1] = -nml[2]*nmlxy[1];
            dP_dv[2] = lengthxy;
            dP_dv *= M_PI;

            dP_du[0] = -nml[1]*(2*M_PI);
            dP_du[1] =  nml[0]*(2*M_PI);
            dP_du[2] = 0;
        }
        else
        {
            UT_ASSERT(NAXES==2);
            // FIXME: Verify that the sign is correct on this!
            dP_du[0] = -nml[1]*(2*M_PI);
            dP_du[1] =  nml[0]*(2*M_PI);
            dP_dv[0] = 0;
            dP_dv[1] = 0;
        }
        return;
    }
    subclass()->primDerivs(hit_info, dP_du, dP_dv);
}

template<uint NAXES,typename SUBCLASS>
void BVHBase<NAXES,SUBCLASS>::pointUVWToPolar(VectorType &uvw) noexcept
{
    float u = SYSatan(uvw[1], uvw[0]) / (float)(2*M_PI);
    if (u < 0)
        u += 1;
    if (NAXES == 3)
    {
        float v = SYSacos(uvw[2]) / (float)M_PI;
        uvw[0] = u;
        uvw[1] = v;
        uvw[2] = 0;
    }
    else
    {
        UT_ASSERT(NAXES==2);
        uvw[0] = u;
        uvw[1] = 0;
    }
}

template<uint NAXES,typename SUBCLASS>
template<GA_AttributeOwner owner,typename T,typename DEST_T>
bool BVHBase<NAXES,SUBCLASS>::getAttribute(const CommonHitInfo &hit_info, const GA_ROHandleT<T> &attrib, const GEO_Detail &detail, DEST_T &value) const noexcept
{
    UT_ASSERT(owner == attrib->getOwner());

    if (owner == GA_ATTRIB_DETAIL)
    {
        value = attrib.get(GA_DETAIL_OFFSET);
        return true;
    }

    exint index = hit_info.myItemIndex;
    if (!SUBCLASS::theHasPrimitives || index < numPoints())
    {
        if (owner != GA_ATTRIB_POINT)
            return false;

        GA_Offset ptoff = myPoints(index);
        value = attrib.get(ptoff);
        return true;
    }
    return subclass()->template primAttribute<owner,T,DEST_T>(hit_info, attrib, detail, value);
}

} // GEO namespace

#undef GEO_BVH_INTERLEAVED
