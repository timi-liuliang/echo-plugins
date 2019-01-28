/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_PrimNURBCurve.h (C++)
 *
 * COMMENTS:
 *	Header file for GU_PrimNURBCurve.h class...
 * 
 */

#ifndef __GU_PrimNURBCurve_h__
#define __GU_PrimNURBCurve_h__

#include "GU_API.h"
#include <GA/GA_NUBBasis.h>
#include <GEO/GEO_PrimNURBCurve.h>
#include <UT/UT_Matrix.h>

class GA_ElementWranglerCache;
template<bool isconst> class GA_PwHandle;
typedef GA_PwHandle<true> GA_PwHandleRO;
class GEO_ConvertParms;
class GU_Detail;
class GU_PrimPoly;
class GU_PrimRBezCurve;
class UT_MemoryCounter;

/////////////////////////////////////////////////////////////
//                                                         //
// Class:     GU_PrimNURBCurve				   //
// Descr:     This class is responsible for operations     //
//	 done on nurb primitives.  As we need to access    //
//       the GU_Detail, the pointer is passed in the       //
//       constructor.  The second constructor creates a    //
//       curve with the given parameters.		   //
//                                                         //
/////////////////////////////////////////////////////////////

class GU_API GU_PrimNURBCurve : public GEO_PrimNURBCurve
{
protected:
    /// NOTE: Primitives should not be deleted directly.  They are managed
    ///       by the GA_PrimitiveList and the stash.
    virtual ~GU_PrimNURBCurve() {}

public:
    /// NOTE: This constructor should only be called via GU_PrimitiveFactory.
    inline GU_PrimNURBCurve(GA_Detail *gdp, GA_Offset offset=GA_INVALID_OFFSET)
        : GEO_PrimNURBCurve(gdp, offset)
    {}

    /// Report approximate memory usage.
    virtual int64 getMemoryUsage() const;

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    /// NOTE: This should always include sizeof(*this).
    virtual void countMemory(UT_MemoryCounter &counter) const;

    virtual const GA_PrimitiveDefinition &getTypeDef() const
    {
        UT_ASSERT(theDefinition);
        return *theDefinition;
    }

    GU_PrimPoly	*makeGrevillePoly(GU_Detail *dest) const;

    virtual int         intersectRay(const UT_Vector3 &o, const UT_Vector3 &d,
				     float tmax = 1E17F, float tol = 1E-12F,
				     float *distance = 0, UT_Vector3 *pos = 0,
				     UT_Vector3 *nml = 0, int accurate = 0,
				     float *u = 0, float *v = 0, 
				     int ignoretrim = 1) const;

    virtual int		intersect(const GEO_Primitive &prim,
				  UT_Array<GU_RayInfoHit> &hitList,
				  float tol = 0.01F, int ignoretrim=1) const;

    // NOTE:  For static member functions please call in the following
    //        manner.  <ptrvalue> = GU_PrimNURBCurve::<functname>
    //        i.e.        meshptr = GU_PrimNURBCurve::build(params...);

    // Optional Build Method. Returns 0 if it fails. 

    static GU_PrimNURBCurve	*build(GU_Detail *gudp, int nelems,
				       int order = 4, int closed = 0,
				       int interpEnds = 1, int appendPoints= 1);

    virtual GEO_Primitive *convert(GEO_ConvertParms &parms,
                                   GA_PointGroup *usedpts = NULL);
    virtual GEO_Primitive *convertNew(GEO_ConvertParms &parms);
    GU_PrimRBezCurve	*convertToBezNew(GA_ElementWranglerCache &wranglers,
					 GA_PointGroup *delpts=0,
					 GA_PrimitiveGroup *delprims=0);

    /// Cut a wedge of the primitive given a domain range
    /// ind1 and ind2 are indices to the refined values
    /// They are updated if negative on input, else used as is.
    /// If keep is zero the curve is only refined and the indices
    /// updated.
    virtual GEO_Curve	*cut(float u1, float u2, 
			     int &ind1, int &ind2, int keep);

    /// Open the primitive at the given domain value
    virtual void	 openAt(float u);
    /// Rotate basis so specified u value becomes first visible u value.
    virtual void	 rotateTo(float u);

    /// This is more powerful than convertNew. It always returns a NEW
    /// object, so free it when you're done with it. It may return
    /// a NURB curve or a Bezier curve depending on the type.
    virtual GEO_Face	*reconfigure(unsigned type, int order, bool open,
				     bool interpends, bool nonrational) const;

