/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __GOP_Manager_h__
#define __GOP_Manager_h__

#include "GOP_API.h"
#include "GOP_AdhocGroup.h"
#include "GOP_GroupParse.h"
#include <UT/UT_Array.h>
#include <SYS/SYS_Deprecated.h>

class GEO_Detail;
class GA_BreakpointGroup;
class GA_EdgeGroup;
class GA_PointGroup;
class GA_PrimitiveGroup;
class GA_VertexGroup;

class GOP_API GOP_Manager
{

public:
    GOP_Manager() {};
    ~GOP_Manager()
    {
        destroyAdhocGroups();
    }

    typedef GOP_GroupParse::ParseInfo ParseInfo;
    typedef GOP_GroupParse::GroupCreator GroupCreator;

    const GA_PrimitiveGroup	*parsePrimitiveGroups(const char *pat,
					      const GroupCreator &creator,
					      bool numok = true,
					      bool ordered = false,
					      bool strict = false,
					      GA_Index prim_offset = GA_Index(0),
					      ParseInfo *info = 0);
    SYS_DEPRECATED_HDK_REPLACE(15.0,GroupCreator version of function and dont cast away const on input geo and be careful since defaults have changed)
    const GA_PrimitiveGroup	*parsePrimitiveGroups(const char *pat,
					      GEO_Detail *pgdp,
					      int numok = 1,
					      int ordered = 1,
					      bool strict = false,
					      int prim_offset = 0,
					      bool allowdetached = false,
					      ParseInfo *info = 0);

    const GA_PointGroup		*parsePointGroups(const char *pat,
					      const GroupCreator &creator,
					      bool numok = true,
					      bool ordered = false,
					      bool strict = false,
					      GA_Index point_offset = GA_Index(0),
					      ParseInfo *info = 0);
    SYS_DEPRECATED_HDK_REPLACE(15.0,GroupCreator version of function and dont cast away const on input geo and be careful since defaults have changed)
    const GA_PointGroup		*parsePointGroups(const char *pat,
					      GEO_Detail *pgdp,
					      int numok = 1,
					      int ordered = 1,
					      bool strict = false,
					      int point_offset = 0,
					      bool allowdetached = false,
					      ParseInfo *info = 0);

    const GA_EdgeGroup		*parseEdgeGroups(const char *pat,
	                                         const GEO_Detail *pgdp,
						 bool strict = true,
						 GA_Index prim_offset = GA_Index(0),
						 GA_Index point_offset = GA_Index(0),
						 ParseInfo *info = 0);

    const GA_BreakpointGroup	*parseBreakpointGroups(const char *pat,
						 const GEO_Detail *pgdp,
						 bool strict = true,
						 GA_Index prim_offset = GA_Index(0));

    // Always returns a detached group.
    const GA_VertexGroup	*parseVertexGroups(const char *pat,
						 const GEO_Detail *pgdp,
						 bool strict = true,
						 GA_Index prim_offset = GA_Index(0));
    const GA_VertexGroup	*parseVertexGroups(const char *pat,
						 const GEO_Detail *pgdp,
						 bool strict,
						 int prim_offset);

    const GA_Group		*parseAllGroups(const char *pattern, 
					    const GroupCreator &creator,
					    bool allow_numeric, 
					    bool ordered, 
					    GA_GroupMaskType mask,
					    GA_GroupType bestguess);
    //
    // Similar to above, except the caller is free to modify its contents
    //

    GA_PrimitiveGroup	*parsePrimitiveGroupsCopy(const char *pat,
						  const GroupCreator &creator,
						  bool numok = true,
						  bool ordered = false,
						  bool strict = false);

    SYS_DEPRECATED_HDK_REPLACE(15.0,GroupCreator version of function and dont cast away const on input geo and be careful since defaults have changed)
    GA_PrimitiveGroup	*parsePrimitiveGroupsCopy(const char *pat,
						  GEO_Detail *pgdp,
						  int numok = 1,
						  int ordered = 1,
						  bool strict = false,
						  bool detached = false);

    GA_PointGroup	*parsePointGroupsCopy(const char *pat,
						  const GroupCreator &creator,
						  bool numok = true,
						  bool ordered = false,
						  bool strict = false);

    SYS_DEPRECATED_HDK_REPLACE(15.0,GroupCreator version of function and dont cast away const on input geo and be careful since defaults have changed)
    GA_PointGroup	*parsePointGroupsCopy(const char *pat,
						  GEO_Detail *pgdp,
						  int numok = 1,
						  int ordered = 1,
						  bool strict = false,
						  bool detached = false);

    GA_EdgeGroup	*parseEdgeGroupsCopy(const char *pat,
					          const GEO_Detail *pgdp,
						  bool strict = true,
						  GA_Index prim_offset = GA_Index(0),
						  GA_Index point_offset = GA_Index(0));

    GA_BreakpointGroup	*parseBreakpointGroupsCopy(const char *pat,
						  const GEO_Detail *pgdp);

    // Always returns a detached group.
    GA_VertexGroup	*parseVertexGroupsCopy(const char *pat,
						  const GEO_Detail *pgdp);

    GA_Group		*parseAllGroupsCopy(const char *pattern, 
					    const GroupCreator &creator,
					    bool allow_numeric, 
					    bool ordered, 
					    GA_GroupMaskType mask,
					    GA_GroupType bestguess);


