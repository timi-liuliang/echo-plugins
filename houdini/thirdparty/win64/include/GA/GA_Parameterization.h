/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_Parameterization.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_Parameterization__
#define __GA_Parameterization__

#include "GA_API.h"

#include <UT/UT_Matrix.h>
#include <SYS/SYS_Types.h>


template<typename T> class UT_VectorT;
typedef UT_VectorT<fpreal64> UT_Vector;

class GA_API GA_Parameterization 
{
public:
    static bool	equalSpace(const UT_MatrixF &data_points,
			    bool wrapped,
			    UT_Vector &param,
			    int end_dimension=-1);
    static bool	chordLength(const UT_MatrixF &data_points,
			    bool wrapped,
			    UT_Vector &param,
			    int end_dimension=-1);
    static bool	centripetal(const UT_MatrixF &data_points,
			    bool wrapped,
			    UT_Vector &param,
			    int end_dimension=-1);
    static bool	approxArcLength(const UT_MatrixF &data_points,
			    bool wrapped,
			    UT_Vector &param,
			    int end_dimension=-1);

    static int	findClosest(const UT_Vector &parm,
			    fpreal val,
			    int &index,
			    int start_index,
			    int end_index);
};

#endif

