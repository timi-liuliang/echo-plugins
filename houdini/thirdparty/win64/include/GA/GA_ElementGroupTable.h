/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_ElementGroupTable.h (GA Library, C++)
 *
 * COMMENTS:	A table for managing GA_ElementGroup objects.
 *
 */

#pragma once

#ifndef __GA_ElementGroupTable_h__
#define __GA_ElementGroupTable_h__

#include "GA_API.h"
#include "GA_ElementGroup.h"
#include "GA_GroupTable.h"
#include "GA_Types.h"

#include <UT/UT_OrderedIterator.h>

#include <iosfwd>


class GA_Detail;
class GA_ElementGroup;
class GA_MergeMap;
class GA_LoadMap;
class GA_Group;
class GA_PointGroup;
class GA_PrimitiveGroup;
class GA_Range;
class GA_RangeMemberQuery;
class GA_VertexGroup;

class UT_IStream;
class UT_JSONParser;
template <typename T> class UT_Array;


class GA_API GA_ElementGroupTable : public GA_GroupTable
{
protected:
    		 GA_ElementGroupTable(GA_Detail &detail,
				      GA_AttributeOwner owner);
public:
    virtual	~GA_ElementGroupTable();

    void	 getList(UT_Array<const GA_ElementGroup *> &list) const;
    void	 getList(UT_Array<GA_ElementGroup *> &list) const;

    typedef GA_GroupTable::iterator<GA_ElementGroup> iterator;

    /// @{
    /// Traverse the group table in creation order.
    ///
    /// @warning It is NOT safe to add or remove groups during traversal.
    GA_GroupTable::iterator<GA_ElementGroup> beginTraverse() const
			{ return begin<GA_ElementGroup>(); }
    GA_GroupTable::iterator<GA_ElementGroup> endTraverse() const
			{ return end<GA_ElementGroup>(); }
    /// @}

    /// @{
    /// Traverse the group table in alpabetic order.
    typedef UT_OrderedIterator<GA_ElementGroup *,
		GA_GroupTable::iterator<GA_ElementGroup> > ordered_iterator;
    static int	compareAlpha(GA_ElementGroup *const*a, GA_ElementGroup *const*b)
		    { return GA_GroupTable::compareGroupName(**a, **b); }
    ordered_iterator	obegin() const
	    { return ordered_iterator(beginTraverse(), compareAlpha); }
    ordered_iterator	oend() const
	    { return ordered_iterator(); }
    /// @}

    bool	 loadGroupsH9(UT_IStream &is,
				GA_AttributeOwner owner, int count);
    bool	 saveGroupsH9(std::ostream &os, int binary) const;

    /// @{
    /// Apply an operation to all element groups
    void	 addElementToAllGroups(GA_Offset dest) const;
    void	 toggleElementInAllGroups(GA_Offset dest) const;
    void	 removeElementFromAllGroups(GA_Offset dest) const;
    /// @}

    /// Add @c dest to the same groups that @c src belongs to.
    void	 unionElementForAllGroups(GA_Offset dest,
				GA_Offset src) const;
    /// Remove @c dest from the groups that @c src does not belong to.
    void	 intersectElementForAllGroups(GA_Offset dest,
				GA_Offset src) const;

    /// @{
    /// @private
    /// Remove elements from each group's ordered list, if any, in preparation
    /// for deleting those elements.  This must occur in advance of the actual
    /// deletion and consequent removal from the group as a GA_RangeMemberQuery
    /// object may be using an element group internally.  The GA_Range version
    /// may allocate a GA_RangeMemberQuery object if not provided one, in which
    /// case the caller is responsible for deleting it.
    ///
    /// @note This will temporarily leave ordered groups in an invalid state
    /// until the element removal is processed.
    void	 removeElementFromGroupOrders(GA_Offset element) const;
    void	 removeElementsFromGroupOrders(const GA_Range &range) const;
    void	 removeElementsFromGroupOrders(
				const GA_Range &range,
				const GA_RangeMemberQuery *&range_query) const;
    /// @}
 
    /// @{
    /// @private
    /// Remove secondary elements from mixed groups, if any, in preparation
    /// for deleting those elements.  The GA_Range version may allocate a
    /// GA_RangeMemberQuery object if not provided one, in which case the
    /// caller is responsible for deleting it.  The final offset in the
    /// GA_SecondaryLookupInfo argument in the GA_Range versions is a place
    /// holder for the offsets in the provided GA_Range and is ignored.
    void	 removeMixElementFromGroups(GA_Offset element,
				    const GA_SecondaryLookupInfo *sec) const;
    void	 removeMixElementsFromGroups(
				    GA_Offset element,
				    const GA_SecondaryLookupInfo *sec_prefix,
				    const GA_Range &sec_range) const;
    void	 removeMixElementsFromGroups(
			    GA_Offset element,
			    const GA_SecondaryLookupInfo *sec_prefix,
			    const GA_Range &sec_range,
			    const GA_RangeMemberQuery *&sec_range_query) const;
    /// @}

    GA_Size	 destroyEmptyGroups();

    /// If you have changed group membership in a multithreaded algorithm,
    /// call this method to invalidate cached group entries for all groups
    /// in this table.
    void	 invalidateGroupEntries();

