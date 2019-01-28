/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	TIL_Image.h (IMGUI library, C++)
 *
 * COMMENTS:
 *	Cacheable raster definition for the image viewer cache.
 */
#ifndef __TIL_Image__
#define __TIL_Image__

#include "TIL_API.h"

#define TIL_MIP_LEVELS	5

#include <iosfwd>
#include <UT/UT_SmallObject.h>
#include <UT/UT_String.h>
#include "TIL_Defines.h"

class TIL_ImageSource;
class TIL_Plane;
class TIL_Raster;
class TIL_Lookup;
class UT_TokenString;

#define TIL_NO_IMAGE_MATCH	0
#define TIL_EXACT_MATCH		1
#define TIL_SCALED_MATCH	2

// describes the full state of the raster.
class TIL_API TIL_ImageState
    : public UT_SmallObject<TIL_ImageState,
			    UT_SMALLOBJECT_CLEANPAGES_OFF, 128,
			    UT_SMALLOBJECT_THREADSAFE_ON>
{
public:
			 TIL_ImageState();
			~TIL_ImageState();

    int64		getMemoryUsage(bool inclusive) const;

    void		setSource(TIL_ImageSource *source);

    TIL_ImageState &	operator=(const TIL_ImageState &);
    int			operator==(const TIL_ImageState &) const;
    int			fitsRequirements(const TIL_ImageState &,
					 bool scaled = false) const;

    void		debugDump(std::ostream &os);
public:
    TIL_ImageSource	*mySource;
    int			 myVersion;

    UT_String		 myName;
    TIL_Plane		*myPlane;
    exint			 myPlaneIndex;
    UT_TokenString	*myID;
    TIL_Packing		 myPacking;
    int			 myFrameIndex;

    float		 myBlack, myWhite;
    
    exint			 myXRes, myYRes;
    int			 myFullImage;
    exint			 myStartU;
    exint			 myEndU;
    exint			 myStartV;
    exint			 myEndV;
    float		 myGamma;
    float		 myAspect;
    float		 myVAspect;
    bool		 mySolidAlpha;
};


class TIL_API TIL_Image
    : public UT_SmallObject<TIL_Image,
			    UT_SMALLOBJECT_CLEANPAGES_OFF, 256,
			    UT_SMALLOBJECT_THREADSAFE_ON>
{
public:
	 TIL_Image();
   	~TIL_Image();

    void		setRaster(TIL_Raster *r);
    TIL_Raster		*getRaster()		 { return myMipRasters[0]; }
    TIL_Raster	       **getMipRasters()	 { return myMipRasters; }
    void		clearRasters();

    void		setState(TIL_ImageState *state);
    TIL_ImageState	*getState()		 { return myState; }
    
    TIL_Image	       *&next()			 { return myNext; }
    TIL_Image	       *&prev()			 { return myPrev; }

    void		incRef();
    void		decRef();
    int			inUse() const;
    void		clearUsage();
    void		setObsolete(bool o = true) { myObsoleteFlag = o; }
    bool		isObsolete() const { return myObsoleteFlag; }

    int64		getMemoryUsage(bool inclusive) const;

    void		debugDump(std::ostream &os);
    
private:
    // the image raster.
    TIL_Raster		*myMipRasters[TIL_MIP_LEVELS];
 
    // raster identification.
    TIL_ImageState	*myState;

    // don't touch.
    TIL_Image		*myNext;
    TIL_Image		*myPrev;
    int			 myRefCount;
    bool		 myObsoleteFlag;
};



#endif
