/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_TextureUtils.h ( GU Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GU_TextureUtils__
#define __GU_TextureUtils__

#include "GU_API.h"

#include <SYS/SYS_Types.h>
class GU_Detail;
class GA_PrimitiveGroup;

/// GUfixVertexTextureSeams() will look for "seams" uv's and try to fix them.
///
/// Seams in textures occur when the texture coordinates "wrap" in periodicity.
/// For example, when doing polar mapping, at one meridian, the u-coordinates
/// will go from 0.9 to 0.1.  So, in this case, rather than mapping from 0.9 to
/// 0.1, the texture engine will interpolate between .1 and .9 and the entire
/// map will appear (reversed) in that span.  This code simply looks for faces
/// which have a delta in coordinates larger than the uv_span passed in and
/// will then attempt to fix this by adjusting coordinates.  In the above case,
/// the coordinates would be set to either (-0.1, 0.1) or (0.9, 1.1).
///
/// This is only applicable to vertex texture coordinates since point textures
/// are shared between primitives and fixing for one primitive would likely
/// break another primitive (which shared points).
GU_API bool	GUfixVertexTextureSeams(GU_Detail &gdp,
				    fpreal uv_span = 0.25,
				    const char *attrib="uv",
				    const GA_PrimitiveGroup *group = NULL);

#endif

