/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_RenderUI.h ( RE Library, C++)
 */

#ifndef __RE_RenderUI__
#define __RE_RenderUI__

#include "RE_API.h"
#include "RE_Types.h"
#include "RE_Font.h"
#include <UT/UT_BoundingRect.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_Vector2Array.h>
#include <UT/UT_Vector3Array.h>
#include <UT/UT_StringArray.h>
#include <UT/UT_SharedPtr.h>
#include <UT/UT_UniquePtr.h>

class re_PolyConvex;
class RE_VertexArray;
class RE_ShaderHandle;
class RE_Render;

class RE_API RE_RenderUI : UT_NonCopyable
{
public:
				 RE_RenderUI();
    virtual			~RE_RenderUI();

    void			 prepare(fpreal xperpixel, fpreal yperpixel,
					fpreal xorigin, fpreal yorigin);
    virtual void		 draw(RE_Render *r);
    virtual void		 clearAllData();

    /// Get some standard fonts.
    static RE_ShaderHandle	*getUIShaderHandle(RE_Render *r);
    static void			 initUIShaderUniforms(RE_Render *r,
					RE_ShaderHandle &shader);
    static fpreal32		 getBufferedRenderZBump();

    /// Change the rendering "layer". This just modifies the depth
    /// coordinate of the primitives.
    void			 bumpLayer(int delta);

    /// Set the base depth for rendering this object. Provide a function
    /// the get the final Z to use as the base for another object.
    virtual void		 setZStart(fpreal32 zStart);
    fpreal32			 getZStart() const;
    fpreal32			 getZEnd() const;
    void			 incrementZEnd();

    /// Get x or y distance in current units that corresponds to 1 pixel.
    fpreal32			 getXPerPixel() const
				 { return myXPerPixel; }
    fpreal32			 getYPerPixel() const
				 { return myYPerPixel; }
    fpreal32			 getXOrigin() const
				 { return myXOrigin; }
    fpreal32			 getYOrigin() const
				 { return myYOrigin; }

    /// These are options that can change per-vertex.
    void			 pushColor();
    void			 pushColor(const UT_Color &c,
					fpreal32 a = 1.0f);
    void			 pushColor(fpreal32 r, fpreal32 g, fpreal32 b,
					fpreal32 a = 1.0f);
    void			 setColor(const UT_Color &c,
					fpreal32 a = 1.0f);
    void			 setColor(fpreal32 r, fpreal32 g, fpreal32 b,
					fpreal32 a = 1.0f);
    void			 setAlpha(fpreal32 a);
    void			 popColor();
    void			 getColor(UT_Color &c) const;
    fpreal32			 getAlpha() const;

    /// Point options that can change per-point.
    void			 pushPointSize(fpreal32 size);
    void			 setPointSize(fpreal32 size);
    void			 popPointSize();

    /// Line options that can change per-line.
    void			 pushLineSmoothing(bool smooth_lines);
    void			 pushLineSmoothing();
    void			 setLineSmoothing(bool smooth_lines);
    void			 popLineSmoothing();
    void			 pushLineStyle(RE_LineStyle style);
    void			 pushLineStyle();
    void			 setLineStyle(RE_LineStyle style);
    void			 popLineStyle();
    void			 pushLineWidth(fpreal32 width);
    void			 pushLineWidth();
    void			 setLineWidth(fpreal32 width);
    void			 popLineWidth();

    /// These functions mark the start or end of primitives.
    void			 beginTriangles();
    void			 endTriangles();
    void			 beginTriangleFan();
    void			 endTriangleFan();
    void			 beginTriMesh();
    void			 endTriMesh();

    void			 beginQuads();
    void			 endQuads();
    void			 beginQuadStrip();
    void			 endQuadStrip();

    void			 beginPolygon(int concave);
    void			 endPolygon();

    void			 beginLine();
    void			 beginClosedLine();
    void			 endLine();
    void			 beginLines();
    void			 endLines();

    void			 beginPoint();
    void			 endPoint();

    /// Set vertex attributes.
    void			 t2DW(const fpreal32 tx[2]);
    void			 c3DW(const fpreal32 clr[3]);
    void			 c4DW(const fpreal32 clr[4]);

