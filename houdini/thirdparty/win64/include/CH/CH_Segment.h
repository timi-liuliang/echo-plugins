/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Channel Library (C++)
 *
 * COMMENTS:	This is the class definition for a segment.
 *
*/

#ifndef __CH_Segment_h__
#define __CH_Segment_h__

#include "CH_API.h"
#include "CH_ExprLanguage.h"
#include "CH_Manager.h"
#include "CH_Types.h"
#include "CH_Support.h"

#include <UT/UT_Vector3.h>
#include <SYS/SYS_Math.h>   // for SYSsqrt() etc
#include <SYS/SYS_Types.h>

#include <iosfwd>


#define CH_EXPRESSION_CONSTANT	"constant()"
#define CH_EXPRESSION_BEZIER	"bezier()"
#define CH_EXPRESSION_LINEAR	"linear()"
#define CH_EXPRESSION_CUBIC	"cubic()"
#define CH_EXPRESSION_EASE	"ease()"
#define CH_EXPRESSION_EASEIN	"easein()"
#define CH_EXPRESSION_EASEOUT	"easeout()"
#define CH_EXPRESSION_SPLINE	"spline()"
#define CH_EXPRESSION_QLINEAR	"qlinear()"
#define CH_EXPRESSION_QCUBIC	"qcubic()"

#define CH_SEGMENT_MINIMUM_LENGTH 0.000001F

class CH_Channel;
class CH_Expression;
class CH_Manager;
class CH_Segment;
class CH_TimeGroup;

class UT_String;

enum
{
    CH_STRETCH_IN = 0x01,
    CH_STRETCH_OUT = 0x02
};

class CH_API CH_SegmentFlags
{
public:
    CH_SegmentFlags()
    {
	setDefaults();
    }
    void	save(std::ostream &os, int binary) const;
    bool	load(UT_IStream &is);

    void	setDefaults();
    unsigned	isDefault() const;

    unsigned isConstant:1,
	       isBezier:1,
	       isLinear:1,
	        isCubic:1,
	         isEase:1,
	       isEaseIn:1,
	      isEaseOut:1,
	       isLocked:1,
	       isSpline:1,
	       modified:1,
	    tieOutAccel:1,
	     tieInAccel:1,
	    tieOutSlope:1,
	     tieInSlope:1,
	    tieOutValue:1,
	     tieInValue:1,
		lockEnd:1,
	      lockStart:1,
	     lockLength:1,
              isCooking:1,
	      isQLinear:1,
	       isQCubic:1,
	    autoInSlope:1,
	   autoOutSlope:1,
	 modifyingAccel:1,
		 isChop:1;
};

class CH_API CH_SegmentValues
{
public:
    fpreal	t0, t1;
    fpreal	iv, ov, im, om, isl, osl;
    
    CH_SegmentValues() {}

    CH_SegmentValues( CH_Segment const& seg );

    void	display();
};

#define CH_MAX_SLOPE		1e8
#define CH_MAX_ACCEL		1e8

class CH_API CH_Segment
{
public:

    // Minimal constructor with no expression
    CH_Segment(CH_Channel *mom);

    // Default constructor creates an expression with which returns the
    //	default value
    CH_Segment(fpreal defVal, CH_Channel *mom, fpreal len = 0);

    // Copy constructor.
    CH_Segment(const CH_Segment &from);

    // Constructor for an expression segment.  By default, there is
    // no callback function and no raw array
    CH_Segment(const char *expr, CH_ExprLanguage language, CH_Channel *mom,
	       fpreal len = 0);

    ~CH_Segment();

    int64	 getMemoryUsage(bool inclusive) const;

    CH_Manager	*getManager() const;
    fpreal	 getTolerance() const;

    void	 save(std::ostream &os, int binary, bool compiled,
		      bool disabled) const;
    template <typename FPREAL_TYPE>
    bool	 load(UT_IStream &is, bool disabled,
		      bool reload=false);

    // Now we define some operators so that we can use these objects
    // in the UT_Array of channels
    CH_Segment	&operator= (const CH_Segment &from);
    int		 operator==(const CH_Segment &) { return 0; }

