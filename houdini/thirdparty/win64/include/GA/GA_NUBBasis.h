/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_NUBBasis.h ( GA Library, C++)
 *
 * COMMENTS:	Stores an array of knots for a NURBS patch
 */

#pragma once

#ifndef __GA_NUBBasis__
#define __GA_NUBBasis__

#include "GA_API.h"
#include "GA_Basis.h"
#include "GA_Defines.h"
#include "GA_KnotVector.h"

#include <UT/UT_VectorTypes.h>
#include <SYS/SYS_Types.h>

#include <iosfwd>


class UT_IStream;

template<typename T> class UT_VectorT;
typedef UT_VectorT<fpreal64> UT_Vector;


/// Knot scale factor for unclamping end interpolating curves/surfaces.
#define GA_NUB_UNCLAMP_SCALE   3.0


/// @brief NURBS basis classes which maintain knot vectors
/// 
/// The GA_NUBBasis class maintains the knot vectors for NURBS.
/// The basis consists of:
///  -# The order of the basis (degree+1)
///  -# The knot vector (an array of doubles)
///  -# A flag for end interpolation
class GA_API GA_NUBBasis : public GA_Basis
{
public:
    /// The default constructor will choose length/order based on the defaults
    /// for the basis type.
    /// default: order=4, length=8
    GA_NUBBasis(int length=-1, int order=-1, bool end_interpolation=true);
    GA_NUBBasis(fpreal start, fpreal step, int length, int order=-1, bool end_interpolation=true);
    // This c-tor allows you to build the equivalent of a Bezier basis. 
    // To end up with a Bezier-like basis, set the multiplicity=ord-1.
    // The end points will always have 'ord' multiplicity, so this is
    // an end-interpolating basis. The multiplicity must be >= 1 and < ord.
    GA_NUBBasis(int bkpoints, int multiplicity, int ord);
    GA_NUBBasis(const GA_NUBBasis &src);

    /// Test to see if two bases are equal
    bool	isEqual(const GA_Basis &basis) const;
    bool	operator==(const GA_NUBBasis &a) const;
    bool	operator!=(const GA_NUBBasis &a) const
		    { return !(*this == a); }

    /// Re-initializes the basis to default values
    virtual void reset();

    /// Return the type of the basis
    virtual GA_BASIS_TYPE	 getType() const;

    /// Return the name of the basis
    virtual const char		*getTypeName() const;

    /// Return the dimension of the basis.  For NURBS, this will be equivalent
    /// to (getLength()-getOrder()).
    virtual int		 getDimension() const;

    /// Return the end-interpolation flag
    bool		 getEndInterpolation() const
			    { return myEndInterpolation; }

    /// Get the boundaries of the valid evaluation interval (as indices)
    virtual bool	getValidInterval(int &a, int &b) const;

    /// Return the index of the knot where the valid evaluation begins.
    /// Typically, for a cubic spline, this index is 3 in the end-point
    /// interpolating case and 1 otherwise. If u is completely outside
    /// the valid knot sequence, this method returns -1.
    ///
    /// Equivalent to findStartOffset(u, uoffset).
    int			findValidStart(fpreal u, int uoffset = -1) const
			    { return findNURBSValidStart(u, uoffset); }

    /// Return the index in the knot vector such that knotVec[idx] <= k and
    /// k < knotVec[idx+1].  If k is less than the first valid knot, return
    /// -1, and if k is greater than the last valid knot, return the index
    /// of the last valid knot (i.e. the dimension).
    int			findStart(fpreal k, int start_idx = 0) const
			    { return findNURBSStart(k, start_idx); }

    /// Evaluate on a specific domain interval, on which at least one
    /// basis function is non-zero given domain point u.
    virtual void	evalInterval(fpreal u, int offset, int deriv, 
				fpreal64 *vals) const;
    virtual void	evalInterval(fpreal u, int offset, 
				int deriv, fpreal32 *vals) const;

    /// Evaluate all the derivatives of the basis from 0 to deriv (inclusive).
    virtual void	evalDerivativeMatrix(fpreal u, int offset, int deriv,
				     fpreal64 bmatx[][GA_MAXORDER]) const;
    virtual void	evalDerivativeMatrix(fpreal u,
				     int offset, int deriv,
				     fpreal32 bmatx[][GA_MAXORDER]) const;

    /// Return the first valid segment.  Returns -1 on an error.
    virtual int	findStartOffset(fpreal u, int uoffset) const;

