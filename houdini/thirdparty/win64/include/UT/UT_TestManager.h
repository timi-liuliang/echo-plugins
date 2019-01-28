/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_TestManager.h (C++)
 *
 * COMMENTS:	This class provides a test harness framework.
 *
 * USAGE:
 *	1.  Create a main() method for your test application like so:
 *		int main(int argc, char *argv[])
 *		{
 *		    return UT_TestManager::get().run(argc, argv);
 *		}
 *	    The return code will be the number of test failures.
 *	    To obtain the command line usage, use "testapp -h".
 *
 *	2.  For each test case, use one of the TEST_REGISTER*() macros for
 *	    registering a test function. The function should return true if the
 *	    test passed.
 */

#ifndef __UT_TESTMANAGER_H__
#define __UT_TESTMANAGER_H__

#include "UT_API.h"

#include "UT_Functor.h"
#include "UT_StopWatch.h"
#include "UT_WorkBuffer.h"
#include <SYS/SYS_Types.h>

class UT_API UT_TestManager
{
public:
    virtual ~UT_TestManager() { }

    /// Access the global test manager
    static UT_TestManager & get();

    /// Determine if the test was interactively invoked
    virtual bool	    isInteractive() const = 0;

    /// Determine if the tests are being run for performance
    virtual bool	    isPerformance() const = 0;

    /// Determine the number of threads requested
    virtual int		    numThreads() const = 0;

    /// Called by main() to run the tests. Returns the number of test failures.
    virtual int		    run(int argc, char *argv[]) = 0;

    /// Called by the TEST_REGISTER*() macros to automatically add a new test
    /// case
    virtual void	    addTest(const char *name,
				    UT_Functor<bool> &callback) = 0;

    /// Allow for logging of intermediate test results.  If logging is turned
    /// on in the test manager, the test name, status and run time will be
    /// saved.
    virtual void	logResult(const char *name, bool success,
				    fpreal64 run_time) = 0;
};

/// @brief Status maintainer used to run a test.
///
/// This class simplifies the book-keeping when running a test.  The unit test
/// will automatically log information (i.e. performance, memory) if the
/// UT_TestManager is so configured.
///
/// Example: @code
/// static bool
/// unitTest(int argument)
/// {
///     UT_TestUnit         status("UnitTest: %d", argument);
/// 
///     if (!doTest(...))
///         return status.fail("Test failed: %s", some_string);
/// 
///     return status.ok();
/// }
/// @endcode
class UT_API UT_TestUnit
{
public:
     UT_TestUnit(const char *format, ...) SYS_PRINTF_CHECK_ATTRIBUTE(2, 3);
    ~UT_TestUnit();

    /// Print out the message given by a format.  This includes the test name,
    /// and other information.  No newline should be included in the format.
    ///
    /// The function @b always returns @b false.
    bool	fail(const char *format=NULL, ...)
		    SYS_PRINTF_CHECK_ATTRIBUTE(2, 3);

    /// Print out the message given by a format.  This includes the test name,
    /// and other information.  No newline should be included in the format.
    ///
    /// The function @b always returns @b true.
    bool	ok(const char *format=NULL, ...)
		    SYS_PRINTF_CHECK_ATTRIBUTE(2, 3);

    /// Restart the timer. If format is non-NULL, then it will print the time
    /// as well from when the timer last started. Note that the timer is
    /// automatically started in the constructor.
    void	restartTimer(const char *format=NULL, ...)
		    SYS_PRINTF_CHECK_ATTRIBUTE(2, 3);

    /// Enable printing of success/failure when this unit test is destroyed.
    void	setAlwaysPrint(bool f)	{ myAlwaysPrint = f; }

    /// Query the status of the test
    bool	getStatus() const	{ return myStatus; }
    bool	setStatus(bool b)	{ myStatus = b; return myStatus; }
private:
    UT_WorkBuffer	myName;		// Test-name
    UT_Timer		myTimer;
    bool		myStatus;
    bool		myPrint;
    bool		myAlwaysPrint;
};

/// Use this define to log information into the result table
#define TEST_LOGRESULT(NAME, SUCCESS, TIME)	\
	    UT_TestManager::get().logResult(NAME, SUCCESS, TIME)

/// Use this macro after your test function is declared. It will register
/// your method with the test manager. Your method should return true if the
/// test succeeded, false otherwise. The name is the label that will be used
/// for invoking this specific test.
#define TEST_REGISTER_FN(name, method)					\
	    class method##Registrar					\
	    {								\
		public:							\
		    method##Registrar()					\
		    {							\
			UT_Functor<bool> callback(&method);		\
			UT_TestManager::get().addTest(name, callback);	\
		    }							\
	    };								\
	    static method##Registrar	the##method##Registrar;

/// Use this macro after your test class is declared. It will create a static
/// instance of your class that then registers your 'bool classname::method()'
/// with the test manager. Your method should return true if the test
/// succeeded, false otherwise. The name is the label that will be used for
/// invoking this specific test.
#define TEST_REGISTER(name, classname, method)				\
	    class classname##method##Registrar				\
	    {								\
		public:							\
		    classname##method##Registrar()			\
		    {							\
			UT_Functor<bool> callback(&myTest,		\
						  &classname::method);	\
			UT_TestManager::get().addTest(name, callback);	\
		    }							\
		private:						\
		    classname	myTest;					\
	    };								\
	    static classname##method##Registrar				\
				    the##classname##method##Registrar;

#endif // __UT_TESTMANAGER_H__
