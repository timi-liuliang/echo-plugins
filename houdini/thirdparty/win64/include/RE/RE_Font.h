/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_Font.h (RE Library, C++)
 *
 * COMMENTS:
 */

#ifndef __RE_Font__
#define __RE_Font__

#include "RE_API.h"
#include "RE_Texture.h"
#include "RE_VertexArray.h"

#include <FONT/FONT_Enums.h>
#include <UT/UT_Array.h>
#include <UT/UT_Pixel.h>
#include <UT/UT_Rect.h>
#include <UT/UT_UniquePtr.h>
#include <UT/UT_String.h>
#include <UT/UT_StringView.h>
#include <UT/UT_Unicode.h>
#include <SYS/SYS_Math.h>

class FONT_Info;
class re_FontCache;
class re_FontInstance;
class re_FontBuffers;
class RE_Render;
class RE_VertexArray;
class UT_WorkBuffer;

/// Specifies which method to use to split string in RE_Font::splitString.
enum RE_StringSplitType
{
    RE_SST_NARROWER_ONLY,	///< Only split at word boundaries if narrower
				///  than the given maximum width. If no split
				///  point was found before, the splitting
				///  fails.
    RE_SST_BOUNDARY_SOFT,	///< Try first to split on word boundaries at
				///  less than the maximum width, otherwise
				///  try word boundaries after.
    RE_SST_BOUNDARY_HARD,	///< Try first to split on word boundaries at
				///  less than maximum width, otherwise do a
				///  hard split at the maximum width, ignoring
				///  any boundary indicators.
};

class RE_FontBuffers
{
public:
    enum { BUFFER_CHUNK_SIZE = 4096 };
    
    RE_VertexArray *myP;
    RE_VertexArray *myUV;
    RE_VertexArray *myCd;
    int		    myArraySize;
    RE_FontBuffers *myNext;
    const int	    myBucket;

    RE_FontBuffers(int bucket)
       : myP(NULL), myUV(NULL), myCd(NULL),
         myNext(NULL), myArraySize(0),
	 myBucket(bucket) {}
    
    ~RE_FontBuffers();

    void	    init(RE_Render *r, int size);
};

class RE_API RE_FontDeferData
{
public:
    RE_FontDeferData()
	: myTexture(NULL),
	  myTextGeo(NULL),
	  myNumVertices(0),
	  myOwnFontBuffers(false)
    {
    }
    ~RE_FontDeferData()
    {
	if(myOwnFontBuffers)
	    delete myTextGeo;
	// else myTextGeo is returned to a buffer pool
    }

    RE_Texture			    *myTexture;
    RE_FontBuffers		    *myTextGeo;	       
    int				     myNumVertices;
    bool			     myOwnFontBuffers;
};

    
class RE_API RE_Font
{
public:
    static RE_Font &get(const FONT_Info &font_info, float size);
    static RE_Font &get(const char *font_name, float size);
    static RE_Font &get(RE_Font &font, FONT_Variant variant);
    static RE_Font &emptyFont();

    bool isValid() const { return myFontValid; }

    /// Returns the full name of the font.
    const char *getName() const;
    
    const FONT_Info &getFontInfo() const { return myFontInfo; }

    /// Get the size, in points, of this font.
    float getSize() const { return myFontSize; }

    bool operator==(const RE_Font &other) const;
    bool operator!=(const RE_Font &other) const 
    { 
	return !(operator==(other)); 
    }

    /// @{
    /// Font query

    /// Returns the height of the ascender, in pixels. The ascender is the
    /// distance from the font's baseline to the top of the tallest glyph.
    float getAscender();
    
    /// The descender is the distance, in pixels,  from the baseline to the
    /// bottom-most pixel of all glyphs in the font.
    float getDescender();

    /// The height is the baseline-to-baseline distance for this font. This
    /// should be used as the appropriate vertical spacing for the font.
    float getHeight();

    /// Get the x-height. This is usually the distance from the baseline to
    /// the top of the lower-case x.
    float getXHeight();

    /// Returns the signed distance from the baseline to where the the
    /// underline for the font should be drawn.
    float getUnderlinePos();

    /// Returns the thickness of the underline to be drawn, corresponding with
    /// the font's size and weight.
    float getUnderlineThickness();

    /// Returns true if the font contains a glyph for the given code point.
    bool hasGlyph(utf32 cp);

    /// Get the horizontal advance, in pixels, of the cursor, after drawing
    /// the given code point's glyph.
    /// NOTE: This ignores kerning! If you want to get the width of a whole
    /// string, use getStringWidth instead.
    float getHorizAdvance(utf32 cp);

