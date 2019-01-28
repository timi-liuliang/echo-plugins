/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PXL_Pixel.C
 *
 * COMMENTS:
 *	Utility class for manipulating different types of pixel data (uint8,
 *		uint16, int32, float32)
 */
#ifndef PXL_PIXEL_C
#define PXL_PIXEL_C

#include <limits.h>
#include <stdlib.h>

#include <SYS/SYS_Types.h>
#include <SYS/SYS_Math.h>
#include <SYS/SYS_Floor.h>

#include <UT/UT_Endian.h>

#include "PXL_Pixel.h"
#include <UT/UT_StackTrace.h>

//#ifndef LONGLONG_MAX
//#define LONGLONG_MAX	9223372036854775807L
//#endif

//#define USE_LUT_FOR_8_16_TO_FP_CONV

#ifdef USE_LUT_FOR_8_TO_FP_CONV
// Hard coded LUTs for 8bit conversion to FP.
static float lut8bitFP[256] =
{
0.000000, 0.003922, 0.007843, 0.011765, 0.015686, 0.019608, 0.023529, 0.027451,
0.031373, 0.035294, 0.039216, 0.043137, 0.047059, 0.050980, 0.054902, 0.058824,
0.062745, 0.066667, 0.070588, 0.074510, 0.078431, 0.082353, 0.086275, 0.090196,
0.094118, 0.098039, 0.101961, 0.105882, 0.109804, 0.113725, 0.117647, 0.121569,
0.125490, 0.129412, 0.133333, 0.137255, 0.141176, 0.145098, 0.149020, 0.152941,
0.156863, 0.160784, 0.164706, 0.168627, 0.172549, 0.176471, 0.180392, 0.184314,
0.188235, 0.192157, 0.196078, 0.200000, 0.203922, 0.207843, 0.211765, 0.215686,
0.219608, 0.223529, 0.227451, 0.231373, 0.235294, 0.239216, 0.243137, 0.247059,
0.250980, 0.254902, 0.258824, 0.262745, 0.266667, 0.270588, 0.274510, 0.278431,
0.282353, 0.286275, 0.290196, 0.294118, 0.298039, 0.301961, 0.305882, 0.309804,
0.313725, 0.317647, 0.321569, 0.325490, 0.329412, 0.333333, 0.337255, 0.341176,
0.345098, 0.349020, 0.352941, 0.356863, 0.360784, 0.364706, 0.368627, 0.372549,
0.376471, 0.380392, 0.384314, 0.388235, 0.392157, 0.396078, 0.400000, 0.403922,
0.407843, 0.411765, 0.415686, 0.419608, 0.423529, 0.427451, 0.431373, 0.435294,
0.439216, 0.443137, 0.447059, 0.450980, 0.454902, 0.458824, 0.462745, 0.466667,
0.470588, 0.474510, 0.478431, 0.482353, 0.486275, 0.490196, 0.494118, 0.498039,
0.501961, 0.505882, 0.509804, 0.513725, 0.517647, 0.521569, 0.525490, 0.529412,
0.533333, 0.537255, 0.541176, 0.545098, 0.549020, 0.552941, 0.556863, 0.560784,
0.564706, 0.568627, 0.572549, 0.576471, 0.580392, 0.584314, 0.588235, 0.592157,
0.596078, 0.600000, 0.603922, 0.607843, 0.611765, 0.615686, 0.619608, 0.623529,
0.627451, 0.631373, 0.635294, 0.639216, 0.643137, 0.647059, 0.650980, 0.654902,
0.658824, 0.662745, 0.666667, 0.670588, 0.674510, 0.678431, 0.682353, 0.686275,
0.690196, 0.694118, 0.698039, 0.701961, 0.705882, 0.709804, 0.713725, 0.717647,
0.721569, 0.725490, 0.729412, 0.733333, 0.737255, 0.741176, 0.745098, 0.749020,
0.752941, 0.756863, 0.760784, 0.764706, 0.768627, 0.772549, 0.776471, 0.780392,
0.784314, 0.788235, 0.792157, 0.796078, 0.800000, 0.803922, 0.807843, 0.811765,
0.815686, 0.819608, 0.823529, 0.827451, 0.831373, 0.835294, 0.839216, 0.843137,
0.847059, 0.850980, 0.854902, 0.858824, 0.862745, 0.866667, 0.870588, 0.874510,
0.878431, 0.882353, 0.886275, 0.890196, 0.894118, 0.898039, 0.901961, 0.905882,
0.909804, 0.913725, 0.917647, 0.921569, 0.925490, 0.929412, 0.933333, 0.937255,
0.941176, 0.945098, 0.949020, 0.952941, 0.956863, 0.960784, 0.964706, 0.968627,
0.972549, 0.976471, 0.980392, 0.984314, 0.988235, 0.992157, 0.996078, 1.000000
};
#endif


// Contructors.
template<>
inline
PXL_Pixel<unsigned char, 0>::PXL_Pixel(unsigned int black,
				       unsigned int white,
				       unsigned char value)
{
    PXLdetermineProcCaps();
    myType = PXL_INT8;
    myBlack = black;
    myWhite = white;
    myValue = value;
    if(myBlack != myWhite)
	myPntScale = 1.0f / (myWhite - myBlack);
    else myPntScale = 1.0f;
}

template<>
inline
PXL_Pixel<unsigned char, 1>::PXL_Pixel(unsigned int black,
				       unsigned int white,
				       unsigned char value)
{
    PXLdetermineProcCaps();
    myType = PXL_INT8;
    myBlack = black;
    myWhite = white;
    myValue = value;
    myPntScale = 1.0f;
}

template<>
inline
PXL_Pixel<unsigned char, 0>::PXL_Pixel(const PXL_Pixel &pix)
{
    PXLdetermineProcCaps();
    myType = PXL_INT8;

    myBlack = pix.myBlack;
    myWhite = pix.myWhite;
    myValue = pix.myValue;
    if(myBlack != myWhite)
	myPntScale = 1.0f / (myWhite - myBlack);
    else myPntScale = 1.0f;
}


template<>
inline
PXL_Pixel<unsigned char, 1>::PXL_Pixel(const PXL_Pixel &pix)
{
    PXLdetermineProcCaps();
    myType = PXL_INT8;

    myBlack = pix.myBlack;
    myWhite = pix.myWhite;
    myValue = pix.myValue;
    myPntScale = 1.0f;
}

