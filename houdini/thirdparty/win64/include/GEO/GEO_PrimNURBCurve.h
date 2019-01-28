/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 * COMMENTS:
 *	This class implements a non-uniform B-Spline curve defined by 
 *	a set of knots, basis function, and CVs.
 *
 */

#pragma once

#ifndef __GEO_PrimNURBCurve_h__
#define __GEO_PrimNURBCurve_h__

#include "GEO_API.h"
#include "GEO_Curve.h"
#include "GEO_PrimType.h"
#include <UT/UT_Array.h>

class GA_Detail;
class GA_NUBBasis;

class GEO_API GEO_PrimNURBCurve : public GEO_Curve
{
public:
    using GEO_Face::refine;

protected:
    /// NOTE: The constructor should only be called from subclass
    ///       constructors.
    inline GEO_PrimNURBCurve(GA_Detail *d, GA_Offset offset = GA_INVALID_OFFSET)
        : GEO_Curve(d, offset)
    {}

    /// NOTE: The destructor should only be called from subclass
    ///       destructors.
    /// Please read the comments on the parent class destructor.
    virtual ~GEO_PrimNURBCurve() {}

public:
    /// Increases div to the first valid number of divisions, given
    /// the order and closed flag.  For NURBs, this is a straightforward
    /// minimum.
    /// Returns true if the divisions changed.
    static bool		forceValidDivisions(int &div, int order, bool closed);

    // Given a domain value (u), store all the basis derivatives (from 0 to du
    // inclusive) into bmatx. Return the min index of the CVs needed
    // for the linear combination. The indices may exceed the number of
    // vertices if the curve is wrapped, so remember to use % getVertexCount().
    virtual int		evaluateBasisDerivs(float u, float bmatx[][GA_MAXORDER],
					    int &cvoffset, unsigned du = 0,
					    int uoffset = -1) const;

    // Evaluate the basis at the given point in the domain and also return
    // the index of the first CV to linearly combine the basis with. The CV
    // index may exceed the number of vertices if the curve is wrapped,
    // so remember to use modulus (%). This method handles both rational and
    // non-rational curves.
    virtual int		evaluateBasis(float u,float *ubvals, int &cvoffset,
				      unsigned du=0, int uoffset=-1) const;

    // Compute the location of the breakpoint. Return 0 if OK, else -1.
    virtual int		evaluateBreakpoint(int uidx, UT_Vector4 &pos,
					   int du=0) const;

    // compute the un-normalized lengths corresponding to points on the curve
    // evaluated at the valid knots parameter values
    // (ones with which lie within curve paramer domain) and store them
    // in the 'lengths' array. Returns the curve "length" (same as last knot).
    // The meaning of "length" depends on ptype, and can mean chord length,
    // square chord length, approximate arc length.
    virtual float	getKnotLengths(GA_ParameterizationType ptype, 
				       UT_FloatArray &lengths) const;

    // Given a CV index figure out the min/max indicies of the knots between
    // which the curve needs to be re-evaluated if the CV changes. If not
    // given a valid CV index, the method returns -1. Otherwise it returns 0.
    // If the curve wraps the domain we return will be rather large.
    virtual int		domainRangeOfCV(int cvidx, int &mink,int &maxk) const;

    // Given a CV index figure out the min/max breakpoints which are
    // affected if the CV changes.  If not given a valid CV index, the
    // method returns -1.  Otherwise it returns 0.  Also returns -1 if
    // no breakpoints are affected.
    // NOTE: use % breakCount since maxbkp may be >= breakCount
    virtual int         breakpointRangeOfCV(int cvidx, int &minbkp,
                                            int &maxbkp) const;

    // Reparameterize the curve by changing its basis. Everything up and 
    // including the first valid knot remains unchanged. This type of 
    // reparameterization is generally NOT shape preserving:
    virtual void	reparameterize(GA_ParameterizationType ptype);

    // Warp the curve at u by the given delta. Change 1 or 2 Cvs and possibly
    // insert a knot once or more as well. If a knot is inserted or we happen
    // to land exactly on a knot, we change only one CV. The bias makes sense
    // only when changing 2 CVs, and will be ignored altogether if < 0.
    // We return the index of the affected knot in the sequence, or -1 if
    // there's an error.
    virtual int		 warp(float u, const UT_Vector3 &delta,
			      GA_AttributeRefMap &map,
			      float sharpness = 0.0f, float bias = -1.0f);

