/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Channel Library (C++)
 *
 * COMMENTS:	Definition of a channel
 *
 */

#ifndef __CH_Channel_h__
#define __CH_Channel_h__

#include "CH_API.h"
#include "CH_Collection.h"
#include "CH_ExprLanguage.h"
#include "CH_EventManager.h"
#include "CH_Manager.h"
#include "CH_Segment.h"
#include "CH_Types.h"

#include <UT/UT_Array.h>
#include <UT/UT_Assert.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_Interval.h>
#include <UT/UT_String.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_SuperInterval.h>
#include <UT/UT_Swap.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_VectorTypes.h>

#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Math.h>
#include <SYS/SYS_Types.h>
#include <iosfwd>
#include <stddef.h>


class UT_IStream;


CH_API extern char
CHvalueNames[2][CH_NUM_VALUES];

typedef enum {
    CH_CHANNEL_DEFAULT,	 // Channel is default value beyond defined range
    CH_CHANNEL_HOLD,	 // Channel is start/end value before/after
    CH_CHANNEL_CYCLE,	 // Channel wraps to defined range
    CH_CHANNEL_EXTEND,	 // Channel function continues outside start/end
    CH_CHANNEL_SLOPE,	 // Channel extends at start/end slope
    CH_CHANNEL_CYCLEOFF, // Channel warps tot defined range and offsets.
    CH_CHANNEL_OSCILLATE, // Channel warps tot defined range and offsets.
    CH_CHANNEL_BEHAVIOR_COUNT
} CH_ChannelBehavior;


// NB: This enum only has 8 bits.
// Do not change these values as they get saved.
typedef enum {
    CH_CHANNEL_NONE	    = 0x00,
    CH_CHANNEL_MODIFIED	    = 0x01,
    CH_CHANNEL_COOKING	    = 0x02,
    CH_CACHE_ENABLED	    = 0x04,	// no longer used!
    CH_CHANNEL_SPARE	    = 0x08,	// no longer used!
    CH_CHANNEL_ACTIVE	    = 0x10,

    CH_CHANNEL_PENDING	    = 0x20,	// only used for saving/loading
    CH_CHANNEL_PENDINGHOLD  = 0x40,	// no longer used!

    CH_CHANNEL_LOCKED	    = 0x80,

    // We save/load CH_CHANNEL_PENDING via myPending
    //CH_CHANNEL_SAVE_MASK    = CH_CHANNEL_PENDING,

    CH_CHANNEL_FLAG_MASK    = 0xFF	// sentinel: we've only got 8 bits
} CH_ChannelFlag;

// Enum for the CH_Channel::getKey and getFullKey methods
typedef enum {
    CH_GETKEY_EXTEND_NONE,	// Don't extend the channel cycle.
    CH_GETKEY_EXTEND_DEFAULT,  // Extend the channel extrapolation, and returns keys in the middle.
    CH_GETKEY_EXTEND_BOUNDARY, // Extend the channel extrapolation, but give a hint that the key is the last or the first.
} CH_GetKeyExtend;


class CH_API CH_HalfKey
{
public:
    fpreal	myV[ CH_NUM_VALUES ];
    bool	myVValid[ CH_NUM_VALUES ];
    bool	myVTied[ CH_NUM_VALUES ];
    bool	myVAuto[ CH_NUM_VALUES ];
};

class CH_API CH_Key
{
public:
    fpreal	myTimeValue;
    bool	myEvaluatedSlopes;
    CH_HalfKey	k[2];

    CH_Key() { clear(); }

    void	display() const;

    void	clear();

    // direction is one of -1, 0, 1
    // -1 = tie using left value
    // 0 = tie averaging both
    // 1 = tie using right value
    void	tie( int direction, CH_ValueTypes t );

    void	tie( int direction );

    void	setAuto( CH_ValueTypes t );

    void	set( CH_ValueTypes t, fpreal value );
    bool	isSet( CH_ValueTypes t ) const;

    void	complete();

    void	get( CH_Segment *left_seg, CH_Segment *right_seg,
		     bool accel_ratio = true );

    void	put( CH_Segment *left_seg, CH_Segment *right_seg,
		     bool accel_ratio = true ) const;

    void	stretch( fpreal xscale, fpreal yscale, bool accel_ratio );

    void	changeAccelsToRatios( fpreal left_seg_len, fpreal right_seg_len );

    void	reverse();

    void	opscript( std::ostream &os, bool use_time,
			    bool only_valid=true ) const;

    bool	verify( bool check_tied_slopes=true ) const;
};

class CH_API CH_FullKey: public CH_Key
{
public:
    bool	myUseExpression;
    UT_String	myExpression;
    CH_ExprLanguage myExprLanguage;

    CH_FullKey()
    {
	myUseExpression = false;
	myExprLanguage = CH_OLD_EXPR_LANGUAGE;
    }

    CH_FullKey( CH_FullKey const& other );
    CH_FullKey &operator=( CH_FullKey const& other );

    void	display() const;
};

class CH_API CH_ReversibleKey: public CH_FullKey
{
public:
    bool	myUseRevExpression;
    UT_String	myRevExpression;
    CH_ExprLanguage myRevExprLanguage;

    CH_ReversibleKey()
    {
	myUseRevExpression = false;
	myRevExprLanguage = CH_OLD_EXPR_LANGUAGE;
    }

