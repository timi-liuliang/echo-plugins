/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO_PointSlider.h (GEO Library, C++)
 *
 * COMMENTS:	Virtual base classes for managing the data structures for
 * 		sliding points across a subset of primitives.
 */

#ifndef __GEO_PointSlider_h__
#define __GEO_PointSlider_h__

#include "GEO_API.h"

#include <GA/GA_Types.h>
#include <UT/UT_VectorTypes.h>

class GA_PrimitiveGroup;

class GEO_API GEO_PointSlider
{
public:
             GEO_PointSlider();
    virtual ~GEO_PointSlider();

    virtual bool project(GA_Offset point,
			 const GA_OffsetArray *referencing_prims,
			 const UT_Vector3 &start_pos,
			 const UT_Vector3 &target_pos, 
			 UT_Vector3 &projected_pos) const = 0;

    virtual bool usesPrimitivesReferencingPoint() const { return false; }
};

class GEO_API GEO_PointSliderCache
{
public:
             GEO_PointSliderCache();
    virtual ~GEO_PointSliderCache();

    virtual GEO_PointSlider	*allocSlider(
					const GA_PrimitiveGroup *group) = 0;
    virtual void		 releaseSlider(
					GEO_PointSlider *slider) = 0;
};

#endif // __GEO_PointSlider_h__

