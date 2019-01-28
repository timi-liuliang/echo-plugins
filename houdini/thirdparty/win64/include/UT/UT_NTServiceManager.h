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
 * NAME:	UT_NTServiceManager.h (Utility Library, C++)
 *
 * COMMENTS:	A C++ class to handle the work of controlling
 *		NT services.
 *
 */

#ifndef __UT_NTServiceManager_h__
#define __UT_NTServiceManager_h__

#include <windows.h>

#include "UT_API.h"

#define HSERVER_SERVICE		"HoudiniServer"
#define SESINETD_SERVICE	"HoudiniLicenseServer"
#define OLD_HSERVER_SERVICE	"HServer"
#define OLD_SESINETD_SERVICE	"SESINetD"

class UT_API UT_NTServiceManager
{
public:
			 UT_NTServiceManager(const char *machine = 0);
    virtual		~UT_NTServiceManager();

    bool		 isOpen() const;

    bool		 startService(const char *name) const;
    bool		 stopService(const char *name) const;
    bool		 installService(const char *name, const char *display,
					const char *path) const;
    bool		 uninstallService(const char *name) const;
    bool		 isServiceInstalled(const char *name) const;
    bool		 isServiceRunning(const char *name) const;

private:
    bool		 waitForStatus(SC_HANDLE service,
				       unsigned long desiredState,
				       int timeout) const;

    SC_HANDLE		 myManager;
};
 
#endif // __UT_NTServiceManager_h__
