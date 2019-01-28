/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_VolumeUtil.h (GT Library, C++)
 *
 * COMMENTS:	Shared utilities between GT_PrimVDB and GT_PrimVolume
 */

#ifndef __GT_VOLUMEUTIL_H_INCLUDED__
#define __GT_VOLUMEUTIL_H_INCLUDED__

#include "GT_API.h"
#include "GT_DANumeric.h"
#include "GT_RefineParms.h"

#include <UT/UT_Assert.h>
#include <UT/UT_Task.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_VectorTypes.h>
#include <SYS/SYS_MathCbrt.h>
#include <SYS/SYS_Types.h>


/// Utility function for determining whether we should resample volumes in GT.
inline bool
GTvolumeNeedResample(
	int xres, int yres, int zres,
	const GT_RefineParms *parms,
	fpreal &scale_factor)
{
    const fpreal max_lod = 4.0;
    const fpreal lod = SYSmin(GT_RefineParms::getLOD(parms), max_lod);

    const exint min_dim = 64;
    const fpreal min_voxels = min_dim * min_dim * min_dim * lod; 
    const fpreal num_voxels = exint(xres) * yres * zres;

    UT_ASSERT(num_voxels > 0);
    if (num_voxels > 0 && num_voxels < min_voxels)
    {
	scale_factor = SYScbrt(num_voxels / min_voxels);
	return true;
    }
    
    // No resampling needed
    scale_factor = 1.0;
    return false;
}

// Adapter for CopyTask
class GT_Vec3ArrayProxy
{
public:
    typedef UT_Vector3F value_type;

    GT_Vec3ArrayProxy(GT_Real32Array &arr) : myArray(&arr)
    {
    }

    UT_Vector3F &operator()(exint i)
    {
	return *reinterpret_cast<UT_Vector3F *>(myArray->getData(i));
    }

    void entries(exint num_items)
    {
	myArray->resize(num_items);
    }

private:
    GT_Real32Array *    myArray;
};

// Created by GTappendCopyTask()
template <typename DstArrayT, typename SrcArrayT>
class GT_CopyTask : public UT_Task
{
public:
    static UT_Task *
    create(DstArrayT &dst, exint dst_start,
	   const SrcArrayT &src, exint num_elems)
    {
	return new (allocate_root())
			GT_CopyTask(dst, dst_start, src, num_elems);
    }

private:
    GT_CopyTask(DstArrayT &dst, exint dst_start,
	     const SrcArrayT &src, exint num_elems)
	: myDst(&dst)
	, myDstStart(dst_start)
	, mySrc(&src)
	, myNumElems(num_elems)
    {
    }

    virtual UT_Task *run()
    {
	exint bytes = myNumElems * sizeof(typename DstArrayT::value_type);
	::memcpy(&(*myDst)(myDstStart), &(*mySrc)(0), bytes);
	return NULL;
    }
    
    DstArrayT *		myDst;
    exint		myDstStart;
    const SrcArrayT *	mySrc;
    exint		myNumElems;
};

/// Used to create an UT_TaskList of array copy tasks
template <typename DstArrayT, typename SrcArrayT>
void
GTappendCopyTask(
	UT_TaskList &tasks,
	DstArrayT &dst, exint dst_start,
	SrcArrayT &src, exint num_elems)
{
    if (num_elems <= 0)
	return;
    tasks.append(*GT_CopyTask<DstArrayT, SrcArrayT>::create(
					dst, dst_start, src, num_elems));
}

/// Copy all the data in the thread chunks into a the destination.
template <typename DstArrayT, typename ThreadChunksT>
void
GTmergeThreadChunks(DstArrayT &dst, const ThreadChunksT &chunks)
{
    exint size = 0;
    exint max_items = 0;

    UT_TaskList copy_tasks;
    typedef typename ThreadChunksT::value_type	    ChunkPtrT;
    typedef typename ThreadChunksT::const_iterator  ChunksIterT;
    ChunksIterT end = chunks.end();
    for (ChunksIterT it = chunks.begin(); it != end; ++it)
    {
	const ChunkPtrT &chunk = it.get();
	if (!chunk)
	    continue;

	exint src_items = chunk->appendCopyTask(copy_tasks, dst, size);
	if (src_items > max_items)
	    max_items = src_items;
	size += src_items;
    }
    dst.entries(size);
    if (max_items > 20000)
	copy_tasks.spawnRootAndWait();
    else
	copy_tasks.runSerial();
}

#endif // __GT_VOLUMEUTIL_H_INCLUDED__
