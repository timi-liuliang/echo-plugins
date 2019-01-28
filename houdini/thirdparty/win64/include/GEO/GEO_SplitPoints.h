/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        GEO_SplitPoints.h (GEO Library, C++)
 *
 * COMMENTS:    Functions for splitting points based on vertices or primitives
 */

#pragma once

#ifndef __GEO_SplitPoints__
#define __GEO_SplitPoints__

#include "GEO_API.h"
#include <GA/GA_Types.h>
#include <SYS/SYS_Types.h>

class GEO_Detail;
class GA_Range;
class GA_Attribute;

/// Returns the number of points added.
GA_Size GEO_API GEOsplitPointsByAttrib(
    GEO_Detail *detail,
    const GA_Range &points,
    const GA_Attribute *attrib,
    bool promotetopointattrib = false,
    fpreal tolerance = 0);

#endif
