/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        UT_BVHImpl.h (UT Library, C++)
 *
 * COMMENTS:    Bounding Volume Hierarchy (BVH) implementation.
 *              The main file is UT_BVH.h; this file is separate so that
 *              files that don't actually need to call functions on the BVH
 *              won't have unnecessary headers and functions included.
 */

#pragma once

#ifndef __UT_BVHImpl_h__
#define __UT_BVHImpl_h__

#include "UT_BVH.h"
#include "UT_Array.h"
#include "UT_Assert.h"
#include "UT_FixedVector.h"
#include "UT_ParallelUtil.h"
#include "UT_SmallArray.h"
#include "UT_Thread.h"
#include <SYS/SYS_BitUtil.h>
#include <algorithm>

#define BVH_TRY_ALL_AXES 0

namespace UT {

template<typename T,uint NAXES>
SYS_FORCE_INLINE bool utBoxExclude(const UT::Box<T,NAXES>& box) noexcept {
    bool has_nan_or_inf = !SYSisFinite(box[0][0]);
    has_nan_or_inf |= !SYSisFinite(box[0][1]);
    for (uint axis = 1; axis < NAXES; ++axis)
    {
        has_nan_or_inf |= !SYSisFinite(box[axis][0]);
        has_nan_or_inf |= !SYSisFinite(box[axis][1]);
    }
    return has_nan_or_inf;
}
template<uint NAXES>
SYS_FORCE_INLINE bool utBoxExclude(const UT::Box<fpreal32,NAXES>& box) noexcept {
    const int32 *pboxints = reinterpret_cast<const int32*>(&box);
    // Fast check for NaN or infinity: check if exponent bits are 0xFF.
    bool has_nan_or_inf = ((pboxints[0] & 0x7F800000) == 0x7F800000);
    has_nan_or_inf |= ((pboxints[1] & 0x7F800000) == 0x7F800000);
    for (uint axis = 1; axis < NAXES; ++axis)
    {
        has_nan_or_inf |= ((pboxints[2*axis] & 0x7F800000) == 0x7F800000);
        has_nan_or_inf |= ((pboxints[2*axis + 1] & 0x7F800000) == 0x7F800000);
    }
    return has_nan_or_inf;
}
template<typename T,uint NAXES>
SYS_FORCE_INLINE T utBoxCenter(const UT::Box<T,NAXES>& box, uint axis) noexcept {
    const T* v = box.vals[axis];
    return v[0] + v[1];
}
template<typename T>
struct ut_BoxCentre {
    constexpr static uint scale = 2;
};
template<typename T,uint NAXES,bool INSTANTIATED>
SYS_FORCE_INLINE T utBoxExclude(const UT_FixedVector<T,NAXES,INSTANTIATED>& position) noexcept {
    bool has_nan_or_inf = !SYSisFinite(position[0]);
    for (uint axis = 1; axis < NAXES; ++axis)
        has_nan_or_inf |= !SYSisFinite(position[axis]);
    return has_nan_or_inf;
}
template<uint NAXES,bool INSTANTIATED>
SYS_FORCE_INLINE bool utBoxExclude(const UT_FixedVector<fpreal32,NAXES,INSTANTIATED>& position) noexcept {
    const int32 *ppositionints = reinterpret_cast<const int32*>(&position);
    // Fast check for NaN or infinity: check if exponent bits are 0xFF.
    bool has_nan_or_inf = ((ppositionints[0] & 0x7F800000) == 0x7F800000);
    for (uint axis = 1; axis < NAXES; ++axis)
        has_nan_or_inf |= ((ppositionints[axis] & 0x7F800000) == 0x7F800000);
    return has_nan_or_inf;
}
template<typename T,uint NAXES,bool INSTANTIATED>
SYS_FORCE_INLINE T utBoxCenter(const UT_FixedVector<T,NAXES,INSTANTIATED>& position, uint axis) noexcept {
    return position[axis];
}
template<typename T,uint NAXES,bool INSTANTIATED>
struct ut_BoxCentre<UT_FixedVector<T,NAXES,INSTANTIATED>> {
    constexpr static uint scale = 1;
};
template<typename T>
SYS_FORCE_INLINE bool utBoxExclude(const UT_Vector2T<T>& position) noexcept {
    return !SYSisFinite(position[0]) || !SYSisFinite(position[1]);
}
template<typename T>
SYS_FORCE_INLINE bool utBoxExclude(const UT_Vector3T<T>& position) noexcept {
    return !SYSisFinite(position[0]) || !SYSisFinite(position[1]) || !SYSisFinite(position[2]);
}
template<typename T>
SYS_FORCE_INLINE bool utBoxExclude(const UT_Vector4T<T>& position) noexcept {
    return !SYSisFinite(position[0]) || !SYSisFinite(position[1]) || !SYSisFinite(position[2]) || !SYSisFinite(position[3]);
}
template<>
SYS_FORCE_INLINE bool utBoxExclude(const UT_Vector2T<fpreal32>& position) noexcept {
    const int32 *ppositionints = reinterpret_cast<const int32*>(&position);
    // Fast check for NaN or infinity: check if exponent bits are 0xFF.
    bool has_nan_or_inf = ((ppositionints[0] & 0x7F800000) == 0x7F800000);
    has_nan_or_inf |= ((ppositionints[1] & 0x7F800000) == 0x7F800000);
    return has_nan_or_inf;
}
template<>
SYS_FORCE_INLINE bool utBoxExclude(const UT_Vector3T<fpreal32>& position) noexcept {
    const int32 *ppositionints = reinterpret_cast<const int32*>(&position);
    // Fast check for NaN or infinity: check if exponent bits are 0xFF.
    bool has_nan_or_inf = ((ppositionints[0] & 0x7F800000) == 0x7F800000);
    has_nan_or_inf |= ((ppositionints[1] & 0x7F800000) == 0x7F800000);
    has_nan_or_inf |= ((ppositionints[2] & 0x7F800000) == 0x7F800000);
    return has_nan_or_inf;
}
template<>
SYS_FORCE_INLINE bool utBoxExclude(const UT_Vector4T<fpreal32>& position) noexcept {
    const int32 *ppositionints = reinterpret_cast<const int32*>(&position);
    // Fast check for NaN or infinity: check if exponent bits are 0xFF.
    bool has_nan_or_inf = ((ppositionints[0] & 0x7F800000) == 0x7F800000);
    has_nan_or_inf |= ((ppositionints[1] & 0x7F800000) == 0x7F800000);
    has_nan_or_inf |= ((ppositionints[2] & 0x7F800000) == 0x7F800000);
    has_nan_or_inf |= ((ppositionints[3] & 0x7F800000) == 0x7F800000);
    return has_nan_or_inf;
}
template<typename T>
SYS_FORCE_INLINE T utBoxCenter(const UT_Vector2T<T>& position, uint axis) noexcept {
    return position[axis];
}
template<typename T>
SYS_FORCE_INLINE T utBoxCenter(const UT_Vector3T<T>& position, uint axis) noexcept {
    return position[axis];
}
template<typename T>
SYS_FORCE_INLINE T utBoxCenter(const UT_Vector4T<T>& position, uint axis) noexcept {
    return position[axis];
}
template<typename T>
struct ut_BoxCentre<UT_Vector2T<T>> {
    constexpr static uint scale = 1;
};
template<typename T>
struct ut_BoxCentre<UT_Vector3T<T>> {
    constexpr static uint scale = 1;
};
template<typename T>
struct ut_BoxCentre<UT_Vector4T<T>> {
    constexpr static uint scale = 1;
};

template<typename BOX_TYPE,typename SRC_INT_TYPE,typename INT_TYPE>
INT_TYPE utExcludeNaNInfBoxIndices(const BOX_TYPE* boxes, SRC_INT_TYPE* indices, INT_TYPE& nboxes) noexcept {

    constexpr INT_TYPE PARALLEL_THRESHOLD = 65536;
    INT_TYPE ntasks = 1;
    if (nboxes >= PARALLEL_THRESHOLD) {
        INT_TYPE nprocessors = UT_Thread::getNumProcessors();
        ntasks = (nprocessors > 1) ? SYSmin(4*nprocessors, nboxes/(PARALLEL_THRESHOLD/2)) : 1;
    }
    if (ntasks == 1) {
        // Serial: easy case; just loop through.

        const SRC_INT_TYPE* indices_end = indices + nboxes;

        // Loop through forward once
        SRC_INT_TYPE* psrc_index = indices;
        for (; psrc_index != indices_end; ++psrc_index) {
            const bool exclude = utBoxExclude(boxes[*psrc_index]);
            if (exclude)
                break;
        }
        if (psrc_index == indices_end)
            return 0;

        // First NaN or infinite box
        SRC_INT_TYPE* nan_start = psrc_index;
        for (++psrc_index; psrc_index != indices_end; ++psrc_index) {
            const bool exclude = utBoxExclude(boxes[*psrc_index]);
            if (!exclude) {
                *nan_start = *psrc_index;
                ++nan_start;
            }
        }
        nboxes = nan_start-indices;
        return indices_end - nan_start;
    }

    // Parallel: hard case.
    // 1) Collapse each of ntasks chunks and count number of items to exclude
    // 2) Accumulate number of items to exclude.
    // 3) If none, return.
    // 4) Copy non-NaN chunks

    UT_SmallArray<INT_TYPE> nexcluded;
    nexcluded.setSizeNoInit(ntasks);
    UTparallelFor(UT_BlockedRange<INT_TYPE>(0,ntasks), [boxes,indices,ntasks,nboxes,&nexcluded](const UT_BlockedRange<INT_TYPE>& r) {
        for (INT_TYPE taski = r.begin(), task_end = r.end(); taski < task_end; ++taski)
        {
            SRC_INT_TYPE* indices_start = indices + (taski*exint(nboxes))/ntasks;
            const SRC_INT_TYPE* indices_end = indices + ((taski+1)*exint(nboxes))/ntasks;
            SRC_INT_TYPE* psrc_index = indices_start;
            for (; psrc_index != indices_end; ++psrc_index)
            {
                const bool exclude = utBoxExclude(boxes[*psrc_index]);
                if (exclude)
                    break;
            }
            if (psrc_index == indices_end) {
                nexcluded[taski] = 0;
                continue;
            }

            // First NaN or infinite box
            SRC_INT_TYPE* nan_start = psrc_index;
            for (++psrc_index; psrc_index != indices_end; ++psrc_index) {
                const bool exclude = utBoxExclude(boxes[*psrc_index]);
                if (!exclude) {
                    *nan_start = *psrc_index;
                    ++nan_start;
                }
            }
            nexcluded[taski] = indices_end - nan_start;
        }
    }, 0, 1);

    // Accumulate
    INT_TYPE total_excluded = nexcluded[0];
    for (INT_TYPE taski = 1; taski < ntasks; ++taski) {
        total_excluded += nexcluded[taski];
        nexcluded[taski] = total_excluded;
    }

    if (total_excluded == 0)
        return 0;

    // TODO: Parallelize this part, if it's a bottleneck and we care about cases with NaNs or infinities.

    INT_TYPE taski = 0;
    while (nexcluded[taski] == 0) {
        ++taski;
    }

    SRC_INT_TYPE* dest_indices = indices + ((taski+1)*exint(nboxes))/ntasks - nexcluded[taski];
    SRC_INT_TYPE* dest_end = indices + nboxes - total_excluded;
    for (++taski; taski < ntasks && dest_indices < dest_end; ++taski)
    {
        const SRC_INT_TYPE* psrc_index = indices + (taski*exint(nboxes))/ntasks;
        const SRC_INT_TYPE* psrc_end = indices + ((taski+1)*exint(nboxes))/ntasks - nexcluded[taski];
        INT_TYPE count = psrc_end - psrc_index;
        memcpy(dest_indices, psrc_index, sizeof(SRC_INT_TYPE)*count);
        dest_indices += count;
    }
    nboxes -= total_excluded;
    return total_excluded;
}

template<uint N>
template<BVH_Heuristic H,typename T,uint NAXES,typename BOX_TYPE,typename SRC_INT_TYPE>
void BVH<N>::init(const BOX_TYPE* boxes, const INT_TYPE nboxes, SRC_INT_TYPE* indices, bool reorder_indices, INT_TYPE max_items_per_leaf) noexcept {
    Box<T,NAXES> axes_minmax;
    computeFullBoundingBox(axes_minmax, boxes, nboxes, indices);

    init<H>(axes_minmax, boxes, nboxes, indices, reorder_indices, max_items_per_leaf);
}

template<uint N>
template<BVH_Heuristic H,typename T,uint NAXES,typename BOX_TYPE,typename SRC_INT_TYPE>
void BVH<N>::init(Box<T,NAXES> axes_minmax, const BOX_TYPE* boxes, INT_TYPE nboxes, SRC_INT_TYPE* indices, bool reorder_indices, INT_TYPE max_items_per_leaf) noexcept {
    // Clear the tree in advance to save memory.
    myRoot.reset();

    if (nboxes == 0) {
        myNumNodes = 0;
        return;
    }

    UT_Array<INT_TYPE> local_indices;
    if (!indices) {
        local_indices.setSizeNoInit(nboxes);
        indices = local_indices.array();
        createTrivialIndices(indices, nboxes);
    }

    // Exclude any boxes with NaNs or infinities by shifting down indices
    // over the bad box indices and updating nboxes.
    INT_TYPE nexcluded = utExcludeNaNInfBoxIndices(boxes, indices, nboxes);
    if (nexcluded != 0) {
        if (nboxes == 0) {
            myNumNodes = 0;
            return;
        }
        computeFullBoundingBox(axes_minmax, boxes, nboxes, indices);
    }

    UT_Array<Node> nodes;
    // Preallocate an overestimate of the number of nodes needed.
    nodes.setCapacity(nodeEstimate(nboxes));
    nodes.setSize(1);
    if (reorder_indices)
        initNodeReorder<H>(nodes, nodes[0], axes_minmax, boxes, indices, nboxes, 0, max_items_per_leaf);
    else
        initNode<H>(nodes, nodes[0], axes_minmax, boxes, indices, nboxes);

    // If capacity is more than 12.5% over the size, rellocate.
    if (8*nodes.capacity() > 9*nodes.size()) {
        nodes.setCapacity(nodes.size());
    }
    // Steal ownership of the array from the UT_Array
    myRoot.reset(nodes.array());
    myNumNodes = nodes.size();
    nodes.unsafeClearData();
}

template <uint N>
int 
BVH<N>::getMaxDepthHelper(INT_TYPE node, int depth) const noexcept
{
    Node	*curnode = &myRoot[node];
    int		 newdepth = depth;

    for (int s = 0; s < N; s++)
    {
        const INT_TYPE node_int = curnode->child[s];
        if (Node::isInternal(node_int)) 
	{
            if (node_int == Node::EMPTY) 
	    {
                // NOTE: Anything after this will be empty too, so we can break.
                break;
            }
	    newdepth = SYSmax(newdepth, 
			    getMaxDepthHelper(Node::getInternalNum(node_int), 
					      depth+1)
			  );
        }
        else 
	{
	    // No additional depth
        }
    }

    return newdepth;
}

template<uint N>
int 
BVH<N>::getMaxDepth() const noexcept
{
    if (!myRoot.get())
	return 0;

    return getMaxDepthHelper(0, 1);
}

template <uint N>
int 
BVH<N>::getPureInternalDepthHelper(INT_TYPE node, int depth) const noexcept
{
    Node	*curnode = &myRoot[node];
    int		 childdepth = -1;

    for (int s = 0; s < N; s++)
    {
        const INT_TYPE node_int = curnode->child[s];
        if (Node::isInternal(node_int)) 
	{
            if (node_int == Node::EMPTY) 
	    {
                // NOTE: Anything after this will be empty too, so we can break.
                break;
            }
	    // Assuming we are pure internal, find this branch's depth
	    int d = getPureInternalDepthHelper(Node::getInternalNum(node_int), 
					       depth+1);
	    if (childdepth < 0)
		childdepth = d;
	    else
		childdepth = SYSmin(childdepth, d);
        }
        else 
	{
	    // This is not a purely internal node, so return the parent's
	    // depth
	    return depth;
        }
    }

    if (childdepth < 0)
    {
	// Nothing matched.  Returning depth causes an empty node
	// to act as if it has an external, which is wrong.  But
	// we shouldn't be constructing such empty nodes outside
	// of Node::EMPTY
	UT_ASSERT(!"Empty node other than Node::EMPTY encountered.");
	return depth;
    }

    return childdepth;
}

template<uint N>
int 
BVH<N>::getPureInternalDepth() const noexcept
{
    if (!myRoot.get())
	return 0;

    return getPureInternalDepthHelper(0, 0);
}

template<uint N>
template<typename LOCAL_DATA,typename FUNCTORS>
void BVH<N>::traverse(
    FUNCTORS &functors,
    LOCAL_DATA* data_for_parent) const noexcept
{
    if (!myRoot)
        return;

    // NOTE: The root is always index 0.
    traverseHelper(0, INT_TYPE(-1), functors, data_for_parent);
}
template<uint N>
template<typename LOCAL_DATA,typename FUNCTORS>
void BVH<N>::traverseHelper(
    INT_TYPE nodei,
    INT_TYPE parent_nodei,
    FUNCTORS &functors,
    LOCAL_DATA* data_for_parent) const noexcept
{
    const Node &node = myRoot[nodei];
    bool descend = functors.pre(nodei, data_for_parent);
    if (!descend)
        return;
    LOCAL_DATA local_data[N];
    INT_TYPE s;
    for (s = 0; s < N; ++s) {
        const INT_TYPE node_int = node.child[s];
        if (Node::isInternal(node_int)) {
            if (node_int == Node::EMPTY) {
                // NOTE: Anything after this will be empty too, so we can break.
                break;
            }
            traverseHelper(Node::getInternalNum(node_int), nodei, functors, &local_data[s]);
        }
        else {
            functors.item(node_int, nodei, local_data[s]);
        }
    }
    // NOTE: s is now the number of non-empty entries in this node.
    functors.post(nodei, parent_nodei, data_for_parent, s, local_data);
}

template<uint N>
template<typename LOCAL_DATA,typename FUNCTORS>
void BVH<N>::traverseParallel(
    INT_TYPE parallel_threshold,
    FUNCTORS& functors,
    LOCAL_DATA* data_for_parent) const noexcept
{
    if (!myRoot)
        return;

    // NOTE: The root is always index 0.
    traverseParallelHelper(0, INT_TYPE(-1), parallel_threshold, myNumNodes, functors, data_for_parent);
}
template<uint N>
template<typename LOCAL_DATA,typename FUNCTORS>
void BVH<N>::traverseParallelHelper(
    INT_TYPE nodei,
    INT_TYPE parent_nodei,
    INT_TYPE parallel_threshold,
    INT_TYPE next_node_id,
    FUNCTORS& functors,
    LOCAL_DATA* data_for_parent) const noexcept
{
    const Node &node = myRoot[nodei];
    bool descend = functors.pre(nodei, data_for_parent);
    if (!descend)
        return;

    // To determine the number of nodes in a child's subtree, we take the next
    // node ID minus the current child's node ID.
    INT_TYPE next_nodes[N];
    INT_TYPE nnodes[N];
    INT_TYPE nchildren = N;
    INT_TYPE nparallel = 0;
    // s is currently unsigned, so we check s < N for bounds check.
    // The s >= 0 check is in case s ever becomes signed, and should be
    // automatically removed by the compiler for unsigned s.
    for (INT_TYPE s = N-1; (std::is_signed<INT_TYPE>::value ? (s >= 0) : (s < N)); --s) {
        const INT_TYPE node_int = node.child[s];
        if (node_int == Node::EMPTY) {
            --nchildren;
            continue;
        }
        next_nodes[s] = next_node_id;
        if (Node::isInternal(node_int)) {
            // NOTE: This depends on BVH<N>::initNode appending the child nodes
            //       in between their content, instead of all at once.
            INT_TYPE child_node_id = Node::getInternalNum(node_int);
            nnodes[s] = next_node_id - child_node_id;
            next_node_id = child_node_id;
        }
        else {
            nnodes[s] = 0;
        }
        nparallel += (nnodes[s] >= parallel_threshold);
    }

    LOCAL_DATA local_data[N];
    if (nparallel >= 2) {
        // Do any non-parallel ones first
        if (nparallel < nchildren) {
            for (INT_TYPE s = 0; s < N; ++s) {
                if (nnodes[s] >= parallel_threshold) {
                    continue;
                }
                const INT_TYPE node_int = node.child[s];
                if (Node::isInternal(node_int)) {
                    if (node_int == Node::EMPTY) {
                        // NOTE: Anything after this will be empty too, so we can break.
                        break;
                    }
                    traverseHelper(Node::getInternalNum(node_int), nodei, functors, &local_data[s]);
                }
                else {
                    functors.item(node_int, nodei, local_data[s]);
                }
            }
        }
        // Now do the parallel ones
        UTparallelFor(UT_BlockedRange<INT_TYPE>(0,nparallel), [this,nodei,&node,&nnodes,&next_nodes,&parallel_threshold,&functors,&local_data](const UT_BlockedRange<INT_TYPE>& r) {
            for (INT_TYPE taski = r.begin(); taski < r.end(); ++taski) {
                INT_TYPE parallel_count = 0;
                // NOTE: The check for s < N is just so that the compiler can
                //       (hopefully) figure out that it can fully unroll the loop.
                INT_TYPE s;
                for (s = 0; s < N; ++s) {
                    if (nnodes[s] < parallel_threshold) {
                        continue;
                    }
                    if (parallel_count == taski) {
                        break;
                    }
                    ++parallel_count;
                }
                const INT_TYPE node_int = node.child[s];
                if (Node::isInternal(node_int)) {
                    UT_ASSERT_MSG_P(node_int != Node::EMPTY, "Empty entries should have been excluded above.");
                    traverseParallelHelper(Node::getInternalNum(node_int), nodei, parallel_threshold, next_nodes[s], functors, &local_data[s]);
                }
                else {
                    functors.item(node_int, nodei, local_data[s]);
                }
            }
        }, 0, 1);
    }
    else {
        // All in serial
        for (INT_TYPE s = 0; s < N; ++s) {
            const INT_TYPE node_int = node.child[s];
            if (Node::isInternal(node_int)) {
                if (node_int == Node::EMPTY) {
                    // NOTE: Anything after this will be empty too, so we can break.
                    break;
                }
                traverseHelper(Node::getInternalNum(node_int), nodei, functors, &local_data[s]);
            }
            else {
                functors.item(node_int, nodei, local_data[s]);
            }
        }
    }
    functors.post(nodei, parent_nodei, data_for_parent, nchildren, local_data);
}

template<uint N>
template<typename LOCAL_DATA,typename FUNCTORS>
void BVH<N>::traverseVector(
    FUNCTORS &functors,
    LOCAL_DATA* data_for_parent) const noexcept
{
    if (!myRoot)
        return;

    // NOTE: The root is always index 0.
    traverseVectorHelper(0, INT_TYPE(-1), functors, data_for_parent);
}
template<uint N>
template<typename LOCAL_DATA,typename FUNCTORS>
void BVH<N>::traverseVectorHelper(
    INT_TYPE nodei,
    INT_TYPE parent_nodei,
    FUNCTORS &functors,
    LOCAL_DATA* data_for_parent) const noexcept
{
    const Node &node = myRoot[nodei];
    INT_TYPE descend = functors.pre(nodei, data_for_parent);
    if (!descend)
        return;
    LOCAL_DATA local_data[N];
    INT_TYPE s;
    for (s = 0; s < N; ++s) {
        if ((descend>>s) & 1) {
            const INT_TYPE node_int = node.child[s];
            if (Node::isInternal(node_int)) {
                if (node_int == Node::EMPTY) {
                    // NOTE: Anything after this will be empty too, so we can break.
                    descend &= (INT_TYPE(1)<<s)-1;
                    break;
                }
                traverseVectorHelper(Node::getInternalNum(node_int), nodei, functors, &local_data[s]);
            }
            else {
                functors.item(node_int, nodei, local_data[s]);
            }
        }
    }
    // NOTE: s is now the number of non-empty entries in this node.
    functors.post(nodei, parent_nodei, data_for_parent, s, local_data, descend);
}

template<uint N>
template<typename SRC_INT_TYPE>
void BVH<N>::createTrivialIndices(SRC_INT_TYPE* indices, const INT_TYPE n) noexcept {
    constexpr INT_TYPE PARALLEL_THRESHOLD = 65536;
    INT_TYPE ntasks = 1;
    if (n >= PARALLEL_THRESHOLD) {
        INT_TYPE nprocessors = UT_Thread::getNumProcessors();
        ntasks = (nprocessors > 1) ? SYSmin(4*nprocessors, n/(PARALLEL_THRESHOLD/2)) : 1;
    }
    if (ntasks == 1) {
        for (INT_TYPE i = 0; i < n; ++i) {
            indices[i] = i;
        }
    }
    else {
        UTparallelFor(UT_BlockedRange<INT_TYPE>(0,n), [indices](const UT_BlockedRange<INT_TYPE>& r) {
            for (INT_TYPE i = r.begin(), end = r.end(); i != end; ++i) {
                indices[i] = i;
            }
        }, 0, 1);
    }
}

template<uint N>
template<typename T,uint NAXES,typename BOX_TYPE,typename SRC_INT_TYPE>
void BVH<N>::computeFullBoundingBox(Box<T,NAXES>& axes_minmax, const BOX_TYPE* boxes, const INT_TYPE nboxes, SRC_INT_TYPE* indices) noexcept {
    if (!nboxes) {
        axes_minmax.initBounds();
        return;
    }
    INT_TYPE ntasks = 1;
    if (nboxes >= 2*4096) {
        INT_TYPE nprocessors = UT_Thread::getNumProcessors();
        ntasks = (nprocessors > 1) ? SYSmin(4*nprocessors, nboxes/4096) : 1;
    }
    if (ntasks == 1) {
        Box<T,NAXES> box;
        if (indices) {
            box.initBounds(boxes[indices[0]]);
            for (INT_TYPE i = 1; i < nboxes; ++i) {
                box.combine(boxes[indices[i]]);
            }
        }
        else {
            box.initBounds(boxes[0]);
            for (INT_TYPE i = 1; i < nboxes; ++i) {
                box.combine(boxes[i]);
            }
        }
        axes_minmax = box;
    }
    else {
        // Combine boxes in parallel, into just a few boxes
        UT_SmallArray<Box<T,NAXES>> parallel_boxes;
        parallel_boxes.setSize(ntasks);
        UTparallelFor(UT_BlockedRange<INT_TYPE>(0,ntasks), [&parallel_boxes,ntasks,boxes,nboxes,indices](const UT_BlockedRange<INT_TYPE>& r) {
            for (INT_TYPE taski = r.begin(), end = r.end(); taski < end; ++taski) {
                const INT_TYPE startbox = (taski*uint64(nboxes))/ntasks;
                const INT_TYPE endbox = ((taski+1)*uint64(nboxes))/ntasks;
                Box<T,NAXES> box;
                if (indices) {
                    box.initBounds(boxes[indices[startbox]]);
                    for (INT_TYPE i = startbox+1; i < endbox; ++i) {
                        box.combine(boxes[indices[i]]);
                    }
                }
                else {
                    box.initBounds(boxes[startbox]);
                    for (INT_TYPE i = startbox+1; i < endbox; ++i) {
                        box.combine(boxes[i]);
                    }
                }
                parallel_boxes[taski] = box;
            }
        }, 0, 1);

        // Combine parallel_boxes
        Box<T,NAXES> box = parallel_boxes[0];
        for (INT_TYPE i = 1; i < ntasks; ++i) {
            box.combine(parallel_boxes[i]);
        }

        axes_minmax = box;
    }
}

template<uint N>
template<BVH_Heuristic H,typename T,uint NAXES,typename BOX_TYPE,typename SRC_INT_TYPE>
void BVH<N>::initNode(UT_Array<Node>& nodes, Node &node, const Box<T,NAXES>& axes_minmax, const BOX_TYPE* boxes, SRC_INT_TYPE* indices, INT_TYPE nboxes) noexcept {
    if (nboxes <= N) {
        // Fits in one node
        for (INT_TYPE i = 0; i < nboxes; ++i) {
            node.child[i] = indices[i];
        }
        for (INT_TYPE i = nboxes; i < N; ++i) {
            node.child[i] = Node::EMPTY;
        }
        return;
    }

    SRC_INT_TYPE* sub_indices[N+1];
    Box<T,NAXES> sub_boxes[N];

    if (N == 2) {
        sub_indices[0] = indices;
        sub_indices[2] = indices+nboxes;
        split<H>(axes_minmax, boxes, indices, nboxes, sub_indices[1], &sub_boxes[0]);
    }
    else {
        multiSplit<H>(axes_minmax, boxes, indices, nboxes, sub_indices, sub_boxes);
    }

    // Count the number of nodes to run in parallel and fill in single items in this node
    INT_TYPE nparallel = 0;
    static constexpr INT_TYPE PARALLEL_THRESHOLD = 1024;
    for (INT_TYPE i = 0; i < N; ++i) {
        INT_TYPE sub_nboxes = sub_indices[i+1]-sub_indices[i];
        if (sub_nboxes == 1) {
            node.child[i] = sub_indices[i][0];
        }
        else if (sub_nboxes >= PARALLEL_THRESHOLD) {
            ++nparallel;
        }
    }

    // NOTE: Child nodes of this node need to be placed just before the nodes in
    //       their corresponding subtree, in between the subtrees, because
    //       traverseParallel uses the difference between the child node IDs
    //       to determine the number of nodes in the subtree.

    // Recurse
    if (nparallel >= 2) {
        // Do the parallel ones first, so that they can be inserted in the right place.
        // Although the choice may seem somewhat arbitrary, we need the results to be
        // identical whether we choose to parallelize or not, and in case we change the
        // threshold later.
        UT_SmallArray<UT_Array<Node>> parallel_nodes;
        parallel_nodes.setSize(nparallel);
        UT_SmallArray<Node> parallel_parent_nodes;
        parallel_parent_nodes.setSize(nparallel);
        UTparallelFor(UT_BlockedRange<INT_TYPE>(0,nparallel), [&parallel_nodes,&parallel_parent_nodes,&sub_indices,boxes,&sub_boxes](const UT_BlockedRange<INT_TYPE>& r) {
            for (INT_TYPE taski = r.begin(), end = r.end(); taski < end; ++taski) {
                // First, find which child this is
                INT_TYPE counted_parallel = 0;
                INT_TYPE sub_nboxes;
                INT_TYPE childi;
                for (childi = 0; childi < N; ++childi) {
                    sub_nboxes = sub_indices[childi+1]-sub_indices[childi];
                    if (sub_nboxes >= PARALLEL_THRESHOLD) {
                        if (counted_parallel == taski) {
                            break;
                        }
                        ++counted_parallel;
                    }
                }
                UT_ASSERT_P(counted_parallel == taski);

                UT_Array<Node>& local_nodes = parallel_nodes[taski];
                // Preallocate an overestimate of the number of nodes needed.
                // At worst, we could have only 2 children in every leaf, and
                // then above that, we have a geometric series with r=1/N and a=(sub_nboxes/2)/N
                // The true worst case might be a little worst than this, but
                // it's probably fairly unlikely.
                local_nodes.setCapacity(nodeEstimate(sub_nboxes));
                Node& parent_node = parallel_parent_nodes[taski];

                // We'll have to fix the internal node numbers in parent_node and local_nodes later
                initNode<H>(local_nodes, parent_node, sub_boxes[childi], boxes, sub_indices[childi], sub_nboxes);
            }
        }, 0, 1);

        INT_TYPE counted_parallel = 0;
        for (INT_TYPE i = 0; i < N; ++i) {
            INT_TYPE sub_nboxes = sub_indices[i+1]-sub_indices[i];
            if (sub_nboxes != 1) {
                INT_TYPE local_nodes_start = nodes.size();
                node.child[i] = Node::markInternal(local_nodes_start);
                if (sub_nboxes >= PARALLEL_THRESHOLD) {
                    // First, adjust the root child node
                    Node child_node = parallel_parent_nodes[counted_parallel];
                    ++local_nodes_start;
                    for (INT_TYPE childi = 0; childi < N; ++childi) {
                        INT_TYPE child_child = child_node.child[childi];
                        if (Node::isInternal(child_child) && child_child != Node::EMPTY) {
                            child_child += local_nodes_start;
                            child_node.child[childi] = child_child;
                        }
                    }

                    // Make space in the array for the sub-child nodes
                    const UT_Array<Node>& local_nodes = parallel_nodes[counted_parallel];
                    ++counted_parallel;
                    INT_TYPE n = local_nodes.size();
                    nodes.bumpSize(local_nodes_start + n);
                    nodes[local_nodes_start-1] = child_node;
                }
                else {
                    nodes.bumpSize(local_nodes_start + 1);
                    initNode<H>(nodes, nodes[local_nodes_start], sub_boxes[i], boxes, sub_indices[i], sub_nboxes);
                }
            }
        }

        // Now, adjust and copy all sub-child nodes that were made in parallel
        adjustParallelChildNodes<PARALLEL_THRESHOLD>(nparallel, nodes, node, parallel_nodes.array(), sub_indices);
    }
    else {
        for (INT_TYPE i = 0; i < N; ++i) {
            INT_TYPE sub_nboxes = sub_indices[i+1]-sub_indices[i];
            if (sub_nboxes != 1) {
                INT_TYPE local_nodes_start = nodes.size();
                node.child[i] = Node::markInternal(local_nodes_start);
                nodes.bumpSize(local_nodes_start + 1);
                initNode<H>(nodes, nodes[local_nodes_start], sub_boxes[i], boxes, sub_indices[i], sub_nboxes);
            }
        }
    }
}

template<uint N>
template<BVH_Heuristic H,typename T,uint NAXES,typename BOX_TYPE,typename SRC_INT_TYPE>
void BVH<N>::initNodeReorder(UT_Array<Node>& nodes, Node &node, const Box<T,NAXES>& axes_minmax, const BOX_TYPE* boxes, SRC_INT_TYPE* indices, INT_TYPE nboxes, const INT_TYPE indices_offset, const INT_TYPE max_items_per_leaf) noexcept {
    if (nboxes <= N) {
        // Fits in one node
        for (INT_TYPE i = 0; i < nboxes; ++i) {
            node.child[i] = indices_offset+i;
        }
        for (INT_TYPE i = nboxes; i < N; ++i) {
            node.child[i] = Node::EMPTY;
        }
        return;
    }

    SRC_INT_TYPE* sub_indices[N+1];
    Box<T,NAXES> sub_boxes[N];

    if (N == 2) {
        sub_indices[0] = indices;
        sub_indices[2] = indices+nboxes;
        split<H>(axes_minmax, boxes, indices, nboxes, sub_indices[1], &sub_boxes[0]);
    }
    else {
        multiSplit<H>(axes_minmax, boxes, indices, nboxes, sub_indices, sub_boxes);
    }

    // Move any children with max_items_per_leaf or fewer indices before any children with more,
    // for better cache coherence when we're accessing data in a corresponding array.
    INT_TYPE nleaves = 0;
    UT_SmallArray<SRC_INT_TYPE> leaf_indices;
    SRC_INT_TYPE leaf_sizes[N];
    INT_TYPE sub_nboxes0 = sub_indices[1]-sub_indices[0];
    if (sub_nboxes0 <= max_items_per_leaf) {
        leaf_sizes[0] = sub_nboxes0;
        for (int j = 0; j < sub_nboxes0; ++j)
            leaf_indices.append(sub_indices[0][j]);
        ++nleaves;
    }
    INT_TYPE sub_nboxes1 = sub_indices[2]-sub_indices[1];
    if (sub_nboxes1 <= max_items_per_leaf) {
        leaf_sizes[nleaves] = sub_nboxes1;
        for (int j = 0; j < sub_nboxes1; ++j)
            leaf_indices.append(sub_indices[1][j]);
        ++nleaves;
    }
    for (INT_TYPE i = 2; i < N; ++i) {
        INT_TYPE sub_nboxes = sub_indices[i+1]-sub_indices[i];
        if (sub_nboxes <= max_items_per_leaf) {
            leaf_sizes[nleaves] = sub_nboxes;
            for (int j = 0; j < sub_nboxes; ++j)
                leaf_indices.append(sub_indices[i][j]);
            ++nleaves;
        }
    }
    if (nleaves > 0) {
        // NOTE: i < N condition is because INT_TYPE is unsigned.
        //       i >= 0 condition is in case INT_TYPE is changed to signed.
        INT_TYPE move_distance = 0;
        INT_TYPE index_move_distance = 0;
        for (INT_TYPE i = N-1; (std::is_signed<INT_TYPE>::value ? (i >= 0) : (i < N)); --i) {
            INT_TYPE sub_nboxes = sub_indices[i+1]-sub_indices[i];
            if (sub_nboxes <= max_items_per_leaf) {
                ++move_distance;
                index_move_distance += sub_nboxes;
            }
            else if (move_distance > 0) {
                SRC_INT_TYPE *start_src_index = sub_indices[i];
                for (SRC_INT_TYPE *src_index = sub_indices[i+1]-1; src_index >= start_src_index; --src_index) {
                    src_index[index_move_distance] = src_index[0];
                }
                sub_indices[i+move_distance] = sub_indices[i]+index_move_distance;
            }
        }
        index_move_distance = 0;
        for (INT_TYPE i = 0; i < nleaves; ++i) {
            INT_TYPE sub_nboxes = leaf_sizes[i];
            sub_indices[i] = indices+index_move_distance;
            for (int j = 0; j < sub_nboxes; ++j)
                indices[index_move_distance+j] = leaf_indices[index_move_distance+j];
            index_move_distance += sub_nboxes;
        }
    }

    // Count the number of nodes to run in parallel and fill in single items in this node
    INT_TYPE nparallel = 0;
    static constexpr INT_TYPE PARALLEL_THRESHOLD = 1024;
    for (INT_TYPE i = 0; i < N; ++i) {
        INT_TYPE sub_nboxes = sub_indices[i+1]-sub_indices[i];
        if (sub_nboxes <= max_items_per_leaf) {
            node.child[i] = indices_offset+(sub_indices[i]-sub_indices[0]);
        }
        else if (sub_nboxes >= PARALLEL_THRESHOLD) {
            ++nparallel;
        }
    }

    // NOTE: Child nodes of this node need to be placed just before the nodes in
    //       their corresponding subtree, in between the subtrees, because
    //       traverseParallel uses the difference between the child node IDs
    //       to determine the number of nodes in the subtree.

    // Recurse
    if (nparallel >= 2) {
        // Do the parallel ones first, so that they can be inserted in the right place.
        // Although the choice may seem somewhat arbitrary, we need the results to be
        // identical whether we choose to parallelize or not, and in case we change the
        // threshold later.
        UT_SmallArray<UT_Array<Node>,4*sizeof(UT_Array<Node>)> parallel_nodes;
        parallel_nodes.setSize(nparallel);
        UT_SmallArray<Node,4*sizeof(Node)> parallel_parent_nodes;
        parallel_parent_nodes.setSize(nparallel);
        UTparallelFor(UT_BlockedRange<INT_TYPE>(0,nparallel), [&parallel_nodes,&parallel_parent_nodes,&sub_indices,boxes,&sub_boxes,indices_offset,max_items_per_leaf](const UT_BlockedRange<INT_TYPE>& r) {
            for (INT_TYPE taski = r.begin(), end = r.end(); taski < end; ++taski) {
                // First, find which child this is
                INT_TYPE counted_parallel = 0;
                INT_TYPE sub_nboxes;
                INT_TYPE childi;
                for (childi = 0; childi < N; ++childi) {
                    sub_nboxes = sub_indices[childi+1]-sub_indices[childi];
                    if (sub_nboxes >= PARALLEL_THRESHOLD) {
                        if (counted_parallel == taski) {
                            break;
                        }
                        ++counted_parallel;
                    }
                }
                UT_ASSERT_P(counted_parallel == taski);

                UT_Array<Node>& local_nodes = parallel_nodes[taski];
                // Preallocate an overestimate of the number of nodes needed.
                // At worst, we could have only 2 children in every leaf, and
                // then above that, we have a geometric series with r=1/N and a=(sub_nboxes/2)/N
                // The true worst case might be a little worst than this, but
                // it's probably fairly unlikely.
                local_nodes.setCapacity(nodeEstimate(sub_nboxes));
                Node& parent_node = parallel_parent_nodes[taski];

                // We'll have to fix the internal node numbers in parent_node and local_nodes later
                initNodeReorder<H>(local_nodes, parent_node, sub_boxes[childi], boxes, sub_indices[childi], sub_nboxes,
                    indices_offset+(sub_indices[childi]-sub_indices[0]), max_items_per_leaf);
            }
        }, 0, 1);

        INT_TYPE counted_parallel = 0;
        for (INT_TYPE i = 0; i < N; ++i) {
            INT_TYPE sub_nboxes = sub_indices[i+1]-sub_indices[i];
            if (sub_nboxes > max_items_per_leaf) {
                INT_TYPE local_nodes_start = nodes.size();
                node.child[i] = Node::markInternal(local_nodes_start);
                if (sub_nboxes >= PARALLEL_THRESHOLD) {
                    // First, adjust the root child node
                    Node child_node = parallel_parent_nodes[counted_parallel];
                    ++local_nodes_start;
                    for (INT_TYPE childi = 0; childi < N; ++childi) {
                        INT_TYPE child_child = child_node.child[childi];
                        if (Node::isInternal(child_child) && child_child != Node::EMPTY) {
                            child_child += local_nodes_start;
                            child_node.child[childi] = child_child;
                        }
                    }

                    // Make space in the array for the sub-child nodes
                    const UT_Array<Node>& local_nodes = parallel_nodes[counted_parallel];
                    ++counted_parallel;
                    INT_TYPE n = local_nodes.size();
                    nodes.bumpSize(local_nodes_start + n);
                    nodes[local_nodes_start-1] = child_node;
                }
                else {
                    nodes.bumpSize(local_nodes_start + 1);
                    initNodeReorder<H>(nodes, nodes[local_nodes_start], sub_boxes[i], boxes, sub_indices[i], sub_nboxes,
                        indices_offset+(sub_indices[i]-sub_indices[0]), max_items_per_leaf);
                }
            }
        }

        // Now, adjust and copy all sub-child nodes that were made in parallel
        adjustParallelChildNodes<PARALLEL_THRESHOLD>(nparallel, nodes, node, parallel_nodes.array(), sub_indices);
    }
    else {
        for (INT_TYPE i = 0; i < N; ++i) {
            INT_TYPE sub_nboxes = sub_indices[i+1]-sub_indices[i];
            if (sub_nboxes > max_items_per_leaf) {
                INT_TYPE local_nodes_start = nodes.size();
                node.child[i] = Node::markInternal(local_nodes_start);
                nodes.bumpSize(local_nodes_start + 1);
                initNodeReorder<H>(nodes, nodes[local_nodes_start], sub_boxes[i], boxes, sub_indices[i], sub_nboxes,
                    indices_offset+(sub_indices[i]-sub_indices[0]), max_items_per_leaf);
            }
        }
    }
}

template<uint N>
template<BVH_Heuristic H,typename T,uint NAXES,typename BOX_TYPE,typename SRC_INT_TYPE>
void BVH<N>::multiSplit(const Box<T,NAXES>& axes_minmax, const BOX_TYPE* boxes, SRC_INT_TYPE* indices, INT_TYPE nboxes, SRC_INT_TYPE* sub_indices[N+1], Box<T,NAXES> sub_boxes[N]) noexcept {
    sub_indices[0] = indices;
    sub_indices[2] = indices+nboxes;
    split<H>(axes_minmax, boxes, indices, nboxes, sub_indices[1], &sub_boxes[0]);

    if (N == 2) {
        return;
    }

    if (H == BVH_Heuristic::MEDIAN_MAX_AXIS) {
        SRC_INT_TYPE* sub_indices_startend[2*N];
        Box<T,NAXES> sub_boxes_unsorted[N];
        sub_boxes_unsorted[0] = sub_boxes[0];
        sub_boxes_unsorted[1] = sub_boxes[1];
        sub_indices_startend[0] = sub_indices[0];
        sub_indices_startend[1] = sub_indices[1];
        sub_indices_startend[2] = sub_indices[1];
        sub_indices_startend[3] = sub_indices[2];
        for (INT_TYPE nsub = 2; nsub < N; ++nsub) {
            SRC_INT_TYPE* selected_start = sub_indices_startend[0];
            SRC_INT_TYPE* selected_end = sub_indices_startend[1];
            Box<T,NAXES> sub_box = sub_boxes_unsorted[0];

            // Shift results back.
            for (INT_TYPE i = 0; i < nsub-1; ++i) {
                sub_indices_startend[2*i  ] = sub_indices_startend[2*i+2];
                sub_indices_startend[2*i+1] = sub_indices_startend[2*i+3];
            }
            for (INT_TYPE i = 0; i < nsub-1; ++i) {
                sub_boxes_unsorted[i] = sub_boxes_unsorted[i-1];
            }

            // Do the split
            split<H>(sub_box, boxes, selected_start, selected_end-selected_start, sub_indices_startend[2*nsub-1], &sub_boxes_unsorted[nsub]);
            sub_indices_startend[2*nsub-2] = selected_start;
            sub_indices_startend[2*nsub] = sub_indices_startend[2*nsub-1];
            sub_indices_startend[2*nsub+1] = selected_end;

            // Sort pointers so that they're in the correct order
            sub_indices[N] = indices+nboxes;
            for (INT_TYPE i = 0; i < N; ++i) {
                SRC_INT_TYPE* prev_pointer = (i != 0) ? sub_indices[i-1] : nullptr;
                SRC_INT_TYPE* min_pointer = nullptr;
                Box<T,NAXES> box;
                for (INT_TYPE j = 0; j < N; ++j) {
                    SRC_INT_TYPE* cur_pointer = sub_indices_startend[2*j];
                    if ((cur_pointer > prev_pointer) && (!min_pointer || (cur_pointer < min_pointer))) {
                        min_pointer = cur_pointer;
                        box = sub_boxes_unsorted[j];
                    }
                }
                UT_ASSERT_P(min_pointer);
                sub_indices[i] = min_pointer;
                sub_boxes[i] = box;
            }
        }
    }
    else {
        T sub_box_areas[N];
        sub_box_areas[0] = unweightedHeuristic<H>(sub_boxes[0]);
        sub_box_areas[1] = unweightedHeuristic<H>(sub_boxes[1]);
        for (INT_TYPE nsub = 2; nsub < N; ++nsub) {
            // Choose which one to split
            INT_TYPE split_choice = INT_TYPE(-1);
            T max_heuristic;
            for (INT_TYPE i = 0; i < nsub; ++i) {
                const INT_TYPE index_count = (sub_indices[i+1]-sub_indices[i]);
                if (index_count > 1) {
                    const T heuristic = sub_box_areas[i]*index_count;
                    if (split_choice == INT_TYPE(-1) || heuristic > max_heuristic) {
                        split_choice = i;
                        max_heuristic = heuristic;
                    }
                }
            }
            UT_ASSERT_MSG_P(split_choice != INT_TYPE(-1), "There should always be at least one that can be split!");

            SRC_INT_TYPE* selected_start = sub_indices[split_choice];
            SRC_INT_TYPE* selected_end = sub_indices[split_choice+1];

            // Shift results over; we can skip the one we selected.
            for (INT_TYPE i = nsub; i > split_choice; --i) {
                sub_indices[i+1] = sub_indices[i];
            }
            for (INT_TYPE i = nsub-1; i > split_choice; --i) {
                sub_boxes[i+1] = sub_boxes[i];
            }
            for (INT_TYPE i = nsub-1; i > split_choice; --i) {
                sub_box_areas[i+1] = sub_box_areas[i];
            }

            // Do the split
            split<H>(sub_boxes[split_choice], boxes, selected_start, selected_end-selected_start, sub_indices[split_choice+1], &sub_boxes[split_choice]);
            sub_box_areas[split_choice] = unweightedHeuristic<H>(sub_boxes[split_choice]);
            sub_box_areas[split_choice+1] = unweightedHeuristic<H>(sub_boxes[split_choice+1]);
        }
    }
}

template<uint N>
template<BVH_Heuristic H,typename T,uint NAXES,typename BOX_TYPE,typename SRC_INT_TYPE>
void BVH<N>::split(const Box<T,NAXES>& axes_minmax, const BOX_TYPE* boxes, SRC_INT_TYPE* indices, INT_TYPE nboxes, SRC_INT_TYPE*& split_indices, Box<T,NAXES>* split_boxes) noexcept {
    if (nboxes == 2) {
        split_boxes[0].initBounds(boxes[indices[0]]);
        split_boxes[1].initBounds(boxes[indices[1]]);
        split_indices = indices+1;
        return;
    }
    UT_ASSERT_MSG_P(nboxes > 2, "Cases with less than 3 boxes should have already been handled!");

    if (H == BVH_Heuristic::MEDIAN_MAX_AXIS) {
        UT_ASSERT_MSG(0, "FIXME: Implement this!!!");
    }

    constexpr INT_TYPE SMALL_LIMIT = 6;
    if (nboxes <= SMALL_LIMIT) {
        // Special case for a small number of boxes: check all (2^(n-1))-1 partitions.
        // Without loss of generality, we assume that box 0 is in partition 0,
        // and that not all boxes are in partition 0.
        Box<T,NAXES> local_boxes[SMALL_LIMIT];
        for (INT_TYPE box = 0; box < nboxes; ++box) {
            local_boxes[box].initBounds(boxes[indices[box]]);
            //printf("Box %u: (%f-%f)x(%f-%f)x(%f-%f)\n", uint(box), local_boxes[box].vals[0][0], local_boxes[box].vals[0][1], local_boxes[box].vals[1][0], local_boxes[box].vals[1][1], local_boxes[box].vals[2][0], local_boxes[box].vals[2][1]);
        }
        const INT_TYPE partition_limit = (INT_TYPE(1)<<(nboxes-1));
        INT_TYPE best_partition = INT_TYPE(-1);
        T best_heuristic;
        for (INT_TYPE partition_bits = 1; partition_bits < partition_limit; ++partition_bits) {
            Box<T,NAXES> sub_boxes[2];
            sub_boxes[0] = local_boxes[0];
            sub_boxes[1].initBounds();
            INT_TYPE sub_counts[2] = {1,0};
            for (INT_TYPE bit = 0; bit < nboxes-1; ++bit) {
                INT_TYPE dest = (partition_bits>>bit)&1;
                sub_boxes[dest].combine(local_boxes[bit+1]);
                ++sub_counts[dest];
            }
            //printf("Partition bits %u: sub_box[0]: (%f-%f)x(%f-%f)x(%f-%f)\n", uint(partition_bits), sub_boxes[0].vals[0][0], sub_boxes[0].vals[0][1], sub_boxes[0].vals[1][0], sub_boxes[0].vals[1][1], sub_boxes[0].vals[2][0], sub_boxes[0].vals[2][1]);
            //printf("Partition bits %u: sub_box[1]: (%f-%f)x(%f-%f)x(%f-%f)\n", uint(partition_bits), sub_boxes[1].vals[0][0], sub_boxes[1].vals[0][1], sub_boxes[1].vals[1][0], sub_boxes[1].vals[1][1], sub_boxes[1].vals[2][0], sub_boxes[1].vals[2][1]);
            const T heuristic =
                unweightedHeuristic<H>(sub_boxes[0])*sub_counts[0] +
                unweightedHeuristic<H>(sub_boxes[1])*sub_counts[1];
            //printf("Partition bits %u: heuristic = %f (= %f*%u + %f*%u)\n",uint(partition_bits),heuristic, unweightedHeuristic<H>(sub_boxes[0]), uint(sub_counts[0]), unweightedHeuristic<H>(sub_boxes[1]), uint(sub_counts[1]));
            if (best_partition == INT_TYPE(-1) || heuristic < best_heuristic) {
                //printf("    New best\n");
                best_partition = partition_bits;
                best_heuristic = heuristic;
                split_boxes[0] = sub_boxes[0];
                split_boxes[1] = sub_boxes[1];
            }
        }

#if 0 // This isn't actually necessary with the current design, because I changed how the number of subtree nodes is determined.
        // If best_partition is partition_limit-1, there's only 1 box
        // in partition 0.  We should instead put this in partition 1,
        // so that we can help always have the internal node indices first
        // in each node.  That gets used to (fairly) quickly determine
        // the number of nodes in a sub-tree.
        if (best_partition == partition_limit - 1) {
            // Put the first index last.
            SRC_INT_TYPE last_index = indices[0];
            SRC_INT_TYPE* dest_indices = indices;
            SRC_INT_TYPE* local_split_indices = indices + nboxes-1;
            for (; dest_indices != local_split_indices; ++dest_indices) {
                dest_indices[0] = dest_indices[1];
            }
            *local_split_indices = last_index;
            split_indices = local_split_indices;

            // Swap the boxes
            const Box<T,NAXES> temp_box = sub_boxes[0];
            sub_boxes[0] = sub_boxes[1];
            sub_boxes[1] = temp_box;
            return;
        }
#endif

        // Reorder the indices.
        // NOTE: Index 0 is always in partition 0, so can stay put.
        SRC_INT_TYPE local_indices[SMALL_LIMIT-1];
        for (INT_TYPE box = 0; box < nboxes-1; ++box) {
            local_indices[box] = indices[box+1];
        }
        SRC_INT_TYPE* dest_indices = indices+1;
        SRC_INT_TYPE* src_indices = local_indices;
        // Copy partition 0
        for (INT_TYPE bit = 0; bit < nboxes-1; ++bit, ++src_indices) {
            if (!((best_partition>>bit)&1)) {
                //printf("Copying %u into partition 0\n",uint(*src_indices));
                *dest_indices = *src_indices;
                ++dest_indices;
            }
        }
        split_indices = dest_indices;
        // Copy partition 1
        src_indices = local_indices;
        for (INT_TYPE bit = 0; bit < nboxes-1; ++bit, ++src_indices) {
            if ((best_partition>>bit)&1) {
                //printf("Copying %u into partition 1\n",uint(*src_indices));
                *dest_indices = *src_indices;
                ++dest_indices;
            }
        }
        return;
    }
    
    uint max_axis = 0;
    T max_axis_length = axes_minmax.vals[0][1] - axes_minmax.vals[0][0];
    for (uint axis = 1; axis < NAXES; ++axis) {
        const T axis_length = axes_minmax.vals[axis][1] - axes_minmax.vals[axis][0];
        if (axis_length > max_axis_length) {
            max_axis = axis;
            max_axis_length = axis_length;
        }
    }

    if (!(max_axis_length > T(0))) {
        // All boxes are a single point or NaN.
        // Pick an arbitrary split point.
        split_indices = indices + nboxes/2;
        split_boxes[0] = axes_minmax;
        split_boxes[1] = axes_minmax;
        return;
    }

#if BVH_TRY_ALL_AXES
    Box<T,NAXES> half_box(axes_minmax);
    half_box.vals[max_axis][1] = 0.5*(half_box.vals[max_axis][0] + half_box.vals[max_axis][1]);
    T good_heuristic = unweightedHeuristic<H>(half_box)*nboxes;
#else
    const INT_TYPE axis = max_axis;
#endif

    constexpr INT_TYPE MID_LIMIT = 2*NSPANS;
    if (nboxes <= MID_LIMIT) {
#if BVH_TRY_ALL_AXES
        // This nullptr is used as an indication to not check best_heuristic and always replace.
        split_indices = nullptr;
        T best_heuristic;
        splitMiddleCountAxis<H>(max_axis, best_heuristic, boxes, indices, nboxes, split_indices, split_boxes);

        // Check whether it might be worth trying other axes
        if (best_heuristic > 1.2*good_heuristic)
        {
            uint axis1 = (max_axis==0) ? 2 : (max_axis-1);
            splitMiddleCountAxis<H>(axis1, best_heuristic, boxes, indices, nboxes, split_indices, split_boxes);

            uint axis2 = (axis1==0) ? 2 : (axis1-1);
            splitMiddleCountAxis<H>(axis2, best_heuristic, boxes, indices, nboxes, split_indices, split_boxes);
        }
        return;
    }

    // More than twice as many boxes as splits (>32 boxes at the time of writing).

    Box<T,NAXES> best_left_split_box;
    Box<T,NAXES> best_right_split_box;
    INT_TYPE best_left_count_split;
    INT_TYPE maxaxis_first_left_count = -1;
    INT_TYPE maxaxis_last_left_count = -1;

    T best_heuristic;
    INT_TYPE best_axis = -1;
    INT_TYPE best_split_index = -1;
    splitLargeCountAxis<H>(max_axis, axes_minmax.vals[max_axis][0], max_axis_length,
        best_heuristic, best_axis, best_split_index,
        best_left_split_box, best_right_split_box, best_left_count_split,
        maxaxis_first_left_count, maxaxis_last_left_count,
        boxes, indices, nboxes);

    // Check whether it might be worth trying other axes
    if (best_heuristic > 1.2*good_heuristic)
    {
        uint axis1 = (max_axis==0) ? 2 : (max_axis-1);
        splitLargeCountAxis<H>(axis1, axes_minmax.vals[axis1][0], axes_minmax.vals[axis1][1] - axes_minmax.vals[axis1][0],
            best_heuristic, best_axis, best_split_index,
            best_left_split_box, best_right_split_box, best_left_count_split,
            maxaxis_first_left_count, maxaxis_last_left_count,
            boxes, indices, nboxes);

        uint axis2 = (axis1==0) ? 2 : (axis1-1);
        splitLargeCountAxis<H>(axis2, axes_minmax.vals[axis2][0], axes_minmax.vals[axis2][1] - axes_minmax.vals[axis2][0],
            best_heuristic, best_axis, best_split_index,
            best_left_split_box, best_right_split_box, best_left_count_split,
            maxaxis_first_left_count, maxaxis_last_left_count,
            boxes, indices, nboxes);
    }

    SRC_INT_TYPE*const indices_end = indices+nboxes;

    if (best_split_index == -1) {
        // No split was anywhere close to balanced, so we fall back to searching for one.

        const INT_TYPE min_count = nboxes/MIN_FRACTION;
        const INT_TYPE max_count = ((MIN_FRACTION-1)*uint64(nboxes))/MIN_FRACTION;

        // First, find the span containing the "balance" point, namely where left_counts goes from
        // being less than min_count to more than max_count.
        // If that's span 0, use max_count as the ordered index to select,
        // if it's span NSPANS-1, use min_count as the ordered index to select,
        // else use nboxes/2 as the ordered index to select.
        //T min_pivotx2 = -std::numeric_limits<T>::infinity();
        //T max_pivotx2 = std::numeric_limits<T>::infinity();
        SRC_INT_TYPE* nth_index;
        if (maxaxis_first_left_count > max_count) {
            // Search for max_count ordered index
            nth_index = indices+max_count;
            //max_pivotx2 = max_axis_min_x2 + max_axis_length/(NSPANS/ut_BoxCentre<BOX_TYPE>::scale);
        }
        else if (maxaxis_last_left_count < min_count) {
            // Search for min_count ordered index
            nth_index = indices+min_count;
            //min_pivotx2 = max_axis_min_x2 + max_axis_length - max_axis_length/(NSPANS/ut_BoxCentre<BOX_TYPE>::scale);
        }
        else {
            // Search for nboxes/2 ordered index
            nth_index = indices+nboxes/2;
            //for (INT_TYPE spliti = 1; spliti < NSPLITS; ++spliti) {
            //    // The second condition should be redundant, but is just in case.
            //    if (left_counts[spliti] > max_count || spliti == NSPLITS-1) {
            //        min_pivotx2 = max_axis_min_x2 + spliti*max_axis_length/(NSPANS/ut_BoxCentre<BOX_TYPE>::scale);
            //        max_pivotx2 = max_axis_min_x2 + (spliti+1)*max_axis_length/(NSPANS/ut_BoxCentre<BOX_TYPE>::scale);
            //        break;
            //    }
            //}
        }
        nthElement<T>(boxes,indices,indices+nboxes,max_axis,nth_index);//,min_pivotx2,max_pivotx2);

        split_indices = nth_index;
        Box<T,NAXES> left_box(boxes[indices[0]]);
        for (SRC_INT_TYPE* left_indices = indices+1; left_indices < nth_index; ++left_indices) {
            left_box.combine(boxes[*left_indices]);
        }
        Box<T,NAXES> right_box(boxes[nth_index[0]]);
        for (SRC_INT_TYPE* right_indices = nth_index+1; right_indices < indices_end; ++right_indices) {
            right_box.combine(boxes[*right_indices]);
        }
        split_boxes[0] = left_box;
        split_boxes[1] = right_box;
    }
    else {
        constexpr uint center_scale = ut_BoxCentre<BOX_TYPE>::scale;
        const T best_axis_min_x2 = center_scale*axes_minmax.vals[best_axis][0];
        const T best_axis_length = axes_minmax.vals[best_axis][1] - axes_minmax.vals[best_axis][0];
        const T pivotx2 = best_axis_min_x2 + (best_split_index+1)*best_axis_length/(NSPANS/center_scale);
        SRC_INT_TYPE* ppivot_start;
        SRC_INT_TYPE* ppivot_end;
        partitionByCentre(boxes,indices,indices+nboxes,best_axis,pivotx2,ppivot_start,ppivot_end);

        split_indices = indices + best_left_count_split;

        // Ignoring roundoff error, we would have
        // split_indices >= ppivot_start && split_indices <= ppivot_end,
        // but it may not always be in practice.
        if (split_indices >= ppivot_start && split_indices <= ppivot_end) {
            split_boxes[0] = best_left_split_box;
            split_boxes[1] = best_right_split_box;
            return;
        }

        // Roundoff error changed the split, so we need to recompute the boxes.
        if (split_indices < ppivot_start) {
            split_indices = ppivot_start;
        }
        else {//(split_indices > ppivot_end)
            split_indices = ppivot_end;
        }

        // Emergency checks, just in case
        if (split_indices == indices) {
            ++split_indices;
        }
        else if (split_indices == indices_end) {
            --split_indices;
        }

        Box<T,NAXES> left_box(boxes[indices[0]]);
        for (SRC_INT_TYPE* left_indices = indices+1; left_indices < split_indices; ++left_indices) {
            left_box.combine(boxes[*left_indices]);
        }
        Box<T,NAXES> right_box(boxes[split_indices[0]]);
        for (SRC_INT_TYPE* right_indices = split_indices+1; right_indices < indices_end; ++right_indices) {
            right_box.combine(boxes[*right_indices]);
        }
        split_boxes[0] = left_box;
        split_boxes[1] = right_box;
    }
}

template<uint N>
template<BVH_Heuristic H,typename T,uint NAXES,typename BOX_TYPE,typename SRC_INT_TYPE>
void BVH<N>::splitMiddleCountAxis(const INT_TYPE axis, T &best_heuristic, const BOX_TYPE* boxes, SRC_INT_TYPE* indices, INT_TYPE nboxes, SRC_INT_TYPE*& split_indices, Box<T,NAXES>* split_boxes) noexcept {
#endif
    // Sort along axis, and try all possible splits.

#if 1
#if BVH_TRY_ALL_AXES
    constexpr INT_TYPE MID_LIMIT = 2*NSPANS;
    UT_ASSERT_P(nboxes <= MID_LIMIT);
#endif

    // First, compute midpoints
    T midpointsx2[MID_LIMIT];
    for (INT_TYPE i = 0; i < nboxes; ++i) {
        midpointsx2[i] = utBoxCenter(boxes[indices[i]], axis);
    }
    SRC_INT_TYPE local_indices[MID_LIMIT];
    for (INT_TYPE i = 0; i < nboxes; ++i) {
        local_indices[i] = i;
    }

    const INT_TYPE chunk_starts[5] = {0, nboxes/4, nboxes/2, INT_TYPE((3*uint64(nboxes))/4), nboxes};

    // For sorting, insertion sort 4 chunks and merge them
    for (INT_TYPE chunk = 0; chunk < 4; ++chunk) {
        const INT_TYPE start = chunk_starts[chunk];
        const INT_TYPE end = chunk_starts[chunk+1];
        for (INT_TYPE i = start+1; i < end; ++i) {
            SRC_INT_TYPE indexi = local_indices[i];
            T vi = midpointsx2[indexi];
            for (INT_TYPE j = start; j < i; ++j) {
                SRC_INT_TYPE indexj = local_indices[j];
                T vj = midpointsx2[indexj];
                if (vi < vj) {
                    do {
                        local_indices[j] = indexi;
                        indexi = indexj;
                        ++j;
                        if (j == i) {
                            local_indices[j] = indexi;
                            break;
                        }
                        indexj = local_indices[j];
                    } while (true);
                    break;
                }
            }
        }
    }
    // Merge chunks into another buffer
    SRC_INT_TYPE local_indices_temp[MID_LIMIT];
    std::merge(local_indices, local_indices+chunk_starts[1],
        local_indices+chunk_starts[1], local_indices+chunk_starts[2],
        local_indices_temp, [&midpointsx2](const SRC_INT_TYPE a, const SRC_INT_TYPE b)->bool {
        return midpointsx2[a] < midpointsx2[b];
    });
    std::merge(local_indices+chunk_starts[2], local_indices+chunk_starts[3],
        local_indices+chunk_starts[3], local_indices+chunk_starts[4],
        local_indices_temp+chunk_starts[2], [&midpointsx2](const SRC_INT_TYPE a, const SRC_INT_TYPE b)->bool {
        return midpointsx2[a] < midpointsx2[b];
    });
    std::merge(local_indices_temp, local_indices_temp+chunk_starts[2],
        local_indices_temp+chunk_starts[2], local_indices_temp+chunk_starts[4],
        local_indices, [&midpointsx2](const SRC_INT_TYPE a, const SRC_INT_TYPE b)->bool {
        return midpointsx2[a] < midpointsx2[b];
    });

    // Translate local_indices into indices
    for (INT_TYPE i = 0; i < nboxes; ++i) {
        local_indices[i] = indices[local_indices[i]];
    }
#if !BVH_TRY_ALL_AXES
        // Copy back
        for (INT_TYPE i = 0; i < nboxes; ++i) {
            indices[i] = local_indices[i];
        }
#endif
#else
    std::stable_sort(indices, indices+nboxes, [boxes,max_axis](SRC_INT_TYPE a, SRC_INT_TYPE b)->bool {
        return utBoxCenter(boxes[a], max_axis) < utBoxCenter(boxes[b], max_axis);
    });
#endif

    // Accumulate boxes
    Box<T,NAXES> left_boxes[MID_LIMIT-1];
    Box<T,NAXES> right_boxes[MID_LIMIT-1];
    const INT_TYPE nsplits = nboxes-1;
    Box<T,NAXES> box_accumulator(boxes[local_indices[0]]);
    left_boxes[0] = box_accumulator;
    for (INT_TYPE i = 1; i < nsplits; ++i) {
        box_accumulator.combine(boxes[local_indices[i]]);
        left_boxes[i] = box_accumulator;
    }
    box_accumulator.initBounds(boxes[local_indices[nsplits-1]]);
    right_boxes[nsplits-1] = box_accumulator;
    for (INT_TYPE i = nsplits-1; i > 0; --i) {
        box_accumulator.combine(boxes[local_indices[i]]);
        right_boxes[i-1] = box_accumulator;
    }

    INT_TYPE best_split = 0;
    T best_local_heuristic =
    unweightedHeuristic<H>(left_boxes[0]) +
    unweightedHeuristic<H>(right_boxes[0])*(nboxes-1);
    for (INT_TYPE split = 1; split < nsplits; ++split) {
        const T heuristic =
            unweightedHeuristic<H>(left_boxes[split])*(split+1) +
            unweightedHeuristic<H>(right_boxes[split])*(nboxes-(split+1));
        if (heuristic < best_local_heuristic) {
            best_split = split;
            best_local_heuristic = heuristic;
        }
    }

#if BVH_TRY_ALL_AXES
    if (!split_indices || best_local_heuristic < best_heuristic) {
        // Copy back
        for (INT_TYPE i = 0; i < nboxes; ++i) {
            indices[i] = local_indices[i];
        }
        split_indices = indices+best_split+1;
        split_boxes[0] = left_boxes[best_split];
        split_boxes[1] = right_boxes[best_split];
        best_heuristic = best_local_heuristic;
    }
#else
        split_indices = indices+best_split+1;
        split_boxes[0] = left_boxes[best_split];
        split_boxes[1] = right_boxes[best_split];
        return;
    }
#endif
#if BVH_TRY_ALL_AXES
}