    /// Compute one basis function (ie. the one with the given index) 
    /// value at u.
    virtual fpreal	computeBValue(int index, fpreal u) const;

    /// Find out if two knot sequences are similar (ie if the knot spacings
    /// are the same all over) and their knot count is the same.
    bool	isSimilar(const GA_NUBBasis &b) const;

    /// Return the number of breakpoints (ie. unique knots) in the knot
    /// vector, in the valid interval.
    virtual int	getBreakCount() const;

    /// @{
    /// Find the index of the last knot in the sequence that defines the given
    /// breakpoint, or the breakpoint of the domain value.  Returns -1 if not
    /// found.
    /// @note The search is done within the limits of the valid interval.
    int		findBreakpoint(int idx) const;
    int		findBreakpoint(fpreal t) const;
    /// @}

    /// Given the index of a knot (kidx) and two bounds in the knot sequence
    /// (a and b) find out the index of the breakpoint that the knot represents,
    /// and possibly adjust kidx so that knot[kidx+1] > knot[kidx]. Return -1
    /// if not found.
    virtual int	knotToBreakpoint(int &kidx, int a, int b) const;

    /// Compute an array of breakpoints (i.e. unique knots) in the valid 
    /// interval and return their number.  The tolerance is used to
    /// differentiate between close knots.
    virtual int	getBreakpoints(GA_KnotVector &a,
				fpreal tol=theBasisTolerance) const;

    /// Return the multiplicity of a domain point or -1 if outside the domain.
    /// "uidx" is the index of the largest breakpoint <= u.
    int         getMultiplicity(fpreal u, int &uidx) const;

    /// Return the expected multiplicity of the end knots (1 by default):
    virtual int		getEndMultiplicity() const;

    /// Change the multiplicity of the knot by inserting it "r" times WITHOUT
    /// checking its current multiplicity. kidx is the index of a knot t such
    /// that k >= t.
    void		refine(fpreal k, int kidx, int r, bool wrapped);

    /// Given a domain range in the valid interval, compute the range of CVs
    /// that will be involved in the evaluation of the curve in that range.
    /// Since the basis doesn't know about wrapping, the endcv index might
    /// be higher than the spline's number of CVs.
    virtual void	getCvRangeOfDomain(int ustartidx, int ustopidx,
				int &start_cv, int &end_cv) const;
    virtual void	getCvRangeOfDomain(fpreal ustart, fpreal ustop,
				int &start_cv, int &end_cv) const;

    /// Given valid breakpoint index, compute the range of CVs that have a
    /// non-zero influence at the knot of the breakpoint.  Since the basis
    /// doesn't know about wrapping, the endcv index might be higher than the
    /// spline's number of CVs.
    virtual void	getCvRangeOfBreakpoint(int bkp,
				int &startcv, int &stopcv) const;

    /// I/O functions for houdini9 format
    virtual bool	saveH9(std::ostream &os, bool wrapped, bool binary) const;
    virtual bool	loadH9(UT_IStream &is, int cvs, bool wrapped);

    /// Transitional method while we might need to import data from a GB_Basis.
    /// Be careful calling this method as the order must be appropriate for the
    /// number of knots.
    /// TODO: Remove when no longer necessary.
    bool	import(int order, const UT_FloatArray &knots,
		       bool end_interpolation=true);

    /// Copy my data from the given source. If performing a compatible copy, we
    /// only do the copy if if b has the same order and length as we do.
    virtual bool	copyFrom(const GA_Basis &b, bool compatible=false);

    /// The validate() method will force the basis to be valid (if possible).
    /// The adapt enum can be used to control the behaviour of this method.
    virtual bool	validate(int adapt=GA_Basis::GA_BASIS_ADAPT_NONE);

    /// The checkValid() methods test to see whether th basis is valid given
    /// a curve with
    ///  -# cvLen vertices
    ///  -# A basis length of bLen
    ///  -# Periodicity determined by doesWrap
    virtual bool	checkValid(int cvLen, int bLen, bool doesWrap) const;

    /// Compute the idx'th greville abscissa of the domain vector. Clamping
    /// to the valid interval is optional and might not make any difference 
    /// for some spline types.
    virtual fpreal	getGreville(int idx, bool clamp=true,
				bool wrap=false) const;

    /// Grow the length of the basis by one, and set the value of the new entry
    /// to last knot+1. The method returns the index of the appended element.
    virtual int		grow(bool wrapped=false);

