/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	IMG_Tags.h
 *
 * COMMENTS:	Contains a lists of known global and plane tags for
 *		IMG_TileDevice.
 */

#ifndef __IMG_Tags__
#define __IMG_Tags__

#include "IMG_API.h"
#include <UT/UT_StringHolder.h>

/// An enum of common tags for communicating with image tile devices (see
/// IMG_TileDevice::writeCustomTag().
enum IMG_TagType
{
    IMG_TAG_INVALID = -1,
    IMG_TAG_SOURCE_NAME,	// "sourcename" <name>
    IMG_TAG_PROGRESS,		// "progress" <percentage>
    IMG_TAG_PROGRESS_MSG,	// "progressmessage" <str>
    IMG_TAG_PROGRESS_ACTION,	// "progressaction" <str>
    IMG_TAG_STATUS_MSG,		// "statusmessage" <str>
    IMG_TAG_S3D_LEFT_PLANE,	// "s3drightplane" <plane_name>
    IMG_TAG_S3D_RIGHT_PLANE,	// "s3dleftplane" <plane_name>
    IMG_TAG_RENDER_MODE,	// "rendermode" <[new-]append|[new-]replace>
    IMG_TAG_RENDER_TIME,	// "rendertime" <final_time_str>
    IMG_TAG_ELAPSED_TIME,       // "elapsedtime" <so_far_time_str>
    IMG_TAG_PEAK_MEM_USAGE,     // "peakmemusage" <peak_mem_usage>
    IMG_TAG_TILE_BEGIN,		// "tilebegin" <tile ID> <x0> <x1> <y0> <y1>
				//             <red> <green> <blue> <alpha>
    IMG_TAG_TILE_DONE,		// "tiledone" <tile ID>
    IMG_TAG_CURRENT_FRAME,	// "currentframe" <frame_no>
    IMG_TAG_FRAME_RANGE,	// "framerange" <start> <end>
    IMG_TAG_FRAME_RANGE_OLD,	// "frange" <start> <end>
    IMG_TAG_MISSING_MODE,	// "missingmode" <black|prev|next|closest>
    IMG_TAG_AUDIO,		// "audio"	<file> [<frame_offset> [...]]
    IMG_TAG_BACKGROUND,		// "background" <file>
    IMG_TAG_FLIPBOOK,		// "flipbook"
    IMG_TAG_HOUDINI_PORT,	// "houdiniportnum" <port_no>
    IMG_TAG_BLOCK_EDIT,		// "blockedit"
    IMG_TAG_BLOCK_CHANNELS,	// "blockchannels"
    IMG_TAG_CROP_WINDOW,	// "cropwindow" <x> <y> <w> <h>
    IMG_TAG_EDGE_STREAK,	// "edgestreak"
    IMG_TAG_ASPECT,		// "aspect" <aspect>
    IMG_TAG_PIXEL_ASPECT,	// "pixelaspect" <aspect>
    IMG_TAG_BLACK_POINT,	// "blackpoint" <value>
    IMG_TAG_WHITE_POINT,	// "whitepoint" <value>
    IMG_TAG_GAMMA,		// "gamma"	 <gamma_exp>
    IMG_TAG_LUT,		// "LUT" <lut_name>
    IMG_TAG_COLOR_SPACE,	// "colorspace" <csname> <gammaval>

    IMG_TAG_EXR_DATA_WINDOW,    // "exrdatawindow" <padding> <threshold> <planes>

    IMG_TAG_PRIORITY_CIRCLE,    // "prioritycircle" <centrex> <centrey> <radius>

    IMG_TAG_STYLE_SHEET,	// "stylesheets" [<style sheets> [...]]
    IMG_TAG_ENV_VARIABLE,	// "envvar" var value
    IMG_TAG_DENOISE_PLANE,	// "denoiseplane" <denoisername> <plane_name> [...]

    IMG_TAG_KNOWN_TAGS		// Number of known tags
};

/// This will a unique identifier for the given token.  If it's a known token,
/// it will be one of the enums above.  If it's a custom tag, it will be >=
/// IMG_TAG_KNOWN_TAGS.
IMG_API extern int		IMGtagId(const char *token);

/// Return the string associated with a given ID.  The string will be
/// "<invalid>" if the tag-id is unknown.
IMG_API extern UT_StringHolder	IMGtagName(int id);

#endif // __IMG_Tags__