template<uint N>
template<BVH_Heuristic H,typename T,uint NAXES,typename BOX_TYPE,typename SRC_INT_TYPE>
void BVH<N>::splitLargeCountAxis(
    const INT_TYPE axis, const T axis_min, const T axis_length,
    T &best_heuristic, INT_TYPE &best_axis, INT_TYPE &best_split_index,
    Box<T,NAXES> &best_left_split_box,
    Box<T,NAXES> &best_right_split_box,
    INT_TYPE &best_left_count_split,
    INT_TYPE &maxaxis_first_left_count,
    INT_TYPE &maxaxis_last_left_count,
    const BOX_TYPE* boxes, SRC_INT_TYPE* indices, INT_TYPE nboxes
) noexcept {
#else
    const T axis_min = axes_minmax.vals[max_axis][0];
    const T axis_length = max_axis_length;
#endif
    Box<T,NAXES> span_boxes[NSPANS];
    for (INT_TYPE i = 0; i < NSPANS; ++i) {
        span_boxes[i].initBounds();
    }
    INT_TYPE span_counts[NSPANS];
    for (INT_TYPE i = 0; i < NSPANS; ++i) {
        span_counts[i] = 0;
    }

    const T axis_min_x2 = ut_BoxCentre<BOX_TYPE>::scale*axis_min;
    // NOTE: Factor of 0.5 is factored out of the average when using the average value to determine the span that a box lies in.
    const T axis_index_scale = (T(1.0/ut_BoxCentre<BOX_TYPE>::scale)*NSPANS)/axis_length;
    constexpr INT_TYPE BOX_SPANS_PARALLEL_THRESHOLD = 2048;
    INT_TYPE ntasks = 1;
    if (nboxes >= BOX_SPANS_PARALLEL_THRESHOLD) {
        INT_TYPE nprocessors = UT_Thread::getNumProcessors();
        ntasks = (nprocessors > 1) ? SYSmin(4*nprocessors, nboxes/(BOX_SPANS_PARALLEL_THRESHOLD/2)) : 1;
    }
    if (ntasks == 1) {
        for (INT_TYPE indexi = 0; indexi < nboxes; ++indexi) {
            const auto& box = boxes[indices[indexi]];
            const T sum = utBoxCenter(box, axis);
            const uint span_index = SYSclamp(int((sum-axis_min_x2)*axis_index_scale), int(0), int(NSPANS-1));
            ++span_counts[span_index];
            Box<T,NAXES>& span_box = span_boxes[span_index];
            span_box.combine(box);
        }
    }
    else {
        // Combine boxes in parallel, into just a few boxes
        UT_SmallArray<Box<T,NAXES>> parallel_boxes;
        parallel_boxes.setSize(NSPANS*ntasks);
        UT_SmallArray<INT_TYPE> parallel_counts;
        parallel_counts.setSize(NSPANS*ntasks);
        UTparallelFor(UT_BlockedRange<INT_TYPE>(0,ntasks), [&parallel_boxes,&parallel_counts,ntasks,boxes,nboxes,indices,axis,axis_min_x2,axis_index_scale](const UT_BlockedRange<INT_TYPE>& r) {
            for (INT_TYPE taski = r.begin(), end = r.end(); taski < end; ++taski) {
                Box<T,NAXES> span_boxes[NSPANS];
                for (INT_TYPE i = 0; i < NSPANS; ++i) {
                    span_boxes[i].initBounds();
                }
                INT_TYPE span_counts[NSPANS];
                for (INT_TYPE i = 0; i < NSPANS; ++i) {
                    span_counts[i] = 0;
                }
                const INT_TYPE startbox = (taski*uint64(nboxes))/ntasks;
                const INT_TYPE endbox = ((taski+1)*uint64(nboxes))/ntasks;
                for (INT_TYPE indexi = startbox; indexi != endbox; ++indexi) {
                    const auto& box = boxes[indices[indexi]];
                    const T sum = utBoxCenter(box, axis);
                    const uint span_index = SYSclamp(int((sum-axis_min_x2)*axis_index_scale), int(0), int(NSPANS-1));
                    ++span_counts[span_index];
                    Box<T,NAXES>& span_box = span_boxes[span_index];
                    span_box.combine(box);
                }
                // Copy the results out
                const INT_TYPE dest_array_start = taski*NSPANS;
                for (INT_TYPE i = 0; i < NSPANS; ++i) {
                    parallel_boxes[dest_array_start+i] = span_boxes[i];
                }
                for (INT_TYPE i = 0; i < NSPANS; ++i) {
                    parallel_counts[dest_array_start+i] = span_counts[i];
                }
            }
        }, 0, 1);

        // Combine the partial results
        for (INT_TYPE taski = 0; taski < ntasks; ++taski) {
            const INT_TYPE dest_array_start = taski*NSPANS;
            for (INT_TYPE i = 0; i < NSPANS; ++i) {
                span_boxes[i].combine(parallel_boxes[dest_array_start+i]);
            }
        }
        for (INT_TYPE taski = 0; taski < ntasks; ++taski) {
            const INT_TYPE dest_array_start = taski*NSPANS;
            for (INT_TYPE i = 0; i < NSPANS; ++i) {
                span_counts[i] += parallel_counts[dest_array_start+i];
            }
        }
    }

    // Spans 0 to NSPANS-2
    Box<T,NAXES> left_boxes[NSPLITS];
    // Spans 1 to NSPANS-1
    Box<T,NAXES> right_boxes[NSPLITS];

    // Accumulate boxes
    Box<T,NAXES> box_accumulator = span_boxes[0];
    left_boxes[0] = box_accumulator;
    for (INT_TYPE i = 1; i < NSPLITS; ++i) {
        box_accumulator.combine(span_boxes[i]);
        left_boxes[i] = box_accumulator;
    }
    box_accumulator = span_boxes[NSPANS-1];
    right_boxes[NSPLITS-1] = box_accumulator;
    for (INT_TYPE i = NSPLITS-1; i > 0; --i) {
        box_accumulator.combine(span_boxes[i]);
        right_boxes[i-1] = box_accumulator;
    }

    INT_TYPE left_counts[NSPLITS];

    // Accumulate counts
    INT_TYPE count_accumulator = span_counts[0];
    left_counts[0] = count_accumulator;
    for (INT_TYPE spliti = 1; spliti < NSPLITS; ++spliti) {
        count_accumulator += span_counts[spliti];
        left_counts[spliti] = count_accumulator;
    }

    // Check which split is optimal, making sure that at least 1/MIN_FRACTION of all boxes are on each side.
    const INT_TYPE min_count = nboxes/MIN_FRACTION;
    UT_ASSERT_MSG_P(min_count > 0, "MID_LIMIT above should have been large enough that nboxes would be > MIN_FRACTION");
    const INT_TYPE max_count = ((MIN_FRACTION-1)*uint64(nboxes))/MIN_FRACTION;
    UT_ASSERT_MSG_P(max_count < nboxes, "I'm not sure how this could happen mathematically, but it needs to be checked.");
    T smallest_heuristic = std::numeric_limits<T>::infinity();
    INT_TYPE split_index = -1;
    for (INT_TYPE spliti = 0; spliti < NSPLITS; ++spliti) {
        const INT_TYPE left_count = left_counts[spliti];
        if (left_count < min_count || left_count > max_count) {
            continue;
        }
        const INT_TYPE right_count = nboxes-left_count;
        const T heuristic =
            left_count*unweightedHeuristic<H>(left_boxes[spliti]) +
            right_count*unweightedHeuristic<H>(right_boxes[spliti]);
        if (heuristic < smallest_heuristic) {
            smallest_heuristic = heuristic;
            split_index = spliti;
        }
    }

#if BVH_TRY_ALL_AXES
    if (maxaxis_first_left_count == -1) {
        maxaxis_first_left_count = left_counts[0];
        maxaxis_last_left_count = left_counts[NSPLITS-1];
    }
    if (split_index == -1) {
        return;
    }

    if (best_axis < 0 || smallest_heuristic < best_heuristic) {
        best_axis = axis;
        best_split_index = split_index;
        best_heuristic = smallest_heuristic;
        best_left_split_box = left_boxes[split_index];
        best_right_split_box = right_boxes[split_index];
        best_left_count_split = left_counts[split_index];
    }
    return;
#else
    SRC_INT_TYPE*const indices_end = indices+nboxes;

    if (split_index == -1) {
        // No split was anywhere close to balanced, so we fall back to searching for one.

        // First, find the span containing the "balance" point, namely where left_counts goes from
        // being less than min_count to more than max_count.
        // If that's span 0, use max_count as the ordered index to select,
        // if it's span NSPANS-1, use min_count as the ordered index to select,
        // else use nboxes/2 as the ordered index to select.
        //T min_pivotx2 = -std::numeric_limits<T>::infinity();
        //T max_pivotx2 = std::numeric_limits<T>::infinity();
        SRC_INT_TYPE* nth_index;
        if (left_counts[0] > max_count) {
            // Search for max_count ordered index
            nth_index = indices+max_count;
            //max_pivotx2 = max_axis_min_x2 + max_axis_length/(NSPANS/ut_BoxCentre<BOX_TYPE>::scale);
        }
        else if (left_counts[NSPLITS-1] < min_count) {
            // Search for min_count ordered index
            nth_index = indices+min_count;
            //min_pivotx2 = max_axis_min_x2 + max_axis_length - max_axis_length/(NSPANS/ut_BoxCentre<BOX_TYPE>::scale);
        }
        else {
            // Search for nboxes/2 ordered index
            nth_index = indices+nboxes/2;
            //for (INT_TYPE spliti = 1; spliti < NSPLITS; ++spliti) {
            //    // The second condition should be redundant, but is just in case.
            //    if (left_counts[spliti] > max_count || spliti == NSPLITS-1) {
            //        min_pivotx2 = max_axis_min_x2 + spliti*max_axis_length/(NSPANS/ut_BoxCentre<BOX_TYPE>::scale);
            //        max_pivotx2 = max_axis_min_x2 + (spliti+1)*max_axis_length/(NSPANS/ut_BoxCentre<BOX_TYPE>::scale);
            //        break;
            //    }
            //}
        }
        nthElement<T>(boxes,indices,indices+nboxes,max_axis,nth_index);//,min_pivotx2,max_pivotx2);

        split_indices = nth_index;
        Box<T,NAXES> left_box(boxes[indices[0]]);
        for (SRC_INT_TYPE* left_indices = indices+1; left_indices < nth_index; ++left_indices) {
            left_box.combine(boxes[*left_indices]);
        }
        Box<T,NAXES> right_box(boxes[nth_index[0]]);
        for (SRC_INT_TYPE* right_indices = nth_index+1; right_indices < indices_end; ++right_indices) {
            right_box.combine(boxes[*right_indices]);
        }
        split_boxes[0] = left_box;
        split_boxes[1] = right_box;
    }
    else {
        const T pivotx2 = axis_min_x2 + (split_index+1)*axis_length/(NSPANS/ut_BoxCentre<BOX_TYPE>::scale);
        SRC_INT_TYPE* ppivot_start;
        SRC_INT_TYPE* ppivot_end;
        partitionByCentre(boxes,indices,indices+nboxes,max_axis,pivotx2,ppivot_start,ppivot_end);

        split_indices = indices + left_counts[split_index];

        // Ignoring roundoff error, we would have
        // split_indices >= ppivot_start && split_indices <= ppivot_end,
        // but it may not always be in practice.
        if (split_indices >= ppivot_start && split_indices <= ppivot_end) {
            split_boxes[0] = left_boxes[split_index];
            split_boxes[1] = right_boxes[split_index];
            return;
        }

        // Roundoff error changed the split, so we need to recompute the boxes.
        if (split_indices < ppivot_start) {
            split_indices = ppivot_start;
        }
        else {//(split_indices > ppivot_end)
            split_indices = ppivot_end;
        }

        // Emergency checks, just in case
        if (split_indices == indices) {
            ++split_indices;
        }
        else if (split_indices == indices_end) {
            --split_indices;
        }

        Box<T,NAXES> left_box(boxes[indices[0]]);
        for (SRC_INT_TYPE* left_indices = indices+1; left_indices < split_indices; ++left_indices) {
            left_box.combine(boxes[*left_indices]);
        }
        Box<T,NAXES> right_box(boxes[split_indices[0]]);
        for (SRC_INT_TYPE* right_indices = split_indices+1; right_indices < indices_end; ++right_indices) {
            right_box.combine(boxes[*right_indices]);
        }
        split_boxes[0] = left_box;
        split_boxes[1] = right_box;
    }
#endif
}

