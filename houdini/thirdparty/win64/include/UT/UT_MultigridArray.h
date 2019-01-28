/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        UT_MultigridArray.h ( UT Library, C++)
 *
 * COMMENTS:
 *      This provides an array that can solve the 2D or 3D Poisson equation
 *      using the multigrid algorithm.
 *      There are only explicit template instantations for fpreal32 and
 *      fpreal64
 *
 *      The created array has elements indexed from 0, ie: [0..xdiv-1].
 */

#ifndef __UT_MultigridArray__
#define __UT_MultigridArray__

#include "UT_API.h"
#include "UT_Vector.h"
#include "UT_Vector3.h"
#include "UT_ThreadedAlgorithm.h"
#include "UT_ValArray.h"
#include "UT_VoxelArray.h"
#include "UT_Matrix.h"


enum UT_BoundaryCondition
{
    UT_DIRICHLET = 0,
    UT_NEUMANN = 1
};

///
/// UT_MultigridArrayT
/// This provides an array that can solve the 2D or 3D Poisson equation
/// using the multigrid algorithm.
/// There are currently only explicit template instantations for fpreal32 and
/// fpreal64.
///
/// The created array has elements indexed from 0, ie: [0..xdiv-1].
template<typename T>
class UT_API UT_MultigridArrayT
{
public:
    /// Default constructor, array will not be initialized after this, i.e.
    /// isInit() == false
    UT_MultigridArrayT();
    virtual ~UT_MultigridArrayT();
    
    /// Initialize array to given size and spacing
    /// NB: For the 2d case, UT_MultigridArrays MUST have the shape (nx, 1, ny).
    void init(          const UT_Vector3I &res, 
                        const UT_Vector3T<T> &spacing,
                        const UT_Vector3I &boundariesNeg,
                        const UT_Vector3I &boundariesPos,
                        int level = 0,
                        UT_Vector3I oddCoarsenings = UT_Vector3I(0,0,0));

    /// Initializes this array from the provided UT_VoxelArray, assuming the
    /// provided grid spacing. After this function returns, isInit() will
    /// return true.
    /// If this array has already been initialized, it will be resized as
    /// needed to match the UT_VoxelArray.
    /// NB: In the 2d case, UT_MultigridArrays MUST have the shape (nx, 1, ny).
    /// This function ensures that will be the case, but if a provided 2D
    /// UT_VoxelArray has a different shape, e.g. (nx, ny, 1), then the shapes
    /// of the two arrays will not match after this function, although the
    /// number of voxels will.  copyToVoxels will only resize if the number of
    /// voxels differs, so initFromVoxels and copyToVoxels with the same
    /// UT_VoxelArray will work fine in the 2D and 3D case.
    void initFromVoxels(const UT_VoxelArray<T> &f,
                        const UT_Vector3T<T> &spacing, 
                        const UT_Vector3I &boundariesNeg,
                        const UT_Vector3I &boundariesPos);
    
    /// Copy the array data to the provided UT_VoxelArray.  The UT_VoxelArray
    /// will be resized if the number of voxels doesn't match the number of
    /// elements in this array.
    void copyToVoxels(UT_VoxelArray<T> &f) const;
                        
    /// Initializes this to be the same dimensions, boundary conditions,
    /// etc, of the given array.
    /// The values of this may be reset to zero.
    void match(         const UT_MultigridArrayT &src);
    
    /// Whether this array has been initialized - default constructor does not.
    bool isInit()
    {
        return myVec.isInit();
    }

    // 3D operator access
    T operator()(int i, int j, int k) const
    {
        return myVec(i + j * myYStride + k * myZStride);
    }

    T &operator()(int i, int j, int k)
    {
        return myVec(i + j * myYStride + k * myZStride);
    };

    T operator()(const UT_Vector3I &idx) const
    {
        return myVec(idx[0] + idx[1] * myYStride + idx[2] * myZStride);
    }

