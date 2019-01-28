/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_MergeUtils.h ( GU Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GU_MergeUtils__
#define __GU_MergeUtils__

#include "GU_API.h"
#include <UT/UT_Array.h>

class GU_Detail;

/// Merge multiple geometry details into a single detail.
///
/// Before merging, this function will perform analysis on the input details to
/// ensure that attributes will be promoted to a level which will prevent any
/// loss of data.
///
/// For example, if one source detail has a point attribute which matches a
/// vertex attribute on another source, the attribute will be promoted to a
/// vertex attribute before merging.
///
/// In addition, if any geometry has detail attributes which have different
/// values from other geometry detail attributes, the attributes will be
/// promoted to primitive attributes to preserve the information.
///
/// @note The source details must be non-const since attributes may be promoted.
GU_API bool	GUmatchAttributesAndMerge(GU_Detail &dest,
				const UT_Array<GU_Detail *> &sources);

/// Perform attribute matching and merging on two details.
GU_API bool	GUmatchAttributesAndMerge(GU_Detail &dest, GU_Detail &src);

/// Perform attribute matching and merging on two details.  This may be
/// significantly more expensive since the source detail is considered
/// constant.  If the source is constant and attributes need to be promoted,
/// the source is duplicated before merging.
GU_API bool	GUexpensiveMatchAndMerge(GU_Detail &dest, const GU_Detail &src);

/// Perform attribute matching and merging on an array of details.
GU_API bool	GUmatchAttributesAndMerge(GU_Detail &dest,
				GU_Detail *const*sources,
				int nsources);

#endif