    // The segment may be evaluated by using any of the following
    // evaluation methods.  The segment can be evaluated by relative
    // time, absolute time or both.
    // If extend is non-zero, the left(1) or right(2) value along the 
    // slope is calculated accordingly.
    fpreal		 evaluate(fpreal ltime, bool extend /*=false*/,
				  int thread);
    void		 evaluateString(UT_String &result,
					fpreal ltime, bool extend /*=false*/,
					int thread);

    // Clears and reparses the expression:
    void		 unresolveLocalVars(int thread);

    CH_Channel		*getChannel(void) const { return myParent; }
    void		 setChannel(CH_Channel *chp) { myParent = chp; }

    void		 setExprLanguage(CH_ExprLanguage language);
    CH_ExprLanguage	 getExprLanguage() const;

    // If we were to ask for the value of the segment as a string, this
    // function returns what the meaning of that string would be (either a
    // literal or expression in some language).
    CH_StringMeaning	 getStringMeaning();

    int			 isMatchFunction(void) const;
    int			 isLockedSegment() const    { return myFlags.isLocked; }
    int			 isChopSegment() const    { return myFlags.isChop; }

    CH_SegmentFlags	   getFlags() const { return myFlags; }
    void		 lockLength(int onoff) {  myFlags.lockLength = onoff; }
    void		  lockStart(int onoff) {  myFlags.lockStart  = onoff; }
    void		    lockEnd(int onoff) {  myFlags.lockEnd    = onoff; }
    void		 tieInValue(int onoff, bool dotie = true);
    void		tieOutValue(int onoff, bool dotie = true);
    void		 tieInSlope(int onoff, bool dotie = true,
					fpreal ar = 0.0 );
    void		tieOutSlope(int onoff, bool dotie = true,
					fpreal ar = 0.0 );
    void		 tieInAccel(int onoff, bool dotie = true);
    void		tieOutAccel(int onoff, bool dotie = true);

    void	        autoInSlope(int onoff);
    void	       autoOutSlope(int onoff);

    // Turn cooking on and set the sample array size or turn cooking off...
    void		cook(int state, int unused = 0);

    int			     isCooking() const { return myFlags.isCooking; }
    int			isLengthLocked() const { return myFlags.lockLength; }
    int			 isStartLocked() const { return myFlags.lockStart; }
    int			   isEndLocked() const { return myFlags.lockEnd; }
    int			 isInValueTied() const { return myFlags.tieInValue; }
    int			isOutValueTied() const { return myFlags.tieOutValue; }
    int			 isInSlopeTied() const { return myFlags.tieInSlope; }
    int			isOutSlopeTied() const { return myFlags.tieOutSlope; }
    int			 isInAccelTied() const { return myFlags.tieInAccel; }
    int			isOutAccelTied() const { return myFlags.tieOutAccel; }
    int			    isConstant() const { return myFlags.isConstant; }
    int			       isCubic() const { return myFlags.isCubic; }
    int			      isBezier() const { return myFlags.isBezier; }
    int			      isLinear() const { return myFlags.isLinear; }
    int			     isQLinear() const { return myFlags.isQLinear; }
    int			      isQCubic() const { return myFlags.isQCubic; }
    int			  isQuaternion() const
					{ return isQLinear() || isQCubic(); }

    int			 isInSlopeAuto() const { return myFlags.autoInSlope; }
    int			isOutSlopeAuto() const { return myFlags.autoOutSlope; }

    int			 isInValueUsed();	// These methods query the
    int			isOutValueUsed();	//  expression to find out
    int			 isInSlopeUsed();
    int			isOutSlopeUsed();
    int			 isInAccelUsed();
    int			isOutAccelUsed();
    int			  isSplineUsed();

    // For simple functions that CH_SegmentFlags supports, return the
    // CH_Support.h user flags for whether the value/slope/accels are used
    // without needing to evaluate the expression. Returns false if all of the 
    // CH_SegmentFlags::isCubic, isBezier, etc. are false.
    bool		getFunctionKeyUserFlags(int thread, unsigned &flags);

    // Does a quick check to see if this segment is time dependent, using the
    // flags set from the last evaluation.
    bool		isTimeDependent() const;
    // If evaluate_to_find_out is false, the expression's flags will just be
    // checked.  In that case, if the expression was never evaluated since
    // it last changed, this function may return that it's not time dependent
    // even if it is.
    bool		isTimeDependent(bool evaluate_to_find_out,
					int thread) const;