    /// Parse a guaranteed copy (ie, strict) group suitable for
    /// writing to.  Must be done on a writable geometry.
    /// Can replace cookInputAllGroups with allow reference false
    /// on your own geometry.
    GA_Group		*parseGroupCopy(const char *pat,
					      GA_GroupType grouptype,
					      GEO_Detail *pgdp,
					      bool forceexistence,
					      bool ordered,
					      bool defaultprim,
					      bool &success);
    /// Parse detached, unordered, groups.
    /// If forceexistence is true, will always return a valid group pointer
    /// by creating an empty group if necessary.
    /// Success will be set to false if the group failed to be created.
    /// The returned group may be an alias of an existing group
    /// if the group naming is trivial
    /// @{
    const GA_PrimitiveGroup	*parsePrimitiveDetached(const char *pat, 
					      const GEO_Detail *pgdp, 
					      bool forceexistence,
					      bool &success);
    const GA_PrimitiveGroup	*parsePrimitiveDetached(const char *pat, 
					      const GEO_Detail *pgdp, 
					      bool forceexistence,
					      bool ordered,
					      bool &success);
    const GA_PointGroup		*parsePointDetached(const char *pat, 
					      const GEO_Detail *pgdp, 
					      bool forceexistence,
					      bool &success);
    const GA_PointGroup		*parsePointDetached(const char *pat, 
					      const GEO_Detail *pgdp, 
					      bool forceexistence,
					      bool ordered,
					      bool &success);
    const GA_EdgeGroup		*parseEdgeDetached(const char *pat,
					      const GEO_Detail *pgdp,
					      bool forceexistence,
					      bool &success);
    const GA_VertexGroup	*parseVertexDetached(const char *pat,
					      const GEO_Detail *pgdp,
					      bool forceexistence,
					      bool &success);
    const GA_Group		*parseGroupDetached(const char *pat,
					      GA_GroupType grouptype,
					      const GEO_Detail *pgdp,
					      bool forceexistence,
					      bool defaultprim,
					      bool &success);
    /// @}

    /// Parse detached, ordered, groups.
    /// If forceexistence is true, will always return a valid group pointer
    /// by creating an empty group if necessary.
    /// Success will be set to false if the group failed to be created.
    /// The returned group may be an alias of an existing group
    /// if the group naming is trivial
    /// @{
    const GA_PrimitiveGroup	*parseOrderedPrimitiveDetached(const char *pat, 
					      const GEO_Detail *pgdp, 
					      bool forceexistence,
					      bool &success);
    const GA_PointGroup		*parseOrderedPointDetached(const char *pat, 
					      const GEO_Detail *pgdp, 
					      bool forceexistence,
					      bool &success);
    /// NB: Vertex groups are always unordered.
    const GA_VertexGroup	*parseOrderedVertexDetached(const char *pat,
					      const GEO_Detail *pgdp,
					      bool forceexistence,
					      bool &success);
    /// @}

    void		 expandGroupMask(const char *pattern,
					 UT_String &outNames,
					 GA_GroupType type,
					 const GEO_Detail *gdp);

    // Create an ahdoc (internal) group and add it to the list of adhoc groups:
    GA_PrimitiveGroup	*createPrimitiveGroup(GEO_Detail &gdp,
					      const char *name = "adhoc",
					      bool detached = false);
    GA_PointGroup	*createPointGroup    (GEO_Detail &gdp,
					      const char *name = "adhoc",
					      bool detached = false);
    GA_EdgeGroup        *createEdgeGroup     (GEO_Detail &gdp,
					      const char *name = "adhoc");
    GA_BreakpointGroup  *createBreakpointGroup(const GEO_Detail &gdp,
					      const char *name = NULL);
    GA_VertexGroup      *createVertexGroup   (GEO_Detail &gdp,
					      const char *name = "adhoc",
					      bool detached = false);

    // Will return oldgroup if it is already of type newtype.
    // May return 0 if conversion impossible.
    // Result will be a correctly typed element group, possibly a new
    // one created as an optionally detached group.
    const GA_ElementGroup	*convertGroupToType(GEO_Detail &gdp,
						    GA_AttributeOwner newtype,
						    const GA_Group *oldgroup,
						    bool detached = false);

    // Remove an adhoc group or all adhocs from the list and delete them:
    int 		 destroyAdhocGroup (const GA_Group *group);
    void		 destroyAdhocGroups();

    // This will destroy all the adhoc groups that match the given gdp.
    void		 destroyAdhocGroups(const GEO_Detail *gdp);

    // Remove an adhoc group or all adhocs from the list.
    // Usually you want to call the destroy methods above to avoid headaches.
    int 		 removeAdhocGroup (const GA_Group *group);
    void		 removeAdhocGroups();

    // TODO: These 2 functions should only be called by the parsers... should we
    //       make them private and make them friends.  It doesn't really
    //       do any harm here though.
    GA_Group *lastAdhocGroup();
    void appendAdhocGroup(GA_Group *group, bool willbeownedbyme);

    // Given a base name, create a unique group name from that.
    static void		 getUniqueGroupName(const char *base, UT_String &name);

    int64 getMemoryUsage(bool inclusive) const;

private:
    UT_Array<GOP_AdhocGroup> myGroupList;
};

#endif
