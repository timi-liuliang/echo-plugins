/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Intercept.h ( UT Library, C++)
 *
 * COMMENTS:
 *
 *	This class is used to intercept and reroute
 *	input events to interested listeners.
 *	
 */

#ifndef __UT_Intercept__
#define __UT_Intercept__

#include "UT_API.h"
#include "UT_ValArray.h"


class UT_InterceptEvent;

typedef int (*UT_InterceptCallback)(int handle_idx, void *data, 
				    UT_InterceptEvent *event,
				    int poll, short xres, short yres);


#define MAX_KEYS	47

typedef enum 
{
    UT_KEY_INVALID,
    UT_KEY_ZERO,
    UT_KEY_ONE,
    UT_KEY_TWO,
    UT_KEY_THREE,
    UT_KEY_FOUR,
    UT_KEY_FIVE,
    UT_KEY_SIX,
    UT_KEY_SEVEN,
    UT_KEY_EIGHT,
    UT_KEY_NINE,
    UT_KEY_A,
    UT_KEY_B,
    UT_KEY_C,
    UT_KEY_D,
    UT_KEY_E,
    UT_KEY_F,
    UT_KEY_G,
    UT_KEY_H,
    UT_KEY_I,
    UT_KEY_J,
    UT_KEY_K,
    UT_KEY_L,
    UT_KEY_M,
    UT_KEY_N,
    UT_KEY_O,
    UT_KEY_P,
    UT_KEY_Q,
    UT_KEY_R,
    UT_KEY_S,
    UT_KEY_T,
    UT_KEY_U,
    UT_KEY_V,
    UT_KEY_W,
    UT_KEY_X,
    UT_KEY_Y,
    UT_KEY_Z,
    UT_KEY_PAD0,
    UT_KEY_PAD1,
    UT_KEY_PAD2,
    UT_KEY_PAD3,
    UT_KEY_PAD4,
    UT_KEY_PAD5,
    UT_KEY_PAD6,
    UT_KEY_PAD7,
    UT_KEY_PAD8,
    UT_KEY_PAD9,
    UT_KEY_ESC

} UT_Key;

class UT_API UT_InterceptEvent {

public:

    UT_Key	key;
    short	mx;
    short	my;


    unsigned	alt_state : 1,
    		ctl_state : 1,
		key_state : 1;
};

class UT_API UT_Intercept {

public:

     UT_Intercept();
    ~UT_Intercept();

    int		 numListeners(void) const
		 { return myListeners.entries(); }

    int		 addListener(UT_InterceptCallback func, void *data);
    void	 removeListener(int idx);

    int		 distributeEvent(UT_InterceptEvent *event);

    void	 setPollMode(int mode)
    		 { myPollMode = mode; }

    int		 getPollMode() const
    		 { return myPollMode; }

    void	 setModeNotifier(void *data)
		 { myModeNotifier = data; }

    void	*getModeNotifier() const
		 { return myModeNotifier; }

    void	 setResolution(short x, short y)
		 { myResolutionX = x; myResolutionY = y;}

private:

    UT_ValArray<UT_InterceptCallback>		 myListeners;
    UT_ValArray<void *>				 myData;
    int						 myPollMode;
    short					 myResolutionX;
    short					 myResolutionY;
    void					*myModeNotifier;
};

UT_API extern UT_Intercept *UTgetIntercept(); // Get the app intercept ptr

#endif

