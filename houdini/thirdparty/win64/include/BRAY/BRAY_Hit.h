/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	BRAY_Hit.h (RAY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __RAY_Hit__
#define __RAY_Hit__

#include "BRAY_API.h"
#include <UT/UT_SmallArray.h>
#include <UT/UT_Vector3.h>

#include <utility>

class BRAY_Object;

/// Class to store hit information for an intersection
class BRAY_API BRAY_Hit
{
public:
    static const uint64	INVALID_ELEMENT = ((1ull << 62) - 1ull);
    using HitStack = UT_SmallArray<unsigned int, 32>;

    BRAY_Hit()
	: myElement(INVALID_ELEMENT)
	, myT(SYS_FP32_MAX)
    {
    }
    ~BRAY_Hit() = default;

    void	set(HitStack hitstack,
		    exint element,
		    float dist,
		    float uv_u, float uv_v, float uv_w,
		    float Ng_x, float Ng_y, float Ng_z,
		    bool transparent=false,
		    bool backface=false)
    {
	myHitStack = std::move(hitstack);
	myElement = element;
	myTransparent = transparent ? 1 : 0;
	myBackface = backface ? 1 : 0;
	myT = dist;
	myU = uv_u;
	myV = uv_v;
	myW = uv_w;
	myNg.assign(Ng_x, Ng_y, Ng_z);
    }

    const HitStack	&getHitStack() const { return myHitStack; }

    bool	isHit() const { return myHitStack.size() > 0; }

    void	clear(float max=SYS_FP32_MAX)
    {
	myHitStack.clear();
	myT = max;
    }

    exint		 element() const { return myElement; }
    float		 getU() const { return myU; }
    float		 getV() const { return myV; }
    float		 getW() const { return myW; }
    float		 getT() const { return myT; }
    const UT_Vector3	&getNg() const { return myNg; }
    bool		 isTransparent() const { return myTransparent; }
    bool		 isBackface() const { return myBackface; }

    void		 validate() const;

    void		setElement(exint e) { myElement = e; }
    void		setU(float u) { myU = u; }
    void		setV(float v) { myV = v; }
    void		setW(float w) { myW = w; }

private:
    HitStack		 myHitStack;	// from root to leaf
    uint64		 myElement;	// Element in the object
    float		 myU, myV, myW; // Parametric coordinates
    float		 myT;		// Distance along the ray
    UT_Vector3		 myNg;		// Geometric normal
    uint32		 myTransparent:1; // The object is transparent
    uint32		 myBackface:1;	// Whether the hit is a backface
};

#endif

