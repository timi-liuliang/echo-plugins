/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME: CE_SparseMatrix.h ( CE Library, C++)
 *
 * COMMENTS: Compute Engine Sparse Matrix.
 */

#ifndef __CE_SparseMatrix__
#define __CE_SparseMatrix__

#include "CE_API.h"
#include "CE_Vector.h"
#include "CE_Context.h"

#include <UT/UT_SparseMatrix.h>

#include <type_traits>

#ifdef CE_ENABLED

/// An ELL-format sparse matrix for OpenCL.  This class is fairly limited;
/// its primary purpose is to initialize with an alread-populated
/// UT_SparseMatrixELL, then call its solveConjugateGradient function to
/// solve a linear system using OpenCL.
template <typename T, bool useexint = false>
class CE_API CE_SparseMatrixELLT
{
    typedef typename std::conditional<useexint, exint, int>::type inttype;

public:

    typedef T	 value_type;

    CE_SparseMatrixELLT();

    virtual 	~CE_SparseMatrixELLT();

    /// Initialize to the given number of rows and non-zeros per row.
    void	init(inttype rows, int nzperrow, bool colmajor = true);

    /// Initialize from the provided UT_SparseMatrixELL.
    template <bool colmajor>
    void	initFromMatrix(const UT_SparseMatrixELLT<T, colmajor, useexint> &src,
			       bool block = true);

    /// Copy back to the provided UT_SparseMatrixELL.  Only callable with
    /// column-major destinations.
    void	matchAndCopyToMatrix(UT_SparseMatrixELLT<T, true, useexint> &dst,
				     bool block = true) const;

    /// Returns the number of rows.
    inttype 	getNumRows() const { return myRows; }

    /// Returns the number of non-zeros per row.
    int 	getNonZerosPerRow() const { return myNonZeros; }

    /// Sparse-matrix vector multiply.
    void 	multVec(const CE_VectorT<T> &v, CE_VectorT<T> &result) const;

    /// Get the inverse of the diagonal.
    void 	getDiagonalInv(CE_VectorT<T> &result) const;

    /// Solve Ax = b using conjugate gradient with Jacobi pre-conditioning,
    float	solveConjugateGradient(CE_VectorT<T> &x, const CE_VectorT<T> &b,
				       T tol2=1e-5, int max_iters = -1,
				       int *iterout = NULL) const;

protected:
    void allocBuffers() const;
    void releaseBuffers();
    cl::Kernel loadKernel(const char *kernelName, const char *opt = NULL) const;
    cl::KernelFunctor bind(const char *kernelName) const;
    mutable cl::Buffer  myRowValsBuffer, myColumnsBuffer;
    inttype		myRows;
    int			myNonZeros;
    bool		myColMajor;
    cl::NDRange		myGlobalRange, myLocalRange;
};

template <typename T, bool useexint>
template <bool colmajor>
void
CE_SparseMatrixELLT<T, useexint>::initFromMatrix(const UT_SparseMatrixELLT<T, colmajor, useexint> &src,
				       bool blocking)
{
    init(src.getNumRows(), src.getNonZerosPerRow(), colmajor);
    size_t totalsize = myRows * (inttype)myNonZeros;
    if (!totalsize)
	return;
    const UT_ValArray<T> &srcValues = src.getRowValues();
    const T *rowdata = &srcValues(0);
    const UT_ValArray<inttype> &cols = src.getColumns();
    const inttype *coldata = &cols(0);
    cl::CommandQueue queue = CE_Context::getContext()->getQueue();
    queue.enqueueWriteBuffer(myRowValsBuffer, CL_FALSE,
			     0, totalsize * sizeof(T), rowdata);
    queue.enqueueWriteBuffer(myColumnsBuffer, blocking,
			     0, totalsize * sizeof(inttype), coldata);
}


typedef CE_SparseMatrixELLT<fpreal32>	CE_SparseMatrixELLF;
typedef CE_SparseMatrixELLT<fpreal64>	CE_SparseMatrixELLD;

#else
template <typename T>
class CE_API CE_SparseMatrixELLT
{
};

#endif
#endif
