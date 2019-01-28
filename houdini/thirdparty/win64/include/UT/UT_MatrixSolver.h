/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Matrix Solver (C++)
 *
 * COMMENTS:
 *
 */

#ifndef	__UT_MatrixSolver_H__
#define	__UT_MatrixSolver_H__

#include "UT_API.h"
#include "UT_Assert.h"
#include "UT_Functor.h"
#include "UT_Matrix.h"
#include "UT_Vector.h"
#include "UT_SparseMatrix.h"
#include "UT_Interrupt.h"
#include <SYS/SYS_Types.h>

// Set below to 1 in order to use old version of LUDecomp
#define USE_OLD_LUDECOMP 0

template <typename T>
class UT_API UT_MatrixSolverT
{
public:
    /// LU Decomposition of a[1..n][1..n] where a has full rank. 
    /// Output: index[1..n] is the row permutation 
    ///         d = +/-1 indicating row interchanges was even or odd, resp.
    ///         return 0 if the matrix is truly singular 
    ///		      1 on success
    ///		      2 if the matrix is singular to the precision of the
    ///			algorithm, where tol is the zero tolerance.
    int 	LUDecompOld(UT_MatrixT<T> &a, UT_Permutation &index, T &d, 
			    T tol = 1e-20);

    /// Solve Ax=b
    /// a[1..n][1..n] is a LU Decomposition of matrix A (eg. from LUDecomp())
    /// index describes the row permutations as output from LUDecomp()
    /// b[1..n] is input as rhs, and on output contains the solution x
    template <typename S>
    void	LUBackSubOld(const UT_MatrixT<T> &a,
			     const UT_Permutation &index, 
			     UT_VectorT<S> &b);

#if USE_OLD_LUDECOMP
    int 	LUDecomp(UT_MatrixT<T> &a, UT_Permutation &index, T &d, 
			 T tol = 1e-20)
    {
	return LUDecompOld(a, index, d, tol);
    }
    template <typename S>
    void	LUBackSub(const UT_MatrixT<T> &a,
			  const UT_Permutation &index, 
			  UT_VectorT<S> &b)
    {
	return LUBackSubOld(a, index, b);
    }
#else
    int 	LUDecomp(UT_MatrixT<T> &a, UT_Permutation &index, T &d, 
			 T tol = 1e-20)
    {
	return LUDecompRect(a, index, d, tol);
    }
    template <typename S>
    void	LUBackSub(const UT_MatrixT<T> &a, const UT_Permutation &index, 
			  UT_VectorT<S> &b)
    {
	return LUDecompBackSub(a, index, b);
    }
#endif

    /// LU decomposition of rectangular A[1..m][1..n] with partial pivoting.
    /// Unlike LUDecomp(), this handles pivot values of 0.
    /// ie. it performs the decomposition of A = P.L.U for unit lower
    ///     triangular L and upper triangular U.
    /// Input:  A[1..m][1..n]
    /// Output: A contains the matrices L and U in compact form, without
    ///           the unit diagonal.
    ///           L is of size min(m,n) by n. U is of size m by min(m,n)
    ///         pivots[1..n] is the row permulation P
    ///         tol is scaled by the largest pivot element
    ///         detsign is the sign of the determinant used by det()
    ///            its +1 if row interchanges was even, -1 if odd
    ///         Returns 0 if the matrix is truly singular 
    ///		        1 on success
    ///		        2 if the matrix is singular to the precision of the
    ///			  algorithm, where tol is the zero tolerance.
    /// This algorithm takes roughly O(m*n^2 - n^3/3) flops
    int		LUDecompRect(UT_MatrixT<T> &A, UT_Permutation &pivots,
			     T &detsign,
			     T tol = 1e-6);

    /// Performs back substitution using the output of LUDecomp() for a square
    /// matrix.
    /// Input:  A[1..n][1..n] and pivots as output from LUDecompRect()
    ///         b[1..n] is the right hand side for solving A.x = b
    /// Output: b is modified to contain the solution x
    template <typename S>
    void	LUDecompBackSub(const UT_MatrixT<T> &A,
				const UT_Permutation &pivots,
				UT_VectorT<S> &b);

