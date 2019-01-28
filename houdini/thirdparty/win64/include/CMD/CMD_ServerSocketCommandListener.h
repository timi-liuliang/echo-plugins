/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CMD_ServerSocketCommandListener.h ( CMD Library, C++ )
 *
 * COMMENTS:
 *	This event generator opens a socket an waits for connections.
 *	When a client makes a connection and sends a command, that
 *	command is run and the command output is sent back.
 *	Note that multiple active connections can exist at once.
 */

#ifndef __CMD_ServerSocketCommandListener_h__
#define __CMD_ServerSocketCommandListener_h__

#include "CMD_API.h"
#include <FS/FS_ServerSocketListener.h>
#if !defined(GAMEOS)
#include <UT/UT_Spawn.h>
#endif

class UT_WorkBuffer;
class CMD_Manager;

class CMD_API CMD_ServerSocketCommandListener : public FS_ServerSocketListener
{
public:
    // Use this static method to create a instance of this class.
    // Note that this method might return null if the socket could not be
    // created (eg. if the port is already in use).
    //     separate_errors: if set, error output will be separated from
    //         regular output by ~~~~.
    //     restrict_to_safe_commands: if set, "unsafe" commands will not be
    //         executed.
    //     ip_mask: restricts who is allowed to connect to this socket.  If
    //         it is null, it defaults to +.+.+.+.
    //     remap_privileged_ports: if set, port numbers will be mapped into
    //         the range of unprivileged port numbers.
    static CMD_ServerSocketCommandListener *createCommandListener(int port,
				CMD_Manager& command_manager,
				bool separate_errors = false,
				bool restrict_to_safe_commands = false,
				const char *ip_mask = 0,
				bool remap_privileged_ports = true,
				bool browser_mode = false);

    virtual const char	*className() const
			    { return "CMD_ServerSocketCommandListener"; }

    bool		 getRestrictToSafeCommands() const
			    { return myRestrictToSafeCmds; }
    void		 setRestrictToSafeCommands(bool restrict_to_safe_cmds)
			    { myRestrictToSafeCmds = restrict_to_safe_cmds; }

private:
    // This private constructor is called by the createCommandListener()
    // static method.
    CMD_ServerSocketCommandListener(UT_NetSocket *server_socket,
				    int requested_port,
				    CMD_Manager &command_manager,
				    bool separate_errors,
				    bool restrict_to_safe_commands,
				    const char *ip_mask,
				    bool browser_mode);

    // This method is overriden so we can create a
    // UI_ConnectSocketCommandListener to listen for commands from an active
    // connection.
    virtual void	 createConnectedSocketListener();


    // Data:

    CMD_Manager	&myCommandManager;
    bool	 mySeparateErrors;
    bool	 myRestrictToSafeCmds;
    bool	 myBrowserMode;
};

#endif
