#pragma once

#include <HAPI/HAPI.h>

namespace Echo
{
	// Local Session
	//	In Process
	//	Creating a Houdini Engine session within the Unreal process is how Houdini Engine is initialized by default.
	//  All the Houdini libraries and necessary files are loaded directly within process. This requires the 
	//  libraries of Houdini, its plug-ins, and their library dependencies to be compatible with the libraries used 
	//  by Unreal and its plug-ins. Otherwise, there could be dynamic library conflicts, library symbol conflicts, or 
	//  even obscure crashes.
	//
	//	Since the introduction of Houdini Engine, a lot of effort have gone into resolving and working around these 
	//  issues and conflicts.However, in some rare situations, there may still be issues that have yet to be discovered.
	void startHAPISessionInProcess(HAPI_Session& Session);

	// Remote Session
	//	Remote sessions allow you to separate the Houdini Engine process from Unreal.Using TCP Socket, Named Pipe or 
	//  domain socket even let you to host the Houdini Engine session on another machine.
	//
	//	However, when using a remote session, it is important to know the location of the assets both on the client and
	//  the server side.The asset should be located on the same path on both of them.If the server can't find the matching
	//  .hda file at the same location, the client will try to send its memory copy of the asset to the server. This allows
	//  you to use .hda files without worrying about their server location, but will not work with expanded HDAs as they 
	//  can't be copied to memory.

	
	// TCP Socket
	//  TCP Socket sessions were added when thin client was introduced in Houdini Engine.In short, thin client allows 
	//  the main Houdini Engine processing to happen in a separate process, outside of the Unreal process.For socket sessions, 
	//  a TCP socket is used to communicate between the Unreal process and the Houdini Engine process.
	//
	//	The TCP socket session supports auto - starting the Houdini Engine server.When the plug - in initializes, the Houdini
	//  Engine server will be automatically started, and a TCP connection created.

	// 	The Houdini Engine server can also be manually started with a listening socket.Then, the socket port and host is entered
	//  into the plug - in.
	void startHAPISessionTcpSocket(HAPI_Session& Session);

	// Named Pipe or domain socket
	//	Named pipe sessions were also added when thin client was introduced in Houdini Engine.In short, thin client allows the 
	//  main Houdini Engine processing to happen in a separate process, outside of the Unreal process.For named pipe sessions, 
	//  a named pipe is used to communicate between the Unreal process and the Houdini Engine process.This is the recommended 
	//  method when using thin client in the same local machine as the host server.
	//
	//	The named pipe session also supports auto - starting the Houdini Engine server.When the plug - in initializes, the Houdini 
	//  Engine server will be automatically started, and a named pipe automatically chosen.
	//
	//	The Houdini Engine server can also be manually started with a known named pipe.Then, the named pipe information is entered 
	//  into the plug - in.
	void startHAPISessionNamedPipe(HAPI_Session& Session);
}