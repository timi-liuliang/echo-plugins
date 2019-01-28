/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_VertexArray.h ( RE Library, C++)
 *
 * COMMENTS:
 *	These classes provide a GL-indepedent way of specify vertex attributes
 *	in array or constant form. Array attributes use VBOs, whereas constant
 *	attributes use glVertexAttrib*() calls.
 */
#ifndef RE_VERTEX_ARRAY_H
#define RE_VERTEX_ARRAY_H

class RE_Render;
class RE_ElementArray;
class RE_OGLConstAttribute;

#include "RE_CachedObject.h"
#include "RE_Types.h"
#include "RE_Texture.h"
#include "RE_OGLBuffer.h"
#include <UT/UT_StringHolder.h>
#include <UT/UT_WorkBuffer.h>
#include <iosfwd>

// Base class for various array types.
class RE_API RE_VertexArray
{
public:
    // Returns a new vertex array
    static RE_VertexArray *newAttribArray(const UT_StringHolder &name,
					  int num_elements=0,
					  const char *cachename = NULL,
					  RE_ArrayType at = RE_ARRAY_POINT,
					  int capacity = 0);

    static RE_VertexArray *newElementArray(int num_elements=0,
					   const char *cachename = NULL,
					   int capacity = 0);

    // Creates a container RE_VertexArray around the passed array.
    static RE_VertexArray *newArrayContainer(RE_OGLBufferHandle &array,
					     const char *cachename = NULL,
					     RE_ArrayType at = RE_ARRAY_POINT);

    static RE_VertexArray *newArrayContainer(UT_Array<RE_OGLBufferHandle>&array,
					     const char *cachename = NULL,
					     RE_ArrayType at = RE_ARRAY_POINT);

    // Returns the cache name 
    static void		   getAttribCacheName(UT_WorkBuffer &str,
					      const char *name,
					      RE_ArrayType attrib_type,
					      const char *cachename);

	    ~RE_VertexArray();

    // Creates a new RE_VertexArray with exactly the same format and
    // buffer references.
    RE_VertexArray *clone() const;

    /// Returns the amount of main memory (NOT graphics memory!)
    /// owned by this RE_VertexArray.
    int64	getMemoryUsage(bool inclusive) const;

    // add this buffer to the GL cache using a tag of 'name'/attribname
    void	cacheBuffer(const UT_StringHolder &name)
		    { myCacheName = name; }
    const char *getCacheName() const { return myCacheName; }

    // Set the version number on the cached buffer.
    void		setCacheVersion(RE_CacheVersion v);
    RE_CacheVersion	getCacheVersion() const;

    // Indicate if this array is actively in use by the cache.
    void		markAsUsed();
    void		markAsUnused(bool move_to_unused_cache = true);
    bool		isUsed() const { return myInUse; }

    // sets the cache tag on underlying arrays. The cache tag is used to notify
    // other entities when the array is evicted from the cache.
    void	setCacheTag(RE_CacheTagHandle h);

    /// Hint to the GL driver how this buffer will be used. May only be set
    /// before the buffer is created (initialize(), setArray(), etc).
    void	setUsage(RE_BufferUsageHint h);

    /// Set the buffer to use persistent mapped buffers, optionally with
    /// coherent behaviour (writes seen automatically). RE_EXT_BUFFER_STORAGE
    /// must be supported, or false will be returned and the setting will have
    /// no effect.
    void	setPersistent(RE_PersistentBufferMode mode);

    // assigns 'num' buffers to this vertex array, cachable with 'cachename'.
    void	adoptArrays(const UT_Array<RE_OGLBufferHandle> &buffers,
			    const UT_StringHolder &cachename);

    // Removes all contained arrays, possibly deleting them if not cached.
    void	resetBuffer(bool delete_cached = false,
			    bool mark_as_unused = true);
    
    // If the size, type or format of the buffer is changed, this method can
    // ensure that the underlying array is initialized afterward. 'created'
    // can be used to query if a new buffer was allocated by this method.
    bool	initialize(RE_Render *r, bool *created = NULL);
    
    // Allows some extra data to be cached with the buffer. Caching must be
    // enabled, and this can't be a constant attribute (no buffer).
    // Owndership of the data is always given up, and it will be deleted when
    // the buffer is.
    bool		setExtraBufferData(RE_CachedExtraData *data);
    RE_CachedExtraDataHandle getExtraBufferData() const;
    
    // Setup
    // standard GL attributes use the buffer types
    void	setBufferType(RE_BufferType type);
    
    // custom attributes are referenced by name.
    void	setAttributeName(const char *attrib);
    
    // Sets the data type for the array. This type is one array 'element'.
    // If normalized is true, fixed formats will be normalized to [0,1]
    void	setFormat(RE_GPUType type, int vectorsize,
			  bool normalized = true);

    // Sets the length of the array, in elements (a fp32 vec3 is 1 element)
    void	setLength(int len);

    // Sets the total capacity of the array, in elements (a fp32 vec3 is 1
    // element). Length may be less than this.
    bool	setCapacity(int cap);

