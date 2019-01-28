/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_EdgeGroupTable.h (GA Library, C++)
 *
 * COMMENTS:	A table for managing GA_EdgeGroup objects.
 *
 */

#ifndef __GA_EdgeGroupTable_h__
#define __GA_EdgeGroupTable_h__

#include "GA_API.h"
#include "GA_EdgeGroup.h"
#include "GA_GroupTable.h"
#include "GA_Types.h"

#include <UT/UT_OrderedIterator.h>


class GA_Detail;
class GA_Defragment;
class GA_Group;
class GA_MergeMap;
class GA_Range;

class UT_JSONParser;
template <typename T> class UT_Array;


class GA_API GA_EdgeGroupTable : public GA_GroupTable
{
public:
    typedef GA_GroupTable::iterator<GA_EdgeGroup> iterator;

    		 GA_EdgeGroupTable(GA_Detail &detail);
    virtual	~GA_EdgeGroupTable();

    void	 getList(UT_Array<const GA_EdgeGroup *> &list) const;
    void	 getList(UT_Array<GA_EdgeGroup *> &list) const;

    /// @{
    /// Traverse the group table in creation order.
    ///
    /// @warning It is NOT safe to add or remove groups during traversal.
    GA_GroupTable::iterator<GA_EdgeGroup> beginTraverse() const
			{ return begin<GA_EdgeGroup>(); }
    GA_GroupTable::iterator<GA_EdgeGroup> endTraverse() const
			{ return end<GA_EdgeGroup>(); }
    /// @}

    /// @{
    /// Traverse the group table in alpabetic order.
    static int	compareAlpha(GA_EdgeGroup *const*a, GA_EdgeGroup *const*b)
		    { return compareGroupName(**a, **b); }

    typedef UT_OrderedIterator<GA_EdgeGroup *,
		    GA_GroupTable::iterator<GA_EdgeGroup> > ordered_iterator;
    ordered_iterator	obegin() const
	    {
		GA_GroupTable::iterator<GA_EdgeGroup>	it = beginTraverse();
		return ordered_iterator(it, compareAlpha);
	    }
    ordered_iterator	oend() const
	    { return ordered_iterator(); }
    /// @}

    void	 mergeTable(const GA_EdgeGroupTable &src,GA_MergeMap &map);

    void	 mergeEdgeGroups(const GA_MergeMap &map);

    void	 removePointsFromGroups(const GA_Range &range);
    void	 removePointFromGroups(const GA_Offset ptoff);
    void	 removePrimsFromGroups(const GA_Range &range);
    void	 removePrimFromGroups(const GA_Offset primoff);

    /// Called by GA_Detail::defragment to update point offsets
    /// in edge groups when the point index map is being defragmented.
    /// It's also called there for the primitive index map, in case
    /// edge groups ever switch to using primitive offsets, instead of
    /// primitive pointers.
    /// NOTE: This will bump data IDs of any modified edge groups.
    void	 defragment(const GA_Detail *gdp,const GA_Defragment &defrag);

    GA_Size	 destroyEmptyGroups();

    void         bumpAllDataIds();

    void         makeAllEdgesValid();

protected:
    /// Method to create an edge group
    /// @see GA_GroupTable::jsonLoad()
    virtual GA_Group	*jsonCreateGroup(UT_JSONParser &p, const char *type,
				    const char *name, bool internal,
				    GA_AttributeOwner owner);

    virtual GA_Group *virtualCreateGroup(
        GA_Detail &detail,
        const UT_StringHolder &name,
        bool internal) const
    {
        return new GA_EdgeGroup(detail, name, internal, false);
    }

    virtual GA_Group *virtualCreateDetachedGroup(
        const GA_Detail &detail) const
    {
        return new GA_EdgeGroup(detail);
    }
};

#endif
