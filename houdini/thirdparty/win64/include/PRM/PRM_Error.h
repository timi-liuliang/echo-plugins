/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PRM_Error.h (Utility Library, C++)
 *
 * COMMENTS:
 *		These are types of errors that are not library specific
 *		eg, file not found.
 */

#ifndef __PRM_Error__
#define __PRM_Error__

//
// These are the error codes. Make sure that the error is not
// one that should be handled by UT_SystemError before adding it
// here.
//
enum PRM_ErrorCode
{
    PRM_ERROR_BAD_TXTFILE = 0,
    PRM_ERROR_BAD_BINFILE,
    PRM_ERROR_SHORT_INPUT_VECTOR,
    PRM_ERROR_LONG_INPUT_VECTOR,
    PRM_ERROR_BAD_CHANNEL,
    PRM_ERROR_UNKNOWN_PARM,
    PRM_ERROR_UNKNOWN_ORDINAL,
    PRM_ERROR_BAD_INTEGER,
    PRM_ERROR_IO_ERROR,
    PRM_ERROR_PARSE_NOT_FOUND,
    PRM_ERROR_NO_CHANNEL,
    PRM_ERROR_BAD_VERSION,
    PRM_ERROR_NIL_PARMPTR,
    PRM_ERROR_NEW_VERSION,
    PRM_ERROR_NO_CHGROUP,
    PRM_ERROR_PRESET_BAD_MAGIC,
    PRM_ERROR_PRESET_PARSE_NOT_FOUND,
    PRM_ERROR_BAD_PRESET_VERSION,
    PRM_WARNING_CANNOT_BE_TIME_DEP,
    PRM_WARNING_CANT_ADD_MULTIPARM,
    PRM_ERROR_INVALID_ELEM_INDEX,
};

#endif
