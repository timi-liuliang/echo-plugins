/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_SolverPop_h__
#define __SIM_SolverPop_h__

#include "SIMZ_API.h"
#include <SIM/SIM_OptionsUser.h>
#include <SIM/SIM_SingleSolver.h>
#include <POPNET/POPNET_Node.h>
#include "SIMZ_Utils.h"

class GU_Detail;
class SIM_Impacts;
class SIM_DataFilter;
class SIM_PopGeometry;
class POP_ContextData;

#define SIM_NAME_INITIALSTATE	PN_INITIALSTATE
#define SIM_NAME_RANDOMSEED	PN_RANDOMSEED
#define SIM_NAME_OVERSAMPLE	PN_OVERSAMPLE
#define SIM_NAME_MAXPARTICLES	PN_MAXPARTICLES
#define SIM_NAME_REMOVEUNUSED	PN_REMOVEUNUSED
#define SIM_NAME_PATH1		PN_PATH1
#define SIM_NAME_PATH2		PN_PATH2
#define SIM_NAME_PATH3		PN_PATH3
#define SIM_NAME_PATH4		PN_PATH4

#define SIM_NAME_COLLIDESTOP		"collidestop"
#define SIM_NAME_COLLIDEKILL		"collidekill"
#define SIM_NAME_COLLIDECONTINUE	"collidecontinue"
#define SIM_NAME_COLLIDESTICK		"collidestick"
#define SIM_NAME_COLLIDEBOUNCE		"collidebounce"

