/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GAS_ParticleForces.h ( GAS Library, C++)
 *
 * COMMENTS:	This node is intended to produce the force model
 *		for dealing with small grains that are represented
 *		by instanced spheres.
 *
 */

#ifndef __GAS_ParticleForces__
#define __GAS_ParticleForces__

#include "GAS_API.h"
#include "GAS_SubSolver.h"
#include "GAS_Utils.h"

#include <UT/UT_ThreadedAlgorithm.h>
#include <UT/UT_HashGrid.h>

#include <vector>

class GU_Detail;
class GAS_GrainObject;
class GAS_GrainSphere;

class GAS_API GAS_ParticleForces : public GAS_SubSolver
{
public:
    GET_DATA_FUNC_S(GAS_NAME_GEOMETRY, GeometryName);
    GET_DATA_FUNC_B("dopressure",   DoPressure);
    GET_DATA_FUNC_B("computev",	    ComputeVelocity);
    GET_DATA_FUNC_B("docollision",  DoCollisions);
    GET_DATA_FUNC_B("usesdfvelocity",  UseSDFVelocity);
    GET_DATA_FUNC_B("usegeovelocity",  UseGEOVelocity);

    // Model and Material params
    GET_DATA_FUNC_F("poissonratio", PoissonRatio);
    GET_DATA_FUNC_F("elasticrepulsion", ElasticRepulsion);
    GET_DATA_FUNC_F("repulsionstrength", RepulsionStrength);
    GET_DATA_FUNC_F("viscousdampening", ViscousDampening);
    GET_DATA_FUNC_F("dampeningstrength", DampeningStrength);
    GET_DATA_FUNC_F("contactresistance", ContactResistance);
    GET_DATA_FUNC_F("contactdampening", ContactDampening);
    GET_DATA_FUNC_F("rollingfriction", RollingFriction);
    GET_DATA_FUNC_F("objrollingfriction", ObjRollingFriction);
    GET_DATA_FUNC_F("objimpulseduration", ObjImpulseDuration );
    GET_DATA_FUNC_F("objtranslationimpulsefactor", ObjTranslationImpulseFactor );
    GET_DATA_FUNC_F("objrollingimpulsefactor", ObjRollingImpulseFactor );
    GET_DATA_FUNC_F("minbounce", MinBounce );

    bool		 shouldMultiThread() { return true; }

protected:
    explicit		 GAS_ParticleForces(const SIM_DataFactory *factory);
    virtual		~GAS_ParticleForces();

    /// Computes a pressure force for the given particle field
    /// and either adjusts the velocity directly, or just
    /// adds to the force variable.
    virtual bool	 solveGasSubclass(SIM_Engine &engine,
				    SIM_Object *obj,
				    SIM_Time time,
				    SIM_Time timestep);

    virtual bool	 postSolveGasSubclass(SIM_Engine &engine,
					      SIM_Object *obj,
					      SIM_Time time,
					      SIM_Time timestep);

protected:

    void		 calcForces(SIM_Engine &engine, SIM_Object *obj, GU_Detail *gdp,
				    const UT_Array<GAS_GrainObject> &grains,
				    SIM_Time time,
				    SIM_Time timestep);

    void		 updateContactDisplacement(SIM_Object *obj, GU_Detail *gdp,
						   const UT_Array<GAS_GrainObject> &grains,
						   SIM_Time timestep);

    /// Compute the repulsive force between two particles.
    /// p is the point of impact, nml the normalized normal of the impact
    /// and relvel the relative velocity at the point of impact.

    UT_Vector3		 computeRepulsiveForce(const UT_Vector3 &p, 
					       fpreal overlap,
					       const UT_Vector3 &nml,
					       const UT_Vector3 &relvel,
					       fpreal R, fpreal mass,
					       fpreal beta, fpreal kr, 
					       fpreal alpha, fpreal kd, 
					       fpreal nu, fpreal bounce,
					       fpreal mu_s,fpreal mu_d, 
					       fpreal dt ) const;

    UT_Vector3		 computeRepulsiveForcePP(const UT_Vector3 &p, 
						 fpreal overlap,
						 const UT_Vector3 &nml,
						 const UT_Vector3 &relvel,
						 fpreal R, fpreal pmass,
						 fpreal beta, fpreal kr, 
						 fpreal alpha, fpreal kd, 
						 fpreal nu, fpreal gamma,
						 fpreal bounce,
						 fpreal mu_s,fpreal mu_d,
						 fpreal kc, 
						 SIM_Time timestep, 
						 UT_Vector3 &shearD ) const;
    
    UT_Vector3  	 computeRepulsiveTorque(const UT_Vector3 &z, 
						const UT_Vector3 &c1,
						const UT_Vector3 &force,
						const UT_Vector3 &nml,
						const UT_Vector3 &angvel,
						fpreal pscale, fpreal pmass,
						fpreal mu_r ) const;

    UT_Vector3 getContactDisplacement(
        GA_Offset ptoff,
        const GA_ROHandleI &cg_gah,
        const GA_ROHandleI &csj_gah,
        const GA_ROHandleI &csk_gah,
        const GA_ROHandleF &cd_gah,
        int cs1, int cg2, int cs2 ) const;

    void                 setContactDisplacement(
        GA_Offset ptoff,
        const GA_RWHandleI &cg_gah,
        const GA_RWHandleI &csj_gah,
        const GA_RWHandleI &csk_gah,
        const GA_RWHandleF &cd_gah,
        std::vector<int> cg,
        std::vector<int> csj,
        std::vector<int> csk,
        std::vector<UT_Vector3> sd
        ) const;
    
    int                  adjustContactDisplacement( SIM_Object *obj, 
						    GU_Detail *gdp,
						    const UT_Array<GAS_GrainObject> &grains,
						    SIM_Time timestep );
    
    void                 resizeContactGrainAttributes( GU_Detail *gdp,
						       int initialsize,
						       int finalsize) const;
    
    void                 applyStiction( UT_Vector3 &totalforce,
					UT_Vector3 &totaltorque,
					const UT_Vector3 v1,
					const UT_Vector3 w1,
					const UT_Vector3 c1,
					const UT_Vector3 N,
					fpreal pscale, fpreal pmass,
					fpreal kv, fpreal kw,
					fpreal dt );
    
    
    void		 createGrainObjects(SIM_Object *obj, GU_Detail *gdp, UT_Array<GAS_GrainObject> &grains);

    UT_HashGrid<const GAS_GrainSphere *> *buildHashGrid(const UT_Array<GAS_GrainObject> &grains);

private:
    static const SIM_DopDescription	*getDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(GAS_ParticleForces,
			GAS_SubSolver,
			"Gas Particle Forces",
			getDopDescription());
};

#endif
