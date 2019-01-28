/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_PrimRBezSurf.h (C++)
 *
 * COMMENTS:
 *	Header file for GU_PrimRBezSurf.h class...
 * 
 */

#ifndef __GU_PrimRBezSurf_h__
#define __GU_PrimRBezSurf_h__

#include "GU_API.h"
#include <GEO/GEO_PrimRBezSurf.h>
#include <GEO/GEO_PrimNURBSurf.h>

class GA_Detail;
class GA_EdgeGroup;
class GA_ElementWranglerCache;
template<bool isconst> class GA_PwHandle;
typedef GA_PwHandle<true> GA_PwHandleRO;
class GEO_ConvertParms;
class GEO_Detail;
class UT_MemoryCounter;

/////////////////////////////////////////////////////////////
//                                                         //
// Class:     GU_PrimRBezSurf				   //
// Descr:     This class is responsible for operations     //
//	 done on bezier primitives.  As we need to access  //
//       the GEO_Detail, the pointer is passed in the      //
//       constructor.  The second constructor creates a    //
//       mesh with the given parameters.		   //
//                                                         //
/////////////////////////////////////////////////////////////

class GU_API GU_PrimRBezSurf : public GEO_PrimRBezSurf
{
protected:
    /// NOTE: Primitives should not be deleted directly.  They are managed
    ///       by the GA_PrimitiveList and the stash.
    virtual ~GU_PrimRBezSurf() {}

public:
    /// NOTE: This constructor should only be called via GU_PrimitiveFactory.
    GU_PrimRBezSurf(GA_Detail *gdp, GA_Offset offset=GA_INVALID_OFFSET)
        : GEO_PrimRBezSurf(gdp, offset)
    {}

    virtual const GA_PrimitiveDefinition &getTypeDef() const
    {
        UT_ASSERT(theDefinition);
        return *theDefinition;
    }

    /// Report approximate memory usage.
    virtual int64 getMemoryUsage() const;

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    /// NOTE: This should always include sizeof(*this).
    virtual void countMemory(UT_MemoryCounter &counter) const;

    // Fill the surface with data and build the bases. Return 0 if OK, and -1
    // if error.
    int			create(int rows, int cols, int orderu=4, int orderv=4,
		   	    int wrapu = 0, int wrapv = 0,
		      	    GEO_SurfaceType type = GEO_PATCH_QUADS,
		      	    int appendPoints = 1);

    // Conversion Method 
    // uses 'this' for surfacetype, primattribs, orderu and orderv.
    GEO_PrimNURBSurf	*convertToNURBNew(GA_ElementWranglerCache &wranglers);

    // This only works with unit patches.  And only well for order 4
    // and less.
    GEO_Primitive	*convertToTriBezNew(GEO_ConvertParms &parms);

    virtual GEO_Primitive *convert(GEO_ConvertParms &parms,
                                   GA_PointGroup *usedpts = NULL);
    virtual GEO_Primitive *convertNew(GEO_ConvertParms &parms);

    virtual int		intersectRay(const UT_Vector3 &o, const UT_Vector3 &d,
				float tmax = 1E17F, float tol = 1E-12F,
				float *distance = 0, UT_Vector3 *pos = 0,
				UT_Vector3 *nml = 0, int accurate = 0,
				float *u = 0, float *v = 0,
				int ignoretrim = 1) const;

    virtual int  intersectSurf(GEO_TPSurf &surf2,
                               GU_IsectCurveSet &curveset,
                               float worldtol = 1e-4F,
                               float domaintol = 1e-2F,
                               int steps = 100,
                               bool docompact = true);
    virtual int  doesIntersect(const GEO_Primitive &prim,
                               float worldtol = 1e-4F, int nontrivial = 0) const;

    // This is more powerful than convertNew. It always returns a NEW
    // object, so free it when you're done with it. It may return
    // a NURB surface or a Bezier surface depending on the type.
    virtual GEO_Hull	*reconfigure(unsigned type, int orderu, int orderv,
				    bool openu, bool openv, bool, bool) const;

    // Cut a wedge of the primitive given a domain range
    // ind1 and ind2 are indices to the refined values
    // They are updated if negative on input, else used as is.
    // If keep is zero the curve is only refined and the indices
    // updated.

    virtual GEO_TPSurf	*cutU(float u1, float u2,
			      int &ind1, int &ind2, int keep);
    virtual GEO_TPSurf	*cutV(float v1, float v2,
			      int &ind1, int &ind2, int keep);

    // Open the primitive at the given domain value
    virtual void	 openAtU(float u);
    virtual void	 openAtV(float v);

    // Divide a surface into its components patches.  Do not
    // modify 'this'.
    //	every_patch	- Split even if this is already a minimal patch
    //	appendPoints	- Add new points for split patches
    //	outprims	- Group to add created primitives to
    // Return:  npatches added or -1 on error
    int			 split(int every_patch = 0,
				int append_points = 0, 
				GA_PrimitiveGroup *out_prims = 0);
			
    /// Construct a curve that has our characteristics in u or v. The
    /// CVs of the new curve are not set.
    /// @{
    virtual GEO_Curve	*buildRowCurve(bool appendPoints,
	    			       GEO_Detail *parent) const;
    virtual GEO_Curve	*buildColCurve(bool appendPoints,
	    			       GEO_Detail *parent) const;
    /// @}

     static GU_PrimRBezSurf	*build(GEO_Detail *gudp, 
				       int rows, int cols, 
				       int orderu = 4, int orderv = 4,
				       int wrapu = 0, int wrapv = 0,
				       GEO_SurfaceType type = GEO_PATCH_QUADS,
				       int appendPoints = 1);

	// Remove as many of the given edges of possible from this poly.
	// If edge #a is removed, bit a in the bit array is set to a & 1.
	// returns 0 if successful, -1 if the poly becomes degenerate, and
	// -2 if the poly should be removed.
     int			 removeEdges(const GA_EdgeGroup &edges,
					     UT_BitArray *remove_edges=0);

    // Get the bounding box for a specific range of u and v
    virtual void	getRangeBBox(const UT_Interval &u,
				     const UT_Interval &v,
				     UT_BoundingBox &bbox,
				     const GA_PwHandleRO &h) const;

protected:
    virtual GEO_Hull    *getSubHull(int top, int left, int bottom, int right,
					GEO_SubHullFlags break_flags);

private:
    static GA_PrimitiveDefinition	*theDefinition;
    friend class			 GU_PrimitiveFactory;
};

#endif
