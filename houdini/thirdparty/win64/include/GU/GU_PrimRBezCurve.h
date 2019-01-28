/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_PrimRBezCurve.h (C++)
 *
 * COMMENTS:
 *	Header file for GU_PrimRBezCurve.h class...
 * 
 */

#ifndef __GU_PrimRBezCurve_h__
#define __GU_PrimRBezCurve_h__

#include "GU_API.h"
#include <GEO/GEO_PrimRBezCurve.h>
#include <GEO/GEO_PrimNURBCurve.h>

class GA_Detail;
class GA_ElementWranglerCache;
template<bool isconst> class GA_PwHandle;
typedef GA_PwHandle<true> GA_PwHandleRO;
class GEO_ConvertParms;
class GU_Detail;
class GU_PrimPoly;
class UT_MemoryCounter;

/////////////////////////////////////////////////////////////
//                                                         //
// Class:     GU_PrimRBezCurve				   //
// Descr:     This class is responsible for operations     //
//	 done on bezier primitives.  As we need to access  //
//       the GU_Detail, the pointer is passed in the       //
//       constructor.  The second constructor creates a    //
//       mesh with the given parameters.		   //
//                                                         //
/////////////////////////////////////////////////////////////

class GU_API GU_PrimRBezCurve : public GEO_PrimRBezCurve
{
protected:
    /// NOTE: Primitives should not be deleted directly.  They are managed
    ///       by the GA_PrimitiveList and the stash.
    virtual ~GU_PrimRBezCurve() {}

public:
    /// NOTE: This constructor should only be called via GU_PrimitiveFactory.
    GU_PrimRBezCurve(GA_Detail *gdp, GA_Offset offset=GA_INVALID_OFFSET)
        : GEO_PrimRBezCurve(gdp, offset)
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

     GU_PrimPoly	*makeGrevillePoly(GU_Detail *dest) const;

    // NOTE:  For static member functions please call in the following
    //        manner.  <ptrvalue> = GU_PrimRBezCurve::<functname>
    //        i.e.        meshptr = GU_PrimRBezCurve::build(params...);

    // Optional Build Method. Returns 0 if it fails.

     static GU_PrimRBezCurve	*build(GU_Detail *gudp,
				       int nelems, int order = 4, int closed=0,
				       int appendPoints = 1);

    virtual int		intersectRay(const UT_Vector3 &o, const UT_Vector3 &d,
				float tmax = 1E17F, float tol = 1E-12F,
				float *distance = 0, UT_Vector3 *pos = 0,
				UT_Vector3 *nml = 0, int accurate = 0,
				float *u = 0, float *v = 0,
				int ignoretrim = 1) const;

    virtual int		intersect(const GEO_Primitive &prim,
				  UT_Array<GU_RayInfoHit> &hitList,
				  float tol = 0.01F, int ignoretrim=1) const;

    virtual GEO_Primitive *convert(GEO_ConvertParms &parms,
                                   GA_PointGroup *usedpts = NULL);
    virtual GEO_Primitive *convertNew(GEO_ConvertParms &parms);
    GEO_PrimNURBCurve	*convertToNURBNew(GA_ElementWranglerCache &wranglers);

    /// This is more powerful than convertNew. It always returns a NEW
    /// object, so free it when you're done with it. It may return
    /// a NURB curve or a Bezier curve depending on the type.
    virtual GEO_Face	*reconfigure(unsigned type,
				int order, bool open, bool interpends,
				bool nonrational) const;

    /// Cut a wedge of the primitive given a domain range
    /// ind1 and ind2 are indices to the refined values
    /// They are updated if negative on input, else used as is.
    /// If keep is zero the curve is only refined and the indices
    /// updated.
    virtual GEO_Curve	*cut(float u1, float u2,
			     int &ind1, int &ind2, int keep);

    /// Open the primitive at the given domain value
    virtual void	 openAt(float u);


    // Given a bit array representing the edges in the curve (1 meaning
    // delete and 0 meaning keep) delete as many edges as possible
    // if a particular edge cannot be deleted then negate the value
    // in removededges[edgenum] DO NOT CHANGE THE MAGNITUDE!
    // returns 0 if successful, -1 if the curve becomes degenerate, and
    // -2 if the curve should be removed.
    int			 removeEdges(const UT_BitArray &edgemap,
				     UT_IntArray &removededges);

    // Methods for fitting.
    void                 interpLocal(const UT_Array<GA_Offset> &goData,
				     int order, int wrapped, int corner);
    void                 interpLocal(const UT_Vector4Array &v4Data,
				     int order, int wrapped, int corner);

    // Get the bounding box for a specific range
    virtual void	getRangeBBox(const UT_Interval &u,
				     UT_BoundingBox &bbox,
				     const GA_PwHandleRO &h) const;

private:

     // Fill the curve with data and build the basis. Return 0 if OK, and -1
     // if error.
     int		create(int nelems, int order = 4, int closed = 0,
					   int appendPoints = 1);

    static GA_PrimitiveDefinition	*theDefinition;
    friend class			 GU_PrimitiveFactory;
};

#endif
