/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_Basis.h ( GA Library, C++)
 *
 * COMMENTS:	Stores an array of knots for a NURBs/Bezier patch
 */

#pragma once

#ifndef __GA_Basis__
#define __GA_Basis__

#include "GA_API.h"
#include "GA_Defines.h"
#include "GA_KnotVector.h"

#include <UT/UT_VectorTypes.h>

#include <SYS/SYS_Types.h>

#include <iosfwd>


class UT_IStream;
class UT_JSONParser;
class UT_JSONValue;
class UT_JSONWriter;


enum GA_BASIS_TYPE
{
    GA_BEZIER_BASIS,
    GA_NURBS_BASIS,
};

#define GA_BASIS_BEZIER_NAME "Bezier"
#define GA_BASIS_NURBS_NAME "NURBS"

/// @brief Bezier or NURBS basis classes which maintain knot vectors
/// 
/// The GA_Basis class maintains the knot vectors for Bezier or NURBS
/// splines.  The basis consists of:
///  -# The order of the basis (degree+1)
///  -# The knot vector (an array of doubles)
///  -# A flag for end interpolation (NURBS only)
class GA_API GA_Basis
{
public:
    /// Tolerance used to check equality of knot values.
    /// The default of 1e-5 used by SYSisEqual et al. is too large
    /// for NURBS curves with 45K vertices.
    static const fpreal theBasisTolerance;

    /// Table of binomial coefficients
    static const fpreal theBinomial[GA_MAXORDER][GA_MAXORDER];

    /// The default constructor will initialize order=0 and length=0.
    GA_Basis();
    GA_Basis(const GA_Basis &src);

    /// Destructor
    virtual ~GA_Basis() {}

    /// Re-initializes the basis to default values
    virtual void reset() = 0;

    /// Compare to see whether the basis is equal
    virtual bool	isEqual(const GA_Basis &basis) const = 0;

    /// Return the type of the basis
    virtual GA_BASIS_TYPE	 getType() const = 0;

    /// Return the name of the basis
    virtual const char		*getTypeName() const = 0;

    /// Return the order of the basis
    int			 getOrder() const	{ return myOrder; }

    /// Return the length of the knot vector
    int			 getLength() const	{ return myData.entries(); }

    /// Return the dimension of the basis.  For NURBS, this will be equivalent
    /// to (getLength()-getOrder()).  For Beziers the dimension is equivalent
    /// to the length of the knot vector.
    virtual int		 getDimension() const = 0;

    /// Get the boundaries of the valid evaluation interval (as indices)
    virtual bool	getValidInterval(int &a, int &b) const = 0;
    /// Get the boundaries of the valid range of evaluation
    bool		getValidRange(fpreal &a, fpreal &b) const;

    /// Evaluate on a specific domain interval, on which at least one
    /// basis function is non-zero given domain point u.
    virtual void	evalInterval(fpreal u, int offset, int deriv, 
				fpreal64 *vals) const = 0;
    virtual void	evalInterval(fpreal u, int offset,
				int deriv, fpreal32 *vals) const = 0;

    /// Evaluate all the derivatives of the basis from 0 to deriv (inclusive).
    virtual void	evalDerivativeMatrix(fpreal u, int offset, int deriv,
				     fpreal64 bmatx[][GA_MAXORDER]) const = 0;
    virtual void	evalDerivativeMatrix(fpreal u,
				     int offset, int deriv,
				     fpreal32 bmatx[][GA_MAXORDER]) const = 0;

    /// Return either the break-point (Bezier) or the first valid segment for
    /// NURBs.  Returns -1 on an error.
    virtual int	findStartOffset(fpreal u, int uoffset) const = 0;

    /// Compute one basis function (ie. the one with the given index) 
    /// value at u.
    virtual fpreal	computeBValue(int index, fpreal u) const = 0;

    /// Return the number of breakpoints (ie. unique knots) in the knot
    /// vector, in the valid interval.
    virtual int	getBreakCount() const = 0;

