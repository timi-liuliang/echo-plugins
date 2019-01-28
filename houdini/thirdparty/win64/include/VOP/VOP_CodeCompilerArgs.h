/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VOP_CodeCompilerArgs.h ( VOP Library, C++)
 *
 * COMMENTS:	Data for expanding local variables when evaluating the compiler
 *		parameter.
 */

#ifndef __VOP_CodeCompilerArgs__
#define __VOP_CodeCompilerArgs__

#include "VOP_API.h"

#include <UT/UT_WorkBuffer.h>
class UT_StringArray;

enum {
    VOP_VCC_SOURCEFILE,
    VOP_VCC_OBJECTFILE,
    VOP_VCC_ERRORFILE,
    VOP_VCC_INCLUDEPATH,
    VOP_VCC_COMPILER
};

class VOP_API VOP_CodeCompilerArgs {
public:
    VOP_CodeCompilerArgs(const char *src, const char *obj, const char *err,
			 UT_StringArray *incs)
    {
	mySourceFile = src;
	myObjectFile = obj;
	myErrorFile = err;
	myIncDirs = incs;
    }

    const char	*getSource() const	{ return mySourceFile; }
    const char	*getObject() const	{ return myObjectFile; }
    const char	*getError() const	{ return myErrorFile; }
    const char	*getIncludes();

    const char		*mySourceFile;	// Source file
    const char		*myObjectFile;	// Desired object file
    const char		*myErrorFile;	// File for log of errors
    UT_StringArray	*myIncDirs;	// Include directories.
    UT_WorkBuffer	 myWorkBuffer;	// For expanded include paths
};

#endif

