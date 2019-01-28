/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry library (C++)
 *
 * COMMENTS:	Base class for display caches. 3 classes are defined here:
 *			GU_CacheFlags:	(dirty) flags for the cache class
 *			GU_DisplayCache:the mother of all display caches
 *			GU_CacheParms:	parms to update the cache with
 *
 */

#ifndef __GU_DisplayCache_H__
#define __GU_DisplayCache_H__

#include "GU_API.h"
#include <UT/UT_Vector3.h>
#include <UT/UT_Matrix4.h>

class GU_DisplayAttribs;


// These defines are used as unsigned short commands, thus we can only deal
// with polygons of less than 65529 vertices
#define GU_GIVEUP		 65535	// Something we can't deal with, so
					// let the GU tessellator handle it.
#define GU_BEGINPOLY		 65534	// Begin polygon
#define GU_BEGINTRI		 65533	// Begin triangle
#define GU_BEGINQUAD		 65532	// Begin quad
#define GU_ENDPOLY		 65531	// End polygon
#define GU_ENDCOMMAND		 65530	// End of all polygons

enum GU_CacheMode	{ GU_CACHEWIRE, GU_CACHESHADED };

SYS_DEPRECATED_PUSH_DISABLE()

class GU_API GU_CacheFlags
{
public:
    SYS_DEPRECATED_HDK(13.0)
    GU_CacheFlags()   {
		allDirty	= 1; // YES, completely dirty by default
		xformDirty	= 0;
		xlateDirty	= 0;
		uvDirty		= 0;
		pushXform	= 0;
    }

    SYS_DEPRECATED_HDK(13.0)
    int		isDirty(void) const
		{ return allDirty|xformDirty|xlateDirty|uvDirty; }
    SYS_DEPRECATED_HDK(13.0)
    int		howDirty(void) const
		{ return allDirty+xformDirty+xlateDirty+uvDirty; }
    SYS_DEPRECATED_HDK(13.0)
    void	clear(void)
		{ allDirty = xformDirty = xlateDirty = uvDirty = 0; }


    unsigned	allDirty:1,	// it's a mess. Must refresh it all
		xformDirty:1,	// the entire cache can be transformed
		xlateDirty:1,	// the entire cache can be translated
		uvDirty:1,	// parts of the domain are dirty
		pushXform:1;	// push xform when render rather than apply
};

class GU_API GU_DisplayCache
{
public:
    SYS_DEPRECATED_HDK(13.0)
    		 GU_DisplayCache(void) { theLOD = -1.0f; }
    SYS_DEPRECATED_HDK(13.0)
    virtual	~GU_DisplayCache(void);

    // Find out if anything is dirty or if we're so messy we should refresh all
    SYS_DEPRECATED_HDK(13.0)
    int		 isDirty() const	{ return theFlags.isDirty();  }
    SYS_DEPRECATED_HDK(13.0)
    int		 isMessy() const	{ return theFlags.allDirty;   }
    SYS_DEPRECATED_HDK(13.0)
    int		 howDirty()const	{ return theFlags.howDirty(); }
    SYS_DEPRECATED_HDK(13.0)
    void	 clear()		{ theFlags.clear(); }
    SYS_DEPRECATED_HDK(13.0)
    void	 dirty()
		 {
		     theFlags.clear();
		     theFlags.pushXform = 0; // forget it, we're too dirty
		     theFlags.allDirty  = 1;
		 }

    // Query or set the xform flag:
    SYS_DEPRECATED_HDK(13.0)
    int		 transformed() const	{ return theFlags.xformDirty; }
    SYS_DEPRECATED_HDK(13.0)
    void	 transform(const UT_Matrix4 &xform, int pushxform = 0);
    
    // Query or set the translate flag:
    SYS_DEPRECATED_HDK(13.0)
    int		 translated(void) const	{ return theFlags.xlateDirty; }
    SYS_DEPRECATED_HDK(13.0)
    void	 translate(const UT_Vector3 &d);

    // Query or set the uvdirty flag:
    SYS_DEPRECATED_HDK(13.0)
    int		 uvChanged(void) const	{ return theFlags.uvDirty; }
    SYS_DEPRECATED_HDK(13.0)
    void	 uvChange(void);

    // If we're about to be "smeared" with a transformation, decide now 
    // whether to apply the xform to the cache or push the matrix before
    // rendering and using the current cache. This flag gets turned off
    // if other things will cause us to be dirty.
    SYS_DEPRECATED_HDK(13.0)
    int		 pushXform(void) const	{ return theFlags.pushXform;  }
    SYS_DEPRECATED_HDK(13.0)
    void	 pushXform(int yesno)	{ theFlags.pushXform = yesno; }

    SYS_DEPRECATED_HDK(13.0)
    float		 lod(void) const		{ return theLOD;   }
    SYS_DEPRECATED_HDK(13.0)
    const UT_Matrix4	&transformation(void) const	{ return theXform; }

protected:
    // Clear the cache data and other related info:
    SYS_DEPRECATED_HDK(13.0)
    virtual void	 initializeData(void);

    // If pushXform is set, the cache data has not been updated with the
    // latest xform matrix. This method makes sure the data _is_ updated,
    // and turns off the flag.
    SYS_DEPRECATED_HDK(13.0)
    virtual void	 assimilateXform(void);

    SYS_DEPRECATED_HDK(13.0)
	  UT_Vector3	&delta(void)			{ return theDelta; }
    SYS_DEPRECATED_HDK(13.0)
    const UT_Vector3	&delta(void) const		{ return theDelta; }
    SYS_DEPRECATED_HDK(13.0)
	  UT_Matrix4	&transformation(void)		{ return theXform; }
    SYS_DEPRECATED_HDK(13.0)
    void 		 lod(float l) 			{ theLOD = l;      }


private:
    GU_CacheFlags	 theFlags;
    UT_Matrix4		 theXform;
    UT_Vector3		 theDelta;
    float		 theLOD;
};


class GU_API GU_CacheParms 
{
public:
    SYS_DEPRECATED_HDK(13.0)
    		 GU_CacheParms(void) {}
    SYS_DEPRECATED_HDK(13.0)
    virtual	~GU_CacheParms(void);

    SYS_DEPRECATED_HDK(13.0)
    void	 	 lod(float l)			{ theLOD  = l;       }
    SYS_DEPRECATED_HDK(13.0)
    float	 	 lod(void) const		{ return theLOD;     }
    SYS_DEPRECATED_HDK(13.0)
    void	 	 mode(GU_CacheMode m)		{ theMode = m;       }
    SYS_DEPRECATED_HDK(13.0)
    GU_CacheMode	 mode(void) const		{ return theMode;    }
    SYS_DEPRECATED_HDK(13.0)
    void	 	 attributes(const GU_DisplayAttribs *a) { theAttribs=a;}
    SYS_DEPRECATED_HDK(13.0)
    const GU_DisplayAttribs *attributes() const	{ return theAttribs; }

private:
    float			 theLOD;	// level of detail
    GU_CacheMode 	 	 theMode;	// display mode: wire, shaded
    const GU_DisplayAttribs	*theAttribs;	// attribute offsets
};

SYS_DEPRECATED_POP_DISABLE()

#endif