template<>
inline
PXL_Pixel<unsigned short, 0>::PXL_Pixel(unsigned int black,
				       unsigned int white,
				       unsigned short value)
{
    PXLdetermineProcCaps();
    myType = PXL_INT16;
    myBlack = black;
    myWhite = white;
    myValue = value;
    if(myBlack != myWhite)
	myPntScale = 1.0f / (myWhite - myBlack);
    else myPntScale = 1.0f;
}

template<>
inline
PXL_Pixel<unsigned short, 1>::PXL_Pixel(unsigned int black,
				       unsigned int white,
				       unsigned short value)
{
    PXLdetermineProcCaps();
    myType = PXL_INT16;
    myBlack = black;
    myWhite = white;
    myValue = value;
    myPntScale = 1.0f;
}

template<>
inline
PXL_Pixel<unsigned short, 0>::PXL_Pixel(const PXL_Pixel &pix)
{
    PXLdetermineProcCaps();
    myType = PXL_INT16;

    myBlack = pix.myBlack;
    myWhite = pix.myWhite;
    myValue = pix.myValue;
    if(myBlack != myWhite)
	myPntScale = 1.0f / (myWhite - myBlack);
    else myPntScale = 1.0f;
}

template<>
inline
PXL_Pixel<unsigned short, 1>::PXL_Pixel(const PXL_Pixel &pix)
{
    PXLdetermineProcCaps();
    myType = PXL_INT16;

    myBlack = pix.myBlack;
    myWhite = pix.myWhite;
    myValue = pix.myValue;
    myPntScale = 1.0f;
}

template<>
inline
PXL_Pixel<unsigned int, 0>::PXL_Pixel(unsigned int black,
				       unsigned int white,
				       unsigned int value)
{
    PXLdetermineProcCaps();
    myType = PXL_INT32;
    myBlack = black;
    myWhite = white;
    myValue = value;
    if(myBlack != myWhite)
	myPntScale = 1.0f / (myWhite - myBlack);
    else myPntScale = 1.0f;
}

template<>
inline
PXL_Pixel<unsigned int, 1>::PXL_Pixel(unsigned int black,
				       unsigned int white,
				       unsigned int value)
{
    PXLdetermineProcCaps();
    myType = PXL_INT32;
    myBlack = black;
    myWhite = white;
    myValue = value;
    myPntScale = 1.0f;
}

template<>
inline
PXL_Pixel<unsigned int, 0>::PXL_Pixel(const PXL_Pixel &pix)
{
    PXLdetermineProcCaps();
    myType = PXL_INT32;

    myBlack = pix.myBlack;
    myWhite = pix.myWhite;
    myValue = pix.myValue;
    if(myBlack != myWhite)
	myPntScale = 1.0f / (myWhite - myBlack);
    else myPntScale = 1.0f;
}


template<>
inline
PXL_Pixel<unsigned int, 1>::PXL_Pixel(const PXL_Pixel &pix)
{
    PXLdetermineProcCaps();
    myType = PXL_INT32;

    myBlack = pix.myBlack;
    myWhite = pix.myWhite;
    myValue = pix.myValue;
    myPntScale = 1.0f;
}

template<>
inline
PXL_Pixel<float, 0>::PXL_Pixel(unsigned int black,
				       unsigned int white,
				       float value)
{
    PXLdetermineProcCaps();
    myType = PXL_FLOAT32;
    myBlack = black;
    myWhite = white;
    myValue = value;
    myPntScale = 1.0f;
}

template<>
inline
PXL_Pixel<float, 1>::PXL_Pixel(unsigned int black,
				       unsigned int white,
				       float value)
{
    PXLdetermineProcCaps();
    myType = PXL_FLOAT32;
    myBlack = black;
    myWhite = white;
    myValue = value;
    myPntScale = 1.0f;
}

template<>
inline
PXL_Pixel<fpreal16, 0>::PXL_Pixel(unsigned int,
				    unsigned int,
				    fpreal16 value)
{
    PXLdetermineProcCaps();
    myType = PXL_FLOAT16;
    myBlack = 0;
    myWhite = 1;
    myValue = value;
    myPntScale = 1.0f;
}

template<>
inline
PXL_Pixel<fpreal16, 1>::PXL_Pixel(unsigned int,
				    unsigned int,
				    fpreal16 value)
{
    PXLdetermineProcCaps();
    myType = PXL_FLOAT16;
    myBlack = 0;
    myWhite = 1;
    myValue = value;
    myPntScale = 1.0f;
}

template<>
inline
PXL_Pixel<float, 0>::PXL_Pixel(const PXL_Pixel &pix)
{
    PXLdetermineProcCaps();
    myType = PXL_FLOAT32;

    myBlack = pix.myBlack;
    myWhite = pix.myWhite;
    myValue = pix.myValue;
    myPntScale = 1.0f;
}


template<>
inline
PXL_Pixel<float, 1>::PXL_Pixel(const PXL_Pixel &pix)
{
    PXLdetermineProcCaps();
    myType = PXL_FLOAT32;

    myBlack = pix.myBlack;
    myWhite = pix.myWhite;
    myValue = pix.myValue;
    myPntScale = 1.0f;
}

template<>
inline
PXL_Pixel<fpreal16, 0>::PXL_Pixel(const PXL_Pixel &pix)
{
    PXLdetermineProcCaps();
    myType = PXL_FLOAT16;

    myBlack = pix.myBlack;
    myWhite = pix.myWhite;
    myValue = pix.myValue;
    myPntScale = 1.0f;
}


template<>
inline
PXL_Pixel<fpreal16, 1>::PXL_Pixel(const PXL_Pixel &pix)
{
    PXLdetermineProcCaps();
    myType = PXL_FLOAT16;

    myBlack = pix.myBlack;
    myWhite = pix.myWhite;
    myValue = pix.myValue;
    myPntScale = 1.0f;
}

template<class Type, int fast>
inline
PXL_Pixel<Type, fast>::PXL_Pixel(unsigned b, unsigned int w, Type v)
{
    PXLdetermineProcCaps();
    myBlack = b;
    myWhite = w;
    myValue = v;
    if(myBlack != myWhite)
	myPntScale = 1.0f / (myWhite - myBlack);
    else myPntScale = 1.0f;
}

