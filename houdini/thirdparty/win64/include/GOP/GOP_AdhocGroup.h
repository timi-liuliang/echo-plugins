/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */
#ifndef __GOP_AdhocGroup_h__
#define __GOP_AdhocGroup_h__

#include "GOP_API.h"
#include <SYS/SYS_Types.h>

class GA_Group;

class GOP_API GOP_AdhocGroup
{
public:
    GOP_AdhocGroup() {}
    
    GOP_AdhocGroup(GA_Group *group, bool ownedbyme)
        : myGroup(group)
        , myOwnedByMe(ownedbyme)
    {}

    bool operator==(const GOP_AdhocGroup &g) const
    {
        return (myGroup == g.myGroup &&
                myOwnedByMe == g.myOwnedByMe);
    }

    GOP_AdhocGroup &operator=(const GOP_AdhocGroup &g) = default;

    // This method will destroy the adhoc group from the gdp.
    // It should return true if it actually removes a group
    // from the gdp, or false otherwise.
    bool destroyAdhocGroup();

    int64 getMemoryUsage(bool inclusive) const;

public:
    GA_Group *myGroup;
    bool      myOwnedByMe;
};

#endif
