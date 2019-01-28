/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_IntrinsicDef.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_IntrinsicDef__
#define __GA_IntrinsicDef__

#include "GA_API.h"
#include "GA_Types.h"

/// @brief Stores information about intrinsic attributes for classes
///
/// When defining a new primitive which has intrinsic attributes, this class
/// stores information about the mapping between the GA_IntrinsicManager and
/// the user data passed in the registration.
class GA_API GA_IntrinsicDef
{
public:
     GA_IntrinsicDef()
	: myStart(-1)
	, myEnd(-1)
    {};
    ~GA_IntrinsicDef()
    {}

private:
    bool	setStart(int start);
    bool	setEnd(int end);
    int		getLocalId(GA_LocalIntrinsic h) const
		{
		    return (h >= myStart && h < myEnd) ? h : -1;
		}

    int		myStart, myEnd;

    friend class	GA_IntrinsicManager;
    friend class	GA_IntrinsicEval;
};

#endif
