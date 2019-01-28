/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_Engine_h__
#define __SIM_Engine_h__

#include "SIM_API.h"
#include "SIM_Cache.h"
#include "SIM_Data.h"
#include "SIM_Error.h"
#include <OP/OP_Value.h>
#include <DEP/DEP_MicroNode.h>
#include <UT/UT_Map.h>
#include <UT/UT_Set.h>
#include <UT/UT_SymbolTable.h>

#define SIM_PATH_SEPCHAR	'/'
#define SIM_OBJECT_SEPCHAR	':'
#define SIM_OBJECTNUM_SEPCHAR	'.'
#define SIM_OBJECTDATA_SEPCHAR	'/'

class UT_InfoTree;
class UT_StringArray;
class UT_StringRef;
class OP_Node;
class SIM_BaseCache;
class SIM_DopDescription;
class SIM_DataFactoryCreator;
class SIM_DataFactory;
class SIM_Data;
class SIM_SimulationState;
class SIM_MetaObject;
class SIM_MetaObjectArray;
class SIM_ConstObjectArray;
class SIM_ObjectArray;
class SIM_SaveCommand;

class SIM_API SIM_SimulationMicroNode : public DEP_MicroNode
{
public:
		SIM_SimulationMicroNode(SIM_Engine &engine);
    virtual const char *    className() const
				{ return "SIM_SimulationMicroNode"; }
    virtual void		becameDirty(DEP_MicroNode &src, const DEP_PropagateData &data);
protected:
    SIM_Engine		&myEngine;
};

/// This is the main engine behind all simulations. Any number of these
/// can be created to create independent simulations. Generally a simulation
/// will be owned by a particular OP_Node. For example, the DopNet object
/// contains a SIM_Engine.
class SIM_API SIM_Engine
{
public:
    /// Constructor to create a new simulation.
				 SIM_Engine(OP_Node *owner);
    /// Destructor to destroy all data associated with a simulation.
    virtual			~SIM_Engine();

    /// Returns the OP_Node that owns this engine. This may be null if
    /// the engine isn't associated with a particular node.
    OP_Node			*getOwner() const;
    /// Gets the full path to the DOP Network node that owns this simulation.
    const UT_String		&getOwnerFullPath() const;
    void			 getOwnerFullPathString(UT_String &str) const;

    /// Allocates a new SIM_Object and adds it to the simulation.
    /// The name parameter is assigned to the object. This name does not
    /// have to be unique.
    SIM_Object			*addSimulationObject(bool solvefirstframe);
    /// Removes a SIM_Object from the simulation.
    /// The index value is the current location of the object in the
    /// simulations's list of objects. This value will not be the same
    /// as the SIM_Object::getObjectId() value, and may change when objects
    /// are added to or removed from the simulation.
    void			 removeSimulationObject(int index);
    /// Removes a SIM_Object from the simulation.
    /// This version of this function takes a SIM_Object pointer instead
    /// of an index. The pointer is converted to an index and the indexed
    /// version of removeSimulationObject() is called.
    void			 removeSimulationObject(SIM_Object *object);
    /// Returns the number of objects currently in the simulation.
    int				 getNumSimulationObjects() const;
    /// Returns the number of objects currently in the simulation.
    int64			 getNumSimulationData() const;
    /// Returns a pointer to the SIM_Object at the specified index.
    /// The index value is the current location of the object in the
    /// simulations's list of objects. This value will not be the same
    /// as the SIM_Object::getObjectId() value.
    const SIM_Object		*getSimulationObject(int index) const;
    /// Returns a pointer to the SIM_Object with the specified unique id.
    /// This function searches the current list of objects for the
    /// specified objectid. If the objectid is not found, this function
    /// returns null.
    const SIM_Object		*getSimulationObjectFromId(int objectid) const;
    /// Returns the "nth" simulation object that matches a supplied filter.
    /// If less than the supplied n objects match the filter, 0 is returned.
    const SIM_Object		*getNthSimulationObject(
					const SIM_DataFilter &filter,
					int n) const;

