/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_NetFDSet.h ( UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_NetFDSet__
#define __UT_NetFDSet__

#include "UT_API.h"
#include "UT_UniquePtr.h"
#include <stddef.h>


// ============================================================================
/// Types of IO events that UT_NetFDSet can monitor a file descriptor for.
enum class UT_NetFDEvent
{
    NETFD_READ,
    NETFD_WRITE,
    NETFD_ERROR
};

// ============================================================================
/// Abstracts the socket file descriptor set for select() or poll() calls.
class UT_API UT_NetFDSet
{
public:
    /// Constructor and desctructor.
     UT_NetFDSet();
    ~UT_NetFDSet();

    /// Adds a file descriptor to the set with the intent of monitoring 
    /// the given IO event.
    void		add(int fd, UT_NetFDEvent event);

    /// Test whether a file descriptor is in the set
    bool		contains(int fd) const;

    /// Removes all added file descriptors for all events (resets the set).
    void		removeAll();

    /// Checks or waits for the IO events on the file descriptors in the set.
    /// Essentially it's a wrapper for select() or poll().
    /// @param timeout_ms The timeout to wait for connection (in miliseconds).
    ///		If zero, the call does not wait for the events, but instead
    ///		checks for the current state of the FDs in the set.
    ///		If negative, the call blocks indefinitely, waiting for the
    ///		first event. This is equivalent to passing NULL to select() or 
    ///		-1 to poll() as time value.
    /// @param force_socket On Windows, indicates to use sockets rather than
    ///		resorting to pipes.
    /// 
    /// @return The total number of file descriptors on which there was
    ///		activitiy (events of the registered type), or -1 if error
    ///		occured. If time limit expires, returns 0.
    int			checkEvents(int timeout_ms, 
				bool force_socket = false) const;

    /// Constants denoting special wait times for checkEvents()
    static const int INDEFINITE_WAIT;	// blocking indefinitely for events
    static const int NO_WAIT;		// returning immediately

    /// Returns true if the set contains no file descriptors.
    bool		isEmpty() const;

    /// Returns true if there was any IO activity of the given type 
    /// on the given file descriptor.
    bool		isActive(int fd, UT_NetFDEvent event) const;

private:
    /// Delegate that implements actual IO check (eg, select() or poll()).
    static const size_t SIZEOF_IMPL = 1576;
    char		myImpl[SIZEOF_IMPL];
};


#endif

