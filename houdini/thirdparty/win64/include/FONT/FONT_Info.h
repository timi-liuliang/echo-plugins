/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	FONT_Info.h ( UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __FONT_Info__
#define __FONT_Info__

#include "FONT_API.h"

#include "FONT_Enums.h"

#include <UT/UT_Array.h>
#include <UT/UT_StringHolder.h>
#include <iosfwd>

class UT_WorkBuffer;
struct FT_LibraryRec_;
typedef struct FT_LibraryRec_  *FT_Library;

class FONT_Info
{
public:
    inline bool		 isValid() const { return myIsValid; }

    /// The typeface (font family) of the font. E.g. 'Helvetica'
    inline const char	*typeface() const { return myTypeface; }

    /// The full name of the font, including style and weight.
    /// E.g. 'Helvetica Bold Oblique'
    inline const char	*name() const { return myName; }

    /// A bitmap of the styles of the face. See FONT_Style for possible
    /// bits.
    FONT_Style	 	 style() const { return myStyle; }

    /// Returns \c true if the font has the specific style flag set.
    bool		 hasStyle(FONT_Style style) const 
			 { return (myStyle & style) == style; } 
	    
    /// Returns true if the font is italic or oblique.
    bool		 isSlanted() const 
			 { return hasStyle(FONT_STYLE_ITALIC) ||
				  hasStyle(FONT_STYLE_OBLIQUE); }

    /// Returns true if the font is bolded in some way.
    bool		 isBolded() const
			 { return int(myWeight) >= int(FONT_WEIGHT_DEMIBOLD); } 
    
    /// The weight of the font.
    FONT_Weight		 weight() const { return myWeight; }

    /// Returns the filename of the font. Can be either a TrueType,
    /// OpenType, or Adobe Type-1 font.
    inline const char	*path() const { return myPath; }

    /// Returns the index of the face in the font file. This is used for
    /// TrueType and OpenType font collection files to select the correct face
    /// from within the file.
    inline int		 faceIndex() const { return myFaceIndex; }
    
    /// Returns the font style name given an FONT_Style bitmap value.
    static bool 	 styleName(FONT_Style style, UT_WorkBuffer &name);
    
    /// Attempts to parse a font's style name into the style bit field. 
    static bool		 parseStyleName(const char *name, FONT_Style &style);

    /// Returns the a weight name closest to the weight value given. The
    /// value is clamped. See FONT_Weight for the known, named values.
    static const char 	*weightName(FONT_Weight weight);

    /// Attempts to parse a font's weight name into the style bit field.
    static bool		 parseWeightName(const char *name, FONT_Weight &weight);

    /// Less-than operator for comparison and sorting.
    inline bool operator<(const FONT_Info &other) const
    {
	return myName < other.myName;
    }
private:
    friend class FONT_Registry;
    FONT_Info()
	: myStyle(FONT_STYLE_NORMAL), myWeight(FONT_WEIGHT_REGULAR),
	  myFaceIndex(-1), myIsValid(false)
    {}
    FONT_Info(
	const UT_StringHolder &typeface, const UT_StringHolder &name,
	FONT_Style style, FONT_Weight weight,
	const UT_StringHolder &path, int face_index = 0)
	: myTypeface(typeface), myName(name),
	  myStyle(style), myWeight(weight),
	  myPath(path), myFaceIndex(face_index)
    { /**/ }

    UT_StringHolder	myTypeface;
    UT_StringHolder	myName;
    FONT_Style		myStyle;
    FONT_Weight		myWeight;
    bool		myIsValid;

    UT_StringHolder	myPath;
    int			myFaceIndex;
};

FONT_API std::ostream &
operator<<(std::ostream &os, const FONT_Info &fi);


#endif // __FONT_Info__