template<class Type, int fast>
inline
PXL_Pixel<Type, fast>::PXL_Pixel(const PXL_Pixel &pix)
{
    PXLdetermineProcCaps();
    myType  = pix.myType;
    myBlack = pix.myBlack;
    myWhite = pix.myWhite;
    myValue = pix.myValue;
    
    if(myBlack != myWhite)
	myPntScale = 1.0f / (myWhite - myBlack);
    else myPntScale = 1.0f;
}

// format names.
template<>
inline const char * PXL_Pixel<unsigned char, 0>::formatName() const
{  return "char:slow"; }
template<>
inline const char * PXL_Pixel<unsigned char, 1>::formatName() const
{  return "char:fast"; }
template<>
inline const char * PXL_Pixel<unsigned short, 0>::formatName() const
{  return "short:slow"; }
template<>
inline const char * PXL_Pixel<unsigned short, 1>::formatName() const
{  return "short:fast"; }
template<>
inline const char * PXL_Pixel<unsigned int, 0>::formatName() const
{  return "int:slow"; }
template<>
inline const char * PXL_Pixel<unsigned int, 1>::formatName() const
{  return "int:fast"; }
template<>
inline const char * PXL_Pixel<float, 0>::formatName() const
{  return "float:slow"; }
template<>
inline const char * PXL_Pixel<float, 1>::formatName() const
{  return "float:fast"; }
template<>
inline const char * PXL_Pixel<fpreal16, 0>::formatName() const
{  return "float16:slow"; }
template<>
inline const char * PXL_Pixel<fpreal16, 1>::formatName() const
{  return "float16:fast"; }

template<class Type, int fast> inline const char *
PXL_Pixel<Type, fast>::formatName() const
{  return "unknown"; }
    

// Min/Max definitions

template<>
inline unsigned char
PXL_Pixel<unsigned char, 0>::maxValue() const
{
    return UCHAR_MAX;
}

template<>
inline unsigned char
PXL_Pixel<unsigned char, 1>::maxValue() const
{
    return UCHAR_MAX;
}

template<>
inline unsigned char
PXL_Pixel<unsigned char,0>::minValue() const
{
    return (unsigned char) 0;
}

template<>
inline unsigned char
PXL_Pixel<unsigned char,1>::minValue() const
{
    return (unsigned char) 0;
}

template<>
inline unsigned short
PXL_Pixel<unsigned short,0>::maxValue() const
{
    return USHRT_MAX;
}

template<>
inline unsigned short
PXL_Pixel<unsigned short,1>::maxValue() const
{
    return USHRT_MAX;
}

template<>
inline unsigned short
PXL_Pixel<unsigned short,0>::minValue() const
{
    return (unsigned short) 0;
}

template<>
inline unsigned short
PXL_Pixel<unsigned short,1>::minValue() const
{
    return (unsigned short) 0;
}

template<>
inline unsigned int
PXL_Pixel<unsigned int,0>::maxValue() const
{
    return UINT_MAX;
}

template<>
inline unsigned int
PXL_Pixel<unsigned int,1>::maxValue() const
{
    return UINT_MAX;
}

template<>
inline unsigned int
PXL_Pixel<unsigned int,0>::minValue() const
{
    return 0;
}

template<>
inline unsigned int
PXL_Pixel<unsigned int,1>::minValue() const
{
    return 0;
}

template<>
inline float
PXL_Pixel<float,0>::minValue() const
{
    return (float) 0.0F;
}

template<>
inline float
PXL_Pixel<float,1>::minValue() const
{
    return (float) 0.0F;
}

template<>
inline float
PXL_Pixel<float,0>::maxValue() const
{
    return 1.0F;
}

template<>
inline float
PXL_Pixel<float,1>::maxValue() const
{
    return 1.0F;
}


template<>
inline fpreal16
PXL_Pixel<fpreal16,0>::minValue() const
{
    return -H_REAL16_MAX;
}

template<>
inline fpreal16
PXL_Pixel<fpreal16,1>::minValue() const
{
    return -H_REAL16_MAX;
}

template<>
inline fpreal16
PXL_Pixel<fpreal16,0>::maxValue() const
{
    return H_REAL16_MAX;
}

template<>
inline fpreal16
PXL_Pixel<fpreal16,1>::maxValue() const
{
    return H_REAL16_MAX;
}


// specific 8 bit pixel definitions.... 0-255   *****************************

template<>
inline unsigned char
PXL_Pixel<unsigned char,0>::operator/(int val) const
{
    int result = (int(myValue) - myBlack) *maxValue() /(myWhite - myBlack);
    result /= val;
    result = result * (myWhite - myBlack) / maxValue() + myBlack;

    if(result > maxValue())
	result = maxValue();
    else if(result < 0)
	result = 0;
    
    return result;
}

template<>
inline unsigned char
PXL_Pixel<unsigned char,1>::operator/(int val) const
{
    return (val > 0) ? (myValue / val) : 0;
}

template<>
inline unsigned char
PXL_Pixel<unsigned char,0>::operator*(int val) const
{
    int64	result = myValue;
    int64	max    = maxValue();

    result = (int64(myValue) - myBlack) * max  / (myWhite - myBlack);
    result *= val;
    result = result * (myWhite - myBlack) / max + myBlack;

    if(result > max)
	result = max;
    else if(result < 0)
	result = 0;

    return result;
}

template<>
inline unsigned char
PXL_Pixel<unsigned char,1>::operator*(int val) const
{
    int64	max    = maxValue();
    int64	result = myValue * val;

    if(result > max)
	result = max;
    else if(result < 0)
	result = 0;

    return result;
}

template<>
inline unsigned char
PXL_Pixel<unsigned char,0>::operator*(float fval) const
{
    int64	result;
    int64	max    = maxValue();
    double f;
    
    result = (int(myValue) - myBlack) *max / (myWhite - myBlack);
	
    f = result * fval;
    result = (f > 0.0) ? int64(f + 0.5) : int64(f-0.5);
    result = result *(myWhite+myBlack) / max + myBlack;
    
    if(result > max)
	result = max;
    else if(result < 0)
	result = 0;
    
    return (unsigned char) result;
}

