/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __POP_ContextData_h__
#define __POP_ContextData_h__

#include "POP_API.h"
#include <SYS/SYS_Math.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_UniquePtr.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_Array.h>
#include <UT/UT_Map.h>
#include <UT/UT_SymbolTable.h>
#include <GEO/GEO_Point.h>
#include <GU/GU_DetailHandle.h>
#include <OP/OP_Context.h>
#include "POP_Guide.h"
#include "POP_ParticleList.h"

// ID where user variables start
#define POP_USER_VARIABLE               600
#define POP_TIMEDELTA			0.01
#define MAX_INPUT_GEOS			4

class GEO_PrimParticle;
class GU_Detail;
class POP_Node;
class POP_UserVar;
class OP_Network;
class CH_LocalVariable;
class PHYS_Constraint;
class POP_BlindData;
class POP_ContextDataHandle;
class pop_ContextDataHandleRef;

/// The POP_GuideHandleList provides indirect references to POP_Guide objects.
typedef UT_Array<GU_DetailHandle> POP_GuideHandleList;

typedef UT_SymbolMap<POP_UserVar*> POP_UserVarMap;
typedef UT_SymbolMap<int> POP_IdMap;
/// A POP_ContextData encapsulates and caches data necessary to cook a POP
/// network.  An instance is typically initialized by the entity that cooks
/// a POP network and passed from POP to POP as the cooking occurs.
///
/// The POP_ContextData contains the geometry detail to update during the
/// cook along with many cached attribute offsets.

class POP_API POP_ContextData : public OP_ContextData
{
public:
             POP_ContextData (const char *owner);
    virtual ~POP_ContextData (void);

    const char *	getName() const { return myName; }

    // These methods are mainly used by classes that cook POPs (e.g.
    // particle viewer, Pop Merge SOP, POP Network SOP).
    //________________________________________________________________________

    // copyFrom() copies to this POP_ContextData, the state of another
    // POP_ContextData. It will make a new copy of the geometry.
    void 		copyFrom (const POP_ContextData &data,
				  GU_Detail* gdp = NULL);

    /// cook() is used to perform cooking on the POP specified by myCookPOP.
    void		cook		(OP_Context& context);

    /// cookGuide() is like cook() except that it is used to recook the 
    /// guide geometry only.
    void		cookGuide	(OP_Context& context);

    /// reset() is used to completely reset the POP_ContextData. It should
    /// be called whenever a restart of the particle system is required.
    void		reset	 	(POP_Node *pop,
				         GU_Detail *gdp,
					 const char *init,
					 bool keepgeo,
					 unsigned seed,
					 bool solveimmediately);

    /// frameReset() is used to reset some information on a per frame 
    /// basis. It should be called before an attempt to cook a POP.
    /// However, once this is set, any number of calls to cook a POP can
    /// be performed assuming the underlying network does not change.
    void		frameReset	(void);

    /// offsetReset() resets the attribute offsets.
    void		offsetReset	(void);

    /// @{
    /// Access the context time
    void	setTime(fpreal time)		{ myTime = time; }
    fpreal	getTime() const			{ return myTime; }
    void	setTimeInc(fpreal tinc)		{ myTimeInc = tinc; }
    fpreal	getTimeInc() const		{ return myTimeInc; }
    /// @}

    /// recacheOffset() will reacquire the attribute offsets from the gdp.
    /// This method should be called if the offsets in the detail may have
    /// changed, say as that result of a merge operation.
    void		recacheOffset   (void);

    /// cacheMissingInternalOffsets() will acquire any attribute offsets from
    /// the gdp that it currently has noted as missing.
    /// @note This method does not affect user attribute offsets.
    void		cacheMissingInternalAttributes();

    // getDisplayDetail() returns the GU_Detail that corresponds to the
    // passed in myDisplayPOP.
    GU_Detail*		getDisplayDetail (void) { return(myDisplayGdp); }

    // Marks the my own gdp as the display gdp, useful if you are
    // setting up a POP_ContextData but haven't cooked it, so the cacheDisplay
    // hasn't run.
    void		setDisplayToMyGdp() { myDisplayGdp = myGdp; }

    /// @name Blind Data Management
    /// @{

    ///	Return the blind data associated with the POP.  If there is no data
    /// stored for the pop, a null ptr will be returned.
    POP_BlindData	*getBlindData(const POP_Node *pop) const;

    ///	Returns 1 if the data was added successfully to the context data.
    ///	At this point, the context assumes ownership of the data and may
    /// delete it from time to time.
    int			 addBlindData(POP_BlindData *data);