// This class implemented a computational fluid dynamics solver.
class SIMZ_API SIM_SolverPop : public SIM_SingleSolver,
			       public SIM_OptionsUser
{
public:
    // Access functions for our specific data.
    GETSET_DATA_FUNCS_B("usepoppath", UsePopPath);
    GETSET_DATA_FUNCS_S(SIM_NAME_POPPATH, PopPath);
    GETSET_DATA_FUNCS_B(SIM_NAME_MAKEPOINTSPARTICLES, MakePointsParticles);
    GETSET_DATA_FUNCS_B(SIM_NAME_INHERITVELOCITY, InheritVelocity);
    GETSET_DATA_FUNCS_S(SIM_NAME_GENERATOR, Generator);
    GETSET_DATA_FUNCS_F(SIM_NAME_LIFESPAN, Lifespan);
    GETSET_DATA_FUNCS_S(SIM_NAME_COLLIDESTOP, CollideStop);
    GETSET_DATA_FUNCS_S(SIM_NAME_COLLIDEKILL, CollideKill);
    GETSET_DATA_FUNCS_S(SIM_NAME_COLLIDECONTINUE, CollideContinue);
    GETSET_DATA_FUNCS_S(SIM_NAME_COLLIDESTICK, CollideStick);
    GETSET_DATA_FUNCS_S(SIM_NAME_COLLIDEBOUNCE, CollideBounce);

    GETSET_DATA_FUNCS_I("rehome", RehomeParticles);
    GETSET_DATA_FUNCS_S("homepath", RehomeParticlePath);

    // Access functions for all out POPNET information.
    GETSET_DATA_FUNCS_S(SIM_NAME_INITIALSTATE, InitialState);
    GETSET_DATA_FUNCS_I(SIM_NAME_RANDOMSEED, RandomSeed);
    GETSET_DATA_FUNCS_I(SIM_NAME_OVERSAMPLE, OverSample);
    GETSET_DATA_FUNCS_I(SIM_NAME_MAXPARTICLES, MaxParticles);
    GETSET_DATA_FUNCS_B(SIM_NAME_REMOVEUNUSED, RemoveUnused);

    // SOP style paths
    GETSET_DATA_FUNCS_S(SIM_NAME_PATH1, Path1);
    GETSET_DATA_FUNCS_S(SIM_NAME_PATH2, Path2);
    GETSET_DATA_FUNCS_S(SIM_NAME_PATH3, Path3);
    GETSET_DATA_FUNCS_S(SIM_NAME_PATH4, Path4);

    // DOP style paths
    GETSET_DATA_FUNCS_S("datapath1", DataPath1);
    GETSET_DATA_FUNCS_S("datapath2", DataPath2);
    GETSET_DATA_FUNCS_S("datapath3", DataPath3);
    GETSET_DATA_FUNCS_S("datapath4", DataPath4);

    // Access our general solver information
    GETSET_DATA_FUNCS_S("geo", GeometryName);
    GETSET_DATA_FUNCS_I("solvemode", SolveMode);

    GETSET_DATA_FUNCS_B("doreapparticles", DoReapParticles);
    GETSET_DATA_FUNCS_B("doapplyrules", DoApplyRules);
    GETSET_DATA_FUNCS_B("suppresspos", SuppressPos);
    GETSET_DATA_FUNCS_B("suppressvel", SuppressVel);
    GETSET_DATA_FUNCS_B("suppressup", SuppressUp);
    GETSET_DATA_FUNCS_B("suppressage", SuppressAge);
    GETSET_DATA_FUNCS_B("suppressrot", SuppressRot);
    GETSET_DATA_FUNCS_B("suppressangvel", SuppressAngVel);
    GETSET_DATA_FUNCS_B("doprepparticles", DoPrepParticles);
    GETSET_DATA_FUNCS_B("docookpop", DoCookPOP);
    GETSET_DATA_FUNCS_B("docookfirst", DoCookFirst);
    GETSET_DATA_FUNCS_B("doimpacts", DoImpacts);
    GETSET_DATA_FUNCS_B("dodopforces", DoDopForces);
    GETSET_DATA_FUNCS_B("convertacceltoforce", ConvertAccelToForce);

protected:
    explicit		 SIM_SolverPop(const SIM_DataFactory *factory);
    virtual		~SIM_SolverPop();

    virtual SIM_Result	 solveSingleObjectSubclass(SIM_Engine &engine,
					SIM_Object &object,
					SIM_ObjectArray &feedbacktoobjects,
					const SIM_Time &timestep,
					bool newobject);
    virtual int		 getRequestedCacheSubclass() const;
    virtual void	 getImpulseMassMatrixSubclass(
				const SIM_Object &object,
				const UT_Vector3 &impulseworldpos,
				UT_DMatrix3 &immatrix) const;
    virtual void	 getPointImpulseMassMatrixSubclass(
				const SIM_Object &object,
				int ptnum, UT_DMatrix3 &immatrix) const;
    virtual void	 getDefaultColliderLabelSubclass(
				const SIM_Object &object,
				UT_String &label) const;
    virtual void	 getDefaultColliderSubclass(
				const SIM_Object &object,
				const UT_String &colliderlabel,
				UT_String &collidertype,
				bool &colliderreverseobjectroles) const;

    virtual SIM_Guide	*createGuideObjectSubclass() const;
    virtual void	 buildGuideGeometrySubclass(const SIM_RootData &root,
						    const SIM_Options &options,
						    const GU_DetailHandle &gdh,
						    UT_DMatrix4 *xform,
						    const SIM_Time &t) const;

private:
    SIM_PopGeometry			*getGeometry(SIM_Object &obj,
						UT_StringArray &oldhomes);
    static const SIM_DopDescription	*getSolverPopDopDescription();

    void		 applySoftConstraintForces(SIM_Object &object,
					POP_ContextData &contextdata) const;
    void		 applyHardConstraintPositions(SIM_Object &object,
					POP_ContextData &contextdata,
					const UT_DMatrix4 &ixform) const;
    void 		 applyShapeMatchForces(SIM_Object &object,
				        POP_ContextData &contextdata,
				        SIM_Time timeinc) const;

    void		 respondToCollision(SIM_Engine &engine,
					SIM_Object &object,
					SIM_PopGeometry &popgeo,
					const SIM_ObjectArray &affectors,
					const UT_DMatrix4 &xform,
					const UT_DMatrix4 &ixform,
					const SIM_Impacts *impacts,
					int impactidx,
					int impactflags,
					GA_Index ptnum,
					bool hasidattribute) const;
    bool		 respondToCollisions(SIM_Engine &engine,
					SIM_Object &object,
					SIM_PopGeometry &popgeo,
					const SIM_ObjectArray &affectors,
					const UT_DMatrix4 &xform,
					const UT_DMatrix4 &ixform,
					const UT_DMatrix4 &posixform,
					const char *impactdataname,
					const SIM_DataFilter &fstop,
					const SIM_DataFilter &fkill,
					const SIM_DataFilter &fcont,
					const SIM_DataFilter &fstick,
					const SIM_DataFilter &fbounce,
					bool doinglastpopsubstep,
					bool checkforduplicates) const;

    // This function ensures that the geometry has the attributes
    // that are mandatory for POPs.
    void		 createRequiredAttributes(GU_Detail *gdp) const;

    // This function ensures that any points in the geometry are part
    // or at least one particle system.
    void		 makePointsParticles(GU_Detail *gdp,
					const char *generator,
					fpreal lifespan) const;
    // This function takes all particle primitives with the same generator
    // attribute value and combines them into a single primitive. This is
    // necessary when combining geometries to ensure that all particles
    // that should be processed by a particular POP node are all processed
    // by that node.
    void		 combineParticlePrimitives(GU_Detail &gdp);

    // Rehomes all of the particle primitive generators to our
    // home settings.  Invokes combineParticlePrimtives if any
    // rehoming occured.
    void		 rehomeParticles(GU_Detail *gdp,
				    UT_StringArray &oldhomes) const;
    void		 restoreHomes(GU_Detail *gdp,
				    const UT_StringArray &oldhomes) const;

    // Converts the accel attribute into a force attribute,
    // multiplying by either mass or density depending on what
    // is present.
    void		 convertAccelToForce(GU_Detail *gdp) const;

    // These functions respond to impact information attached to the
    // particles.
    void		 changeParticleStateFromImpacts(
					POP_ContextData &contextdata,
					const SIM_Impacts *impacts,
					int impactidx,
					int statechange,
					GA_Index ptnum) const;
    void		 createConstraintsFromImpacts(
					SIM_Engine &engine,
					SIM_Object &object,
					const SIM_ObjectArray &affectors,
					const SIM_Impacts *impacts,
					int impactidx,
					GA_Index ptnum,
					bool hasidattribute) const;
    void		 bounceParticlesFromImpacts(
					POP_ContextData &contextdata,
					const UT_DMatrix4 &xform,
					const UT_DMatrix4 &ixform,
					const SIM_Impacts *impacts,
					int impactidx,
					GA_Index ptnum) const;

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_SolverPop,
			SIM_SingleSolver,
			"POP Solver",
			getSolverPopDopDescription());
};

#endif

