/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	ROP_Error.h
 *
 * COMMENTS:
 *		This class is used to store ROP errors or warnings.
 *
*/

#ifndef __ROP_Error__
#define __ROP_Error__

enum ROP_ErrorCodes
{
    ROP_NO_OUTPUT		= 0,
    ROP_COOK_ERROR		= 1,
    ROP_SAVE_ERROR		= 2,
    ROP_MEMORY			= 3,
    ROP_RENDER_SAVE		= 4,
    ROP_BAD_CONTEXT		= 5,
    ROP_FILE_EXISTS		= 6,
    ROP_RENDER_ERROR		= 7,
    ROP_LOAD_ERROR		= 8,
    ROP_UNKNOWN_RENDERER	= 9,
    ROP_CONNECTION_ERROR	= 10,
    ROP_EXECUTE_ERROR		= 11,
    ROP_NONCOMMERCIAL_ERROR	= 12,
    ROP_TOOLARGE_ERROR		= 13,
    ROP_COMMAND_CRASH		= 14,
    ROP_BAD_COMMAND		= 15,
    ROP_MISSING_CAMERA		= 16,
    ROP_IPR_INPROGRESS		= 17,
    ROP_IRMA_BLOCKED		= 18,
    ROP_MISSING_FILE		= 19,
    ROP_NO_SOHO_PROGRAM		= 20,
    ROP_NO_SOHO_OUTPUT		= 21,
    ROP_NO_SOHO_TDISK		= 22,
    ROP_MESSAGE			= 23,
    ROP_SOHO_AUTOHEADLIGHT	= 24,
    ROP_POINTSCHANGED_ERROR	= 25,
    ROP_BACKWARDS_RENDER	= 26,
    ROP_MISSING_SCENE		= 27,
    ROP_OPENGL_FBO_UNSUPPORTED	= 28,
    ROP_OPENGL_FBO_TOO_LARGE	= 29,
    ROP_PREPOST_NO_RENDER_ROP   = 30,
    ROP_PREPOST_SAME_PRE_POST_ROP = 31,
    ROP_PREPOST_RENDER_IS_PRE_POST = 32,
    ROP_PREPOST_PRE_ROP_USED	= 33,
    ROP_PREPOST_POST_ROP_USED	= 34,
    ROP_CREATE_DIRECTORY_FAIL	= 35,

    ROP_NUM_ERROR_CODES
};

#endif
