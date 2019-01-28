/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PRM_KeySetType.h (Parameter Library)
 *
 */

#ifndef __PRM_KeySetType_h__
#define __PRM_KeySetType_h__

#include "PRM_API.h"

// ways of setting values at keyframes:
enum PRM_KeySetType
{
    PRM_USE_PREFERENCES,	 // use the user preferences
    PRM_KEY_FORCE_PENDING,	 // always set values as pending at keyframes
    PRM_KEY_FORCE_COMMIT	 // always commit the value change at keyframes
};
    
#endif
