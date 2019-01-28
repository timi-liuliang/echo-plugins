/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_SharedLoadDataStat.h
 *
 * COMMENTS:
 */

#ifndef GA_SHAREDLOADDATASTAT_H_
#define GA_SHAREDLOADDATASTAT_H_

#include "GA_API.h"
#include <UT/UT_SharedPtr.h>

/// Class to hold stat of shared data when delayed loading
///
/// Users can ask a GA_SharedDataHandle for a stat of their shared data
/// to load a subset of usefull information about it without taking up
/// too much time.
class GA_API GA_SharedLoadDataStat
{
public:
    GA_SharedLoadDataStat() {}
    virtual ~GA_SharedLoadDataStat() {}
private:
};

#endif // GA_SHAREDLOADDATASTAT_H_
