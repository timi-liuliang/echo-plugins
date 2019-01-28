/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GD_IOJSON.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GD_IOJSON__
#define __GD_IOJSON__

#include "GD_API.h"
#include <GA/GA_IOJSON.h>

/// @brief Extension of GA_IOJSON which saves GD specific properties
///
/// The GD_IOJSON saves two additional keywords
///  - "altitude"     @n The altitude of the trim profiles/curves
///  - "trimregions"  @n Trim region sections
class GD_API GD_IOJSON : public GA_IOJSON
{
public:
	     GD_IOJSON() {}
    virtual ~GD_IOJSON() {}

protected:
    bool	jsonLoadExtra(GA_Detail &gdp, UT_JSONParser &p,
				const char *token, const GA_LoadMap &map) const;
    bool	jsonSaveExtra(const GA_Detail &gdp, UT_JSONWriter &w,
				const GA_SaveMap &map) const;
};

#endif