    // Translate the CVs such that the given breakpoint change positions by
    // the given delta.  Return -1 if something goes wrong, 0 if translation
    // was successful.
    // NOTE: uindices cannot contain any duplicates.  If the curve is closed,
    //       the first and last breakpoint are considered the same.
    virtual int          translateBreakpoints(const UT_IntArray &uindices,
                                              const UT_Vector3 &delta,
					      int fixbkpts = 1,
					      GA_PointGroup *ptgroup = NULL,
					      GEO_Delta *geodelta = 0);
    virtual int		 transformBreakpoints(const UT_IntArray &uindices,
					      const UT_Matrix4 &matx,
					      int fixbkpts = 1,
					      GA_PointGroup *ptgroup = NULL,
					      GEO_Delta *geodelta = 0);

    // Append another face to us in one of two ways: blend the two endpoints
    // or connect them straight or rounded. The bias ranges from 0 to 1 and is
    // relevant only to blending. The tolerance for blending: if 0, the two
    // endpoints will merge into one point with a discontinuity; if less than
    // 1, we insert knots into the curves to minimize the affected areas; if 1,
    // no refinement is done. For the non-blend case, the tolerance will
    // generate a span whose shape goes from round to straight; 0 tolerance
    // means straight connection. If unrefine is on, we'll try to reduce the
    // complexity of the face if we're connecting rounded. We return 0 if OK
    // and -1 if error. Both curves must be open, clamped, and have the same
    // order.
    virtual int		attach(const GEO_Face &face, int blend = 1,
			       float bias = 0.5f, float tolerance = 1.0f,
			       int unrefine = 1,GA_PointGroup *ptgroup=0);

    // Build a planar (domain) face of the same type as us and with the same
    // number of vertices. Copying the (x,y) values of our points to the planar
    // face is optional.
    virtual GD_Face	*planar(GD_Detail &dgdp, int copyxy = 0) const;

    // Set the wrap or open flag, and make sure the basis knot vectors are
    // consistent with the change. These functions are virtual by inheritance.

    // Convert a topologically open curve into a topologically closed one.
    // The rounded flag is mainly for surface end caps to indicate a half cap
    // or a full cap.
    //
    // If rounded = 1: 
    //		closed rounded curve. 
    //		non end interpolated.
    //		may or may not preserve shape depending on the flag.
    //		(however, always preserve shape if original curve is 
    //		 non end interpolated)
    // If rounded = 0: 
    //		closed curve with a straight edge.
    //		end interpolated. 
    //		always preserve the shape. (ignore preserveShape flag)
    virtual void	close(int rounded = 1, int preserveShape = 0);

    // Convert a topologically closed curve into a topologically open one. 
    //
    // For an end interpolation closed curve, we get (topologically) open
    // curve:
    // 1. For preserve shape: 
    //		exactly the same shape without any open gap.
    //		end interpolated open curve.
    // 2. For non preserve shape: 
    //		hull remained but the curve changed shape with an open gap.
    //		end interpolated open curve.
    //
    // For a non end interpolation closed curve, we get (topologically) open
    // curve:
    // 1. For preserve shape: 
    //		a portion of the original curve, thus have open gap.
    //		non end interpolated open curve.
    // 2. For non preserve shape:
    //		hull remained but the curve changed shape with an open gap.
    //		Note: end interpolated open curve.
    virtual void	open(int preserveShape = 0, int = 0);

    // Check if the basis interpolates the endpoints, or change the flag.
    void			toggleEndCondition(void);

    /// Test end interpolation
    bool			getEndInterpolation() const;

    // Insert or delete vertices. The insertion methods return the index if
    // successful and -1 otherwise. The deletion methods return 0 if ok and
    // -1 otherwise. The insertion methods create the point if it does not
    // exist.
    virtual GA_Size	insertVertex(GA_Offset ppt=GA_INVALID_OFFSET, GA_Size where=0);
    virtual GA_Size	appendVertex(GA_Offset ppt=GA_INVALID_OFFSET);
    virtual int		deleteVertex(GA_Size num);