template<uint N>
template<uint PARALLEL_THRESHOLD, typename SRC_INT_TYPE>
void BVH<N>::adjustParallelChildNodes(INT_TYPE nparallel, UT_Array<Node>& nodes, Node& node, UT_Array<Node>* parallel_nodes, SRC_INT_TYPE* sub_indices) noexcept
{
    UTparallelFor(UT_BlockedRange<INT_TYPE>(0,nparallel), [&node,&nodes,&parallel_nodes,&sub_indices](const UT_BlockedRange<INT_TYPE>& r) {
        INT_TYPE counted_parallel = 0;
        INT_TYPE childi = 0;
        for (INT_TYPE taski = r.begin(), end = r.end(); taski < end; ++taski) {
            // First, find which child this is
            INT_TYPE sub_nboxes;
            for (; childi < N; ++childi) {
                sub_nboxes = sub_indices[childi+1]-sub_indices[childi];
                if (sub_nboxes >= PARALLEL_THRESHOLD) {
                    if (counted_parallel == taski) {
                        break;
                    }
                    ++counted_parallel;
                }
            }
            UT_ASSERT_P(counted_parallel == taski);

            const UT_Array<Node>& local_nodes = parallel_nodes[counted_parallel];
            INT_TYPE n = local_nodes.size();
            INT_TYPE local_nodes_start = Node::getInternalNum(node.child[childi])+1;
            ++counted_parallel;
            ++childi;

            for (INT_TYPE j = 0; j < n; ++j) {
                Node local_node = local_nodes[j];
                for (INT_TYPE childj = 0; childj < N; ++childj) {
                    INT_TYPE local_child = local_node.child[childj];
                    if (Node::isInternal(local_child) && local_child != Node::EMPTY) {
                        local_child += local_nodes_start;
                        local_node.child[childj] = local_child;
                    }
                }
                nodes[local_nodes_start+j] = local_node;
            }
        }
    }, 0, 1);
}

