/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU library (C++)
 *
 * COMMENTS:	Turbulence functions
 *		This function never deletes the data gdp.  Therefore, if
 *		the source changes, it's up to the caller to reset the data.
 *
 */

#ifndef __GU_Turbulence_h__
#define __GU_Turbulence_h__

#include "GU_API.h"
#include <UT/UT_Vector3.h>
#include <UT/UT_BitArray.h>
#include <GEO/GEO_Detail.h>
#include <GEO/GEO_PrimPart.h>	// Only for GEO_ParticleVertexIndex
#include "GU_Attractor.h"
#include "GU_Detail.h"

class GEO_PrimParticle;
class GU_RayIntersect;

enum GU_INTEGRATION_TYPE
{
    GU_EULER,
    GU_RUNGEKUTTA_2,		// 2nd order Runge-Kutta
    GU_RUNGEKUTTA_4		// 4th order Runge-Kutta
};

enum
{
    GU_LOOPPOINTREUSE = 0,
    GU_UNUSEDPOINTREUSE,
    GU_NOPOINTREUSE
};

class GU_API GU_TurbulenceFlags
{
public:
    GU_TurbulenceFlags() {
	euler = GU_EULER;
	singleAttractor = 0;
	modifySource = 0;
	birthAll = 0;
	destroyMode = 0;
	accurateMove = 0;
	jitterBirth = 0;
	addId = 0;
	splitting = 0;
	contact = 0;
	pointReuse = 0;
	revertFixed = 1;
    };
    
    unsigned	singleAttractor:1,
		modifySource:1,		// Modify source (no particles)
		birthAll:1,		// If we're modifying source, start all
					// at the first frame
		destroyMode:1,		// Destroy dead particles
		accurateMove:1,		// Accurate move mode
		jitterBirth:1,		// Jitter births across time step
		addId:1,		// Adds unique particle IDs
		splitting:2,		//   0 = no splitting
					//   1 = split on contact
					//   2 = split on death
		contact:2,		//   0 = particles die on contact
					//   1 = particles bounce on contact
					//   2 = particles stick on contact
		pointReuse:2,		//   0 = reuse points in a loop
					//   1 = use first unused point
					//   2 = don't reuse points
		revertFixed:1;		// Fixed points snap to source pos
    GU_INTEGRATION_TYPE		euler;	// Integration type
};


class GU_API GU_Turbulence
{
public:
	     GU_Turbulence();
    virtual ~GU_Turbulence();

//
//  The cook method will update the gdp to time "now"
    void	cook(float now, float time_step = 1./30.);

//
//  This method forces a reset of the system
    void	reset();

    void	 setGdp(GU_Detail *g) 		{ myGdp = g; 
						  initDetail(g);
						}
    void	 setSource(GU_Detail *gdp)	{ source = gdp;
						  initSource(source);
						}
    void	 setCollision(GU_Detail *gdp)	{ collision = gdp;
						  buildCollisionCache();
						  initCollision(collision);
						}
    void	 setStartTime(float t)		{ startTime = t; }
    void	 setPreroll(float t)		{ preroll = t; }
    void	 setLimits(const UT_Vector3 &neg, const UT_Vector3 &pos);
    void	 setGain(float n, float t)	{ gainNml = n; gainTan = t; }

