/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_CameraParms.h (C++)
 *
 * COMMENTS:	The GU_CameraParms class is a class for holding camera
 *		information for generating perspective texture coordinates.
 */

#ifndef __GU_CameraParms_h__
#define __GU_CameraParms_h__

#include "GU_API.h"
#include <UT/UT_Matrix4.h>

class GU_API GU_CameraParms
{
public:

     GU_CameraParms();
    ~GU_CameraParms();

    UT_Matrix4		toTexture;
    float		minPlane;
    float		maxPlane;
};

#endif
