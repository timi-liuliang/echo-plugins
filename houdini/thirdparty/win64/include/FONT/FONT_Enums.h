/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	FONT_Enums.h ( FONT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __FONT_Enums__
#define __FONT_Enums__

#include "FONT_API.h"

/// An enum used with FONT_Registry::getVariant to pick a specific font 
/// variation given a base font. For oblique variations, this can either mean
/// oblique or italic fonts, although oblique is preferred if it exists.
enum FONT_Variant
{
    FONT_VARIANT_NORMAL,    	///< Returns a normal weight, non-oblique variation. 
    FONT_VARIANT_OBLIQUE,	///< Returns a normal weight, oblique variation.
    FONT_VARIANT_BOLD,	    	///< Returns a bold weight, non-oblique variation.
    FONT_VARIANT_BOLD_OBLIQUE   ///< Returns a bold weight, oblique variation.
};


/// The font's style describes its visual appearance.
enum FONT_Style
{
    FONT_STYLE_NORMAL	    = 0,	  ///< No extra visual styles apply
    FONT_STYLE_ITALIC	    = 1 << 0, ///< Font is italic (calligraphic)
    FONT_STYLE_OBLIQUE	    = 1 << 1, ///< Font is slanted
    FONT_STYLE_MONOSPACED   = 1 << 2, ///< Font is mono-spaced
    FONT_STYLE_DUALSPACED   = 1 << 3, ///< Font is dual-spaced (e.g. CJK)
    FONT_STYLE_SYMBOL	    = 1 << 4  ///< Font is a symbol font (e.g. wingdings).
};

static inline FONT_Style
operator|(FONT_Style s1, FONT_Style s2)
{
    return FONT_Style((int)s1 | (int)s2);
}
static inline FONT_Style
operator&(FONT_Style s1, FONT_Style s2)
{
    return FONT_Style((int)s1 & (int)s2);
}
enum FONT_Weight 
{
    FONT_WEIGHT_THIN		= 100,
    FONT_WEIGHT_EXTRALIGHT	= 200,
    FONT_WEIGHT_LIGHT		= 300,
    FONT_WEIGHT_BOOK		= 380,
    FONT_WEIGHT_REGULAR		= 400,
    FONT_WEIGHT_MEDIUM		= 500,
    FONT_WEIGHT_DEMIBOLD	= 600,
    FONT_WEIGHT_BOLD		= 700,
    FONT_WEIGHT_EXTRABOLD	= 800,
    FONT_WEIGHT_BLACK		= 900,
};

// For UT::ArraySet
namespace UT {

template<typename T>
struct DefaultClearer;

template<>
struct DefaultClearer<FONT_Style>
{
    static constexpr FONT_Style SENTINEL = FONT_Style(0x80000000);
    static void clear(FONT_Style &v) { v = SENTINEL; }
    static bool isClear(FONT_Style v) { return v == SENTINEL; }
    static void clearConstruct(FONT_Style *p) { clear(*p); }

    static const bool clearNeedsDestruction = false;
};

template<>
struct DefaultClearer<FONT_Weight>
{
    static constexpr FONT_Weight SENTINEL = FONT_Weight(0);
    static void clear(FONT_Weight &v) { v = SENTINEL; }
    static bool isClear(FONT_Weight v) { return v == SENTINEL; }
    static void clearConstruct(FONT_Weight *p) { clear(*p); }

    static const bool clearNeedsDestruction = false;
};

} // namespace UT

#endif // __FONT_Enums__
