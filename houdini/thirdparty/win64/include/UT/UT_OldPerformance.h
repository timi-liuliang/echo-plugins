/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_OldPerformance.h ( UT Library, C++)
 *
 * COMMENTS:
 *
 *		The old performance monitor (< H12).
 *
 */

#ifndef __UT_OldPerformance__
#define __UT_OldPerformance__

#include "UT_API.h"

#include <SYS/SYS_Time.h>
#include "UT_Lock.h"
#include "UT_ValArray.h"
#include "UT_FloatArray.h"
#include "UT_ThreadSpecificValue.h"
#include "UT_WorkBuffer.h"

enum {
    LOG_OFF = 0,
    LOG_WINDOW,
    LOG_STDOUT
};

enum {
    TIME_FORMAT_ALL = 0,
    TIME_FORMAT_MS,
    TIME_FORMAT_SEC
};

enum {
    PERFORMANCE_WINDOW_LOG	= 0,
    PERFORMANCE_ENDFRAME	= 1,
    PERFORMANCE_WINDOW_LOG_HIGH = 2	// Important message to be highlighted.
};

enum UT_OldPerformanceFilter
{
    UT_PERF_FILTER_OFF,
    UT_PERF_FILTER_HIGHLIGHT,		// Highlight long ops
    UT_PERF_FILTER_HIDE			// Hide short ops
};

typedef void (*UT_OldPerformanceCB)(
    int type, void *data, const char *msg, int refresh);

class utPerfEvent;
class ut_Perf;

class UT_API UT_OldPerformance
{
public:
     UT_OldPerformance();
    virtual ~UT_OldPerformance();

	///
	/// Methods to log performance events. These methods are recorded on
	/// a per thread basis
	///
	// @{
	void		startEvent(int thread,
				   const char *type,
				   const char *name = 0,
				   const char *info = 0,
				   int count = -1);
	fpreal		stopEvent(int thread, bool log = true);

	/// This allows one to manually add to the accumulated time.
	void	        logAccumulatedTime(int thread, fpreal time_ms);

	// @}

	void		logMessage(const char *m, int refresh = 1,
				   fpreal dtime = -1,
				   const utPerfEvent *event = 0);

	void		filterLog(UT_OldPerformanceFilter filter,
				  fpreal time_threshold_ms);

	void		logEnable(int yesno)
	    		{ 
			    if( myLogEnable )
				frameComplete(myLastFrame);
			    myLogEnable = yesno; 
			    myFrameState = 0;
			}
	int		logEnabled() const
	    		{ return myLogEnable && myLogType; }

	int		opTiming() const
	    		{ 
			    return 
			     (myOpInfo || myOpHighlight ||
			     (myLogEnable && myLogType && myLogCookTime));
			}

	int		frameTiming() const
	    		{ 
			    return logEnabled() && myLogFLength;
			}

	void		setCallback(UT_OldPerformanceCB func, void *data)
	    		{
			    myFunc = func; 
			    myData = data;
			}

	///
	/// frame methods
	///
	/// @{

	/// frameChange is for automatically timing a series of frames.
	/// it stops timing the previous frame and starts timing the given one
	void		frameChange(fpreal frame);

	/// use frameStart and frameComplete for single-frame timing
	void		frameStart(fpreal frame);
	void		frameComplete(fpreal frame);
	fpreal		getLastFrame() const { return myLastFrame; }

	/// @}

	///
	/// op methods
	///
	/// @{

	int		opInfo() const		{ return myOpInfo; }
	void		opInfo(int yesno)	{ myOpInfo = yesno; }

	int		opHighlight() const	{ return myOpHighlight; }
	void		opHighlight(int yesno)	{ myOpHighlight = yesno; }

	/// @}

	///
	/// memory methods
	///
	/// @{
	void		startMemoryLog();
	void		stopMemoryLog(const char *label);
	/// @}

	///
	/// log query methods
	///
	/// @{

	int		logType() const { return myLogType; }
	void		logType(int type);
	void		logPause(int yesno);
	void		logSorted(int yesno);

