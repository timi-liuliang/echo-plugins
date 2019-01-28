/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        GA_ElementGroup.h (GA Library, C++)
 *
 * COMMENTS:    GA_ElementGroup is actually defined in GA_ATIGroupBool.h,
 *              now that GA_ElementGroup and GA_ATIGroupBool are the same class.
 *              This contains subclasses for different owners.
 */

#pragma once

#ifndef __GA_ElementGroup__
#define __GA_ElementGroup__

#include "GA_API.h"

#include "GA_ATIGroupBool.h"
#include "GA_Types.h"

class GA_Detail;
class GA_Primitive;
class GA_RangeMemberQuery;
class GA_SecondaryLookupInfo;

template <GA_AttributeOwner OWNER>
class GA_ElementGroupTableT;

class GA_API GA_PointGroup final : public GA_ElementGroup
{
protected:
    GA_PointGroup(
            GA_Detail &detail, const UT_StringHolder &name, bool internal, bool detached)
        : GA_ElementGroup(detail, GA_ATTRIB_POINT, name, internal, detached)
    {}
public:
    /// Makes a detached point group
    GA_PointGroup(const GA_Detail &detail)
        : GA_ElementGroup(detail, GA_ATTRIB_POINT)
    {}
    GA_PointGroup &operator|=(const GA_PointGroup &g)
    {
        return static_cast<GA_PointGroup &>(GA_ElementGroup::operator|=(g));
    }
    GA_PointGroup &operator&=(const GA_PointGroup &g)
    {
        return static_cast<GA_PointGroup &>(GA_ElementGroup::operator&=(g));
    }
    GA_PointGroup &operator-=(const GA_PointGroup &g)
    {
        return static_cast<GA_PointGroup &>(GA_ElementGroup::operator-=(g));
    }
    GA_PointGroup &operator^=(const GA_PointGroup &g)
    {
        return static_cast<GA_PointGroup &>(GA_ElementGroup::operator^=(g));
    }
    GA_PointGroup &operator =(const GA_PointGroup &g)
    {
        return static_cast<GA_PointGroup &>(GA_ElementGroup::operator=(g));
    }

    friend class GA_GroupTable;
    friend class GA_ElementGroupTable;
    friend class GA_ElementGroupTableT<GA_ATTRIB_POINT>;
};

class GA_API GA_PrimitiveGroup final : public GA_ElementGroup
{
protected:
    GA_PrimitiveGroup(
            GA_Detail &detail, const UT_StringHolder &name, bool internal, bool detached)
        : GA_ElementGroup(detail, GA_ATTRIB_PRIMITIVE, name, internal, detached)
    {}
public:
    /// Makes a detached primitive group
    GA_PrimitiveGroup(const GA_Detail &detail)
        : GA_ElementGroup(detail, GA_ATTRIB_PRIMITIVE)
    {}
    GA_PrimitiveGroup &operator|=(const GA_PrimitiveGroup &g)
    {
        return static_cast<GA_PrimitiveGroup &>(GA_ElementGroup::operator|=(g));
    }
    GA_PrimitiveGroup &operator&=(const GA_PrimitiveGroup &g)
    {
        return static_cast<GA_PrimitiveGroup &>(GA_ElementGroup::operator&=(g));
    }
    GA_PrimitiveGroup &operator-=(const GA_PrimitiveGroup &g)
    {
        return static_cast<GA_PrimitiveGroup &>(GA_ElementGroup::operator-=(g));
    }
    GA_PrimitiveGroup &operator^=(const GA_PrimitiveGroup &g)
    {
        return static_cast<GA_PrimitiveGroup &>(GA_ElementGroup::operator^=(g));
    }
    GA_PrimitiveGroup &operator =(const GA_PrimitiveGroup &g)
    {
        return static_cast<GA_PrimitiveGroup &>(GA_ElementGroup::operator=(g));
    }

    /// @{
    /// Access elements by a pointer to their GA_Primitive
    /// NOTE: When using add, toggle, remove in parallel,
    ///       you *must* call invalidateGroupEntries() afterward, else
    ///       entries() may return an incorrect value.
    /// NOTE: These cannot be used in parallel on an ordered group.
    void add(const GA_Primitive *prim);
    void toggle(const GA_Primitive *prim);
    void remove(const GA_Primitive *prim);
    /// @}

