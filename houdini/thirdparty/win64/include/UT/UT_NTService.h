/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 *
 * Authored by:
 *	Chris Thompson
 *	Side Effects Software Inc.
 *
 * NAME:	UT_NTService.h (Utility Library, C++)
 *
 * COMMENTS:	A C++ class to encapsulate most of the nasty
 *		work involved in writing Windows NT services.
 *		Derive from this to implement your own service.
 *
 */

#ifndef __UT_NTService_h__
#define __UT_NTService_h__

#include <windows.h>

#include "UT_API.h"
#include "UT_Defines.h"

#define UT_SERVICE_STOPPED		0
#define UT_SERVICE_RUNNING		1
#define UT_SERVICE_STARTING		2

// Similar to WM_USER... use this as an offset for
// custom service control messages.
#define SERVICE_CONTROL_USER 128

// For some reason this is not getting picked up
// from WinUser.h.
#define MB_SERVICE_NOTIFICATION 0x00200000L


class UT_API UT_NTService
{
public:
    explicit UT_NTService(const char *serviceName);
    virtual ~UT_NTService();

    virtual const char  *getAboutString();
    virtual const char  *getVersionString();
    virtual const char  *getInstalledString();

    const char 		*getServiceExeName();
    const char 		*getServiceUserName();

    UT_Bool		 isInstalled();
    int			 isRunning();
    
    UT_Bool logEvent(WORD  type,
		     DWORD id,
                     const char* string1 = UT_NULL,
                     const char* string2 = UT_NULL,
                     const char* string3 = UT_NULL);
    
    UT_Bool startService();
    
    void setStatus(DWORD state);
    
    UT_Bool initialize();
    
    virtual void run();
    
    virtual UT_Bool onInit();
    virtual void onStop();
    virtual void onInterrogate();
    virtual void onPause();
    virtual void onContinue();
    virtual void onShutdown();
    virtual void onUserLogout();
    virtual UT_Bool onUserControl(DWORD opcode);

    void debugMsg(const char* format, ...);
    void userMsg(const char* format, ...);
    void userDisplayLastError();
    void userDisplayAdminError();

    static void WINAPI serviceMain(DWORD argc, LPTSTR* argv);
    static void WINAPI handler(DWORD opcode);

    HINSTANCE getHInstance() { return myHInstance; };

    // We maintain the 'this' pointer so that serviceMain() and
    // handler() can access the object.
    static UT_NTService* myThis;

    char		    myServiceName[64];
    SERVICE_STATUS_HANDLE   myServiceStatus;
    SERVICE_STATUS	    myStatus;
    UT_Bool		    myIsRunning;

protected:
    void userMessageBox(LPCSTR msg, LPCSTR title, UINT type);

    static DWORD WINAPI msgBoxMain(LPVOID msgParam); 
    static BOOL	WINAPI logoutHandler(DWORD ctrlType);
    static BOOL CALLBACK EnumCloseProc(HWND hwnd, LPARAM lParam);

    HINSTANCE	myHInstance;
    HANDLE	myEventSource;
};
 
#endif // __UT_NTService_h__
