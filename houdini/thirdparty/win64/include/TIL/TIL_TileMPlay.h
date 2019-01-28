/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	TIL_TileMPlay (C++)
 *
 * COMMENTS:	This class writes to deep raster tiles to a Net socket.
 *		
 */

#ifndef __TIL_TileMPlay_H__
#define __TIL_TileMPlay_H__

#include "TIL_API.h"
class TIL_Sequence;
class TIL_Tile;
class UT_NetPacket;
class UT_NetSocket;

#include <IMG/IMG_TileDevice.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_Lock.h>
#include <UT/UT_SysClone.h>
#include <UT/UT_StopWatch.h>

template<typename T>
class UT_Vector3T;

// The size of the UT_NetPacket buffer
static const size_t	TIL_MPLAY_PACKETSIZE = 65536;
// Tile info data structure size, as a number of integers
static const int	TIL_MPLAY_TILEDATASIZE = 5;

class TIL_API TIL_TileMPlay : public IMG_TileDevice
{
public:
	     TIL_TileMPlay(int middle, bool isflipbook);
    virtual ~TIL_TileMPlay();

    // If enabled, this will not start another MPlay session. Instead, the
    // open will fail. This is generally used for rendering sequences to a
    // single MPlay session. Interruption to that MPlay (exit, not listening)
    // will stop the sequence from rendering.
    void		 noNewMPlay(bool enable); 

    // If enabled, this class sends the PID to the host, which is
    // used to terminate this process when rendering is stopped.  If disabled, 
    // the PID is not sent and the host does not terminate the client.
    void		 setSendPIDFlag(bool flag);

    virtual const char	*className() const;
    virtual void	 getDescription(UT_WorkBuffer &wbuf) const;

    virtual int		 open(const IMG_TileOptions &finfo,
			      int xres, int yres, int twidth, int theight,
			      fpreal aspect);

    virtual int		 openMulti(IMG_TileOptionList &flist,
				   int xres, int yres,
				   int twidth, int theight, fpreal aspect);

    virtual void	 setWhitePoint(fpreal w);

    virtual void	 writeCustomTag(const char *tagname, int size,
					       const char *const *values);

    virtual int		 writeTile(const void *data, unsigned x0, unsigned x1,
						     unsigned y0, unsigned y1);
    virtual bool writeSamples(
        int nsamples,
        int floats_per_sample,
        const UT_Vector3T<int> *samples,
        const float *data);

    virtual void	 flush();
    virtual int		 close(bool keep_alive=false);

    // more convienient functions for COPs.
    int			 openDeep(const char *filename,
				  const TIL_Sequence &info,
				  int tilex, int tiley,
				  bool startnew=false,
				  const IMG_TileOptions *finfo=0);

    int			 writeDeepTile(int plane_index, const void *data,
				       int x0, int x1, int y0, int y1);
    void		 setStatusMsg(const char *msg);

    bool		 isOpen() const { return mySocket && myPacket; }
    bool		 checkInterrupt() { drainSocket(); return isOpen(); }

    virtual bool	 getMouseClickPosition(int &x, int &y) const;
    virtual float        getMouseClickRadius() const;

    void		 setMouseClickPosition(int x, int y)
			 { myMouse[0] =x; myMouse[1] =y; }

    void		 streakEdges(int x1, int y1, int x2, int y2,
				     int plane = -1);

    bool		 wasRemoteQuitRequested() { return myQuitFlag; }

    // returns the name of the lock file.
    static void		 getLockFile(UT_WorkBuffer &buf, bool isflipbook,
				    const char *label);

    virtual void	 terminateOnConnectionLost(bool);

    virtual bool	 wantsHoudiniPort() const { return true; }

    virtual bool	 isInteractive() const	{ return true; }

    static void		 enableColorCorrection(bool enable);
    static void		 setColorCorrectionLUT(bool enable, const char *lut);
    static void		 setColorCorrectionGamma(bool enable, fpreal gamma);

    virtual void        setStyleSheets(UT_StringArray& style_sheets)
			{ myStyleSheetData = style_sheets; }
private:
    int			 openSocket(const char *host, int port,
				const char *label);
    bool		 drainSocket(bool keep_alive=false);
    void		 clean();

private:
    void		 sendTileOptions(const IMG_TileOptions &options);

    TIL_Sequence	*mySequence;
    UT_Lock		 myWriteLock;
    bool		 myIsFlipbook;
    UT_IntArray		 myPlaneIndex;
    UT_IntArray		 myPlaneSize;

    UT_NetSocket	*mySocket;
    UT_NetPacket	*myPacket;
    int			 myBPP, myMiddle;
    float                myMouse[2];
    float                myMouseRadius;
    bool		 myQuitFlag;
    UT_StopWatch	 myFlushTimer;
    bool		 myNoNewMPlayFlag;
    bool		 mySendPIDFlag;

    UT_StringArray	 myStyleSheetData;
};

#endif
