/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_OGLExt.h (C++)
 *
 * COMMENTS:	Cross-platform wrapper for OpenGL extensions
 *
 */

#ifndef __RE_OGLExt_h__
#define __RE_OGLExt_h__

#include <UT/UT_Assert.h>
#include "RE_API.h"
#include "RE_OGL.h"
#include "RE_Extension.h"

class UT_StringSet;
class UT_String;
class re_ExtData;

class RE_API RE_OGLExt
{
public:
     RE_OGLExt();
    ~RE_OGLExt();

    // NB: You must have a valid OpenGL context set before calling initialize()
    //     initialize() must be called at least once before using anything else
    void	initialize();


    bool	hasExtension( RE_Extension ext )
		{
		    UT_ASSERT( theInitializedOnce && myExtensions );
		    return (myExtensions[ext]);
		}

    // returns true if the 
    bool	hasExtensionName( const char *ext_name );

    // returns true if Houdini uses this GL extension in some way.
    bool	usesExtension( const char *glextname);

    // Returns the GL extension(s) that make up Houdini extension 'hou_ext'.
    int		getNumGLExtensionsRequired(RE_Extension ext);
    void	getGLExtensionRequired(RE_Extension hou_ext, int index,
				       UT_String &glext);

    // Returns the core OpenGL version that this extension was rolled into.
    // If it is still not part of the core, false is returned.
    bool	getCoreVersion(RE_Extension ext,
			       int &glmajor, int &glminor) const;
    			       
    //
    // These function pointer names should be copied out of glext.h and into
    // RE_OGL.h, prefixed with an 'RE_'. Use RE_ versions  in this file.
    // Note that some GL types have been typedef-ed to RE_<type>, such as
    // RE_GLchar and RE_GLintptrARB - see the top of the RE_OGL.h header.
    // All GL_* defines for the extension must be copied into RE_OGL.h.
    //

    // RE_EXT_BLEND_COLOR
    PFNGLBLENDCOLORPROC		    glBlendColor;

    // RE_EXT_BLEND_MINMAX
    PFNGLBLENDEQUATIONPROC	    glBlendEquation;

    // RE_EXT_BLEND_SEPARATE
    PFNGLBLENDFUNCSEPARATEPROC	    glBlendFuncSeparate;
	
    // RE_EXT_TEXTURE_3D 
    PFNGLTEXIMAGE3DPROC		    glTexImage3D;
    PFNGLTEXSUBIMAGE3DPROC	    glTexSubImage3D;
    PFNGLCOPYTEXSUBIMAGE3DPROC	    glCopyTexSubImage3D;

    // RE_EXT_MULTI_TEXTURE
    PFNGLACTIVETEXTUREPROC	    glActiveTexture;
    
