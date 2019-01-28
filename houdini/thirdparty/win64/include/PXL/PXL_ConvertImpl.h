/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PXL_Convert.C
 *
 * COMMENTS:
 *	Conversion routines for converting a pixel to other pixel types.
 */
#ifndef PXL_CONVERT_C
#define PXL_CONVERT_C

#include "PXL_Convert.h"

// Very simple, but works in only a few cases (with anything and float). 
template<class ToType, int fast1, class FromType, int fast2>
inline void
PXL_Convert<ToType,fast1, FromType, fast2>::convert(PXL_Pixel<ToType,fast1> &to,
						   const PXL_Pixel<FromType,fast2> &from)
{
    to = ((float) from);
}

// 8 bit conversions
template<>
inline void
PXL_Convert<unsigned char,0,unsigned char,0>::convert(PXL_Pixel<unsigned char,0> &to,
						     const PXL_Pixel<unsigned char,0> &from)
{
    if(to.getBlack() == from.getBlack() && to.getWhite() == from.getWhite())
	to.assign((unsigned char) from.getValue());
    else
    {
	// ranges do not match -- convert to int and back.
	float f = (float) from;
	to = f;
    }
}

template<>
inline void
PXL_Convert<unsigned char,1,unsigned char,0>::convert(PXL_Pixel<unsigned char,1> &to,
						     const PXL_Pixel<unsigned char,0> &from)
{
    if(to.getBlack() == from.getBlack() && to.getWhite() == from.getWhite())
	to.assign((unsigned char) from.getValue());
    else
    {
	// ranges do not match -- convert to int and back.
	float f = (float) from;
	to = f;
    }
}

template<>
inline void
PXL_Convert<unsigned char,0,unsigned char,1>::convert(PXL_Pixel<unsigned char,0> &to,
						     const PXL_Pixel<unsigned char,1> &from)
{
    if(to.getBlack() == from.getBlack() && to.getWhite() == from.getWhite())
	to.assign((unsigned char) from.getValue());
    else
    {
	// ranges do not match -- convert to int and back.
	float f = (float) from;
	to = f;
    }
}

template<>
inline void
PXL_Convert<unsigned char,1,unsigned char,1>::convert(PXL_Pixel<unsigned char,1> &to,
						     const PXL_Pixel<unsigned char,1> &from)
{
    to.assign((unsigned char) from.getValue());
}

template<>
inline void
PXL_Convert<unsigned char,0,unsigned short,0>::convert(PXL_Pixel<unsigned char,0> &to,
						  const PXL_Pixel<unsigned short,0> &from)
{
    if(to.getBlack() == 0 && to.getWhite() == to.maxValue())
	to.assign((unsigned char) from);
    else
    {
	float f = (float) from;
	to = f;
    }
}

template<>
inline void
PXL_Convert<unsigned char,0,unsigned short,1>::convert(PXL_Pixel<unsigned char,0> &to,
						  const PXL_Pixel<unsigned short,1> &from)
{
    if(to.getBlack() == 0 && to.getWhite() == to.maxValue())
	to.assign((unsigned char) from);
    else
    {
	float f = (float) from;
	to = f;
    }
}

template<>
inline void
PXL_Convert<unsigned char,1,unsigned short,0>::convert(PXL_Pixel<unsigned char,1> &to,
						  const PXL_Pixel<unsigned short,0> &from)
{
    to.assign((unsigned char) from);
}

template<>
inline void
PXL_Convert<unsigned char,1,unsigned short,1>::convert(PXL_Pixel<unsigned char,1> &to,
						  const PXL_Pixel<unsigned short,1> &from)
{
    to.assign((unsigned char) from);
}


template<>
inline void
PXL_Convert<unsigned char,0,unsigned int,0>::convert(PXL_Pixel<unsigned char,0> &to,
					   const PXL_Pixel<unsigned int,0> &from)
{
    if(to.getBlack() == 0 && to.getWhite() == to.maxValue())
	to.assign((unsigned char) from);
    else
	to = (float) from;
}

template<>
inline void
PXL_Convert<unsigned char,1,unsigned int,0>::convert(PXL_Pixel<unsigned char,1> &to,
					   const PXL_Pixel<unsigned int,0> &from)
{
    if(to.getBlack() == 0 && to.getWhite() == to.maxValue())
	to.assign((unsigned char) from);
    else
	to = (float) from;
}

template<>
inline void
PXL_Convert<unsigned char,0,unsigned int,1>::convert(PXL_Pixel<unsigned char,0> &to,
					   const PXL_Pixel<unsigned int,1> &from)
{
    if(to.getBlack() == 0 && to.getWhite() == to.maxValue())
	to.assign((unsigned char) from);
    else
	to = (float) from;
}

template<>
inline void
PXL_Convert<unsigned char,1,unsigned int,1>::convert(PXL_Pixel<unsigned char,1> &to,
					   const PXL_Pixel<unsigned int,1> &from)
{
    to.assign((unsigned char) from);
}

template<>
inline void
PXL_Convert<unsigned char,0,float,0>::convert(PXL_Pixel<unsigned char,0> &to,
					 const PXL_Pixel<float,0> &from)
{
    to = (float) from;
}

