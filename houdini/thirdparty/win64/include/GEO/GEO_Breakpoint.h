/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO_Breakpoint (C++)
 *
 * COMMENTS:
 * 	Utility methods for GA_Breakpoints.
 *
 */

#ifndef __GEO_Breakpoint_h__
#define __GEO_Breakpoint_h__

#include "GEO_API.h"
#include <GA/GA_Types.h>
#include <GA/GA_Breakpoint.h>
#include <UT/UT_VectorTypes.h>

class GA_Basis;
class GEO_Detail;

class GEO_API GEO_Breakpoint
{
public:
    typedef int (*iteratePointFuncPtr)(void *data, GEO_Detail &gdp,
                                       GA_Offset ptoff);
    typedef int (*iterateVertexFuncPtr)(void *data, GA_Offset vtx,
					int linear_index);

    // Compute the location of the breakpoint. Return 0 if OK, else -1.
    static int		 evaluate(const GA_Breakpoint &bkpt, UT_Vector4 &pos,
				  int du=0, int dv=0);

    // This method performs the iterateFunc on all the CVs affecting the
    // breakpoint and returns the sum of the results.
    static int		 iterateOverCVs(const GA_Breakpoint &bkpt,
					void *data, iteratePointFuncPtr func);
    static int		 iterateOverCVs(const GA_Breakpoint &bkpt,
					void *data, iterateVertexFuncPtr func);

    // These methods return the knot indices corresponding to the breakpoint,
    // or the parameter values at these knot indices.  Pointers to the bases
    // are returned in ubasis_ret and vbasis_ret, if these arguments are
    // not nil.
    static void		 getKnotIndices(const GA_Breakpoint &bkpt,
					int &ukidx, int &vkidx,
					const GA_Basis **ubasis_ret=0,
					const GA_Basis **vbasis_ret=0);
    static void		 getParameter(const GA_Breakpoint &bkpt,
				      float &u, float &v);
};

#endif