    // RE_EXT_SHADERS
    PFNGLATTACHSHADERPROC	    glAttachShader;
    PFNGLCREATEPROGRAMPROC	    glCreateProgram;
    PFNGLCREATESHADERPROC	    glCreateShader;
    PFNGLDELETEPROGRAMPROC	    glDeleteProgram;
    PFNGLDELETESHADERPROC	    glDeleteShader;
    PFNGLDETACHSHADERPROC	    glDetachShader;
    PFNGLUSEPROGRAMPROC		    glUseProgram;
    PFNGLGETPROGRAMINFOLOGPROC	    glGetProgramInfoLog;
    PFNGLGETSHADERINFOLOGPROC	    glGetShaderInfoLog;
    PFNGLISPROGRAMPROC 		    glIsProgram;
    PFNGLSHADERSOURCEPROC	    glShaderSource;
    PFNGLCOMPILESHADERPROC	    glCompileShader;
    PFNGLLINKPROGRAMPROC	    glLinkProgram;
    PFNGLVERTEXATTRIB4FVPROC	    glVertexAttrib4fv;
    PFNGLVERTEXATTRIB3FVPROC	    glVertexAttrib3fv;
    PFNGLVERTEXATTRIB2FVPROC	    glVertexAttrib2fv;
    PFNGLVERTEXATTRIB1FVPROC	    glVertexAttrib1fv;
    PFNGLVERTEXATTRIB4DVPROC	    glVertexAttrib4dv;
    PFNGLVERTEXATTRIB3DVPROC	    glVertexAttrib3dv;
    PFNGLVERTEXATTRIB2DVPROC	    glVertexAttrib2dv;
    PFNGLVERTEXATTRIB1DVPROC	    glVertexAttrib1dv;
    PFNGLVERTEXATTRIBL4DVPROC	    glVertexAttribL4dv;
    PFNGLVERTEXATTRIBL3DVPROC	    glVertexAttribL3dv;
    PFNGLVERTEXATTRIBL2DVPROC	    glVertexAttribL2dv;
    PFNGLVERTEXATTRIBL1DVPROC	    glVertexAttribL1dv;
    PFNGLVERTEXATTRIBI4IVPROC	    glVertexAttribI4iv;
    PFNGLVERTEXATTRIBI3IVPROC	    glVertexAttribI3iv;
    PFNGLVERTEXATTRIBI2IVPROC	    glVertexAttribI2iv;
    PFNGLVERTEXATTRIBI1IVPROC	    glVertexAttribI1iv;
    PFNGLVERTEXATTRIBI4UIVPROC	    glVertexAttribI4uiv;
    PFNGLVERTEXATTRIBI3UIVPROC	    glVertexAttribI3uiv;
    PFNGLVERTEXATTRIBI2UIVPROC	    glVertexAttribI2uiv;
    PFNGLVERTEXATTRIBI1UIVPROC	    glVertexAttribI1uiv;
    PFNGLGETVERTEXATTRIBDVPROC	    glGetVertexAttribdv;
    PFNGLGETVERTEXATTRIBFVPROC	    glGetVertexAttribfv;
    PFNGLGETVERTEXATTRIBIVPROC	    glGetVertexAttribiv;
    PFNGLUNIFORM1FVPROC		    glUniform1fv;
    PFNGLUNIFORM2FVPROC		    glUniform2fv;
    PFNGLUNIFORM3FVPROC		    glUniform3fv;
    PFNGLUNIFORM4FVPROC		    glUniform4fv;
    PFNGLUNIFORM1DVPROC		    glUniform1dv;
    PFNGLUNIFORM2DVPROC		    glUniform2dv;
    PFNGLUNIFORM3DVPROC		    glUniform3dv;
    PFNGLUNIFORM4DVPROC		    glUniform4dv;
    PFNGLUNIFORM1FPROC		    glUniform1f;
    PFNGLUNIFORM2FPROC		    glUniform2f;
    PFNGLUNIFORM3FPROC		    glUniform3f;
    PFNGLUNIFORM4FPROC		    glUniform4f;
    PFNGLUNIFORM1IVPROC		    glUniform1iv;
    PFNGLUNIFORM2IVPROC		    glUniform2iv;
    PFNGLUNIFORM3IVPROC		    glUniform3iv;
    PFNGLUNIFORM4IVPROC		    glUniform4iv;
    PFNGLUNIFORM1UIVPROC	    glUniform1uiv;
    PFNGLUNIFORM2UIVPROC	    glUniform2uiv;
    PFNGLUNIFORM3UIVPROC	    glUniform3uiv;
    PFNGLUNIFORM4UIVPROC	    glUniform4uiv;
    PFNGLGETUNIFORMLOCATIONPROC	    glGetUniformLocation;
    PFNGLGETUNIFORMFVPROC	    glGetUniformfv;
    PFNGLGETUNIFORMIVPROC	    glGetUniformiv;
    PFNGLGETUNIFORMDVPROC	    glGetUniformdv;
    PFNGLUNIFORMMATRIX2FVPROC	    glUniformMatrix2fv;
    PFNGLUNIFORMMATRIX3FVPROC	    glUniformMatrix3fv;
    PFNGLUNIFORMMATRIX4FVPROC	    glUniformMatrix4fv;
    PFNGLUNIFORMMATRIX2DVPROC	    glUniformMatrix2dv;
    PFNGLUNIFORMMATRIX3DVPROC	    glUniformMatrix3dv;
    PFNGLUNIFORMMATRIX4DVPROC	    glUniformMatrix4dv;
    PFNGLGETACTIVEATTRIBPROC	    glGetActiveAttrib;
    PFNGLGETACTIVEUNIFORMPROC	    glGetActiveUniform;
    PFNGLGETATTRIBLOCATIONPROC	        glGetAttribLocation;
    PFNGLBINDATTRIBLOCATIONPROC		glBindAttribLocation;
    PFNGLGETSHADERIVPROC		glGetShaderiv;
    PFNGLGETPROGRAMIVPROC		glGetProgramiv;
    PFNGLENABLEVERTEXATTRIBARRAYPROC    glEnableVertexAttribArray;
    PFNGLDISABLEVERTEXATTRIBARRAYPROC   glDisableVertexAttribArray;
    PFNGLVERTEXATTRIBPOINTERPROC	glVertexAttribPointer;
    PFNGLVERTEXATTRIBIPOINTERPROC	glVertexAttribIPointer;
    PFNGLVERTEXATTRIBLPOINTERPROC	glVertexAttribLPointer;
    PFNGLGETSHADERSOURCEPROC		glGetShaderSource;
    PFNGLVALIDATEPROGRAMPROC		glValidateProgram;
    
