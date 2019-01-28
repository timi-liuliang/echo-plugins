/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PRM_ChanState.h (Parameter Library)
 *
 * COMMENTS:
 */

#ifndef __PRM_ChanState__
#define __PRM_ChanState__

enum PRM_ChanState
{
    PRM_NO_KEYS = 0,		// not animated
    PRM_AT_KEY = 0x01,		// either at a keyframe, or there is no channel
    PRM_OFF_KEY = 0x02,		// a channel exists and curr time not at keyfr
    PRM_CHANGE_PENDING = 0x04	// like above, but user has attempted val change
};

#endif
