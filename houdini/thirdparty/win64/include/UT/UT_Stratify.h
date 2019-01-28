/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Stratify.h ( UT Library, C++)
 *
 * COMMENTS:	Generate stratified samples over a grid.  The samples may be
 *		biased in either the u or the v direction if desired.
 *		This class isn't rocket science.
 */

#ifndef __UT_Stratify__
#define __UT_Stratify__

#include "UT_API.h"
#include <SYS/SYS_Types.h>

class UT_API UT_Stratify {
public:
     UT_Stratify()	{}
    ~UT_Stratify()	{}

    // Factor total_samples into a usize and vsize.  usize*vsize will 
    // be less than or equal to total_samples and will be weighted in 
    // the u direction by uweight.  If exact is set to true, we will ensure
    // that usize*vsize == total_samples.
    static void	weightedFactor(unsigned int total_samples, fpreal uweight,
			       int &usize, int &vsize, bool exact = false);

    // Initialize so that N samples will be generated.  The samples generated
    // will be between 0 and (1-inc) where inc is based on the number of
    // samples.  With a u-weight of 1, the samples will be uniformly
    // distributed in the u and v directions.  With a u weight of 2, there will
    // be twice as many samples in the u direction.  Since it's impossible to
    // generate a perfect number of samples (i.e. not all numbers are nice
    // composites), there will be a number of un-stratified samples generated
    // as well.
    void	init(unsigned int total_samples, fpreal uweight=1);

    //
    // Reset the stratification
    void	reset()
		{
		    myU = myV = 0;
		    myDoneRegular = false;
		}
    //
    // Move on to the next sample
    void	nextSample()
		{
		    if (!myDoneRegular)
		    {
			myU++;
			if (myU == myUSize)
			{
			    myV++;
			    if (myV == myVSize)
				myDoneRegular = true;
			    else myU = 0;
			}
		    }
		}
    //
    // Set an arbitrary sample
    void	setSample(unsigned int sample_number)
		{
		    // s = usize*row + col
		    // col = s % usize
		    // row = s / vszie
		    myV = sample_number / myUSize;
		    myU = sample_number - myV*myUSize;
		    myDoneRegular = myU >= myUSize || myV >= myVSize;
		}

    //
    // As input to the stratify method, the variables u and v should be
    // initialized with (random) numbers between 0 and 1.
    void	stratify(fpreal32 &u, fpreal32 &v)
		{
		    if (!myDoneRegular)
		    {
			u = (u + (fpreal)myU) * myUInc;
			v = (v + (fpreal)myV) * myVInc;
		    }
		}
    void	stratify(fpreal64 &u, fpreal64 &v)
		{
		    if (!myDoneRegular)
		    {
			u = (u + (fpreal)myU) * myUInc;
			v = (v + (fpreal)myV) * myVInc;
		    }
		}
    int		getUSize() const		{ return myUSize; }
    int		getVSize() const		{ return myVSize; }
    fpreal	getUInc() const			{ return myUInc; }
    fpreal	getVInc() const			{ return myVInc; }
    int		getStratifiedSize() const	{ return myUSize*myVSize; }
    int		isStratifiedSample() const	{ return myDoneRegular; }

private:
    fpreal	myUInc, myVInc;
    int		myU, myV;
    int		myUSize, myVSize;
    bool	myDoneRegular;
};

#endif

