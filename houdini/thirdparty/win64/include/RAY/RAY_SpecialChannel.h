/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:     RAY_SpecialChannel.h (RAY Library, C++)
 *
 * COMMENTS: Special image plane channel types
 *
 */

#pragma once

#ifndef __RAY_SpecialChannel_h__
#define __RAY_SpecialChannel_h__

// Special imager channels
enum RAY_SpecialChannel
{
    RAY_SPECIAL_NONE,
    RAY_SPECIAL_CFAF,
    RAY_SPECIAL_PZ,
    RAY_SPECIAL_STYID,
    RAY_SPECIAL_OPID,
    RAY_SPECIAL_PRIMID,
    RAY_SPECIAL_SUBDID,
    RAY_SPECIAL_GROUPID,
    RAY_SPECIAL_MATERIALID,
    RAY_SPECIAL_COMBINEDIDS,
    RAY_SPECIAL_RTIME,
    RAY_SPECIAL_SHADINGSAMPLES,
    RAY_SPECIAL_OPACITYSAMPLES,
    RAY_SPECIAL_PIXELSAMPLES,
    RAY_SPECIAL_MAX
};

#endif
