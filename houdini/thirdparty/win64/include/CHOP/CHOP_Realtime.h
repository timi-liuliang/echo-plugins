/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CHOP Library (C++)
 *
 * COMMENTS:	All realtime Channel Filters should derive from this class.
 *
 */

#ifndef __CHOP_Realtime_h__
#define __CHOP_Realtime_h__

#include "CHOP_API.h"
#include "CHOP_Node.h"
#include <CL/CL_SaveVersion.h>

#define REALTIME_MODE_TRIM		0
#define REALTIME_MODE_ACCUM		1
#define REALTIME_MODE_CONTINUOUS	2
#define REALTIME_MODE_CURRENT_FRAME	3
#define REALTIME_MODE_FRAME_ONE		4

#define REALTIME_INPUT_INTEREST_NONE (-1)
#define REALTIME_INPUT_INTEREST_ALL  (-2)

#define DEF_REALTIME_SETTLING_TIME	3

class UT_OStream;
class CL_Filter;
class chop_RealtimeData;

// Base Class for realtime Data blocks
class CHOP_API ut_RealtimeData
{
public:
	     ut_RealtimeData(const char *name, int input = -1);
    virtual ~ut_RealtimeData();

    const UT_String & tag() { return myTag; }
    int		      inputNum() { return myInputNum; }
    void	      setInput(int n) { myInputNum = n; }
    
    virtual bool	 loadStates(UT_IStream &is, int version);
    virtual bool	 saveStates(UT_OStream &os);
    
private:
    UT_String	myTag;
    int		myInputNum; 
};

// class for filtering data blocks (para eq, band eq, filter, etc).
class CHOP_API ut_FilterData : public ut_RealtimeData
{
public:
	     ut_FilterData(const char *name, int filter_size, int overlap);
    virtual ~ut_FilterData();

    virtual bool	 loadStates(UT_IStream &is, int version);
    virtual bool	 saveStates(UT_OStream &os);
    
    CL_Filter *myFilter;
};

class CHOP_API CHOP_Realtime : public CHOP_Node
{
protected:

		 CHOP_Realtime(OP_Network *parent,
			       const char *name,
			       OP_Operator *entry);
    virtual	~CHOP_Realtime();

    virtual OP_ERROR cookMySlice(OP_Context &, int start, int end) =0;
    virtual const char *getTimeSliceExtension() = 0;
    virtual int		getSaveVersion()
			    { return CL_SAVE_VERSION; }


    // called when the realtime button is turned on or off.
    virtual void	realtimeInit(fpreal t);
    virtual void	realtimeCleanup();
    
    // these methods are called when the framebar is started or stopped.
    virtual void     realtimeStart(fpreal t);
    virtual void     realtimeStop();

    // this method returns a newly allocated ut_RealtimeData block.
    // override this if you have subclassed ut_RealtimeData.
    // One data block per channel.
    virtual ut_RealtimeData * newRealtimeDataBlock(const char *name,
						   const CL_Track *track,
						   fpreal t);

    // overridden from CHOP_Node.
    virtual	int usesRealtime()	{ return 1; }
    virtual	int isRealtimeCook()	{ return REALTIME(); }
    virtual	int isRealtimeActive(OP_Context &)  { return 1; }

    // normally all realtime chops are time dependent
    virtual	int isTimeDependent();
    virtual     int isSteady() const;
    virtual	int getSettlingTime() const
		    { return DEF_REALTIME_SETTLING_TIME; }
    virtual	int allowDoubleCooking() const { return 0; }

		int isFirstRealtimeCook() const;
	        void setFirstRealtimeCook(bool first);
    
    // if true, this will trim out only the current slice.
    virtual	int realtimeMode()  { return REALTIME_MODE_TRIM; }

    // if true, the realtime CHOP will change the start/end of the CHOP.
    // otherwise, it leaves it alone.
    virtual	int adjustClip() { return 1; }

    // do not override this method.
    virtual OP_ERROR doCookRealtime(OP_Context &context);

    // which input's channels are our names & number of channels dependent on
    // (-1 for none)
    virtual int		getInputInterest() { return 0; } // first input

    // override if you need the slice to end at a time other than the
    // current frame (+/- adjustment).
    virtual fpreal	getRealtimeFrameAdjustment(fpreal ) { return 0.0; }

    // override if the sample rate is set by the chop, and not the input.
    virtual fpreal	getRealtimeSampleRate() { return getSampleRate(); }

    // returns true if a range has been modified, plus that range.
    virtual int		getModifiedRange(int &first,int &last,int clear=1);

    // returns true if wraparound occured before this cook.
	    int		isWraparound() const;

	    int		getNumDataBlocks() const;
    ut_RealtimeData *	getDataBlock(int n);
    ut_RealtimeData *	getDataBlock(const char *tag);

	    void	getFrameRange(int &start, int &end);
    unsigned int	getAbsoluteFrame(); 

    // input channel name & data cache management for straight through
    // filters.
    virtual void		cacheChannelData(fpreal t);
    virtual void		resetDataCache();

    void			setPreviousCookTime(fpreal t);

    void			getCurrentRange(int &start, int &end) const;
    void			setCurrentRange(int start, int end);

    // for classes that manage their own data blocks; use with caution.
    ut_RealtimeData	       *addDataBlock(const char *name,
					     const CL_Track *track =0,
					     fpreal t = 0.0);
    ut_RealtimeData	       *insertDataBlock(int index,
						const char *name,
						const CL_Track *track =0,
						fpreal t = 0.0);
    void			removeDataBlock(const char *name);
    void			removeDataBlock(int index);

    void			updateDataBlocks();

    void			copyRealtimeInput(const CL_Clip *clip,
						  int start, int end);

    virtual OP_ERROR		 save(std::ostream &os, const OP_SaveFlags &flags,
				     const char *pathPrefix,
				     const UT_String &name_override = UT_String());

    virtual bool		load(UT_IStream &is, const char *extension,
				     const char *path);

    virtual OP_ERROR		saveStates(UT_OStream &os);
    virtual bool		loadStates(UT_IStream &is, int version);

    // Allow the Reset At First Frame playbar button to clear all data in
    // doRealtimeCook()
    virtual bool		allowResetAtFirstFrame(OP_Context &ctx)
				    { return isRealtimeActive(ctx); }

protected:

    chop_RealtimeData	       *newRealtimeBlock();
    
    chop_RealtimeData	       *myRealtimeData;

private:
    void			getParentTimeSlice(int &start,int &end);

public:
    chop_RealtimeData	       *getRealtimeData() { return myRealtimeData; }
    void			setRealtimeData(chop_RealtimeData *d)
					{ myRealtimeData = d; }
    static void			deleteRealtimeData(chop_RealtimeData *data);

};

#endif