    /// LU decompose a companion matrix
    /// The matrix given is the base of the companion matrix:
    /// [aI bI 0  0 ]
    /// [0  aI bI 0 ]
    /// [0  0  aI bI]
    /// [P1 P2 P3 P4]
    /// ie: [P1 P2 P3 P4]
    /// The resulting LU decompostion is written into the R matrix and
    /// is:
    /// [aI 0   0 0] [I b/aI  0    0  ]
    /// [0  aI  0 0] [0  I   b/aI  0  ]
    /// [0  0  aI 0] [0  0    I   b/aI]
    /// [R1 R2 R3 L] [0  0    0    U  ]
    /// LU is  stored in the last portion of the matrix & index is the
    /// permuation that resulted in it.  Thus, index should be the height
    /// of P, NOT the entire companion matrix.
    ///
    /// Return 0 on success, -1 for singular, and -2 for singular to
    /// machine precision (As judged by tol), and -3 for a being near zero.
    int		LUDecompCompanion(const UT_MatrixT<T> &P, T a, T b,
				  UT_MatrixT<T> &R, UT_Permutation &index,
				  T tol=1e-20);

    /// This takes the result of the previous operation and does a 
    /// back substitution.
    template <typename S>
    void	LUBackSubCompanion(const UT_MatrixT<T> &R, T a, T b, 
				   const UT_Permutation &index,
				   UT_VectorT<S> &y);

    ///
    /// Cholesky factorization
    ///
    /// To solve a full rank system A . x = b where A can be undertermined
    /// (ie. A has less rows than columns), we can make use of the Cholesky
    /// factorization functions below by way of the normal equations like so:
    /// 1. Left multiply both sides by the tranpose of A, A^T to get
    ///         (A^T . A) . x = (A^T . b)
    /// 2. Now solve for x using P . x = c where P = A^T . A and c = A^T . b
    ///    This is done by first calling choleskyDecomp() using P since it
    ///    will be symmetric non-negative definite. Then use choleskyBackSub()
    ///    to obtain x for particular values of c.

    /// Performs a Cholesky decomposition of the symmetric non-negative definite
    /// matrix A such that A = L . L^T where L^T is tranpose of the lower
    /// triangular matrix L.
    /// Input:  Symmetric non-negative definite matrix a[1..n][1..n].
    ///         Only the upper triangle of a is read from.
    ///         tol is the tolerance check for when a sum is deemed 0
    /// Output: Cholesky factor L is returned in the lower triangle of A, except
    ///         for its diagonal elements which is returned in d[1..n]
    ///         Returns the number of articial zero's placed into d. If we
    ///         encounter negative diagonal values, they are zeroed out and we
    ///         return -N where N is the number of negative values found.
    template <typename S>
    int		choleskyDecomp(UT_MatrixT<T> &a, UT_VectorT<S> &d, T tol=1e-5);

    /// Solves the set of n linear equations, A.x = b, where a is a nonnegative
    /// definite symmetric matrix.
    /// Input:  a[1..n][1..n] and d[1..n] as output from choleskyDecomp() above.
    ///         Only the lower triangle of a is read from.
    ///	       b[1..n] is the right-hand side of A.x = b to solve for
    ///         tol is the tolerance check for when an element of d is deemed 0
    /// Output: x[1..n], the solution of A.x = b
    ///         Returns the number of articial zero's placed into x.
    template <typename S>
    int		choleskyBackSub(const UT_MatrixT<T> &a, const UT_VectorT<S> &d,
				const UT_VectorT<S> &b, UT_VectorT<S> &x,
				T tol=1e-5);
    
