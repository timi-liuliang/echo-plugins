/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PXL_Pixel.h
 *
 * COMMENTS:
 *	Utility class for manipulating different types of pixel data (uint8,
 *		uint16, int32, float32)
 *	fast - setting to zero does white/black point calculations,
 *	       1 does the faster int calculations.
 */

#ifndef PXL_PIXEL_H
#define PXL_PIXEL_H

#include "PXL_Common.h"

template<class Type, int fast> class PXL_Pixel
{
public:
	PXL_Pixel(unsigned int black=0, unsigned int white=0, Type value =0);
	PXL_Pixel(const PXL_Pixel &pix);

    inline bool		isSameType(PXL_DataFormat format,
				   unsigned b,
				   unsigned w) const;
    
    inline void		setBWPoints(float b, float w);
    
    inline Type		operator + (int) const;
    inline Type		operator - (int) const;
    inline Type		operator / (int) const;
    inline Type		operator * (int) const;

    inline Type		operator +=(int);
    inline Type		operator -=(int);
    inline Type		operator /=(int);
    inline Type		operator *=(int);

    // Float addition and subtraction treated as (0.0 = black, 1.0 = white)
    inline Type		operator + (float) const;
    inline Type		operator - (float) const;
    inline Type		operator / (float) const;
    inline Type		operator * (float) const;

    inline Type		operator +=(float);
    inline Type		operator -=(float);
    inline Type		operator /=(float);
    inline Type		operator *=(float);

    inline		operator unsigned char  () const;
    inline		operator unsigned short () const;
    inline		operator unsigned int	() const;
    inline		operator float		() const;

    // just assigns explicitly (no conversion), though clamping is done.
    inline Type		assign(unsigned int);

    // assigns explicitly.
    inline Type		set(Type );
    
    inline Type		operator= (float);
    
    inline bool		operator==(unsigned int) const;
    inline bool		operator!=(unsigned int v) const
    { return !(*this == v); }
    inline bool		operator==(float) const;
    inline bool		operator!=(float v) const
    { return !(*this == v); }

    inline Type		maxValue() const;
    inline Type		minValue() const;

    inline void		setRange(unsigned int b, unsigned int w);
    
    inline unsigned int	getWhite() const { return myWhite; }
    inline unsigned int	getBlack() const { return myBlack; }
    inline Type		getValue() const { return myValue; }
    inline PXL_DataFormat getFormat() const { return myType;  }

    inline void		mapValue(unsigned int val);

    inline int		getIntFromFloat(float f) const;


    inline const char  *formatName() const;
private:
    Type		myValue;
    unsigned int	myBlack;
    unsigned int	myWhite;
    PXL_DataFormat	myType;
    float		myPntScale;
};


#include "PXL_PixelImpl.h"

#endif
