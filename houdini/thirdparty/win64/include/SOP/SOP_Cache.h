/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SOP_Cache.h ( SOP Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SOP_Cache__
#define __SOP_Cache__

#include "SOP_API.h"
#include "SOP_Node.h"
#include <UT/UT_Map.h>
#include <utility>

class UT_InfoTree;
class CH_Manager;

class SOP_API SOP_Cache : public SOP_Node
{
public:
	     SOP_Cache(OP_Network *net, const char *name, OP_Operator *entry);
    virtual ~SOP_Cache();

    static OP_Node		*myConstructor(OP_Network *net,
					       const char *name,
					       OP_Operator *entry);

    static PRM_Template		 myTemplateList[];

    virtual void		 getNodeSpecificInfoText(OP_Context &context,
					OP_NodeInfoParms &iparms);
    
    // Fill in tree with this node's details
    virtual void                 fillInfoTreeNodeSpecific(UT_InfoTree &tree, 
					const OP_NodeInfoTreeParms &parms);

    virtual bool		 unloadData();
    virtual int64	 	 getLoadedMemoryUsage() const;

    int64			 getCacheRemoveSerial() const
				 { return myRemoveSerial; }

    virtual bool                updateParmsFlags();
    virtual void syncNodeVersion(
        const char *old_version, const char *cur_version, bool *node_deleted);

    static fpreal roundTime(fpreal current_t);

    void pruneCache(exint maxframes, fpreal current_t);

    /// NOTE: This MUST NOT be named "clearCache", because there's a virtual
    ///       function on OP_Node with that name!
    void clearCacheSopCache();

protected:
    virtual OP_ERROR	 cookMySop(OP_Context &context);
    void		 setInstance(GU_DetailHandle *igdh);
    virtual int		 isRefInput(unsigned int i) const;
    virtual const char  *inputLabel(unsigned idx) const;

private:
    bool                ANYFRAME()              { return evalInt("anyframe", 0, 0) != 0; }
    int                 MAXFRAMES(fpreal t)     { return evalInt("maxframes",0, t); }
    bool                CLEARONCHANGE(fpreal t) { return evalInt("clearonchange", 0, t) != 0; }

    int			START(fpreal t)		{ return evalInt("range", 0, t); }
    int			END(fpreal t)		{ return evalInt("range", 1, t); }
    int			INC(fpreal t)		{ return evalInt("range", 2, t); }

    fpreal		INDEX(fpreal t)		{ return evalFloat("index", 0, t); }
    void		PARAM(UT_String &str)	{ evalString(str, "param", 0, 0); }

    int			INDEXTIME()		{ return evalInt("indextime", 0, 0); }

    int			LOADMETHOD()		{ return evalInt("loadmethod", 0, 0); }

//  int			LOAD()			{ return evalInt("reload", 0, 0); }
//  int			RESET()			{ return evalInt("clear", 0, 0); }

    bool		CACHEPOINTS()		{ return evalInt("cachepoints", 0, 0) != 0; }
    bool		BLENDPOS()		{ return evalInt("blendpos", 0, 0) != 0; }

    void		 clearGDPLists();
    void		 clearGDPList(fpreal t);

    void		 loadLocalGeometry(fpreal t, bool cache_points, int ind,
	    				   int check_merge = 0);
    GU_DetailHandle	*getLocalGeometry(fpreal t, bool cache_points, int ind,
	    				  int check_merge = 0);

    static int		 preLoadLocalCache(void *data, int index, fpreal t,
					    const PRM_Template *);
    static int		 preClearLocalCache(void *data, int index, fpreal t,
					    const PRM_Template *);
    static int		 preClearLocalCacheFrame(void *data, int index, fpreal t,
						 const PRM_Template *);

    void		 blendGDP(fpreal t, bool cache_points, fpreal index);
    void		 setGDP(fpreal t, bool cache_points, int i0);

    const GU_Detail	*getLocalInput(fpreal t, fpreal val, const char *param,
	    			       CH_Manager *cman);

    void		 getRange(int &start, int &end, int &inc, fpreal t);

    void		 shiftCache(int start, int end, int inc);

    /// This maps from frame time to detail.  Each detail has an
    /// integer that indicates the cache access count when
    /// it was last accessed.
    /// When removing one item from the cache, the minimum integer is
    /// found and removed. When removing everything (or all but one)
    /// from the cache, it's easily cleared.  Otherwise, the items
    /// are sorted for removal.
    UT_Map<fpreal, std::pair<GU_DetailHandle *,exint> > myCache;

    /// This is a cache access count, used for tracking age in myCache.
    exint myCacheAccessCount;

    /// Cache whether any of the cooks in the cache have been time dependent.
    bool myTimeDep;

    int			myCacheStart;
    int			myCacheEnd;
    int			myCacheInc;
    int			myTotal;

    UT_Array<GU_DetailHandle *>  myGDPLists;
    fpreal			 myLastIndex;
    int64			 myCachedMemoryUsage;

    int64			 myRemoveSerial;
    int				 myLastRefInputId;
    int				 myLastRefInputModVer;
};

#endif
