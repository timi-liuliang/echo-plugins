/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Pixel.h ( UT Library, C++)
 *
 * COMMENTS:	Contains useful classes for representing pixels in a raster
 */

#ifndef __UT_Pixel__
#define __UT_Pixel__

#include "UT_API.h"
#include <SYS/SYS_Types.h>
#include <SYS/SYS_Math.h>

class UT_Color;

namespace
{
    // Inline helpers to do unit-value conversions between different
    // scalar types.

    template<typename S, typename D> 
    static inline D unitConvert(S s); // Intentionally left blank
    
    template<>
    inline uint8 unitConvert(uint16 s) { return uint8(s >> 8); }
    
    template<>
    inline uint16 unitConvert(uint8 s) { return uint16((s << 8) | s); }
    
    template<>
    inline uint8 unitConvert(fpreal32 s) 
    { 
	return (uint8)(SYSrint(SYSclamp(s, 0.f, 1.f) * SYS_UINT8_MAX)); 
    }
    
    template<>
    inline fpreal32 unitConvert(uint8 s)  
    { 
	return s / (fpreal32)SYS_UINT8_MAX; 
    }

    template<>
    inline uint16 unitConvert(fpreal32 s) 
    {
	return (uint16)(SYSrint(SYSclamp(s, 0.f, 1.f) * SYS_UINT16_MAX)); 
    }
    
    template<>
    inline fpreal32 unitConvert(uint16 s)  
    { 
	return s / (fpreal32)SYS_UINT16_MAX; 
    }
    
    template<>
    inline fpreal32 unitConvert(fpreal64 s) { return fpreal32(s); } 
}


template<typename T>
struct UT_RGBAT
{
    enum ZeroInitType { ZERO_INIT };

    UT_RGBAT() = default; // deliberately left uninitialized

    UT_RGBAT(ZeroInitType) { r = g = b = a = T(0); }
    explicit UT_RGBAT(T r, T g, T b, T a) : r(r), g(g), b(b), a(a) {}
    
    template<typename U>
    UT_RGBAT(U r, U g, U b, U a) 
	: r(unitConvert<U,T>(r)), g(unitConvert<U,T>(g)), 
	  b(unitConvert<U,T>(b)), a(unitConvert<U,T>(a)) 
    {}
    
    UT_RGBAT(const UT_RGBAT &s) = default;

    template<typename U>
    UT_RGBAT(const UT_RGBAT<U> &s)
    {
	*this = s;
    }
    
    UT_RGBAT &operator=(const UT_RGBAT &s) = default;

    template<typename U>
    UT_RGBAT<T> &operator=(const UT_RGBAT<U> &s)
    {
	r = unitConvert<U,T>(s.r);
	g = unitConvert<U,T>(s.g);
	b = unitConvert<U,T>(s.b);
	a = unitConvert<U,T>(s.a);
	return *this;
    }
    
    T 	r, g, b, a;
};

// Specializations that alias on the old type names.
typedef UT_RGBAT<uint8> UT_RGBA; 
typedef UT_RGBAT<uint16> UT_BIG_RGBA; 
typedef UT_RGBAT<fpreal32> UT_FRGBA; 



class UT_API UT_HSVA {
public:
    UT_HSVA()		{}
    UT_HSVA(unsigned short ch, unsigned short cs, char cv, char ca)
    { h = (short)ch; s = (short)cs; 
      v = (unsigned short)((cv << 8) | cv); 
      a = (unsigned short)((ca << 8) | cv); }

    UT_HSVA(unsigned short ch, unsigned short cs,
	    unsigned short cv, unsigned short ca)
    { h = (short)ch; s = (short)cs; v = cv; a = ca; }

    UT_HSVA(const UT_RGBA &rgba)     { *this = rgba; }
    UT_HSVA(const UT_BIG_RGBA &rgba) { *this = rgba; }
    UT_HSVA(const UT_HSVA &hsva)     { *this = hsva; }

    UT_HSVA	&operator=(const UT_RGBA     &rgba);
    UT_HSVA	&operator=(const UT_BIG_RGBA &rgba);
    UT_HSVA	&operator=(const UT_HSVA     &hsva);
    UT_HSVA	&operator=(const UT_Color    &c);

    operator	UT_RGBA()     const;
    operator	UT_BIG_RGBA() const;

    // Returns previous value:
    static int	enableAlphaDivide(int state);
    static int     getAlphaDivide() { return hsvDoAlphaDivide; }

    short		h, s;
    unsigned short	v, a;

private:
    static char		hsvDoAlphaDivide;
};

class UT_API UT_BUMP {
public:
    UT_BUMP() {}
    UT_BUMP(short u, short v)	{ du = u; dv = v; }

    short	du, dv;
};

class UT_API UT_BIG_BUMP {
public:
    UT_BIG_BUMP() {}
    UT_BIG_BUMP(long u, long v)	{ du = (int)u; dv = (int)v; }

    int		du, dv;
};

#endif