    // 3.0 core shaders
    PFNGLBINDFRAGDATALOCATIONPROC	glBindFragDataLocation;
    PFNGLGETFRAGDATALOCATIONPROC	glGetFragDataLocation;
	
    // RE_EXT_GEOMETRY_SHADER
    PFNGLPROGRAMPARAMETERIPROC		glProgramParameteri;

    // RE_EXT_TESS_SHADER
    PFNGLPATCHPARAMETERIPROC		glPatchParameteri;
    PFNGLPATCHPARAMETERFVPROC		glPatchParameterfv;

    // RE_EXT_SEPARATE_SHADERS
    PFNGLUSEPROGRAMSTAGESPROC		glUseProgramStages;
    PFNGLACTIVESHADERPROGRAMPROC	glActiveShaderProgram;
    PFNGLCREATESHADERPROGRAMVPROC	glCreateShaderProgramv;	
    PFNGLBINDPROGRAMPIPELINEPROC	glBindProgramPipeline;
    PFNGLDELETEPROGRAMPIPELINESPROC	glDeleteProgramPipelines;
    PFNGLGENPROGRAMPIPELINESPROC	glGenProgramPipelines;
    PFNGLISPROGRAMPIPELINEPROC		glIsProgramPipeline;
    PFNGLGETPROGRAMPIPELINEIVPROC	glGetProgramPipelineiv;
    PFNGLPROGRAMUNIFORM1IPROC		glProgramUniform1i;
    PFNGLPROGRAMUNIFORM1IVPROC		glProgramUniform1iv;
    PFNGLPROGRAMUNIFORM1FPROC		glProgramUniform1f;
    PFNGLPROGRAMUNIFORM1FVPROC		glProgramUniform1fv;
    PFNGLPROGRAMUNIFORM1DPROC		glProgramUniform1d;
    PFNGLPROGRAMUNIFORM1DVPROC		glProgramUniform1dv;
    PFNGLPROGRAMUNIFORM1UIPROC		glProgramUniform1ui;
    PFNGLPROGRAMUNIFORM1UIVPROC		glProgramUniform1uiv;
    PFNGLPROGRAMUNIFORM2IPROC		glProgramUniform2i;
    PFNGLPROGRAMUNIFORM2IVPROC		glProgramUniform2iv;
    PFNGLPROGRAMUNIFORM2FPROC		glProgramUniform2f;
    PFNGLPROGRAMUNIFORM2FVPROC		glProgramUniform2fv;
    PFNGLPROGRAMUNIFORM2DPROC		glProgramUniform2d;
    PFNGLPROGRAMUNIFORM2DVPROC		glProgramUniform2dv;
    PFNGLPROGRAMUNIFORM2UIPROC		glProgramUniform2ui;
    PFNGLPROGRAMUNIFORM2UIVPROC		glProgramUniform2uiv;
    PFNGLPROGRAMUNIFORM3IPROC		glProgramUniform3i;
    PFNGLPROGRAMUNIFORM3IVPROC		glProgramUniform3iv;
    PFNGLPROGRAMUNIFORM3FPROC		glProgramUniform3f;
    PFNGLPROGRAMUNIFORM3FVPROC		glProgramUniform3fv;
    PFNGLPROGRAMUNIFORM3DPROC		glProgramUniform3d;
    PFNGLPROGRAMUNIFORM3DVPROC		glProgramUniform3dv;
    PFNGLPROGRAMUNIFORM3UIPROC		glProgramUniform3ui;
    PFNGLPROGRAMUNIFORM3UIVPROC		glProgramUniform3uiv;
    PFNGLPROGRAMUNIFORM4IPROC		glProgramUniform4i;
    PFNGLPROGRAMUNIFORM4IVPROC		glProgramUniform4iv;
    PFNGLPROGRAMUNIFORM4FPROC		glProgramUniform4f;
    PFNGLPROGRAMUNIFORM4FVPROC		glProgramUniform4fv;
    PFNGLPROGRAMUNIFORM4DPROC		glProgramUniform4d;
    PFNGLPROGRAMUNIFORM4DVPROC		glProgramUniform4dv;
    PFNGLPROGRAMUNIFORM4UIPROC		glProgramUniform4ui;
    PFNGLPROGRAMUNIFORM4UIVPROC		glProgramUniform4uiv;
    PFNGLPROGRAMUNIFORMMATRIX2FVPROC	glProgramUniformMatrix2fv;
    PFNGLPROGRAMUNIFORMMATRIX3FVPROC	glProgramUniformMatrix3fv;
    PFNGLPROGRAMUNIFORMMATRIX4FVPROC	glProgramUniformMatrix4fv;	
    PFNGLPROGRAMUNIFORMMATRIX2DVPROC	glProgramUniformMatrix2dv;
    PFNGLPROGRAMUNIFORMMATRIX3DVPROC	glProgramUniformMatrix3dv;
    PFNGLPROGRAMUNIFORMMATRIX4DVPROC	glProgramUniformMatrix4dv;	
    PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC	glProgramUniformMatrix2x3fv;
    PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC	glProgramUniformMatrix3x2fv;
    PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC	glProgramUniformMatrix2x4fv;
    PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC	glProgramUniformMatrix4x2fv;
    PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC	glProgramUniformMatrix3x4fv;
    PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC	glProgramUniformMatrix4x3fv;
    PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC	glProgramUniformMatrix2x3dv;
    PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC	glProgramUniformMatrix3x2dv;
    PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC	glProgramUniformMatrix2x4dv;
    PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC	glProgramUniformMatrix4x2dv;
    PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC	glProgramUniformMatrix3x4dv;
    PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC	glProgramUniformMatrix4x3dv;
    PFNGLVALIDATEPROGRAMPIPELINEPROC	glValidateProgramPipeline;
    PFNGLGETPROGRAMPIPELINEINFOLOGPROC	glGetProgramPipelineInfoLog;

