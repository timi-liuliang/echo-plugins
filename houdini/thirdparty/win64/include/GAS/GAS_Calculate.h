/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_Calculate.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_Calculate__
#define __GAS_Calculate__

#include "GAS_API.h"

#include <UT/UT_ThreadedAlgorithm.h>
#include <GU/GU_Detail.h>

#include <SIM/SIM_RawField.h>
#include <SIM/SIM_PhysicalParms.h>

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class GAS_API GAS_Calculate : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_FIELDDEST, FieldDstName);
    GET_DATA_FUNC_S(GAS_NAME_FIELDSOURCE, FieldSrcName);
    GET_DATA_FUNC_S("dstblend", DstBlend);
    GET_DATA_FUNC_S("srcblend", SrcBlend);

    GET_DATA_FUNC_S("srcindex", SrcIndexName);
    GET_DATA_FUNC_I("property", Property);

    GET_DATA_FUNC_I("srctimescale", SrcTimeScale);
    GET_DATA_FUNC_I("dsttimescale", DstTimeScale);
    GET_DATA_FUNC_I("posttimescale", PostTimeScale);

    GET_DATA_FUNC_I("calculationtype", CalculationType);
    GET_DATA_FUNC_I("blendmethod", BlendMethod);
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

    GET_DATA_FUNC_F("dstblendpreadd", DstBlendPreAdd);
    GET_DATA_FUNC_F("dstblendpremul", DstBlendPreMul);
    GET_DATA_FUNC_I("dstblendconvert", DstBlendConvert);
    GET_DATA_FUNC_I("dstblendscale", DstBlendScale);
    GET_DATA_FUNC_F("srcblendpreadd", SrcBlendPreAdd);
    GET_DATA_FUNC_F("srcblendpremul", SrcBlendPreMul);
    GET_DATA_FUNC_I("srcblendconvert", SrcBlendConvert);
    GET_DATA_FUNC_I("srcblendscale", SrcBlendScale);

    GET_DATA_FUNC_B(GAS_NAME_USETIMESTEP, UseTimeStep);
    GET_DATA_FUNC_F(GAS_NAME_TIMESCALE, TimeScale);

    // Blend methods
    enum BLEND_NAMES
    {
	BLEND_NONE,
	BLEND_WEIGHTEDAVERAGE,
	BLEND_WEIGHTEDSUM,
	BLEND_NETNEWAVERAGE,
	BLEND_NETNEWSDF,
	NUM_BLEND
    };

    // Extra mix methods to handle SDF specific functions
    enum CONVERT_NAMES {
	CONVERT_NONE,
	CONVERT_SDFTOFOG,
	CONVERT_SDFTOSURFACE,
	CONVERT_ABS,
	NUM_CONVERT
    };

    /// Helper methods to apply conversion and blend methods.
    SYS_STATIC_FORCE_INLINE float	 convertValues(CONVERT_NAMES type, LENGTHSCALE_NAMES lengthscale, float premul, float preadd, float d, float width)
    {
	// Ensure we have a consistent idea of how to perform these.
	d *= premul;
	d += preadd;

	// Use our extra mix types...
	switch (type)
	{
	    case CONVERT_NONE:
		// Nice and simple
		break;

	    case CONVERT_SDFTOFOG:
		// Something exactly 0 should be 0.5, total width of kernel
		// is width
		d = SIM_RawField::toHeaviside(d, width);
		break;

	    case CONVERT_SDFTOSURFACE:
		// Computes our surface layer.  Reducing this with SUM
		// should give us our total surface area modulo the 
		// voxelsize.
		// The choice of function is so that a flat plane aligned
		// with the voxel grid will have the same surface area
		// regardless of its sub-voxel location.
		d = SYSabs(d);
		if (d > width/2)
		    d = 0;
		else
		    d = 1.0 - (d / (width/2));
		break;

	    case CONVERT_ABS:
		d = SYSabs(d);
		break;

	    default:
		UT_ASSERT(!"Unhandled mix type!");
		d = 0.0;
		break;
	}

	// Apply the length scale.
	d = applyLengthScale(d, width, lengthscale);

	return d;
    }

    static bool		 blendValues(BLEND_NAMES blendmethod, float &d, float dblend, float &sfactor, float sblend);

protected:
    explicit		 GAS_Calculate(const SIM_DataFactory *factory);
    virtual		~GAS_Calculate();

    /// Calculates the desired procedure with dest & src, result
    /// going into dest.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

    THREADED_METHOD4(GAS_Calculate, dst->shouldMultiThread(),
		     mixEachVoxel,
		     SIM_RawField *, dst,
		     const SIM_RawField *, src,
		     const GU_SDF *, sdf,
		     const UT_DMatrix4 &, tosdf)
    void		 mixEachVoxelPartial(SIM_RawField *dst,
				const SIM_RawField *src,
				const GU_SDF *sdf,
				const UT_DMatrix4 &tosdf,
				const UT_JobInfo &info);

    THREADED_METHOD6(GAS_Calculate, dst->shouldMultiThread(),
		     mixEachVoxelIndex,
		     SIM_Engine &, engine,
		     SIM_RawField *, dst,
		     const UT_DMatrix4 &, towolrd,
		     const SIM_IndexField *, src,
		     int, srcaxis,
		     fpreal, velscale)
    void		 mixEachVoxelIndexPartial(SIM_Engine &engine,
				SIM_RawField *dst,
				const UT_DMatrix4 &toworld,
				const SIM_IndexField *src,
				int srcaxis,
				fpreal velscale,
				const UT_JobInfo &info);


    THREADED_METHOD6(GAS_Calculate, dst->shouldMultiThread(),
		     blendEachVoxel,
		     SIM_RawField *, dst,
		     const SIM_RawField *, src,
		     const GU_SDF *, sdf,
		     const UT_DMatrix4 &, tosdf,
		     const SIM_RawField *, dstblend,
		     const SIM_RawField *, srcblend)
    void		 blendEachVoxelPartial(SIM_RawField *dst,
				const SIM_RawField *src,
				const GU_SDF *sdf,
				const UT_DMatrix4 &tosdf,
				const SIM_RawField *dstblend,
				const SIM_RawField *srcblend,
				const UT_JobInfo &info);

    THREADED_METHOD8(GAS_Calculate, dst->shouldMultiThread(),
		     blendEachVoxelIndex,
		     SIM_Engine &, engine,
		     SIM_RawField *, dst,
		     const UT_DMatrix4 &, toworld,
		     const SIM_IndexField *, src,
		     int, srcaxis,
		     const SIM_RawField *, dstblend,
		     const SIM_RawField *, srcblend,
		     fpreal, velscale)
    void		 blendEachVoxelIndexPartial(SIM_Engine &engine,
				SIM_RawField *dst,
				const UT_DMatrix4 &toworld,
				const SIM_IndexField *src,
				int srcaxis,
				const SIM_RawField *dstblend,
				const SIM_RawField *srcblend,
				fpreal velscale,
				const UT_JobInfo &info);

    /// Returns true if the current getProperty() is safe to do in
    /// a multithreaded manner.  Ideally would have to invoke
    /// SIM_PropertyResolver for all potential indices, but currently
    /// relies on apriori knowledge that only the fancy velocity
    /// operations are not threadsafe.
    bool	isPropertyThreadsafe() const;

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_Calculate,
			GAS_SubSolver,
			"Gas Calculate",
			getDopDescription());

    // Cache our timestep for so we can calculate the timescale
    // effects.
    float		myTimeStep;
};

#endif
