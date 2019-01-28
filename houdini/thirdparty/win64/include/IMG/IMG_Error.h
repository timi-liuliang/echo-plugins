/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	IMG_Error.h (C++)
 *
 * COMMENTS:
 *		This class is used to store IMG errors or warnings.
 *
*/

#ifndef __IMG_Error__
#define __IMG_Error__

enum IMG_ErrorCodes
{
    IMG_ERR_NO_SUCH_FILE	= 0,	// file doesn't exist
    IMG_ERR_INVALID_FILE	= 1,	// unknown file type
    IMG_ERR_FAILED_CREATE	= 2,	// couldn't create file
    IMG_ERR_NO_TRANSLATOR	= 3,	// unsupported translation required
    IMG_ERR_FAILED_READ		= 4,	// scanline read failed
    IMG_ERR_FAILED_WRITE	= 5,	// scanline write failed
    IMG_ERR_FAILED_CLOSE	= 6,	// close operation failed
    IMG_ERR_READ_WRITEFILE	= 7,	// tried to read a write mode file
    IMG_ERR_WRITE_READFILE	= 8,	// tried to write a read mode file
    IMG_ERR_YUV_WRONG_RES	= 9,	// tried to save .yuv at wrong res
    IMG_WARN_PIC_VERSION	= 10,	// pic is higher version than reader
    IMG_ERROR_GENERIC		= 11,	// generic warning/error message
};

#endif
