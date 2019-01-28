/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_CharSetUtils.h (UT Library, C++)
 *
 * COMMENTS:
 *		Utilities for encoding, decoding, and converting text
 *		from, to, and between character sets and other encodings.
 */

#ifndef __UT_CharSetUtils__
#define __UT_CharSetUtils__

#include "UT_API.h"

class UT_String;
class UT_StringHolder;
class UT_WorkBuffer;


class UT_API UT_CharSetUtils
{
public:
    // The encoding of the source code strings (ie, the char* stuff)
    // Currently it specifies ISO-9959-1 (Latin 1) char set.
    static const char * const	SRC_CODE_ENCODING;

    // The encoding used in the XML file. Currently it specifies UTF-8.
    static const char * const	XML_ENCODING;

    /// Conversion method from the source encoding to UTF-8 Unicode encoding.
    /// src_str	     - the input string bytestream in the src_encoding
    /// out_str	     - the output string bytesream in the UTF-8 encoding.
    /// src_encoding - always Latin 1
    //
    /// Returns true if ok, false otherwise.
    static bool	    encodeUTF8( const char * src_str, 
				UT_WorkBuffer & out_str );

    /// Conversion method from the source encoding of UTF-8 to the encoding
    /// specified by the out_encoding.
    /// src_str	     - the input string bytestream in the UTF-8 encoding.
    /// out_str	     - the output string bytesream in the out_encoding.
    /// out_encoding - always Latin 1
    //
    /// Returns true if ok, false otherwise.
    /// @{
    static bool	    decodeUTF8( const char * src_str,
				UT_WorkBuffer & out_str );
    static bool	    decodeUTF8( const char * src_str,
				UT_String & out_str );
    static bool	    decodeUTF8( const char * src_str,
				UT_StringHolder & out_str );
    /// @}
};

#endif

