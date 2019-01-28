/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	STY_SubjectNamed.h ( STY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __STY_SubjectNamed__
#define __STY_SubjectNamed__

#include "STY_API.h"
#include "STY_Subject.h"
#include <SYS/SYS_Types.h>

/// Helper class for subjects that have names and IDs.
class STY_API STY_SubjectNamed : public STY_Subject
{
protected:
    // Returns true if name matches.
    bool		    matchesNameHelper( const char *name ) const;

    // Returns true if does not match the path or matches only partially.
    // The match argument contains the result.
    bool		    matchesFullPathHelper( 
				STY_TargetMatchStatus &match,
				const STY_TargetHandle &target ) const;

    // Obtains name and ID from subclass.
    virtual UT_StringHolder	getSubjectName() const = 0;
    virtual int64		getSubjectID() const = 0;
};

#endif