	int		logTimeFormat() const { return myTimeFormat; }
	void		logTimeFormat( int format ) { myTimeFormat = format; }

	int		logCookTime() const	
			{ return logEnabled() && myLogCookTime; }
	void		logCookTime(int yesno)	
			{ myLogCookTime = yesno; }

	bool		logSolveTime() const
			{ return logEnabled() && myLogSolveTime; }
	void		logSolveTime(bool enable)
			{ myLogSolveTime = enable; }

	int		logLongCHOPs() const	
			{ return logEnabled() && myLogLongCHOPs; }
	void		logLongCHOPs(int yesno)	
			{ myLogLongCHOPs = yesno; }
	
	int		logObjectViewTime() const
			{ return logEnabled() && myLogObjectViewTime; }
	void		logObjectViewTime(int yesno)
			{ myLogObjectViewTime = yesno; }

	int		logViewPortTime() const
			{ return logEnabled() && myLogViewPortTime; }
	void		logViewPortTime(int yesno)
			{ myLogViewPortTime = yesno; }

	int		logFLength() const
			{ return logEnabled() && myLogFLength; }
	void		logFLength(int yesno)
			{ myLogFLength = yesno; }

	int		logMemory() const	
			{ return logEnabled() && myLogMemory; }
	void		logMemory(int yesno)	
			{ myLogMemory = yesno; }

	int		logErrors() const
			{ return logEnabled() && myLogErrors; }
	void		logErrors(int yesno)
			{ myLogErrors = yesno; }

	void		clearFrameAverage()
	    		{ myFrameCount = 0; myFrameTotal = 0.0; }

	/// @}

	//
	// misc
	//
	// @{
	static void formatTime(UT_WorkBuffer &buf, fpreal t, 
					    const char *prefix,
					    const char *suffix = "\n\r",
					    int format = TIME_FORMAT_ALL);
	// @}

private:

    /// Option that determines whether performance information is displayed 
    /// with Node info.
//    bool		    myDisplayNodeInfo;

	// decomposition of frameChange
	void		 logFrameAverage();
	void		 clearAccumTime();
	void		 startFrame(fpreal frame);
	void		 endFrame(fpreal dtime);
	fpreal		 elapsedTime(SYS_TimeVal &etime);

	int		 myLogType;
	int		 myTimeFormat;
	int		 myFrameState;

	unsigned int	 myLogEnable : 1,

			 myOpInfo : 1,
			 myOpHighlight : 1,

			 myLogPause : 1,
			 myLogSorted : 1,
			 myLogCookTime : 1,
			 myLogLongCHOPs : 1,
			 myLogViewPortTime : 1,
			 myLogObjectViewTime : 1,
			 myLogFLength : 1,
			 myLogMemory : 1,
			 myLogErrors : 1,
			 myLogSolveTime : 1;

	void		 appendMessage(const char *m, fpreal dtime,
				       const utPerfEvent *event);

	void		 clearMessages();
	void		 flushMessages();


	UT_ValArray<char *>		 myMessage;
	UT_ValArray<utPerfEvent *>	 myMessageEvent;
	UT_FloatArray			 myMessageTime;
	int				 myMessageCount;

	UT_OldPerformanceCB	 myFunc;
	void			*myData;

	int			 myFrameCount;
	fpreal			 myFrameTotal;

	size_t			 myMemoryUsage;

	UT_ThreadSpecificValue<ut_Perf> *myPerf;

	fpreal				myLastFrame;
	SYS_TimeVal			myStartTime;

	/// am i currently timing a frame?
	bool				myTiming;
	
	UT_OldPerformanceFilter		myFilter;
	fpreal				myFilterThreshold;
	int				myFilterHiddenEvents;

	UT_Lock				myLock;
};

UT_API UT_OldPerformance    *UTgetOldPerformance(bool create=true);
UT_API void		     UTsetOldPerformance(UT_OldPerformance *perfmon);

#endif