    T &operator()(const UT_Vector3I &idx)
    {
        return myVec(idx[0] + idx[1] * myYStride + idx[2] * myZStride);
    };

    /// Returns the ghost value for the negative side boundaries, given the 
    /// values at idx and idxadj, which should represent the index for a
    /// border cell and the one adjacent to it along the provided axis.
    T ghostValueNeg(int axis, const UT_Vector3I &idx, 
                    const UT_Vector3I &idxadj) const
    {
        return myBoundModNeg[axis] * (*this)(idx) + 
                myBoundAdjModNeg[axis] * (*this)(idxadj);
    }

    /// Returns the ghost value for the positive side boundaries, given the 
    /// values at idx and idxadj, which should represent the index for a
    /// border cell and the one adjacent to it along the provided axis.
    T ghostValuePos(int axis, const UT_Vector3I &idx, 
                    const UT_Vector3I &idxadj) const
    {
        return myBoundModPos[axis] * (*this)(idx) + 
                myBoundAdjModPos[axis] * (*this)(idxadj);
    }

    /// Returns the ghost value for the negative side boundaries, given the 
    /// provided values U and Uadj, which should be the values at the border
    /// cell and the one adjacent to it along the provided axis.
    T ghostValueNeg(int axis, T U, T Uadj) const
    {
        return myBoundModNeg[axis] * U + myBoundAdjModNeg[axis] * Uadj;
    }
    
    /// Returns the ghost value for the positive side boundaries, given the 
    /// provided values U and Uadj, which should be the values at the border
    /// cell and the one adjacent to it along the provided axis.
    T ghostValuePos(int axis, T U, T Uadj) const
    {
        return myBoundModPos[axis] * U + myBoundAdjModPos[axis] * Uadj;
    }

    UT_MultigridArrayT<T> &operator =(const UT_MultigridArrayT &a);

    UT_MultigridArrayT<T> &operator +=(const UT_MultigridArrayT &a);

    void zero()
    {
        myVec.zero();
    }

    T norm(int n) const
    {
        return myVec.norm(n);
    }

    exint getRes(int axis) const
    {
        return myRes[axis];
    }

    UT_Vector3I getRes() const 
    {
        return myRes;
    }
    
    const UT_Vector3T<T> &getSpacing() const 
    {
        return mySpacing;
    }
    
    const UT_Vector3I &getBoundariesNeg() const
    {
        return myBoundariesNeg;
    }
    
    const UT_Vector3I &getBoundariesPos() const
    {
        return myBoundariesPos;
    }
    
    int getLevel() const
    {
        return myLevel;
    }
    
    UT_Vector3I getParity() const
    {
        UT_Vector3I parity;
        for (int i=0; i < 3; i++)
            parity[i] = getRes(i) % 2;
        return parity;
    }
    
    /// Returns the total number of elements in the array.
    exint numElements()  const 
    {
        return myRes[0] * myRes[1] * myRes[2];
    }
    
    /// Calculates the maximum coarse grid elements in the coarsest grid, based
    /// on the minPerAxis parameter.
    exint getMaxCoarse(exint minPerAxis) const;

    /// Returns whether this is a 1-D array.
    bool is1D() const
    {
	int ndim = 0;
	for (int i=0; i < 3; i++)
	    if (getRes(i) > 1)
		ndim++;
	return ndim < 2;
    }

    bool shouldMultiThread() const 
    {
        return numElements() > 1000;
    }

    const T *data() const 
    {
        return myVec.getData();
    }
    
    T *data() 
    {
        return myVec.getData();
    }

    /// Computes the 0 and 2-norm of this in one pass
    void computeNorms(fpreal64 &norminf, fpreal64 &norm2) const;

    /// Computes the inf and 2-norm of the residual r = b - A * x,
    /// where b == *this
    void computeResidualNorms(const UT_MultigridArrayT &x, 
                               fpreal64 &norminf, fpreal64 &norm2) const;