template<uint N>
template<typename T,typename BOX_TYPE,typename SRC_INT_TYPE>
void BVH<N>::nthElement(const BOX_TYPE* boxes, SRC_INT_TYPE* indices, const SRC_INT_TYPE* indices_end, const uint axis, SRC_INT_TYPE*const nth) noexcept {//, const T min_pivotx2, const T max_pivotx2) noexcept {
    while (true) {
        // Choose median of first, middle, and last as the pivot
        T pivots[3] = {
            utBoxCenter(boxes[indices[0]], axis),
            utBoxCenter(boxes[indices[(indices_end-indices)/2]], axis),
            utBoxCenter(boxes[*(indices_end-1)], axis)
        };
        if (pivots[0] < pivots[1]) {
            const T temp = pivots[0];
            pivots[0] = pivots[1];
            pivots[1] = temp;
        }
        if (pivots[0] < pivots[2]) {
            const T temp = pivots[0];
            pivots[0] = pivots[2];
            pivots[2] = temp;
        }
        if (pivots[1] < pivots[2]) {
            const T temp = pivots[1];
            pivots[1] = pivots[2];
            pivots[2] = temp;
        }
        T mid_pivotx2 = pivots[1];
#if 0
        // We limit the pivot, because we know that the true value is between min and max
        if (mid_pivotx2 < min_pivotx2) {
            mid_pivotx2 = min_pivotx2;
        }
        else if (mid_pivotx2 > max_pivotx2) {
            mid_pivotx2 = max_pivotx2;
        }
#endif
        SRC_INT_TYPE* pivot_start;
        SRC_INT_TYPE* pivot_end;
        partitionByCentre(boxes,indices,indices_end,axis,mid_pivotx2,pivot_start,pivot_end);
        if (nth < pivot_start) {
            indices_end = pivot_start;
        }
        else if (nth < pivot_end) {
            // nth is in the middle of the pivot range,
            // which is in the right place, so we're done.
            return;
        }
        else {
            indices = pivot_end;
        }
        if (indices_end <= indices+1) {
            return;
        }
    }
}

