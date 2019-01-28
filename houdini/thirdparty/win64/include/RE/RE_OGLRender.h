/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_OGLRender.h ( RE Library, C++)
 *
 * COMMENTS:
 *   Interface class for Open GL. Comamnds are grouped, in this order,
 *   under these tags:
 *
 *	- RENDERING ENVIRONMENT		(gl version, extensions, driver)
 *	- MONITOR SCREEN		(monitor setup)
 *	- DISPLAY MODE & WINDOWS	(windows, visuals)
 *	- ERROR REPORTING		(gl errors)
 *	- FRAMEBUFFER OBJECTS
 *	- BUFFER CONTROL		
 *	- FRUSTUM & CLIPPING
 *	- TRANSFORMATION
 *	- COLOR
 *	- LINE & POINT STYLES
 *	- STENCIL, LOGIC & DEPTH
 *	- BLENDING & SMOOTHING
 *	- DISPLAY LISTS
 *	- MATERIALS
 *	- GLSL SHADERS
 *	- TEXTURING
 *	- LIGHTING & SHADING
 *	- PICKING
 *	- RASTER OPERATIONS
 *	- SIMPLE PRIMITIVES		(rect, circles, arcs)
 *	- FONT RENDERING
 *	- SURFACES			(nurbs)
 *	- OBSOLETE DRAWING METHODS	(begin/end stuff, vertex calls)
 */
#ifndef __RE_OGLRender__
#define __RE_OGLRender__

#include "RE_API.h"

#include <SYS/SYS_Floor.h>
#include <SYS/SYS_Types.h>

#include <UT/UT_Array.h>
#include <UT/UT_Assert.h>
#include <UT/UT_Color.h>
#include <UT/UT_IntArray.h>
#include <UT/UT_ArrayMap.h>
#include <UT/UT_Pixel.h>
#include <UT/UT_Rect.h>
#include <UT/UT_SpinLock.h>
#include <UT/UT_Thread.h>
#include <UT/UT_ThreadSpecificValue.h>
#include <UT/UT_Vector2.h>
#include <UT/UT_VectorTypes.h>

#include <PXL/PXL_Common.h>

#include "RE_Extension.h"
#include "RE_Material.h"
#include "RE_OGL.h"
#include "RE_OGLExt.h"
#include "RE_OGLState.h"
#include "RE_RenderFlush.h"
#include "RE_Texture.h"
#include "RE_TextureTypes.h"
#include "RE_Uniform.h"

template <typename T1, typename T2> class UT_Pair;
template <typename ITEM_T> class UT_StringMap;
class UT_WorkBuffer;
class UT_StopWatch;
class UT_StringHolder;
class IMG_Raster;
class TIL_Raster;
class PXL_Raster;
class PXL_Lookup;

class RE_Cursor;
class RE_FeedbackPrimitivesWrittenQuery;
class RE_FontBuffers;
class RE_Geometry;
class RE_OcclusionQuery;
class RE_OGLFramebuffer;
class RE_OGLTexture;
class RE_OGLVertexArrayList;
class RE_PrimitivesGeneratedQuery;
class RE_Render;
class RE_Server;
class RE_Shader;
class RE_ShaderHandle;
class RE_Visual;
class RE_Window;
class RE_WindowList;

#define OPEN_GL

// These values are the keys in the table returned by getDriverInfoDict().
#define RE_DRIVER_INFO_VENDOR "OpenGL Vendor"
#define RE_DRIVER_INFO_RENDERER "OpenGL Renderer"
#define RE_DRIVER_INFO_VERSION "OpenGL Version"
#define RE_DRIVER_INFO_SHADING_LANGUAGE "OpenGL Shading Language"
#define RE_DRIVER_INFO_DETECTED "Detected"

// --------------------------------------------------------------------------

class RE_API RE_OGLRender : public RE_RenderFlush
{
public:
    virtual ~RE_OGLRender();

    const char		*className() const;
    int			 getContextID() const { return myUniqueID; }

    static void		 initialize();
    bool		 isInitialized() const { return !myFirstInitialize; }
    
    // For RE_OGLRender methods to pass an RE_Render * to various objects
    RE_Render		*getRender()
			    { return reinterpret_cast<RE_Render*>(this); }
    const RE_Render	*getRender() const
			    { return reinterpret_cast<const RE_Render*>(this); }

    // RENDERING ENVIRONMENT ------------------------------------------------
    // You can use these methods to query support for a specific OpenGL
    // version on the machine Houdini is running on.
    static bool		 hasGL11();
    static bool		 hasGL12(); 
    static bool		 hasGL13();
    static bool		 hasGL14();
    static bool		 hasGL15();
    static bool		 hasGL20();
    static bool		 hasGL21();
    
    static bool		 hasGL3(int minor);
    static bool		 hasGL4(int minor);
    
    static int		 glMajorVersion();
    static int		 glMinorVersion();

    // Determines the version of GLSL. Currently it's either 1.xx or 0.0 (unsup)
    int			 glslMajorVersion();  // 1
    int			 glslMinorVersion();  // 0, 10, 20, 30, 40 or 50
    int			 glslMaxOutputComponents(RE_ShaderType stage);
    int			 glslMaxUniformComponents(RE_ShaderType stage);

    bool		 hasGLExtension(RE_Extension e) const;
    
    // Low-level hardware and driver verisons. Use these only to work
    // around driver issues. All other features should use the GL version,
    // or RE_OGLExt.
    
    // Returns the hardware doing the rendering.
    static RE_GraphicsDevice getGraphicsDevice();

    // The driver version. NVidia uses XX.XX, while ATI uses X.XX.XX
    static bool		 hasDriverVersion();
    static int		 getDriverMajorVersion();
    static int		 getDriverMinorVersion();
    static int		 getDriverBuildMajorVersion();
    static int		 getDriverBuildMinorVersion();
    static void		 getDriverInfo(UT_WorkBuffer &info);
    static const UT_StringMap<UT_StringHolder> &getDriverInfoDict();
    static bool		 getUsingCoreProfile();
    static void		 setUsingCoreProfile(bool using_core_profile);

    // Access to the RE_OGLExt methods. Should not be used outside of RE; use
    // hasGLExtension() instead.
    RE_OGLExt		*getExt() const;

    // clear out all VBOs and textures in the graphics cache.
    static void		 clearGraphicsCache();

    // Queries to return the total free memory, and optionally the largest
    // available chunk of memory on the GPU. Extension RE_EXT_MEMORY_QUERY
    // is required.
    int64		getFreeTextureMemoryKB(int64 *largest_chunk = nullptr);
    int64		getFreeBufferMemoryKB(int64 *largest_chunk = nullptr);
    int64		getFreeFramebufferMemoryKB(int64 *largest_chunk = nullptr);

    // Returns the amount of free memory on the GPU when Houdini first started.
    static int64	getInitialFreeMemoryKB();

    // Call in the case of a fatal startup error due to insufficient driver
    // support.
    static void		exitWithBadDriver();
    
    // MONITOR SCREEN ---------------------------------------------------------
   
    static int		resX();
    static int		resY();

    static void		updateScreenSize();

    // Multi-monitor support. Primary monitor is screen 0.
    //
    // returns true if all the screens are the same size
    static bool		areScreensUniform();
    // returns a work area for the screen(s) occupied by 'area', for use when
    // multiple screens aren't uniform in size.
    static UT_DimRect	getWorkAreaForScreen(UT_DimRect area);

    // this method positions rect entirely on one screen, shifting if
    // required. If screen_hint is not -1, the screen containing that
    // coord will be used. It returns the screen chosen.
    //
    static int		positionOnScreen(UT_DimRect &area,
					 int screen_hint=-1);

