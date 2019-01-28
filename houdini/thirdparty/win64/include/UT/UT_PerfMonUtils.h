/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_PerfMonUtils.h (UT Library, C++)
 *
 * COMMENTS:
 *
 *	Pure static class that provides utilities for the performance monitor
 *	to merge and manipulate sets of times 
 */

#ifndef __UT_PerfMonUtils__
#define __UT_PerfMonUtils__

#include "UT_API.h"
#include "UT_PriorityQueue.h"
#include "UT_ValArray.h"

#include <SYS/SYS_Time.h>


struct UT_TimeBlock;
typedef UT_ValArray<UT_TimeBlock> UT_TimeBlockList;

// A block of time with a start and end.
struct UT_API UT_TimeBlock
{
    UT_TimeBlock(const SYS_TimeVal &start, const SYS_TimeVal &end);

    fpreal time() const;

    SYS_TimeVal	myStart;
    SYS_TimeVal myEnd;
};

// Comparator for time blocks.
class UT_API UT_TimeBlockCompare
{
public:
    bool operator()(const UT_TimeBlock *block1, const UT_TimeBlock *block2) const;
};

typedef UT_PriorityQueue<UT_TimeBlock *, UT_TimeBlockCompare, false> 
UT_TimeBlockQueue;

class UT_API UT_PerfMonUtils
{
public:
    static bool isTimeLess(const SYS_TimeVal &time1, const SYS_TimeVal &time2);
    static bool doTimeBlocksOverlap(const UT_TimeBlock *block1, 
				    const UT_TimeBlock *block2);
    static void mergeTimeBlock(const UT_TimeBlock &time_block, 
				UT_TimeBlockQueue &time_blocks);
};

#endif
