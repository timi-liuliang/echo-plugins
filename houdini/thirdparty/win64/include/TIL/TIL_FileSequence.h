/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	TIL_FileSequence.h
 *
 * COMMENTS:
 *	Class for deal with sequences of images (determining start/end, etc).
 */
#ifndef TIL_FILE_SEQUENCE_H
#define TIL_FILE_SEQUENCE_H

#include "TIL_API.h"
#include <sys/types.h>
#include <UT/UT_String.h>
#include <UT/UT_Rect.h>
#include <UT/UT_BitArray.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_Array.h>
#include <UT/UT_IntArray.h>

#include "TIL_Defines.h"

class IMG_Stat;
class TIL_Sequence;
class til_FrameInfo;

class TIL_API TIL_FileSequence
{
public:
     TIL_FileSequence();
    ~TIL_FileSequence();

    int64 getMemoryUsage(bool inclusive) const;

    TIL_FileSequence &operator=(const TIL_FileSequence &);

    // returns 1 if the pattern is different than the current pattern, else 0.
    int		 setNewPattern(const char *filepattern);

    // fills the sequence with the start/end range, size, data type, etc.
    // returns 1 if successful, and 0 if the sequence doesn't exist.
    int		 getFileInfo(TIL_Sequence &seq_info,
			     int override_size = 0,
			     int override_range = 0,
			     TIL_DataFormat *depth_override = 0,
			     int use_rgba = 0,
			     bool useframe = false, int frame = 0,
			     bool clearseq = true,
			     bool coloronly = false,
			     bool fractional = false);

    int		 getSingleFileInfo(const char *name, TIL_Sequence &seq_info,
				   bool override_size = true,
				   TIL_DataFormat *override_depth = 0,
				   int use_rgba = 0,
				   bool clearseq = true,
				   bool coloronly = false);

    // returns the frame range. The filename is only used if the frame range
    // hasn't been determined yet (by calling getFileInfo). 
    bool	 getFrameRange(int &start, int &end, bool fractional=false);

    // gets the filename at a specific frame. Returns 0 if the frame doesn't
    // exist. If frame == -MAX_INT, the first frame is returned.
    bool	 getFilename(UT_String &name, int frame = 0x80000000);

    bool	 doesFrameExist(const UT_String &framename) const;
    bool	 doesFrameExist(int frame) const;
    int		 getClosestFrame(int to_frame) const;
    int		 getClosestPrevFrame(int to_frame) const;
    int		 getClosestNextFrame(int to_frame) const;

    int		 getPlaneIndex(const char *name) const;

    PXL_ColorSpace getPlaneColorSpace(int plane_index) const;
    fpreal	   getPlaneColorSpaceGamma(int plane_index) const;

    // attempts to determine the frame step of the files by analyzing the gaps.
    // only call after getFileInfo.
    int		 getFrameStep();

    // call this to determine whether or not the specified sequence was
    // only a single frame.  Only call this after first calling
    // getFileInfo() or getSingleFileInfo().
    bool	 isSingleFrame() const { return myFrameSingle ? 1 : 0; }

    int		 getNumFrames() const { return myFrames.entries(); }
    
    // call this to determine whether the sequence is really a movie
    // file which contains the frames.  Only call this after first 
    // calling getFileInfo() or getSingleFileInfo().  Also note that
    // this is the only method of this class that understands movie
    // files because they aren't file sequences.  You should use the
    // API on IMG_File to query and read frames.
    bool	 isMovieFile() const;

    bool	 isTopFirstImage() const { return myIsTopFirst; }
    IMG_ColorModel getColorModel() const { return myColorModel; }

    // returns true if the digits was set by the user (ie $4F), and sets the
    // digits parm to the number of file index digits. If nothing was specified
    // this returns false.
    bool	 getFileDigits(int &digits) const;
    void	 setFileDigits(int digits);

    int		 getFractionFrameIndex(float frame) const;

    UT_InclusiveRect getBounds(int frame, bool &streak);
    UT_InclusiveRect getBounds(const char *filename, bool &streak);

    fpreal getRenderTime(int frame) const;
    int64 getRenderMemory(int frame) const;

    // Removes the frame from the list.
    void	 markBadFrame(int frame);
    // resets the list of bad frames.
    void	 resetBadFrames();

    // returns the list of bad frames.
    const UT_IntArray &getBadFrames() const { return myBadFrames; }

    void	 setComment(const UT_String &comment, int frame);
    void	 getComment(UT_String &comment, int frame) const;

    static int getInfoForFilename(const char *name, TIL_Sequence &seq_info,
				    bool override_size = true,
				    TIL_DataFormat *override_depth = NULL,
				    bool use_rgba = false,
				    bool clearseq = true,
				    bool coloronly = false,
				    bool *topfirst = NULL,
				    IMG_Stat **stat = NULL);

private:
    int		 lookupRange(char sepchar, bool fractional);
    void	 removeBadFrames();
    void	 reorganizeFractions();
    void	 clearBoundsCache();
    void         clearRenderTimesCache();
    bool         prepRenderTimeCache(int &frame) const;
    UT_InclusiveRect getBoundsByIndex(int index, bool &streak);
    
    static int	 compareFileToRange(const char *filename, void *me);
    static int	 checkExist(const char *filename, void *me);
	
    UT_String			 myFilePattern;
    UT_String			 myMatchPattern;
    int				 myFileNumberDigits;
    IMG_Stat			*myStat;
    IMG_ColorModel		 myColorModel;
    bool			 myIsTopFirst;

    // file range lookup info
    int				 myFrameStart;
    int				 myFrameSingle;
    int				 myFrameEnd;
    int				 myFrameInit;
    int				 myFrameHasFraction;
    float			 myFrameFractionScale;
    bool			 myFractional;
    UT_ValArray<til_FrameInfo *> myFrames;
    UT_IntArray			 myBadFrames;
    UT_String			 myBase, mySuffix;
    UT_String			 myPath;
    bool			 myUserSpecifiedDigits;
    UT_BitArray			 myCachedBoundsFrame;
    UT_Array<UT_InclusiveRect>   myCachedBounds;
    UT_BitArray			 myCachedBoundsStreak;
    mutable UT_Lock              myCachedRenderTimesLock;
    mutable UT_BitArray          myCachedRenderTimesFrame;
    mutable UT_Array<fpreal>     myCachedRenderTimes;
    mutable UT_Array<int64>      myCachedRenderMemories;
};

#endif