    ///	Returns the blind data for the pop and gives up	any responsiblity or
    /// ownership of the data.
    POP_BlindData	*stealBlindData(const POP_Node *pop);
    /// @}

    // These methods are mainly used by POPs themselves.
    //________________________________________________________________________

    /// random() returns a random number between 0-1. This should be used
    /// instead of SYSfastRandom() so that random numbers used within 
    /// POPs are completely deterministic.
    float		random		(void);
    /// This uses the same seed as random() but sends the result through
    /// a hash...  This is slower than random, but has the advantage
    /// that succeeding calls produce values that are "more" independent
    /// of each other.  Specifically, if you sample a cube, you'll get a 
    /// nice distribution
    float		randomSample	(void);

    unsigned		getRandomSeed() const { return myFastRandomSeed; }

    /// @name GEO_PrimParticle Management
    /// @{

    /// getPrimPart() returns the particle primitive generated by a 
    /// particular POP. If this particle doesn't exist already, it
    /// creates a new one.
    GEO_PrimParticle*	getPrimPart 	(POP_Node* node);

    /// hasPrimPart() returns whether the node already has a particle
    /// primitive assigned to it.
    GEO_PrimParticle*	hasPrimPart	(POP_Node* node);

    /// getGenerator() returns the POP_Node that created a particular
    /// particle primitive.
    POP_Node*		getGenerator 	(GEO_PrimParticle* part) const;
    /// @}

    /// getDetail() returns the GU_Detail being processed by this
    /// POP_ContextData.
    GU_Detail*		getDetail       (void) const { return(myGdp); }

    /// @name Event Management
    /// @{

    /// getEventAttr() returns the event attribute.
    GA_RWAttributeRef	getEventAttr	(void);

    /// addEvent() signals that an event has occurred.
    void		addEvent	(UT_String& events, float t);

    /// removeEvent() signals that an event is over.
    void		removeEvent	(UT_String& events);

    /// clearAllEvents() clears any currently active events.
    void		clearAllEvents  (void);
    /// @}

    // cacheDetail() will cache the current gdp.
    void		cacheDetail    (POP_Node* node);

    // addToParticleList() adds a new primitive to the stored particle
    // list. This particle list is used to keep track of all particle
    // primitives to be processed for timestepping.
    void		addToParticleList (GEO_PrimParticle* part, 
					   POP_Node* pop, int idx);

    /// Erases the particle lists. You will need to reset() to rebuild
    /// them, but this avoids having dangling pointers to primitives
    /// You must clear lists if you will be losing control of the gdp
    /// and someone else may delete primitives.
    void		clearParticleLists()
    { myParticleList.clear(); myInitParticleList.clear(); }

    /// @name POP_Cache Reference Counting
    /// Reference counting, used for caching in POP_Cache (through POPNET_Node)
    /// Note that when the count reaches 0, myGdp is deleted along with this, 
    /// because in the POP_Cache a new gdp is created for each context data.
    /// @{
    void		incRef()
			{
			    myRefCount++;
			}
    void		decRef()
			{
			    myRefCount--; 
			    if (!myRefCount)
			    {
				delete myGdp;
				delete this;
			    }
			}
    /// @}

    // This method is here for the use of POPNET_Node to ensure that its
    // local contextdata has a pointer to the detail if no other method
    // has yet been called to set it.
    void		setRefDetail(GU_Detail *gdp) { myGdp = gdp; }

    /// @name Cook Control Flags
    /// These flags control how the pop network will cook.  They globally
    /// turn off various update rules so it is possible to, for example,
    /// only perform the velocity update, or only perform the cooking.
    /// @{
    bool		shouldDoReap() const { return myDoReap; }
    void		setDoReap(bool v) { myDoReap = v; }
    bool		shouldDoPrep() const { return myDoPrep; }
    void		setDoPrep(bool v) { myDoPrep = v; }
    bool		shouldDoApplyRules() const { return myDoApplyRules; }
    void		setDoApplyRules(bool v) { myDoApplyRules = v; }
    bool		shouldDoCook() const { return myDoCook; }
    void		setDoCook(bool v) { myDoCook = v; }
    bool		shouldDoCookFirst() const { return myDoCookFirst; }
    void		setDoCookFirst(bool v) { myDoCookFirst = v; }
    /// @}

    /// Globally force the suppression of certain update rules when executing
    /// POP_Node::applyRulesToList().  This should be a mask (bitfield) of the
    /// relevant PART_STAT_SUPP* flags you want to suppress.
    int			getRuleSuppression() { return myRuleSuppression; }
    /// @copydoc getRuleSuppression()
    void		setRuleSuppression(int suppress) { myRuleSuppression = suppress; }