    // Assuming the curve is closed, "unroll" it so that the CVs that are
    // shared to form a wrapped curve are made unique. Also, the curve becomes
    // open. The base class method only flips the open flag. If the curve is
    // not closed, the method returns -1. Otherwise it returns 0.
    virtual int		 unroll(int append_pts = 1);

    // Shift the array of vertices by an offset and wrap around. 
    // Cycle a subrange if the curve is closed and cycle the basis
    // accordingly. The offset can be either negative or positive.
    // Optionally remap the new basis to the original length and origin
    virtual int		 cycle(int amount, int keepSpan = 1);

    // Raise the number of CVs to match the newcount. The shape of the curve
    // (especially if parametric) should NOT change. Return 0 upon success
    // and -1 otherwise.  start and stop define which indices to examine
    // if newcount is negative it is taken as a relative value.
    virtual int          loft(int newcount, int start=-1, int stop=-1);

    // If this curve is open and non-end-interpolating, insert knots such that
    // it becomes end-interpolating without changing its shape. Return 0 if
    // successful and -1 othewise.
    int			 clamp(GA_PointGroup *delpoints = 0);
    void		 unclamp(void);

    // Remove the interior knot at knotIdx num times if possible, 
    // where 1 <= num <= the multiplicity mult of the knot, and
    // U[knotIdx] != U[knotIdx+1].
    // If multiplicity mult is not given, the procedure will compute it.
    // The tol specifies the tolerance of distance between the knot removable
    // position and its actual position. To force knot removal, set tol= -1.0F 
    // The deleteGroup is used to gather unused cv geo points to be deleted.
    // (It is more efficient to delete points in a group.)
    // Note: During the process of knot removal, interior cvs geo points 
    //       may changed and/or deleted.
    // Output: the actual number of times that the knot got removed.

    int			 unrefine(int knotIdx, GA_AttributeRefMap &hlist,
				int num, int mult = 0,
				fpreal tol = 1e-4F,
				GA_PointGroup *deleteGroup = 0);

    int			 unrefine(int knotIdx, int num, int mult	= 0,
				 float tol			= 1e-4F,
				 GA_PointGroup *deleteGroup	= 0);


    // Get error bound for removing of a knot once.
    float		 getKnotRemovalBound(int knotIdx, int mult=0) const;

    // Merge a bunch of NURB curves together. The curves are assumed to 
    // be all NURBS, all of the same order and characteristics such as
    // non/end-interp, open/closed.
    static int 		 mergeGroup(GEO_Detail *gdp, GA_PrimitiveGroup *nurbs);
    static int		 mergeGroup(GEO_Detail *gdp, UT_Array<GEO_Primitive *> *nurbcurves_array);

    // Reverses the vertices of a given face. 
    virtual void         reverse();

    // If ustart and ustop are two values in the valid interval,ustart < ustop,
    // return the part of curve defined by ustart and ustop in a new primitive.
    // Return 0 if a problem is encountered.
    virtual GEO_Curve	*extract(float ustart,float ustop) const;

    virtual const GA_PrimitiveJSON	*getJSON() const;

    /// Builds NURB curves using the specified range of point offsets,
    /// as dictated by curvesizelist and curvepointnumbers, in parallel.
    /// curvepointnumbers lists the *offsets* of the points used by
    /// each curve *MINUS* startpt, i.e. they are offsets relative to startpt,
    /// *not* indices relative to startpt. The offset of the first curve is
    /// returned, and the rest are at consecutive offsets. All
    /// curvepointnumbers must be between 0 (inclusive) and
    /// npoints (exclusive).
    ///
    /// NOTE: Existing primitives *are* allowed to be using the points in
    /// the specified range already, and the curves being created do not
    /// need to use all of the points in the range.  However,
    /// these cases may impact performance.
    /// @{
    static GA_Offset buildBlock(GA_Detail *detail,
            const GA_Offset startpt,
            const GA_Size npoints,
            const GEO_PolyCounts &curvesizelist,
            const int *curvepointnumbers,
            const UT_Array<int> &porders,
            const int uorder = 4,
            const bool closed = false,
            const bool interpends = true);
    static GA_Offset buildBlock(GA_Detail *detail,
            const GA_Offset startpt,
            const GA_Size npoints,
            const GA_PolyCounts &curvesizelist,
            const int *curvepointnumbers,
            const UT_Array<int> &porders,
            const int uorder = 4,
            const bool closed = false,
            const bool interpends = true);
    /// @}

protected:
    static GA_PrimitiveFamilyMask 	buildFamilyMask()
			    { return GEO_Curve::buildFamilyMask(); }