    // returns true if the area fits entirely within one of the work areas.
    static bool		positionOkay(const UT_DimRect &area);
    
    static int		dpi();	     // system dpi * dpi scale factor * UI scale
    static fpreal	dpiScale();  // dpi scale factor (layout level scale)
    static fpreal	uiScale();   // UI scale factor (HOUDINI_UISCALE)
    static float	pixelsToInches(int n);
    static int		inchesToPixels(float i);
    static void		scaleDPI(fpreal dpi);
    static int		scaledSize(int size);
    static fpreal	scaledSize(fpreal size);

    // does a quick check to determine if the computer entered a suspend (sleep)
    // mode, where many GPU computed textures and buffers will need to be
    // recomputed.
    void		 checkForSuspendResume();
    static void		 addSuspendResumeCB(void (*cb)(void*), void *data);
    static void		 removeSuspendResumeCB(void (*cb)(void*), void *data);

    // DISPLAY MODE & WINDOWS ----------------------------------------------
    
    RE_DisplayMode	 getMode() const;
    RE_VisualType	 getVisualType() const;
    
    void		 enableMultisample(bool enable);
    bool		 isMultisampleEnabled() const;

    void		 enableAlphaToCoverage(bool enable);
    bool		 isAlphaToCoverage() const;

    void		 getMaxViewportSize(int &w, int &h) const;
    int			 getMaxColorSamples() const;
    int			 getMaxDepthSamples() const;
    
    static RE_Visual	*getVisualForMode(RE_DisplayMode newmode);
    static RE_Server	*getServer();
    static RE_Cursor	*getCursor(const char *name);

    void		 setWindow(RE_Window *, bool attach = true);
    
    bool		 makeCurrent(bool ignore_errors = false);
    bool		 makeCurrentQt();
    bool		 isCurrent() const
			    { return theCurrentRender.get() == this; }
    static void		 resetCurrent();
    bool		 contextIsValid() const { return (bool)myContext; }
    RE_OGLContext	 getContext() const { return myContext; }

    // When using Pbuffers instead of FBOs, we have to set them up as the
    // render target. You cannot push more than 1 buffer at a time.
    bool		 pushOffscreenBuffer(OGLDrawable type);
    void		 popOffscreenBuffer();

    // This is the context of the main window, which should persist always.
    void		 setMainContext(RE_Window *w);
    static RE_Window	*getMainContext(); 

    static RE_OGLRender	*getCurrentRender();
    static RE_WindowList *getWindowList();
    const  RE_Window	*getCurrentWindow() const; 
	   RE_Window	*getCurrentWindow();
    
    // These methods prepares for another thread to access OpenGL.
    // They are called from the same thread that will be accessing OpenGL.
    // If they're called from the main thread, they do nothing.
    //
    // These methods work by giving each thread its own OpenGL context.
    // Because other threads do not know when the main thread changes the
    // OpenGL context or drawable, they must call the prepare...() method
    // to explicitly set their context, before they main RE/OGL calls.
    //
    // These methods do not provide support for concurrent access to RE
    // from multiple threads: only one thread at a time may make RE calls.
    // The main thread must be blocked while the other thread makes the
    // calls.  That other thread must call lock...() while the main thread
    // is blocked, make its RE calls, call unlock...(), and only then may it
    // unblock the main thread.
    //
    // These methods are called by HOMF_HOMGLAutoLock so HOMF methods can
    // call code that eventually makes OpenGL calls, even when the Python
    // interpreter is running in a separate thread.
    void		 lockContextForRender();
    void		 unlockContextAfterRender();
    bool		 tryLockContextForRender();
    bool		 isContextLocked() const;
    
    void		 bumpRenderCount() { myRenderCount ++; }
    int64		 getRenderCount() const { return myRenderCount; }

    // ERROR REPORTING --------------------------------------------------
    int			 getGLError();
    int			 getNextGLError();
    void		 clearGLErrors();
    const char		*getGLErrorString( int error );
    void		 printGLError(const char *header,
				      bool assert = false );
    void		 printAllGLErrors( const char *header,
					   bool assert = false );

    void		 dumpNewState(bool show_all = false);
    void		 dumpViewportState(bool show_all = false);
    void		 dumpTextureState() const;
    void		 dumpVertexAttributes() const;
    void		 dumpFragmentTests(bool show_all = false) const;
    void		 dumpRasterizeState(bool show_all = false) const;
    void		 dumpTextureCacheState(int stack_level =-1) const;

    void		 getLimits(UT_WorkBuffer &os);

    // RE_EXT_DEBUG_OUTPUT2 (GL_KHR_debug) required with env HOUDINI_OGL_DEBUG>0
    bool		 isDebugLabelSupported() const;

    // HOUDINI_OGL_DEBUG > 0 and RE_EXT_DEBUG_OUTPUT2 is supported.
    bool		 isDebugging() const;
    
    int			 pushDebug(bool enable);
    void		 popDebug(int *nest = nullptr);

    int			 pushDebugGroup(const char *name, int id=1987);
    void		 popDebugGroup(int *nest = nullptr);

    void		 addComment(const char *name,
				    RE_Severity sev = RE_SEVERITY_MESSAGE,
				    int id =1987);
    

    // FRAMEBUFFER OBJECTS -------------------------------------------------
    
    // There are two bindings for framebuffers, read and draw. If
    // RE_EXT_FRAME_BUFFER_OBJECT_ARB is not supported, these will be the same.
    int			 pushDrawFramebuffer(); // no change to FBO state
    int			 pushDrawFramebuffer(RE_OGLFramebuffer *fb);
    void		 setDrawFramebuffer(RE_OGLFramebuffer *fb);
    RE_OGLFramebuffer	*getDrawFramebuffer();
    void		 popDrawFramebuffer(int *nest = nullptr);

    int			 pushReadFramebuffer(); // no change to FBO state
    int			 pushReadFramebuffer(RE_OGLFramebuffer *fb);
    void		 setReadFramebuffer(RE_OGLFramebuffer *fb);
    RE_OGLFramebuffer	*getReadFramebuffer();
    void		 popReadFramebuffer(int *nest = nullptr);

    void		 updateFramebuffer();
    // Only called by RE_OGLFramebuffer; updates the current FBO state.
    void		 updateCurrentFramebuffer(RE_OGLFramebuffer *fb,
						  bool for_draw);

    void		 registerFBO(RE_OGLFramebuffer *fb, int fb_id);
    void		 unregisterFBO(int id);
    RE_OGLFramebuffer   *getFBO(int id) const;

    // Sample state
    void 		 useSampleMask(bool use);
    bool 		 usingSampleMask() const;
    int 		 getMaxIntegerSamples() const;
    int 		 getSampleMask(int mask_number);
    void 		 setSampleMask(int mask_number, int mask);
    static int		 getMatteMaskForSampleCount(int samples);
    
    // BUFFER CONTROL -------------------------------------------------------

    // If possible, call clearCZ() instead of clear() followed by clearZ(),
    // since a single clearCZ() is much faster on all post-Indy machines.
    void		clearC(float *color = nullptr);
    void		clearZ(float z = 1.0);
    void		clearCZ(float z = 1.0);
    void		clearS();

    // Using these methods, you can disable updates to the colour and/or depth
    // bits of the frame buffer.  Be careful with these, since the associated
    // calculations will still be performed (eg. even if color buffer updates
    // are disabled, useless lighting calculations are still done if
    // GL_LIGHTING is enabled).  Same with GL_DEPTH_TEST.  So you can easily
    // write inefficient code.  
    void		enableColorBufferWriting();
    void		disableColorBufferWriting();
    bool		getColorBufferWriting();
    void		updateColorBufferWriting();

