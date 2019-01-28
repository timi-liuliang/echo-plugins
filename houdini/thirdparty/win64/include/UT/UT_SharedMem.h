/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_SharedMem.h (C++)
 *
 * COMMENTS:
 *   Typical use is as follows:
 *
 *	UT_SharedMem	 ipc(IPC_PATHNAME);
 *	void		*mem;
 *
 *	if( !(mem = ipc.getMemory()) )
 *	{
 *	    ipc.setSize(SHARED_MEM_SIZE);
 *	    mem = ipc.getMemory();
 *	    if( !mem ) abortOrSomething();
 *	}
 *
 *
 *
 */
#ifndef __UT_SharedMem__
#define __UT_SharedMem__

#include "UT_API.h"
#include <sys/types.h>
#include "UT_String.h"

#ifdef WIN32
    typedef void * key_t;
#endif

class UT_API UT_SharedMem
{
public:
//
//   This constructor is for using non-private shared memory segments
//   and will create a shared memory key using the path and the id value.
//   The named file should exist and is used only to create a unique
//   shared memory key. "size" is the size of the shared memory segment
//   in bytes. If size is non-zero this object will try to create the
//   memory, otherwise it is assumed to already exist. If no arguments
//   are given then the user must later call setKey and setSize for public
//   memory or just setSize for private.
//
     explicit UT_SharedMem(const char *keypath=0, int size=0, int keyid=0);

//
//   This constructor is for using non-private shared memory segments
//   with a given key. If size is non-zero this object will try to
//   create the memory, otherwise it is assumed to already exist.
//
     UT_SharedMem(key_t key, int size);

//
//   This constructor will create a private shared memory segment.
//
     explicit UT_SharedMem(int size);

//
//   The destructor will detach the shared memory segment and if we
//   were the creators it will also delete the segment.
//
    virtual ~UT_SharedMem();

//
//  setKey will return 1 upon success and 0 upon failure.
//  It will only fail if the memory is already attached.
//
    int		setKey(const char *path, int id=0);
    int		setKey(key_t key);
#ifdef WIN32
    int		setKey(int key);
#endif

//
//  setSize will return 1 upon success and 0 upon failure.
//  It will only fail if the memory is already attached.
//
    int		setSize(int size);

//
//  detach will disassociate the shared memory segment from this process.
//  It returns 1 upon success and 0 if there was no memory to detach.
//
    virtual int		detach();

//
//  destroy will detach from the memory and then attempt to remove the
//  shared memory segment from the system.
//
    virtual int		destroy();

//
//  getMemory will return a pointer to the shared memory segment.
//  If this method returns zero then the global "errno" should indicate
//  the reason for the failure.
//
    virtual void	*getMemory( int reuse = 1 );


protected:
    UT_String	 ourKeyPath;
    int		 ourKeyId;

private:
    int		 mySize;
    key_t	 myShmKey;
    key_t	 myShmId;
    void	*myMemory;
};

#endif /* __UT_SharedMem__ */
