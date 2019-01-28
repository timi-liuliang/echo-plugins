/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __SIM_SopMergeField__
#define __SIM_SopMergeField__

#include "SIMZ_API.h"

#include <UT/UT_ThreadedAlgorithm.h>

#include <SIM/SIM_RawField.h>

#include <GAS/GAS_Calculate.h>
#include <GAS/GAS_Utils.h>

class SIMZ_API SIM_SopMergeField : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_FIELDDEST, FieldDstName);
    GET_DATA_FUNC_S("soppath", SopPath);
    GET_DATA_FUNC_S("srcname", VolumeName);
    GET_DATA_FUNC_S("dopmask", DopMask);
    GET_DATA_FUNC_B("dabsolutemask", DAbsoluteMask);
    GET_DATA_FUNC_B("dsdfmask", DSDFMask);
    GET_DATA_FUNC_S("sopmask", SopMask);
    GET_DATA_FUNC_B("sabsolutemask", SAbsoluteMask);
    GET_DATA_FUNC_B("ssdfmask", SSDFMask);

    GET_DATA_FUNC_I("srctimescale", SrcTimeScale);
    GET_DATA_FUNC_I("dsttimescale", DstTimeScale);
    GET_DATA_FUNC_I("posttimescale", PostTimeScale);

    GET_DATA_FUNC_B("clamped", Clamped);
    GET_DATA_FUNC_B("usevectornorm", UseVectorNorm);

    GET_DATA_FUNC_I("calculationtype", CalculationType);
    GET_DATA_FUNC_B("guidedir", GuideDir);
    GET_DATA_FUNC_F("mastrength", AccGuideStrength);
    GET_DATA_FUNC_F("mdstrength", DecGuideStrength);
    GET_DATA_FUNC_F("dirstrength", DirGuideStrength);

    GET_DATA_FUNC_F("dstpreadd", DstPreAdd);
    GET_DATA_FUNC_F("dstpremul", DstPreMul);
    GET_DATA_FUNC_I("dstconvert", DstConvert);
    GET_DATA_FUNC_I("dstscale", DstScale);
    GET_DATA_FUNC_F("srcpreadd", SrcPreAdd);
    GET_DATA_FUNC_F("srcpremul", SrcPreMul);
    GET_DATA_FUNC_I("srcconvert", SrcConvert);
    GET_DATA_FUNC_I("srcscale", SrcScale);
    GET_DATA_FUNC_F("postadd", PostAdd);
    GET_DATA_FUNC_F("postmul", PostMul);
    GET_DATA_FUNC_I("postconvert", PostConvert);
    GET_DATA_FUNC_I("postscale", PostScale);

    GET_DATA_FUNC_B("warnmissing", WarnMissing);
    GET_DATA_FUNC_B(GAS_NAME_USETIMESTEP, UseTimeStep);
    GET_DATA_FUNC_F(GAS_NAME_TIMESCALE, TimeScale);

    struct SIM_SopMergeParms
    {
        float                               accstr, decstr, dirstr;
        bool                                clamped;
        bool                                s_op, d_op, p_op;
        bool                                sop_mask, sop_absolutemask,
                                            sop_sdfmask;
        bool                                dop_mask, dop_absolutemask,
                                            dop_sdfmask;
        float                               d_preadd, d_premul;
        float                               s_preadd, s_premul;
        float                               postadd, postmul;
        MIX_NAMES		                    calctype;
        LENGTHSCALE_NAMES               	srcscale, dstscale, postscale;
        GAS_Calculate::CONVERT_NAMES        srcconvert, dstconvert,
                                            postconvert;
        float				                d_width;
        float                               s_width;
    };

protected:
    explicit        SIM_SopMergeField(const SIM_DataFactory *factory);
    virtual		    ~SIM_SopMergeField();

    /// Calculates the desired procedure with dest and src, result going into
    /// dest.
    virtual bool    solveGasSubclass(SIM_Engine &engine,
                                     SIM_Object *obj,
                                     SIM_Time time,
                                     SIM_Time timestep);

