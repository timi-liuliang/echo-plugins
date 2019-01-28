/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	TIL_ProxyManager.h	(Tile Image Library, C++)
 *
 * COMMENTS:
 *	This class is a multithreaded proxy manager (disk cache manager). 
 */

#ifndef TIL_PROXYMANAGER_H
#define TIL_PROXYMANAGER_H

#include "TIL_API.h"
#include <UT/UT_LinkList.h>
#include <UT/UT_String.h>
#include "TIL_Defines.h"

class UT_Thread;
class TIL_Tile;
class til_ProxyCommand;
class TIL_ProxyState;

class TIL_API TIL_ProxyManager
{
public:

	TIL_ProxyManager();
       ~TIL_ProxyManager();

    // Proxy manager commands.
    int			readProxy (const char *cop_path, TIL_Tile *into_tile,
				   int xres, int yres);
    void		writeProxy(const char *cop_path, TIL_Tile *from_tile,
				   int xres, int yres);
    
    void		clearCopProxies(const char *cop_path);
    void		clearAllProxies();

    void		init();

    // Proxy Options.
    void		setProxyGeneration(int on);
    void		setProxyDir(const char *dir);
    void		setProxyLimit(int on);
    void		setProxyDiskLimit(int size);
    void		setProxyCrossPlatform(int enable);

    int			getProxyGeneration() const;
    int			getProxyLimit() const;
    int			getProxyDiskLimit() const;
    const char		*getProxyDir() const;
    int			getProxyCrossPlatform() const;
    
private:
    void		launchManagerProcess();
    static void *	threadProcess(void *data);

    // Separate thread functions:
    void		proxyManagerLoop();

    // command execution
    int			executeNextCommand();

    // if disk space limiting is on, gotta garbage collect...
    void		garbageCollect(int limit);

    // determine the state of all the proxies on disk, called once at startup
    void		determineProxyState(const char *proxy_dir);

    // the actual operations decoupled from the program calls. 
    void		doWriteTile(const char *path, TIL_Tile *tile,
				    int xres, int yres);
    void		doClearCopProxies(const char *path);
    void		doClearAllProxies();

    // directory validation & creation... returns 0 if the path can't be
    // created.
    int			ensurePathExists(const char *path);

    void		addProxyState(TIL_ProxyState *);
    TIL_ProxyState *	usedProxyState(TIL_ProxyState *);
    void		removeProxyState(TIL_ProxyState *);
    
private:
    UT_LinkList		myCommandList;

    // options
    int			myProxyEnable;
    int			myProxyLimit;
    int			myProxyDiskLimit;
    UT_String		myProxyUserDir;
    UT_String		myProxyDir;
    int			myProxyCrossPlatform;

    // proxy management stuff.
    UT_String		myProxyIndexFile;
    UT_String		myProxyDataFile;

    UT_LinkList		*myProxyStateQueue;
    TIL_ProxyState	*myProxyStateTable[PROXY_TABLE_SIZE];
    int			 myTotalProxySize;
    
    // multithread support
    UT_Thread		*myThread;
    UT_Lock		 myCommandLock;
    UT_Lock		 myCurrentPathLock;
    UT_Lock		 myIndexFileLock;
    UT_Lock		 myReadLock;
    const char		*myCurrentReadPath;
    const char		*myCurrentWritePath;

    TIL_ProxyManager	*myThis;
};

#endif
