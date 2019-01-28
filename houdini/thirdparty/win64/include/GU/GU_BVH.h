/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        GU_BVH.h (GU Library, C++)
 *
 * COMMENTS:    Bounding Volume Hierarchy (BVH) implementation for GEO_Detail.
 */

#pragma once

#include "GU_API.h"
#include <GEO/GEO_BVH.h>
#include <GA/GA_Handle.h>
#include <GA/GA_Types.h>
#include <UT/UT_ArrayMap.h>
#include <UT/UT_UniquePtr.h>
#include <UT/UT_Vector3.h>
#include <VM/VM_SIMD.h>
#include <limits>

class UT_Lock;
class GU_PackedImpl;
class GU_RayPrimInfo;

namespace GU {

template<uint NAXES,typename SUBCLASS>
using BVHBase = GEO::BVHBase<NAXES,SUBCLASS>;

class GU_API BVH : public BVHBase<3, BVH>
{
    using Base = BVHBase<3, BVH>;
    friend Base;
    static constexpr uint NAXES = 3;

    static constexpr bool theHasPrimitives = true;

    enum OtherPrimType : uint
    {
        OTHERPRIM_BVHXLATESCALE,
        OTHERPRIM_BVHUNIFORM,
        OTHERPRIM_BVHFULLXFORM,
        OTHERPRIM_RAYPRIMINFO,
        OTHERPRIM_SEGMENT,
        OTHERPRIM_NGON_TRI,
        OTHERPRIM_NGON_QUAD
    };

    struct TranslateScale
    {
        UT_Vector3D translate;
        double inverse_scale;

        SYS_FORCE_INLINE void transformPosition(UT_Vector3D &position) const
        {
            position = position/inverse_scale + translate;
        }
        SYS_FORCE_INLINE void inverseTransformPosition(UT_Vector3D &position) const
        {
            position = (position - translate)*inverse_scale;
        }
        SYS_FORCE_INLINE void inverseTransformDirection(UT_Vector3D &vector) const
        {
            // Nothing to do, since we'd just normalize out the scale anyway
        }
        SYS_FORCE_INLINE void transformVector(UT_Vector3D &vector) const
        {
            vector /= inverse_scale;
        }
        SYS_FORCE_INLINE float scaleT(float t) const
        {
            return t/inverse_scale;
        }
        SYS_FORCE_INLINE float inverseScaleT(float t) const
        {
            return t*inverse_scale;
        }
    };
    struct UniformTransform
    {
        /// This is scaled by sqrt(inverse_scale), so that if we do
        /// rotation.conjugate()*P*rotation, without normalizing,
        /// it'll automatically apply the inverse scale.
        UT_QuaternionD rotation;
        UT_Vector3D translate;
        double scale;

        SYS_FORCE_INLINE void transformPosition(UT_Vector3D &position) const
        {
            // NOTE: UT_QuaternionT::rotate assumes that rotation is normalized.
            //       We're using the fact that it's not, to our advantage,
            //       to automatically apply the scale.
            position = (rotation*scale).rotate(position) + translate;
        }
        SYS_FORCE_INLINE void inverseTransformPosition(UT_Vector3D &position) const
        {
            // NOTE: UT_QuaternionT::rotateInverse assumes that rotation is normalized.
            //       We're using the fact that it's not, to our advantage,
            //       to automatically apply the inverse scale.
            position = rotation.rotateInverse(position - translate);
        }
        SYS_FORCE_INLINE void inverseTransformDirection(UT_Vector3D &vector) const
        {
            vector = rotation.rotateInverse(vector);
            // NOTE: UT_QuaternionT::rotateInverse assumes that rotation is normalized,
            //       but it's not, so we need to normalize vector.
            vector.normalize();
        }
        SYS_FORCE_INLINE void transformVector(UT_Vector3D &vector) const
        {
            vector = rotation.rotate(vector);
        }
        SYS_FORCE_INLINE float scaleT(float t) const
        {
            return t*scale;
        }
        SYS_FORCE_INLINE float inverseScaleT(float t) const
        {
            return t/scale;
        }
    };

    struct PrimData
    {
        GA_Offset myPrimOff;
        union {
            const BVH *myBVHPtr;
            /// NOTE: This might be nullptr if the GU_RayPrimInfo was degenerate,
            ///       even if myOtherType is OTHERPRIM_RAYPRIMINFO.
            GU_RayPrimInfo *myRayPrimPtr;
            void *myDataPtr;
            uint myDataInts[2];
            /// NOTE: This can't be GA_Offset, since on strict types builds,
            ///       GA_Offset has a non-trivial default constructor.
            GA_Size myPt0Off;
        };
        union {
            struct {
                /// Used for triangles
                float myN[NAXES];
                /// Used for w component of polygons in polygon soups
                float myF;
            };
            struct {
                union {
                    /// Used if myOtherType is not OTHERPRIM_RAYPRIMITIVE
                    void *myOtherTransform;