    // This version reparametrizes the curve.  It does this by implementing
    // a map of the basis from the source parameter values to the corresponding
    // dest values.  Interpolation is linear, thus new knots are inserted
    // at each of the specified points.  If the map is a uniform map,
    // no break points are inserted.
    virtual int		 matchKnots(const UT_Vector &source, 
				    const UT_Vector &dest, float tol = 1e-2F);

    // Compute the location of the breakpoint. Return 0 if OK, else -1.
    virtual int		evaluateBreakpoint(int uidx, UT_Vector4 &pos,
					   int du=0) const;

    // Given a bit array representing the edges in the curve (1 meaning
    // delete and 0 meaning keep) delete as many edges as possible
    // if a particular edge cannot be deleted then negate the value
    // in removededges[edgenum] DO NOT CHANGE THE MAGNITUDE!
    // returns 0 if successful, -1 if the curve becomes degenerate, and
    // -2 if the poly should be removed.
    int			 removeEdges(const UT_BitArray &edgemap,
				     UT_IntArray &removededges);

    // Remove the interior knot at knotIdx num times if possible,
    // where 1 <= num <= the multiplicity mult of the knot,
    // and U[knotIdx] != U[knotIdx+1].
    // If multiplicity mult is not given, the procedure will compute it.
    // The tol specifies the tolerance of distance between the knot removable
    // position and its actual position. To force knot removal, set tol = -1.0F.
    // The deleteGroup is used to gather unused cv geo points to be deleted.
    // (It is more efficient to delete points in a group.)
    // The uniqueInteriorCvs flag indicates whether the interior cvs need to be
    // uniqued before removing knots. This is because during the process of
    // knot removal, interior cvs may changed and/or got removed.
    // Output: the actual number of times that the knot got removed.
    int                  removeKnot(int knotIdx, int num, int mult=0,
    				    float tol=1e-4F,GA_PointGroup *delGroup=0,
                                    int uniqueInteriorCvs=0);

    // Perform data reduction by removing possible knots (and cvs) while
    // maintaining the errors between all the data points and the actual
    // points on the curve corresponding to the parm to be within the tolerance
    // tol. If num is specified, the procedure will remove at least that number
    // of knots even when removing them may result in error > tol.
    // The error[k] is the current error between data point corresponding
    // to parm[k]. The parm list has to be in non-decreasing order.
    // The deleteGroup is used to gather unused cv geo points to be deleted.
    // Output: update the error vector, and reduce knots and cvs.
    void		 reduceKnots(const UT_Vector &parm, UT_Vector &error,
				     float tol=1e-1F, int num=0,
				     GA_PointGroup *deleteGroup=0);

    // Methods for fitting.
    // Interpolate a set of data points.
    // Return 0 if resulted in singular matrix (ie. can not solve).
    //        1 if ok.
    int 	 	 interpGlobal(const GA_Range &point_range,
			    int order=4, int wrapped=0,
			    GA_ParameterizationType parmType=GA_PARAMETERIZATION_CHORD,
			    GA_KnotSpaceType knotSpaceType=GA_KNOT_SPACE_AVERAGING,
			    const UT_Vector *parmValues = 0);
    int 	 	 interpGlobal(const UT_Vector4Array &v4Data,
			    int order=4, int wrapped=0,
			    GA_ParameterizationType parmType=GA_PARAMETERIZATION_CHORD,
			    GA_KnotSpaceType knotSpaceType=GA_KNOT_SPACE_AVERAGING,
			    const UT_Vector *parmValues = 0);

    int			 interpGlobal(const UT_MatrixF &pointData,
			    const UT_MatrixF &derivData, int order=4,
			    int wrapped = 0, 
			    GA_ParameterizationType parmType=GA_PARAMETERIZATION_CHORD,
			    GA_KnotSpaceType knotSpaceType=GA_KNOT_SPACE_AVERAGING);
    int			 interpGlobal(const UT_MatrixF &pointData,
			    const UT_MatrixF &derivData,
			    const UT_MatrixF &deriv2Data, int order=6,
			    int wrapped = 0, 
			    GA_ParameterizationType parmType=GA_PARAMETERIZATION_CHORD,
			    GA_KnotSpaceType knotSpaceType=GA_KNOT_SPACE_AVERAGING);