    // This function will return if the value is time dependent, and, if
    // it's not, the value argument will be set.  This way, you can compare
    // adjacent values between segments to quickly check for time dependencies.
    // This function will evaluate the expression if it's not already flagged
    // as time dependent.
    bool		isTimeDependentAndGetFloat(fpreal &value,
						   int thread) const;

    // This function is like isTimeDependentAndGetFloat(), except it will
    // evaluate the expression as a string.  Note that the char* may be
    // set to null if the segment is time dependent.  If it's not null, it
    // points to a strdup'd string that you must free().
    bool		isTimeDependentAndGetString(UT_String &value,
						    int thread) const;

    bool		hasNonIntegerKeys() const;
    unsigned		isDataDependent() const;

    //  The following methods are used to change the segment times
    //	(start, end, duration), and also for checking whether the
    //	segment can be changed.
    int			canChange(fpreal start, fpreal end) const;
    void		changeLength(fpreal len,
				     CH_SegmentScale how=CH_SCALE_ANY,
				     bool accel_ratio = true);
    void		changeTimes(fpreal s, fpreal e);

    // This method should only be called by CH_Channel - hands off.
    void		stretch(fpreal scale, int adjust_slopes,
				int adjust_accel);

    //  Some query methods
    const char		*getExpression(void) const;
    const CH_Expression	*getCHExpr() const  { return myExpression; }
    CH_Expression	*getCHExpr()	    { return myExpression; }

    bool		 isEndSegment() const
			 {
			    return myLength == CONST_FPREAL(0);
			 }
    fpreal		 getInValue(void)      const { return inValue; }
    fpreal		 getOutValue(void)     const { return outValue; }
    fpreal		 getInSlope(void)      const { return inSlope; }
    fpreal		 getOutSlope(void)     const { return outSlope; }
    fpreal		 getInAccel(void)      const { return inAccel; }
    fpreal		 getOutAccel(void)     const { return outAccel; }
    fpreal		 getInAccelRatio() const
			 {
			     return getAccelRatio( inSlope, inAccel, myLength );
			 }
    fpreal		 getOutAccelRatio() const
			 {
			     return getAccelRatio( outSlope, outAccel,
						    myLength );
			 }
    static fpreal	 getAccelRatio(fpreal slope, fpreal accel,
				       fpreal length);
    static fpreal	 getAccelFromRatio(fpreal slope, fpreal ratio,
					   fpreal length)
			 {
			     if( length==0.0 )
				length = 1.0;
			    return length * ratio * SYSsqrt( slope*slope + 1 );
			 }

    CH_Segment		*getNext()	    { return myNext; }
    const CH_Segment	*getNext() const    { return myNext; }
    void		 setNext(CH_Segment *segp)
					    { myNext = segp; }
    CH_Segment		*getPrev()	    { return myPrev; }
    const CH_Segment	*getPrev() const    { return myPrev; }
    void		 setPrev(CH_Segment *segp)
					    { myPrev = segp; }

    fpreal		 getLength (void) const { return myLength; }
    fpreal		 getILength(void) const { return myInverseLength; }

    fpreal		 getStart(void) const { return myStart; }
    fpreal		 getEnd  (void) const { return myEnd; }

    fpreal		 relativeTime(fpreal t) const
			 {
			     return (t - myStart) * myInverseLength;
			 }

    void		changeExpression(const char *expr,
				CH_ExprLanguage language, bool convert_accels);
    void		changeExpression(const char *expr,
				CH_ExprLanguage language);

    // Return whether or not an expression is a call to an animation function.
    // Note that such expressions are compatible between different languages.
    // The static method is for an arbitrary expression string and the
    // non-static is for this segment.
    static bool		expressionIsAnimationFunctionCall(
			    const char *expression);
    bool		expressionIsAnimationFunctionCall();

    // When an application calls the following methods to set in/out values,
    // they should leave the doTie as 1, it's only used internally.
    void		 setInValue(fpreal v, bool dotie = true);
    void		setOutValue(fpreal v, bool dotie = true);
    void		 setInSlope(fpreal v, bool dotie = true, fpreal ar = 0.0);
    void		setOutSlope(fpreal v, bool dotie = true, fpreal ar = 0.0);
    void		 setInAccel(fpreal v, bool dotie = true);
    void		setOutAccel(fpreal v, bool dotie = true);
    void		 setInAccelRatio(fpreal ratio, bool dotie = true)
			 {
			    fpreal v = getAccelFromRatio(inSlope, ratio,
							 myLength);
			    setInAccel( v, dotie );
			 }
    void		 setOutAccelRatio(fpreal ratio, bool dotie = true )
			 {
			    fpreal v = getAccelFromRatio(outSlope, ratio,
							 myLength);
			    setOutAccel( v, dotie );
			 }
    void		clampInAccel();
    void		clampOutAccel();
    void		normalizeAccels();

