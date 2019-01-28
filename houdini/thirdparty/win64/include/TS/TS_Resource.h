/* Resource.h
 *
 * Copyright 1992 by Algorithmic Arts, Inc.
 * All Rights Reserved.  Use, disclosure or reproduction of this software 
 * is prohibited without prior express written consent of Algorithmic Arts.
 *
 * $XRevision: 1.1.1.1 $
 *
 */

#ifndef TESSERA_RESOURCE_H
#define TESSERA_RESOURCE_H

#include "TS_API.h"
#include <SYS/SYS_AtomicInt.h>

// Base class for reference counted objects.
class TS_API TS_Resource
{
public:
	     TS_Resource() : myCount(0) { }
    virtual ~TS_Resource();
    void		ref()		 { myCount.add(1); }
    void		unref()		 { if (!myCount.add(-1)) delete this; }
    int			refCount() const { return myCount.load(); }
private:
    SYS_AtomicInt32	myCount;
};

#endif // TESSERA_RESOURCE_H
