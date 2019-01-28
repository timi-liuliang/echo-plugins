/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Sparse Matrix Class (C++)
 *
 * COMMENTS:
 *
 * Storage format: Co-ordinate (COO). See
 *     Y. Saad. SPARSKIT: A basic toolkit for sparse matrix computations.
 *         Research Institute for Advanced Computer Science, RIACS-90-20,
 *         1990.
 * for details.
 */

#ifndef __UT_SparseMatrix_H__ 
#define __UT_SparseMatrix_H__

#include "UT_API.h"

#include "UT_IntArray.h"
#include "UT_ThreadedAlgorithm.h"
#include "UT_Vector.h"
#include "UT_Interrupt.h"
#include "UT_ParallelUtil.h"

#include "UT_VectorTypes.h"

#include <iterator>
#include <type_traits>

class UT_IStream;
class UT_BitArray;

template <typename T> class UT_API UT_SparseMatrixRowT;

template <typename T, bool IsPaged>
class UT_API UT_SparseMatrixT
{
    class ut_MatrixCell
    {
    public:
        int myRow;
        int myCol;
        T myValue;

	inline bool operator<(const ut_MatrixCell &o) const
	{
	    if (myRow < o.myRow)
		return true;
	    if (myRow == o.myRow && myCol < o.myCol)
		return true;
	    return false;
	}
    };

    // For paged matrices, store the size of each page
    // We always allocate full pages.
    static const int CELL_PAGESIZE = 1024;
    static const int CELL_PAGEMASK = 1023;
    static const int CELL_PAGEBITS = 10;

    class ut_CellIterator : public std::iterator<std::random_access_iterator_tag, ut_MatrixCell>
    {
    public:
	ut_CellIterator(const UT_SparseMatrixT<T, IsPaged> *matrix, int idx)
	{ myMatrix = matrix; myPos = idx; repage(); }

	ut_CellIterator operator+(ptrdiff_t n) { return ut_CellIterator(myMatrix, myPos+n); }
	ut_CellIterator operator-(ptrdiff_t n) { return ut_CellIterator(myMatrix, myPos-n); }
	ut_CellIterator &operator+=(ptrdiff_t n) { myPos += n; repage(); return *this; }
	ut_CellIterator &operator-=(ptrdiff_t n) { myPos -= n; repage(); return *this; }
	ut_CellIterator &operator++() { myPos++;  myOffset++;  myData++; if (myOffset >= CELL_PAGESIZE) repage(); return *this; }
	ut_CellIterator &operator--() { myPos--;  myOffset--;  myData--; if (myOffset < 0) repage(); return *this; }
	ut_CellIterator operator++(int) { ut_CellIterator result = *this; myPos++;  myOffset++;  myData++; if (myOffset >= CELL_PAGESIZE) repage(); return result; }
	ut_CellIterator operator--(int) { ut_CellIterator result = *this; myPos--;  myOffset--;  myData--; if (myOffset < 0) repage(); return result; }
	int		operator-(ut_CellIterator b) const { return myPos - b.myPos; }
	ut_MatrixCell   &operator[](ptrdiff_t idx) { return myMatrix->getCell(idx + myPos); }

	bool		operator<(ut_CellIterator b) const { return myPos < b.myPos; }
	bool		operator>(ut_CellIterator b) const { return myPos > b.myPos; }
	bool		operator>=(ut_CellIterator b) const { return myPos >= b.myPos; }
	bool		operator==(ut_CellIterator b) const { return myPos == b.myPos; }
	bool		operator!=(ut_CellIterator b) const { return myPos != b.myPos; }

	ut_MatrixCell	&operator*() const { return *myData; }
	ut_MatrixCell	*operator->() const { return myData; }

    protected:

	void		repage()
	{ myPage = myPos >> CELL_PAGEBITS; myOffset = myPos & CELL_PAGEMASK;
	  myData = 0;
	  if (myPage < myMatrix->myCellPages.entries())
	      myData = &myMatrix->myCellPages(myPage)[myOffset];
	}

	const UT_SparseMatrixT<T, IsPaged>	*myMatrix;
	ut_MatrixCell			*myData;
	ptrdiff_t			 myPos;
	int				 myPage, myOffset;
    };

    // For compact compiled matrix cells, specify the number of values to
    // pack into a single cell.
    static const int CELLBITS = 2;
    static const int CELLSIZE = 1 << CELLBITS;
    static const int CELLMASK = CELLSIZE-1;

    class ut_4MatrixCell
    {
    public:
        T myValue[CELLSIZE];
	int myRow;
        int myCol;
    } SYS_ALIGN16;

public:
    UT_SparseMatrixT();

    // creates a valid 'rows' x 'cols' zero matrix
    UT_SparseMatrixT(int rows, int cols);
    UT_SparseMatrixT(const UT_SparseMatrixT<T, IsPaged> &m);
    ~UT_SparseMatrixT();

    int getNumRows() const { return myNumRows; }
    int getNumCols() const { return myNumCols; }

    /// Return the amount of memory used by this array.
    int64	 getMemoryUsage() const;
    int64	 getIdealMemoryUsage() const;

    /// Ensures at least this number of cells are available, useful
    /// if you can predict your size.
    void	 reserve(int numcells);

    /// Shrinks the array to exactly fit the number of cells present
    void	 shrinkToFit();

    // sets equal to a 'rows' x 'cols' zero matrix
    void init(int rows, int cols);

    // Sets all entries of the matrix to zero.
    void zero();