    // RE_EXT_PROGRAM_INTERFACE_QUERY
    PFNGLGETPROGRAMINTERFACEIVPROC	glGetProgramInterfaceiv;
    PFNGLGETPROGRAMRESOURCEINDEXPROC	glGetProgramResourceIndex;
    PFNGLGETPROGRAMRESOURCENAMEPROC	glGetProgramResourceName;
    PFNGLGETPROGRAMRESOURCEIVPROC	glGetProgramResourceiv;
    PFNGLGETPROGRAMRESOURCELOCATIONPROC	glGetProgramResourceLocation;
    PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC glGetProgramResourceLocationIndex;
    // RE_EXT_TRANSFORM_FEEDBACK
    PFNGLBEGINTRANSFORMFEEDBACKPROC	 glBeginTransformFeedback;
    PFNGLENDTRANSFORMFEEDBACKPROC	 glEndTransformFeedback;
    PFNGLTRANSFORMFEEDBACKVARYINGSPROC   glTransformFeedbackVaryings;
    PFNGLGETTRANSFORMFEEDBACKVARYINGPROC glGetTransformFeedbackVarying;
	
    // RE_EXT_TRANSFORM_FEEDBACK_STREAM
    PFNGLBEGINQUERYINDEXEDPROC	    glBeginQueryIndexed; 
    PFNGLENDQUERYINDEXEDPROC	    glEndQueryIndexed;
    PFNGLGETQUERYINDEXEDIVPROC	    glGetQueryIndexediv;	
    PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC glDrawTransformFeedbackStream;
	
    // RE_FRAME_BUFFER_OBJECT
    PFNGLGENFRAMEBUFFERSPROC		glGenFramebuffers;
    PFNGLDELETEFRAMEBUFFERSPROC		glDeleteFramebuffers;
    PFNGLISFRAMEBUFFERPROC		glIsFramebuffer;
    PFNGLCHECKFRAMEBUFFERSTATUSPROC	glCheckFramebufferStatus;
    PFNGLFRAMEBUFFERTEXTURE2DPROC	glFramebufferTexture2D;
    PFNGLFRAMEBUFFERTEXTURE3DPROC	glFramebufferTexture3D;
    PFNGLFRAMEBUFFERTEXTURELAYERPROC	glFramebufferTextureLayer;
    PFNGLFRAMEBUFFERTEXTUREPROC		glFramebufferTexture;
    PFNGLBINDFRAMEBUFFERPROC		glBindFramebuffer;
    