    /// Performs inverse iteration to find the closest eigenvalue(s)
    /// to s.
    /// Does a two pass method, first using single iterations until
    /// starttol is reached, in which case singles are used to
    /// achieve final tolerance.  If starttol is not reached in
    /// startiter iterations, uses double method to find using
    /// final tolerance/iterations.
    ///
    /// The vector q is updated with the newest eigenvector.
    /// Source matrices are:
    ///        [1  0  0]        [0 1 0]
    ///  C1' = [0  1  0]  C2' = [0 0 1]
    ///        [0  0 C1]        [ C2  ]
    /// Thus, the initial sizes of the matrices/vectors are (row*col):
    /// C1: n*n, C2: n*(n*deg), R: n*(n*deg), tmp: n*(n*deg),
    /// index: n, q: n*deg, Aq: n*deg
    /// The result code determines how s1 & s2 are to be interpreted:
    /// Result:
    ///  0 - One eigenvalue found at s1, eigenvector q.
    ///  1 - Two real eigenvalues found: s1 & s2.
    ///  2 - Complex conjugate eigenvalues found: s1 +- s2i.
    /// -1 - Failed to converge to final tolerance using single method.
    /// -2 - Failed to converge to final tolerance using double method.
    template <typename S>
    int		inversePowerIterate(const UT_MatrixT<T> &C1, 
				const UT_MatrixT<T> &C2, 
				UT_MatrixT<T> &R, UT_MatrixT<T> &tmp, 
				UT_Permutation &index, 
				T s, 
				T &s1, T &s2,
				UT_VectorT<S> &q, UT_VectorT<S> &Aq,
				T starttol = 1e-2, int startiter = 5,
				T finaltol = 1e-5, 
				T doubletol = 1e-4, int finaliter = 50);

    /// Compute the inverse of a LU decomposed matrix.
    /// If you want to solve systems of equations AX=B,
    /// where B is a matrix. It is better (faster) to do a 
    /// LU decomposition and use back substituation for columns of B.
    void	inverse(UT_MatrixT<T> &a, UT_Permutation &index,
			UT_MatrixT<T> &ia);

    /// Finds all linearly independent rows in A, does NOT require
    /// A to be any particular shape.  Returns the number of independent
    /// rows & first entries of index are their indices in the original
    /// matrix.
    /// Destroys A.
    int		findLinIndRows(UT_MatrixT<T> &A, UT_Permutation &index, 
			       T tol = 1E-6);

    /// Similar to findLinIndRows, performs gaussian elimination
    /// with full pivoting on G.  Updates as well Gu, and Gv, the
    /// partial derivitive matrix of G(u, v).  After this is done,
    /// the determinant of G & partials of the determinant can
    /// be computed.
    void	fullGaussianElimination(UT_MatrixT<T> &G, UT_MatrixT<T> &Gu,
					UT_MatrixT<T> &Gv,
					UT_Permutation &rowperm,
					UT_Permutation &colperm,
					T &detsign);

    /// After full Gaussian Elimination, this finds the determinant
    /// and partials thereof:
    void	detWithPartials(const UT_MatrixT<T> &G, const UT_MatrixT<T> &Gu,
				const UT_MatrixT<T> &Gv, 
				const UT_Permutation &rowperm,
				const UT_Permutation &colperm,
				T detsign,
				T &retdet, T &detu, T &detv);


    /// Compute the determinant of a LU decomposed matrix.
    /// The d is what one gets from LU decomposition.
    T	det(UT_MatrixT<T> &a, T d);

    /// Find the determiment:
    /// (A(p,q), B(r,s), C(t,u)))
    inline T	det3x3(const UT_MatrixT<T> &A, const UT_MatrixT<T> &B, 
		    const UT_MatrixT<T> &C, int p, int q, int r, int s,
		    int t, int u)
	    {
		return (A(p,q) * (B(r,s) * C(t,u) - B(t,u) * C(r,s)) -
			B(p,q) * (A(r,s) * C(t,u) - A(t,u) * C(r,s)) +
			C(p,q) * (A(r,s) * B(t,u) - A(t,u) * B(r,s)) );
	    }

    /// Estimates condtion number of upper triangular matrix:
    template <typename S>
    void	condEstimate(const UT_MatrixT<T> &A, UT_VectorT<S> &y);