    void		setPrevNext(CH_Segment *p, CH_Segment *n)
			{
			    myPrev = p;
			    myNext = n;
			}

    // Sorry, you can only flag a segment as being changed, it's got it's
    // own internal mechanism for flagging this
    void		setChanged();

    // Only the CH_Channel class should call the following method(s)
    void		reverse();

    // Op dependencies
    void		buildOpDependencies(void *ref_id, int thread);
    int			changeOpRef(const char *new_fullpath,
				    const char *old_fullpath,
				    const char *old_cwd,
				    const char *chan_name,
				    const char *old_chan_name,
				    int thread);

    int			findString(const char *str, bool fullword,
				   bool usewildcards) const;
    int			changeString(const char *from, const char *to,
				     bool fullword, int thread);

    void		transferGroup(CH_Segment &from);

    int			 wasModified() const { return myFlags.modified; }
    void		 setModified(int onOff);

    void		dirtyExprCache();

    fpreal		getValue(bool left, CH_ValueTypes t) const;
    void		setValue(bool left, CH_ValueTypes t, fpreal value);
    bool		getValueValid(bool left, CH_ValueTypes t);
    bool		getValueTied(bool left, CH_ValueTypes t) const;
    void		setValueTied(bool left, CH_ValueTypes t, bool on_off);
    bool		getValueAuto(bool left, CH_ValueTypes t) const;
    void		setValueAuto(bool left, CH_ValueTypes t, bool on_off);

    void		display();

    CH_TimeGroup	*getTimeGroup(void)  const { return myTimeGroup; }

private:
    void		 setTimeGroup(CH_TimeGroup *g, bool refresh=true); 

    void		 setExpression(const CH_Expression *newexpr);
    void		 checkExpression();
    void		 constructorInit(void);	// Initialize all values
    void		 isCooking(int state)	// Toggle state during cook
			 { myFlags.isCooking = state; }
    bool		 loadTimeGroups(UT_IStream &is);
    void		 saveTimeGroups(std::ostream &is, int binary) const;

    void		 holdLastKey(fpreal v, bool dotie);

    void		 updateExprFlags();

    bool		 isExprTimeDep() const;

    // the member variables marked with (*) should be removed as they are
    // actually useless

    CH_SegmentFlags	 myFlags;

    // Here is the local context information for the segment.  It is
    // be used for evaluation of the segment.
    //
    // All times and lengths of time are in the channel local time.
    CH_Expression	*myExpression;

    fpreal		 inValue, outValue;
    fpreal		 inSlope, outSlope;
    fpreal		 inAccel, outAccel;

    CH_TimeGroup	*myTimeGroup;

    // (*) we shouldn't store length, just use getNext()->myStart - myStart
    // storing inverse length is acceptable
    fpreal		 myLength;
    fpreal		 myInverseLength;	// Inverse stored for speed

    // (*) myEnd is redundant, and is not really that much more efficient
    fpreal		 myStart, myEnd;	// Redundant; for efficiency

    CH_Channel		*myParent;		// Our channel.

    // (**) ideally we would store segments in a ref array, in which case
    // getNext could ideally be (this+1), ditto for prev, but there are some
    // boundary cases of course where this gets weird
    CH_Segment		*myPrev, *myNext;	// For easy reference

    friend class	 CH_TimeGroup;
};

inline bool
CH_Segment::isTimeDependent() const
{
    if (myFlags.isConstant || myFlags.isLinear || myFlags.isEase ||
	    myFlags.isEaseIn || myFlags.isEaseOut)
	return inValue != outValue;

    if (myFlags.isCubic || myFlags.isBezier)
	return (inValue != outValue) || inSlope || outSlope;

    // If the expression is already flagged as time-dependent, we don't
    // need to do any evaluation.
    return isExprTimeDep();
}

#endif
