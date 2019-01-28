/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __POP_Cache_h__
#define __POP_Cache_h__

#include "POP_API.h"
#include <UT/UT_ValArray.h>
#include <UT/UT_FloatArray.h>

class POP_ContextData;

class POP_API POP_Cache
{
public:
		 POP_Cache();
		~POP_Cache();

    void	 setCacheEnable(int on);
    void	 setCacheUnlimited(int on);
    void	 setMaxSize(int size, POP_ContextData *context = 0);

    POP_ContextData *cache(POP_ContextData *, float t);
    
    POP_ContextData *getContext(float t);
    
    void	 clear();
    void	 clearPastGivenTime(float t);
    void	 getCacheSettings(int &enabled, int &unlimited, int &cachesize);
    int		 getMaxSize() { return myMaxCacheSize; }

    float	 getStartTime();
    float	 getEndTime();

    void	 print();


private:

    int		 getIndex(int &index, float t);
    int		 getIndex(int &index, POP_ContextData *context);
    void	 truncate(int keepStart, int keepEnd);
    
    UT_ValArray<POP_ContextData *>	myContexts;
    UT_FloatArray			myCookTimes;
    int					myCacheEnabled;
    int					myCacheUnlimited;
    int					myMaxCacheSize;
};



#endif