template<>
inline unsigned char
PXL_Pixel<unsigned char,1>::operator*(float fval) const
{
    int64	result;
    int64	max    = maxValue();
    
    if(fval > 0.0F)
    {
	double	f;
	
	result = myValue;
	f = result * fval;
	result = (f>0.0) ? int64(f + 0.5) : int64(f - 0.5);
    }
    else
	result = 0;
    
    if(result > max)
	result = max;
    else if(result < 0)
	result = 0;
    
    return (unsigned char) result;
}

template<>
inline PXL_Pixel<unsigned char,0>::operator unsigned char () const
{
    int result = (int(myValue) - myBlack) *UCHAR_MAX / (myWhite - myBlack);

    if(result > UCHAR_MAX)
	result = UCHAR_MAX;
    else if(result < 0)
	result = 0;

    return (unsigned char) result;
}

template<>
inline PXL_Pixel<unsigned char,1>::operator unsigned char () const
{
    return myValue;
}

template<>
inline PXL_Pixel<unsigned char,0>::operator unsigned short () const
{
    int64 result = (((int64)myValue) - ((int64)myBlack)) *((int64)USHRT_MAX)
			/ (int64)(myWhite - myBlack);

    if(result > (int64)USHRT_MAX)
	result = (int64)USHRT_MAX;
    else if(result < 0)
	result = 0;

    return (unsigned short) (result&0x0000FFFFU);
}

template<>
inline PXL_Pixel<unsigned char,1>::operator unsigned short () const
{
    unsigned short val = (((unsigned short)myValue) << 8)  +
	((unsigned short)myValue);
    
    return val;
}


template<>
inline PXL_Pixel<unsigned char,0>::operator unsigned int () const
{
    int64 val = (int64)((myValue) - myBlack) *
	           ((int64)UINT_MAX) / ((int64)(myWhite - myBlack));

    if(val < 0) val =0;
    if(val > (int64) UINT_MAX) val = (int64) UINT_MAX;

    return (unsigned int) val;
}

template<>
inline PXL_Pixel<unsigned char,1>::operator unsigned int () const
{
    unsigned int result = (((unsigned int)myValue) << 24U) +
		 (((unsigned int)myValue) << 16U) +
		 (((unsigned int)myValue) <<  8U) +
		 (((unsigned int)myValue));
    return result;
}

template<>
inline PXL_Pixel<unsigned char,0>::operator float () const
{
    return float((int)(myValue - myBlack)) * myPntScale;
}

template<>
inline PXL_Pixel<unsigned char,1>::operator float () const
{
#ifdef USE_LUT_FOR_8_TO_FP_CONV
      return lut8bitFP[myValue];
#else
    return (((int)myValue) * 3.921569e-03f); 
#endif    
}

template<>
inline void
PXL_Pixel<unsigned char,0>::mapValue(unsigned int v)
{
    int max    = maxValue();
    int half   = (max>>1);
    int result = (v * (myWhite - myBlack) + half) / max  + myBlack;

    if(result > max)
	result = max;
    else if(result < 0)
	result = 0;

    myValue = (unsigned char) result;
}

template<>
inline void
PXL_Pixel<unsigned char,1>::mapValue(unsigned int v)
{
    int max    = maxValue();
    
    if(v > max)
	v = max;

    myValue = (unsigned char) v;
}

template<>
inline  int
PXL_Pixel<unsigned char,0>::getIntFromFloat(float f) const
{
    int		range = myWhite - myBlack;
    float	max   = float(int(maxValue()) - myBlack) / float(range);
    float	min   = float(int(minValue()) - myBlack) / float(range);

    if(f > max)
	f = max;
    else if(f < min)
	f = min;
    
    return (int) SYSfloorIL(range *f +myBlack +0.5F);
}

template<>
inline  int
PXL_Pixel<unsigned char,1>::getIntFromFloat(float f) const
{
    if(f > 1.0F)
	f = 1.0F;
    else if(f < 0.0F)
	f = 0.0F;
    
    return int(SYSfloorIL(maxValue() *f +0.5F));
}

template<>
inline void
PXL_Pixel<unsigned char, 0>::setBWPoints(float b, float w)
{
    if(b < 0.0F) b = 0.0F;
    if(w < 0.0F) w = 0.0F;
    if(b > 1.0F) b = 1.0F;
    if(w > 1.0F) w = 1.0F;
    
    myBlack = (unsigned int)SYSfloorIL(b * UCHAR_MAX + 0.5F);
    myWhite = (unsigned int)SYSfloorIL(w * UCHAR_MAX + 0.5F);
    if(myBlack != myWhite)
	myPntScale = 1.0f / (myWhite - myBlack);
    else
	myPntScale = 1.0f;
}

template<>
inline void
PXL_Pixel<unsigned char, 1>::setBWPoints(float , float )
{
    myPntScale = 1.0f; // the fast version does not use b/w points.
}


// 16 bit unsigned definitions (0-65535) ************************************

template<>
inline unsigned short
PXL_Pixel<unsigned short,0>::operator/(int val) const
{
    int64 result = (int64(myValue) -myBlack) * USHRT_MAX / (myWhite - myBlack);
    result /= val;
    result = result * (myWhite - myBlack) / USHRT_MAX + myBlack;

    if(result > maxValue())
	result = maxValue();
    else if(result < 0)
	result = 0;

    return (unsigned short) result;
}

template<>
inline unsigned short
PXL_Pixel<unsigned short,1>::operator/(int val) const
{
    return (val > 0) ? (myValue / val) : 0;
}

template<>
inline unsigned short
PXL_Pixel<unsigned short,0>::operator*(int val) const
{
    int64	result = myValue;
    int64	max    = maxValue();

    result = (int64(myValue) - myBlack) * USHRT_MAX / (myWhite - myBlack);
    result *= val;
    result = result * (myWhite - myBlack) / USHRT_MAX + myBlack;

    if(result > max)
	result = max;
    else if(result < 0)
	result = 0;

    return result;
}

template<>
inline unsigned short
PXL_Pixel<unsigned short,1>::operator*(int val) const
{
    int64	result = myValue * val;
    int64	max    = maxValue();

    if(result > max)
	result = max;
    else if(result < 0)
	result = 0;

    return result;
}