    PFNGLGENRENDERBUFFERSPROC	    glGenRenderbuffers;
    PFNGLDELETERENDERBUFFERSPROC    glDeleteRenderbuffers;
    PFNGLISRENDERBUFFERPROC	    glIsRenderbuffer;
    PFNGLBINDRENDERBUFFERPROC	    glBindRenderbuffer;
    PFNGLRENDERBUFFERSTORAGEPROC    glRenderbufferStorage; 
    PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC
				    glRenderbufferStorageMultisample; 
    PFNGLBLITFRAMEBUFFERPROC	    glBlitFramebuffer;
    PFNGLFRAMEBUFFERRENDERBUFFERPROC  glFramebufferRenderbuffer;
    PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC
				    glGetFramebufferAttachmentParameteriv;
    PFNGLGENERATEMIPMAPPROC	    glGenerateMipmap;

    PFNGLDRAWBUFFERSPROC	    glDrawBuffers;

    // core GL 3.0
    PFNGLCLEARBUFFERIVPROC 	    glClearBufferiv;
    PFNGLCLEARBUFFERUIVPROC 	    glClearBufferuiv;
    PFNGLCLEARBUFFERFVPROC 	    glClearBufferfv;
    PFNGLCLEARBUFFERFIPROC 	    glClearBufferfi;
    
    // RE_EXT_BUFFER_STORAGE
    PFNGLBUFFERSTORAGEPROC	    glBufferStorage;
    PFNGLMEMORYBARRIERPROC	    glMemoryBarrier;

    // RE_EXT_MULTI_BIND
    PFNGLBINDBUFFERSBASEPROC	    glBindBuffersBase;
    PFNGLBINDBUFFERSRANGEPROC	    glBindBuffersRange;
    PFNGLBINDTEXTURESPROC	    glBindTextures;
    PFNGLBINDVERTEXBUFFERSPROC	    glBindVertexBuffers;
    // TODO: once the parent extensions are added, add these methods.
    //PFNGLBINDSAMPLERSPROC		    glBindSamplers;
    //PFNGLBINDIMAGETEXTURESPROC	    glBindImageTextures;

    // RE_EXT_VERTEX_BUFFER_OBJECT
    // RE_EXT_PIXEL_BUFFER_OBJECT
    PFNGLGENBUFFERSPROC		    glGenBuffers;
    PFNGLBINDBUFFERPROC		    glBindBuffer;
    PFNGLBUFFERDATAPROC		    glBufferData;
    PFNGLDELETEBUFFERSPROC	    glDeleteBuffers;
    PFNGLBUFFERSUBDATAPROC	    glBufferSubData;
    PFNGLGETBUFFERSUBDATAPROC	    glGetBufferSubData;
    PFNGLMAPBUFFERPROC		    glMapBuffer;
    PFNGLUNMAPBUFFERPROC	    glUnmapBuffer;
    PFNGLMAPBUFFERRANGEPROC	    glMapBufferRange;
    PFNGLFLUSHMAPPEDBUFFERRANGEPROC glFlushMappedBufferRange;
    PFNGLISBUFFERPROC		    glIsBuffer;
    
    // RE_EXT_VERTEX_ARRAY_OBJECT
    PFNGLBINDVERTEXARRAYPROC		    glBindVertexArray;
    PFNGLDELETEVERTEXARRAYSPROC		    glDeleteVertexArrays;
    PFNGLGENVERTEXARRAYSPROC		    glGenVertexArrays;
    PFNGLISVERTEXARRAYPROC		    glIsVertexArray;

    // RE_EXT_DRAW_INSTANCED
    PFNGLDRAWARRAYSINSTANCEDPROC	    glDrawArraysInstanced;
    PFNGLDRAWELEMENTSINSTANCEDPROC	    glDrawElementsInstanced;
    
    // RE_EXT_INSTANCED_ARRAYS
    PFNGLVERTEXATTRIBDIVISORPROC	    glVertexAttribDivisor;
    