    /// Finds the condition number of a LUDecomposed matrix.
    /// This is just an approximation
    /// You must provide the infinite norm of A before LUDecomposition.
    T	getConditionLUD(const UT_MatrixT<T> &A, 
				const UT_Permutation &index, T anorm);

    /// Find householder vector of x.
    template <typename S>
    void	house(const UT_VectorT<S> &x, UT_VectorT<S> &v, T &b);
    template <typename S>
    void	house(const UT_MatrixT<T> &x, UT_VectorT<S> &v, T &b);

    /// Finds the givens rotation required to 0 b:
    void	findGivens(T a, T b, T &c, T &s,
			   T zerotol = 1e-20);

    /// Find half givens, return 0 if success, otherwise the required
    /// rotation is impossible (ie:complex eigenvalue)
    int		findHalfGivens(const UT_MatrixT<T> &A, T &c, T &s,
			       T tol = 0.0);

    /// Upper triangulizes A with householder transforms, applying
    /// simultaneously to A and optionally Q.
    void	triangularize(UT_MatrixT<T> &A, UT_MatrixT<T> &B,
			      UT_MatrixT<T> *Q = 0);

    /// Transforms A into upper hessenburg and B into upper triangular.
    void	hessentri(UT_MatrixT<T> &A, UT_MatrixT<T> &B);

    /// Converts A into quasi upper triangular & B into upper triangular
    /// form using the QZ algorithm.
    int 	realSchurFormQZ(UT_MatrixT<T> &oA, UT_MatrixT<T> &oB,
				T tol=1e-6,
				int maxIteration = 30);

    /// Transform A into a hessenberg matrix through a series of 
    /// householder reflections.
    void	hessenberg(UT_MatrixT<T> &A);
    /// Computes P st A' = P^T A P
    void	hessenberg(UT_MatrixT<T> &A, UT_MatrixT<T> &P);

    /// Transforms A into a bidiagonal matrix via householder transformations.
    int 	bidiagonalize(UT_MatrixT<T> &A);

    /// Reduces the matrix to real Schur form:
    /// Orthogonal transform responible for getting to real schur form
    /// is accumulated in Q, if it is non-zero.
    int 	realSchurForm(UT_MatrixT<T> &A, UT_MatrixT<T> *Q = 0, 
			      T tol = 1E-6,
			      int maxIteration = 30);

    /// Golub-Kahan SVD algorithm.  Resulting SVD matrix found in
    /// diaganol of A.
    int		gkSVD(UT_MatrixT<T> &A,
		      T tol = 1e-6,
		      int maxIteration = 30);

    /// Use Numerical Recipes method for SVD decomposition, with results
    /// that are suitable for doing back substitution.
    /// NOTE: The UT_MatrixF should start with 1,1!
    ///
    /// Decomposition a = u * w * v^T
    ///   a[1..m][1..n] also stores the result u[1..m][1..n]
    ///   w[1..n][1..n] is a diagonal matrix, stored as a vector
    ///   v[1..n][1..n]
    template <typename S>
    int		SVDDecomp(UT_MatrixT<T> &a, UT_VectorT<S> &w, UT_MatrixT<T> &v,
			  int maxIterstion = 30);
    /// Perform back substitution on an SVD decomposed matrix. Solves
    /// equation Ax = b.
    template <typename S>
    int		SVDBackSub(UT_MatrixT<T> &u, UT_VectorT<S> &w, UT_MatrixT<T> &v,
			   UT_VectorT<S> &b, UT_VectorT<S> &x, T tol = 1e-6);

    /// Finds eigenvalues from a real schur form matrix:
    template <typename S>
    void	findEigenValues(const UT_MatrixT<T> &A, 
				UT_VectorT<S> &vreal, UT_VectorT<S> &vimg);
    /// Finds eigenvalues from generalized system (A - lambdaB)
    template <typename S>
    void	findEigenValues(const UT_MatrixT<T> &A, const UT_MatrixT<T> &B,
				UT_VectorT<S> &sreal, UT_VectorT<S> &simg,
				UT_VectorT<S> &treal, UT_VectorT<S> &timg);