    /// Given the index of a knot (kidx) and two bounds in the knot sequence
    /// (a and b) find out the index of the breakpoint that the knot represents,
    /// and possibly adjust kidx so that knot[kidx+1] > knot[kidx]. Return -1
    /// if not found.
    virtual int	knotToBreakpoint(int &kidx, int a, int b) const = 0;

    /// Compute an array of breakpoints (i.e. unique knots) in the valid
    /// interval and return their number.  The tolerance is used to
    /// differentiate between close knots.
    virtual int	getBreakpoints(GA_KnotVector &a,
				fpreal tol=theBasisTolerance) const = 0;

    /// Compute an array of multiplicities for each knot in the knot sequence.
    /// Return the number of breakpoints (i.e. unique knots).
    int		getMultiplicities(UT_IntArray &arr) const;

    /// Return the multiplicity of a domain point or -1 if outside the domain.
    /// "uidx" is the index of the largest breakpoint <= u.
    virtual int         getMultiplicity(fpreal u, int &uidx) const = 0;

    /// Return the expected multiplicity of the end knots (1 by default):
    virtual int		getEndMultiplicity() const;

    /// Given a domain range in the valid interval, compute the range of CVs
    /// that will be involved in the evaluation of the curve in that range.
    /// Since the basis doesn't know about wrapping, the endcv index might
    /// be higher than the spline's number of CVs.
    virtual void	getCvRangeOfDomain(int ustartidx, int ustopidx,
				int &start_cv, int &end_cv) const = 0;
    virtual void	getCvRangeOfDomain(fpreal ustart, fpreal ustop,
				int &start_cv, int &end_cv) const = 0;

    /// Given valid breakpoint index, compute the range of CVs that have a
    /// non-zero influence at the knot of the breakpoint.  Since the basis
    /// doesn't know about wrapping, the endcv index might be higher than the
    /// spline's number of CVs.
    virtual void	getCvRangeOfBreakpoint(int bkp,
				int &startcv, int &stopcv) const = 0;

    /// Generate a new basis of the species specified in the name or type
    /// respectively.
    static GA_Basis	*newSpecies(GA_BASIS_TYPE type);

    /// Returns true if any NANs snuck into this vector.
    bool testForNan() const;

    /// I/O functions for houdini9 format
    virtual bool	saveH9(std::ostream &os, bool wrapped, bool binary) const = 0;
    virtual bool	loadH9(UT_IStream &is, int cvs, bool wrapped) = 0;

    /// Save data to a JSON stream.
    /// @section JSON-GA_Basis JSON Schema: GA_Basis
    /// @code
    /// {
    ///     "name"          : "GA_Basis",
    ///     "description"   : "A basis definition.",
    ///     "type"          : "orderedmap",
    ///     "properties": {
    ///         "type": {
    ///             "type"        : "string",
    ///             "description" : "Type of basis (case sensitive)",
    ///             "enum"        : [ "Bezier", "NURBS" ],
    ///         },
    ///         "order": {
    ///             "type"        : "integer",
    ///             "description" : "Order of the basis",
    ///             "minimum"     : 2,
    ///         },
    ///         "endinterpolation": {
    ///             "type"        : "boolean",
    ///             "description" : "End interpolation",
    ///             "default"     : true,
    ///             "optional"    : true,
    ///         },
    ///         "knots": {
    ///             "type"        : { "$ref" : "GA_KnotVector" }
    ///             "description" : "Knot vector",
    ///         },
    /// }
    /// @endcode
    /// @see @ref JSON_FileFormat
    bool		jsonSave(UT_JSONWriter &w) const;

    /// Save to a JSON value
    bool		jsonSave(UT_JSONValue &v) const;

    /// Load basis from a JSON parser
    bool		jsonLoad(UT_JSONParser &p);

    /// Load basis from a JSON value
    bool		jsonLoad(UT_JSONParser &p, const UT_JSONValue &v);