    // Determines if we are large enough to justify mulithreading.
    // The 5000 was determined from experiments with UT_Vector.
    bool	 shouldMultiThread() const
		 {
#ifdef CELLBE
		     return false;
#else
		     return getNumRows() > 5000;
#endif
		 }

    // does M(row,col) += value
    bool addToElement(int row, int col, T value);

    // Returns the index of the cell for which cell->row >= row is true
    // first.  This could be myCount if row is too high.
    // myCells[result].myRow may be strictly greater than row if it isn't
    // present.
    int findCellFromRow(int row) const;

    // computes M * v
    inline void multVec(const UT_VectorT<T> &v, UT_VectorT<T> &result) const
    {
	compile();
	multVecInternal(v, result);
    } 

    /// Do result = result - (M * v)
    inline void subtractMultVec(const UT_VectorT<T> &v, UT_VectorT<T> &result) const
    {
	compile();
	subtractMultVecInternal(v, result);
    } 
    inline void subtractMultVecNoThread(const UT_VectorT<T> &v, UT_VectorT<T> &result) const
    {
	compile();
	subtractMultVecInternalNoThread(v, result);
    } 

    // computes M^T * v
    void transposeMultVec(const UT_VectorT<T> &v, UT_VectorT<T> &result) const;

    /// Square of L2-norm of all columns
    /// Use to form the diagonal part of A^tA of an overdetermined system A.
    /// Needed for both Jacobi and Gauss-Seidel iterations.
    void allColNorm2(UT_VectorT<T> &result) const;

    /// Initializes out to a matrix where the (i,j) elements are
    /// (rowstart+i, colstart+j) elements of this.  The rowend and
    /// colend represent the exclusive end - [rowstart,rowend) will
    /// be extracted for numrows = rowend - rowstart
    /// This function will compile *this, if it wasn't before the call.
    /// The resulting matrix will be compiled (by construction).
    void extractSubMatrix(UT_SparseMatrixT<T, IsPaged> &out,
			    int rowstart, int rowend,
			    int colstart, int colend) const;
    /// Extract submatrix without compiling *this.
    /// The resulting submatrix won't be compiled either.
    void extractSubMatrixUncompiled(UT_SparseMatrixT<T, IsPaged> &out,
			    int rowstart, int rowend,
			    int colstart, int colend) const;

    // Get fixed-size square submatrix
    // This will trigger compilation 
    void getSmallSquareSubMatrix(T submatrix[],
	                         const exint indices[], const int num_indices) const;

    // For each row, column, and value call visitor(row, column, value)
    // visitor may be called with the same (row, column) pair multiple times
    // the sum of all the associated values is the component in the matrix for (row, column)
    template<typename Visitor>
    void accept(Visitor& visitor) const
    {
	for(exint ci = 0; ci < myCount; ++ci)
	{
	    ut_MatrixCell *cell = &getCell(ci);
	    visitor(cell->myRow, cell->myCol, cell->myValue);
	}
    }

    /// Clear the rows and columns specified by the given bit field to a row 
    /// from the identity matrix
    void clearRowsAndColumns(const UT_BitArray &toclear);

    /// Extract the diagonal vector from a matrix. This does not
    /// require the matrix to be compiled, nor does it try to compile it.
    /// For entries (i, j), this method will extract the entries where
    /// (i-j == idx).
    void extractDiagonal(UT_VectorT<T> &out, int idx = 0) const;

    /// Extract everything but the diagonal:
    /// out(i, j) = 0,           if i == j,
    //              *this(i, j), otherwise
    void extractNondiagonal(UT_SparseMatrixT<T, IsPaged> &out) const;

    /// Incomplete Cholesky Factorization.
    /// Does a Cholesky Factorization but does not make any elements non-zero
    /// The result of this operation is an UPPER triangular matrix G
    /// such that A = Gt G.
    /// The input must be a symmetric matrix.
    /// Note that this factorization is not always stable.
    /// Returns 0 on success, -1 if semi-definite (diagonal contained zeros
    /// within the given tolerance) and -2 if not positive definite (diagonal
    /// contained negative numbers)
    int		incompleteCholeskyFactorization(T tol=1e-5);

    /// Modified Incomplete Cholesky
    /// Same as incomplte cholesky, except attempt to account for the
    /// discarded entries by adjusting the diagonal.
    /// tau is a tuning constant.
    int		modifiedIncompleteCholesky(T tau = 0.97, 
					    T mindiagratio = 0.25,
					    T tol=1e-5);

    /// Assumes this is a lower triangular matrix.  Solves the equation
    /// A x = b
    /// If the diagonal of A is zero within tolerance, the corresponding
    /// x coordinate is zero.
    /// Returned is the number of artifical zeros places into x.  0 means
    /// the solution encountered no singularities, 10 would mean 10 
    /// singularities.
    int 	solveLowerTriangular(UT_VectorT<T> &x, const UT_VectorT<T> &b,
				    T tol=1e-5) const;
    int 	solveUpperTriangular(UT_VectorT<T> &x, const UT_VectorT<T> &b,
				    T tol=1e-5) const;
    /// Given an upper triangular matrix, solves the lower triangular
    /// transposed of it and negates the result.
    int  	solveLowerTriangularTransposeNegate(UT_VectorT<T> &x, 
				    const UT_VectorT<T> &b,
				    T tol=1e-5) const;