    /// Computes rhe residual r = b - A * x, where b == *this
    THREADED_METHOD2_CONST(UT_MultigridArrayT, shouldMultiThread(),
                           subtractApplyLaplacian,
                           const UT_MultigridArrayT&, x,
                           UT_MultigridArrayT&, r)
    void subtractApplyLaplacianPartial(const UT_MultigridArrayT& x, 
				       UT_MultigridArrayT &r,
                                       const UT_JobInfo& info) const;
                                       
    /// Performs red-black Gauss-Seidel smoothing according to parity, where b
    /// == *this, and the implicit matrix is the Laplacian operator.
    /// This should be called twice, with parity=0 and 1, for a full smoothing
    /// cycle.
    THREADED_METHOD2_CONST(UT_MultigridArrayT, shouldMultiThread(),
                            smoothLaplacianGaussSeidel,
                            int, parity,
                            UT_MultigridArrayT&, x)                     
    void smoothLaplacianGaussSeidelPartial(int parity, UT_MultigridArrayT& x, 
                                           const UT_JobInfo& info) const;

    /// Performs coarsening using full-weighting along one axis.
    /// On output uh.shape[axis] == this->shape[axis] / 2
    THREADED_METHOD2_CONST(UT_MultigridArrayT, shouldMultiThread(),
                           coarsenAlongAxis,
                           UT_MultigridArrayT&, uh,
                           int, axis)
    void coarsenAlongAxisPartial(UT_MultigridArrayT& uh, int axis,
                                 const UT_JobInfo& info) const;

    /// Interpolate this array into u using inverse of full-weighting along one
    /// axis.
    /// On output u.shape[axis] == this->shape[axis] * 2 + parity.
    THREADED_METHOD2_CONST(UT_MultigridArrayT, shouldMultiThread(),
                           interpolateAlongAxis,
                           UT_MultigridArrayT&, u,
                           int, axis)
    void interpolateAlongAxisPartial(UT_MultigridArrayT& u, int axis,
                                     const UT_JobInfo& info) const;


    /// Build the dense matrix in A that corresponds to this grid's Laplacian
    /// operator.
    template <typename S>
    void buildMatrix(UT_MatrixT<S> &A) const;

    /// Directly solve the Poisson equation that this array represents, using
    /// either Cholesky, LU, or SVD decomposition, depending on the nature of
    /// the Poisson problem.  Should only be called for very coarse grids.
    void directSolve(UT_MultigridArrayT &x) const;

    /// Coarsen this array into a lower resolution version of the grid.
    /// coarsenAxis is a vector of booleans specifying which axes to coarsen.
    /// Where coarsenAxis[axis] is true, ensure that uh[axis] = getRes(axis) / 2
    UT_Vector3I coarsen(exint minPerAxis, UT_MultigridArrayT &uh) const;

    /// Interpolate this array into a fine version of the grid.
    /// interpolateAxis is a vector of booleans specifying which axes to
    /// interpolate
    /// Where interpolateAxis[axis] is true, ensure that 
    ///  u[axis] = getRes(axis) * 2 + parity[axis].
    void interpolate(const UT_Vector3I &coarsenAxis, const UT_Vector3I &parity, 
                        UT_MultigridArrayT &u) const;

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
    void vcycle(exint minPerAxis, int nSmoothDown, int nSmoothUp,
                UT_MultigridArrayT &x, bool smoothTopLevelDown = true) const;

    /// Performs recursive Full Multigrid to generate an initial guess for the
    /// solution of the Poisson equation represented by this array.  At
    /// each level, nSmoothDown and nSmoothUp iterations of smoothing are done.  
    void fullMultigrid(exint minPerAxis, int nSmoothDown, 
                       int nSmoothUp, UT_MultigridArrayT &x) const;
        