    void	 setColorMask(bool red, bool green, bool blue, bool alpha);
    void	 getColorMask(bool &red, bool &green, bool &blue, bool &alpha);

    bool	 	swapbuffers();

    // By default, the buffer is not changed (RE_BUFFER_UNINIT)
    int			pushDrawBuffer( RE_RenderBuf buf = RE_BUFFER_UNINIT);
    void		popDrawBuffer(int *nest = 0);
    void		setDrawBuffer( RE_RenderBuf buf );
    RE_RenderBuf	getDrawBuffer();
    bool		getDrawBufferIsBlendable();

    int			pushReadBuffer( RE_RenderBuf buf = RE_BUFFER_UNINIT);
    void		popReadBuffer(int *nest = 0);
    void		setReadBuffer( RE_RenderBuf buf );
    RE_RenderBuf	getReadBuffer();

    // set by the current window, UI_Window::setContext(). returns if the
    // front buffer has been written to since last swap (for marquee drawing)
    //
    bool		isFrontBufferDirty() const;
    void		setFrontBufferDirty(bool d);

    void		flush(int wait=0);

    /// Ensure all writes are complete to the specified resource(s).
    /// GL4.2, RE_EXT_BUFFER_STORE or RE_EXT_SHADER_LOAD_STORE must be
    /// supported, or this will return false, doing nothing.
    bool		memoryBarrier(RE_MemoryBarrierBitfield barriers);
    
    // FRUSTUM & CLIPPING  ------------------------------------------------

    // masked rendering (scissor)
    void	 viewport2DI(const UT_DimRect &rect);
    void	 screenMask2DI(const UT_DimRect &rect);
    void	 disableScreenMask();
    void	 intersectMask2DI(const UT_DimRect &rect);
    UT_DimRect	 getScreenMask2DI();
    UT_DimRect	 getViewport2DI();
    bool	 getScreenMask();

    // Clip distance enables
    int		 getOGLMaxClipPlanes();
    void	 enableClipDistance(int clip_plane, bool enable);
    bool	 clipDistanceEnabled(int clip_plane) const;

    // Active Occlusion query (only 1 active at a time)
    void	 setActiveOcclusionQuery(RE_OcclusionQuery *q);
    RE_OcclusionQuery *getActiveOcclusionQuery() const;

    // Backface culling
    int		 pushBackfaceCulling();
    int		 pushBackfaceCulling(bool backface);
    void	 popBackfaceCulling(int *nested = 0);
    
    bool	 isBackface() const;
    void	 setBackface(bool removeBackface);
    bool	 getReverseWinding();
    void	 setReverseWinding(bool reverse_winding);
    RE_FaceMode  getCullFace() const;
    void	 setCullFace(RE_FaceMode m);

    // polygon/line offset
    bool	 isPointOffset();
    bool	 isLineOffset();
    bool	 isPolygonOffset();
    void	 pointOffset( bool onoff );
    void	 lineOffset( bool onoff );
    void	 polygonOffset( bool onoff );
    void	 setOffsetAmount( float variable, float constant );
    void	 getOffsetAmount(float *variable, float *constant) const;
    


    // Start collecting vertex/geometry shader attribute output into buffers.
    // There must be a shader active, and this shader must have one or more
    // transform feedback buffers enabled. 'mode' must be one of RE_PRIM_POINTS,
    // RE_PRIM_LINES, or RE_PRIM_TRIANGLES, and only those types can be
    // rendered. 'rasterize_too' controls whether primitives continue on to the
    // fragment shader to be rasterized or stop at the transform feedback stage.
    // 'stream_bitfield' allows multiple vertex streams to be captured when GL4
    // is present (bit N = vertex stream N). 
    bool	 beginTransformFeedback(RE_PrimType mode,
					bool rasterize_too,
					int stream_bitfield = 0x1);

    // only call end if begin returned true.
    void	 endTransformFeedback();

    // returns the number of primitives collected in the transform feedback
    // buffers. Must be called after endTransformFeedback().
    int		 getNumFeedbackPrimitives(int stream = 0);

    // Returns true if the feedback buffers overflowed. Must be called after
    // endTransformFeedback()
    bool	 hasFeedbackBufferOverflow(int stream = 0);

    // Transform feedback state queries.
    bool	 isTransformFeedbackEnabled() const;
    RE_PrimType  getFeedbackMode() const;
    bool	 isRasterizeEnabled() const;

    // Line & Point sizes
    int		 pushLineWidth(float w);
    void	 popLineWidth(int *nest = nullptr);
    float	 getLineWidth() const;
    float	 getMaxSmoothLineWidth();

    int		 pushPointSize(float size, bool program_point_size = false);
    void	 popPointSize(int *nest = nullptr);
    void	 setPointSize(float size);

    // If enabled, point size is taken from vertex or geometry shader's
    // gl_PointSize output
    void	 setProgramPointSize(bool enable);
    bool	 getProgramPointSize() const;
    
    // STENCIL, LOGIC & DEPTH ------------------------------------------------

    // The depth state consists of the depth test bit, depth compare function
    // depth clamp, and the depth buffer writemask. Note that pushBlendState()
    // can affect the depth buffer writemask, so it is important that if they
    // are nested, the order of the pops is reversed from the pushes.
    int		  pushDepthState();
    void	  popDepthState(int *nest = nullptr);

    // In order to enable depth buffer writes, a depth buffer must be present
    // in the current window or framebuffer.
    void	  enableDepthBufferWriting(); 
    void	  disableDepthBufferWriting();
    bool	  getDepthBufferWriting();
    void	  updateDepthBufferWriting();
    
    // Depth testing must be enabled for depth buffer writes to have any effect.
    void	  enableDepthTest();
    void	  disableDepthTest();
    bool	  getDepthTest();
    void	  updateDepthTest();

    // The depth function only has an effect if depth testing is enabled.
    void	  setZFunction(RE_ZFunction func);
    RE_ZFunction  getZFunction() const;
    void	  updateZFunction() const;

    // Depth clamping causes objects outside of [near,far] to clamp their Z to
    // near or far. By default it is off.
    void	  enableDepthClamp(bool b);
    bool	  getDepthClamp();
    void	  updateDepthClamp();

    void	  setDepthRange(double nr,  double fr);
    void	  getDepthRange(double &nr, double &fr) const;

    // Stencil state
    int		  pushStencilState();
    void	  popStencilState(int *nest = nullptr);
    bool	  isStencil();
    void	  setStencil(bool enable);
    void	  setSClearValue(int clearValue);
    void	  setSWriteMask(int writeMask);
    void	  setSFunction(RE_SFunction func, int ref, int mask);
    void	  setSOperation(RE_SOperation sfail,
				RE_SOperation dpfail,
				RE_SOperation dppass);
    void	  updateStencilState();
    void	  resetSFunction();
    void	  resetSWriteMask();
    void	  resetSClearValue();
    void	  resetSOperation();

    // Logic state
    void	  enableLogicOp();
    void	  disableLogicOp();
    void	  invertPixels();
    void	  xorPixels();
    
    // BLENDING and SMOOTHING -----------------------------------------------
    int		  pushBlendState();
    void	  popBlendState(int *nesting = 0);
    
    bool	  isBlending() const;
    void	  blend(int onoff);
    void 	  setBlendFunction(RE_BlendSourceFactor  sourceFactor,
				   RE_BlendDestFactor    destFactor);
    bool	  setAlphaBlendFunction(RE_BlendSourceFactor  sourceFactor,
					RE_BlendDestFactor    destFactor,
					bool force = false);
    void	  getBlendFunction(RE_BlendSourceFactor* sourceFactor,
				  RE_BlendDestFactor*   destFactor);
    void	  getAlphaBlendFunction(RE_BlendSourceFactor  *sourceFactor,
					RE_BlendDestFactor    *destFactor);

