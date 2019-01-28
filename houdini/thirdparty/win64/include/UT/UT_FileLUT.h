/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT library (C++)
 *
 * COMMENTS:
 *	Defines a LUT that is read from a file; values are assumed to be
 *	unsigned shorts.
 * 
 */

#ifndef __UT_FILE_LUT_H__
#define __UT_FILE_LUT_H__

#include "UT_API.h"
#ifndef WIN32
#include <sys/stat.h>
#endif

#include "UT_String.h"

class UT_API UT_FileLUT
{
public:
    enum {
	LOAD_FORWARD_LUT = 0,
	LOAD_INVERSE_LUT,
	LOAD_FULL_LUT
    };

    enum {
	CINEON_CLIPPED = 0,
	CINEON_FULL_RANGE,
	CINEON_WHITE_GIVEN
    };
    

	     UT_FileLUT();
    virtual ~UT_FileLUT();

    UT_FileLUT &operator=(const UT_FileLUT &) ;
    int		operator==(const UT_FileLUT &) const;

    // loads a LUT file
    int		load(const char *name, int lutsize, int lutrange,
		     int mode = LOAD_FORWARD_LUT);

    // creates a cineon LUT 
    int		createCineon(int white_point, fpreal gamma, int full_range,
			     int mode = LOAD_FORWARD_LUT,int imagepnt = 4095);

    void	create(int size, float *data, int range,
		       int mode = LOAD_FORWARD_LUT,
		       const char *filename = 0,
		       int whitepoint = 65535);
    
    // LUT accessers
    int				getLUTSize() const    { return myLUTSize; }
    unsigned short *		getLUT()	      { return myLUT; }
    const unsigned short *	getLUT() const	      { return myLUT; }

    int				getInvLUTSize() const { return myInvLUTSize; }
    unsigned short *		getInvLUT()	      { return myInvLUT; }
    const unsigned short *	getInvLUT() const     { return myInvLUT; }

    int				getWhitePoint() const
				{ return myComputedWhitePoint; }

    // comparison functions
    int			 compareExistingLUT(const char *name, int mode);
    int			 compareExistingLUT(int white, fpreal gamma,
					    int mode, int iwhite);

    void		 debugOut(const char *file);

private:

    int			 myLUTSize;
    unsigned short	*myLUT;
    int			 myInvLUTSize;
    unsigned short	*myInvLUT;

    // comparison info
    int			 myCreationMode;
    UT_String		 myFilename;
    int			 myWhitePoint;
    int			 myImageWhite;
    float		 myGamma;

#ifndef WIN32    
    time_t		 myFileCreationTime;
#endif

    // the image white point.
    int			myComputedWhitePoint;
};

#endif
