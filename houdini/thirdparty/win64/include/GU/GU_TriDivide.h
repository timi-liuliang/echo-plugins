/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_TriDivide.h ( GU Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GU_TriDivide__
#define __GU_TriDivide__

#include "GU_API.h"
#include <GA/GA_Edge.h>
#include <GA/GA_EdgeMap.h>

class GA_PrimitiveGroup;
class guDivTri;

// using TriEdgeMap = GA_EdgeMap<guDivTri *>;
#include <UT/UT_Map.h>
using TriEdgeMap = UT_Map<GA_Edge, guDivTri *>;

class GU_API GU_TriDivide
{
public:
    GU_TriDivide(GU_Detail *gdp);

    virtual ~GU_TriDivide();

    void	 setDetail(GU_Detail *gdp) { myGdp = gdp; }
    GU_Detail	*getDetail() const { return myGdp; }

    void	 setRefDetail(const GU_Detail *refgdp) { myRefGdp = refgdp; }
    const GU_Detail *getRefDetail() const { return myRefGdp; }

    /// Builds the triangle list.
    void	 buildTopology(const GA_PrimitiveGroup *grp=0);
    
    /// Does root(3) subdivision.
    void	 tridivide(int depth);

    /// Equalizes edges by splitting the longest edges.
    /// Splits are done until minimum length is less than minlength
    /// or number of splits numsplits has been done.  A value of -1
    /// is ignored.
    void	 equalizeEdges(fpreal minlength, int numsplit);

    /// Transforms the triangle list into actual triangles.
    void	 buildGeometry();

    /// Technically should be friended to our guDivTri.
    guDivTri	*allocTri();

protected:
    void	 buildTri(GEO_PrimPoly *poly, TriEdgeMap &edgemap);

    fpreal	 calculateLength(GA_Index aidx, GA_Index bidx,
				bool useref,
				const UT_Vector3Array &refpos) const;

protected:
    GU_Detail			*myGdp;
    const GU_Detail		*myRefGdp;
    UT_Array<guDivTri *>	 myTriList;
};

#endif