    /// Creates a new relationship. The creationflags parameter accepts the
    /// same SIM_DATA_* flags accepted by the creationflags parameter of
    /// SIM_Data::createNamedSubData().
    SIM_Relationship		*addRelationship(const char *name,
						 int creationflags);
    /// Returns the number of relationships in the current simulation state.
    int				 getNumRelationships() const;
    /// This function allows looping through the simulations relationships.
    const SIM_Relationship	*getRelationship(int index) const;
    /// Returns the relationship with the specified name.
    const SIM_Relationship	*getRelationship(const char *name) const;
    /// Gets all the relationships of a particular type. The type of a
    /// relationship is defined by the one subdata on the SIM_Relationship.
    void			 filterConstRelationships(
					const SIM_DataFilter &filter,
					SIM_ConstDataArray &rels) const;
    /// Removes the relationship at the specified position.
    void			 removeRelationship(int index);
    /// Removes the relationship with the specified name.
    void			 removeRelationship(const char *name);
    /// Removes the specified relationship.
    void			 removeRelationship(SIM_Relationship *rel);
    /// Removes all relationships created by a particular DOP node. If -1
    /// is specified, then all relationships are removed. Relationships in
    /// the "except" array are not removed.
    void			 removeAllRelationships(int createdby,
					const SIM_RelationshipArray *except);

    /// Returns either a SIM_Relationship or SIM_Object depending on the
    /// information stored in the id parameter.
    const SIM_RootData		*getRootDataFromId(
					const SIM_RootDataId &id) const;

    // Only HOM should call this method:
    SIM_RootData		*getNonConstRootDataFromId(
					const SIM_RootDataId &id);

    /// Returns a pointer to the state of an object at some time in the past.
    /// This function searches for these past object states in three ways.
    /// First, if the specified time is equal to a past time step, the state
    /// for that time step is returned. Second, if the specified time is not
    /// equal to a past time step, then past time steps are searched for
    /// object states stored explicitly with createSubStepObject(). Finally,
    /// if both these methods fail to find a matching object state, then the
    /// closest object states before and after the specified time are found
    /// using the first two approaches. Then a new object state is created by
    /// interpolating between these two states using SIM_Data::interpolate().
    /// This third approach is skipped if allowinterpolation is set to false.
    /// If no previous or no future object state can be found, then this
    /// function returns null.
    const SIM_Object		*getObjectAtTime(const SIM_Object &object,
						 const SIM_Time &time,
						 bool allowinterpolation) const;

    /// Returns a non-const pointer to an object for a particular time.
    /// This function is very similar to getObjectAtTime() except for two
    /// things. First, it requires a non-const starting object, and it is
    /// a non-const function. Second, if the specified time exactly matches
    /// a time in the past, the returned object will be a copy of that past
    /// object, not the object itself. Otherwise modifying that object could
    /// modify the past. However, if the current time is sepcified, the actual
    /// object (i.e. a pointer to the passed in SIM_Object) is returned.
    /// This function is for use by solvers that want to modify their
    /// affector objects and which do substepping.
    SIM_Object			*getAffectorAtTime(SIM_Object &object,
						 const SIM_Time &time,
						 bool allowinterpolation);

    /// This is a more direct, more efficient method for reading 
    /// cached versions of objects from previous frames, without interpolation.
    ///
    /// Before the call, 'past_objects' must be a valid array of 
    /// SIM_Object* arrays and 'past_times' must be a valid array of SIM_Time.
    /// 'past_objects' must have size 'max_past'
    /// each element of 'past_objects' must be an array of SIM_Object* of
    //// size 'num_objects'
    /// 'past_times' must be an array of size 'max_past'
    ///
    /// Output:
    /// The number of cached frames is returned. 
    /// The cached version of object[oi] at time past_times[i]
    /// is stored in the array past_objects[i] at location oi,
    /// for all 0 <= i < "the returned number of cached frames"
    
    int getObjectsAtPastTimes(
        const SIM_Object**const past_objects[],
        SIM_Time past_times[],
        const int num_objects,
        const SIM_Object*const objects[],
        const int max_past
    );
    
