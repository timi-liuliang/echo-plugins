/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU library (C++)
 *
 * COMMENTS:	Tracks state of cells of trimmed patches .
 *
 */

#ifndef __GU_TrimCells_h__
#define __GU_TrimCells_h__

#include "GU_API.h"

class GU_API GU_TrimCells
{
public:
    GU_TrimCells(int udiv, int vdiv);
    ~GU_TrimCells();

    int64 getMemoryUsage(bool inclusive) const;

    void	addCurve(int i, int j) 
			  { if (i >= 0 && i < myVDiv && j >= 0 && j < myUDiv)
				myCellStates[i*myUDiv + j] = 1; }
    char	&operator()(int i, int j)
			  { return myCellStates[i*myUDiv + j]; }
    char	operator()(int i, int j) const 
			  { return myCellStates[i*myUDiv + j]; }
    char	*operator()(int i) const
			  { return &myCellStates[i*myUDiv]; }

    // Returns runlength of cell at i,j.
    int		runLength(int i, int j) const;

    // Do 4 directional fill from i/j replacing with state state. 
    // 0 elements are considered empty.
    void	fill(int i, int j, char state);

private:
    int		 myUDiv, myVDiv;
    char	*myCellStates;
};

#endif
