/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PXL_Lookup.h ( PXL Library, C++)
 *
 * COMMENTS:
 *	Simple lookup table class.
 */
#ifndef PXL_LOOKUP_H
#define PXL_LOOKUP_H

#include "PXL_API.h"

class PXL_Plane;
class PXL_Raster;
class UT_FileLUT;

#include <stdio.h>
#include <SYS/SYS_Types.h>
#include "PXL_Common.h"
#include <CL/CL_Clip.h>

enum PXL_LookupType
{
    PXL_LOOKUP_R	= 0x001, // 1 table for Red only.
    PXL_LOOKUP_G	= 0x002,
    PXL_LOOKUP_B	= 0x004,
    PXL_LOOKUP_A	= 0x008, // 1 table for Alpha only.
    
    PXL_LOOKUP_RGB	= 0x017, // 3 tables, 1 each for Red, Green & Blue.
    PXL_LOOKUP_RGBA	= 0x01F, // 4 tables, 1 each for R, G, B and alpha.
    
    PXL_LOOKUP_COLOR	= 0x027, // 1 table for RGB.		
    PXL_LOOKUP_ALL	= 0x02F, // 1 table for all channels.

    PXL_LOOKUP_3D	= 0x040, // 3D LUT for color only.
    PXL_LOOKUP_3D_1D	= 0x060, // 3D LUT with 1D pre-pass, color only.
    PXL_LOOKUP_3D_HDR	= 0x140  // 3D LUT with log sampling
};

// Masks for PXL_LookupType
#define PXL_LOOKUP_1D			0x3F
#define PXL_SEPARATE_CHANNELS		0x10
#define PXL_SINGLE_CHANNEL		0x20
#define PXL_ILLEGAL_MATCH		0x30
#define PXL_LOG_SAMPLE			0x100


class PXL_API PXL_Lookup
{
public:
			 PXL_Lookup();
			~PXL_Lookup();

    int64		getMemoryUsage(bool inclusive) const;

    void		reset();

    // common LUT applications

    // single color modification
    void		evalLUT(float color[3]) const;

    // set a 1D LUT.
    void		setLUT(const CL_Clip &clip);

    // set the 3D LUT.
    void		set3DLUT(int size, float *lut);

    // applies the LUT to a raster (along with gamma). black/white are the
    // BW points of the image in the raster. 'comp' is used to convert the
    // raster to show only 1 component (ie, r->rgb).
    void		applyLUT(PXL_Raster &r, float gamma = 1.0f,
				 float black=0.0f, float white=1.0f,
				 int comp = -1) const;

    // returns the value for sampling at integer LUT index 'i'. If the
    // value is passed to evalLUT(), it will return the sample at table index i.
    // For 3D LUTs, this needs to be called once for each axis. 
    fpreal		indexToValue(int i) const;
    
    // --------------------------------------------------------------------
    // accessers

    bool		isValid() const;
    
    // 1D accessers
    PXL_LookupType      getType() const;
    bool		getLogSampling() const;
    float		getLogBias()     const;
    int			getNumChannels() const;
    int			getTableSize()   const;
    float		getBlackPoint()  const;
    float		getWhitePoint()  const;
    void		getDomain(float &s, float &e) const;
    void		getRange(float &s, float &e) const;

    bool		hasSpecificFormat(PXL_DataFormat &format) const;
    bool	        hasLUTfor(PXL_LookupType type) const;

    void		setType(PXL_LookupType type);
    void		setLogSampling(bool log);
    void		setLogBias(float bias);
    void		setSpecificFormat(PXL_DataFormat format);
    void		setNumChannels(int num);
    void		setTableSize(int size);
    void		setBlackWhitePoints(float b, float w);
    void		setDomain(float s,float e);
    void		setRange(float s, float e);
    void		invert(int newsize = 0);

    bool		isHalfPrec() const { return myHalfPrecFlag; }

    // 3D accessers
    bool		is3DLUT() const { return myCubeSize != 0; }
    int			 get3DSize() const { return myCubeSize; }

    // --------------------------------------------------------------------
    // 1D LUT evaluation routines 
    fpreal		*getRawLUT(PXL_LookupType type);
    const fpreal	*getRawLUT(PXL_LookupType type) const;
    
    bool		 evalLUT(PXL_LookupType,
				 int nsamples,
				 float *data,
				 unsigned b = 0,
				 unsigned w = 0) const;
    
