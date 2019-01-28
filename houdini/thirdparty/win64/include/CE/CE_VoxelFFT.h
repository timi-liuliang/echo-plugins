/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME: CE_VoxelFFT.h ( CE Library, C++)
 *
 * COMMENTS: Compute Engine FFT.
 */

#ifndef __CE_VoxelFFT__
#define __CE_VoxelFFT__

#include "CE_API.h"
#include <UT/UT_VoxelFFT.h>
#ifdef CE_ENABLED

class ce_clfft;

class CE_API CE_VoxelFFT: public UT_VoxelFFT
{
private:
    ce_clfft *myFFT;

public:
    CE_VoxelFFT();
    virtual ~CE_VoxelFFT();

protected:
    // Override to create ce_clfft to actually compute the fft.
    virtual void fftSequence(const UT_ValArray<UT_VoxelArrayF *> &realvox,
			    const UT_ValArray<UT_VoxelArrayF *> &imagvox,
			    int begin, int end,
			    bool invert, bool shift,
			    bool normalize, int sliceaxis, bool realdata) override;

};

#endif
#endif