    CH_ReversibleKey( CH_ReversibleKey const& other );
    CH_ReversibleKey &operator=( CH_ReversibleKey const& other );

    void	display() const;

    void	stretch( fpreal xscale, fpreal yscale, bool accel_ratio );

    void	reverse();
};

//
// Iterator for going through .chn and .bchn files, 
// channel by channel, until the end of channels for current collection is
// reached. The iterator can be restarted by calling begin() again.
//
class CH_API CH_ChannelIterator
{
// After using begin or nextChannel, the position in the stream
// will be proper for using CH_Channel::load or CH_Collection::loadChannelRange
public:
    CH_ChannelIterator(UT_IStream &is, int binary);
    bool begin(UT_StringHolder &name, const UT_StringHolder *path=NULL);
    bool end() { return noMoreChannels; }
    bool nextChannel(UT_StringHolder &name);
    int getLastError() { return lastError; }

private:
    UT_IStream *curr_is;
    UT_StringRef curr_coll;
    int is_binary;
    int lastError;
    bool noMoreChannels;
};


class CH_API CH_Channel
{
public:
    typedef enum
    {
	SNAP_SNAP,
	SNAP_SNAP_AND_DELETE,
	SNAP_REPLACE,
	SNAP_REPLACE_DOUBLE
    } SnapType;

    CH_Channel(CH_Collection *dad, const UT_StringHolder &name,
	       fpreal default_value = 0,
	       const char *default_string = 0);
    CH_Channel(CH_Collection *dad, const CH_Channel &from);
   ~CH_Channel();

    void		 initializeFirstSegment(const char *expr,
						CH_ExprLanguage language);
    
    // Initialize parents of mySegments and myDisableSegments to this
    void                 initializeSegmentParents();
    
    // UTILITIES
    void		 clear();
    void		 swap( CH_Channel &other );

    const UT_StringHolder &getName(void) const { return myName; }
    const UT_StringHolder &getAlias(void) const
			    {
				if (myAlias.isstring())
				    return myAlias;
				else
				    return myName;
			    }

    void		 getFullPath(UT_String &path) const
			 {
			    getCollection()->getFullPath(this, path);
			 }
    void		 getFullPath(UT_StringHolder &path) const
			 {
			    getCollection()->getFullPath(this, path);
			 }

    //  Used only by CH_Collection
    void		 setName(const UT_StringHolder &s) { myName = s; }
    void		 setAlias(const UT_StringHolder &s) { myAlias = s; }

    CH_Manager		*getManager()   const { return myParent->getManager(); }

    fpreal	 	 getTolerance() const
				    { return getManager()->getTolerance(); }

    // This returns if the channel is time dependent without evaluating
    // the channel.
    bool		 isTimeDependent() const;

    // These return if the channel is time dependent and will catch more cases
    // than isTimeDependent() by evaluating the channel.
    bool		 isTimeDependentSlow(int thread) const;
    bool		 isStringTimeDependentSlow(int thread) const;

    bool		 hasNonIntegerKeys() const;
    bool		 isDataDependent(fpreal gtime) const;

    // are we using a zero-length segment to represent one key?
    bool		 hasOnlyOneSegment() const;

    bool		 isRotationChannel() const;

    // KEYS
    bool		 getSurroundingSegs( fpreal gtime,
			    CH_Segment *&left, CH_Segment *&right ) const;

    CH_Segment		*getSegmentAfterKey( fpreal gtime ) const;

    //  isAtKey returns true if the channel is considered modifiable at
    //  the given time. This means there's either no segment there or the
    //  segment is constant or there's a timemark there (segment start|end).
    bool		 isAtKey(fpreal gtime) const;

    //  isAtHardKey returns true only if there's a timemark (segment start
    //  or end) at the given time.
    bool		 isAtHardKey(fpreal gtime) const;
    bool		 isAtHardKeyframe(int frame) const;

    fpreal		 findKey(fpreal gtime, fpreal direction) const;
    int			 findKeyframe(int frame, int direction) const;

    class CH_API    ConstIntervalIter;
    class CH_API    IntervalIter;

    /// Return an iterator for keys within the given interval. If ascending is
    /// false, then the iterator is initialized at the end of the range
    /// instead of at the beginning of the range.
    /// @{
    IntervalIter	 intervalIter(
			    const UT_SuperIntervalR & range,
			    const bool ascending=true) const;
    ConstIntervalIter	 constIntervalIter(
			    const UT_SuperIntervalR & range,
			    const bool ascending=true) const;
    /// @}

    // sample functions evaluate the channel to find values
    // getKey functions only evaluate if the time is not already at a hard key
    void		 sampleValueSlope( CH_Segment *seg, fpreal gtime,	
					   int thread, fpreal &v, fpreal &s );
    bool		 sampleVSA( CH_Segment *seg, fpreal gtime, int thread,
					    fpreal &v, fpreal &s, fpreal a[2] );
    void		 sampleKey( CH_Segment *seg, fpreal gtime, int thread,
				    CH_Key &key );
    bool		 getKey( fpreal gtime, CH_Key &key,
				 bool accel_ratios = true,
				 CH_GetKeyExtend extend = CH_GETKEY_EXTEND_NONE );
    bool		 getFullKey( fpreal gtime, CH_FullKey &key,
				     bool reverse = false,
				     bool accel_ratios = true,
				     CH_GetKeyExtend extend = CH_GETKEY_EXTEND_NONE );
    bool		 getReversibleKey( fpreal gtime, CH_ReversibleKey &key );

