/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GR_VisInterface.h ( GR Library, C++)
 *
 * COMMENTS:
 * 	Basic definition needed to access functions defined in VIS which is
 * 	set above the GR level.
 */

#ifndef __GR_VisInterface_h__
#define __GR_VisInterface_h__

#include "GR_API.h"


class GR_API GR_VisInterface
{
public:
    GR_VisInterface() {}
    virtual ~GR_VisInterface() {}

    virtual bool	 isHookVisualizerActive(const char *vis_name,
                		int vis_context, int display_set) = 0;

    static void 	 setHandler(GR_VisInterface *handler)
			    { theHandler = handler; }

    static
    GR_VisInterface 	*getHandler()
			    { return theHandler; }

    static
    GR_VisInterface	*theHandler;
};


#endif