    /// @{
    /// Access elements by a reference to their GA_Primitive
    /// NOTE: When using add, toggle, remove in parallel,
    ///       you *must* call invalidateGroupEntries() afterward, else
    ///       entries() may return an incorrect value.
    /// NOTE: These cannot be used in parallel on an ordered group.
    void add(const GA_Primitive &prim)      { add(&prim); }
    void toggle(const GA_Primitive &prim)   { toggle(&prim); }
    void remove(const GA_Primitive &prim)   { remove(&prim); }
    /// @}

    /// @{
    /// Test membership
    using GA_ElementGroup::contains;
    bool contains(const GA_Primitive *prim) const;
    bool contains(const GA_Primitive &prim) const
                    { return contains(&prim); }
    /// @}

    /// Functions for "mixed" entries in primitive groups, i.e. references
    /// to trim curves or profile curves on NURBS or Bezier surface primitives.
    /// @{
    void addMixOffset(GA_Offset primary_prim, const GA_SecondaryLookupInfo *sec);
    void removeMixOffset(GA_Offset primary_prim, const GA_SecondaryLookupInfo *sec);
    bool containsMixOffset(GA_Offset primary_prim, const GA_SecondaryLookupInfo *sec) const;

    void addMix(const GA_Primitive *primary_prim, const GA_SecondaryLookupInfo *sec);
    void removeMix(const GA_Primitive *primary_prim, const GA_SecondaryLookupInfo *sec);
    bool containsMix(const GA_Primitive *element, const GA_SecondaryLookupInfo *sec) const;

    void addMix(const GA_Primitive &primary_prim, const GA_SecondaryLookupInfo *sec)
                    { addMix(&primary_prim, sec); }
    void removeMix(const GA_Primitive &primary_prim, const GA_SecondaryLookupInfo *sec)
                    { removeMix(&primary_prim, sec); }
    bool containsMix(const GA_Primitive &element, const GA_SecondaryLookupInfo *sec) const
                    { return containsMix(&element, sec); }

    void removeAllMix(GA_Offset element,
                      const GA_SecondaryLookupInfo *sec_prefix,
                      const GA_RangeMemberQuery &sec_range_query);
    /// @}

    friend class GA_GroupTable;
    friend class GA_ElementGroupTable;
    friend class GA_ElementGroupTableT<GA_ATTRIB_PRIMITIVE>;
};

class GA_API GA_VertexGroup final : public GA_ElementGroup
{
protected:
    GA_VertexGroup(
            GA_Detail &detail, const UT_StringHolder &name, bool internal, bool detached)
        : GA_ElementGroup(detail, GA_ATTRIB_VERTEX, name, internal, detached)
    {}
public:
    /// Makes a detached vertex group
    GA_VertexGroup(const GA_Detail &detail)
        : GA_ElementGroup(detail, GA_ATTRIB_VERTEX)
    {}
    GA_VertexGroup &operator|=(const GA_VertexGroup &g)
    {
        return static_cast<GA_VertexGroup &>(GA_ElementGroup::operator|=(g));
    }
    GA_VertexGroup &operator&=(const GA_VertexGroup &g)
    {
        return static_cast<GA_VertexGroup &>(GA_ElementGroup::operator&=(g));
    }
    GA_VertexGroup &operator-=(const GA_VertexGroup &g)
    {
        return static_cast<GA_VertexGroup &>(GA_ElementGroup::operator-=(g));
    }
    GA_VertexGroup &operator^=(const GA_VertexGroup &g)
    {
        return static_cast<GA_VertexGroup &>(GA_ElementGroup::operator^=(g));
    }
    GA_VertexGroup &operator =(const GA_VertexGroup &g)
    {
        return static_cast<GA_VertexGroup &>(GA_ElementGroup::operator=(g));
    }
    friend class GA_GroupTable;
    friend class GA_ElementGroupTable;
    friend class GA_ElementGroupTableT<GA_ATTRIB_VERTEX>;
};

#endif
