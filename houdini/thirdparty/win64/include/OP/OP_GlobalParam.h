/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_GlobalParam.h ( OP Library, C++)
 *
 * COMMENTS:	This file contains accessors for global parameters.  These
 *		parameters make use of the stamp/param mechanism and allow for
 *		proper propagation of changes.
 */

#ifndef __OP_GlobalParam__
#define __OP_GlobalParam__

#include "OP_API.h"
#include <SYS/SYS_Types.h>
#include <UT/UT_Thread.h>

enum {
    OP_GLOBAL_PARAM_INSTANCEPOINT
};

class OP_API OP_GlobalParam {
public:
    static fpreal	getInstancePoint(int thread,
					 bool add_dependencies=true);
    static void		setInstancePoint(int ptnum);
};

class OP_API OP_GlobalParamPusher {
public:
    explicit	OP_GlobalParamPusher(int param)
		    : myParam(param)
    {
	switch (param)
	{
	    case OP_GLOBAL_PARAM_INSTANCEPOINT:
		myIValue = (int)OP_GlobalParam::getInstancePoint(SYSgetSTID(),
							false);
		break;
	}
	myChanged = false;
    }
    ~OP_GlobalParamPusher()
    {
	if (myChanged)
	{
	    switch (myParam)
	    {
		case OP_GLOBAL_PARAM_INSTANCEPOINT:
		    OP_GlobalParam::setInstancePoint(myIValue);
		    break;
	    }
	}
    }
    void	setIValue(int val)
		{
		    switch (myParam)
		    {
			case OP_GLOBAL_PARAM_INSTANCEPOINT:
			    if (val != myIValue || myChanged)
			    {
				OP_GlobalParam::setInstancePoint(val);
				myChanged = true;
			    }
			    break;
		    }
		}
    int		getIValue() const	{ return myIValue; }
private:
    int		myParam;
    int		myIValue;
    bool	myChanged;
};

#endif