    /// All subclasses should call this method to register the curve intrinsics.
    /// @see GA_IntrinsicManager
    static GA_IntrinsicManager::Registrar
			registerIntrinsics(GA_PrimitiveDefinition &defn)
			    { return GEO_Curve::registerIntrinsics(defn); }

    // Get a new basis of a type that matches our type:
    virtual GA_Basis	*newBasis(void) const;

    /// Set the basis to a copy of the passed-in basis.
    /// NOTE: basis *must* be valid for this curve!
    virtual void        setBasisCopy(const GA_Basis *basis);

    // Parameter correction by Newton iteration.
    void		 correctParam(const UT_Vector4 &p, float &u, 
				      float distTol=1e-10F, float angTol=1e-2F,
				      int maxIter=50) const;

    // Protected methods which translate/transform selected breakpoints and
    // perform an interpolation.
    virtual int		 translateBkptsFixed(const UT_IntArray &uindices,
                                             const UT_Vector3 &delta,
					     GA_PointGroup *ptgroup = NULL,
					     GEO_Delta *geodelta = 0);
    virtual int		 transformBkptsFixed(const UT_IntArray &uindices,
					     const UT_Matrix4 &matx,
					     GA_PointGroup *ptgroup = NULL,
					     GEO_Delta *geodelta = 0);

    // Methods which translate/transform selected breakpoints and do not
    // perform an interpolation.
    int			 translateBkptsNonFixed(const UT_IntArray &uindices,
						const UT_Vector3 &delta,
						GA_PointGroup *ptgroup = NULL,
						GEO_Delta *geodelta = 0);
    int			 transformBkptsNonFixed(const UT_IntArray &uindices,
						const UT_Matrix4 &matx,
						GA_PointGroup *ptgroup = NULL,
						GEO_Delta *geodelta = 0);

    // Increase the order. Return 0 if successful, -1 otherwise (eg.
    // order cannot be increased because it's >= MAXORDER).
    virtual int		 raiseOrderRefMap(int neworder, GA_AttributeRefMap &m);
    virtual int		 raiseOrderInt	  (int neworder);

    // Given a parameter in the face domain, insert as many CVs as necessary to
    // create a discontinuity at the corresponding point on the curve.The shape
    // of the curve should NOT change. Return u's index upon success and -1
    // otherwise.
    virtual int		subdivideRefMap	(float u, GA_AttributeRefMap &map);
    virtual int		subdivideFloat	(float u);
 
    // Change the multiplicity of the knot by inserting it "r" times after
    // checking its current multiplicity. The maximum valid r for a knot is
    // equal to the degree of the basis (ie order-1). Return -1 if k is outside
    // the valid interval or greater than the number of CVs. Otherwise, return
    // the index of the inserted knot and the initial multiplicity.
    virtual int		refineRefMap(float k, GA_AttributeRefMap &hl, int r=1);
    virtual int		refineInt(float k,int r = 1);
private:
    int			 xformBreakpoints(const UT_IntArray &uindices,
					  const UT_Vector3Array &trans,
					  GA_PointGroup *ptgroup = NULL,
					  GEO_Delta *geodelta = 0);

    // Merge the given basis into ours and refine the curve accordingly.
    // It is assumed that both bases have the same order and that "basis"
    // contains all our knots plus possibly others. Also, it is assumed that
    // "basis" has been GA_Basis::map()-ed onto our knot interval. The 
    // method returns 0 if OK and -1 if problems.
    // Finally, it is assumed that the basis has no knots of higher
    // multiplicity than degree.  This is not checked for here, so one
    // must ensure the resulting basis is of proper length.
    int			merge(const GA_NUBBasis &basis);

    // Return the multiplicity of knot k, cycling the curve first and/or
    // unsharing the wrapped points if necessary. 
    int			 alignKnot(float k, int &kidx, 
				    int &cycfix, int &interpfix);
    void		 unAlignKnot(int &kidx, int cycfix, int interpfix);
SYS_DEPRECATED_PUSH_DISABLE()
};
SYS_DEPRECATED_POP_DISABLE()

#endif