    void		 putKey( fpreal gtime, CH_Key const& key,
				 bool accel_ratios = true );
    void		 putFullKey( fpreal gtime, CH_FullKey const& key,
				     bool accel_ratios = true );

    void		 transferKey(fpreal to_time,
			    CH_Channel *from_chp, fpreal from_time);

    void		 applyPendingToKey( CH_Key &key, fpreal gtime );
    void		 applyDefaultSlopeToKey( CH_Key &key );

    /// Insert key at given time
    void		 insertKeyFrame(fpreal global_t, bool use_auto_slope_pref=true);

    /// Delete key at given time
    void	 	 destroyKeyFrame(fpreal global_t);

    /// Move key from old time to new time. Requires that a key exists at
    /// old_gtime and that there is no key at new_gtime.
    void		 moveKeyFrame( fpreal old_gtime, fpreal new_gtime );

    void	  	 saveKeyFrameForUndo( fpreal global_t );

    // this function is a bit confusing
    // Sets the in/out values for a segment to the val at a time specified
    //The method returns 0 if it was successfull,
    //			   -1 if there was no key at the time specified
    // set_pending = if true,the pending value is set to val, unless commit_keys
    //               is true, and gtime falls on a key frame.
    // commit_keys = if true and gtime falls on a key frame, the keyed value
    //               for that frame is set to val, pending is cleared if 
    //               set_pending is true
    int			 setKeyValue(fpreal val, fpreal gtime,
				     bool set_pending = false,
				     bool commit_keys = true,
				     bool propagate = true);
    int			 setKeyString(const UT_String &str, fpreal gtime,
				      bool set_pending = false,
				      bool commit_keys = true,
				      bool propagate = true);
    int			 setKeyString(const UT_String &str, fpreal gtime,
				      CH_ExprLanguage language,
				      bool set_pending = false,
				      bool commit_keys = true,
				      bool propagate = true);
    void		 holdValue( fpreal gtime, int thread );

    // utility functions for getting keyframe numbers:
    // they return array of keyframes in the channel 
    // the frames are unique and sorted in a ascending order.
    void		 getKeyFrames( UT_SuperIntervalR const& range,
					UT_IntArray &frames,
					bool error_frames_only );

    /// Similar to the above, but gets times of all keys in the channel
    /// in the specified range.
    void		 getKeyTimes( UT_SuperIntervalR const& range,
					UT_FprealArray &times,
					bool error_frames_only );

    // the frames are unique and sorted in a descending order.
    void		 getDisabledFrames(UT_IntArray &frames,
					     int minframe, int maxframe);

    // EXPRESSIONS
    CH_Segment		*getExpressionSegment( fpreal gtime ) const;
    const char		*getExpression( fpreal gtime ) const;
    bool		 changeExpression( fpreal gtime,
					    const char *expr,
					    CH_ExprLanguage language,
					    bool convert_accels );
    CH_ExprLanguage	 getCollectionExprLanguage() const;
    CH_ExprLanguage	 getExprLanguageAtTime(fpreal time) const;
    void		 setExprLanguage(CH_ExprLanguage language);
    void		 setExprLanguageAtTime(CH_ExprLanguage language,
					       fpreal time);

    // If we were to ask for the value of the channel as a string at a
    // particular time, this function returns what the meaning of that string
    // would be (either a literal or expression in some language).
    CH_StringMeaning	 getStringMeaning(fpreal time) const;

    // MULTI-SEGMENT MANIPULATION
    void	 clearSegments();

    /// Delete keys in given range
    void	 deleteKeys( UT_SuperIntervalR const& range );

    /// Delete keys at the given key indices.
    /// @note Must be sorted in ascending order!
    void	 deleteKeys(const UT_IntArray &ascending_sorted_key_indices);

    void	 moveFrames(const UT_IntArray &frames, int amount);

    bool	 copyRangeOverwritesKeys(const CH_Channel &src,
				    UT_SuperIntervalR const& range,
				    UT_IntervalR const& to_range);
    bool	 copyRangeOverwritesFrames(const CH_Channel &src,
				    UT_SuperIntervalR const& range,
				    UT_IntervalR const& to_range);
    bool	 copyRangeOverwrites(const CH_Channel &src,
				    UT_SuperIntervalR const& range,
				    UT_IntervalR const& to_range);

    void	 copyRange(const CH_Channel &src,
				    UT_SuperIntervalR const& range,
				    UT_IntervalR const& to_range);

    void 	 copyRangeFrames(const CH_Channel &src,
		            UT_SuperIntervalR const& range,
		            UT_IntervalR const& to_range,
		            const UT_IntArray * frames = NULL);

    void 	 deleteRangeFrames(UT_SuperIntervalR const& range,
    				   const UT_IntArray * frames = NULL);

    // copyContents copies everything except name and parent
    void	 copyContents(const CH_Channel &from);
    void	 swapContents( CH_Channel &other );
    // TODO remove this function:
    // copy a range of keys. Assumes that we already have keys in the src
    // channel at those times
    // another way to do this is: (once we pass UT_SuperIntervalR by value)
    // ch.clear();
    // ch.copyRange( src, UT_SuperIntervalR( start, end ), 0.0f );
    void	 copyRange(const CH_Channel &src,
				   fpreal global_start, fpreal global_end);

