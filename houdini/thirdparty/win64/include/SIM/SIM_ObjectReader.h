/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_ObjectReader_h__
#define __SIM_ObjectReader_h__

#include "SIM_API.h"

#include <GA/GA_Handle.h>
#include <GU/GU_DetailHandle.h>
#include <SYS/SYS_TypeTraits.h>

class GEO_Primitive;
class GU_Agent;
class GU_PrimPacked;
class SIM_Object;

SIM_API void
SIMcomputePositionTransform(const UT_Vector3 &pos, const UT_Vector3 &pivot,
                            const UT_Quaternion &orient, UT_Matrix4D &xform);

class SIM_API SIM_BaseObjectReader
{
public:
    SIM_BaseObjectReader(const SIM_Object &obj);
    virtual ~SIM_BaseObjectReader() {}

    /// Return the SIM_Object.
    const SIM_Object &getObject() const { return myObject; }

    virtual void getPositionTransform(UT_Matrix4D &xform) const = 0;
    virtual UT_Quaternion getOrientation() const = 0;
    virtual UT_Vector3 getCentroid() const = 0;

    /// Return the geometry of the RBD object (i.e. for a packed primitive, the
    /// unpacked geometry).
    virtual GU_ConstDetailHandle getObjectGdp() const = 0;
    /// Return the SIM_Object's geometry.
    virtual const GU_Detail *getSimGdp() const = 0;

protected:
    const SIM_Object &myObject;
};

class SIM_API SIM_SingleObjectReader : public SIM_BaseObjectReader
{
public:
    SIM_SingleObjectReader(const SIM_Object &obj);

    virtual void getPositionTransform(UT_Matrix4D &xform) const;
    virtual UT_Quaternion getOrientation() const;
    virtual UT_Vector3 getCentroid() const;

    virtual GU_ConstDetailHandle getObjectGdp() const;
    virtual const GU_Detail *getSimGdp() const;

private:
    GU_DetailHandleAutoReadLock myGeometry;
};

class SIM_API SIM_PackedPrimitiveReader : public SIM_BaseObjectReader
{
public:
    SIM_PackedPrimitiveReader(const SIM_Object &obj, const GU_Detail &gdp);

    GA_Offset getPointOffset() const { return myPointOffset; }
    /// Returns the primitive associated with the current point offset.
    const GEO_Primitive *getPrimitive() const { return myPrimitive; }

    /// Returns the unpacked geometry of the packed primitive.
    virtual GU_ConstDetailHandle getObjectGdp() const;
    virtual const GU_Detail *getSimGdp() const;

    virtual void getPositionTransform(UT_Matrix4D &xform) const;
    virtual UT_Vector3 getCentroid() const;
    const UT_StringHolder &getName() const;

    static bool isPackedObject(const SIM_Object *obj);
    /// Returns true if the primitive can represent an object (e.g. a packed
    /// primitive or sphere).
    static bool isValidPrimitive(const GEO_Primitive &prim);

    void setPrimOffset(GA_Offset offset);

    UT_Vector3 getPivot() const;
    UT_Vector3 getPosition() const;
    UT_Quaternion getOrientation() const;
    UT_Vector3 getVelocity() const;
    UT_Vector3 getAngularVelocity() const;

    bool isPivotValid() const;
    bool isPositionValid() const;
    bool isOrientationValid() const;
    bool isVelocityValid() const;
    bool isAngularVelocityValid() const;
    bool isNameValid() const;

private:
    const GU_Detail &myGdp;

    GA_Offset myPointOffset;
    const GEO_Primitive *myPrimitive;

    GA_ROHandleV3 myPivot;
    GA_ROHandleQ myOrientation;
    GA_ROHandleV3 myAngularVelocity;
    GA_ROHandleV3 myVelocity;
    GA_ROHandleS myName;
    GA_ROHandleV3 myRest;
};

class SIM_API SIM_PackedObjectWriter
{
public:
    SIM_PackedObjectWriter(GU_Detail &gdp);

    GA_Offset getPointOffset() const { return myPointOffset; }
    void setPrimOffset(GA_Offset offset);

    void setPivot(const UT_Vector3 &pivot);
    void setOrientation(const UT_Quaternion &orient);
    void setPosition(const UT_Vector3 &pos);
    void setVelocity(const UT_Vector3 &vel);
    void setAngularVelocity(const UT_Vector3 &avel);
    void setName(const char *name);

    void updatePivotIntrinsic(const UT_Vector3 &pivot,
                              const UT_Matrix4D &position_xform);