    // getParticleList() returns the stored particle list.
    POP_ParticleList*   getParticleList (void) { return(&myParticleList); };

    /// @name Query Attribute Offsets
    /// Methods to query cached offsets for common attributes to replace more
    /// expensive lookups.
    /// @{
#define BUILD_OFFSET_CACHE( ATTRIBNAME, TYPE ) 		\
public:							\
    const GA_RWAttributeRef &get##ATTRIBNAME##Offset       () const { return my##ATTRIBNAME##Offset; }					\
    const GA_RWHandle##TYPE &get##ATTRIBNAME##Handle       () const { return my##ATTRIBNAME##Handle; }					\
    void    set##ATTRIBNAME##Offset       (const GA_RWAttributeRef &offset) { my##ATTRIBNAME##Offset = offset; my##ATTRIBNAME##Handle.bind(offset.getAttribute()); }							\
    void    clear##ATTRIBNAME##Offset       () { my##ATTRIBNAME##Offset.clear(); my##ATTRIBNAME##Handle.clear(); }							\
private:						\
    GA_RWAttributeRef		my##ATTRIBNAME##Offset; \
    GA_RWHandle##TYPE		my##ATTRIBNAME##Handle; \
public:							\

    BUILD_OFFSET_CACHE(State, I)
    BUILD_OFFSET_CACHE(Life, F)
    BUILD_OFFSET_CACHE(Velocity, V3)
    BUILD_OFFSET_CACHE(Accel, V3)
    BUILD_OFFSET_CACHE(Backtrack, V4)
    BUILD_OFFSET_CACHE(Born, F)
    BUILD_OFFSET_CACHE(Source, I)
    BUILD_OFFSET_CACHE(NextID, I)
    BUILD_OFFSET_CACHE(Alpha, F)
    BUILD_OFFSET_CACHE(AngVel, V3)
    BUILD_OFFSET_CACHE(Attract, I)
    BUILD_OFFSET_CACHE(Bounce, F)
    BUILD_OFFSET_CACHE(Charge, F)
    BUILD_OFFSET_CACHE(Cling, F)
    BUILD_OFFSET_CACHE(Com, V3)
    BUILD_OFFSET_CACHE(Diffuse, V3)
    BUILD_OFFSET_CACHE(Distance, F)
    BUILD_OFFSET_CACHE(Drag, F)
    BUILD_OFFSET_CACHE(Follow, F)
    BUILD_OFFSET_CACHE(FDynamic, F)
    BUILD_OFFSET_CACHE(FStatic, F)
    BUILD_OFFSET_CACHE(Gen, I)
    BUILD_OFFSET_CACHE(Generator, S)
    BUILD_OFFSET_CACHE(HitDiffuse, V3)
    BUILD_OFFSET_CACHE(HitID, I)
    BUILD_OFFSET_CACHE(HitForce, F)
    BUILD_OFFSET_CACHE(HitNormal, V3)
    BUILD_OFFSET_CACHE(HitPos, V3)
    BUILD_OFFSET_CACHE(HitPosUV, V2)
    BUILD_OFFSET_CACHE(HitTime, F)
    BUILD_OFFSET_CACHE(HitUV, V2)
    BUILD_OFFSET_CACHE(ID, I)
    BUILD_OFFSET_CACHE(Instance, S)
    BUILD_OFFSET_CACHE(Mass, F)
    BUILD_OFFSET_CACHE(Nearest, I)
    BUILD_OFFSET_CACHE(NearestDist, F)
    BUILD_OFFSET_CACHE(Normal, V3)
    BUILD_OFFSET_CACHE(NumHit, I)
    BUILD_OFFSET_CACHE(NumProximity, I)
    BUILD_OFFSET_CACHE(OrbitAxis, V3)
    BUILD_OFFSET_CACHE(OrbitCenter, V3)
    BUILD_OFFSET_CACHE(Orbit, F)
    BUILD_OFFSET_CACHE(OrbitRadius, F)
    BUILD_OFFSET_CACHE(OrbitSpeed, F)
    BUILD_OFFSET_CACHE(Orient, Q)
    BUILD_OFFSET_CACHE(Origin, I)
    BUILD_OFFSET_CACHE(Parent, I)
    BUILD_OFFSET_CACHE(Pivot, V3)
    BUILD_OFFSET_CACHE(PosPath, S)
    BUILD_OFFSET_CACHE(PosPrim, I)
    BUILD_OFFSET_CACHE(PosUV, V2)
    BUILD_OFFSET_CACHE(PrevVel, V3)
    BUILD_OFFSET_CACHE(Rest, V3)
    BUILD_OFFSET_CACHE(Rot, Q)
    BUILD_OFFSET_CACHE(Scale, F)
    BUILD_OFFSET_CACHE(Scale3, V3)
    BUILD_OFFSET_CACHE(Speed, F)
    BUILD_OFFSET_CACHE(SpeedLimit, F)
    BUILD_OFFSET_CACHE(SpringK, F)
    BUILD_OFFSET_CACHE(SpriteRot, F)
    BUILD_OFFSET_CACHE(SpriteScale, V3)
    BUILD_OFFSET_CACHE(SpriteShop, S)
    BUILD_OFFSET_CACHE(SpriteUV, V2)
    BUILD_OFFSET_CACHE(Tension, F)
    BUILD_OFFSET_CACHE(Texture, V3)
    BUILD_OFFSET_CACHE(Trans, V3)
    BUILD_OFFSET_CACHE(Torque, V3)
    BUILD_OFFSET_CACHE(Up, V3)