    // if os is provided, a verbose description of what's happening is printed
    void	 snapKeysInRange( const UT_SuperIntervalR &range,
				    SnapType type=SNAP_SNAP,
				    std::ostream *os=0 );

    // ENTIRE CHANNEL MANIPULATION
    void	scroll (fpreal newStart, bool update = true);
    void	stretch(fpreal newStart, fpreal newEnd);
    void	reverse(fpreal gstart=0, fpreal gend=0, bool whole_chan = true);
    // scroll on the y axis (returns success)
    bool	increaseKeyValues( fpreal delta );

    // DISABLING
    bool	isAllDisabled() const;
    bool	isAllEnabled() const;
    bool	isDisabled(fpreal gtime) const;
    void	disableAll(fpreal gtime);
    void	enableAll();
    void	disable(fpreal gstart, fpreal gend);
    void	enable(fpreal gstart, fpreal gend);

    // PENDING METHODS
    SYS_FORCE_INLINE
    bool		 isPending() const
			    { return myPending; }
    bool		 isPending(fpreal gtime) const;
    bool		 isPendingLocal(fpreal ltime) const;
    void		 clearPending();
    void		 updatePending( fpreal gtime );
    fpreal		 getPendingEvalTime() const
			    { return globalTime(myPendingEvalTime); }
    SYS_FORCE_INLINE
    bool		 isPendingHold() const
			 { return myPendingHold; }
    void		 setPendingHold( bool state );

    // SAVE and LOAD
    void		 save(std::ostream &os, int binary, bool compiled) const;
    template <typename FPREAL_TYPE>
    bool		 load(UT_IStream &is);
    void		 display() const;
    void		 displayAsKeys() const;

    void		 setDefaultValue(fpreal dv) { myDefValue = dv; }
    fpreal		 getDefaultValue() const   { return myDefValue; }

    void		 setDefaultString(const UT_StringHolder &dv)
			 { myDefString = dv; }
    const UT_StringHolder &
			 getDefaultString() const   { return myDefString; }

    void		 setChannelLeftType(CH_ChannelBehavior t);
    void		 setChannelRightType(CH_ChannelBehavior t);
    CH_ChannelBehavior	 getChannelLeftType(void) const  { return myLeftType; }
    CH_ChannelBehavior	 getChannelRightType(void) const { return myRightType; }

    static const char	*getChannelTypeName(CH_ChannelBehavior type);

    void		 setCollection(CH_Collection *chp){ myParent = chp; }
    const CH_Collection	*getCollection() const { return myParent; }
    CH_Collection	*getCollection()       { return myParent; }

    void		 setIsTemporary( bool temp ) { myIsTemporary = temp; }
    bool		 isTemporary() const { return myIsTemporary; }

    // FLAGS
    void		 setChangeActive(int state)
			 {
			    if( state )	myFlags |=  CH_CHANNEL_ACTIVE;
			    else	myFlags &= ~CH_CHANNEL_ACTIVE;
			 }
    bool		 isChangeActive() const
    			 { return (myFlags & CH_CHANNEL_ACTIVE) ? 1 : 0; }
    

    bool		 getLocked() const
			 { return ((myFlags & CH_CHANNEL_LOCKED) != 0); }
    void		 setLocked(bool f);

    int		 	 getChanged() const
			 { return (myFlags & CH_CHANNEL_MODIFIED) ? 1 : 0; }
    void		 setChanged(bool on,
				    CH_CHANGE_TYPE type = CH_CHANNEL_CHANGED);
    void		 dirtyExprCache();

    int			 canAccess(uint mask) const
			 {
			    if( myParent )
				return myParent->canAccessChannel(mask, this);
			    else
				return 1;
			 }

    // map collection time to channel time...
    inline fpreal	 localTime(fpreal t)   const { return (t-myStart); }

    // map channel time to global time...
    inline fpreal	 globalTime(fpreal t)  const { return t + myStart; }

    // Returns collection global time...
    fpreal	getStart()	     const { return myStart; }
    fpreal	getEnd()	     const { return myStart + myLength;}
    fpreal	getLength()	     const { return myLength; }

    // Used to reset the local variables:
    void		 unresolveLocalVars(int thread);

    // Op dependencies
    void		 buildOpDependencies(void *ref_id, int thread);
    void		 changeOpRef(const char *new_fullpath,
				     const char *old_fullpath,
				     const char *old_cwd,
				     const char *chan_name,
				     const char *old_chan_name,
				     int thread);

    // Enable or disable value caching in expression segments...
    void	 cook(int state);

    // Load values as keys. Returns the number of samples actually loaded.
    int		 setRawValues(fpreal from, fpreal to, fpreal *vals, int nsamples);
    int		 setRawValues(fpreal *times, fpreal *vals, int n);

    // Channel Scope Flag
    void	 setScope(bool on_off);
    bool	 isScoped() const { return myScope; }

    // Does this channel have any segments with editable components (eg. value,
    // slope, acceleration, etc.)?
    bool	 isEditable();

    // Query methods. Used only by CH_Collection
    fpreal	 evaluate(fpreal t, bool no_disabling, int thread);
    void	 evaluate(fpreal from, fpreal to, fpreal *vals, int nsamples,
	                  bool no_disabling, bool use_cache, int thread);
    fpreal	 evaluateSegment(CH_Segment *eval, fpreal localtime,
	    		         bool extend, int thread);
    void	 evaluateString(UT_String &result, fpreal t, int thread);
    void	 evaluateStringSegment(UT_String &result, CH_Segment *eval,
				       fpreal localtime, bool extend,
				       int thread);

