/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SYS_Handle.h (SYS Library, C++)
 *
 * COMMENTS:	Platform independent handle for marshalling between Unix file
 *		descriptors (int) and Windows HANDLE objects (which are assumed
 *		to be void *).
 */

#ifndef __SYS_HANDLE_H_INCLUDED__
#define __SYS_HANDLE_H_INCLUDED__

#include "SYS_API.h"


typedef void *		    SYS_WinHandle;
typedef int		    SYS_UnixHandle;

// We assume that sizeof(SYS_WinHandle) > sizeof(UnixHandle) so we use that as
// the invalid value.
#define SYS_INVALID_HANDLE  SYS_WinHandle(-1)


union SYS_API SYS_Handle
{
public:
			    SYS_Handle()
				: myWinHandle(SYS_INVALID_HANDLE)
			    {
			    }
			    explicit
			    SYS_Handle(SYS_UnixHandle h)
				: myWinHandle(SYS_INVALID_HANDLE)
			    {
				myUnixHandle = h;
				if (h < 0)
				    invalidate();
			    }
			    explicit
			    SYS_Handle(SYS_WinHandle h)
				: myWinHandle(h)
			    {
			    }

    void		    invalidate()
				{ myWinHandle = SYS_INVALID_HANDLE; }
    bool		    isValid() const
				{ return (myWinHandle != SYS_INVALID_HANDLE); }

    SYS_UnixHandle	    unixHandle() const
				{ return myUnixHandle; }
    void		    setUnixHandle(SYS_UnixHandle h)
				{ myUnixHandle = h; }

    SYS_WinHandle	    winHandle() const
				{ return myWinHandle; }
    void		    setWinHandle(SYS_WinHandle h)
				{ myWinHandle = h; }

private:
    SYS_WinHandle	    myWinHandle;
    SYS_UnixHandle	    myUnixHandle;
};

#endif // __SYS_HANDLE_H_INCLUDED__