                    /// Used if myOtherType is OTHERPRIM_RAYPRIMITIVE
                    const GEO_Detail *myDetail;

                    /// Used if myOtherType is OTHERPRIM_SEGMENT
                    /// NOTE: This can't be GA_Offset, since on strict types builds,
                    ///       GA_Offset has a non-trivial default constructor.
                    GA_Size myPt1Off;
                };
                /// Used if in the "other" primitive range
                OtherPrimType myOtherType;

                /// Used if myOtherType is OTHERPRIM_SEGMENT
                uint mySegment;
            };
            struct {
                /// Used for mesh primitives
                uint myCols;
                uint myRows;
            };
        };
    };

    struct NGonTri
    {
        UT_Vector3 myN;
        char myNGonXAxis;
        char myNGonYAxis;
        GA_Offset myPtOffs[3];
        float myNGonCentreX;
        float myNGonCentreY;
    };
    struct NGonQuad
    {
        GA_Offset myPtOffs[4];
        float myNGonCentreX;
        float myNGonCentreY;
        char myNGonXAxis;
        char myNGonYAxis;
    };

    /// Point offsets for triangles
    /// This array actually contains myQuadPoints and myTetPoints, too.
    UT_UniquePtr<GA_Offset[]> myTriPoints;
    /// Point offsets for quads
    /// This is actually a pointer into myTriPoints.
    GA_Offset *myQuadPoints;
    /// Point offsets for tets
    /// This is actually a pointer into myTriPoints.
    GA_Offset *myTetPoints;

    GA_Size myTriStart;
    GA_Size myQuadStart;
    GA_Size myTetStart;
    GA_Size myOtherPrimStart;
    GA_Size myNItems;

    GA_DataId myTopologyDataId;
    GA_DataId myPrimitiveListDataId;

    /// Additional data for primitives, especially primitives that are not
    /// triangles, quads, or tets.
    UT_UniquePtr<PrimData[]> myPrimData;

    /// These are for managing packed primitives.
    /// We share trees for common details, and we share details for common
    /// packed primitive implementations that don't directly provide a detail.
    /// @{
    using SecondaryTreeMap = UT_ArrayMap<const GEO_Detail*,BVH*>;
    using SecondaryDetailMap = UT_ArrayMap<const GU_PackedImpl*,GEO_Detail*>;
    SecondaryTreeMap mySecondaryTrees;
    SecondaryDetailMap mySecondaryDetails;
    /// @}

    bool myHasSecondary;

public:
    SYS_FORCE_INLINE BVH() noexcept
        : Base()
        , myTriPoints(nullptr)
        , myQuadPoints(nullptr)
        , myTetPoints(nullptr)
        , myTriStart(0)
        , myQuadStart(0)
        , myTetStart(0)
        , myOtherPrimStart(0)
        , myNItems(0)
        , myTopologyDataId(GA_INVALID_DATAID)
        , myPrimitiveListDataId(GA_INVALID_DATAID)
        , myPrimData(nullptr)
        , myHasSecondary(false)
    {}
    SYS_FORCE_INLINE ~BVH() noexcept
    {
        clear();
    }

    SYS_FORCE_INLINE GA_Offset primitiveOffset(exint prim_index) const noexcept
    {
        return myPrimData[prim_index].myPrimOff;
    }

    void clear() noexcept;

    SYS_FORCE_INLINE GA_Size numItems() const noexcept
    {
        return myNItems;
    }

    struct Options
    {
        enum PointStyle
        {
            NO_POINTS,
            DISCONNECTED_POINTS,
            ALL_POINTS
        };

        PointStyle points = NO_POINTS;

        const GA_Range *point_range = nullptr;
        const GA_Range *prim_range = nullptr;
        bool just_edges = false;

        /// If true, only triangles on the unshared surface of tetrahedra will
        /// be added, not the solid tetrahedra themselves.  findClosest will
        /// find the closest surface point, and sendRay will only intersect the
        /// surface.
        ///
        /// If false, only the solid tetrahedra will be added.  findClosest will
        /// find the current point if it's inside a tetrahedron, else the
        /// closest surface point, and sendRay will intersect the current point
        /// if it's inside a tetrahedron, else the first surface intersected.
        bool tet_surface = false;
    };