template<>
inline unsigned short
PXL_Pixel<unsigned short,0>::operator*(float fval) const
{
    int64	result;
    int64	max    = maxValue();
    
    result = (int(myValue) - myBlack) *max / (myWhite - myBlack);
    double f;
	
    f = result * fval;
    result = (f > 0.0) ? int64(f + 0.5) : int64(f-0.5);
    result = result *(myWhite+myBlack) / max + myBlack;
    
    if(result > max)
	result = max;
    else if(result < 0)
	result = 0;
    
    return (unsigned short) result;
}

template<>
inline unsigned short
PXL_Pixel<unsigned short,1>::operator*(float fval) const
{
    int64	result;
    int64	max    = maxValue();
    
    if(fval > 0.0F)
    {
	double	f;
	
	result = myValue;
	f = result * fval;
	result = (f>0.0) ? int64(f + 0.5) : int64(f - 0.5);
    }
    else
	result = 0;
    
    if(result > max)
	result = max;
    else if(result < 0)
	result = 0;
    
    return (unsigned short) result;
}

template<>
inline PXL_Pixel<unsigned short,0>::operator unsigned char () const
{
    float result = ((float)myValue-(float)myBlack) / float(myWhite-myBlack);

    if(result > 1.0f)
	result = 1.0f;
    else if(result < 0.0f)
	result = 0.0f;

    return (int) SYSfloorIL(result * UCHAR_MAX);
}

template<>
inline PXL_Pixel<unsigned short,1>::operator unsigned char () const
{
    unsigned char val = (unsigned char) ((myValue >> 8)&0x00FF);
    if(val < UCHAR_MAX && (myValue &0x0080))
	val++;

    return val;
}

template<>
inline PXL_Pixel<unsigned short,0>::operator unsigned short () const
{
    int result = (int(myValue) - myBlack) *USHRT_MAX / (myWhite - myBlack);

    if(result > USHRT_MAX)
	result = USHRT_MAX;
    else if(result < 0)
	result = 0;
    
    return (unsigned short) result;
}

template<>
inline PXL_Pixel<unsigned short,1>::operator unsigned short () const
{
    return myValue;
}

template<>
inline PXL_Pixel<unsigned short,0>::operator unsigned int () const
{
    float val = float(myValue - myBlack) *myPntScale;

    if(val < 0.0f) val = 0.0f;
    else if(val > 1.0f) val = 1.0f;

    return (unsigned int) SYSfloorIL(val * UINT_MAX);
}

template<>
inline PXL_Pixel<unsigned short,1>::operator unsigned int () const
{
    unsigned int result = (((unsigned int)myValue) << 15) +
			(((unsigned int)myValue) >>  1);
    return result;
}

template<>
inline PXL_Pixel<unsigned short,0>::operator float () const
{
    return float((int)(myValue - myBlack)) * myPntScale;
}

template<>
inline PXL_Pixel<unsigned short,1>::operator float () const
{
    return ((int)myValue) * 1.5259021e-5f;
}

template<>
inline void
PXL_Pixel<unsigned short,0>::mapValue(unsigned int v)
{
    int max    = maxValue();
    int half   = (max>>1);
    int result = (v * (myWhite - myBlack) + half) / max  + myBlack;

    if(result > max)
	result = max;
    else if(result < 0)
	result = 0;

    myValue = (unsigned short) result;
}

template<>
inline void
PXL_Pixel<unsigned short,1>::mapValue(unsigned int v)
{
    int max    = maxValue();

    if(v > max)
	v = max;

    myValue = (unsigned short) v;
}


template<>
inline  int
PXL_Pixel<unsigned short,0>::getIntFromFloat(float f) const
{
    int		range = myWhite - myBlack;
    float	max   = float(int(maxValue()) - myBlack) / float(range);
    float	min   = float(int(minValue()) - myBlack) / float(range);

    if(f > max)
	f = max;
    else if(f < min)
	f = min;

    return int(SYSfloorIL(range *f +myBlack +0.5F));
}

template<>
inline  int
PXL_Pixel<unsigned short,1>::getIntFromFloat(float f) const
{
    if(f > 1.0F)
	f = 1.0F;
    else if(f < 0.0F)
	f = 0.0F;
    
    return int(SYSfloorIL(maxValue() *f +0.5F));
}

template<>
inline void
PXL_Pixel<unsigned short, 0>::setBWPoints(float b, float w)
{
    if(b < 0.0F) b = 0.0F;
    if(w < 0.0F) w = 0.0F;
    if(b > 1.0F) b = 1.0F;
    if(w > 1.0F) w = 1.0F;
    
    myBlack = (unsigned int)SYSfloorIL(b * USHRT_MAX + 0.5F);
    myWhite = (unsigned int)SYSfloorIL(w * USHRT_MAX + 0.5F);
    if(myBlack != myWhite)
	myPntScale = 1.0f / (myWhite - myBlack);
    else
	myPntScale = 1.0f;
}

template<>
inline void
PXL_Pixel<unsigned short, 1>::setBWPoints(float , float )
{
    myPntScale = 1.0f; // the fast version does not use b/w points.
}






// 32 Bit Integer (-2e9 to 2e9)		**************************************


template<>
inline PXL_Pixel<unsigned int,1>::operator unsigned int () const
{
    return myValue;
}

template<>
inline PXL_Pixel<unsigned int,0>::operator float () const
{
    return (float)((double(myValue) - myBlack) * myPntScale);
}

template<>
inline PXL_Pixel<unsigned int,1>::operator float () const
{
    return myValue / double(UINT_MAX);
}

template<>
inline unsigned int
PXL_Pixel<unsigned int,0>::operator/(int val) const
{
    int64 result = (int64(myValue) -myBlack)*INT_MAX / (myWhite - myBlack);
    result /= val;
    result = result * (myWhite - myBlack) / INT_MAX + myBlack;

    if(result > maxValue())
	result = maxValue();
    else if(result < minValue())
	result = minValue();

    return (unsigned int) result;
}

template<>
inline unsigned int
PXL_Pixel<unsigned int,1>::operator/(int val) const
{
    return (val != 0) ? (myValue / val) : 0;
}

#define ABS(x)	((x) < 0 ? -x : x)

