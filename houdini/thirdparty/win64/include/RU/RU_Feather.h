/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RU_Feather.h
 *
 * COMMENTS:
 *	Does a feather on a matte.
 */
#ifndef RU_FEATHER_H
#define RU_FEATHER_H

#include "RU_API.h"
#include <PXL/PXL_LumFuncs.h>

#include "RU_Algorithm.h"

enum RU_FeatherType
{
    RU_FEATHER_EXPAND =0,
    RU_FEATHER_SHRINK,
    RU_FEATHER_FILL_HOLES,
    RU_FEATHER_REMOVE_SPECKS
};
	
enum RU_FeatherDropoff
{
    RU_FEATHER_LINEAR =0,
    RU_FEATHER_GAUSSIAN,
    RU_FEATHER_EASE_IN,
    RU_FEATHER_EASE_OUT,
    RU_FEATHER_HALF_COS,
    RU_FEATHER_RAMP		// for RU_PolySurface only, currently.
};
	

class RU_API RU_Feather : public RU_Algorithm
{
public:
		 RU_Feather();
    virtual	~RU_Feather();

    virtual int  initialize(float );

    DECLARE_FILTER_OP(RU_FeatherOp)

    void	 setLUTVal(int x, int y, float v);
    float	 getLUTVal(int x, int y) const;
    
    float	 evalLUT(float x, float y) const;

    // determines whether to expand off the luminance or per-component
    bool		myUseLuma;
    PXL_LumFunction	myLumFunc;

    // operation.
    RU_FeatherType	myFeatherType;
    
    // Parameters for expand/shink
    float		myExpandSize;
    float		myExpandYSize;
    
    float		myFeatherSize;
    float		myFeatherYSize;
    RU_FeatherDropoff	myFeatherDropoff;
    int			mySmoothLevel;
    bool		myFeatherFast;

    // parameters for fill holes
    float		myHoleThreshold;

    // treat these as private:
    float		*myFeatherLUT;
    int			 myXSize;
    int			 myYSize;

    // mask - must be the same size as the data passed in.
    float		*myMask;
};


#endif
