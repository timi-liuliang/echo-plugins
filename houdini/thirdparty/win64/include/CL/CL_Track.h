/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CL_Track.h ( Clip Library, C++)
 *
 * COMMENTS:
 *		A set of raw data
 *
 */


#ifndef __CL_Track__
#define __CL_Track__

#include "CL_API.h"
#include <UT/UT_ValArray.h>
#include <UT/UT_FloatArray.h>
#include <UT/UT_StringHolder.h>
#include <SYS/SYS_Types.h>

class   UT_IStream;
class	UT_OStream;
class	CL_Clip;
class	CL_Track;
class	CL_SubRange;

typedef UT_ValArray<CL_Track *>   	CL_TrackList;
typedef UT_ValArray<const CL_Track *>   CL_TrackListC;

//
// how to evaluate the track outside its defined range
//

typedef enum {
    CL_TRACK_HOLD,	// Track is start/end value before/after
    CL_TRACK_SLOPE,	// Track extends at start/end slope
    CL_TRACK_CYCLE,	// Track wraps to defined range (repeat)
    CL_TRACK_MIRROR,	// Track wraps+reflects to defined range
    CL_TRACK_DEFAULT,	// Track is default value beyond defined range
    CL_TRACK_CYCLE_STEP // Track is cycled and advanced
} CL_TrackOutside;

enum CL_TrackLimitMethod
{
    CL_TRACK_LIMIT_OFF,
    CL_TRACK_LIMIT_CLAMP,
    CL_TRACK_LIMIT_LOOP,
    CL_TRACK_LIMIT_ZIGZAG
};

enum CL_QuantizeValueMethod
{
    CL_QUANTIZE_VALUE_OFF,
    CL_QUANTIZE_VALUE_CEILING,
    CL_QUANTIZE_VALUE_FLOOR, 
    CL_QUANTIZE_VALUE_ROUND
};

enum CL_TrackResampleMethod
{
    CL_TRACK_INTERP_NONE,
    CL_TRACK_INTERP_LINEAR,
    CL_TRACK_INTERP_CUBIC,
    CL_TRACK_INTERP_EDGE_PRESERVE
};


class CL_API CL_Track {

public:

     CL_Track(CL_Clip &dad, const CL_Track &from);
     CL_Track(CL_Clip &dad, const UT_StringHolder &name);
    ~CL_Track();


    CL_Track		&operator= (const CL_Track &from);

    void		 init(CL_Clip &dad, const UT_StringHolder &name);

    fpreal		*getData()
			 { return myData.array(); }

    const fpreal	*getData() const
			 { return myData.getRawArray(); }

    int			 getTrackLength() const
			 { return myData.entries(); }

    const UT_StringHolder &getName() const
			 { return myName; }

    //
    // Warning this method must be used with extreme caution
    void		 setName(const UT_StringHolder &name);

    // These methods are used for special cases, to access tracks more quickly.
    // Normally a symboltable in CL_Clip is used to track access by name.
    unsigned short	 getNameHash() const
			 { return (unsigned short)myName.hash(); }

    unsigned short	 getNameBaseLength() const
			 { return myNameBaseLength; }

    unsigned short	 getNameBaseHash() const
			 { return myNameBaseHash; }

    int			 getNameNumber() const
			 { 
			     if (myNameNumberDirty)
				 ((CL_Track *)this)->calcNameNumber();
			     return myNameNumber; 
			 }

    static void		 getNameComposition(const UT_StringRef &name,
					    unsigned short &base_length,
					    unsigned short &base_hash);


    const char		*info(fpreal t, const char *prefix = 0,
	    		      const char *suffix = 0) const;

    int			 isTimeDependent() const;

    int			 isWithinRange(fpreal low,fpreal hi,int completely=1) const;

    // these methods define the evaluation of the array outside
    // its defined interval

    void		setLeft(CL_TrackOutside type);
    void		setRight(CL_TrackOutside type);
    void		setDefault(fpreal value);

    CL_Clip		*getClip(void)
			 { return myParent; }

    const CL_Clip	*getClip(void) const
			 { return myParent; }


    const char		*getLeft(void) const;
    const char		*getRight(void) const;

    int64		 getMemoryUsage(bool inclusive) const;

    void		 getExtendCond(CL_TrackOutside &left, 
				       CL_TrackOutside &right) const;
    fpreal		 getDefault() const
			 { return myDefault; }

    void		 dupAttributes(const CL_Track *,
				       int data_too=0, int name_too=0);

    // data methods

    void		 reverse();
    void		 constant(fpreal value=0);
    void		 cycle(int howmany);

    int			 convolve(const CL_Track &t1, const CL_Track &t2,
	    			  int add = 0);

    int			 convolveCenter(fpreal *data2, int n2, int passes = 1);
    int			 convolveCenterDirect(fpreal *data2, int n2, int passes);
    int			 convolveCenterFFT(fpreal *data2,int n2,int passes);

    int			 copyStamp(const CL_Track &t1, fpreal toffset, int clr);

    int			 getThresholdTimes(fpreal thresh, 
					   UT_FprealArray &toffsets,
					   UT_FprealArray &values) const;

    void		 despike(fpreal tol, fpreal effect=1.0, int maxwidth=1);

    void		 blend(fpreal start, const fpreal *values, int len,
	    		       fpreal base, const CL_SubRange *effect,
			       fpreal cycle);

    void		 blend(fpreal start, const fpreal *values, int len,
	    		       fpreal base, const CL_Track *effect_track,
			       fpreal cycle);

