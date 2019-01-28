/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Defines.h (C++)
 *
 * COMMENTS:    Common defines and typedefs for UT_Library 
 *
 */

#ifndef _UT_Defines_
#define _UT_Defines_

// DEFINES

#define UT_GIANTBUF	16384
#define UT_BIGBUF	8192
#define UT_MIDBUF	4096
#define UT_SMALLBUF	1024
#define UT_TINYBUF	512
// UT_NUMBUF is the maximum number of digits required by itoa()
// It is sizeof(int64)*8 + 1 for the terminating '\0'
#define UT_NUMBUF	65
#define UT_BUFSIZE	UT_BIGBUF
#define UT_NULL		0
#define UT_FALSE	0
#define UT_TRUE		1
#define UT_MAXORDER	11


// A character used for separating the index file path from a section name.
#define UT_SECTION_SEPARATOR_CHAR	'?'
#define UT_SECTION_SEPARATOR_STRING	"?"

// A file protocol prefix for referencing an OTL library given an operator
// type name. Eg, "oplib:/Object/myhda".
#define UT_OTL_LIBRARY_PREFIX		"oplib:"

// A file protocol prefix for referencing an HDA definition given an operator 
// type name. Eg, "opdef:/Object/myhda".
#define UT_HDA_DEFINITION_PREFIX	"opdef:"


// TYPEDEFS
typedef int		UT_Bool;
#ifdef WIN32
typedef int		pid_t;
#endif

#endif
