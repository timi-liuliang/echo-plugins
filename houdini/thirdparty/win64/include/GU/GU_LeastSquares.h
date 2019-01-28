/*
* PROPRIETARY INFORMATION.  This software is proprietary to
* Side Effects Software Inc., and is not to be reproduced,
* transmitted, or disclosed in any way without written permission.
*
* NAME:	GU_LeastSquares.h (GU Library, C++)
*
* COMMENTS: Solver works across rows. Each row is a polynomial equation
*	    and each column is a term in that equation.
*/

#ifndef __GU_LEASTSQUARES_H__
#define __GU_LEASTSQUARES_H__

#include "GU_API.h"
#include <SYS/SYS_Types.h>

#include <Eigen/Core>

// Returns true if matrix is symmetric within a given tolerance
bool GU_API
GUisSymmetric(
    const Eigen::Matrix<fpreal, Eigen::Dynamic, Eigen::Dynamic> &A,
    fpreal tolerance = SYS_FTOLERANCE_D);

// Solve the LS problem A = Q . B with Partial pivot LU decomposition
bool GU_API
GUleastSquaresPartialPivLU(
    const Eigen::Matrix<fpreal, Eigen::Dynamic, Eigen::Dynamic> &A,
    const Eigen::Matrix<fpreal, Eigen::Dynamic, Eigen::Dynamic> &Q,
    Eigen::Matrix<fpreal, Eigen::Dynamic, Eigen::Dynamic> &B,
    fpreal damping);

// Solve the LS problem A = Q . B with Cholesky decomposition
bool GU_API
GUleastSquaresCholesky(
    const Eigen::Matrix<fpreal, Eigen::Dynamic, Eigen::Dynamic> &A,
    const Eigen::Matrix<fpreal, Eigen::Dynamic, Eigen::Dynamic> &Q,
    Eigen::Matrix<fpreal, Eigen::Dynamic, Eigen::Dynamic> &B,
    fpreal damping);

// Solve the LS problem (Q^T . A) = (Q^T . Q) . B using
// the normal equations with Cholesky decomposition
bool GU_API
GUleastSquaresCholeskyNormalEquation(
    const Eigen::Matrix<fpreal, Eigen::Dynamic, Eigen::Dynamic> &A,
    const Eigen::Matrix<fpreal, Eigen::Dynamic, Eigen::Dynamic> &Q,
    Eigen::Matrix<fpreal, Eigen::Dynamic, Eigen::Dynamic> &B,
    fpreal damping);

// Solve the LS problem A = Q . B using singular value
// decomposition
bool GU_API
GUleastSquaresSVD(
    const Eigen::Matrix<fpreal, Eigen::Dynamic, Eigen::Dynamic> &A,
    const Eigen::Matrix<fpreal, Eigen::Dynamic, Eigen::Dynamic> &Q,
    Eigen::Matrix<fpreal, Eigen::Dynamic, Eigen::Dynamic> &B,
    int max_iterations);

#endif // __GU_LEASTSQUARES_H__
