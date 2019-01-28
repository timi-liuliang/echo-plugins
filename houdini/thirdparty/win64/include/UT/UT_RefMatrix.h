/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Utility Library (C++)
 *
 * COMMENTS:
 *	This is a class template that implements a resizable matrix of 
 *      arbitrary objects. The template parameter represents the type
 *	of object, and is called utRef. You can instantiate this class
 *	with any object or pointer, such as:
 *		UT_RefMatrix<int>		iObj;
 *		UT_RefMatrix<GeoPrimMesh>	mObj;
 *		UT_RefMatrix<GeoPoint*>	pObj; ... etc
 *	The "Ref" in the class name stands for "passed by reference", which 
 *	is the argument passing strategy used by this class. Use this class
 *	if you want to construct 2D arrays of arbitrary objects.
 */

#ifndef __UT_RefMatrix_h__
#define __UT_RefMatrix_h__

#include "UT_Array.h"


template <typename T>
class UT_RefMatrix {
public:
    typedef int (*Comparator)(const T *, const T *);

    // Trivial constructor and destructor:
    explicit UT_RefMatrix(unsigned int mSz = 0, unsigned int nSz = 0);
    ~UT_RefMatrix(void);

    int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += myArray.getMemoryUsage(false);
        return mem;
    }

    // Copy constructor that copies each T from the source matrix to
    // this matrix using the '=' operator.
    UT_RefMatrix(const UT_RefMatrix<T> &m);

    void		swap(UT_RefMatrix<T> &other);

    // Append a row or insert it at a given index. The matrix is grown to 
    // accommodate it.  Return the index of the row that was added.
    unsigned int	insertRow(unsigned int index); 
    unsigned int	insertRow(const UT_Array<T> &r, unsigned int index);
    unsigned int	appendRow(const UT_Array<T> &r);

    // Append a column or insert it at a given index. The matrix is grown to 
    // accommodate it.  Return the index of the column that was added.
    unsigned int	insertCol(unsigned int index); 
    unsigned int	insertCol(const UT_Array<T> &c, unsigned int index);
    unsigned int	appendCol(const UT_Array<T> &c);

    // Remove a row or column from the matrix
    int			removeRow(unsigned int index);
    int			removeCol(unsigned int index);

    // Performs a cyclical shift on the rows and columns
    void		cycle(int rowshift, int colshift);

    // Compare the two matrices: to return 1, they should have the same 
    // number or entries, and their Things should all match when compared
    // with '==' or compare() respectively.
    // t1 and t2 must be pointers to T.
    int			operator==(const UT_RefMatrix<T> &m) const;

    // Assignment operator that copies the array piece by piece using the
    // '=' operator defined in UT_Array. The row and column size of this
    // matrix must be greater or equal to the "used" row and column count of
    // matrix m.
    UT_RefMatrix<T>	&operator=(const UT_RefMatrix<T> &m);

    // Return an element of the matrix, WITHOUT any BOUND CHECKING. Use 
    // safeGet() if you don't know whether you are within range or not.
    const T			&operator()(unsigned i, unsigned j) const
				{
				    return myArray(i*myCols+j);
				}
    T				&operator()(unsigned i, unsigned j) 
				{
				    return myArray(i*myCols+j);
				}

    // Return an element of the matrix, growing the matrix if necessary in
    // the non-const case.
    const T			safeGet(unsigned i, unsigned j) const
				{
				    return (i < rows() && j < cols()) ?
					(*this)(i,j) : T();
				}
    T				&safeGet(unsigned i, unsigned j) 
				{
				    if (i >= rows()) insertRow(i);
				    if (j >= cols()) insertCol(j);
				    return (*this)(i,j);
				}

    // Search for t in each row in one of two ways: linearly using the '==' 
    // operator, or binary using the function specified in the parameter list 
    // respectively. find() returns 0 and the indices of the matching element,
    // or -1 if not found. t1 and t2 must be pointers to T.
    int		find(const T &t, unsigned int *m, unsigned int *n) const;
    int		find(const T &t, unsigned int *m, unsigned int *n,
		     Comparator compare) const;

    // Return the number of rows and column space has been allocated for, and
    // the number of entries used in the rows and columns.
    unsigned int	rows(void) const	{ return myRows; }
    unsigned int	cols(void) const	{ return myCols; }

    // Resize the array.  This function will change the number of rows or
    // columns in the matrix.
    void		resize(unsigned int mSz, unsigned int nSz);

    // Apply a user-defined function to each element of the matrix 
    // as int as the function returns zero. If applyFct returns
    // 1, apply() stops traversing the list and returns 1; otherwise, apply()
    // returns 0;
    int			apply(int (*applyFct)(T &t, void *d), void *d);

private:
    // Array of rows, where each row is a reference array of type T:
    UT_Array<T>	myArray;
    unsigned int	myRows;
    unsigned int	myCols;
};

#if defined(WIN32) || defined(LINUX) || defined(MBSD) || defined(GAMEOS)
    #include "UT_RefMatrixImpl.h"
#endif


#endif