template<uint N>
template<typename T,typename BOX_TYPE,typename SRC_INT_TYPE>
void BVH<N>::partitionByCentre(const BOX_TYPE* boxes, SRC_INT_TYPE*const indices, const SRC_INT_TYPE*const indices_end, const uint axis, const T pivotx2, SRC_INT_TYPE*& ppivot_start, SRC_INT_TYPE*& ppivot_end) noexcept {
    // TODO: Consider parallelizing this!

    // First element >= pivot
    SRC_INT_TYPE* pivot_start = indices;
    // First element > pivot
    SRC_INT_TYPE* pivot_end = indices;

    // Loop through forward once
    for (SRC_INT_TYPE* psrc_index = indices; psrc_index != indices_end; ++psrc_index) {
        const T srcsum = utBoxCenter(boxes[*psrc_index], axis);
        if (srcsum < pivotx2) {
            if (psrc_index != pivot_start) {
                if (pivot_start == pivot_end) {
                    // Common case: nothing equal to the pivot
                    const SRC_INT_TYPE temp = *psrc_index;
                    *psrc_index = *pivot_start;
                    *pivot_start = temp;
                }
                else {
                    // Less common case: at least one thing equal to the pivot
                    const SRC_INT_TYPE temp = *psrc_index;
                    *psrc_index = *pivot_end;
                    *pivot_end = *pivot_start;
                    *pivot_start = temp;
                }
            }
            ++pivot_start;
            ++pivot_end;
        }
        else if (srcsum == pivotx2) {
            // Add to the pivot area
            if (psrc_index != pivot_end) {
                const SRC_INT_TYPE temp = *psrc_index;
                *psrc_index = *pivot_end;
                *pivot_end = temp;
            }
            ++pivot_end;
        }
    }
    ppivot_start = pivot_start;
    ppivot_end = pivot_end;
}