    // Use the conjugate gradient method to solve Ax=b.
    // NB: 'x' should be initialized with the initial estimate
    bool	solveConjugateGradient(UT_VectorT<T> &x, const UT_VectorT<T> &b,
			const UT_SparseMatrixT<T, IsPaged> *GT,
			bool (*callback_func)(void *) = 0,
			void *callback_data = 0, T tol2=1e-5,
			int max_iters = -1) const;

    /// Transposes this matrix.
    THREADED_METHOD(UT_SparseMatrixT, shouldMultiThread(), transpose)
    void	transposePartial(const UT_JobInfo &info);

    /// Makes this a transposed copy of source.  By not working
    /// in place we can avoid a sort if the source is compiled.
    /// If source is not compiled, we compile it.
    void	transposeCompiled(const UT_SparseMatrixT<T, IsPaged> &src);

    /// *this = -*this.
    THREADED_METHOD(UT_SparseMatrixT, shouldMultiThread(), negate)
    void	negatePartial(const UT_JobInfo &info);

    UT_SparseMatrixT<T, IsPaged> &operator=(const UT_SparseMatrixT<T, IsPaged> &m);
    UT_SparseMatrixT<T, IsPaged> &operator*=(T scalar);
    UT_SparseMatrixT<T, IsPaged> &operator+=(const UT_SparseMatrixT<T, IsPaged> &m);

    // Writes out the full matrix as an Row X Column array.  Likely
    // not too useful with big matrices...
    void		printFull(std::ostream &os) const;

    // Writes out all of the non-zero elements.
    void		printSparse(std::ostream &os) const;

    // Writes out all of the non-zero elements as a Matlab "sparse" expression.
    void        printSparseMatlab(std::ostream &os,
                                  const UT_String &varname) const;

    // Binary save and load of a sparse matrix
    void		save(std::ostream &os) const;
    void		load(UT_IStream &is);
    
    /// Reorders the cell array to be sorted by row than column.
    /// Consolidates any duplicate entries by adding them together.
    /// Culls any zero entries.
    /// compile will be a no-op if the matrix's structure has not changed
    /// since the last invocation.
    /// While compilation is expensive, it also performs the collapse/cull
    /// operation, so you may want to invoke it explicitly if you are
    /// repeatedly gathering similar matrices, say with +=.
    void			 compile() const;
    bool			 isCompiled() const { return myCompiledFlag; }
    bool			 isStillSorted() const { return myStillSortedFlag; }

    bool			 hasNan() const;
    void			 testForNan() const;

private:
    /// Always recompiles the matrix regardless of the compile flag setting.
    void			 forceCompile() const;

    /// Extract a cell by its index.
    inline ut_MatrixCell	&getCell(int idx) const 
    { 
	if (IsPaged)
	    return myCellPages(idx >> CELL_PAGEBITS)[idx & CELL_PAGEMASK]; 
	else
	    return myCells[idx]; 
    }

    THREADED_METHOD2_CONST(UT_SparseMatrixT, shouldMultiThread(), 
		    multVecInternal, const UT_VectorT<T> &, v,
		    UT_VectorT<T> &, result)
    void multVecInternalPartial(const UT_VectorT<T> &v, UT_VectorT<T> &result,
			const UT_JobInfo &info) const;

    THREADED_METHOD2_CONST(UT_SparseMatrixT, shouldMultiThread(), 
	                   subtractMultVecInternal,
			   const UT_VectorT<T>&, v,
			   UT_VectorT<T>&, result)
    void subtractMultVecInternalPartial(const UT_VectorT<T>& v, UT_VectorT<T>& result,
	                        const UT_JobInfo& info) const;

    /// Compile into 4-value cell data structure
    void			 compile4() const;

    int				 myNumRows;
    int				 myNumCols;
    mutable UT_ValArray<ut_MatrixCell *>	myCellPages;
    mutable ut_MatrixCell 	*myCells;
    mutable int			 myCount;
    mutable int			*my4RowOffsets;
    mutable ut_4MatrixCell	*my4Cells;
    mutable int			 my4Count;
    int				 myMaxSize;
    mutable bool		 myCompiledFlag;
    mutable bool		 myStillSortedFlag;
    mutable bool		 my4CompiledFlag;

    friend class	UT_SparseMatrixRowT<T>;
};

typedef UT_SparseMatrixT<fpreal32, false>	UT_SparseMatrixF;
typedef UT_SparseMatrixT<fpreal64, false>	UT_SparseMatrixD;
typedef UT_SparseMatrixT<fpreal64, false>	UT_SparseMatrix;


///
/// This is a highly specialized varient of the SparseMatrix
/// which does not let you change the number or position of cells
/// It has, however, compiled fixed row offsets to make it fast
/// to look up specific rows in the matrix.
///
template <typename T>
class UT_API UT_SparseMatrixRowT
{
    class ut_MatrixCell
    {
    public:
        int myCol;
        T myValue;
    };

public:
    UT_SparseMatrixRowT();
    ~UT_SparseMatrixRowT();

    // Can't mix paged and non paged, and this is a non-paged algorithm
    // Destroys the source matrix in the process as we reuse its
    // myCells array.
    void	 buildFrom(UT_SparseMatrixT<T, false> &m,
			    bool invertdiag = false,
			    T tol=1e-5f);

    int getNumRows() const { return myNumRows; }
    int getNumCols() const { return myNumCols; }

    /// Return the amount of memory used by this array.
    int64	 getMemoryUsage() const;

    // The 5000 was determined from experiments with UT_Vector.
    bool	 shouldMultiThread() const
		 {
		     return getNumRows() > 5000;
		 }

