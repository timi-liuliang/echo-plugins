/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * Authored by:
 *	Chris Thompson
 *	Side Effects Software Inc.
 *
 * NAME:	Console handler for utility library (C++)
 *
 * COMMENTS:
 *
 */

#ifndef __UT_Console_h__
#define __UT_Console_h__

#include "UT_API.h"
#include "UT_Defines.h"

class UT_ConsoleHandler;

class UT_API UT_Console
{
public:
    // Any class in Houdini can call initConsole().
    // Be sure to call this before using stdin/stdout/stderr.
    // You can call this as many times as you'd like.
    static void		 initConsole();

    // This function returns either zero or CREATE_NO_WINDOW, which is the
    // process creation flag that prevents the process from communicating
    // with a console. For graphical applications, we want CREATE_NO_WINDOW
    // so that the application doesn't pop up a console window. For non-
    // graphical applications, we want zero so that the spawned application
    // can access the existing application's console.
    static unsigned	 childProcessCreateWindowFlag();

    // This should only be called once (when Houdini terminates).
    static void		 terminateConsole();

    static bool		 hasTextConsole();

    // This method allows you to control whether or not the
    // application will be allowed to terminate while the console
    // is displaying some text.  
    // If UT_TRUE, the user must explicitly close the console
    //    (this is useful for apps that display 'usage' messages
    //     then terminate immediately).
    // If UT_FALSE, the console dies when the user closes the
    //    application.
    //
    // The default is UT_TRUE.
    //
    // In Houdini, the policy is this:  setWaitForUserToClose()
    // is UT_TRUE *until* the main window is opened, then 
    // it is switched off.
    //
    // Of course, if the console does not exist on a platform
    // (eg. SGI), this has no effect.
    static void		 setWaitForUserToClose( UT_Bool val );

private:
    static UT_Bool		 theConsoleInitialized;
    static UT_ConsoleHandler	*theConsoleHandler;
    static UT_Bool		 theConsoleWaitForUser;
    static bool			 theHasTextConsole;
};

#endif // __UT_Console_h__
