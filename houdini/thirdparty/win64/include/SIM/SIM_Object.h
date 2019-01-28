/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_Object_h__
#define __SIM_Object_h__

#include "SIM_API.h"
#include "SIM_RootData.h"
#include "SIM_PhysicalParms.h"
#include "SIM_RelationshipArray.h"
#include "SIM_ColliderInfo.h"

class UT_IStream;
class UT_TokenString;
class SIM_Collider;
class SIM_ColliderLabel;
class SIM_Geometry;
class SIM_Position;
class SIM_ObjectSolveInfo;
class SIM_Solver;
class SIM_MetaObject;
class SIM_ObjectArray;
class SIM_ConstObjectArray;
class SIM_SimulationState;

/// This class is a simple container for all the data that is used to
/// represent an object in a simulation. Each piece of data has a name
/// and can be of any type that is subclassed from SIM_Data.
class SIM_API SIM_Object : public SIM_RootData
{
public:
    /// Returns the unique identifier for the object. This identifier is
    /// unique for the duration of a simulation and will not be reused even
    /// if the object is deleted.
    int			 getObjectId() const;
    /// Sets the name ofthe object to a new value. This value does not need
    /// to be unique. If the value is not a valid object name (e.g. it
    /// contains spaces or other invalid characters), this function modifies
    /// the name to make it valid.
    void		 setName(const char *name);
    /// Returns true if the object's baked flag is set. This flag indicates
    /// that the object was loaded from a file or merged from another
    /// simulation.
    bool		 getIsBaked() const;
    /// Returns true if the object allows cachine.  Uncached objects
    /// will not persist in the cache.
    bool		 getAllowCaching() const;
    void		 setAllowCaching(bool allowcaching);
    /// Returns true if the object was created with the solve first frame
    /// flag turned on.
    bool		 getSolveFirstFrame() const;
    /// Returns the affector information for this object. The returned
    /// array contains a list of all objects that affect this object.
    void		 getAffectors(SIM_ObjectArray &affobjs,
				const char *relationshiptype);
    /// Returns the affector information for this object. The returned
    /// array contains a const list of all objects that affect this object.
    void		 getConstAffectors(SIM_ConstObjectArray &affobjs,
				const char *relationshiptype) const;
    /// Returns true if the supplied object is an affector of this object.
    /// Checking this way is faster than building a full affector list and
    /// comparing against that.
    bool		 getIsAffector(const SIM_Object *affector,
				const char *relationshiptype) const;
    /// Gets an array of objects we will collide with, what collider to
    /// use, and how to apply impact data to those objects.
    void		 getColliderInfo(
                                SIM_ColliderInfoArray &colliderinfo) const;
    /// Fills a UT_TokenString with enough information to uniquely determine
    /// what the SIM_ColliderInfoArray for this object will contain. This lets
    /// use very quickly determine if two objects will have the same array
    /// of collider infos.
    void		 getColliderInfoTokenString(UT_TokenString &ts) const;

    /// This enumeration defines the possible solve states for an object.
    typedef enum {
	SIM_UNSOLVED,
	SIM_SOLVING,
	SIM_FEEDBACK,
	SIM_SOLVED
    } SIM_SolveState;
    /// Returns the current solved state of this object.
    const SIM_SolveState	&getSolveState() const;

    /// Returns our cached SIM_Solver pointer. This function is equivalent to
    /// but much faster than using the SIM_DATA_GETCONST() macro.
    const SIM_Solver		*getSolver() const;
    /// Returns our cached SIM_Position pointer. This function is equivalent to
    /// but much faster than using the SIM_DATA_GETCONST() macro.
    const SIM_Position		*getPosition() const;
    /// Returns our cached SIM_Geometry pointer. This function is equivalent to
    /// but much faster than using the SIM_DATA_GETCONST() macro.
    const SIM_Geometry		*getGeometry() const;
    /// Returns our cached SIM_PhysicalParms pointer. This function is
    /// equivalent to but much faster than using the SIM_DATA_GETCONST() macro.
    const SIM_PhysicalParms	*getPhysicalParms() const;
    /// Returns our cached SIM_ColliderLabel pointer. This function is
    /// equivalent to but much faster than using the SIM_DATA_GETCONST() macro.
    const SIM_ColliderLabel	*getColliderLabel() const;
    /// Gets the actual collider label string from our collider label subdata.
    /// If we have no collider label subdata, our solver is queried to get
    /// the default collider label for an object handled by that solver.
    void			 getColliderLabel(UT_String &label) const;