    // Returns the index of the cell for which cell->row >= row is true
    // first.  This could be myCount if row is too high.
    // myCells[result].myRow may be strictly greater than row if it isn't
    // present.
    int 	findCellFromRow(int row) const
		{ return myRowOffsets(row); }

    // Get the inverted diagonal.
    void getDiagonalInv(UT_VectorT<T> &out) const;

    // computes M * v
    THREADED_METHOD2_CONST(UT_SparseMatrixRowT, shouldMultiThread(), multVec,
		    const UT_VectorT<T> &, v,
		    UT_VectorT<T> &, result)
    void multVecPartial(const UT_VectorT<T> &v, UT_VectorT<T> &result,
			const UT_JobInfo &info) const;

    // computes M * v and accumulates the dot product of v & result
    void multVecAndDot(
        const UT_VectorT<T> &v,
        UT_VectorT<T> &result,
        fpreal64 *dotpq) const;

    /// Assumes this is a lower triangular matrix.  Solves the equation
    /// A x = b
    /// If the diagonal of A is zero within tolerance, the corresponding
    /// x coordinate is zero.
    /// Returned is the number of artifical zeros places into x.  0 means
    /// the solution encountered no singularities, 10 would mean 10 
    /// singularities.
    int 	solveUpperTriangular(UT_VectorT<T> &x, const UT_VectorT<T> &b,
				    T tol=1e-5) const;
    /// Given an upper triangular matrix, solves the lower triangular
    /// transposed of it and negates the result.
    int  	solveLowerTriangularTransposeNegate(UT_VectorT<T> &x, 
				    const UT_VectorT<T> &b,
				    T tol=1e-5) const;

    /// Solves conjugate gradient using our specialized functions.
    /// These allow us to perform some normal and dot operations
    /// while the cache is still hot.
    /// This matrix is the matrix to solve.  The provided GT matrix
    /// is the upper triangular result of cholesky factoriziation.
    /// Norm is hardcoded to 2.
    /// If the GT matrix is null, no preconditioner will be used.
    float	solveConjugateGradient(UT_VectorT<T> &x, const UT_VectorT<T> &b,
			const UT_SparseMatrixRowT<T> *GT, T tol2=1e-5,
			int max_iters = -1, int *iterout = NULL) const;
private:
    THREADED_METHOD3_CONST(UT_SparseMatrixRowT, shouldMultiThread(), 
        multVecAndDotInternal,
        const UT_VectorT<T> &, v,
        UT_VectorT<T> &, result,
        fpreal64 *, dotpq)
    void multVecAndDotInternalPartial(const UT_VectorT<T> &v, UT_VectorT<T> &result,
        fpreal64 *dotpq,
        const UT_JobInfo &info) const;

    static const exint PARALLEL_BLOCK_SIZE = 1024;

    /// Extract a cell by its index.
    inline ut_MatrixCell	&getCell(int idx) const 
    { 
	return myCells[idx]; 
    }

    int				 myNumRows;
    int				 myNumCols;
    ut_MatrixCell 		*myCells;
    int			 	 myCount;
    UT_IntArray			 myRowOffsets;
    UT_VectorT<T>		 myDiagonal;
};

typedef UT_SparseMatrixRowT<fpreal32>	UT_SparseMatrixRowF;
typedef UT_SparseMatrixRowT<fpreal64>	UT_SparseMatrixRowD;



/// This is another highly specialized varient of the SparseMatrix
/// which allows multithreaded addition of elements to individual rows and
/// supports a conjugate gradient solve.  This can be useful for solvers which
/// need to create and solve large linear systems in parallel.
/// It uses the ELL format which is appropriate for matrices with a limited
/// but fairly consistent numer of non-zeros per row, for example the result
/// of discretizations across regular grids.
template <typename T, bool colmajor = false, bool useexint = false>
class UT_SparseMatrixELLT
{
    typedef typename std::conditional<useexint, exint, int>::type inttype;

public:
    
    UT_SparseMatrixELLT();

    UT_SparseMatrixELLT(inttype rows, int nzeros);

    void init(inttype rows, int nzeros);

    void zero();

    exint getNumRows() const { return myRows; }

    int getNonZerosPerRow() const { return myNonZeros; }

    const UT_ValArray<T> &getRowValues() const { return myRowVals; }
    UT_ValArray<T> &getRowValues() { return myRowVals; }
  
    const UT_ValArray<inttype> &getColumns() const { return myColumns; }
    UT_ValArray<inttype> &getColumns() { return myColumns; }

    exint index(inttype row, int nz) const
    {
    	if (colmajor)
    	    return (exint)nz * myRows + (exint)row;
    	return (exint)row * (exint)myNonZeros + (exint)nz;
    }

    bool appendRowElement(inttype row, inttype col, T val, int &rowidx)
    {
    	UT_ASSERT(rowidx < myNonZeros);
    	exint idx = index(row, rowidx);
    	myRowVals(idx) = val;
    	myColumns(idx) = col;
    	rowidx++;
    	return true;
    }

    // Sort rows by column index.
    THREADED_METHOD(UT_SparseMatrixELLT, shouldMultiThread(),
		     sortRows)
    void sortRowsPartial(const UT_JobInfo &info);

    /// Every reference to given rows is removed from the diagonal.
    /// Because searching the whole matrix is expensive, assumes it
    /// is a standard fluid matrix, so for each specified row, it
    /// looks at all columns that are not itself and removes those.
    void removeEffectOfRows(const UT_ExintArray &rows);

    bool shouldMultiThread() const
    {
	return getNumRows() > 5000;
    }

