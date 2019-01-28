/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	FS_DiskCache.h (UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __FS_DiskCache__
#define __FS_DiskCache__

#include "FS_API.h"
#include <SYS/SYS_AtomicInt.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_WorkBuffer.h>
#include <UT/UT_Access.h>

class UT_JSONWriter;

/// A disk cache can store data on disk.  The cache is designed to be
/// thread-safe, even across multiple processes.
///
/// This class is intended to be a singleton.
class FS_API FS_DiskCache 
{
public:
    /// The key is composed of two main parts:
    /// 1)  The value is used to uniquely identify the data in the file.  This
    ///	    value is hashed to compute the filename in the cache and can be
    ///	    arbitrarily long.
    ///	2)  The suffix is a short identifier - The first 32 characters of this
    ///	    suffix are appended to the value hash to create the filename.
    /// The key will automatically add the current major.minor release of the
    /// software (but not the build).
    class FS_API Key
    {
    public:
	Key(const UT_StringRef &value = UT_StringRef(),
		const UT_StringRef &suffix = UT_StringRef())
	{
	    computeFilename(value, suffix);
	}
	const UT_StringHolder	&filename() const { return myFilename; }
    private:
	void	computeFilename(const UT_StringRef &value,
				const UT_StringRef &suffix);
	UT_StringHolder	myFilename;
    };

    /// The Accessor class is used to access items in the cache.  When an
    /// accessor is in write-mode, the thread/process will have unique access
    /// to the cache item until the Accessor is destructed.  Multiple
    /// threads/processes can have read-only access to the item.
    class FS_API Accessor : public UT_NonCopyable
    {
    public:
	Accessor()
	    : myCache(nullptr)
	    , myFilename()
	    , myBytesWritten(0)
	    , myFd(-1)
	    , myAccess(0)
	    , myLocked(false)
	    , myStoreInCache(false)
	{
	}
	~Accessor()
	{
	    clear();
	}

	/// Clear the accessor.  This will close any file descriptors and
	/// release any locks.
	void	clear();

	/// Write data to the file.  This method can be called multiple times.
	bool	write(const void *data, exint len);

	/// Convenience methods to write data to the cache
	bool	write(const UT_WorkBuffer &buf)
		    { return write(buf.buffer(), buf.length()); }

	/// Return the number of bytes written
	exint	bytesWritten() const { return myBytesWritten; }

	/// Read the contents of the stream.
	bool	read(UT_WorkBuffer &contents) const;

	/// Whether the accessor is valid
	bool	isValid() const { return myFd >= 0; }

	/// Whether the accessor was opened as read-only
	bool	isRead() const { return myAccess & R_OK; }

	/// Whether the accessor was opened for writing
	bool	isWrite() const { return myAccess & W_OK; }

	/// Return the full path of the file in the cache
	const UT_StringHolder	&filename() const { return myFilename; }

	/// Normally, accessors that write data are counted to the cache
	/// storage.  This can disable this behaviour.
	void	storeInCache(bool v) { myStoreInCache = v; }

	void	dump() const;
	void	dump(UT_JSONWriter &w) const;

    private:
	/// Return the size of the read stream (or 0 if there was an error)
	exint	fileSize() const;

	/// @private - used solely by the FS_DiskCache to initialize
	friend class FS_DiskCache;	// Provide access to open()

	const FS_DiskCache	*myCache; // Parent cache
	UT_StringHolder		 myFilename;
	exint			 myBytesWritten;
	int			 myFd; // File descriptor
	int			 myAccess; // Access mode
	bool			 myLocked;
	bool			 myStoreInCache;
    };

    /// Statistics for the cache
    class FS_API Stats
    {
    public:
	Stats()
	{
	    fullClear();
	}
	~Stats() {}

	void	clearStats()
	{
	    myCacheHits = 0;
	    myCacheMiss = 0;
	    myCacheError = 0;
	    myFileCount = 0;
	    myDiskSize = 0;
	    myPurgeCount = 0;
	}
	void	fullClear()
	{
	    clearStats();
	    myMaxSize = 0;
	    myMaxLogSize = 0;
	    myCreateTime = 0;
	    myLastRecompute = 0;
	}

	/// Merge stats from a different set of stats
	void	merge(const Stats &s)
	{
	    myCacheHits += s.myCacheHits;
	    myCacheMiss += s.myCacheMiss;
	    myCacheError += s.myCacheError;
	    myFileCount += s.myFileCount;
	    myDiskSize += s.myDiskSize;
	    myPurgeCount += s.myPurgeCount;
	}

	/// @{
	/// Access statistics
	int64	cacheHits() const	{ return myCacheHits; }
	int64	cacheMiss() const	{ return myCacheMiss; }
	int64	cacheError() const	{ return myCacheError; }
	int64	purgeCount() const	{ return myPurgeCount; }
	int64	fileCount() const	{ return myFileCount; }
	int64	diskSize() const	{ return myDiskSize; }
	int64	maxSize() const		{ return myMaxSize; }
	int64	maxLogSize() const	{ return myMaxLogSize; }
	time_t	createTime() const	{ return myCreateTime; }
	time_t	lastRecompute() const	{ return myLastRecompute; }
	/// @}

	/// Load stats from a given file descriptor
	bool	loadFrom(const Accessor &fd);
	/// Write stats to the file descriptor
	bool	saveTo(Accessor &a) const;

	/// Update stats with a cache hit
	void	addCacheHit() { myCacheHits++; }

	/// Update stats from the given accessor
	void	addToCache(const Accessor &a);

	/// Purge from the given accessor
	int64	rmFromCache(const Accessor &a, const char *filepath);

	/// Reset the file count and size
	void	reset(int64 nfiles, int64 size)
	{
	    myFileCount = nfiles;
	    myDiskSize = size;
	}

	void	dump() const;
	void	dump(UT_JSONWriter &w) const;
    private:
	int64	myCacheHits;	// Cache hits
	int64	myCacheMiss;	// Cache misses
	int64	myCacheError;	// Cache errors
	int64	myFileCount;	// Files in cache
	int64	myPurgeCount;	// Files removed from cache
	int64	myDiskSize;	// Size of cache

	friend class FS_DiskCache;
	UT_StringHolder myCachePath; // Path to the cache
	int64		myMaxSize; // Maximum cache size
	int64		myMaxLogSize; // Maximum size for log file
	time_t		myCreateTime; // Time cache was created
	time_t		myLastRecompute; // Time cache was last recomputed
    };

    FS_DiskCache(const UT_StringHolder &path=UT_StringHolder());
    ~FS_DiskCache();

    /// Check to see if the cache is valid
    bool	isValid() const	{ return myPath.isstring(); }

    /// Initialize the disk cache from a given path.  If the path doesn't
    /// exist, the cache will be created.
    bool	initialize(const UT_StringHolder &path);

    /// Set the maximum data size of the cache
    bool	setMaxSize(int64 size_in_bytes = 256*1024*1024);

    /// Set the maximum log size of the cache
    bool	setMaxLogSize(int64 size_in_bytes = 1024*1024);

    /// Return the maximum size of the cache
    int64	maxSize() const { return myMaxSize; }

    /// Return the maximum size of the log file
    int64	maxLogSize() const { return myMaxLogSize; }

    /// Get statistics for the cache
    bool	stats(Stats &stats) const;

    /// Return the location of the path
    const UT_StringHolder	&location() const { return myPath; }

    /// Reset the cache
    bool	recompute();

    /// Clear the cache contents
    bool	clearCache() const;

    /// Reset the cache entirely
    bool	clobberCache();

    /// Possible return values for the @c insert() function
    enum InsertStatus
    {
	INSERT_CREATE,	///< New item, accessor has write-only file
	INSERT_EXIST,	///< Existing item, accessor has read-only file
	INSERT_ERROR,	///< Unknown error, accessor will be invalid
    };

    /// Similar to UT_ConcurrentHash, this method will try to insert an item
    /// into the cache.  If the item exists in the cache, @c insert() will
    /// return false and the @c Accessor will have read-only access to the
    /// item.
    InsertStatus	insert(Accessor &access, const Key &key);

    /// Find an item in the disk cache.  If found, the Accessor will have
    /// read-only access to the item.  The item cannot be deleted from the
    /// cache while it's referenced.
    bool		find(Accessor &access, const Key &key) const;

    /// Purge items from a cache to meet size requirements using LRU
    bool		purge(bool force=false) const;

    /// Dump debug information about the cache
    void		dump() const;
    bool		dump(UT_JSONWriter &w) const;

    void		printLog(const char *fmt, ...) const
			    SYS_PRINTF_CHECK_ATTRIBUTE(2, 3);

    template<typename... Args>
    void		log(const char *fmt, const Args &...args) const
    {
	UT_WorkBuffer	msg;
	msg.format(fmt, args...);
	printLog("%s", msg.buffer());
    }

private:
    void		cacheFilePath(Accessor &a, const Key &k,
				const FS_DiskCache *cache) const;

    void	initStats(Stats &s) const
    {
	s.myCachePath = myPath;
	s.myMaxSize = myMaxSize;
	s.myMaxLogSize = myMaxLogSize;
	s.myCreateTime = myCreateTime;
	s.myLastRecompute = myLastRecompute;
    }
    void	statPath(const char *filename, UT_WorkBuffer &path) const;
    void	statPath(int bucket, UT_WorkBuffer &path) const;
    void	statPath(const Accessor &a, UT_WorkBuffer &path) const
		    { statPath(a.filename(), path); }

    friend class Accessor;	// Accessor needs to call cacheMiss()
    void	cacheHit(const Accessor &a) const;
    void	cacheMiss(const Accessor &a) const;
    void	cachePurge(const Accessor &a, const char *filepath) const;


    void	setDefaults();
    bool	saveConfig() const;
    bool	loadConfig();
    bool	initSubdir(const UT_StringRef &path) const;
    bool	saveStats(const char *path, const Stats &s) const;

    InsertStatus	writeLock(Accessor &access, const char *path,
				bool must_create, bool in_cache) const;
    bool		readLock(Accessor &access, const char *path) const;

    UT_StringHolder		myPath;
    int64			myMaxSize;
    int64			myMaxLogSize;
    time_t			myCreateTime;
    time_t			myLastRecompute;
    mutable SYS_AtomicCounter	myCurrSize;
};

#endif