    // blendAlpha uses (Sa, 1-Sa). blendAlphaPremult uses (1, 1-Sa) (assumes
    // color is premultiplied)
    void	  blendAlpha(int onoff = 1);
    void	  blendAlphaPremult(bool onoff = true);
    
    int		  pushSmoothLines();
    void	  popSmoothLines(int *nesting = 0);
    void	  forceSmooth();
    void	  smoothBlendLines(RE_SmoothMode mode);
    void	  smoothBlendLinesNoFlagChange(bool by_smooth);
    RE_SmoothMode getSmoothLines();

    int		  getBlendSmoothLevel() const;

    static void	  allowSmoothLines(int yn);
    static int	  allowsSmoothLines();

    static int	  getGLBlendSource(RE_BlendSourceFactor sourceFactor);
    static int	  getGLBlendDest(RE_BlendDestFactor destFactor);
    
    // MATERIALS -------------------------------------------------------------

    // The layer here is indexed from 0, while the layers in the GU_Details
    // are indexed from 1.
    void	 setMaterial(const RE_MaterialPtr &mat,
			     int layer=0, bool texture=true,
			     RE_ShaderTarget sht = RE_SHADER_TARGET_TRIANGLE,
			     RE_Shader *override_mat_shader = nullptr);
    int		 pushMaterial(const RE_MaterialPtr &mat,
			      int layer=0, bool texture=true,
			      RE_ShaderTarget sht = RE_SHADER_TARGET_TRIANGLE,
			      RE_Shader *override_mat_shader = nullptr);
    int		 pushMaterial();
    void	 popMaterial(int *nest = nullptr);


    // material used as a default material
    const RE_MaterialPtr	&getDefaultMaterial();

    // GLSL SHADERS ---------------------------------------------------------

    int		 pushShader(); // no change to state
    int		 pushShader(RE_Shader *s);
    int		 pushShader(RE_ShaderHandle &sh);

    void	 bindShader(RE_Shader *s);
    void	 bindShader(RE_ShaderHandle &sh);
    
    RE_Shader	*getShader();
    void	 popShader(int *nest = nullptr);
    
    void	 updateShader(RE_Shader *s); // only called by RE_Shader.

    int		 pushShaderTransformFeedback(RE_Shader *sh,
					     RE_PrimType type,
					     bool rasterize = true,
					     int stream_bitfield = 0x1);
    int		 pushShaderTransformFeedback(RE_ShaderHandle &sh,
					     RE_PrimType type,
					     bool rasterize = true,
					     int stream_bitfield = 0x1);
    void	 popShaderTransformFeedback(int *nest = nullptr);
    
    // When on, the GL shader will never be set back to nullptr by popShader();
    // however, an RE_Geometry::draw() command when the shader should be nullptr
    // will enter fixed function mode. This is basically a 'lazy pop' mode.
    // immediate mode begin/end should not be used with this shading mode.
    void	 setShaderOnlyMode(bool enable);
    void	 requestFixedFunction();

    /// Set the tesselation default outer level parameters (with no tess control
    /// shader) for the tesselator. Requires RE_EXT_TESS_SHADER or GL4.
    void	 setPatchOuterLevel(const UT_Vector4F &outer);
    
    /// Set the tesselation default inner level parameters (with no tess control
    /// shader) for the tesselator. Requires RE_EXT_TESS_SHADER or GL4.
    void	 setPatchInnerLevel(const UT_Vector2F &inner);

    /// Set the number of vertices per patch for tesselation. Requires
    /// RE_EXT_TESS_SHADER or GL4.
    void	 setPatchVertices(int num_verts);

    // for multisampled buffers on GL4+ hardware, the fragment shader can be
    // run to produce more than one color sample. The rate is a fraction between
    // 0.0 and 1.0, which will guarentee at least ceil(rate*samples) color
    // samples are produced in multisampled buffers. Note this can degrade
    // performance substantially for some heavy fragment shaders.
    // If RE_EXT_SAMPLE_SHADING is unsupported, this has no effect and
    // setShaderSampleRate() will return false.
    bool	 setShaderSampleRate(fpreal rate);
    fpreal	 getShaderSampleRate() const;

    bool	 isSampleShading() const;
    
    // Assign a new uniform value to built-in uniform. If nullptr is passed,
    // the value is unchanged but the current value copied to the uniform stack.
    void	 pushUniform(RE_UniformBuiltIn bindex,
			     RE_Uniform *var = nullptr);
    // Assign raw values to a built-in uniform. The array must be at least of
    // size getUniform(builtin_var_type)->getByteSize().
    void	 pushUniformData(RE_UniformBuiltIn bindex,
				 const void *data);
    void	 pushUniformInt(RE_UniformBuiltIn bindex, int i);
    void	 pushUniformColor(RE_UniformBuiltIn bindex,
				  const UT_Color &clr, fpreal32 alpha = 1.0f);
    void	 pushUniformSampler(RE_UniformBuiltIn bindex,
				    RE_Texture *tex);
    void	 pushUniformMatrix(RE_UniformBuiltIn bindex,
				   const UT_Matrix4D &mat);
    void	 assignUniformData(RE_UniformBuiltIn bindex,
				   const void *data);
    void	 assignUniformInt(RE_UniformBuiltIn bindex, int i);
    void	 assignUniformSampler(RE_UniformBuiltIn bindex,
				      RE_Texture *tex);
    void	 assignUniformColor(RE_UniformBuiltIn bindex,
				    const UT_Color &clr, fpreal32 alpha = 1.0f);
    void	 assignUniformMatrix(RE_UniformBuiltIn bindex,
				   const UT_Matrix4D &mat);
    void	 popUniform(RE_UniformBuiltIn bindex);
    void	 popUniformData(RE_UniformBuiltIn bindex)
		 { popUniform(bindex); }
    
    RE_Uniform  *getUniform(RE_UniformBuiltIn builtin_var_type) const;

    // Prints all builtins that have been assigned, or only those which the
    // current shader has bound (if bound_only is true).
    void	 printBuiltInUniforms(bool bound_only);

    // Print a single builtin's current value.
    void	 printBuiltInUniform(RE_UniformBuiltIn b);
    
    // Bind all builtin uniforms to the shader. Returns the # of uniforms bound
    int		 bindBuiltInUniforms(RE_Shader *s);

    // Bind the specified uniform to the shader. returns true if a uniform was
    // present to bind.
    bool	 bindBuiltInUniform(RE_Shader *s,
				    RE_UniformBuiltIn uniform);

    int64	 getBuiltInUniformSerial() const
					{ return myBuiltInUniformSerial; }
    void	 bumpBuiltInUniformSerial() { myBuiltInUniformSerial++; }

    // Assigns a generic uniform, which will be bound to any shaders with this
    // name. These will override any uniform values when the shader is bound, or
    // values on the current shader when the uniform is pushed. The name and
    // uniform type must match exactly for an override to occur.
    // If more than one uniform of the same name and type is pushed, the last
    // one pushed takes precedent. Popping it will restore the previous uniform.
    void	 pushUniform(RE_Uniform &uniform);
    void	 popUniform(RE_Uniform &uniform);

    // Sets a GLSL hint to compute derivatives - fast = false, nice = true
    void	 setFragmentDerivativeHint(bool nicest);
    bool	 getFragmentDerivativeHint();

    // Maximum number of vertex attributes supported for vertex shader input
    int		 getMaxVertexAttribs() const { return myMaxVertexAttribs; }