    /// @}

    /// @name Cache Attribute Offsets
    /// Methods to set the cached offsets for common attributes which can
    /// later be queried to avoid more expensive lookups.
    /// @{
    // void    setAlphaOffset       (const GA_RWAttributeRef &offset) { myAlphaOffset = offset; }
    /// @}

    /// @name User Local Variables
    /// @{
    CH_LocalVariable		*getUserVariables (void) { return myUserVars; }
    const CH_LocalVariable	*getUserVariables(void) const
						    { return myUserVars; }


    GA_RWAttributeRef	addUserVariable(const char* name, int tuplesize, 
				  GA_StorageClass type,
                                  GA_Defaults def, const char* local = 0);

    int	    isUserVariable	 (int index) 
					{ return (index >= POP_USER_VARIABLE); }

    float   getUserVariableValue (GEO_PointP ppt, int index, int thread);

    uint64  getUserVariableSerial() const { return myUserVarSerial; }
    /// @}

    /// isGuideOnly() returns true when POP_Node::cookPop() implementations
    /// should only update the guide geometry.
    int		isGuideOnly	   (void) const { return myGuideOnly; }

    /// nextId() returns the integer ID to assign to the next birthed particle.
    /// @warning This method increments the internal state and so should only
    /// be called when initializing the "id" attribute of a newly birthed
    /// particle.
    int 	nextID	 	   (void);

    /// bumpNumParticles() updates the running count of particles in the
    /// network.
    void	bumpNumParticles   (int delta) { myNumParticles += delta; }

    /// allowBirth() returns whether a new particle can be born.
    int 	allowBirth	   (void);

    /// Query the path to the specified context geometry.
    /// @param[out] path    node path
    /// @param[in] index    [0 .. MAX_INPUT_GEOS-1]
    void		 getInputGeo(UT_String &path, int index) const;
    /// Set the path to the specified context geometry.
    /// @param[in] path     node path
    /// @param[in] index    [0 .. MAX_INPUT_GEOS-1]
    void		 setInputGeo(const UT_String &path, int index);

    /// The following method is used to append guide geometry during a cook.
    /// The specified GU_DetailHandle must be pointing to a POP_Guide.
    void		 appendGuide(POP_Node *, const GU_DetailHandle &);

    /// The following method is used to specify the instance guide geometry
    /// during a cook.  The specified GU_DetailHandle must be pointing to a
    /// POP_Guide.
    void		 setInstanceGuide(POP_Node *, const GU_DetailHandle &);

    // Methods to set or query whether the last cook (or cookGuide) call
    // changed the selection.
    bool		 getSelectionChanged() const
						{ return mySelectionChanged; }
    void		 setSelectionChanged()	{ mySelectionChanged = true; }

    // The following method is used mainly by expression functions.
    //________________________________________________________________________

    /// Query the context with respect to which expression functions are to be
    /// evaluated.
    static POP_ContextData	*getExpressionData(int thread);

    // Inputs
    //________________________________________________________________________

    fpreal	myTime;			// time 
    fpreal	myTimeInc;		// per frame time increment
    void*	myUserData;		// user data
    int		myRemoveUnused;		// remove unused points flag 
    OP_Network*	myXformObj;		// transform object
    POP_Node*	myCookPOP;		// POP to cook
    POP_Node*	myDisplayPOP;		// POP to display 
    int		myDoInfoButton;		// compute info button text
    int		myDoUpdateViewport;	// update viewport like guides.
    int		myMaxParticles;		// Maximum number of particles allowed.

    // Outputs
    //________________________________________________________________________

    // Rebuilt per cook.
    POP_GuideHandleList		myOPGuides;   // guide geometries to display
    UT_IntArray			myGuidePOPs;  // POPs that create guide geos

