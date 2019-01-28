/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_ForwardDifferenceMatrix.h (GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_ForwardDifferenceMatrix__
#define __GA_ForwardDifferenceMatrix__

#include "GA_API.h"
#include "GA_Defines.h"

#include <SYS/SYS_Types.h>


/// @brief Helper class for a polynomial forward differencer
///
/// This class creates matrices used for forward differencing of polynomial
/// functions. To use this class, you need to specify the order of the
/// polynomial.
class GA_API GA_ForwardDifferenceMatrix
{
public:
    /// Perform forward differencing
     GA_ForwardDifferenceMatrix();

    /// Initialize the forward differencer matrix with 
    /// @param uord Order of the spline in U
    /// @param uinc Stepping increment for integration in U
    /// @param vord Order of the spline in V.  For 1D curves, this is ignored.
    /// @param vinc Stepping increment for integration in V
    void	initialize(int uord, fpreal uinc,
			int vord=1, fpreal vinc=1);

    ///@{
    /// Return the specified U matrix element
    fpreal	getU(int row, int col) const { return myU[row][col]; }
    /// Return the initialized U order
    int		getUOrder() const	{ return myUOrder; }
    /// Return the initialized U integration step size
    fpreal	getUStep() const	{ return myUStep; }

    /// Return the specified V matrix element
    fpreal	getV(int row, int col) const { return myV[row][col]; }
    /// Return the initialized V order
    int		getVOrder() const	{ return myVOrder; }
    /// Return the initialized V integration step size
    fpreal	getVStep() const	{ return myVStep; }
    ///@}

private:
    fpreal		myU[GA_MAXORDER][GA_MAXORDER];
    fpreal		myV[GA_MAXORDER][GA_MAXORDER];
    fpreal		myUStep, myVStep;
    int			myUOrder, myVOrder;
};

#endif