    /// Get a const reference to our array of relationships. Note that even
    /// though the returned array is const, it's members are non-const, and
    /// so the SIM_Relationship data can be modified. Thus this must be a
    /// non-const function on SIM_Object.
    const SIM_RelationshipArray	&getRelationships(bool ingroup);
    /// Returns the number of relationships to which this object belongs.
    int				 getNumRelationships(bool ingroup) const;
    /// This function allows looping through the relationships for an object.
    /// This is a const function, so it returns a const SIM_Relationship.
    const SIM_Relationship	*getRelationship(bool ingroup, int index) const;
    /// Get the relationship for this object with a particular name. Because
    /// This is a const function, so it returns a const SIM_Relationship.
    const SIM_Relationship	*getRelationship(bool ingroup,
					const char *name) const;
    /// Gets all the relationships of a particular type. The type of a
    /// relationship is defined by the one subdata on the SIM_Relationship.
    void			 filterConstRelationships(bool ingroup,
					const SIM_DataFilter &filter,
					SIM_ConstDataArray &rels) const;

    /// Returns whether or not this object is a substep object. This is
    /// true for both temporary interpolated objects created with
    /// SIM_Engine::getObjectAtTime() and substep objects explicitly
    /// created with SIM_Engine::createSubStepObject().
    bool			 getIsSubStepObject() const;
    /// Returns false if the object is either a baked object (because it
    /// was loaded from a file) or it is passive.
    bool			 getIsStatic() const;
    /// Returns true if the object is marked as intangible, ie, should
    /// ignore all auto-relationships from merge nodes and only
    /// deal with explicit merges.
    bool			 getIsIntangible() const;

    /// Gets the meta-object pointer for this object. The meta-object is
    /// a value that is used only by the SIM_Engine.
    SIM_MetaObject		*getMetaObject() const;

    /// Returns a pointer to a SIM_Collider that should be used to do
    /// collision detection between this object and the supplied affector
    /// object. This function should be called only if the SIM_ColliderInfo
    /// for this affector object did not specify a SIM_Collider to use.
    /// The return value from this function can be null, in which case no
    /// collision detection should be done between the objects.
    ///
    /// The method for determining the SIM_Collider to use is as follows:
    /// First, we look at the collider label for the affector, and look
    /// for SIM_Collider subdata on the SIM_ColliderLabel of this object
    /// with a name that matches the affector's collider label. If a
    /// SIM_Collider is found with that subdata name, we return that
    /// collider. Otherwise, we go to the solver attached to this object.
    /// The SIM_Solver::getDefaultCollider() function is called, passing
    /// in this object, the affector object's collider label. This function
    /// returns the name of the SIM_Collider subclass we should use based
    /// on the needs of the solver. We then create subdata on the solver
    /// itself, and attach a SIM_Collider of the requested type. Then
    /// we return a pointer to this new collider.
    ///
    /// For the special case of self-collision, the collider label of
    /// the affector is replaced with SIM_COLLIDERLABEL_SELF, which allows
    /// the solver and the collider label data to treat self-collision as
    /// a special case. In some cases this means using a spcial SIM_Collider,
    /// and in other cases it means returning null and skipping collision
    /// detection altogether (e.g. the RBD Solver doesn't do self-collision
    /// detection).
    const SIM_Collider		*getCollider(const SIM_Object &affector,
					const char *defaultlabel=0) const;
    /// Stores impacts information from the temporary locations to the
    /// permanent locations. Copies impact information from the
    /// SIM_NEWIMPACTS_DATANAME data into SIM_IMPACTS_DATANAME, and
    /// from SIM_NEWFEEDBACK_DATANAME to SIM_FEEDBACK_DATANAME. Then
    /// the NEW impact data is removed. This should be called after
    /// a set of new impacts have been processed successfully.
    void			 preserveImpacts(SIM_Engine &engine);
    /// Eliminates any temporary impact data. Temporary impacts data is
    /// stored in SIM_Impacts data named SIM_NEWIMPACTS_DATANAME and
    /// SIM_NEWFEEDBACK_DATANAME. Unlike preserveImpacts(), which copies
    /// the data into SIM_IMPACTS_DATANAME and SIM_FEEDBACK_DATANAME,
    /// this function simply deletes the data. This funciton should be
    /// called when rolling back a substep.
    void			 rollbackImpacts(SIM_Engine &engine);

