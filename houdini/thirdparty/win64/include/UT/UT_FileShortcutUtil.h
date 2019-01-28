/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_FileShortcutUtil.h (FS Library, C++)
 *
 * COMMENTS:	Class for manipulating shortcut files on Windows.
 *		NOTE: This object must be constructed/destructed in the same
 *                    thread!
 *
 *		For other platforms, this class does nothing.
 */

#ifndef __UT_FILESHORTCUTUTIL_H__
#define __UT_FILESHORTCUTUTIL_H__

#include "UT_API.h"
#include "UT_UniquePtr.h"

class FS_Stat;
class UT_String;

class UT_API UT_FileShortcutUtil
{
public:
    UT_FileShortcutUtil();
    ~UT_FileShortcutUtil();

    /// Find the real path represented by the give shortcut. If
    /// prompt_missing is true, then when the OS fails to find the shortcut
    /// target it will ask the user. When given the is_dir flag, it will be
    /// set to true if the real path is a directory.
    /// This function returns true on success.
    bool    resolve(const char *shortcut_path, UT_String &real_path,
		    bool prompt_missing, bool *is_dir = NULL) const;

    /// Create a shortcut
    bool    create(const char *shortcut_path, const char *real_path) const;

private:
    /// Not implemented
    // @{
    UT_FileShortcutUtil(const UT_FileShortcutUtil &);
    UT_FileShortcutUtil &operator=(const UT_FileShortcutUtil &);
    // @}

private:
    struct Data;
    UT_UniquePtr<Data>	myData;
};

#endif // __UT_FILESHORTCUTUTIL_H__