    /// Create vertices for primitives.
    void			 v2DW(const fpreal32 vtx[2]);
    void			 v2DI(const int vtx[2]);
    void			 vertex2DW(fpreal32 x, fpreal32 y);
    void			 vertex2DI(int x, int y);
    void			 vertex2DS(int x, int y);

    /// Text rendering functions.
    void			 setFont(RE_Font &font);
    void			 textMoveW(fpreal32 x, fpreal32 y);
    void			 textMoveS(int x, int y);
    void			 putMultiLineString(const utf8 *str);
    void			 putString(RE_Font &font,
					   const utf8 *start, const utf8 *end,
					   UT_Unicode::transform *xform = NULL);
    void			 putString(RE_Font &font,
        			           const UT_StringView &sv,
					   UT_Unicode::transform *xform = NULL)
				 { putString(font,sv.begin(),sv.end(),xform); }
    void			 putString(const utf8 *start, const utf8 *end);
    void			 putString(const utf8 *str);
    void			 putChar(utf32 c);

    /// Unfilled rectangles.
    void			 rect2DW(fpreal32 x1, fpreal32 y1,
					fpreal32 x2, fpreal32 y2);
    void			 rect2DS(int x1, int y1,
					int x2, int y2);
    void			 box2DS(int x1, int y1,
					int x2, int y2);
    /// Filled rectangles.
    void			 rectf2DW(fpreal32 x1, fpreal32 y1,
					fpreal32 x2, fpreal32 y2);
    void			 rectf2DS(int x1, int y1,
					int x2, int y2);
    void			 boxf2DS(int x1, int y1,
					int x2, int y2);

    /// Unfilled arcs.
    void			 circleW(float x, float y, float r);
    void			 circleS(int x, int y, int r);
    void			 arcW(float x, float y, float r,
					int angle_start, int angle_end);
    void			 arcS(int x, int y, int r,
					int angle_start, int angle_end);

    /// Filled arcs.
    void			 circlefW(float x, float y, float r);
    void			 circlefS(int x, int y, int r);
    void			 arcfW(float x, float y, float r,
					int angle_start, int angle_end);
    void			 arcfS(int x, int y, int r,
					int angle_start, int angle_end);

    /// Partially filled circles (filled between inner and outer radii).
    void			 ringfW(float x, float y,
					float inner_r, float outer_r);
    void			 ringfS(int x, int y, int inner_r, int outer_r);

    /// Partially filled arcs (filled between inner and outer radii).
    void			 sectorW(float x, float y,
					float inner_r, float outer_r, 
					float angle_start, float angle_end);
    void			 sectorfW(float x, float y,
					float inner_r, float outer_r, 
					float angle_start, float angle_end);

    /// A few limited functions for handling drawing to a 3D viewport.
    void			 v3DW(const float vtx[3]);
    void			 vertex3DW(float x, float y, float z);
    void			 textMove3W(float x, float y, float z);

protected:
    /// The underlying function for adding a new vertex to our data arrays.
    virtual void		 addVertex(fpreal32 x, fpreal32 y, fpreal32 z,
					   fpreal32 u = -1.0f,
					   fpreal32 v = -1.0f);
    virtual void		 createVertexArrays(RE_Render *r, int nelems);
    virtual void		 fillVertexArrays(RE_Render *r, int nelems);
    virtual void		 bindVertexArrays(RE_Render *r);
    virtual void		 unbindVertexArrays(RE_Render *r);
    virtual bool		 applyDepthTestingToText() const;

    RE_Texture			*createPatternTexture(RE_Render *r);
    void			 pushDepthStateBeforeDraw(RE_Render *r);
    void			 popDepthStateAfterDraw(RE_Render *r);
    bool			 prepareText(RE_Render *r,
					    bool use_shared_buffers);
    void			 renderText(RE_Render *r);

    void			 resetCurrentZ();
    bool			 hasAnyData();
    void			 clearSourceData();

    /// Basic vertex properties.
    UT_Vector3Array			 myPositionData;
    UT_UniquePtr<RE_VertexArray>	 myPositionArray;
    UT_Array<UT_FRGBA>			 myColorData;
    UT_UniquePtr<RE_VertexArray>	 myColorArray;
    UT_Vector2Array			 myPatternUVData;
    UT_UniquePtr<RE_VertexArray>	 myPatternUVArray;
    UT_Vector2Array			 myImageUVData;
    UT_UniquePtr<RE_VertexArray>	 myImageUVArray;

