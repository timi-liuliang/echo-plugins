/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_AdjustElasticity.h ( GAS Library, C++)
 *
 * COMMENTS:	This node is intended to simulate elastic forces
 *		within a particle-based fluid using a simple
 *		spring setup.  This is similar to the technique
 *		described in "Particle-based Viscoelastic Fluid
 *		Simultion" (see paper repository).  The purpose of
 *		this particular node is to dynamically adjust the
 *		lengths of springs between fluid particles.
 *
 */

#ifndef __GAS_AdjustElasticity__
#define __GAS_AdjustElasticity__

#include "GAS_API.h"

#include "GAS_SubSolver.h"
#include "GAS_Utils.h"
#include "GAS_SPH.h"

class GU_Detail;
class GAS_SPH;

#include <UT/UT_ThreadedAlgorithm.h>

class GAS_API GAS_AdjustElasticity : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_GEOMETRY,	GeometryName);
    GET_DATA_FUNC_B("doelasticity",	DoElasticity);
    GET_DATA_FUNC_F("plasticityconst",	PlasticityConstant);
    GET_DATA_FUNC_F("yieldratio",	YieldRatio);
    GET_DATA_FUNC_B("clampsprings",	ClampNumSprings);
    GET_DATA_FUNC_I("maxsprings",	MaxSprings);
    GET_DATA_FUNC_B("useid",		UseID);

    bool		 shouldMultiThread() { return true; }

protected:
    explicit		 GAS_AdjustElasticity(const SIM_DataFactory *factory);
    virtual		~GAS_AdjustElasticity();

    /// Adds and removes springs between particles as necessary,
    /// and also dynamically adjusts the lengths of springs
    /// between particles.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				    SIM_Object *obj,
				    SIM_Time time,
				    SIM_Time timestep);

    THREADED_METHOD5(GAS_AdjustElasticity, shouldMultiThread(),
				    adjustSprings, GU_Detail *, gdp,
				    const GAS_SPH &, sph,
				    fpreal, yieldratio,
				    fpreal, plasticityconstant,
				    SIM_Time, timestep)
    void		 adjustSpringsPartial(GU_Detail *gdp,
				    const GAS_SPH &sph,
				    fpreal yieldratio,
				    fpreal plasticity,
				    SIM_Time timestep,
				    const UT_JobInfo &info);

private:
    bool		 myUseID;

    /// A similar function to the one above, but this writes
    /// the spring information in to attribute handles.
    void		 setSprings(GA_Offset ptoff,
                                    const GA_RWHandleR &lengthattrib,
				    const GA_RWHandleI &indexattrib,
				    const UT_FloatArray &lengthlist,
				    const UT_IntArray &idxlist,
				    fpreal h, int initiallength);


    /// A similar function to the one above that reads the
    /// information from attribute handles.  We assume that
    /// the point has already been set in the handle.
    void		 getSprings(GA_Offset ptoff,
                                    const GA_ROHandleR &lengthattrib,
				    const GA_ROHandleI &indexattrib,
				    UT_FloatArray &weightmap,
				    UT_IntArray &indices,
				    const UT_Map<int,int> &weighthash);

    /// Adjusts the size of the spring attributes, if necessary.
    void		 resizeSpringAttributes(GU_Detail *gdp, GAS_SPH &sph);

    /// Queries the sph structure at each point to determine the
    /// number of entries required in the spring attributes.
    int			 getAttribSize(GU_Detail *gdp, GAS_SPH &sph);

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

    /// Reduce the given point list to the given maximum size.
    /// The points closest to the reference point will be kept.
    void		 sortAndCullPoints(const GA_Detail &detail,
                                GA_Offset ptoff,
				GAS_SPH::gas_PointList &ptlist,
				int newmax) const;

    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_AdjustElasticity,
			GAS_SubSolver,
			"Gas Adjust Elasticity",
			getDopDescription());
};

#endif