    void 		 limitValues(fpreal max, fpreal min, 
	    			     CL_TrackLimitMethod method, 
				     int start, int end);

    static fpreal	 limitValue(fpreal max, fpreal min,
	    			    CL_TrackLimitMethod method, fpreal v);

    // Resample from the given source into ourselves

    int			 resample(const fpreal *values, int num, 
	    			  CL_TrackResampleMethod method, 
				  int constant_area, int iscycle = 0, 
				  fpreal cycle = 0);

    // Resample from ourselves into the given destination

    int			 resample(fpreal *data, int size, int start, int end,
				  CL_TrackResampleMethod method,
				  int constant_area,
				  int iscycle = 0,
				  fpreal cycle = 0) const;

    void		 quantizeValue(fpreal max, fpreal min,
				       CL_QuantizeValueMethod method,
				       fpreal step, fpreal offset,
				       int clamp, int start, int end);

    void		 normalize();
    void		 absolute();
    void		 smoothRotate(fpreal delta = 180.0);


    // Filter size must be a power of 2, and the filter itself should be
    // in the form of a DFT filter
    // the callback function takes parms 'node' 'size', 'freq step'
    //   'mag filter' 'phase filter'
    // 'index' and 'chunk' -- mag & phase filter are the arrays to be filted
    // by the callback function. Called before filtering each chunk, if
    // not null.
    void		 filterFrequency(int filter_size,
					 fpreal freqstep,
					 fpreal overlap,
					 fpreal discard,
					 fpreal *filter,
					 fpreal *phase_filter = 0,
					 void (*filter_design)(void *,int,fpreal,fpreal *,fpreal *,int,int)=0,
					 void *node = 0,
					 int	doblend = 1);

    
    void		 pitchShift(fpreal shift,int chunk,
				    const CL_Track *animate=0,
				    fpreal (*callback)(void *,int,int)=0,
				    void *node=0);

    //	This actually is a DFT followed by a multipy, and an IDFT
    void		 fastConvolve(int chunk,fpreal *data);


    // This method allows this track to assume the same data array as 'track'.
    // They will share the same buffer until unalias() is called. aliasTrack()
    // may be called multiple times before unalias() is called. Both tracks
    // must have the same number of samples.
    void		 aliasTrack(CL_Track &track);
    void		 unalias();
    bool		 isAliased() const { return myPrevDataAlias != 0; }

    // swaps data with 'track'.
    void		 swapData(CL_Track &track);

    int getIndex() const;

    static UT_StringHolder fixName(const UT_StringHolder &old);

private:

    // should only be called by CL_Clip

    void		 evaluate(fpreal start_index, fpreal stop_index,
		                  fpreal *data, int size) const;


    // this single version is much less efficient than the range methods
    fpreal		 evaluateSingle(fpreal index) const;

    // optimizied evaluations which work on a range
    // entire range is evaluated with a particular extend condition

    void		 evaluatePortion(fpreal i0, fpreal i1, 
	    				 fpreal start_index, fpreal
		                         stop_index, fpreal *data, int size, 
					 fpreal *fix, int portion) const;

    void		 evaluateCenter(fpreal start_index, fpreal stop_index,
		                        fpreal *data, int size) const;
							     
    void		 evaluateLeftSlope(fpreal start_index, fpreal stop_index,
		                           fpreal *data, int size) const;
    void		 evaluateRightSlope(fpreal start_index, fpreal stop_index,
		                            fpreal *data, int size) const;

    void		 evaluateLeftHold(fpreal *data, int size) const;
    void		 evaluateRightHold(fpreal *data, int size) const;

    void		 evaluateDefault(fpreal *data, int size) const;
    void		 evaluateCycle(fpreal start_index, fpreal stop_index,
		                       fpreal *data, int size) const;
    void		 evaluateMirror(fpreal start_index, fpreal stop_index,
					fpreal *data, int size) const;
    void		 evaluateCycleStep(fpreal start_index, fpreal stop_index,
					   fpreal *data, int size) const;

    void		 updateName();
    //
    // I/O methods
    //
    class CL_API BinaryIO
    {
	public:
	    BinaryIO() : myTaintVersion(0), myOffset(0), myDoubleFlag(false) {}

	    int			 myTaintVersion;
	    int			 myOffset;
	    bool		 myDoubleFlag;
    };

    int                  save(UT_OStream &os, BinaryIO *binary_options) const;
    bool		 load(UT_IStream &is, BinaryIO *binary_options,
			      const char *path = 0);

    void		 setDefaults();
    void		 calcNameNumber();
    void setIndex(int);

    UT_StringHolder	 myName;
    UT_FprealArray	 myData;
    CL_Clip		*myParent;

    unsigned short	 myNameBaseLength;
    unsigned short	 myNameNumber;
    unsigned short	 myNameBaseHash;

    char		 myNameNumberDirty;


    CL_TrackOutside	 myLeft;	// eval method outside range
    CL_TrackOutside	 myRight;	// eval method outside range
    fpreal		 myDefault;	// used in CL_TRACK_DEFAULT
    fpreal		*myPrevDataAlias; // used by alias/unalias.

    int			 myIndex; // track index in the clip array.

    friend		 class CL_Clip;
    friend		 class CL_AsciiClipReader;
    friend		 class CL_AsciiClipWriter;
    friend		 class CL_BinaryClipReader;
    friend		 class CL_BinaryClipWriter;
};


#endif
