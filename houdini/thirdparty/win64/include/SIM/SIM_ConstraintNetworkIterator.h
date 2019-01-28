/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 */

#ifndef __SIM_ConstraintNetworkIterator_h__
#define __SIM_ConstraintNetworkIterator_h__

#include "SIM_API.h"

#include "SIM_DataFilter.h"
#include "SIM_OptionsUser.h"

#include <GA/GA_AttributeRef.h>
#include <GA/GA_Handle.h>
#include <GU/GU_DetailHandle.h>
#include <UT/UT_NonCopyable.h>
#include <UT/UT_StringMap.h>
#include <UT/UT_VectorTypes.h>

class GA_Primitive;
class GA_PrimitiveGroup;
class GU_Detail;
class SIM_BaseObjectReader;
class SIM_Data;
class SIM_Geometry;
class SIM_GeometryAutoWriteLock;
class SIM_GeometryCopy;
class SIM_Object;
class SIM_Time;

namespace SIM_ConstraintNetwork
{
    /// The type of a constraint (linear, angular, or both).
    enum class ConstraintType
    {
        Position,
        Rotation,
        All
    };

    /// The type of anchor specfied by 'anchor_type' attribute. Defaults to
    /// point.
    enum class AnchorType
    {
        Point,
        Vertex,
        Agent
    };

    /// How the position of the anchor is interpreted.
    enum class PositionType
    {
        WorldSpace,
        RelativeOffset,
        GeometryPoint,
        AgentXform
    };

    /// Creates the 'force' and 'distance' primitive attributes.
    SIM_API void addForceAttributes(GU_Detail &gdp, GA_RWHandleF &force,
                                    GA_RWHandleF &distance);
    /// Creates the 'torque' and 'angle' primitive attributes.
    SIM_API void addTorqueAttributes(GU_Detail &gdp, GA_RWHandleF &torque,
                                     GA_RWHandleF &angle);

    /// Implements filtering of the constraint types that are recognized by the
    /// caller, as well as caching / reading those constraint's parameter
    /// values.
    /// See @ref GenericConstraintAccessor for the default implementation.
    class SIM_API ConstraintAccessor
    {
    public:
        virtual ~ConstraintAccessor() {}

        /// Initialize with the constraint network's GU_Detail, and the number
        /// of constraint types referenced by the geometry (from the
        /// 'constraint_name' primitive attribute).
        virtual void init(const GU_Detail *gdp, exint num_constraint_types) = 0;

        /// Returns whether the constraint type should be included when
        /// iterating over the constraint network, and performs any necessary
        /// caching with the constraint data and its index.
        virtual bool acceptConstraint(const SIM_Data &constraint_data,
                                      exint idx) = 0;
    };

    /// Implements the mapping of anchors to objects recognized by the solver,
    /// as well as evaluating anchor attributes (position, orientation, etc).
    /// See @ref GenericAnchorAccessor for the default implementation.
    class SIM_API AnchorAccessor
    {
    public:
        virtual ~AnchorAccessor() {}

        /// Initialize with the constraint network's GU_Detail and transform.
        virtual bool init(
            const GU_Detail &gdp, const UT_Matrix4D &xform,
            const SIM_Data &container, const SIM_Data &data_root,
            const SIM_Time &t) = 0;
    };

    class SIM_API const_iterator : UT_NonCopyable
    {
    public:
        /// @param container A SIM_Relationship or SIM_Object that contains the
        /// constraint network as subdata.
        /// @param data_root Parent of the SIM_ConRel subdata.
        const_iterator(const SIM_Data &container, const SIM_Data &data_root,
                       const SIM_Geometry *geo, const SIM_Time &t,
                       ConstraintAccessor &constraint_accessor,
                       AnchorAccessor &anchor_accessor);

        const_iterator(const SIM_Data &container, const SIM_Data &data_root,
                       const SIM_Geometry *geo, const GU_Detail *gdp,
                       const GA_PrimitiveGroup *broken_group, const SIM_Time &t,
                       ConstraintAccessor &constraint_accessor,
                       AnchorAccessor &anchor_accessor);

        void rewind();
        bool atEnd() const;
        void advance();

        /// The constraint network geometry.
        const GU_Detail &getGdp() const { return *myGdp; }
        /// The number of valid constraints that have been seen so far.
        exint getConstraintIndex() const { return myConstraintIndex; }
        /// The primitive index.
        GA_Index getPrimIndex() const;
        /// The primitive offset.
        GA_Offset getPrimOffset() const { return myPrimOffset; }

        /// Returns the index of the current constraint data type.
        exint getConstraintDataIndex() const;

        /// Returns whether the constraint affects position / rotation / all.
        ConstraintType getConstraintType() const;

