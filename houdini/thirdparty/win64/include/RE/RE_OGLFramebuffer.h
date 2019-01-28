 /*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_OGLFramebuffer.h ( RE Library, C++)
 *
 * COMMENTS:
 *
 *   This class uses the Framebuffer object extension to do offscreen
 *   rendering to render buffers & textures. Note that rendering to texture
 *   is not supported on all platforms, so please check that the framebuffer
 *   is valid before rendering (either isValid(), or the return value of
 *   begin).
 *
 *   You can attach many buffers, including GL_DEPTH_ATTACHMENT_EXT,
 *   GL_STENCIL_ATTACHMENT_EXT, and up to GL_MAX_COLOR_ATTACHMENTS color
 *   attachments (sequential from GL_COLOR_ATTACHMENT0_EXT). You must include
 *   a render buffer for depth or stencil if you intend to use depth or
 *   stencil tests.
 *
 *   For color, you can render to either a texture or a renderbuffer.
 *   Generally, textures are used for passes, and renderbuffers are used to
 *   pull images from the GPU.
 *
 *   Deleting the class will clean up the frame buffer object and any
 *   renderbuffers still attached, but textures will not be deleted.
 */
#ifndef RE_OGLFramebuffer_h
#define RE_OGLFramebuffer_h

#include "RE_Render.h"

#include <stdio.h>
#include <UT/UT_ValArray.h>
#include "RE_ContextSpecificObject.h"
#include "RE_Texture.h"

class re_FrameBufferAttachment;
class RE_OGLRenderbuffer;

enum RE_BufferAttachment
{
    RE_COLOR_BUFFER,
    RE_DEPTH_BUFFER,
    RE_DEPTH_STENCIL_BUFFER	// a depth-stencil buffer instead.
};

// The status of the framebuffer. Some implementations can support more varied
// FBO configs (different formats, res, color targets etc), which others will
// throw errors.
enum RE_FBOStatus
{
    RE_FBO_STATUS_OK =0,

    RE_FBO_STATUS_ATTACHMENT_INCOMPLETE,// Not all attachments are FBO-complete
    RE_FBO_STATUS_MISSING_ATTACHMENT,	// A required attachment is missing
    RE_FBO_STATUS_FORMAT_MISMATCH,	// formats do not match
    RE_FBO_STATUS_SIZE_MISMATCH,	// resolutions do not match
    RE_FBO_STATUS_UNSUPPORTED,		// GL impl. doesn't support config
    RE_FBO_STATUS_MULTISAMPLE_MISMATCH,	// #buffer samples do not match
    
    RE_FBO_STATUS_DRAW_MISSING,		// Attempt to draw to missing attach
    RE_FBO_STATUS_READ_MISSING,		// Attempt to read from missing attach

    RE_FBO_STATUS_LAYERS_MISMATCH,	// GeoSh: attachments differ in #layers
    RE_FBO_STATUS_LAYERS_COUNT,		// Too many layers
    
    RE_FBO_STATUS_UNKNOWN_ERROR		// Future-proof against upcoming errors
};


class RE_API RE_OGLFramebuffer : public RE_ContextSpecificObject
{
public:
	     RE_OGLFramebuffer(const char *name = NULL);
	    ~RE_OGLFramebuffer();

    virtual bool init(RE_Render *r);

    const char  *getName() { return myName.isstring()?(const char *)myName:""; }

    /// Returns true if the framebuffer is w x h, with #samples = samples
    bool	 matches(int width, int height, int samples = 1) const;
    
    /// Attach a texture to the framebuffer. You are still responsible for
    /// deleting it. 'Level' is the mip level for 2D textures, the cube face
    /// (RE_TextureCubeFace) for cube maps, and the Z-coord for 3D textures.
    bool	 attachTexture(RE_Render *r, RE_Texture *t,
			int level = -1,
			RE_BufferAttachment buffer_type = RE_COLOR_BUFFER,
			int colorbufnum = 0);

    /// Create a 2D texture and attach it to the framebuffer. You are still
    /// responsible for deleting it.  Specify rect = true to create a
    /// texture rectangle (check to see that this extension
    /// is supported before calling this method). Samples > 0 requires the
    /// texture-multisample extension, and these can only be accessed via
    /// shaders.
    RE_Texture  *createTexture(RE_Render *r, 
			RE_GPUType data_type, int vectorsize,
			int miplevel = -1,
			RE_BufferAttachment buffer_type = RE_COLOR_BUFFER,
			int colorbufnum = 0,
			bool rect = false,
			int samples = 0);

    /// Create a renderbuffer and attach it to the framebuffer. This will
    /// be cleaned up when you delete the render buffer, or call
    /// clearAttachment() on it. If 'samples' > 1, this is a multisample (AA)
    /// Framebuffer, which must be blit'ed to a single-sample FBO before being
    /// read.
    RE_OGLRenderbuffer *createRenderbuffer(RE_Render *r, 
			RE_GPUType data_type, int vectorsize,
			RE_BufferAttachment buffer_type = RE_COLOR_BUFFER,
			int colorbufnum = 0,
			int samples=0);
    bool	 attachRenderbuffer(RE_Render *r,
			RE_OGLRenderbuffer *rbuf,
			RE_BufferAttachment buffer_type = RE_COLOR_BUFFER,
			int colorbufnum = 0);

