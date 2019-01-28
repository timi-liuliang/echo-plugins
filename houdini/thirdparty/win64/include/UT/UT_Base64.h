/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Base64.h ( UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_Base64__
#define __UT_Base64__

#include "UT_API.h"

#include <SYS/SYS_Types.h>

class UT_WorkBuffer;
class UT_IStream;

class UT_API UT_Base64
{
public:
    /// @{
    /// Encode a stream of binary into base64 encoding. If the input is not
    /// a multiple of three, padding is added to indicate proper size.
    static void encode(const UT_WorkBuffer &src, UT_WorkBuffer &dst);
    static void encode(const uint8 *src, exint src_len, UT_WorkBuffer &dst);
    /// @}

    /// @{
    /// Decode a base64 encoded string into binary. Returns @c false if
    /// decoding failed (bad input character, or wrong padding). Whitespace
    /// is ignored on parsing.
    static bool decode(const UT_WorkBuffer &src, UT_WorkBuffer &dst);
    static bool decode(const char *src, UT_WorkBuffer &dst);
    static bool decode(UT_IStream &is, UT_WorkBuffer &dst);
    /// @}
};

#endif // __UT_Base64__
