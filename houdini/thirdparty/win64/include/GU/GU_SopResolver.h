/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_SopResolver.h ( GU Library, C++)
 *
 * COMMENTS:	Resolver class for resolving filenames like "op:/soppath"
 */

#ifndef __GU_SopResolver__
#define __GU_SopResolver__

#include "GU_API.h"
#include <SYS/SYS_Types.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_String.h>
#include <UT/UT_StringMap.h>
#include <UT/UT_Matrix4.h>
#include <VEX/VEX_GeoInputs.h>
#include "GU_DetailHandle.h"

// Class to query an input on a VEX geometry
class GU_API GU_VexGeoInputs : public VEX_GeoInputs
{
public:
    GU_VexGeoInputs();
    virtual ~GU_VexGeoInputs();

    // You can register this class as a callback for when geo inputs
    // are destroyed.
    class GU_API ClearCB
    {
    public:
	ClearCB() {}
	virtual ~ClearCB() {}

	virtual void	clear() {}
    };

    /// When originally designed the idea was to allow delayed discovery
    /// of VEX inputs.  In practice this proves infeasible, so we mandate
    /// all inputs are cooked & stored prior to execution.  peekInput()
    /// and getInput() thus become equivalent.
    const GU_Detail *peekInput(int idx) const
    {
	return getInput(idx);
    }
    const GU_Detail *getInput(int idx) const
    {
	if (idx < minInputIdx() || idx > maxInputIdx())
	    return 0;
	if (idx < 0)
	    return myNegInputCache(-(idx+1));
	else
	    return myPosInputCache(idx);
    }

    /// Does not gain ownership of gdp, the caller should keep
    /// it in scope until destruction
    void setInput(int idx, const GU_Detail *gdp)
    {
	if (idx < 0)
	{
	    idx = - (idx + 1);
	    myNegInputCache.forcedRef(idx) = gdp;
	    myNegVexGeoCache.forcedRef(idx) = 0;
	}
	else
	{
	    myPosInputCache.forcedRef(idx) = gdp;
	    myPosVexGeoCache.forcedRef(idx) = 0;
	}
    }

    /// Adds a callback to be invoked when the GU_VexGeoInputs
    /// is destroyed.
    void addClearCB(ClearCB *cb)
    {
	myClearCBList.append(cb);
    }

    /// The GVEX layer stores the cache for the input in these structures,
    /// this ensures each invocation of VEX will have its own
    /// cache.  The GU_VexGeoInputs should thus be shared across threads
    /// to ensure separate threads use the same cache (so point clouds
    /// are not built twice, etc)
    void 		*getVexCache(int idx) const
    {
	if (idx < minInputIdx() || idx > maxInputIdx())
	    return 0;
	if (idx < 0)
	    return myNegVexGeoCache(-(idx+1));
	else
	    return myPosVexGeoCache(idx);
    }

    void 		*getVexCache(const UT_StringRef &key) const
    {
	auto it = myVexGeoCacheMap.find(key);

	if( it == myVexGeoCacheMap.end() )
	    return NULL;

	return it->second;
    }
    void		 setVexCache(int idx, void *cache)
    {
	UT_ASSERT(idx >= minInputIdx() && idx <= maxInputIdx());
	{
	    // Must either be clearing or setting the first time,
	    // setting an already set cache suggests this was not
	    // properly locked by the caller.
	    if (idx < 0)
	    {
		idx = - (idx + 1);
		UT_ASSERT(!myNegVexGeoCache(idx) || !cache);
		myNegVexGeoCache(idx) = cache;
	    }
	    else
	    {
		UT_ASSERT(!myPosVexGeoCache(idx) || !cache);
		myPosVexGeoCache(idx) = cache;
	    }
	}
    }

    void		 setVexCache(const UT_StringRef &key, void *cache)
    {
	// Must either be clearing or setting the first time,
	// setting an already set cache suggests this was not
	// properly locked by the caller.
	UT_ASSERT(!myVexGeoCacheMap.contains(key) || !cache );
	myVexGeoCacheMap.insert( std::make_pair(key,cache ) );
    }


