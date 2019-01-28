/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	OP_ParmCache.h ( OP Library, C++)
 *
 * COMMENTS:
 *	This class handles tracking what the last value of the 
 * parm was & whether it has changed.
 */

#ifndef __OP_ParmCache__
#define __OP_ParmCache__

#include "OP_API.h"
#include <UT/UT_Defines.h>
#include <UT/UT_ValArray.h>
#include <SYS/SYS_Types.h>

class OP_Context;
class OP_Parameters;
class op_ParmCacheEntry;

class OP_API OP_ParmCache
{
public:
    explicit OP_ParmCache(OP_Parameters *node);
    ~OP_ParmCache();

    // This checks if the specified index is dirty.  If it is not known,
    // it is added to our cached values table (but no value is cached yet)
    bool		 isDirty(int idx, fpreal t);

    // This caches all the values that we've built references to.
    void		 updateValues(const OP_Context &context);

    int64		 getMemoryUsage(bool inclusive) const;

private:
    OP_Parameters			*myNode;	// Not _quite_ a node...
    UT_ValArray<op_ParmCacheEntry *>	 myCachedValues;
};

#endif

