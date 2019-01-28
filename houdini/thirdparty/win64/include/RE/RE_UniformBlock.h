/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_UniformBlock.h ( RE Library, C++)
 *
 * COMMENTS:
 *	Encapsulates a uniform buffer object and provides high-level interface
 *	for setting the variables within. Setting a variable will mark the block
 *	dirty, so the next time it is bound it will be uploaded again.
 */
#ifndef RE_UniformBlock_h
#define RE_UniformBlock_h

#include "RE_Shader.h"
#include <UT/UT_IntrusivePtr.h>

class RE_OGLUniformBuffer;
class RE_Render;
class RE_UniformBlock;
class RE_UniformData;

typedef UT_IntrusivePtr<RE_UniformBlock> RE_UniformBlockHandle;

/// A uniform block is a structure of uniforms used by a GLSL shader. This
/// class can represent either a Uniform Buffer Object (UBO) or a
/// Shader Storage Buffer Objects (SSBO, GL4.3).
class RE_API RE_UniformBlock : public UT_IntrusiveRefCounter<RE_UniformBlock>
{
public:
    explicit		RE_UniformBlock(const char *name = NULL);
			~RE_UniformBlock();

    /// Returns the amount of main memory (NOT graphics memory!)
    /// owned by this RE_UniformBlock.
    int64 getMemoryUsage(bool inclusive) const;

    /// Name of the uniform block (uniform <name> { } )
    const char *	getName() const;

    /// Optional instance name of the uniform block (uniform block { } <name>)
    /// This will need to be specified to bind a UBO to a specific instance
    /// when instanced UBOs are used by the shader.
    void		setInstanceName(const char *name);
    
    /// Optional instance name of the uniform block (uniform block { } <name>)
    const char *	getInstanceName() const;

    /// Array length of variably sized arrays (for GL4 SSBOs).
    int			getArrayLength() const { return myArrayLength; }

    /// Creates a new uniform block with its own buffer based on the structure
    /// of this uniform block. If data_too is true, the data inside the uniform
    /// block is also copied.
    RE_UniformBlock    *duplicate(bool block_data_too = true) const;

    /// Create a new shader storage block, specifying an array size for
    /// array member data with an unspecified array size. If there are no
    /// arrays with unspecified bounds, array_size is ignored.
    /// Data is left uninitialized.
    RE_UniformBlock    *arrayDuplicate(int array_size) const;
    
    /// returns true if 'block' shares the same size and structure as this block
    bool		isCompatible(const RE_UniformBlock *block) const;

    /// Size of the underlying GL buffer.
    int			getSizeB() const;

    /// Returns true if this block has a member variable called 'name'
    bool		hasUniform(const char *name) const;
    /// Number of member variables in the uniform block.
    int			getNumUniforms() const;
    /// Returns the name of the member variable at 'index'
    const char	       *getUniformName(int index) const;
    /// Which UBO or SSBO index this uniform block was created for.
    int			getUniformShaderIndex(int index) const;


    /// Copy the data for uniform 'name' into the buffer. If name does not
    /// exist in the buffer, or its type/array size doesn't match,
    /// bind...() will return false. If its value is not different from the
    /// current value in the block, it will not cause an upload.
    /// Shader Storage Blocks can have open-ended arrays, and variables within
    /// those arrays can specify their array index with 'array_index'.
    bool		bindInt(const char *name, int v,
				int array_index = 0);
    /// Bind a single float value.
    bool		bindFloat(const char *name, fpreal32 v,
				int array_index = 0);
    /// Bind a single double value. GPU must support native FP64 values.
    bool		bindDouble(const char *name, fpreal64 v,
				int array_index = 0);
    /// Bind a single handle value to a sampler. GPU must support bindless
    /// textures.
    bool		bindUint64(const char *name,
				   uint64 v,
				   int array_index = 0);

    /// Set values for a fixed-length integer array. 
    bool		bindInts(const char *name, const int *valarray,
				 int array_size,
				 int array_index = 0);
    /// Set values for a fixed-length float array. 
    bool		bindFloats(const char *name, const fpreal32 *valarray,
				   int array_size,
				   int array_index = 0);
    /// Set values for a fixed-length double array. 
    bool		bindDoubles(const char *name, const fpreal64 *valarray,
				    int array_size,
				    int array_index = 0);
    /// Set values for a uint64 handle array.
    bool		bindUint64s(const char *name, const uint64 *valarray,
				    int array_size,
				    int array_index = 0);

    /// vec3 is a little different, as it is padded to vec4 in uniform blocks.
    /// SSBOs with std420 packing should use bindVector().

    /// bind a series of ivec3s, using std140 packing (padded to ivec4)
    bool		bindIntVector3(const char *name, const int *valarray,
				       int array_size,
				       int array_index = 0);
    /// bind a series of vec3s, using std140 packing (padded to ivec4)
    bool		bindVector3(const char *name, const fpreal32 *valarray,
				    int array_size,
				    int array_index = 0);
    /// bind a series of dvec3s, using std140 packing (padded to ivec4)
    bool		bindDVector3(const char *name, const fpreal64 *valarray,
				     int array_size,
				     int array_index = 0);

    /// Bind a single vec2 uniform value.
    bool		bindVector(const char *name, const UT_Vector2F &v,
				   int array_index = 0);
    /// Bind a single vec3 uniform value.
    bool		bindVector(const char *name, const UT_Vector3F &v,
				   int array_index = 0);
    /// Bind a single vec4 uniform value.
    bool		bindVector(const char *name, const UT_Vector4F &v,
				   int array_index = 0);