    /// Finds the u & v eigenvalues from an eigenvector, whose udeg,
    /// vdeg, and tdeg are as specified.  Returns 0 if fails.
    /// NB: Does the s/(1+s) transformation!
    template <typename S>
    int		findUVfromEigenvector(const UT_VectorT<S> &vect,
			int udeg, int vdeg, int tdeg, T &u, T &v);

    /// Finds a specified eigenvector from a real schur form matrix.
    /// The eigenvector must be real!
    template <typename S>
    void	findRightEigenVector(const UT_MatrixT<T> &A,
			const UT_MatrixT<T> &Q, UT_VectorT<S> &vector, int i);
				

private:
    /// Pushes down zeros, causing A to be a reduced hessenberg.
    void	chaseZerosQZ(UT_MatrixT<T> &A, UT_MatrixT<T> &B, int zeropos);

    /// Pushes across zeros generated in Golub-Kahan SVD method
    void	chaseZerosGK(UT_MatrixT<T> &B, int zeropos);

    /// Applies one iteration of the QZ algorithm to unreduced
    /// upper hessenberg A & non-singular upper triangular B.
    void	QZStep(UT_MatrixT<T> &A, UT_MatrixT<T> &B);

    /// Performs the francisQRStep on H.
    void 	francisQRStep(UT_MatrixT<T> &H);

    // Performs francisQRStep, updating H12 = H12*Z,
    // Q = Q*Z, H23 = ZT * H23 simultaneously with finding H = ZT H Z
    void 	francisQRStep(
			UT_MatrixT<T> &H, 
			UT_MatrixT<T> &H12,
			UT_MatrixT<T> &H23,
			UT_MatrixT<T> &Q);

    /// Performs one step in the Golub-Kahan SVD algorithm:
    /// B is a bidiagonal matrix with no zeros on diagonal nor supradiagonal,
    void	golubKahanSVDStep(UT_MatrixT<T> &B);

    /// Solves (A - lambda I) x = gamma B for x, requires A is quasi triangular.
    /// B is column.
    template <typename S>
    void	solveQuasiUpperTriangular(const UT_MatrixT<T> &A, T lambda,
				    T gamma, 
				    const UT_MatrixT<T> &B, UT_VectorT<S> &x,
				    T tol = 1e-6);

    /// Performs inverse iteration until convergence or maxiter is reached.
    /// The vector is updated with the newest eigenvector.
    /// Call with findDecomp = 0 to use R directly as opposed to decomposing
    /// it.  Source matrices are:
    ///        [1  0  0]        [0 1 0]
    ///  C1' = [0  1  0]  C2' = [0 0 1]
    ///        [0  0 C1]        [ C2  ]
    /// Returns:
    ///  1: Decomp found the pencil to be degenerate, perfect solution.
    ///  0: Successfully converged
    /// <0: Failed to converge
    template <typename S>
    int		inversePowerSingle( const UT_MatrixT<T> &C1, 
				    const UT_MatrixT<T> &C2, 
				    UT_MatrixT<T> &R, UT_MatrixT<T> &tmp, 
				    UT_Permutation &index, 
				    T &s, T lasts,
				    UT_VectorT<S> &q, UT_VectorT<S> &Aq,
				    int findDecomp = 1, 
				    T tol = 1e-5, int maxiter = 50);

    /// Performs inverse iteration until convergence or maxiter is reached.
    /// The vector is updated with the newest eigenvector.
    /// Call with findDecomp = 0 to use R directly as opposed to decomposing
    /// it.  Source matrices are:
    ///        [1  0  0]        [0 1 0]
    ///  C1' = [0  1  0]  C2' = [0 0 1]
    ///        [0  0 C1]        [ C2  ]
    /// Computes two solutions, namely P & Q such that
    /// A = (C2 - sC1), and
    /// lim i->inf (A^i+2 - pA^i+1 - qA^i) u = 0
    /// Returns:
    ///  1: Decomp found the pencil to be degenerate, perfect solution.
    ///  0: Successfully converged
    /// <0: Failed to converge
    template <typename S>
    int		inversePowerDouble( const UT_MatrixT<T> &C1, 
				    const UT_MatrixT<T> &C2, 
				    UT_MatrixT<T> &R, UT_MatrixT<T> &tmp, 
				    UT_Permutation &index, 
				    T s, 
				    T &p, T &q,
				    UT_VectorT<S> &u, UT_VectorT<S> &Au,
				    int findDecomp = 1, 
				    T tol = 1e-5, int maxiter = 50);

};