    int		 findString(const char *str, bool fullword,
			    bool usewildcards) const;
    int		 changeString(const char *from, const char *to,
			      bool fullword, bool update /*= true*/,
			      int thread);

    //
    //  Methods used only in the channel library glue
    //
    static inline fpreal    getGlueIV(int thread);
    static inline void	    getGlueTime(
				fpreal &t, fpreal &t0, fpreal &t1,
				fpreal &v0, fpreal &v1, int thread);
    static inline void	    getGlueSlope(
				fpreal *m0, fpreal *m1,
				fpreal *a0, fpreal *a1, int thread);

//  Used only by CH_Collection
    inline const CH_Segment *getEvaluationSegment(int thread) const
			{
			    return getManager()->evalContext(thread).segment();
			}
    inline CH_Segment *getEvaluationSegment(int thread)
			{
			    return getManager()->evalContext(thread).segment();
			}
    const CH_Segment	*getPrevEvaluationSegment(int thread) const;
    CH_Segment		*getPrevEvaluationSegment(int thread);
    const CH_Segment	*getNextEvaluationSegment(int thread) const;
    CH_Segment		*getNextEvaluationSegment(int thread);

    fpreal	 getInTime(int thread) const;
    fpreal	 getOutTime(int thread) const;
    fpreal	 getPrevInTime(int thread) const;
    fpreal	 getNextOutTime(int thread) const;

    // This method will attempt to match the channel name with the pattern,
    // searching as many parents as necessary regardless of the cwd
    // eg. tx will match true with tx xform1/tx geo1/xform1/tx

    int		match(const char *pattern) const;


    // sets all keys to be 'value', and zeros all slopes & accels.
    void	 setConstantValue(fpreal value);
   
    // sets the data and keys so that the channel approximates the
    // raw data of samples within given error using cubic interpolation
    // INPUTS:
    //   data      - an array of pairs (time, value) to be fitted. 
    //   n_pts     - number of elements in data array
    //   error2	   - the error to which the channel should approximate data
    //   delete_old_segments - if true, all old segments are cleared before
    //               new ones are generated from data. If false, the segments
    //               that fit the data replace old segments (thus, some
    //               old segments that are outside of the data range will 
    //               remain unchanged)
    // OUTPUTS:
    //		   The clip will approximate the curve sampled by data.
    //		   The clip will have its start time equal to
    //             the time component of the first pair in the data array,
    //             and the end time equal to the time of the last pair.
    // RETURNS:	   
    //		   True on success, false if failed. 
    bool	 fitToPointData( UT_Vector2R *data, int n_pts, 
				 bool preserveExtrema,
				 fpreal error2 = 0.01f,
				 bool delete_old_segments = false);

    // refits the channel data (or portion of it) with cubic interpolation 
    // within given error tolerance. Inserts new keys, etc.
    // delete_old_segments - if true, all segments are deleted before
    //     refitted range is inserted. Otherwise, old segments outside of the
    //     refitted range are not affected.
    void	 refit( fpreal tolerance, bool preserveExtrema );
    void	 refit( fpreal tolerance, bool preserveExtrema,
			fpreal from_gtime, fpreal to_gtime,
			bool delete_old_segments);

    // SEGMENT MANAGEMENT
    bool	 isEmpty() const;
    int		 getNSegments() const;
    int		 getNKeys() const;
    int		 getNKeysBefore( fpreal gtime ) const;
    int		 getNKeysBeforeOrAt( fpreal gtime ) const;

    fpreal	 getKeyTime( unsigned idx ) const;

    unsigned	 getSegmentIdx(fpreal local_time) const;
    CH_Segment	*getSegment(unsigned idx) const;
    CH_Segment	*getSegment(fpreal local_time) const;

    CH_Segment	*getNextSegment(unsigned idx) const; // Depends on behavior
    CH_Segment	*getPrevSegment(unsigned idx) const; // Depends on behavior

    CH_Segment	*getFirstSegment()  const
		 { 
		     // empty means no keys in the channel if user delete them
		     // all
		     UT_ASSERT_P( isEmpty() || myEndSegment );
		     return isEmpty()
			? 0 : getSegment( (unsigned)0 );
		 }
    CH_Segment	*getLastSegment()  const
		 { 
		     // empty means no keys in the channel if user delete them
		     // all
		     UT_ASSERT_P( isEmpty() || myEndSegment );

		     // old behaviour maintained (ie. don't use end segment)
		     return isEmpty()
			? 0 : getSegment( (unsigned)mySegments.entries()-1 );
		 }

    int64	 getMemoryUsage(bool inclusive) const;

    //  The first method takes global time, the second, segment local time.
    void	 changeSegLength     (CH_Segment *seg, fpreal gtime,
					CH_SegmentScale how = CH_SCALE_ANY,
					bool adjust_and_update = true);

    bool	 getHaveCompiledExpressions() const;

    bool	 verify();
    bool	 verify(bool verify_tied_values);

    // Snapshots or buffer curves.
    // Create a deep copy of the channel data for fast modifications and revert workflow.
    // It's more intuitive than to have to do multiple undo to get back to the previous 
    // keyframes.
    void saveSnapshot();
    void undoSnapshot(CH_Channel *previous, bool previousSnapshotCleared);
    bool loadSnapshot();
    bool swapSnapshot();
    bool clearSnapshot();
    CH_Channel* getSnapshotChannel();


