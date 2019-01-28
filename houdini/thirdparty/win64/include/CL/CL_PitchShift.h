/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CHOP_ParaEq.h ( Channel Operator Library, C++)
 *
 * COMMENTS:
 *
 *     Shifts the pitch of an audio waveform
 */

#ifndef __CL_PitchShift__
#define __CL_PitchShift__

#include "CL_API.h"
#include "CL_SlidingWindow.h"
#include <SYS/SYS_Types.h>

class CL_API CL_PitchShift : public CL_SlidingWindow
{
public:

		 CL_PitchShift(int chunk, int oversample);
    virtual	~CL_PitchShift();

    void	 setPitchShift(fpreal shift);
    void	 setOversample(int oversample);
    void	 setChunkSize(int chunk);
    
    virtual void reset(fpreal val =0.0);
    
private:

    void	 makeBlendCurve();
    void	 pitchShiftUp();
    void	 pitchShiftDown();
    
    virtual void doWindowOp(int nsamples,fpreal *dest);

    int		 myOversample;
    fpreal	 myPitchShift;
    fpreal	*mySingleSample;
    fpreal	*myBlendCurve;
    fpreal	*myData;
};

#endif 
