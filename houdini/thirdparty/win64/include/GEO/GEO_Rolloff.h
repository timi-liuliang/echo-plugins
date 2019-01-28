/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO Library (C++)
 *
 * COMMENTS:  Rolloff class for proportional modeling
 *             
 */

#ifndef __GEO_Rolloff_h__
#define __GEO_Rolloff_h__

#include "GEO_API.h"
// This must match PRM_Shared::prmSoftTypes in order...

enum GEO_RolloffType
{
    GEO_ROLLOFF_LINEAR,
    GEO_ROLLOFF_QUADRATIC,
    GEO_ROLLOFF_SMOOTH,
    GEO_ROLLOFF_META,
    GEO_ROLLOFF_INVALID
};

class TS_MetaKernel;


class GEO_API GEO_Rolloff
{
public:
    GEO_Rolloff(GEO_RolloffType t = GEO_ROLLOFF_SMOOTH,
		float r = 1.0f, bool c = true, 
		float tdeg0 = 0.0f, float tdeg1 = 0.0f,
		TS_MetaKernel *kernel = 0);

      // Not all rolloff functions require a fsqrted distance, so
      // avoid it if necessary.
      float	apply(float dist2, bool normalize = true) const;

      void	radius(float r)		{ 
					  myRadius  = (r >= 0.0f ? r : 0.0f); 
					  myRadius2 = myRadius * myRadius;
					}
      float	radius(void) const	{ return myRadius; }
      float	radius2(void) const	{ return myRadius2; }

      void	connected(bool c)	{ myConnected = c;    }
      bool	connected(void) const	{ return myConnected; }

      void	type(GEO_RolloffType t)	{ myType = t; }


private:
    GEO_RolloffType	myType;
    float		myRadius; // normalized 
    float		myRadius2;  // myRadius squared cached for speed
    bool		myConnected;

    float		myTanRad0;	// Angle of tangent slope
    float		myTanRad1;	
    TS_MetaKernel	*myKernel;	// My meta kernel
};

#endif
