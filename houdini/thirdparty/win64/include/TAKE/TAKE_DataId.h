/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	TAKE_DataId.h ( TAKE Library, C++)
 *
 * COMMENTS:	An identifier object for TAKE_Data
 */

#ifndef __TAKE_Hash__
#define __TAKE_Hash__

#include "TAKE_API.h"

#include <UT/UT_StringHolder.h>
#include <SYS/SYS_Hash.h>

class TAKE_API TAKE_DataId 
{
public:
    TAKE_DataId(int opid, const UT_StringHolder &label)
    {
	myLabel = label;
	myOpId  = opid;
    }
    virtual ~TAKE_DataId() {}

    bool operator==(const TAKE_DataId &key) const
    {
	return myOpId == key.myOpId && myLabel == key.myLabel;
    }
   
    bool operator!=(const TAKE_DataId &key) const
    {
	return myOpId != key.myOpId || myLabel != key.myLabel;
    }
    
    virtual size_t	hash() const {
	SYS_HashType hash = myLabel.hash();
	SYShashCombine(hash, myOpId);
	return hash;
    }

    virtual int64 getMemoryUsage(bool inclusive) const
    {
        // NOTE: We could try to count myLabel, but it might be
        //       shared by millions of TAKE_Hash objects.
        return inclusive ? sizeof(*this) : 0;
    }

    UT_StringRef getLabel() const	{ return myLabel; }
    int	 	 getOpId() const	{ return myOpId; }

    struct Hasher
    {
	size_t operator()(const TAKE_DataId &k) const
	{
	    return k.hash();
	}
    };
private:
    UT_StringHolder myLabel;
    int		 myOpId;
};

#endif
