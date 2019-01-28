/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_ThreadedIO.h ( UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_ThreadedIO__
#define __UT_ThreadedIO__

#include "UT_API.h"

#include "UT_Thread.h"
#include "UT_ThreadQueue.h"

class UT_ThreadedIOTask;

///
/// UT_ThreadedIO__ takes care of all the thread administration that 
/// threaded io requires.  
///
/// Its features are to maintain single global IO thread
/// to which all desired write jobs are posted.
/// 
class UT_API UT_ThreadedIO
{
public:
    UT_ThreadedIO(exint maxqueuebytes);
    ~UT_ThreadedIO();

    /// Blocks until next IO task is done.  Returns the number of
    /// tasks left.
    /// Will return immediatenly if no active or pending tasks.
    int	 	 awaitIOTaskDone();

    /// Current task count.  Includes those blocking waiting to
    /// be added.  Includes those still processing.
    int		 numTasks() const;
    
    /// Adds the task to the IO queue.
    /// This will block until there is enough free space
    /// on the queue.
    void	 postTask(UT_ThreadedIOTask *task);

private:
    static void *threadCB(void *data);
    void	 exec();

    UT_ThreadQueue<UT_ThreadedIOTask *> 	myQueue;
    UT_Thread	 *myIOThread;

    SYS_AtomicInt32		myTaskCount;

};

///
/// IO tasks should be subclassed from this.
/// Your destructor will be called on the io thread!
///
class UT_API UT_ThreadedIOTask
{
public:
    UT_ThreadedIOTask() {}
    virtual ~UT_ThreadedIOTask() {}

    /// This is invoked when it is your turn to write.
    /// when you exit from this, your task will be destructed.
    virtual void	doWrite() = 0;

    /// A rough estimate of the number of bytes of RAM are
    /// lost by waiting for this task to finish.  This avoids
    /// us spooling up too much and swapping.
    virtual exint	memoryEstimate() const = 0;
};

UT_API UT_ThreadedIO *UTgetIOThread();


#endif
