/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_TimerQuery.h ( RE Library, C++)
 *
 * COMMENTS:
 *
 *    This class implements the GL_ARB_timer_query feature, which allows
 *    timer start/stop commands to be inserted into the GL command stream so
 *    that the time it takes the hardware to complete a task is accurately
 *    measured. Simply timing the GL commands themselves is not sufficient
 *    because OpenGL processes them asynchronously.
 *
 *    The body of this class is implemented in RE_OGLQuery.C.
 */
#ifndef RE_TIMER_QUERY_H
#define RE_TIMER_QUERY_H

#include "RE_OGLQuery.h"

class RE_Render;

/// Basic GL timer query implementation
class RE_API RE_TimerQuery : public RE_OGLQuery
{
public:
		RE_TimerQuery();
   virtual     ~RE_TimerQuery() {}

   /// @brief Fetch elapsed render time between begin/end in nanoseconds.
   /// Returns the elapsed time in nanoseconds. If this extension is not
   /// supported, it will always return 0 and assert. Calling this
   /// without previously calling begin/end, or inside begin/end, will also
   /// assert.
   int64	getElapsedTimeNS(RE_Render *r)
		   { return getLongResult(r); }

   /// Alternate query - request the GPU timestamp. This is useful because timer
   /// queries cannot be nested, so higher level intervals can be computed
   /// by differencing 2 timestamps. RE_GPUTimer does this.
   bool		recordTimestamp(RE_Render *r) { return queryCounter(r); }

   /// Fetch the timestamp recorded by recordTimestamp()
   int64	getTimeStampNS(RE_Render *r)
		   { return getLongResult(r); }
};

/// Regular begin/end Timer queries cannot be nested, so this class uses 2
/// timers to record the start and end timestamp instead.
class RE_API RE_GPUTimer
{
public:
		RE_GPUTimer() : myHasTime(false), myTime(0) {}
	
    /// Initialize the queries (queries cannot be created if a query is running)
    void	init(RE_Render *r)
		{
		    myStart.init(r);
		    myEnd.init(r);
		    myTime = 0;
		    myHasTime = false;
		}

    /// @brief Mark the beginning and end of GPU commands to time
    /// Bracket the draw calls to measure with begin and end. Multiple begin/end
    /// calls without a reset() will accumulate time.
    /// @{
    void	begin(RE_Render *r)
		{
		    if(myHasTime)
		    {
			myTime += getElapsedTimeNS(r);
			myHasTime = false;
		    }
		    myStart.recordTimestamp(r);
		}
    
    void	end(RE_Render *r)
		{
		    myEnd.recordTimestamp(r);
		    myHasTime = true;
		}
    /// @}

    /// Returns true if a begin/end was performed. Does not mean the result is
    /// necessarily available yet.
    bool	hasResult() const { return myHasTime; }

    /// Returns true if the query result is available from the GL server.
    bool	isResultAvailable(RE_Render *r)
		    {
			return (myStart.isResultAvailable(r) &&
				myEnd.isResultAvailable(r));
		    }

    /// Fetch elapsed render time between begin/end in nanoseconds.
    int64	getElapsedTimeNS(RE_Render *r)
		    {
			int64 t = myTime;

			if(myHasTime)
			    t += (myEnd.getTimeStampNS(r) -
				  myStart.getTimeStampNS(r));
			
			myHasTime = false;
			
			return t;
		    }

    /// Reset the queries without deleting them.
    void	reset() { myHasTime = false; myTime = 0; }

    /// Deletes the queries. init() must be called again.
    void	destroy()
		{
		    myStart.destroy();
		    myEnd.destroy();
		    myHasTime = false;
		    myTime = 0;
		}
    
private:
    RE_TimerQuery myStart;
    RE_TimerQuery myEnd;
    int64	  myTime;
    bool	  myHasTime;
};

#endif
