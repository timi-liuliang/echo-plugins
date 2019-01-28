/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_PhysicalParms_h__
#define __SIM_PhysicalParms_h__

#include "SIM_API.h"
#include "SIM_Data.h"
#include "SIM_DataUtils.h"
#include "SIM_OptionsUser.h"

class SIM_Object;
class SIM_Solver;
class GEO_PointTree;
class SIM_PointTree;
class SIM_Geometry;
class ut_KDPQueue;
class SIM_RawField;
class SIM_VectorField;

#include <GEO/GEO_AttributeHandle.h>
#include <GU/GU_Detail.h>
#include <GU/GU_DetailHandle.h>


/// This enumeration lists all the properties that can be accessed using
/// the getPropertyAtPosition() and getPropertyAtPoint() functions.
typedef enum {
    SIM_PROPERTY_FRICTION = 0,
    SIM_PROPERTY_DYNAMICFRICTION,
    SIM_PROPERTY_BOUNCE,
    SIM_PROPERTY_TEMPERATURE,
    SIM_PROPERTY_BOUNCEFORWARD,
    // If you add any, you have to update Gas Calculate which 
    // hard codes PROPERTY COUNT
    SIM_PROPERTY_COUNT		// Tracks number of properties.
} SIM_Property;

/// This simple verb class is designed to create a fast and
/// usually threadsafe way to lookup lots of property value queries.
/// For threadsafety it is important you act on a resolver local
/// to your thread, you can create such a thing with the copy() function.
class SIM_API SIM_PropertyResolver
{
public:
	     SIM_PropertyResolver() {}
    virtual ~SIM_PropertyResolver() {}

    /// Duplicates this.
    virtual SIM_PropertyResolver *copy() const = 0;

    virtual fpreal	getF(const UT_Vector3 &pos) = 0;
    virtual UT_Vector3	getV3(const UT_Vector3 &pos) = 0;

    /// Sometimes despite our best efforts the lookup isn't threadsafe.
    virtual bool	threadsafe() const { return true; }
};

class SIM_API SIM_PropertyResolverConstant : public SIM_PropertyResolver
{
public:
	    SIM_PropertyResolverConstant(fpreal v) { myValue = v; }
	    SIM_PropertyResolverConstant(UT_Vector3 v) { myValue = v; }

    virtual SIM_PropertyResolver *copy() const
    {
	return new SIM_PropertyResolverConstant(*this);
    }
    
    virtual fpreal getF(const UT_Vector3 &pos) { return myValue.x(); }
    virtual UT_Vector3 getV3(const UT_Vector3 &pos) { return myValue; }
protected:
    UT_Vector3		myValue;
};

class SIM_API SIM_PropertyResolverNearestPoint : public SIM_PropertyResolver
{
public:
	    SIM_PropertyResolverNearestPoint(const SIM_PointTree *pttree,
					    const SIM_Geometry *geo,
					    const char *attribname,
					    const UT_DMatrix4 &worldtogdp);
    virtual ~SIM_PropertyResolverNearestPoint();

    virtual SIM_PropertyResolver *copy() const;
    
    virtual fpreal	getF(const UT_Vector3 &pos);
    virtual UT_Vector3	getV3(const UT_Vector3 &pos);
    GA_Offset		getPtOff(const UT_Vector3 &pos);
protected:

    GU_ConstDetailHandle	 myGdpHandle;
    const GU_Detail		*myGdp;
    GA_ROHandleF 	 	 myAttrib;
    GA_ROHandleV3 	 	 myAttribV3;
    UT_DMatrix4	  		 myWorldToGdp;
    GEO_PointTree		*myPointTree;
    ut_KDPQueue			*myQueue;
};

class SIM_API SIM_PropertyResolverVelNearestPoint : public SIM_PropertyResolver
{
public:
	    SIM_PropertyResolverVelNearestPoint(const SIM_Engine &engine, const SIM_Object *object);
    virtual ~SIM_PropertyResolverVelNearestPoint();

    virtual SIM_PropertyResolver *copy() const;
    
    virtual UT_Vector3	getV3(const UT_Vector3 &pos);
    virtual fpreal getF(const UT_Vector3 &pos) { return getV3(pos).x(); }
protected:

    GU_ConstDetailHandle	 myGdpHandle;
    const GU_Detail		*myGdp;
    GA_ROHandleV3		 myAttrib;
    UT_DMatrix4	  		 myWorldToGdp;
    GEO_PointTree		*myPointTree;
    ut_KDPQueue			*myQueue;

    UT_Vector3			 myPivot, myLinVel, myAngVel;
    UT_DMatrix3			 myVelToWorld;

    bool			 myHasOld;
    GU_ConstDetailHandle	 myOldGdh;
    const GU_Detail		*myOldGdp;
    UT_DMatrix4			 myNewGdpToGeo;
    UT_DMatrix4			 myOldGdpToGeo;
    UT_DMatrix3			 myGeoVelToWorld;
    fpreal			 myTimestep;
};

