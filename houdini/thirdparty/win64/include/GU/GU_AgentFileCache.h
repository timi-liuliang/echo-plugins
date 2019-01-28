/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_AgentFileCache.h (GU Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GU_AgentFileCache__
#define __GU_AgentFileCache__

#include "GU_API.h"

#include <UT/UT_FileStat.h>
#include <UT/UT_IntrusivePtr.h>
#include <UT/UT_Lock.h>
#include <UT/UT_StringMap.h>
#include <UT/UT_WorkBuffer.h>

template <typename T>
class GU_API GU_AgentFileCache
{
public:
    /// Searches for a cached file with the same key, and returns it if the
    /// timestamp matches the file on disk. Otherwise, the callback is invoked
    /// to load the file, which is then inserted into the cache and returned.
    /// The key should include the filename and any dependencies on other agent
    /// files (e.g. when loading the same clip for slightly different rigs).
    template <typename F>
    UT_IntrusivePtr<T> findCachedFileOrLoad(const UT_StringRef &filename,
                                            const UT_StringRef &key,
                                            F load_new_item);

    /// Removes a cached file with the given key.
    void removeCachedFile(const UT_StringRef &key);

private:
    struct CachedFile
    {
        CachedFile(T *ptr = nullptr, time_t mod_time = 0)
            : myPtr(ptr), myModTime(mod_time)
        {
        }

        T      *myPtr;
        time_t  myModTime;
    };

    UT_Lock                     myMutex;
    UT_StringMap<CachedFile>    myFiles;
};

/// Builds a list of paths to use when attempting to load an external file,
/// ordered by priority.
GU_API extern void GUgetAgentFilePaths(UT_StringArray &paths,
                                       const UT_StringHolder &condensed_path,
                                       const UT_StringHolder &expanded_path);

template <typename T>
template <typename F>
UT_IntrusivePtr<T>
GU_AgentFileCache<T>::findCachedFileOrLoad(const UT_StringRef &filename,
                                           const UT_StringRef &key,
                                           F load_new_item)
{
    UT_FileStat info;
    time_t mod_time = 0;
    if (UTfileStat(filename, &info) == 0)
        mod_time = info.myModTime;

    UT_Lock::Scope lock(myMutex);

    auto item = myFiles.find(key);
    if (item != myFiles.end() && item->second.myModTime == mod_time)
    {
        return item->second.myPtr;
    }
    else
    {
        auto new_item = load_new_item();
        if (new_item)
            myFiles[key] = CachedFile(new_item.get(), mod_time);

        return new_item;
    }
}

template <typename T>
void
GU_AgentFileCache<T>::removeCachedFile(const UT_StringRef &key)
{
    UT_Lock::Scope lock(myMutex);
    myFiles.erase(key);
}

#endif
