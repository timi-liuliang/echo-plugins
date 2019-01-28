/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_OGLBuffer.h ( RE Library, C++)
 *
 * COMMENTS:
 *   Buffer objects are merely registered chunks of memory that is
 *   managed by the graphics driver. Although the come in different
 *   flavours, they are all created and accessed in the same way.
 *
 *   One buffer object represents one type of data - texture coords,
 *   point positions, pixels or colors. There are two main types in terms
 *   of usage, vertex and pixel buffers.
 *
 *   1. Vertex-oriented buffers are used in lieu of glVertex, glNormal,
 *	glColor, etc.. They are much more efficient in that you can send
 *      a lot of data with one call, and you can update parts of the
 *	list instead of rewriting the entire list. They are used in
 *	conjunction with glDrawArrays() to create geometry. As an example:
 *
 *	   RE_OGLAttributeBuffer vBuf("P", nVertices);
 *	   RE_OGLAttributeBuffer texBuf("uv", nVertices);
 *
 *	   vBuf.setData(vertex_list);
 *	   texBuf.setData(tex_coord_list); // you can free the lists after this
 *
 *	   vBuf.enable();
 *	   texBuf.enable();
 *
 *	   r->getExt()->glDrawArrays(GL_TRIANGLES, 0, nVertices);
 *
 *	   vBuf.disable();
 *	   texBuf.disable();
 *
 *	The data is always assumed to be interleaved (xyzxyz...)
 *
 *   2. Pixel buffers are used to write and rewrite pixel data to a GL structure
 *	(normally a texture). In this case, there use is slighly different.
 *	Instead of passing the image data directly to glTexImage...(), you
 *      pass NULL and instead enable the pixel buffer first. Ex:
 *
 *	   RE_OGLPixelWriteBuffer pBuf(xres, yres);
 *
 *	   pBuf.setFormat(RE_GPU_UINT8, 4);
 *	   pBuf.setData( image_data );
 *	   pBuf.enable();
 *	   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, xres, yres, 0,
 *			GL_UNSIGNED_BYTE, GL_RGBA, NULL);
 *	   pBuf.disable();
 *
 *	After that, you can change the texture (or a portion of the texture)
 *	by calling setData() again.
 *
 *	You can also use a pixel buffer as a target for reading data back from
 *	the GPU (via glReadPixels(), glGetTexImage(), etc).
 *
 *	   RE_OGLPixelReadBuffer pBuf(xres, yres);
 *
 *	   pBuf.enable();
 *	   glReadPixels(0,0, xres, yres, GL_RGB, GL_UNSIGNED_BYTE, NULL);
 *	   pBuf.disable();
 *
 *	   ptr = pBug.getData();
 *
 *	This allows you to do asynchronous reads. The glReadPixels() starts the
 *	DMA transfer, but does not wait - the getData() does.
 *
 *   Finally, the various types of buffer objects, and their default settings,
 *   are:
 *
 *   Vertex:
 *	RE_OGLAttributeBuffer	 shader vertex attrib	3 fp32
 *
 *   Pixel:
 *	RE_OGLPixelReadBuffer    read pixels from GPU	4 uint8
 *	RE_OGLPixelWriteBuffer   write pixels to GPU	4 uint8
 *
 *   Other:
 *	RE_OGLUniformBuffer	 uniforms for shaders	1 uint8
 *	RE_OGLElementBuffer	 primitive connectivity	1 uint32
 *	RE_OGLTexBuffer		 texture buffer object  1 fp32
 */
#ifndef RE_OGLBuffer_H
#define RE_OGLBuffer_H

#include "RE_CachedObject.h"
#include "RE_Types.h"
#include "RE_OGL.h"
#include <SYS/SYS_Types.h>
#include <UT/UT_String.h>
#include <UT/UT_NonCopyable.h>
#include <UT/UT_SharedPtr.h>

