/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	STY_TargetMatchStatus.h ( STY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __STY_TargetMatchStatus__
#define __STY_TargetMatchStatus__

#include "STY_API.h"
#include "STY_Types.h"

//  =========================================================================== 
/// Contains information about the style subject match to the style target.
class STY_API STY_TargetMatchStatus
{
public:
    /// Constructor: for fully matched or not matched target test result.
    			STY_TargetMatchStatus( bool is_match )
			    :   myIsMatch( is_match )
			{}

    /// Constructor: for partially matched target test result.
    			STY_TargetMatchStatus( bool is_match,
			    const STY_TargetHandle & next_target )
			    :   myIsMatch( is_match )
			    ,   myNextTarget( next_target )
			{}

    /// Returns true if the subject matched the target, whether fully or
    /// partially.
    bool		isMatch() const
			{ return myIsMatch; }

    /// Returns true if the subject fully matched the target.
    bool		isFullMatch() const
			{ return myIsMatch && !myNextTarget; }

    /// Returns the target that should be tested next.
    const STY_TargetHandle &getNextTarget() const
			{ return myNextTarget; }

private:
    /// Flag set to true if the style matched the target, fully or partially.
    bool		myIsMatch;

    /// Holds the target that should be matched next.
    /// If null, the last test fully matched the target, otherwise
    /// it points to sub-target that still needs to be tested and be matched.
    STY_TargetHandle	myNextTarget;
};

#endif