    void smoothAutoSlopes( CH_Segment *seg=NULL );


    // Layer Scope Flag
    void	 setLayerScope(bool on_off);
    bool	 isLayerScoped() const { return myLayerScope; }

    // Set an array pointer to capture the copyRange actions
    void setCaptureCopiedKeys( UT_Array<fpreal> *capture_array );

    // chkey version of putFullKey() doesn't do the same validations as putFullKey()
    void putChFullKey( fpreal gtime, CH_FullKey const& key, bool accel_ratios = true );
private:
		 CH_Channel( CH_Channel const& other );	// unimplemented

    void	 ensureEndSegmentCreated();
    void	 updateEndSegment();
    void	 destroyEndSegment();

    bool	 isTwoSegChannelTimeDep() const;

    // TODO: Remove this. It's abysmally slow, doing a binary search each time
    bool	 nextTimeInRange(UT_SuperIntervalR const& range,
		                 fpreal &t, bool first) const;
					    
    class BatchModifyScope;
    friend class CH_Channel::BatchModifyScope;

    void	 moveKeyInternal(fpreal old_gtime, fpreal new_gtime,
				 BatchModifyScope &scope);
    void	 splitSegment(fpreal gtime, bool accel_ratio = true);
    void	 destroyKeyInternal(unsigned seg_i, BatchModifyScope &scope);

    // cuts out a portion of a channel. Returns an index at which the cut began
    int		 cutOut( fpreal from_gtime, fpreal to_gtime );

    CH_Segment	*appendSegment(CH_Segment *seg);

    // remove and delete the segment
    void	 deleteSegment(unsigned idx);

    int		 getNDisableSegments(void) const
		 { return myDisableSegments.entries(); }
    CH_Segment	*getDisableSegment(unsigned idx) const
		 {
		    return (idx < myDisableSegments.entries()) ?
			    myDisableSegments(idx) : 0;
		 }

    CH_Segment	*getDisableSegmentForLocalTime(fpreal local_time) const;
    CH_Segment	*getPrevDisableSegment(CH_Segment *) const;
    CH_Segment	*getNextDisableSegment(CH_Segment *) const;

    int		 getDisableSegmentIndex(fpreal ltime, bool closest_prev) const;

    CH_Segment	*getFirstDisableSegment()  const
		 { 
		    return (myDisableSegments.entries() > 0) ?
			myDisableSegments(0) : 0;
		 }
    CH_Segment	*getLastDisableSegment()  const
		 { 
		    return (myDisableSegments.entries() > 0)
			  ? myDisableSegments(myDisableSegments.entries()-1)
			  : 0;
		 }
    CH_Segment	*appendDisableSegment(CH_Segment *seg);

    CH_Segment	*insertDisableSegment(CH_Segment *seg);
    CH_Segment	*insertDisableSegment(CH_Segment *seg, int idx);
    CH_Segment	*insertDisableSegment(CH_Segment *seg, CH_Segment *before);

    // Remove but don't delete
    void	 removeDisableSegment(CH_Segment *seg);
    void	 removeDisableSegment(int idx);

    void	 deleteDisableSegment(CH_Segment *);
    void	 deleteDisableSegment(int idx);

    void	 setPending( bool on_off, fpreal ltime );
    void	 updatePendingStateToManager();

    void	 deNormalize(fpreal scale);

    int		 isCooking() const
		 {
		    return (myFlags & CH_CHANNEL_COOKING) != 0;
		 }
    void	 setCooking(bool on_off)
		 {
		    if (on_off)	myFlags |=  CH_CHANNEL_COOKING;
		    else	myFlags &= ~CH_CHANNEL_COOKING;
		 }

    void	 adjustSegments(int begin_i, int end_i,
				CH_CHANGE_TYPE event_type);

    // Get Segment methods that can return disabled segments.
    CH_Segment	*getFirstSegment(bool no_disabling )  const;
    CH_Segment	*getLastSegment(bool no_disabling )  const;
    CH_Segment	*getEndSegment(bool no_disabling )  const;
    CH_Segment	*getSegment(bool no_disabling, fpreal ltime ) const;

    enum chGetOrSampleReturn
    {
	Sampled, Get, FirstLast
    };
    chGetOrSampleReturn getOrSampleKey( fpreal ltime, CH_Key &key, bool accel_ratio, int thread );

    CH_Collection		*myParent;	// Who I belong to
    UT_StringHolder		 myName;
    UT_StringHolder		 myAlias;

    CH_ChannelBehavior		 myLeftType;	// Type of channel
    CH_ChannelBehavior		 myRightType;	// Type of channel
    CH_Segment			*myEndSegment;

    fpreal			 myDefValue;	// Default/Pending value
    fpreal			 myStart;	// Start time of channel
    fpreal			 myLength;	// Kept for efficiency (local)

    UT_StringHolder		 myDefString;   // Default/Pending string value

    UT_ValArray<CH_Segment *>	 mySegments;	// Array of segments
    UT_ValArray<CH_Segment *>	 myDisableSegments;

    fpreal			 myPendingEvalTime;
    CH_Channel			*mySnapshot;
    UT_Array<fpreal>		*myCaptureCopiedKeys;