    /// Creates an object in the current state with a particular time stamp.
    /// This function is useful for solvers that need to do their own time
    /// stepping with step sizes smaller than the globally specified time
    /// step size. These explicitly stored subsamples are accessible later
    /// with the getObjectAtTime() function.
    SIM_Object			*createSubStepObject(const SIM_Object &object,
						     const SIM_Time &time);
    /// Removes an object previously created by createSubStepObject().
    /// If the allwithmatchingid parameter is set to true, not only will
    /// the specified object be destroyed, but all other objects with the
    /// same object id created using createSubStepObject() will be destroyed.
    /// This provides a quick way to destroy all temporary working copies
    /// of a particular object.
    void			 removeSubStepObject(SIM_Object *objectattime,
						     bool allwithmatchingid);

    /// Loads a simulation state from a file and merges it into the current
    /// simulation state. The data that gets loaded is not allowed to do any
    /// sharing of data with the current state, since the data id's can't
    /// be expected to match. Objects from the file will replace objects from
    /// the current state if their object ids match. Objects loaded from the
    /// file will be flagged so that they will not be solved in the current
    /// timestep. In subsequent timesteps these objects will be solved.
    /// The filter parameter specifies which objects stored in the file
    /// are merged in.
    bool			 mergeSimulationFile(const char *filename,
					const SIM_DataFilter &filter,
					const SIM_ObjectArray &replaceobjects,
					const OP_Node *defaultcreator,
					bool forcecreatortodefault,
					bool acceptnewobjects,
					bool matchbyname,
					const char *nameprefix);
    /// Schedules some objects to be saved to a files at the end of the
    /// current timestep. The filter parameter makes it possible to save
    /// only some of the objects to the file. The actual save happens
    /// in the postSimulationStep().
    void			 saveSimulationFile(OP_Node *node,
					const char *filename,
					const SIM_DataFilter &filter,
					const SIM_ObjectArray &objects,
					UT_CompressionType type = UT_COMPRESSION_TYPE_BLOSC) const;
    /// Saves the entire current state of the simulation immediately.
    bool			 saveSimulationState(std::ostream &os,
				 UT_CompressionType type = UT_COMPRESSION_TYPE_BLOSC) const;
    bool			 saveSimulationStateToFile(const char *fname,
				 UT_CompressionType type,
				 bool threadedio) const;

    /// Sets the current time for the simulation. If the new simulation
    /// time is less than the current time, the SIM_Cache is simply set to
    /// use its best match for the chosen time. If the new simulation time
    /// is greater than the current time, the simulation is stepped forward
    /// in time to the new time. The resimlasttimestep parameter causes the
    /// most recent cache entry to be deleted before trying to set the
    /// current time. This is useful if some parameter of the simulation
    /// changes on the current timestep. It causes the timestep to be
    /// recooked with the new parameter. This resimulation may also happen
    /// if the simulation was interrupted the last time this function was
    /// called. The forceresetsim parameter tells the SIM_Engine to reset
    /// the simulation before trying to move to the specified time, whether
    /// the reset is required or not.
    void			 setSimulationTime(const SIM_Time &t,
						   bool resimlasttimestep,
						   bool forceresetsim,
						   bool allowsimulation);

    /// Marks this engine as invalid without actually clearing or
    /// resetting anything.   A proper reset implies also a frame
    /// advance to the first frame that can be expensive.
    /// Note the cache will not be cleared until it is cooked, however!
    void			 resetToFirstSimulationStep()
				 { myFirstSimulationStep = true; }