    /// Copy to the supplied SparseMatrix.
    void copyToSparse(UT_SparseMatrixT<T, false> &A) const;

    // Extract the inverted diagonal
    THREADED_METHOD1_CONST(UT_SparseMatrixELLT, shouldMultiThread(),
			   getDiagonalInv, UT_VectorT<T> &, out)

    /// Get the inverse of the diagonal.  Required for the conjugate gradient
    /// solve when using Jacobi pre-conditioning.
    void getDiagonalInvPartial(UT_VectorT<T> &out,
			       const UT_JobInfo &info) const;

    // computes M * v
    THREADED_METHOD2_CONST(UT_SparseMatrixELLT, shouldMultiThread(),
			   multVec, const UT_VectorT<T> &, v,
			   UT_VectorT<T> &, result)

    void multVecPartial(const UT_VectorT<T> &v, UT_VectorT<T> &result,
			const UT_JobInfo &info) const;

    // computes M * v and accumulates the dot product of v & result
    void multVecAndDot(
        const UT_VectorT<T> &v,
        UT_VectorT<T> &result,
        fpreal64 *dotpq) const;

    // computes M * v and accumulates the dot product of v & result
    void multVecAndDotUpTo(
        const UT_VectorT<T> &v,
        UT_VectorT<T> &result,
        fpreal64 *dotpq,
        exint solverbase) const;

    /// Solves conjugate gradient using our specialized functions.
    /// These allow us to perform some normal and dot operations
    /// while the cache is still hot.
    /// This matrix is the matrix to solve.  The provided GT matrix
    /// is the upper triangular result of cholesky factoriziation.
    /// Norm is hardcoded to 2.
    /// If the GT matrix is null, no preconditioner will be used.
    float	solveConjugateGradient(UT_VectorT<T> &x, const UT_VectorT<T> &b,
			const UT_SparseMatrixRowT<T> *GT, T tol2=1e-5,
			int max_iters = -1, int *iterout = NULL) const;

    // Writes out all of the non-zero elements as a Matlab "sparse" expression.
    void printSparseMatlab(std::ostream &os,
			   const UT_String &varname) const;

    // Writes out all of the non-zero elements in Matrix Market format.
    void printSparseMatrixMarket(std::ostream &os) const;

private:

    THREADED_METHOD3_CONST(UT_SparseMatrixELLT, shouldMultiThread(),
        multVecAndDotInternal,
        const UT_VectorT<T> &, v,
        UT_VectorT<T> &, result,
        fpreal64 *, dotpq)

    void multVecAndDotInternalPartial(const UT_VectorT<T> &v, UT_VectorT<T> &result,
        fpreal64 *dotpq,
        const UT_JobInfo &info) const;

    THREADED_METHOD4_CONST(UT_SparseMatrixELLT, shouldMultiThread(),
        multVecAndDotUpToInternal,
        const UT_VectorT<T> &, v,
        UT_VectorT<T> &, result,
        fpreal64 *, dotpq,
        exint,  solverbase)

    void multVecAndDotUpToInternalPartial(const UT_VectorT<T> &v, UT_VectorT<T> &result,
        fpreal64 *dotpq, exint solverbase,
        const UT_JobInfo &info) const;

    static const exint PARALLEL_BLOCK_SIZE = 1024;

private:

    UT_ValArray<T>	    myRowVals;
    UT_ValArray<inttype>    myColumns;
    exint		    myRows;
    int 		    myNonZeros;
};

typedef UT_SparseMatrixELLT<fpreal32>	UT_SparseMatrixELLF;
typedef UT_SparseMatrixELLT<fpreal64>	UT_SparseMatrixELLD;

template <typename T> class UT_API UT_SparseMatrixBuilderT;

/// Sparse matrix class that uses the compressed sparse row storage scheme.
/// This is a general-purpose sparse matrix class that can contain an arbitrary
/// number of non-zero elements per row.
template <typename T>
class UT_API UT_SparseMatrixCSRT
{
public:
    /// Triplet to construct matrix. Is used only for setValues and addValues
    /// to construct the matrix.
    struct Triplet {
        exint myRow;
        exint myCol;
        T myValue;

        /// Construct a zero triplet.
        Triplet() : myRow(0), myCol(0), myValue(0) {}

        /// Construct a triplet for the given row and column with value v.
        Triplet(exint r, exint c, T v) : myRow(r), myCol(c), myValue(v) {}

        /// Returns true if the triplet's coordinate comes before the given
        /// triplet's coordinate.
        inline bool operator<(const Triplet &t) const {
            return (myRow < t.myRow || (myRow == t.myRow && myCol < t.myCol));
        }
    };

    /// Defines one cell in the matrix. Can also be used to directly construct
    /// the matrix with setValuesFromRows, which is easier to multithread.
    struct ColumnValue {
        exint myCol;
        T myValue;

        /// Construct a zero column-value.
        ColumnValue() : myCol(0), myValue(0) {}

        /// Construct a column value for the given column with value v.
        ColumnValue(exint c, T v) : myCol(c), myValue(v) {}

        /// Returns true if the column-value's column comes before the given
        /// column-value's column.
        inline bool operator<(const ColumnValue &cv) const {
            return myCol < cv.myCol;
        }
    };

    /// Constructs an uninitialized sparse matrix
    UT_SparseMatrixCSRT();

    /// Constructs a sparse matrix initialized with the given rows and columns.
    UT_SparseMatrixCSRT(exint rows, exint columns);

