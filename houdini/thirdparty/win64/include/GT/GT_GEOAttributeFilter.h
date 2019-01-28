/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_GEOAttributeFilter.h ( GEO Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_GEOAttributeFilter__
#define __GT_GEOAttributeFilter__

#include "GT_API.h"
#include <GA/GA_Attribute.h>

/// Class to filter attributes when building GT_AttributeLists
class GT_API GT_GEOAttributeFilter
{
public:
    GT_GEOAttributeFilter() {}
    virtual ~GT_GEOAttributeFilter();

    /// Sub-classes should likely call the base class filtering
    virtual bool	isValid(const GA_Attribute &attrib) const
			{
			    if (attrib.getScope() != GA_SCOPE_PUBLIC)
				return false;
			    return  attrib.getAIFTuple() || 
				    attrib.getAIFNumericArray() ||
				    attrib.getAIFStringTuple() ||
				    attrib.getAIFSharedStringArray();
			}
private:
};

/// Quadrics don't want to pick up either P or N if the attributes exist
class GT_API GT_GEOQuadricAttributeFilter : public GT_GEOAttributeFilter
{
public:
    GT_GEOQuadricAttributeFilter(const GA_Attribute *P,
				const GA_Attribute *N)
	: myP(P)
	, myN(N)
    {}
    virtual ~GT_GEOQuadricAttributeFilter() {}

    virtual bool	isValid(const GA_Attribute &a) const
			{
			    if (&a == myP || &a == myN)
				return false;
			    return GT_GEOAttributeFilter::isValid(a);
			}
protected:
    const GA_Attribute	*myP, *myN;
};

/// Only pick up floating point attributes which have a tuple size that matches
/// the standard VEX types (i.e. 1, 3, 4, 9 or 16 floats).
class GT_API GT_GEOStdVexAttributeFilter : public GT_GEOAttributeFilter
{
public:
    GT_GEOStdVexAttributeFilter() {}
    virtual ~GT_GEOStdVexAttributeFilter() {}

    virtual bool	isValid(const GA_Attribute &a) const
			{
			    if (a.getScope() != GA_SCOPE_PUBLIC)
				return false;

			    if (a.getStorageClass() != GA_STORECLASS_FLOAT)
				return false;

			    int	tsize = a.getTupleSize();
			    return tsize == 1 ||
				    tsize == 3 ||
				    tsize == 4 ||
				    tsize == 9 ||
				    tsize == 16;
			}
};

#endif