    /// Valid ranges are inclusive from minInputIdx to maXInputIdx.
    /// Note this can be negative!
    exint		 minInputIdx() const { return - myNegInputCache.entries(); }
    exint		 maxInputIdx() const { return myPosInputCache.entries() - 1; }

protected:
    // We do not own these, they are assumed to be owned
    // by the entity invoking setInput.
    mutable UT_Array<const GU_Detail *> myPosInputCache;
    mutable UT_Array<const GU_Detail *> myNegInputCache;
    mutable UT_Array<void *> myPosVexGeoCache;
    mutable UT_Array<void *> myNegVexGeoCache;
    mutable UT_StringMap<void*>  myVexGeoCacheMap;

    // We own this list of callback objects.
    UT_ValArray<ClearCB *>	 myClearCBList;
};

// Class to query a SOP's detail for the resolver
class GU_API GU_SopQuery
{
public:
     GU_SopQuery()
     {
	 myOpId = -1;
	 myFrameSpecified = false;
	 myTimeSpecified = false;
	 myIsDopData = false;
	 myXform.identity();
     }
     // If a detail has been locked, then it will be unlocked automatically on
     // destruction.
    ~GU_SopQuery() { clear(); }

    void		 clear();
    void		 clearButLeaveTime();

    void		 setDetail(const GU_ConstDetailHandle &handle)
			     { myDetail = handle; 
			       myDetail.addPreserveRequest();
			     }
    void		 setXform(const UT_Matrix4D &xform)
			     { myXform = xform; }

    GU_ConstDetailHandle &getDetail(const char *fullpath, int relative_to=-1);
    void		 getWorldTransform(UT_Matrix4D &xform,
				const char *fullpath, int relative_to=-1);
    int			 getOpId() const	{ return myOpId; }
    bool		 getFlagInterest() const{ return myFlagInterest; }
    int			 getSourceId() const	{ return mySourceOpId; }
    bool		 isDopData() const { return myIsDopData; }
    void		 setIsDopData(bool isdop) { myIsDopData = isdop; }
    void		 addDopReference(int callerid);
    bool		 getFrame(fpreal &frame) const
			 {
			     if (myFrameSpecified)
				 frame = myFrame;
			     return myFrameSpecified;
			 }

    void		 setFrame(fpreal frame)
			 {
			     myFrameSpecified = true;
			     myFrame = frame;
			 }
    
    bool		 getTime(fpreal &frame) const
			 {
			     if (myTimeSpecified)
				 frame = myTime;
			     return myTimeSpecified;
			 }

    void		 setTime(fpreal frame)
			 {
			     myTimeSpecified = true;
			     myTime = frame;
			 }

    const UT_String	&getSubPath() const { return mySubPath; }

private:
    GU_ConstDetailHandle	 myDetail;
    UT_Matrix4D		 myXform;
    int			 myOpId;
    bool		 myFlagInterest;
    int			 mySourceOpId;
    bool		 myFrameSpecified;
    fpreal		 myFrame;
    bool		 myTimeSpecified;
    fpreal		 myTime;
    UT_String		 mySubPath;
    bool		 myIsDopData;
    friend class	 GU_SopResolver;
};

//
// Note:  The GU_SopResolver class shouldn't be accessed directly.  All access
// should be performed through the GU_SopQuery class.
//
class GU_API GU_SopResolver
{
public:
	     GU_SopResolver();
    virtual ~GU_SopResolver();

    static void			 setResolver(GU_SopResolver *resolver);
    static GU_SopResolver	*getResolver();

    /// Stealing the resolver will give you a copy of the current resolver,
    /// clearing out ownership of the resolver.  The caller of @c stealResolver
    /// is responsible for deleting the stolen resolver.
    static GU_SopResolver	*stealResolver();

    // Lookup a path to see if it's a path to something that has geometry
    static bool			 lookup(const char *fullpath, GU_SopQuery &q);
    static bool			 lookupXform(const char *fullpath, GU_SopQuery &q);

    // Sub-class responsibilities:  These shouldn't be called directly.  They
    // are only public for the SopQuery class.

    // Given an operator path, return the unique ID associated with the Op.
    // Otherwise, return -1.
    virtual int			 resolveId(const char *fullpath,
					    int relative_to, bool &flaginterest) = 0;

    // The resolveDetail method should set the handle in the query
    // appropriately.
    virtual bool		resolveDetail(GU_SopQuery &q) = 0;
    virtual bool		resolveXform(GU_SopQuery &q) = 0;

    virtual void		addDopReference(GU_SopQuery &q, int callerid) = 0;
};

#endif

