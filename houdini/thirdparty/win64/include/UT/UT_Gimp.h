/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Gimp.h ( UT Library, C++)
 *
 * COMMENTS:
 *
 *	Gimp Callback interface for the Gimp COP	
 */

#ifndef __UT_Gimp__
#define __UT_Gimp__

#include "UT_API.h"
#define GIMP_CURSOR_CHANGED	1
#define GIMP_IMAGE_CHANGED	2
#define GIMP_SHUTDOWN		3

struct utGimpInfoStruct
{
    // naming conv: variables starting with 'app' are only set by Houdini.
    // variables starting with 'gimp' are set by gimp.
    
    // flags
    unsigned char	gimpReady;
    unsigned char	appReady;
    unsigned char	gimpImageBusy;
    unsigned char	appImageBusy;
    unsigned char	gimpDone;
    unsigned char	appDone;
    unsigned char	gimpImageFullUpdate;

    // cursor, image size & serial #
    unsigned int	appImageSizeX;
    unsigned int	appImageSizeY;
    unsigned int	appExportTileSize;
    unsigned int	appDragUpdate;
    int			gimpCursorX;
    int			gimpCursorY;
    unsigned int	gimpImageSerial;

    // tile update data follows (1 byte per tile, tiles are 64x64 blocks)
    // image data follows here...
};

class UT_Gimp;

typedef void (*UT_GimpCallback)(int why, void *data);
typedef UT_Gimp *(*UT_GimpCreator)(UT_GimpCallback func, utGimpInfoStruct *,void *data);


class UT_API UT_Gimp
{
public:
	     UT_Gimp(UT_GimpCallback, utGimpInfoStruct *info, void *data);
    virtual ~UT_Gimp();

    virtual int isValid();
    virtual void startChecks();
    virtual void stopChecks();

    virtual void updateCursor(int is_on);
};

UT_API extern void UTsetGimpCreator(UT_GimpCreator  func);
UT_API extern UT_Gimp * UTgetNewGimp(UT_GimpCallback func,
				     utGimpInfoStruct *info,
				     void *data);

#endif
