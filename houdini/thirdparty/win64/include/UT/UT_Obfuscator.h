/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        UT_Obfuscator.h
 *
 * COMMENTS:
 *		This is a simple class that encodes and decodes ASCII
 *		strings. It uses a simple substitution cipher, so it
 *		is by no  means "secure" in the same way true encryption
 *		is, and shouldn't be relied on exclusively for protection.
 *		While it uses a fixed table generated based on a seed,
 *		it can also take in a secondary key which it uses to permute
 *		the string to be encoded. Note that to decode the string back,
 *		the same key must be passed in.
 *
 *		Please note that it currently only supports [A-Z][a-z][0-9] and
 *		'_' characters. Any other characters will not be substituted and
 *		will remain unchanged.
 * 
 *		The result of the encoding is guaranteed to be a valid ASCII string.
 */


#ifndef __UT_Obfuscator_h__
#define __UT_Obfuscator_h__

#include "UT_API.h"

#define NUM_LETTERS	26
#define NUM_DIGITS	10
// +1 for underscore
#define TOTAL_MAP_LEN	(NUM_LETTERS*2 +  NUM_DIGITS + 1)


class UT_String;
class UT_WorkBuffer;


/*****************************************************************************/
class UT_API UT_Obfuscator
{
public:
    UT_Obfuscator();
    ~UT_Obfuscator();

    void encode(UT_String& str_in_out, const char* extra_key_in = nullptr);

    void decode(UT_String& str_in_out, const char* extra_key_in = nullptr)
	    { decodeImpl(str_in_out, extra_key_in); }
    void decode(UT_WorkBuffer& str_in_out, const char* extra_key_in = nullptr)
	    { decodeImpl(str_in_out, extra_key_in); }

private:

    void reset();

    void generateMap();
    int getIndexForChar(char chr);

    void factorInExtraKey(UT_String& str_in_out, const char* extra_key);

    template <typename StringT>
    void factorOutExtraKey(StringT& str_in_out, const char* extra_key);
    template <typename StringT>
    void decodeImpl(StringT& str_in_out, const char* extra_key_in);

private:

    char myMap[TOTAL_MAP_LEN];
    char myRevMap[TOTAL_MAP_LEN];
};
/*****************************************************************************/
#endif
