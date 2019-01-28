/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	TIL_ImageSource.h (TIL library, C++)
 *
 * COMMENTS:
 *	This is the base class for an image source (ie, COP for Houdini,
 *	or disk files for a standalone viewer).
 */

#ifndef TIL_IMAGESOURCE_H
#define TIL_IMAGESOURCE_H

#include "TIL_API.h"
#include <SYS/SYS_Types.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_Color.h>
#include <PXL/PXL_Common.h>
#include <stdlib.h>
#include <limits>

#define TIL_GLOBAL_TIME		(std::numeric_limits<fpreal>::infinity())

class UT_String;
class UT_StringHolder;
class UT_TokenString;
class UT_WorkBuffer;
class IMG_TileOptions;

class TIL_Plane;
class TIL_Raster;
class TIL_Sequence;
class TIL_ColorCurves;

class STY_StyleSheet;
class OP_Node;

struct TIL_API TIL_CookRegion
{
    int		myID;
    
    int		myX0, myX1;
    int		myY0, myY1;

    UT_Color	myColor;
    fpreal	myAlpha;
};

typedef UT_Array<TIL_CookRegion> TIL_CookRegionList;

class TIL_API TIL_ImageSource
{
public:
				 TIL_ImageSource();

    void			 bumpRefCount(int i);

    void			 invalidate()	 { myValidFlag = false; }
    bool			 isValid() const { return myValidFlag; }

    // global stuff... (not source specific).
    virtual fpreal		 getGlobalTime() =0;
    virtual fpreal		 getSampleRate() =0;
    virtual fpreal		 getStartTime() =0;
    virtual fpreal		 getEndTime() =0;

    // returns the image index (0 to length-1) or the frame (start to end)
    // based on the current time.
    int				 getImageIndexFromTime(fpreal t);
    int				 getFrameFromTime(fpreal t);

    virtual int			 doesImageExist(int /*frame*/,
						bool /*adjust*/ = true)
						{ return 1; }

    virtual int			 getActualFrame(int frame) { return frame; }
    virtual void		 removeFrame(int /*frame*/) { }
    virtual void		 addFrame(int /*frame*/) { }
    virtual void		 removeSequence() {}
    virtual bool		 canAddFrame() { return false; }

    virtual bool		 isNetwork() { return false; }

    // allows time mapping (mostly for disk files).
    virtual fpreal		 getFrameTime(fpreal t,
					      bool /*shift_only*/ = false,
					      const TIL_Sequence * /*ref*/ = 0)
				 { return t; }
    virtual int			 getFrameShift(int fr,
					       bool /*shift_only*/ = false,
					       const TIL_Sequence * /*ref*/ =0)
				 { return fr; }

    // Allows you to remap a frame for information purposes (like the Houdini
    // frame number when rendered to mplay).
    virtual int			getApparentFrame(int fr) { return fr; }

     // name of the source (cop name, disk file).
    virtual const char		*getName() = 0;
    virtual void		 getFullName(UT_String &name) = 0;
    virtual void		 getFrameName(int frame, UT_String &name);

    // ah, well.. whaddayathink
    virtual int			 equals(const TIL_ImageSource *) = 0;

    // Called to open the source (return 0 on failure). if reset is set, all
    // errors will be cleared.
    virtual int			 open(short & /*key*/, 
				      int /*reset*/ = 0,
				      fpreal /*t*/ = TIL_GLOBAL_TIME)
				 { return 0; }

    // called to close the source.
    virtual void		close(short /*key*/) { ; }

    // returns true if the res & data type are constant for the sequence.
    virtual bool		 isConstantSequence() const { return false; }

    // return a completed TIL_Sequence structure.
    virtual const TIL_Sequence	*getSequence(fpreal t) = 0;

    // return an identifier that can be used to identify a single frame
    // out of the source, unique across multiple sources. 
    virtual UT_TokenString	*getID(fpreal t, int xres, int yres,
				       const TIL_Plane &plane,
				       int array_index) = 0;

    virtual void		 getImageBounds(const TIL_Plane &plane,
						int array_index,
						fpreal t, int xres, int yres,
						int &x1, int &y1,
						int &x2, int &y2);

    // Returns a list of regions currently being cooked
    virtual void		 getCookRegions(TIL_CookRegionList &region) = 0;

    /// Returns true iff there is currently a priority circle active.
    /// If there is, centrex, centrey, and radius are set to those of the circle.
    virtual bool                 getPriorityCircle(float &centrex, float &centrey, float &radius) const
    {
        return false;
    }

    // return an int that uniquely identifies the source. 
    virtual int			 getID() const = 0;

    // fill the TIL_Raster with the appropriate image (ie, cook it).
    // return 0 on failure.
    virtual int			getImage(TIL_Raster *image,
					 fpreal t, int xres, int yres,
					 const TIL_Plane &plane,
					 int array_index,
					 int xstart, int ystart,
					 int xend, int yend, float gamma,
					 bool include_alpha = false,
					 bool is_interactive = false,
					 int fxres = 0, int fyres = 0) = 0;