    /// Shrink the basis by one. The basis should not shrink beyond a valid
    /// length. Return the new length.
    virtual int		shrink(bool wrapped=false);

    /// Insert a new knot as a result of a CV insertion at index cvIdx. The
    /// knot index is cvIdx + 1, which is returned by growAt() if all goes OK.
    /// -1 is returned upon failure.
    /// (NURBS basis only)
    int		growAt(unsigned cvIdx, bool wrapped);

    /// Delete a knot as a result of a CV removal at index cvIdx. The knot
    /// index is cvIdx + 1. The method returns the new length if successful,
    /// and -1 otherwise.
    int		shrinkAt(unsigned cvIdx, bool wrapped);

    /// Attach another basis to us and grow our basis as a result. The bases
    /// must have the same type and order.
    /// If "overlap" is true, we overlap the beginning of b with 
    /// our end.
    /// Spreading makes sense when you can have multiple knots in the basis, and
    /// causes identical knots to be spread within range of the neighbouring
    /// knots.
    virtual bool	attach(const GA_Basis &b, bool overlap=true,
				bool spread=false);

    /// Change the size of the basis (and maybe some of its values too) to 
    /// make it a valid basis for wrapped splines.
    virtual void	setWrapping(bool wrap);

    /// Change end interpolation
    void		setEndInterpolation(bool value, bool modify_knots);

    /// Reverse the breakpoints in the basis.
    virtual void	reverse(bool wrapped);

    /// Find index in the knot vector for the break point corresponding to k. 
    virtual int		findOffset(fpreal k, int startIdx=0) const;

    /// Create a new basis containing the partial merging of bases a and b
    /// If their order is different return -1. Otherwise return 0.
    /// The resultant basis contains knots a0..a1 of basis a merged with
    /// knots b0..b1 from basis b (after being mapped onto a0..a1)
    /// If the aknot or bknot array is given, they are set with the
    /// (appropriately mapped) knots values corresponding to the new insertions
    bool	mergePartial(const GA_NUBBasis &a, const GA_NUBBasis &b,
				int a0, int a1, int b0, int b1,
				GA_KnotVector *aknots = 0,
				GA_KnotVector *bknots = 0);

    /// Rebuild the basis as a uniform sequence with a given step.
    virtual void	rebuild(fpreal ustart=0, fpreal ustep=1);

    /// Make the basis uniform of just find out if it is uniform:
    virtual void	makeUniform(fpreal ustep=1);

    /// Reparameterize the basis using the chord-length method, and clamp it
    /// to the valid interval. The origin and length of the valid domain remains
    /// unchanged.
    virtual void	chord(UT_Vector4Array &cvs);

    /// Slide the knots found in the given range left or right by an amount at
    /// most as large as the distance to the nearest knot outside the range.
    /// The bias is a percentage value of the left-right distance, its default
    /// value is 0.5 (i.e. don't change anything), and is clamped to [0,1].
    virtual bool	slideRange(fpreal umin, fpreal umax,
				fpreal ubias=0.5);

    /// Cycle the knots, optionally keeping the original span length and origin.
    bool		cycle(int amount, bool keepSpan);

    /// @{
    /// Set up the knot vector with the given parameterization for the
    /// approximiation.
    void	setKnotsByEqualSpacing(UT_Vector &param, bool wrapped=false);
    void	setKnotsByAveraging(UT_Vector &param, bool wrapped=false);
    void	setKnotsBySpreading(UT_Vector &param);
    void	setKnotsByBreakpoints(UT_Vector &param);
    /// @}

    /// TODO: Public for GD_PrimNURBCurve use

    void	toggleEndInterpolation()
		    { setEndInterpolation(!myEndInterpolation, true); }
    void	makeNURBSPeriodic();

    // TODO: Public for IGES_EntityBSplineSurface use
    void	updateEndInterpolation();

    /// Return the amount of memory used
    virtual int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += GA_Basis::getMemoryUsage(false);
        return mem;
    }

    /// Test whether the basis is a uniform basis.
    bool	isUniform() const;

private:
    void	initialize(fpreal start, fpreal step,
			    int length, int order, bool interp);

    int		findNURBSStart(fpreal k, int start=0) const;
    int		findNURBSValidStart(fpreal u, int off=-1) const;
    void	initializeNURBS(fpreal first, fpreal step);
    void	spreadNURBSKnots(int ai, int bi);

    // NUB basis interpolation end condition
    bool	myEndInterpolation;
};

#endif
