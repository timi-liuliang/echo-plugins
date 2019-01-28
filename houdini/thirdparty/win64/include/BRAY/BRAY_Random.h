/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	BRAY_Random.h (BRAY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __BRAY_Random__
#define __BRAY_Random__

#include "BRAY_API.h"
#include <UT/UT_Vector2.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_Vector4.h>
#include <UT/UT_NonCopyable.h>

class BRAY_API BRAY_Random
    : public UT_NonCopyable
{
public:
    BRAY_Random(int64 id=0);

    void	copyFrom(const BRAY_Random &src)
    {
	myId = src.myId;	// Keep track of source
	mySeed = SYSwang_inthash(src.mySeed);	// New seed
    }

    void	setSeed(int64 id);

    int64	sampleId() const { return myId; }

    float	randomF() { return random(); }
    UT_Vector2	random2()
    {
	float x = random();
	float y = random();
	return UT_Vector2(x, y);
    }
    UT_Vector3	random3()
    {
	float x = random();
	float y = random();
	float z = random();
	return UT_Vector3(x, y, z);
    }
    UT_Vector4	random4()
    {
	float x = random();
	float y = random();
	float z = random();
	float w = random();
	return UT_Vector4(x, y, z, w);
    }
private:
    float	random()
    {
	return SYSfastRandom(mySeed);
    }
    int64	myId;
    uint	mySeed;
};

#endif
