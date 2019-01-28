/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Matrix of arbitrary size (C++)
 *
 * COMMENTS:	From Numerical Recipes
 * 	See Matrix Computations, Gene H. Golub, Charles F. Van Loan
 *	Rewritten without row array by Jeff Lait
 *
 */

#ifndef	__UT_Matrix_H__
#define	__UT_Matrix_H__

#include "UT_API.h"
#include "UT_Assert.h"
#include "UT_Matrix2.h"
#include "UT_Vector.h"
#include "UT_VectorTypes.h"
#include <SYS/SYS_Types.h>
#include <stdlib.h>
#include <iosfwd>


template <typename T>
class UT_API UT_MatrixT
{
public:

    typedef T	value_type;

     UT_MatrixT();
     
     // Input the row and column index range [nrl..nrh][ncl..nch].
     // The "standard" way to set up a matrix of R rows and C columns
     // would be:
     // UT_MatrixF		foo(1, R, 1, C);
     
     UT_MatrixT(int nrl, int nrh, int ncl, int nch);
     UT_MatrixT(int nrl, int nrh, int ncl, int nch, T *array);
     UT_MatrixT(int nrl, int ncl, UT_MatrixT &mat);
     explicit UT_MatrixT(const UT_MatrixT<T> &mat);	// deep copy constructor
    ~UT_MatrixT();

    // Allocate space.  Resulting memory is not initialized.
    void	 init(int nrl, int nrh, int ncl, int nch);

    // Enlarge/reduce matrix to have a new number of rows and
    // columns.  NRL & NCL are unchanged.  Afterwards, the
    // matrix always owns its data!
    void	 resize(int nrows, int ncols);

    // Enlarge by one row/column and copy given data in (if given, otherwise
    // filled in with zeroes
    void 	 appendCol(T *new_col=0);
    void 	 appendRow(T *new_row=0);

    // insert new row of zeros (or nw_row, if given) at the specified row idx
    //
    // this has been tested and it works, we just don't use it anywhere.
    // uncomment it if you need it.
    // void 	 insertRowAt(int at, T *new_row=0);

    int		 isInit() const { return myMatrix ? 1 : 0; }

    // Steal from another matrix or array
    // Resulting matrix has origin 1, 1.
    void	 submatrix(const UT_MatrixT<T> &A, 
			    int nrl, int nrh, int ncl, int nch);
    // &(*this)(nrl, ncl) == array.  If array is wider than
    // nch - ncl + 1, stride should be set.
    void	 submatrix(T *array, 
			    int nrl, int nrh, int ncl, int nch, int stride=-1);

    // Sets to zeros.  The version with a range will set a sub-block
    // to zero.
    void	 zero(int nrl, int nrh, int ncl, int nch);
    void	 zero()
		    { zero(myNRL, myNRH, myNCL, myNCH); }

    /// These methods allow one to read out and write into blocks of
    /// the UT_Matrix using our other matrix classes.  This is very useful
    /// when working with block algorithms.
    /// Keep in mind that the usual UT_MatrixF? methods are 0 based while
    /// this class is usually 1 based.
    /// Only double matrices are supported as they are the preferred
    /// matrix format.
    void	 getSubmatrix2(UT_Matrix2T<T> &mat, int row, int col) const;
    void	 setSubmatrix2(int row, int col, const UT_Matrix2T<T> &mat);
    void	 addSubmatrix2(int row, int col, const UT_Matrix2T<T> &mat);

    void	 getSubmatrix3(UT_Matrix3T<T> &mat, int row, int col) const;
    void	 setSubmatrix3(int row, int col, const UT_Matrix3T<T> &mat);
    void	 addSubmatrix3(int row, int col, const UT_Matrix3T<T> &mat);

    void	 getSubmatrix4(UT_Matrix4T<T> &mat, int row, int col) const;
    void	 setSubmatrix4(int row, int col, const UT_Matrix4T<T> &mat);
    void	 addSubmatrix4(int row, int col, const UT_Matrix4T<T> &mat);

    // Make identity:
    void	 makeIdentity();

    // Negate the matrix.
    void	 negate();

    // Get the row low index.
    int	 	 getNRL() const { return myNRL; }

    // Get the row high index.
    int	 	 getNRH() const { return myNRH; }

    // Get the column low index.
    int 	 getNCL() const { return myNCL; }

    // Get the column high index.
    int	 	 getNCH() const { return myNCH; }

    // Get number of rows
    int		 rows() const { return myNRH - myNRL + 1; }

    // Get number of columns
    int		 columns() const { return myNCH - myNCL + 1; }

    // Change the row and column low indices, 
    // and high indices will adjust themselves.
    void	 changeNRLAndNCL(int nrl, int ncl);

    // Change the indices in a very shallow manner, i.e. simply change
    // the index without shifting the data. This is highly dangerous, yet
    // very useful if used with care.
    void	 setShallowNRL(int nrl)	{ myNRL = nrl; }
    void	 setShallowNRH(int nrh)	{ myNRH = nrh; }
    void	 setShallowNCL(int ncl)	{ myNCL = ncl; }
    void	 setShallowNCH(int nch)	{ myNCH = nch; }