    bool		 evalLUT(PXL_LookupType,
				 int nsamples,
				 float *data,
				 float black, float white,
				 float from = 0.0f, float to = 1.0f) const;
    float		 evalLUT(PXL_LookupType, float pos) const;

    void		 eval1DLUT(float color[3]) const;
    void		 get1DLUT(float *lut[4], int size, float b, float w,
				  float sc, float sh, float gamma) const;

    void		 apply1DLUT(PXL_Raster &raster,
				    float gamma = 1.0f,
				    float black = 0.0f,
				    float white = 1.0f,
				    int comp = -1) const;
    // ---------------------------------------------------------------------
    // 3D LUT evaluation routines 
    void		 eval3DLUT(float color[3]) const;

    // makes a copy of the 3D LUT. 'lut' must be of size get3DSize()^3 * 3 if
    // type is a 3D linear or log LUT. If 3D with prelut is used, prelut must
    // non-NULL and of size get3DSize()^2.
    void		 copy3DLUT(float *lut, float *prelut = 0) const;
    void		 copy3DLUT(fpreal16 *lut, fpreal16 *prelut = 0) const;
    
    // clamps the 3D LUT to 0,1. Used for display LUTs.
    void		 clamp3DLUT();
    
    void		 apply3DLUT(PXL_Raster &r, float gamma = 1.0f,
				    float black = 0.0f, float white = 1.0f,
				    int component = -1) const;

    
    // File I/O.
    bool		load(const char *filename, bool header_only = false);
    bool		save(const char *filename, bool half_prec=false) const;

    // these load and save methods do not close fp.
    bool		load(FILE *fp, bool header_only);
    bool		loadBinary(FILE *fp, bool header_only);
    bool		save(FILE *fp) const;
    bool		saveBinary(FILE *fp, bool half_prec = false) const;

    
    // create builtin lut types
    enum Dir
    {
	LINEAR_TO_GAMMA,
	GAMMA_TO_LINEAR
    };
    
    bool		buildGamma(fpreal gamma,
				   PXL_Lookup::Dir dir = LINEAR_TO_GAMMA,
				   int lutsize = 1024);
    
    bool		buildSRGB(PXL_Lookup::Dir dir = LINEAR_TO_GAMMA,
				  int lutsize = 1024);
    
    bool		buildREC709(PXL_Lookup::Dir dir = LINEAR_TO_GAMMA,
				    int lutsize = 1024);

    bool		buildOpenColorIO(const char *src_color_space,
					 const char *dest_color_space,
					 const char * looks = NULL);

    // creates a cineon LUT for our Cineon lib. This allocates a new LUT,
    // which should be freed with delete. Filename is only for comparing
    // UT_FileLUTs, it is optional.
    UT_FileLUT	       *getCineonLUT(bool forward,
				     const char *filename = 0) const;

    // Merge 2 LUTs into one LUT (this LUT). This has the same effect as running
    // a pixel through 'first_lut', then 'second_lut', with some possible
    // precision loss if one is a large 1D LUT and the other is a 3D LUT.
    bool		 mergeLUTs(const PXL_Lookup &first_lut,
				   const PXL_Lookup &second_lut);

    void		 setTextureID(void *id) { myTextureID = id; }
    void		*getTextureID() const;
    void		 clearTextureID();

    void		 setPreLutID(void *id) { myPreLutID = id; }
    void		*getPreLutID() const;
    
private:
    int			getIndexFor(PXL_LookupType type) const;

    bool		correctLUT();

    inline void		lerpPreLUT(float &color, float prelut_scale,
               		           const fpreal *prelut, int prelut_last) const;

    void		save1D(FILE *fp, bool bin, bool half = false) const;
    void		save3D(FILE *fp, bool bin, bool half = false) const;

    CL_Clip	       *myTable;
    PXL_LookupType	myType;
    PXL_DataFormat	myFormat;

    // 3D LUT
    float	       *myCubeLUT;
    int			myCubeSize;
    
    float		myBlack;
    float		myWhite;
    float		myDomStart;
    float		myDomEnd;
    float		myRangeStart;
    float		myRangeEnd;
    float		myLogBias;
    void	       *myTextureID;
    void	       *myPreLutID;
    bool		myHalfPrecFlag;
};

#endif
