/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_PrimPoly.h (C++)
 *
 * COMMENTS:
 *	Header file for GU_PrimPoly.h class...
 * 
 */

#pragma once

#ifndef __GU_PrimPoly_h__
#define __GU_PrimPoly_h__

#include "GU_API.h"
#include <GEO/GEO_PrimPoly.h>
#include <UT/UT_Vector3.h>

class GA_ElementWranglerCache;
class GA_Detail;
class GEO_ConvertParms;
class GEO_Hull;
class GEO_PrimCircle;
class GEO_PrimRBezCurve;
class GEO_PrimNURBCurve;
class GEO_PrimTriBezier;
class GU_RayInfoHit;
class UT_MemoryCounter;

/////////////////////////////////////////////////////////////
//                                                         //
// Class: 	GU_PrimPoly				   //
// Descr:     This class is responsible for operations     //
//	done on polygon primitives.  As we need to access  //
//       the GA_Detail, the pointer is passed in the      //
//       constructor.  The second constructor creates a    //
//       polygon with a given number of points (npts) and  //
//       a flag indicating if it is open or not.           //
//                                                         //
/////////////////////////////////////////////////////////////

class GU_API GU_PrimPoly : public GEO_PrimPoly
{
public:
    /// NOTE: This constructor should only be called via GU_PrimitiveFactory.
    SYS_FORCE_INLINE
    GU_PrimPoly(GA_Detail *gdp, GA_Offset offset=GA_INVALID_OFFSET)
        : GEO_PrimPoly(gdp, offset)
    {}

    /// This constructor is for making a representation of a polygon
    /// on the stack, so that you can call GU_PrimPoly functions on
    /// the polygon without needing the detail to allocate one.
    SYS_FORCE_INLINE
    GU_PrimPoly(GA_Detail *gdp, GA_Offset offset, const GA_OffsetListRef &vertex_list)
        : GEO_PrimPoly(gdp, offset)
    { myVertexList = vertex_list; }

    // Conversion Methods

    virtual GEO_Primitive *convert(GEO_ConvertParms &parms,
				 GA_PointGroup *usedpts = 0);

    virtual GEO_Primitive *convertNew(GEO_ConvertParms &parms);

    GEO_Hull		*convertToSurfNew(GEO_ConvertParms &parms);
    GEO_Hull		*convertToPotatoChip(void);
    GEO_Hull		*convertToTrim(GEO_ConvertParms &parms);

    GEO_PrimRBezCurve	*convertToBezierNew(GA_ElementWranglerCache &wranglers,
					int order, int interphull = 1);
    GEO_PrimNURBCurve	*convertToNURBNew(GA_ElementWranglerCache &wranglers,
					int order, int interphull = 1);
    GEO_PrimCircle	*convertToCircleNew();

    // Converts tris into triangle beziers by the PN method.
    GEO_PrimTriBezier	*convertToTriBezier(GA_ElementWranglerCache &wranglers);


    // Cut a wedge of the primitive given a domain range
    // ind1 and ind2 are indices to the refined values
    // They are updated if negative on input, else used as is.
    // If keep is zero the curve is only refined and the indices
    // updated.
    GU_PrimPoly	*cut(float u1, float u2,
			    int &ind1, int &ind2, int keep);

    // Open the primitive at the given domain value
    void		 openAt(float u);

    // This is more powerful than convertNew. It always returns a NEW
    // object, so free it when you're done with it. It may return a polygon,
    // a NURB curve or a Bezier curve depending on the type.
    GEO_Face		*reconfigure(unsigned type, int order, bool open,
				    bool nonrational) const;

     virtual int	 intersectRay(const UT_Vector3 &o, const UT_Vector3 &d,
				float tmax = 1E17F, float tol = 1E-12F,
				float *distance = 0, UT_Vector3 *pos = 0,
				UT_Vector3 *nml = 0, int accurate = 0,
				float *u = 0, float *v = 0, 
				int ignoretrim=1) const;

    // Returns number of hits appended, or minus one if error.
    // t values in hitList are paramter on poly of hits, assuming
    // uniform parameterization.
    virtual int	 	 intersect(const GEO_Primitive &prim,
				    UT_Array<GU_RayInfoHit> &hitList,
				    float tol = 0.01F, int ignoretrim=1) const;

    virtual void 	 clip(UT_Vector3 normal, float distance=0,
				GA_PrimitiveGroup *clipgrp = NULL);

     void	 	 crease(UT_Vector3 &normal, float d=0, int normalize=1,
				int outputGroups=0,
                                GA_PrimitiveGroup*above=0,
                                GA_PrimitiveGroup*below=0);

    /// Optional Build Method 
    /// You may call it in the following manner, 
    /// ptr = GU_PrimPoly::build(bla, bla, bla);
    static GU_PrimPoly	*build(GA_Detail *gdp, int npts,
			       int open=0,
			       int appendpts=1)
    { return (GU_PrimPoly *)GEO_PrimPoly::build(gdp, npts, open, appendpts); }

    /// Given a bit array representing the edges in the poly (1 meaning
    /// delete and 0 meaning keep) delete as many edges as possible.
    /// If a particular edge cannot be deleted then negate the value
    /// in removededges[edgenum]. DO NOT CHANGE THE MAGNITUDE!
    /// Returns 0 if successful, -1 if the poly becomes degenerate, and
    /// -2 if the poly should be removed.  The signature for this method
    /// is consistent with those in other classes, but note that there is
    /// never a case where an edge cannot be removed from a polygon.
    int			 removeEdges(const UT_BitArray &edgemap,
				     UT_IntArray &, bool del_bridges = true)
			 {
			     GEO_Face		*dummy;
			     return removeEdges(edgemap, dummy, del_bridges);
			 }

    /// Same as above method, but instead of accepting an (unused) array
    /// of edge indices, it returns one (possibly one of many) polygons
    /// created because this polygon had to be split.
    int		 removeEdges(const UT_BitArray &edgemap,
			     GEO_Face *&newpoly,
			     bool del_bridges=true);

    /// Same as above, but it returns all polygons created because of polysplit
    int		 removeEdges(const UT_BitArray &edgemap,
			     UT_Array<GEO_Face*> &newpoly,
			     bool del_bridges=true);


    // Looks at the first shared edge between us and poly and checks their
    // orientation.  Returns 0 if they are pointing in opposite directions and
    // 1 if they are pointing in the same direction and -1 if no shared edge
    // could be found.
    int				 isWindingReversed(const GEO_PrimPoly &poly);

private:
    // Vertex sides is 1 or 0 for each vertex depending on whether
    // it is above the plane.
    void	makeClippedPoly(const UT_Vector3 &n, float d, 
				int *vtxsides,
				int bothSides=0,
				int outputGroups = 0,
                                GA_PrimitiveGroup *above = 0,
                                GA_PrimitiveGroup *below = 0,
				GA_PrimitiveGroup *delgrp = 0);

    void	computeVertices(GEO_Face &face) const;

// Don't warn about overriding deprecated virtual methods
SYS_DEPRECATED_PUSH_DISABLE()
};
SYS_DEPRECATED_POP_DISABLE()

#endif
