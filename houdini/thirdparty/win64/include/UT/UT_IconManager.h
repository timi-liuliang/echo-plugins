/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_IconManager.h (C++)
 *
 * COMMENTS:	A very simple class to help with finding and otherwise
 * 		dealing with icon files.
 */

#ifndef __UT_IconManager__
#define __UT_IconManager__

#include "UT_API.h"
#include "UT_UniquePtr.h"


#define UT_ICON_MAXSIZE		64

enum
UT_IconType
{
    UT_ICON_IMG,
    UT_ICON_SVG
};

class UT_PathSearch;
class UT_String;

class UT_API UT_IconManager
{
public:
    UT_IconManager();
    ~UT_IconManager();

    /// Gets the first icon (of type SGI, image, or SVG) in the icon path
    /// that matches the icon root name. Sets 'iconpath' to the full path
    /// to that icon file, and if type is set to a non-NULL pointer, sets
    /// the type appropriately.
    /// Returns true if an icon was found, and false otherwise.
    bool			 getFirstIconFile(const char *iconroot,
						  UT_String &iconpath,
						  UT_IconType *type = NULL);

    /// As in getFirstIconFile, but only finds image icons.
    bool			 getFirstImageFile(const char *imageroot,
						   UT_String &imagepath);

    /// As in getFirstIconFile, but only finds SVG icons.
    bool			 getFirstSVGFile(const char *svgroot,
						 UT_String &svgpath);

    // This returns a pointer to a list of pointers to strings.  The last
    // one will be null marking the end of the list.
    static const char	       **getValidExts();

    // Returns true if the extension in the filename matches one of the valid
    // extensions.
    static bool			 hasValidExt(const UT_String &filename);

private:
    bool			 getFirstFile(const char *iconroot,
					UT_String &iconpath,
					UT_IconType *type,
					bool doimages,
					bool dosvg);

    static const char		*theValidExts[];
    static const UT_IconType	 theExtTypes[];
};

UT_API UT_IconManager	*UTgetIconManager();

#endif

