/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PXL_OCIO.h (UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __PXL_OCIO__
#define __PXL_OCIO__

#include "PXL_API.h"
#include <SYS/SYS_Types.h>

class UT_StringHolder;
class UT_StringArray;
class UT_Options;

/// Abstract interface to OpenColorIO
class PXL_API PXL_OCIO
{
public:
    /// Private class
    class ColorSpace;

    /// Returns whether the user has set $OCIO, $OCIO_ACTIVE_DISPLAYS or
    /// $OCIO_ACTIVE_VIEWS is set.  If there's an error loading the
    /// configuration, this method will return @c false.
    static bool		isOCIOEnvSet(bool check_for_active_vars=true);

    /// Returns the current color space name
    static const char	*getDefaultDisplay();

    /// Returns the current color space name
    static const char	*getDefaultView(const char *display = NULL);

    /// Return the list of views for a given display.
    static void		getViewsForDisplay(const char *display,
				UT_StringArray &view_names);

    /// Returns the list of active displays
    static void 	getActiveDisplays(UT_StringArray &names);

    /// Returns the list of active views
    static void		getActiveViews(UT_StringArray &names);

    /// Returns a list of the supported color spaces
    static void	getColorSpaces(UT_StringArray &names);
    
    /// Returns a list of looks (color transforms)
    static void getLooks(UT_StringArray &looks);
    
    /// Returns a list of the supported roles
    static void getRoles(UT_StringArray &names);

    static const char *getDefaultRole();
    static const char *getReferenceRole();
    static const char *getDataRole();
    static const char *getSceneLinearRole();
    static const char *getCompLogRole();
    static const char *getColorPickingRole();

    // find an sRGB color space in the given config file
    static const char *getsRGBColorSpace(const char *display = nullptr);

    /// Return the colorspace defined by the view for a given display.
    static const char *getColorSpaceForDisplayView(const char *display_name,
						   const char *view_name);

    /// Return the looks attached to the view for a given display.
    static const char *getLooksForDisplayView(const char *display_name,
					      const char *view_name);

    /// Guess at the colorspace using the rightmost colorspace found, or NULL
    /// if none are found.
    static const char *parseColorSpaceFromString(const char *string);

    /// Return a color space handle given a name.  The name may be a color
    /// space name or prefixed with "role:" to specify a color space role.
    static const ColorSpace	*lookupSpace(const UT_StringHolder &name);

    /// Return the name of the color space
    static const char	*getName(const ColorSpace *space);
    /// Return the description of the color space
    static const char	*getDescription(const ColorSpace *space);
    /// Return the allocation variable meta data about the color space
    static bool		getAllocationVars(const ColorSpace* space,
				bool &uniform,
				fpreal &min,
				fpreal &max,
				fpreal &offset);
    /// Set 'fp' to true if the color space is defined for FP, false for uint
    static bool		isFloatPointSpace(const ColorSpace* space,
					  bool &fp);
    
    /// Fill out metadata for color space in a UT_Options
    /// Keys are the tokens in the config.ocio file:
    ///	- @c name
    ///	- @c family
    ///	- @c equalitygroup
    ///	- @c bitdepth
    ///	- @c description
    ///	- @c isdata
    ///	- @c allocation
    ///	- @c allocationvars
    static bool		getInfo(const ColorSpace *space, UT_Options &info);

    /// @{
    /// Transform color data from one color space to another color space.
    /// The color space names may either refer to a color space name, or it can
    /// also be prefixed with "role:" and refer to a color space role. The
    /// 'looks' is a comma separated list of color gradings (aka looks)
    static bool	transform(const UT_StringHolder &src,
			  const UT_StringHolder &dest,
			  const UT_StringHolder &looks,
			  fpreal32 *data, int npixels, int nchannels);
    static bool	transform(const ColorSpace *src,
			  const ColorSpace *dest,
			  const UT_StringHolder &looks,
			  fpreal32 *data, int npixels, int nchannels);
    /// @}

    /// Transform from a colorspace to a display/view, including looks.
    static bool	transformToView(const UT_StringHolder &src,
				const UT_StringHolder &display,
				const UT_StringHolder &view,
				fpreal32 *data, int npixels, int nchannels);

private:
};

#endif
