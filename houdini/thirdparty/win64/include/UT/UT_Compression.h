/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Compression.h ( UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef UT_COMPRESSION_H_
#define UT_COMPRESSION_H_

#include "UT_String.h"

enum UT_CompressionType
{
    UT_COMPRESSION_TYPE_NONE = 0,
    UT_COMPRESSION_TYPE_GZIP,
    UT_COMPRESSION_TYPE_BLOSC
};

#define UT_BLOSC_BLOCKSIZE (1048576)
#define UT_BLOSC_TYPESIZE (sizeof(float))
#define UT_BLOSC_COMPRESSION_LEVEL (9)

/// Converts the string we get from the parameter into an
/// enumeration that is used interally
/// The strange names for none and gzip (off and on) are for
/// backwards compatability with pervious compression parameter.
inline UT_CompressionType
UTcompressionTypeFromString(const UT_String& string)
{
    if (string == "blosc")
    {
	return UT_COMPRESSION_TYPE_BLOSC;
    }
    else if (string == "off" || string == "none")
    {
	return UT_COMPRESSION_TYPE_NONE;
    }
    else if (string == "on" || string == "gzip")
    {
	return UT_COMPRESSION_TYPE_GZIP;
    }
    else
    {
	UT_ASSERT(false && "Compression type not known. "
	"You probably added the type to OBJ_Shared.C and not here.");
	// If not assert then do default.
	return UT_COMPRESSION_TYPE_BLOSC;
    }
}


#endif /* UT_COMPRESSION_H_ */