template<>
inline unsigned int
PXL_Pixel<unsigned int,0>::operator*(int val) const
{
    int64	result = myValue;
    int64	max    = maxValue();
    int64	maxmult; 

    result = (int64(myValue) - myBlack) * INT_MAX / (myWhite - myBlack);

    // prevent overflow by determining the maximum factor allowable (the nutty
    // expression is the max value of an int64, since I know of no way to
    // declare long long constants on LINUX)
#if defined(WIN32) || (defined(LINUX) && !defined(INTEL_COMPILER))
    maxmult = ABS( ( ((~int64(0))^(int64(1)<<63)) / result ) );
#else
    maxmult = ABS(9223372036854775807LL / result);
#endif

    UT_StackTrace	trace;
    trace.doTraceback();

    // If the result will overflow, clamp to -/+ max,
    // depending on the signs of the multiplicands.
    if(abs(val) > maxmult)
    {
	if((val > 0 && result > 0) ||
	   (val < 0 && result < 0))
	{
	    result = max;
	}
	else
	{
	    result = -max -1;
	}
    }
    else
    {
	result *= val;
	result = result * (myWhite - myBlack) / INT_MAX + myBlack;
    }
    
    if(result > max)
	result = max;
    else if(result < minValue())
	result = minValue();

    return result;
}

template<>
inline unsigned int
PXL_Pixel<unsigned int,1>::operator*(int val) const
{
    int64	result = myValue * val;
    int64	max    = maxValue();

    if(result > max)
	result = max;
    else if(result < minValue())
	result = minValue();

    return result;
}

template<>
inline unsigned int
PXL_Pixel<unsigned int,0>::operator*(float fval) const
{
    int64	result;
    int64	max    = maxValue();
    double f;
    
    result = (int(myValue) - myBlack) * max / (myWhite - myBlack);
    
    f = result * fval;
	
    result = (f > 0.0) ? int64(f + 0.5) : int64(f-0.5);
    result = result *(myWhite+myBlack) / max + myBlack;
    
    if(result > max)
	result = max;
    else if(result < minValue())
	result = minValue();
    
    return (int) result;
}

template<>
inline unsigned int
PXL_Pixel<unsigned int,1>::operator*(float fval) const
{
    int64	result;
    int64	max    = maxValue();
    double	f;
	    
    result = myValue;
    f = result * fval;
    result = (f>0.0) ? int64(f + 0.5) : int64(f - 0.5);
	
    if(result > max)
	result = max;
    else if(result < minValue())
	result = minValue();
    
    return (int) result;
}

template<>
inline PXL_Pixel<unsigned int,0>::operator unsigned char () const
{
    int64 result = (((int64)(myValue)) -myBlack) *UCHAR_MAX /(myWhite-myBlack);

    if(result > UCHAR_MAX)
	result = UCHAR_MAX;
    else if(result < 0)
	result = 0;

    return (unsigned char) result;
}

template<>
inline PXL_Pixel<unsigned int,1>::operator unsigned char () const
{
    unsigned int val;

    val = (unsigned char) ((myValue >> 24U)&0x000000FFU);
    if(val < UCHAR_MAX && (myValue & 0x00800000U))
	val++;
    
    return val;
}

template<>
inline PXL_Pixel<unsigned int,0>::operator unsigned short () const
{
    int64 result = (int64)((int64)myValue-(int64)myBlack) *((int64)USHRT_MAX) /
		  ((int64)(myWhite - myBlack));

    if(result > (int64) USHRT_MAX)
	result = (int64) USHRT_MAX;
    else if(result < 0)
	result = 0;

    return (unsigned short) (result&0x0000FFFF);
}

template<>
inline PXL_Pixel<unsigned int,1>::operator unsigned short () const
{
    int val;
    
    val = (unsigned short) ((myValue >> 16U)&0x0000FFFFU);
    if(val < USHRT_MAX && (myValue & 0x00008000U))
	val++;
    
    return (unsigned short) val;
}

template<>
inline PXL_Pixel<unsigned int,0>::operator unsigned int () const
{
    int64	result = myValue;
    result = (result - myBlack) *INT_MAX / (myWhite - myBlack);
    if(result > INT_MAX)
	result = INT_MAX;
    else if(result < INT_MIN)
	result = INT_MIN;

    return (unsigned int) result;
}

template<>
inline void
PXL_Pixel<unsigned int,0>::mapValue(unsigned int v)
{
    int64 max    = maxValue();
    int64 half   = (max>>1);
    int64 result = (int64(v) * (myWhite - myBlack) + half) / max  + myBlack;

    if(result > max)
	result = max;
    else if(result < minValue())
	result = minValue();

    myValue = (unsigned int) result;
}

template<>
inline void
PXL_Pixel<unsigned int,1>::mapValue(unsigned int v)
{
    int64 max    = maxValue();

    if(v > max)
	v = max;
    else if(v < minValue())
	v = minValue();

    myValue = (int) v;
}

template<>
inline  int
PXL_Pixel<unsigned int,0>::getIntFromFloat(float f) const
{
    int		range = myWhite - myBlack;
    float	max   = float(maxValue() - myBlack) / float(range);
    float	min   = float(minValue() - myBlack) / float(range);

    if(f > max)
	f = max;
    else if(f < min)
	f = min;

    return int(SYSfloorIL(range *f +myBlack +0.5F));
}

template<>
inline int
PXL_Pixel<unsigned int,1>::getIntFromFloat(float f) const
{
    if(f > 1.0F)
	f = 1.0F;
    else if(f < 0.0F)
	f = 0.0F;

    return (unsigned int)(SYSfloorIL(maxValue() *f +0.5F));
}

template<>
inline void
PXL_Pixel<unsigned int, 0>::setBWPoints(float b, float w)
{
    if(b < 0.0F) b = 0.0F;
    if(w < 0.0F) w = 0.0F;
    if(b > 1.0F) b = 1.0F;
    if(w > 1.0F) w = 1.0F;
    
    myBlack = (unsigned int)SYSfloorIL(b * UINT_MAX + 0.5F);
    myWhite = (unsigned int)SYSfloorIL(w * UINT_MAX + 0.5F);
    if(myBlack != myWhite)
	myPntScale = 1.0f / (myWhite - myBlack);
    else
	myPntScale = 1.0f;
}

template<>
inline void
PXL_Pixel<unsigned int, 1>::setBWPoints(float , float )
{
    myPntScale = 1.0f;    // the fast version does not use b/w points.
}



// 32 bit floating point pixels    *****************************************

template<>
inline float
PXL_Pixel<float,0>::operator +(int val) const
{
    return myValue + val;
}

