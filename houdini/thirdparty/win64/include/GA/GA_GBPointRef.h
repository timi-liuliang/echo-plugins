/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 * COMMENTS:    A couple utility functions used by selections.
 *              You'll probably want to use primgroup->combine(ptgroup) instead.
 */

#ifndef __GA_GBPointRef_H__
#define __GA_GBPointRef_H__

#include "GA_API.h"
#include "GA_Types.h"

class UT_BitArray;
class GA_Detail;
class GA_PointGroup;
class GA_PrimitiveGroup;

/// This function adds all the adjacent primitives of the pt_group to
/// prim_group within gdp. If given a restrict_group, then we use it to
/// restrict the list of primitives that we consider. The behaviour of this
/// group is determined by the restrict_if_contains flag. If this flag is true,
/// then only primitives in the restrict_group are considered. If the flag is
/// false, then only primitives _not_ in the restrict_group are used.
///
/// NOTE: You'll probably want to use primgroup->combine(ptgroup) instead.
GA_API extern void GAgetPrimitiveClosure(const GA_Detail &gdp,
					 const GA_PointGroup *pt_group,
					 GA_PrimitiveGroup *prim_group,
					 const GA_PrimitiveGroup *restrict_group = 0,
					 bool restrict_if_contains = true);
/// Same version of the above function except pt_group is a bit array with each
/// bit representing if the particular point is in the pt_group.
///
/// NOTE: You'll probably want to use primgroup->combine(ptgroup) instead.
GA_API extern void GAgetPrimitiveClosure(const GA_Detail &gdp,
					 const UT_BitArray &pt_array,
					 GA_PrimitiveGroup *prim_group,
					 const GA_PrimitiveGroup *restrict_group = 0,
					 bool restrict_if_contains = true);

#endif
