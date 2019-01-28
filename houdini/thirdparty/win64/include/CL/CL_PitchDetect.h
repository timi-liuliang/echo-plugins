/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CL_PitchDetect.h ( Clip Library, C++)
 *
 * COMMENTS:
 *	
 *	Detects the most dominant frequency in an audio sample
 *
 */

#ifndef __CL_PitchDetect__
#define __CL_PitchDetect__

#include "CL_API.h"
#include "CL_SlidingWindow.h"
#include <UT/UT_FFT.h>
#include <UT/UT_Array.h>
#include <SYS/SYS_Types.h>

class cl_PitchNode;

class CL_API CL_PitchDetect : public CL_SlidingWindow
{
public:

		 CL_PitchDetect(int size, fpreal freqres);
    virtual     ~CL_PitchDetect();

	    void	setPitchMode(int multi);

	    void	setMultiParms(fpreal start, fpreal end, int divs,
				      int log, fpreal *results);

	    void	setBandwidth(fpreal low, fpreal high);
	    void	setMinLevel(fpreal min);
	    void	setFloatingBandwidth(int on, fpreal bandwidth);
	    void	setHarmonicCompensation(int on, int correct,fpreal error);
	    void	setSmoothJumps(int on, fpreal minjump);
	    void	setFrequencyHint(int on, fpreal hint, fpreal bandwidth);

	    void	getPitchResult(fpreal &pitch, fpreal &volume);
	    fpreal      *getPitchResult();
    
    virtual void	reset(fpreal val=0.0);
    virtual int		maxFilterSamples();

	    void	clearSamples(int from);
private:

    // override this function to do the operation you want
    virtual void	doWindowOp(int nsamples,fpreal *dest);

    void	selectBand();
    void	selectMultipleBands();
    void	detectHarmonics();
    void	designFilter(int size);

    void	setupPitch();
    void	cleanupPitch();
    
protected:

    fpreal	myMinLevel;
    fpreal	myFreqRes;
    
    
    fpreal	myLowFreq;
    fpreal	myHighFreq;
    
    int		myFloatingBandwidthOn;
    fpreal	myFloatingBandwidth;
    
    int		myHarmonicCompensation;
    fpreal	myHarmonicError;
    int		myHarmonicCorrect;
        
    int		mySmoothJumps;
    fpreal	myMinJumpLevel;
    
    int		myUseHint;
    fpreal	myFrequencyHint;
    fpreal	myHintBandwidth;

    fpreal	myLastFreq;
    fpreal	myPitchFreq;
    fpreal	myPitchVolume;

    int		myMode;
    int		myLogMode;
    int		myDivisions;
    fpreal	*myMultiResults;
    
    fpreal	*myWindow;
    fpreal	*myData;
    fpreal	*myReal;
    fpreal	*myImag;
    fpreal	*myKernel;
    fpreal	*myFiltered;
    int		 myKernelSize;
    UT_FFT<fpreal> *myTransform;

    UT_Array<cl_PitchNode>	myHarmonics;
};

class CL_API cl_PitchNode
{
public:

    cl_PitchNode()
	{ myFrequency = mySingleMag = myMag = 0.0; myNextHarmonic = 0;
	 myPrevHarmonic = 0; myAmAHarmonic = 0; }
    
    cl_PitchNode(fpreal freq, fpreal mag)
	{ myFrequency = freq; mySingleMag = myMag = mag; myNextHarmonic = 0;
	  myAmAHarmonic = 0; myPrevHarmonic = 0; }

    cl_PitchNode & operator=(const cl_PitchNode &);
    int		  operator==(const cl_PitchNode &);
	
    fpreal	  myFrequency;
    fpreal	  myMag;
    fpreal	  mySingleMag;
    cl_PitchNode *myNextHarmonic;
    cl_PitchNode *myPrevHarmonic;
    int		  myAmAHarmonic;
};

#endif
