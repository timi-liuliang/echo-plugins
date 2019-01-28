/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO_IORibExt.h ( GEO Library, C++)
 *
 * COMMENTS:	Encapsulated definition of RIB extensions supported by the GEO
 *		library.
 */

#ifndef __GEO_IORibExt__
#define __GEO_IORibExt__

#include "GEO_API.h"
#include <string.h>

class UT_OptionFile;

enum
{
    // Features supported
    GEO_RIB_RIPOINT = 0,	// Supports RiPoint
    GEO_RIB_RICURVE,		// Supports RiCurve
    GEO_RIB_RINUCURVE,		// Supports RiNuCurve
    GEO_RIB_RISUBDIVIDE,	// Supports Sub-Division surfaces
    GEO_RIB_RIBLOBBY,		// Supports Blobbys

    // Bug fixes in specific cases
    GEO_RIB_PRMAN36_PRIMCLR,	// Prior to prman3.7, they processed
				// primitive attributes backwards on
				// PointsGeneralPolygon
    GEO_RIB_PRMAN38_UCURVE,	// Prior to prman3.9 uniform attributes on
				// curves were set for the whole curve set.
				// After 3.9 the uniform attribute is per-curve

    // prman 10
    GEO_RIB_FACEVARYING,	// Face varying extension

    GEO_RIB_MAX_EXTENSIONS
};


class GEO_API GEO_IORibExt
{
public:
     GEO_IORibExt();
    ~GEO_IORibExt();

    void	setRenderer(const char *renderer, int major, int minor);

    int		hasExtension(int ext)
		{
		    if (ext < 0 || ext >= GEO_RIB_MAX_EXTENSIONS)
			return 0;
		    return myExtensions[ext];
		}

private:
    void	initExtensions(const UT_OptionFile &options);
    char	myExtensions[GEO_RIB_MAX_EXTENSIONS];
};

#endif