    /// Constructs a sparse matrix that is a deep copy of the given sparse
    /// matrix.
    explicit UT_SparseMatrixCSRT(const UT_SparseMatrixCSRT &matrix);

    /// Sets the size of the matrix with the given rows and columns and clears
    /// the matrix.
    void init(exint rows, exint columns);

    /// Clears all data in the matrix
    void zero();

    /// Swaps size and data elements with the given sparse matrix.
    void swap(UT_SparseMatrixCSRT &other);

    /// The number of rows in the matrix.
    exint getNumRows() const { return myNumRows; }

    /// The number of columns in the matrix.
    exint getNumCols() const { return myNumCols; }

    /// The total number of non-zero elements in the matrix.
    exint getNumNonZeros() const { return myCells.size(); }

    /// The number of non-zero elements in the given row.
    exint getNumCellsInRow(exint row) const { return rowEnd(row) - rowBegin(row); }

    /// Clears then fills the matrix with the given array of triplets.
    /// This sorts the triplets array.
    /// Triplets with the same indices will be summed together.
    /// A cell whose value is smaller than the tolerance will be deleted.
    void setValues(UT_Array<Triplet> &triplets, T tolerance = 0.0);

    /// Clears then fills the matrix with the given array of ColumnValue arrays.
    /// rowValues must have the same size as the number of rows in this matrix.
    /// This data layout makes it easier to multithread the construction of the
    /// sparse matrix entries.
    void setValuesFromRows(UT_Array<UT_Array<ColumnValue>> &rowValues,
        T tolerance = 0.0);

    /// Adds the given cells to the matrix. Triplets with the same coordinates
    /// as cells already present in the matrix will be added together.
    /// The array of triplets will be sorted unless alreadySorted is true.
    /// Cells whose magnitude is below the tolerance value after the addition
    /// will be discarded
    void addValues(UT_Array<Triplet> &triplets, bool alreadySorted = false,
        T tolerance=1e-5);

    /// Writes the diagonal values of the matrix into the given vector
    /// If invert is true, the values will be inverted (i.e. 1/x).
    /// If a row diagonal's value <= tolerance, then defaultValue is placed
    /// in its result row.
    void extractDiagonal(UT_VectorT<T> &result, bool invert = false,
        T tolerance = 0, T defaultValue = 0) const;

    /// Matrix-vector multiplication. Computes result = M * v.
    void multVec(const UT_VectorT<T> &v, UT_VectorT<T> &result) const;

    /// Matrix-vector multiplication.Computes result = M * v and returns the dot
    /// product of v and the result.
    UT_PreciseT<T> multVecAndDot(const UT_VectorT<T> &v, UT_VectorT<T> &result) const;

    /// Matrix-vector multiplication. Computes result = y - Ax. result can refer
    /// to the same vector as y.
    void subtractMultVec(const UT_VectorT<T> &x, const UT_VectorT<T> &y,
        UT_VectorT<T> &result) const;

    /// Matrix-matrix multiplication. Computes result = M * B. Resultant
    /// elements whose values are smaller than the given tolerance are deleted.
    void multMatrix(const UT_SparseMatrixCSRT<T> &B, UT_SparseMatrixCSRT &result,
        T tolerance = 0.0) const;

    /// Computes the transpose of this matrix. result can refer to this matrix.
    void transpose(UT_SparseMatrixCSRT<T> &result) const;

    /// Element-wise scalar multiplication. Every element in this matrix is
    /// multiplied by the given scalar.
    void scale(const T& scalar);

    /// Element-wise scalar multiplication. Every element in row i of the matrix
    /// is multiplied by scalars[i]
    void scaleRows(const UT_VectorT<T> &scalars);

    /// Multiples each row such that the row diagonal becomes 1. If the row
    /// diagonal is below the tolerance, the row is unchanged.
    void normalizeRows(T tolerance = 1e-5);

    /// Calculates the norms of each row. result must have the same size as the
    /// number of rows in this matrix. p determines the exponent of each column.
    /// i.e. result[i] = SUM( |Aij|^p ) for j in row(i)
    /// If invert is true, then inverts each sum
    /// i.e. result[i] = 1.0 / SUM( |Aij|^p ) for j in row(i)
    /// If the row norm is smaller than the tolerance, defaultValue is
    /// placed in its result entry instead.
    void rowNorms(UT_VectorT<T> &result, int p = 2, bool invert = false,
        T tolerance = 0, T defaultValue = 0) const;

    /// Analyzes the matrix and constructs a directed acyclic graph as a level
    /// set, where each level contains rows that are independent of each other.
    /// More specifically, level i of the levelSet contains rows that are only
    /// dependent on rows present in levels [0, i-1].
    /// The size of the levelSet is the number of levels.
    /// If buildForUpper is true, this constructs a level set for the upper
    /// triangular part of the matrix. Otherwise, this constructs a level set
    /// for the lower triangular part of the matrix.
    typedef UT_Array<UT_ExintArray> LevelSet;
    void buildDependencyLevelSet(LevelSet &levelSet, bool buildForUpper = false) const;

    /// Assumes this is a lower triangular matrix. Solves the equation Ax = b.
    /// Values may be present in the upper triangular part of the matrix
    /// but they will be ignored.
    /// If unitDiagonal is true, then 1 will be used instead of the
    /// current diagonal values.
    /// If a diagonal of A is zero within the tolerance, 0 will be placed
    /// in the corresponding row for x.
    /// If negate is true, then x is negated
    /// x can refer to the same vector as b to solve in-place.
    void solveLowerTriangular(UT_VectorT<T> &x, const UT_VectorT<T> &b,
        bool unitDiagonal = false, T tolerance = 1e-5,
        bool negate = false) const;

