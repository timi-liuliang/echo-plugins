/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CPP_TokenStream.h ( FS Library, C++)
 *
 * COMMENTS:	Reads a stream of tokens using CPP
 */

#ifndef __CPP_TokenStream__
#define __CPP_TokenStream__

#include "FS_API.h"
#include <UT/UT_String.h>
#include <UT/UT_WorkBuffer.h>
#include <UT/UT_WorkArgs.h>

class CPP_Manager;
class UT_IStream;

class FS_API CPP_TokenStream
{
public:
	     CPP_TokenStream(const char *filename, int pathvar=-1,
				const char **cpp_options=0);
	     CPP_TokenStream(UT_IStream &is, const char *filename="<internal>",
				const char **cpp_options=0);
    virtual ~CPP_TokenStream();

    void		 addIncludePath(const char *path);
    void		 addIncludePathRaw(const char *path);

    int			 getLineNumber();
    const char		*getFileName();

    // Set to true to preserve backslashes in the stream. Otherwise escaped
    // backslashes and quotes ('\\' and '\"') get expanded automatically.
    // The default is to expand automatically (false).
    void		setPreserveBackslashes(bool flag);

    int			getToken(UT_String &token);
    void		ungetToken(const UT_String &token);

    virtual void	cppError(const char *message) const;
    virtual void	cppWarning(const char *message) const;

protected:
    void		init(int pathvar, const char **cpp_options);

    UT_WorkArgs		 myArgs;
    UT_WorkBuffer	 myArgBuf;
    UT_String		 myUnget;
    CPP_Manager		*myCPP;
    int			 myOptind;
    bool		 myPreserveBackslashes;
};

#endif

