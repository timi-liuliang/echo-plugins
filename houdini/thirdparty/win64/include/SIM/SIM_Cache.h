/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_Cache_h__
#define __SIM_Cache_h__

#include "SIM_API.h"
#include <UT/UT_LinkList.h>
#include "SIM_Utils.h"
#include "SIM_Object.h"
#include "SIM_ObjectArray.h"
#include "SIM_CacheOptions.h"

class SIM_Engine;
class SIM_CacheEntry;
class SIM_SimulationState;

/// This class is the cache for a SIM_Engine.
/// It holds all the actual objects for the simulation for the entire
/// cached time range.
class SIM_API SIM_Cache
{
public:
    /// Constructor intializes this class with no object data.
    /// A single cache entry is created to hold any new objects.
				 SIM_Cache();
    /// Destructor destroys all objects and temp files.
				~SIM_Cache();

    /// Initialize this object with a SIM_Engine pointer. This must be
    /// done as soon as the SIM_Cache is created.
    void			 setEngine(SIM_Engine *engine);
    /// Returns a reference to our caching option data.
    const SIM_CacheOptions	&getCacheOptions() const;
    /// Sets the cache options.
    void			 setCacheOptions(const SIM_CacheOptions &src);

    /// Caches the current object data and advances by one time step.
    /// Basically this consists of creating a copy of all objects in the
    /// simulation and setting the new current time. If the cache size
    /// is exceeded, a cache entry will be deleted. A cache entry may
    /// also get pushed to disk. If caching is disabled, this function
    /// does nothing except advance the current time.
    void			 cacheAndAdvance();
    /// Tells the cache to move to a new current time.
    /// Whatever cache entry is the best match for the specfied time will
    /// be made the current cache entry. If the new time is earlier than any
    /// cache entry, then the earliest cache entry is used. If the new time
    /// is later than the latest cache entry, the last entry is used. Note
    /// that this means the cache's current time won't necessarily be set
    /// to the requested value. It is up to the SIM_Engine to perform
    /// simulation steps to get to the actual desired time.
    void			 setCurrentTime(const SIM_Time &time);
    /// Deletes all cache entries after the specified time. The cache
    /// entry for the specified time itself is also deleted. The cache
    /// is moved to the entry just prior to the earliest deleted entry.
    /// If there is insufficient cache to fulfil the request, then nothing
    /// is changed and the function returns false. Otherwise it returns
    /// true.
    bool			 deleteAfterTime(const SIM_Time &time);
    /// Clears all cache entries. A single empty cache entry is then
    /// created to act as the current entry to shich new objects may be
    /// added.
    void			 clear();
    /// Attempts to re-load explicit cache entries from the last
    /// loaded etnry
    void			 preload();

    /// Returns the time of the current cache entry. This function is
    /// called by the SIM_Engine::getSimulationTime() function.
    const SIM_Time		&getCurrentTime() const;
    /// Returns the time of the earliest cache entry. This function is
    /// called by the SIM_Engine::getEarliestCacheTime() function.
    const SIM_Time		&getEarliestTime() const;
    const SIM_Time		&getLatestTime() const;
    /// Returns the objects that belong to the current cache entry.
    /// This function is called by the SIM_Engine getSimulationObject()
    /// and related functions.
    SIM_SimulationState		&getCurrentSimulationObjects() const;
    /// Returns the live cached frames
    void			 getCachedFrames(UT_IntArray &frames, bool memory) const;

    /// Returns the specified object at an earlier time step. If the object
    /// at the exact specified time cannot be found, we create a new object
    /// by interpolating between the previous and next available states of
    /// the object.
    const SIM_Object		*getObjectAtTime(const SIM_Object &object,
						const SIM_Time &time,
						bool allowinterpolation);
    /// Returns a non-const object from the specified earlier time. The
    /// returned value is always a pointer to the passed in object, or a
    /// substep object. If a time in some earlier timestep is specified,
    /// the earlier object is copied as a substep object of the current
    /// time. Otheriwse this function could be used to modify the past.
    SIM_Object			*getAffectorAtTime(SIM_Object &object,
						const SIM_Time &time,
						bool allowinterpolation);

    /// Before the call, 'past_objects' must be a valid array of valid arrays,
    /// and 'past_times' must be a valid array.
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

    /// Returns a substep version of the specified object. The substep
    /// object is always created within the current simulation state
    /// regardless of the time value. The reason the SIM_Cache is used
    /// as an intermediary here is so that if we already have an
    /// interpolated version of the object at the requested time, we
    /// can convert the interpolated object into a substep (which is
    /// really different only in the sense of whether the SIM_Cache or
    /// the SIM_SimulationState owns it).
    SIM_Object			*createSubStepObject(const SIM_Object &object,
						const SIM_Time &time);
    /// Deletes any objects from the myInterpolatedObjects array that
    /// match the provided object id. This function is called by the
    /// engine just prior to solving the object. The idea is that the
    /// object may have been used to create an interpolated object prior
    /// to its solve for a given timestep. After its solve, all the
    /// interpolation is incorrect, so leaving the interpolated objects
    /// around is potentially misleading and harmful.
    void			 clearInterpolatedObjects(
				    const SIM_ObjectArray &objects);
    /// This performs the cleanup required after performing a simulation step.
    /// All temporary interpolated objects are deleted, and any excess cache
    /// entries in memory are flushed to disk.
    void			 postSimulationStep();

private:
    /// Loads a cache entry from disk (if it's not already in memory).
    void			 loadFromDisk(SIM_CacheEntry *entry);
    /// Moves some cache entries from memory to disk if we have too many
    /// in memory.
    void			 ensureCacheCompliesWithOptions();
    /// Interpolated objects use for display are stored outside of any
    /// cache entry. This function clears out those interpolated objects.
    void			 clearInterpolatedObjects(bool clearsubsteps);
    /// Utility function used by getObjectAtTime() and getAffectorAtTime().
    /// It finds the closest existing objects to the requested object.
    void			 getBestObjects(const SIM_Object *object,
					const SIM_Time &time,
					bool allowinterpolation,
					const SIM_Object *&matchobject,
					SIM_Object *&nonconstmatchobject,
					const SIM_Object *&bestbeforeobject,
					SIM_Time &bestbeforetime,
					const SIM_Object *&bestafterobject,
					SIM_Time &bestaftertime);
    /// Get the initial entry, which is the simulation state that shows
    /// up for all times less then zero, and which is copied to be the
    /// first timestep state when the cache is cleared.
    SIM_CacheEntry		*getInitialEntry();
    /// Clears the initial entry.
    void			 clearInitialEntry();
    /// Get our current cache entry.
    SIM_CacheEntry		*getCurrentEntry() const;
    /// Set our current cache entry.
    void			 setCurrentEntry(SIM_CacheEntry *newentry);

    SIM_Engine			*myEngine;
    SIM_CacheOptions		 myCacheOptions;
    UT_LinkList			 myCacheEntries;
    SIM_CacheEntry		*myCurrentEntry;
    SIM_CacheEntry		*myInitialEntry;
    SIM_ObjectAtTimeMatrix	 myInterpolatedObjects;

    /// Do we have any temporary disk caches?
    /// Does not count explicit entries.
    /// We use this so we can determine if our getAllowSaveToDisk flag
    /// needs to be enforced.
    bool			 myHasDiskCache;
};

#endif

