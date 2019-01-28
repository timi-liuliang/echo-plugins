/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	IMG_FileParm.h ( IMG Library, C++)
 *
 * COMMENTS:
 *	Parameter class for options to IMG_File::open() and IMG_File::create()
 */
#ifndef __IMG_FILE_PARMS_H__
#define __IMG_FILE_PARMS_H__

#include "IMG_API.h"

#include <SYS/SYS_Types.h>
#include <UT/UT_FilterType.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_Rect.h>
#include <PXL/PXL_LumFuncs.h>

#include "IMG_FileTypes.h"

class UT_Options;
class IMG_TileOptions;
class IMG_Stat;
class IMG_Format;

/// @brief  File options for manipulating image data on load or save.
/// This class allows you to modify the incoming or outgoing image data by
/// scaling, flipping, converting or cropping the data. It can be optionally
/// passed to IMG_File::open() or IMG_File::create()
class IMG_API IMG_FileParms
{
public:
    IMG_FileParms();

    /// This method turns off any option that would result in image
    /// translation.
    void readAsIs();

    // DATA ORGANIZATION ----------------------------------------------------

    /// @brief Convert to a different data type
    /// convert the image to this data type, if needed (default = use native)
    void   setDataType(IMG_DataType dt);
 
    /// @brief Convert to a different number of pixel components
    /// convert to a standard color model, if needed (RGB, RGBA, single).
    /// When moving from fewer components to more components, the
    /// data is either duplicated (1chan->RGB) or generated (RGB-RGBA, A=1). 
    void   setColorModel(IMG_ColorModel cm);
    
    /// @brief Convert to either an RGBA or ABGR ordering
    /// converts the component ordering to RGBA or ABGR (or RGB/BGR, RG/GR)
    /// without forcing the  color model to a specific number of components.
    void   setComponentOrder(IMG_ComponentOrder order);

    /// @brief Interleaves or deinterleaves pixel data
    /// Determines how to format the data.
    ///  - IMG_INTERLEAVE_AS_IS  - leave it interleaved or non, as in the file.
    ///  - IMG_INTERLEAVED       - always interleave (rgbrgbrgb). Default.
    ///  - IMG_NON_INTERLEAVED   - always non-interleaved (rrrgggbbb)
    void   setInterleaved(IMG_Interleave i);

    /// If the color model is set to IMG_1CHAN, and the actual color model
    /// is RGB or higher, this method determines how to convert the vector
    /// into a scalar. By default, the luminance is taken.
    void   setLuminanceFunc(PXL_LumFunction f);

    /// If true, alpha will be read into its own plane, instead of an RGBA
    /// color plane. Color will be read as its own plane as well, RGB.
    void   readAlphaAsPlane();

    /// If demoting from a deep raster to an RGB(A) image, these methods
    /// allow you to specify the plane(s) to copy to RGB(A), by name or index.
    /// Selects a plane by index. Indices are specified from 1 to # planes.
    void   selectPlanes(const UT_IntArray &planeindices);

    /// If demoting from a deep raster to an RGB(A) image, these methods
    /// allow you to specify the plane(s) to copy to RGB(A), by name or index.
    /// Selects several planes by numeric pattern, ie. "1", "1 3 4", "[1-3] 5"
    void   selectPlanes(const char *pattern);

    /// If demoting from a deep raster to an RGB(A) image, these methods
    /// allow you to specify the plane(s) to copy to RGB(A), by name or index.
    /// Selects serveral planes by name pattern, such as "C", "C A Pz" "P? C*"
    void   selectPlaneNames(const char *name);

    
    /// RESOLUTION -----------------------------------------------------------
    /// using these methods always scales. To crop, use the data window methods
    /// Note that the filter is shared, so if more than one of these is called,
    /// the last filter setting is used.

    /// Scale the image to resolution (x,y). (0 = use original dimension)
    void   scaleImageTo(int x, int y, UT_FilterType ft=UT_FILTER_BOX);

    /// scale the image by scaling factors (x,y).
    void   scaleImageBy(fpreal x, fpreal y, UT_FilterType ft=UT_FILTER_BOX);

    /// limit the image to a maximum resolution. Scale to this res, preserving
    /// the aspect ratio if requested.
    void   limitImageRes(int x, int y, UT_FilterType ft=UT_FILTER_BOX,
			 bool preserve_aspect = true);

    /// images must be read as powers of two. Does not preserve aspect ratio.
    void   powerTwoOnly();

    // DATA WINDOW ---------------------------------------------------------

    /// Normally, a data window is expanded or cropped to the image resolution.
    /// calling this will always read only the data window. 
    /// Used only for reading.
    void   setDataWindowOnly();

    /// read the image region in 'area' only (even if we need to crop or expand
    /// the image to fill it). Used only for reading.
    void   setWindow(const UT_DimRect &area);

    /// read the image region in 'area' in UV coords. Used only for reading.
    void   setWindow(float u1, float v1, float u2, float v2);

