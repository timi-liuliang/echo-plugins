/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_Sync.h ( RE Library, C++)
 *
 * COMMENTS:
 *    Implements the GL_ARB_sync extension, which can be used to wait until
 *    a certain GL command has been processed by the server. Either the client
 *    or the server can wait. In the case of the client, the thread making
 *    GL calls will be suspended until the sync command is processed. For the
 *    Server, the GL command stream is suspended until the sync command is
 *    processed.
 *	
 *    Sync objects can be used to synchronize multiple contexts as well,
 *    which is useful for the loader/renderer paradigm.
 *
 *
 *    RE_RenderWait implements a client sync.
 *    RE_CommandSync implements a server sync.
 */
#ifndef RE_Sync_h
#define RE_Sync_h

#include <SYS/SYS_Types.h>
#include "RE_API.h"

class re_OGLSync;
class RE_Render;

class RE_API RE_RenderWait
{
public:
	    RE_RenderWait();
	   ~RE_RenderWait();

   // Inserts a sync command into the GL render stream.
   bool    insertSyncPoint(RE_Render *r);

   // Checks to see if a sync point has been created. Useful for syncing
   // multiple threads, as the client thread may not have created the sync yet
   // that another thread waits on.
   bool	   doesSyncPointExist() const		{ return mySyncObject != NULL; }
   
   // All of the wait and sync methods returns true if the sync point was
   // reached. They will return false if an error occurred, if the timeout
   // expires, or if the sync has not be already set with insertSyncPoint().

   // Checks if the point has been reached, or returns false immediately.
   bool	   poll(RE_Render *r);
   
   // Waits indefinitely for the sync point to be reached.
   bool	   wait(RE_Render *r);

   // Waits until sync point is hit or the timeout expires. 
   bool	   wait(RE_Render *r, uint64 timeout_in_ns);

   // resets the sync object for reuse, so that previous results aren't
   // mistakenly interpreted as complete.
   void	   reset();

private:
    re_OGLSync *mySyncObject;
};


class RE_API RE_CommandSync
{
public:
		RE_CommandSync();
	       ~RE_CommandSync();

    // Inserts a sync point into the command stream. Will return false if the
    // sync extension is not supported, otherwise true.
    bool	insertSyncPoint(RE_Render *r);

    // Inserts a wait point into the command stream, which waits until the
    // previous sync point has completed. Will return false if no sync point
    // was previously set.
    bool	insertWaitPoint(RE_Render *r);

    void	reset();
private:
    re_OGLSync *mySyncObject;
};

#endif
