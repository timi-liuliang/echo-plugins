/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_PointSplat.h (GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_POINTSPLAT_H_INCLUDED__
#define __GT_POINTSPLAT_H_INCLUDED__

#include "GT_API.h"
#include "GT_VolumeUtil.h"
#include <UT/UT_Task.h>
#include <UT/UT_ThreadSpecificValue.h>
#include <UT/UT_UniquePtr.h>
#include <SYS/SYS_Types.h>


namespace GT_PointSplat
{

struct Chunk
{
    Chunk(exint num_items)
	: myPoints(/*capacity*/num_items)
	, myNormals(/*capacity*/num_items)
    {
    }

    template <typename ChunkArrayT>
    exint
    appendCopyTask(UT_TaskList &tasks, ChunkArrayT &dst, exint dst_start) const
    {
	GTappendCopyTask(tasks,
			 dst.myPoints, dst_start,
			 myPoints, myPoints.entries());
	GTappendCopyTask(tasks,
			 dst.myNormals, dst_start,
			 myNormals, myNormals.entries());
	return myPoints.entries();
    }

    UT_Vector3FArray    myPoints;
    UT_Vector3FArray    myNormals;
};

struct ChunkProxy
{
    ChunkProxy(GT_Real32Array &pnts, GT_Real32Array &nmls)
	: myPoints(pnts), myNormals(nmls)
    {
    }

    void entries(exint num_items)
    {
	myPoints.entries(num_items);
	myNormals.entries(num_items);
    }

    GT_Vec3ArrayProxy	myPoints;
    GT_Vec3ArrayProxy	myNormals;
};

typedef UT_ThreadSpecificValue<UT_UniquePtr<Chunk>> ThreadChunks;

} // namespace GT_PointSplat

#endif // __GT_POINTSPLAT_H_INCLUDED__