#if 0
template<uint N>
void BVH<N>::debugDump() const {
    printf("\nNode 0: {\n");
    UT_WorkBuffer indent;
    indent.append(80, ' ');
    UT_Array<INT_TYPE> stack;
    stack.append(0);
    stack.append(0);
    while (!stack.isEmpty()) {
        int depth = stack.size()/2;
        if (indent.length() < 4*depth) {
            indent.append(4, ' ');
        }
        INT_TYPE cur_nodei = stack[stack.size()-2];
        INT_TYPE cur_i = stack[stack.size()-1];
        if (cur_i == N) {
            printf(indent.buffer()+indent.length()-(4*(depth-1)));
            printf("}\n");
            stack.removeLast();
            stack.removeLast();
            continue;
        }
        ++stack[stack.size()-1];
        Node& cur_node = myRoot[cur_nodei];
        INT_TYPE child_nodei = cur_node.child[cur_i];
        if (Node::isInternal(child_nodei)) {
            if (child_nodei == Node::EMPTY) {
                printf(indent.buffer()+indent.length()-(4*(depth-1)));
                printf("}\n");
                stack.removeLast();
                stack.removeLast();
                continue;
            }
            INT_TYPE internal_node = Node::getInternalNum(child_nodei);
            printf(indent.buffer()+indent.length()-(4*depth));
            printf("Node %u: {\n", uint(internal_node));
            stack.append(internal_node);
            stack.append(0);
            continue;
        }
        else {
            printf(indent.buffer()+indent.length()-(4*depth));
            printf("Tri %u\n", uint(child_nodei));
        }
    }
}
#endif

template<uint BVH_N,typename ITEM_BOX,typename NODE_BOX>
SYS_FORCE_INLINE void createBVHNodeBoxes(const UT::BVH<BVH_N> &bvh, const ITEM_BOX *item_boxes, NODE_BOX *node_boxes) noexcept
{
    struct NodeBoxFunctors
    {
        NODE_BOX *myNodeData;
        //const uint *const myIndices;
        const ITEM_BOX *myItemBoxes;

        SYS_FORCE_INLINE NodeBoxFunctors(
            NODE_BOX *node_data,
            //const uint *indices,
            const ITEM_BOX *item_boxes)
            : myNodeData(node_data)
            //, myIndices(indices)
            , myItemBoxes(item_boxes)
        {}
        constexpr SYS_FORCE_INLINE bool pre(const int nodei, ITEM_BOX *data_for_parent) const
        {
            return true;
        }
        void SYS_FORCE_INLINE item(const int itemi, const int parent_nodei, ITEM_BOX &data_for_parent) const
        {
            data_for_parent = myItemBoxes[itemi];//myIndices[itemi]];
        }

        void post(const int nodei, const int parent_nodei, ITEM_BOX *data_for_parent, const int nchildren, const ITEM_BOX *child_data_array) const
        {
            ITEM_BOX box(child_data_array[0]);
            for (int i = 1; i < nchildren; ++i)
                box.combine(child_data_array[i]);

            if (data_for_parent)
                *data_for_parent = box;

            NODE_BOX &current_node_data = myNodeData[nodei];
            current_node_data = box;
        }
    };

    constexpr uint PARALLEL_THRESHOLD = 4096;
    NodeBoxFunctors functors(node_boxes, item_boxes);
    bvh.template traverseParallel<ITEM_BOX>(PARALLEL_THRESHOLD, functors);
}

template<uint NAXES,typename T,uint BVH_N,typename ITEM_BOX,typename NODE_BOX>
SYS_FORCE_INLINE void createBVHInterleavedBoxes(const UT::BVH<BVH_N> &bvh, const ITEM_BOX *item_boxes, NODE_BOX *node_boxes, float expand_factor) noexcept
{
    struct NodeBoxFunctors
    {
        NODE_BOX *const myNodeData;
        //const uint *const myIndices;
        const ITEM_BOX *const myItemBoxes;
        const float myExpandFactor;

        SYS_FORCE_INLINE NodeBoxFunctors(
            NODE_BOX *node_data,
            //const uint *indices,
            const ITEM_BOX *item_boxes,
            const float expand_factor=0.0f)
            : myNodeData(node_data)
            //, myIndices(indices)
            , myItemBoxes(item_boxes)
            , myExpandFactor(expand_factor)
        {}
        constexpr SYS_FORCE_INLINE bool pre(const int nodei, ITEM_BOX *data_for_parent) const
        {
            return true;
        }
        void SYS_FORCE_INLINE item(const int itemi, const int parent_nodei, ITEM_BOX &data_for_parent) const
        {
            data_for_parent = myItemBoxes[itemi];//myIndices[itemi]];
        }

        void post(const int nodei, const int parent_nodei, ITEM_BOX *data_for_parent, const int nchildren, const ITEM_BOX *child_data_array) const
        {
            ITEM_BOX box(child_data_array[0]);
            for (int i = 1; i < nchildren; ++i)
                box.combine(child_data_array[i]);

            if (data_for_parent)
                *data_for_parent = box;

            NODE_BOX &current_node_data = myNodeData[nodei];

            SYS_STATIC_ASSERT(sizeof(NODE_BOX) == NAXES*2*BVH_N*sizeof(T));
            SYS_STATIC_ASSERT(sizeof(current_node_data.vals[0][0]) == BVH_N*sizeof(T));
            if (!myExpandFactor)
            {
                for (int i = 0; i < nchildren; ++i)
                {
                    const ITEM_BOX &local_box = child_data_array[i];
                    for (int axis = 0; axis < NAXES; ++axis)
                    {
                        ((T*)&current_node_data.vals[axis][0])[i] = local_box.vals[axis][0];
                        ((T*)&current_node_data.vals[axis][1])[i] = local_box.vals[axis][1];
                    }
                }
            }
            else
            {
                for (int i = 0; i < nchildren; ++i)
                {
                    const ITEM_BOX &local_box = child_data_array[i];
                    for (int axis = 0; axis < NAXES; ++axis)
                    {
                        float minv = local_box.vals[axis][0];
                        float maxv = local_box.vals[axis][1];
                        SYS_STATIC_ASSERT(sizeof(local_box.vals[axis][0]) == sizeof(int32));
                        // We add an epsilon, to ensure that we always end up with expanded values.
                        // NOTE: The -1 is just so that it's twice the real epsilon, without any additional multiplication.
                        int32 epsilon = SYSmax(0x00800000, SYSmax(
                            ((int32*)local_box.vals[axis])[0] & 0x7F800000,
                            ((int32*)local_box.vals[axis])[1] & 0x7F800000) - ((23-1)<<23));
                        float diff = myExpandFactor*(maxv-minv) + (*(fpreal32*)&epsilon);
                        minv -= diff;
                        maxv += diff;
                        ((T*)&current_node_data.vals[axis][0])[i] = minv;
                        ((T*)&current_node_data.vals[axis][1])[i] = maxv;
                    }
                }
            }
            for (int i = nchildren; i < BVH_N; ++i)
            {
                // Set to invalid, to avoid intersecting any of them.
                for (int axis = 0; axis < NAXES; ++axis)
                {
                    // Using max and -max instead of intinity and -infinity,
                    // just in case something needs to compute the centre of a potential box,
                    // since max-max is zero, but infinity-infinity is NaN.
                    ((T*)&current_node_data.vals[axis][0])[i] = std::numeric_limits<T>::max();
                    ((T*)&current_node_data.vals[axis][1])[i] = -std::numeric_limits<T>::max();
                }
            }
        }
    };

    // On Linux, setting this to 64 ensures that thread-reliant architectures
    // like Ryzen will split often enough to be competitive.  
    constexpr uint PARALLEL_THRESHOLD = 64;
    NodeBoxFunctors functors(node_boxes, item_boxes, expand_factor);
    bvh.template traverseParallel<ITEM_BOX>(PARALLEL_THRESHOLD, functors);
}

template<typename T,uint NAXES,bool PARALLEL,typename INT_TYPE0,typename DATA,typename INT_TYPE>
void utCreateBVHInterleavedBoxesHelper(
    INT_TYPE nodei,
    INT_TYPE next_node_id,
    const DATA& data,
    UT::Box<T,NAXES>* data_for_parent) noexcept
{
    const auto &node = data.myRoot[nodei];

    static constexpr uint N = 4;
    using Node = typename UT::BVH<N>::Node;

    // To determine the number of nodes in a child's subtree, we take the next
    // node ID minus the current child's node ID.
    INT_TYPE next_nodes[N];
    INT_TYPE nnodes[N];
    INT_TYPE nchildren = N;
    INT_TYPE nparallel = 0;
    if (PARALLEL)
    {
        // s is currently unsigned, so we check s < N for bounds check.
        // The s >= 0 check is in case s ever becomes signed, and should be
        // automatically removed by the compiler for unsigned s.
        for (INT_TYPE s = N-1; (std::is_signed<INT_TYPE>::value ? (s >= 0) : (s < N)); --s) {
            const INT_TYPE node_int = node.child[s];
            if (node_int == Node::EMPTY) {
                --nchildren;
                continue;
            }
            next_nodes[s] = next_node_id;
            if (Node::isInternal(node_int)) {
                // NOTE: This depends on BVH<N>::initNode appending the child nodes
                //       in between their content, instead of all at once.
                INT_TYPE child_node_id = Node::getInternalNum(node_int);
                nnodes[s] = next_node_id - child_node_id;
                next_node_id = child_node_id;
            }
            else {
                nnodes[s] = 0;
            }
            nparallel += (nnodes[s] >= data.myParallelThreshold);
        }
    }

    using LOCAL_DATA = UT::Box<T,NAXES>;
    LOCAL_DATA local_data[N];

    auto&& item_functor = [](const DATA& data, LOCAL_DATA& local_data, INT_TYPE s, INT_TYPE nodei, INT_TYPE node_int) {
        const int32 count = ((const int32*)(data.myNodeNItems+nodei))[s];
        UT_ASSERT_P(count >= 1);
        if (count == 1) {
            INT_TYPE0 index = data.myIndices ? data.myIndices[node_int] : INT_TYPE0(node_int);
            local_data.initBounds(data.myItemBoxes[index]);
        }
        else {
            INT_TYPE0 index0;
            INT_TYPE0 index1;
            if (data.myIndices) {
                index0 = data.myIndices[node_int];
                index1 = data.myIndices[node_int+1];
            }
            else {
                index0 = INT_TYPE0(node_int);
                index1 = INT_TYPE0(node_int+1);
            }
            local_data.initBoundsUnordered(data.myItemBoxes[index0], data.myItemBoxes[index1]);
            const INT_TYPE end_node_int = node_int+count;
            node_int += 2;
            for (; node_int < end_node_int; ++node_int) {
                const INT_TYPE0 index = data.myIndices ? data.myIndices[node_int] : INT_TYPE0(node_int);
                local_data.combine(data.myItemBoxes[index]);
            }
        }
    };

    if (nparallel >= 2) {
        // Do any non-parallel ones first
        if (nparallel < nchildren) {
            for (INT_TYPE s = 0; s < N; ++s) {
                if (nnodes[s] >= data.myParallelThreshold) {
                    continue;
                }
                const INT_TYPE node_int = node.child[s];
                if (Node::isInternal(node_int)) {
                    if (node_int == Node::EMPTY) {
                        // NOTE: Anything after this will be empty too, so we can break.
                        break;
                    }
                    // next_node_id isn't needed for serial case
                    utCreateBVHInterleavedBoxesHelper<T,NAXES,false,INT_TYPE0>(Node::getInternalNum(node_int), INT_TYPE(-1), data, &local_data[s]);
                }
                else {
                    item_functor(data, local_data[s], s, nodei, node_int);
                }
            }
        }
        // Now do the parallel ones
        UTparallelFor(UT_BlockedRange<INT_TYPE>(0,nparallel), [&node,&nnodes,&next_nodes,&data,&local_data](const UT_BlockedRange<INT_TYPE>& r) {
            for (INT_TYPE taski = r.begin(); taski < r.end(); ++taski) {
                INT_TYPE parallel_count = 0;
                // NOTE: The check for s < N is just so that the compiler can
                //       (hopefully) figure out that it can fully unroll the loop.
                INT_TYPE s;
                for (s = 0; s < N; ++s) {
                    if (nnodes[s] < data.myParallelThreshold) {
                        continue;
                    }
                    if (parallel_count == taski) {
                        break;
                    }
                    ++parallel_count;
                }
                const INT_TYPE node_int = node.child[s];
                UT_ASSERT_MSG_P(Node::isInternal(node_int), "item_functor is fairly fast, so we don't support having leaves run in parallel, unlike traverseParallel");
                UT_ASSERT_MSG_P(node_int != Node::EMPTY, "Empty entries should have been excluded above.");
                utCreateBVHInterleavedBoxesHelper<T,NAXES,true,INT_TYPE0>(Node::getInternalNum(node_int), next_nodes[s], data, &local_data[s]);
            }
        }, 0, 1);
    }
    else {
        // All in serial
        for (INT_TYPE s = 0; s < N; ++s) {
            const INT_TYPE node_int = node.child[s];
            if (Node::isInternal(node_int)) {
                if (node_int == Node::EMPTY) {
                    // NOTE: Anything after this will be empty too, so we can break.
                    break;
                }
                // next_node_id isn't needed for serial case
                utCreateBVHInterleavedBoxesHelper<T,NAXES,false,INT_TYPE0>(Node::getInternalNum(node_int), INT_TYPE(-1), data, &local_data[s]);
            }
            else {
                item_functor(data, local_data[s], s, nodei, node_int);
            }
        }
    }

    UT::Box<T,NAXES> box(local_data[0]);
    for (int i = 1; i < nchildren; ++i)
        box.combine(local_data[i]);

    if (data_for_parent)
        *data_for_parent = box;

    auto &current_node_data = data.myNodeData[nodei];

    SYS_STATIC_ASSERT(sizeof(current_node_data) == NAXES*2*4*sizeof(T));
    SYS_STATIC_ASSERT(sizeof(current_node_data.vals[0][0]) == 4*sizeof(T));
    for (int i = 0; i < nchildren; ++i)
    {
        const UT::Box<T,NAXES> &local_box = local_data[i];
        for (int axis = 0; axis < NAXES; ++axis)
        {
            ((T*)&current_node_data.vals[axis][0])[i] = local_box.vals[axis][0];
            ((T*)&current_node_data.vals[axis][1])[i] = local_box.vals[axis][1];
        }
    }
    for (int i = nchildren; i < N; ++i)
    {
        // Set to invalid, to avoid intersecting any of them.
        for (int axis = 0; axis < NAXES; ++axis)
        {
            // Using max and -max instead of intinity and -infinity,
            // just in case something needs to compute the centre of a potential box,
            // since max-max is zero, but infinity-infinity is NaN.
            ((T*)&current_node_data.vals[axis][0])[i] = std::numeric_limits<T>::max();
            ((T*)&current_node_data.vals[axis][1])[i] = -std::numeric_limits<T>::max();
        }
    }
}

