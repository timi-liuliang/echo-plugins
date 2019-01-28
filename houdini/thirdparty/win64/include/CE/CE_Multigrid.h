/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME: CE_Multigrid.h ( CE Library, C++)
 *
 * COMMENTS: Compute Engine Multigrid solver
 */

#ifndef __CE_Multigrid__
#define __CE_Multigrid__

#include "CE_Grid.h"

#ifdef CE_ENABLED

#include <UT/UT_Vector3.h>

/// This class provides a specializaion of CE_Grid that can solve the 2D or
/// 3D Poisson equation using the multigrid algorithm, running on an OpenCL
/// device.  It's interface closely matches that of UT_MultigridArray.
class CE_API CE_Multigrid: public CE_Grid
{
public:
    CE_Multigrid();
    virtual ~CE_Multigrid();

    /// Initialize array to given size, spacing, and boundary conditions.
    void init(          const UT_Vector3I &res,
                        const UT_Vector3  &spacing,
                        const UT_Vector3I &boundariesNeg,
                        const UT_Vector3I &boundariesPos,
                        int level = 0,
                        UT_Vector3I oddCoarsenings = UT_Vector3I(0, 0, 0));

    /// Initializes this grid from the provided UT_VoxelArray, assuming the
    /// provided grid spacing.
    void initFromVoxels(const UT_VoxelArrayF &f,
                        const UT_Vector3  &spacing,
                        const UT_Vector3I &boundariesNeg,
                        const UT_Vector3I &boundariesPos);

    /// Update the grid's ghost cells with values that enforce the boundary
    /// conditions at its particular level in the multigrid hierarchy.
    void updateBoundaryConditions() const;

    /// Calculates the maximum coarse grid elements in the coarsest grid, based
    /// on the minPerAxis parameter.
    int getMaxCoarse(int minPerAxis) const;

    /// Returns a grid's level within the multigrid hierarchy.
    int getLevel() const { return myLevel; }

    /// Returns a vector containing the odd/even parity for each axis.
    UT_Vector3I getParity() const
    {
        UT_Vector3I parity;
        for (int i = 0; i < 3; i++)
            parity[i] = getRes(i) % 2;
        return parity;
    }

    /// Initializes this to be the same dimensions, boundary conditions,
    /// etc, of the given grid.
    void match(const CE_Multigrid &src);

    /// Computes rhe residual r = b - A * x, where b == *this
    void subtractApplyLaplacian(CE_Multigrid& x, CE_Multigrid &r) const;


    /// Computes the inf and 2-norm of the residual r = b - A * x,
    /// where b == *this.
    void computeResidualNorms(const CE_Multigrid &x,
                              fpreal64 &norminf, fpreal64 &norm2) const;

    /// Performs red-black Gauss-Seidel smoothing, where b
    /// == *this, and the implicit matrix is the Laplacian operator. If
    /// useSmall is true, this use callsmoothLaplacianGaussSeidelSmall() if
    /// possible.
    void smoothLaplacianGaussSeidel(CE_Multigrid &x, int iterations = 1,
                                    bool useSmall=true) const;

    /// Performs red-black Gauss-Seidel smoothing, using a kernel that only
    /// works when the entire grid can fit in one workgroup on the OpenCL
    /// device.  Returns false if the criteria to run are not met.
    bool smoothLaplacianGaussSeidelSmall(CE_Multigrid &x,
                                         int iterations = 1) const;

    /// Directly solve the Poisson equation that this grid  represents.  This
    /// function should only be called for small grids.  At the moment this
    /// always calls smoothLaplacianGaussSeidelSmall() with a large
    /// number of iterations.
    void directSolve(CE_Multigrid &x) const;

    /// Performs coarsening using full-weighting along one axis.
    /// On output @code uh.getReas(axis) == getRes(axis) / 2 @endcode
    void coarsenAlongAxis(CE_Multigrid& uh, int axis) const;

    /// Coarsen this grid into a lower resolution version of the grid.
    UT_Vector3I coarsen(int minPerAxis, CE_Multigrid &uh) const;

    /// Interpolate this array into u using inverse of full-weighting along one
    /// axis. On output
    /// @code u.getRes(axis) == getRes(axis) * 2 + parity[axis] @endcode
    void interpolateAlongAxis(CE_Multigrid& u, int axis) const;


    /// Interpolate this grid into a fine version of the grid.
    /// coarsenAxis is a vector of booleans specifying which axes to
    /// interpolate
    /// Where interpolateAxis[axis] is true, ensure that
    ///  @code u[axis] = getRes(axis) * 2 + parity[axis] @endcode
    void interpolate(const UT_Vector3I &coarsenAxis, const UT_Vector3I &parity,
                     CE_Multigrid &u) const;

    /// Perform recursive V-cycle for multigrid algorithm, until reaching
    /// numElements() < maxcoarse,
    /// at which point a direct solve is done using directSolve..  At
    /// each level, nSmoothDown and nSmoothUp iterations of Gauss-Seidel
    /// smoothing are done, unless smoothTopLevelDown is false, in which
    /// case no smoothing is done for the top-level grid on the down-cycle. This
    /// can be used as an optimization when calling vcycle iteratively; the
    /// previous vcycle call will have smoothed the top-level grid as its last
    /// step, so you can often skip smoothing that same grid on the next vcycle
    /// without loss of convergence.
    void vcycle(int minPerAxis, int nSmoothDown, int nSmoothUp,
                CE_Multigrid &x, bool smoothTopLevelDown = true) const;

    /// Performs recursive Full Multigrid to generate an initial guess for the
    /// solution of the Poisson equation represented by this array.  At
    /// each level, nSmoothDown and nSmoothUp iterations of smoothing are done.
    void fullMultigrid(int minPerAxis, int nSmoothDown,
                       int nSmoothUp, CE_Multigrid &x) const;

    /// Iteratively solve the Poisson equation, assuming *this is the right
    /// hand side. This function will do at least miniter iterations, then
    /// continue to iterate until the 2-norm of the residual has
    /// been reduced by reltol, the inf-norm of the residual is below abstol,
    /// or maxiters is reached.
    /// The optional resnorm0 and resnorm2 parameters will contain the
    /// 0- and 2-norms of the residuals for each iteration.
    int solvePoisson(fpreal64 abstol, fpreal64 reltol, int miniter, int maxiter,
                     CE_Multigrid &x,
                     UT_ValArray<fpreal64> *resnorminf,
                     UT_ValArray<fpreal64> *resnorm2,
                     bool finishBetweenIters = false) const;

protected:
    void initLaplacian();

    fpreal32            myDiag, myOmega;
    UT_Vector3          myInvDx2, mySpacing;
    UT_Vector3I         myBoundariesNeg, myBoundariesPos;
    UT_Vector3          myBoundModNeg, myBoundModPos;
    UT_Vector3          myBoundAdjModNeg, myBoundAdjModPos;
    UT_Vector3          myDiagModNeg, myDiagModPos;
    int                 myLevel;
    bool                myAllOpen, myAllClosed;
    UT_Vector3I         myOddCoarsenings;
};



#endif
#endif
