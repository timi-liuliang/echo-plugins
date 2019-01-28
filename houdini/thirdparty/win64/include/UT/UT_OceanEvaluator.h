/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_OceanEvaluator.h ( UT Library, C++)
 *
 * COMMENTS:  Functions for evaluating ocean spectra.
 */

#ifndef __UT_OCEANEVALUATOR_H
#define __UT_OCEANEVALUATOR_H

#include "UT_API.h"
#include "UT_Matrix4.h"
#include "UT_Vector3.h"
#include "UT_VoxelArray.h"
#include "UT_ThreadedAlgorithm.h"

class UT_API UT_OceanEvaluator
{
public:

    UT_OceanEvaluator() {}

    virtual ~UT_OceanEvaluator();

    bool build(const UT_VoxelArrayF &phase,
	       const UT_VoxelArrayF &freq,
	       const UT_VoxelArrayF &amp,
	       const UT_Matrix4F &xform,
	       float hscale, float time,
	       int mode, int downsample);

    UT_Vector3 getValue(int x, int y, int z) const;

    UT_Vector3 getValue(const UT_Vector3 &pos) const;

private:
    THREADED_METHOD9(UT_OceanEvaluator, phase.numTiles() > 16,
		     computeCoefficients,
		     const UT_VoxelArrayF &, phase,
		     const UT_VoxelArrayF &, freq,
		     const UT_VoxelArrayF &, amp,
		     float, gridsize, float, chop, float, time,
		     UT_VoxelArrayF &, Ximag,
		     UT_VoxelArrayF &, Yimag,
		     UT_VoxelArrayF &, Zimag);
    void computeCoefficientsPartial(const UT_VoxelArrayF &phase,
			     const UT_VoxelArrayF &freq,
			     const UT_VoxelArrayF &amp,
			     float gridsize, float chop, float time,
			     UT_VoxelArrayF &Ximag,
			     UT_VoxelArrayF &Yimag,
			     UT_VoxelArrayF &Zimag,
			     const UT_JobInfo &info);

    UT_VoxelArrayF	myX, myY, myZ;
    UT_Matrix4F		myInvXform;
    int			myMode;
};

#endif
