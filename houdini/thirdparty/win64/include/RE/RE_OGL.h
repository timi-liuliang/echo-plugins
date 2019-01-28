/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_OGL.h (C++)
 *
 * COMMENTS:	Cross-platform wrapper for OpenGL 
 *
 */

#ifndef __RE_OGL_h__
#define __RE_OGL_h__

#include <glcorearb.h>

// Prevent GL.h from being included.
#define __gl_h_
#define __GL_H__

#ifndef GL_POLYGON
#define GL_POLYGON 9
#endif

#include <stddef.h>

#include <SYS/SYS_Types.h>

// Direct State access
typedef void (APIENTRYP RE_PFNGLBINDMULTITEXTUREEXTPROC) (GLenum texunit, GLenum target, GLuint texture);

#ifndef GL_TEXTURE_MAX_ANISOTROPY
#define GL_TEXTURE_MAX_ANISOTROPY	  0x84FE
#endif
#ifndef GL_MAX_TEXTURE_MAX_ANISOTROPY
#define GL_MAX_TEXTURE_MAX_ANISOTROPY	  0x84FF
#endif

#ifndef GL_VBO_FREE_MEMORY_ATI
#define GL_VBO_FREE_MEMORY_ATI                     0x87FB
#endif
#ifndef GL_TEXTURE_FREE_MEMORY_ATI
#define GL_TEXTURE_FREE_MEMORY_ATI                 0x87FC
#endif
#ifndef GL_RENDERBUFFER_FREE_MEMORY_ATI
#define GL_RENDERBUFFER_FREE_MEMORY_ATI            0x87FD
#endif

#ifndef GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX
#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX	   0x9048
#endif

#ifndef GL_POINT_SPRITE
#define GL_POINT_SPRITE                        0x8861
#endif

#ifndef GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX  0x9049
#endif
#ifndef GLX_CONTEXT_ROBUST_ACCESS_BIT_ARB
#define GLX_CONTEXT_ROBUST_ACCESS_BIT_ARB		   0x00000004
#endif
#ifndef GLX_CONTEXT_RESET_NOTIFICATION_STRATEGY_ARB
#define GLX_CONTEXT_RESET_NOTIFICATION_STRATEGY_ARB	   0x8256
#endif
#ifndef GLX_NO_RESET_NOTIFICATION_ARB
#define GLX_NO_RESET_NOTIFICATION_ARB			   0x8261
#endif
#ifndef GLX_LOSE_CONTEXT_ON_RESET_ARB
#define GLX_LOSE_CONTEXT_ON_RESET_ARB                      0x8252
#endif

#ifdef WIN32
typedef const char * (APIENTRYP RE_PFNWGLGETEXTENSIONSSTRINGPROC)(void);
typedef const char * (APIENTRYP RE_PFNWGLGETEXTENSIONSSTRINGARBPROC) (HDC hdc);
typedef BOOL (APIENTRYP RE_PFNWGLSWAPINTERVALPROC) (int);
typedef BOOL (APIENTRYP RE_PFNWGLCHOOSEPIXELFORMATARBPROC)
		(HDC hdc, const int *piAttribIList,
		 const FLOAT *pfAttribFList,
		 UINT nMaxFormats,
		 int *piFormats,
		 UINT *nNumFormats);
#else
#ifndef PFNGLXSWAPINTERVALSGIPROC
typedef int (APIENTRYP RE_PFNGLXSWAPINTERVALSGIPROC)(int);
#else
#define RE_PFNGLXSWAPINTERVALSGIPROC PFNGLXSWAPINTERVALSGIPROC
#endif
#endif

// For wglChoosePixelFormat()
#if defined(WIN32) && !defined(WGL_NUMBER_PIXEL_FORMATS_ARB)
#define WGL_NUMBER_PIXEL_FORMATS_ARB			0x2000
#define WGL_DRAW_TO_WINDOW_ARB				0x2001
#define WGL_DRAW_TO_BITMAP_ARB				0x2002
#define WGL_ACCELERATION_ARB				0x2003
#define WGL_SWAP_LAYER_BUFFERS_ARB			0x2006
#define WGL_SWAP_METHOD_ARB				0x2007
#define WGL_SUPPORT_OPENGL_ARB				0x2010
#define WGL_DOUBLE_BUFFER_ARB				0x2011
#define WGL_COLOR_BITS_ARB				0x2014
#define WGL_RED_BITS_ARB				0x2015
#define WGL_RED_SHIFT_ARB				0x2016
#define WGL_GREEN_BITS_ARB				0x2017
#define WGL_GREEN_SHIFT_ARB				0x2018
#define WGL_BLUE_BITS_ARB				0x2019
#define WGL_BLUE_SHIFT_ARB			        0x201A
#define WGL_ALPHA_BITS_ARB				0x201B
#define WGL_ALPHA_SHIFT_ARB				0x201C
#define WGL_ACCUM_BITS_ARB				0x201D
#define WGL_ACCUM_RED_BITS_ARB				0x201E
#define WGL_ACCUM_GREEN_BITS_ARB			0x201F
#define WGL_ACCUM_BLUE_BITS_ARB				0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB			0x2021
#define WGL_DEPTH_BITS_ARB				0x2022
#define WGL_STENCIL_BITS_ARB				0x2023
#define WGL_FULL_ACCELERATION_ARB			0x2027
#define WGL_SWAP_EXCHANGE_ARB				0x2028
#define WGL_SAMPLE_BUFFERS_ARB				0x2041
#define WGL_SAMPLES_ARB					0x2042
#endif

#ifndef GL_SHADING_LANGUAGE_VERSION
#define GL_SHADING_LANGUAGE_VERSION       0x8B8C
#endif

#ifdef WIN32
#ifndef	WGL_CONTEXT_MAJOR_VERSION_ARB
#define	WGL_CONTEXT_MAJOR_VERSION_ARB		0x2091
#endif
#ifndef	WGL_CONTEXT_MINOR_VERSION_ARB
#define	WGL_CONTEXT_MINOR_VERSION_ARB		0x2092
#endif
#ifndef	WGL_CONTEXT_LAYER_PLANE_ARB
#define	WGL_CONTEXT_LAYER_PLANE_ARB		0x2093
#endif
#ifndef	WGL_CONTEXT_FLAGS_ARB
#define	WGL_CONTEXT_FLAGS_ARB			0x2094
#endif
#ifndef	WGL_CONTEXT_PROFILE_MASK_ARB
#define	WGL_CONTEXT_PROFILE_MASK_ARB		0x9126
#endif
#ifndef	WGL_CONTEXT_DEBUG_BIT_ARB
#define	WGL_CONTEXT_DEBUG_BIT_ARB		0x0001
#endif
#ifndef	WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB
#define	WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB	0x0002
#endif
#ifndef	WGL_CONTEXT_CORE_PROFILE_BIT_ARB
#define	WGL_CONTEXT_CORE_PROFILE_BIT_ARB	0x00000001
#endif
#ifndef	WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB
#define	WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002
#endif
#ifndef	ERROR_INVALID_VERSION_ARB
#define	ERROR_INVALID_VERSION_ARB		0x2095
#endif

typedef HGLRC (APIENTRYP RE_PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hdc,
								HGLRC share,
						       const int *attrib_list);

#endif

#endif // __RE_OGL_h__
