/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CMD_ConnectedSocketCommandListener.h ( CMD Library, C++ )
 *
 * COMMENTS:
 *	This event generator listens for commands that are sent to a
 *	connected socket and runs them.  If the socket is closed, this event
 *	generator removes and deletes itself.  You must create this event
 *	generator with new and you must not keep a pointer to it.
 *
 *	You probably don't want to use this class directly.  Instead, you
 *	probably want to use CMD_ServerSocketCommandListener.
 */

#ifndef __CMD_ConnectedSocketCommandListener_h__
#define __CMD_ConnectedSocketCommandListener_h__

#include "CMD_API.h"
#include <FS/FS_ConnectedSocketListener.h>
class CMD_Manager;
class UT_WorkBuffer;
class UT_XMLNode;

class CMD_API CMD_ConnectedSocketCommandListener
    : public FS_ConnectedSocketListener
{
public:
    // The socket is used to write the command result back to the other end of
    // the socket.  The ip mask restricts who is allowed to connect to this
    // socket.  If the ip mask is null it defaults to +.+.+.+.
    CMD_ConnectedSocketCommandListener(UT_NetSocket &server_socket,
				       CMD_Manager &command_manager,
				       bool separate_errors = false,
				       bool restrict_to_safe_commands = false,
				       const char *ip_mask = 0,
				       bool browser_mode = false);

    virtual const char	*className() const
				{ return "CMD_ConnectedSocketCommandListener"; }

private:
    // This static callback is called when data is available on the socket.
    // It will call processSocketData().
    static void	 processSocketDataCallback(
	void *opaque_object,
	const char *data,
	int length,
	UT_NetSocket &socket);

    // Process data received a socket (optionally reading in more data) and
    // return the result on the socket.
    void	 processSocketData(
		    const UT_WorkBuffer &command, UT_NetSocket &socket);

    void	 runHscriptCommand(
		    const char *command, UT_WorkBuffer &result);
    void	 runPythonExpression(
		    const char *expression, UT_WorkBuffer &result);
    void	 runPythonStatements(
		    const char *statements, UT_WorkBuffer &result);

    void	 processXML(const char *xml, UT_WorkBuffer &result);
    void	 processXMLNode(UT_XMLNode &node, UT_WorkBuffer &result);

    void	 processData(const char *data, UT_WorkBuffer &result);

    void	 processHTTPRequest(
		    const UT_WorkBuffer &request,
		    UT_NetSocket &socket,
		    UT_WorkBuffer &result);

    // Data:

    CMD_Manager	&myCommandManager;
    bool	 mySeparateErrors;
    bool	 myRestrictToSafeCmds;
    bool	 myBrowserMode;
};

#endif
