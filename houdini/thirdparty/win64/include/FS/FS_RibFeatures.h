/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	FS_RibFeatures.h ( FS Library, C++)
 *
 * COMMENTS:	This class provides a database of renderers and which features
 *		they support.
 *
 *	RIB support in Houdini has a fixed number of extensions supported by
 *	different renderers.  For example, different versions of prman may
 *	support different feature sets.  By querying for a specific renderer,
 *	we can find out the feature set supported by that renderer.
 *
 *	Some common options are:
 *		rib.renderer.token	string	The name of the renderer
 *		rib.renderer.label	string	The label associated
 *		rib.release.major	int	The major release
 *		rib.release.minor	int	The minor release
 *		rib.dice.stitch		bool	Renderer supports dice stitch
 *		rib....				Other features of the renderer
 */

#ifndef __FS_RibFeatures__
#define __FS_RibFeatures__

#include "FS_API.h"
class UT_OptionFile;

class FS_API FS_RibFeatures {
public:
    static UT_OptionFile	*getRenderer(const char *renderer);
    static UT_OptionFile	*getRenderer(int id);
    static int			 entries();	// Number of renderers defined
    static const char		*getDefaultRenderer();
};

#endif
