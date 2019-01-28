/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_PrimNURBSurf.h (C++)
 *
 * COMMENTS:
 *	Header file for GU_PrimNURBSurf.h class...
 * 
 */

#ifndef __GU_PrimNURBSurf_h__
#define __GU_PrimNURBSurf_h__

#include "GU_API.h"
#include <GEO/GEO_PrimNURBSurf.h>
#include <GEO/GEO_PrimRBezSurf.h>
#include <GA/GA_NUBBasis.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_PtrMatrix.h>
#include <UT/UT_Matrix.h>

class GA_EdgeGroup;
class GA_ElementWranglerCache;
template<bool isconst> class GA_PwHandle;
typedef GA_PwHandle<true> GA_PwHandleRO;
class GEO_ConvertParms;
class GEO_Detail;
class GU_PrimRBezSurf;
class UT_MemoryCounter;

//////////////////////////////////////////////////////////////
///                                                         //
/// Class:     GU_PrimNURBSurf                              //
/// Descr:     This class is responsible for operations     //
///       done on nurb primitives.  As we need to access    //
///       the GEO_Detail, the pointer is passed in the      //
///       constructor.  The second constructor creates a    //
///       mesh with the given parameters.                   //
///                                                         //
//////////////////////////////////////////////////////////////

class GU_API GU_PrimNURBSurf : public GEO_PrimNURBSurf
{
protected:
    /// NOTE: Primitives should not be deleted directly.  They are managed
    ///       by the GA_PrimitiveList and the stash.
    virtual ~GU_PrimNURBSurf() {}

public:
    /// NOTE: This constructor should only be called via GU_PrimitiveFactory.
    GU_PrimNURBSurf(GA_Detail *gdp, GA_Offset offset=GA_INVALID_OFFSET)
        : GEO_PrimNURBSurf(gdp, offset)
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

    /// Conversion Method 

    /// uses 'this' for surfacetype, primattribs, orderu and orderv.
    GU_PrimRBezSurf	*convertToBezNew(GA_ElementWranglerCache &wranglers,
					 GA_PointGroup *delpoints=0,
					 GA_PrimitiveGroup *delprim=0);

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

    /// This is more powerful than convertNew. It always returns a NEW
    /// object, so free it when you're done with it. It may return
    /// a NURB surface or a Bezier surface depending on the type.
    virtual GEO_Hull	*reconfigure(unsigned type, int orderu, int orderv,
				     bool openu, bool openv,
				     bool endsu, bool endsv) const;

    /// Cut a wedge of the primitive given a domain range
    /// ind1 and ind2 are indices to the refined values
    /// They are updated if negative on input, else used as is.
    /// If keep is zero the curve is only refined and the indices
    /// updated.

    virtual GEO_TPSurf	*cutU(float u1, float u2,
			      int &ind1, int &ind2, int keep);

    virtual GEO_TPSurf	*cutV(float v1, float v2,
			      int &ind1, int &ind2, int keep);


    /// Open the primitive at the given domain value
    virtual void	 openAtU(float u);
    virtual void	 openAtV(float v);

    /// Construct a curve that has our characteristics in u or v. The
    /// CVs of the new curve are not set.
    /// @{
    virtual GEO_Curve	*buildRowCurve(bool appendPoints,
				       GEO_Detail *parent) const;
    virtual GEO_Curve	*buildColCurve(bool appendPoints,
				       GEO_Detail *parent) const;
    /// @}

     static GU_PrimNURBSurf	*build(GEO_Detail *gudp,
				       int rows, int cols,
				       int orderu = 4, int orderv = 4,
				       int wrapu = 0, int wrapv = 0,
				       int interpEndsU= 1, int interpEndsV= 1,
				       GEO_SurfaceType type = GEO_PATCH_QUADS,
				       int appendPoints = 1);