    SYS_FORCE_INLINE
    GA_ElementGroup *find(const UT_StringRef &name) const
    {
        return UTverify_cast<GA_ElementGroup *>(GA_GroupTable::find(name));
    }

protected:
    void	 baseMergeTable(const GA_ElementGroupTable &src,
				GA_MergeMap &map,
				GA_AttributeOwner owner);

    virtual GA_Group	*jsonCreateGroup(UT_JSONParser &p, const char *type,
				const char *name,
				bool internal,
				GA_AttributeOwner owner);

    virtual GA_AttributeOwner	getOwner() const = 0;

private:
    bool	 loadGroup(UT_JSONParser &p, const GA_LoadMap &map,
			    const char *name, const char *type, bool ordered);

protected:
    /// A template class, the specializations of which wrap a typedef to the
    /// the corresponding subclass of GA_ElementGroup.  Effectively mapping
    /// GA_AttributeOwner to a GA_ElementGroup subclass.  The template class
    /// is protected so that external specializations cannot be added.
    ///
    /// The DUMMY parameter is necessary because explicit specialization is
    /// allowed at this scope, while, strangely, the same does not hold for
    /// partial specialization.
    template <GA_AttributeOwner OWNER, bool DUMMY>
	class GA_API ElementType {};

    template <bool DUMMY>	// GA_ATTRIB_POINT -> GA_PointGroup
    class GA_API ElementType<GA_ATTRIB_POINT, DUMMY>
    {
    public:
	typedef GA_PointGroup	Class;
    };

    template <bool DUMMY>	// GA_ATTRIB_PRIMITIVE -> GA_PrimitiveGroup
    class GA_API ElementType<GA_ATTRIB_PRIMITIVE, DUMMY>
    {
    public:
	typedef GA_PrimitiveGroup	Class;
    };

    template <bool DUMMY>	// GA_ATTRIB_VERTEX -> GA_VertexGroup
    class GA_API ElementType<GA_ATTRIB_VERTEX, DUMMY>
    {
    public:
	typedef GA_VertexGroup	Class;
    };
};


/// OWNER must be one of GA_ATTRIB_POINT, GA_ATTRIB_PRIMITIVE, GA_ATTRIB_VERTEX
template <GA_AttributeOwner OWNER>
class GA_ElementGroupTableT : public GA_ElementGroupTable
{
public:
    typedef typename ElementType<OWNER, true>::Class GROUP_TYPE;
    typedef GA_GroupTable::iterator<GROUP_TYPE> iterator;

    explicit GA_ElementGroupTableT(GA_Detail &detail)
	: GA_ElementGroupTable(detail, OWNER) {}

    virtual	~GA_ElementGroupTableT() {}

    void	 getList(UT_Array<const GROUP_TYPE *> &list) const
					{ getListT<const GROUP_TYPE>(list); }
    void	 getList(UT_Array<GROUP_TYPE *> &list) const
					{ getListT<GROUP_TYPE>(list); }

    /// A table merge matches our groups with source, adding any missing, and
    /// updating map.  This method acts as GA_AIFMerge::destroyDestination()
    /// and GA_AIFMerge::addDestination() for element groups.  The merging of
    /// individual element groups is handled by the attributes (GA_AIFMerge).
    void	 mergeTable(const GA_ElementGroupTableT<OWNER> &src,
			    GA_MergeMap &map)
					{ baseMergeTable(src, map, OWNER); }

    GA_GroupTable::iterator<GROUP_TYPE> beginTraverse() const
			{ return begin<GROUP_TYPE>(); }
    GA_GroupTable::iterator<GROUP_TYPE> endTraverse() const
			{ return end<GROUP_TYPE>(); }

    static int	compareAlpha(GROUP_TYPE *const*a, GROUP_TYPE *const*b)
		    { return GA_GroupTable::compareGroupName(**a, **b); }

    typedef UT_OrderedIterator<GROUP_TYPE *,
		    GA_GroupTable::iterator<GROUP_TYPE> > ordered_iterator;
    ordered_iterator	obegin() const
	    { return ordered_iterator(beginTraverse(), compareAlpha); }
    ordered_iterator	oend() const
	    { return ordered_iterator(); }

    static const GROUP_TYPE	*castGroup(const GA_ElementGroup *group)
				    { return (const GROUP_TYPE *)group; }
    static       GROUP_TYPE	*castGroup(GA_ElementGroup *group)
				    { return (GROUP_TYPE *)group; }
protected:
    virtual GA_AttributeOwner getOwner() const { return OWNER; }

    virtual GA_Group *virtualCreateGroup(
        GA_Detail &detail,
        const UT_StringHolder &name,
        bool internal) const
    {
        return new GROUP_TYPE(detail, name, internal, false);
    }

    virtual GA_Group *virtualCreateDetachedGroup(
        const GA_Detail &detail) const
    {
        return new GROUP_TYPE(detail);
    }
};

typedef GA_ElementGroupTableT<GA_ATTRIB_POINT> GA_PointGroupTable;
typedef GA_ElementGroupTableT<GA_ATTRIB_PRIMITIVE> GA_PrimitiveGroupTable;
typedef GA_ElementGroupTableT<GA_ATTRIB_VERTEX> GA_VertexGroupTable;

#endif
