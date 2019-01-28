/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_FileMonitor.h	    UT library (C++)
 *
 * COMMENTS:	Check if files in a given directory have been changed.
 *
 *
 */
 
#ifndef __UT_FileMonitor_h__
#define __UT_FileMonitor_h__

#include "UT_API.h"
#include "UT_StringArray.h"
#include "UT_Array.h"
#include "UT_IntArray.h"
#include "UT_Lock.h"
#include "UT_Signal.h"
#include "UT_Thread.h"


class UT_FileMonitor;
class UT_String;
class UT_Thread;

typedef bool ( *UTmonitorFunc )( void *op , const char* filename );

class UT_API UT_FileMonitor
{
public:
    UT_FileMonitor();
    virtual ~UT_FileMonitor();
 
    // add file to the monitor list 
    bool                 addFile( const char *filename, 
				  UTmonitorFunc func, void *data );
    // remove file from the monitor list
    bool                 removeFile( void *data , const char* filename=0 );

    const UT_StringArray &getFileList() const { return myMonitoredFiles; } ;
    
    void                 startMyThread();
    void                 stopMyThread();
    
    void                 getDirectory( const char *str, UT_String &dir );
    
    void                 executeCallbacks( const char* file );


protected:
    UT_Array < UTmonitorFunc >          myCallbacks;


private:
    static void*         monitorFiles( void *data );
    
    void                 removeMonitoredFile( const char *filename );
    void                 removeFileEntry( int index );
	
    
    UT_Lock                         myFileLock;
    UT_ValArray < void* >           myOPs;
    UT_StringArray                  myOPFileRoots;
    UT_StringArray                  myOPFiles;     
    UT_StringArray                  myDirList;
    UT_StringArray                  myMonitoredFiles;
    UT_IntArray                     myFHandle;
    UT_IntArray                     myFTime;

    UT_Thread                      *myThread;
    bool                            myQuitThread;
};

UT_API extern UT_FileMonitor	*UTgetFileMonitor();


#endif //__UT_FileMonitor_h__
