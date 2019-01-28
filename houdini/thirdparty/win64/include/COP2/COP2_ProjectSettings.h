/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	COP2_ProjectSettings.h
 *
 * COMMENTS:
 *	Holds the project settings for the compositor.
 */
#ifndef __COP2_ProjectSettings_h__
#define __COP2_ProjectSettings_h__

#include "COP2_API.h"
#include <OP/OP_Error.h>
#include <iosfwd>

#define COP2_PROJECT_SETTINGS_PREF_FILE	"compproject.pref"

class CMD_Args;

class COP2_API COP2_ProjectSettings
{
public:
    static bool		 setRes(int x, int y);
    static void		 getRes(int &x, int &y);
    
    static bool		 setPixelAspect(float aspect);
    static float	 getPixelAspect();
    
    static bool		 setDepth(int depth);
    static int		 getDepth();

    static bool		 resetBWPoints();
    static bool		 setBWPoints(int b, int w);
    static void		 getBWPoints(int &b, int &w);

    static bool		 setProxyLevel(int level);
    static int		 getProxyLevel();
    static float	 getProxyRes();
    static void		 getIconRes(int &x, int &y);

    static void		 setFileDefaults(int res, int depth);
    static int		 getFileResDefault();
    static int		 getFileDepthDefault();
    
    static void		 setFileLastFileDefault(int enable);
    static int		 getFileLastFileDefault();

    static void		 setLargeCanvas(bool enable);
    static bool		 getLargeCanvas();

    static const char   *getProxyLevelString(int index);

    static void		 initialize();
    static OP_ERROR	 saveSettings(std::ostream &, void *);
    static void		 compProjectCmd(CMD_Args &args);

    static void		 setViewerCallback(void (*cb)(float ));

private:
    static void	 loadDefaultPrefs();

};    

#endif
