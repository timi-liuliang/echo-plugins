/*
* PROPRIETARY INFORMATION.  This software is proprietary to
* Side Effects Software Inc., and is not to be reproduced,
* transmitted, or disclosed in any way without written permission.
*
* NAME:	UT_TempFileManager.h (UT Library, C++)
*
* COMMENTS:
*
*/

#ifndef __UT_TempFileManager__
#define __UT_TempFileManager__

#include "UT_API.h"
#include "UT_StringArray.h"
#include "UT_StringHolder.h"

#include <stdio.h>

class UT_TempFileManager;
class UT_OFStream;

class UT_TempFileManager
{
public:
    UT_TempFileManager();
    ~UT_TempFileManager();

    // Create a temp file that will be deleted when the process exits
    // and return the file ptr to it.
    UT_API static FILE
	    *createTempFile(UT_StringHolder &filename,
			    const char *prefix = nullptr);

    // Create a temp file that will be deleted when the process exits
    // and return a stream pointer to it.  The caller assumes all ownership.
    UT_API static UT_OFStream
	    *createTempFileStream(UT_StringHolder &filename,
				  const char *prefix = nullptr);

    // Delete a temp file that was previously created.
    UT_API static void		 removeTempFile(const char *filename);

    // Get a temporary filename of a file that will be deleted when the
    // process exits.
    UT_API static UT_StringHolder
	     getTempFilename(const char *prefix = nullptr);

    // Get an array of all managed temp files.
    UT_API static const UT_StringArray &getAllTempFilenames();

private:
    UT_API static UT_TempFileManager	*getInstance()
    {
	if (!theInstance)
	    theInstance = new UT_TempFileManager();
	return theInstance;
    }

    UT_StringArray			 myTempFilenames;
    static UT_TempFileManager		*theInstance;
};

#endif