template<>
inline void
PXL_Convert<unsigned char,1,float,0>::convert(PXL_Pixel<unsigned char,1> &to,
					 const PXL_Pixel<float,0> &from)
{
    to = (float) from;
}

template<>
inline void
PXL_Convert<unsigned char,0,float,1>::convert(PXL_Pixel<unsigned char,0> &to,
					 const PXL_Pixel<float,1> &from)
{
    to = (float) from;
}

template<>
inline void
PXL_Convert<unsigned char,1,float,1>::convert(PXL_Pixel<unsigned char,1> &to,
					 const PXL_Pixel<float,1> &from)
{
    to = (float) from;
}

// 16 bit conversions. ----------------------------------------------------

template<>
inline void
PXL_Convert<unsigned short,0,unsigned char,0>::convert(PXL_Pixel<unsigned short,0>&to,
					const PXL_Pixel<unsigned char,0> &from)
{
    if(to.getBlack() == 0 && to.getWhite() == to.maxValue())
	to.assign((unsigned short) from);
    else
    {
	float f = (float) from;
	to = f;
    }
}

template<>
inline void
PXL_Convert<unsigned short,1,unsigned char,0>::convert(PXL_Pixel<unsigned short,1>&to,
					const PXL_Pixel<unsigned char,0> &from)
{
    if(to.getBlack() == 0 && to.getWhite() == to.maxValue())
	to.assign((unsigned short) from);
    else
    {
	float f = (float) from;
	to = f;
    }
}

template<>
inline void
PXL_Convert<unsigned short,0,unsigned char,1>::convert(PXL_Pixel<unsigned short,0>&to,
					const PXL_Pixel<unsigned char,1> &from)
{
    if(to.getBlack() == 0 && to.getWhite() == to.maxValue())
	to.assign((unsigned short) from);
    else
    {
	float f = (float) from;
	to = f;
    }
}

template<>
inline void
PXL_Convert<unsigned short,1,unsigned char,1>::convert(PXL_Pixel<unsigned short,1>&to,
					const PXL_Pixel<unsigned char,1> &from)
{
    to.assign((unsigned short) from);
}

template<>
inline void
PXL_Convert<unsigned short,0,unsigned short,0>::convert(PXL_Pixel<unsigned short,0>&to,
					 const PXL_Pixel<unsigned short,0> &from)
{
    if(to.getBlack() == from.getBlack() && to.getWhite() == from.getWhite())
	to.assign(from.getValue());
    else
    {
	// ranges do not match -- convert to int and back.
	to = (float) from;
    }
}

template<>
inline void
PXL_Convert<unsigned short,1,unsigned short,0>::convert(PXL_Pixel<unsigned short,1>&to,
					 const PXL_Pixel<unsigned short,0> &from)
{
    if(to.getBlack() == from.getBlack() && to.getWhite() == from.getWhite())
	to.assign(from.getValue());
    else
    {
	// ranges do not match -- convert to int and back.
	to = (float) from;
    }
}

template<>
inline void
PXL_Convert<unsigned short,0,unsigned short,1>::convert(PXL_Pixel<unsigned short,0>&to,
					 const PXL_Pixel<unsigned short,1> &from)
{
    if(to.getBlack() == from.getBlack() && to.getWhite() == from.getWhite())
	to.assign(from.getValue());
    else
    {
	// ranges do not match -- convert to int and back.
	to = (float) from;
    }
}

template<>
inline void
PXL_Convert<unsigned short,1,unsigned short,1>::convert(PXL_Pixel<unsigned short,1>&to,
					 const PXL_Pixel<unsigned short,1> &from)
{
    to.assign(from.getValue());
}

template<>
inline void
PXL_Convert<unsigned short,0,unsigned int,0>::convert(PXL_Pixel<unsigned short,0> &to,
					    const PXL_Pixel<unsigned int,0> &from)
{
    if(to.getBlack() == 0 && to.getWhite() == to.maxValue())
	to.assign((unsigned short) from);
    else
	to = (float) from;
}

template<>
inline void
PXL_Convert<unsigned short,1,unsigned int,0>::convert(PXL_Pixel<unsigned short,1> &to,
					    const PXL_Pixel<unsigned int,0> &from)
{
    if(to.getBlack() == 0 && to.getWhite() == to.maxValue())
	to.assign((unsigned short) from);
    else
	to = (float) from;
}

template<>
inline void
PXL_Convert<unsigned short,0,unsigned int,1>::convert(PXL_Pixel<unsigned short,0> &to,
					    const PXL_Pixel<unsigned int,1> &from)
{
    if(to.getBlack() == 0 && to.getWhite() == to.maxValue())
	to.assign((unsigned short) from);
    else
	to = (float) from;
}

template<>
inline void
PXL_Convert<unsigned short,1,unsigned int,1>::convert(PXL_Pixel<unsigned short,1> &to,
					    const PXL_Pixel<unsigned int,1> &from)
{
    to.assign((unsigned short) from);
}

