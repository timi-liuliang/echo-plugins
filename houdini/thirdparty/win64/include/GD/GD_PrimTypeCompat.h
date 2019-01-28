/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GD_PrimTypeCompat.h (GD Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GD_PrimTypeCompat_h__
#define __GD_PrimTypeCompat_h__

/// @file GD_PrimTypeCompat.h
/// Contains transitional objects to provide some backward compatibility for
/// code that references old GD primitive ids.  New code should explicitly use
/// either GA_Primitive::getTypeId() or GA_Primitive::getFamilyMask(), as
/// appropriate.  These transitional objects will be removed in the next major
/// release.

#include "GD_API.h"
#include <GA/GA_PrimCompat.h>

/// The compatibility objects are wrapped in a name space to stress the change
/// between the obsolete type bitmasks (e.g. GDPRIMPOLY) and the new type ids
/// (e.g. GD_PRIMPOLY).
namespace GD_PrimTypeCompat
{
    //
    //  Standard face types
    GD_API extern const GA_PrimCompat::TypeMask GDFACE;
    GD_API extern const GA_PrimCompat::TypeMask GDPRIMPOLY;
    GD_API extern const GA_PrimCompat::TypeMask GDPRIMNURBCURVE;
    GD_API extern const GA_PrimCompat::TypeMask GDPRIMBEZCURVE;

    GD_API extern const GA_PrimCompat::TypeMask GDCURVE;

    GD_API extern const GA_PrimCompat::TypeMask GDPRIMALL;

    //
    //  Type strings
    GD_API extern const char *GDPRIMPOLYN;
    GD_API extern const char *GDPRIMNURBCURVEN;
    GD_API extern const char *GDPRIMBEZCURVEN;

    //
    //  File type identifiers
    enum
    {
	GDPRIMPOLY_FILEID      = 0x00000001,
	GDPRIMNURBCURVE_FILEID = 0x00000002,
	GDPRIMBEZCURVE_FILEID  = 0x00000004
    };
}

#endif