typedef UT_MatrixSolverT<fpreal>    UT_MatrixSolverR;
typedef UT_MatrixSolverT<fpreal32>  UT_MatrixSolverF;
typedef UT_MatrixSolverT<fpreal64>  UT_MatrixSolverD;
typedef UT_MatrixSolverT<fpreal64>  UT_MatrixSolver;  // Matches UT_MatrixF

// UT_MatrixIterSolver is mainly for sparse matrices.
// The sparse matrices may be defined explicitly with UT_SparseMatrix,
// or implicitly with functions.
template <typename T>
class UT_API UT_MatrixIterSolverT
{
public:
    typedef UT_Functor2<void, const UT_VectorT<T> &, UT_VectorT<T> &> MatrixOp;
    typedef UT_Functor2<bool, exint, T> StopConditionOp;
    typedef UT_Functor2<UT_PreciseT<T>, const UT_VectorT<T> &, UT_VectorT<T> &>
        MatrixDotOp;
    typedef UT_SparseMatrixCSRT<T> SparseMatrixType;

    // Creates a new iterative matrix solver.
    // Inputs:
    // 1. maxIterations: The upper bound of iterations at which the iterative
    //                   solver will automatically stop.
    //                   If this is < 0 then the default will be 3*N where N
    //                   is the length of the solving vector
    // 2. tolerance: The maximum error at which the iteration will stop
    // 3. useAbsoluteError: If true, the tolerance will be used as an absolute
    //                      tolerance. Otherwise, the tolerance is divided by
    //                      the vector norm to produce a relative tolerance.
    UT_MatrixIterSolverT(exint maxIterations = -1,
        UT_PreciseT<T> tolerance = 1e-5, bool useAbsoluteError = false);

    // Sets the operation for A*x using a custom functor
    void setMultVec(const MatrixOp &op);

    // Sets the operation for A*x using UT_SparseMatrix::multVec.
    // Also sets myMultVecAndDotOp to UT_SparseMatrix::multVecAndDot to speed
    // up solvers that can use it.
    void setMultVec(const SparseMatrixType &matrix);

    // Sets the operation for At*x using a custom functor
    void setTransposeMultVec(const MatrixOp &op);

    // Sets the operation for At*x using UT_SparseMatrix::multVec
    void setTransposeMultVec(const SparseMatrixType &transposematrix);

    // Sets the operation for inv(M)*x where M is the preconditioner using a
    // custom functor
    void setPreconditioner(const MatrixOp &op);

    // Sets the operation for inv(M)*x using an inverted diagonal vector and
    // UT_Vector::multAndSet
    void setPreconditioner(const UT_VectorT<T> &invertedDiagonal);

    // Sets the operation for inv(M)*x using lower and upper triangular matrices
    // For r = inv(LU)*x, this solves Lz = x, Ur = z.
    // L and U can refer to the same matrix if the matrix is storing both.
    // Set LhasUnitDiagonal/UhasUnitDiagonal to true if either L/U should have
    // its diagonal assumed to be all ones.
    void setPreconditioner(const SparseMatrixType &L, const SparseMatrixType &U,
        bool LhasUnitDiagonal = false, bool UhasUnitDiagonal = false);

    // Sets the operation for inv(Mt)*x. The usage is identical to
    // setPreconditioner.
    void setTransposePreconditioner(const MatrixOp &op);
    void setTransposePreconditioner(const UT_VectorT<T> &invertedDiagonal);