    void	 setModifySource(int onOff)	{ flags.modifySource = onOff; }
    int		 getModifySource() const	{ return flags.modifySource; }
    void	 setPointReuse(int val)		{ flags.pointReuse = val; }
    int		 getPointReuse() const		{ return flags.pointReuse; }
    void	 setBirthAll(int onOff)		{ flags.birthAll = onOff; }
    int		 getBirthAll() const		{ return flags.birthAll; }
    void	 setRevertFixed(int onOff)	{ flags.revertFixed = onOff; }
    int		 getRevertFixed() const		{ return flags.revertFixed; }
//
//  Euler integration is faster than Runge-Kutta (euler = 0), however,
//	Runge-Kutta produces more realistic results (and is less likely
//	to explode).
    void		setEuler(int onOff)
			{
			    flags.euler = (onOff) ? GU_EULER : GU_RUNGEKUTTA_4;
			}
    void		setIntegration(GU_INTEGRATION_TYPE t)
			{
			    flags.euler = t;
			}
    int			getEuler(void) const	{ return flags.euler==GU_EULER;}
    GU_INTEGRATION_TYPE	getIntegration() const	{ return flags.euler; }

//
//  When dealing with particle systems, it's more efficient to leave
//	dead particles in the system as potential new births, however,
//	if the destroyMode is set, these particles are actually destroyed.
//	This is important if the number of points must equal the number of
//	particles.
    void	setDestroyMode(int onOff)	{ flags.destroyMode = onOff; }
    int		getDestroyMode() const		{ return flags.destroyMode; }

//
//  One problem is that if the source geometry is changing, as we cook
//	over multiple frames, the source geometry doesn't change.  If the
//	accuracy mode is set, then we call the protected update methods
//	to make sure the geometry is accurate for jumping frames.
//	This is potentially very expensive if the jitterBirth option is
//	set.
    void	setAccurateMove(int onOff)	{ flags.accurateMove = onOff; }
    int		getAccurateMove() const		{ return flags.accurateMove; }

//
//  Jittering the birth causes particles to be born at random times over the
//	time step.
    void	setJitterBirth(int onOff)	{ flags.jitterBirth = onOff; }
    int		getJitterBirth() const		{ return flags.jitterBirth; }

//
//  Single attractor causes each particle to be acted upon by only one
//  attractor.
    void	setSingleAttractor(int onOff)	
		    { flags.singleAttractor = onOff; }
    int		getSingleAttractor() const	
		    { return flags.singleAttractor; }

    void	setAddId(int onOff)		{ flags.addId = onOff; }
    int		getAddId() const		{ return flags.addId; }

    void	setSplitOff()			{ flags.splitting = 0; }
    void	setSplitContact()		{ flags.splitting = 1; }
    void	setSplitDeath()			{ flags.splitting = 2; }
    int		getSplitting() const		{ return flags.splitting; }

    void	setDieOnContact()		{ flags.contact = 0; }
    void	setBounceOnContact()		{ flags.contact = 1; }
    void	setStickOnContact()		{ flags.contact = 2; }
    int		getContact()			{ return flags.contact; }

    void	setFixedPoints(const GA_PointGroup *group);
    void	setInputGroup(const GA_PointGroup *group);

    void 	 setAttractor (GU_Detail* gdp)
			   { attractor.setAttractor(gdp); initAttractor(gdp); }

//
//
    void	initAttractor()
			   { attractor.initPointAttractorForce(); }

    void	cleanupAttractor()
			   { attractor.cleanPointAttractorForce(); }

// 
//  Get the force on a point/particle - feel free to call "getAttractorForce"
//  from within getForce()
    void	 getAttractorForce (GA_Offset ptoff, UT_Vector3 &force)
		    { attractor.getAttractorForce(myGdp->getPos3(ptoff), force,
			    getSingleAttractor(), myGdp->pointIndex(ptoff)); }
//
//  In case the sub-class needs to know anything about the attractor
    virtual void initAttractor (GU_Detail* ) {};

//
//  Calling this method will force a re-find of the system next cook
    void		 zeroSystem() { system = 0; }
    GEO_PrimParticle	*getParticles() { return system; }

