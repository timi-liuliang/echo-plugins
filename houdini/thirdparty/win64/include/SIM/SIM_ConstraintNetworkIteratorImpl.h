/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 */

#ifndef __SIM_ConstraintNetworkIteratorImpl_h__
#define __SIM_ConstraintNetworkIteratorImpl_h__

#include <GA/GA_Primitive.h>
#include <GU/GU_Detail.h>

namespace SIM_ConstraintNetwork
{
inline void
const_iterator::skipInvalid()
{
    while (!myPrimIterator.atEnd())
    {
        myPrimOffset = *myPrimIterator;

        myConstraintDataIndex = myConstraintNameRef.getIndex(myPrimOffset);

        // Ignore invalid primitives and broken constraints. There also
        // must be subdata attached in order to get any attribute values.
        if ((myConstraintDataIndex >= 0
             && myValidConstraintData.getBitFast(myConstraintDataIndex))
            && !(myBrokenGroup && myBrokenGroup->contains(myPrimOffset)))
        {
            GA_OffsetListRef vertices = myGdp->getPrimitiveVertexList(
                myPrimOffset);
            if (vertices.size() == 2)
            {
                myPtAOffset = myGdp->vertexPoint(vertices[0]);
                myPtBOffset = myGdp->vertexPoint(vertices[1]);
                break;
            }
        }

        myPrimIterator.advance();
    }
}

inline void
const_iterator::rewind()
{
    myPrimIterator.rewind();
    myConstraintIndex = 0;

    if (myGdp)
        skipInvalid();
}

inline bool
const_iterator::atEnd() const
{
    return !myGdp || myPrimIterator.atEnd();
}

inline void
const_iterator::advance()
{
    myPrimIterator.advance();
    ++myConstraintIndex;

    skipInvalid();
}

inline GA_Index
const_iterator::getPrimIndex() const
{
    return myGdp->primitiveIndex(myPrimOffset);
}

inline ConstraintType
const_iterator::getConstraintType() const
{
    // If the 'constraint_type' attribute is missing, default to constraining
    // position.
    if (!myConstraintTypeRef.isValid())
        return ConstraintType::Position;

    const GA_StringIndexType idx = myConstraintTypeRef.getIndex(myPrimOffset);
    if (idx == GA_INVALID_STRING_INDEX)
        return ConstraintType::Position;

    UT_ASSERT_P(idx < myConstraintTypes.entries());
    return myConstraintTypes[idx];
}

SYS_FORCE_INLINE GA_Offset
const_iterator::getPointOffset(bool anchor1) const
{
    return anchor1 ? myPtBOffset : myPtAOffset;
}

inline GA_Index
const_iterator::getPointNumber(bool anchor1) const
{
    return myGdp->pointIndex(getPointOffset(anchor1));
}

inline exint
const_iterator::getConstraintDataIndex() const
{
    // This function should only be called when the iterator is on a valid
    // constraint primitive.
    UT_ASSERT_P(myConstraintDataIndex >= 0);
    return myConstraintDataIndex;
}

inline void
GenericConstraintAccessor::advance(exint constraint_idx, GA_Offset primoff)
{
    myConstraintDataIndex = constraint_idx;
    myPrimOffset = primoff;
}

inline const SIM_Data *
GenericConstraintAccessor::getConstraintData() const
{
    return myConstraintData[myConstraintDataIndex];
}

template <typename T>
T
GenericConstraintAccessor::get(const UT_StringRef &attribute_name) const
{
    T value;
    get(attribute_name, *myGdp, myCachedAttributes, value);
    return value;
}

template <typename T>
T
GenericConstraintAccessor::getDataOption(
    const UT_StringRef &attribute_name) const
{
    const SIM_OptionsUser *options =
        myConstraintDataOptions[myConstraintDataIndex];

    T value;
    getOption(options, attribute_name, value);
    return value;
}

template <typename T>
bool
GenericConstraintAccessor::getPrimitiveAttribute(
    const UT_StringRef &attribute_name, T &value) const
{
    return getPrimitiveAttribute(
        attribute_name, *myGdp, myCachedAttributes, value);
}

template <typename T>
bool
GenericConstraintAccessor::getPrimitiveAttribute(
    const UT_StringRef &attribute_name, const GU_Detail &gdp,
    AttribCache &cache, T &value) const
{
    // First, look for a cached attribute reference.
    auto it = cache.find(attribute_name);
    if (it != cache.end())
    {
        if (it->second)
        {
            GA_ROHandleT<T> h(it->second);
            if (h.isValid())
            {
                value = h.get(myPrimOffset);
                return true;
            }
        }

        return false;
    }
    else
    {
        // Otherwise, look up the attribute. Even if it's invalid, we still
        // want to cache that result so that we don't try to look for it again.
        const GA_Attribute *attrib = gdp.findPrimitiveAttribute(attribute_name);
        cache.emplace(attribute_name, attrib);

        if (attrib)
        {
            GA_ROHandleT<T> h(attrib);
            if (h.isValid())
            {
                value = h.get(myPrimOffset);
                return true;
            }
        }

        return false;
    }
}

template <typename T>
void
GenericConstraintAccessor::get(
    const UT_StringRef &attribute_name, const GU_Detail &gdp,
    AttribCache &cache, T &value) const
{
    // If the primitive attribute doesn't exist, look for the attribute on the
    // SIM_ConRel subdata.
    if (!getPrimitiveAttribute(attribute_name, gdp, cache, value))
        value = getDataOption<T>(attribute_name);
}

inline void
GenericConstraintAccessor::get(
    const UT_StringRef &attribute_name, const GU_Detail &gdp,
    AttribCache &cache, fpreal &value) const
{
    value = getDataOption<fpreal>(attribute_name);

    fpreal attrib_value = 0;
    if (getPrimitiveAttribute(attribute_name, gdp, cache, attrib_value))
        value *= attrib_value;
}

inline GA_Index
GenericAnchorAccessor::getAnchorPointIndex(bool anchor1) const
{
    const CachedObjectData *data = getCachedData(anchor1);
    GA_Offset off = getAnchorPointOffset(anchor1, data);

    return off == GA_INVALID_OFFSET ? GA_INVALID_INDEX :
                                      data->getGdp()->pointIndex(off);
}

inline int
GenericAnchorAccessor::getAnchorNumConDOFs(bool anchor1) const
{
    return myNumDOFsRef.isValid() ? myNumDOFsRef.get(getPointOffset(anchor1)) :
                                    3;
}

inline UT_Vector3
GenericAnchorAccessor::getAnchorDOFVector(bool anchor1) const
{
    return myDOFVectorRef.isValid() ?
               myDOFVectorRef.get(getPointOffset(anchor1)) :
               UT_Vector3(0, 1, 0);
}

inline const UT_StringHolder &
GenericAnchorAccessor::getAnchorName(bool anchor1) const
{
    return myNameRef.isValid() ? myNameRef.get(getPointOffset(anchor1)) :
                                 UT_StringHolder::theEmptyString;
}

inline int
GenericAnchorAccessor::getAnchorId(bool anchor1) const
{
    return myAnchorIdRef.isValid() ?
               GA_Index(myAnchorIdRef.get(getPointOffset(anchor1))) :
               GA_INVALID_INDEX;
}

inline UT_Quaternion
GenericAnchorAccessor::getInitialAnchorOrientation(bool anchor1) const
{
    if (myOrientRef.isValid())
        return myOrientRef.get(getPointOffset(anchor1));
    else if (myRotationRef.isValid())
    {
        UT_Vector3 rotation = myRotationRef.get(getPointOffset(anchor1));
        rotation.degToRad();
        UT_XformOrder order;
        return UT_Quaternion(rotation, order);
    }
    else
        return UT_Quaternion(0, 0, 0, 1);
}

// The type of position depends on whwther the 'name' and 'point_id' point
// attributes are defined on the constraint network geometry.
// If the 'name' and 'point_id' together refer to a valid point on a simulation
// object, then POSITION_GEOMETRYPOINT is returned.
// If the 'name' refers to a valid object, and 'point_id' wither doesnt exist,
// or is -1, then POSITION_RELATIVEOFFSET is returned.
// Otherwise POSITION_WORLDSPACE is returned.
inline PositionType
GenericAnchorAccessor::getAnchorPositionType(bool anchor1) const
{
    PositionType type = PositionType::WorldSpace;

    // If there is no name specified, we just have a world space position.
    // Otherwise, check whether this is a relative position or an actual point
    // on the geometry.
    const CachedObjectData *data = getCachedData(anchor1);
    if (data)
    {
        // Look up point offset of pointid on named object
        const GA_Offset ptoff = getAnchorPointOffset(anchor1, data);
        const AnchorType anchor_type = getAnchorType(anchor1);

        if (anchor_type == AnchorType::Agent &&
            (myLocalPosRef.isValid() || myLocalOrientRef.isValid()))
        {
            type = PositionType::AgentXform;
        }
        else if (ptoff != GA_INVALID_OFFSET)
            type = PositionType::GeometryPoint;
        else if (getAnchorId(anchor1) == -1)
            type = PositionType::RelativeOffset;
    }

    return type;
}

inline GA_StringIndexType
GenericAnchorAccessor::getAnchorNameIndex(bool anchor1) const
{
    return myNameRef.isValid() ? myNameRef.getIndex(getPointOffset(anchor1)) :
                                 GA_INVALID_STRING_INDEX;
}

inline const GenericAnchorAccessor::CachedObjectData *
GenericAnchorAccessor::getCachedData(bool anchor1) const
{
    const CachedObjectData *data = nullptr;
    GA_StringIndexType str_idx = getAnchorNameIndex(anchor1);
    if (str_idx >= 0 && str_idx < myCachedData.entries())
        data = myCachedData(str_idx).get();

    return data;
}

}

#endif
