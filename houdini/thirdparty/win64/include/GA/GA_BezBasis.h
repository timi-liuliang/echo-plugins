/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_BezBasis.h ( GA Library, C++)
 *
 * COMMENTS:	Stores an array of knots for a Bezier patch
 */

#pragma once

#ifndef __GA_BezBasis__
#define __GA_BezBasis__

#include "GA_API.h"
#include "GA_Basis.h"
#include "GA_Defines.h"                 // for GA_MAXORDER
#include "GA_KnotVector.h"              // for GA_KnotVector

#include <UT/UT_VectorTypes.h>          // for UT_FloatArray, etc

#include <SYS/SYS_Types.h>              // for fpreal, etc

#include <iosfwd>


class UT_IStream;


/// @brief Bezier basis classes which maintain knot vectors
/// 
/// The GA_BezBasis class maintains the knot vectors for Bezier
/// splines.  The basis consists of:
///  -# The order of the basis (degree+1)
///  -# The knot vector (an array of doubles)
class GA_API GA_BezBasis : public GA_Basis
{
public:
    /// The default constructor will choose length/order based on the defaults
    /// for the basis type.
    /// default: order=4, length=2
    GA_BezBasis(int length=-1, int order=-1);
    GA_BezBasis(fpreal start, fpreal step, int length, int order=-1);
    GA_BezBasis(const GA_BezBasis &src);

    /// Test to see if two bases are equal
    virtual bool	isEqual(const GA_Basis &basis) const;
    bool	operator==(const GA_BezBasis &a) const;
    bool	operator!=(const GA_BezBasis &a) const
		    { return !(*this == a); }

    /// Re-initializes the basis to default values
    virtual void reset();

    /// Return the type of the basis
    virtual GA_BASIS_TYPE	 getType() const;

    /// Return the name of the basis
    virtual const char		*getTypeName() const;

    /// Return the dimension of the basis.  For Beziers the dimension is
    /// equivalent to the length of the knot vector.
    virtual int		 getDimension() const;

    /// Get the boundaries of the valid evaluation interval (as indices)
    virtual bool	getValidInterval(int &a, int &b) const;

    /// Evaluate on a specific domain interval, on which at least one
    /// basis function is non-zero given domain point u.
    virtual void	evalInterval(fpreal u, int offset, int deriv, 
				fpreal64 *vals) const;
    virtual void	evalInterval(fpreal u, int offset, int deriv, 
				fpreal32 *vals) const;

    /// Evaluate all the derivatives of the basis from 0 to deriv (inclusive).
    virtual void	evalDerivativeMatrix(fpreal u, int offset, int deriv,
				     fpreal64 bmatx[][GA_MAXORDER]) const;
    virtual void	evalDerivativeMatrix(fpreal u, int offset, int deriv,
				     fpreal32 bmatx[][GA_MAXORDER]) const;

    /// Return the first break-point.  Returns -1 on an error.
    virtual int	findStartOffset(fpreal u, int uoffset) const;

    /// Compute one basis function (ie. the one with the given index) 
    /// value at u.
    virtual fpreal	computeBValue(int index, fpreal u) const;

    /// Return the number of breakpoints (ie. unique knots) in the knot
    /// vector, in the valid interval.
    virtual int	getBreakCount() const;

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
    virtual int         getMultiplicity(fpreal u, int &uidx) const;

    /// Return the expected multiplicity of the end knots (1 by default):
    virtual int		getEndMultiplicity() const;

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
    bool	import(int order, const UT_FloatArray &knots);

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
    virtual fpreal 	getGreville(int idx, bool clamp=true,
				bool wrap=false) const;

    /// Grow the length of the basis by one, and set the value of the new entry
    /// to last knot+1. The method returns the index of the appended element.
    virtual int		grow(bool wrapped=false);

    /// Shrink the basis by one. The basis should not shrink beyond a valid
    /// length. Return the new length.
    virtual int		shrink(bool wrapped=false);

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

    /// Reverse the breakpoints in the basis.
    virtual void	reverse(bool wrapped);

    /// Find index in the knot vector for the break point corresponding to k. 
    virtual int		findOffset(fpreal k, int startIdx=0) const;

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

private:
    void	initialize(fpreal start, fpreal step, int length, int order);

    /// Test whether the basis is a uniform basis.
    bool	isUniform() const;

    /// Return -1 if v is less than the 0th breakpoint or the index of the
    /// last bkp + 1 if v is greater than the last breakpoint.  Otherwise
    /// returns the index of the last breakpoint less than or equal to v.
    int		findBezierBreakpoint(fpreal v) const;
};

#endif
