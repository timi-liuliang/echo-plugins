/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_LinearCombination.h ( GAS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GAS_LinearCombination_h__
#define __GAS_LinearCombination_h__

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"
#include <GA/GA_SplittableRange.h>

class GAS_LinearCombination : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_FIELDDEST, FieldDstName);
    GET_DATA_FUNC_S(GAS_NAME_GEOMETRY, GeometryName);

    GET_DATA_FUNC_B("createattrib", CreateAttrib);
    GETSET_DATA_FUNCS_I("operation", Operation);
    GET_DATA_FUNC_S("source1", FieldSrc1Name);
    GETSET_DATA_FUNCS_F("coeff1", Coeff1);
    GET_DATA_FUNC_S("source2", FieldSrc2Name);
    GETSET_DATA_FUNCS_F("coeff2", Coeff2);
    GET_DATA_FUNC_S("source3", FieldSrc3Name);
    GETSET_DATA_FUNCS_F("coeff3", Coeff3);
    GETSET_DATA_FUNCS_B("useconstant", UseConstant);
    GETSET_DATA_FUNCS_F("constant", Constant);
    GETSET_DATA_FUNCS_F("coeffpost", CoeffPost);

    GET_DATA_FUNC_B(GAS_NAME_USETIMESTEP, UseTimeStep);
    GET_DATA_FUNC_F(GAS_NAME_TIMESCALE, TimeScale);
    GET_DATA_FUNC_B(SIM_NAME_OPENCL, UseOpenCL);

protected:
    explicit   GAS_LinearCombination(const SIM_DataFactory *factory);
    virtual  ~GAS_LinearCombination();


    /// The overloaded callback that GAS_SubSolver will invoke to
    /// perform our actual computation.  We are giving a single object
    /// at a time to work on.
    virtual bool  solveGasSubclass(SIM_Engine &engine,
                                   SIM_Object *obj,
                                   SIM_Time time,
                                   SIM_Time timestep);


private:

    /// Applies the scales for timescale%s and lengthscale%s
    fpreal	applyScales(fpreal val, const char *suffix,
			    const UT_Vector3 &voxelsize,
			    SIM_Time timestep) const;

    void getCoefficientsAndConstant(fpreal coeff[3],
                                    fpreal &constant,
                                    bool &useconstant,
                                    MIX_NAMES &mixtype,
                                    fpreal &coeffpost,
                                    const UT_Vector3 &voxsize,
                                    SIM_Time timestep,
                                    bool valid[3]) const;

    THREADED_METHOD8(GAS_LinearCombination, dst->shouldMultiThread(),
		     linearCombine,
		     SIM_RawField *, dst,
		     int, numsrc,
		     const SIM_RawField **, src,
		     float *, coeff,
		     bool, useconstant,
		     float , constant,
		     float , coeffpost,
             MIX_NAMES, mixtype)
    void	 linearCombinePartial(SIM_RawField *dst,
				int numsrc,
				const SIM_RawField **src,
				float *coeff,
				bool useconstant,
				float constant,
				float coeffpost,
                MIX_NAMES mixtype,
				const UT_JobInfo &info);


    class linearCombineAttrParms
    {
    public:
        GA_RWHandleF *dest_h;
        int numsrc;
        const GA_ROHandleF *src;
        const float *coeff;
        bool useconstant;
        float constant;
        float coeffpost;
        MIX_NAMES mixtype;
        int comp;
    };


    THREADED_METHOD2(GAS_LinearCombination, range.canMultiThread(),
                    linearCombineAttr,
                    const GA_SplittableRange &, range,
                    linearCombineAttrParms &, parms);
    void     linearCombineAttrPartial(const GA_SplittableRange &range,
                                      linearCombineAttrParms &parms,
                                      const UT_JobInfo &info);

    bool     linearCombineCL(SIM_Object *obj,
                             SIM_RawField *dst,
                             int numsrc,
                             const SIM_RawField **src,
                             float *coeff,
                             bool useconstant,
                             float constant,
                             float coeffpost,
                             MIX_NAMES mixtype,
                             int axis);

    bool    linearCombineField(SIM_RawField* dst, SIM_Object *obj,
                               SIM_Time timestep, int a1, int a2);

    bool    linearCombineAttribute(GA_Detail *gdp,
				   const GA_PointGroup *ptgrp,
                                   GA_RWHandleF &dest_h,
                                   SIM_Object *obj,
                                   SIM_Time timestep, int comp);

    /// We define this to be a DOP_Auto node which means we do not
    /// need to implement a DOP_Node derivative for this data.  Instead,
    /// this description is used to define the interface.
    static const SIM_DopDescription *getDopDescription();

    /// These macros are necessary to bind our node to the factory and
    /// ensure useful constants like BaseClass are defined.
    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_LinearCombination,
                        GAS_SubSolver,
                        "Gas Linear Combination",
                        getDopDescription());
};


#endif

