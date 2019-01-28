/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	FS_IconHelper.h (FS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __FS_ICONHELPER_H_INCLUDED__
#define __FS_ICONHELPER_H_INCLUDED__

#include "FS_API.h"
#include "FS_Reader.h"
#include "FS_Info.h"

class FS_API FS_IconReadHelper : public FS_ReaderHelper
{
public:
				FS_IconReadHelper();
    virtual			~FS_IconReadHelper();

    virtual FS_ReaderStream*	createStream(
				    const char *source,
				    const UT_Options *options = nullptr);
};

class FS_API FS_IconInfoHelper : public FS_InfoHelper
{
public:
				FS_IconInfoHelper();
    virtual			~FS_IconInfoHelper();

    virtual bool		canHandle(const char* source);
    virtual bool		hasAccess(const char* source, int mode);
    virtual bool		getIsDirectory(const char* source);
    virtual int			getModTime(const char* source);
    virtual int64		getSize(const char* source);
    virtual bool		getContents(
				    const char* source,
				    UT_StringArray& contents,
				    UT_StringArray* dirs);
    virtual char		getNextSepChar(const char *source);
    virtual char		getPrevSepChar(const char *source);
};

#endif // __FS_ICONHELPER_H_INCLUDED__