    /// Gets the velocity of a point in the space local to an object. The
    /// returned velocity is also in local space. There are two methods
    /// that can be used to calculate the velocity. The first method uses
    /// the SDF representing the geometry at the previous time step
    /// compared to the current time step. The second method uses the
    /// velocity point attribute attached to the geometry. If no velocity
    /// attribute is found, the geometry at this time step is compared to
    /// the geometry at the previous time step to calculate the velocity
    /// for each point.
    UT_Vector3			 getLocalVelocityAtPosition(
					const UT_Vector3 &localspacepos,
					bool usesdfhistory,
					bool usegeohistory) const;

    /// Gets the velocity of a point on the object specified in world space.
    /// The returned velocity is also in world space. The two boolean values
    /// specify what method should be used to determine the local space
    /// portion of the velocity. If both these values are false, no local
    /// velocity component is calculated and it is assumed that the geometry
    /// attached to the object is static.
    UT_Vector3			 getVelocityAtPosition(
					const UT_Vector3 &worldspacepos,
					bool usesdfhistory,
					bool usegeohistory,
					fpreal integrateovertime = 0.0) const;

    /// Returns a resolver for efficiently computing velocity values
    /// The caller must delete the returned object.
    SIM_PropertyResolver	*getVelocityAtPositionResolver(
					bool usesdfhistory, 
					bool usegeohistory, 
					fpreal integrateovertime = 0.0) const;

    /// Gets the value of some physical property for the supplied object
    /// at a given position in world space. This function calls
    /// SIM_Solver::getPropertyAtPosition().
    fpreal			 getPropertyAtPosition(const UT_Vector3 &wpos,
					const SIM_Property &property) const;
    /// Gets the value of some physical property for the supplied object
    /// at a given point on the object's geometry. This function calls
    /// SIM_Solver::getPropertyAtPoint().
    fpreal			 getPropertyAtPoint(GA_Index ptnum,
					const SIM_Property &property) const;

    /// Returns a resolver for efficiently computing property values
    /// The caller must delete the returned object.
    SIM_PropertyResolver	*getPropertyResolver(const SIM_Property &property) const;

    /// Gets the velocity of a point on the object specified by a point
    /// number. The returned velocity is in world space. If a velocity
    /// point attribute exists, that value is used as the local velocity.
    UT_Vector3			 getVelocityAtPoint(GA_Index ptnum,
					const SIM_Geometry *geo = 0) const;
    /// Gets the simulation space position of a point on our geometry.
    UT_Vector3			 getPositionOfPoint(GA_Index ptnum,
					const SIM_Geometry *geo = 0) const;
    /// Returns the point number closest to a world space position.
    GA_Index			 getNearestPoint(const UT_Vector3 &wspos) const;

    /// Returns a resolver to lookup the closest point and extract
    /// the given attribute.  If attribute not found, 1.0 is returned.
    SIM_PropertyResolver	*getNearestPointAttribResolver(const char *attribname) const;

    /// Fetches the value of the supplied point attribute from our geometry.
    /// Returns true if the attribute was found, otherwise it returns false.
    /// The attribute must be a simple float attribute.
    bool			 getPointAttribute(GA_Index ptnum,
					const char *attrib,
					fpreal &value) const;

