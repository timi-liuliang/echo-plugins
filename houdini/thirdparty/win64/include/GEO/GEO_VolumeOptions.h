/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO_VolumeOptions.h (GEO Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GEO_VOLUMEOPTIONS_H_INCLUDED__
#define __GEO_VOLUMEOPTIONS_H_INCLUDED__

#include <UT/UT_VoxelArray.h>

#include "GEO_API.h"

enum GEO_VolumeBorder
{
    GEO_VOLUMEBORDER_CONSTANT,
    GEO_VOLUMEBORDER_REPEAT,
    GEO_VOLUMEBORDER_STREAK,
    GEO_VOLUMEBORDER_SDF,
    NUM_GEO_VOLUMEBORDERS
};

enum GEO_VolumeVis
{
    GEO_VOLUMEVIS_SMOKE,
    GEO_VOLUMEVIS_RAINBOW,
    GEO_VOLUMEVIS_ISO,
    GEO_VOLUMEVIS_INVISIBLE,
    GEO_VOLUMEVIS_HEIGHTFIELD,
    NUM_GEO_VOLUMEVIS
};

enum GEO_VolumeVisLod
{
    GEO_VOLUMEVISLOD_INVALID_MODE = -1,
    GEO_VOLUMEVISLOD_BOX,
    GEO_VOLUMEVISLOD_LEAFBOX,
    GEO_VOLUMEVISLOD_FULL,
    GEO_VOLUMEVISLOD_FULLINCORE,

    NUM_GEO_VOLUMEVISLOD
};

struct GEO_API GEO_VolumeOptions
{
    explicit
    GEO_VolumeOptions(
	    GEO_VolumeVis mode = GEO_VOLUMEVIS_SMOKE,
	    fpreal iso = 0.0,
	    fpreal density = 1.0,
	    GEO_VolumeVisLod lod = GEO_VOLUMEVISLOD_FULL)
	: myMode(mode)
	, myIso(iso)
	, myDensity(density)
	, myLod(lod)
    {
    }
    bool operator==(const GEO_VolumeOptions &v) const
    {
	return (myMode == v.myMode
		&& myIso == v.myIso
		&& myDensity == v.myDensity
		&& myLod == v.myLod);
    }
    bool operator!=(const GEO_VolumeOptions &v) const
    {
	return !(*this == v);
    }

    GEO_VolumeVis	    myMode;
    fpreal		    myIso;
    fpreal		    myDensity;
    GEO_VolumeVisLod	    myLod;
};

/// Return string token from GEO_VolumeBorder enum value
GEO_API const char *	    GEOgetVolumeBorderToken(
				GEO_VolumeBorder border);
/// Return GEO_VolumeBorder enum value from string token. def is returned if
/// token is unknown.
GEO_API GEO_VolumeBorder    GEOgetVolumeBorderEnum(
				const char *token,
				GEO_VolumeBorder def);

/// Return string token from GEO_VolumeVis enum value
GEO_API const char *	    GEOgetVolumeVisToken(
				GEO_VolumeVis vis);
/// Return GEO_VolumeVis enum value from string token. def is returned if
/// token is unknown.
GEO_API GEO_VolumeVis	    GEOgetVolumeVisEnum(
				const char *vis,
				GEO_VolumeVis def);

/// Return string token from GEO_VolumeVisLod enum value
GEO_API const char *	    GEOgetVolumeVisLodToken(
				GEO_VolumeVisLod lod);
/// Return GEO_VolumeVisLod enum value from string token. def is returned if
/// token is unknown.
GEO_API GEO_VolumeVisLod    GEOgetVolumeVisLodEnum(
				const char *token,
				GEO_VolumeVisLod def);

inline size_t format(char *buf, size_t bufsize, const GEO_VolumeBorder &v)
{
    UT::Format::Writer	writer(buf, bufsize);
    UT::Format::Formatter<> f;
    return f.format(writer, "{}", {GEOgetVolumeBorderToken(v)});
}

inline size_t format(char *buf, size_t bufsize, const GEO_VolumeVis &v)
{
    UT::Format::Writer	writer(buf, bufsize);
    UT::Format::Formatter<> f;
    return f.format(writer, "{}", {GEOgetVolumeVisToken(v)});
}

inline size_t format(char *buf, size_t bufsize, const GEO_VolumeVisLod &v)
{
    UT::Format::Writer	writer(buf, bufsize);
    UT::Format::Formatter<> f;
    return f.format(writer, "{}", {GEOgetVolumeVisLodToken(v)});
}

#endif // __GEO_VOLUMEOPTIONS_H_INCLUDED__
