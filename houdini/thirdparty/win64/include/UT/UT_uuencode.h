/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Stream methods to uuencode & uudecode a stream (C++)
 *
 * COMMENTS:
 *
 */

#ifndef __UT_uuencode_h__
#define __UT_uuencode_h__

#include "UT_API.h"
#include <iosfwd>

class UT_IStream;
class UT_String;

//
// Encode a binary istream and write the uuencoded data to os.
//	The file permissions are defined by mode.
//	This will read from is until it's out of data...
UT_API extern bool	UTuuencode(UT_IStream &is, std::ostream &os,
			   const char *name, int mode = 0666);

//
// Take a uuencoded istream and write the uudecoded data to os
//	the filename is returned in the UT_String.  If the mode
//	is not a null pointer, it will be set to the mode of the
//	file.  This will terminate when the "end" statement is reached
UT_API extern bool	UTuudecode(UT_IStream &is, std::ostream &os,
			   UT_String &name, int *mode = 0);

#endif
