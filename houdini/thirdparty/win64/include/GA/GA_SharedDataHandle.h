/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_SharedDataHandle.h
 *
 * COMMENTS:
 */

#ifndef GU_SHAREDDATAHANDLE_H_
#define GU_SHAREDDATAHANDLE_H_

#include "GA_API.h"
#include "GA_PrimitiveDefinition.h"

#include <UT/UT_StringHolder.h>
#include <UT/UT_SharedPtr.h>
#include <UT/UT_Lock.h>
#include <UT/UT_DoubleLock.h>
#include <SYS/SYS_Types.h>

class GA_Primitive;
class GA_SharedDataContext;
class GA_SharedLoadData;
class GA_SharedLoadDataStat;

class UT_JSONParser;


///
///
///
class GA_API GA_SharedDataHandle
{
public:
    /// @param filename Filename to retrieve shared data from
    /// @param offset Offset into file given (usually given from index, -1 if no index)
    /// @param loader The loader to use to retrieve the shared data
    /// Setting the  offset to -1 means that there was no index in the file.
    GA_SharedDataHandle(const UT_StringHolder &filename,
			    exint offset,
			    exint length,
			    const GA_PrimitiveDefinition::SharedDataLoader* loader);
    ~GA_SharedDataHandle();

    /// Forces the loading of the shared data immediately from the
    /// parser given. Used for backwards compatibility with files that
    /// do not have an index.
    void forceLoadSharedData(GA_SharedLoadData* loadedData);

    /// Called by shared data users when they want to retrieve the
    /// loaded data. After calling this function they should clear
    /// Their shared pointer to this object so the loaded data
    /// can be freed.
    const GA_SharedLoadData* resolveSharedData(GA_Primitive* caller);
    const GA_SharedLoadData* resolveSharedData(GA_Primitive* caller, GA_SharedDataContext &context);

    /// Called by shared data users to get metadata on the shared data
    /// that can be resolved though this handle.
    /// Note: This function will return NULL if we have already
    /// resolved our shared data (and hence stating it would not save time)
    const GA_SharedLoadDataStat* statSharedData();

    /// Returns true if we have already resolved our shared data
    bool hasResolvedSharedData() const;

private:
    /// Caller can be null if load is false.
    /// If load is true geometry will be loaded. If not it will be stated
    void parseSharedData(UT_DoubleLock<GA_SharedLoadData*>& lock, GA_Primitive* caller, bool load, GA_SharedDataContext &context);
    void loadOrStatWithParser(UT_DoubleLock<GA_SharedLoadData*>& lock, UT_JSONParser& parser,
	    GA_Primitive* caller, bool load);

    /// Filename of file where shared data is kept
    UT_StringHolder myFileName;

    /// Offset of the shared data into file
    /// If -1 then we have no index
    exint myOffset;

    /// Length of shared data in file
    exint myLength;

    /// Loader to load shared data from file
    const GA_PrimitiveDefinition::SharedDataLoader* myLoader;

    /// Storage of loaded shared data
    /// Will be deleted when shared data handle is destroyed
    GA_SharedLoadData* myLoadedData;

    /// Storage of stated shared data
    /// Will be deleted when shared data handle is destroyed or
    /// shared data is loaded.
    GA_SharedLoadDataStat* myStatedData;

    /// Used to allow resolveSharedData/statSharedData to be safely called from
    /// multiple threads.
    UT_Lock myLock;
};

typedef UT_SharedPtr<GA_SharedDataHandle> GA_SharedDataHandlePtr;

#endif // GU_SHAREDDATAHANDLE_H_