    GU_Detail &getSimGdp() { return myGdp; }

private:
    GA_Offset myPointOffset;
    GU_PrimPacked *myPrimPacked;
    GU_Detail &myGdp;

    GA_RWHandleV3 myPosW;
    GA_RWHandleV3 myPivotW;
    GA_RWHandleQ myOrientationW;
    GA_RWHandleV3 myAngularVelocityW;
    GA_RWHandleV3 myVelocityW;
    GA_RWHandleS myNameW;
    GA_RWHandleV3 myRestW;
};

class SIM_API SIM_AgentShapeReader : public SIM_BaseObjectReader
{
public:
    template <typename ARRAY_T>
    struct BatchROHandleA
    {
        BatchROHandleA(const GA_Attribute *attrib = nullptr);
        bool isValid() const { return myAttrib.isValid(); }

        GA_ROHandleT<ARRAY_T> myAttrib;
        mutable ARRAY_T myVals;
        mutable GA_Offset myCacheOffset;
        int myTupleSize;
    };

    /// Batches all writes to array elements for the current offset, until a
    /// flush() or its destructor.
    template <typename ARRAY_T>
    struct BatchRWHandleA
    {
        BatchRWHandleA(GA_Attribute *attrib = nullptr);
        ~BatchRWHandleA() { flush(); }

        void flush();
        bool isValid() const { return myAttrib.isValid(); }

        void bumpDataId() { myAttrib.bumpDataId(); }

        void bind(GA_Detail *gdp, GA_AttributeOwner owner,
                  const UT_StringRef &name, int minsize = 1);

        GA_RWHandleT<ARRAY_T> myAttrib;
        mutable ARRAY_T myVals;
        mutable GA_Offset myCacheOffset;
        int myTupleSize;
    };

    SIM_AgentShapeReader(const SIM_Object &obj, const GU_Detail &gdp);

    void setCurrentShape(GA_Offset prim_offset, exint transform_id);

    const GU_PrimPacked *getPrimitive() const { return myPrimitive; }
    const GU_Agent *getAgent() const { return myAgent; }
    const UT_StringHolder &getAgentName() const;
    GA_Offset getPointOffset() const { return myPointOffset; }
    exint getTransformId() const { return myTransformId; }

    /// Computes the two names that can be used to reference agents in a
    /// constraint network - either 'dopobject/agentname/transform' or
    /// 'agentname/transform' can be used. Returns false if the agent does not
    /// have a name.
    bool getAnchorNames(UT_WorkBuffer &full_name, UT_StringRef &name) const;

    bool hasPositionData() const;
    UT_Vector3 getPivot() const;
    UT_Vector3 getPosition() const;
    UT_Quaternion getOrientation() const;
    void getCurrentWorldTransform(UT_Matrix4D &xform) const;

    virtual void getPositionTransform(UT_Matrix4D &xform) const;
    virtual UT_Vector3 getCentroid() const;

    /// Unpack the shape(s) attached to the current transform.
    bool unpackBoundShapes(GU_Detail &gdp) const;
    /// Return the geometry of the RBD object (i.e. the shapes attached to the
    /// current transform).
    virtual GU_ConstDetailHandle getObjectGdp() const;
    /// Return the SIM_Object's geometry.
    virtual const GU_Detail *getSimGdp() const;

    /// Helper function for reading an element from an array attribute at the
    /// given offset.
    template <typename T, typename ARRAY_T>
    static bool
    getArrayAttribElement(const BatchROHandleA<ARRAY_T> &handle,
                          GA_Offset offset, exint i, T &elem);

    /// Helper function for updating an element of an array attribute at the
    /// given offset.
    template <typename T, typename ARRAY_T>
    static void
    setArrayAttribElement(BatchRWHandleA<ARRAY_T> &handle,
                          GA_Offset offset, exint i, exint n, const T &elem);

private:
#define IS_SCALAR SYS_IsArithmetic<T>::value || SYS_IsSame<T, UT_StringHolder>::value
#define ENABLE_IF_SCALAR typename SYS_EnableIf<IS_SCALAR>::type * = 0
#define ENABLE_IF_VECTOR typename SYS_DisableIf<IS_SCALAR>::type * = 0

    template <typename T>
    static int getTupleSize(ENABLE_IF_VECTOR) { return T::tuple_size; }

    template <typename T>
    static int getTupleSize(ENABLE_IF_SCALAR) { return 1; }

