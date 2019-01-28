#ifndef __RE_Cursor__
#define __RE_Cursor__

#include "RE_API.h"
#include "RE_IDType.h"

#include <UT/UT_Rect.h>
#include <UT/UT_UniquePtr.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_Vector2.h>

#define RE_CURSOR_DEFAULT		"iris"


#define RE_CURSOR_I_AM_BUSY		"busy"
#define RE_CURSOR_I_AM_INTERRUPTABLE	"interruptable"
#define RE_CURSOR_ARROW_RIGHT		"arrowRight"
#define RE_CURSOR_ARROW_UP		"arrowUp"
#define RE_CURSOR_ARROW_LEFT		"arrowLeft"
#define RE_CURSOR_ARROW_DOWN		"arrowDown"
#define RE_CURSOR_ARROW_ALL		"arrowAll"
#define RE_CURSOR_ARROW_ALL_D		"arrowAllDiagonal"
#define RE_CURSOR_ARROW_UP_DOWN		"arrowUpDown"
#define RE_CURSOR_ARROW_LEFT_RIGHT	"arrowLeftRight"
#define RE_CURSOR_ARROW_D1		"arrowD1"
#define RE_CURSOR_ARROW_D2		"arrowD2"

#define RE_CURSOR_ARROW_NE		"arrowNE"
#define RE_CURSOR_ARROW_NW		"arrowNW"
#define RE_CURSOR_ARROW_SE		"arrowSE"
#define RE_CURSOR_ARROW_SW		"arrowSW"

#define RE_CURSOR_ARROW_NWSE		"arrowNWSE"
#define RE_CURSOR_ARROW_SWNE		"arrowSWNE"

#define RE_CURSOR_ARROW_KEYLOCK		"arrowKeyLock"

#define RE_CURSOR_CROSS			"cross"
#define RE_CURSOR_CUT			"cut"
#define RE_CURSOR_ARROW			"arrow"
// WARNING: The following cursor (RE_CURSOR_IRIS) is hardcoded in
// BM_OpState.h
#define RE_CURSOR_IRIS			"iris"
#define RE_CURSOR_X			"X"

#define RE_CORNER_BL			"cornerBL"
#define RE_CORNER_BR			"cornerBR"
#define RE_CORNER_TL			"cornerTL"
#define RE_CORNER_TR			"cornerTR"

#define RE_CURSOR_DRAGDROP_ON		"dragdropOn"
#define RE_CURSOR_DRAGDROP_OFF		"dragdropOff"

#define RE_CURSOR_HAND			"hand"
#define RE_CURSOR_HAND_POINT		"handpoint"
#define RE_CURSOR_QUESTION		"question"
#define RE_CURSOR_SELECT_IRIS		"select"
#define RE_CURSOR_TEXT			"ibeam"

// This is not used anywhere and the 'smallBusy.cursor'
// file is corrupt.
//   #define RE_CURSOR_HOUR_GLASS	"smallBusy"

// Selection cursors
#define RE_CURSOR_SELECT_PREFIX		"select_"
#define RE_CURSOR_SELECT_WHITE_REPLACE	"select_white_replace"
#define RE_CURSOR_SELECT_WHITE_ADD	"select_white_add"
#define RE_CURSOR_SELECT_WHITE_REMOVE	"select_white_remove"
#define RE_CURSOR_SELECT_WHITE_TOGGLE	"select_white_toggle"
#define RE_CURSOR_SELECT_BLACK_REPLACE	"select_black_replace"
#define RE_CURSOR_SELECT_BLACK_ADD	"select_black_add"
#define RE_CURSOR_SELECT_BLACK_REMOVE	"select_black_remove"
#define RE_CURSOR_SELECT_BLACK_TOGGLE	"select_black_toggle"
#define RE_CURSOR_SELECT_LASER_REPLACE	"select_laser_replace"
#define RE_CURSOR_SELECT_LASER_ADD	"select_laser_add"
#define RE_CURSOR_SELECT_LASER_REMOVE	"select_laser_remove"
#define RE_CURSOR_SELECT_LASER_TOGGLE	"select_laser_toggle"

// Handle locate cursors
#define RE_CURSOR_HANDLE_LOCATE		"handle_locate"
#define RE_CURSOR_HANDLE_LOCATE_X	"handle_locate_x"
#define RE_CURSOR_HANDLE_LOCATE_Y	"handle_locate_y"
#define RE_CURSOR_HANDLE_LOCATE_Z	"handle_locate_z"
#define RE_CURSOR_HANDLE_LOCATE_XY	"handle_locate_xy"
#define RE_CURSOR_HANDLE_LOCATE_YZ	"handle_locate_yz"
#define RE_CURSOR_HANDLE_LOCATE_XZ	"handle_locate_xz"
#define RE_CURSOR_HANDLE_LOCATE_XYZ	"handle_locate_xyz"
#define RE_CURSOR_HANDLE_LOCATE_U	"handle_locate_u"
#define RE_CURSOR_HANDLE_LOCATE_V	"handle_locate_v"
#define RE_CURSOR_HANDLE_LOCATE_W	"handle_locate_w"
#define RE_CURSOR_HANDLE_LOCATE_UV	"handle_locate_uv"

class RE_Server;
class QCursor;

class RE_API RE_Cursor
{
public:
    RE_Cursor(const char *name);
    
    ~RE_Cursor();

    RE_IDType	 getDeviceId();

    // Return a rectangle enclosing the cursor, in relative device coordinates,
    // where 0,0 is the origin.
    UT_DimRect getCursorRect() const { return myRect; }

    const char *getName() const { return myName.c_str(); }

    /// Return the current mouse position in Houdini's UI coordinate system.
    static void position(int &x, int &y);

private:
    void	 loadCursor_();
    void	 loadSVGCursor_(const char *filename, int x, int y);
    void	 loadOldCursor_(const char *filename);
    void	 computeBoundingBox_();

    
    UT_StringHolder myName;

    /// The Qt cursor object create for this cursor
    UT_UniquePtr<QCursor> myQtCursor;

#ifdef WIN32
    /// The default cursor size on Windows
    UT_Vector2i		 mySysCursorSize;
#endif

    /// The cursor rect
    UT_DimRect		 myRect;
};
#endif
