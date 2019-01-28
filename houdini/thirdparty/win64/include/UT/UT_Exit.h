/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Exit.h ( UT Library, C++)
 *
 * COMMENTS:	General exit mechanism...  This allows a list of callback
 *		functions to be added when exit is being called
 *		The callbacks are called in the order that they were added.
 *		The callback function will only be added once (even if the data
 *		is different).
 */

#ifndef __UT_Exit__
#define __UT_Exit__

#include "UT_API.h"

class UT_API UT_Exit
{
public:
    /// The enumeration of the exit codes that Houdini producs can use.
    /// (see notes in http://tldp.org/LDP/abs/html/exitcodes.html)
    /// @note The numeric values for the specific exit reasons should not
    ///       change: some customers explicitly test the numeric value of the
    ///       exit code and take a specific action based on it. E.g., they may
    ///       test the exit code for the license failure on a render farm for
    ///       re-submission, but if we change the exit code for license failure
    ///       their scripts will need to be updated to account for that.
    ///
    /// @note Don't use exit code value of 259. On Windows, the exit code 259 
    ///	      means STILL_ACTIVE, and should not be used as an exit error code;
    ///	      otherwise the return value of GetExitCodeProcess() calls may be
    ///	      misinterpreted.
    enum UT_ExitCode
    {
	/// no error
	EXIT_OK			= 0,	

	/// catch-all error code
	EXIT_GENERIC_ERROR	= 1,

	/// misuse of shell builtins (according to Bash documentation)
	EXIT_BUILTIN_ERROR	= 2,	
	
	/// failure to check out or verify an appropriate product license
	EXIT_LICENSE_ERROR	= 3,

	/// socket communication failure
	EXIT_SOCKET_ERROR	= 4,

	/// failed to parse the UI definition file
	EXIT_PARSE_UI_ERROR	= 5
    };

    /// Calls exit(), which implicitly leads to our callbacks being called.
    static void		exit( UT_ExitCode exit_code = EXIT_OK );

    /// Just like exit(), except sets the exit code to the properly offset
    /// signal number according to the convention: 128 + signal_number.
    static void		exitWithSignalNumber( int signal_number );

    /// An exit method that takes any exit code (as integer). Should be used
    /// sparingly and only if necessary.  We want the exit codes to be
    /// consistent, stable and well-known to the customers (we can document 
    /// based on the above enum), so use exit() with UT_ExitCode, if possible.
    static void		exitWithSpecificCode( int exit_code );

    /// Return true if the application is in the process of exiting
    /// and false otherwise.
    static bool		isExiting();

    /// Calls our callbacks directly, without calling exit(). This should
    /// only be used by our core dump handler.
    static void		runExitCallbacks();

    /// Adds a callback to the end of the list of callbacks to be run when the
    /// process exits.
    /// Returns 1 if the function was added, the function can only be added one
    /// time. Returns 0 if this is a duplicate.
    ///
    /// @warning Destructors of global variables are called BEFORE the callback
    /// is invoked on Windows!
    static int		addExitCallback(void (*exitcallback)(void *data),
					void *data = 0);

    /// Adds a callback to the front of the list of callbacks to be run when
    /// the process exits.
    /// Returns 1 if the function was added, the function can only be added one
    /// time. Returns 0 if this is a duplicate.
    ///
    /// @warning Destructors of global variables are called BEFORE the callback
    /// is invoked on Windows!
    static int		addEarlyExitCallback(void (*exitcallback)(void *data),
					void *data = 0);

    /// Returns 1 if the function was removed. Returns 0 if the function was
    /// never added.
    static int		removeExitCallback(void (*exitcallback)(void *data),
					   void *data = 0);

    /// Remove all the exit callbacks.
    static void		removeAllExitCallbacks();

    /// Register a callback to shut down the QApplication object (if any).
    static void		registerQtAppExitCallback(
			    void (*exitcallback)(void *data), 
			    void *data=nullptr);

private:
    static void		doExit(int exit_code, bool fast_exit);
};

#endif