    /// Returns the current simulation time.
    const SIM_Time		&getSimulationTime() const;
    /// Returns the simulation time that corresponds to the given global time.
    const SIM_Time		 getEngineTime(const SIM_Time &t) const;
    /// Returns the global time that corresponds to the given simulation time.
    const SIM_Time		 getGlobalTime(const SIM_Time &t) const;
    /// Returns the time of the earliest simulation state in the cache.
    const SIM_Time		&getEarliestCacheTime() const;
    const SIM_Time		&getLatestCacheTime() const;
    /// Returns the cached frames.
    /// Inclusive.
    void			 getCachedFrames(UT_IntArray &frames, bool inmemory) const;
    /// Returns a reference to the cache options for this simulation.
    const SIM_CacheOptions	&getCacheOptions() const;
    /// Sets the cache options for this simulation.
    void			 setCacheOptions(const SIM_CacheOptions &o);
    /// Returns the time step size.
    const SIM_Time		&getTimeStep() const;
    /// Sets the global time step size.
    void			 setTimeStep(const SIM_Time &time);
    /// Gets the maximum number of allowed feedback iterations.
    int				 getMaxFeedbackIterations() const;
    /// Sets the maximum number of allowed feedback iterations.
    void			 setMaxFeedbackIterations(int max);
    /// Gets the current feedback iteration we are on. If we are not
    /// currently solving, this function will return -1;
    int				 getFeedbackIteration() const;
    /// Returns true if the engine is providing data hierarchy hints.
    bool			 getProvideDataHints() const;
    /// Specifies whether the engine should provide data hierarchy hints.
    void			 setProvideDataHints(bool providehints);
    /// Gets the id for the node that is currently being processed.
    int				 getCurrentCreatorId() const
    { return myCurrentCreatorId; }
    /// Gets the index of the node output that is currently being processed.
    int				 getCurrentCreatorIndex() const
    { return myCurrentCreatorIndex; }
    /// Set the creator information. These values are automatically reset
    /// to (-1, -1) before performing a simulation step.
    void			 setCreatorInfo(int id, int index);
    /// Returns the SIM_Data pointer with the specified unique id.
    const SIM_Data		*getConstDataWithId(const UT_Guid &id) const;
    /// Returns the simulation frame number that corresponds to the given
    /// time. The simulation frame number is the number of timesteps that
    /// need to be run to get to the specified time, plus one (so that
    /// time zero is frame 1).
    int				 getSimulationFrame(const SIM_Time &time) const;
    /// Returns the simulation time that corresponds to the given simulation
    /// frame.
    const SIM_Time		 getSimulationTime(int frame) const;

    /// Adds an error to the engine. The object and data parameters allow
    /// the DOP_Engine to put the error message on an appropriate DOP Node.
    /// Errors are found in the SIM error file. This function simply calls
    /// addErrorSubclass().
    void			 addError(const SIM_RootData *rootdata,
					  const SIM_Data *data,
					  int errorcode,
					  const char *errorparm,
					  UT_ErrorSeverity severity) const;

    /// Returns the total amount of memory used by all data currently in
    /// this simulation, at the current time or in the cache.
    int64			 getTotalMemorySize() const;

    /// Removes all node interests from the engine.
    void			 removeAllOPInterests() const;
    /// Adds an interest in the supplied node. When the node changes, the
    /// engine's handleExternalNodeChange function will be called.
    void			 addOPInterest(OP_Node *node) const;
    void			 addGuideOPInterest(OP_Node *node) const;

    /// Passes up to our owner to alert anything that needs an OP_Event
    /// in response to use becoming dirty.
    virtual void		 notifySimulationChangeSinksOfChange() {}

    /// Returns all available data type names.
    /// If a null type pointer is passed in, this function returns a list
    /// of every data type that the SIM_Engine can create. If a data type
    /// name is passed in, the list returned will only contain those data
    /// types that can be cast to the specified data type using the
    /// SIM_Data::getPointerToType() function.
    void			 getDataTypes(UT_StringArray &datatypes,
					      const UT_StringRef &datatype) const;
    /// Returns all available data types and the source of their definition.
    /// The definition source will be an empty string if the data type is
    /// defined internally to Houdini. Otherwise it will be the name of the
    /// DSO or DLL file that contains the data type.
    void			 getDataTypeSources(UT_StringArray &datatypes,
						    UT_StringArray &srcs) const;
    /// Returns the descriptive name for a data type.
    /// Given the unique identifying name for a data type this function
    /// finds the appropriate data factory and returns the descriptive
    /// string assigned to the type.
    const char			*getDescription(
					const UT_StringRef &datatype) const;
    /// Returns the SIM_DopDescription from the SIM_DataFactory of a
    /// particular data type.
    const SIM_DopDescription	*getDopDescription(
					const UT_StringRef &datatype) const;