    /// Copy my data from the given source. If performing a compatible copy, we
    /// only do the copy if if b has the same order and length as we do.
    virtual bool 	copyFrom(const GA_Basis &b, bool compatible=false);

    /// When making the basis valid, the adapt enum will be used to adjust the
    /// end-interpolation and knot values to ensure that the basis is valid.
    enum
    {
	GA_BASIS_ADAPT_NONE,
	GA_BASIS_ADAPT_FLAGS,	// Change flags to match knot values
	GA_BASIS_ADAPT_KNOTS,	// Change knots to match flags
    };
    /// The validate() method will force the basis to be valid (if possible).
    /// The adapt enum can be used to control the behaviour of this method.
    virtual bool	validate(int adapt=GA_Basis::GA_BASIS_ADAPT_NONE) = 0;

    /// The checkValid() methods test to see whether th basis is valid given
    /// a curve with
    ///  -# cvLen vertices
    ///  -# A basis length of bLen
    ///  -# Periodicity determined by doesWrap
    virtual bool	checkValid(int cvLen, int bLen, bool doesWrap) const = 0;
    /// Convenience method to check validity using the basis's length
    bool	checkValid(int cvLen, bool doesWrap) const
		    { return checkValid(cvLen, getLength(), doesWrap); }

    /// Compute the idx'th greville abscissa of the domain vector. Clamping
    /// to the valid interval is optional and might not make any difference 
    /// for some spline types.
    virtual fpreal 	getGreville(int idx, bool clamp=true,
				bool wrap=false) const = 0;

    /// Grow the length of the basis by one, and set the value of the new entry
    /// to last knot+1. The method returns the index of the appended element.
    virtual int		grow(bool wrapped=false) = 0;

    /// Shrink the basis by one. The basis should not shrink beyond a valid
    /// length. Return the new length.
    virtual int		shrink(bool wrapped=false) = 0;

    /// Attach another basis to us and grow our basis as a result. The bases
    /// must have the same type and order.
    /// If "overlap" is true, we overlap the beginning of b with 
    /// our end.
    /// Spreading makes sense when you can have multiple knots in the basis, and
    /// causes identical knots to be spread within range of the neighbouring
    /// knots.
    virtual bool	attach(const GA_Basis &b, bool overlap=true,
				bool spread=false) = 0;

    /// Change the size of the basis (and maybe some of its values too) to 
    /// make it a valid basis for wrapped splines.
    virtual void	setWrapping(bool wrap) = 0;

    /// Reverse the breakpoints in the basis.
    virtual void	reverse(bool wrapped) = 0;

    /// Find index in the knot vector for the break point corresponding to k. 
    virtual int		findOffset(fpreal k, int startIdx=0) const = 0;

    /// Convert a value from unit to real knot. Usually you'll want to map
    /// only within the valid interval.
    fpreal	unitToReal(fpreal u_unit, bool valid_interval=true) const;
    /// Convert from real to unit values.  Usually, you'll want to map only
    /// within the valid interval.
    fpreal	realToUnit(fpreal u_real, bool valid_interval=true) const;

    /// Append a knot value to the end of the knot vector
    int		append(fpreal v)	{ return myData.append(v); }
    /// Insert a knot value at a specific location
    /// @note The order of arguments is different that UT_ValArray
    int		insertKnot(fpreal v, int i)
		{
		    return myData.insertKnot(v, i);
		}
    /// Remove a knot value (by index)
    int		remove(int i)		{ return myData.removeIndex(i); }

    /// Merge the given basis into ours provided they both have the same order.
    /// If their order is different returns false. Otherwise return true.
    /// A copy of "b"'s knots is mapped to our knot interval first, then merged
    /// with us.
    /// The second method stres the mapping into "b" itself.
    bool	merge(const GA_Basis &b);
    bool	merge(GA_Basis &b);

