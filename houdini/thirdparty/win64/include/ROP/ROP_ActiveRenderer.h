/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	ROP_ActiveRenderer.h (ROP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __ROP_ActiveRenderer__
#define __ROP_ActiveRenderer__

#include "ROP_API.h"

#include <UT/UT_IntArray.h>
#include <UT/UT_StringArray.h>

class UT_WorkArgs;

class ROP_API ROP_ActiveRenderer
{
public:
    enum
    {
	// Renderer is active due to:
	ENVIRONMENT 	= 1,		// Environment flag
	PREFERENCE 	= 2,		// Preference
	ROP	   	= 4		// ROP placement
    };
public:
    static const char	*getLabel(uint index);
    static const char	*getParmType(uint index);
    static uint		 getRendererCount();
    static uint		 getActiveCount();
    static const char	*getShopType(uint index);
    static bool		 isActive(uint index);
    static void		 setActive(uint index, int type, bool active);
    static int		 getActiveSource(uint index);
    
    static void		 setActiveFromString(const UT_String &renderers,
					     int type);
    static void		 clearAllActive(int type);
    
    static void		 setShowAllROPs( bool flag );
    static bool		 getShowAllROPs();

private:
    static void		 loadRenderers(const char *filename);

    static UT_StringArray	 theLabels;
    static UT_StringArray	 theParmTypes;
    static UT_StringArray	 theShopTypes;
    static UT_IntArray		 theActiveSource;
    static bool			 theLoadedFlag;
    static bool			 theShowAllROPsFlag;
};

#endif

