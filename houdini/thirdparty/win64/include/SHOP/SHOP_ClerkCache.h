/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SHOP_ClerkCacheEntry.h ( SHOP Library, C++)
 *
 * COMMENTS:	Classes used for allowing clerks to cache information.
 */

#ifndef __SHOP_ClerkCacheEntry__
#define __SHOP_ClerkCacheEntry__

#include "SHOP_API.h"
#include <OP/OP_Node.h>
#include <SYS/SYS_Hash.h>

class SHOP_Clerk;

class SHOP_API SHOP_ClerkCacheKey
{
public:
    SHOP_ClerkCacheKey(const SHOP_Clerk *clerk, OP_Node *shop, 
		       OP_Node *obj, OP_Node *sop)
    {
	myClerk = clerk;
	myShop = shop ? shop->getUniqueId() : -1;
	myObj  = obj  ? obj->getUniqueId()  : -1;
	mySop  = sop  ? sop->getUniqueId()  : -1;
	
	myHash = SYShash(myClerk);
	SYShashCombine(myHash, myShop);
	SYShashCombine(myHash, myObj);
	SYShashCombine(myHash, mySop);
    }

    bool operator==(const SHOP_ClerkCacheKey &key) const
    {
	return myClerk == key.myClerk && myShop == key.myShop &&
	       myObj == key.myObj && mySop == key.mySop;
    }
    
    bool operator!=(const SHOP_ClerkCacheKey &key) const
    {
	return myClerk != key.myClerk || myShop != key.myShop ||
	       myObj != key.myObj || mySop != key.mySop;
    }
    
    SYS_HashType hash() const { return myHash; }

private:
    const SHOP_Clerk	*myClerk;
    int			 myShop, myObj, mySop;
    SYS_HashType	 myHash;
};

// boost.hash implementation
static inline std::size_t hash_value(const SHOP_ClerkCacheKey &key) { return key.hash(); }

class SHOP_API SHOP_ClerkCacheEntry {
public:
    // Common entry class so that SHOP_Node
	     SHOP_ClerkCacheEntry();
    virtual ~SHOP_ClerkCacheEntry();
};

#endif
