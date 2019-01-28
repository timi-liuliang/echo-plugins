/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_SharedMemoryManager.h ( UT Library, C++)
 *
 * COMMENTS:
 *
 */

#ifndef __UT_SharedMemoryManager__
#define __UT_SharedMemoryManager__

#include "UT_API.h"

#include "UT_Map.h"
#include "UT_StringMap.h"
#include "UT_SpinLock.h"

class SYS_SharedMemory;

class UT_API UT_SharedMemoryManager
{
public:
    static UT_SharedMemoryManager &get();


    // Retrievs an existing memory block, or creates an empty one and marks
    // it as being used.
    SYS_SharedMemory	*get(const char *key);

    // Mark all shared memory blocks with keys matching the pattern as being
    // unused. If a block is retrieved with get() it gets marked as used. Any
    // blocks remaining unused after an operation can be erased by calling
    // erase() with 'only_unused' set to true.
    void		 setUnused(const char *pattern);

    // Destroys all shared memory blocks with keys matching the given pattern.
    // 
    void		 erase(const char *pattern, bool only_unused = false);

    // Exit callback to destroy all known shared memory blocks, so they don't
    // linger on-disk or in page file after the owning process dies.
    static void	exitCallback(void *);
private:
     UT_SharedMemoryManager();
    ~UT_SharedMemoryManager();

    // Finds a list of keys matching the pattern. If all keys match the pattern
    // the array is empty but the function still returns true.
    bool		find(const char *pattern, UT_Array<const char *> &keys);
    
    static UT_SpinLock		  theInstanceLock;
    static UT_SharedMemoryManager *theInstance;

    struct ut_AutoSharedMemory
    {
	ut_AutoSharedMemory() : mySharedMemory(0), myUsed(false) {}
	~ut_AutoSharedMemory();
	
	SYS_SharedMemory	*mySharedMemory;
	bool			 myUsed;
    };

    typedef UT_StringMap<ut_AutoSharedMemory>	 ut_SharedMemPool;

    ut_SharedMemPool		mySharedMemPool;
};

#endif // __UT_SharedMemoryManager__