    template <typename ATTRIB_T, typename T>
    static void getValue(ATTRIB_T *src, T &dest, ENABLE_IF_VECTOR)
    {
        std::copy(src, src + T::tuple_size, dest.data());
    }

    template <typename ATTRIB_T, typename T>
    static void getValue(ATTRIB_T *src, T &dest, ENABLE_IF_SCALAR)
    {
        dest = *src;
    }

    template <typename T, typename ATTRIB_T>
    static void setValue(const T &src, ATTRIB_T *dest, ENABLE_IF_VECTOR)
    {
        std::copy(src.data(), src.data() + T::tuple_size, dest);
    }

    template <typename T, typename ATTRIB_T>
    static void setValue(T src, ATTRIB_T *dest, ENABLE_IF_SCALAR)
    {
        *dest = src;
    }

#undef IS_SCALAR
#undef ENABLE_IF_SCALAR
#undef ENABLE_IF_VECTOR

    const GU_Detail &myGdp;
    const GU_PrimPacked *myPrimitive;
    const GU_Agent *myAgent;
    GA_Offset myPointOffset;
    exint myTransformId;

    GA_ROHandleS myName;
    BatchROHandleA<UT_Fpreal32Array> myPivot;
    BatchROHandleA<UT_Fpreal32Array> myOrientation;
    BatchROHandleA<UT_Fpreal32Array> myPosition;
};

template <typename ARRAY_T>
SIM_AgentShapeReader::BatchROHandleA<ARRAY_T>::BatchROHandleA(
    const GA_Attribute *attrib)
    : myAttrib(attrib),
      myCacheOffset(GA_INVALID_OFFSET),
      myTupleSize(myAttrib.isValid() ? myAttrib->getTupleSize() : -1)
{
}

template <typename ARRAY_T>
SIM_AgentShapeReader::BatchRWHandleA<ARRAY_T>::BatchRWHandleA(
    GA_Attribute *attrib)
    : myAttrib(attrib),
      myCacheOffset(GA_INVALID_OFFSET),
      myTupleSize(myAttrib.isValid() ? myAttrib->getTupleSize() : -1)
{
}

template <typename ARRAY_T>
void
SIM_AgentShapeReader::BatchRWHandleA<ARRAY_T>::flush()
{
    if (GAisValid(myCacheOffset))
    {
        myAttrib.set(myCacheOffset, myVals);
        myCacheOffset = GA_INVALID_OFFSET;
    }
}

template <typename ARRAY_T>
void
SIM_AgentShapeReader::BatchRWHandleA<ARRAY_T>::bind(GA_Detail *gdp,
                                                    GA_AttributeOwner owner,
                                                    const UT_StringRef &name,
                                                    int minsize)
{
    myAttrib.bind(gdp, owner, name, minsize);
    myTupleSize = myAttrib.isValid() ? myAttrib->getTupleSize() : -1;
}

template <typename T, typename ARRAY_T>
bool
SIM_AgentShapeReader::getArrayAttribElement(
    const BatchROHandleA<ARRAY_T> &handle, GA_Offset offset,
    exint i, T &elem)
{
    UT_ASSERT(handle.isValid());
    if (!handle.isValid())
        return false;

    const int tuple_size = handle.myTupleSize;
    if (handle.myTupleSize != getTupleSize<T>())
    {
        UT_ASSERT_MSG(false, "Incorrect tuple size.");
        return false;
    }

    // Update our cache if we're reading from a different offset.
    if (handle.myCacheOffset != offset)
    {
        handle.myAttrib.get(offset, handle.myVals);
        handle.myCacheOffset = offset;
    }

    if ((i + 1) * tuple_size > handle.myVals.entries())
        return false;

    getValue(handle.myVals.array() + i * tuple_size, elem);

    return true;
}

template <typename T, typename ARRAY_T>
void
SIM_AgentShapeReader::setArrayAttribElement(BatchRWHandleA<ARRAY_T> &handle,
                                            GA_Offset offset, exint i, exint n,
                                            const T &elem)
{
    const int tuple_size = handle.myTupleSize;
    if (tuple_size != getTupleSize<T>())
    {
        UT_ASSERT_MSG(false, "Incorrect tuple size.");
        return;
    }

    // If we're writing to a different offset, write out the array for the
    // previous offset.
    if (handle.myCacheOffset != offset)
    {
        handle.flush();

        handle.myCacheOffset = offset;
        handle.myAttrib.get(offset, handle.myVals);
        handle.myVals.setSize(n * tuple_size);
    }

    setValue(elem, handle.myVals.array() + i * tuple_size);
}

#endif
