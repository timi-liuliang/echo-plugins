/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	TS_KernelList.h ( TS Library, C++)
 *
 * COMMENTS:	Kernel definitions for TS library.  This class provides a
 *		mechanism to extend the kernel definitions as well as to access
 *		the definitions.
 *
 * WARNING:	Each kernel must begin with a unique character.  This
 *		restriction may be removed in the future.  The kernel tokens
 *		are case sensitive.
 */

#ifndef __TS_KernelList__
#define __TS_KernelList__

#include "TS_API.h"
#include <UT/UT_ValArray.h>

class TS_MetaKernel;

//
// This is the file where you should install custom kernel functions.  For
// example, install this file in $HOME/houdiniX.X/MetaKernels where X.X is your
// Houdini major.minor version.
// The file should contain the path to the kernel table where your .so/.dll
// file is located relative to the search paths in the HOUDINI_DSO_PATH
// environment variable. For example:
//
//	# Kernel extension table
//	TS_MyKernel.so
//
// In this case, you could install the TS_MyKernel.so file in
// $HOME/houdiniX.X/dso.  The newMetaKernel() function should look like:
//	void newMetaKernel()
//	{ TS_KernelList::getList()->addKernel(&myKernel); }
//
#define TS_KERNEL_TABLE_FILE	"MetaKernels"

extern "C" {
    SYS_VISIBILITY_EXPORT extern void	newMetaKernel();
};

class TS_API TS_KernelList
{
public:
    static TS_KernelList	*getList();

    int			 getEntries() const	{ return myKernels.entries(); }
    TS_MetaKernel	*getKernel(int i);
    TS_MetaKernel	*getKernel(const char *token);

    // Unlike getKernel() findKernel() will return NULL if there's no matching
    // kernel
    TS_MetaKernel	*findKernel(const char *token);

    void		 addKernel(TS_MetaKernel *kernel);
    void		 removeKernel(TS_MetaKernel *kernel);
    void		 removeKernel(const char *token);
private:
     TS_KernelList();
#ifdef LINUX
    public:
    ~TS_KernelList();
    private:
#else
    ~TS_KernelList();
#endif

    UT_ValArray<TS_MetaKernel *>	myKernels;
};

#endif
