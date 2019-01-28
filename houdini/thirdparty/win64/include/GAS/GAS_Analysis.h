/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_Analysis.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_Analysis__
#define __GAS_Analysis__

#include "GAS_API.h"

#include <UT/UT_ThreadedAlgorithm.h>
#include <GU/GU_Detail.h>

#include <SIM/SIM_RawField.h>
#include <SIM/SIM_ScalarField.h>
#include <SIM/SIM_VectorField.h>
#include <SIM/SIM_PhysicalParms.h>

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class SIM_ScalarField;
class SIM_VectorField;
class SIM_MatrixField;

class GAS_API GAS_Analysis : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_FIELDDEST, FieldDstName);
    GET_DATA_FUNC_S(GAS_NAME_FIELDSOURCE, FieldSrcName);

    GET_DATA_FUNC_I("analysis", Analysis);
    GET_DATA_FUNC_B(SIM_NAME_OPENCL, UseOpenCL);

    // Analysis methods
    enum ANALYSIS_NAMES
    {
	CURVATURE,
	GRADIENT,
	LAPLACIAN,
	DIVERGENCE,
	CURL,
	NORMALIZE,
	LENGTH,
	NUM_ANALYSIS
    };

protected:
    explicit		 GAS_Analysis(const SIM_DataFactory *factory);
    virtual		~GAS_Analysis();

    bool		 shouldMultiThread(const SIM_RawField *field) const 
			 { return field->field()->numTiles() > 1; }

    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

