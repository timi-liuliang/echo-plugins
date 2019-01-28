/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CL_Filter.h ( Clip Library, C++)
 *
 * COMMENTS:
 *	Does continuous digital filtering
 *
 */


#ifndef __CL_Filter__
#define __CL_Filter__

#include "CL_API.h"
#include "CL_SlidingWindow.h"
#include <UT/UT_FFT.h>
#include <SYS/SYS_Types.h>

class UT_IStream;

class CL_API CL_Filter : public CL_SlidingWindow
{
public:

			 CL_Filter(int size, int overlap);
			 CL_Filter(UT_IStream &is);
    virtual		~CL_Filter();

    // must be 'size' floats long.
    void		setFilter(fpreal *filter,fpreal *phase = 0);
    
    virtual void	reset(fpreal val = 0.0);
    virtual int		maxFilterSamples();

private:
    virtual void doWindowOp(int nsamples, fpreal *dest);

    // Should only be called from the constructor
    void	initialize();

    void	window(fpreal *windower);
    void	filter();
    void	computeWindow();

    fpreal	 myLastDataSample;
    fpreal	 myLastSlope;
    
    fpreal	*myData;
    fpreal	*myReal, * myImag;
    fpreal	*myFilter;
    fpreal	*myPhaseFilter;

    fpreal	*myWindow;
    fpreal	*myUnWindow;

    UT_FFT<fpreal> *myTransform;
    
};

#endif