class RE_Render;
class RE_OGLBuffer;
class RE_OGLTexture;

class RE_API RE_OGLBuffer : public RE_CachedObjectBase
{
public:
    // Alternatively, you can use the convenience classes listed below.

    RE_OGLBuffer(RE_BufferType type, int num_elements, int capacity = -1);

    // Create a new RE_OGLBuffer derived class of 'type'. 
    // 'name' is the requied name for an attribute array, ignored for other
    // types. 'cap' is the optional, actual size of the buffer.
    static RE_OGLBuffer *newBuffer(RE_BufferType type,
				   int num_elements,
				   const char *name = nullptr,
				   int capacity = -1);

    // Setup
    // standard GL attributes use the buffer types
    void	setBufferType(RE_BufferType type);
    
    // custom attributes are referenced by name.
    void	setAttributeName(const char *attrib);
    
    // Sets the data type for the array. This type is one array 'element'. 
    bool	setFormat(RE_GPUType type, int vectorsize);

    // If an integer format is chosen (byte, short, int; unsigned or signed)
    // and hooked up to a floating point vertex shader attribute, this specifies
    // how the conversion is done. If normalize is true, unsigned ints are
    // converted to [0,1] FP and signed ints to [-1, 1] FP. If normalize is
    // false, the integer is converted as-is to FP (not remapped).
    // This has no effect on FP formats, or integer arrays connected to integer
    // vertex attributes. This needs to be called before the array is bound.
    // The default is unnormalized.
    void	setNormalizedFormat(bool normalize = true);

    // Sets the length of the array, in elements (a fp32 vec3 is 1 element)
    // Returns true if the length changed, false if the array was already len.
    bool	setLength(int len);

    // Sets the capacity of the array, in elements (a fp32 vec3 is 1 element)
    // Returns true if the size changed, false if the array was already that
    // size. If capacity is smaller than the current length, it will also be set
    // to that size.
    bool	setCapacity(int size);

    // Increment this attribute once every 'nprim' primitives, rather than once
    // per vertex. RE_EXT_INSTANCE_ARRAYS is required.
    void	setInstanceStride(int nprim);

    // You can optionally give GL a hint how you'll be using the data.
    // Hints are set reasonably for the convenience classes below. If called
    // after setData(), this has no effect.
    void	setUsage(RE_BufferUsageHint hint);

    // Use persistent buffers, if RE_EXT_BUFFER_STORAGE is supported.
    // Normal persistent buffers require the use of RE_Render::memoryBarrier()
    // with RE_BARRIER_PERSISTENT_BUFFER. Coherent buffers are automatically
    // updated before a draw call
    void	setPersistentBuffer(RE_PersistentBufferMode mode);

    // This index is for binding matrix arrays, which are composed of multiple
    // vertex arrays (a 4x4 uses 4 vertex array bindings). This index indicates
    // which part of the matrix this vertex array maps to.
    void	setSubComponentIndex(int index);

    // Requires RE_EXT_DEBUG_LABEL extension (GL_ARB_debug_label). Sets a
    // debug label on this array for GL debugging. Only applicable to buffers.
    void	setLabel(RE_Render *, const char *);
    
    // Array data methods
    
    // Specifies all the data at once, or a subrange of the data if sublen!=0.
    // offset and sublen are specified in elements, not bytes.
    bool	setData(RE_Render *r, const void *data,
			int offset = 0, int sublen = 0);

    bool	 replaceData(RE_Render *r, const void *subdata,
			    int suboffset, int sublength)
		    { return setData(r, subdata, suboffset, sublength); }

    // fetch data from the buffer into 'data', which should be large enough to
    // hold it.
    bool	getData(RE_Render *, void *data,
			int offset = 0, int sublen = 0) const;

    // Cut the current buffer loose and hand it over to the driver, so that
    // it's possible to start writing to a new buffer immediately. Has no effect
    // if no buffer was previously allocated.
    void	 orphan(RE_Render *r);

