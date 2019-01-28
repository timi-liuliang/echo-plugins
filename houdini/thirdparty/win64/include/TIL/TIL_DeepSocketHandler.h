/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	TIL_DeepSocketHandler.h ( TIL Library, C++)
 *
 * COMMENTS:	This class handles reading the interface for reading an image
 *		from a socket.  The class will read messages from the socket
 *		and send the data to the reader.
 *		The class assumes that the initial connection has been made and
 *		that the deep reader is configured to accept data.
 */

#ifndef __TIL_DeepSocketHandler__
#define __TIL_DeepSocketHandler__

#include "TIL_API.h"
#include <UT/UT_SysClone.h>

class UT_NetSocket;
class UT_NetPacket;
class TIL_DeepReader;

class TIL_API TIL_DeepSocketHandler {
public:
     TIL_DeepSocketHandler(pid_t client,
			   UT_NetSocket *socket,
			   TIL_DeepReader *reader);
    ~TIL_DeepSocketHandler();

    int64		 getMemoryUsage(bool inclusive) const;

    int			 getFileDescriptor() const;
    UT_NetSocket	*getSocket()	{ return mySocket; }

    int		processInput(int max_packets = 1e8);
    bool	terminate(bool kill_proc = true);
    void	setMousePosition(float mx, float my, float radius);
    void	requestMaterialStyleSheet(int style_sheet_index);
    void	autoUpdate();
    void	halt();

private:
    void		 clean();
    bool		 int32Message(int32 *ival, int nints);
    bool		 fpreal32Message(fpreal32 *fval, int nfloats);

    UT_NetSocket	*mySocket;
    UT_NetPacket	*myPacket;
    TIL_DeepReader	*myReader;
    pid_t		 myPid;
    float myMouseX;
    float myMouseY;
    float myMouseRadius;
    bool                 mySwapBytes;
};

#endif
