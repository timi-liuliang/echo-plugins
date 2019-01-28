/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry library (C++)
 *
 * COMMENTS:	This is the trimming region class.
 *
 */

#ifndef __GD_TrimRegion_h__
#define __GD_TrimRegion_h__

#include "GD_API.h"
#include <GA/GA_Types.h>
#include <UT/UT_LinkList.h>
#include <UT/UT_BoundingRect.h>
#include <SYS/SYS_Math.h>

class UT_IStream;
class GA_PrimitiveGroup;
class GD_Detail;
class GD_Face;
class GD_Primitive;
class GD_TrimLoop;
class GA_LoadMap;
class GA_SaveMap;

class GD_API GD_TrimFace : public UT_LinkNode
{
public:
    GD_TrimFace(GD_Face &face, float ustart, float ustop) : myFace(face)
    {
	myUstart = ustart;
	myUstop  = ustop;
    }
    virtual ~GD_TrimFace();

    const GD_Face	&face (void) const { return myFace; }
    float		 uStop (void) const { return myUstop; }
    float		 uStart(void) const { return myUstart; }

    int		 operator==(const GD_TrimFace &tface) const
		 {
		     return (&myFace == &tface.myFace ) &&
				SYSisEqual( myUstart, tface.myUstart) &&
				SYSisEqual( myUstop,  tface.myUstop );
		 }

    void	 reverse();
    GD_TrimLoop *buildLoop(void) const;

    int		 save(std::ostream &os, int binary) const;
    bool	 jsonSave(UT_JSONWriter &w, const GA_SaveMap &map) const;

    // Load information for a face from a JSON file.
    static bool	 jsonLoad(UT_JSONParser &p, const GA_LoadMap &map,
				GA_Offset &face_offset, fpreal &u0, fpreal &u1);

private:
    GD_Face		&myFace;
    float		 myUstart;
    float		 myUstop;
};


class GD_API GD_TrimRegionFlags
{
public:
	GD_TrimRegionFlags()	{ dirty = 1; opencasual = 0; domainclipped=0; }
	unsigned	  opencasual:1,
			  dirty:1,
			  domainclipped:1;
};


class GD_API GD_TrimRegion : public UT_LinkList
{
public:
	     GD_TrimRegion(int opencasual = 0); // keep direction of open faces
    virtual ~GD_TrimRegion();

    virtual int64 getMemoryUsage(bool inclusive) const;

    // Reverse the order of the trim faces in the region
    virtual void	 reverse(void);

    // Find out about our flags:
    int			 isDirty(void) const	{ return myFlags.dirty;      }
    int			 isOpenCasual()const	{ return myFlags.opencasual; }
    int			 isValid(void) const    { return myLoop &&
							!myFlags.dirty;      }

    int			 getNum (void) const	{ return myNum;  }

    // Grab a handle to the loop of curve pieces. The non-const method 
    // builds it if necessary.
    const GD_TrimLoop	*getLoop(void) const	{ return myLoop; }
	  GD_TrimLoop	*getLoop(const UT_BoundingRect &brect,
				 float tolerance = 1e-4F,
				 int doDomainClip = 1);

    // Find out if the region contains the given primitive or any primitive
    // in the group:
    int			 contains(const GD_Primitive      &prim ) const;
    int			 contains(const GA_PrimitiveGroup &group,
				  int	count_how_many = 0	) const;

    // Returns the number of GD_TrimFaces removed or 0 if none.
    int			 remove(const GD_Primitive      &prim );
    int			 remove(const GA_PrimitiveGroup &group);

    // Merges into the given group all faces inside this trimregion:
    void		 merge(GA_PrimitiveGroup *group) const;

    // Copy from source. Return 0 if OK and -1 otherwise.
    int			 copy(GD_Detail &gdp, const GD_TrimRegion &src,
			      int primoffset);
    
    // I/O stuff:
    bool		 load(GD_Detail &gdp, UT_IStream &is);
    int			 save(std::ostream &os, int binary) const;


    /// @{
    /// JSON IO
    ///
    bool		jsonLoad(GD_Detail &gdp, UT_JSONParser &p, const GA_LoadMap &map);
    bool		jsonSave(UT_JSONWriter &w, const GA_SaveMap &map) const;
    /// @}

protected:
    void		 setNum(int num)	{ myNum = num; }
    void		 initialize(void);

private:
    int			 myNum;		// Only the detail can set this...
    GD_TrimRegionFlags	 myFlags;
    GD_TrimLoop		*myLoop;

    friend class	 GD_Detail;
};

#endif