    bool	 hasAttachment(RE_Render *r,
			RE_BufferAttachment buffer_type,
			int colorbufnum = 0);
    RE_OGLRenderbuffer *getBufferAttachment(RE_Render *r,
			RE_BufferAttachment buffer_type,
			int colorbufnum = 0);
    RE_Texture	*getTextureAttachment(RE_Render *r,
			RE_BufferAttachment buffer_type,
			int colorbufnum = 0);

    /// Pushes an attachment, setting the specified buffer to NULL. Calls to
    /// pop attachments will always pop to the original location, replacing
    /// anything that has been put there in the mean time.
    void	 pushAttachment(RE_Render *r,
			RE_BufferAttachment buffer_type,
			int colorbufnum = 0);
    void	 popAttachment(RE_Render *r);

    /// Returns true if an attachment if framebuffer-complete.
    bool	 verifyBufferAttachment(RE_Render *r,
			RE_BufferAttachment buffer_type,
			int colorbufnum = 0,
			RE_FBOStatus *status = NULL);

    /// You must call this if attaching textures (if you are creating textures,
    /// the resulotion of the textures you pass is used). 
    void	 setResolution(int width, int height);
    int		 getWidth() const { return myWidth; }
    int		 getHeight() const { return myHeight; }

    void	 setSamples(int samples);
    int		 getSamples() const { return mySamples; }

    /// Sets the origin for the lower-left corner. Normally 0,0.
    void	 setOrigin(fpreal x, fpreal y);
    void	 getOrigin(fpreal &x, fpreal &y) const;
    
    /// Returns true if the framebuffer is ready to render. Optionally returns
    /// a more detailed failure description if 'status' is provided.
    bool	 isValid(RE_Render *r, RE_FBOStatus *status = NULL);

    /// Returns the error string for the given status.
    static const char *getFBOErrorMessage(RE_FBOStatus status);

    /// Returns true if the texture is attached to this framebuffer.
    bool	 isAttached(RE_Render *r, RE_Texture *tex);

    /// Binds the framebuffer as the active draw or read FBO
    bool	 bind(RE_Render *r, bool for_draw = true);

    /// Unbind the current framebuffer. 'switch_only' indicates that you are
    /// immediately changing to another FBO.
    void	 unbind(RE_Render *r,
			bool switch_only = false,
			bool restore_ms = true);

    /// do all rendering between the begin/end statements. Renders to the
    /// given attachment. Specify initview = true set the modelview matrix
    /// to an orthographic projection the size of the framebuffer, the
    /// viewport to the size of the framebuffer, and to push the current
    /// GL state (via RE_OGLRender::pushState(1)).
    bool	 begin(RE_Render *r, RE_BufferAttachment buf,
		       int colorbufnum = 0, bool initview = true);
    bool	 begin(RE_Render *r); // use last buffer setting, no view init
    void         end(RE_Render *r);

    bool	 beginRead(RE_Render *r,
			RE_BufferAttachment buffer_type = RE_COLOR_BUFFER,
			int colorbufnum = 0);

    /// Returns true if the framebuffer is being drawn to.
    bool	 isActiveDraw() { return myBegun && !myReadFlag; }

    void	 interruptDraw(RE_Render *r) { unbindFrameBuffer(r); }
    void	 resumeDraw(RE_Render *r)   { bindFrameBuffer(r); }

    void	 drawToBuffer(RE_Render *r,
			RE_BufferAttachment buffer_type = RE_COLOR_BUFFER,
			int colorbufnum = 0);
    void	 readFromBuffer(RE_Render *r,
			RE_BufferAttachment buffer_type = RE_COLOR_BUFFER,
			int colorbufnum = 0);

    /// allows you to draw to multiple buffers at once.
    /// The type RE_COLOR_BUFFER is assumed. There is a limit on the
    /// number you can write to simultaneously, getMaxDrawBuffers().
    void	 drawToBuffers(RE_Render *r, int num, const int *bufferlist);

    /// Draws to all color buffers up to getMaxDrawBuffers(). The Z/Stencil
    /// buffers are written as well if depth or stencil writing is enabled.
    void	 drawToAllBuffers(RE_Render *r);

    /// Initialize the GL state with the various settings in this FBO:
    /// project_mat - set the projection matrix
    /// scissor - set the scissor to the resolution of the view
    /// clear - clear the contents
    /// This always sets the viewport to the size of the FBO.
    void	 initDraw(RE_Render *r,
			  bool project_mat,
			  bool scissor, 
			  bool clear) const;

