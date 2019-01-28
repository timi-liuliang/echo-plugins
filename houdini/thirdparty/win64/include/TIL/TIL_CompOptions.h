/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	COP2_CompOptions.h
 *
 * COMMENTS:
 *	Holds the global options for the compositor.
 */
#ifndef __TIL_CompOptions_h__
#define __TIL_CompOptions_h__

#include "TIL_API.h"
#include <SYS/SYS_Types.h>

class TIL_TileManager;
class UT_FileLUT;

class TIL_API TIL_CompOptions
{
public:
    TIL_CompOptions();

    // Cache
    static void		 setCookCacheSize(int size);
    static int		 getCookCacheSize();
    
    static void		 setCacheInactiveReduce(int reduce);
    static int		 getCacheInactiveReduce();
    
    static void		 setCacheInactiveSize(int size);
    static int		 getCacheInactiveSize();
    
    static void		 setPerformanceCaching(int on);
    static int		 getPerformanceCaching();
    
    static void		 setResLimit(int x,int y);
    static void		 getResLimit(int &x, int &y);

    // returns the recommended memory usage in kb.
    static void		 getRecommendedMemoryUsage(int64 &cook, int64 &disp);
    static int		 getRecommendedThreads();

    // Cooking
    static void		 setMultithreadedCooking(int on);
    static int		 getMultithreadedCooking();
    
    static void		 setNumCookThreads(int threads);
    static int		 getNumCookThreads();
    
    static void		 setUseCollapsableOps(int enable);
    static int		 getUseCollapsableOps();

    static void		 setUseGPUAssist(bool enable);
    static bool		 getUseGPUAssist(bool check_for_hardware_support=true);
    static bool		 isGPUAssistInitialized();

    static void		 setCookOnStart(int enable);
    static int		 getCookOnStart();

    static void		 setIncrementalCook(int type);
    static int		 getIncrementalCook();
    
    static void		 setIncrementalCookStyle(int type);
    static int		 getIncrementalCookStyle();

    // Proxies.
    static void		 setProxyGeneration(int on);
    static int		 getProxyGeneration();
    
    static void		 setProxyLimit(int on);
    static int		 getProxyLimit();
    
    static void		 setProxyDiskLimit(int size);
    static int		 getProxyDiskLimit();
    
    static void		 setProxyDir(const char *dir);
    static const char   *getProxyDir();
    
    static void		 setTileSize(int x, int y);
    static void		 getTileSize(int &x, int &y);

    // Plane names.
    static void		 setColorName(const char *n);
    static const char	*getColorName();

    static void		 setAlphaName(const char *n);
    static const char	*getAlphaName();

    static void		 setMaskName(const char *n);
    static const char	*getMaskName();

    static void		 setDepthName(const char *n);
    static const char	*getDepthName();

    static void		 setLumName(const char *n);
    static const char	*getLumName();

    static void		 setBumpName(const char *n);
    static const char	*getBumpName();

    static void		 setPointName(const char *n);
    static const char	*getPointName();

    static void		 setNormalName(const char *n);
    static const char	*getNormalName();

    static void		 setVelocityName(const char *n);
    static const char	*getVelocityName();

    // Selectors.
    static void		 setSelectorNoOptional(int noopt);
    static int		 getSelectorNoOptional();
    
    static void		 setSelectorNoGenInputs(int nogen);
    static int		 getSelectorNoGenInputs();
    
    static void		 setSelectorPlaceCurrent(int placecurrent);
    static int		 getSelectorPlaceCurrent();
    
    static void		 setSelectorGenNewView(int newview);
    static int		 getSelectorGenNewView();

    // COP Colors.
    static void		 setUseCopColoring(int use);
    static int		 getUseCopColoring();

    static void		 setGeneratorColorEnable(int use);
    static int		 getGeneratorColorEnable();
    
    static void		 setGeneratorColor(float r, float g, float b);
    static void		 getGeneratorColor(float &r, float &g, float &b);
    
    static void		 setPixelColorEnable(int use);
    static int		 getPixelColorEnable();
    
    static void		 setPixelColor(float r, float g, float b);
    static void		 getPixelColor(float &r, float &g, float &b);
    
    static void		 setTimingColorEnable(int use);
    static int		 getTimingColorEnable();
    
    static void		 setTimingColor(float r, float g, float b);
    static void		 getTimingColor(float &r, float &g, float &b);

    static void		 setVexColorEnable(int use);
    static int		 getVexColorEnable();
    
    static void		 setVexColor(float r, float g, float b);
    static void		 getVexColor(float &r, float &g, float &b);

    // Fast Interactive Cooking.
    static void		 setFastInteractive(bool enable);
    static bool		 getFastInteractive();

    static void		 setFastInteractiveOnSlow(bool enable);
    static bool		 getFastInteractiveOnSlow();

    static void		 setFastInteractiveQuality(int resdownby);
    static int		 getFastInteractiveQuality();

    static void		 setFastInteractiveMinSize(int  x, int  y);
    static void		 getFastInteractiveMinSize(int &x, int &y);

    // Load's o cineon stuff.
    static void		 initCineon();

    // This sets the defaults for building Cineon luts.
    static void		 setCineonParms(bool convert, const char *lut,
					int white, float gamma, int black);
    static void		 getCineonParms(bool &convert, const char *&lut,
					int &white, float &gamma, int &black);

    // Returns a new LUT based on the passed parms.
    static UT_FileLUT	*buildReadLUT(bool convert, const char *lut,
				      int white, float gamma, int black);
    static UT_FileLUT	*buildWriteLUT(bool convert, const char *lut,
				      int white, float gamma, int black);
				      
    // Internal function for building default LUTs (based on the parms set by
    // setCineonParms).
    static void		 buildCineonLUTs(bool read,
					 bool write,
					 UT_FileLUT *&rlut,
					 UT_FileLUT *&wlut);

    // The workhorse function for creating luts. 
    static void		 createCineonLUT(bool convert, const char *lut,
					 int white, float gamma, int iwhite,
					 bool read, bool write,
					 UT_FileLUT *&rlut,
					 UT_FileLUT *&wlut);
    
    static UT_FileLUT *  defaultLUTcreator(bool dowrite);

   
    static TIL_TileManager	*getTileManager();

    static const char   *getPrefFileName();
    static void		 loadPreferences();
    static void		 savePreferences();
};

#endif
