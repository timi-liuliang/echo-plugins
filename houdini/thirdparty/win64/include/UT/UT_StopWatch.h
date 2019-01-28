#ifndef	_UT_StopWatch_h_
#define	_UT_StopWatch_h_

#include "UT_API.h"
#include <time.h>
#include <stdlib.h>
#include <SYS/SYS_Time.h>
#include <SYS/SYS_Types.h>

typedef SYS_TimeVal UT_TIMERVAL;

//
//  a simple stopwatch class.  It implements functions much like
//	a regular stopwatch:
//	    start():	resets stopwatch to zero, and starts it.
//	    lap():	returns the elapsed time.
//	    stop();	stop the stopwatch
//	    resume();	resumes the stopwatch
//	    getTime();	returns elapsed time, may be used when stopped.
//	    operator-:	difference in start time between two watches
//

class UT_API UT_StopWatch
{
public:
    UT_StopWatch();

    void	clear();
    void	start();
    fpreal64	lap() const;
    fpreal64	stop();
    void	resume();

    fpreal64	getTime() const;
    bool	isRunning() const;

    fpreal64	operator-( const UT_StopWatch & );


    static fpreal64 _timeDiff(const UT_TIMERVAL &later, const UT_TIMERVAL &sooner);

protected:
    bool	_running;		// the watch is running
    fpreal64	_elapsedTime;

    UT_TIMERVAL	_startTime;
};

// this class replaces the old timeStamp function, and can be used to more
// easily take timing measurements
//
// you still have the old timeStamp function that works in the same way, but
// also, you can use begin and end event functions which will output a duration
// along with the timestamps. another benefit is that it becomes easy to see
// the contained duration of a function.
//
// you can additionally use the indent function to give you the right
// indentation when inserting your own printfs
//
//
// so, to add timestamps, do this:
// UT_Timer		ts( "block 1" );
// ts.end();	// no need if scope ends, otherwise it displays
//
// don't mismatch your begins and ends or else everything will break
// if you want running totals send in the pointer to total
class UT_API UT_Timer
{
public:
    UT_Timer();
    explicit UT_Timer(const char *block_name);

    ~UT_Timer();

    static void indent();

    static void	timeStamp(const char *msg1, const char *msg2=0);

    void clear();

    void begin( const char *block_name = NULL );
    int lap() { myCount++; return myCount; }
    void end();

    bool     isRunning() const { return myBegun; }

    fpreal64 getBlockDuration();
    fpreal64 getTotalDuration();
    fpreal64 getAverageDuration();

    void setDisplay( bool d )	{ myDisplay = d; }
    bool willDisplay() const	{ return myDisplay; }

    void displayBlockDuration( const char *block_name = NULL );
    void displayTotalDuration( const char *block_name = NULL );
    void displayAverageDuration( const char *block_name = NULL );

    static UT_Timer *getTimer(int id);

private:
    bool			myBegun;
    bool			myDisplay;
    UT_StopWatch		myBlockTimer;
    UT_StopWatch		mySavedBlockTimer;
    fpreal64			myTotal;
    int				myCount;

    static int			theIndentation;
    static UT_StopWatch		theLastBlockTimer;
};

// For use with UT_TimerAuto to keep track of total time
class UT_TimerDisplayTotal : public UT_Timer
{
public:
    UT_TimerDisplayTotal(const char* block_name)
	: myBlockName(block_name)
    {
    }
    ~UT_TimerDisplayTotal()
    {
	displayAverageDuration(myBlockName);
	displayTotalDuration(myBlockName);
    }
private:
    const char* myBlockName;
};

// Used to accumulate time into a UT_Timer/UT_TimerDisplayTotal
class UT_API UT_TimerAuto
{
public:
    UT_TimerAuto(UT_Timer &ts, const char *name): myTimeStamp( ts )
    {
	myTimeStamp.begin( name );
    }

    UT_TimerAuto(UT_Timer &ts): myTimeStamp( ts )
    {
	myTimeStamp.begin();
    }

    ~UT_TimerAuto()
    {
	myTimeStamp.end();
    }

    UT_Timer	&myTimeStamp;
};

#endif	// _UT_StopWatch_h_