    // Sets the operation to determine if iteration should prematurely stop.
    // The operation is called every iteration and will break the iteration if
    // it returns true.
    void setStopCondition(const StopConditionOp &op);

    // Solves Ax = b using the Preconditioned Conjugate Gradient method,
    // where A is a symmetric positive definite matrix.
    // Requires MultVec and PreconditionerOp to be set.
    // myMultVecAndDotOp will be used if set for a performance increase.
    void PCG(UT_VectorT<T> &x, const UT_VectorT<T> &b,
        exint *resultIters = NULL, UT_PreciseT<T> *resultError = NULL,
        UT_Interrupt *boss = NULL) const;

    // Solves Ax = b using the Preconditioned Conjugate Gradient method
    // for least squares. i.e. Computes vector x that minimizes the Euclidean
    // 2-norm ||b - Ax||^2. A does not need to be square.
    // Given A is an MxN matrix, this requires length(x) == N, length(b) == M.
    // Requires MultVec, TransposeMultVec and PreconditionerOp to be set.
    void PCGLS(UT_VectorT<T> &x, const UT_VectorT<T> &b,
        exint *resultIters = NULL, UT_PreciseT<T> *resultError = NULL,
        UT_Interrupt *boss = NULL) const;

    // Solve Ax = b using the Preconditioned Biconjugate Gradient method.
    // A must be square but does not need to be symmetric or self-adjoint.
    // Requires MultVec, TransposeMultVec, PreconditionerOp, and
    // TransposePreconditionerOp to be set.
    void PBiCG(UT_VectorT<T> &x, const UT_VectorT<T> &b,
        exint *resultIters = NULL, UT_PreciseT<T> *resultError = NULL,
        UT_Interrupt *boss = NULL) const;

    // Solves Ax = b using the Preconditioned Biconjugate Gradient Stabilized
    // method. A must be square but does not need to be symmetric/self-adjoint.
    // Requires MultVec and PreconditionerOp to be set.
    void PBiCGStab(UT_VectorT<T> &x, const UT_VectorT<T> &b,
        exint *resultIters = NULL, UT_PreciseT<T> *resultError = NULL,
        UT_Interrupt *boss = NULL) const;

private:

    exint myMaxIterations;
    UT_PreciseT<T> myTolerance;
    bool myUseAbsError;

    MatrixOp myMultVecOp;
    MatrixOp myTransposeMultVecOp;
    MatrixOp myPreconditionerOp;
    MatrixOp myTransposePreconditionerOp;
    StopConditionOp myStopConditionOp;
    MatrixDotOp myMultVecAndDotOp;

public:
    // TODO: Deprecate these

    /// Preconditioned Conjugate Gradient method for solving Ax=b,
    /// given the symmetric positive definite matrix A[0..n-1][0..n-1] 
    /// indirectly through AMult (ie. Ax),
    /// a preconditioner for A through ASolve, 
    /// the vector b[0..n-1] and an initial guess x[0..n-1].
    /// maxIter defaults to 2n.
    /// error |b-Ax|/|b| < tol.
    /// normType indicates the type of norm used for error:
    ///     0    L-infinity norm (ie. max)
    ///     1    L1-norm         (ie. sum of abs)
    ///     2    L2-norm         (ie. Euclidean distance)
    /// Output: x[0..n-1] and error.
    T	PCG(void (*AMult)(const UT_VectorT<T> &x, UT_VectorT<T> &result),
                    void (*ASolve)(const UT_VectorT<T> &b, UT_VectorT<T> &x),
                    int n, UT_VectorT<T> &x, const UT_VectorT<T> &b,
                    T tol=1e-3, int normType=2, int maxIter=-1);

    /// Preconditioned Conjugate Gradient method for solving Ax=b with
    /// the symmetric positive definite matrix A, preconditioner, and iteration
    /// conditions implicitly defined by functors.
    static void PCG(UT_VectorT<T> &x, const UT_VectorT<T> &b,
        const UT_Functor2<void, const UT_VectorT<T> &, UT_VectorT<T> &> &AMult,
        const UT_Functor2<void, const UT_VectorT<T> &, UT_VectorT<T> &> &ASolve,
        const UT_Functor2<bool, int, const UT_VectorT<T> &> &iterateTest);