    // Set a vertex attribute at input 'loc' to 'vsize' constant values.
    // 'vsize' must be between 1 and 4.
    void	 setConstAttribF32(int loc, int vsize, const fpreal32 *data);

    // Set a vertex attribute to constant values. Downcasts to FP32 in GL call.
    void	 setConstAttribF32(int loc, int vsize, const fpreal64 *data);
    
    // Set a vertex attribute to constant values. Sets as proper FP64.
    void	 setConstAttribF64(int loc, int vsize, const fpreal64 *data);
    
    // Set a vertex attribute to constant values. 
    void	 setConstAttribI32(int loc, int vsize, const int32 *data);
    
    // Set a vertex attribute to constant values. 
    void	 setConstAttribU32(int loc, int vsize, const uint32 *data);

    // Notify the context that 'list' is now the current VAO.
    bool		   setCurrentVAO(RE_OGLVertexArrayList *list);

    // Query the current VAO.
    RE_OGLVertexArrayList *getCurrentVAO();

    // Query the default VAO for this context.
    RE_OGLVertexArrayList *getDefaultVAO() { return myDefaultVAO; }

    // Don't call this. 
    void	setDefaultVAO(RE_OGLVertexArrayList *vao) { myDefaultVAO = vao;}

    // Notify the context that 'list' is being deleted, and any referenece
    // should be cleared.
    void	 resetCurrentVAO(RE_OGLVertexArrayList *deleted_list);

    // Returns a list of the currently bound uniform blocks to this context.
    UT_IntArray	&getBoundUniformBlocks() { return myState.myBoundUniformBlocks;}

    // TEXTURING -------------------------------------------------------------

    // Bind the given texture to the specified, or active, texture unit. The
    // texturing mode can be optionally set at the same time (if not set to
    // UNKNOWN). If texturing for the texture's type is not enabled on the
    // texturing unit, it is enabled.
    void	 bindTexture(const RE_OGLTexture *tex,
			     int unit = RE_ACTIVE_UNIT,
			     RE_TextureMode mode = RE_TEXTURE_UNKNOWN);
    void	 verifyTextureIsNotBound(RE_OGLTexture *texture);

    // Bind texture buffer 'id' to 'unit' and return the previously bound
    // texture at 'unit'. This is only used by low-level RE methods.
    const RE_OGLTexture *bindTextureBuffer(int id, int unit);

    // Unbind all instances of texture 'tex'. This can be an expensive call.
    void	 unbindTexture(const RE_OGLTexture *tex);
    
    // Query & set the active texture units.  Note these are 0 based,
    // not GL_TEXTURE0 based.
    void	 setActiveTexture(int textureunit);
    int		 getActiveTexture() const;

    // Save the texture state for the specified texture unit, or all of
    // them (RE_ALL_UNITS). If the entire state is pushed, the active texture
    // unit is saved as well. RE_ALL_UNITS is relatively expensive, so use
    // with care. push returns a nesting index which can be passed to pop to
    // ensure proper nesting of push/pop calls.
    int		 pushTextureState(int textureunit = RE_ACTIVE_UNIT);
    void	 popTextureState(int *nest = nullptr);

    // The type and texture object currently bound to the given,
    // or current (-1), texture unit
    RE_TextureDimension getBoundTextureType(int texunit = RE_ACTIVE_UNIT) const;
    const RE_OGLTexture *getBoundTexture(int texunit = RE_ACTIVE_UNIT) const;

    // Returns the first texture unit with no bound unit, starting the search
    // at starting_unit. -1 is returned if no free units are available.
    int			findFirstFreeTextureUnit(int starting_unit = 0) const;
    
    // Reads in initial values for all the textures.
    void	 updateTextureState() const;
    
    // Query the maximum number of texture units we support.
    int		 getMaxShaderTextureUnits() const
			{ return myMaxTextureShaderUnits; }
    int		 getMaxVertexTextureUnits() const
			{ return myMaxTextureVertexUnits; }
    int		 getMaxGeometryTextureUnits() const
			{ return myMaxTextureGeometryUnits; }
    int		 getMaxFragmentTextureUnits() const
			{ return myMaxTextureFragmentUnits; }
    int		 getTextureAnisotropyLimit() const
			{ return myMaxTextureAnisotropy; }

    // Query the maximum size of a texture that is supported (either width or
    // height)
    int		 getMaxTextureSize() const
			{ return myMaxTextureSize; }
    int		 getMaxTexture3DSize() const
			{ return myMaxTexture3DSize; }
    int		 getMaxTextureRectangleSize()
			{ return myMaxTextureRectSize; }

    // Query the maximum number of frame buffer color attachments.
    int		 getMaxColorBuffers() const
			{ return myMaxColorBuffers; }
    // Query the max number of draw buffers.
    int		 getMaxDrawBuffers() const
			{ return myMaxDrawBuffers; }

    // This will disable for all texture units.
    void	 disableAllTextures();
    void	 removeTextureRefs(RE_OGLTexture *tex);

    // These methods ensure that the pixel store options are set so that the
    // 
    int		 pushReadAlignment(const void *data, int scansize);
    void	 popReadAlignment(int *nest = nullptr);

    int		 pushWriteAlignment(const void *data, int scansize);
    void	 popWriteAlignment(int *nest = nullptr);

    // Enables seamless cubemap filtering for all cubemaps, if supported.
    void	 useSeamlessMaps(bool enable);
    bool	 usingSeamlessMaps();

    // Returns an 8b RGBA 64x64 texture with 0-1 random values in all comps.
    RE_OGLTexture *getRandomTexture(unsigned int seed, bool interp,
				    int size = 64);

    // Returns an 8b RGBA size^3 3D texture with 0-1 random values
    RE_OGLTexture *getRandomTexture3D(unsigned int seed, bool interp,
				      int size = 32);
    // Returns an 8b RGBA size^2 2D texture array of size 'num_layers'
    RE_OGLTexture *getRandomTextureArray(unsigned int seed, bool interp,
					 int num_layers, int size = 64 );

    // returns an int32 single channel texture that returns a random sample
    // mask. The texture is size x (nsamples+1) in size, with each row having
    // 'N' bits randomly enabled, where N is the row # (0..nsamples). This can
    // be used for gl_SampleMask
    RE_OGLTexture *getRandomSampleMaskTexture(unsigned int seed, int nsamples,
					      int size = 64);
    
    // Texture Image drawing ------------------------------------------------
    
    // makes a texture out of the raster, and stores the id in the raster.
    void	 convertRasterToTexture(PXL_Raster *raster,
					int convert16bit = 0);
    void	 convertLUTToTexture(PXL_Lookup *lookup);
    bool	 setupShaderForLUT(RE_Shader *shader,
				   PXL_Lookup *lookup,
				   float gamma,
				   bool add_shader);
    bool	 is3DLUTSupported() const;

    void	 buildRasterTextureGeometry(
			fpreal32 x0, fpreal32 y0, fpreal32 z,
			fpreal32 x1, fpreal32 y1,
			fpreal32 u0, fpreal32 v0,
			fpreal32 u1, fpreal32 v1,
			RE_Geometry &geo);
    void	 displayRasterTexture(float x, float y, float z,
				      const PXL_Raster *raster,
				      const RE_RasterOpts *opts = 0);
    void	 displayRasterTexture(float x, float y, float z,
				      float w, float h,
				      const IMG_Raster *raster,
				      const RE_RasterOpts *opts = 0);
    void	 displayRasterTexture(float x, float y, float z,
				      int w, int h,
				      UT_RGBA *r, int stride,
				      float zoomx=-1.0f,
				      float zoomy=-1.0f,
				      int dither=1,
				      float alpha_mult=1.0f);
    
