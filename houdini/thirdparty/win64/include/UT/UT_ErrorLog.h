/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_ErrorLog.h (UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_ErrorLog__
#define __UT_ErrorLog__

#include "UT_API.h"
#include <time.h>
#include "UT_SymbolTable.h"
#include "UT_String.h"
#include "UT_Lock.h"

class UT_Error;

class UT_API UT_ErrorLog
{
public:
    UT_ErrorLog();
    ~UT_ErrorLog();

    /// Functor to handle errors printed out using the mantra* methods.
    class UT_API ErrorCallback
    {
    public:
	ErrorCallback() {}
	/// The destructor automatically de-registers the callback
	virtual ~ErrorCallback();

	/// If the method returns @c true, the error will not be printed out
	/// by the built-in error handler.  If it returns @c false, processing
	/// continues in the normal way.
	/// The @c level is the verbosity level of the error.
	/// The built-in process will print out:
	///	("%s%s", prefix, message).
	virtual bool	filterError(int level,
				    const char *message,
				    const char *prefix) = 0;
    };
    /// If you want to un-register the callback, just delete the object.
    static void	registerCallback(ErrorCallback *err);

    // Log the error's description to the error file if its severity is greater
    // than or equal to our minimum level
    void 		logError(UT_Error *);

    // We only worry about locking when we set strings; we assume integers
    // and booleans are atomic
    const UT_String    &getLogFileName() const 
			{ return myLogFileName; }
    void		setLogFileName(const UT_String &name) 
			{
			    myLock.lock(); 
			    myLogFileName.harden(name);
			    myLogFileName.expandVariables();
			    myLock.unlock();
			}

    int 		getMinLevel() const 
			{ return myMinLevel; }
    void 		setMinLevel(const int level)
			{ myMinLevel = level; }

    bool 		getDoLogging() const
			{ return myDoLogging; }
    void 		setDoLogging(bool log)
			{ myDoLogging = log; }

    void		resetState()
			{
			    myLock.lock(); 
			    myErrorLogTimes.clear();
			    myLock.unlock();
			}

    // Callback function to check whether mantra verbosity is enabled for a
    // particular callback level.
    typedef int		(*MantraVerboseCallback)(void);
    typedef int		(*MantraColorCallback)(void);
    typedef bool	(*MantraTimestampCallback)(void);

    // Set the verbosity level for mantra error messages.  For any
    // printouts to occur, it is necessary to first call this method with a
    // non-negative value - otherwise calls to mantraPrintf, mantraWarning,
    // and mantraError will be ignored.  As an alternative, you can provide
    // a callback that will dynamically update the verbosity level.
    static void		setMantraVerboseLevel(int level);
    static int		getMantraVerboseLevel();

    // This method will also cause errors and warnings to use a "mantra: "
    // prefix.
    static void		setMantraVerboseCallback(MantraVerboseCallback cb);
    static void		setMantraColorCallback(MantraColorCallback cb);
    static void		setMantraTimestampCallback(MantraTimestampCallback cb);

    static int		mantraPrintf(int level, const char *msg, ...)
			    SYS_PRINTF_CHECK_ATTRIBUTE(2,3);
    static int		mantraWarning(const char *msg, ...)
			    SYS_PRINTF_CHECK_ATTRIBUTE(1,2);
    static int		mantraError(const char *msg, ...)
			    SYS_PRINTF_CHECK_ATTRIBUTE(1,2);

    // Print the error message only if it has not been printed before
    static int		mantraPrintfOnce(int level, const char *msg, ...)
			    SYS_PRINTF_CHECK_ATTRIBUTE(2,3);
    static int		mantraWarningOnce(const char *msg, ...)
			    SYS_PRINTF_CHECK_ATTRIBUTE(1,2);
    static int		mantraErrorOnce(const char *msg, ...)
			    SYS_PRINTF_CHECK_ATTRIBUTE(1,2);

    // Clear tables of messages that have been printed before so that if
    // an error recurs, it will be printed again.
    static void		clearMantraOnceErrors();
protected:

private:
    int			myMinLevel;
    UT_String		myLogFileName;
    bool		myDoLogging;
    UT_SymbolMap<time_t> myErrorLogTimes;
    time_t		myLastErrorTime;
    UT_Lock		myLock;
};

UT_API UT_ErrorLog *UTgetErrorLog();

#endif