// 32 bit conversions. -----------------------------------------------------
template<>
inline void
PXL_Convert<unsigned int,0,unsigned char,0>::convert(PXL_Pixel<unsigned int,0> &to,
				       const PXL_Pixel<unsigned char,0> &from)
{
    if(to.getBlack() == 0 && to.getWhite() == to.maxValue())
	to.assign((unsigned int) from);
    else
	to = (float) from;
}

template<>
inline void
PXL_Convert<unsigned int,1,unsigned char,0>::convert(PXL_Pixel<unsigned int,1> &to,
				       const PXL_Pixel<unsigned char,0> &from)
{
    if(to.getBlack() == 0 && to.getWhite() == to.maxValue())
	to.assign(from);
    else
	to = (float) from;
}

template<>
inline void
PXL_Convert<unsigned int,0,unsigned char,1>::convert(PXL_Pixel<unsigned int,0> &to,
				       const PXL_Pixel<unsigned char,1> &from)
{
    if(to.getBlack() == 0 && to.getWhite() == to.maxValue())
	to.assign((unsigned int) from);
    else
	to = ((float) from);
}

template<>
inline void
PXL_Convert<unsigned int,1,unsigned char,1>::convert(PXL_Pixel<unsigned int,1> &to,
				       const PXL_Pixel<unsigned char,1> &from)
{
    to.assign((unsigned int) from);
}

template<>
inline void
PXL_Convert<unsigned int,0,unsigned short,0>::convert(PXL_Pixel<unsigned int,0> &to,
					const PXL_Pixel<unsigned short,0> &from)
{
    if(to.getBlack() == 0 && to.getWhite() == to.maxValue())
	to.set((unsigned int) from);
    else
	to = ((float) from);
}

template<>
inline void
PXL_Convert<unsigned int,1,unsigned short,0>::convert(PXL_Pixel<unsigned int,1> &to,
					const PXL_Pixel<unsigned short,0> &from)
{
    if(to.getBlack() == 0 && to.getWhite() == to.maxValue())
	to.set(from);
    else
	to = ((float) from);
}

template<>
inline void
PXL_Convert<unsigned int,0,unsigned short,1>::convert(PXL_Pixel<unsigned int,0> &to,
					const PXL_Pixel<unsigned short,1> &from)
{
    if(to.getBlack() == 0 && to.getWhite() == to.maxValue())
	to.assign(from);
    else
	to = ((float) from);
}

template<>
inline void
PXL_Convert<unsigned int,1,unsigned short,1>::convert(PXL_Pixel<unsigned int,1> &to,
					const PXL_Pixel<unsigned short,1> &from)
{
    to = ((float) from);
}

template<>
inline void
PXL_Convert<unsigned int,0,unsigned int,0>::convert(PXL_Pixel<unsigned int,0> &to,
			     const PXL_Pixel<unsigned int,0> &from)
{
    if(to.getBlack() == from.getBlack() && to.getWhite() == from.getWhite())
    {
	to.set(from.getValue());
    }
    else
    {
	// ranges do not match -- convert to int and back.
	to = ((float) from);
    }
}

template<>
inline void
PXL_Convert<unsigned int,1,unsigned int,0>::convert(PXL_Pixel<unsigned int,1> &to,
			     const PXL_Pixel<unsigned int,0> &from)
{
    if(to.getBlack() == from.getBlack() && to.getWhite() == from.getWhite())
    {
	to.set(from.getValue());
    }
    else
    {
	// ranges do not match -- convert to int and back.
	to = ((float) from);
    }
}

template<>
inline void
PXL_Convert<unsigned int,0,unsigned int,1>::convert(PXL_Pixel<unsigned int,0> &to,
			     const PXL_Pixel<unsigned int,1> &from)
{
    if(to.getBlack() == from.getBlack() && to.getWhite() == from.getWhite())
    {
	to.set(from.getValue());
    }
    else
    {
	// ranges do not match -- convert to int and back.
	to = ((float) from);
    }
}

template<>
inline void
PXL_Convert<unsigned int,1,unsigned int,1>::convert(PXL_Pixel<unsigned int,1> &to,
			     const PXL_Pixel<unsigned int,1> &from)
{
    to.set(from.getValue());
}

template<>
inline void
PXL_Convert<fpreal16,1,float,1>::convert(PXL_Pixel<fpreal16,1> &to,
					 const PXL_Pixel<float,1> &from)
{
    fpreal16 val = from.getValue();
    to.set(val);
}

template<>
inline void
PXL_Convert<fpreal16,0,float,1>::convert(PXL_Pixel<fpreal16,0> &to,
					 const PXL_Pixel<float,1> &from)
{
    fpreal16 val = from.getValue();
    to.set(val);
}

template<>
inline void
PXL_Convert<fpreal16,0,float,0>::convert(PXL_Pixel<fpreal16,0> &to,
					 const PXL_Pixel<float,0> &from)
{
    fpreal16 val = from.getValue();
    to.set(val);
}

template<>
inline void
PXL_Convert<fpreal16,1,float,0>::convert(PXL_Pixel<fpreal16,1> &to,
					 const PXL_Pixel<float,0> &from)
{
    fpreal16 val = from.getValue();
    to.set(val);
}

#endif