        /// The point offset for this anchor in the constraint network geometry.
        GA_Offset getPointOffset(bool anchor1) const;

        /// The point number for this anchor in the constraint network geometry.
        GA_Index getPointNumber(bool anchor1) const;

    private:
        void init(
            const SIM_Data &container, const SIM_Geometry &geo,
            const GU_Detail &gdp, const SIM_Time &t,
            ConstraintAccessor &constraint_accessor,
            AnchorAccessor &anchor_accessor);

        void skipInvalid();

        /// Returns false if nothing matched the filter.
        bool buildConstraintDataCache(
            const GU_Detail &gdp, ConstraintAccessor &constraint_accessor);
        void buildConstraintTypeCache();

        const SIM_Data &myRootData;

        UT_UniquePtr<GU_DetailHandleAutoReadLock> myLock;
        const GU_Detail *myGdp;

        GA_Offset myPrimOffset;
        exint myConstraintDataIndex;
        GA_Iterator myPrimIterator;

        /// Cached offsets for the primitive's two points.
        GA_Offset myPtAOffset;
        GA_Offset myPtBOffset;

        /// Track how many constraints we've seen so far.
        exint myConstraintIndex;

        /// Primitive group containing constraints that have been broken.
        /// These are ignored by the iterator.
        const GA_PrimitiveGroup *myBrokenGroup;

        /// Map from the 'constraint_type' attribute's string indices to the
        /// corresponding ConstraintType enum values.
        UT_Array<ConstraintType> myConstraintTypes;

        /// Track whether each unique value of 'constraint_name' is a valid
        /// constraint data type.
        UT_BitArray myValidConstraintData;

        GA_ROHandleS myConstraintNameRef;
        GA_ROHandleS myConstraintTypeRef;
    };

    /// Provides a convenient way to process all of the constraints in the
    /// network, and update attributes of the constraints.
    class SIM_API iterator : UT_NonCopyable
    {
    public:
        /// Use the filter parameter to process only a certain constraint type.
        /// The default behaviour is to process everything.
        iterator(
            const SIM_Data &container, const SIM_Data &data_root,
            SIM_GeometryCopy *geo, const SIM_Time &t,
            ConstraintAccessor &constraint_accessor,
            AnchorAccessor &anchor_accessor);
        ~iterator();

        void rewind() { return myImpl.rewind(); }
        bool atEnd() const { return myImpl.atEnd(); }
        void advance() { myImpl.advance(); }

        /// Return the number of constraints that have been seen so far.
        exint getConstraintIndex() const { return myImpl.getConstraintIndex(); }

        /// Return the primitive number corresponding to the current constraint.
        GA_Index getPrimIndex() const { return myImpl.getPrimIndex(); }
        GA_Offset getPrimOffset() const { return myImpl.getPrimOffset(); }

        /// Returns the index of the current constraint data type.
        exint getConstraintDataIndex() const { return myImpl.getConstraintDataIndex(); }

        /// Return the point number corresponding to an anchor.
        GA_Index getPointNumber(bool anchor1) const;

        GU_Detail &getGdp() const { return *myWriteableGdp; }

        /// Mark the current constraint as being broken.
        /// This will cause it to switch to the next constraint type (if
        /// defined) or break and be skipped on any further traversals of the
        /// constraints. The broken constraints are placed into a primitive
        /// group, which can then be used in a SOP solver to trigger events
        /// when constraints break.
        void breakConstraint();

        /// Convenience method to update the constraint about the
        /// forces or torques applied to satisfy it.
        void updateLinearConstraintState(fpreal force, fpreal distance);
        void updateAngularConstraintState(fpreal torque, fpreal angle);

        /// Returns the underlying const_iterator
        const_iterator &getConstIterator() { return myImpl; }

    private:
        static GA_PrimitiveGroup *getBrokenGroup(GU_Detail *gdp);

        SIM_GeometryCopy *myGeoCopy;
        UT_UniquePtr<SIM_GeometryAutoWriteLock> myLock;
        GU_Detail *myWriteableGdp;
        GA_PrimitiveGroup *myBrokenGroup;

        GA_RWHandleF myForceAttrib;
        GA_RWHandleF myDistanceAttrib;
        GA_RWHandleF myTorqueAttrib;
        GA_RWHandleF myAngleAttrib;

        GA_RWHandleS myConstraintName;
        GA_RWHandleS myConstraintType;
        GA_RWHandleS myNextConstraintName;
        GA_RWHandleS myNextConstraintType;

        const_iterator myImpl;
    };

