/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	henv.h ( tools Library, C++)
 *
 * COMMENTS:	Generic environment variable handling routines.
 */

#ifndef __henv_h__
#define __henv_h__

#include "tools_API.h"
#include <SYS/SYS_Deprecated.h>

tools_API extern void		 HoudiniSetenv(const char *name,
					       const char *value);
SYS_DEPRECATED(14.0)
tools_API extern void		 HoudiniPutenv(const char *namevalue);
tools_API extern const char	*HoudiniGetenv(const char *name);
tools_API extern void		 HoudiniUnsetenv(const char *name);
tools_API extern char * const	*HoudiniGetenvlist();

#endif
