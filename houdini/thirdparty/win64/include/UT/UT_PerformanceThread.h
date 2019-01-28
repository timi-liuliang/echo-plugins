/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_PerformanceThread.h (UT Library, C++)
 *
 * COMMENTS:	
 *
 */

#ifndef __UT_PerformanceThread__
#define __UT_PerformanceThread__

#include "UT_API.h"

/// @file
/// This include provides a light weight way to record thread stats without
/// pulling in the full UT_Performance.h include.
///

class UT_TaskScope;

/// Determine if we're currently recording thread stats
UT_API bool UTperformanceIsRecordingThreadStats();

UT_API int  UTperformanceStartTaskScope(const UT_TaskScope *task_scope);
UT_API void UTperformanceStopTaskScope(const UT_TaskScope *task_scope,
				       int event_id);

#endif
