/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	POP_GBElement.h ( GA Library, C++)
 *
 * COMMENTS:
 * 	Class to maintain old GB/GD/GEO/GU/ interfaces to elements.  For
 * 	points and vertices, instances are mere interface stubs wrapping
 * 	an index map and an offset.  GA_Primitive, however, derives from
 * 	GA_GBElement to permit maximum legacy compatibility.
 *	
 *	This class provides methods from GB_Element, GB_AttributeElem, as
 *	well as utility methods to aid in implementing the interfaces of
 *	GB_Vertex (and its subclasses) and [GD|GEO]_Point.
 */

#pragma once

#ifndef __POP_GBElement__
#define __POP_GBElement__

#include "POP_API.h"

#include <GA/GA_GBElement.h>

/// Simple wrapper to create a handle to access detail attributes in the same
/// fashion as other GB elements.  The GA_GBDetailElement acts as a "handle" to
/// the underlying detail, but allows for access using methods like: @code
///   GA_GBDetailElement det(gdp);
///   UT_Vector3         clr = det.getValue<UT_Vector3>(GA_ROAttributeRef &h);
/// @endcode;
class POP_API GA_GBDetailElement : public GA_GBElement
{
public:
    SYS_DEPRECATED_HDK(13.0)
    GA_GBDetailElement(const GA_Detail &gdp)
	: GA_GBElement(gdp.getIndexMap(GA_ATTRIB_GLOBAL), GA_Offset(0)) {}
    SYS_DEPRECATED_HDK(13.0)
    GA_GBDetailElement(const GA_GBDetailElement &src)
	: GA_GBElement(src) {}
    SYS_DEPRECATED_HDK(13.0)
    ~GA_GBDetailElement() {}
};


#endif
