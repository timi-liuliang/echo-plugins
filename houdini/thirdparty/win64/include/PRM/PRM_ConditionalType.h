/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PRM_ConditionalType.h ( PRM Library, C++)
 */

#ifndef __PRM_ConditionalType__
#define __PRM_ConditionalType__

enum PRM_ConditionalType
{
    PRM_CONDTYPE_DISABLE = 0,	// disable_when
    PRM_CONDTYPE_HIDE,		// hide_when
    PRM_CONDTYPE_NOCOOK,	// nocook_when
    
    NB_PRM_CONDTYPES		// Must be the last.
};

#endif