    // The saveRaster method saves the front buffer to a raster.  The
    //	raster should be the same size as the w and h parms.  As well,
    //	currently, we only support 8 bit channels in the raster. A zoom of -1
    // uses the current zoom (default).
    void	 saveRaster(int x, int y, int w, int h,
				    IMG_Raster *r, bool use_backbuf = true);
    TIL_Raster * saveRaster(int x, int y, int w, int h,
				    PXL_DataFormat format, bool alphatoo,
				    bool use_backbuf = true);
    

    // The following method is used to align text in the viewport where the
    // current xform makes attempting to use textMove* impractical for this
    // purpose.
    // NOTE: Clipping is performed using the actual raster position (set
    // through textMove*) and not the offset position.
    // NOTE: Only works for bitmap fonts, not texture fonts.
    void	 setViewportFontOffset(int x, int y);
    int		 getViewportFontOffsetX() const;
    int		 getViewportFontOffsetY() const;

    RE_FontBuffers *fetchFontBufferFromPool(int size);
    void	  returnFontBufferToPool(RE_FontBuffers *buf);
    
    // DRAWING METHODS -------------------------------------------------------

    // Begin a conditional render based on an occlusion query's samples result.
    // If the query returns zero samples, all drawing is ignored until
    // the conditional render is ended. Otherwise, rendering is done as normal.
    // Conditional rendering cannot be nested.
    //
    // 'render_wait' specifies whether the render should wait for the results
    //		     of the occlusion query, if they are not available.
    // 'region_support' allows the conditional render to happen at a fine
    //			grain level, which is implementation specific.
    
    // beginConditionalRender() will return false if the the query_object has
    // not be run yet, if a conditional render is already in progress, or if
    // RE_EXT_CONDITIONAL_RENDER is not supported. Do not call
    // endConditionalRender() in this case.
    bool		beginConditionalRender(RE_OcclusionQuery *query_obj,
					       bool render_wait,
					       bool region_support);
    void		endConditionalRender();

    // enables a primitive restart index, which begins a new primitive when
    // using an element buffer.
    void		enablePrimitiveRestart(bool enable);
    void		setPrimitiveRestartIndex(unsigned int index);

    // A few higher level functions for simple tasks rendering to the
    // viewport. These encapsulate a lot of boilerplate code.
    void		drawViewportPoint(const fpreal32 p[3],
				fpreal32 size);
    void		preDrawViewportString(UT_Matrix4D &view,
				UT_Matrix4D &proj,
				UT_DimRect &viewport);
    void		drawViewportString(const fpreal32 p[3],
				const char *str,
				const UT_Color *clr = nullptr,
				const UT_Matrix4D *view = nullptr,
				const UT_Matrix4D *proj = nullptr,
				const UT_DimRect *viewport = nullptr);
    void		postDrawViewportString();

    // Called by various RE classes; do not call directly.
    static void  destroyShaderObject(unsigned int progid, bool unreg,
				     bool shader);
    static void	 destroyTextureObject(unsigned int texid, RE_OGLTexture *tex);
    static void  destroyRenderbufferObject(unsigned int rb);
    static void	 destroyBufferObject(unsigned int buftype, unsigned int buf);
    static void	 destroySync(void *sync);

    static void	 clearCachedUniformBuffer(int id);

    // Context specific objects
    void	destroyVertexArrayObject(unsigned int vao);
    void	destroyQuery(unsigned int query);
    void	destroyFramebufferObject(unsigned int fbo);

    void	fetchDriverInfo(UT_WorkBuffer &info, bool full_info);

    // unbind common objects that might get stomped on by HDK calls:
    // shader, VAO, textures
    void	unbindPipeline();

    // Invalidates all cached state. Useful when GL calls outside of RE are
    // used which change the GL state away from the cached state, and it is too
    // difficult to track the state, which is often the case with HDK plugins.
    void	invalidateCachedState();
    
    // If GL calls outside of RE are used and change the state without restoring
    // it, call this method to update RE_OGLRender to the new state. This method
    // is somewhat expensive (causing a GPU/CPU sync) and should not be called
    // frequently or performance will suffer.
    void	resync() { updateGLState(); }

// Private methods ----------------------------------------------------------
protected:

    /// Protected constructor to avoid having it created standalone. Strong
    /// assumptions are made that RE_OGLRender objects are always an instance
    /// of an RE_Render subclass.
		RE_OGLRender(int do_foreground, const char *appname = 0);

    //  use macros for state change functions. Member variables are in
    // RE_OGLState.
#define	RE_FLAG_STATE(RE_NAME,GL_FLAG) \
    void enable##RE_NAME () { if( myState.my##RE_NAME##State != 1 ) { ::glEnable( GL_FLAG ); myState.my##RE_NAME##State = 1; }  } \
    void disable##RE_NAME () { if( myState.my##RE_NAME##State != 0 ) { ::glDisable( GL_FLAG ); myState.my##RE_NAME##State = 0; }  } \
    bool get##RE_NAME () { if(myState.my##RE_NAME##State == 3) update##RE_NAME(); return (myState.my##RE_NAME##State ==1); } \
    void update##RE_NAME () { myState.my##RE_NAME##State = ::glIsEnabled( GL_FLAG ); }	\
    void invalidate##RE_NAME() { myState.my##RE_NAME##State = 3; }

    RE_FLAG_STATE(Scissor,GL_SCISSOR_TEST);
    RE_FLAG_STATE(LineSmoothing,GL_LINE_SMOOTH);
    RE_FLAG_STATE(Stencil,GL_STENCIL_TEST);
#undef	RE_FLAG_STATE

#define	RE_FLAG_11_STATE(RE_NAME,GL_FLAG) \
    void enable##RE_NAME () { if( (myState.my##RE_NAME##State) != 1 && hasGL11() ) {::glEnable( GLenum(GL_FLAG) ); myState.my##RE_NAME##State = 1; }  } \
    void disable##RE_NAME () { if( myState.my##RE_NAME##State && hasGL11() ) { ::glDisable( GLenum(GL_FLAG) ); myState.my##RE_NAME##State = 0; } } \
    bool get##RE_NAME () { if(myState.my##RE_NAME##State == 3) update##RE_NAME(); return (myState.my##RE_NAME##State==1) ; } \
    void update##RE_NAME () { if( hasGL11() ) { myState.my##RE_NAME##State = ::glIsEnabled( GLenum(GL_FLAG) ); } else { myState.my##RE_NAME##State = 0; } } \
    void invalidate##RE_NAME() { if(hasGL11()) myState.my##RE_NAME##State=3; }

    RE_FLAG_11_STATE(PointOffset,GL_POLYGON_OFFSET_POINT);
    RE_FLAG_11_STATE(LineOffset,GL_POLYGON_OFFSET_LINE);
    RE_FLAG_11_STATE(FillOffset,GL_POLYGON_OFFSET_FILL);
#undef	RE_FLAG_11_STATE

    void	updateOffsetAmount();
    
    static bool	 initGLVersionInfo();
    virtual void initGLState();
    virtual void initGLExtensions();
    virtual void updateGLState();// load the following state variables
    virtual void updateStacks();
    void	 switchTexture(int textureunit, const RE_OGLTexture *texture);

    static void	 determineTwinView();
    bool	 switchContextForRender(bool ignore_errors);

#ifdef WIN32
private:
#endif

    void		updateBlendSmoothState();
    int			privPushBlendSmoothState(bool smooth_too);
    void		privPopBlendSmoothState(bool smooth_too, int *idx);

    void		setupDebugging();

    void		freePendingObjects();
    
    UT_DimRect	 adjustFramebufferViewport(const UT_DimRect &rect, bool to_fbo);