template<uint NAXES,typename T,typename ITEM_BOX,typename NODE_BOX,typename INT_TYPE0>
SYS_FORCE_INLINE void createBVHInterleavedBoxes(
    const UT::BVH<4>& bvh,
    const ITEM_BOX* item_boxes,
    NODE_BOX* node_boxes,
    const v4uu* node_nitems,
    const INT_TYPE0* indices_mapping) noexcept
{
    if (!bvh.getNodes())
        return;

    static constexpr uint PARALLEL_THRESHOLD = 4096;
    struct NodeBoxData
    {
        const UT::BVH<4>::Node *myRoot;
        uint myParallelThreshold;
        const INT_TYPE0* myIndices;
        const ITEM_BOX* myItemBoxes;
        const v4uu* myNodeNItems;
        NODE_BOX *myNodeData;
    };

    NodeBoxData data;
    data.myRoot = bvh.getNodes();
    data.myParallelThreshold = PARALLEL_THRESHOLD;
    data.myIndices = indices_mapping;
    data.myItemBoxes = item_boxes;
    data.myNodeNItems = node_nitems;
    data.myNodeData = node_boxes;

    UT::Box<T,NAXES> box;
    utCreateBVHInterleavedBoxesHelper<T,NAXES,true,INT_TYPE0>(uint(0), bvh.getNumNodes(), data, &box);
}

template<uint NAXES,typename INT_TYPE>
void getIntersectingBoxes(
    const UT::BVH<4>& bvh,
    const UT::Box<v4uf,NAXES>* node_boxes,
    const UT::Box<float,NAXES>& query_box,
    UT_Array<INT_TYPE>& box_indices,
    BVHUnorderedStack& stack) noexcept
{
    const uint nnodes = bvh.getNumNodes();

    if (nnodes == 0)
    {
        // Nothing to hit
        return;
    }

    // NOTE: Although we don't clear out box_indices, we DO clear out stack, just in case.
    stack.clear();
    stack.append(0);

    getIntersectingBoxesFromStack(bvh, node_boxes, query_box, box_indices, stack);
}

template<uint NAXES,typename INT_TYPE>
void getIntersectingBoxesFromStack(
    const UT::BVH<4>& bvh,
    const UT::Box<v4uf,NAXES>* node_boxes,
    const UT::Box<float,NAXES>& query_box,
    UT_Array<INT_TYPE>& box_indices,
    BVHUnorderedStack& stack) noexcept
{
    using NodeType = UT_BVH<4>::Node;
    using BoxType = UT::Box<v4uf,NAXES>;
    const NodeType *nodes = bvh.getNodes();

    auto stack_ptr = stack.data();
    exint stack_capacity = stack.capacity();
    exint stack_entries = stack.entries();

    auto box_ptr = box_indices.data();
    auto box_end = box_ptr + box_indices.capacity();
    box_ptr += box_indices.entries();

    const BoxType query_boxes(query_box);

    do
    {
	stack_entries--;
        uint current_node = stack_ptr[stack_entries];

        while (true)
        {
            const BoxType &box = node_boxes[current_node];

            BoxType intersection;
            box.intersect(query_boxes, intersection);
            // NOTE: DO NOT change this to <, else axis-aligned polygons could be missed.
            v4uu hit_mask = (intersection[0][0] <= intersection[0][1]);
            for (uint axis = 1; axis < NAXES; ++axis)
            {
                hit_mask &= (intersection[axis][0] <= intersection[axis][1]);
            }
            uint hit_bits = _mm_movemask_ps(V4SF(hit_mask.vector));
            if (hit_bits == 0)
                break;
            const NodeType &node = nodes[current_node];
            uint local_index = SYSfirstBitSet(hit_bits);
            current_node = node.child[local_index];
            if (!(hit_bits & (hit_bits-1)))
            {
                // Only 1 bit set
                if (NodeType::isInternal(current_node))
                {
                    current_node = NodeType::getInternalNum(current_node);
                    continue;
                }
		{
		    if (box_ptr >= box_end)
		    {
			box_indices.setSizeNoInit(box_ptr-box_indices.data());
			box_indices.bumpCapacity(box_end-box_indices.data()+1);

			box_ptr = box_indices.data();
			box_end = box_ptr + box_indices.capacity();
			box_ptr += box_indices.entries();
		    }
		    *box_ptr++ = current_node;
		}
                break;
            }
            uint ninternal;
            if (NodeType::isInternal(current_node))
            {
                ninternal = 1;
                current_node = NodeType::getInternalNum(current_node);
            }
            else
            {
                ninternal = 0;
		{
		    if (box_ptr >= box_end)
		    {
			box_indices.setSizeNoInit(box_ptr-box_indices.data());
			box_indices.bumpCapacity(box_end-box_indices.data()+1);

			box_ptr = box_indices.data();
			box_end = box_ptr + box_indices.capacity();
			box_ptr += box_indices.entries();
		    }
		    *box_ptr++ = current_node;
		}
            }
            // NOTE: -2 has only bit 0 clear; this removes the lowest set bit.
            hit_bits &= (uint(-2)<<local_index);

            do
            {
                local_index = SYSfirstBitSet(hit_bits);
                uint local_node = node.child[local_index];
                if (NodeType::isInternal(local_node))
                {
                    local_node = NodeType::getInternalNum(local_node);
                    if (!ninternal)
                        current_node = local_node;
                    else
		    {
			if (stack_entries >= stack_capacity)
			{
			    stack.setSizeNoInit(stack_entries);
			    stack.bumpCapacity(stack_capacity+1);
			    stack_capacity = stack.capacity();
			    stack_ptr = stack.data();
			}
			stack_ptr[stack_entries++] = local_node;
		    }
                    ++ninternal;
                }
                else
                {
		    if (box_ptr >= box_end)
		    {
			box_indices.setSizeNoInit(box_ptr-box_indices.data());
			box_indices.bumpCapacity(box_end-box_indices.data()+1);

			box_ptr = box_indices.data();
			box_end = box_ptr + box_indices.capacity();
			box_ptr += box_indices.entries();
		    }
		    *box_ptr++ = local_node;
                }

                // NOTE: -2 has only bit 0 clear; this removes the lowest set bit.
                hit_bits &= (uint(-2)<<local_index);
            } while (hit_bits);

            // If there were no internal nodes, current_node isn't valid, so we
            // have to break to get a new node from the stack.
            if (!ninternal)
                break;
        }
    } while (stack_entries);

    box_indices.setSizeNoInit(box_ptr - box_indices.data());
}

template<uint NAXES,typename INT_TYPE>
void getIntersectingNodes(
    const UT::BVH<4>& bvh,
    const UT::Box<v4uf,NAXES>* node_boxes,
    const UT::Box<float,NAXES>& query_box,
    UT_Array<INT_TYPE>& node_indices,
    BVHUnorderedStack& stack) noexcept
{
    using NodeType = UT_BVH<4>::Node;
    using BoxType = UT::Box<v4uf,NAXES>;
    const NodeType *nodes = bvh.getNodes();
    const uint nnodes = bvh.getNumNodes();

    if (nnodes == 0)
    {
        // Nothing to hit
        return;
    }

    const BoxType query_boxes(query_box);

    v4uf queryboxsize = v4uf(query_box.axis_sum());

    // NOTE: Although we don't clear out node_indices, we DO clear out stack, just in case.

    auto stack_ptr = stack.data();
    exint stack_capacity = stack.capacity();
    exint stack_entries = 0;

    if (stack_entries >= stack_capacity)
    {
	stack.bumpCapacity(stack_capacity+1);
	stack_capacity = stack.capacity();
	stack_ptr = stack.data();
    }
    stack_ptr[stack_entries++] = 0;

    auto box_ptr = node_indices.data();
    auto box_end = box_ptr + node_indices.capacity();
    box_ptr += node_indices.entries();

    do
    {
	stack_entries--;
        uint current_node = stack_ptr[stack_entries];

        while (true)
        {
            const NodeType &node = nodes[current_node];
	    v4uu  nodechild = *((const v4uu *)node.child);

	    int   internalmask = signbits(nodechild);

	    if (internalmask != 0xf)
	    {
		// We have an external item, so leave for
		// the next pass.
		if (box_ptr >= box_end)
		{
		    node_indices.setSizeNoInit(box_ptr-node_indices.data());
		    node_indices.bumpCapacity(box_end-node_indices.data()+1);

		    box_ptr = node_indices.data();
		    box_end = box_ptr + node_indices.capacity();
		    box_ptr += node_indices.entries();
		}
		*box_ptr++ = current_node;
		break;
	    }

	    // Now we know all our children are empty or internal,
	    // simplyfing the following tests.
            const BoxType &box = node_boxes[current_node];

	    // Determine the size of the boxes.
	    v4uf	boxsize = box.axis_sum();
	    int		negative = vm_signbits(boxsize.vector);
	    int 	bigenough = signbits(boxsize > queryboxsize);
	    if ((negative | bigenough) != 0xf)
	    {
		// Our box has go smaller than our query box, it
		// could be our source is really big but we should
		// break out of batch mode regardless.
		if (box_ptr >= box_end)
		{
		    node_indices.setSizeNoInit(box_ptr-node_indices.data());
		    node_indices.bumpCapacity(box_end-node_indices.data()+1);

		    box_ptr = node_indices.data();
		    box_end = box_ptr + node_indices.capacity();
		    box_ptr += node_indices.entries();
		}
		*box_ptr++ = current_node;
		break;
	    }

            BoxType intersection;
            box.intersect(query_boxes, intersection);
            // NOTE: DO NOT change this to <, else axis-aligned polygons could be missed.
            v4uu hit_mask = (intersection[0][0] <= intersection[0][1]);
            for (uint axis = 1; axis < NAXES; ++axis)
            {
                hit_mask &= (intersection[axis][0] <= intersection[axis][1]);
            }
            uint hit_bits = signbits(hit_mask);
            if (hit_bits == 0)
                break;

            uint local_index = SYSfirstBitSet(hit_bits);
            current_node = node.child[local_index];
	    current_node = NodeType::getInternalNum(current_node);
            if (!(hit_bits & (hit_bits-1)))
            {
                // Only 1 bit set
		continue;
            }

            // NOTE: -2 has only bit 0 clear; this removes the lowest set bit.
            hit_bits &= (uint(-2)<<local_index);

            do
            {
                local_index = SYSfirstBitSet(hit_bits);
                uint local_node = node.child[local_index];
		local_node = NodeType::getInternalNum(local_node);

		if (stack_entries >= stack_capacity)
		{
		    stack.setSizeNoInit(stack_entries);
		    stack.bumpCapacity(stack_capacity+1);
		    stack_capacity = stack.capacity();
		    stack_ptr = stack.data();
		}
		stack_ptr[stack_entries++] = local_node;

                // NOTE: -2 has only bit 0 clear; this removes the lowest set bit.
                hit_bits &= (uint(-2)<<local_index);
            } while (hit_bits);

	    // There must have been internal nodes as we skipped
	    // if there were any external.
        }
    } while (stack_entries);

    node_indices.setSizeNoInit(box_ptr - node_indices.data());
}

template<uint NAXES,typename INT_TYPE, int BATCH_SIZE>
void getIntersectingBoxesBatch(
    const UT::BVH<4>& bvh,
    const UT::Box<v4uf,NAXES>* node_boxes,
    const UT::Box<float,NAXES> *query_box,
    UT_Array<INT_TYPE> *box_indices,
    BVHUnorderedStack& stack) noexcept
{
    using NodeType = UT_BVH<4>::Node;
    using BoxType = UT::Box<v4uf,NAXES>;
    const NodeType *nodes = bvh.getNodes();
    const uint nnodes = bvh.getNumNodes();

    if (nnodes == 0)
    {
        // Nothing to hit
        return;
    }

    BoxType query_boxes[BATCH_SIZE];
    for (int i = 0; i < BATCH_SIZE; i++)
	query_boxes[i] = query_box[i];

    // NOTE: Although we don't clear out box_indices, we DO clear out stack, just in case.
    stack.setSize(0);
    stack.append(0);
    do
    {
        uint current_node = stack.last();
        stack.removeLast();

        while (true)
        {
            const BoxType &box = node_boxes[current_node];

	    uint		local_hit_bits[BATCH_SIZE];
	    uint		hit_bits = 0;

	    for (int i = 0; i < BATCH_SIZE; i++)
	    {
		BoxType intersection;
		box.intersect(query_boxes[i], intersection);
		// NOTE: DO NOT change this to <, else axis-aligned polygons could be missed.
		v4uu hit_mask = (intersection[0][0] <= intersection[0][1]);
		for (uint axis = 1; axis < NAXES; ++axis)
		{
		    hit_mask &= (intersection[axis][0] <= intersection[axis][1]);
		}
		local_hit_bits[i] = signbits(hit_mask);
		hit_bits |= local_hit_bits[i];
	    }
            
	    if (hit_bits == 0)
                break;

            const NodeType &node = nodes[current_node];
            uint local_index = SYSfirstBitSet(hit_bits);
            current_node = node.child[local_index];
            if (!(hit_bits & (hit_bits-1)))
            {
                // Only 1 bit set
                if (NodeType::isInternal(current_node))
                {
                    current_node = NodeType::getInternalNum(current_node);
                    continue;
                }
		// Write out each batch that matches.  Only one bit
		// set overall, so only need to do zero test.
		for (int i = 0; i < BATCH_SIZE; i++)
		{
		    if (local_hit_bits[i])
			box_indices[i].append(current_node);
		}
                break;
            }
            uint ninternal;
            if (NodeType::isInternal(current_node))
            {
                ninternal = 1;
                current_node = NodeType::getInternalNum(current_node);
            }
            else
            {
                ninternal = 0;
		uint curbit = 1 << local_index;
		for (int i = 0; i < BATCH_SIZE; i++)
		{
		    if (local_hit_bits[i] & curbit)
			box_indices[i].append(current_node);
		}
            }
            // NOTE: -2 has only bit 0 clear; this removes the lowest set bit.
            hit_bits &= (uint(-2)<<local_index);

            do
            {
                local_index = SYSfirstBitSet(hit_bits);
                uint local_node = node.child[local_index];
                if (NodeType::isInternal(local_node))
                {
                    local_node = NodeType::getInternalNum(local_node);
                    if (!ninternal)
                        current_node = local_node;
                    else
                        stack.append(local_node);
                    ++ninternal;
                }
                else
                {
		    uint curbit = 1 << local_index;
		    for (int i = 0; i < BATCH_SIZE; i++)
		    {
			if (local_hit_bits[i] & curbit)
			    box_indices[i].append(local_node);
		    }
                }

                // NOTE: -2 has only bit 0 clear; this removes the lowest set bit.
                hit_bits &= (uint(-2)<<local_index);
            } while (hit_bits);

            // If there were no internal nodes, current_node isn't valid, so we
            // have to break to get a new node from the stack.
            if (!ninternal)
                break;
        }
    } while (!stack.isEmpty());
}

template<bool PARALLEL,typename DATA>
void utComputeNodeNItemsHelper(
    uint nodei,
    uint next_node_id,
    uint next_item_id,
    const DATA &data) noexcept
{
    const auto &node = data.myNodes[nodei];

    static constexpr uint N = 4;
    using Node = typename UT::BVH<N>::Node;
    using INT_TYPE = typename UT::BVH<N>::INT_TYPE;

    // Process the current node first
    int32* current_counts = (int32*)(data.myNodeData+nodei);
    uint next_items[4];
    for (INT_TYPE i = 0; i < N-1; ++i) {
        if (current_counts[i] < 0) {
            current_counts[i] = 0;
        }
        else if (current_counts[i+1] < 0) {
            current_counts[i] = next_item_id-current_counts[i];
            next_items[i] = next_item_id;
        }
        else {
            current_counts[i] = current_counts[i+1]-current_counts[i];
            next_items[i] = current_counts[i+1];
        }
    }
    if (current_counts[N-1] < 0) {
        current_counts[N-1] = 0;
    }
    else {
        current_counts[N-1] = next_item_id-current_counts[N-1];
        next_items[N-1] = next_item_id;
    }

    // To determine the number of nodes in a child's subtree, we take the next
    // node ID minus the current child's node ID.
    INT_TYPE next_nodes[N];
    INT_TYPE nnodes[N];
    INT_TYPE nchildren = N;
    INT_TYPE nparallel = 0;
    if (PARALLEL)
    {
        // s is currently unsigned, so we check s < N for bounds check.
        // The s >= 0 check is in case s ever becomes signed, and should be
        // automatically removed by the compiler for unsigned s.
        for (INT_TYPE s = N-1; (std::is_signed<INT_TYPE>::value ? (s >= 0) : (s < N)); --s) {
            const INT_TYPE node_int = node.child[s];
            if (node_int == Node::EMPTY) {
                --nchildren;
                continue;
            }
            next_nodes[s] = next_node_id;
            if (Node::isInternal(node_int)) {
                // NOTE: This depends on BVH<N>::initNode appending the child nodes
                //       in between their content, instead of all at once.
                INT_TYPE child_node_id = Node::getInternalNum(node_int);
                nnodes[s] = next_node_id - child_node_id;
                next_node_id = child_node_id;
            }
            else {
                nnodes[s] = 0;
            }
            nparallel += (nnodes[s] >= data.myParallelThreshold);
        }
    }

    if (nparallel >= 2) {
        // Do any non-parallel ones first
        if (nparallel < nchildren) {
            for (INT_TYPE s = 0; s < N; ++s) {
                if (nnodes[s] >= data.myParallelThreshold) {
                    continue;
                }
                const INT_TYPE node_int = node.child[s];
                if (Node::isInternal(node_int)) {
                    if (node_int == Node::EMPTY) {
                        // NOTE: Anything after this will be empty too, so we can break.
                        break;
                    }
                    // next_node_id isn't needed for serial case
                    utComputeNodeNItemsHelper<false>(Node::getInternalNum(node_int), INT_TYPE(-1), next_items[s], data);
                }
            }
        }
        // Now do the parallel ones
        UTparallelFor(UT_BlockedRange<INT_TYPE>(0,nparallel), [&node,&nnodes,&next_nodes,&next_items,&data](const UT_BlockedRange<INT_TYPE>& r) {
            for (INT_TYPE taski = r.begin(); taski < r.end(); ++taski) {
                INT_TYPE parallel_count = 0;
                // NOTE: The check for s < N is just so that the compiler can
                //       (hopefully) figure out that it can fully unroll the loop.
                INT_TYPE s;
                for (s = 0; s < N; ++s) {
                    if (nnodes[s] < data.myParallelThreshold) {
                        continue;
                    }
                    if (parallel_count == taski) {
                        break;
                    }
                    ++parallel_count;
                }
                const INT_TYPE node_int = node.child[s];
                UT_ASSERT_MSG_P(Node::isInternal(node_int), "item_functor is fairly fast, so we don't support having leaves run in parallel, unlike traverseParallel");
                UT_ASSERT_MSG_P(node_int != Node::EMPTY, "Empty entries should have been excluded above.");
                utComputeNodeNItemsHelper<true>(Node::getInternalNum(node_int), next_nodes[s], next_items[s], data);
            }
        }, 0, 1);
    }
    else {
        // All in serial
        for (INT_TYPE s = 0; s < N; ++s) {
            const INT_TYPE node_int = node.child[s];
            if (Node::isInternal(node_int)) {
                if (node_int == Node::EMPTY) {
                    // NOTE: Anything after this will be empty too, so we can break.
                    break;
                }
                // next_node_id isn't needed for serial case
                utComputeNodeNItemsHelper<false>(Node::getInternalNum(node_int), INT_TYPE(-1), next_items[s], data);
            }
        }
    }
}