    /// Bind a single dvec2 uniform value.
    bool		bindVector(const char *name, const UT_Vector2D &v,
				   int array_index = 0);
    /// Set a single dvec3 uniform value.
    bool		bindVector(const char *name, const UT_Vector3D &v,
				   int array_index = 0);
    /// Set a single dvec4 uniform value.
    bool		bindVector(const char *name, const UT_Vector4D &v,
				   int array_index = 0);

    /// Set a single 2x2 matrix value.
    bool		bindMatrix(const char *name, const UT_Matrix2F &m,
				   int array_index = 0);
    /// Set a single 3x3 matrix value.
    bool		bindMatrix(const char *name, const UT_Matrix3F &m,
				   int array_index = 0);
    /// Set a single 4x4 matrix value.
    bool		bindMatrix(const char *name, const UT_Matrix4F &m,
				   int array_index = 0);

    /// Set a single 2x2 matrix value (dmat2).
    bool		bindMatrix(const char *name, const UT_Matrix2D &m,
				   int array_index = 0);
    /// Set a single 3x3 matrix value (dmat3).
    bool		bindMatrix(const char *name, const UT_Matrix3D &m,
				   int array_index = 0);
    /// Set a single 4x4 matrix value (dmat4).
    bool		bindMatrix(const char *name, const UT_Matrix4D &m,
				   int array_index = 0);
    
    // Note that you cannot include texture samplers in a uniform block, so
    // there are no bindTextureMap() calls here.

    
    /// commits any changes to the buffer object, creating the buffer object
    /// if required.
    void		uploadBuffer(RE_Render *r);

    /// Fetches the contents of the GL buffer into the main mem block contained
    /// by this class.
    void		downloadBuffer(RE_Render *r);

    /// The raw GL id of the buffer. Will not exist unless uploadBuffer() was
    /// previously called. Returns -1 if there is no buffer object.
    int			getID() const { return myID; }

    /// Debug printout of the block, including currently bound values.
    void		print(std::ostream *os = NULL);

    static void		dirtyAllUniformBlocks() { theResetCounter++; }

    
    // ---------------------------------------------------------------------
    // The remaining methods should only be called by RE_Shader. 
    
    /// Name of the uniform block (uniform <name> { } ), generally set by the
    /// shader.
    void		setName(const char *name);
    
    /// Size of the underlying GL buffer, in bytes. Generally set by the shader.
    void		setSize(int bytes);
    
    /// Called during shader intitialization to layout the structure of this
    /// block.
    void		addUniform(const char	 *name,
				   RE_UniformType type,
				   int		  array_size,
				   int		  offset,
				   int		  location,
				   int		  top_array_size,
				   int		  top_array_stride);
    void		clearUniforms();
    void		setBufferSizes(int fixed_size_bytes,
				       int variable_size_bytes);
    
    /// Update the passed uniform block with the uniforms found in this block.
    /// This is generally done by the shader before a draw call. 
    bool		updateBlock(RE_UniformBlock *b,
				    const RE_Shader *sh) const;
    
    /// Whether this block requires uploading to the GPU.
    bool		isDirty() const { return myDirtyFlag ||
						 myResetCount!=theResetCounter;}
    /// Which shader this block is currently laid out for.
    void		setInitializedFor(const RE_Shader *sh);
    /// Which shader this block is currently laid out for.
    bool		isInitializedFor(const RE_Shader *sh) const;
    /// Which shader this block is currently laid out for.
    const RE_Shader    *getInitializedShader() const { return myInitShader; }
    /// Program ID of the program this block was created from.
    void		setOriginProgramID(int pid) { myLastProgramID = pid; }


private:
    int			getUniformIndex(const char *name) const;
    void		copyToBuffer(const void *data, int len, int offset,
				     const char *uniform_name);
    void		copyToBuffer3(const void *data, int len, int dsize,
				      int stride, int offset,
				      const char *uniform_name);
    void		printMember(std::ostream &os, int idx,
				    int maxlen, int offset);
    
    UT_String		 myName;
    UT_String		 myInstanceName;
    RE_OGLUniformBuffer	*myBufferObject;
    void		*myBuffer;
    bool		 myDirtyFlag;
    UT_ValArray<RE_UniformData *> myUniforms;
    int			 mySize;
    int			 myID;
    UT_IntArray		 myDataMatch;
    const RE_Shader	*myInitShader;

    // for buffer storage objects, which can have a non-array section at the
    // beginning of the block (fixed), followed by array data at the end
    // (variable). myArrayLength determines the number of entries in the array.
    int			 myFixedSize;
    int			 myVariableSize;
    int			 myArrayLength;

    int			 myLastProgramID;
    int			 myResetCount;
    static int		 theResetCounter;
};

inline const char *
RE_UniformBlock::getName() const
{
    return myName;
}

inline const char *
RE_UniformBlock::getInstanceName() const
{
    return myInstanceName;
}

inline void
RE_UniformBlock::setInitializedFor(const RE_Shader *sh)
{
    myInitShader = sh;
}

inline bool
RE_UniformBlock::isInitializedFor(const RE_Shader *sh) const
{
    return myInitShader == sh;
}

#endif
