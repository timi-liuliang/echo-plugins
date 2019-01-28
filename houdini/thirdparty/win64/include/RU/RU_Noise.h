/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RU_Noise.h
 *
 * COMMENTS:
 *	Creates noise.
 */
#ifndef RU_NOISE_H
#define RU_NOISE_H

#include "RU_API.h"
#include <UT/UT_Noise.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_Vector4.h>

#include "RU_Algorithm.h"

class TIL_TileList;


class RU_API RU_Noise : public RU_Algorithm
{
public:
		 RU_Noise();
    virtual	~RU_Noise();

    virtual int	initialize(float t);

    DECLARE_GENERATOR_OP(RU_NoiseOp)
    
    // Parameters.
    int				mySeed;
    UT_Noise::UT_NoiseType	myNoiseType;
    int				myTurb;
    float			myRough;
    float			myExp;
    UT_Vector4			myAmp;
    UT_Vector4			myShift;

    UT_Vector3			myFreq;
    UT_Vector3			myOffset1;
    UT_Vector3			myOffset2;

    float			myScaleX;
    float			myScaleY;

    int				 myStride;
    int				 myHeight;
    UT_Noise			*myNoiseFunc;
    bool			 myPerComp;
    bool			myRandomNoiseFlag;
};


#endif