    // If non-zero, this attribute advances once every 'nprims' number of
    // primtives instead of once per vertex. RE_EXT_INSTANCED_ARRAYS required.
    void	setInstanceStride(int nprims);

    // if non-negative, specifies an instance draw group to which this belongs
    void	setInstanceGroup(int instance_group)
		    { myInstanceGroup = instance_group; }

    // Sets the sampling frequency of this array - either per point, vertex,
    // primitive, instance or randomly accessed.
    void	setArrayType(RE_ArrayType t)	{ myArrayType = t; }

    // Array data methods
    
    // Specifies all the data at once, or a subrange of the data if sublen!=0.
    // offset and sublen are specified in elements, not bytes.
    bool	 setArray(RE_Render *r,
			  const void *data,
			  int offset = 0,
			  int sublen = 0,
			  int array_index = 0);

    bool	 replaceArray(RE_Render *r,
			      const void *data,
			      int array_index = 0)
		    {  return setArray(r,data, 0,myLength, array_index); }

    bool	 replaceArrayRange(RE_Render *r,
				   const void *data,
				   int offset,
				   int length,
				   int array_index)
		    { return setArray(r, data, offset, length, array_index); }


    // fetch data from a buffer, starting at element 'offset' for 'sublen'
    // elements (the size of an element depends on the format & type). If
    // sublen is zero, it will fetch to the end of the buffer.
    bool	getArray(RE_Render *r,
			 void *data,
			 int offset = 0,
			 int sublen = 0,
			 int array_index = 0) const;

    // Cuts loose the current buffer and recreates a new one. Will still
    // maintain the same buffer ID, but the underlying chunk of memory will be
    // handed off to the driver.
    void	 orphan(RE_Render *r);
    
    // The attribute is constant-valued. Data must be big enough to fill one
    // element, and the data is always FP. Constant values cannot be mapped.
    void	 setConstant(const fpreal32 *data);
    void	 setConstant(const fpreal64 *data);

    // Ensure that a constant is created without assigning any data.
    void	 createConstant();

    // Internally constant data is stored as double.
    fpreal64	*getConstant();

    // Indicate that the vertex array is a default value placeholder.
    void	 markAsDefaultValue(bool def) { myDefaultValue = def; }
    bool	 isDefaultValue() const	      { return myDefaultValue; }
    
    // Returns a pointer to the array so it can be modified. must call unmap()
    // when finished, before attempting to use the array again. This maps the
    // entire buffer, so in some instances it may be better to use setArray()
    // with a subrange.
    // NOTE: map()/unmap() is generally slower than setArray(), so use only
    //       for reading or persistent buffers.
    void	*map(RE_Render *r,
		     RE_BufferAccess access = RE_BUFFER_WRITE_ONLY,
		     int array_index = 0);
    void	*mapRange(RE_Render *r, int start, int length,
			  RE_BufferAccess access = RE_BUFFER_WRITE_ONLY,
			  int array_index = 0);
    void	 unmap(RE_Render *r, int array_index = 0);

    // For persistent mapped buffers, this returns the currently mapped data.
    // Will only return a value if mapped, persisent is set, and
    // RE_EXT_BUFFER_STORAGE is supported.
    void		*getPersistentMap(int array_index = 0) const;

    // Returns true if this is a persistently mapped buffer
    bool		isPersistentMap() const;
    
    // Zero the buffer.
    void	 zero(RE_Render *r);

    // Attach a texture buffer object to a VERTEX, PRIM or RANDOM array type.
    bool	attachTexBuffer(RE_Render *r,
				RE_Texture *tex);

    // Return the TBO attached to a VERTEX, PRIM or RANDOM array type.
    RE_Texture *getTextureBufferObject() const; // inlined below

    // Binding methods

    // Binds the array to the given array attribute type for subsequent draw
    // calls. When drawing with multiple attributes, the first N-1 should be
    // bound, and the Nth should call draw...Prims().
    // geo_obj is only true when called from RE_Geometry.
    bool	bind(RE_Render *r, unsigned int stride = 0,
		      bool geo_obj=false,
		      const char *shader_attrib = NULL);
    bool	unbind(RE_Render *r);

    // Rebinds an array to a new attribute location due to a shader change.
    bool	rebindAttrib(RE_Render *r, int location);
    bool	unbindAttrib(RE_Render *r);

    // Bind to a VAO.
    bool	setupForAttribIndex(RE_Render *r, int attrib_location,
				    RE_GPUType t, int stride);


    // These bind methods bind the buffer to the specificied buffer binding
    // point, which may be different than the buffer type's default binding
    // point. Some binding points are indexed (uniform and transform feedback)
    // while most have a single binding point.
    bool	bindBuffer(RE_Render *r, RE_BufferBinding point, int index=0,
			   int array_index=0);
    bool	unbindBuffer(RE_Render *r, RE_BufferBinding point, int index=0,
			     int array_index=0);
    
    // Drawing methods

    // Draws primitives using every n'th point (where n=stride) in this array 
    // as 'prim' primitive type.
    void	drawPrims(RE_Render *r, 
			  RE_PrimType prim, 
			  unsigned int stride,
			  int num_instances,
			  bool enable_arrays,
			  int vertices_per_patch);

