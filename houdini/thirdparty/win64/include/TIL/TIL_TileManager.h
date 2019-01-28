/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	TIL_CacheManager.h	(Tile Image Library, C++)
 *
 * COMMENTS:
 *	Manager class for all tile access operations; manages the cache,
 *	proxy and pool managers.
 */
#ifndef TIL_TILEMANAGER_H
#define TIL_TILEMANAGER_H

#include "TIL_API.h"
class    TIL_TileCache;
class    TIL_ProxyManager;

class TIL_API TIL_TileManager
{
public:
     TIL_TileManager();
    ~TIL_TileManager();

    TIL_TileCache	*cache()	{ return myCache; }
    TIL_ProxyManager	*proxy()	{ return myProxy; }

private:
    TIL_TileCache	*myCache;
    TIL_ProxyManager	*myProxy;
};

#endif
