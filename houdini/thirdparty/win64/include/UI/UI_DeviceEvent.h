#ifndef __UI_DeviceEvent__
#define __UI_DeviceEvent__

#include "UI_API.h"

#include <RE/RE_IDType.h>
#include <SYS/SYS_Types.h>
#include <UT/UT_Array.h>
#include <UT/UT_StringHolder.h>

#include "UI_EventType.h"

/*
 *	The UI_DeviceEvent is a generic event structure which
 *	various input device drivers will generate as sub-classes
 *	of UI_EventGenerator.
 */

/*
 *  Note: these are flag bits since more than one button can be down:
 */
#define UI_NO_BUTTON		0x0
#define UI_LEFT_BUTTON		0x01
#define UI_MIDDLE_BUTTON	0x02
#define UI_RIGHT_BUTTON		0x04

#define	UI_ALT_KEY		0x001	//
#define	UI_SHIFT_KEY		0x002	//
#define	UI_CTRL_KEY		0x004	//
#define UI_COMMAND_KEY		0x008	// Windows key on Win/Linux
#define UI_META_KEYS		(UI_ALT_KEY| \
				 UI_SHIFT_KEY| \
				 UI_CTRL_KEY| \
				 UI_COMMAND_KEY)
#define UI_ALPHASHIFT_KEY	0x010	// Caps-lock
#define UI_AUTOREPEAT		0x020	// Auto-repeat is on for the key
#define UI_IME_PREEDIT		0x040	// Key is IME pre-edit string
#define UI_IME_COMMIT		0x080	// Key is IME commit string
#define UI_KEYPAD_KEY		0x100	// The key came from the keypad
#define UI_TABLET_MOTION	0x200	// Motion event triggered from tablet.

// Conversion from our Z value to 0..1
#define UI_PRESSURE_SCALE	16384.0F
// Conversion of T/U/V values to degrees.
#define UI_ANGLE_SCALE		100.0F

enum UI_ArrowKeys
{
    UI_UP_ARROW 	= 1,
    UI_DOWN_ARROW	= 2,
    UI_LEFT_ARROW	= 3,
    UI_RIGHT_ARROW	= 4
};

enum UI_Dimension {
	DEVICE_DIM_T = 0,
	DEVICE_DIM_U = 1,
	DEVICE_DIM_V = 2,
	W = 3,
	X = 4,
	Y = 5,
	Z = 6
};

#define DIMENSIONS	7


class UI_API UI_DeviceEvent 
{
public:
    UI_DeviceEvent(int event_id = 0);

    /// Returns an array of Unicode code points corresponding to typed in
    /// characters from a keyboard event.
    bool getTypedCharacters(UT_Array<utf32> &cps) const;
    
    UI_EventType type;
    RE_IDType	 context;		// Device specific context
    double	 time;			// Time in seconds.
    
    exint	 data;			// Event data (at least pointer sized)
    int		 values[DIMENSIONS];	// Usually valuator values
    short	 altFlags;		// Event flags
    short	 id;			// Event identifier
    
    int		 qt_key;
    UT_StringHolder key;		// Key input for UI_EVENT_KEYBOARD
};

inline
UI_DeviceEvent::UI_DeviceEvent(int event_id)
{
    id  = event_id;

    type = UI_EVENT_NO_EVENT;

    context   = 0;
    data      = 0;
    altFlags  = 0;
    values[DEVICE_DIM_T] = 0;
    values[DEVICE_DIM_U] = 0;
    values[DEVICE_DIM_V] = 0;
    values[W] = 0;
    values[X] = 0;
    values[Y] = 0;
    values[Z] = 0;
    
    time = -1.0;
    qt_key = 0;
}

#endif
