/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_FileList.h ( UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef _UT_FILELIST_H_
#define _UT_FILELIST_H_

#include "UT_API.h"
#include "UT_String.h"
#include "UT_StringArray.h"
#include "UT_VarScan.h"

class UT_API UT_FileList
{
public:
    UT_FileList();
    // UT_FileList( UTVarCallback lookup, void *userdata );

    int		 entries() const 		{ return myFiles.entries(); }
    UT_StringHolder operator()(int i) const 	{ return myFiles(i); }

    void	 append(const UT_String &file)  { myFiles.append(file); }

    int		 addRangeOfFiles(const char *filePattern, const int min,
				 const int max, const int inc);
    void	 sortFiles();
    void	 reverse();

private:
    static const char	*static_lookup(const char *variable, void *userdata);

    UT_StringArray  myFiles;
    int		    myFrameNumber;
    char	    myFrameText[40];
    UTVarCallback   myExtraLookup;
    void *	    myUserData;
};

#endif