    /// Fetches the SIM_Position data associated with the geometry data
    /// at the supplied location. 
    /// This uses the SIM_NAME_POSITIONPATH relative path to find the
    /// SIM_Position that the data refers to.  Supported data are
    /// SIM_Geometry and SIM_*Field.
    const SIM_Position		*getPositionForGeometry(const char *geo) const;

    /// Returns the object-specific solve info for this object.
    SIM_ObjectSolveInfo		*getSolveInfo() const;

protected:
    /// The constructor for a new simulation object.
    explicit		 SIM_Object(const SIM_SimulationState *factory);
    /// The object destructor.
    virtual		~SIM_Object();

    /// Clears all attached data.
    virtual void	 initializeSubclass();
    /// Makes two objects equal, but not their names or object ids.
    virtual void	 makeEqualSubclass(const SIM_Data *source);
    /// Saves an object to a stream.
    virtual void	 saveSubclass(std::ostream &os) const;
    /// Loads an object from a stream.
    virtual bool	 loadSubclass(UT_IStream &is);
    /// Caches the solver, collider, and position data for quick access.
    virtual void	 setNamedSubDataSubclass(const char *dataname,
						 const SIM_Data *data);
    /// Clears the solver, collider, and position cached pointers.
    virtual void	 removeNamedSubDataSubclass(const char *dataname);
    /// Creates a query object for getting SIM_Object-specific information.
    virtual SIM_Query	*createQueryObjectSubclass() const;
    /// Overrides interpolateSubclass() to set our interpolated flag.
    virtual void	 interpolateSubclass(const SIM_Data *source1,
					     const SIM_Data *source2,
					     fpreal interp);
    /// Get the amount of memory used by this object.
    virtual int64	 getMemorySizeSubclass() const;

    /// Override the SIM_RootData function for getting our identifier.
    virtual void	 getRootDataIdSubclass(SIM_RootDataId &id) const;
    /// Override the SIM_RootData function for matching us to a string.
    virtual bool	 getMatchesStringSubclass(const char *pattern) const;

private:
    void		 getAffectorsUnsafe(SIM_ObjectArray &affectors,
					    const char *relationshiptype) const;
    void		 setMetaObject(SIM_MetaObject *metaobject);
    void		 setSubStepCopyFromObject(SIM_Object *fromobject);
    void		 setObjectId(int id);
    void		 setIsBaked(bool baked);
    void		 setSolveFirstFrame(bool solvefirstframe);
    void		 setSolveState(const SIM_SolveState &solvestate);
    void		 copyObjectIdentifiers(const SIM_Object &source);
    UT_Vector3		 getSDFVelocity(const UT_Vector3 &localpos) const;
    UT_Vector3		 getPointVelocity(const UT_Vector3 &localpos) const;

    /// Get a const reference to our array of relationships.
    const SIM_RelationshipArray	&getRelationshipArray(bool ingroup) const;
    /// Get a non-const reference to our array of relationships.
    SIM_RelationshipArray	&getRelationshipArray(bool ingroup);

    const SIM_SimulationState	*mySimulationState;

    SIM_MetaObject		*myMetaObject;
    SIM_Object			*mySubStepCopyFromObject;
    SIM_SolveState		 mySolveState;
    SIM_RelationshipArray	 myGroupRelationships;
    SIM_RelationshipArray	 myAffectorRelationships;

    const SIM_Solver		*myCachedSolver;
    const SIM_Geometry		*myCachedGeometry;
    const SIM_ColliderLabel	*myCachedColliderLabel;
    const SIM_PhysicalParms	*myCachedPhysicalParms;
    mutable SIM_ObjectSolveInfo	*mySolveInfo;

    int				 myObjectId;
    bool			 myIsBaked;
    bool			 mySolveFirstFrame;
    bool			 myAllowCaching;

    friend class		 SIM_SimulationState;
    friend class		 SIM_Relationship;
    friend class		 SIM_ObjectSolveInfo;

    DECLARE_STANDARD_GETCASTTOTYPE();
    DECLARE_CLASSNAME(SIM_Object, SIM_RootData);
};

#endif