    /// Preconditioned Conjugate Gradient method for solving least squares Ax=b,
    /// given the matrix A[0..m-1][0..n-1] indirectly through AMult 
    /// (ie. Ax if int = 0 or A^tx if int = 1),
    /// a preconditioner for A^tA through AtASolve, 
    /// the vector b[0..m-1] and an initial guess x[0..n-1].
    /// maxIter defaults to 2n.
    /// error |A^t(b-Ax)|/|A^tb| < tol.
    /// normType indicates the type of norm used for error:
    ///     0    L-infinity norm (ie. max)
    ///     1    L1-norm         (ie. sum of abs)
    ///     2    L2-norm         (ie. Euclidean distance)
    /// Output: x[0..n-1] and error.
    T       PCGLS(void (*AMult)(const UT_VectorT<T> &x, UT_VectorT<T> &result, 
				    int transpose),
                  void (*AtASolve)(const UT_VectorT<T> &b, UT_VectorT<T> &x),
                  int m, int n,
                  UT_VectorT<T> &x, const UT_VectorT<T> &b,
                  T tol=1e-3, int normType=2, int maxIter=-1);

    ///
    /// Modified interface for the above Preconditioned Conjugate Gradient
    /// for solving Ax=b. The template parameter class C must implement
    /// the following two methods:
    ///
    ///
    /// 1) A method to compute either Av or A^tv depending on transpose
    /// being 0 or 1.
    ///
    /// void AMult(const UT_VectorT<T> &v, UT_VectorT<T> &result,
    /// 	   bool transpose);
    ///
    ///
    /// 2) The preconditioner as above.
    ///
    /// void AtASolve(const UT_VectorT<fpreal> &b, UT_VectorT<fpreal> &x)
    ///
    /// The rel_tol and abs_tol respectively represent relative and absolute
    /// tolerance. Set either to a positive number to stop the solver as soon
    /// as the corresponding error drops below the given threshold. If both
    /// positive, the solver stops as soon as one of the errors meets its
    /// corresponding threshold.
    ///
    template <typename AMult, typename AtASolve>
    T 	    PCGLS(const AMult &amult, const AtASolve &atasolve,
		  int rows, int cols, UT_VectorT<T> &x, const UT_VectorT<T> &b,
		  fpreal tol = -1.0, int norm_type = 2, int max_iter = -1);

    /// Preconditioned Conjugate Gradient method for solving Ax=b,
    /// given the matrix A[0..m-1][0..n-1] indirectly through AMult 
    /// (ie. Ax if int = 0 or A^tx if int = 1),
    /// a preconditioner for A through ASolve, 
    /// the vector b[0..m-1] and an initial guess x[0..n-1].
    /// maxIter defaults to 2n.
    /// error |b-Ax|/|b| < tol.
    /// normType indicates the type of norm used for error:
    ///     0    L-infinity norm (ie. max)
    ///     1    L1-norm         (ie. sum of abs)
    ///     2    L2-norm         (ie. Euclidean distance)
    /// Output: x[0..n-1] and error.
    T       biPCG(void (*AMult)(const UT_VectorT<T> &x, UT_VectorT<T> &result, 
				    int transpose),
                  void (*ASolve)(const UT_VectorT<T> &b, UT_VectorT<T> &x, 
                                int transpose),
                  int n,
              UT_VectorT<T> &x, const UT_VectorT<T> &b,
              T tol=1e-3, int normType=2, int maxIter=-1);
};

typedef UT_MatrixIterSolverT<fpreal>    UT_MatrixIterSolverR;
typedef UT_MatrixIterSolverT<fpreal32>  UT_MatrixIterSolverF;
typedef UT_MatrixIterSolverT<fpreal64>  UT_MatrixIterSolverD;
typedef UT_MatrixIterSolverT<fpreal64>  UT_MatrixIterSolver;

#include "UT_MatrixSolverImpl.h"

#endif
