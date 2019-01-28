/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	tools library (C++)
 *
 * COMMENTS:	Houdini path searching.
 *		With HoudiniFindFile, if the result passed in is a null
 *		pointer, then the return code will be an allocated string.
 *		Otherwise, the return code will be a pointer to result
 *		if the file is found.  If no file is found, then the
 *		return code will be 0.
 *		HoudiniMakePath() will create all the sub-directories in
 *		the path specified.  For example, if passed /usr/tmp/foo/bar
 *		There will be a directory foo created in /usr/tmp, and also
 *		a directory bar in foo.  0 is returned on failure
 *
 */

#ifndef __hpath_h__
#define __hpath_h__

#include "tools_API.h"

#ifdef WIN32
    typedef int mode_t;
    #define PATH_SEP_CHAR ';'
    #define PATH_SEP_STRING ";"
    #define PATH_IS_SEP_CHAR(c)	((c) == ';')
#else
    #define PATH_SEP_CHAR ':'
    #define PATH_SEP_STRING ":"
    #define PATH_IS_SEP_CHAR(c)	((c) == ':' || (c) == ';')
#endif

#include <sys/types.h>

tools_API extern int HoudiniMakeDirectory(const char *file, mode_t mode);

#endif
