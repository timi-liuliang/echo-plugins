/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CL_SlidingWindow.h ( Clip Library, C++)
 *
 * COMMENTS:
 *	Implements a queue-style sliding window for continuous windowed
 *	operations
 *
 */


#ifndef __CL_SlidingWindow__
#define __CL_SlidingWindow__

#include "CL_API.h"
#include <SYS/SYS_Types.h>

class UT_IStream;
class UT_OStream;

class CL_API CL_SlidingWindow
{
public:

		 CL_SlidingWindow(int size, int overlap);
		 CL_SlidingWindow(UT_IStream &is);
    virtual     ~CL_SlidingWindow();

    void	 filterSamples(int n,const fpreal *source,fpreal *dest);
    void	 shiftSamples(int nsource, const fpreal *source,
			      int ndest, fpreal *dest, int shift);
    
    void		setOverlap(int overlap);
    void		resize(int size, int preserve = 0);

    virtual void	reset(fpreal val = 0.0);
    virtual int		maxFilterSamples();

    virtual void	save(UT_OStream &os);
    virtual bool	load(UT_IStream &is, int version);

private:

    // override this function to do the operation you want
    virtual void	doWindowOp(int nsamples,fpreal *dest);
    virtual void	doShiftOp(int nsamples,const fpreal *source,int nshift);

protected:
    int		 myWindowSize;
    int		 myOverlap;
    int		 myLastFilteredSample;
    
    fpreal	*myPreviousData;
    fpreal      *myMirrorData;
};

#endif
