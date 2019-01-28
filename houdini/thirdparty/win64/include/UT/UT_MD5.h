/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        UT_MD5.h
 *
 * COMMENTS:
 *	Modified by Side Effects Software Inc. to add an easy-to-use
 *	interface, to port to 64-bit platforms, and to add base-64 output.
 *
 *	See RFC 1321 for information on how the algorithm works.
 */

/*
 **********************************************************************
 ** RSA Data Security, Inc. MD5 Message Digest Algorithm             **
 ** Created: 2/17/90 RLR                                             **
 ** Revised: 12/27/90 SRD,AJ,BSK,JT Reference C version              **
 ** Revised (for MD5): RLR 4/27/91                                   **
 **   -- G modified to have y&~z instead of y&z                      **
 **   -- FF, GG, HH modified to add in last register done            **
 **   -- Access pattern: round 2 works mod 5, round 3 works mod 3    **
 **   -- distinct additive constant for each step                    **
 **   -- round 4 added, working mod 7                                **
 **********************************************************************
 */

/*
 **********************************************************************
 ** Copyright (C) 1990, RSA Data Security, Inc. All rights reserved. **
 **                                                                  **
 ** License to copy and use this software is granted provided that   **
 ** it is identified as the "RSA Data Security, Inc. MD5 Message     **
 ** Digest Algorithm" in all material mentioning or referencing this **
 ** software or this function.                                       **
 **                                                                  **
 ** License is also granted to make and use derivative works         **
 ** provided that such works are identified as "derived from the RSA **
 ** Data Security, Inc. MD5 Message Digest Algorithm" in all         **
 ** material mentioning or referencing the derived work.             **
 **                                                                  **
 ** RSA Data Security, Inc. makes no representations concerning      **
 ** either the merchantability of this software or the suitability   **
 ** of this software for any particular purpose.  It is provided "as **
 ** is" without express or implied warranty of any kind.             **
 **                                                                  **
 ** These notices must be retained in any copies of any part of this **
 ** documentation and/or software.                                   **
 **********************************************************************
 */

#ifndef __UT_MD5_h__
#define __UT_MD5_h__

#include "UT_API.h"
class UT_WorkBuffer;

// Return a 32-character null-terminated base-16 string representing the
// 128-bit message digest of the given string.
UT_API extern void UT_md5(const char *input, char result[33]);
UT_API extern void UT_md5(const char *input, UT_WorkBuffer &result);

// The version returns a 22-character base-64 string of the message digest.
UT_API extern void UT_md5_base64(const char *input, char result[23]);
UT_API extern void UT_md5_base64(const char *input, UT_WorkBuffer &result);

#endif
