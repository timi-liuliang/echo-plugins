/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_FMalloc.h ( UT Library, C++)
 *
 * COMMENTS:	Disk file based malloc.
 *
 *	Memory is allocated and freed using fmalloc/fcalloc and ffree.
 *	In order to read from the memory, you must call access().  This locks
 *	the memory pages in RAM.
 *	When the data has been modified, it should be flushed to disk using the
 *	write() method.
 *	When you no longer need to access the memory, close() should be called.
 *	This will possibly free up RAM so that it can be used by other blocks.
 *	
 */

#ifndef __UT_FMalloc__
#define __UT_FMalloc__

#include "UT_API.h"

#include <stdio.h>
#include <SYS/SYS_Types.h>

class ut_FMemoryPage;

#define UT_FMALLOC_NBUCKETS	39

class UT_API UT_FMalloc {
public:
     UT_FMalloc(const char *filename);
    ~UT_FMalloc();

    /// Allocate memory from the disk.  This creates a memory block which can
    /// be accessed at a later time by locking the page in memory.
    ut_FMemoryPage	*fmalloc(size_t size);

    /// Free a block of memory.  This allows the block to be re-used.  If a
    /// locked page is freed, it's contents will not be written to disk
    /// regardless of it's read/write status.
    void		 ffree(ut_FMemoryPage *mem);

    /// Lock the memory in RAM.  If the page is marked as read-write, when it's
    /// unlocked, it will be re-flushed to disk (regardless of whether the
    /// block was actually modified).  If it's not flagged as read-write, the
    /// in-core memory can be modified without changing the state of the disk
    /// contents.
    void	*lock(ut_FMemoryPage *mem, bool readwrite=true);

    /// If the page was locked as read-only, you can change the status to write
    /// by calling modify.  Alternatively, you can abort the write to disk by
    /// passing readwrite=false.
    void	 modify(ut_FMemoryPage *mem, bool readwrite=true);

    /// Unlocking the page will release the memory associated with the page.
    /// If the memory has been modified, the page will be written to disk
    /// (unless the modified flag has been turned off).
    void	 unlock(ut_FMemoryPage *mem);

    /// Query methods for fmalloc.  In theory, you probably don't need these,
    /// but it might be handy.
    /// @{
    /// The file may not be allocated until a page is actually written to disk,
    /// so querying the file pointer may return a null pointer.
    FILE	*getFile()	{ return myFile; }
    size_t	 getSize(ut_FMemoryPage *mem);
    int64	 getSeek(ut_FMemoryPage *mem);
    /// This method will return a null ptr if there is no memory locked to the
    /// current page.  This can be used to find out the state of the page.
    void	*getLockedMemory(ut_FMemoryPage *mem);
    /// @}

    /// This will garbage collect the disk file.  All used pages will be
    /// compacted to a better memory layout.
    void	 compactTrash();

    /// Get the state of memory fragmentation.  There's no metric defined for
    /// this yet.
    int		 getEntropy() const	{ return myEntropy; }

    /// Debug builds only to dump statistics on memory usage.  Please see
    /// UT_Memory for additional information.
    void	 dumpStats();

private:
    // The free memory bins
    ut_FMemoryPage	*myBins[UT_FMALLOC_NBUCKETS];

    // Pointers to used memory blocks (required during compacting)
    char		*myPath;
    FILE		*myFile;

    // The entropy is a measure of how much fragmentation there is in the disk
    // file.  This can be used for garbage collection of the used pages.
    int			 myEntropy;
    int64		 myFileSize;
    int			 myBlockCount;
};

#endif
