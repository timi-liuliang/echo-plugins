/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CH_EvalContextFwd.h (CH Library, C++)
 *
 * COMMENTS:
 */

#ifndef __CH_EVALCONTEXTFWD_H_INLUDED__
#define __CH_EVALCONTEXTFWD_H_INLUDED__

#include "CH_API.h"
#include <DEP/DEP_ContextOptionsFwd.h>
#include <UT/UT_COW.h>
#include <UT/UT_NonCopyable.h>
#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Types.h>

class CH_AutoEvalCollection;
class CH_AutoEvaluateTime;
class CH_Channel;
class CH_Collection;
class CH_Manager;
class CH_ScriptAccessManager;
class CH_Segment;

/// @file CH_EvalContextFwd.h
///
/// Define CH_EvalContext enough to be used as data members.
/// Include CH_EvalContext.h which has additional includes to be able to call
/// all methods on it.

/// Information regarding the current evaluation.
///
/// NOTES:
/// - All of the pointers may be NULL in general.
/// - If channel() is non-NULL, it must belong to the non-NULL collection()
/// - If channel() is non-NULL, then channelName() and segment() is non-NULL
/// - If segment() is non-NULL <==> collection()/channel() are non-NULL
/// - If segment() is non-NULL it must belong to the non-NULL channel()
///
class CH_API CH_EvalContext
{
public:
    class Scope;

    inline fpreal		time() const
				    { return myTime; }
    inline const CH_Collection*	collection() const
				    { return myCollection; }
    inline CH_Collection*	collection()
				    { return myCollection; }
    inline const DEP_ContextOptionsStack *contextOptionsStack() const
				    { return myContextOptionsStack; }
    inline DEP_ContextOptionsReadHandle contextOptions() const
				    { return myContextOptions; }
    inline const CH_Channel*	channel() const
				    { return myChannel; }
    inline CH_Channel*		channel()
				    { return myChannel; }
    inline const CH_Segment*	segment() const
				    { return mySegment; }
    inline CH_Segment*		segment()
				    { return mySegment; }
    inline const char*		channelName() const
				    { return myChannelName; }
    inline CH_EvalContext	*parentContext()
				    { return myParentContext; }

private:
    fpreal				 myTime;
    CH_Collection			*myCollection;
    const DEP_ContextOptionsStack	*myContextOptionsStack;
    DEP_ContextOptionsReadHandle	 myContextOptions;
    CH_Channel				*myChannel;
    CH_Segment				*mySegment;

    // Note that because hscript expression string parms can add
    // dependencies even though they don't have a channel, we have to store
    // both the evaluating channel and the evaluating channel name.
    const char				*myChannelName;
    CH_EvalContext			*myParentContext;

    friend class CH_Manager;
    friend class CH_EvalContext::Scope;
    friend class CH_AutoEvaluateTime;
    friend class CH_AutoEvalCollection;
};

// Make CH_EvalContext a POD type for UT_ThreadSpecificValue for zero init.
template <> constexpr SYS_STATIC_INLINE bool
SYSisPOD<CH_EvalContext>() { return true; }

/// Object to save the current CH_EvalContext and then restore it when it goes
/// out of scope.
class CH_API CH_EvalContext::Scope : UT_NonCopyable
{
public:
    Scope(int thread);
    Scope(int thread, bool push_access_manager);
    ~Scope();

    inline fpreal		time() const
				    { return myContext.myTime; }
    inline const CH_Collection*	collection() const
				    { return myContext.myCollection; }
    inline const CH_Channel*	channel() const
				    { return myContext.myChannel; }
    inline const CH_Segment*	segment() const
				    { return myContext.mySegment; }
    inline const char*		channelName() const
				    { return myContext.myChannelName; }

    inline void
    setTime(fpreal t)
    {
	myContext.myTime = t;
    }

    inline void
    setCollection(CH_Collection* collection)
    {
	myContext.myCollection = collection;
    }

    // Sets myChannel to channel, myChannelName to myChannel->getName(), and
    // mySegment to seg. (not inline to avoid #including CH_Channel.h)
    void setChannel(CH_Channel* channel, CH_Segment* seg);

    // Sets myChannel/mySegment to NULL and myChannelName to channel_name.
    void setChannelName(const char* channel_name)
    {
	myContext.myChannel = NULL;
	myContext.mySegment = NULL;
	myContext.myChannelName = channel_name;
    }


    inline void
    setSegment(CH_Segment* segment)
    {
	myContext.mySegment = segment;
    }

    // Common combinations
    inline void
    set(fpreal t, CH_Collection* collection, CH_Channel* chp, CH_Segment* seg)
    {
	myContext.myTime = t;
	myContext.myCollection = collection;
	setChannel(chp, seg);
    }
    inline void
    set(fpreal t, CH_Collection* collection)
    {
	myContext.myTime = t;
	myContext.myCollection = collection;
	setChannel(NULL, NULL);
    }
    inline void
    set(CH_Collection* collection, CH_Channel* channel)
    {
	myContext.myCollection = collection;
	setChannel(channel, NULL);
    }
    inline void
    set(CH_Collection* collection, const char* channel_name)
    {
	myContext.myCollection = collection;
	setChannelName(channel_name);
    }

private:
    CH_EvalContext&		myContext;
    CH_ScriptAccessManager*	myAccessMgr;
    CH_EvalContext		myPrevContext;
};

#endif // __CH_EVALCONTEXTFWD_H_INLUDED__
