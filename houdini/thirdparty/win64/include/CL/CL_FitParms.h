/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CL_FitParms.h ( CL Library, C++)
 *
 * COMMENTS:
 *
 *		A container class to hold conversion parameters
 *		between raw and spline fit data
 */

#ifndef __CL_FitParms__
#define __CL_FitParms__

#include "CL_API.h"
#include "CL_Defines.h"
#include <UT/UT_String.h>
#include <SYS/SYS_Types.h>

class UT_IStream;
class UT_OStream;

class CL_API CL_FitParms
{

public:

     CL_FitParms();
    ~CL_FitParms();

    CL_FitParms		&operator= (const CL_FitParms &v);


    CL_Unit		units;
    int			relative;
    fpreal		start;
    fpreal		end;
    fpreal		tolerance;
    UT_String		scope;
    UT_String		status;

    bool		save(UT_OStream &os);
    bool		load(UT_IStream &is);
    int64		getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += scope.getMemoryUsage(false);
        mem += status.getMemoryUsage(false);
        return mem;
    }
};

#endif