    /// Returns whether we can cast to a superclass data type without creating
    /// the SIM_Data.
    static bool			 canCastToDataType(
					const UT_StringRef &from_type,
					const UT_StringRef &to_type);

    /// Filters all our simulation objects based on a string pattern.
    /// Note that the time value is a simulation time, not global time.
    void			 findAllObjectsFromString(const char *objspec,
					SIM_ConstObjectArray &objects,
					const SIM_Time &t,
					bool interpolateobj) const;
    /// Returns a single object matching a specified string pattern.
    /// Note that the time value is a simulation time, not global time.
    const SIM_Object		*findObjectFromString(const char *objspec,
					int whichmatch, int *nummatch,
					const SIM_Time &t,
					bool interpolateobj) const;
    /// Returns a list of objects created by the specified node, or any
    /// nodes inside the specified node (if it is a subnet).
    void			 findAllObjectsCreatedBy(OP_Node *creator,
					SIM_ConstObjectArray &objects) const;
    /// Filters all our relationships based on a string pattern.
    /// Note that the time value is a simulation time, not global time.
    void			 findAllRelationshipsFromString(
					const char *relspec,
					SIM_ConstDataArray &relationships,
					const SIM_Time &t,
					bool interpolaterel) const;
    /// Returns a single relationship matching a specified string pattern.
    /// Note that the time value is a simulation time, not global time.
    const SIM_Relationship	*findRelationshipFromString(
					const char *relspec,
					int whichmatch, int *nummatch,
					const SIM_Time &t,
					bool interpolaterel) const;

    /// Gets a list of all selected data in the simulation.
    void			 getSelection(SIM_ConstDataArray &data) const;
    /// Clears the selection flag on all data in this simulation.
    void			 clearSelection() const;

    /// Returns the scale factor to convert a value in the provided units
    /// into the equivalent MKS value.
    fpreal			 scaleToMKS(const char *fromunits) const;
    /// Returns the scale factor to convert a value in MKS units into the
    /// equivalent value in the provided units.
    fpreal			 scaleFromMKS(const char *tounits) const;

    /// The micronode representing the simulation result. It is marked as
    /// dirtied and propagated whenever the simulation is cleared. Callers can
    /// use this to express dependencies on/from the simulation.
    DEP_MicroNode &	 simMicroNode()
				    { return mySimMicroNode; }
    const DEP_MicroNode & simMicroNode() const
				    { return mySimMicroNode; }
    /// This micronode holds the simulation input.  It is wired directly
    /// to the simMicroNode, however, does not care about refilter
    /// events.
    DEP_MicroNode &	 simInputMicroNode()
				    { return mySimInputMicroNode; }
    const DEP_MicroNode & simInputMicroNode() const
				    { return mySimInputMicroNode; }
    DEP_MicroNode &	 resimParmsMicroNode()
				    { return myResimParmsMicroNode; }
    const DEP_MicroNode & resimParmsMicroNode() const
				    { return myResimParmsMicroNode; }
    DEP_MicroNode &	 refilterParmsMicroNode()
				    { return myRefilterParmsMicroNode; }
    const DEP_MicroNode & refilterParmsMicroNode() const
				    { return myRefilterParmsMicroNode; }

    /// Sets this simulation as a player. It will replace it's cache
    /// with simulation data from the files given and not simulate,
    /// just read the files.
    void setIsPlayer(bool isPlayer);
    bool isPlayer() const { return myIsPlayer; }