    void	 assignPushedUniforms();

    void         fetchDriverInfoDict(UT_StringMap<UT_StringHolder> &dict);

protected:
    // get the viewportState from GL
    void		updateViewport();

    static RE_ZFunction	 oglToREzfunc( int oglZFunc );
    static int		 reToOGLzfunc( RE_ZFunction reZFunc );
    static RE_SFunction	 oglToREsfunc( int oglSFunc );
    static int		 reToOGLsfunc( RE_SFunction reSFunc );
    static RE_SOperation oglToREsoperation( int oglSOperation );
    static int		 reToOGLsoperation( RE_SOperation reSOperation );

    static const char *bufferName(GLint e);
    static const char *faceName(GLint e);
    static const char *hintName(GLint e);
    static const char *cullName(GLint e);

    // ---------------------------------------------------------------------
    // Cached OpenGL State 

    mutable RE_OGLState  myState;

    // ------------------------------------------------------------------------
    // State independent data
    
    bool	myFirstInitialize;

    RE_OGLExt	*myExt;
    RE_Window	*currentWindow;
    RE_Window	*mySavedMainThreadCurrentWindow;
    short	 currentDisplayMode;
    
    RE_OGLContext  pushedGraphicsContext;
    
    RE_OGLContext  myContext;
    bool	   myContextInitialized;
    OGLDrawable	   myDrawable;
    OGLDrawable	   myPushedDrawable;
    
    UT_RecursiveSpinLock    myOpenGLLock;
    int			    myOpenGLContextLockCount;

    int		 myMaxTextureShaderUnits;
    int		 myMaxTextureVertexUnits;
    int		 myMaxTextureGeometryUnits;
    int		 myMaxTextureFragmentUnits;
    int		 myMaxTextureSize;
    int		 myMaxTexture3DSize;
    int		 myMaxTextureRectSize;
    int		 myMaxTextureAnisotropy;
    int		 myMaxVertexAttribs;
    int		 myMaxColorBuffers;
    int		 myMaxDrawBuffers;

    GLint	 maxClipPlanes;
    UT_Vector2F	 myLineWidthRange;

    bool	 myFrontBufferDirty;
    RE_RenderBuf myDrawBuffer[RE_BUFFER_STACK_SIZE];
    int		 myDrawBufferIndex;
    RE_RenderBuf myReadBuffer[RE_BUFFER_STACK_SIZE];
    int		 myReadBufferIndex;

    bool	 myMultisampleEnable;
    bool	 myMultisampleAlphaToCoverage;

    int		 currGLError;
    
    // Bound built-in uniform list for RE_Uniform.
    RE_Uniform		 *myBuiltInUniforms[RE_UNIFORM_BUILT_IN__count];
    RE_UniformStackEntry *myBuiltInStack[RE_UNIFORM_BUILT_IN__count];
    int			  myBuiltInStackIndex[RE_UNIFORM_BUILT_IN__count];

    UT_Array<RE_Uniform *>	 myPushedUniforms;

    int		   myViewportFontOffsetX, myViewportFontOffsetY;

    RE_OcclusionQuery		*myActiveQuery;

    RE_MaterialPtr	 myMaterialStack[RE_MAX_TEXTURE_STATE_STACK];
    bool		 myMaterialTextureStack[RE_MAX_TEXTURE_STATE_STACK];
    int			 myMaterialLayerStack[RE_MAX_TEXTURE_STATE_STACK];
    bool		 myMaterialLighting;
    int			 myMatStackIndex;

    bool		 myTransformFeedbackEnabled;
    bool		 myRasterizeEnabled;
    int			 myMaxTransformFeedbackBuffers;
    int			 myMaxTransformFeedbackComponents;
    RE_PrimType		 myTransformFeedbackType;
    UT_Array<RE_PrimitivesGeneratedQuery *> myTransformFeedbackPrimitiveQuery;
    UT_Array<RE_FeedbackPrimitivesWrittenQuery*> myTransformFeedbackWrittenQuery;
    int			 myVertexMaxStreams;
    int			 myActiveStreams;
    
    bool		 myActiveConditionalRender;
    bool		 mySampleShading;
    fpreal		 myShaderSampleRate;

    int			 myShaderOnlyModeCount;
    RE_Shader		*myShaderOnlyActiveShader;
    int			 myFragmentDerivativeHint;

    int			 myUniqueID;
    int64		 myBuiltInUniformSerial;

    int64		 myRenderCount;

    UT_IntArray		 myDebugStack;
    int			 myDebugGroupStackSize;
    int			 myMaxDebugGroupStackSize;
    bool		 myDebuggingSupported;
    bool		 myDebugLabelSupported;
    
    mutable UT_IntArray  myClipDistance;
    
    // Static data ----------------------------------------------------------
    static int		 majorGLVersion;  
    static int		 minorGLVersion;  
	   int		 majorGLSLVersion;  
	   int		 minorGLSLVersion;  

    static RE_Server	*theServer;
    static int	   theXRes;
    static int	   theYRes;
    static int	   theDotsPerInch;
    static fpreal  theUIScale;
    static bool    theSmoothLinesAllowed;

    static UT_Array<UT_Pair<RE_Visual *, RE_DisplayMode> > theVisuals;
    static RE_WindowList *theWindowList;
    static RE_Window 	*theMainContextWindow;
    
    // Card and driver info
    static RE_GraphicsDevice	theDevice;
    static int			theDriverMajor;
    static int			theDriverMinor;
    static int			theDriverBuildMajor;
    static int			theDriverBuildMinor;
    static bool			theUsingCoreProfile;

    // These objects are per-context.
    UT_IntArray	 myPendingDeleteQueries;
    UT_IntArray	 myPendingDeleteVertexArrays;
    UT_IntArray  myPendingDeleteFBOs;

    UT_ArrayMap<int,RE_OGLFramebuffer*> myFBOTable;

    class re_ConstVertexAttrib
    {
    public:
	re_ConstVertexAttrib()
	    : is_cached(false), type(RE_GPU_FLOAT32), vector_size(4)
	    { value[0] = 0.0; value[1] = 0.0; value[2] = 0.0; value[3] = 0.0; }
	bool		is_cached;
	RE_GPUType	type;
	int		vector_size;
	fpreal64	value[4];// This is just a scratch buffer for comparing
    };
    
    UT_Array<re_ConstVertexAttrib> myConstVertexAttribs;
    RE_OGLVertexArrayList	  *myCurrentVAO;
    RE_OGLVertexArrayList	  *myDefaultVAO;

    // Suspend check on Linux
    void		suspendTestPatternRender();
    bool	        suspendTestPatternCheck();
    
    UT_StopWatch       *mySuspendCheckTimer;
    RE_OGLFramebuffer  *mySuspendTestFBO;
    RE_OGLFramebuffer  *mySuspendResultFBO;
    RE_OGLTexture      *mySuspendTestTexture;
    RE_OGLTexture      *mySuspendResultTexture;
    RE_Geometry	       *mySuspendBox;

protected:
    static UT_ThreadSpecificValue <RE_OGLRender*> theCurrentRender;
    static UT_ThreadSpecificValue <RE_OGLRender*> theLastRender;

    // which thread this context was created in.
    ut_thread_id_t	myNativeThread;
    bool	 myIsAllowingOtherThreads;

    friend class RE_RenderFlush;
};

class RE_API RE_RenderAutoLock
{
public:
     RE_RenderAutoLock(RE_OGLRender *r)
	 : myR(r),
	   myLockedFlag(false)
     {
	 if(!myR->isContextLocked())
	 {
	     myR->lockContextForRender();
	     myLockedFlag = true;
	 }
     }
     