    /// Remove as many of the given edges of possible from this poly.
    /// If edge #a is removed, bit a in the bit array is set to a & 1.
    /// returns 0 if successful, -1 if the poly becomes degenerate, and
    /// -2 if the poly should be removed.
    int				removeEdges(const GA_EdgeGroup &edges,
			        	     UT_BitArray *remove_edges=0);

    /// Remove the interior knot at knotIdx once if possible,
    /// where mult is the multiplicity of the knot,
    /// and U/V[knotIdx] != U/V[knotIdx+1].
    /// If multiplicity mult is not given, the procedure will compute it.
    /// The tol specifies the tolerance of distance between the knot removable
    /// position and its actual position. To force knot removal, set tol = -1.0F.
    /// The deleteGroup is used to gather unused cv geo points to be deleted.
    /// (It is more efficient to delete points in a group.)
    /// The uniqueInteriorCvs flag indicates whether the interior cvs need to be
    /// uniqued before removing knots. This is because during the process of
    /// knot removal, interior cvs may changed and/or got removed.
    /// Output: 1 if the knot got removed, else 0.
    int                  removeKnot(int uDir, int knotIdx, int mult=0,
                                    float tol=1e-4F,
				    GA_PointGroup *deleteGroup=0,
                                    int uniqueInteriorCvs=0);

    /// Perform data reduction by removing possible knots (and cvs) while
    /// maintaining the errors between all the data points and the actual
    /// points on the surface corresponding to the parm to be within the
    /// tolerance tol. If u/vNum is specified, the procedure will remove
    /// at least that number of knots in their corresponding parameters even
    /// when removing them may result in error > tol.
    /// The error(i,j) is the current error between data point corresponding
    /// to uParm(i) and vParm(j). The parameter lists have to be in
    /// non-decreasing order.
    /// The deleteGroup is used to gather unused cv geo points to be deleted.
    /// Output: update the error matrix, and reduce knots and cvs.
    void                 reduceKnots(const UT_Vector &uParm,
				     const UT_Vector &vParm,
				     UT_MatrixF &error, float tol=1e-1F, 
				     int uNum=0, int vNum=0,
                                     GA_PointGroup *deleteGroup=0);

    /// Compute the location of the breakpoint. Return 0 if OK, else -1.
    virtual int		 evaluateBreakpoint(int uidx, int vidx,
					    UT_Vector4 &pos,
					    int du=0, int dv=0) const;

    /// Methods for fitting.

    /// Interpolate the mesh of points.
    /// Return 0 if ok.
    ///        1 if the data resulted in a singular matrix.
    ///        2 if interrupted.
    int		 	 interpGlobal(const GA_OffsetMatrix &goMesh,
			  int uOrder=4, int vOrder=4,
			  bool uWrapped=false, bool vWrapped=false,
			  GEO_SurfaceType type=GEO_PATCH_QUADS,
			  GA_ParameterizationType uParmType=GA_PARAMETERIZATION_CHORD,
			  GA_ParameterizationType vParmType=GA_PARAMETERIZATION_CHORD,
			  GA_KnotSpaceType uKnotSpaceType=GA_KNOT_SPACE_AVERAGING,
			  GA_KnotSpaceType vKnotSpaceType=GA_KNOT_SPACE_AVERAGING,
			  const UT_Vector *uParmValues = 0,
			  const UT_Vector *vParmValues = 0);

    /// Same as global interpolation, but fits to break points.
    /// If not open or at least order four, reverts to interpGlobal.
    int		 	 interpBreakpoints(
			  const GA_OffsetMatrix &goMesh,
			  int uOrder=4, int vOrder=4,
			  bool uWrapped=false, bool vWrapped=false,
			  GEO_SurfaceType type=GEO_PATCH_QUADS,
			  GA_ParameterizationType uParmType=GA_PARAMETERIZATION_CHORD,
			  GA_ParameterizationType vParmType=GA_PARAMETERIZATION_CHORD,
			  GA_KnotSpaceType uKnotSpaceType=GA_KNOT_SPACE_AVERAGING,
			  GA_KnotSpaceType vKnotSpaceType=GA_KNOT_SPACE_AVERAGING,
			  const UT_Vector *uParmValues = 0,
			  const UT_Vector *vParmValues = 0);