    unsigned char	 	 myFlags;
    bool			 myPending;
    bool			 myPendingHold;
    bool			 myScope;
    bool			 myLayerScope;
    bool			 myIsTemporary;
    bool			 myDoingChKey;
    bool			 mySnapshotCleared;
						
};

UT_SWAPPER_CLASS(CH_Channel)

inline CH_Segment *
CH_Channel::getSegment(unsigned idx) const
{
    if( idx<mySegments.entries() )
	return mySegments(idx);
    if( idx==(unsigned)-1 || idx==mySegments.entries() )
    {
	UT_ASSERT(myEndSegment || mySegments.entries() == 0);
	return myEndSegment;
    }
    UT_ASSERT( false );
    return NULL;
}

inline int
CH_Channel::getNSegments() const
{
    return myEndSegment ? 1 + mySegments.entries() : 0;
}

inline bool
CH_Channel::isEmpty() const
{
    return mySegments.isEmpty() && !myEndSegment;
}

inline int
CH_Channel::getNKeys() const
{
    return getNSegments();
}

inline fpreal
CH_Channel::getKeyTime( unsigned idx ) const
{
    CH_Segment	*segp;
    if( idx == mySegments.entries() )
    {
	segp = myEndSegment;
	UT_ASSERT( segp );
	return globalTime( segp->getStart() );
    }
    segp = getSegment( idx );
    UT_ASSERT( segp );
    return globalTime( segp->getStart() );
}

inline bool
CH_Channel::hasOnlyOneSegment() const
{
    return getNSegments() == 1;
}

inline bool
CH_Channel::isRotationChannel() const
{
    // NB: This is actually slightly weaker than PRM_Float::isRotationParm()
    return myName == "rx" || myName == "ry" || myName == "rz";
}


/// Const iterator over an interval
class CH_Channel::ConstIntervalIter
{
public:
    ConstIntervalIter()
	: myChannel(NULL)
	, myI(-1)
	, myTol(0.0)
    {
    }

    ConstIntervalIter &operator++()
    {
	++myI;
	return *this;
    }
    ConstIntervalIter &operator--()
    {
	--myI;
	return *this;
    }

    bool atEnd() const
    {
	if (myI >= myChannel->getNSegments())
	    return true;
	fpreal lt = localTime();
	return !(myRange.myMaxInclusive ?
		      SYSisLessOrEqual(lt, myRange.max, myTol)
		    : SYSisLess(lt, myRange.max, myTol));
    }
    bool atPre() const
    {
	if (myI < 0)
	    return true;
	fpreal lt = localTime();
	return !(myRange.myMinInclusive ?
		      SYSisGreaterOrEqual(lt, myRange.min, myTol)
		    : SYSisGreater(lt, myRange.min, myTol));
    }

    unsigned index() const
    {
	return (unsigned)myI;
    }

    const CH_Segment &segment() const
    {
	return *(myChannel->getSegment((unsigned)myI));
    }

    // Local time at start of segment
    fpreal localTime() const
    {
	return myChannel->getSegment((unsigned)myI)->getStart();
    }

    fpreal operator*() const { return myChannel->globalTime(localTime()); }

protected:
    ConstIntervalIter(
	    const CH_Channel &ch, const UT_SuperIntervalR &range,
	    const bool ascending)
	: myChannel(&ch)
	, myRange(range)
	, myTol(ch.getTolerance())
    {
	myRange.min = ch.localTime(myRange.min);
	myRange.max = ch.localTime(myRange.max);
	UT_ASSERT(myRange.min <= myRange.max);
	if (ascending)
	{
	    // If myRange.min is less than the first segment's start time, then
	    // the first segment is returned by getSegmentIdx().
	    myI = ch.getSegmentIdx(myRange.min);
	    CH_Segment *seg = ch.getSegment((unsigned)myI);
	    if (seg)
	    {
		fpreal lt = seg->getStart();
		if (range.myMinInclusive)
		{
		    if (SYSisLess(lt, myRange.min, myTol))
			++myI;
		}
		else
		{
		    if (SYSisLessOrEqual(lt, myRange.min, myTol))
			++myI;
		}
	    }
	    else
	    {
		myI = ch.getNSegments();
	    }
	    UT_ASSERT(atEnd() || myRange.contains(localTime(), myTol));
	}
	else // descending
	{
	    myI = ch.getSegmentIdx(myRange.max);
	    CH_Segment *seg = ch.getSegment((unsigned)myI);
	    if (seg)
	    {
		fpreal lt = seg->getStart();
		if (range.myMaxInclusive)
		{
		    if (SYSisGreater(lt, myRange.max, myTol))
			--myI;
		}
		else
		{
		    if (SYSisGreaterOrEqual(lt, myRange.max, myTol))
			--myI;
		}
	    }
	    else
	    {
		myI = -1;
	    }
	    UT_ASSERT(atPre() || myRange.contains(localTime(), myTol));
	}
    }

private:
    const CH_Channel *	myChannel;
    UT_SuperIntervalR	myRange;
    int			myI;
    fpreal		myTol;

    friend class CH_Channel;
};

/// Iterator over an interval
class CH_Channel::IntervalIter : public CH_Channel::ConstIntervalIter
{
public:
    IntervalIter()
	: ConstIntervalIter()
    {
    }
    
    CH_Segment &segment() const
    {
	return const_cast<CH_Segment &>(ConstIntervalIter::segment());
    }

