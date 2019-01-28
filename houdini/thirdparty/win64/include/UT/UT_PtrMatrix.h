/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Utility Library (C++)
 *
 * COMMENTS:
 *	This class is a wrapper for UT_RefMatrix provided for backward
 *	compatibility.
 */

#ifndef __UT_PtrMatrix_h__
#define __UT_PtrMatrix_h__

#include "UT_RefMatrix.h"

template <class utPtr>
class UT_PtrMatrix : public UT_RefMatrix<utPtr> {
public:
    // Trivial constructor and destructor:
     UT_PtrMatrix(unsigned mSz = 0, unsigned nSz = 0) : UT_RefMatrix<utPtr>(mSz, nSz) {}

    // Copy constructor that copies each utPtr from the source matrix to
    // this matrix using the '=' operator.
    UT_PtrMatrix(const UT_PtrMatrix<utPtr> &m)
	: UT_RefMatrix<utPtr>(m)
    {
    }
};

#endif