    UT_ValArray<GEO_PointP>	myInstancePoints;  
    GU_DetailHandle		myInstanceOPGuide; // handle to a POP_Guide
    int				myInstanceGuidePOP;

private:
    void		recacheUserOffset   (void);

    void		setupAttributes(GU_Detail *gdp);
    void		setupInitialState(GU_Detail *gdp);

    pop_ContextDataHandleRef	*getIndirectHandle();

    UT_String		myName;

    // Inputs
    int			myGuideOnly;
#ifdef INTEL_COMPILER
	// This is a way of avoiding an internal compiler error 
	// when using Intel's compiler. In what looks like a
	// compiler bug, it does not like an array of UT_Strings
	// in this particular file.
	std::vector<UT_String> myInputPaths;
#else	
    UT_String		myInputPaths[MAX_INPUT_GEOS];
#endif

    // Rebuilt per cook.
    UT_UniquePtr<GU_Detail> myCacheGdp;
    GU_Detail*		myDisplayGdp;

    // State
    unsigned		myFastRandomSeed;
    POP_IdMap		*myPrimParts;
    POP_IdMap		*myGenerators;
    POP_IdMap		*myInitPrimParts;
    GU_Detail*		myGdp;
    POP_ParticleList	myParticleList;
    POP_ParticleList	myInitParticleList;
    float		myPrevTimeInc;
    bool		mySolveImmediately;
    bool		mySelectionChanged;
    int			myNumParticles;

    // Flags controlling certain timestep behaviours.  These allow
    // us to globally turn off certain rules in the POP timestepping.
    bool		myDoPrep;
    bool		myDoReap;
    bool		myDoApplyRules;
    bool		myDoCook;
    bool		myDoCookFirst;

    // Global rule overrides.  This uses the PART_STATE_SUP* flags for
    // reference.
    int			myRuleSuppression;

    // user variables
    //________________________________________________________________________

    CH_LocalVariable*    getLocalVariable (const char* symbol);

    void		 resetUserVariables();
    // The blind data flag is:
    //	0 = bypass
    //	1 = reset handling
    //	2 = forced cleanup
    void		 resetBlindData(int forced);

    int                  	 myUserVarSize;
    int                  	 myUserVarEntries;
    CH_LocalVariable*    	 myUserVars;
    POP_UserVarMap  		*myUserVarSymbols;
    POP_UserVarMap		*myUserVarIDs;
    uint64               	 myUserVarSerial;
    UT_Map<int, POP_BlindData *> *myBlindData;

    int				 myRefCount;

    pop_ContextDataHandleRef	*myIndirectHandle;

    friend class POP_ContextDataHandle;
};

inline float
POP_ContextData::random (void)
{
    return SYSfastRandom(myFastRandomSeed);
}

inline float
POP_ContextData::randomSample (void)
{
    return SYSrandom(myFastRandomSeed);
}

/// A POP_ContextDataHandle is designed to allow arms length references to a
/// POP_ContextData.  The handle is notified upon deletion of the underlying
/// data buffering any code using the handle from potentially dereferencing
/// a deleted object.  This class does not provide any sort of access control
/// to the underlying POP_ContextData, nor does it bump the reference count
/// on the POP_ContextData.  In other words, an active handle to a particular
/// POP_ContextData instance will not prevent said instance from destruction.
class POP_API POP_ContextDataHandle
{
public:
     POP_ContextDataHandle();
     POP_ContextDataHandle(const POP_ContextDataHandle &handle);
    ~POP_ContextDataHandle();

    const POP_ContextDataHandle	&operator=(const POP_ContextDataHandle &handle);

    /// Return a pointer to the bound data if it is still valid, 0 otherwise.
    POP_ContextData		*getContextData() const;

    /// Clear reference to any bound POP_ContextData.  Equivalent to bind(0).
    void			 clear();

    /// Bind to the specified data, releasing any current binding.
    void			 bind(POP_ContextData *data);
private:
    void			 setHandleRef(pop_ContextDataHandleRef *handle);
    pop_ContextDataHandleRef	*myHandle;
};

/// A POP_UserVarResolveInfo caches information that can later be compared to
/// determine if local variable resolutions cached in compiled expressions may
/// no longer be valid.
class POP_API POP_UserVarResolveInfo
{
public:
    POP_UserVarResolveInfo() : mySerial(0) {}

    /// Update and return true if compiled expressions need to resolve local
    /// variables again.
    bool update(POP_ContextData *data);

private:
    POP_ContextDataHandle	 myContextData;
    uint64			 mySerial;
};

#endif