    // RE_EXT_OCCLUSION_QUERY
    PFNGLGENQUERIESPROC			    glGenQueries;
    PFNGLDELETEQUERIESPROC		    glDeleteQueries;
    PFNGLISQUERYPROC			    glIsQuery;
    PFNGLBEGINQUERYPROC			    glBeginQuery;
    PFNGLENDQUERYPROC			    glEndQuery;
    PFNGLGETQUERYIVPROC			    glGetQueryiv;
    PFNGLGETQUERYOBJECTIVPROC		    glGetQueryObjectiv;
    PFNGLGETQUERYOBJECTUIVPROC		    glGetQueryObjectuiv;

    // RE_EXT_TIMER_QUERY
    PFNGLGETQUERYOBJECTI64VPROC		    glGetQueryObjecti64v;
    PFNGLGETQUERYOBJECTUI64VPROC	    glGetQueryObjectui64v;
    PFNGLQUERYCOUNTERPROC		    glQueryCounter;
    
    // RE_EXT_TEXTURE_MULTISAMPLE
    PFNGLTEXIMAGE2DMULTISAMPLEPROC	    glTexImage2DMultisample;
    PFNGLTEXIMAGE3DMULTISAMPLEPROC	    glTexImage3DMultisample;
    PFNGLGETMULTISAMPLEFVPROC		    glGetMultisamplefv;
    PFNGLSAMPLEMASKIPROC		    glSampleMaski;

    // RE_EXT_UNIFORM_BUFFER
    PFNGLGETUNIFORMINDICESPROC	            glGetUniformIndices;
    PFNGLGETACTIVEUNIFORMSIVPROC	    glGetActiveUniformsiv;
    PFNGLGETACTIVEUNIFORMNAMEPROC	    glGetActiveUniformName;
    PFNGLGETUNIFORMBLOCKINDEXPROC	    glGetUniformBlockIndex;
    PFNGLGETACTIVEUNIFORMBLOCKIVPROC	    glGetActiveUniformBlockiv;
    PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC      glGetActiveUniformBlockName;
    PFNGLUNIFORMBLOCKBINDINGPROC	    glUniformBlockBinding;
    PFNGLBINDBUFFERBASEPROC		    glBindBufferBase;
    PFNGLBINDBUFFERRANGEPROC		    glBindBufferRange;
    PFNGLGETINTEGERI_VPROC		    glGetIntegeri_v;

    // RE_EXT_TEXTURE_BUFFER
    PFNGLTEXBUFFERPROC			    glTexBuffer;

    // RE_EXT_IMAGE_LOAD_STORE
    PFNGLBINDIMAGETEXTUREPROC		    glBindImageTexture;

    // RE_EXT_COMPUTE_SHADER
    PFNGLDISPATCHCOMPUTEPROC		    glDispatchCompute;
    PFNGLDISPATCHCOMPUTEINDIRECTPROC	    glDispatchComputeIndirect;
    
    // RE_EXT_SHADER_STORAGE_BUFFER
    PFNGLSHADERSTORAGEBLOCKBINDINGPROC      glShaderStorageBlockBinding;

    // RE_EXT_TEXTURE_BINDLESS
    PFNGLGETTEXTUREHANDLEARBPROC	    glGetTextureHandleARB;
    PFNGLGETTEXTURESAMPLERHANDLEARBPROC	    glGetTextureSamplerHandleARB;
    PFNGLMAKETEXTUREHANDLERESIDENTARBPROC   glMakeTextureHandleResidentARB;	
    PFNGLMAKETEXTUREHANDLENONRESIDENTARBPROC glMakeTextureHandleNonResidentARB;	
    PFNGLGETIMAGEHANDLEARBPROC		    glGetImageHandleARB;
    PFNGLMAKEIMAGEHANDLERESIDENTARBPROC	    glMakeImageHandleResidentARB;
    PFNGLMAKEIMAGEHANDLENONRESIDENTARBPROC  glMakeImageHandleNonResidentARB;
    PFNGLUNIFORMHANDLEUI64ARBPROC	    glUniformHandleui64ARB;
    PFNGLUNIFORMHANDLEUI64VARBPROC	    glUniformHandleui64vARB;
    PFNGLPROGRAMUNIFORMHANDLEUI64ARBPROC    glProgramUniformHandleui64ARB;
    PFNGLPROGRAMUNIFORMHANDLEUI64VARBPROC   glProgramUniformHandleui64vARB;	
    PFNGLISTEXTUREHANDLERESIDENTARBPROC	    glIsTextureHandleResidentARB;
    PFNGLISIMAGEHANDLERESIDENTARBPROC	    glIsImageHandleResidentARB;
    PFNGLVERTEXATTRIBL1UI64ARBPROC	    glVertexAttribL1ui64ARB;
    PFNGLVERTEXATTRIBL1UI64VARBPROC	    glVertexAttribL1ui64vARB;
    PFNGLGETVERTEXATTRIBLUI64VARBPROC	    glGetVertexAttribLui64vARB;
    