    ~RE_RenderAutoLock()
    {
	if(myLockedFlag)
	    myR->unlockContextAfterRender();
    }
    
private:
    RE_OGLRender *myR;
    bool	  myLockedFlag;
};


// Inlines ---------------------------------------------------------------

inline bool
RE_OGLRender::hasGL11() { return ((majorGLVersion > 1) ||
				  ((majorGLVersion >= 1) &&
				   (minorGLVersion >= 1))); };
inline bool
RE_OGLRender::hasGL12() { return ((majorGLVersion > 1) ||
				  ((majorGLVersion >= 1) &&
				   (minorGLVersion >= 2))); };
inline bool
RE_OGLRender::hasGL13() { return ((majorGLVersion > 1) ||
				  ((majorGLVersion >= 1) &&
				   (minorGLVersion >= 3))); };
inline bool
RE_OGLRender::hasGL14() { return ((majorGLVersion > 1) ||
				  ((majorGLVersion >= 1) &&
				   (minorGLVersion >= 4))); };
inline bool
RE_OGLRender::hasGL15() { return ((majorGLVersion > 1) ||
				  ((majorGLVersion >= 1) &&
				   (minorGLVersion >= 5))); };
inline bool
RE_OGLRender::hasGL20() { return ((majorGLVersion > 2) ||
				  ((majorGLVersion >= 2) &&
				   (minorGLVersion >= 0))); };
inline bool
RE_OGLRender::hasGL21() { return ((majorGLVersion > 2) ||
				  ((majorGLVersion >= 2) &&
				   (minorGLVersion >= 1))); };

inline bool
RE_OGLRender::hasGL3(int minor) { return ((majorGLVersion > 3) ||
					  ((majorGLVersion == 3) &&
					   (minorGLVersion >= minor))); };

inline bool
RE_OGLRender::hasGL4(int minor) { return ((majorGLVersion > 4) ||
					  ((majorGLVersion == 4) &&
					   (minorGLVersion >= minor))); };

inline int RE_OGLRender::glMajorVersion() { return majorGLVersion; }
inline int RE_OGLRender::glMinorVersion() { return minorGLVersion; }

inline int RE_OGLRender::glslMajorVersion() { return majorGLSLVersion; }
inline int RE_OGLRender::glslMinorVersion() { return minorGLSLVersion; }

inline RE_GraphicsDevice
RE_OGLRender::getGraphicsDevice() { return theDevice; }

inline bool RE_OGLRender::hasDriverVersion()
{
    return !(theDriverMajor == 0
	     && theDriverMinor == 0
	     && theDriverBuildMajor == 0
	     && theDriverBuildMinor == 0);
}
inline int RE_OGLRender::getDriverMajorVersion() { return theDriverMajor;}
inline int RE_OGLRender::getDriverMinorVersion() { return theDriverMinor;}
inline int RE_OGLRender::getDriverBuildMajorVersion() { return theDriverBuildMajor;}
inline int RE_OGLRender::getDriverBuildMinorVersion() { return theDriverBuildMinor;}

inline RE_OGLExt *RE_OGLRender::getExt() const	 { return myExt; }
inline bool RE_OGLRender::hasGLExtension(RE_Extension e) const
{
    UT_ASSERT_P( myExt );
    return myExt->hasExtension( e );
}

inline int RE_OGLRender::resX()		{ return theXRes; }
inline int RE_OGLRender::resY()		{ return theYRes; }
inline int RE_OGLRender::dpi()
{
    return SYSrint(theDotsPerInch * dpiScale() * uiScale());
}

inline fpreal RE_OGLRender::uiScale()
{
    return theUIScale;
}

inline float
RE_OGLRender::pixelsToInches(int n)
{
    return (float)n / dpi();
}

inline int
RE_OGLRender::inchesToPixels(float i)
{
    return (int)SYSrint(i*(float)dpi());
}

inline int
RE_OGLRender::scaledSize(int size)
{
    return (int)scaledSize((fpreal)size);
}

inline fpreal
RE_OGLRender::scaledSize(fpreal size)
{
#if defined(MBSD)
    // Native Qt widgets are automatically scaled by the OS using the display's
    // backing scale factor.  Scaling again will make native Qt UI look
    // comically out-of-place.
    return size;
#else
    if (size == 0)
	return size;

    fpreal dpi_scale = dpiScale();
    if (dpi_scale == 1.0)
	return size;

    fpreal scaled_size = dpi_scale * size + 0.5;
    if (scaled_size < 1.0)
	return 1.0;

    return scaled_size;
#endif
}

inline RE_Window *
RE_OGLRender::getMainContext() { return theMainContextWindow; }

inline RE_OGLRender *
RE_OGLRender::getCurrentRender() { return theCurrentRender.get(); }

inline RE_WindowList *
RE_OGLRender::getWindowList()	{ return theWindowList; }

inline const RE_Window *
RE_OGLRender::getCurrentWindow() const { return currentWindow; }

inline RE_Window *
RE_OGLRender::getCurrentWindow()	  { return currentWindow; }

inline bool
RE_OGLRender::isFrontBufferDirty() const
{ return myFrontBufferDirty;}

inline void
RE_OGLRender::setFrontBufferDirty(bool d)
{ myFrontBufferDirty = d; }

inline int
RE_OGLRender::getOGLMaxClipPlanes() { return maxClipPlanes; }

inline bool
RE_OGLRender::isPointOffset() { return getPointOffset(); }

inline bool
RE_OGLRender::isLineOffset() { return getLineOffset();  }

inline bool
RE_OGLRender::isPolygonOffset() { return getFillOffset(); }

inline void
RE_OGLRender::pointOffset(bool onoff)
{
    if( onoff )
	enablePointOffset();
    else
	disablePointOffset();
}

inline void
RE_OGLRender::lineOffset(bool onoff)
{
    if( onoff )
	enableLineOffset();
    else
	disableLineOffset();
}

inline void
RE_OGLRender::polygonOffset( bool onoff )
{
    if( onoff )
	enableFillOffset();
    else
	disableFillOffset();
}

inline void
RE_OGLRender::getOffsetAmount(float *variable, float *constant) const
{
    *variable = myState._offset_variable;
    *constant = myState._offset_constant;
}

inline bool
RE_OGLRender::isBlending() const
{
    return myState.myBlendSmoothStack[myState.myBlendSmoothLevel].myBlend==1;
}
inline int
RE_OGLRender::getBlendSmoothLevel() const
{
    return myState.myBlendSmoothLevel;
}

inline void RE_OGLRender::allowSmoothLines(int yn) { theSmoothLinesAllowed=yn; }
inline int  RE_OGLRender::allowsSmoothLines() { return theSmoothLinesAllowed; }

inline int	 
RE_OGLRender::getViewportFontOffsetX() const
{ return myViewportFontOffsetX; }

inline int	 
RE_OGLRender::getViewportFontOffsetY() const
{ return myViewportFontOffsetY; }


inline bool
RE_OGLRender::isTransformFeedbackEnabled() const
{
    return myTransformFeedbackEnabled;
}

inline RE_PrimType
RE_OGLRender::getFeedbackMode() const
{
    return myTransformFeedbackType;
}

inline bool
RE_OGLRender::isRasterizeEnabled() const
{
    return myRasterizeEnabled;
}

inline RE_Uniform *
RE_OGLRender::getUniform(RE_UniformBuiltIn builtin_var_type) const
{
    return myBuiltInUniforms[builtin_var_type];
}

inline bool
RE_OGLRender::isDebugLabelSupported() const
{
    return myDebugLabelSupported;
}

inline bool
RE_OGLRender::isDebugging() const
{
    return myDebuggingSupported;
}

#endif
