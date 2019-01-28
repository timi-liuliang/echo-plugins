/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CH_KeyState.h (Channel Library, C++)
 *
 * COMMENTS:
 *
 */

#ifndef __CH_KeyState_h__
#define __CH_KeyState_h__

enum CH_KeyState 
{
    CH_KEY_NONE	           = 0x00,	// not animated
    CH_KEY_ANIMATED        = 0x01,	// has channel
    CH_KEY_ON              = 0x02,	// has key
    CH_KEY_PENDING         = 0x04,	// changes are pending
    CH_KEY_OTHER_LANGUAGE  = 0x08,	// changes are pending

    CH_KEY_ATKEY	= CH_KEY_ANIMATED|CH_KEY_ON,	// at a keyframe
    CH_KEY_OFFKEY	= CH_KEY_ANIMATED,		// animated but no key 
    CH_KEY_AT_PENDING	= CH_KEY_ANIMATED|CH_KEY_PENDING,

    CH_KEY_ATKEY_OTHER_LANGUAGE	= CH_KEY_ATKEY|CH_KEY_OTHER_LANGUAGE,
    CH_KEY_OFFKEY_OTHER_LANGUAGE = CH_KEY_OFFKEY|CH_KEY_OTHER_LANGUAGE,
    CH_KEY_AT_PENDING_OTHER_LANGUAGE = CH_KEY_AT_PENDING|CH_KEY_OTHER_LANGUAGE,

    CH_KEY_NUM_STATES	// sentinel
};

#endif // __CH_KeyState_h__