    /// Rebuilds the player cache.
    void resetPlayer();
    /// Tries to expand the cached frame range
    void updatePlayerFrames();

protected:
    /// Resets the simulation to a clean state. This function calls
    /// clearSimulation(), then builds a single empty simulation state.
    virtual void		 resetSimulation();
    /// This function is run before passing each object to its solver.
    /// The implementation at this level clears out all temporary data
    /// from the simulation objects.
    virtual void		 preSimulationStep();
    /// This function is run after all objects are processed by their solvers.
    virtual void		 postSimulationStep();
    /// This function is called by our cache whenever the most recent
    /// simulated timestep is being removed from memory (either deleted
    /// or moved onto disk).
    virtual void		 clearReferencesToMostRecentState();
    /// Initializes a newly created object. This function is called by
    /// addSimulationObject(). The default implementation does nothing.
    virtual void		 objectCreated(SIM_Object *object);
    /// Allows special handling of objects that are being removed. This
    /// function is called by removeSimulationObject(). The default
    /// implementation does nothing.
    virtual void		 objectRemoved(SIM_Object *object);
    /// Override this method to handle changes to external nodes in which
    /// we have an interest. The default implementation does nothing.
    virtual void		 handleExternalNodeChangeSubclass(
					OP_Node *changednode,
					OP_EventType eventtype,
					void *data,
					bool isguide);
    /// The default implementation of this function does nothing.
    virtual void		 addErrorSubclass(const SIM_RootData *rootdata,
					const SIM_Data *data,
					int errorcode,
					const char *errorparm,
					UT_ErrorSeverity severity) const;
    /// Returns the simulation time that corresponds to the given global time.
    virtual const SIM_Time	 getEngineTimeSubclass(const SIM_Time &t) const;
    /// Returns the global time that corresponds to the given simulation time.
    virtual const SIM_Time	 getGlobalTimeSubclass(const SIM_Time &t) const;
    /// This function is the only way for a SIM_Engine subclass to get a
    /// non-const SIM_Object pointer.
    SIM_Object			*getNonConstSimulationObject(int index);

    /// Alerts our owner that we are simulating due to an internally
    /// generated need.  Returns the previous state
    virtual bool		 setIsSimulating(bool issimulating) const;

private:
    /// Gets the unique id of the OP_Node that owns this engine.
    int                          getOwnerUniqueId() const;
    /// Completely clears all existing data related to a simulation.
    void			 clearSimulation();
    /// Takes one time step forward in the simulation.
    void			 doSimulationStep(bool forreset);
    /// Solves a meta object for one time step. All affectors will already
    /// be solved.
    void			 doSimulationStepForMetaObject(
					SIM_MetaObject &metaobject,
					const SIM_Time &timestep,
					bool isLastMetaObject);
    /// Merges another simulation state in with our current state. This
    /// state can come from a file or another live simulation.
    void			 mergeSimulation(
					SIM_SimulationState &fromstate,
					const SIM_DataFilter *filter,
					const SIM_ObjectArray *replaceobjects,
					bool acceptnewobjects,
					bool matchbyname,
					const char *nameprefix);

    /// Solves a relationship for one timestep.  All objects will
    /// already be solved.
    void			 solveRelationship(SIM_Relationship *relation,
						const SIM_Time &timestep);

    /// Gets the current simulation state. Notably, it returns non-const.
    /// It is quite questionable to use this non-const function in
    /// solvers!  One should acquire non-const SIM_Objects through
    /// your affector lists or other ways.
    SIM_SimulationState		&getSimulationObjects();
    /// Gets a const reference to the current simulation state.
    const SIM_SimulationState	&getSimulationObjects() const;

    SIM_Object			*getNonConstSimulationObjectFromId(
					int objectid);
    SIM_Relationship		*getNonConstRelationship(const char *name);
    SIM_Relationship		*getNonConstRelationshipFromIndex(int index);

    /// Returns the SIM_Data pointer with the specified unique id.
    SIM_Data		*getDataWithId(const UT_Guid &id) const;
    /// Destroy any data that has a zero reference count.
    void		 purgeUnusedData() const;

