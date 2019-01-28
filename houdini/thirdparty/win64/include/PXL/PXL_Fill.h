/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PXL_Fill.h
 *
 * COMMENTS:
 *	Class for filling regions with image data, possibly with
 *	type conversion.
 */
#ifndef PXL_FILL_H
#define PXL_FILL_H

#include <UT/UT_Lock.h>
#include "PXL_API.h"
#include "PXL_Common.h"
#include <iosfwd>

class til_gamma8;
class til_gamma16;

class PXL_API PXL_FillParms
{
public:
	PXL_FillParms();
       ~PXL_FillParms();

    // Source image parms

    void setSourceType(PXL_DataFormat type, unsigned b=0, unsigned w=0);

    // Source area is in inclusive rectangle format in pixels.
    // Since PXL_FillParms has no color model associated with it (ie, RGB),
    // multiple components need to be done in separate passes, or by assuming
    // the source area is 'components' times wider. 
    void setSourceArea(exint x1, exint y1, exint x2, exint y2);
    
    const void		*mySource;
    PXL_DataFormat	 mySType;
    exint			 mySX1, mySY1;
    exint			 mySX2, mySY2;
    unsigned int	 mySBlack, mySWhite;
    exint			 mySInc;
    exint			 mySVInc;
    exint			 mySVOff;
    int			 mySFast;

    // Dest image parms
    void setDestType(PXL_DataFormat type, unsigned b=0, unsigned w=0);
    void setDestArea(exint x1, exint y1, exint x2, exint y2);
    
    const void		*myDest;
    PXL_DataFormat	 myDType;
    exint			 myDX1, myDY1;
    exint			 myDX2, myDY2;
    unsigned int	 myDBlack, myDWhite;
    exint			 myDInc;
    int			 myDFast;

    // transforms
    exint			 myXShift, myYShift;
    int			 myXFlip, myYFlip;

    // colour transforms
    int			 myUseGrey;
    float		 myFillColor;
    float		 myGamma;
    bool		 myMultPerPixel;
    float		*myMultData;
    float		 myMultFactor;

    til_gamma8		*myGTable8;
    til_gamma16		*myGTable16;

    void		 debugPrint(std::ostream &);
};

class PXL_API PXL_Fill
{
public:
    static void	fill(const PXL_FillParms &parms);

    // only uses dest parms and fill colour.
    static void clear(const PXL_FillParms &parms);

    static void invert(const PXL_FillParms &parms);
    static void multiply(const PXL_FillParms &parms);
    static void divide(const PXL_FillParms &parms);
};


// These are quick lookup gamma classes for 8 and 16 bit images.
class PXL_API til_gamma8
{
public:
    til_gamma8() : myTable(0), myBlack(0U), myWhite(0U),
		   myGamma(1.0f), myLock(0) {}
    ~til_gamma8() { delete [] myTable; }
    
    unsigned char *build(float gamma, unsigned int b, unsigned int w);
    unsigned char lookup(unsigned char val) { return myTable[val]; }
    
private:
    unsigned char *myTable;
    unsigned int   myBlack;
    unsigned int   myWhite;
    float	   myGamma;
    UT_Lock	   myLock;
};

class PXL_API til_gamma16
{
public:
     til_gamma16() : myTable(0), myBlack(0U), myWhite(0U),
		     myGamma(1.0f), myLock(0) {}
    ~til_gamma16() { delete [] myTable; }
    
    unsigned short *build(float gamma, unsigned int b, unsigned int w);
    unsigned short lookup(unsigned short val) { return myTable[val]; }
	
private:
    unsigned short *myTable;
    unsigned int   myBlack;
    unsigned int   myWhite;
    float	   myGamma;
    UT_Lock	   myLock;
};

#endif
