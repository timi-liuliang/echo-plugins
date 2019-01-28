/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	IMG_Plane.h ( IMG Library, C++)
 *
 * COMMENTS:
 *	Description class of a single plane in a deep raster file.
 *      IMG_Stat contains a list of IMG_Plane structures.
 */
#ifndef __IMG_PLANE_INFO_H__
#define __IMG_PLANE_INFO_H__

#include "IMG_API.h"

#include <SYS/SYS_Types.h>
#include <UT/UT_String.h>
#include <UT/UT_Rect.h>
#include <PXL/PXL_Common.h>

#include "IMG_FileTypes.h"

class IMG_Stat;

/// @brief standard plane names.
struct IMG_API IMG_PlaneName
{
    static const char *Color;		// "C"
    static const char *Alpha;		// "A"
    static const char *Opacity;		// "Of"
    static const char *Depth;		// "Pz"
    static const char *Position;	// "P"
    static const char *Normal;		// "N"

    /// For deep images, this plane contains the sample count for each pixel.
    static const char *DepthComplexity;	// "Depth-Complexity"
};

/// @brief Describes the format and layout of a single plane in an image
/// The plane specifies the format and name of a single image plane in a deep
/// raster image. IMG_Stat contains a list of planes found in the image.
/// Non-deep raster formats only contain one plane. 
class IMG_API IMG_Plane
{
public:
    /// @{
    /// Planes are generally not created by the user, but by 
    /// IMG_Stat::addPlane()
    IMG_Plane();
    IMG_Plane(const IMG_Plane &p);
    IMG_Plane(const char *name, IMG_DataType dt, IMG_ColorModel cm,
		  int index);
    /// @}

    /// Returns the amount of memory owned by this IMG_Plane object
    int64 getMemoryUsage(bool inclusive) const;

    /// @{
    /// plane name - must be unique in the list of planes found in IMG_Stat
    void	setName(const char *name);
    const char *getName() const;
    /// @}

    /// @{
    /// data format of the individual pixel components (8bit - FP)
    void	 setDataType(IMG_DataType dt);
    IMG_DataType getDataType() const;
    /// @}

    /// @{
    /// color model of the pixel, which specifies the components per pixel.
    /// (SINGLE_CHAN, RGB, RGBA)
    void	   setColorModel(IMG_ColorModel cm);
    IMG_ColorModel getColorModel() const;
    /// @}

    /// @{
    /// type info of the data contained in the plane. This is purely an
    /// advisory indicator on how the data should be handled.
    /// Note that this can affect the colorspace of the plane if not COLOR.
    void	   setTypeInfo(IMG_TypeInfo ti);
    IMG_TypeInfo   getTypeInfo() const;
    /// @}

    /// @{
    /// colorspace of the image data in the plane. This helps determine how to
    /// display the image data. If PXL_CS_UNKNOWN is returned, the format's
    /// colorspace will be used instead. 'gamma' is only needed for
    /// PXL_CS_CUSTOM_GAMMA.
    void	   setColorSpace(PXL_ColorSpace cs, fpreal gamma = 0.0);
    PXL_ColorSpace getColorSpace() const;
    fpreal	   getColorSpaceGamma() const;
    /// @}

    /// @{
    /// Sets the individual component name(s) (default: r,g,b,a). Always call 
    /// after setColorModel().
    void	setComponentNames(const char *c1,
				  const char *c2=0,
				  const char *c3=0,
				  const char *c4=0);
    void	setComponentName(int comp, const char *name);

    /// Returns the name of the individual component of a pixel (r,g,b,a, x,y,z)
    const char *getComponentName(int comp) const;
    /// @}

    

    /// @{
    /// Specifies black/white points for integer formats (not supported for FP)
    /// always set the data type before setting the BW points.
    /// The black and white points must be within the range of the intger data
    /// format (ie, 8b - 0-255). 
    void	   setBlackWhitePoints(int64 b, int64 w);
    bool	   hasBlackWhitePoints() const;
    void	   getBlackWhitePoints(int64 &b, int64 &w) const;
    /// @}

    /// the size of a single pixel, in bytes, including the data format and
    /// the color model.
    int		   getPixelSize() const;

    /// This is the index of this plane, in the list of planes found in
    /// IMG_Stat (0 to getNumPlanes()-1).
    int		   getPlaneIndex() const { return myIndex; }
    
    /// Returns the number of components this plane consists of.
    int		   getComponentCount() const
		   { return IMGvectorSize(myColorModel); }

private:
    // only used by IMG_Stat.
    void	   setParentStat(IMG_Stat *stat) { myStat = stat; }
    void	   setPlaneIndex(int index) { myIndex = index; }
    void	   updateComponentNames();

    // data members
    UT_String		myName;
    UT_String		myComponentNames[4];
    int			myIndex;
    
    IMG_DataType	myDataType;
    IMG_ColorModel	myColorModel;
    PXL_ColorSpace	myColorSpace;
    fpreal		myColorSpaceGamma;
    IMG_TypeInfo	myTypeInfo;
    int64		myBlackPoint;
    int64		myWhitePoint;
    IMG_Stat	       *myStat;

    friend class IMG_Stat;
};



#endif
