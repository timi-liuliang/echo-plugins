/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 *	See: https://tools.ietf.org/html/rfc3492
 *
 */

#ifndef __UT_Punycode__
#define __UT_Punycode__

#include "UT_API.h"
#include <SYS/SYS_Types.h>

class UT_String;
class UT_StringHolder;
class UT_StringRef;
class UT_WorkBuffer;
template <typename T> class UT_Array;


class UT_API UT_Punycode
{
public:
    /// @{ Encodes an expanded source string @p src into Punycode encoding
    /// that uses just base (ASCII) code points. The src is a zero-terminated
    /// array of utf32 code points. The @dst is the encoded result.
    /// Returns true on success; false otherwise.
    static bool	encode(UT_WorkBuffer &puny_dst, const utf32 *utf32_src);
    static bool	encode(UT_WorkBuffer &puny_dst, const UT_StringRef &utf8_src);
    /// @}

    /// @{ Decodes a source string @p src encoded using Punycode. The decoded
    /// string is returned in @p dst but is NOT zero-terminated (this can
    /// be done explicitly in the caller, if needed).
    /// Returns true on success; false otherwise.
    static bool	decode(UT_Array<utf32> &utf32_dst,const UT_StringRef &puny_src);
    static bool	decode(UT_WorkBuffer &utf8_dst, const UT_StringRef &puny_src);
    /// @}

    /// @{ Encodes the @p utf8 src string (usually a word/token) into 
    /// a valid C-style variable name, ie, a string containing only 
    /// alphanumeric and underscore characters.  
    /// The decode function reverses the encoding.
    static bool	encodeToVarName(UT_WorkBuffer &dst, const UT_StringRef &src);
    static bool	decodeFromVarName(UT_WorkBuffer &dst, const UT_StringRef &src);
    /// @}
    
    /// @{ Returns an encoded variable name, if @var contains any charactedrs 
    /// illegal in a C-style variable name. Note, the encoding is prefixed.
    /// If @var contains only valid C-variable characters, returns it unencoded.
    static UT_StringHolder  encodeToVarNameIfNeeded(const UT_StringHolder &var);
    static UT_StringHolder  decodeVarNameIfNeeded(const UT_StringHolder &var);
    static void	encodeToVarNameIfNeeded(UT_String &var);
    static void decodeVarNameIfNeeded(UT_String &var);
    /// @}
};

#endif