class SIM_API SIM_PropertyResolverField : public SIM_PropertyResolver
{
public:
	    SIM_PropertyResolverField(const SIM_RawField *field);
	    SIM_PropertyResolverField(const SIM_VectorField *field);

    virtual SIM_PropertyResolver *copy() const
    {
	return new SIM_PropertyResolverField(*this);
    }
    
    
    virtual fpreal	getF(const UT_Vector3 &pos);
    virtual UT_Vector3	getV3(const UT_Vector3 &pos);
protected:
    const SIM_RawField	*myField[3];
};

class SIM_API SIM_PropertyResolverMotion : public SIM_PropertyResolver
{
public:
	    SIM_PropertyResolverMotion(const UT_Vector3 &linvel,
				       const UT_Vector3 &pivot,
				       const UT_Vector3 &angvel);

    virtual SIM_PropertyResolver *copy() const
    {
	return new SIM_PropertyResolverMotion(*this);
    }
    
    virtual fpreal getF(const UT_Vector3 &pos) { return getV3(pos).x(); }
    virtual UT_Vector3 getV3(const UT_Vector3 &pos);

protected:
    UT_Vector3		myPivot, myLinVel, myAngVel;
};

class SIM_API SIM_PropertyResolverVelAtPosition : public SIM_PropertyResolver
{
public:
	    SIM_PropertyResolverVelAtPosition(const SIM_Object *obj,
					bool usesdfhistory,
					bool usegeohistory,
					fpreal integrateovertime)
	    {
		myObject = obj;
		myUseSDF = usesdfhistory;
		myUseGeo = usegeohistory;
		myIntegrate = integrateovertime;
	    }

    virtual SIM_PropertyResolver *copy() const
    {
	return new SIM_PropertyResolverVelAtPosition(*this);
    }
    
    virtual fpreal getF(const UT_Vector3 &pos) { return getV3(pos).x(); }
    virtual UT_Vector3 getV3(const UT_Vector3 &pos);

    virtual bool	threadsafe() const { return false; }
protected:
    const		SIM_Object *myObject;
    bool		myUseSDF, myUseGeo;
    fpreal		myIntegrate;
};

/// This simple verb class is designed to create a fast and
/// usually threadsafe way to lookup lots of property value queries.
/// For threadsafety it is important you act on a resolver local
/// to your thread, you can create such a thing with the copy() function.
class SIM_API SIM_PointImpulseMassMatrixResolver
{
public:
	     SIM_PointImpulseMassMatrixResolver(const SIM_Solver *solver,
						const SIM_Object *obj)
	     {
		 mySolver = solver;
		 myObject = obj;
	     }
    virtual ~SIM_PointImpulseMassMatrixResolver() {}

    virtual void get(GA_Index ptnum, UT_DMatrix3 &immatrix) const;

    /// The default implementation just calls
    /// SIM_Solver::getPointImpulseMassMatrix and is not threadsafe.
    virtual bool	threadsafe() const { return false; }

protected:
    const SIM_Object 		*myObject;
    const SIM_Solver 		*mySolver;
};


/// A simple, threadsafe mass matrix resolver that just looks up the mass point
/// attribute and returns 1/mass if exists else 1.
class SIM_API SIM_ParticleImpulseMassMatrixResolver:
				    public SIM_PointImpulseMassMatrixResolver
{
public:
	     SIM_ParticleImpulseMassMatrixResolver(const SIM_Solver *solver,
						      const SIM_Object *obj);
    virtual ~SIM_ParticleImpulseMassMatrixResolver();

    virtual void get(GA_Index ptnum, UT_DMatrix3 &immatrix) const;

    virtual bool	threadsafe() const { return true; }

protected:
    GU_ConstDetailHandle	 myGdpHandle;
    const GU_Detail		*myGdp;
    GA_ROHandleF		 myMassAttrib;
};

/// This class contains physical attributes that apply to all objects.
class SIM_API SIM_PhysicalParms : public SIM_Data,
				  public SIM_OptionsUser
{
public:
    /// Coefficient of friction.
    GETSET_DATA_FUNCS_F(SIM_NAME_FRICTION, Friction)
    /// Dynamic friction multiplier.
    GETSET_DATA_FUNCS_F(SIM_NAME_DYNAMICFRICTION, DynamicFriction)
    /// Bounce coefficient.
    GETSET_DATA_FUNCS_F(SIM_NAME_BOUNCE, Bounce)
    /// Temperature coefficient
    GETSET_DATA_FUNCS_F(SIM_NAME_TEMPERATURE, Temperature)
    /// Forward bounce coefficient
    GETSET_DATA_FUNCS_F(SIM_NAME_BOUNCEFORWARD, BounceForward)

    fpreal		 getProperty(const SIM_Property &property) const;

protected:
    explicit		 SIM_PhysicalParms(const SIM_DataFactory *factory);
    virtual		~SIM_PhysicalParms();

private:
    static const SIM_DopDescription	*getPhysicalParmsDopDescription();

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_DATAFACTORY(SIM_PhysicalParms,
			SIM_Data,
			"Physical Parameters",
			getPhysicalParmsDopDescription());
};

#endif