    /// Triangle primitives.
    UT_IntArray				 myTriangleElementData;
    UT_UniquePtr<RE_VertexArray>	 myTriangleElementArray;

    /// Line primitives.
    UT_IntArray				 myLineElementData;
    UT_UniquePtr<RE_VertexArray>	 myLineElementArray;
    UT_IntArray				 myLineSmoothElementData;
    UT_UniquePtr<RE_VertexArray>	 myLineSmoothElementArray;
    UT_IntArray				 myLineWideElementData;
    UT_UniquePtr<RE_VertexArray>	 myLineWideElementArray;

    /// Point primitives.
    UT_IntArray				 myPointElementData;
    UT_UniquePtr<RE_VertexArray>	 myPointElementArray;
    UT_Fpreal32Array			 myPointSizeData;
    UT_UniquePtr<RE_VertexArray>	 myPointSizeArray;

private:
    bool			 getPrimitiveIsActive() const;

    /// Data for tracking primitives while building the data arrays.
    int					 myTriangleVertexCount;
    bool				 myTriangleBuildFan;
    bool				 myTriangleBuildStrip;
    int					 myPolygonMaxVertexCount;
    int					 myLineVertexCount;
    fpreal32				 myLinePatternPos;
    bool				 myLineClosed;
    bool				 myLineSegments;
    int					 myPointVertexCount;

    /// Additional attributes.
    UT_Array<UT_FRGBA>			 myColorStack;
    UT_Array<bool>			 myLineSmoothStack;
    UT_Array<RE_LineStyle>		 myLineStyleStack;
    UT_Array<fpreal32>			 myLineWidthStack;
    UT_Array<fpreal32>			 myPointSizeStack;

    /// Global state.
    UT_Vector2F				 myImageUV;
    fpreal32				 myZ;
    fpreal32				 myZStart;
    fpreal32				 myXPerPixel;
    fpreal32				 myYPerPixel;
    fpreal32				 myXOrigin;
    fpreal32				 myYOrigin;

    /// Text rendering data.
    RE_Font				*myTextFont;
    UT_Vector3				 myTextPosition;
    UT_StringArray			 myTextStrings;
    UT_Array<UT_Unicode::transform *>	 myTextXforms;
    UT_Array<RE_Font *>			 myTextFonts;
    UT_Vector3Array			 myTextPositions;
    UT_Array<UT_FRGBA>			 myTextColors;
    UT_Array<RE_FontDeferData>		 myTextDeferData;

    /// Polygon convexing.
    UT_UniquePtr<re_PolyConvex>		 myConvexer;
};

typedef UT_SharedPtr<RE_RenderUI> RE_RenderUIHandle;
typedef UT_Array<RE_RenderUIHandle> RE_RenderUIArray;

inline void
RE_RenderUI::t2DW(const fpreal32 tx[2])
{
    myImageUV.assign(tx[0], tx[1]);
}

inline void
RE_RenderUI::c3DW(const fpreal32 clr[3])
{
    setColor(clr[0], clr[1], clr[2], 1.0f);
}

inline void
RE_RenderUI::c4DW(const fpreal32 clr[4])
{
    setColor(clr[0], clr[1], clr[2], clr[3]);
}

inline void
RE_RenderUI::vertex2DW(fpreal32 x, fpreal32 y)
{
    fpreal32	     vtxf[2] = { x, y };
    v2DW(vtxf);
}

inline void
RE_RenderUI::vertex2DI(int x, int y)
{
    fpreal32	     vtxf[2] = { fpreal32(x+0.375f), fpreal32(y+0.375f) };
    v2DW(vtxf);
}

inline void
RE_RenderUI::vertex2DS(int x, int y)
{
    fpreal32	     vtxf[2] = { fpreal32(x+0.375f), fpreal32(y+0.375f) };
    v2DW(vtxf);
}

inline void
RE_RenderUI::v2DI(const int vtx[2])
{
    fpreal32	     vtxf[2] = { fpreal32(vtx[0]+0.375f),
				 fpreal32(vtx[1]+0.375f) };
    v2DW(vtxf);
}

#endif // __RE_RenderUI__