    /// Iteratively solve the Poisson equation, assuming *this is the right
    /// hand side. This function will do at least miniter iterations, then 
    /// continue to iterate until the 2-norm of the residual has
    /// been reduced by reltol, the inf-norm of the residual is below abstol, 
    /// or maxiters is reached.
    /// The optional resnorm0 and resnorm2 parameters will contain the 
    /// 0- and 2-norms of the residuals for each iteration.
    int solvePoisson(fpreal64 abstol, fpreal64 reltol, int miniter, int maxiter, 
                        UT_MultigridArrayT &x, 
                        UT_ValArray<fpreal64> *resnorminf,
                        UT_ValArray<fpreal64> *resnorm2) const;

protected:
    /// Directly solves the Poisson equation for this right-hand side, storing
    /// the result in x, using UT_MatrixSolver::choleskyDecomp and
    /// UT_MatrixSolver::choleskyBackSub.
    /// This should only be called for relatively small N when the coarsest
    /// grid level has been reached, and only when the result of buildMatrix
    /// is positive (semi-definite)
    bool solvePoissonCholesky(UT_MultigridArrayT &x) const;

    /// Directly solves the Poisson equation for this right-hand side, using
    /// LU decomposition.
    /// This should only be called for relatively small N when the coarsest
    /// grid level has been reached, and only when the result of buildMatrix
    /// is non-singular.
    bool solvePoissonLU(UT_MultigridArrayT &x) const;
    
    /// Directly solves the Poisson equation for this right-hand side, using
    /// SVD decomposition.  This can be used when the result of buildMatrix is
    /// singular. This should only be called for relatively small N when the
    /// coarsest grid level has been reached.
    bool solvePoissonSVD(UT_MultigridArrayT &x) const;
    
    void initStorage();
    void initLaplacian();

    THREADED_METHOD2_CONST(UT_MultigridArrayT, shouldMultiThread(),
                           computeNormsInternal,
                           fpreal64*, norminf,
                           fpreal64*, norm2)
    void computeNormsInternalPartial(fpreal64 *norminf, fpreal64 *norm2,
                                       const UT_JobInfo& info) const;

    THREADED_METHOD3_CONST(UT_MultigridArrayT, shouldMultiThread(),
                           computeResidualNormsInternal,
                           const UT_MultigridArrayT&, x,
                           fpreal64*, norminf,
                           fpreal64*, norm2)
    void computeResidualNormsInternalPartial(const UT_MultigridArrayT& x, 
                                     fpreal64 *norminf, fpreal64 *norm2,
                                       const UT_JobInfo& info) const;

    /// Since the multi-threading threshold is 1000,
    /// pick a block size less than that.
    static const exint PARALLEL_BLOCK_SIZE_VEC = 512;

    /// This is for parallelism over just z, so a z section of size just 8
    /// can be huge if x and y are large.
    static const exint PARALLEL_BLOCK_SIZE_Z = 8;

    /// UT_VectorT that holds the array data.
    UT_VectorT<T> 	        myVec;
    
    /// Number of elements in each dimension.
    UT_Vector3I 	        myRes;
    
    /// Grid spacing in each dimension.
    UT_Vector3T<T> 	        mySpacing;
    
    /// Array stride in y and z axes.
    exint 		            myYStride, myZStride;
    
    /// Laplacian operator values calculated based on grid spacing.
    T 		    	        myInvdx2, myInvdy2, myInvdz2, myDiag, myOmega;
    UT_Vector3I             myBoundariesNeg, myBoundariesPos;
    UT_Vector3T<T>          myBoundModNeg, myBoundModPos;
    UT_Vector3T<T>          myBoundAdjModNeg, myBoundAdjModPos;
    UT_Vector3T<T>          myDiagModNeg, myDiagModPos;
    int                     myLevel;
    bool                    myAllOpen, myAllClosed;
    UT_Vector3I             myOddCoarsenings;
};

typedef UT_MultigridArrayT<fpreal> 	UT_MultigridArrayR;
typedef UT_MultigridArrayT<fpreal32> 	UT_MultigridArrayF;
typedef UT_MultigridArrayT<fpreal64> 	UT_MultigridArrayD;

#endif
