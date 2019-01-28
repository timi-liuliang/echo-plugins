/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        GR_ViewportFont.h ( GR Library, C++)
 *
 * COMMENTS:
 *	Renders a fixed width bitmap font to screen positions.
 */
#ifndef GR_ViewportFont_h
#define GR_ViewportFont_h

#include "GR_API.h"
#include "GR_Defines.h"

#include <SYS/SYS_Types.h>
#include <UT/UT_Vector2.h>

class RE_Render;
class UT_String;

class GR_API GR_ViewportFont
{
public:

	     GR_ViewportFont() {}
    	     GR_ViewportFont(RE_Render *r,
			     GR_DecorFontSize size,
			     GR_DecorFontType charset);
    virtual ~GR_ViewportFont();

    /// @{
    /// The line number and text direction
    enum YPosition
    {
	BOTTOM,
	TOP,
	ELEVATED
    };
    enum XPosition
    {
	LEFT,
	RIGHT,
	CENTRE
    };
    /// @}

    /// @{
    /// Functions for drawing certain types of data to the screen.
    /// eg. fpreal, int, string, and then vectors of these types
    ///    pos is the screen position
    ///    x_offset is an initial offset from pos (useful for rendering
    //     over multiple calls to label)
    int  label      (RE_Render *r,
		     fpreal64 num,
		     const UT_Vector3F &pos,
		     const UT_Vector3F &col,
		     fpreal32 *&dst_points,
		     fpreal32 *&dst_texcoord,
		     fpreal32 *&dst_colors,
		     XPosition xpos, YPosition ypos,
		     fpreal &x_offset) const;

    int  label      (RE_Render *r,
		     int32 num,
		     const UT_Vector3F &pos,
		     const UT_Vector3F &col,
		     fpreal32 *&dst_points,
		     fpreal32 *&dst_texcoord,
		     fpreal32 *&dst_colors,
		     XPosition xpos, YPosition ypos,
		     fpreal &x_offset) const
    {
	return label (r, static_cast<int64>(num), pos, col,
		      dst_points, dst_texcoord, dst_colors,
		      xpos, ypos, x_offset);
    }

    int  label 	    (RE_Render *r,
		     int64 num,
		     const UT_Vector3F &pos,
		     const UT_Vector3F &col,
		     fpreal32 *&dst_points,
		     fpreal32 *&dst_texcoord,
		     fpreal32 *&dst_colors,
		     XPosition xpos, YPosition ypos,
		     fpreal &x_offset) const;

    int  label 	    (RE_Render *r, const UT_String &str,
		     const UT_Vector3F &pos, const UT_Vector3F &col,
		     fpreal32 *&dst_points,
		     fpreal32 *&dst_texcoord,
		     fpreal32 *&dst_colors,
		     XPosition xpos, YPosition ypos,
		     fpreal &x_offset) const;

    template <typename T>
    int  vectorLabel (RE_Render *r,
		      T vec,
		      const UT_Vector3F &pos,
		      const UT_Vector3F &col,
		      fpreal32 *&dst_points,
		      fpreal32 *&dst_texcoord,
		      fpreal32 *&dst_colors,
		      XPosition xpos, YPosition ypos,
		      fpreal &x_offset) const;
    /// @}


    /// The offset from the vertex when drawing vertex label decor
    UT_Vector2F  getVertexOffsetDist()
    {
	return 0.75 * (myWidth - myHeight) * myPixelSize;
    }

private:
    /// Draws a single glyph
    void 	glyph(fpreal32 *&dst_points,
		      fpreal32 *&dst_texcoord,
		      fpreal32 *&dst_colors,
		      const UT_Vector3F &pos,       // Position of start of line
		      const UT_Vector3F &col,       // Color of text
		      const UT_Vector2F &offset,    // Offset into line
		      fpreal w, fpreal h, 	    // Size of character
		      fpreal u, fpreal u1) const;  // Position in character map

    /// Moves x_offset one space forward
    void 	space(fpreal &x_offset, int dir) const
    {
	x_offset += dir * (myWidth * 0.66);
    }

    UT_Vector3F  normalizeIntCoord(const UT_Vector2F &offset) const;


    // These reference screen sizes in pixels
    // myPad is the default kerning.
    // myHeight and myWidth are the
    fpreal myPad, myHeight, myWidth;

    // These reference uv coords [0,1]
    fpreal myWidthTexture, myPadTexture;

    // Size of a pixel in either direction in NDC
    UT_Vector2F myPixelSize;
};


template <typename T>
int
GR_ViewportFont::vectorLabel(RE_Render *r, T vec,
			     const UT_Vector3F &pos, const UT_Vector3F &col,
			     fpreal32 *&dst_points,
			     fpreal32 *&dst_texcoord,
			     fpreal32 *&dst_colors,
			     XPosition xpos, YPosition ypos,
			     fpreal &x_offset) const
{
    T numbers;
    if (xpos == LEFT)
    {
	// We draw the last component first if we are drawing left
	for (int i=0; i<vec.tuple_size; i++)
	{
	    int j = vec.tuple_size - 1 - i;
	    numbers[j] = vec[i];
	}
    }
    else
	for (int i=0; i<vec.tuple_size; i++)
	    numbers[i] = vec[i];

    int digit_count = 0;
    for (int j=0; j<vec.tuple_size; j++)
    {
	digit_count += label(r, numbers[j], pos, col,
			     dst_points, dst_texcoord, dst_colors,
			     xpos, ypos, x_offset);
	if (j != vec.tuple_size-1)
	    space(x_offset, (xpos == LEFT) ? -1 : 1);
    }

    return digit_count;
}

#endif