    // Retrieve data
    T		&operator()(int row, int col)
		 {
		     UT_ASSERT_P(row >= myNRL && row <= myNRH);
		     UT_ASSERT_P(col >= myNCL && col <= myNCH);
		     return myMatrix[row*myStride + col];
		 }
    T		 operator()(int row, int col) const
		 {
		     UT_ASSERT_P(row >= myNRL && row <= myNRH);
		     UT_ASSERT_P(col >= myNCL && col <= myNCH);
		     return myMatrix[row*myStride + col];
		 }

    UT_MatrixT<T>	&operator=(const UT_MatrixT<T> &m);

    // Component wise operators
    UT_MatrixT<T>	&operator+=(const UT_MatrixT<T> &m);
    UT_MatrixT<T>	&operator-=(const UT_MatrixT<T> &m);

    // left multiply this matrix[1..m][1..n] by the given matrix
    // A[1..p][1..m] and puts the product in 'result[1..p][1..n]'
    // NOTE: this, A, and result must all have the same lower index
    //       for rows and columns
    void	 preMult(const UT_MatrixT<T> &A, UT_MatrixT<T> &result) const;

    // right multiply this matrix[1..m][1..n] by the given matrix
    // A[1..n][1..p] and puts the product in 'result[1..m][1..p]'
    // NOTE: this, A, and result must all have the same lower index
    //       for rows and columns
    void	 postMult(const UT_MatrixT<T> &A, UT_MatrixT<T> &result) const;

    // Vector multiplication, before and after.
    // result = xT * this
    template <typename S>
    void	 preMult(const UT_VectorT<S> &x, UT_VectorT<S> &result) const;
    // result = this * x
    template <typename S>
    void	 postMult(const UT_VectorT<S> &x, UT_VectorT<S> &result) const
				    { multVec(x, result); }

    // Fills the upper-triangular portion of this matrix with A^T . A
    void	 upperNormalUpdate(const UT_MatrixT<T> &A);

    // Scaled outer product update.
    // this += b * x * yT
    template <typename S>
    void	 outerproductUpdate(T b, 
			    const UT_VectorT<S> &x, const UT_VectorT<S> &y);

    // Add another matrix scaled appropriately:
    void	 addScaledMatrix(const UT_MatrixT<T> &A, T scale);
    // this = A*scale
    void	 setAndScale(const UT_MatrixT<T> &A, T scale);

    // Givens functions: do a pre or post multiplication with
    // the givens matrix.
    void	 preMultGivensInPlace(T c, T s);
    void	 postMultGivensInPlace(T c, T s);
    
    template <typename S>
    void 	 multVec(const UT_VectorT<S> &x, UT_VectorT<S> &result) const;

    // Calculate the L2-distance (Euclidean) between two rows for columns [cl..ch].
    T		 rowsL2dist(int r1, int r2, int cl=-1, int ch=-1) const;

    // Norm calculations.
    T		 normFrobenius() const;
    T		 norm1() const;
    T		 normInfinite() const;

    // place this matrix's transpose in the given result matrix
    void	 transpose( UT_MatrixT<T> &result ) const;

    // Returns true if matrix is symmetric within given tolerance
    bool	 isSymmetric(T tolerance = SYS_FTOLERANCE_D) const;

    // Get the pointer to a specific row.
    // Note real data starts at row[myNCL]
    T		*row(int i) const
		 {
		     UT_ASSERT_P(i >= myNRL && i <= myNRH);
		     return &myMatrix[i * myStride];
		 }

    void	 clearAndDestroy();

    int 	 save(std::ostream &os, int binary) const;

    // I/O friends
    friend std::ostream	&operator<<(std::ostream &os, const UT_MatrixT<T> &m )
			{
			    m.outTo(os);
			    return os;
			}

    int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        if (myOwnData && myMatrix)
            mem += (myNRH - myNRL + 1)*(myNCH - myNCL + 1)*sizeof(*myMatrix);
        return mem;
    }

protected:
    // I/O methods
    void	 outTo( std::ostream &os ) const;

private:
    int	 	 myNRL, myNRH, myNCL, myNCH;
    T		*myMatrix;
    int		 myStride, myOffset;
    char	 myOwnData;
};

typedef UT_MatrixT<fpreal>	UT_MatrixR;
typedef UT_MatrixT<fpreal32>	UT_MatrixF;
typedef UT_MatrixT<fpreal64>	UT_MatrixD;
typedef UT_MatrixT<fpreal64>	UT_Matrix;  // Match precision w/UT_Vector

// Overload for custom formatting of UT_MatrixT<T> with UTformat.
template<typename T>
UT_API size_t format(char *buffer, size_t buffer_size, const UT_MatrixT<T> &v);

#endif