    // Enable or disable this buffer, binding it to a bind point that depends
    // on its derived type.
    virtual bool enable(RE_Render *r, unsigned int stride = 0,
			bool geo_object = false);
    virtual bool disable(RE_Render *r);

    bool	rebindAttrib(RE_Render *r,
			     int location,    // attrib location
			     int stride = -1, // use previous if -1
			     RE_GPUType *type = NULL);
    bool	unbindAttrib(RE_Render *r);

    // Allows binding of this buffer to any buffer binding point (enable() only
    // binds the buffer to the specific binding point for its type)
    bool	bindBuffer(RE_Render *r,
				   RE_BufferBinding point,
				   int bind_index = 0);
    bool	unbindBuffer(RE_Render *r,
				     RE_BufferBinding point,
				     int bind_index = 0);
    
    // Drawing methods

    // Draws primitives using every n'th point (where n=stride) in this array 
    // as 'prim' primitive type.
    virtual void draw(RE_Render *r, 
		      RE_PrimType prim, 
		      unsigned int stride,
		      int num_instanced,
		      int vertices_per_patch);
    // Draws primitives using a subrange of points in this array
    virtual void drawRange(RE_Render *r,
			   RE_PrimType prim,
			   int start,int num,
			   int num_instanced,
			   int vertices_per_patch);
    // Draws primitives using some points in this array using a 2nd array as
    // the indices. The array must be an element array.
    virtual void drawElements(RE_Render *r,
			      RE_PrimType prim,
			      RE_OGLBuffer *index_array,
			      int offset, int num,
			      int num_instanced,
			      int vertices_per_patch);

    //  If you want to write or read directly from the memory chunk,
    //  this returns a pointer to the data. You must upmap it when you're done.
    //    Do NOT delete this pointer - it is owned by the graphics driver.
    //  mapDataRange() maps a subrange. offset and length are in element units.
    void	       *map(RE_Render *r,
			    RE_BufferAccess access = RE_BUFFER_WRITE_ONLY);
    void	       *mapRange(RE_Render *r, int offset, int length,
				 RE_BufferAccess access = RE_BUFFER_WRITE_ONLY);
    void		unmap(RE_Render *r);

    // For persistent mapped buffers, this returns the currently mapped data.
    // Will only return a value if mapped, persisent is set, and
    // RE_EXT_BUFFER_STORAGE is supported.
    void		*getPersistentMap() const
			    { return myMappedPersistentData; }

    /// Graphics memory usage estimate
    int64		getSizeBytes() const;

    /// Returns the amount of main memory owned by this RE_OGLBuffer.
    virtual int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
	mem += myAttribName.getMemoryUsage(false);
        return mem;
    }
    
    // Query
    RE_BufferType	getBufferType() const	 { return myBufferType; }
    RE_GPUType		getDataType() const	 { return myType; }
    int			getVectorSize() const	 { return myVectorSize; }
    int			getSubComponentIndex() const { return mySubIndex; }
    int			getLength() const	 { return myLength; }
    int			getCapacity() const	 { return myCapacity; }
    int			getInstanceStride() const{ return myInstanceStride; }
    const char	       *getAttributeName() const { return myAttribName; }
    bool		isBound() const		 { return myBoundFlag; }
    bool		isMapped() const	 { return myMappedFlag; }
    bool		isEnabled() const	 { return myEnableFlag; }
    RE_BufferAccess	getMapAccess() const	 { return myMappedAccess; }

    // Return the GL buffer ID for the underlying GL buffer.
    int			getID() const		 { return myBufferID; }

    // Return a unique ID for this buffer instance (not the GL identifier)
    int			getUniqueID() const	{ return myUniqueID; }

    // Returns the GL maximum size of a vertex array and an element array when
    // using glDrawRangeElements().
    static int	getMaxVertexArraySize(RE_Render *r);
    static int	getMaxElementArraySize(RE_Render *r);

    // Treat as protected for RE_VertexArray.
    bool	setEnabled(bool enabled)
			{ bool e=myEnableFlag; myEnableFlag=enabled; return e; }

    // For caching.
    void	incref()	{ myRefCount++; }
    void	decref()
		{
		    myRefCount--;
		    if (myRefCount== 0)
			delete this;
		}
    int		getRefCount() const { return myRefCount; }

    void	   setTBO(RE_OGLTexture *tbo);
    RE_OGLTexture *getTBO() const { return myTBO; }
    
    // Cached only; not actively in use if 0.
    void	markAsUsed()   { myUseCount ++; }
    void	markAsUnused() { myUseCount --; UT_ASSERT(myUseCount>=0); }
    bool	isUsed() const { return myUseCount>0; }
    int		getUseCount() const { return myUseCount; }
    
