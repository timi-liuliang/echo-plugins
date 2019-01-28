/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CL_CycleDetect.h ( Clip Library, C++)
 *
 * COMMENTS:
 *
 *	Detects the most likely period of a repeating but not quite periodic
 *	waveform.
 */

#ifndef __CL_CycleDetect__
#define __CL_CycleDetect__

#include "CL_API.h"
#include <SYS/SYS_Types.h>

class CL_API CL_CycleDetect
{
public:

	CL_CycleDetect(int mincyclelength = 30, int maxcyclelength = 200,
		       int cyclestouse = 2);
	~CL_CycleDetect();

    // returns 1 if a cycle could be found, 0 otherwise.
    // start/end set to the start and end of one period of the waveform.
    // start/end should initially be the start and end of the array data (0-n)
    int		determineCycle(const fpreal *data,int &start, int &end,
			       int straightcompare = 0);

private:

    void	trimAwaySilence(const fpreal *data,int &start, int &end);
    void	findFlattestRegion(const fpreal *data,int &start, int &end);
    int		findCycle(const fpreal *data, int &start,int &end);
    void	halfCheck(const fpreal *data, int &start, int &end,fpreal prev);
    
    int	myMaxCycle;
    int	myMinCycle;
    int myNumCycles;
    
};

#endif