    const GA_PointGroup	*getFixedPoints() { return fixedPoints; }

protected:
//
//  The following virtual methods are used to define the behaviour of the
//	system.  They are intended to be sub-classed by the user of this
//	class.

//
//  The following method is can be used to pre-evaluate stuff.  It is
//	called every time the time changes.
    virtual int		changeTime(float t);
//
//  When birthing particles, we need to know how long lived they are
    virtual float	getBirthRate(float t);
//
//  Get the life time for a particle.  Split will be 1 if the particle
//	is a split particle, 0 otherwise
    virtual float	getLifetime(float t, int split);
//
//  Number of particles to split into (on contact or death)
    virtual int		getNumSplit(float t);

//
//  Set the birth attributes for a particle.  This routine is responsible
//	for setting all attributes except for id and lifetime.  It is
//	very important to set the velocity (by default, it will be 0)
//	If the birth particle is a split particle, then the source particle
//	is the point passed, otherwise, it's the point from the source which
//	is passed.
    virtual void	setBirthAttributes(GA_Offset ptoff,
					   GA_Offset srcptoff,
					   bool split) = 0;

//
//  Get the mass of a point
    virtual float	getMass(float t, GA_Offset ptoff) = 0;

    virtual void	getForce(float t, GA_Offset ptoff,
					UT_Vector3 &force) = 0;

//
//  These methods are possibly called when the accurateMove flag is set
    virtual void	updateSource(float t);
    virtual void	updateCollision(float t);
    virtual void	updateAttractor(float t);

//
//  Initialize all the attributes for a GU_Detail - the base class method
//	should always be called (if subclassed)
    virtual void	initDetail(GU_Detail *gdp);

//
//  In case the sub-class needs to know anything about the source
    virtual void	initSource(GU_Detail *src);

//
//  In case the sub-class needs to do anything whenever a reset is done.
    virtual void	resetSource(GU_Detail *src);

//
//  In case the sub-class needs to know anything about the collision object
    virtual void	initCollision(GU_Detail *coll);

//  WARNING, calling this method may force a particle system to be added
    GEO_PrimParticle	*getSystem();

//  Just in case someone needs to be able to get at these...
    GU_Detail		*getMySource()	{ return source; }
    GU_Detail		*getMyGdp()	{ return myGdp; }

protected:
//  Here is the protected data
//  First, the attribute offsets which must exist for particle systems
    GA_RWHandleV3	 myVelocityHandle;	// Handle of velocity attribute
    GA_RWHandleV2	 myLifeHandle;		// Handle of time alive
    GA_RWHandleI	 myIDHandle;
    GA_RWHandleI	 myPStateHandle;	// Handle of the particle
						// state attribute

    UT_Vector3		 negLimit;		// Limit planes
    UT_Vector3		 posLimit;
    float		 gainNml, gainTan;

    UT_BitArray		 deadPoints;	// A bit array for modifying source
    UT_BitArray		 stuckPoints;	// A bit array of all stuck points
    const GA_PointGroup	*fixedPoints;

private:
    void		 cookTime(float t, float tinc);
    void		 updateBitFields();
    void		 changeVelocity(float t, float tinc, GA_Offset ptoff);
    void		 splitParticle(float t, GA_Offset vtxoff,
				GEO_PrimParticle *part);
    void		 killParticle(float t, GEO_ParticleVertexIndex vtxidx,
				GA_Offset vtxoff, GEO_PrimParticle *part);
    void		 birthParticle(GEO_PrimParticle *, float t, float tinc,
				GA_Offset splitpt = GA_INVALID_OFFSET);
    void		 killPoint(GA_Offset ptoff);
    int			 birthPoint(float t, float tinc);
    int			 movePoint(GU_Detail *gdp, GA_Offset ptoff, float tinc);

    // This method is called every time the collision gdp is set
    void		 buildCollisionCache();


    GU_Detail		*myGdp;
    GU_Detail		*source;
    GU_Detail		*collision;
    GU_RayIntersect	*rayIntersector;  // Collision object intersector
    GU_Attractor	attractor;

    GEO_PrimParticle	*system;

    const GA_PointGroup	*inputGroup;	// only give birth from the points here 

    GU_TurbulenceFlags	 flags;

    float		 lastCookTime;
    float		 startTime;
    float		 preroll;

    int			 idCount;	// For unique IDs
    GA_Iterator		 mySourcePoint;	// Source point to use for birthing
    float		 nborn;		// Residue of particles to birth
    unsigned		 mySeed;
};

#endif
