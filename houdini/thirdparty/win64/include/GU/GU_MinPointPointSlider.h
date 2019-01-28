/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_MinPointPointSlider.h (GU Library, C++)
 *
 * COMMENTS:	Classes for managing the GU_RayIntersect for sliding points
 * 		across a subset of primitives using minimumPoint().
 */

#ifndef __GU_MinPointPointSlider_h__
#define __GU_MinPointPointSlider_h__

#include "GU_API.h"

#include <GEO/GEO_PointSlider.h>

class GU_Detail;

class GU_API GU_MinPointPointSliderCache : public GEO_PointSliderCache
{
public:
             GU_MinPointPointSliderCache(const GU_Detail &gdp);
    virtual ~GU_MinPointPointSliderCache();

    virtual GEO_PointSlider	*allocSlider(
					const GA_PrimitiveGroup *group);
    virtual void		 releaseSlider(
					GEO_PointSlider *slider);
private:
    const GU_Detail 	&myGdp;
};

#endif // __GU_MinPointPointSlider_h__

