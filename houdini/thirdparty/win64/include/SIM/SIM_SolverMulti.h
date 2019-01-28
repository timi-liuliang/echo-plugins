/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_SolverMulti_h__
#define __SIM_SolverMulti_h__

#include "SIM_API.h"
#include "SIM_Solver.h"
#include "SIM_OptionsUser.h"

/// This solver class passes the solve responsibility to one or more other
/// solvers. It searches for any SIM_Solver subdata attached to itself, and
/// calls each one in turn.
class SIM_API SIM_SolverMulti : public SIM_Solver,
				public SIM_OptionsUser
{
public:
    GETSET_DATA_FUNCS_I(SIM_NAME_PRIMARYSOLVER, PrimarySolver);

protected:
    /// Basic SIM_SolverMulti constructor.
    explicit		 SIM_SolverMulti(const SIM_DataFactory *factory);
    /// Basic SIM_SolverMulti destructor.
    virtual		~SIM_SolverMulti();

    /// Override this function to pass this call along to our child solvers.
    virtual SIM_Result	 solveObjectsSubclass(SIM_Engine &engine,
					SIM_ObjectArray &objects,
					SIM_ObjectArray &newobjects,
					SIM_ObjectArray &feedbacktoobjects,
					const SIM_Time &timestep);
    virtual SIM_Result	 solveRelationshipSubclass(SIM_Engine &engine,
					SIM_Relationship &relationship,
					const SIM_Time &timestep);
    /// Override this function to get the requirements of our child solvers.
    virtual int		 getRequestedCacheSubclass() const;
    virtual void	 getImpulseMassMatrixSubclass(
				const SIM_Object &object,
				const UT_Vector3 &impulseworldpos,
				UT_DMatrix3 &immatrix) const;
    virtual void	 getPointImpulseMassMatrixSubclass(
				const SIM_Object &object,
				int ptnum, UT_DMatrix3 &immatrix) const;
    virtual SIM_PointImpulseMassMatrixResolver *getPointImpulseMassMatrixResolverSubclass(
				const SIM_Object &object) const;
    virtual fpreal	 getPropertyAtPositionSubclass(const SIM_Object &object,
				const UT_Vector3 &worldpos,
				const SIM_Property &property) const;
    virtual fpreal	 getPropertyAtPointSubclass(const SIM_Object &object,
				int ptnum,
				const SIM_Property &property) const;
    virtual void	 getDefaultColliderLabelSubclass(
				const SIM_Object &object,
				UT_String &label) const;
    virtual void	 getDefaultColliderSubclass(
				const SIM_Object &object,
				const UT_String &colliderlabel,
				UT_String &collidertype,
				bool &colliderreverseobjectroles) const;

private:
    /// Gets the solver to use for getting object properties.
    const SIM_Solver			*getCurrentSolver() const;

    static const SIM_DopDescription	*getSolverMultiDopDescription();

    SIM_Solver				*myCurrentSolver;

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_SolverMulti,
			SIM_Solver,
			"Multiple Solver",
			getSolverMultiDopDescription());
};

#endif

