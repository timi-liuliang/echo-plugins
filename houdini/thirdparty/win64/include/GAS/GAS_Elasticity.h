/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_Elasticity.h ( GAS Library, C++)
 *
 * COMMENTS:	This node is intended to simulate elastic forces
 *		within a particle-based fluid using a simple
 *		spring setup.  This is similar to the technique
 *		described in "Particle-based Viscoelastic Fluid
 *		Simultion" (see paper repository).
 *
 */

#ifndef __GAS_Elasticity__
#define __GAS_Elasticity__

#include "GAS_API.h"

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

#include <GA/GA_Handle.h>
#include <GA/GA_Types.h>

#include <UT/UT_Map.h>
#include <UT/UT_ThreadedAlgorithm.h>
#include <UT/UT_ValArray.h>

class GU_Detail;
class GAS_SPH;
class UT_String;

class GAS_API GAS_Elasticity : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_GEOMETRY,	GeometryName);
    GET_DATA_FUNC_F("elasticityconst",	ElasticityConstant);
    GET_DATA_FUNC_B("doelasticity",	DoElasticity);
    GET_DATA_FUNC_B("computev",		ComputeVelocity);
    GET_DATA_FUNC_B("useid",		UseID);

    bool		 shouldMultiThread() { return true; }

protected:
    explicit		 GAS_Elasticity(const SIM_DataFactory *factory);
    virtual		~GAS_Elasticity();

    /// Computes pairwise elastic forces between particles in the
    /// given particle field and either adjusts the velocity
    /// directly, or just adds to the force attribute.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				    SIM_Object *obj,
				    SIM_Time time,
				    SIM_Time timestep);

    THREADED_METHOD4(GAS_Elasticity, shouldMultiThread(), applySprings,
				GU_Detail *, gdp, 
				const GAS_SPH &, sph,
				fpreal, elasticityconst,
				SIM_Time, timestep)
    void		 applySpringsPartial(GU_Detail *gdp,
				const GAS_SPH &sph,
				fpreal elasticityconst, SIM_Time timestep,
				const UT_JobInfo &info);

private:
    bool		 myUseID;

    /// This function reads spring length information from the
    /// springlengths and springindices point attributes.
    void		 getSprings(GA_Offset ptoff,
                                    const GA_ROHandleR &lengthattrib,
				    const GA_ROHandleI &indexattrib,
				    UT_FloatArray &weightmap,
				    UT_IntArray &indices,
				    const UT_Map<int,int> &weighthash);

    /// Creates a float attribute of the given length to store
    /// spring lengths.
    void		 createLengthAttrib(GU_Detail *gdp, int size);

    /// Creates an int attribute of the given length to store
    /// spring indices.
    void		 createIndexAttrib(GU_Detail *gdp, int size);

    /// Grow these attributes to the given size.
    void		 growLengthAttrib(GU_Detail *gdp,
				    int initialsize, int finalsize);	
    void		 growIndexAttrib(GU_Detail *gdp,
				    int initialsize, int finalsize);	

    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_Elasticity,
			GAS_SubSolver,
			"Gas Elasticity",
			getDopDescription());

};

#endif
