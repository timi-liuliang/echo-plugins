/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_Primitive.h ( GT Library, C++)
 *
 * COMMENTS:
 */
#ifndef GT_PackedGeoCache_h
#define GT_PackedGeoCache_h

#include "GT_API.h"
#include "GT_Primitive.h"
#include <GEO/GEO_PackedTypes.h>
#include <UT/UT_Cache.h>
#include <UT/UT_StringMap.h>
#include <vector>
#include <string>

class GT_API GT_PackedGeoCache : public UT_Cache
{
public:
    static bool		isCachingAvailable();

    /// Find a GT prim in the cache under 'name'. The version and extra_flags
    /// must match. If 'return_anim' is specified, the animation type will be
    /// assigned to that pointer. version and extra_flags can be anything that
    /// makes sense to the primitive being cached; the cache only tests them
    /// for inequality.
    static GT_PrimitiveHandle	findInstance(const UT_StringHolder &name,
					     int64 version,
					     int64 extra_flags,
					     GEO_AnimationType *return_anim= 0);

    /// Cache the instance geometry along with its version, and animation type.
    /// 'version' and 'extra_flags' can be anything, but a mismatch during
    /// findInstance() will cause a cache miss.
    static void		cacheInstance(const UT_StringHolder &name,
				      const GT_PrimitiveHandle &instance,
				      int64 version,
				      int64 extra_flags,
				      GEO_AnimationType type);
    
    /// Recache the geometry (possibly due to modification like convexing)
    /// The geometry should already exist in the cache, otherwise this fails.
    static bool		recacheInstance(const UT_StringHolder &name,
					const GT_PrimitiveHandle &instance);
    
    /// Remove the geometry with the given name, returning true if removed.
    static bool		removeInstance(const UT_StringHolder &name);

    /// Find the animation type of the instance without fetching the geometry.
    static bool		findAnimation(const UT_StringHolder &name,
				      int64 version,
				      GEO_AnimationType &type);
  

    // Convenience methods for building various cache keys

    // Alembic object
    static void buildAlembicArchiveName(UT_StringHolder &archive,
					const std::vector<std::string> &paths);
    static void	buildAlembicName(UT_StringHolder &name,
				 const char *object,
				 const char *archive,
				 fpreal frame);
    /// Clear a specified alembic, or all alembics.
    static void clearAlembics(const char *filename);
    static int64 getAlembicVersion(const char *archive);
    static bool hasAlembicArchive(const char *archive);

    
    // Disk File (filename is the disk file, index is the GT prim within the
    // refined disk file geometry as it may have multiple GT prim types)
    static void	buildFileRefName(UT_StringHolder &name,
				 const char *filename,
				 exint index);

    // Returns the number of cached GTs primitives that resulted from a disk
    // file
    static int	getNumDiskFilePrims(const UT_StringHolder &name);
    static void setNumDiskFilePrims(const UT_StringHolder &name, int count);

    
    // UT_Cache interface
    virtual const char *utGetCacheName() const { return "Packed Geometry"; }
    virtual int64	utGetCurrentSize() const { return myGeoCacheSize; }
    virtual int64	utReduceCacheSizeBy(int64 amount);
    virtual int64       utGetMinSize() const;
    virtual int64       utGetMaxSize() const;

    static void initialize();

private:
    
    GT_PackedGeoCache();
    ~GT_PackedGeoCache();
    void	privClearAlembics(const char *filename);
    
    class gt_CachedGeometry
    {
    public:
	gt_CachedGeometry() : lru(0), mem(0), version(0), locked(false) {}
	gt_CachedGeometry(const GT_PrimitiveHandle &h, int64 L, int64 M,
			  int64 v, int64 e, GEO_AnimationType anim, bool lock)
	    : geometry(h), lru(L), mem(M), locked(lock), version(v),
	      extra_flags(e), animation(anim) {}
	GT_PrimitiveHandle    geometry;
	int64		      lru;
	int64		      mem;
	int64		      version;
	int64		      extra_flags;
	GEO_AnimationType     animation;
	bool		      locked;
    };

    // Caching
    UT_StringMap<gt_CachedGeometry> myGeoCache;
    int64			    myLRUCount;
    int64			    myGeoCacheSize;

    // File cache
    UT_StringMap<int64>		    myFileVersion;
    UT_StringMap<int64>		    myDiskFileCount;
};

#endif