    /// Find the index of a breakpoint such that the distance between it and the
    /// next breakpoint is the largest in the whole sequence. The search is
    /// done between two specified indices.
    int		findMaxSpan(int start, int stop) const;

    /// Convenience method to normalize this basis.
    void	normalize(fpreal scale=0, const fpreal *neworig=0);

    /// Rebuild the basis as a uniform sequence with a given step.
    virtual void	rebuild(fpreal ustart=0, fpreal ustep=1) = 0;

    /// Make the basis uniform of just find out if it is uniform:
    virtual void	makeUniform(fpreal ustep=1) = 0;

    /// Normalize the vector to the new length and optionally shift it to 
    /// a new origin. If "knots" is given, the resulting values are put in it.
    /// The first method maps us to the interval of basis "b". In other words,
    /// our extremities will be changed to match those of basis "b".
    void		map(const GA_Basis &b);
    static void		map(GA_KnotVector &knots, const GA_KnotVector &src,
				fpreal newlen=1, const fpreal *neworig=0);
    void		map(fpreal newlen=1, const fpreal *neworig=0)
			{
			    map(myData, myData, newlen, neworig);
			}

    /// Map domain value "u", which should be between our min and max knots,
    /// to a domain value in basis "b". If you know the index of the knot
    /// in our sequence such that k[uoffset] <= u, enter in as the last
    /// parameter. The method returns -1 if it has failed (the two bases don't
    /// have the same length or u is out of range). Otherwise it returns the
    /// uoffset.
    int			map(const GA_Basis &b, fpreal &u,
				int uoffset=0) const;

    /// Reparameterize the basis using the chord-length method, and clamp it
    /// to the valid interval. The origin and length of the valid domain remains
    /// unchanged.
    virtual void	chord(UT_Vector4Array &cvs) = 0;

    /// Slide the knots found in the given range left or right by an amount at
    /// most as large as the distance to the nearest knot outside the range.
    /// The bias is a percentage value of the left-right distance, its default
    /// value is 0.5 (i.e. don't change anything), and is clamped to [0,1].
    virtual bool	slideRange(fpreal umin, fpreal umax,
				fpreal ubias=0.5) = 0;

    /// Set the order of the basis. Careful when you use it because it must
    /// be <= GA_MAXORDER and must be appropriate for the number of knots.
    void		setOrder(int ord)	{ myOrder = ord; }

    /// Get the greatest value knot that is smaller than the search value.
    /// Return false if search value is out of range.
    /// The default search range is from order-1 to dimension.
    bool 		findClosest(fpreal val, int &idx, 
				int startidx, int endidx) const;

    /// Return the index of the first knot that is approximately equal to 
    /// val. The entire sequence is searched from startidx.
    /// Return  -1 if not found.
    int			findApproximate(fpreal val, int startidx,
				fpreal tol=theBasisTolerance) const;

    /// Compute alphas needed for degree elevation. "increment" is the amount
    /// by which we want the degree to grow.
    void		computeRaiseOrderAlphas(int increment,
				fpreal bezalfs[][GA_MAXORDER]) const;

    // Grow or shrink the basis array
    void	resize(int sz);

    /// @{
    /// Get the knot vector for the basis
    const GA_KnotVector	&getVector() const	{ return myData; }
    GA_KnotVector	&getVector() 		{ return myData; }
    const GA_KnotVector	&getKnotVector() const	{ return myData; }
    GA_KnotVector	&getKnotVector() 	{ return myData; }
    /// @}

    /// Return the amount of memory used
    virtual int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += myData.getMemoryUsage(false);
        return mem;
    }

protected:
    const GA_KnotFloat	*getData() const	{ return myData.getRawArray(); }
    GA_KnotVector	myData;	// TODO: Replace with UT_DoubleArray
    int			myOrder;

    static void doMerge(GA_KnotVector &avec, const GA_KnotVector &bvec,
			GA_KnotVector *inserts=0);
};

#endif
