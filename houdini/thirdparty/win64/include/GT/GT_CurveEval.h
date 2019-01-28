/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_CurveEval.h (GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_CurveEval__
#define __GT_CurveEval__

#include "GT_API.h"
#include "GT_Types.h"

#include "GT_Handles.h"
#include <UT/UT_Array.h>

class UT_JSONWriter;

/// Class to perform evaluation of the curve spline types.
///
/// This is similar to UT_Spline but supports higher order curves and a
/// different set of basis functions.
///
/// To evaluate a curve, you do something like: @code
///	const GT_CurveEval	eval(...);
///	int			nspans = eval.spanCount(nvertices);
///	type			result[nspans*points_per_span];
///
/// Note: There only dependency on GT is the basis type.  This can likely be
/// moved to a lower library (i.e. UT).
class GT_API GT_CurveEval
{
public:
    static int	maximumBezierOrder();
    static void	orderRange(GT_Basis basis, int &min_order, int &max_order);

    GT_CurveEval(GT_Basis basis, bool periodic);
    GT_CurveEval(const GT_CurveEval &eval);

    void	setOrder(int order)
    {
	if (order != myOrder)
	    cacheOrderComputation(order);
    }

    /// Class to hold non-uniform knot vector.
    class Knots
    {
    public:
	Knots()
	    : myKnots(0)
	    , mySize(0)
	{
	}
	/// The knots passed in are not owned by this class, so they must stay
	/// in existence while the Knots object is in existence.
	Knots(const fpreal *knots, int length)
	    : myKnots(knots)
	    , mySize(length)
	{
	}
	/// Test validity
	bool		 valid() const	{ return myKnots && mySize > 1; }
	/// Return the full knot vector
	const fpreal	*knots() const	{ return myKnots; }
	/// Return the knots for the given span
	const fpreal	*knots(int span) const { return myKnots + span; }
	/// For a given @c span, and interpolant @c t (between 0 and 1), return
	/// the proper knot value for interpolation.
	fpreal		 getU(int span, int order, fpreal t) const
	{
	    int		n = span + order - 1;
	    UT_ASSERT(n >= 0 && n+1 < mySize);
	    return SYSlerp(myKnots[n], myKnots[n+1], t);
	}
	/// Return the length of the knot vector
	int		 size() const	{ return mySize; }
	/// Lookup a knot
	fpreal		 operator()(int i) const
	{
	    UT_ASSERT(i >= 0 && i < mySize);
	    return myKnots[i];
	}
	/// Lookup a knot
	fpreal		 operator[](int i) const
	{
	    UT_ASSERT(i >= 0 && i < mySize);
	    return myKnots[i];
	}

	/// Set up knot vector
	void	setKnots(const fpreal *knots, int size)
	{
	    myKnots = knots;
	    mySize = size;
	}

	/// @{
	/// Debug
	bool	save(UT_JSONWriter &w);
	void	dump(const char *msg="");
	/// @}
    private:
	const fpreal	*myKnots;
	int		 mySize;
    };

    /// Test if there's a valid count for the given curve type (of the given
    /// order).
    bool	validCount(int nvertices, int order) const;

    /// @{
    /// Access member data
    GT_Basis	basis() const		{ return myBasis; }
    bool	periodic() const	{ return myPeriodic; }
    /// @}

    /// Return the step size for iterating through spans
    int		step() const	{ return myStep; }

    /// Return the number of spans for a given hull
    int		spanCount(int nvtx, int order) const
		    { return GTbasisSpans(myBasis, nvtx, myPeriodic, order); }

    /// The EvalBuffer class is used to store state for evaluation
    class EvalBuffer
    {
    public:
	 EvalBuffer() {}
	~EvalBuffer() {}

	enum
	{
	    // This size is tied to constants in GT_Binomial.h
	    EVAL_BUF_SIZE = 32
	};

	/// Weights for each element in the span
	const fpreal	*w() const	{ return myW; }
	fpreal		*w()		{ return myW; }

	/// @{
	/// Which element in the span contributes the most
	int		conditional() const	{ return myConditional; }
	void		setConditional(int c)	{ myConditional = c; }
	/// @}

	/// Find the element which contributes the most in the span
	void	computeConditional(int nvals)
	{
	    myConditional = 0;
	    for (int i = 1; i < nvals; ++i)
		if (myW[i] > myW[myConditional])
		    myConditional = i;
	}

    private:
	fpreal	myW[EVAL_BUF_SIZE];
	int	myConditional;
    };
    /// @{
    /// Evaluate a the hull of a curve at a given parametric coordinate.
    /// There should be @c order elements in the hull.
    ///
    /// The default template implementation performs conditional copying.
    template <typename DATA_T>
    inline DATA_T	eval(int order, const EvalBuffer &ebuf,
				const DATA_T *hull, int stride=1) const
			{ return hull[ebuf.conditional()*stride]; }
    /// @}

    /// @{
    /// Evaluate a the hull of a curve at a given parametric coordinate and an
    /// indirection array.  The elements in the indirection array are used to
    /// perform the lookup.
    /// There should be @c order elements in the index array.
    template <typename DATA_T>
    inline DATA_T	evalIndex(int order, const EvalBuffer &ebuf,
				const DATA_T *hull, const int *index) const
			{ return hull[index[ebuf.conditional()]]; }
    /// @}

    /// Precompute the evaluation buffer for a span
    void	preCompute(int order, EvalBuffer &ebuf, fpreal t) const;

    /// @{
    /// Refine to numeric arrays
    ///  - @c dest must be an array of GT_DANumeric sub-classes
    ///  - @c dest_offset into the dest arrays to begin writing the evaluated
    ///       data
    ///  - @c dest_count the number of evaluations
    ///  - @c hull The attributes to be evaluated
    ///  - @c hull_offset the offset into the hull arrays
    ///  - @c hull_count is the number of elements in the array for the hull
    /// For example, given a GT_PrimCurveMesh with a two curves: @code
    ///   // Refine 1st curve to 10 points and the second to 32
    ///	  int dpts[] = { 10, 32 }
    ///   UT_Array<GT_DataArrayHandle> arrays;
    ///   const GT_AttributeListHandle &vertex = curve->getcVertexAttributes();
    ///   eval.allocateNumericArrays(arrays, vertex, 42);
    ///   refineToNumeric(arrays, 0, 10, vertex,
    ///		curve->getVertexOffset(0),
    ///		curve->getVertexCount(0));
    ///   refineToNumeric(arrays, 10, 32, vertex,
    ///		curve->getVertexOffset(1),
    ///		curve->getVertexCount(1));
    ///   // Create a destination attribute list
    ///   GT_AttributeListHandle   dest(new GT_AttributeListHandle(*vertex));
    ///   // And copy the array data into the attribute list
    ///	  copyToAttributes(dest, arrays);
    /// @endcode
    /// @note You must ensure that @c setOrder() has been called
    void	allocateNumericArrays(UT_Array<GT_DataArrayHandle> &arrays,
				const GT_AttributeListHandle &hull,
				GT_Size total_points) const;
    bool	refineToNumeric(int order,
				UT_Array<GT_DataArrayHandle> &arrays,
				GT_Offset dest_offset,
				GT_Size dest_count,
				const GT_AttributeListHandle &hull,
				GT_Offset hull_offset,
				GT_Size hull_count,
				const GT_DataArrayHandle &knots,
				GT_Offset knot_offset,
				int segment=0) const;
    bool	copyToAttributes(GT_AttributeListHandle &dest,
				UT_Array<GT_DataArrayHandle> &arrays,
				int segment=0) const;
    /// @}

    /// When computing with knots, there need to be Order*2+1 knots for a span
    void	preCompute(int order, EvalBuffer &ebuf, int span, fpreal t,
			    const Knots &knots) const;

private:
    void	cacheOrderComputation(int order);

    template <typename T>
    inline T	evalF(int order, const EvalBuffer &ebuf,
			const T *hull, int stride) const
    {
	fpreal		 v = 0;
	const fpreal	*w = ebuf.w();
	for (int i = 0; i < order; ++i)
	    v += w[i] * hull[i*stride];
	return v;
    }
    template <typename DATA_T>
    inline DATA_T	evalFIndex(int order, const EvalBuffer &ebuf,
				const DATA_T *hull,
				const int *index) const
    {
	fpreal		 v = 0;
	const fpreal	*w = ebuf.w();
	for (int i = 0; i < order; ++i)
	    v += w[i] * hull[index[i]];
	return v;
    }
    GT_Basis	 myBasis;
    int		 myOrder;
    int		 myStep;
    bool	 myPeriodic;
};

/// @{
/// Explicit floating point specializations that perform interpolation of values
template <> inline fpreal16
GT_CurveEval::eval<fpreal16>(int order, const EvalBuffer &ebuf,
			const fpreal16 *hull, int stride) const
		{ return evalF(order, ebuf, hull, stride); }
template <> inline fpreal32
GT_CurveEval::eval<fpreal32>(int order, const EvalBuffer &ebuf,
			const fpreal32 *hull, int stride) const
		{ return evalF(order, ebuf, hull, stride); }
template <> inline fpreal64
GT_CurveEval::eval<fpreal64>(int order, const EvalBuffer &ebuf,
			const fpreal64 *hull, int stride) const
		{ return evalF(order, ebuf, hull, stride); }

template <> inline fpreal16
GT_CurveEval::evalIndex<fpreal16>(int order, const EvalBuffer &ebuf,
			const fpreal16 *hull, const int *index) const
		{ return evalFIndex<fpreal16>(order, ebuf, hull, index); }
template <> inline fpreal32
GT_CurveEval::evalIndex<fpreal32>(int order, const EvalBuffer &ebuf,
			const fpreal32 *hull, const int *index) const
		{ return evalFIndex<fpreal32>(order, ebuf, hull, index); }
template <> inline fpreal64
GT_CurveEval::evalIndex<fpreal64>(int order, const EvalBuffer &ebuf,
			const fpreal64 *hull, const int *index) const
		{ return evalFIndex<fpreal64>(order, ebuf, hull, index); }
/// @}


#endif
