/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SYS_SharedMemory.h ( UT Library, C++)
 *
 * COMMENTS:
 *
 */

#ifndef __SYS_SharedMemory__
#define __SYS_SharedMemory__

#include "SYS_API.h"
#include "SYS_Types.h"

#include <iostream>

class SYS_SharedMemoryImpl;
class SYS_SharedMemoryViewImpl;
class SYS_SharedMemoryStreamBuffer;

/// Creates a read-only shared memory object, that accesses an existing shared
/// memory buffer, using a unique identifier.
class SYS_API SYS_SharedMemory
{
public:
    /// Create a new shared memory block of a given size. 
    SYS_SharedMemory(exint size = 0);

    /// Open an existing shared memory block through the given identifier.
    SYS_SharedMemory(const char *id, bool read_only=true);
     
    ~SYS_SharedMemory();

    /// On some operating systems, shared memory is handled using a file system
    /// interface.  On these systems, we need to know where the temporary files
    /// should be installed.  This is typically in HOUDINI_TEMP_DIR.
    ///
    /// Note: This is not thread safe, and should only be done once
    static void	setTempDir(const char *path);

    /// Resets the shared memory to the given size. This has the effect of 
    /// removing the previous shared memory segment (unless it's already open by
    /// someone else, in which case it gets deleted when that user closes it).
    /// This operation will also result in a new id() being generated to avoid
    /// a potential conflict with the potentially still open shared memory
    /// section.
    /// The shared memory cannot be reset if it was opened from an existing
    /// identifier.
    bool	 reset(exint size);
    
    /// Returns the identifier of the shared memory segment, such that it can
    /// be opened from another process. The pointer is valid as long as this
    /// object is.
    const char 	*id() const;

    /// Returns the size of the shared memory, in bytes.
    exint 	 size() const;

    /// Returns true if this shared memory is read-only.
    bool	 readOnly() const;

    /// Returns an approximation of the memory used by these structures,
    /// NOT the shared memory itself.
    int64	 getMemoryUsage(bool inclusive) const;

private:
    friend class SYS_SharedMemoryView;
    SYS_SharedMemoryImpl	*myImpl;
};

class SYS_API SYS_SharedMemoryView
{
public:
    // Initialize a new shared memory view from a given shared memory. The
    // offset and size will be truncated to fit inside the size of the
    // shared memory.
    SYS_SharedMemoryView(SYS_SharedMemory &shm,
			 exint offset = 0, exint size = SYS_EXINT_MAX);
    
    ~SYS_SharedMemoryView();

    // Sets a new view on the shared memory data.
    void	 setView(exint offset = 0, exint size = SYS_EXINT_MAX);

    // Returns the offset into the shared memory that this view represents.
    exint	 offset() const;

    // Returns the size of this shared memory view. If the size is 0, then
    // the view is not valid.
    exint	 size() const;

    // Returns true if the shared memory this view applies to is read only.
    bool	 readOnly() const;

    // Returns a pointer to the data represented by this view. If the view is
    // not valid, a NULL pointer is returned.
    // Note: If the view is marked read-only, a write to the data pointed to
    // will very likely cause a crash.
    void	*data() const;

    // This can be called to synchronize the data with the underlying
    // representation while keeping the view open.
    void	 sync();
    
    /// Returns an approximation of the memory used by these structures,
    /// NOT the shared memory itself.
    int64	 getMemoryUsage(bool inclusive) const;

private:
    SYS_SharedMemoryViewImpl	*myImpl;
};

// An adapter for std::istream that can be used to read from a shared memory
// buffer in an iostream context.
class SYS_API SYS_SharedMemoryInputStream : public std::istream
{
public:
    SYS_SharedMemoryInputStream(SYS_SharedMemory &shm, exint block_size = 0);
    ~SYS_SharedMemoryInputStream();

    exint blockSize() const;
private:
    SYS_SharedMemoryStreamBuffer *myBuf;
};

// An adapter fro std::ostream that can be used to write to a shared memory
// buffer in an iostream context. It has two modes of operation, one which can
// be initialized with a NULL buffer to simply count how many bytes would have
// been written to the buffer and a second mode where a shared memory, of the
// appropriate size, is supplied to write to.
class SYS_API SYS_SharedMemoryOutputStream : public std::ostream
{
public:
    SYS_SharedMemoryOutputStream(SYS_SharedMemory *shm, exint block_size = 0);
    ~SYS_SharedMemoryOutputStream();

    exint size() const;
    exint blockSize() const;
private:
    SYS_SharedMemoryStreamBuffer *myBuf;
};

#endif // __SYS_SharedMemory__
