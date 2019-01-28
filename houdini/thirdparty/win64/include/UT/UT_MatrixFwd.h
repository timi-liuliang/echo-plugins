/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_MatrixFwd.h (UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_MATRIXFWD_H_INCLUDED__
#define __UT_MATRIXFWD_H_INCLUDED__

#include "UT_API.h"
#include <SYS/SYS_Types.h>

template <typename T>
class UT_MatrixT;

typedef UT_MatrixT<fpreal>	    UT_MatrixR;
typedef UT_MatrixT<fpreal32>	    UT_MatrixF;
typedef UT_MatrixT<fpreal64>	    UT_MatrixD;
typedef UT_MatrixT<fpreal64>	    UT_Matrix;  // Match precision w/UT_Vector

#endif // __UT_MATRIXFWD_H_INCLUDED__
