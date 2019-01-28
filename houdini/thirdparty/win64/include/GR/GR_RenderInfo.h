/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        GR_RenderInfo.h ( GR Library, C++)
 *
 * COMMENTS:
 *	Class containing common data to all GR_Primitives that originate from
 *	the same GUI_GeoRender object. Do not cache any of this data, as it may
 *	be deleted.
 */
#ifndef GR_RenderInfo_h
#define GR_RenderInfo_h

#include "GR_API.h"
#include "GR_Defines.h"
#include "GR_PickRecord.h"

#include <UT/UT_Array.h>
#include <UT/UT_StringHolder.h>
#include <RE/RE_CachedObject.h>

class OP_Node;
class RE_OGLTexture;
class GR_DisplayOption;
class GR_ErrorManager;

class GR_API GR_RenderInfo
{
public:
    GR_RenderInfo(GR_RenderVersion render_version)
	:  myRenderVersion(render_version)
    {
	myShowSelections = false;
	myCanDoInstancing = false;
	myObjectSelection = 0;
	myObjectGhosted = false;
	myPostPassID = 0;
	myDispOpts = NULL;
    }

    enum { UNSELECTED = 0, SELECTED_OBJ = 1, CURRENT_OBJ = 2 };

    GR_RenderVersion		 getRenderVersion() const
				 { return myRenderVersion; }
    bool			 showSelections() const
				 { return myShowSelections; }
    bool			 canDoInstancing() const
				 { return myCanDoInstancing; }
    bool			 isObjectSelection() const
				 { return (myObjectSelection != UNSELECTED); }
    bool			 isObjectCurrent() const
				 { return (myObjectSelection == CURRENT_OBJ); }
    bool			 isObjectGhosted() const
				 { return myObjectGhosted; }
    RE_CacheTagHandle		 getCacheTag() const
				 { return myCacheTagHandle; }
    UT_Array<GR_PickRecord>	&getPickArray() const
				 { return myPicks; }
    const GR_DisplayOption	*getDisplayOption() const
				 { return myDispOpts; }
    static GR_ErrorManager	*getErrorHandler()
				 { return theErrorHandler; }
    const UT_StringHolder       &getNodePath() const
				 { return myNodePath; }

    // Reserves a post-processing pass for the primitive. Returns an ID that
    // will uniquely identify the post-pass for the primitive.
    int				 requestRenderPostPass() const
				 { myPostPassID++; return myPostPassID; }
    // When rendering a post pass, check again the post-pass ID to make sure it
    // is the pass you requested
    int				 getPostPassID() const
				 { return myPostPassID; }

private:
    GR_RenderVersion			 myRenderVersion;
    bool				 myShowSelections;
    bool				 myCanDoInstancing;
    bool				 myObjectGhosted;
    int					 myObjectSelection;
    RE_CacheTagHandle			 myCacheTagHandle;
    const GR_DisplayOption		*myDispOpts;
    mutable UT_Array<GR_PickRecord>	 myPicks;
    mutable int				 myPostPassID;
    static GR_ErrorManager		*theErrorHandler;
    UT_StringHolder			 myNodePath;

    friend class			 GUI_GeoRender;
    friend class			 DM_VPortAgent;
    friend class			 DM_SceneManager;
    friend class			 jedi_UsdGLRender;
};

#endif