    // This method allows you to begin an ascyncronous read from a series of
    // buffers. Note that it will only be asyncronous if the
    // RE_PIXEL_BUFFER_OBJECT extension is supported. 
    bool	readAsyncFromBuffers(RE_Render *r, int num, int *bufferlist);

    /// reads the data from the render buffer specified into 'buffer'.
    /// 'buffer' must be large enough to contain the full renderbuffer size.
    bool	readFromRenderBuffer(RE_Render *r, void *image,
			RE_BufferAttachment buffer_type = RE_COLOR_BUFFER,
			int colorbufnum = 0);

    /// Copies the framebuffer 'from_fbo' to this framebuffer. This is often
    /// used for resolving multisample framebuffers into an image.
    /// if destbufnum == -1, colorbufnum is used as the dest buffer as well.
    bool	blitFromFramebuffer(RE_Render *r,
			RE_OGLFramebuffer *from_fbo,
			RE_BufferAttachment buffer_type = RE_COLOR_BUFFER,
			int colorbufnum = 0,
			int destbufnum = -1,
			const UT_DimRect *from_area = NULL);

    /// Removes the attachment, and frees the renderbuffer or texture if
    /// freeme is true.
    bool	detach(RE_Render *r,
			RE_BufferAttachment buffer_type = RE_COLOR_BUFFER,
			int colorbufnum = 0,
			bool freeme = false);
		       
    bool	detachAndFree(RE_Render *r,
			RE_BufferAttachment buffer_type = RE_COLOR_BUFFER,
			int colorbufnum = 0)
		{ return detach( r, buffer_type, colorbufnum ,true); }

    enum RE_BufferBindMode 
    {
      RE_BIND_NONE = 0, // Cannot be used as a parameter to bindFrameBuffer()
      RE_BIND_READ = 1,
      RE_BIND_DRAW = 2,
    };

    /// Mostly for debug, this writes out the given attachment to an image file.
    bool	writeToFile(RE_Render *r, const char *filename,
			    RE_BufferAttachment buf, int colorbufnum = 0);

    void	dumpConfig(RE_Render *r, FILE *);
    // debug check.
    static bool	verifyBinding(RE_Render *r, RE_OGLFramebuffer *fbo,
			      RE_BufferBindMode bindmode = RE_BIND_DRAW);

    /// Raw GL ID of the FBO
    GLuint	getID() const { return myFramebuffer; }

private:
    bool	initImpl(RE_Render *r, bool generate);
    
    bool	privateReadFromBuffer(RE_Render *r,
				      re_FrameBufferAttachment *attach,
				      RE_RenderBuf buf,
				      void *image_data);

    bool	hasColorBuffer() const;
    bool	hasDepthBuffer() const;

    bool	bindFrameBuffer(RE_Render *r, 
				RE_BufferBindMode bindmode = RE_BIND_DRAW);
    void	unbindFrameBuffer(RE_Render *r);
    void	restoreDrawBuffers(RE_Render *r, int mask);

    unsigned    getGLBuffer() const;
    RE_FBOStatus getREStatusFromGL(int glstatus);
    
    GLuint					 myFramebuffer;
    fpreal32					 myX;
    fpreal32					 myY;
    int						 myWidth;
    int						 myHeight;
    bool					 myBegun;
    bool					 myRestoreView;
    RE_BufferBindMode				 myOpenMode;
    UT_Array<re_FrameBufferAttachment *>	 myAttachments;
    UT_Array<re_FrameBufferAttachment *>	 myPushedAttachments;
    int						 mySamples;
    bool					 myReadFlag;
    UT_String					 myName;
    int						 myDrawBuffersMask;
    int						 myReadBuffer;
    bool					 myPrevMultisample;
    bool					 mySavedMultisample;

    // State values: 0=invalid, 1=valid, -1=not known
    int						 myValidState;
    RE_FBOStatus				 myLastStatus;
};

// Renderbuffers are buffers that can be attached to framebuffers instead of
// textures.
class RE_API RE_OGLRenderbuffer
{
public:
    RE_OGLRenderbuffer();
    ~RE_OGLRenderbuffer();

    // These can be changed on an existing renderbuffer if not attached to a
    // framebuffer.
    void	setBufferType(RE_BufferAttachment buffer_type);
    void	setFormat(RE_GPUType data_type, int vectorsize);
    void	setResolution(int w, int h);
    void	setSamples(int samples);

    // Creates the actual GL renderbuffer.
    bool	create(RE_Render *r);
    void	destroy();

    // Raw GL renderbuffer ID.
    GLuint	getID() const
		{ return myID; }
    RE_GPUType	getFormatType() const
		{ return myFormatType; }
    
private:
    GLuint		myID;
    int			myW;
    int			myH;
    RE_GPUType		myFormatType;
    int			myVectorSize;
    int			mySamples;
    RE_BufferAttachment	myBufferType;
    bool		myAttachedFlag;
    
    friend class RE_OGLFramebuffer;
};

#endif
