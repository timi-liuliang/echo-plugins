/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:    GAS_Viscosity.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_Viscosity_h__
#define __GAS_Viscosity_h__

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"
#include <UT/UT_ThreadedAlgorithm.h>

class SIM_VectorField;
class SIM_RawField;
class SIM_ScalarField;
class SIM_RawIndexField;

template<typename T>
class SparseMatrix;

class GAS_API GAS_Viscosity : public GAS_SubSolver
{
public:
    /// These macros are used to create the accessors
    /// getFieldDstName and getFieldSrcName functions we'll use
    /// to access our data options.
    GET_DATA_FUNC_S(GAS_NAME_VELOCITY, Velocity);
    GET_DATA_FUNC_F(SIM_NAME_SCALE, Scale);
    GET_DATA_FUNC_F(SIM_NAME_TOLERANCE, Tolerance);
    GET_DATA_FUNC_F("minviscosity", MinViscosity);
    GET_DATA_FUNC_F("extrapdist", ExtrapolationDistance);
    GET_DATA_FUNC_I("numsupersamples", NumSuperSamples);
    GET_DATA_FUNC_I("floatprecision", FloatPrecision);
    GET_DATA_FUNC_B("usepreconditioner", UsePreconditioner);
    GET_DATA_FUNC_B(SIM_NAME_OPENCL, UseOpenCL);
    GET_DATA_FUNC_I("maxsolveriterations", MaxSolverIterations);

    GET_DATA_FUNC_B("collisionweighttoggle", UseComputeCollisionWeights);

    GET_DATA_FUNC_F("waterline",Waterline);
    GET_DATA_FUNC_B("usewaterline",UseWaterline);
    GET_DATA_FUNC_V3("waterlinedirection",WaterlineDirection);
   
    GET_DATA_FUNC_B(GAS_NAME_USETIMESTEP, UseTimeStep);
    GET_DATA_FUNC_F(GAS_NAME_TIMESCALE, TimeScale);

protected:
    explicit         GAS_Viscosity(const SIM_DataFactory *factory);
    virtual         ~GAS_Viscosity();

    /// The overloaded callback that GAS_SubSolver will invoke to
    /// perform our actual computation.  We are giving a single object
    /// at a time to work on.
    virtual bool     solveGasSubclass(SIM_Engine &engine,
                                      SIM_Object *obj,
                                      SIM_Time time,
                                      SIM_Time timestep);

    virtual void    initializeSubclass();


private:
    /// We define this to be a DOP_Auto node which means we do not
    /// need to implement a DOP_Node derivative for this data.  Instead,
    /// this description is used to define the interface.
    static const SIM_DopDescription *getDopDescription();

    /// These macros are necessary to bind our node to the factory and
    /// ensure useful constants like BaseClass are defined.
    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_Viscosity,
                        GAS_SubSolver,
                        "Gas Viscosity",
                        getDopDescription());

};


#endif

