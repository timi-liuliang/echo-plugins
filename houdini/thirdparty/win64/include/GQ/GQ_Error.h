/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GQ_Error.h (C++)
 *
 * COMMENTS:
 *
 */

#ifndef __GQ_Error__
#define __GQ_Error__

enum GQ_ErrorCodes
{
    GQ_ERROR_FAIL       = 0,    // Unknown failure reason
    GQ_ERROR_DEGEN,
    GQ_ERROR_BOOLEAN_A_NOT_CLOSED,
    GQ_ERROR_BOOLEAN_B_NOT_CLOSED,
    GQ_ERROR_BOOLEAN_A_NOT_POLY,
    GQ_ERROR_BOOLEAN_B_NOT_POLY,
    GQ_ERROR_BOOLEAN_PRECISION,
    GQ_ERROR_SKELETON_BAD_TRIANGULATION,
    GQ_WARNING_SKELETON_DUPLICATE_POINTS,
    GQ_WARNING_SKELETON_FALLBACK_INSIDE_OUTSIDE,
    GQ_WARNING_SKELETON_BAD_PARTITION_OF_COLLAPSED_POLY,
    GQ_WARNING_SKELETON_FLIP_EVENTS_ABORTED
};


#endif
