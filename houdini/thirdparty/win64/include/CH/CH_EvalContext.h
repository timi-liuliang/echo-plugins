/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CH_EvalContext.h (CH Library, C++)
 *
 * COMMENTS:
 */

#ifndef __CH_EVALCONTEXT_H_INCLUDED__
#define __CH_EVALCONTEXT_H_INCLUDED__

/// @file CH_EvalContext.h
/// This file is used to split off inline implementations which require more
/// headers.

#include "CH_EvalContextFwd.h"	// See this for actual definition

#include "CH_Channel.h"
#include "CH_Manager.h"
#include "CH_ScriptAccessManager.h"


inline
CH_EvalContext::Scope::Scope(int thread)
    : myContext(CHgetManager()->evalContext(thread))
    , myAccessMgr(CHgetManager()->getAccessManager(thread))
{
    myPrevContext = myContext;
    myContext.myParentContext = &myPrevContext;
    if (myAccessMgr) 
	myAccessMgr->pushChEvalContext();
}

inline
CH_EvalContext::Scope::Scope(int thread, bool push_access_manager)
    : myContext(CHgetManager()->evalContext(thread))
    , myAccessMgr(push_access_manager
		    ? CHgetManager()->getAccessManager(thread) : NULL)
{
    myPrevContext = myContext;
    myContext.myParentContext = &myPrevContext;
    if (myAccessMgr) 
	myAccessMgr->pushChEvalContext();
}

inline
CH_EvalContext::Scope::~Scope()
{
    myContext = myPrevContext;
    if (myAccessMgr)
	myAccessMgr->popChEvalContext();
}

inline
void
CH_EvalContext::Scope::setChannel(CH_Channel* channel, CH_Segment* seg)
{
    // Note that because hscript expression string parms can add
    // dependencies even though they don't have a channel, we have to store
    // both the evaluating channel and the evaluating channel name.
    myContext.myChannel = channel;
    myContext.mySegment = seg;
    myContext.myChannelName = channel ? channel->getName().buffer() : NULL;
}

#endif // __CH_EVALCONTEXT_H_INCLUDED__
