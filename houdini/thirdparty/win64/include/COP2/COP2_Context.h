/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	COP2_ContextData.C
 *
 * COMMENTS:
 *	Class to contain context-dependent data for multi-context cooks 
 *	(usually accessing a COP at several different times during a cook).
 *	COPs that generate static data before cooking should add a class
 *	that derives from this class.
 *
 */
#ifndef _COP2_CONTEXTDATA_H_
#define _COP2_CONTEXTDATA_H_

#include "COP2_API.h"
#include <UT/UT_SmallObject.h>
#include <TIL/TIL_Defines.h>
#include <TIL/TIL_Plane.h>

class COP2_Node;
class COP2_Context;


class COP2_API COP2_ContextData
    : public UT_SmallObject<COP2_ContextData,
			    UT_SMALLOBJECT_CLEANPAGES_OFF, 31,
			    UT_SMALLOBJECT_THREADSAFE_ON, 512>
{
public:
	     COP2_ContextData();
    virtual ~COP2_ContextData();

protected:
    // override to determine under which cases to create a new context data
    virtual bool createPerPlane() const { return false; }
    virtual bool createPerRes() const   { return true; }
    virtual bool createPerTime() const	{ return true; }
    virtual bool createPerThread() const{ return false; }

private:
    // Specific instance expression flags
    void	 setExpressionFlags(bool plane, bool array, bool res);

    bool	 hasPlaneExpr() const { return myExprPlane; }
    bool	 hasArrayExpr() const { return myExprArray; }
    bool	 hasResExpr() const   { return myExprRes; }

    short		changeRefCount(short count);

    int64		getMemSize() const;
    
    short		myCount;
    unsigned char	myExprPlane : 1,
			myExprArray : 1,
			myExprRes   : 1;
    
    friend class COP2_Context;
    friend class COP2_Node;
};

class COP2_API COP2_Context
    : public UT_SmallObject<COP2_Context,
			    UT_SMALLOBJECT_CLEANPAGES_OFF, 160,
			    UT_SMALLOBJECT_THREADSAFE_ON, 256>
{
public:
    COP2_ContextData	*data();

    // context information.
    union {
	const TIL_Plane	*myPlane;
	COP2_Context	*myNext;	// used by allocators.
    };
    fpreal		 getTime() const	{ return myTime; }
    void		 setTime(fpreal t)	{ myTime = t; }
    int			 xres() const		{ return myXres; }
    int			 yres() const		{ return myYres; }
    void		 setXres(int xres)	{ myXres = xres; }
    void		 setYres(int yres)	{ myYres = yres; }

    int			 myArrayIndex;
    float		 myTime;
    int			 myThreadIndex;
    int			 myXres;
    int			 myYres;
    int			 myXsize;
    int			 myYsize;
    int			 myXorig;
    int			 myYorig;

    // Image bounds modification routines.
    void		 setImageBounds(int x1, int y1, int x2, int y2);
    void		 enlargeImageBounds(int x, int y);
    void		 enlargeImageBounds(float x, float y);

    // Image bounds query. Returns false if they are not set.
    bool		 getImageBounds(int &x1, int &y1, int &x2, int &y2);
    bool		 areBoundsSet() const { return myBoundsInit; }
    
private:
    // only COP2_Node can allocate.
		 COP2_Context(COP2_Node *node);
		~COP2_Context();
    void	 reset();

    bool	 dataMatch(const TIL_Plane *plane, int array, float time,
			   int xres, int yres, int thread) const;

    bool	 contextMatch(const TIL_Plane *plane, int array, float time,
			      int xres, int yres, int thread) const;

    void	 set(const TIL_Plane *plane, int arrayindex, float t,
		     int xres, int yres, int thread, bool ownsplane = true);

    void	 setData(COP2_ContextData *data);
    void	 setPendingData(bool pd) { myPendingData = pd; }
    bool	 isPendingData() const { return myPendingData; }

    int64	 getMemSize() const;

    TIL_FastLock	 myLock;
    COP2_ContextData	*myData;
    COP2_Node		*myCop;
    volatile unsigned	 myBoundsInit :1,
			 myPlaneOwner :1,
			 myPendingData :1;

    friend class COP2_Node;
    friend class COP2_CookScheduler;
};

#endif
