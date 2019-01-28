/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU Library (C++)
 *
 * COMMENTS:
 *	Provides access to the GOP level group functions so you
 *	can parse groups and keep track of adhoc group membership.
 *
 *	During the scope of this object any groups created
 *	will be preserved, and they will all be deleted when
 *	it is out of scope.
 *
 *	If working with const gdps, the detached versions should be used.
 *
 */

#ifndef __GU_GroupManager_h__
#define __GU_GroupManager_h__

#include "GU_API.h"
#include <SYS/SYS_Deprecated.h>
#include <SYS/SYS_Types.h>

class UT_String;
class GU_Detail;
class GA_PointGroup;
class GA_PrimitiveGroup;
class GA_VertexGroup;
class GOP_Manager;

class GU_API GU_GroupManager
{
public:
    SYS_DEPRECATED_HDK_REPLACE(16.0, GOP_Manager)
	     GU_GroupManager();
    SYS_DEPRECATED_HDK_REPLACE(16.0, GOP_Manager)
	    ~GU_GroupManager();

    // Always return detached, unordered, groups.
    // If forceexistence is true, will always return a valid group pointer
    // by creating an empty group if necessary.
    // Success will be set to false if the group failed to be created.
    // The returned group may be an alias of an existing group
    // if the group naming is trivial
    SYS_DEPRECATED_HDK_REPLACE(16.0, GOP_Manager::parsePrimitiveDetached())
    const GA_PrimitiveGroup	*parsePrimitiveDetached(const char *pat, 
					      const GU_Detail *pgdp, 
					      bool forceexistence,
					      bool &success);
    SYS_DEPRECATED_HDK_REPLACE(16.0, GOP_Manager::parsePointDetached())
    const GA_PointGroup		*parsePointDetached(const char *pat, 
					      const GU_Detail *pgdp, 
					      bool forceexistence,
					      bool &success);
    
    SYS_DEPRECATED_HDK_REPLACE(16.0, GOP_Manager::parseVertexDetached())
    const GA_VertexGroup	*parseVertexDetached(const char *pat,
					      const GU_Detail *pgdp,
					      bool forceexistence,
					      bool &success);

private:
    GOP_Manager			*myManager;
};

#endif
