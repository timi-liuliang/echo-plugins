/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_RenderFlush.h ( RE Library, C++)
 */

#ifndef __RE_RenderFlush__
#define __RE_RenderFlush__

#include "RE_API.h"
#include "RE_ShaderHandle.h"
#include "RE_Uniform.h"
#include "RE_RenderUI.h"
#include <UT/UT_BoundingRect.h>
#include <UT/UT_Rect.h>

class RE_Light;
class RE_PerContextData;

class RE_API RE_RenderFlush : public RE_RenderUI
{
public:
				 RE_RenderFlush();
    virtual			~RE_RenderFlush();

    /// Flag this object as being used to render into a 3D viewport.
    void			 setViewportRenderer();
    static RE_Font		&getViewportFont();

    /// Holds a pointer to per-context data we use to provide OGL buffers.
    void			 setContextData(RE_PerContextData *data);
    RE_PerContextData		*getContextData() { return myContextData; }

    // Override for setting the base Z value.
    virtual void setZStart(fpreal32 zStart);
    // Override to reuse OGL Buffers.
    virtual void draw(RE_Render *r);
    // Flush render commands built up to this point.
    bool	 flushRender(bool render_complete = false);

    // TRANSFORMATION ------------------------------------------------------
    void	 pushViewport();
    void	 popViewport();

    // pushes both the viewport and the matrix. 
    void	 pushState(int viewing);
    void	 popState();

    // Called by RE_OGLRender when our viewport changes.
    void	 handleViewportUpdate(const UT_DimRect &rect);
    void	 handleScissorRectUpdate(const UT_DimRect &rect);
    void	 handleScissorEnableUpdate(bool enable);

    // Clipping plane support.
    void	 clipPlaneDisable(int which);
    void	 clipPlaneEnable(int which, const UT_Vector4D &plane);

    UT_Vector4D	 planeToEyeCoords(const UT_Vector4D &plane);

    // NB: The following methods to push/pop the clip plane assume that they
    //     occur within a clipPlaneEnable()/clipPlaneDisable() block, hence
    //     the "replace" prefix.
    void	 replacePushClipPlane(int which, const UT_Vector4D &plane,
				      bool xform_to_eye_coords,
				      RE_UniformStackEntry &stack_entry);
    void	 replacePopClipPlane(int which,
				     const RE_UniformStackEntry &stack_entry);

    // Matrix operations.
    void	 pushMatrix(bool all_matrices = true,
			RE_MatrixMode mmode = RE_MATRIX_VIEWING);
    void	 popMatrix(bool all_matrices = true,
			RE_MatrixMode mmode = RE_MATRIX_VIEWING);
    
    void	 loadIdentityMatrix(RE_MatrixMode mmode = RE_MATRIX_VIEWING);
    void	 loadMatrix(const UT_Matrix4 &m,
			RE_MatrixMode mmode = RE_MATRIX_VIEWING);
    void	 loadMatrix(const UT_DMatrix4 &m,
			RE_MatrixMode mmode = RE_MATRIX_VIEWING);
    
    void	 getMatrix(UT_Matrix4 &m,
			RE_MatrixMode mmode = RE_MATRIX_VIEWING);
    void	 getMatrix(UT_DMatrix4 &m,
			RE_MatrixMode mmode = RE_MATRIX_VIEWING);
    
    void	 multiplyMatrix(const UT_Matrix4 &m);
    void	 multiplyMatrix(const UT_DMatrix4 &m);

    void	 translate(float x, float y, float z = 0.0);
    void	 scale(float x = 1.0, float y = 1.0, float z = 1.0);
    void	 rotate(float angle, char axis);
    
    void	 ortho2DW(float l, float r, float b, float t);
    void	 orthoFromViewport2DI(const UT_DimRect &rect);
    void	 ortho3DW(float l, float r, float b, float t,
			float n, float f);
    void	 window3DW(float l, float r, float b, float t,
			float n, float f);

    // Generates a matrix equivalent to glOrtho.
    static UT_DMatrix4	 getOrthoMatrix(float l, float r,
				float b, float t,
				float n, float f);
    // Generates a matrix equivalent to glFrustum.
    static UT_DMatrix4	 getPerspectiveMatrix(float l, float r,
				float b, float t,
				float n, float f);
    // Generates a matrix equivalent to gluLookAt.
    static UT_DMatrix4	 getLookatMatrix(const UT_Vector3F &eye,
				const UT_Vector3F &target,
				const UT_Vector3F &up);

    // Maps screen space coordinates to a world space position and a ray
    // into the scene. Lets you know where in the scene the mouse is.
    static bool	 mapWorld(fpreal xs, fpreal ys,
			const UT_DimRect &viewport,
			const UT_Matrix4D &proj, 
			const UT_Matrix4D &view,
			fpreal *x1, fpreal *y1, fpreal *z1,
			fpreal *x2, fpreal *y2, fpreal *z2);
    // Maps world space coordinates into a screen space position. This is the
    // pixel location of an object in the scene.
    static bool	 mapScreen(fpreal xw, fpreal yw, fpreal zw, 
			const UT_DimRect &viewport,
			const UT_Matrix4D &proj, 
			const UT_Matrix4D &view,
			fpreal *xs, fpreal *ys);
 
    void	 getProjectiveTextureTransform(const RE_Light *light,
					       bool bias, UT_Matrix4 &mat);

protected:
    void	 setRender(RE_Render *r);

private:
    void	 flush(bool render_complete);
    void	 updatePixelRatios();

    RE_Render				*myRender;
    UT_IntArray				 myStateStack;
    UT_Array<UT_DimRect>		 myViewportStack;
    UT_Array<UT_DimRect>		 myScissorRectStack;
    UT_Array<bool>			 myScissorEnableStack;
    UT_Array<UT_BoundingRect>		 myOrthoStack;
    RE_Uniform				 myClipPlane0Uniform;
    RE_PerContextData			*myContextData;

    /// Flag to prevent recursive flushing of render data.
    bool				 myFlushing;

    /// Keeps track of whether this object is being used for dawing
    /// into a viewport or drawing regular UI.
    bool				 myViewportRenderer;

    /// Cache our two available fonts.
    static RE_Font			*theViewportFont;
    static RE_Font			*theViewportSymbolFont;

};

inline bool
RE_RenderFlush::flushRender(bool render_complete)
{
    // Ignore nested flush calls.
    if (myFlushing)
	return false;
    
    // If this is the last flush of a render pass, we can reset our Z value
    // because the next thing we draw is a whole new render.
    if (render_complete)
	resetCurrentZ();

    // If we have nothing to do, bail out immediately.
    if (!hasAnyData())
	return false;

    flush(render_complete);

    return true;
}

#endif // __RE_RenderFlush__

