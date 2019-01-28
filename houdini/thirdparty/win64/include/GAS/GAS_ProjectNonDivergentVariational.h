/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_ProjectNonDivergentVariational.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_ProjectNonDivergentVariational_h__
#define __GAS_ProjectNonDivergentVariational_h__

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"
#include <UT/UT_ThreadedAlgorithm.h>
#include <SIM/SIM_RawIndexField.h>

class SIM_VectorField;
class SIM_RawField;
class SIM_ScalarField;
class gas_VoxelIndex;

template <typename T, bool colmajor, bool exint>
class UT_SparseMatrixELLT;

class GAS_API GAS_ProjectNonDivergentVariational : public GAS_SubSolver
{
public:
    /// These macros are used to create the accessors
    /// getFieldDstName and getFieldSrcName functions we'll use
    /// to access our data options.
    GET_DATA_FUNC_S(GAS_NAME_VELOCITY, Velocity);
    GET_DATA_FUNC_F("mindensity", MinDensity);
    GET_DATA_FUNC_F("maxdensity", MaxDensity);
    GET_DATA_FUNC_F(SIM_NAME_TOLERANCE, Tolerance);
    GET_DATA_FUNC_F("extrapdist", ExtrapolationDistance);
    GET_DATA_FUNC_B("usepreconditioner", UsePreconditioner);
    GET_DATA_FUNC_B(SIM_NAME_OPENCL, UseOpenCL);

    GET_DATA_FUNC_S(GAS_NAME_TRACKERADDR, TrackerAddress);
    GET_DATA_FUNC_I(GAS_NAME_TRACKERPORT, TrackerPort);
    GET_DATA_FUNC_S(GAS_NAME_JOBNAME, JobName);
    GET_DATA_FUNC_I("slice", Slice);
    GET_DATA_FUNC_I("numslice", NumSlice);

    GET_DATA_FUNC_B("airincompressibility", AirIncompressibility);
    GET_DATA_FUNC_B("applyaircollisions", ApplyAirCollisions);
    GET_DATA_FUNC_F("minairregionsize", MinAirRegionSize);

    GET_DATA_FUNC_F("waterline",Waterline);
    GET_DATA_FUNC_B("usewaterline",UseWaterline);
    GET_DATA_FUNC_V3("waterlinedirection",WaterlineDirection);

    GET_DATA_FUNC_B(GAS_NAME_USETIMESTEP, UseTimeStep);
    GET_DATA_FUNC_F(GAS_NAME_TIMESCALE, TimeScale);

protected:
    explicit		 GAS_ProjectNonDivergentVariational(const SIM_DataFactory *factory);
    virtual		~GAS_ProjectNonDivergentVariational();

    /// The overloaded callback that GAS_SubSolver will invoke to
    /// perform our actual computation.  We are giving a single object
    /// at a time to work on.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				SIM_Object *obj,
				SIM_Time time,
				SIM_Time timestep);

    virtual void initializeSubclass();

    THREADED_METHOD5(GAS_ProjectNonDivergentVariational,
                     divergence.shouldMultiThread(),
                     computeDivergenceWeighted,
                     const SIM_VectorField *, velocity,
                     const SIM_VectorField *, collisionvel,
                     const SIM_RawField **, weights,
                     SIM_RawField &, divergence,
		     const SIM_RawIndexField &, index);

    void computeDivergenceWeightedPartial(const SIM_VectorField *velocity,
					  const SIM_VectorField *collisionvel,
					  const SIM_RawField *weights[3],
					  SIM_RawField &divergence,
					  const SIM_RawIndexField &index,
					  const UT_JobInfo &info);

    struct classifyVoxelsParms
    {
	const SIM_VectorField *velocity;
	const SIM_RawField *weights[3];
	const SIM_RawField *surfweights[3];
	const SIM_RawIndexField *sliceindex;
	const SIM_RawField *collision;
	SIM_RawIndexField *index;

	fpreal extrapdist;
	int thisslice;
	SIM_BoundaryLine indexblines[3];
    };

    THREADED_METHOD2(GAS_ProjectNonDivergentVariational,
                     surface.shouldMultiThread(),
                     classifyVoxels,
		     const classifyVoxelsParms &, parms,
		     const SIM_RawField &, surface);

    void classifyVoxelsPartial(
			    const classifyVoxelsParms &parms,
			    const SIM_RawField &surface,
			    const UT_JobInfo &info) const;

    exint buildIndex(const SIM_RawField &surface,
                     const SIM_RawField &collision,
                     const SIM_VectorField *velocity,
                     const SIM_RawField *weights[3],
                     const SIM_RawField *surfweights[3],
		     const SIM_RawIndexField *sliceindex,
		     int thisslice,
                     SIM_RawIndexField &index,
		     UT_Array<UT_Array<gas_VoxelIndex> *> &sliceoverflow,
		     UT_ExintArray &slicebase,
		     SIM_BoundaryLine indexblines[3]);
    
    struct applyPressureParms
    {
	const SIM_RawField *u_weights;
	const SIM_RawField *u_surfweights;
 	const SIM_RawField *pressure;
 	const SIM_RawField *surface;
 	const SIM_RawField *surfpressure;
	const SIM_RawField *density;
	const SIM_RawIndexField *index;
	SIM_RawField *valid;
        fpreal dt;
	int axis;
	SIM_BoundaryLine indexbline;
    };

    THREADED_METHOD2_CONST(GAS_ProjectNonDivergentVariational, u.shouldMultiThread(),
		    applyPressureGradientFace,
		    const applyPressureParms &, parms,
		    SIM_RawField &, u);

    void applyPressureGradientFacePartial(
				    const applyPressureParms &parms,
				    SIM_RawField &u,
				    const UT_JobInfo &info) const;


    struct openSurfaceCellsParms
    {
	UT_ValArray<bool> *results;
	SIM_RawField *div;
	const SIM_RawIndexField *index;
	SIM_VectorField *vel;
	const SIM_RawField **weights;
	const SIM_RawIndexField *sliceindex;
	int thisslice;
	SIM_BoundaryLine indexblines[3];
    };


    THREADED_METHOD2(GAS_ProjectNonDivergentVariational,
                     comp.shouldMultiThread(),
                     findOpenSurfaceCells,
		     const openSurfaceCellsParms &, parms,
		     const SIM_RawIndexField &, comp);

    void findOpenSurfaceCellsPartial(
				    const openSurfaceCellsParms & parms,
				    const SIM_RawIndexField & comp,
				    const UT_JobInfo &info) const;

private:
    /// We define this to be a DOP_Auto node which means we do not
    /// need to implement a DOP_Node derivative for this data.  Instead,
    /// this description is used to define the interface.
    static const SIM_DopDescription	*getDopDescription();

    /// These macros are necessary to bind our node to the factory and
    /// ensure useful constants like BaseClass are defined.
    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_ProjectNonDivergentVariational,
			GAS_SubSolver,
			"Gas Project Non Divergent Variational",
			getDopDescription());

};


#endif

