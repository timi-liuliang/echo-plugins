/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_VoxelArrayJSON.h ( UT Library, C++)
 *
 * COMMENTS:
 * 	A class to hide the static data used by UT_VoxelArray during JSON
 * 	writing/parsing.  We don't want to include this data directly in
 * 	UT_VoxelArray.C as that file is included in UT_VoxelArray.h.
 */

#ifndef __UT_VoxelArrayJSON__
#define __UT_VoxelArrayJSON__

#include "UT_API.h"

#include "UT_JSONParser.h"

class UT_API UT_VoxelArrayJSON
{
public:
    enum ArrayTokenID {
	ARRAY_CONSTARRAY,
	ARRAY_TILEDARRAY,
	ARRAY_SHAREDARRAY,
	ARRAY_VERSION,
	ARRAY_COMPRESSIONTYPES,
	ARRAY_TILES
    };

    enum TileTokenID {
	TILE_COMPRESSION,
	TILE_DATA
    };

    static const char	*getToken(ArrayTokenID id);
    static const char	*getToken(TileTokenID id);

    static int		 getArrayID(const char *symbol);
    static int		 getTileID(const char *symbol);
};

#endif

