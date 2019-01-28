/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_CacheOptions_h__
#define __SIM_CacheOptions_h__

#include "SIM_API.h"
#include "SIM_Utils.h"
#include <UT/UT_String.h>

/// This class stores all the caching options used by the SIM_Cache.
class SIM_API SIM_CacheOptions
{
public:
    /// Constructor sets up default caching options.
				 SIM_CacheOptions();
    /// Destructor for cache options.
				~SIM_CacheOptions();

    /// Returns the name of the initial state file.
    const UT_String		&getInitialState() const;
    /// Sets the initial state file name.
    void			 setInitialState(const char *filename);
    /// Returns whether or not caching is enabled.
    bool			 getIsCacheEnabled() const;
    /// Sets the enable state of the cache.
    void			 setIsCacheEnabled(bool enabled);
    /// Returns whether or not cache entries can be sent to disk.
    bool			 getAllowSaveToDisk() const;
    /// Sets whether or not cache entries can be saved to disk.
    void			 setAllowSaveToDisk(bool allow);
    /// Returns whether or not cache should save substep data.
    bool			 getAllowSaveSubsteps() const;
    /// Sets whether or not cache should save substep data.
    void			 setAllowSaveSubsteps(bool allow);
    /// Returns the maximum memory footprint for all cached data.
    int64			 getMaxCacheMemSize() const;
    /// Sets the maximum allowed memory footprint of the cache (in bytes).
    void			 setMaxCacheMemSize(int64 maxsize);

    /// Determines if we compress our .sim files.  Compression can
    /// be very slow.
    void			 setCompressSims(UT_CompressionType type) { myCompressionType = type; }
    UT_CompressionType	 getCompressSims() const { return myCompressionType; }

    /// Determines if we are overriding the cache files with an
    /// explicit list of cache files.  These will always be saved
    /// when added and only deleted according to history and
    /// checkpoint spacing
    /// This is also used when we are a player. When we are a player
    /// history is set to 0 and the cache files are never deleted.
    void			 setExplicitCache(bool useexplicit,
						    const char *explicitfname,
						    int history,
						    int checkpointspacing);
    bool			 getUseExplicit() const 
    { 
	return myUseExplicit && myExplicitName.isstring(); 
    }
    const UT_String		&getExplicitFname() const { return myExplicitName; }
    int				 getExplicitHistory() const { return myExplicitHistory; }
    int				 getCheckpointSpacing() const { return myExplicitCheckpointSpacing; }

    /// Assignment operator copies options from one instance to another.
    const SIM_CacheOptions	&operator=(const SIM_CacheOptions &src);

private:
    UT_String			 myInitialState;
    int64			 myMaxCacheMemSize;
    bool			 myIsCacheEnabled;
    bool			 myAllowSaveToDisk;
    bool			 myAllowSaveSubsteps;
    bool			 myUseExplicit;
    UT_String			 myExplicitName;
    int				 myExplicitHistory;
    int				 myExplicitCheckpointSpacing;
    UT_CompressionType	 myCompressionType;
};

#endif

