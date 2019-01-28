/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_Prim.h (C++)
 *
 * COMMENTS:	The GU_Prim class is a the base class for which all
 *		the primitive classes are subclassed off of.  Common 
 *		utility operations are done here like conversion from one
 *		primitive to another.
 * 
 */

#ifndef __GU_ConvertParms_h__
#define __GU_ConvertParms_h__

#include "GU_API.h"

#include <GEO/GEO_ConvertParms.h>
#include <GA/GA_Detail.h>
#include <GA/GA_Range.h>

typedef GEO_ConvertStyle GU_ConvertStyle;
typedef GEO_MetaConvertStyleType GU_MetaConvertStyleType;
typedef GEO_ConvertMethod GU_ConvertMethod;
typedef GEO_ConvertParms GU_ConvertParms;
typedef GEO_MetaConvertParms GU_MetaConvertParms;


/// Simple helper class for tracking a range of new primitives and points
/// NOTE: This class is deprecated in favour of GA_Detail::OffsetMarker and
///       GA_IndexMap::Marker.
class GU_API GU_ConvertMarker
{
public:
    SYS_DEPRECATED(14.0)
    GU_ConvertMarker(const GA_Detail &geo)
	: marker(geo)
    {}

    SYS_DEPRECATED(14.0)
    GA_Range getPrimitives() const
    { return marker.primitiveRange(); }
    SYS_DEPRECATED(14.0)
    GA_Range getPoints() const
    { return marker.pointRange(); }

    SYS_DEPRECATED(14.0)
    GA_Offset	primitiveBegin() const	{ return marker.primitiveBegin(); }
    SYS_DEPRECATED(14.0)
    GA_Offset	pointBegin() const	{ return marker.pointBegin(); }

    SYS_DEPRECATED(14.0)
    GA_Size numPrimitives() const	{ return marker.primitiveEnd() - marker.primitiveBegin(); }
    SYS_DEPRECATED(14.0)
    GA_Size numPoints() const		{ return marker.pointEnd() - marker.pointBegin(); }

private:
    const GA_Detail::OffsetMarker marker;
};


/// Handle copying of attributes and groups from a primitive which contains a
/// single vertex to a range of destination primitives/points.
GU_API extern void
GUconvertCopySingleVertexPrimAttribsAndGroups(
	GU_ConvertParms &parms,
	const GA_Detail &src,
	GA_Offset src_primoff,
	GA_Detail &dst,
	const GA_Range &dst_prims,
	const GA_Range &dst_points);


#endif