    /// NOTE: If options is different from what it was before, you must set
    ///       force_rebalance to true.
    /// NOTE: With this signature, radscale scales the pscale attribute
    ///       if it's a valid attribute, else it's the point radius.
    void init(const GEO_Detail &detail,
        const GA_ROHandleT<VectorType> &P,
        const Options &options,
        const GA_ROHandleF &pscale,
        const float radscale = 1.0f,
        const bool force_rebalance=false,
        SecondaryTreeMap *secondary_trees=nullptr,
        SecondaryDetailMap *secondary_details=nullptr,
        UT_Lock *secondary_lock=nullptr) noexcept;

    /// NOTE: With this signature, radius is the point radius.
    void init(const GEO_Detail &detail,
        const GA_ROHandleT<VectorType> &P,
        const Options &options,
        const float radius = 0.0f,
        const bool force_rebalance=false) noexcept
    {
        init(detail, P, options, GA_ROHandleF(), radius, force_rebalance);
    }

    SYS_FORCE_INLINE bool
    isNGon(exint index) const noexcept
    {
        if (index < myOtherPrimStart)
            return false;
        exint prim_index = index - myTriStart;
        OtherPrimType type = myPrimData[prim_index].myOtherType;
        return type == OTHERPRIM_NGON_TRI || type == OTHERPRIM_NGON_QUAD;
    }

    /// The UVWs returned for n-gons are for the sub-polygons,
    /// so we need to compute the true UVWs from the positions.
    UT_Vector3 findNGonUVW(exint index, const UT_Vector3 &position, const GEO_Detail &detail) const noexcept;

    SYS_FORCE_INLINE bool
    isPacked(exint index) const noexcept
    {
        if (index < myOtherPrimStart)
            return false;
        exint prim_index = index - myTriStart;
        OtherPrimType type = myPrimData[prim_index].myOtherType;
        return type == OTHERPRIM_BVHUNIFORM || type == OTHERPRIM_BVHXLATESCALE || type == OTHERPRIM_BVHFULLXFORM;
    }

protected:
    /// These are just used in the init function to change the decision about
    /// clearing after some things have been initialized.
    /// @{
    void clearPrimDataAllocations() noexcept;
    void clearSecondary() noexcept;
    /// @}

    template<bool farthest,bool rm_backface,bool reverse,typename FUNCTOR>
    bool intersectPrim(
        uint index, const VectorType &origin, const VectorType &direction,
        const VectorType &inverse_direction,
        int &max_dir, VectorType &N0, VectorType &N1,
        float &outer_tmax, float &outer_tmin, FUNCTOR &hit_info) const noexcept;
    template<bool farthest>
    void closestPrim(
        uint index, const VectorType &origin, float &max_dist_squared,
        exint &hit_index, UT_Vector3 &hit_uvw, VectorType &hit_position,
        const UT_FixedVector<v4uf,NAXES> &vorigin,
        UT_Array<exint> *nesting_array,
        exint nesting_array_base) const noexcept;
    template<bool normalize>
    VectorType primGeometricNormal(const CommonHitInfo &hit_info) const noexcept;
    SYS_FORCE_INLINE void primDerivs(const CommonHitInfo &hit_info, VectorType &dP_du, VectorType &dP_dv) const noexcept;
    template<GA_AttributeOwner owner,typename T,typename DEST_T>
    SYS_FORCE_INLINE bool primAttribute(const CommonHitInfo &hit_info, const GA_ROHandleT<T> &attrib, const GEO_Detail &detail, DEST_T &value) const noexcept;

    template<bool farthest,bool rm_backface,bool reverse,bool bidirectional=false,typename FUNCTOR>
    static SYS_FORCE_INLINE bool intersectQuad(
        const UT_Vector3 &origin,
        const UT_Vector3 &inverse_direction,
        const UT_Vector3 pos[4],
        const int max_dir,
        const UT_Vector3 &N0,
        const UT_Vector3 &N1,
        const PrimData &prim_data,
        float &outer_tmin,
        float &outer_tmax,
        const uint index,
        FUNCTOR &hit_info);

    template<bool farthest,bool rm_backface,bool reverse,bool bidirectional=false,typename FUNCTOR>
    static SYS_FORCE_INLINE bool intersectTet(
        const UT_Vector3 &origin,
        const UT_Vector3 &inverse_direction,
        const UT_Vector3 pos[4],
        const PrimData &prim_data,
        float &outer_tmin,
        float &outer_tmax,
        const uint index,
        FUNCTOR &hit_info);