private:
    static const SIM_DopDescription	*getDopDescription();


    // We have helper wrappers around the same->same methods to avoid
    // read/modify/write problems where the source and destination fields
    // are the same.  This is impossible for the ones that change
    // dimensions.
    void		calculateCurvature(SIM_RawField *dst, SIM_RawField *src) const;
    THREADED_METHOD2_CONST(GAS_Analysis, shouldMultiThread(dst),
			threadCurvature,
			SIM_RawField *, dst,
			const SIM_RawField *, src);
    void		threadCurvaturePartial(SIM_RawField *dst, 
				    const SIM_RawField *src, 
				    const UT_JobInfo &info) const;

    void		calculateLaplacian(SIM_RawField *dst, const SIM_RawField *src) const;
    THREADED_METHOD2_CONST(GAS_Analysis, shouldMultiThread(dst),
			threadLaplacian,
			SIM_RawField *, dst,
			const SIM_RawField *, src);
    void		threadLaplacianPartial(SIM_RawField *dst, 
				    const SIM_RawField *src, 
				    const UT_JobInfo &info) const;

    THREADED_METHOD2_CONST(GAS_Analysis, shouldMultiThread(src->getField()),
			calculateScalarGradientAligned,
			SIM_VectorField *, dst,
			const SIM_ScalarField *, src);
    void		calculateScalarGradientAlignedPartial(
				    SIM_VectorField *dst, 
				    const SIM_ScalarField *src, 
				    const UT_JobInfo &info) const;
    THREADED_METHOD3_CONST(GAS_Analysis, shouldMultiThread(dst),
			calculateScalarGradient,
			SIM_RawField *, dst,
			int, axis,
			const SIM_ScalarField *, src);
    void		calculateScalarGradientPartial(SIM_RawField *dst, 
				    int axis, 
				    const SIM_ScalarField *src, 
				    const UT_JobInfo &info) const;
    THREADED_METHOD2_CONST(GAS_Analysis, shouldMultiThread(src->getField(0)),
			calculateVectorGradientAligned,
			SIM_MatrixField *, dst,
			const SIM_VectorField *, src);
    void		calculateVectorGradientAlignedPartial(
				    SIM_MatrixField *dst, 
				    const SIM_VectorField *src, 
				    const UT_JobInfo &info) const;
    THREADED_METHOD4_CONST(GAS_Analysis, shouldMultiThread(dst),
			calculateVectorGradient,
			SIM_RawField *, dst,
			int, a1,
			int, a2,
			const SIM_VectorField *, src);
    void		calculateVectorGradientPartial(SIM_RawField *dst, 
				    int a1, int a2, 
				    const SIM_VectorField *src, 
				    const UT_JobInfo &info) const;

    THREADED_METHOD2_CONST(GAS_Analysis, shouldMultiThread(dst),
			calculateDivergence,
			SIM_RawField *, dst,
			const SIM_VectorField *, src);
    void		calculateDivergencePartial(SIM_RawField *dst, 
				    const SIM_VectorField *src, 
				    const UT_JobInfo &info) const;

    void		calculateCurl(SIM_VectorField *dst, const SIM_VectorField *src);

    THREADED_METHOD2_CONST(GAS_Analysis, shouldMultiThread(dst[0]),
			threadCurlAligned,
			SIM_RawField **, dst,
			const SIM_VectorField *, src);
    void		threadCurlAlignedPartial(SIM_RawField **dst,
				const SIM_VectorField *src,
				const UT_JobInfo &info) const;
    THREADED_METHOD2_CONST(GAS_Analysis, shouldMultiThread(dst[0]),
			threadCurlAlignedFace,
			SIM_RawField **, dst,
			const SIM_VectorField *, src);
    void		threadCurlAlignedFacePartial(SIM_RawField **dst,
				const SIM_VectorField *src,
				const UT_JobInfo &info) const;
    THREADED_METHOD3_CONST(GAS_Analysis, shouldMultiThread(dst),
			threadCurl,
			SIM_RawField *, dst,
			int, axis,
			const SIM_VectorField *, src);
    void		threadCurlPartial(SIM_RawField *dst, 
				    int axis, 
				    const SIM_VectorField *src, 
				    const UT_JobInfo &info) const;

    void		calculateNormalize(SIM_VectorField *dst, const SIM_VectorField *src);
    THREADED_METHOD2_CONST(GAS_Analysis, shouldMultiThread(dst->getField(0)),
			threadNormalizeAligned,
			SIM_VectorField *, dst,
			const SIM_VectorField *, src);
    void		threadNormalizeAlignedPartial(SIM_VectorField *dst, 
				    const SIM_VectorField *src, 
				    const UT_JobInfo &info) const;
    THREADED_METHOD3_CONST(GAS_Analysis, shouldMultiThread(dst),
			threadNormalize,
			SIM_RawField *, dst,
			int, axis,
			const SIM_VectorField *, src);
    void		threadNormalizePartial(SIM_RawField *dst, 
				    int axis, 
				    const SIM_VectorField *src, 
				    const UT_JobInfo &info) const;

    THREADED_METHOD2_CONST(GAS_Analysis, shouldMultiThread(dst),
			calculateLengthAligned,
			SIM_RawField *, dst,
			const SIM_VectorField *, src);
    void		calculateLengthAlignedPartial(SIM_RawField *dst, 
				    const SIM_VectorField *src, 
				    const UT_JobInfo &info) const;
    THREADED_METHOD2_CONST(GAS_Analysis, shouldMultiThread(dst),
			calculateLength,
			SIM_RawField *, dst,
			const SIM_VectorField *, src);
    void		calculateLengthPartial(SIM_RawField *dst, 
				    const SIM_VectorField *src, 
				    const UT_JobInfo &info) const;

    THREADED_METHOD2_CONST(GAS_Analysis, shouldMultiThread(dst),
			calculateMatrixLengthAligned,
			SIM_RawField *, dst,
			const SIM_MatrixField *, src);
    void		calculateMatrixLengthAlignedPartial(SIM_RawField *dst, 
				    const SIM_MatrixField *src, 
				    const UT_JobInfo &info) const;
    THREADED_METHOD2_CONST(GAS_Analysis, shouldMultiThread(dst),
			calculateMatrixLength,
			SIM_RawField *, dst,
			const SIM_MatrixField *, src);
    void		calculateMatrixLengthPartial(SIM_RawField *dst, 
				    const SIM_MatrixField *src, 
				    const UT_JobInfo &info) const;

    // OpenCL functions
    void        calculateCurlCL(SIM_Object *obj,
                                SIM_VectorField *dst,
                                const SIM_VectorField *src);
    void        calculateLengthCL(SIM_Object *obj,
                                  SIM_ScalarField *dst,
                                  const SIM_VectorField *src);
    void        calculateScalarGradientCL(SIM_Object *obj,
                                          SIM_VectorField *dst,
                                          const SIM_ScalarField* src,
                                          bool doNormalize = false);
    void        calculateNormalizeCL(SIM_Object *obj,
                                     SIM_VectorField* dst,
                                     SIM_VectorField* src);
    void        calculateDivergenceCL(SIM_Object *obj,
                                     SIM_ScalarField* dst,
                                     SIM_VectorField* src);

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_Analysis,
			GAS_SubSolver,
			"Gas Analysis",
			getDopDescription());

    // Cache our timestep for so we can calculate the timescale
    // effects.
    float		myTimeStep;
};

#endif