    IntervalIter &operator++()
    {
	ConstIntervalIter::operator++();
	return *this;
    }
    IntervalIter &operator--()
    {
	ConstIntervalIter::operator--();
	return *this;
    }

protected:
    IntervalIter(
	    const CH_Channel &ch, const UT_SuperIntervalR &range, bool reverse)
	: ConstIntervalIter(ch, range, reverse)
    {
    }

private:

    friend class CH_Channel;
};

inline CH_Channel::IntervalIter
CH_Channel::intervalIter(
	const UT_SuperIntervalR & range, const bool ascending) const
{
    return IntervalIter(*this, range, ascending);
}
inline CH_Channel::ConstIntervalIter
CH_Channel::constIntervalIter(
	const UT_SuperIntervalR & range, const bool ascending) const
{
    return ConstIntervalIter(*this, range, ascending);
}

/// This function stretches the modifies the given slope and accel values
/// according to the given factors. If the accel value is given as a ratio,
/// then accel_ratio should be true.
CH_API extern void
CHstretchSlopeAccel(fpreal xscale, fpreal yscale, fpreal &slope, fpreal &accel,
		    bool accel_ratio);

// this will move attempted_dt towards accepted_dt so that:
// keys[i]+attempted_dt does not land on another key
// attempted_dt stays on the same side of accepted_dt as it started
// 	(so that if you're moving one way, they don't snap backwards before
// 	where they started)
// if snap_to_frame, attempted_dt+base_time will land on a frame
CH_API extern void
CHfindMoveKeysDelta( const CH_Channel *chp, bool snap_to_frame, fpreal base_time,
			const UT_FprealArray &orig_keys,
			fpreal accepted_dt, fpreal &attempted_dt );

CH_API extern void
CHmoveKeysWithDelta( CH_Channel *chp, const UT_FprealArray &orig_keys,
			fpreal old_accepted_dt, fpreal new_accepted_dt );

///////////////////////////////////////////////////////////////////////////////
//
// Inline Implementations
//

/*static*/ inline fpreal
CH_Channel::getGlueIV(int thread)
{
    const CH_EvalContext &	ctx = CHgetManager()->evalContext(thread);
    const CH_Channel *		eval_channel = ctx.channel();
    const CH_Segment *		segp = ctx.segment();

    if (segp)
    {
	if (eval_channel->isPending(ctx.time()))
	    return eval_channel->getDefaultValue();
	else
	    return segp->getInValue();
    }

    return 0.0f;
}

/*static*/ inline void
CH_Channel::getGlueTime(fpreal &t, fpreal &t0, fpreal &t1,
			fpreal &v0, fpreal &v1, int thread)
{
    CH_Manager &	mgr = *CHgetManager();
    CH_EvalContext &	ctx = mgr.evalContext(thread);
    const CH_Channel *	eval_channel = ctx.channel();
    const CH_Segment *	segp = ctx.segment();

    if (eval_channel)
    {
	t = eval_channel->localTime(ctx.time());
	if (segp)
	{
	    t0 = segp->getStart();
	    t1 = segp->getEnd();
	    if( eval_channel->isPendingLocal(t) )
	    {
		v0 = v1 = eval_channel->getDefaultValue();
		t0 = t1 = t;
	    }
	    else
	    {
		v0 = segp->getInValue();
		v1 = segp->getOutValue();
		if( segp->isEndSegment() )
		{
		    // Since our segment length is zero, we want fake it so
		    // that it uses either the in or out values because most
		    // existing expression functions will take the average of
		    // them when encountering a zero length segement.
		    // NB: the in/out values are swapped on end segments.
		    if (SYSisGreaterOrEqual(t, t0, mgr.getTolerance()))
			v1 = v0;
		    else
			v0 = v1;
		}
	    }
	}
	else
	{
	    t0 = eval_channel->getStart();
	    t1 = eval_channel->getEnd();
	    v0 = v1 = 0;
	}
    }
    else
    {
	t = 0.0f;
	t0 = 0.0f;
	t1 = 0.0f;
	v0 = 0.0f;
	v1 = 0.0f;
    }
}

/*static*/ inline void
CH_Channel::getGlueSlope(fpreal *m0, fpreal *m1, fpreal *a0, fpreal *a1,
			 int thread)
{
    CH_EvalContext &	ctx = CHgetManager()->evalContext(thread);
    const CH_Segment *	segp = ctx.segment();

    if (segp)
    {
	if (m0) *m0 = segp->getInSlope();
	if (m1) *m1 = segp->getOutSlope();
	if (a0) *a0 = segp->getInAccel();
	if (a1) *a1 = segp->getOutAccel();
    }
    else
    {
	if (m0) *m0 = 0.0f;
	if (m1) *m1 = 0.0f;
	if (a0) *a0 = 0.0f;
	if (a1) *a1 = 0.0f;
    }
}

// this returns if the channel is time dependent without evaluating
// the channel
inline bool
CH_Channel::isTimeDependent() const
{
    const int nsegs = getNSegments();

    if (nsegs == 0)
	return false;

    if (nsegs == 1)
	return getSegment(0U)->isTimeDependent();

    if (nsegs == 2)
	return isTwoSegChannelTimeDep();

    // Else, assume time dependent when more than 1 segment
    UT_ASSERT_P(nsegs > 1);
    return true;
}

#endif // __CH_Channel_h__