protected:
    bool	initialize(RE_Render *r, const void *data, int length = 0);
    void	resetBuffer();
    void	drawArraysCommon(RE_Render *r, 
				 RE_PrimType prim,
				 int start,
				 int num,
				 unsigned int stride,
				 int num_instances,
				 int vertices_per_patch);
    
    bool	bindAttribute(RE_Render *r, int location,
			      RE_GPUType type, int stride,
			      const void *data);
    
    bool	enableAttribArray(RE_Render *r,
				  const char *name,
				  RE_GPUType datatype, int vectorsize,
				  unsigned int stride,
				  const void *data);
    void	disableAttribArray(RE_Render *r, const char *name);

    virtual ~RE_OGLBuffer();

    UT_String		myAttribName;
    RE_BufferType	myBufferType;
    RE_GPUType		myType;
    int			myVectorSize;
    int			mySubIndex;
    int			myNormalized;
    int			myLength;
    int			myCapacity;
    int			myInstanceStride;
    int			myLastStride;
    int			myAttribLocation;
    RE_BufferUsageHint  myUsage;
    RE_PersistentBufferMode myPersistentBufferMode;
    RE_BufferAccess	myMappedAccess;
    
    bool		myMappedFlag;
    bool		myBoundFlag;
    bool		myEnableFlag;
    
    GLenum		myDataType;
    int			myDataSize;
    GLuint		myBufferID;
    GLenum		myGLBufferType;
    
    void	       *myMappedDataEnd;
    void	       *myMappedPersistentData;
    RE_OGLTexture      *myTBO;
    int64		myRefCount;
    int			myUseCount;

    int			myUniqueID;
};

static inline void intrusive_ptr_add_ref(RE_OGLBuffer *b) { b->incref(); }
static inline void intrusive_ptr_release(RE_OGLBuffer *b) { b->decref(); }

class RE_OGLVertexBuffer : public RE_OGLBuffer
{
public:
    RE_OGLVertexBuffer(RE_BufferType type, int num_elements, int capacity=-1);

    virtual bool	enable(RE_Render *r, unsigned int stride = 0,
			       bool geo_object = false);
    virtual bool	disable(RE_Render *r);
    
    // This is a convenience method that will draw the object with all
    // currently enabled buffer objects (you only need to call this on one of
    // the buffers, if multiple buffers are enabled). 'gltype' is the GL
    // primitive you want to draw (GL_LINES, GL_TRIANGLE_STRIP, GL_QUADS, etc).
    // The stride will all you to draw using every n'th element in the buffer.
    virtual void	draw(RE_Render *r, RE_PrimType prim_type, 
			     unsigned int stride,
			     int num_instanced,
			     int vertices_per_patch);

    // This is a convenience method that will draw objects using a portion of
    // the vertices in the buffer.
    virtual void	drawRange(RE_Render *r, RE_PrimType prim_type,
				  int start, int num,
				  int num_instanced,
				  int vertices_per_patch);
    