private:
    /// This function determines which ops (source, destination, post) actually
    /// need to be performed (are not no ops).
    void decideOps(SIM_SopMergeParms& parms)
    {
        parms.s_op = (parms.s_preadd != 0) ||
                     (parms.s_premul != 1) ||
                     (parms.srcscale != LENGTHSCALE_NONE) ||
                     (parms.srcconvert !=
                        GAS_Calculate::CONVERT_NAMES::CONVERT_NONE);
        parms.d_op = (parms.calctype != MIX_COPY) &&
                     ((parms.d_preadd != 0) ||
                     (parms.d_premul != 1) ||
                     (parms.dstscale != LENGTHSCALE_NONE) ||
                     (parms.dstconvert !=
                        GAS_Calculate::CONVERT_NAMES::CONVERT_NONE));
        parms.p_op = (parms.postadd != 0) ||
                     (parms.postmul != 1) ||
                     (parms.srcscale != LENGTHSCALE_NONE) ||
                     (parms.srcconvert !=
                        GAS_Calculate::CONVERT_NAMES::CONVERT_NONE);
    }

    /// Mixes a single raw field with a single primitive (volume or VDB).
    THREADED_METHOD6(SIM_SopMergeField, dst->shouldMultiThread(),
                     mixEachVoxelVolume,
                     SIM_RawField*, dst,
                     const GEO_Primitive*, src,
                     const SIM_RawField*, mask,
                     const GEO_Primitive*, sopmask,
                     const UT_DMatrix4&, xform,
                     const SIM_SopMergeParms&, parms)
    void            mixEachVoxelVolumePartial(SIM_RawField* dst,
                                              const GEO_Primitive* src,
                                              const SIM_RawField* mask,
                                              const GEO_Primitive* sopmask,
                                              const UT_DMatrix4& xform,
                                              const SIM_SopMergeParms& parms,
                                              const UT_JobInfo& info);

    /// Mixes a triplet of raw fields with a triplet of volume or VDB
    /// primitives, when calculation type is set to min or max and comparison
    /// is performed using vector norms. Stores the result in the aligned fin
    /// fields. The result is the difference between calculated and destination
    /// values when storediff is set to true. In all cases, inactive regions
    /// are left untouched in clamped mode; thus, fin fields should probably be
    /// initialized to 0 when storediff is true.
    /// Also works with a vector VDB, which should be in src[0]; in this case,
    /// src[1] must be NULL.
    THREADED_METHOD8(SIM_SopMergeField,
                     fin[0]->shouldMultiThread() && fin[1]->shouldMultiThread()
                        && fin[2]->shouldMultiThread(),
                     mixEachVoxelVolumeNormed,
                     SIM_RawField**, fin,
                     SIM_RawField**, dst,
                     const GEO_Primitive**, src,
                     const SIM_RawField*, mask,
                     const GEO_Primitive*, sopmask,
                     const UT_DMatrix4&, xform,
                     const SIM_SopMergeParms&, parms,
                     bool, storediff)
    void            mixEachVoxelVolumeNormedPartial(SIM_RawField* fin[3],
                                                    SIM_RawField* dst[3],
                                                    const GEO_Primitive* src[3],
                                                    const SIM_RawField* mask,
                                                    const GEO_Primitive* sopmask,
                                                    const UT_DMatrix4& xform,
                                                    const SIM_SopMergeParms& parms,
                                                    bool storediff,
                                                    const UT_JobInfo& info);

    /// Mixes a triplet of raw fields with a single vector VDB primitive. This
    /// function uses component-wise comparison if calculation is set to min or
    /// max.
    /// The destination fields are assumed to be aligned. Non-aligned
    /// components should be set to NULL.
    THREADED_METHOD6(SIM_SopMergeField,
                     (dst[0] == NULL || dst[0]->shouldMultiThread())
                        && (dst[1] == NULL || dst[1]->shouldMultiThread())
                        && (dst[2] == NULL || dst[2]->shouldMultiThread()),
                     mixEachVoxelVDBV,
                     SIM_RawField**, dst,
                     const GEO_PrimVDB*, src,
                     const SIM_RawField*, mask,
                     const GEO_Primitive*, sopmask,
                     const UT_DMatrix4&, xform,
                     const SIM_SopMergeParms&, parms)
    void            mixEachVoxelVDBVPartial(SIM_RawField* dst[3],
                                            const GEO_PrimVDB* src,
                                            const SIM_RawField* mask,
                                            const GEO_Primitive* sopmask,
                                            const UT_DMatrix4& xform,
                                            const SIM_SopMergeParms& parms,
                                            const UT_JobInfo& info);

    /// This function adds contents of src to dst. If clamped is set to true,
    /// voxels outside of the src bounding box can be skipped.
    /// By the way this function is used by this class, we don't even bother
    /// checking if src is aligned with dst (we know it's not).
    THREADED_METHOD3(SIM_SopMergeField,
                     dst->shouldMultiThread(),
                     addEachVoxel,
                     SIM_RawField*, dst,
                     SIM_RawField*, src,
                     bool, clamped)
    void            addEachVoxelPartial(SIM_RawField* dst,
                                        SIM_RawField* src,
                                        bool clamped,
                                        const UT_JobInfo& info);

    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_SopMergeField,
                        GAS_SubSolver,
                        "SOP Merge Field",
                        getDopDescription());

    float           myTimeStep;
};

#endif

