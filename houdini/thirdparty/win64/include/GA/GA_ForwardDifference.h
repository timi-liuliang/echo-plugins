/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_ForwardDifference.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_ForwardDifference__
#define __GA_ForwardDifference__

#include "GA_API.h"
#include "GA_Defines.h"
#include "GA_ForwardDifferenceMatrix.h"
#include "GA_GenericHandle.h"
#include "GA_Types.h"

#include <SYS/SYS_Types.h>


class GA_AttributeRefMap;


/// @brief Polynomial forward differencer
///
/// This class performs forward differencing of polynomial functions by forward
/// differencing.  To use this class, you need to specify
///  - The order of the polynomial
///  - The N derivatives of the polynomial (one for each degree)
/// The forward differencer can then evaluate the polynomial very efficiently.
template <typename T>
class GA_API GA_ForwardDifference 
{
public:
    /// Perform forward differencing
     GA_ForwardDifference(const GA_ROGenericHandleVertex<T> &gah);
     GA_ForwardDifference(const GA_ROGenericHandleVertex<T> &gah,
	     int uord, fpreal uinc, int vord=1, fpreal vinc=1);
    ~GA_ForwardDifference();

    /// Initialize the forward differencer with 
    /// @param uord Order of the spline in U
    /// @param uinc Stepping increment for integration in U
    /// @param vord Order of the spline in V.  For 1D curves, this is ignored.
    /// @param vinc Stepping increment for integration in V
    void	initialize(const GA_ROGenericHandleVertex<T> &gah,
	    int uord, fpreal uinc, int vord=1, fpreal vinc=1);

    ///@{
    /// Return the initialized U order
    int		getUOrder() const	{ return myMatrix.getUOrder(); }
    /// Return the initialized U integration step size
    fpreal	getUStep() const	{ return myMatrix.getUStep(); }

    /// Return the initialized V order
    int		getVOrder() const	{ return myMatrix.getVOrder(); }
    /// Return the initialized V integration step size
    fpreal	getVStep() const	{ return myMatrix.getVStep(); }

    /// Return the offset for the derivative
    T		&getDerivative(int u, int v=0)	{ return myDeriv[u][v]; }
    ///@}

    /// 1D forward difference a hull into a curve
    ///	@param gah is the set of attributes to evaluate. 
    ///	@param ucount is the number of vertices to fill
    ///	@param uoff Offset into the results.  That is, the results will be
    ///		stored in @code results[uoff]-results[uoff+ucount-1] @endcode
    ///	@param deriv Nth Derivatives.  When the hull and the results 
    ///		exist in different details, the derivatives are represented by
    ///		the RHS in the gah (source mapping).
    ///	@param results List of vertex offsets where the result should be
    ///		stored.  When the hull and results are in different details,
    ///		the results are the LHS of the gah (destination mapping).
    void	walk(const GA_ROGenericHandleVertex<T> &gah,
			int ucount, int uoff,
			T *results);

    /// 2D forward difference a hull into a mesh
    ///	@param gah is the set of attributes to evaluate. 
    ///	@param ucount is the number of vertices to fill in U (columns)
    ///	@param vcount is the number of vertices to fill in V (rows)
    ///	@param uoff Offset into the results (stride).
    ///	@param voff Offset into the results (stride).
    /// @param ncols Number of columns in the full matrix of result vertices.
    ///		This is used to stride into the offsets/rows.
    ///	@param deriv Nth Derivatives.  When the hull and the results 
    ///		exist in different details, the derivatives are represented by
    ///		the RHS in the gah (source mapping).
    ///	@param results List of vertex offsets where the result should be
    ///		stored.  When the hull and results are in different details,
    ///		the results are the LHS of the gah (destination mapping).
    void	walk(const GA_ROGenericHandleVertex<T> &gah,
			int ucount, int vcount,
			int uoff, int voff, int ncols,
			T *results);

private:
    GA_GenericTempVertex<T>	myInit;
    GA_ForwardDifferenceMatrix	myMatrix;
    T				myDeriv[GA_MAXORDER][GA_MAXORDER];
};

#endif

