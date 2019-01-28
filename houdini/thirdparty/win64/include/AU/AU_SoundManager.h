/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	AU_SoundManager.h ( AU Library, C++)
 *
 * COMMENTS:
 */

#ifndef __AU_SoundManager__
#define __AU_SoundManager__

#include "AU_API.h"
#include "AU_Defines.h"

class CL_Clip;

class AU_API AU_SoundManager
{
public:

     AU_SoundManager();
    ~AU_SoundManager();

    //
    // General access methods
    //

    void		  setMute(int onoff);
    int			  getMute() const;
    void		  setMono(int onoff);
    int			  getMono() const;
    void		  setLeftVolume(float volume);
    float		  getLeftVolume() const;
    void		  setRightVolume(float volume);
    float		  getRightVolume() const;


    //
    // Test clip methods
    //
    // The test buffer allows you to load a test sound clip and
    // play portions of it with resumable interruptions.
    // Very similiar to a tape deck operation.
    //

    int			  loadTest(const CL_Clip *clip, int rewind);
    int			  unloadTest(void);

    int			  isTestActive(void) const;
    int			  rewindTest(void);
    int			  forwardTest(void);
    void		  waitTestEnd(void);

    int		  	  setTestDirection(int dir);	// -1=rev,0=stop,1=for
    int		  	  setTestLoop(int yesno);

    int		  	  setTestFull(int yesno);	// if no use range 
    void		  setTestRange(float min, float max);

    //
    // Play methods
    //
    // The play buffer loads and plays a single uninterruptable sound clip
    // It is mainly intended to play single sound effect files
    //

    int		 	  playFile(const char *filename, float l, float r);
    int			  isPlaying(void) const;

    //
    // Scrub methods
    //
    // The scrub buffer loads a sound clip which can be sampled and
    // scrubbed.
    //


    int			  loadScrub(const CL_Clip *clip);
    int			  unloadScrub(void);
    int			  isScrubbing(void) const;

    void		  scrubReset();
    void		  setScrubRealTime(int level);
    int 		  getScrubRealTime() const;
    void		  setScrubRepeat(int onoff);
    int			  getScrubRepeat() const;
    void		  setScrubSustain(float sustain);
    float		  getScrubSustain() const;
    void		  setScrubOffset(float offset);
    float		  getScrubOffset() const;
    void		  setScrubFrequency(int frequency);
    int 		  getScrubFrequency() const;
    void		  setScrubDeltas(float fore, float back);
    void		  getScrubDeltas(float &fore, float &back) const;
    void		  setScrubIndex(float index);
    float	  	  getScrubIndex() const;
    void		  setScrubSmooth(int smooth);
    int			  getScrubSmooth() const;
    void		  setScrubReverse(bool is_reverse);
    bool		  getScrubReverse() const;
    void		  setScrubOneFrame(bool is_oneframe);
    bool		  getScrubOneFrame() const;

    //
    // Realtime methods
    //

    int			  loadRealtime(const CL_Clip *clip, int start,int end,
				       float queue);
    void		  startRealtime(int rate, int channels,float queue);
    void		  stopRealtime();
    int			  isRealtime(void) const;

    //
    // Misc methods
    //

    void		  getMaxLevel(float *l, float *r) const; // meter
    void		  shutDown();	// unload test/scrub buffers
    int			  isAUDeviceOpen()  const;// query 
    void		  loseAUInterest() const;// reset
    int			  obtainAUDevice() ;	  // get
    int			  releaseAUDevice();	  // release audio device

protected:

private:

    int		  	  sendAudioCommand(int command);
    int			  loadFromClip(const CL_Clip *clip);
    void		  loadSegmentFromClip(const CL_Clip *clip, 
					      float *data[AU_MAX_CHANNELS],
					      int start, int end,
					      int &length);

};


AU_API extern AU_SoundManager *AUgetSoundManager(); // Get sound interrupt ptr

#endif