    /// Approximate the set of data points given a tolerance.
    /// uNoMultipleKNots and vNoMultipleKnots specify that no multiple knots is
    /// allowed in u and v parameteric direction respectively.
    ///
    /// This approxmation is mainly for unwrapped surface.
    /// For wrapped curve, it may not behave well on the wrapped patch.
    /// However, you can improve its behaviour by duplicating the first 
    /// row/col data points and put it at the end.
    void		 approxGlobal(const GA_OffsetMatrix &gpMesh,
			      int uOrder, int vOrder, 
			      int uWrapped, int vWrapped,
			      GEO_SurfaceType type,
			      float tol, float smooth=0.0F,
			      int uNoMultipleKnots=1, int vNoMultipleKnots=1);


    /// Get the bounding box for a specific range of u and v
    virtual void	getRangeBBox(const UT_Interval &u,
				     const UT_Interval &v,
				     UT_BoundingBox &bbox,
				     const GA_PwHandleRO &h) const;

protected:
    virtual GEO_Hull    *getSubHull(int top, int left, int bottom, int right,
				    GEO_SubHullFlags break_flags);

private:
    // Private methods to translate/transform selected breakpoints and
    // perform an interpolation.
    virtual int          translateBkptsFixed(const UT_IntArray &uindices,
                                             const UT_IntArray &vindices,
                                             const UT_Vector3 &delta,
					     GA_PointGroup *ptgroup=NULL,
					     GEO_Delta *geodelta = 0);
    virtual int		 transformBkptsFixed(const UT_IntArray &uindices,
					     const UT_IntArray &vindices,
					     const UT_Matrix4 &matx,
					     GA_PointGroup *ptgroup=NULL,
					     GEO_Delta *geodelta = 0);

    // Helper functions for approximation.

    void 		 fit(const GA_OffsetMatrix &gpMesh,
			     UT_Vector *data[3], UT_Vector *vData,
			     int prevUEnd, int cvUEnd, 
			     int prevVEnd, int cvVEnd, 
			     UT_Vector &uSol, UT_Vector &vSol,
			     UT_Vector &uParm, UT_Vector &vParm,
			     UT_MatrixF &error, float smooth);

    void		 makeSafeDegreeEvaluateNoMult(int uDir, int m, int n, 
			     const UT_Vector &uParm, const UT_Vector &vParm,
			     UT_MatrixF &error, float tol, 
			     GA_PointGroup  *deleteGroup,
			     int &cvUEnd, int &cvVEnd);

    void		 makeSafeDegreeEvaluateWMult(int uDir, int m, int n, 
			     const UT_Vector &uParm, const UT_Vector &vParm,
			     UT_MatrixF &error, float tol, 
			     GA_PointGroup  *deleteGroup,
			     int &cvUEnd, int &cvVEnd, int &breakCount);

    void		 degreeEvaluateNoMult(GA_NUBBasis *basis, 
					      GA_KnotVector &knots, 
					      GA_KnotVector &tmpKnots);

    void		 degreeEvaluateWMult(GA_NUBBasis *basis, 
					     GA_KnotVector &knots, 
					     GA_KnotVector &tmpKnots,
					     int breakCount);

    // Fill the surface with data and build the bases. Return 0 if OK, and -1
    // if error.
    int			 create(int rows, int cols, int orderu=4, int orderv=4,
				int wrapu = 0, int wrapv = 0,
				int interpEndsU = 1, int interpEndsV = 1,
				GEO_SurfaceType type = GEO_PATCH_QUADS,
				int appendPoints = 1);

    static GA_PrimitiveDefinition	*theDefinition;
    friend class			 GU_PrimitiveFactory;
};

#endif
