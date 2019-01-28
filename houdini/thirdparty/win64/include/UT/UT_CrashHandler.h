/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_CrashHandler.h (UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_CrashHandler__
#define __UT_CrashHandler__

#include "UT_API.h"
#include "UT_UniquePtr.h"
#include "UT_Signal.h"
#include "UT_String.h"


/// This class handles crashing signals
///
/// This must be a singleton object.  It traps crashing signals and handles
/// them properly.
///  a)  When a thread catches a signal, the signal is passed to the main thread
///  b)  A stack trace is can be printed
/// This detects recursion into the signal handler and takes appropriate action
///
/// Depending on the environment variable HOUDINI_COREDUMP, houdini will generate a core or 
class UT_API UT_CrashHandler
{
public:
    /// Construct using an array of signals.  The array should be terminated by
    /// a zero.
    explicit UT_CrashHandler(const char *app_name);
    virtual ~UT_CrashHandler();

    /// Access to the singleton
    static const UT_CrashHandler	*getCrashHandler();

    /// A subclass gets a chance to handle the signal before the base class
    /// takes any action.  If the method returns @c true, the signal handler
    /// will just return from the signal handler and the process will continue.
    /// The default method returns @c false, letting the signal be processed
    /// properly.
    ///
    /// Also, please note that this method may be called multiple times.  It
    /// may be called from a thread, then from the main thread, even for the
    /// same signal.  When called for a thread, the same signal will always be
    /// invoked for the main thread, so you may only want to do your processing
    /// when UT_Thread::isMainThread() is true.
    ///
    /// Please note that this is called from within a signal handler.  The
    /// signal might have been raised within Python, so don't call Python from
    /// the handler.  The signal handler might have been called within
    /// malloc()/free(), so there might be a system lock on malloc, so try not
    /// to call malloc/free from within your handler.
    virtual bool	handledSignal(UTsignalHandlerArg arg);

    /// This virtual allows the signal handler to have a "chaser" function, one
    /// which gets called after the stack trace has been printed out, but
    /// before core is actually dumped.
    virtual void	chaser(UTsignalHandlerArg arg);

    /// Indicate whether the stack trace should be printed to stderr on a crash
    /// Defaults to @c true
    virtual bool	 traceToStderr() const;

    /// Return the application name
    const char		*applicationName() const
			    { return myAppName; }

    /// Set the application name
    void		 setApplicationName(const char *app_name)
			    { myAppName = app_name; }

    /// Access to signals
    const UT_Signal	*signals() const
			    { return mySignals.get(); }

    /// Access to signal handler function
    UTsigHandler	 signalFunction() const;

private:
    UT_String		    myAppName;
    UT_UniquePtr<UT_Signal> mySignals;
};

#endif
