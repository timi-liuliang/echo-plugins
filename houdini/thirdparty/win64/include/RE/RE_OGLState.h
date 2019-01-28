/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_OGLState.h ( UI Library, C++)
 *
 * COMMENTS:
 *	The stored GL state for RE_OGLRender.
 *
 */
#ifndef RE_OGLState_h
#define RE_OGLState_h

#include "RE_API.h"
#include "RE_Types.h"
#include "RE_TextureTypes.h"
#include "RE_Uniform.h"

#include <UT/UT_Rect.h>
#include <UT/UT_Array.h>
#include <UT/UT_BitArray.h>
#include <UT/UT_IntArray.h>

class RE_OGLFramebuffer;
class RE_OGLTexture;
class RE_Render;
class RE_Shader;

class RE_API RE_OGLState
{
public:
		 RE_OGLState();
		~RE_OGLState();

    // clear all cached information without altering actual GL state
    void	 invalidate();

// State data
    int			_colorBufferWriting;

    // 0 = disabled, 1 = enabled, 3 = state is unknown
    unsigned
	myFillOffsetState : 2,
	myLineOffsetState : 2,
	myLineSmoothingState : 2,
	myPointOffsetState : 2,
	mySampleMaskState : 2,
	myScissorState : 2,
	myStencilState : 2,
	mySeamlessMapState : 2;

    float	 wPosX, wPosY, wPosZ;
    float	 rx, ry, rz;

    UT_DimRect	 scissorState;
    UT_DimRect	 viewportState;

    int		 viewportStateValid;
    int		 scissorStateValid;

    float	 myXZoom, myYZoom;

    float	 zNear, zFar;
    float	 _offset_variable;
    float	 _offset_constant;

    UT_Int32Array mySampleMask;
    UT_BitArray   mySampleMaskValid;

    unsigned	 myColorMask;

    int			 _s_clear_value;
    int			 _s_write_mask;
    RE_SFunction	 _s_function;
    int			 _s_function_ref;
    int			 _s_function_mask;
    RE_SOperation	 _s_operation_sfail;
    RE_SOperation	 _s_operation_dpfail;
    RE_SOperation	 _s_operation_dppass;
    
    // Stacks
    RE_OGLFramebuffer  *myDrawFBOStack[RE_FRAMEBUFFER_STACK_SIZE];
    int			myDrawFBOStackIndex;
    RE_OGLFramebuffer  *myReadFBOStack[RE_FRAMEBUFFER_STACK_SIZE];
    int			myReadFBOStackIndex;
    
    float	   lineWidthStack[PATTERN_STACK_SIZE];
    int		   lineWidthIndex;

    struct
    {
	float pointSize;
	bool  programPointSize;
    } pointSizeStack[PATTERN_STACK_SIZE];
    int		   pointSizeIndex;

    struct
    {
	unsigned char read:4,
		     write:4;
    } myAlignmentStack[RE_ALIGN_STACK_SIZE];
    int		   myAlignmentStackIndex;
	
    // These are the state caches for our mulitple texture units:
    struct re_TexStackItem
    {
	int	 myActiveUnit;
	int	 myRestoreUnit;
	uint8	 myMinTexAdjust;
	uint8	 myMaxTexAdjust;
	
	const RE_OGLTexture *myUnits[RE_MAX_TEXTURES];
	
    } myTexStack[RE_MAX_TEXTURE_STATE_STACK];

    int			myTexStackIndex;
    
    // smooth lines stack
    int		        myBlendSmoothLevel;
    re_BlendSmoothState myBlendSmoothStack[RE_SMOOTH_STACK_SIZE];

    // stenciling stack
    int			myStencilLevel;
    re_StencilState	myStencilStack[RE_STENCIL_STACK_SIZE];

    // shader stack
    int			myShaderLevel;
    RE_Shader	       *myShaderStack[RE_SHADER_STACK_SIZE];

    // depth stack
    int			myDepthLevel;
    re_DepthState	myDepthStack[RE_DEPTH_STACK_SIZE];

    // backface/winding stack
    UT_IntArray		myBackfaceWinding;
    
    unsigned		myShaderStackValid : 1,
			myFBOStackValid : 1;

    UT_IntArray		myBoundUniformBlocks;
};

#endif
