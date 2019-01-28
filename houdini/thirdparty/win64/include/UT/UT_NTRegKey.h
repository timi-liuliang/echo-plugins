/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_NTRegKey.h (UT Library, C++)
 *
 * COMMENTS:	
 *
 */

#ifndef __UT_NTREGKEY_H_INCLUDED__
#define __UT_NTREGKEY_H_INCLUDED__

#ifdef _WIN32

#include "UT_API.h"
#include "UT_WorkBuffer.h"

#include <stddef.h>
#include <windows.h>


class UT_NTRegKey
{
public:
    UT_NTRegKey()
	: myHandle(NULL)
    {
    }
    ~UT_NTRegKey()
    {
	closeIfOpen();
    }
    
    bool open(HKEY key_parent, const char *reg_path)
    {
	closeIfOpen();

	if (RegOpenKeyEx(key_parent, reg_path, 0, KEY_QUERY_VALUE, &myHandle)
		== ERROR_SUCCESS)
	    return true;

	myHandle = NULL;
	return false;
    }

    bool open(HKEY key_parent, const UT_WorkBuffer &reg_path)
    {
	return open(key_parent, reg_path.buffer());
    }

    void closeIfOpen()
    {
	if (myHandle)
	{
	    RegCloseKey(myHandle);
	    myHandle = NULL;
	}
    }

    bool getValue(const char *key_name, UT_WorkBuffer &value)
    {
	value.clear();

	if (!myHandle)
	    return false;

	DWORD len;
	if (RegQueryValueEx(myHandle, key_name, 0, 0, NULL, &len)
		!= ERROR_SUCCESS)
	    return false;

	bool ok = (RegQueryValueEx(myHandle, key_name, 0, 0,
				   (LPBYTE)value.lock(0, len), &len)
			== ERROR_SUCCESS);
	value.release(true);

	return (ok && value.isstring());
    }

private:
    HKEY    myHandle;
};

#endif // _WIN32

#endif // __UT_NTREGKEY_H_INCLUDED__
