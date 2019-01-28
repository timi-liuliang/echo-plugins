/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SYS_Socket.h (SYS Library, C++)
 *
 * COMMENTS:	Platform-independent header for socket related functionality.
 *		Only include this in .C files since it pulls in <windows.h> !
 */

#ifndef __SYS_SOCKET_H_INCLUDED__
#define __SYS_SOCKET_H_INCLUDED__

#include "SYS_API.h"

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/select.h>
#include <sys/socket.h>
#endif

#endif // __SYS_SOCKET_H_INCLUDED__
