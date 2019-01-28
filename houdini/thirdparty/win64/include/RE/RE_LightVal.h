/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_LightVal.h (RE Library, C++)
 *
 * COMMENTS:
 */

#ifndef __RE_LightVal__
#define __RE_LightVal__

#include "RE_API.h"

class RE_Light;

class RE_API RE_LightVal
{
public:
    typedef enum {
	SPOT_LIGHT,
	INFINITE_LIGHT,
	AMBIENT_LIGHT
    } LightType;

    float	position[4];
    float	ambient[4];
    float	diffuse[4];
    float	specular[4];
    float	spotdirection[3];
    float	spotcutoff;
    float	spotexponent;
    float	atten[3];		// Const, Linear, Quadratic attenuation
    LightType	light_type;
    int		gl_index;
    unsigned	is_on:1,
		is_diffuse:1,
		is_specular:1,
		is_ambient_shaded:1;

    RE_Light   *myLight;

protected:
    // these are not implemented to prevent them from being used
    int		 operator==(const RE_LightVal &);
};

#endif // __RE_LightVal__
