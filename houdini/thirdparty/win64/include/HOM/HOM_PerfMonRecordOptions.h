/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __HOM_PerfMonRecordOptions_h__
#define __HOM_PerfMonRecordOptions_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_Errors.h"
#include "HOM_Module.h"
#include <string>

SWIGOUT(%feature("notabstract") HOM_PerfMonRecordOptions;)
SWIGOUT(%rename(PerfMonRecordOptions) HOM_PerfMonRecordOptions;)

class HOM_API HOM_PerfMonRecordOptions
{
public:
#ifdef SWIG
%extend {
    %kwargs HOM_PerfMonRecordOptions;
    HOM_PerfMonRecordOptions(
	bool cook_stats=true, 
	bool solve_stats=true,
	bool draw_stats=true, 
	bool gpu_draw_stats=false, 
	bool viewport_stats=true, 
	bool script_stats=true, 
	bool render_stats=true, 
	bool thread_stats=false,
	bool frame_stats=true,
	bool memory_stats=false,
	bool errors=true)
    {
	return HOM().newPerfMonRecordOptions(
	    cook_stats, solve_stats, draw_stats, gpu_draw_stats, viewport_stats,
	    script_stats, render_stats, thread_stats, frame_stats, 
	    memory_stats, errors);
    }
};
#else
    HOM_PerfMonRecordOptions()
    { HOM_CONSTRUCT_OBJECT(this) }
    HOM_PerfMonRecordOptions(HOM_PerfMonRecordOptions &options)
    {
	myRecordCookStats = options.myRecordCookStats;
	myRecordSolveStats = options.myRecordSolveStats;
	myRecordDrawStats = options.myRecordDrawStats;
	myRecordGPUDrawStats = options.myRecordGPUDrawStats;
	myRecordViewportStats = options.myRecordViewportStats;
	myRecordScriptStats = options.myRecordScriptStats;
	myRecordRenderStats = options.myRecordRenderStats;
	myRecordThreadStats = options.myRecordThreadStats;
	myRecordFrameStats = options.myRecordFrameStats;
	myRecordMemoryStats = options.myRecordMemoryStats;
	myRecordErrors = options.myRecordErrors;

	{ HOM_CONSTRUCT_OBJECT(this) }
    }
#endif

    virtual ~HOM_PerfMonRecordOptions()
    { HOM_DESTRUCT_OBJECT(this) }

    virtual std::string __repr__();

    bool recordCookStats();
    bool recordSolveStats();
    bool recordDrawStats();
    bool recordGPUDrawStats();
    bool recordViewportStats();
    bool recordScriptStats();
    bool recordRenderStats();
    bool recordThreadStats();
    bool recordFrameStats();
    bool recordMemoryStats();
    bool recordErrors();

    void setRecordCookStats(bool record);
    void setRecordSolveStats(bool record);
    void setRecordDrawStats(bool record);
    void setRecordGPUDrawStats(bool record);
    void setRecordViewportStats(bool record);
    void setRecordScriptStats(bool record);
    void setRecordRenderStats(bool record);
    void setRecordThreadStats(bool record);
    void setRecordFrameStats(bool record);
    void setRecordMemoryStats(bool record);
    void setRecordErrors(bool record);

private:

    bool myRecordCookStats;
    bool myRecordSolveStats;
    bool myRecordDrawStats;
    bool myRecordGPUDrawStats;
    bool myRecordViewportStats;
    bool myRecordScriptStats;
    bool myRecordRenderStats;
    bool myRecordThreadStats;
    bool myRecordFrameStats;
    bool myRecordMemoryStats;
    bool myRecordErrors;
};

#endif
