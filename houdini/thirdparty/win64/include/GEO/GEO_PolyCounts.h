/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        GEO_PolyCounts.h (GEO Library, C++)
 *
 * COMMENTS:    Implementation of a simple run-length encoded array
 *              intended for storing polygon vertex counts.
 */

#ifndef __GEO_PolyCounts__
#define __GEO_PolyCounts__

#include "GEO_API.h"

#include <GA/GA_PolyCounts.h>

class GEO_API GEO_PolyCounts : public GA_PolyCounts
{
public:
    GEO_PolyCounts()
        : GA_PolyCounts()
    {}

    GEO_PolyCounts(const GEO_PolyCounts &that)
        : GA_PolyCounts(that)
    {}

    GEO_PolyCounts &operator=(const GEO_PolyCounts &that)
    {
        GA_PolyCounts::operator=(that);
        return *this;
    }
};

#endif
