/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	STY_StylerMatch.h (STY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __STY_StylerMatch__
#define __STY_StylerMatch__

#include "STY_API.h"
#include "STY_Types.h"
#include "STY_Target.h"
#include "STY_TargetMatchClosure.h"
#include <UT/UT_NonCopyable.h>
#include <UT/UT_IntrusivePtr.h>

class STY_Style;

/// Active entry referencing an underlying style sheet entry, 
/// however with new target and with closure data constructed
/// from matching previous target components.
class STY_API STY_StylerMatch :
	public UT_NonCopyable,
	public UT_IntrusiveRefCounter<STY_StylerMatch>
{
public:
    /// Constructor.
    STY_StylerMatch(const STY_Style *style,
	    const STY_TargetHandle &target,
	    const STY_TargetMatchClosureHandle &closure,
	    bool owns_style_entry = false);
    ~STY_StylerMatch();

    /// @{ Accessors.
    const STY_Style			*getStyle() const
					 { return myStyle; }
    const STY_TargetHandle		&getTarget() const  
					 { return myTarget; }
    const STY_TargetMatchClosureHandle	&getClosure() const 
					 { return myClosure; }
    /// @}

    /// Returns true if this is a full match, meaning all child subjects
    /// will match because there are no additional targeting conditions.
    bool				 isFullMatch() const;

private:
    /// An underlying original style entry from the original style sheet.
    const STY_Style			*myStyle;

    /// A new target associated with the entry. 
    /// This is the target that the next subject needs to match. 
    /// If NULL, the original target has been fully matched already.
    STY_TargetHandle			 myTarget;

    /// Closure containing information and data from previous matches
    /// of the original target components (sub-targets).
    STY_TargetMatchClosureHandle	 myClosure;

    /// Tracks whether this object owns its myStyle pointer.
    bool				 myOwnStyle;
};

#endif

