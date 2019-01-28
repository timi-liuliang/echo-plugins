/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	TIL_Sequence.h	(Tile Image Library, C++)
 *
 * COMMENTS:
 *	Defines the information for a series of images.
 */
#ifndef TIL_SEQUENCE_H
#define TIL_SEQUENCE_H

#include "TIL_API.h"
#include <iosfwd>
#include <UT/UT_Options.h>
#include <UT/UT_SmallObject.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_String.h>

#include "TIL_Defines.h"

// image rounding 
#define SEQUENCE_NEAREST	0
#define SEQUENCE_PREVIOUS	1
#define SEQUENCE_NEXT		2

class UT_WorkBuffer;
class UT_InfoTree;
class TIL_Plane;
class IMG_Stat;

class TIL_API TIL_Sequence :
    public UT_SmallObject<TIL_Sequence,
			  UT_SMALLOBJECT_CLEANPAGES_OFF, 196,
			  UT_SMALLOBJECT_THREADSAFE_ON>

{
public:
			TIL_Sequence();
			TIL_Sequence(const TIL_Sequence &);

			~TIL_Sequence();

    int64		getMemoryUsage(bool inclusive) const;

    void		reset();
    bool		isValid() const { return myValidFlag; }
    void		setValid(bool valid) { myValidFlag = valid; }

    // copies everything.
    TIL_Sequence &	operator=(const TIL_Sequence &);

    bool		operator==(const TIL_Sequence &) const;

    bool		matches(const TIL_Sequence &other_seq,
				bool metdata_tool = true) const;

    void		copyFrom(const IMG_Stat &stat);
    void		copyTo(IMG_Stat &stat) const;

    // copies everything but the plane information.
    void		copyAttributes(const TIL_Sequence &);

    void		getInfoText(UT_WorkBuffer &info,
				bool timedep = true,
				bool mark_unscoped = false) const;
    void		getInfoTree(UT_InfoTree &tree,
				bool timeDep) const;

    // querying/setting

    bool		isSingleImage()	const	       { return mySingleImage;}
    void		setSingleImage(bool on)	       { mySingleImage = on; }
    
    double		getFrameRate() const		{ return myRate; }
    void		setFrameRate(double rate)	{ myRate = rate; }

    exint		getLength() const		{ return myLength; }
    void		setLength(exint n)		{ myLength = n; }

    double		getStart() const		{ return myStart; }
    void		setStart(double start)		{ myStart = start; }

    double		getEnd() const		{ return myStart+myLength-1; }

    double		getStartTime() const { return (myStart-1) / myRate; }
    double		getEndTime() const
				{ return (myStart + myLength -2) / myRate; } 

    // 'Res' is the visible resolution of the image. 'Size' is the actual
    // size of the image, which is equal to or greater than the res.
    void		getRes(int &x,int &y) const
			{ x = myXRes; y = myYRes; }
    
    void		setRes(int x, int y);

    void		setAspectRatio(float aspect)
			{ myAspect = (aspect > 0.0f) ? aspect : 1.0f; }
    float		getAspectRatio() const { return myAspect; }
    float		getVAspectRatio() const 
			{ return (getInterlace()==INTERLACE_HALF) ? 2.0f:1.0f; }

    fpreal              getRenderTime() const
			    { return myRenderTime; }
    void                setRenderTime(fpreal t)
			    { myRenderTime = t; }
    int64               getRenderMemory() const
			    { return myRenderMemory; }
    void                setRenderMemory(int64 mem)
			    { myRenderMemory = mem; }

    void		getTemporalExtend(TIL_Extend &b, TIL_Extend &a) const
			{   b = myPreExtend;   a = myPostExtend; }
    void		getTemporalHold(int &before, int &after) const
			{   before = myPreHold;  after = myPostHold;  }
    
    void		setTemporalExtend(TIL_Extend before, TIL_Extend after)
			{   myPreExtend = before;   myPostExtend = after; }
    void		setTemporalHold(int before, int after)
			{   myPreHold = before;  myPostHold = after; }

    // utility functions

    // These return the actual image index, from 0 to N-1
    exint		getImageIndex(double t, int clamp_range = 1,
				      int round_off = SEQUENCE_NEAREST) const;
    double		getImageTime(double t, int clamp_range =1,
				     int round_off = SEQUENCE_NEAREST) const;

    // These return the frame index for interlaced frames (imageindex/2)
    exint		getFrameIndex(double t, int clamp_range = 1,
				      int round_off = SEQUENCE_NEAREST) const;
    double		getFrameTime(double t,int clamp_range =1,
				     int round_off = SEQUENCE_NEAREST) const;

    // returns the time of image at image_index (in the range 0 - length-1)
    double		getIndexTime(exint image_index) const;

    exint		checkIndexBounds(exint image_index) const;
	
    // plane management
    int			getNumPlanes() const { return myPlanes.entries(); }

    // If reorder is true, then addPlane can re-order the plane and the
    // caller cannot assume that we will append the plane to the end
    TIL_Plane *		addPlane(const char *name, TIL_DataFormat format,
				 const char *vn1 = 0,
				 const char *vn2 = 0,
				 const char *vn3 = 0,
				 const char *vn4 = 0,
				 bool reorder=false);
    TIL_Plane *		addPlane(const TIL_Plane *plane,
				 bool reorder=false);
    
    // removes and delete a plane.
    void		removePlane(int index);
    void		removePlane(const char *name);

    void		clearAllPlanes();

    // plane accessor functions.
    TIL_Plane *		getPlane(int index);
    const TIL_Plane *	getPlane(int index) const;

    TIL_Plane *		getPlane(const char *name);
    const TIL_Plane *	getPlane(const char *name) const;

    TIL_Plane *		getPlaneWithElement(const char *elem_name);
    const TIL_Plane *	getPlaneWithElement(const char *elem_name) const;

    int			getPlaneByName(const char *name) const;

    // Interlacing
    void		setInterlace(TIL_Interlace i, TIL_Dominance d);
    TIL_Interlace	getInterlace() const	  { return myInterlace; }
    TIL_Dominance	getFieldDominance() const { return myDominance; }

    void		print(std::ostream &os) const;

    void		bumpColorAlphaToFront();

    // metadata support
    UT_Options	       &metaData()	 { return myMetaData; }
    const UT_Options   &metaData() const { return myMetaData; }

    void		clearMetaData() { myMetaData.clear(); }
    
private:
    void		makeUnique(UT_String &name);

    unsigned	myValidFlag :1,
	        mySingleImage : 1;

    // sequence rate, start & length
    exint	myLength;
    double	myStart;
    double	myRate;

    // image resolution. 
    int		myXRes;
    int		myYRes;
    float	myAspect;

    // Temporal extend conditions
    TIL_Extend	myPreExtend;
    TIL_Extend	myPostExtend;

    int		myPreHold;
    int		myPostHold;

    // sequence planes (r,g,b, etc)
    UT_ValArray<TIL_Plane *> myPlanes;

    TIL_Interlace  myInterlace;
    TIL_Dominance  myDominance;

    UT_Options	   myMetaData;

    // TODO: Move these into a generic metadata structure
    fpreal myRenderTime;
    int64 myRenderMemory;
};

#endif