template<>
inline float
PXL_Pixel<float,1>::operator +(int val) const
{
    return myValue + val;
}

template<>
inline float
PXL_Pixel<float,0>::operator/(int val) const
{
    return myValue / val;
}

template<>
inline float
PXL_Pixel<float,1>::operator/(int val) const
{
    return myValue / val;
}

template<>
inline float
PXL_Pixel<float,0>::operator*(int val) const
{
    return myValue * val;
}

template<>
inline float
PXL_Pixel<float,1>::operator*(int val) const
{
    return myValue * val;
}

template<>
inline float
PXL_Pixel<float,0>::operator + (float fval) const
{
    return myValue + fval;
}

template<>
inline float
PXL_Pixel<float,1>::operator + (float fval) const
{
    return myValue + fval;
}

template<>
inline float
PXL_Pixel<float,0>::operator / (float fval) const
{
    return myValue / fval;
}

template<>
inline float
PXL_Pixel<float,1>::operator / (float fval) const
{
    return myValue / fval;
}

template<>
inline float
PXL_Pixel<float,0>::operator * (float fval) const
{
    return myValue * fval;
}

template<>
inline float
PXL_Pixel<float,1>::operator * (float fval) const
{
    return myValue * fval;
}

template<>
inline float
PXL_Pixel<float,0>::operator +=(float fval)
{
    return myValue += fval;
}

template<>
inline float
PXL_Pixel<float,1>::operator +=(float fval)
{
    return myValue += fval;
}

template<>
inline PXL_Pixel<float,0>::operator unsigned char () const
{
    unsigned char val;
    float f;

    f = myValue;
    if(f < 0.0F)
	f = 0.0F;
    else if(f > 1.0F)
	f = 1.0F;
       
    val = (unsigned char) SYSfloorIL(f * UCHAR_MAX + 0.5F);

    return val;
}

template<>
inline PXL_Pixel<float,1>::operator unsigned char () const
{
    unsigned char val;
    float f;

    f = myValue;
    if(f < 0.0F)
	f = 0.0F;
    else if(f > 1.0F)
	f = 1.0F;
       
    val = (unsigned char) SYSfloorIL(f * UCHAR_MAX + 0.5F);

    return val;
}

template<>
inline PXL_Pixel<float,0>::operator unsigned short () const
{
    unsigned short val;
    float f;

    f = myValue;
    if(f < 0.0F)
	f = 0.0F;
    else if(f > 1.0F)
	f = 1.0F;
       
    val = (unsigned short) SYSfloorIL(f * USHRT_MAX + 0.5F);

    return val;
}

template<>
inline PXL_Pixel<float,1>::operator unsigned short () const
{
    unsigned short val;
    float f;

    f = myValue;
    if(f < 0.0F)
	f = 0.0F;
    else if(f > 1.0F)
	f = 1.0F;
       
    val = (unsigned short) SYSfloorIL(f * USHRT_MAX + 0.5F);

    return val;
}

template<>
inline PXL_Pixel<float,0>::operator unsigned int () const
{
    unsigned int result;
    float f;

    f = myValue;
    if(f < 0.0F)
	f = 0.0F;
    else if(f > 1.0F)
	f = 1.0F;

    result = (unsigned int)SYSfloorIL(f * UINT_MAX);
        
    return result;
}

template<>
inline PXL_Pixel<float,1>::operator unsigned int () const
{
    unsigned int result;
    float f;

    f = myValue;
    if(f < 0.0F)
	f = 0.0F;
    else if(f > 1.0F)
	f = 1.0F;

    result = (unsigned int)SYSfloorIL(f * UINT_MAX);
        
    return result;
}

template<>
inline PXL_Pixel<float,0>::operator float () const
{
    return myValue;
}

template<>
inline PXL_Pixel<float,1>::operator float () const
{
    return myValue;
}

template<>
inline PXL_Pixel<fpreal16,0>::operator float () const
{
    return (float)myValue;
}

template<>
inline PXL_Pixel<fpreal16,1>::operator float () const
{
    return (float)myValue;
}

template<>
inline float
PXL_Pixel<float,0>::assign(unsigned int val)
{
    return (myValue = (float)val);
}

template<>
inline float
PXL_Pixel<float,1>::assign(unsigned int val)
{
    return (myValue = (float)val);
}

template<>
inline bool
PXL_Pixel<float,0>::operator==(unsigned int val) const
{
    return SYSisEqual(myValue, float(val));
}

template<>
inline bool
PXL_Pixel<float,1>::operator==(unsigned int val) const
{
    return SYSisEqual(myValue, float(val));
}

template<>
inline float
PXL_Pixel<float,0>::operator =(float fval)
{
    return (myValue = fval);
}

template<>
inline float
PXL_Pixel<float,1>::operator =(float fval)
{
    return (myValue = fval);
}

template<>
inline bool
PXL_Pixel<float,0>::operator==(float fval) const
{
    return SYSisEqual(myValue, fval);
}

template<>
inline bool
PXL_Pixel<float,1>::operator==(float fval) const
{
    return SYSisEqual(myValue, fval);
}

template<>
inline  int
PXL_Pixel<float,0>::getIntFromFloat(float f) const
{
    return (int)f;
}

template<>
inline  int
PXL_Pixel<float,1>::getIntFromFloat(float f) const
{
    return (int)f;
}


template<>
inline fpreal16
PXL_Pixel<fpreal16,0>::assign(unsigned int val)
{
    return (myValue = (float)val);
}

template<>
inline fpreal16
PXL_Pixel<fpreal16,1>::assign(unsigned int val)
{
    return (myValue = (float)val);
}

template<>
inline bool
PXL_Pixel<fpreal16,0>::operator==(unsigned int val) const
{
    return SYSisEqual((float)myValue, float(val));
}

template<>
inline bool
PXL_Pixel<fpreal16,1>::operator==(unsigned int val) const
{
    return SYSisEqual((float)myValue, float(val));
}

template<>
inline fpreal16
PXL_Pixel<fpreal16,0>::operator =(float fval)
{
    myValue = fval;
    return (float) myValue;
}

template<>
inline fpreal16
PXL_Pixel<fpreal16,1>::operator =(float fval)
{
    myValue = fval;
    return (float) myValue;
}