    // same as above, but uses an element buffer to store the indices. If
    // num is not zero, offset/num define a subrange of the buffer to render.
    virtual void	drawElements(RE_Render *r,
				     RE_PrimType prim_type,
				     RE_OGLBuffer *element_bufer,
				     int offset, int num,
				     int num_instanced,
				     int vertices_per_patch);

    /// Returns the amount of main memory (NOT graphics memory!)
    /// owned by this RE_OGLVertexBuffer.
    virtual int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += RE_OGLBuffer::getMemoryUsage(false);
        return mem;
    }

protected:
    virtual ~RE_OGLVertexBuffer();
    
    bool		myBoundByGeoObject;
};

class RE_OGLAttributeBuffer : public RE_OGLVertexBuffer
{
public:
    RE_OGLAttributeBuffer(const char *name, int num = 0, int cap = -1)
	: RE_OGLVertexBuffer(RE_BUFFER_ATTRIBUTE, num, cap)
    {
	setFormat(RE_GPU_FLOAT32, 3);
	setAttributeName(name);
    }
};


// Non-vertex buffer objects ----------------------------------------------

class RE_API RE_OGLElementBuffer : public RE_OGLBuffer
{
public:
    RE_OGLElementBuffer(int num = 0, int cap = -1)
	: RE_OGLBuffer(RE_BUFFER_ELEMENT, num, cap)
    {
	setFormat(RE_GPU_UINT32, 1);
    }
};

class RE_API RE_OGLUniformBuffer : public RE_OGLBuffer
{
public:
    RE_OGLUniformBuffer(int size)
	: RE_OGLBuffer(RE_BUFFER_UNIFORM, size)
    {
	setFormat(RE_GPU_UINT8, 1);
    }
    virtual ~RE_OGLUniformBuffer();
};

class RE_API RE_OGLTexBuffer : public RE_OGLBuffer
{
public:
    RE_OGLTexBuffer(int size, int cap =-1)
	: RE_OGLBuffer(RE_BUFFER_TEXTURE, size, cap)
    {
	setFormat(RE_GPU_FLOAT32, 1);
    }
};


// A pixel read buffer is used to read data back from the GPU.
// Data type defaults to 3 8bit ints.
class RE_API RE_OGLPixelReadBuffer : public RE_OGLBuffer
{
public:
    RE_OGLPixelReadBuffer(int xres = 0, int yres = 1)
	: RE_OGLBuffer(RE_BUFFER_PIXEL_READ, xres * yres)
    {
	setFormat(RE_GPU_UINT8, 4);
	setUsage(RE_BUFFER_READ_FREQUENT);
    }
};

// A pixel write buffer is used to write data to the GPU.
// Data type defaults to 3 8bit ints.
class RE_API RE_OGLPixelWriteBuffer : public RE_OGLBuffer
{
public:
    RE_OGLPixelWriteBuffer(int xres = 0, int yres = 1)
	: RE_OGLBuffer(RE_BUFFER_PIXEL_WRITE, xres * yres)
    {
	setFormat(RE_GPU_UINT8, 4);
	setUsage(RE_BUFFER_WRITE_FREQUENT);
    }
};

// This pixel buffer is used for copying data between objects on the GPU,
// without returning the data across the bus to the CPU.
class RE_API RE_OGLCopyPixelBuffer : public RE_OGLBuffer
{
public:
    RE_OGLCopyPixelBuffer(int xres = 0, int yres = 1)
	: RE_OGLBuffer(RE_BUFFER_PIXEL_WRITE, xres*yres)
    {
	setFormat(RE_GPU_UINT8, 4);
	setUsage(RE_BUFFER_READ_WRITE_FREQUENT);
    }

    // Enables the buffer for either reading or writing - note the slightly
    // different sematic for the last parameter to indicate this.
    virtual bool enable(RE_Render *r, unsigned int = 0,
			bool bind_for_gpu_read = true);
};
#endif

