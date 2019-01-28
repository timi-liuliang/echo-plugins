/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 * COMMENTS:
 *	This class implements a piecewise Bezier curve defined by 
 *	a set of breakpoints, basis function, and CVs.
 *
 * 
 */

#pragma once

#ifndef __GEO_PrimRBezCurve_h__
#define __GEO_PrimRBezCurve_h__

#include "GEO_API.h"
#include "GEO_Curve.h"
#include "GEO_PrimType.h"

class GA_Detail;

class GEO_API GEO_PrimRBezCurve : public GEO_Curve
{
public:
    using GEO_Face::refine;

protected:
    /// NOTE: The constructor should only be called from subclass
    ///       constructors.
    GEO_PrimRBezCurve(GA_Detail *d, GA_Offset offset = GA_INVALID_OFFSET)
        : GEO_Curve(d, offset)
    {}

    /// NOTE: The destructor should only be called from subclass
    ///       destructors.
    /// Please read the comments on the parent class destructor.
    virtual ~GEO_PrimRBezCurve() {}

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
    virtual int		evaluateBasisDerivs(float u,float bmatx[][GA_MAXORDER],
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
					   int du = 0) const;

    //  Remove all repeated vertices - (only gets consecutive vertices)
    virtual GA_Size     removeRepeatedVertices(bool check_order = false,
				bool count_only = false,
				bool deleteOrphanedPoints = false);

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

    // Reparameterize the curve by changing its basis. The first breakpoint
    // remains unchanged.
    virtual void	reparameterize(GA_ParameterizationType ptype);

    // Set the closed or open flag, and make sure the basis knot vector is
    // consistent with the change. These functions are virtual by inheritance.
    // They change the number of CVs, so use them with care. Also, they do
    // not keep a history of changes: if you start with a closed curve
    // and then you open and close it again, the re-closed curve uses 
    // 'degree-2' new CVs that have been generated for this purpose.

    // Close up an open curve.
    //
    // If preserveShape = 1:
    // 		add extra cvs at the end to define the last span.
    //		if rounded, 
    //			the new cvs just added will be arranged to 
    //			give a rounded appearance.
    //          else
    //			the new cvs just added will form a straight 
    //			line between the first and last endpoint.
    // If preserveShape = 0:
    //		just grow the basis by one span, without adding any new cvs.
    //		ignore the rounded flag.
    virtual void	close(int rounded = 1, int preserveShape = 0);

    // Open up a closed curve.
    //
    // If preserveShape = 1:
    // 		the open curve will have exactly the same geometry
    // 		(ie. still look closed) with the first point duplicated.
    // If preserveShape = 0:
    //		the open curve will remove the last span in the basis, 
    //		thus open up a gap in the curve. 
    //		last few cvs are dangling which do not form a valid 
    //		curve segment, if the safe flag = 0. 
    // The safe flag is to remove unneed cvs for ensuring the resulting
    // curve is a valid bezier curve.
    virtual void	open(int preserveShape = 0, int safe = 0);

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
    virtual int		unroll(int append_pts = 1);

    // Raise the number of CVs to match the newcount. The shape of the curve
    // (especially if parametric) should NOT change. Return 0 upon success
    // and -1 otherwise.  start and stop define which indices to examine
    // if newcount is negative it is taken as a relative value.
    virtual int		loft(int newcount, int start=-1, int stop=-1);

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
    // means straight connection.  We return 0 if OK and -1 if error. Both
    // curves must be open and have the same order.
    virtual int		 attach(const GEO_Face &face, int blend = 1,
			       float bias = .5f, float tolerance = 1.f, int=0,
			       GA_PointGroup *ptgroup=0);

    // Build a planar (domain) face of the same type as us and with the same
    // number of vertices. Copying the (x,y) values of our points to the planar
    // face is optional.
    virtual GD_Face	*planar(GD_Detail &dgdp, int copyxy = 0) const;

    // reverse the vertices of the curve, preserving shape
    virtual void 	 reverse(void);

    // If ustart and ustop are two values in the valid interval,ustart < ustop,
    // return the part of curve defined by ustart and ustop in a new primitive.
    // Return 0 if a problem is encountered.
    virtual GEO_Curve	*extract(float ustart,float ustop) const;

    virtual const GA_PrimitiveJSON	*getJSON() const;

    /// Builds Bezier curves using the specified range of point offsets,
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
            const bool closed = false);
    static GA_Offset buildBlock(GA_Detail *detail,
            const GA_Offset startpt,
            const GA_Size npoints,
            const GA_PolyCounts &curvesizelist,
            const int *curvepointnumbers,
            const UT_Array<int> &porders,
            const int uorder = 4,
            const bool closed = false);
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

    // Two different closing methods:
    void		 closeSharp(void);
    void		 closeRounded(void);

    // increase order by 1.
    void		 incrementOrder(GA_AttributeRefMap &map);
    void		 incrementOrder(void);

    // Increase the order. Return 0 if successful, -1 otherwise (eg.
    // order cannot be increased because it's >= MAXORDER).
    virtual int		 raiseOrderRefMap(int neworder, 
					    GA_AttributeRefMap &map);
    virtual int		 raiseOrderInt (int neworder);

    // Insert a breakpoint at the given point in the domain and return its 
    // index if successful. Return -1 in case of failure (eg. if outside the 
    // valid domain or equal to an existing breakpoint).
    virtual int		refineRefMap(float k, GA_AttributeRefMap &hlist, int=0);
    virtual int		refineInt(float k, int=0);

private:
    // Nothing.
SYS_DEPRECATED_PUSH_DISABLE()
};
SYS_DEPRECATED_POP_DISABLE()
#endif