    template<bool farthest>
    static SYS_FORCE_INLINE bool triClosestPoint(
        uint index,
        const VectorType &origin,
        const VectorType pos[3],
        const PrimData &prim_data,
        const UT_Vector3 &normal,
        float &max_dist_squared,
        exint &hit_index,
        UT_Vector3 &hit_uvw,
        UT_Vector3 &hit_position);
    template<bool farthest>
    static SYS_FORCE_INLINE bool quadClosestPoint(
        uint index,
        const VectorType &origin,
        const UT_FixedVector<v4uf,3> &vorigin,
        const VectorType pos[4],
        const PrimData &prim_data,
        float &max_dist_squared,
        exint &hit_index,
        UT_Vector3 &hit_uvw,
        UT_Vector3 &hit_position);

    template<typename V3_ARRAY>
    SYS_FORCE_INLINE static void addTriangleData(
        GA_Offset *&tri_points,
        const V3_ARRAY &positions,
        PrimData *primdata,
        SingleBoxType *prim_box_start,
        exint &tri_primnum,
        GA_Offset primoff,
        int data_int = -1,
        float data_float = 0) noexcept;

    template<typename V3_ARRAY>
    SYS_FORCE_INLINE static void addQuadData(
        GA_Offset *&quad_points,
        const V3_ARRAY &positions,
        PrimData *primdata,
        SingleBoxType *prim_box_start,
        exint &quad_primnum,
        GA_Offset primoff,
        int data_int = -1,
        float data_float = 0) noexcept;

    SYS_FORCE_INLINE static void addRayPrimInfo(
        PrimData &primdata,
        GA_Offset primoff,
        SingleBoxType &primbox,
        GU_RayPrimInfo *rayprim,
        exint &other_primnum,
        const GEO_Detail &detail,
        float w = 0) noexcept;
};

#if 0
class GU_API BVH_2D : public BVHBase<2, BVH_2D>
{
    using Base = BVHBase<2, BVH_2D>;
    friend class Base;
    static constexpr uint NAXES = 2;

    static constexpr bool theHasPrimitives = true;

    struct PrimData
    {
        GA_Offset myPrimOff;
        union {
            void *myDataPtr;
            uint myDataInts[2];
        };
        VectorType myN;
        float myF;
    };

    /// Point offsets for segments
    UT_UniquePtr<GA_Offset[]> mySegmentPoints;

    GA_Size myOtherPrimStart;

    /// Additional data for primitives.
    UT_UniquePtr<PrimData[]> myPrimData;

public:
    SYS_FORCE_INLINE BVH_2D() noexcept
        : Base()
        , mySegmentPoints(nullptr)
    {}

    SYS_FORCE_INLINE GA_Offset primitiveOffset(exint prim_index) const noexcept
    {
        return myPrimData[prim_index].myPrimOff;
    }

    SYS_FORCE_INLINE void clear() noexcept
    {
        Base::clear();
        mySegmentPoints.reset();
        myPrimData.reset();
    }

    struct Options
    {
        enum PointStyle
        {
            NO_POINTS,
            DISCONNECTED_POINTS,
            ALL_POINTS
        };

        PointStyle points = NO_POINTS;

        const GA_Range *point_range = nullptr;
        const GA_Range *prim_range = nullptr;
        bool just_edges = false;
    };

    void init(const GA_Detail &detail,
        const GA_ROHandleT<VectorType> &P,
        const Options &options,
        const GA_ROHandleF &pscale=GA_ROHandleF(),
        const bool force_rebalance=false) noexcept;

protected:
    template<bool farthest,bool rm_backface,bool reverse>
    SYS_FORCE_INLINE bool intersectPrim(
        uint index, const VectorType &origin, const VectorType &direction,
        const VectorType &inverse_direction,
        int &max_dir, VectorType &N0, VectorType &N1,
        float &outer_tmax, float &outer_tmin, exint &hit_index, UT_Vector3 &hit_uvw) const noexcept;
    template<bool farthest>
    SYS_FORCE_INLINE void closestPrim(
        uint index, const VectorType &origin, float &max_dist_squared,
        exint &hit_index, UT_Vector3 &hit_uvw, VectorType &hit_position,
        const UT_FixedVector<v4uf,NAXES> &vorigin,
        UT_Array<exint> *nesting_array,
        exint nesting_array_base) const noexcept;
    template<bool normalize>
    SYS_FORCE_INLINE VectorType primGeometricNormal(const CommonHitInfo &hit_info) const noexcept;
    SYS_FORCE_INLINE void primDerivs(const CommonHitInfo &hit_info, VectorType &dP_du, VectorType &dP_dv) const noexcept;
    template<GA_AttributeOwner owner,typename T,typename DEST_T>
    SYS_FORCE_INLINE bool primAttribute(const CommonHitInfo &hit_info, const GA_ROHandleT<T> &attrib, const GEO_Detail &detail, DEST_T &value) const noexcept;
};
#endif

} // GU namespace

using GU_BVH = GU::BVH;

