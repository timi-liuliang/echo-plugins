/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO_PrimClassifier.h ( GEO Library, C++)
 *
 * COMMENTS:  This routine classifies primitives according to what
 * 	      meta-primitive they belong to.  The exact definition of a
 * 	      meta-primitive depends on the classification done.
 */

#ifndef __GEO_PrimClassifier__
#define __GEO_PrimClassifier__

#include "GEO_API.h"
#include <UT/UT_IntArray.h>
#include <GA/GA_Handle.h>

class GEO_Detail;
class GEO_DetachedHedgeInterface;
class GA_PrimitiveGroup;
class GA_EdgeGroup;

class GEO_API GEO_PrimClassifier
{
public:
    GEO_PrimClassifier()
        : myNumClass(0)
    {}

    void                 classifyByVertexUV(const GEO_Detail &gdp,
                                const GA_ROHandleV3 &attrib,
                                const GA_PrimitiveGroup *exclude_prims=NULL);
    void		 classifyBySharedPoints(const GEO_Detail &gdp,
				const GA_PrimitiveGroup *exclude_prims=NULL);
    void		 classifyByEdgeCuts(const GEO_Detail &gdp,
				const GEO_DetachedHedgeInterface &hi,
				const GA_EdgeGroup *cuts=NULL,
				const GA_PrimitiveGroup *exclude_prims=NULL);

    int			 getClass(int idx) const
			 { return myClasses((unsigned)idx); }
    int			 getNumClass() const
			 { return myNumClass; }
    const UT_IntArray	&getClassArray() const
			 { return myClasses; }

    int64		 getMemoryUsage(bool inclusive) const
			 {
			    int64 mem = inclusive ? sizeof(*this) : 0;
			    mem += myClasses.getMemoryUsage(false);
			    return mem;
			 }

protected:
    // Methods which collapse & search for classes...
    int			 findClass(int idx);

    UT_IntArray		 myClasses;
    int			 myNumClass;
};

#endif

