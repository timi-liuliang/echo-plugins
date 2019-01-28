/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_SolverEnable_h__
#define __SIM_SolverEnable_h__

#include "SIM_API.h"
#include "SIM_Solver.h"
#include "SIM_Query.h"
#include "SIM_QueryCombine.h"
#include "SIM_SolverEnableParms.proto.h"

class SIM_SolverEnableTestCache;

/// This solver class passes the solve responsibility to one or more other
/// solvers. It searches for any SIM_Solver subdata attached to itself, and
/// and calls the one that matches the object's swithch value.
class SIM_API SIM_SolverEnable : public SIM_Solver
{
public:
    SIM_SolverEnableParms	&param() { return myParms; }
    const SIM_SolverEnableParms	&param() const { return myParms; }

    void			 clearCachedSubdata() const
    { mySolverListCached = false; mySolverList.clear(); }

protected:

    class TestCache
    {
    public:
	void		update(const SIM_SolverEnableParms &parms);
	bool		test(const SIM_SolverEnableParms &parms, const SIM_RootData *object);
    protected:
	UT_StringHolder	myDataNamesCache, myAttribNamesCache;
	UT_StringArray	myParsedData, myParsedAttrib;
	SIM_ConstDataArray	myData;
    };

    /// Basic SIM_SolverEnable constructor.
    explicit		 SIM_SolverEnable(const SIM_DataFactory *factory);
    /// Basic SIM_SolverEnable destructor.
    virtual		~SIM_SolverEnable();

    /// Creates a SIM_QueryArrays object to treat impact as a record
    virtual SIM_Query	*createQueryObjectSubclass() const
    {
	return new SIM_QueryCombine(BaseClass::createQueryObjectSubclass(), param().createQueryObject(this));
    }
    virtual void makeEqualSubclass(const SIM_Data *source)
    {
	BaseClass::makeEqualSubclass(source);
	const SIM_SolverEnable *src = SIM_DATA_CASTCONST(source, SIM_SolverEnable);
	if (src)
	{
	    param() = src->param();
	}
    }
    virtual void	 saveSubclass(std::ostream &os) const
    {
	BaseClass::saveSubclass(os);
	myParms.save(os);
    } 
    virtual bool	 loadSubclass(UT_IStream &is)
    {
	if (!BaseClass::loadSubclass(is))
	    return false;
	if (!myParms.load(is))
	    return false;
	return true;
    }
    bool		 enableSolvers(const SIM_RootData *object) const;

    /// We are static if our switch is invalid or our input is.
    virtual bool	 isStaticSubclass(const SIM_Object *obj) const;

    /// Override this function to pass this call along to our child solvers.
    virtual SIM_Result	 solveObjectsSubclass(SIM_Engine &engine,
					SIM_ObjectArray &objects,
					SIM_ObjectArray &newobjects,
					SIM_ObjectArray &feedbacktoobjects,
					const SIM_Time &timestep);
    virtual SIM_Result	 solveRelationshipSubclass(SIM_Engine &engine,
					SIM_Relationship &relation,
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
				int ptnum, const SIM_Property &property) const;
    virtual void	 getDefaultColliderLabelSubclass(
				const SIM_Object &object,
				UT_String &label) const;
    virtual void	 getDefaultColliderSubclass(
				const SIM_Object &object,
				const UT_String &colliderlabel,
				UT_String &collidertype,
				bool &colliderreverseobjectroles) const;

private:
    static const SIM_DopDescription	*getSolverEnableDopDescription();

    SIM_Solver				*myCurrentSolver;
    SIM_SolverEnableParms		myParms;

    mutable bool				mySolverListCached;
    mutable SIM_SolverArray			mySolverList;
    mutable TestCache		myTestCache;

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_SolverEnable,
			SIM_Solver,
			"Enable Solver",
			getSolverEnableDopDescription());
};

#endif

