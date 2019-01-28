/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	STY_SubjectGroup.h ( STY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __STY_SubjectGroup__
#define __STY_SubjectGroup__

#include "STY_API.h"
#include <UT/UT_UniquePtr.h>

class STY_Subject;

/// Represents a collection of STY_Subjects in a single class. This is used
/// by the multi-subject member functions of STY_Styler.
class STY_API STY_SubjectGroup
{
public:
    virtual			~STY_SubjectGroup();

    /// Returns the number of subjects in this group.
    virtual int			 getNumSubjects() const = 0;
    /// Returns a subject by index.
    virtual const STY_Subject	&getSubject(int idx) const = 0;
};

#endif

