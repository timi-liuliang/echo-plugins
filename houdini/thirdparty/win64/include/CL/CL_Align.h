/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CL_Align.h ( Clip Library, C++)
 *
 * COMMENTS:
 *
 *	Aligns a collection.
 */

#ifndef __CL_Align__
#define __CL_Align__

#include "CL_API.h"
#include "CL_Clip.h"
#include <SYS/SYS_Types.h>

enum CL_Alignment { NO_ALIGN =0,
		    STRETCH_TO_FILL,
		    ALIGN_TO_START,
		    ALIGN_TO_END,
		    SHIFT_TO_FIRST,
		    TRUNC_TO_FIRST,
		    STRETCH_TO_FIRST,
		    TRUNC_TO_MIN,
		    SQUASH_TO_MIN     };

class CL_API CL_Align
{
public:

			CL_Align(fpreal current_time);
			~CL_Align();


    int			getAlignment(CL_TrackListC &toAlign, 
				     CL_Alignment align,
				     fpreal &start, fpreal &end);

    // Sets the start, length and rate on the 'dest' clip, adds all the tracks
    // in toAlign and aligns them.
    int			align(CL_TrackListC &toAlign,CL_Clip *dest,
			      CL_Alignment align, fpreal rate,
			      fpreal *fstart = 0, fpreal *fend = 0);

    // This version sets the start, length and rate on the clip, but does not
    // add tracks.
    int			alignOnly(CL_TrackListC &toAlign, CL_Clip *dest,
				  CL_Alignment align, fpreal rate);
    
    // after a call to getAlignment this method will return 1 if
    // aligning will produce new data, zero otherwise.

    int			needsAlignment(fpreal rate);

    // this version returns the aligned data of
    // track "source" in the array "data". Used for extracting data one
    // track at a time. Always call getAlignment() first.
    
    int			align(const CL_Track *source,
			      CL_Alignment align,
			      fpreal *dest, fpreal rate);

    // this version of align does the same as above, but for a short
    // segment, rather than the full interval. segstart and segend are
    // in seconds, and are "post-alignment" times
    int			align(const CL_Track *source,
			      CL_Alignment align,
			      fpreal *dest, fpreal rate,
			      fpreal segstart, fpreal segend);

    // align() or getAlignment must be called first. This returns the number
    // of samples that the alignment, sample rate and sources would produce.
    int			getNumSamples() const { return myNumSamples; }
private:

    fpreal		getClipStartTime(const CL_Clip *c)const;
    fpreal		getClipEndTime(const CL_Clip *c)const;
    fpreal		findEarliestStart() const;
    fpreal		findLatestEnd() const;
    fpreal		findLatestStart() const;
    fpreal		findEarliestEnd() const;
    
    const CL_Clip	*findLongestClip(fpreal *cliplen=0) const;
    const CL_Clip	*findShortestClip(fpreal *cliplen=0) const;

    void		alignNone(CL_Clip *dest,fpreal start,fpreal end);
    
    void		alignStart(CL_Clip *dest,fpreal length);
    void		alignEnd(CL_Clip *dest,fpreal length);
    
    void		alignStretch(CL_Clip *dest);


    CL_TrackListC      *myTracks;
    int  		myNumSamples;
    fpreal		myStart;
    fpreal		myEnd;
    fpreal		myCurrentTime;
};

#endif
