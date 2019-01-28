/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        IMG_DataWindow.h (IMG Library, C++)
 *
 * COMMENTS:    Functions for finding and recording window of non-zero
 *              data in existing images.
 */

#pragma once

#ifndef __IMG_DataWindow__
#define __IMG_DataWindow__

#include "IMG_API.h"
#include <SYS/SYS_Types.h>
#include <iosfwd>

class IMG_File;
class UT_DimRectImpl;
template <typename T> class UT_Rect;
typedef UT_Rect<UT_DimRectImpl> UT_DimRect;
class UT_Options;

/// Finds the window of data above the thresholdvalue in the specified file.
/// Returns true if the window is less than the full image rectangle.
/// NOTE: The only reason the IMG_File is non-const is because of
///       IMG_File::read being non-const.
bool IMG_API IMGfindDataWindow(
    IMG_File *file,
    UT_DimRect &datawindow,
    fpreal thresholdvalue = 0.0,
    std::ostream *error_stream = NULL
);

/// Returns true if it succeeded and false otherwise.
bool IMG_API IMGfindAndSetDataWindow(
    const char *input_filename,
    const char *output_filename = NULL,
    fpreal render_time = -1,
    int64 render_memory = 0,
    bool compute_data_window = true,
    int pad = 0,
    bool crop_to_window = false,
    bool crop_to_window_if_unsupported = false,
    const char *src_planes = "C A",
    fpreal threshold_value = 0.0,
    bool skip_if_window_not_needed = false,
    std::ostream *error_stream = NULL,
    std::ostream *output_stream = NULL,
    bool info_only = false,
    UT_Options *extra_opt = NULL
);

#endif
