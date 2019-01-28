/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_Cross.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_Cross__
#define __GAS_Cross__

#include "GAS_API.h"

#include <UT/UT_ThreadedAlgorithm.h>
#include <GU/GU_Detail.h>

#include <SIM/SIM_RawField.h>
#include <SIM/SIM_VectorField.h>
#include <SIM/SIM_PhysicalParms.h>

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

class SIM_ScalarField;
class SIM_VectorField;
class SIM_MatrixField;

class GAS_API GAS_Cross : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_B(SIM_NAME_OPENCL, UseOpenCL);

protected:
    explicit		 GAS_Cross(const SIM_DataFactory *factory);
    virtual		~GAS_Cross();

    bool		 shouldMultiThread(SIM_RawField *field) const 
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
    void		calculateCross(SIM_VectorField *dst, const SIM_VectorField *a, const SIM_VectorField *b) const;

    THREADED_METHOD3_CONST(GAS_Cross, shouldMultiThread(dst->getField(0)),
			threadCrossAligned,
			SIM_VectorField *, dst,
			const SIM_VectorField *, a,
			const SIM_VectorField *, b);
    void		threadCrossAlignedPartial(SIM_VectorField *dst, 
				    const SIM_VectorField *a,
				    const SIM_VectorField *b,
				    const UT_JobInfo &info) const;
    THREADED_METHOD4_CONST(GAS_Cross, shouldMultiThread(dst),
			threadCross,
			SIM_RawField *, dst,
			int, axis,
			const SIM_VectorField *, a,
			const SIM_VectorField *, b);
    void		threadCrossPartial(SIM_RawField *dst, 
				    int axis, 
				    const SIM_VectorField *a,
				    const SIM_VectorField *b,
				    const UT_JobInfo &info) const;

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_Cross,
			GAS_SubSolver,
			"Gas Cross",
			getDopDescription());

    void calculateCrossCL(SIM_Object *obj, SIM_VectorField* dst,
                          SIM_VectorField* a, SIM_VectorField* b);

    // Cache our timestep for so we can calculate the timescale
    // effects.
    float		myTimeStep;
};

#endif
