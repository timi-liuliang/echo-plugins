/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO_PrimTypeCompat.h (GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GEO_PrimTypeCompat_h__
#define __GEO_PrimTypeCompat_h__

/// @file GEO_PrimTypeCompat.h
/// @brief Contains transitional objects to provide some backward compatibility
/// for code that references old GEO primitive ids.  New code should explicitly
/// use either GA_Primitive::getTypeId() or GA_Primitive::getFamilyMask(), as
/// appropriate.  These transitional objects will be removed in the next major
/// release.

#include "GEO_API.h"
#include <GA/GA_PrimCompat.h>

/// The compatibility objects are wrapped in a name space to stress the change
/// between the obsolete type bitmasks (e.g. GEOPRIMPOLY) and the new type ids
/// (e.g. GEO_PRIMPOLY).
namespace GEO_PrimTypeCompat
{
    using GA_PrimCompat::TypeMask;

    GEO_API extern const TypeMask GEOPRIMNONE;
    //
    //  Standard face types
    GEO_API extern const TypeMask GEOFACE;
    GEO_API extern const TypeMask GEOPRIMPOLY;
    GEO_API extern const TypeMask GEOPRIMNURBCURVE;
    GEO_API extern const TypeMask GEOPRIMBEZCURVE;

    GEO_API extern const TypeMask GEOCURVE;

    //
    //  Standard hull types
    GEO_API extern const TypeMask GEOHULL;
    GEO_API extern const TypeMask GEOPRIMMESH;
    GEO_API extern const TypeMask GEOPRIMNURBSURF;
    GEO_API extern const TypeMask GEOPRIMBEZSURF;

    GEO_API extern const TypeMask GEOTPSURF;
    GEO_API extern const TypeMask GEOSPLINE;

    //
    //  Standard paste types
    GEO_API extern const TypeMask GEOPRIMPASTESURF;

    //
    //  Standard quadric types
    GEO_API extern const TypeMask GEOPRIMQUADRIC;
    GEO_API extern const TypeMask GEOPRIMCIRCLE;
    GEO_API extern const TypeMask GEOPRIMSPHERE;
    GEO_API extern const TypeMask GEOPRIMTUBE;

    //
    //  Standard particle types
    GEO_API extern const TypeMask GEOPRIMPART;

    //
    //  Standard meta primitive types
    GEO_API extern const TypeMask GEOPRIMMETA;
    GEO_API extern const TypeMask GEOPRIMMETABALL;
    GEO_API extern const TypeMask GEOPRIMMETASQUAD;
    GEO_API extern const TypeMask GEOPRIMMETALINE;
    GEO_API extern const TypeMask GEOPRIMMETABEZ;
    GEO_API extern const TypeMask GEOPRIMMETATRI;

    //
    // Miscelleneous primitive types.
    // These guys must be detected via operator==, not operator&.
    GEO_API extern const TypeMask GEOPRIMOTHER;
    GEO_API extern const TypeMask GEOPRIMTRIFAN;
    GEO_API extern const TypeMask GEOPRIMTRISTRIP;
    GEO_API extern const TypeMask GEOPRIMTRIBEZIER;
    GEO_API extern const TypeMask GEOPRIMVOLUME;
    GEO_API extern const TypeMask GEOPRIMTETRAHEDRON;
    GEO_API extern const TypeMask GEOPRIMPOLYSOUP;
    GEO_API extern const TypeMask GEOPRIMVDB;
    GEO_API extern const TypeMask GEOALLFUTURE;

    GEO_API extern const TypeMask GEOPRIMALL;

    //
    // Classifies as U or UV primitives:
    GEO_API extern const TypeMask GEOPRIMU;
    GEO_API extern const TypeMask GEOPRIMUV;

    //
    //  Type strings
    GEO_API extern const char *GEOPRIMPOLYN;
    GEO_API extern const char *GEOPRIMNURBCURVEN;
    GEO_API extern const char *GEOPRIMBEZCURVEN;
    GEO_API extern const char *GEOPRIMMESHN;
    GEO_API extern const char *GEOPRIMNURBSURFN;
    GEO_API extern const char *GEOPRIMBEZSURFN;
    GEO_API extern const char *GEOPRIMPASTESURFN;
    GEO_API extern const char *GEOPRIMCIRCLEN;
    GEO_API extern const char *GEOPRIMSPHEREN;
    GEO_API extern const char *GEOPRIMTUBEN;
    GEO_API extern const char *GEOPRIMPARTN;
    GEO_API extern const char *GEOPRIMMETABALLN;
    GEO_API extern const char *GEOPRIMMETASQUADN;
    GEO_API extern const char *GEOPRIMMETALINEN;
    GEO_API extern const char *GEOPRIMMETABEZN;
    GEO_API extern const char *GEOPRIMMETATRIN;
    GEO_API extern const char *GEOPRIMTRIFANN;
    GEO_API extern const char *GEOPRIMTRISTRIPN;
    GEO_API extern const char *GEOPRIMTRIBEZIERN;
    GEO_API extern const char *GEOPRIMVOLUMEN;

    /// Map from the compatibility type mask to the enum values defined in
    /// GEO_PrimType.h.  If the mask doesn't match one of the enummerated types
    /// exactly, GEO_PRIMNONE will be returned.  In other words @code
    ///	   getPrimType(GEO_PrimTypeCompat::GEOPRIMPOLY) == GEO_PRIMPOLY
    ///    getPrimType(GEO_PrimTypeCompat::GEOFACE) == GEO_PRIMNONE
    /// @endcode
    GEO_API extern int	getPrimType(const TypeMask &mask);

    //
    //  File type identifiers
    enum
    {
	GEOPRIMPOLY_FILEID      = 0x00000001,	// Polygon
	GEOPRIMNURBCURVE_FILEID = 0x00000002,	// NURB Curve
	GEOPRIMBEZCURVE_FILEID  = 0x00000004,	// Rational Bezier Curve
	GEOPRIMMESH_FILEID      = 0x00000010,	// Linear patch
	GEOPRIMNURBSURF_FILEID  = 0x00000020,	// NURB Surface
	GEOPRIMBEZSURF_FILEID   = 0x00000040,	// Rational Bezier Surface
	GEOPRIMPASTESURF_FILEID = 0x00000f00,	// TPsurf Paste Hierarchy
	GEOPRIMCIRCLE_FILEID    = 0x00001000,	// Ellipse/Circle
	GEOPRIMSPHERE_FILEID    = 0x00002000,	// Ellipsoid/Sphere
	GEOPRIMTUBE_FILEID      = 0x00004000,	// Cylinder
	GEOPRIMPART_FILEID      = 0x00008000,	// Particle
	GEOPRIMMETABALL_FILEID  = 0x00100000,	// Meta-ball
	GEOPRIMMETASQUAD_FILEID = 0x00200000,	// Super quadric
	GEOPRIMMETALINE_FILEID  = 0x00300000,	// Meta-line
	GEOPRIMMETABEZ_FILEID   = 0x00400000,	// Meta-bezier
	GEOPRIMMETATRI_FILEID   = 0x00500000,	// Meta-triangle
	GEOPRIMTRIFAN_FILEID    = 0x10000000,	// Triangle fans
	GEOPRIMTRISTRIP_FILEID  = 0x20000000,	// Generalized triangle strips
	GEOPRIMTRIBEZIER_FILEID = 0x30000000,	// Triangular beziers
	GEOPRIMVOLUME_FILEID    = 0x40000000,	// Voxel volume
    };

    /// Map from an old file bitfield to the compatibility type mask.
    GEO_API extern TypeMask convertMaskFromFile(unsigned mask);

    /// Inverse of convertMaskFromFile()
    GEO_API extern unsigned convertToFileFromMask(TypeMask mask);
}

#endif
