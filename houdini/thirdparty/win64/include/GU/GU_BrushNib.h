/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_BrushNib.h ( GU Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GU_BrushNib__
#define __GU_BrushNib__

#include "GU_API.h"
#include <UT/UT_Vector3.h>
#include <UT/UT_Matrix3.h>

class TS_MetaKernel;
class TIL_TextureMap;

// This must match the ordered list in SOPs.
enum GU_BrushBitmapChan
{
    GU_BRUSHBMPCHAN_ALPHA,
    GU_BRUSHBMPCHAN_LUM,
    GU_BRUSHBMPCHAN_RED,
    GU_BRUSHBMPCHAN_GREEN,
    GU_BRUSHBMPCHAN_BLUE
};

//
// This class describes how to do a single dab with the brush.  For each
// point it describes what the alpha should be:
//
class GU_API GU_BrushNib
{
public:
    GU_BrushNib();
    virtual ~GU_BrushNib();

    // Determine if pos is within the nibs depth.
    // It will initialize depth to the depth to the point in the nib.  If
    // depth clipping is off, depth will always be set to 0.0f.
    bool			 testDepth(const UT_Vector3 &pos, float &depth) 
					const;
    
    virtual float		 getAlpha(const UT_Vector3 &pos);

    virtual float		 getAlphaColor(const UT_Vector3 &pos, 
						UT_Vector3 &cd);

    int				 hasStamping() const { return myStampColor; }

    void			 setOpacity(float val) { myOpacity = val; }
    float			 getOpacity() const { return myOpacity; }

    void			 setDepth(bool usedepth, 
					    float depth, float height)
				 { myUseDepth = usedepth; 
				   myDepth = depth; 
				   myHeight = height; 
				 }

    void			 setStamping(int stamp)
				 { myStampColor = stamp; }
    void			 setColor(const UT_Vector3 &cd) 
				 { myColor = cd; }

    void			 setOrientation(const UT_Vector3 &orig,
		    const UT_Vector3 &dir, const UT_Vector3 &upvector,
		    float rad, float angle, float squash);

    void			 setSoftEdge(float softedge, 
					    TS_MetaKernel *kernel);

    void 			 toBrushSpace(UT_Vector3 &pos);
    float			 getBrushDist2(const UT_Vector3 &pos);

    void			 setSplatter(float brush, float paper)
				 { myPaperNoise = paper; myBrushNoise = brush; }

    // Finds the amount of splatter to apply to the given worldspace
    // coordinate.
    float			 getSplatter(const UT_Vector3 &pos);

    void			 getOrientation(UT_Matrix3 &orient) const
				 { orient = myOrient; }

protected:
    float			 myOpacity, myInvRadius, myInvRadius2;
    float			 myRadius, myRadius2;
    bool			 myUseDepth;
    float			 myDepth;	// How deep we will apply.
    float			 myHeight;	// How high we will apply.
    float			 myAngle, mySquash;
    UT_Vector3			 myOrig, myDir, myUpVector;
    UT_Matrix3			 myOrient;	// This is from world space
						// to normalized brush space
    float			 myPaperNoise, myBrushNoise;

    UT_Vector3			 myColor;	// Nib's base colour.
    int				 myStampColor;	// Apply colour to stencil?
    // This is the percentage of the nib that should have a soft
    // edge applied.
    float			 mySoftEdge;
    TS_MetaKernel		*myKernel;
};

class GU_API GU_BrushNibSquare : public GU_BrushNib
{
public:
    GU_BrushNibSquare();
    virtual ~GU_BrushNibSquare();

    virtual float		 getAlpha(const UT_Vector3 &pos);
};

class GU_API GU_BrushNibBitmap : public GU_BrushNib
{
public:
    GU_BrushNibBitmap();
    virtual ~GU_BrushNibBitmap();

    virtual float		 getAlphaColor(const UT_Vector3 &pos, 
						UT_Vector3 &cd);


    void			 setRaster(TIL_TextureMap *raster);
    void			 setRasterChan(GU_BrushBitmapChan chanidx)
				 { myChannel = chanidx; }

    virtual float		 getAlpha(const UT_Vector3 &pos);

protected:
    float			 getAlphaFromColor(const UT_Vector4 &clr) const;
    
    TIL_TextureMap	*myRaster;
    GU_BrushBitmapChan	 myChannel;
};


#endif

