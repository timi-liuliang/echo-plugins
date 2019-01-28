/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        MPI_Dependency.h ( Modeling Op-Handle Link Library, C++ )
 *
 * COMMENTS:
 *
 */

#ifndef __UT_Dependency_H__
#define __UT_Dependency_H__

#include "UT_API.h"
#define DECLARE_DEPENDENCY(Library) \
class Library##_API DependencyOn##Library \
{ \
public: \
    DependencyOn##Library(); \
    ~DependencyOn##Library(); \
};

#define IMPLEMENT_DEPENDENCY(Library) \
DependencyOn##Library::DependencyOn##Library() { } \
DependencyOn##Library::~DependencyOn##Library() { }

#endif
