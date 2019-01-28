/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Utility Library (C++)
 *
 * COMMENTS:
 */

#ifndef __UT_RefMatrixImpl__
#define __UT_RefMatrixImpl__

#ifndef __UT_RefMatrix_h__
#error Do not include this file directly. Include UT_RefMatrix.h instead.
#endif

#include "UT_Assert.h"
#include "UT_Swap.h"

template <class T>
UT_RefMatrix<T>::UT_RefMatrix(unsigned int m, unsigned int n)
    : myRows(0)
    , myCols(0)
{
    resize(m, n);
}

template <class T>
UT_RefMatrix<T>::UT_RefMatrix(const UT_RefMatrix<T> &m)
    : myArray(m.myArray)
    , myRows(m.myRows)
    , myCols(m.myCols)
{
}

template <class T>
UT_RefMatrix<T>::~UT_RefMatrix(void)
{
}

template <class T>
void
UT_RefMatrix<T>::swap(UT_RefMatrix<T> &other)
{
    myArray.swap(other.myArray);
    UTswap(myRows, other.myRows);
    UTswap(myCols, other.myCols);
}

template <class T>
unsigned int
UT_RefMatrix<T>::insertRow(unsigned int index)
{
    if (index > rows())
	myRows = index+1;
    else
	myRows++;
    myArray.entries(myRows*myCols);
    for (int i = rows(); i-- > index+1; )
	for (int j = 0; j < cols(); j++)
	    (*this)(i, j) = (*this)(i-1, j);
    return index;
}

template <class T>
unsigned int
UT_RefMatrix<T>::appendRow(const UT_Array<T> &r)
{
    return insertRow(r, rows());
}

template <class T>
unsigned int
UT_RefMatrix<T>::insertRow(const UT_Array<T> &r, 
			      unsigned int index)
{
    insertRow(index);
    for (int j = 0; j < cols(); j++)
	(*this)(index, j) = r(j);
    return index;
}

template <class T>
unsigned int
UT_RefMatrix<T>::insertCol(unsigned int index)
{
    int	pcols = myCols;
    if (index > cols())
	myCols = index+1;
    else
	myCols++;

    myArray.entries(myRows*myCols);
    for (int i = rows(); i-- > 0; )
    {
	for (int j = cols(); j-- > index+1; )
	    myArray(i*myCols+j) = myArray(i*pcols+j-1);
	for (int j = index; j-- > 0; )
	    myArray(i*myCols+j) = myArray(i*pcols+j);
    }
    return index;
}

template <class T>
unsigned int	
UT_RefMatrix<T>::appendCol(const UT_Array<T> &r)
{
    return insertCol(r, cols());
}

template <class T>
unsigned int	
UT_RefMatrix<T>::insertCol(const UT_Array<T> &r, unsigned int index)
{
    insertCol(index);
    for (int i = 0; i < rows(); i++)
	(*this)(i, index) = r(i);
    return index;
}

template <class T>
int	
UT_RefMatrix<T>::removeRow(unsigned int index)
{
    for (int i = index; i < rows()-1; i++)
    {
	for (int j = 0; j < cols(); j++)
	    (*this)(i, j) = (*this)(i+1, j);
    }
    myRows--;
    myArray.entries(myRows*myCols);
    return index;
}

template <class T>
int	
UT_RefMatrix<T>::removeCol(unsigned int index)
{
    int	ncols = myCols-1;
    for (int i = 0; i < rows(); i++)
    {
	for (int j = 0; j < index; j++)
	    myArray(i*ncols+j) = myArray(i*myCols+j);
	for (int j = index; j < cols()-1; j++)
	    myArray(i*ncols+j) = myArray(i*myCols+j+1);
    }
    myCols = ncols;
    myArray.entries(myRows*myCols);
    return index;
}

template <class T>
void
UT_RefMatrix<T>::cycle(int rowshift, int colshift)
{
    if (rowshift || colshift)
    {
	UT_RefMatrix<T>	tmp(*this);
	int		ic, jc;

	ic = rowshift % rows();
	ic = ic < 0 ? ic+rows() : ic;
	for (int i = 0; i < rows(); i++)
	{
	    jc = colshift % cols();
	    jc = jc < 0 ? jc+cols() : jc;
	    for (int j = 0; j < cols(); j++)
	    {
		(*this)(ic, jc) = tmp(i, j);
		jc++; if (jc >= cols()) jc -= cols();
	    }
	    ic++; if (ic >= rows()) ic -= rows();
	}
    }
}

template <class T>
int		
UT_RefMatrix<T>::find(const T &t, unsigned int *m, 
			                  unsigned int *n) const
{
    for (*m = 0; *m < rows(); (*m)++)
	for (*n = 0; *n < cols(); (*n)++)
	    if ((*this)(*m,*n) == t) return 0;
    return -1;
}

template <class T>
int		
UT_RefMatrix<T>::find(const T &t, unsigned int *m, unsigned int *n,
		    Comparator compare) const
{
    if ((*m = myArray.find(t, compare)) != -1)
    {
	*n = *m % myCols;
	*m = *m / myCols;
	return 0;
    }
    return -1;
}

template <class T>
void	
UT_RefMatrix<T>::resize(unsigned int mSz, unsigned int nSz)
{
    if (mSz == rows() && nSz == cols())
	return;

    UT_RefMatrix<T>	tmp;

    // Avoid a copy
    tmp.swap(*this);

    myRows = mSz;
    myCols = nSz;
    myArray.entries(myRows*myCols);

    for (int i = 0; i < SYSmin(rows(), tmp.rows()); i++)
	for (int j = 0; j < SYSmin(cols(), tmp.cols()); j++)
	    (*this)(i, j) = tmp(i, j);
}

template <class T>
UT_RefMatrix<T>	&
UT_RefMatrix<T>::operator=(const UT_RefMatrix<T> &m)
{
    if (this == &m) return *this;
    myArray = m.myArray;
    myRows = m.myRows;
    myCols = m.myCols;
    return *this;
}

template <class T>
int			
UT_RefMatrix<T>::operator==(const UT_RefMatrix<T> &m) const
{
    if (this == &m) return 1;
    if (!(rows() == m.rows() && cols() == m.cols()))
	return 0;
    return myArray == m.myArray;
}

template <class T>
int		
UT_RefMatrix<T>::apply(int (*applyFct)(T &t, void *d), void *d)
{
    if (myArray.apply(applyFct,d) != myArray.entries()) return 1;
    return 0;
}

#endif // __UT_RefMatrixImpl__