    // RE_EXT_PRIMITIVE_RESTART
    PFNGLPRIMITIVERESTARTINDEXPROC	    glPrimitiveRestartIndex;

    // RE_EXT_ELEMENTS_BASE_VERTEX
    PFNGLDRAWELEMENTSBASEVERTEXPROC	    glDrawElementsBaseVertex;
    PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC    glDrawRangeElementsBaseVertex;
    PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC
					    glDrawElementsInstancedBaseVertex;
    PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC    glMultiDrawElementsBaseVertex;

    // RE_EXT_ELEMENTS_BASE_INSTANCE
    PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC
					    glDrawArraysInstancedBaseInstance;
    PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC
					    glDrawElementsInstancedBaseInstance;
    PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC
				  glDrawElementsInstancedBaseVertexBaseInstance;
    
    // RE_EXT_SYNC
    PFNGLFENCESYNCPROC		    glFenceSync;
    PFNGLISSYNCPROC		    glIsSync;
    PFNGLDELETESYNCPROC		    glDeleteSync;
    PFNGLWAITSYNCPROC		    glWaitSync;
    PFNGLGETINTEGER64VPROC	    glGetInteger64v;
    PFNGLGETSYNCIVPROC		    glGetSynciv;
    PFNGLCLIENTWAITSYNCPROC	    glClientWaitSync;

    // RE_EXT_CONDITIONAL_RENDER
    PFNGLBEGINCONDITIONALRENDERPROC glBeginConditionalRender;
    PFNGLENDCONDITIONALRENDERPROC   glEndConditionalRender;

    // GL 1.2
    PFNGLDRAWRANGEELEMENTSPROC	    glDrawRangeElements;

    // RE_EXT_SAMPLE_SHADING
    PFNGLMINSAMPLESHADINGPROC	    glMinSampleShading;

    // RE_EXT_DEBUG_OUTPUT
    PFNGLDEBUGMESSAGECONTROLPROC    glDebugMessageControl; 
    PFNGLDEBUGMESSAGEINSERTPROC	    glDebugMessageInsert;
    PFNGLDEBUGMESSAGECALLBACKPROC   glDebugMessageCallback;
    PFNGLGETDEBUGMESSAGELOGPROC	    glGetDebugMessageLog;

    // RE_EXT_DIRECT_STATE_ACCESS (add as needed)
    RE_PFNGLBINDMULTITEXTUREEXTPROC glBindMultiTextureEXT;

    // RE_EXT_DEBUG_KRONOS
    PFNGLPUSHDEBUGGROUPPROC	    glPushDebugGroup;
    PFNGLPOPDEBUGGROUPPROC	    glPopDebugGroup;
    PFNGLOBJECTLABELPROC	    glObjectLabel;
    PFNGLGETOBJECTLABELPROC	    glGetObjectLabel;
    PFNGLOBJECTPTRLABELPROC	    glObjectPtrLabel;
    PFNGLGETOBJECTPTRLABELPROC	    glGetObjectPtrLabel;

    PFNGLGETGRAPHICSRESETSTATUSPROC glGetGraphicsResetStatus;
	
#ifdef WIN32
    RE_PFNWGLSWAPINTERVALPROC	    wglSwapIntervalEXT;
    RE_PFNWGLGETEXTENSIONSSTRINGPROC wglGetExtensionsStringEXT;
#elif !defined(MBSD_COCOA)
    RE_PFNGLXSWAPINTERVALSGIPROC    glXSwapIntervalSGI;
#endif

    PFNGLGETSTRINGIPROC		    glGetStringi;

    // Only RE_Render should ever call these, and only to work around driver
    // issues.
    void		 enableExtension( RE_Extension ext );
    void		 disableExtension( RE_Extension ext );

    static void		*getExtFunction(void *a_out_handle, const char *symbol);

private:
    void		 setupTheExtensions();
    void		 checkExtensions(re_ExtData *exts,
					 const UT_StringSet &syms);
    bool		*myExtensions;
    UT_StringSet	*myGLExtensionTable;
    static bool		 theInitializedOnce;
};

#endif // __RE_OGLExt_h__
