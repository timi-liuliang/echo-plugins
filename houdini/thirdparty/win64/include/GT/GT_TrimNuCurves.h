/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_TrimNuCurves.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_TrimNuCurves__
#define __GT_TrimNuCurves__

#include "GT_API.h"
#include "GT_CountArray.h"
#include "GT_DataArray.h"
#include "GT_Handles.h"

class UT_JSONWriter;

/// @brief Trim loops specified by a number of individual NURBS curves
class GT_API GT_TrimNuCurves
{
public:
    /// Default constructor
    GT_TrimNuCurves();
    /// Copy constructor
    GT_TrimNuCurves(const GT_TrimNuCurves &src);
    /// The @c loop_counts array is a list of the number of curves per loop.
    /// For example, a trim loop may be made of multiple curves.
    ///
    /// The @c curve_counts array is a list of the number of vertices in each
    /// curve.  The array should be sum(curve_counts) entries long (one entry
    /// for each individual curve).
    ///
    /// The @c curve_orders array is a list of the order of the NURBS curves
    /// for each curve.  The array should be sum(curve_counts) entries long
    /// (one entry for each individual curve).
    ///
    /// The @c knots array is a single array storing the knots of the curves
    /// concatenated together.  The array should be <tt>sum(curve_orders) +
    /// sum(vertex_counts)</tt> entries long.
    ///
    /// The @c min and @c max arrays represent the basis extents for each
    /// curve.  The arrays should be sum(curve_counts) entries long (one for
    /// each individual curve).
    ///
    /// The @c uv array is the list of point positions, either 2D
    /// (non-rational) or 3D (rational).  There should be @c sum(vertex_counts)
    /// uv positions.
    GT_TrimNuCurves(const GT_DataArrayHandle &loop_counts,
		    const GT_DataArrayHandle &curve_counts,
		    const GT_DataArrayHandle &curve_orders,
		    const GT_DataArrayHandle &knots,
		    const GT_DataArrayHandle &min,
		    const GT_DataArrayHandle &max,
		    const GT_DataArrayHandle &uv);
    ~GT_TrimNuCurves();

    bool	isValid() const	{ return myCurves.entries(); }

    /// Return the total number of segments
    GT_Size		getCurveCount() const	{ return myCurves.entries(); }
    GT_Size		getCurveCount(int loop) const
			    { return myLoops.getCount(loop); }
    GT_Size		getCurveOffset(int loop) const
			    { return myLoops.getOffset(loop); }

    /// Return the number of cv's
    GT_Size		getVertexCount() const	{ return myUV->entries(); }

    /// Return the number of vertices for the Nth curve
    GT_Size		getVertexCount(GT_Offset curve) const
			    { return myCurves.getCount(curve); }
    /// Get the order for the given curve
    int			getOrder(GT_Offset curve) const
			    { return myOrders->getI32(curve); }

    /// @{
    /// Accessors
    const GT_CountArray		&getLoopCountArray() const { return myLoops; }
    const GT_CountArray		&getCurveCountArray() const { return myCurves; }
    const GT_DataArrayHandle	&getOrders() const	{ return myOrders; }
    const GT_DataArrayHandle	&getKnots() const	{ return myKnots; }
    const GT_DataArrayHandle	&getMin() const		{ return myMin; }
    const GT_DataArrayHandle	&getMax() const		{ return myMax; }
    const GT_DataArrayHandle	&getUV() const		{ return myUV; }
    /// @}

    /// Save the primitive to a JSON stream.  The default method does nothing
    /// and returns false.
    bool	save(UT_JSONWriter &w) const;
    void	dump() const;

    /// Harden all arrays
    void	harden();

private:
    void	validate();
    void	makeInvalid();

    GT_CountArray	myLoops;
    GT_CountArray	myCurves;
    GT_DataArrayHandle	myOrders;
    GT_DataArrayHandle	myKnots;
    GT_DataArrayHandle	myMin;
    GT_DataArrayHandle	myMax;
    GT_DataArrayHandle	myUV;
};

#endif
