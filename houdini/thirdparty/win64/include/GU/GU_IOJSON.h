/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_IOJSON.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GU_IOJSON__
#define __GU_IOJSON__

#include "GU_API.h"
#include <GA/GA_IOJSON.h>

/// @brief Extension of GA_IOJSON which saves GU specific properties
///
/// At the current time, there are no specific values saved with GU details.
class GU_API GU_IOJSON : public GA_IOJSON
{
public:
	     GU_IOJSON() {}
    virtual ~GU_IOJSON() {}

protected:
    bool	jsonLoadExtra(GA_Detail &gdp, UT_JSONParser &p,
				const char *token, const GA_LoadMap &map) const;
    bool	jsonSaveExtra(const GA_Detail &gdp, UT_JSONWriter &w,
				const GA_SaveMap &map) const;

    bool	finishLoad(GA_Detail &gdp, const GA_LoadMap &map) const;
};

#endif