    /// Generic implementation of @ref ConstraintAccessor, which evaluates
    /// constraint parameter values by name.
    class SIM_API GenericConstraintAccessor : public ConstraintAccessor
    {
    public:
        /// Default constructor uses SIM_DataFilterAll.
        GenericConstraintAccessor();

        GenericConstraintAccessor(UT_UniquePtr<SIM_DataFilter> filter);

        void init(const GU_Detail *gdp, exint num_constraint_types) override;
        bool acceptConstraint(const SIM_Data &constraint_data,
                              exint idx) override;

        /// Update the current constraint data type index and primitive offset.
        void advance(exint constraint_idx, GA_Offset primoff);

        const SIM_Data *getConstraintData() const;

        /// Returns the value of an attribute of the constraint. If there is no
        /// primitive attribute with that name, a default value will be taken
        /// from the attached subdata if possible.
        template <typename T>
        T get(const UT_StringRef &attribute_name) const;

        /// Retrieve a value from the attached subdata for the constraint.
        template <typename T>
        T getDataOption(const UT_StringRef &attribute_name) const;

        /// Looks up a value for a primitive attribute.
        /// Returns true if the attribute exists, and false otherwise.
        template <typename T>
        bool getPrimitiveAttribute(
            const UT_StringRef &attribute_name, T &value) const;

    private:
        using AttribCache = UT_StringMap<const GA_Attribute *>;

        /// Looks up a value for a primitive attribute.
        /// Returns true if the attribute exists, and false otherwise.
        template <typename T>
        bool getPrimitiveAttribute(
            const UT_StringRef &attribute_name, const GU_Detail &gdp,
            AttribCache &cache, T &value) const;

        /// Look up an attribute value. If the primitive attribute is not
        /// present, the value from the subdata will be used.
        template <typename T>
        void get(const UT_StringRef &attribute_name, const GU_Detail &gdp,
                 AttribCache &cache, T &value) const;

        /// Look up a floating-point attribute value. If present, the primitive
        /// attribute value will be multiplied with the value from the subdata.
        void get(const UT_StringRef &attribute_name, const GU_Detail &gdp,
                 AttribCache &cache, fpreal &value) const;

        /// @{
        /// Helper functions to look up values in a SIM_OptionsUser object.
        static void getOption(
            const SIM_OptionsUser *data, const UT_StringRef &attribute_name,
            int &value)
        {
            value = data ? data->getOptions().getOptionI(attribute_name) : 0;
        }
        static void getOption(
            const SIM_OptionsUser *data, const UT_StringRef &attribute_name,
            fpreal &value)
        {
            value = data ? data->getOptions().getOptionF(attribute_name) : 0;
        }

        static void getOption(
            const SIM_OptionsUser *data, const UT_StringRef &attribute_name,
            UT_Vector2 &value)
        {
            value = data ? data->getOptions().getOptionV2(attribute_name) :
                           UT_Vector2D(0, 0);
        }

        static void getOption(
            const SIM_OptionsUser *data, const UT_StringRef &attribute_name,
            UT_Vector3 &value)
        {
            value = data ? data->getOptions().getOptionV3(attribute_name) :
                           UT_Vector3D(0, 0, 0);
        }

        static void getOption(
            const SIM_OptionsUser *data, const UT_StringRef &attribute_name,
            UT_Quaternion &value)
        {
            value = data ? data->getOptions().getOptionQ(attribute_name) :
                           UT_QuaternionD(0, 0, 0, 1);
        }
        /// @}

        const GU_Detail *myGdp;
        UT_Array<const SIM_Data *> myConstraintData;
        UT_Array<const SIM_OptionsUser *> myConstraintDataOptions;

        exint myConstraintDataIndex;
        GA_Offset myPrimOffset;

        /// Cache references to attributes.
        mutable AttribCache myCachedAttributes;

        UT_UniquePtr<SIM_DataFilter> myFilter;
    };

    /// Generic implementation of @ref AnchorAccessor, which builds its own
    /// cache of the objects (transforms, etc) referenced by the anchor points.
    class SIM_API GenericAnchorAccessor : public AnchorAccessor, UT_NonCopyable
    {
    public:
        GenericAnchorAccessor();

        bool init(
            const GU_Detail &gdp, const UT_Matrix4D &xform,
            const SIM_Data &container, const SIM_Data &data_root,
            const SIM_Time &t) override;

        void advance(GA_Offset pt_a, GA_Offset pt_b)
        {
            myPtAOffset = pt_a;
            myPtBOffset = pt_b;
        }

        /// Get the point offset of the point referred to by the anchor.
        /// Either the anchor directly references a point on an object,
        /// or it references a vertex on an object which has an associated
        /// point. Returns GA_INVALID_OFFSET in the case where the anchor doesnt
        /// reference a point. If 'anchor_pid' or 'anchor_vid' is present, then
        /// return the first point (or vertex) with same id as on the
        /// constraint.
        GA_Offset getAnchorPointOffset(bool anchor1) const;