    void 		 interpLocal(const UT_Array<GA_Offset> &gpData,
				     int order=4, int wrapped=0, int corner=0);
    void 		 interpLocal(const UT_Vector4Array &v4Data,
				     int order=4, int wrapped=0, int corner=0);

    // Fit through the breakpoints:
    int 	 	 interpBreakpoints(
			    const GA_OffsetList &point_offsets,
			    int order=4, int wrapped=0,
			    GA_ParameterizationType parmType=GA_PARAMETERIZATION_CHORD,
			    const UT_Vector *parmValues = 0);
    int 	 	 interpBreakpoints(
			    const UT_Vector4Array &v4Data,
			    int order=4, int wrapped=0,
			    GA_ParameterizationType parmType=GA_PARAMETERIZATION_CHORD,
			    const UT_Vector *parmValues = 0);

    // This approxmation is mainly for open curve.
    // For closed curve, it may not behave well on the closed segment.
    // However, you can improve its behaviour by duplicating the first 
    // data point and put it at the end.
    void 		 approxGlobal(const UT_Vector4Array &v4Data,
			              int order=4, int wrapped=0,
				      float tol=1e-1f, float smooth=0.0F,
				      int noMultipleKnots=1);

    // Static methods for fitting.

    // Global curve interpolation through dataPts[0..n][0..Dimension-1]
    // with parameterization param[0..n] and basis should have its knot
    // vector and order already set up.
    // Output: cvs[0..n][0..Dimension-1]
    // If natural_end is set it means that we are computing the 2nd derivative
    // at the ends.
    static int       	 interpGlobal(const UT_MatrixF &dataPts,
				      const UT_Vector &param,
				      const GA_NUBBasis &basis,
				      UT_MatrixF &cvs, int natural_end = 0);
    // Takes points and/or derivatives in dataPts[0..n][0..Dimension-1]
    // with parameterization param[0..n] and corresponding derivatives
    // dervs[0..n].  Basis should have its knot vector and order already
    // set up.
    static int		 interpGlobal(const UT_MatrixF &dataPts,
				      const UT_Vector &param,
				      const GA_NUBBasis &basis,
				      UT_MatrixF &cvs,
				      const UT_IntArray &dervs);
    // Takes derivitives at each point.  CV matrix should be
    // cvs[0..2n+1][0..Dimmension-1]
    static int       	 interpGlobal(const UT_MatrixF &dataPts,
				      const UT_MatrixF &dataTans,
				      const UT_Vector &param,
				      const GA_NUBBasis &basis,
				      UT_MatrixF &cvs);
    // Takes derivitives at each point.  CV matrix should be
    // cvs[0..3n+2][0..Dimmension-1]
    static int       	 interpGlobal(const UT_MatrixF &dataPts,
				      const UT_MatrixF &dataTans,
				      const UT_MatrixF &dataCurvature,
				      const UT_Vector &param,
				      const GA_NUBBasis &basis,
				      UT_MatrixF &cvs);

    // Get the bounding box for a specific range
    virtual void	getRangeBBox(const UT_Interval &u,
				     UT_BoundingBox &bbox,
				     const GA_PwHandleRO &h) const;

private:
    // Private methods to translate/transform selected breakpoints and
    // interpolate through them.
    virtual int		 translateBkptsFixed(const UT_IntArray &uindices,
                                             const UT_Vector3 &delta,
					     GA_PointGroup *ptgroup = NULL,
					     GEO_Delta *geodelta = 0);
    virtual int		 transformBkptsFixed(const UT_IntArray &uindices,
					     const UT_Matrix4 &matx,
					     GA_PointGroup *ptgroup = NULL,
					     GEO_Delta *geodelta = 0);

    // Fill the curve with data and build the basis. Return 0 if OK, and -1
    // if error.
    int			create(int nelems, int order=4, int closed = 0,
			       int interpEnds = 1, int appendPoints = 1);

    // Use for approximation.
    void		fit(const UT_Vector4Array &v4Data,
			    UT_Vector *data, int curOrd, int prevEnd,
			    int cvEnd, UT_Vector *coord, UT_Vector &param,
			    UT_Vector &error, float smooth);

    static GA_PrimitiveDefinition	*theDefinition;
    friend class			 GU_PrimitiveFactory;
};

#endif
