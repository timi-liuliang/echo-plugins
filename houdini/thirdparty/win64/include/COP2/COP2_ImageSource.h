/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	COP2_CopImageSource.h (COP2 library, C++)
 *
 * COMMENTS:
 *	This is the derived class for a cop image source for TIL.
 */
#ifndef COP2_IMAGESOURCE_H
#define COP2_IMAGESOURCE_H

#include "COP2_API.h"
#include <TIL/TIL_ImageSource.h>
#include "COP2_Node.h"

class COP2_API COP2_ImageSource : public TIL_ImageSource
{
public:
				 COP2_ImageSource(COP2_Node *);
  
    // global stuff...
    virtual fpreal		 getGlobalTime();
    virtual fpreal		 getSampleRate();
    virtual fpreal		 getStartTime();
    virtual fpreal		 getEndTime();

    //
    virtual const char		*getName();
    virtual void		 getFrameName(int frame, UT_String &name);
    virtual void		 getFullName(UT_String &name);
    virtual int			 doesImageExist(int frame, bool adjust = true);
    
    virtual int			 equals(const TIL_ImageSource *);

    virtual int			 open(short &key,
				      int reset = 0,
				      fpreal t = TIL_GLOBAL_TIME);
    virtual void		 close(short key);

    virtual bool		 isNetwork();

    virtual bool		 isConstantSequence() const { return true; }
    virtual const TIL_Sequence	*getSequence(fpreal t);

    virtual int			 getID() const { return myID; }
    virtual UT_TokenString	*getID(fpreal t, int xres, int yres,
				       const TIL_Plane &plane,
				       int array_index);

    virtual void		 getImageBounds(const TIL_Plane &plane,
						int array_index,
						fpreal t, int xres, int yres,
						int &x1, int &y1,
						int &x2, int &y2);
    
    virtual void		 getCookRegions(TIL_CookRegionList &regions)
				 {}

    virtual int			 getImage(TIL_Raster *image,
					  fpreal t, int xres, int yres,
					  const TIL_Plane &plane,
					  int array_index,
					  int xstart, int ystart,
					  int xend, int yend, float gamma,
					  bool include_alpha =false,
					  bool is_interactive=false,
					  int fxres = 0, int fyres = 0);

    virtual void		getInfo(UT_WorkBuffer &infotext);

    virtual int			 writeImage(const char *filename,
					    const IMG_TileOptions *finfo,
					    const TIL_Sequence *, int step =1,
					    const char *uselut = 0,
					    float gamma = 1.0F,
					    void (*info)(void*, const char*)=0,
					    void *data=0,
					    bool suppress_summary_dialog=false,
					    bool adjust_color_space = true,
					    bool overwrite = false);
    
    virtual int			 writeImage(const char *filename,
					    const IMG_TileOptions *finfo,
					    int xres, int yres,
					    const char *color,
					    const char *alpha,
					    int start, int end, int step =1,
					    const char *uselut = 0,
					    float gamma = 1.0F,
					    void (*info)(void*, const char*)=0,
					    void *data=0,
					    bool suppress_summary_dialog=false,
					    bool adjust_color_space = true,
					    bool overwrite = false);

    virtual size_t		 getVersion();
    virtual int			 isFrameLocked(fpreal t);
    virtual int			 isAnythingLocked();
    virtual int			 isPlaneLocked(const TIL_Plane *, fpreal t);
    virtual const TIL_Plane	*getPreviewPlane(int index);
    

    virtual int			 canModifyImage() { return 1; }
    virtual int			 useHandles() { return 1; }

    COP2_Node			*getCop() const { return isValid()?myCop:0; }
    COP2_Node			*getCop()	{ return isValid()?myCop:0; }
    
    virtual bool		 isSlowOperation() const
			 { return isValid() ? myCop->isSlowOperation():false; }
    
    virtual fpreal		 getFrameTime(fpreal t, bool shift_only=false,
					      const TIL_Sequence * /*ref*/ =0);
    virtual int			 getFrameShift(int fr, bool shift_only=false,
					      const TIL_Sequence * /*ref*/ =0);
    
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

    virtual bool  getPreferredViewingPlane(UT_WorkBuffer &buf) const;
    
    virtual int64		 getMemoryUsage(bool inclusive) const;
protected:    
    virtual			~COP2_ImageSource();
private:
    
    COP2_Node			*myCop;
    int				 myID;
    UT_TokenString		*myLastToken;
};


#endif
