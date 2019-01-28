/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_DisturbFieldCL.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_DisturbFieldCL_h__
#define __GAS_DisturbFieldCL_h__

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"
#include <GA/GA_SplittableRange.h>

class GAS_DisturbFieldCL : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S( "disturbfield", DisturbField);

    GET_DATA_FUNC_S( "threshold", ThresholdField);
    GET_DATA_FUNC_S( "control", ControlField );

    GET_DATA_FUNC_B( SIM_NAME_OPENCL, UseOpenCL  );
    GET_DATA_FUNC_B( "useblock",      UseBlocks  );
    GET_DATA_FUNC_B( "usecontrol",    UseControl );

    GET_DATA_FUNC_F( "disturbance",   Disturbance );
    GET_DATA_FUNC_F( "cutoff", 	      Cutoff      );
    GET_DATA_FUNC_F( "blocksize",     BlockSize   );
    GET_DATA_FUNC_F( "timescale",     TimeScale   );
    GET_DATA_FUNC_I( "locality",      Locality    );

    GET_DATA_FUNC_F( "controlinfluence",    ControlInfluence  );
    GET_DATA_FUNC_F( "controlmin",    ControlMin  );
    GET_DATA_FUNC_F( "controlmax",    ControlMax  );

protected:
    explicit   GAS_DisturbFieldCL(const SIM_DataFactory *factory);
    virtual  ~GAS_DisturbFieldCL();


    /// The overloaded callback that GAS_SubSolver will invoke to
    /// perform our actual computation.  We are giving a single object
    /// at a time to work on.
    virtual bool  solveGasSubclass(SIM_Engine &engine,
                                   SIM_Object *obj,
                                   SIM_Time time,
                                   SIM_Time timestep);


private:
    float myTime, myTimeInc, myFrame;

    bool disturb(SIM_RawField *dest, SIM_Object *obj,
                 SIM_Time timestep, int a1, int a2);

    // Performs disturb field using opencl 
    bool disturbFieldCL(SIM_Object *obj,
                        SIM_RawField* dst, 
			const SIM_RawField* control,
			const SIM_RawField* threshold,
			UT_WorkBuffer & flags );

    void disturbField( SIM_RawField *dst,
		const SIM_RawField  *control,
		const SIM_RawField  *threshold,
		int   vectorWidth );

    // Performs disturb field using CPU method
    void doDisturbFieldPartial( SIM_RawField *dst,
			const SIM_RawField *threshold_field,
			const SIM_RawField *control_field,
			UT_Vector3 scale_par,
			UT_Vector3 control_par,
			float blocksize, 
			float locality_inv,
			int vector_width,
			const UT_JobInfo &info );

    // Declare multithreaded Disturb Field
    THREADED_METHOD8(GAS_DisturbFieldCL, dst->shouldMultiThread(),
			doDisturbField,
			SIM_RawField *, dst,	
			const SIM_RawField *, threshold_field,
			const SIM_RawField *, control_field,
			UT_Vector3, 	scale_par,
			UT_Vector3, 	control_par,
			float, 		blocksize, 
			float, 		locality,
			int, 		vector_width );


    /// We define this to be a DOP_Auto node which means we do not
    /// need to implement a DOP_Node derivative for this data.  Instead,
    /// this description is used to define the interface.
    static const SIM_DopDescription *getDopDescription();

    /// These macros are necessary to bind our node to the factory and
    /// ensure useful constants like BaseClass are defined.
    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_DisturbFieldCL,
                        GAS_SubSolver,
                        "Gas Disturb Field CL",
                        getDopDescription());
};

#endif