    virtual void		getInfo(UT_WorkBuffer &infotext);

    virtual void		getComment(UT_String &comment, fpreal t);
    virtual void		setComment(const UT_String &comment, fpreal t);

    // There's duplicate information in the IMG_TileOptions class, we only use
    // the Format and FormatOptions in the writing process.
    virtual int			writeImage(const char *filename,
					   const IMG_TileOptions *finfo,
					   const TIL_Sequence *, int step =1,
					   const char *uselut = 0,
					   float gamma = 1.0F,
					   void (*info)(void*, const char*)=0,
					   void *data=0,
					   bool suppress_summary_dialog=false,
					   bool adjust_color_space = true,
					   bool overwrite = false);

    virtual int			writeImage(const char *filename,
					   const IMG_TileOptions *finfo,
					   int xres, int yres,
					   const char *color,
					   const char *alpha,
					   int start, int end, int step =1,
					   const char *uselut = 0,
					   float gamma = 1.0F,
					   void (*info)(void*, const char*)=0,
					   void *data =0,
					   bool suppress_summary_dialog=false,
					   bool adjust_color_space = true,
					   bool overwrite = false);

    virtual void		writeImageComplete(int successcount,
					       int failcount,
					       const UT_String &failedfiles);

    // normally, once you're done with an image, it will stay cached if there's
    // room. The parm is the frame index. Return false to have it deallocated
    // immediately.
    virtual bool		isCachingNeeded(int  ) const { return true; }

    // only really needed for COPs. 
    virtual size_t		getVersion()			{ return 1; }
    virtual int			isFrameLocked(fpreal /*t*/)	{ return 0; }
    virtual int			isAnythingLocked()		{ return 0; }
    virtual int			isPlaneLocked(const TIL_Plane *,
					      fpreal /*t*/)	{ return 0; }
    virtual const TIL_Plane	*getPreviewPlane(int /*index*/) { return 0; }

    virtual void		 setMouseLocation(int mx, int my,
                                                  float radius = 64.0f);

    virtual void		 bumpVersion(bool ) { }
    virtual bool		 isSlowOperation() const    { return false; }

    virtual bool		 getColorCurves(const char *planename,
						int array, fpreal t, 
						TIL_ColorCurves &curves,
						UT_String &first,
						bool allnodes, int comp=-1,
						bool natural_range = true,
						float start = 0.0f,
						float end   = 1.0f,
						int evalpoints = 0);

    virtual bool		 allowUserSelection() const { return false; }

    virtual TIL_ImageSource	*selectParentSource(int px, int py,
						    const char *planename,
						    int array_index);

    // Allow the source to have a callback invoked.  The function should return
    // true if a script is run
    virtual bool		 hasPixelScript(int slot) const;
    virtual bool		 runPixelScript(int px, int py, int slot);

    // Returns true if the image source is stereoscopic, ie, it can provide
    // left / right images (as two separate planes).
    virtual bool		 isStereoscopicSource(fpreal t) const;
    virtual int			 getLeftPlaneIndex(fpreal t) const;
    virtual const char		*getLeftPlaneName(fpreal t) const;
    virtual int			 getRightPlaneIndex(fpreal t) const;
    virtual const char		*getRightPlaneName(fpreal t) const;

    // NOTE: This level has nothing to destruct.
    virtual int64		 getMemoryUsage(bool inclusive) const = 0;

    virtual fpreal getRenderTime(int frame) const
    { return -1.0; }

    virtual int64 getPeakMemUsage(int frame) const
    { return 0; }

    virtual bool  getPreferredViewingPlane(UT_WorkBuffer &buf) const
				 { return false; }

    // Returns the style sheet json string at a given pixel.
    // By default, there is none
    virtual bool getStyleSheetJSON(int x, int y, UT_StringHolder& str)
				 { return false; }

    // Returns the style sheet object at a given pixel.
    // By default there is none
    virtual STY_StyleSheet* getStyleSheet(int x, int y)
				{ return NULL; }

    // Returns the material node at a given pixel
    virtual OP_Node* getMaterialNode(int x, int y)
				{ return NULL; }

    // Indicates that an auto update is being performed
    virtual void onAutoUpdate() {}

protected:
    virtual			~TIL_ImageSource();

private:
    TIL_ImageSource &		 operator=(const TIL_ImageSource &);

    int				 myRefCount;
    bool			 myValidFlag;
};


// Convenience class for opening and closing a source.
class TIL_API TIL_Access
{
public:
     TIL_Access(TIL_ImageSource &source, int reset=0,
		fpreal t = TIL_GLOBAL_TIME);
    ~TIL_Access();

    bool	isOpen() const { return myOpen; }

private:
    TIL_Access &	 operator=(const TIL_Access &);

    TIL_ImageSource	&mySource;
    short		 myAccessKey;
    bool		 myOpen;
};

#endif
