/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __SIM_RootDataId_h__
#define __SIM_RootDataId_h__

#include "SIM_API.h"

#include <UT/UT_StringHolder.h>
#include <SYS/SYS_Hash.h>

class SIM_API SIM_RootDataId
{
public:
    SIM_RootDataId() { myId = -1; }
			 
    explicit SIM_RootDataId(exint id) : myId(id) {}
    explicit SIM_RootDataId(const UT_StringHolder &name) : myId(-1), myName(name) {}
    
			 SIM_RootDataId(const SIM_RootDataId &src) = default;

    UT_StringHolder 	 getName() const
			 { return myName; }
    int			 getId() const
			 { return myId; }

    void		 clear()
			 { myId = -1; myName = ""; }
    bool		 getIsClear() const
			 { return (myId == -1 && !myName.isstring()); }

    bool operator==(const SIM_RootDataId &other) const
    {
	return myId == other.myId && myName == other.myName;
    }
    
    bool operator!=(const SIM_RootDataId &other) const
    {
	return myId != other.myId || myName != other.myName;
    }

    SYS_HashType hash() const
    {
	SYS_HashType hash = myName.hash();
	SYShashCombine(hash, myId);
	return hash;
    }

private:
    UT_StringHolder	 myName;
    exint		 myId;
};

inline SYS_HashType hash_value(const SIM_RootDataId &id) { return id.hash(); } 

#endif