    /// Get the horizontal advance, in pixels, of the cursor, after drawing
    /// the given code point's glyph, and including it's kerning pair glyph.
    float getHorizAdvance(utf32 cp, utf32 cp_next);

    /// Get the width of a string, including all kerning adjustments.
    float getStringWidth(const utf8 *s, const utf8 *e = NULL,
                         const UT_Unicode::transform *cp_xform = NULL);
    float getStringWidth(const UT_StringView &str,
          	         const UT_Unicode::transform *cp_xform = NULL)
    { 
	return getStringWidth(str.begin(), str.end(), cp_xform); 
    }
    
    /// Quick check to see if a string exceeds the given width. Usually faster
    /// than calling getStringWidth if only the threshold check is required.
    bool isStringWiderThan(const utf8 *s, const utf8 *e, float width, 
                           const UT_Unicode::transform *cp_xform = NULL);
    bool isStringWiderThan(const UT_StringView &str, float width, 
                           const UT_Unicode::transform *cp_xform = NULL)
    {
	return isStringWiderThan(str.begin(), str.end(), width, cp_xform);
    }
    
    /// Get the length of string that can be put in the given space, using
    /// the provided ellipsis text to replace removed characters.
    bool getTrimmedString(const utf8 *&s, const utf8 *&e, const utf8 *ellipsis,
                          float max_width, bool trim_right,
                          const UT_Unicode::transform *cp_xform = NULL,
			  float *trimmed_width = NULL);

    /// Split the string into two parts, at a breakable space or a breakable
    /// hyphen, using the font's metric to ensure that the left part doesn't 
    /// exceed \c max_width. The particular splitting strategy, and whether
    /// \c max_width is strictly honored, is governed by \c split_type. 
    /// If the splitting fails, this function returns \c false.
    bool splitString(const UT_StringView &s,
                     UT_StringView &left, UT_StringView &right, float max_width,
                     RE_StringSplitType split_type = RE_SST_NARROWER_ONLY,
                     const UT_Unicode::transform *cp_xform = NULL);
    
    /// Takes an input string, and splits it up into a list of strings none
    /// wider than \c max_width (depending on splitting strategy). The string
    /// is initially split by newline characters, and then each sub-string 
    /// split using \c splitString to satisfy the maximum width requirement.
    /// If \c max_height is given, then only as many lines as will fit in that
    /// vertical space, assuming the lines are spaced out by the amount 
    /// returned by \c getHeight.
    /// Returns \c false if it failed to fully  wrap the input string.
    bool wrapString(const UT_StringView &s, UT_StringViewArray &result,
                    float max_width, float max_height = FLT_MAX,
                    RE_StringSplitType split_type = RE_SST_BOUNDARY_HARD,
                    const UT_Unicode::transform *cp_xform = NULL);
    
    /// Performs rendering of the render data generated by createFontDeferData.
    static void renderFontDeferData(RE_Render *r,
			UT_Array<RE_FontDeferData> &deferData);

    /// Builds directly renderable data structures from the provided set
    /// of strings, fonts, positions, and colors.
    static void createFontDeferData(RE_Render *r,
			const UT_StringArray &strings,
			const UT_Array<RE_Font *> &fonts,
			const UT_Array<UT_Unicode::transform *> &xforms,
			const UT_Array<UT_FRGBA> &clrs,
			const UT_Array<UT_Vector3F> &positions,
			fpreal32 sx, fpreal32 sy,
			fpreal32 tx, fpreal32 ty,
			UT_Array<RE_FontDeferData> &defer_data,
			bool use_shared_font_buffers);

    /// Faster String width method for integers.
    float getIntegerWidth( const char* s );

    /// Faster String width method for floats.
    float getFloatWidth( const char* s );

private:
    RE_Font(const FONT_Info &font_info, float size);
    RE_Font(RE_Font &ref, float scale);
    RE_Font();

    RE_Font &fallbackFontForCodepoint(utf32 cp);
    
    bool ensureFont();

    const FONT_Info	&myFontInfo;
    re_FontInstance	*myInstance;

    /// The size, in points, of this font.
    float		 myFontSize;

    /// When we are a reference to another font, this value stores the
    /// amount by which we want to scale the glyphs in that referenced font.
    float		 myFontScale;

    bool		 myFontValid;

    friend class re_FontCache;
    friend class RE_RenderUI;
};
#endif