    /// Assumes this is an upper triangular matrix. Solves the equation Ax = b.
    /// Values may be present in the lower triangular part of the matrix
    /// but they will be ignored.
    /// If unitDiagonal is true, then 1 will be used instead of the
    /// current diagonal values.
    /// If a diagonal of A is zero within the tolerance, 0 will be placed
    /// in the corresponding row for x.
    /// If negate is true, then x is negated
    /// x can refer to the same vector as b to solve in-place.
    void solveUpperTriangular(UT_VectorT<T> &x, const UT_VectorT<T> &b,
        bool unitDiagonal = false, T tolerance = 1e-5,
        bool negate = false) const;

    /// Solves the lower triangular system A^T x = b.
    /// A is assumed to be an upper triangular matrix.
    /// Values may be present in the lower triangular part of the matrix
    /// but they will be ignored.
    /// If unitDiagonal is true, then 1 will be used instead of the
    /// current diagonal values.
    /// If a diagonal of A is zero within the tolerance, 0 will be placed
    /// in the corresponding row for x.
    /// If negate is true, then x is negated.
    void solveLowerTriangularTranspose(UT_VectorT<T> &x, const UT_VectorT<T> &b,
        bool unitDiagonal = false, T tolerance = 1e-5,
        bool negate = false) const;

    /// Solves the upper triangular system A^T x = b.
    /// A is assumed to be a lower triangular matrix.
    /// Values may be present in the upper triangular part of the matrix
    /// but they will be ignored.
    /// If unitDiagonal is true, then 1 will be used instead of the
    /// current diagonal values.
    /// If a diagonal of A is zero within the tolerance, 0 will be placed
    /// in the corresponding row for x.
    /// If negate is true, then x is negated.
    void solveUpperTriangularTranspose(UT_VectorT<T> &x, const UT_VectorT<T> &b,
        bool unitDiagonal = false, T tolerance = 1e-5,
        bool negate = false) const;

    /// Factorizes this matrix into a triangular matrix M such that M*M^T ~= A
    /// This keeps the non-zero structure of the matrix. The matrix must be
    /// symmetric.
    /// This is unstable depending on the diagonal; Returns -1 if the diagonal
    /// contains zeros within the given tolerance, and -2 if not positive
    /// definite (diagonal has negative numbers).
    /// Returns 0 on success.
    /// If useModified = true, then this uses the Modified Incomplete Cholesky
    /// Factorization, where discarded entries are accounted for by adjusting
    /// the diagonal. tau is a tuning constant that scales the adjustment from
    /// the missing entries. mindiagratio is the relative tolerance at which
    /// the original diagonal will be used if the matrix becomes negative
    /// definite as a result of the computation.
    int incompleteCholeskyFactorization(T tol=1e-5, bool useModified = false,
        T tau = 0.97, T mindiagratio = 0.25);

    /// Factorizes this matrix into L and U where LU approximates this matrix.
    /// The non-zero structure of LU matches this matrix; that is, this produces
    /// ILU(0) of this matrix. A lower triangular level set is needed for
    /// multithreading.
    /// e.g.
    ///   LevelSet levelSet;
    ///   A.buildDependencyLevelSet(levelSet);
    ///   A.incompleteLUFactorization(L, U, 1e-5, levelSet);
    void incompleteLUFactorization(UT_SparseMatrixCSRT<T> &L,
        UT_SparseMatrixCSRT<T> &U, T tolerance,
        const LevelSet &workUnits, UT_Interrupt *boss = NULL) const;

    /// Initializes out to a matrix with size (rowend-rowstart, colend-colstart)
    /// and fills it with the elements defined by the submatrix in rows
    /// [rowstart, rowend), [colstart, colend).
    void extractSubMatrix(UT_SparseMatrixCSRT<T> &out,
        exint rowstart, exint rowend, exint colstart, exint colend) const;

    /// Calls inspect(row, col, value) on each cell. Each (row, col) is visited
    /// exactly once. This is done in parallel over the rows by default, but can
    /// be done serially by setting serial = true. inspect can modify the cell
    /// values if it has the signature (exint, exint, &T).
    template <typename CellFunction>
    void visit(const CellFunction& inspect, bool serial = false)
    {
        parallelExecute(UT_BlockedRange<exint>(0, myNumRows), [this, &inspect]
            (const UT_BlockedRange<exint> &r)
        {
            for (exint row = r.begin(); row < r.end(); ++row)
                for (exint i = rowBegin(row), n = rowEnd(row); i < n; ++i)
                    inspect(row, cellColumn(i), cellValue(i));
        }, serial, myLightRowOpSubRatio, myLightRowOpGrainSize);
    }

    /// Condition is called for each cell exactly once. If the condition returns
    /// true, then the cell is removed from the matrix. The function signature
    /// of the condition should be (exint row, exint col, T value) -> bool.
    /// condition can modify the cell vakyes if it has the signature
    /// (exint, exint, &T)
    template <typename CellFunction>
    void removeIf(const CellFunction& condition)
    {
        exint idx = -1, i = rowBegin(0);
        for (exint row = 0; row < myNumRows; ++row)
        {
            for (exint n = rowEnd(row); i < n; ++i)
                if (!condition(row, cellColumn(i), cellValue(i)) && ++idx != i)
                    myCells[idx] = myCells[i];
            myRowOffsets[row+1] = idx+1;
        }
        myCells.setSize(idx+1);
    }

