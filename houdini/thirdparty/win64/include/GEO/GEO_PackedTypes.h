/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GEO_PackedTypes.h (GEO Defered Primitive Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GEO_PackedTypes__
#define __GEO_PackedTypes__

#include "GEO_API.h"
#include <SYS/SYS_Types.h>

enum GEO_AnimationType
{
    GEO_ANIMATION_INVALID = -1,
    GEO_ANIMATION_CONSTANT,	// Constant animation (i.e. no animation)
    GEO_ANIMATION_TRANSFORM,	// Only transform is animated
    GEO_ANIMATION_ATTRIBUTE,	// Attribute or transform are animated
    GEO_ANIMATION_TOPOLOGY,	// Topology is animated (changes)
};

enum GEO_ViewportLOD
{
    GEO_VIEWPORT_INVALID_MODE = -1,
    GEO_VIEWPORT_HIDDEN,
    GEO_VIEWPORT_CENTROID,
    GEO_VIEWPORT_BOX,
    GEO_VIEWPORT_POINTS,
    GEO_VIEWPORT_FULL,
    GEO_VIEWPORT_SUBDIVISION,
    GEO_VIEWPORT_DEFORM,

    GEO_VIEWPORT_NUM_MODES,
};

GEO_API extern const char	*GEOviewportLOD(GEO_ViewportLOD type, 
                         	                bool label=false);
GEO_API extern GEO_ViewportLOD	 GEOviewportLOD(const char *type);
GEO_API extern const char	*GEOanimationType(GEO_AnimationType type);
GEO_API extern GEO_AnimationType GEOanimationType(const char *type);

GEO_API size_t format(char *buf, size_t bufsize, const GEO_ViewportLOD &lod);
GEO_API size_t format(char *buf, size_t bufsize, const GEO_AnimationType &type);

#endif
