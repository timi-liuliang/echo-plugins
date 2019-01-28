/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:     SYS_BoostStreams.h
 *
 * COMMENTS: This header should be included by anyone wanting to use
 * 	     boost streams, rather than including the boost header directly.
 * 	     It provied a namespace mapping and avoids inclusion on platforms
 * 	     that don't support boost streams.
 */

#ifndef __SYS_BoostStreams_h__
#define __SYS_BoostStreams_h__

#include <hboost/iostreams/filtering_stream.hpp>
#include <hboost/iostreams/filter/gzip.hpp>
#include <hboost/iostreams/device/mapped_file.hpp>
#include <hboost/iostreams/device/file_descriptor.hpp>
namespace bios = hboost::iostreams;

#endif