    /// Returns true if any non-zero element in this matrix has value NaN.
    bool hasNan() const;

    /// Asserts that the matrix contains no element with value NaN.
    void testForNan() const;

    /// Writes the full matrix, including zeros, to the given output stream.
    void printFull(std::ostream &os) const;

    /// Serializes the matrix to the given output stream.
    void save(std::ostream &os) const;

    /// Deserializes the matrix from the given input stream.
    void load(UT_IStream &is);

    /// Deep copies all data from the given matrix.
    UT_SparseMatrixCSRT<T> &operator=(const UT_SparseMatrixCSRT<T> &m);

    /// Equivalent to A.scale(scalar)
    UT_SparseMatrixCSRT<T> &operator*=(const T& scalar) { scale(scalar); return *this; }

private:

    // Helper functions to get value and column
    inline T& cellValue(exint offset) { return myCells[offset].myValue; }
    inline const T &cellValue(exint offset) const { return myCells[offset].myValue; }
    inline const exint &cellColumn(exint offset) const { return myCells[offset].myCol; }

    // Helper functions to return the beginning and end indexes into myValues
    // for the given row. This can be used to iterate over the non-zero values
    // of a particular row.
    // e.g. for (exint i = rowBegin(row); i < rowEnd(row); ++i)
    //          ... Do something with cellValue(i) and cellColumn(i)
    inline const exint &rowBegin(exint row) const { return myRowOffsets[row]; }
    inline const exint &rowEnd(exint row) const { return myRowOffsets[row + 1]; }

    // Multithreading constants
    // If the number of rows is less than this number, all operations run
    // serially
    static const int myParallelThreshold   = 1000;

    // The subratio and grainsize used for row operations with cheap work units
    static const int myLightRowOpSubRatio  = 0;
    static const int myLightRowOpGrainSize = 64;

    // Executes the given body over the given range. Can force this method to
    // run serially by setting serial = true
    template <typename Body>
    inline void parallelExecute(const UT_BlockedRange<exint> &range,
        const Body &body, bool serial = false, int subratio = 0,
        int grainsize = 32, exint parallelThreshold = -1) const {
        // Use internal parallel threshold by default
        if (parallelThreshold < 0)
            parallelThreshold = myParallelThreshold;
        // Perform serially if specified or if number of rows is below threshold
        if (myNumRows < parallelThreshold || serial)
            UTserialFor(range, body);
        else
            UTparallelFor(range, body, subratio, grainsize);
    };

private:
    exint myNumRows;
    exint myNumCols;

    // Stores the non-zero cells of the matrix, sorted by row and column.
    // Has size Nz, where Nz is the number of non-zero values.
    UT_Array<ColumnValue> myCells;

    // Stores the offset of myValues at which each row starts.
    // Has size myNumRows + 1.
    // myRowOffsets[r] is the offset of myValues where row r begins.
    // myRowOffsets[r + 1] - myRowOffsets[r] is the number of
    // non-zero values in row r.
    // myRowOffsets[myNumRows] has the sentinel value Nz
    UT_ExintArray myRowOffsets;

    friend class UT_SparseMatrixBuilderT<T>;
};

typedef UT_SparseMatrixCSRT<fpreal32> UT_SparseMatrixCSRF;
typedef UT_SparseMatrixCSRT<fpreal64> UT_SparseMatrixCSRD;

/// Simple helper class to construct a SparseMatrixCSRT. This operates on the
/// matrix data in-place and is generally much more performant than constructing
/// via triplets, but the builder has the limitation of only being able to add
/// to rows sequentially; i.e. each call to startRow must start the construction
/// of a row greater than the last, while addToColumn can be called with any
/// column ordering.
/// 1. UT_SparseMatrixCSRD matrix(M, N);
/// 2. UT_SparseMatrixBuilderT builder(matrix);
/// 3. for (exint r = startRow; r < endRow; ++r)
/// 4.     builder.startRow(r);
/// 5.     for (any column)
/// 6.         builder.addToColumn(column, value);
/// 7. builder.finish();
template <typename T>
class UT_API UT_SparseMatrixBuilderT
{
public:
    typedef UT_SparseMatrixCSRT<T> SparseMatrixType;

    /// Construct an uninitialized sparse matrix builder.
    UT_SparseMatrixBuilderT() : myMatrix(NULL) {}

    /// Construct a sparse matrix builder for the given matrix.
    UT_SparseMatrixBuilderT(SparseMatrixType &matrix) { init(matrix); }

    /// Initializes the builder to start building the given matrix
    void init(SparseMatrixType &matrix);

    /// Sets the minimum capacity of non-zero elements for the given matrix.
    /// This is used to preallocate memory for the matrix to avoid excessive
    /// reallocation as elements are added.
    void setCapacity(exint capacity);

    /// Advances the current row to the given row. The row must be greater than
    /// the current row.
    void startRow(exint row);

    /// Adds the given value at the given column to the current row. If a cell
    /// is already present for the given column, the given value is added to it.
    void addToColumn(exint col, T value);

    /// Completes the remaining rows and finalizes the matrix.
    void finish();

private:
    exint myCurrentRow;
    SparseMatrixType *myMatrix;
};

typedef UT_SparseMatrixBuilderT<fpreal32> UT_SparseMatrixBuilderF;
typedef UT_SparseMatrixBuilderT<fpreal64> UT_SparseMatrixBuilderD;

#endif