inline void computeNodeNItems(
    const UT::BVH<4>& bvh,
    v4uu* node_nitems,
    exint nitems) noexcept
{
    // First, fill in item number starts.

    struct ItemStartFunctors
    {
        v4uu* myNodeData;

	ItemStartFunctors() = default;

        SYS_FORCE_INLINE ItemStartFunctors(v4uu *node_data)
            : myNodeData(node_data)
        {}
        constexpr SYS_FORCE_INLINE bool pre(const int nodei, int32 *data_for_parent) const
        {
            return true;
        }
        void SYS_FORCE_INLINE item(const int itemi, const int parent_nodei, int32 &data_for_parent) const
        {
            data_for_parent = itemi;
        }

        void post(const int nodei, const int parent_nodei, int32 *data_for_parent, const int nchildren, const int32 *child_data_array) const
        {
            if (data_for_parent)
                *data_for_parent = child_data_array[0];

            SYS_STATIC_ASSERT(sizeof(v4uu) == 4*sizeof(int32));
            int32 *current_node_data = (int32*)&myNodeData[nodei];

            current_node_data[0] = child_data_array[0];
            int i = 1;
            for (; i < nchildren; ++i)
                current_node_data[i] = child_data_array[i];
            for (; i < 4; ++i)
                current_node_data[i] = -1;
        }
    };

    constexpr uint PARALLEL_THRESHOLD = 4096;
    ItemStartFunctors functors(node_nitems);
    bvh.template traverseParallel<int32>(PARALLEL_THRESHOLD, functors);

    // Then, convert them to counts.

    struct ItemCountData {
        uint myParallelThreshold;
        const UT::BVH<4>::Node* myNodes;
        v4uu* myNodeData;
    };
    ItemCountData data;
    data.myParallelThreshold = PARALLEL_THRESHOLD;
    data.myNodes = bvh.getNodes();
    data.myNodeData = node_nitems;
    utComputeNodeNItemsHelper<true>(0, bvh.getNumNodes(), nitems, data);
}

template<bool farthest,typename INT_TYPE0>
class ut_BVHOrderedStackCompare
{
public:
    SYS_FORCE_INLINE
    ut_BVHOrderedStackCompare(const INT_TYPE0* indices_mapping) noexcept
        : myIndicesMapping(indices_mapping)
    {
        UT_ASSERT_MSG_P(indices_mapping, "Use std::less or std::greater if not mapping.");
    }

    SYS_FORCE_INLINE
    bool operator()(const BVHOrderedStackEntry& a,const BVHOrderedStackEntry& b) const noexcept {
        if (a.myDistSquared != b.myDistSquared) {
            return (!farthest)
                ? (a.myDistSquared < b.myDistSquared)
                : (a.myDistSquared > b.myDistSquared);
        }

        // Break tie using mapping
        return (!farthest)
            ? (myIndicesMapping[a.myNode] < myIndicesMapping[b.myNode])
            : (myIndicesMapping[a.myNode] > myIndicesMapping[b.myNode]);
    }
private:
    const INT_TYPE0* myIndicesMapping;
};

template<bool farthest,bool use_max_points,uint NAXES,bool INSTANTIATED,typename INT_TYPE0,typename POSITION_ARRAY,typename RADIUS_ARRAY>
SYS_FORCE_INLINE void utHandleSingleClosePoint(
    const uint index,
    const INT_TYPE0* indices_mapping,
    const POSITION_ARRAY& positions,
    const UT_FixedVector<float,NAXES,INSTANTIATED>& query_point,
    BVHOrderedStack& output_queue,
    const RADIUS_ARRAY& radii,
    exint max_points,
    float& max_dist_squared) noexcept
{
    using StackEntry = BVHOrderedStackEntry;

    // Points as spheres
    INT_TYPE0 ptoff;
    if (indices_mapping)
        ptoff = indices_mapping[index];
    else
        ptoff = INT_TYPE0(index);
    float d2 = query_point.distance2(positions[ptoff]);
    if (!allRadiiZero(radii))
    {
        float radius = radii[ptoff];
        if (radius != 0)
        {
            float distance = SYSsqrt(d2) - SYSabs(radius);
            d2 = distance*distance;
            d2 = (distance < 0) ? -d2 : d2;
        }
    }
    if ((!farthest) ? (d2 > max_dist_squared) : (d2 < max_dist_squared))
        return;

    if (!use_max_points || output_queue.size() < max_points) {
        output_queue.append(StackEntry(d2, index));
        if (use_max_points && output_queue.size() == max_points) {
            if (max_points > 1) {
                if (!farthest) {
                    // Max heap, since we'll want to remove the max dist first
                    if (indices_mapping) {
                        ut_BVHOrderedStackCompare<false,INT_TYPE0> comparator(indices_mapping);
                        std::make_heap(output_queue.data(), output_queue.data()+output_queue.size(), comparator);
                    }
                    else {
                        std::make_heap(output_queue.data(), output_queue.data()+output_queue.size());
                    }
                }
                else {
                    // Min heap, since we'll want to remove the min dist first
                    if (indices_mapping) {
                        ut_BVHOrderedStackCompare<true,INT_TYPE0> comparator(indices_mapping);
                        std::make_heap(output_queue.data(), output_queue.data()+output_queue.size(), comparator);
                    }
                    else {
                        std::make_heap(output_queue.data(), output_queue.data()+output_queue.size(), std::greater<StackEntry>());
                    }
                }
                max_dist_squared = output_queue[0].myDistSquared;
            }
            else {
                max_dist_squared = d2;
            }
        }
    }
    else if (max_points == 1) {
        bool select_new = true;
        if (d2 == max_dist_squared)
        {
            // Break tie using external index.
            INT_TYPE0 other_ptoff;
            if (indices_mapping)
                other_ptoff = indices_mapping[output_queue[0].myNode];
            else
                other_ptoff = INT_TYPE0(output_queue[0].myNode);
            select_new = (!farthest) ? (ptoff < other_ptoff) : (ptoff > other_ptoff);
        }
        if (select_new)
        {
            output_queue[0].myDistSquared = d2;
            output_queue[0].myNode = index;
            max_dist_squared = d2;
        }
    }
    else {
        // Already at max_points.
        // Remove the index with the largest distance2,
        // then set max_dist_squared to the next largest distance2.
        auto start = output_queue.data();
        auto end = output_queue.data()+output_queue.size();
        if (!farthest) {
            if (indices_mapping) {
                // Break ties with the max correctly, the way the comparator would
                if (d2 == max_dist_squared && indices_mapping[start->myNode] < indices_mapping[index])
                    return;

                ut_BVHOrderedStackCompare<false,INT_TYPE0> comparator(indices_mapping);
                std::pop_heap(start, end, comparator);
                end[-1].myDistSquared = d2;
                end[-1].myNode = index;
                std::push_heap(start, end, comparator);
            }
            else {
                // Break ties with the max correctly
                if (d2 == max_dist_squared && start->myNode < index)
                    return;

                std::pop_heap(start, end);
                end[-1].myDistSquared = d2;
                end[-1].myNode = index;
                std::push_heap(start, end);
            }
        }
        else {
            if (indices_mapping) {
                // Break ties with the max (min) correctly, the way the comparator would
                if (d2 == max_dist_squared && indices_mapping[start->myNode] > indices_mapping[index])
                    return;

                ut_BVHOrderedStackCompare<true,INT_TYPE0> comparator(indices_mapping);
                std::pop_heap(start, end, comparator);
                end[-1].myDistSquared = d2;
                end[-1].myNode = index;
                std::push_heap(start, end, comparator);
            }
            else {
                // Break ties with the max (min) correctly
                if (d2 == max_dist_squared && start->myNode > index)
                    return;

                std::pop_heap(start, end, std::greater<StackEntry>());
                end[-1].myDistSquared = d2;
                end[-1].myNode = index;
                std::push_heap(start, end, std::greater<StackEntry>());
            }
        }

        max_dist_squared = start[0].myDistSquared;
    }
}

template<bool farthest,bool reordered,bool use_max_points,uint NAXES,bool INSTANTIATED,typename INT_TYPE0,typename POSITION_ARRAY,typename RADIUS_ARRAY>
void findClosestPoints(
    const UT::BVH<4>& bvh,
    const UT::Box<v4uf,NAXES>* node_boxes,
    const v4uu* node_nitems,
    const INT_TYPE0* indices_mapping,
    const POSITION_ARRAY& positions,
    const UT_FixedVector<float,NAXES,INSTANTIATED>& query_point,
    BVHOrderedStack& stack,
    BVHOrderedStack& output_queue,
    const RADIUS_ARRAY& radii,
    exint max_points,
    float max_dist_squared) noexcept
{
    using NodeType = UT_BVH<4>::Node;
    using BoxType = UT::Box<v4uf,NAXES>;
    const NodeType* nodes = bvh.getNodes();
    const uint nnodes = bvh.getNumNodes();

    if (nnodes == 0 || (use_max_points && max_points<=0))
    {
        // Nothing to hit
        return;
    }

    UT_ASSERT_P(!reordered || node_nitems);

    using StackEntry = BVHOrderedStackEntry;

    UT_FixedVector<v4uf,NAXES> vorigin;
    for (uint axis = 0; axis < NAXES; ++axis)
    {
        vorigin[axis] = v4uf(query_point[axis]);
    }

    // Be careful, because this is a fairly big buffer on the stack,
    // and there's the potential to recurse into packed primitives.
    // If we have deeply nested packed primitives, that could be an issue.
    stack.setSize(0);
    stack.append(StackEntry((!farthest) ? 0 : std::numeric_limits<float>::max(),reordered ? 0 : NodeType::markInternal(0)));

    do
    {
        StackEntry entry = stack.last();
        stack.removeLast();

        uint next_node = entry.myNode;
        // When farthest is true, max_dist_squared is actually a minimum distance squared.
        // FIXME: Compare the node index for stability if d2 == max_dist_squared
        if ((!farthest) ? (entry.myDistSquared > max_dist_squared) : (entry.myDistSquared < max_dist_squared))
            continue;

        while (true)
        {
            if (reordered || NodeType::isInternal(next_node))
            {
                // It's very unlikely that we'd have an empty item here,
                // but if the query point is VERY far away, it could happen
                // that it's closer to the empty box than max_dist_squared.
                if (next_node == (reordered ? NodeType::getInternalNum(NodeType::EMPTY) : NodeType::EMPTY))
                    break;

                if (!reordered)
                    next_node = NodeType::getInternalNum(next_node);
                const BoxType &box = node_boxes[next_node];
                v4uf dist2 = (!farthest) ? box.minDistance2(vorigin) : box.maxDistance2(vorigin);
                v4uu hit_mask = (!farthest) ? (dist2 <= max_dist_squared) : (dist2 >= max_dist_squared);
                uint hit_bits = signbits(hit_mask);
                if (hit_bits == 0)
                    break;
                const NodeType &node = nodes[next_node];

                exint stack_size = stack.size();
                StackEntry *stack_last = stack.data()+stack_size-1;

                if (reordered)
                {
                    union { v4sf d2v; float d2s[4]; };
                    d2v = dist2.vector;
                    uint cur_node = next_node;
                    next_node = uint(-1);
                    float d20;
                    do
                    {
                        uint local_index = SYSfirstBitSet(hit_bits);
                        uint index = node.child[local_index];

                        if (!NodeType::isInternal(index))
                        {
                            const uint count = ((const uint32*)(node_nitems+cur_node))[local_index];
                            for (uint i = 0; i < count; ++i, ++index)
                            {
                                utHandleSingleClosePoint<farthest,use_max_points,NAXES>(
                                    index, indices_mapping, positions, query_point,
                                    output_queue, radii, max_points, max_dist_squared);
                            }
                        }
                        else if (next_node == uint(-1))
                        {
                            next_node = NodeType::getInternalNum(index);
                            d20 = d2s[local_index];
                        }
                        else
                        {
                            // Already have at least one other node from this parent
                            uint child1 = NodeType::getInternalNum(index);
                            float d21 = d2s[local_index];
                            if ((!farthest) ? (d20 > d21) : (d20 < d21))
                            {
                                uint childtmp = next_node;
                                next_node = child1;
                                child1 = childtmp;
                                float tmp = d20;
                                d20 = d21;
                                d21 = tmp;
                            }

                            if (!stack_size || ((!farthest) ? (stack_last->myDistSquared >= d20) : (stack_last->myDistSquared <= d20)))
                            {
                                // Inserting d21
                                // Check end of stack first, since it's most likely that the box will go near the end.
                                if (!stack_size || ((!farthest) ? (stack_last->myDistSquared >= d21) : (stack_last->myDistSquared <= d21)))
                                {
                                    stack.append(StackEntry(d21, child1));
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
                                            stack.insert(StackEntry(d21, child1), i+1);
                                            break;
                                        }
                                    }
                                    if (i < limiti)
                                    {
                                        stack.insert(StackEntry(d21, child1), i+1);
                                    }
                                }
                            }
                            else
                            {
                                StackEntry entry = stack.last();
                                stack_last->myNode = child1;
                                stack_last->myDistSquared = d21;
                                stack.append(StackEntry(d20, next_node));
                                next_node = entry.myNode;
                                d20 = entry.myDistSquared;
                            }
                            stack_last = stack.data()+stack_size;
                            ++stack_size;
                        }

                        hit_bits &= (uint(-2)<<local_index);
                    } while (hit_bits != 0);

                    if (next_node == uint(-1))
                    {
                        break;
                    }
                    continue;
                }

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
                // When farthest is true, these are max distances squared.
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
                            stack.append(StackEntry(d21, child1));
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
                                    stack.insert(StackEntry(d21, child1), i+1);
                                    break;
                                }
                            }
                            if (i < limiti)
                            {
                                stack.insert(StackEntry(d21, child1), i+1);
                            }
                        }
                    }
                    else
                    {
                        next_node = stack_last->myNode;
                        stack_last->myNode = child1;
                        stack_last->myDistSquared = d21;
                        stack.append(StackEntry(d20, child0));
                    }
                    continue;
                }

                // 3-4 bits set
                uint nhit = (hit_bits == 0xF) ? 4 : 3;
                uint local_index2 = third_bit_index[hit_bits];
                uint children[4] = {
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
                        stack.append(StackEntry(d2, child));
                    }
                    else
                    {
                        // Insert in sorted order, but go back at most a constant
                        // number of entries, to avoid n^2 behaviour.
                        exint i;
                        exint limiti = SYSmax(0, stack_size-64);
                        for (i = stack_size-2; i >= limiti; --i)
                        {
                            if ((!farthest) ? (stack[i].myDistSquared <= d2) : (stack[i].myDistSquared >= d2))
                            {
                                stack.insert(StackEntry(d2, child), i+1);
                                break;
                            }
                        }
                        if (i < limiti)
                        {
                            stack.insert(StackEntry(d2, child), i+1);
                        }
                    }
                    stack_last = stack.data()+stack_size;
                    ++stack_size;
                }
                continue;
            }

            // Leaf
            uint index = next_node;
            utHandleSingleClosePoint<farthest,use_max_points,NAXES>(
                index, indices_mapping, positions, query_point,
                output_queue, radii, max_points, max_dist_squared);
            break;
        }
    } while (!stack.isEmpty());
}

} // UT namespace

#undef BVH_TRY_ALL_AXES

#endif
