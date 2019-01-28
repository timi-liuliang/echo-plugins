/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	FS_EventGenerator.h ( FS Library, C++ )
 *
 * COMMENTS:
 *	This class is used to define an event generator for the UI,
 *	without actually relying on the UI library.  There is a
 *	wrapper around this class that allows it to fit into the UI
 *	event loop and there is a wrapper that lets it fit into
 *	hbatch's event loop.  These wrapper classes call setInstaller()
 *	set set a callback to notify them when an event generator should
 *	be installed or uninstalled.  Note that event generators can only
 *	be installed after setInstaller() has been called.
 */

#ifndef __FS_EventGenerator__
#define __FS_EventGenerator__

#include "FS_API.h"
class FS_EventGenerator;
typedef int	(*FS_EventInstaller)(FS_EventGenerator *gen, int install);
typedef void	(*FS_PollCallback)(bool /*drain_queue*/);

class FS_API FS_EventGenerator {
public:
	     FS_EventGenerator();
    virtual ~FS_EventGenerator();

    // Method to get the class name for the event generator
    virtual const char	*getClassName() const = 0;

    // For efficiency in the UI event loop, it's advantageous to be able to
    // tell if there are actually events waiting to be processed.  There are
    // two mechanisms for doing this.
    //
    // If a file descriptor is available, the UI event loop will check to see
    // if there's any data waiting to be read.  If there is, the event
    // processor will be called.  If a negative number is returned, then we
    // cannot use this mechanism.  By default, this method assumes no file
    // descriptor exists.
    virtual int		 getFileDescriptor();

    // If file descriptor is not available, then we have to poll for events.
    // The poll time is expressed in milliseconds, so a poll time of 10 will
    // poll every 10 ms.
    //
    // Note that event generators with valid file descriptors will also poll
    // if they specify a positive poll time.  The default poll time is -1,
    // meaning that polling will not occur when the file descriptor is valid.
    virtual int		 getPollTime();

    // Return whether or not events are waiting.
    virtual bool	 areEventsWaiting() { return false; }

    // When there is either data on the file descriptor, or the poll time has
    // elapsed, the processEvent() method is called.  The processEvents method
    // should return 1 if there are futher possible events, 0 if there aren't.
    // For example, if you delete the event generator, this should return 0.
    virtual int		 processEvents() = 0;

    // These methods install the generator into the event loop or uninstall it
    // from the event loop.  Note that UI applications and hbatch both have
    // event loops. The installGenerator() command will return 0 if the
    // application does not have an event loop.
    int			 installGenerator();
    void		 uninstallGenerator();

    // This method is called by the event loop to set an installer callback
    // that is used to install FS_EventGenerators into the real event loop.
    // It should not be called by users.
    static void		 setInstaller(FS_EventInstaller installer);

    // This method is called by the event loop to set a callback.  The callback
    // is used by the event generator when it needs to explicitly drain
    // events on the queue.  It's used by the tcl and java commands while it
    // waits for the hwish and java processes to exit.
    static void		 setEventLoopPollCallback(FS_PollCallback callback);

    // Has the event loop callback been set?
    static bool		 isEventLoopPollCallbackSet();

    // This method uses the event loop poll callback to poll event generators
    // for any events that are waiting.  If the event loop callback wasn't set,
    // it won't do anything.  If drain_queue is false, events will be processed
    // and possibly added to the event queue, but the queue will not be drained.
    void		 pollEventLoop(bool drain_queue=true);

private:
};

#endif
