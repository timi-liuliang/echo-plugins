/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_Font.h (C++)
 *
 * COMMENTS:
 *
 */

#ifndef __GU_Font_h__
#define __GU_Font_h__

#include "GU_API.h"
#include <UT/UT_Vector2.h>
#include <UT/UT_Vector3.h>

class	GU_Detail;
class	UT_IStream;
class	UT_String;
class	gu_Font;

class GU_API GU_FontBuildOptions
{
public:
    enum HAlignment
    {
	HALIGN_LEFT,
	HALIGN_CENTER,
	HALIGN_RIGHT
    };

    enum VAlignment
    {
	VALIGN_FIRST_LINE,
	VALIGN_TOP,
	VALIGN_MIDDLE,
	VALIGN_BOTTOM
    };
    
public:
    GU_FontBuildOptions()
    {
	myOrigin.assign(0,0,0);
	myScale.assign(1,1);
	myTracking.assign(0,0);
	myAutoKerning = true;
	myOblique = 0;
	myHAlign = HALIGN_LEFT;
	myVAlign = VALIGN_TOP;
	myAlignDescender = false;
	myAddAttributes = true;
    }

    void setOrigin(const UT_Vector3 &origin) 	{ myOrigin = origin; }
    void setScale(const UT_Vector2 &scale) 	{ myScale = scale; }
    void setTracking(const UT_Vector2 &track)	{ myTracking = track; }
    void setAutoKerning(bool enable)		{ myAutoKerning = enable; }
    void setOblique(fpreal oblique)		{ myOblique = oblique; }
    void setHAlignment(HAlignment h_align)	{ myHAlign = h_align; }
    void setVAlignment(VAlignment v_align)	{ myVAlign = v_align; }
    void setVAlignOnDescender(bool ad)		{ myAlignDescender = ad; }
    void setAddAttributes(bool add_attribs)	{ myAddAttributes = add_attribs; }
    
private:
    friend class gu_Font;

    UT_Vector3	myOrigin;
    UT_Vector2 	myScale;
    UT_Vector2 	myTracking;
    fpreal	myOblique;
    
    HAlignment 	myHAlign;
    VAlignment 	myVAlign;
    
    bool 	myAlignDescender;
    bool	myAutoKerning;
    
    bool 	myAddAttributes;	// Add primitive attributes
};

class GU_API GU_Font
{
public:
     GU_Font();
    ~GU_Font();

    /// Loads a TrueType, OpenType or Adobe Type-1 font from the given stream
    /// and initializes with the given face index. Usually this index should be
    /// left at zero, but for font collections (.ttc / .otc) this value refers
    /// to the index of the font within the collection
    /// (see \c FONT_Info::faceIndex)
    bool	  	 loadFont(UT_IStream &is, int face_index = 0);
    void	  	 buildText(GU_Detail &gdp, const UT_String &text,
				   const GU_FontBuildOptions &opts);

    bool		 isValid() const;
    const char		*getFontName() const;

private:
    gu_Font		*myFont;
};

#endif
