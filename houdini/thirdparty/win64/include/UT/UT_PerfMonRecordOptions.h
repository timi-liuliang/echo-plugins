/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_PerfMonRecordOptions.h (UT Library, C++)
 *
 * COMMENTS:
 *
 *		Performance monitor recording options.
 */

#ifndef __UT_PerfMonRecordOptions__
#define __UT_PerfMonRecordOptions__

#include "UT_API.h"

class UT_API UT_PerfMonRecordOptions
{
public:
    UT_PerfMonRecordOptions(
	bool cook_stats=true, bool solve_stats=true, bool node_draw_stats=true,
	bool node_gpu_draw_stats=false, bool viewport_stats=true, 
	bool script_stats=true, bool render_stats=true, 
	bool thread_stats=false, bool frame_stats=true, 
	bool memory_stats=false, bool errors=true);
    UT_PerfMonRecordOptions(const UT_PerfMonRecordOptions &options);
    ~UT_PerfMonRecordOptions();

    bool		     recordCookStats() const;
    bool		     recordSolveStats() const;
    bool		     recordObjectDrawStats() const;
    bool		     recordObjectGPUDrawStats() const;
    bool		     recordViewportStats() const;
    bool		     recordScriptStats() const;
    bool		     recordRenderStats() const;
    bool		     recordThreadStats() const;
    bool		     recordFrameStats() const;
    bool		     recordMemoryStats() const;
    bool		     recordErrors() const;

    /// Convenience method which returns true if the options are
    /// configured to record any time stats.
    bool		     isRecordingTimedStats() const;

    void		     setRecordCookStats(bool record);
    void		     setRecordSolveStats(bool record);
    void		     setRecordObjectDrawStats(bool record);
    void		     setRecordObjectGPUDrawStats(bool record);
    void		     setRecordViewportStats(bool record);
    void		     setRecordScriptStats(bool record);
    void		     setRecordRenderStats(bool record);
    void		     setRecordThreadStats(bool record);
    void		     setRecordFrameStats(bool record);
    void		     setRecordMemoryStats(bool record);
    void		     setRecordErrors(bool record);

    UT_PerfMonRecordOptions &operator=(const UT_PerfMonRecordOptions &options);

private:

    bool		     myRecordCookStats;
    bool		     myRecordSolveStats;
    bool		     myRecordObjectDrawStats;
    bool		     myRecordObjectGPUDrawStats;
    bool		     myRecordViewportStats;
    bool		     myRecordScriptStats;
    bool		     myRecordRenderStats;
    bool		     myRecordThreadStats;
    bool		     myRecordFrameStats;
    bool		     myRecordMemoryStats;
    bool		     myRecordErrors;
};

#endif

