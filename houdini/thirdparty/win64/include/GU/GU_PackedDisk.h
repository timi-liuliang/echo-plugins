/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_PackedDisk.h (GU Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GU_PackedDisk__
#define __GU_PackedDisk__

#include "GU_PackedImpl.h"
#include <UT/UT_StringHolder.h>

class GA_PrimitiveFactory;
class UT_MemoryCounter;

/// Geometry File procedural
class GU_API GU_PackedDisk : public GU_PackedImpl
{
public:
	     GU_PackedDisk();
	     GU_PackedDisk(const GU_PackedDisk &src);
    virtual ~GU_PackedDisk();

    /// Convenience method to create a packed primitive in the destination
    /// detail.
    ///
    /// Returns a NULL pointer if the process fails.
    static GU_PrimPacked	*packedDisk(GU_Detail &dest,
					const char *filename,
					bool do_expanding,
					fpreal expand_frame);

    /// Clear the disk cache.  This will clear the cache, but not dirty any
    /// existing primitives.  This is what gets invoked when calling
    /// GU_PrimPacked::clearCachedGeometry().  The method returns the number of
    /// items cleared.
    static exint	clearDiskCache();

    /// Install the procedural
    static void	install(GA_PrimitiveFactory *prim);

    /// Get the type ID for the GU_PackedDisk primitive type.
    static GA_PrimitiveTypeId typeId()
    {
        return theTypeId;
    }

    /// @{
    /// Implementation of GU_PackedImpl interface
    virtual GU_PackedFactory	*getFactory() const;
    virtual GU_PackedImpl	*copy() const;
    virtual void		 clearData();

    virtual bool	isValid() const;
    virtual bool	load(GU_PrimPacked *prim, const UT_Options &options, const GA_LoadMap &map)
			    { return loadFrom(prim, options, map); }
    virtual bool	supportsJSONLoad() const	{ return true; }
    virtual bool	loadFromJSON(
        GU_PrimPacked *prim,
        const UT_JSONValueMap &options,
        const GA_LoadMap &map)
    { return loadFrom(prim, options, map); }
    virtual void	update(GU_PrimPacked *prim, const UT_Options &options);
    virtual bool	save(UT_Options &options, const GA_SaveMap &map) const;
    virtual bool	getBounds(UT_BoundingBox &box) const;
    virtual bool	getRenderingBounds(UT_BoundingBox &box) const;
    virtual void	getVelocityRange(UT_Vector3 &min, UT_Vector3 &max)const;
    virtual void	getWidthRange(fpreal &wmin, fpreal &wmax) const;
    virtual void	getPrimitiveName(const GU_PrimPacked *prim, UT_WorkBuffer &wbuf) const;

    virtual bool        unpack(GU_Detail &destgdp, const UT_Matrix4D *transform) const;
    virtual bool			isLoaded() const;
    virtual GU_ConstDetailHandle	getPackedDetail(GU_PackedContext *context = 0) const;

    /// Report memory usage (includes all shared memory)
    virtual int64 getMemoryUsage(bool inclusive) const;

    /// Count memory usage using a UT_MemoryCounter in order to count
    /// shared memory correctly.
    virtual void countMemory(UT_MemoryCounter &counter, bool inclusive) const;
    /// @}

    /// @{
    /// Member data accessors for intrinsics
    UT_StringHolder filename() const { return myCache.filename(); }
    UT_StringHolder intrinsicFilename(const GU_PrimPacked *prim) const { return filename(); }
    UT_StringHolder unexpanded() const { return myUnexpanded; }
    UT_StringHolder intrinsicUnexpanded(const GU_PrimPacked *prim) const { return unexpanded(); }
    void	setUnexpanded(GU_PrimPacked *prim, const UT_StringHolder &f)
		{
		     myUnexpanded = f;
		     makeFilename(prim);
		}
    fpreal      expandFrame() const { return myExpandFrame; }
    fpreal      intrinsicExpandFrame(const GU_PrimPacked *prim) const
                { return expandFrame(); }
    void	setExpandFrame(GU_PrimPacked *prim, fpreal f)
		{
		    myExpandFrame = f;
		    makeFilename(prim);
		}
    bool        expandFilename() const { return myExpandFilename; }
    bool        intrinsicExpandFilename(const GU_PrimPacked *prim) const { return expandFilename(); }
    void	setExpandFilename(GU_PrimPacked *prim, bool b)
		{
		    myExpandFilename = b;
		    makeFilename(prim);
		}
    /// @}

    static exint getCacheVersion(); 

    /// Load the point geometry into the given detail
    GU_ConstDetailHandle	 getPointCloud() const;

    class GU_API CacheItem
    {
    public:
	CacheItem()
	    : myFilename()
	    , myResolved(false)
	{
	}
	CacheItem(const UT_StringHolder &filename)
	    : myFilename(filename)
	    , myResolved(false)
	{
	}
	CacheItem(const CacheItem &src)
	    : myFilename(src.myFilename)
	    , myResolved(false)
	{
	}
	~CacheItem()
	{
	    clear();
	}

	/// Assignment operator
	CacheItem	&operator=(const CacheItem &src)
	{
	    if (&src != this)
	    {
		clear();
		setFilename(src.myFilename);
	    }
	    return *this;
	}

	/// Clear the item (possibly releasing from the cache)
	void	clear()
	{
	    if (myResolved)
	    {
		releaseFromCache(myFilename);
		myResolved = false;
	    }
	}

	/// Check to see if the cache would be dirtied with the given filename
	bool	isDifferentFile(const char *filename) const;

	/// Change the filename.  Returns true if the filename changed
	void	setFilename(const UT_StringHolder &filename)
	{
	    if (filename != myFilename)
	    {
		clear();
		myFilename = filename;
	    }
	}

	/// Access the file
	GU_ConstDetailHandle	get(const GU_PackedImpl &impl) const
	{
	    return GU_PackedDisk::loadFromCache(myFilename, myResolved, impl);
	}

	/// Get point cloud for file
	GU_ConstDetailHandle	getPointCloud(const GU_PackedImpl &impl) const
	{
	    return GU_PackedDisk::loadPointCloudFromCache(myFilename,
		    myResolved, impl);
	}

	/// Get bounding box
	bool	getBounds(UT_BoundingBox &box) const
	{
	    return GU_PackedDisk::loadBoxFromCache(myFilename, myResolved, box);
	}
	/// Get velocity range
	bool	getVelocityRange(UT_Vector3 &vmin, UT_Vector3 &vmax) const
	{
	    return GU_PackedDisk::loadVelocityRangeFromCache(myFilename,
		    myResolved, vmin, vmax);
	}
	/// Get width range
	bool	getWidthRange(fpreal &wmin, fpreal &wmax) const
	{
	    return GU_PackedDisk::loadWidthRangeFromCache(myFilename,
		    myResolved, wmin, wmax);
	}

	/// Check if cache has been loaded from disk
	bool isLoaded() const { return GU_PackedDisk::checkIsLoaded(myFilename, myResolved); }


	/// Accessors
	const UT_StringHolder	&filename() const	{ return myFilename; }

    private:
	UT_StringHolder	myFilename;
	mutable bool	myResolved;
    };
protected:
    template <typename T>
    bool	loadFrom(GU_PrimPacked *prim, const T &options, const GA_LoadMap &map);

    /// Returns if the filename has changed
    void		 makeFilename(GU_PrimPacked *prim);

    CacheItem		myCache;
    UT_StringHolder	myUnexpanded;
    fpreal		myExpandFrame;
    bool		myExpandFilename;

private:
    friend class CacheItem;
    /// Resolve a disk file, using the shared cache for disk files
    /// The bool passed in should be initialized to false.  You @b must call @c
    /// releaseFile() if you no longer reference the file (and the cache can
    /// remove it).  It's a good idea to use @c CacheItem
    static GU_ConstDetailHandle	loadFromCache(const UT_StringRef &filename,
				    bool &resolved,
				    const GU_PackedImpl &impl);

    /// Load point cloud from a disk file, using the shared disk cache.
    /// The bool passed in should be initialized to false.  You @b must call @c
    /// releaseFile() if you no longer reference the file (and the cache can
    /// remove it).  It's a good idea to use @c CacheItem
    static GU_ConstDetailHandle	loadPointCloudFromCache(
				    const UT_StringRef &filename,
				    bool &resolved,
				    const GU_PackedImpl &impl);

    /// Resolve bounding box for a disk file using the shared disk cache.
    /// The bool passed in should be initialized to false.  You @b must call @c
    /// releaseFile() if you no longer reference the file (and the cache can
    /// remove it).  It's a good idea to use @c CacheItem
    static bool	loadBoxFromCache(const UT_StringRef &filename,
				    bool &resolved, UT_BoundingBox &box);

    /// Get the velocity range for a disk file using the shared disk.
    /// The bool passed in should be initialized to false.  You @b must call @c
    /// releaseFile() if you no longer reference the file (and the cache can
    /// remove it).  It's a good idea to use @c CacheItem
    static bool	loadVelocityRangeFromCache(const UT_StringRef &filename,
				    bool &resolved,
				    UT_Vector3 &vmin, UT_Vector3 &vmax);

    /// Get the width attribute range for a disk file using the shared disk.
    /// The bool passed in should be initialized to false.  You @b must call @c
    /// releaseFile() if you no longer reference the file (and the cache can
    /// remove it).  It's a good idea to use @c CacheItem
    static bool	loadWidthRangeFromCache(const UT_StringRef &filename,
				    bool &resolved,
				    fpreal &vmin, fpreal &vmax);

    /// Indicate you no longer need the item in the cache.  It's a good idea to
    /// use @c CacheItem
    static void			releaseFromCache(const UT_StringRef &filename);

    /// Checked if the cached file has been loaded
    static bool checkIsLoaded(const UT_StringRef &filename, bool& resolved);

    static GA_PrimitiveTypeId theTypeId;
};

#endif
