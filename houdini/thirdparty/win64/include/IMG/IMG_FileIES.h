/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	IMG_FileIES.h ( IMG Library, C++)
 *
 * COMMENTS:	Class to read/write IES format.  Images loaded from IES
 *		format will show only the candela values, not the angles.
 *		To map from a direction vector to an image pixel
 *		coordinate, use the getEnvCoord() method.  Specification
 *		for this file format are located here:
 *
 * http://www.ltblight.com/English.lproj/LTBLhelp/pages/iesformat.html
 */

#ifndef __IMG_FileIES__
#define __IMG_FileIES__

#include "IMG_API.h"
#include "IMG_File.h"

class IMG_API IMG_FileIES : public IMG_File {
public:
     IMG_FileIES();
    ~IMG_FileIES();

    static void	 registerFormat();

    virtual int	 open();
    virtual int	 readScanline(int y, void *buf);
    virtual int	 writeScanline(int y, const void *buf);

    virtual int	 closeFile();

    // This operation converts a direction vector to the IES image
    // coordinates based on the angles specified in the file.
    void	 getEnvCoord(const UT_Vector3 &r, float &u, float &v) const;

private:
    // Helper to construct the actual pixel data for filtering.  This
    // method handles the different symmetry types at load time so that
    // image filtering can be performed without seams.
    void	 buildImageData();

private:
    struct Header {
	// Line 10
	int	myNLamps;
	float	myLumensPerLamp;
	float	myCandelaMultiplier;
	int	myVerticalAngles;
	int	myHorizontalAngles;
	int	myPhotometricType;
	int	myUnitsType;
	float	myWidth;
	float	myLength;
	float	myHeight;

	// Line 11
	float	myBallastFactor;
	float	myBallastPhotometricFactor;
	float	myInputWatts;
    };

    UT_String		myFormat;
    UT_String		myTilt;
    Header		myHeader;
    UT_FloatArray	myVerticalAngles;
    UT_FloatArray	myHorizontalAngles;
    UT_FloatArray	myData;

    // IES file data processed into an angle-parameterized sphere map
    UT_FloatArray	myXAngles;
    UT_FloatArray	myYAngles;
    UT_FloatArray	myImageData;
    int			myXRes;
    int			myYRes;
};

#endif