    // Draws primitives using a subrange of points in this array
    void	drawSomePrims(RE_Render *r,
			      RE_PrimType prim,
			      int start,
			      int num,
			      int num_instances,
			      bool enable_arrays,
			      int vertices_per_patch);

    // Draws primitives using some points in this array using a 2nd array as
    // the indices. The array must be an element array.
    void	drawSomePrims(RE_Render *r,
			      RE_PrimType prim,
			      RE_VertexArray *index_array,
			      int index_offset,
			      int index_num_elements,
			      int num_instances,
			      bool enable_arrays,
			      int vertices_per_patch);

    // Query
    RE_BufferType	getBufferType() const	{ return myBufferType; }
    RE_GPUType		getDataType() const	{ return myType; }
    int			getVectorSize() const	{ return myVectorSize; }
    int			getLength() const	{ return myLength; }
    int			getLengthScale() const	{ return myLengthScale; }
    int			getCapacity() const	{ return myCapacity; }
    int			getNumBuffers() const   { return myArray.entries(); }
    const RE_OGLBufferHandle &getArrayBuffer(int i=0) const
	    { return (i < myArray.entries()) ? myArray(i) : theNullArray; }
    const char	       *getAttributeName() const { return myAttribName; }
    RE_ArrayType	getArrayType() const	{ return myArrayType; }
    bool		isBufferBound(int array_index=0) const;
    bool		isConstantBuffer() const { return myConstantFlag; }
    void		getHoudiniName(UT_StringHolder &name) const;
    void		getGLName(UT_StringHolder &name) const;
    int			getInstanceStride() const { return myInstanceStride; }
    int			getInstanceGroup() const { return myInstanceGroup; }

    int64		getSizeBytes() const;

    // Debugging prints
    void		print(std::ostream *os = nullptr) const;
    void		printValues(RE_Render *r, std::ostream &os,
				    int start=0, int len=0) const;

    void		addRef(void (*callback)(void *), void *data);
    void		removeRef(void (*callback)(void *), void *data);

    // Some attributes are 'known' types, as listed in RE_GenericAttribID.
    // Setting the ID can help performance. Do not change the type of an attrib
    // while bound to an RE_Geometry; this should only be called once.
    void	setGenericID(RE_GenericAttribID id)
		    { UT_ASSERT(myGenID == RE_GENATTRIB_UNDEF || myGenID == id);
			myGenID = id; }
    RE_GenericAttribID getGenericID() const { return myGenID; }
    
protected:
    RE_VertexArray(RE_BufferType type, int nelements = 0, int capacity = 0);
    RE_VertexArray(const UT_StringHolder &name, int nelements = 0,
		   int capacity= 0 );
    RE_VertexArray(const RE_VertexArray &va);
    
    void		detachTexBuffer(int index);
    void		generateCacheName(UT_StringHolder &cachename,
					  int index) const;
 
    UT_StringHolder	myCacheName;
    UT_StringHolder	myAttribName;
    RE_BufferType	myBufferType;
    RE_GPUType		myType;
    int			myVectorSize;
    int			myLength;
    int			myLengthScale;
    int			myCapacity;
    int			myInstanceStride;
    UT_Array<RE_OGLBufferHandle> myArray;
    static RE_OGLBufferHandle	 theNullArray;
    RE_OGLConstAttribute *myConstAttrib;
    RE_CacheTagHandle   myCacheTagHandle;
    RE_BufferUsageHint  myUsage;
    bool		myInUse;
    
    bool		myMappedFlag[4];
    bool		myConstantFlag;
    bool		myNormalizedFlag;
    RE_ArrayType	myArrayType;
    RE_GenericAttribID  myGenID;
    int			myInstanceGroup;
    RE_PersistentBufferMode myPersistentBufferMode;
    bool		myPersistentMapCopied;
    bool		myDefaultValue;

    class re_VertexArrayCB
    {
    public:
	re_VertexArrayCB(void (*cb)(void *),void *d) : callback(cb), data(d) {}
	void (*callback)(void *);
	void *data;
    };

    UT_Array<re_VertexArrayCB> myRefs;
};

inline RE_Texture *
RE_VertexArray::getTextureBufferObject() const
{
    return (myArray.entries() == 1) ? myArray(0)->getTBO() : NULL;
}

/// Shallow reference to an RE_VertexArray. Will be cleared if the
/// RE_VertexArray is deleted.
class RE_API RE_VertexArrayRef
{
public:
	    RE_VertexArrayRef() : myArray(NULL) {}
	   ~RE_VertexArrayRef();

    bool	    isValid() { return myArray!=NULL; }
    void	    operator=(RE_VertexArray &va);
    void	    clear();
    RE_VertexArray *get() { return myArray; }
    RE_VertexArray *operator->()
	{
	    UT_ASSERT(myArray);
	    return myArray;
	}

private:
    static void arrayDeleted(void *);
    
    RE_VertexArray *myArray;
};

#endif
