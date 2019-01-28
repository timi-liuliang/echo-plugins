/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CL_Despike.h ( Clip Library, C++)
 *
 * COMMENTS:
 *	Does continuous despiking of data
 *
 */


#ifndef __CL_Despike__
#define __CL_Despike__

#include "CL_API.h"
#include "CL_SlidingWindow.h"
#include <SYS/SYS_Types.h>

class UT_OStream;

class CL_API CL_Despike : public CL_SlidingWindow
{
public:

			CL_Despike(int size,fpreal last);
    virtual		~CL_Despike();

    // must be 'size' floats long.
    void		setDespikeParms(int size,fpreal tolerance,fpreal effect);
    
    virtual void	reset(fpreal val =0.0);

    virtual void	save(UT_OStream &os);
    virtual bool	load(UT_IStream &is, int version);

private:
    virtual void doShiftOp(int nsamples, const fpreal *source, int ndest);

    fpreal	      mySpikeSlope;

    fpreal	      myTolerance;
    fpreal	      myEffect;
};

#endif