    // ORIENTATION ----------------------------------------------------------
    
    /// options for orienting and flipping the image. Default orientation is
    /// LEFT_FIRST, BOTTOM_FIRST. You can set each to 'none' if you don't care.
    void   orientImage(IMG_XOrientation x, IMG_YOrientation y);

    /// @brief Flip the image in either direction.
    /// flip the images in either direction. May cancel out any orientation
    /// flipping.
    /// @{
    void   flipImageVertical();
    void   flipImageHorizontal();
    /// @}

    /// rotate the image 90', flopping it on its side.
    void   flopImage();


    // COLOR CORRECTION -----------------------------------------------------
    
    /// @brief Color correct an image using a LUT
    /// apply a lookup table to the data, but only those planes that match the
    /// scope (ie, "C spec diff", "C*", "*beauty*")
    void   applyLUT(const char *lut, const char *plane_scope = "*", bool ocio = false);

    /// @brief Color correct an image using a gamma setting
    /// Apply gamma to the planes matching the scope (ie, "C spec diff", "C*",
    /// "*beauty*")
    void   applyGamma(fpreal gamma, const char *gamma_scope = "*");

    /// Set the gamma and scope to apply any needed gamma correction to planes
    /// being written. This overwrites any applyGamma() changes.
    void   adjustGammaForFormat(const IMG_Stat &stat,
				const IMG_Format *format,
				IMG_DataType override_type = IMG_DT_ANY);

    /// Specifies the destination colorspace when reading, and the source
    /// colorspace when writing (ie, the colorspace the raster is in, not the
    /// file contents). Default colorspace is the scene linear role.
    /// Only active if an OCIO LUT is used.
    void   setOCIOColorspace(const char *ocio_colorspace);

    /// One or more looks (color transforms) that are applied to the image
    /// after reading or before writing, in order of specification (whitespace
    /// separated list).
    void   setOCIOLooks(const char *looks);

    // INFORMATION ----------------------------------------------------------

    /// @brief Returns information about the image's alpha channel
    /// Checks to see if the alpha plane is constant 1, cutout 0/1,
    /// constant valued, varying, out-of-range (>1 or <0) or absent. Load only.
    void   detectAlphaDetails();

    // FORMAT SPECIFIC OPTIONS ----------------------------------------------

    /// set an input/output tag option for the format.
    void	setOption(const char *option, const char *value);

    /// options are a list of argument pairs, such as "artist", "Tom Smith".
    /// The options list is terminated by a NULL.
    void	setOptions(const char **options);

    /// options are in a whitespace separated string "Artist 'Tom Jones'"
    void	setOptionsString(const char *option_pair_string);

    /// options are specified in the IMG_TileOptions structure
    void	setOptions(const IMG_TileOptions &info_with_options);

    /// set options based on a UT_Options (this is similar to using
    /// IMG_TileOptions)
    void	setOptions(const UT_Options &options);

    /// @brief Enable the tile inteface for reading or writing
    /// If called, we're reading or writing tiles using IMG_File::readTile
    /// and IMG_File::writeTile. The scanline versions will not work.
    void   	useTileInterface();

    /// If set, files will not report errors and subsequent scanlines
    /// will be returned as black. 
    void   	continueOnError();

private:
    IMG_ImageType    myImageType;
    IMG_DataType     myDataType;
    IMG_ColorModel   myColorModel;
    IMG_ComponentOrder myComponentOrder;
    IMG_XOrientation myOrientX;
    IMG_YOrientation myOrientY;
    int		     myMaxX, myMaxY;
    int		     myResX, myResY;
    fpreal	     myScaleX, myScaleY;
    UT_FilterType    myFilterType;
    fpreal	     myGamma;
    IMG_Interleave   myInterleaved;
    PXL_LumFunction  myLumFunc;
    fpreal	     myGain;
    fpreal	     myOffset;

    bool	   myFlipHorizontal : 1,
		   myFlipVertical : 1,
	 	   myFlopImage      : 1,
		   myReadDataWindowOnly :1,
		   myReadAreaFlag :1,
		   myReadUVAreaFlag : 1,
		   myUseTiles :1,
		   myUseLUT : 1,
		   myPowerTwoRes :1,
	           myResLimit:1,
		   myPreserveAspect : 1,
		   myAlphaSplit : 1,
		   myContinueOnError : 1,
		   myAlphaDetect: 1,
		   myIsNormal: 1,
                   myLutIsOCIO : 1;
    
    UT_DimRect	   myReadArea;
    fpreal	   myReadUVArea[4];
    UT_String	   myLUTFilename;
    UT_String	   myLUTScope;
    UT_String	   myGammaScope;
    UT_String      mySelectPlaneName;
    UT_IntArray	   mySelectPlaneIndex;
    UT_StringHolder myOCIOColorspace;
    UT_StringHolder myOCIOLooks;

    UT_StringArray myOptions;
    UT_StringArray myOptionValues;
    
    friend class IMG_File;
};



#endif
