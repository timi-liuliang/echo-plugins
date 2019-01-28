/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OBJ_XformCache.h ( OBJ Library, C++)
 *
 * COMMENTS:
 */

#ifndef __OBJ_XformCacher__
#define __OBJ_XformCacher__

#include "OBJ_API.h"
#include <UT/UT_Cache.h>
#include <UT/UT_Matrix4.h>
#include <UT/UT_SpinLock.h>
#include <UT/UT_ThreadSafeCache.h>
#include <UT/UT_VectorTypes.h>
#include <OP/OP_Version.h>

class CMD_Args;
class OBJ_XformCacheData;
class OBJ_XformCacheTag; 
class OBJ_XformCacheHashTable;

/// The OBJ_XformCache maintains a table of recently calculated transforms
/// which describe world position and orientation of objects.
///
/// It can also watch memory usage to see what transform matrices should be
/// removed from the table.
class OBJ_API OBJ_XformCache : public UT_Cache
{
public:

    // specifies transforms that a single object is able to cache
    enum OBJ_XformType 
    {
	WORLD_XFORM,		//world transform of an object
	LOCAL_XFORM,		//local transform of an object
    };

    // specifies the results returned by cache lookups
    enum OBJ_LookupStatus
    {
	HIT,		// lookup is successful
	MISS_VERSION,	// lookup failed: invalidated data
	MISS_DATA   	// lookup failed: data never cached
    };

    // some constants
    enum
    {
	// default memory limit for the cache in bytes NB: for a production size
	// scene with three characters there are around 1000 bones, which take
	// about 350kB per frame to cache 1000 objects.  So 300 frames take
	// around 100MB of cache memory.
	DEFAULT_CACHE_SIZE_KB = 150*1024
    };


public:
     OBJ_XformCache();
    ~OBJ_XformCache();

    /// Obtains the transform matrix cache used by all objects
    static OBJ_XformCache	*getCache();

    /// querries the cache for the transform matrix
    /// INPUTS:
    ///   id      - the unique identification number of the operator
    ///   time    - the time at which the xform is applied to the object
    ///   type    - tranformation type
    ///   version - version of parameters used to construct the xform
    ///  OUTPUT:
    ///   xform   - on cache hit, the transform matrix, otherwise unchanged
    ///  RETURNS:
    ///   the status of the lookup (hit or miss)
    ///
    const UT_DMatrix4 &		 getXform( int id, fpreal time,
					   OBJ_XformType type,
					   OP_VERSION version,
					   OBJ_LookupStatus &status );

    /// sets the transform matrix in the cache
    /// INPUTS:
    ///   id      - the unique identification number of the operator
    ///   time    - the time at which the xform is applied to the object
    ///   type    - tranformation type
    ///   version - version of parameters used to construct the xform
    ///  RETURNS:
    ///	  true:			if succeeded 
    ///   false:		if failed
    ///
    bool			 setXform( int id, fpreal time, 
					   OBJ_XformType type,
					   OP_VERSION version,
					   const UT_DMatrix4 &xform );

    // TODO: add higher performance version of getXform, which returns
    //       cache data container to be set directly by cache caller
    //       on cache miss (determined by version ID)


    /// Used as the interface to objcache, which is installed in
    /// MOT_Command.C
    void			 parseCommand( CMD_Args &args );


    /// Sets the new memory limit for the cache size in bytes
    void			 setMemoryLimit( int64 max_memory );

    /// Clears the cache and frees memory
    void			 clear();

    typedef UT_ThreadSafeCache<UT_RecursiveSpinLock> Policy;

    enum OBJ_MemoryState
    {
        MEMORY_CHECK_NEVER,	// never track memory size
        MEMORY_CHECK_ALWAYS	// always watch for memory limit
    };

    OBJ_MemoryState              getMemoryState() const;

protected:
    // the UT_Cache interface
    virtual const char *utGetCacheName() const
			{return "Object Transform Cache";}
    virtual int64	utGetCurrentSize() const
			{ return entries()* getEntryMemUsage(); }
    
    virtual bool	utHasMaxSize() const { return true; }
    virtual int64	utGetMaxSize() const
			{ return myMaxEntries* getEntryMemUsage(); }
    virtual void	utSetMaxSize(int64 size);
    
    virtual int64	utReduceCacheSizeBy(int64 amount);

private: //methods
    // sets the give tag as most recently used
    void			 setMRU(OBJ_XformCacheTag *tag);
    // remove least recently used entry
    void			 deallocateItem(OBJ_XformCacheTag *tag);
    // returns the memory size of a cache entry
    static int64		 getEntryMemUsage();
    // returns number of entries in the cache (NOT THREAD-SAFE!)
    int64			 entries() const;

private:
    /// This stores the transformation matrices computed previously
    OBJ_XformCacheHashTable	*myData;

    /// Access policy
    Policy			 myCachePolicy;

    // Memory management variables
    int64			 myMaxEntries;	    //memory limit (# of xforms)

    const UT_DMatrix4		 theIdentity; // Identify matrix when failing and returning a const reference.
private:
    friend class OBJ_XformCacheTag;
};


#endif

