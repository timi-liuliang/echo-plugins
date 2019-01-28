/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_OutputCode.h ( OP Library, C++)
 *
 * COMMENTS: The paramater structure for the output code functions.
 */

#ifndef __OP_OutputCode__
#define __OP_OutputCode__

#include "OP_API.h"
#include <UT/UT_String.h>
#include <UT/UT_ValArray.h>

class OP_API OP_OutputCodeParms
{
public:
    OP_OutputCodeParms();
    ~OP_OutputCodeParms();

    // Take space delimitted list and add all the parms individually:
    void			addFParms(const char *parms);
    void			addIParms(const char *parms);
    void			addPreDefines(const char *defines);

    UT_String			myFncName;
    UT_String			myCFile, myHFile;
    UT_ValArray<char *>		myFParm, myIParm;
    UT_ValArray<char *>		myPreDefines;
    int				myTiming;
    int				myTabSpacing;
    int				myChannelForce;
};

#endif