template<>
inline bool
PXL_Pixel<fpreal16,0>::operator==(float fval) const
{
    return SYSisEqual((float)myValue, fval);
}

template<>
inline bool
PXL_Pixel<fpreal16,1>::operator==(float fval) const
{
    return SYSisEqual((float)myValue, fval);
}

template<>
inline  int
PXL_Pixel<fpreal16,0>::getIntFromFloat(float f) const
{
    return (int) ((float)f);
}

template<>
inline  int
PXL_Pixel<fpreal16,1>::getIntFromFloat(float f) const
{
    return (int) ((float)f);
}

// Generic Definitions			**************************************

template<class Type, int fast> inline bool
PXL_Pixel<Type,fast>::isSameType(PXL_DataFormat format,
				 unsigned b, unsigned w) const
{
    return myType == format && (myType == PXL_FLOAT16 ||
				myType == PXL_FLOAT32 ||
				(b == myBlack && w == myWhite));
}


template<class Type, int fast> inline Type
PXL_Pixel<Type,fast>::operator+(int val) const
{
    Type result = myValue;
    Type max    = maxValue();

    if(val > 0)
    {
	if(val > max - myValue)
	    result = max;
	else
	    result += val;
    }
    else if(val < 0)
    {
	if(-val > myValue)
	    result = 0;
	else
	    result += val;
    }
	    
    return result;
}

template<class Type, int fast> inline Type
PXL_Pixel<Type,fast>::operator-(int val) const
{
    return *this + (-val);
}

template<class Type, int fast> inline  Type
PXL_Pixel<Type,fast>::operator+=(int val)
{
    myValue = *this + val;
    return myValue;
}

template<class Type, int fast> inline  Type
PXL_Pixel<Type,fast>::operator-=(int val)
{
    myValue = *this + (-val);
    return myValue;
}

template<class Type, int fast> inline  Type
PXL_Pixel<Type,fast>::operator/=(int val)
{
    myValue = *this / val;
    return myValue; 
}

template<class Type, int fast> inline  Type
PXL_Pixel<Type,fast>::operator*=(int val)
{
    myValue = *this * val;
    return myValue;
}


template<class Type, int fast> inline  Type
PXL_Pixel<Type,fast>::operator+(float fval) const
{
    return *this + getIntFromFloat(fval);
}

template<class Type, int fast> inline  Type
PXL_Pixel<Type,fast>::operator-(float fval) const
{
    return *this + (-fval);
}

template<class Type, int fast> inline  Type
PXL_Pixel<Type,fast>::operator/(float fval) const
{
    if(fval == 0.0F)
	return maxValue();
    else
	return *this * (1.0F/fval);
}


template<class Type, int fast> inline  Type
PXL_Pixel<Type,fast>::operator+=(float fval)
{
    int val = (int) SYSfloorIL((myWhite-myBlack) *fval +myBlack + 0.5F);
    return *this += val;
}

template<class Type, int fast> inline  Type
PXL_Pixel<Type,fast>::operator-=(float fval)
{
    return *this += (-fval);
}

template<class Type, int fast> inline  Type
PXL_Pixel<Type,fast>::operator*=(float fval)
{
    return myValue = (*this * fval);
}

template<class Type, int fast> inline  Type
PXL_Pixel<Type,fast>::operator/=(float fval)
{
    if(fval == 0.0F)
    {
	myValue = maxValue();
	return myValue;
    }
    else
	return *this *= (1.0F/fval);
}

template<>
inline unsigned char
PXL_Pixel<unsigned char,1>::operator =(float fval)
{
    int t = (int) SYSfloorIL(fval * 255.0f +0.5F);

    myValue = (unsigned char) PXLclampChar(t);
	
    return myValue;
}

template<>
inline unsigned char
PXL_Pixel<unsigned char,0>::operator =(float fval)
{
    float	f   = fval * float(myWhite - myBlack) + (float)myBlack;
    int t = (int) SYSfloorIL(f +0.5F);

    myValue = (unsigned char) PXLclampChar(t);

    return myValue;
}

template<>
inline unsigned short
PXL_Pixel<unsigned short,1>::operator =(float fval)
{
    int t = (int) SYSfloorIL(fval * 65535.0f +0.5F);

    myValue = (unsigned short) PXLclampShort(t);
    
    return myValue;
}

template<>
inline unsigned short
PXL_Pixel<unsigned short,0>::operator =(float fval)
{
    float  f = fval * float(myWhite - myBlack) + (float)myBlack;
    int    t = (int) SYSfloorIL(f +0.5F);

    myValue = PXLclampShort(t);
    
    return myValue;
}

template<class Type, int fast> inline  Type
PXL_Pixel<Type,fast>::operator =(float fval)
{
    double	f   = fval * double(myWhite - myBlack) + myBlack;

    f = SYSfloorIL(f +0.5F);

    if(f < minValue())
	set(minValue());
    else if(f > maxValue())
	set(maxValue());
    else
	set((unsigned int) f);

    return myValue;
}

template<class Type, int fast> inline  Type
PXL_Pixel<Type,fast>::assign(unsigned int val)
{
    if(val > maxValue())
	myValue = maxValue();
    else if(val < minValue())
	myValue = minValue();
    else
	myValue = val;

    return myValue;
}

template<class Type, int fast> inline  Type
PXL_Pixel<Type,fast>::set(Type val)
{
    myValue = val;
    return myValue;
}



template<class Type, int fast> inline bool
PXL_Pixel<Type,fast>::operator==(float fval) const
{
    unsigned int val = (int) SYSfloorIL(fval *(myWhite-myBlack) +myBlack +0.5F);
    
    return (*this == val);
}

template<class Type, int fast> inline bool
PXL_Pixel<Type,fast>::operator==(unsigned int val) const
{
    if(val > maxValue())
	val = maxValue();
    
    return (val == myValue) ? 1 : 0;
}

template<class Type, int fast> inline  void
PXL_Pixel<Type,fast>::setRange(unsigned int b, unsigned int w)
{
    myWhite = w;
    myBlack = b;
}

template<class Type, int fast> inline  void
PXL_Pixel<Type,fast>::mapValue(unsigned int v)
{
    // no mapping done.
    myValue = v;
}

template<class Type, int fast> void
PXL_Pixel<Type, fast>::setBWPoints(float , float )
{
    myPntScale = 1.0f;
}

#endif