    /// Register a new piece of data with the engine. This is
    /// called from the SIM_DataFactory newData function to add
    /// the data to myAllData and myAllChangedSizeData.
    void		 registerNewData(SIM_Data *newdata) const;
    /// Create data using our factories.
    /// Only SIM_Data should use this functions.
    SIM_Data		*newData(const UT_StringRef &datatype,
				 bool doinit=true) const;
    /// Create data using our factories, and set the uniqueid of the new data.
    /// Only SIM_SimulationState should use this functions.
    SIM_Data		*newData(const UT_StringRef &datatype,
				 const UT_Guid &uniqueid) const;
    /// Unregister a piece of data that is being deleted. This is
    /// called from the SIM_DataFactory deleteData function to remove
    /// the data from myAllData and myAllChangedSizeData.
    void		 unregisterDeletedData(SIM_Data *deleteddata) const;
    /// Delete data using our factories.
    /// Only SIM_Data should use this functions.
    void		 deleteData(SIM_Data *data) const;
    /// This function is called when the unique id of a SIM_Data changes.
    /// It lets us keep the myAllData hash table up to date.
    void		 changeDataUniqueId(SIM_Data *data,
					    const UT_Guid &newid) const;
    /// This function is called when a piece of data has decided it needs
    /// to recalculate its memory size.
    void		 setNeedsToRecalculateMemorySize(
					    const SIM_Data *data) const;

    /// Access our data factories.
    SIM_DataFactory	*getDataFactory(const UT_StringRef &name) const;
    void		 addDataFactory(SIM_DataFactory *factory);

    /// This function is called when an external node in which we have an
    /// interest changes. It calls handleExternalNodeChangeSubclass.
    void		 handleExternalNodeChange(OP_Node *changednode,
						  OP_EventType eventtype,
						  void *data,
						  bool isguide);
    /// This static version of handleExternalNodeChange converts the callee
    /// pointer to a SIM_Engine pointer and calls handleExternalNodeChange
    /// on that engine.
    static void		 handleExternalNodeChange(OP_Node *changednode,
						  void *callee,
						  OP_EventType eventtype,
						  void *data);
    static void		 handleExternalGuideNodeChange(OP_Node *changednode,
						  void *callee,
						  OP_EventType eventtype,
						  void *data);

    /// Adds a SIM_DataFactoryCreator to the global list.
    static void	 addDataFactoryCreator(SIM_DataFactoryCreator *creator);
    /// Removes a SIM_DataFactoryCreator from the global list.
    static void	 removeDataFactoryCreator(SIM_DataFactoryCreator *creator);
    /// Returns the global list of data factory creators.
    static UT_ValArray<SIM_DataFactoryCreator *>	&getCreators();

    UT_SymbolMap<SIM_DataFactory *>		 myFactories;
    SIM_Cache					 myCache;
    SIM_BaseCache				*myBaseCache;
    SIM_Time					 myTimeStep;
    OP_Node					*myOwner;
    int						 myMaxFeedbackIterations;
    int						 myFeedbackIteration;
    int						 myCurrentCreatorId;
    int						 myCurrentCreatorIndex;
    mutable UT_String				 myOwnerFullPath;
    mutable UT_ValArray<SIM_SaveCommand *>	 mySaveCommands;
    mutable UT_Map<UT_Guid, SIM_Data *>          myAllData;
    mutable UT_Set<const SIM_Data *>             myAllChangedSizeData;
    mutable UT_Set<OP_Node *>			 myOPInterestNodes;
    mutable UT_Set<OP_Node *>			 myGuideOPInterestNodes;
    mutable int64				 myTotalMemorySize;
    bool					 myProvideDataHints;
    bool					 myFirstSimulationStep;
    bool					 myLastCookInterrupted;

    friend class				 SIM_Data;
    friend class				 SIM_DataFactory;
    friend class				 SIM_DataFactoryCreator;
    friend class				 SIM_SimulationState;
    friend class				 SIM_SaveCommand;
    friend class				 SIM_BaseCache;
    friend class				 SIM_Cache;

protected:
    SIM_SimulationMicroNode		 	 mySimInputMicroNode;
    DEP_MicroNode		 	 	 mySimMicroNode;
    DEP_MicroNode			 	 myResimParmsMicroNode;
    DEP_MicroNode			 	 myRefilterParmsMicroNode;
    bool					 myIsPlayer;

};

#endif