        /// Gets the point index of the point referred to by the anchor in a
        /// similar way to getAnchorPointOffset.
        GA_Index getAnchorPointIndex(bool anchor1) const;

        UT_Vector3 getAnchorPosition(bool anchor1) const;
        UT_Vector3 getAnchorVelocity(bool anchor1) const;
        /// Returns the type of position described by the anchor (e.g. relative
        /// offset).
        PositionType getAnchorPositionType(bool anchor1) const;

        UT_Quaternion getAnchorOrientation(bool anchor1) const;
        UT_Vector3 getAnchorAngularVelocity(bool anchor1) const;

        bool getObjectCentroid(bool anchor1, UT_Vector3 &pos) const;

        int getAnchorNumConDOFs(bool anchor1) const;
        UT_Vector3 getAnchorDOFVector(bool anchor1) const;

        const UT_StringHolder &getAnchorName(bool anchor1) const;

        /// Returns the id of the object named by the anchor.
        /// Returns -1 of the anchor does not name an object.
        int getObjectId(bool anchor1) const;
        const SIM_Object *getObject(bool anchor1) const;

    private:
        class CachedObjectData
        {
        public:
            CachedObjectData(const SIM_BaseObjectReader &reader,
                             bool need_object_geo,
                             const UT_Matrix4D *agent_xform);
            ~CachedObjectData() {}

            const GU_Detail *getGdp() const
            {
                if (myGeoReadLock)
                {
                    const GU_Detail *gdp = myGeoReadLock->getGdp();
                    UT_ASSERT(gdp);
                    return gdp;
                }
                else
                    return 0;
            }

            const SIM_Object &myObject;
            UT_UniquePtr<GU_DetailHandleAutoReadLock> myGeoReadLock;

            UT_DMatrix4 myPosXform;
            UT_DMatrix4 myGeoXform;
            UT_Matrix4D myAgentXform;
            UT_Quaternion myPosRotation;
            UT_Quaternion myGeoRotation;
            UT_Quaternion myAgentRotation;
            const UT_Vector3 myCentroid;

            GA_ROHandleQ myOrientRef;
            GA_ROHandleV3 myVelocityRef;
            GA_ROHandleV3 myWRef;

            GA_ROHandleI myAnchorPIDRef;
            GA_ROHandleI myAnchorVIDRef;
        };

        /// Get the point offset of the point referenced by the anchor.
        /// Either the anchor directly references a point on an object,
        /// or it references a vertex on an object which has an associated
        /// point. Returns GA_INVALID_OFFSET in the case where the anchor doesnt
        /// reference a point
        GA_Offset
        getAnchorPointOffset(bool anchor1, const CachedObjectData *data) const;

        /// Returns the cached data for an anchor by looking up the name
        const CachedObjectData *getCachedData(bool anchor1) const;

        /// Get the value in the anchor's anchor_id attribute
        /// (-1 if no such attribute exists)
        int getAnchorId(bool anchor1) const;

        /// Get the type of the Anchor (Point or Vertex)
        AnchorType getAnchorType(bool anchor1) const;

        UT_Quaternion getInitialAnchorOrientation(bool anchor1) const;

        GA_StringIndexType getAnchorNameIndex(bool anchor1) const;

        SYS_FORCE_INLINE GA_Offset getPointOffset(bool anchor1) const
        {
            return anchor1 ? myPtBOffset : myPtAOffset;
        }

        const GU_Detail *myGdp;

        GA_Offset myPtAOffset;
        GA_Offset myPtBOffset;

        /// Mapping from object names to cached data necessary for certain
        /// functions
        UT_Array<UT_UniquePtr<CachedObjectData>> myCachedData;

        UT_DMatrix4 myXform;
        UT_Quaternion myOrient;
        const SIM_Data *myRootData;

        GA_ROHandleS myNameRef;
        GA_ROHandleI myNumDOFsRef;
        GA_ROHandleV3 myDOFVectorRef;
        GA_ROHandleV3 myLocalPosRef;
        GA_ROHandleQ myLocalOrientRef;
        GA_ROHandleI myAnchorIdRef;
        GA_ROHandleS myAnchorTypeRef;
        GA_ROHandleV3 myVelocityRef;
        GA_ROHandleV3 myRotationRef;
        GA_ROHandleQ myOrientRef;
        GA_ROHandleV3 myAngularVelocityRef;
    };

}

#include "SIM_ConstraintNetworkIteratorImpl.h"

#endif
