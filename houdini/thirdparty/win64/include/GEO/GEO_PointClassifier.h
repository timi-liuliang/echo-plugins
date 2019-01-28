/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO_PointClassifier.h ( GEO Library, C++)
 *
 * COMMENTS:  This routine classifies points according to what
 * 	      meta-primitive they belong to.  A meta primitive is all
 *	      the primitives which share at least one point.
 */

#ifndef __GEO_PointClassifier__
#define __GEO_PointClassifier__

#include "GEO_API.h"
#include <UT/UT_IntArray.h>
class GEO_Detail;
class GA_PointGroup;
class GA_PrimitiveGroup;

class GEO_API GEO_PointClassifier
{
public:
    GEO_PointClassifier();
    virtual ~GEO_PointClassifier();

    void	classifyPoints(const GEO_Detail &gdp, 
			       const GA_PrimitiveGroup *exclude_prims = NULL);

    // This will only classify points within the given radius of the ptgroup
    // and dump all others will be set to the class of -1.
    void	classifyPoints(const GEO_Detail &gdp, 
			    const GA_PointGroup &ptgroup, float radius);

    int		getClass(int ptidx) const { return myClasses((unsigned)ptidx); }
    int		getNumClass() const { return myNumClass; }

    int64	getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += myClasses.getMemoryUsage(false);
        return mem;
    }

protected:
    // Methods which collapse & search for classes...
    int		findClass(int ptidx);

    UT_IntArray		myClasses;
    int			myNumClass;
};

#endif

