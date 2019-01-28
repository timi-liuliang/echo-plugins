/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_IOS.h (UT Library, C++)
 *
 * COMMENTS:
 *
 */

#ifndef __UT_IOS_H_INCLUDED__
#define __UT_IOS_H_INCLUDED__

enum UT_IOS_TYPE
{
    UT_IOS_ASCII  = 0,		// ASCII stream
    UT_IOS_BINARY = 1		// Binary stream
};

enum UT_IOS_SEEK
{
    UT_IOS_SEEK_BEG = 0,	// Seek from beginning of stream
    UT_IOS_SEEK_CUR = 1,	// Seek from current location
    UT_IOS_SEEK_END = 2		// Seek from the end of the stream
};

#endif // __UT_IOS_H_INCLUDED__
