
/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Render Library (C++)
 *
 * COMMENTS:
 *     OpenGL GLSL/Cg Shader base definition. Abstracts the difference
 *     between the two.
 *
 */
#ifndef __RE_Shader__
#define __RE_Shader__

#include "RE_API.h"
#include "RE_Texture.h"
#include "RE_TextureTypes.h"
#include "RE_Types.h"
#include "RE_Uniform.h"

#include <UT/UT_Array.h>
#include <UT/UT_Color.h>
#include <UT/UT_IntrusivePtr.h>
#include <UT/UT_Matrix2.h>
#include <UT/UT_Matrix3.h>
#include <UT/UT_Matrix4.h>
#include <UT/UT_String.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_SymbolTable.h>
#include <UT/UT_ValArray.h>
#include <UT/UT_Vector2.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_Vector4.h>
#include <UT/UT_VectorTypes.h>
#include <SYS/SYS_Math.h>
#include <SYS/SYS_Types.h>
#include <iosfwd>

class RE_ElementArray;
class RE_Geometry;
class RE_OGLTexture;
class RE_Render;
class RE_ShaderStage;
class RE_UniformBlock;
class RE_UniformData;
class RE_VertexArray;
class RE_VertexMap;
class RE_VertexState;
class UT_StringArray;
class UT_WorkBuffer;

class re_LibFunc;
class re_VertexName;

typedef UT_IntrusivePtr<RE_UniformBlock> RE_UniformBlockHandle;

class RE_API RE_Shader
{
public:
    /// Program name is readable, shader_version may be 0 or a
    /// valid GLSL version (110, 120, 130, 140, 150, 330, 400, 410, 420..)
    enum { USE_GLSL_BASE_VERSION = -1,
	   USE_SHADER_GLSL_VERSION = 0 };
    
		 RE_Shader(const char *program_name, int shader_version);
    virtual	~RE_Shader();

    /// Returns the amount of main memory (NOT graphics memory!)
    /// owned by this RE_Shader.
    virtual int64 getMemoryUsage(bool inclusive) const;

    /// If true, register this shader so that RE_Shader::getShader() can find
    /// this shader (default is true).
    void	 setRegisterProgram(bool register_program);

    ///Call to allocate a specific shader type. If you want to allocate a shader
    ///of a specific GLSL version, set the version to the same number as the
    ///#version string in the shader (ie, 120 for 1.20, 330 for 3.30). Otherwise
    ///the shader inherits the version from the first shader object attached.
    static RE_Shader	*create(const char *name,
				int shader_version = 0,
				RE_ShaderLanguage lang=RE_SHADER_LANGUAGE_GLSL);

    ///Create a shader and load all shader files listed in shader_files, up to
    ///a nullptr filename. If the shader compiles and links, a non-nullptr shader will
    ///be returned. 'compile_msg' will contain any error messages otherwise.
    ///Shader stages must have extensions .vert, .geom and .frag. 
    static RE_Shader *createShader(RE_Render *r,
				   const char *name,
				   const char **shader_files,
				   int shader_version = USE_SHADER_GLSL_VERSION,
				   UT_String *compile_msg = nullptr,
				   bool use_houdini_libs = true,
				   RE_ShaderLanguage s=RE_SHADER_LANGUAGE_GLSL,
				   bool register_program = true,
				   const char *extra_defines = nullptr);

    /// A version with a single string containing whitespace-separated filenames
    static RE_Shader *createShader(RE_Render *r,
				   const char *name,
				   const char *shader_files,
				   int shader_version = USE_SHADER_GLSL_VERSION,
				   UT_String *compile_msg = nullptr,
				   bool use_houdini_libs = true,
				   RE_ShaderLanguage s=RE_SHADER_LANGUAGE_GLSL,
				   bool register_program = true,
				   const char *extra_defines = nullptr);

    /// Readable name of the shader
    const char  *getName() const	{ return myProgramName; }

    /// Any define commands used to alter this shader
    const char  *getDefines() const	{ return myExtraDefines; }

    /// Raw GL id of the shader
    int		 getProgram() const	{ return myProgramObject; }

    /// True if the shader has been successfully linked
    bool	 isLinked() const	{ return myLinkedFlag; }
    
    /// Returns the version of the shader (110,120,130,140,150,330,400,410).
    int		 getCodeVersion() const  { return myCodeVersion; }

    /// Returns the minimum GLSL version supported (base requirement)
    static int	 getBaseGLSLVersion();

    /// determine the version of the shader from the source (#version <num>)
    /// return 100 if no version is specified.
    static int	 determineCodeVersion(const char *shader_string);

    /// Allocate a new shader stage of the specified type, with an optional
    /// readable name.
    virtual RE_ShaderStage *newShader(RE_ShaderType type,
				       const char *name = 0) = 0;

    /// @{
    /// loadShader() loads a shader from an ASCII file. 
    /// addShader() is used when the source is already available.
    /// 'messages' returns the warnings & errors (if any) from shader
    /// compilation.
    /// Multiple shaders can be attached to the same shader type; it is expected
    /// that one is the main shader, and others are functions.
    RE_ShaderStage *loadShader(RE_Render *r,
			       RE_ShaderType stype,
			       const char *filename,
			       UT_String *messages = nullptr,
			       const char *shader_name = nullptr,
			       const char *defines = "",
			       int default_code_version=USE_GLSL_BASE_VERSION);
    
    RE_ShaderStage *addShader(RE_Render *r,
			       RE_ShaderType stype,
			       const char *shader_source,
			       const char *shader_name,
			       int codeversion,
			       UT_String *messages = nullptr);
    /// @}
    
    /// If true, this shader may use Houdini's builtin functions.
    void	 useHoudiniLibraryFuncs();

    /// If true, this shader follows the default Houdini attribute map.
    void	 useDefaultAttribMap(bool enable = true);

    /// If true, this shader defines its own attribute locations in shader code
    /// Do not move any attributes.
    void	 useExplicitAttribMap(bool enable = true);

    ///Individual Shader objects can be created separately and attached. returns
    ///true if the shader was attached, and false if it was already attached.
    virtual bool attachShader(RE_Render *r,
			      RE_ShaderStage *obj,
			      UT_String *messages = nullptr);

    // Detaching a shader object prevents it from being deleted when the shader
    // is deleted. Returns true if the shader was detached, or false if the
    // shader was not present in the program.
    virtual bool detachShader(RE_Render *r,
			      RE_ShaderStage *obj);

    /// Returns an attached shader by the given name.
    RE_ShaderStage *getShader(const char *shader_name,
			       RE_ShaderType hint = RE_SHADER_ALL);

    /// bind vertex shader input 'name' to vertex array index 'vertex_index'.
    /// Must be called before linking, or a re-link must be done.
    virtual bool setVertexInput(RE_Render *r,
				const char *name,
				int vertex_index);
    
    /// bind fragment shader output 'name' to color attachment 'buffer_index'.
    /// Must be called before linking. Only needed for custom fragment output
    /// (ie, not gl_FragData[] or gl_FragColor).
    virtual bool setFragmentOutput(RE_Render *r,
				   const char *name,
				   int buffer_index);

    /// Links all the shaders together; called implicitly if needed by
    /// RE_Render::setShader(). Note that linking the shader will clear
    /// out any uniforms that have been set.
    virtual bool linkShaders(RE_Render *r, UT_String *messages = nullptr);

    /// Bumped every time a link is performed (uniforms invalidated, etc).
    int		 getLinkSerial() const { return myShaderSerial; }

    /// Ensures that the shader can run given the current GL state. This mostly
    /// deals with texture bindings matching what is expected by the shader.
    /// If this returns false, the shader will not run, and 'messages' can be
    /// used to fetch the reason.
    virtual bool validateShader(RE_Render *r, UT_String *messages = nullptr);

    /// Clears and deletes all the shader objects of a certain type (or all)
    virtual void clearShaders(RE_Render *r,
			      RE_ShaderType types = RE_SHADER_ALL);


    /// Returns true if this shader program has a shader of 'type' attached.
    /// type may be a bitmask of shader types, in which case it returns true if
    /// any of the shader types exist.
    bool	 hasShader(RE_ShaderType type) const;

    // GEOMETRY SHADERS ------------------------------------------------------

    /// @name Geometry Shaders
    /// Geometry shaders must be initialized with a cap on how many vertices
    /// they emit per primitive, as well as the primitives they operate on,
    /// and the primitives they produce. A vertex and fragment shader MUST be
    /// used with a geometry shader. A geometry shader will fail if the
    /// primitives drawn do not match the geometry shader input.
    /// Input primitives can be points, line variants (lines, line loop, line
    /// strip) or triangle variants (tris, tristrips, trifans). Quads and polys
    /// are not supported. The output must be points, linestrip or tristrip.
    //@{
    
    /// Specify geometry shader parameters.
    virtual bool setGeometryParms(RE_Render *r,
				  int max_vertices,
				  RE_PrimType input,
				  RE_PrimType output);

    /// @brief Fetch current geometry shader values.
    /// Return the geometry shader parameters specified by setGeometryParms() or
    /// the shader itself (via layout(in=triangles), etc).
    void	 getGeometryParms(int &maxverts,
				  RE_PrimType &input_type,
				  RE_PrimType &output_type) const
		 {
		      maxverts   = myGeoShaderMaxVertices;
		      input_type = myGeoShaderInputPrims;
		      output_type= myGeoShaderOutputPrims;
		 }
    //@}

    // TRANSFORM FEEDBACK ----------------------------------------------------

    /// @name Transform Feedback
    /// Using transform feedback, it is possible to capture vertex or geometry
    /// shader outputs and write these values to buffers (RE_VertexArrays).
    /// Method to add or remove shader outputs should be called before the
    /// shader is linked, otherwise they will cause a re-link of the shader.
    /// Note that relinking with clear out any uniform values, so these buffers
    /// should be set up early.
    //@{
    
    /// @brief Specify an RE_Geometry to collect data from Transform Feedback.
    /// Specify a geometry object to hold all the arrays for transform feedback.
    /// You retain ownership of the object unless 'give_ownership' is true.
    /// If no object is attached when the shader is linked, one will be
    /// created for you (with ownership maintained by this shader).
    void	addCaptureGeometry(RE_Geometry *object,
				   bool give_ownership = false);

    /// Return the capture geometry for transform feedback.
    RE_Geometry *getCaptureGeometry(bool take_ownership = false); 

    /// Specify a generic attribute to capture during transform feedback.
    bool	addFeedbackBuffer(const char *attrib_name);

    /// Remove a generic attribute from the capture geometry.
    bool	removeFeedbackBuffer(const char *attrib_name);

    /// Will generate a new owned capture geometry if one isn't present.
    void	setFeedbackBufferSize(int num_vertices);

    /// Remove all attributes from the capture gemetry.
    void	clearAllFeedbackBuffers();

    /// True if this shader has transform feedback varyings available.
    bool	hasTransformFeedback() const;

    /// @brief Fetch a vertex array written to by the shader. 
    /// These are only valid after the shader has been linked for
    /// transform feedback (usually due to a draw call). Returns a vertex array
    /// for the named feedback buffer.
    RE_VertexArray *getFeedbackBuffer(const char *attrib_name) const;
    

    /// @brief Check if a feedback buffer will be written by this shader
    /// Returns true if the attribute is available to be captured. Will return
    /// false if no buffer was setup to capture the attribute, or the shader
    /// does not output that attribute.
    /// This method must be called after the shader is linked.
    bool	isFeedbackBufferUsed(RE_BufferType glattrib,
				     int texlevel = 0) const;
    
    /// @brief Check if a feedback buffer will be written by this shader
    /// Returns true if the attribute is available to be captured. Will return
    /// false if no buffer was setup to capture the attribute, or the shader
    /// does not output that attribute.
    /// This method must be called after the shader is linked.
    bool	isFeedbackBufferUsed(const char *attrib) const;

    /// @private For RE_Render only.
    virtual bool	 bindForTransformFeedback(RE_Render *r);
    
    /// @}
    
    // SHADING OPTIONS -------------------------------------------------------

    /// For multisampled framebuffers, this sets the fraction of samples that
    /// should be shaded by a fragment shader, from 0.0 to 1.0. For example,
    /// setting this to 0.5 would guarentee _at least_ 4 color samples for a 8x
    /// multisample buffer. The default is 0.0 (1 color sample, N coverage)
    /// This only has an effect on GL4 hardware. It is ignored unless
    /// RE_EXT_SAMPLE_SHADING is supported.
    /// @{
    void	 setShadingRate(fpreal rate)
				    { myShadingRate = SYSclamp(rate,0.0,1.0); }
    fpreal	 getShadingRate() const
				    { return SYSmax(myShadingRate,0.0); }
    bool	 isSampleShading() const { return myShadingRate >= 0.0; }
    /// @}

    /// Disables the shading rate for this shader. The rate set by
    /// RE_Render::setShaderSampleRate() is used instead.
    void	 disableShadingRate() { myShadingRate = -1.0; }

    // UNIFORMS --------------------------------------------------------------

    /// @brief Set the value of a uniform.
    /// Set uniform value(s) for a specific type. False will be returned if the
    /// shader is not linked (errors?), the given name does not exist, or the
    /// type does not match the GLSL uniform type.
    /// @{
    bool	 bindInt(RE_Render *r, const char *name, int v);
    bool	 bindFloat(RE_Render *r, const char *name, float v);
    bool	 bindDouble(RE_Render *r, const char *name, double v);

    bool	 bindColor(RE_Render *r, const char *name, const UT_Color &c);
    bool	 bindColor(RE_Render *r, const char *name, const UT_Color &c,
			   fpreal alpha);
    
    bool	 bindVector(RE_Render *r,const char *name,const UT_Vector2F &v);
    bool	 bindVector(RE_Render *r,const char *name,const UT_Vector3F &v);
    bool	 bindVector(RE_Render *r,const char *name,const UT_Vector4F &v);

    bool	 bindVector(RE_Render *r,const char *name,const UT_Vector2D &v);
    bool	 bindVector(RE_Render *r,const char *name,const UT_Vector3D &v);
    bool	 bindVector(RE_Render *r,const char *name,const UT_Vector4D &v);

    bool	 bindVector(RE_Render *r,const char *name,const UT_Vector2i &v);
    bool	 bindVector(RE_Render *r,const char *name,const UT_Vector3i &v);
    bool	 bindVector(RE_Render *r,const char *name,const UT_Vector4i &v);

    bool	 bindVectors(RE_Render *r, const char *name,
			     const UT_Vector2FArray &vals);
    bool	 bindVectors(RE_Render *r, const char *name,
			     const UT_Vector3FArray &vals);
    bool	 bindVectors(RE_Render *r, const char *name,
			     const UT_Vector4FArray &vals);

    bool	 bindVectors(RE_Render *r, const char *name,
			     const UT_Vector2DArray &vals);
    bool	 bindVectors(RE_Render *r, const char *name,
			     const UT_Vector3DArray &vals);
    bool	 bindVectors(RE_Render *r, const char *name,
			     const UT_Vector4DArray &vals);

    bool	 bindMatrix(RE_Render *r,const char *name,const UT_Matrix2F &m);
    bool	 bindMatrix(RE_Render *r,const char *name,const UT_Matrix3F &m);
    bool	 bindMatrix(RE_Render *r,const char *name,const UT_Matrix4F &m);

    bool	 bindMatrix(RE_Render *r,const char *name,const UT_Matrix2D &m);
    bool	 bindMatrix(RE_Render *r,const char *name,const UT_Matrix3D &m);
    bool	 bindMatrix(RE_Render *r,const char *name,const UT_Matrix4D &m);
    /// @}

    /// Bind a texture map using named textures in TIL_TextureMap. You can also
    /// bind a texture with its raw OpenGL ID using bindVariableInt().
    /// @{
    /// Bind a new texture object using the mapname (filename, op:/coppath)
    virtual bool bindTextureMap(RE_Render *r,
				const char *name,
				const char *mapname,
				const char *relative_to) = 0;
    /// @}

    /// Return the texture type for the sampler named 'name'.
    virtual RE_TextureDimension getTextureMapType(RE_Render *r,
						  const char *name) = 0;

    /// Return the number of samplers this shader references
    int		 getNumTextures() const { return myTextureUniforms.entries(); }

    /// Return the number of image samplers this shader references (GL4.2
    /// shader load/store images, not regular texture samplers)
    int		 getNumImages() const { return myImageUniforms.entries(); }

    /// @brief Return the name of a sampler uniform
    /// Return the texture sampler name of the enumerated samplers at 'tex_index'
    /// Must be within [0, getNumTextures()-1]. 
    const char  *getTextureSamplerName(int tex_index) const;

    /// Return the texture sampler type at 'tex_index' [0, getNumTextures()-1].
    RE_TextureDimension getTextureSamplerType(int tex_index) const;

    /// @brief return the texture unit of sampler 'uniform_name'
    /// Return the texture unit this sampler has been assigned to, or -1 if
    /// 'uniform_name' does not exist, or is not a sampler.
    int		 getUniformTextureUnit(const char *uniform_name) const;
    
    /// @brief return the image unit of sampler 'uniform_name'
    /// Return the image unit this image uniform has been assigned to, or -1 if
    /// 'uniform_name' does not exist, or is not a image uniform. Images are
    /// different than textures, used by RE_EXT_IMAGE_LOAD_STORE (GL4.2).
    int		 getUniformImageUnit(const char *uniform_name) const;

    /// Return the uniform type of the uniform named 'uniform_name', or
    /// RE_UNIFORM_INVALID if it does not exist.
    RE_UniformType getUniformType(const char *uniform_name) const;


    /// Returns the number of active uniforms in this shader.
    int		getNumUniforms() const		{ return myUniforms.entries(); }

    /// Returns the name of active uniform 'idx' [0, getNumUniforms()-1]
    const char *getUniformName(int idx) const;

    /// Return the GL uniform location of uniform 'idx' [0, getNumUniforms()-1]
    int		getUniformLocation(int idx) const;

    /// Return the GL uniform type 'idx' [ 0, getNumUniforms()-1]
    RE_UniformType getUniformType(int idx) const;

    /// Return the index of the uniform block a uniform is in, -1 for none.
    int		getBlockIndexForUniform(int idx) const;


    /// @name Setting Uniforms
    /// These methods set uniform values in a shader. 'name' is the name of the
    /// uniform in the shader. 'val' is the value that uniform should take.
    /// 'array_size' is used if the uniform is an array. 'saved_idx' is an
    /// optimization to speed up subsequent accesses to the uniforms. Initially
    /// it should be -1.
    //@{

    /// Generic Integer scalar
    virtual bool	 bindVariableInt(RE_Render *r,
					 const char *name, const int *val,
					 int array_size=1, int *saved_idx=0)=0;
    /// Generic Integer vector-2
    virtual bool	 bindVariableInt2(RE_Render *r,
					  const char *name, const int *val,
					  int array_size=1, int *saved_idx=0)=0;
    /// Generic Integer vector-3
    virtual bool	 bindVariableInt3(RE_Render *r,
					  const char *name, const int *val,
					  int array_size=1, int *saved_idx=0)=0;
    /// Generic Integer vector-4
    virtual bool	 bindVariableInt4(RE_Render *r,
					  const char *name, const int *val,
					  int array_size=1, int *saved_idx=0)=0;
    
    /// 64b Integer handle for bindless texturing
    virtual bool	 bindUint64(RE_Render *r,
				    const char *name, uint64 *val,
				    int array_size = 1, int *saved_idx=0) =0;

    virtual bool	 bindVariable1(RE_Render *r,
				       const char *name, const fpreal32 *val,
				       int array_size=1, int *saved_idx=0)=0;
    virtual bool	 bindVariable2(RE_Render *r,
				       const char *name, const fpreal32 *val,
				       int array_size=1, int *saved_idx=0)=0;
    virtual bool	 bindVariable3(RE_Render *r,
				       const char *name, const fpreal32 *val,
				       int array_size=1, int *saved_idx=0)=0;
    virtual bool	 bindVariable4(RE_Render *r,
				       const char *name, const fpreal32 *val,
				       int array_size=1, int *saved_idx=0)=0;

    virtual bool	 bindVariable1(RE_Render *r,
				       const char *name, const fpreal64 *val,
				       int array_size=1, int *saved_idx=0)=0;
    virtual bool	 bindVariable2(RE_Render *r,
				       const char *name, const fpreal64 *val,
				       int array_size=1, int *saved_idx=0)=0;
    virtual bool	 bindVariable3(RE_Render *r,
				       const char *name, const fpreal64 *val,
				       int array_size=1, int *saved_idx=0)=0;
    virtual bool	 bindVariable4(RE_Render *r,
				       const char *name, const fpreal64 *val,
				       int array_size=1, int *saved_idx=0)=0;

    virtual bool	 bindMatrix2(RE_Render *r,
				     const char *name, const fpreal32 *val,
				     int array_size=1, int *saved_idx=0)=0;
    virtual bool	 bindMatrix3(RE_Render *r,
				     const char *name, const fpreal32 *val,
				     int array_size=1, int *saved_idx=0)=0;
    virtual bool	 bindMatrix4(RE_Render *r,
				     const char *name, const fpreal32 *val,
				     int array_size=1, int *saved_idx=0)=0;

    virtual bool	 bindMatrix2(RE_Render *r,
				     const char *name, const fpreal64 *val,
				     int array_size=1, int *saved_idx=0)=0;
    virtual bool	 bindMatrix3(RE_Render *r,
				     const char *name, const fpreal64 *val,
				     int array_size=1, int *saved_idx=0)=0;
    virtual bool	 bindMatrix4(RE_Render *r,
				     const char *name, const fpreal64 *val,
				     int array_size=1, int *saved_idx=0)=0;

    /// Sampler buffer bound to a texture. The shader manages the texture
    /// unit assignment, the binding of the texture to the texture unit, and
    /// the assingment of the sampler buffer uniform.
    virtual bool	 bindTexture(RE_Render *r,
				     const char *name,
				     RE_Texture *tex,
				     int *saved_idx=0)=0;
    //@}

    /// Bind a texture to an image slot for reading, writing, or both.
    /// Requires RE_EXT_IMAGE_LOAD_STORE (or GL4.2).
    virtual bool	 bindImage(RE_Render *r,
				   const char *name,
				   RE_Texture *image,
				   RE_BufferAccess image_access) = 0;
    
    /// Bind a texture array layer, cubemap layer, or a 3D texture slice
    /// to an image slot for reading, writing, or both.
    /// Requires RE_EXT_IMAGE_LOAD_STORE (or GL4.2).
    virtual bool bindImageLayer(RE_Render *r,
				const char *name,
				RE_Texture *image,
				RE_BufferAccess image_access,
				int layer) = 0;

    /// assign a bindless texture to its handle uniform. 'texture' must have
    /// a texture handle, and uniform_name must refer to a texture handle.
    bool setTextureHandle(RE_Render *r,
			  const char *uniform_name,
			  RE_Texture *texture,
			  int *saved_idx = 0);

    /// Remove a bindless handle from the uniform, clearing it to 0 (no tex).
    bool clearTextureHandle(RE_Render *r,
			    const char *uniform_name,
			    int *saved_idx = 0);
    
    /// @brief Bind an RE_Uniform's values to the corresponding GL uniform
    /// Bind an RE_Uniform to this shader.  If 'uniform' is an array uniform
    /// then 'index' specifies which array element to bind.  'index' must be
    /// less than uniform->getSize().  Pass -1 for 'index' (the default) to
    /// bind all elements of an array; passing -1 for non-arrays is safe and
    /// is equivalent to passing 0.
    virtual bool	 bindUniform(RE_Render *r,
				     const RE_Uniform *uniform, int index = -1);

    /// returns true if the named uniform exists in the shader.
    bool		 hasUniform(const char *uniform_name) const;

    /// @brief Queries the use of a builtin uniform
    /// Returns true if this shader has the specified built-in uniform.
    /// 'uniform' must be an RE_UniformBuiltIn.  Note that certain
    /// built-in uniforms may only be supported for certain languages.
    bool		hasBuiltInUniform(RE_UniformBuiltIn uniform) const;

    /// Returns a list of RE_UniformBuiltIn IDs this shader uses.
    const UT_IntArray	&getBuiltInUniformList() const
						{ return myBuiltInUniforms; }

    /// @brief Bind all builtin uniforms.
    /// Bind all builtin uniforms this shader uses to their current values in
    /// RE_Render
    void		 bindBuiltIns(RE_Render *r,
				      bool force_update = false);

    /// Called to invalidate the cached uniform, usually by
    /// RE_Uniform::unregisterBuiltIn().
    void		 invalidateUniform(RE_UniformBuiltIn uniform);

    /// Prints all the active uniforms plus their values to stream os (or cerr)
    virtual void	 printUniforms(RE_Render *r,
				       std::ostream *os = nullptr) const {}
    
    // UNIFORM BLOCKS -------------------------------------------------------
    
    // RE_EXT_UNIFORM_BUFFER must be supported, or these will always return 0
    // NOTE: these methods are specifically for swapping blocks in and out for
    //	     optimization purposes.
    //       You do not need to use these directly to bind values to uniform
    //	     blocks - the bind...() methods will do that automatically.
    //       See RE_UniformBuffer.h for more information.

    /// Returns the OpenGL maximum number of supported blocks.
    virtual int		 getMaxUniformBlocks(RE_Render *) { return 0; }
    
    /// Returns the OpenGL maximum size of a uniform block, in bytes.
    virtual int		 getMaxUniformBlockSizeB(RE_Render *) { return 0; }

    
    /// Returns the number of blocks and specific uniform blocks.
    virtual int		     getNumUniformBlocks() const { return 0; }

    /// Return an attached uniform block by index
    virtual RE_UniformBlockHandle getUniformBlock(int ) const { return nullptr; }

    /// Return an attached uniform block by name
    virtual RE_UniformBlockHandle getUniformBlock(const char *name) const
							 { return nullptr; }

    /// Returns the block binding point of the named block.
    virtual int		     getUniformBlockIndex(const char *name) const
							 { return -1; }

    /// @brief Attach an RE_UniformBlock to this shader
    /// Attach a single uniform block to the shader. The shader owns the block
    /// while attached, and will delete it if the shader is deleted, or may
    /// delete it if linkShaders() is called and it is no longer in use.
    /// If you want to cache a block, detach it when you're done with the shader.
    /// If a block with the same name is already attached to this shader, it will
    /// be deleted.
    virtual bool	 attachUniformBlock(RE_UniformBlock *) { return false; }

    /// @brief Detach a named RE_UniformBlock from this shader
    /// Detach a single uniform block by name or pointer. The shader will no
    /// longer keep a reference to it, and will not delete it when relinking or
    /// when the shader is deleted.
    /// Note that a shader must have all blocks attached when executed. Detaching
    /// a uniform block does not remove the GL binding to the block, it merely
    /// removes this shader's ownership of the block (for performance reasons)
    virtual RE_UniformBlockHandle detachUniformBlock(const char *) {return nullptr;}

    /// Detach a specific RE_UniformBlock from this shader.
    virtual bool	 detachUniformBlock(RE_UniformBlock *) { return false; }

    /// Detaches all blocks referenced by this shader.
    virtual bool	 detachAllUniformBlocks() { return false; }

    /// Uploads attached uniform blocks that are dirty (uniform modifications)
    /// and ensures they are bound to the shader. 
    virtual bool	 bindAllUniformBlocks(RE_Render *) { return false; }

    /// @brief Initialize a uniform block based on this shader's block layout
    /// Initializes a uniform block with the proper offsets and size for block
    /// index 'index'. If index is -1, block must have the name of one of the
    /// uniform blocks in the shader.
    virtual bool	 initUniformBlock(RE_Render *r,
					  RE_UniformBlock *block,
					  int index = -1) const
			 { return false; }

    /// Copies the structure of the named uniform block, and possibly the data.
    virtual RE_UniformBlockHandle copyUniformBlock(const char *name,
					      bool data_too) const
			 { return nullptr; }

    /// Update an offset/name/size information in 'b' with the uniforms for the
    /// correspondingly-named uniform block in this shader.
    virtual bool	 updateUniformBlock(const RE_UniformBlockHandle &b) const
				    { return false; }

    /// Override the currently bound uniform block without deleting the block
    /// currently residing at that index. If 'block_index' is -1, the name of
    /// the block is used to find the block index. Only one override can be
    /// active at once. Passing nullptr for the block clears the override.
    virtual int		 overrideUniformBlock(RE_UniformBlock *block,
					      int block_index = -1)
			 { return -1; }
    /// Remove an override block from the shader, by index.
    virtual void	 removeOverrideBlock(int ) {}
    /// Remove a specific override block from the shader.
    virtual void	 removeOverrideBlock(RE_UniformBlock *b) {}
    /// Remove all override blocks from the shader.
    virtual void	 removeOverrideBlocks()
				{ myLightsId=-1; myLightsSerial=-1; }
    /// Return the currently bound override block for an index, if any is bound
    virtual RE_UniformBlockHandle getOverrideBlock(int ) const { return nullptr; }

    // STORAGE BUFFER BLOCKS -----------------------------------------------
    
    /// Create a buffer for buffer block 'name'. If it has an open-ended array,
    /// array_size specifies the size of that array, othwer it is ignored.
    virtual RE_UniformBlockHandle createStorageBlock(const char *name,
						     int array_size)
			    { return nullptr; }

    /// Return the block definition for storage block 'name'
    virtual RE_UniformBlockHandle getStorageBlock(const char *name)
			    { return nullptr; }

    /// Return the storage block bound to 'name' (bound by attachStorageBlock)
    virtual RE_UniformBlockHandle getStorageBlockBinding(const char *name)
			    { return nullptr; }

    /// Return the raw buffer bound to 'name' (bound by attachStorageBlock)
    virtual RE_VertexArray	 *getStorageBufferBinding(const char *name)
			    { return nullptr; }

    /// Attach a shader storage block to the binding point corresponding to its
    /// name. Shader storage blocks should be created with createStorageBlock().
    virtual bool	 attachStorageBlock(const RE_UniformBlockHandle &block)
			    { return false; }

    /// Detach a shader storage block from the binding point corresponding to
    /// its name. 
    virtual bool	 detachStorageBlock(const RE_UniformBlockHandle &block)
			    { return false; }

    /// Attach a GL buffer to the shader storage block binding point that shares
    /// its name (getAttributeName()). This method is for simple storage blocks,
    /// such as "buffer blockname { vec4 color[]; }". 
    virtual bool	 attachStorageBuffer(RE_VertexArray *raw_buffer)
			    { return false; }
    /// Detach a GL buffer from the shader storage block binding point that
    /// shares its name (raw_buffer->getAttributeName()).
    virtual bool	 detachStorageBuffer(RE_VertexArray *raw_buffer)
			    { return false; }

    /// Unbind the block or buffer attached to binding point 'bind_name'.
    virtual bool	 detachStorageBinding(const char *bind_name)
			    { return false; }
 
    // ATTRIBUTES ----------------------------------------------------------

    /// Return if the vertex shader has input attributes
    bool		 hasShaderAttribs() const
			   { return myAttribNames.entries() > 0; }

    /// Return the number of vertex shader input attributes
    int			 getNumShaderAttribs() const 
			   { return myAttribNames.entries(); }

    /// Return the name of vertex shader input at index 'i'
    const char		*getShaderAttribName(int i) const
			   { return myAttribNames(i); }

    /// Return the ID of the attribute if it is a known type, or
    /// RE_GENATTRIB_UNDEF if it is not.
    RE_GenericAttribID	 getShaderAttribGenID(int i) const;
    
    /// Return the GL attribute location of vertex shader input at index 'i'
    int			 getShaderAttribLocation(int i) const
			   { return myAttribLocations(i); }
    
    /// Return the type of vertex shader input at index 'i'
    RE_GPUType		 getShaderAttribType(int i) const
			   { return myAttribTypes(i); }

    /// Return the vector size of vertex shader input at index 'i'
    int			 getShaderAttribSize(int i) const
			   { return myAttribSizes(i); }
    
    /// Return a string array of all vertex shader input names.
    const UT_StringList &getShaderAttribNames() const
			   { return myAttribNames; }

    /// Return an int array of all vertex shader input locations.
    const UT_IntArray   &getShaderAttribLocations() const
			   { return myAttribLocations; }
    
    /// Return an int array of all vertex shader input sizes.
    const UT_IntArray   &getShaderAttribSizes() const
			   { return myAttribSizes; }

    /// Returns the name of a vertex shader input at GL location 'loc'
    const char *	 getAttribNameForLocation(int loc) const;

    /// Return the default GL location of generic vertex attribute 't'
    int			 getAttributeModeLoc(RE_GenericAttribID t) const
			    { return myAttrModeLocs(t); }
    
    /// Return the texture sampler unit of a texture buffer object for 't'
    int			 getAttributeSamplerLoc(RE_GenericAttribID t) const
			    { return myAttrSamplerLocs(t); }
	
    /// Return the number of GL-builtin vertex shader inputs (gl_Vertex, gl_*)
    int			 getNumBuiltinAttribs() const
			   { return myBuiltInAttribNames.entries(); }
    /// Return the name of GL-builtin vertex shader input at index 'i'
    const char *	 getBuiltinAttrib(int i) const
			   { return myBuiltInAttribNames(i); }

    /// Return the number of texture-buffer object attributes
    int			 getNumBufferAttribs() const
			   { return myBufferAttribNames.entries(); }
    /// Return the name of texture-buffer object attribute at index 'i'
    const char *	 getBufferAttrib(int i) const
			   { return myBufferAttribNames(i); }
    RE_GenericAttribID	 getBufferAttribID(int i) const
			   { return myBufferAttribGenIDs(i); }

    /// Return the number of texture buffer object samplers in this shader
    int			 getNumBufferTextures() const;
    /// Name of the texture buffer object sampler at index 'i'
    const char *	 getBufferTextureName(int i) const;
    /// Type of the texture buffer object sampler at index 'i' (float, int, uint)
    RE_GPUType		 getBufferTextureType(int i) const;
    /// True if the shader has a texture buffer object sampler named 'name'
    bool		 hasBufferTexture(const char *name) const;

    /// @brief Return information about vertex shader input 'attrib_name'
    /// Returns the location and size of the named vertex attribute, or false if
    /// it does not exist in this shader.
    /// This also includes the GL built-ins, though these will not set location
    /// type, or size; only return true/false for their presence in the shader.
    bool		 getAttribute(const char *attrib_name,
				      int &location,
				      RE_GPUType &type,
				      int &size) const;

    /// Simpler version of getAttribute() - just returns if the attribute exists
    bool		 hasAttribute(const char *attrib_name) const
			    {
				int loc,size;
				RE_GPUType t;
				return getAttribute(attrib_name,loc,t,size);
			    } 

    /// @brief Return information about vertex shader input at 'index'
    /// Similar to getAttribute(), but the name of the input does not need to be
    /// known. GL builtins are not supported by this method. index must be
    /// within [0, getNumAttribs()-1].
    bool		 getAttributeIndex(int	       index,
					   UT_String  &name,
					   int	      &location,
					   RE_GPUType &type,
					   int	      &size) const;

    void		 printAttributes(std::ostream *os = nullptr) const;

    /// @brief Zero out the constant attribute location given by 'array'
    /// Bind a constant zero for this array. If data is non-nullptr,
    /// this data is used, otherwise the current constant data for this array
    /// is used (default 0's).
    bool	 bindZeroConstant(RE_Render *r, RE_VertexArray *array);

    // TEXTURES ------------------------------------------------------------

    /// Loads texture maps specified by the RE_TextureMap bindTextureMap()
    virtual void	 loadShaderTexMaps(RE_Render *) = 0;
    virtual void	 loadBoundTextures(RE_Render *r) = 0;
    virtual void	 unloadBoundTextures(RE_Render *r) = 0;

    /// Returns the number of texture samplers used by shader 'stage'. As
    /// RE_ShaderType is a bitfield, multiple stages can be queried at once
    /// and the sum will be returned. Requires RE_EXT_PROGRAM_INTERFACE_QUERY.
    int			 getNumTextures(RE_ShaderType stage) const;

    /// Override texture compression for texture bound via bindTextureMap()
    // @{
    RE_TextureCompress	 getTextureFormat() const { return myTexFormat; }
    void		 setTextureFormat(RE_TextureCompress format)
						  { myTexFormat = format; }
    // @}

    /// Limit the resolution of the textures to a max width and height. 
    //@{
    void		 limitTextureSize(bool limit, int w, int h);
    bool		 limitTextureSize() const { return myLimitTextureSize; }
    
    int			 getMaxTextureWidth() const  { return myMaxTexW; }
    int			 getMaxTextureHeight() const { return myMaxTexH; }
    //@}

    /// Scale by a constant scale factor for textures bound via bindTextureMap()
    //@{
    void		 setTextureScale(float sc)   { myTextureScale = sc; }
    float		 getTextureScale() const     { return myTextureScale; } 
    //@}

    /// Enable or disable mipmapping for textures bound via bindTextureMap()
    //@{
    bool		 mipMapTextures() const { return myMipMapTexture; }
    void		 mipMapTextures(bool yes) { myMipMapTexture = yes; }
    //@}

    /// Set anisotropic filtering for textures bound via bindTextureMap()
    //@{
    void		 setAnisotropySamples(int samples)
			    { myTextureAnisotropy = samples; }
    int			 getAnisotropySamples() const
			    { return myTextureAnisotropy; }
    //@}

    /// Returns the shader source of some or all attached shaders
    void		 getShaderSource(RE_Render *r,
					 UT_String &source,
					 RE_ShaderType sh = RE_SHADER_ALL);

    /// @brief Prepare this shader for rendering
    /// Binds uniform buffer objects and all builtin uniforms. Optionally
    /// re-binds vertex attribute locations in 'obj'.
    bool		 prepForDrawing(RE_Render *r,
					RE_Geometry *obj = nullptr,
					RE_ElementArray *element = nullptr,
					RE_VertexState *state = nullptr,
					unsigned int vertex_stride = 0,
					RE_OverrideList *atrbover=0,
					int instance_group = 0);

    bool		 cleanupAfterDrawing(RE_Render *r,
					     RE_Geometry *obj=nullptr,
					     RE_VertexState *vstate = nullptr,
					     RE_OverrideList *atrbover=0);

    /// Cached RE_LightList information (uniform blocks)
    //@{
    bool		 matchesLights(int light_serial, int light_id) const;
    void		 setLightID(int light_serial, int light_id, int mask);
    int			 getLightMask() const { return myLightMask; }
    //@}

    /// COMPUTE
    virtual bool	 runCompute(RE_Render *r,
				    int num_wg_x,
				    int num_wg_y = 1,
				    int num_wg_z = 1);
    virtual bool	 runCompute(RE_Render *r,
				    RE_VertexArray *wg_buffer, int offset);

    /// @brief Fetch a shader based on GL program id
    /// Returns the RE_Shader object with the specified program object if it
    /// exists; otherwise, returns nullptr.
    static RE_Shader	*getShader(int program_object);

    // Loads only the source from the Houdini OGL shader path. Returns true if
    // the file was successfully loaded.
    static bool		loadFile(const char *filename,
				 const char *defines,
				 UT_WorkBuffer &src,
				 int &codeversion,
				 UT_String *messages,
				 int default_code_version = 0,
				 bool loadingInclude = false);
protected:
    virtual void	 removeTextureRefs(RE_OGLTexture *) {} 
    
    // Get the index of an attribute or uniform based on its name.  The
    // returned index can be used to look up corresponding elements in
    // the myAttrib*s arrays or the myUniform*s arrays, respectively.
    virtual int		 getAttributeIndex(const char *name) const;
	    int		 getUniformIndex(const char *name) const;

    // Creates/destroys a mapping from the specified program id to this shader
    // in the shader table.
    static void		 registerProgram(RE_Shader *shader, int program_object);

    // Loads any built-in Houdini shaders that loadShader() noted if
    // useHoudiniLibraryFuncs() is on.
    bool		 loadHoudiniLibraryFuncs(RE_Render *r, UT_String *msg);

    const char	       **getTransformFeedbackAttribs(int &num_attribs);


    static int		 loadProgramFile(RE_Render *r,
					 const char *file,
					 const char *basepath,
					 UT_String &prog_name,
					 UT_StringArray &files,
					 UT_StringArray &files_names,
					 UT_StringArray &inputs,
					 UT_StringArray &outputs,
					 UT_StringArray &defines,
					 int &codeversion,
					 fpreal &shading_rate,
					 bool &use_attrib_map,
					 bool &explicit_attrib_map,
					 int max_version,
					 RE_ShaderLanguage &shader_lang,
					 UT_String *msg);

    static RE_Texture *	 getDefaultTBO(RE_Render *r);
    
    struct re_NeededLibFuncs
    {
	re_NeededLibFuncs(const char *file, const char *func, RE_ShaderType s)
	    : myFile(file), myFunc(func), myStage(s), myLoaded(false) {}
	UT_StringHolder myFile;
	UT_StringHolder myFunc;
	RE_ShaderType   myStage;
	bool		myLoaded;
    };
    static void		 scanSourceForFuncs(const char	*line,
					    RE_ShaderType	 shtype,
					    UT_Array<re_NeededLibFuncs> &needed,
					    re_LibFunc	*library,
					    int		 glmajor,
					    int		 glminor);
    static bool		 getCodeBlock(const char *block_name,
				      UT_StringHolder &block_source);
    

    UT_String		 myProgramName;
    UT_String		 myExtraDefines;
    int			 myProgramObject;
    bool		 myLinkedFlag;
    bool		 myUseDefaultAttribMap;
    bool		 myUseExplicitAttribMap;
    bool		 myRegisterProgram;
    RE_TextureCompress	 myTexFormat;
    bool		 myLimitTextureSize;
    int			 myMaxTexW, myMaxTexH;
    float		 myTextureScale;
    bool		 myMipMapTexture;
    int			 myTextureAnisotropy;

    UT_ValArray<RE_ShaderStage *> myVertexShaders;
    UT_ValArray<RE_ShaderStage *> myTessControlShaders;
    UT_ValArray<RE_ShaderStage *> myTessEvalShaders;
    UT_ValArray<RE_ShaderStage *> myGeometryShaders;
    UT_ValArray<RE_ShaderStage *> myFragmentShaders;
    UT_ValArray<RE_ShaderStage *> myComputeShaders;

    UT_IntArray		 myNumTexturesPerStage;

    int			 myGeoShaderMaxVertices;
    RE_PrimType		 myGeoShaderInputPrims;
    RE_PrimType		 myGeoShaderOutputPrims;

    UT_StringList	 myBuiltInAttribNames;
    UT_StringList	 myAttribNames;
    UT_StringList	 myBufferAttribNames;
    UT_Array<RE_GenericAttribID> myBufferAttribGenIDs;
    UT_IntArray		 myAttribLocations;
    UT_Array<RE_GenericAttribID> myAttribGenIDs;
    UT_Array<RE_GPUType> myAttribTypes;
    UT_IntArray		 myAttribSizes;
    UT_StringList	 myFeedbackNames;
    
    UT_ValArray<RE_UniformData *> myUniforms;
    UT_IntArray		 myBuiltInUniforms;
    UT_IntArray		 myBuiltInVersion;
    UT_ValArray<const RE_Uniform *> myBuiltInLastUniform;
    UT_SymbolMap<int>	 myUniformTable;
    UT_SymbolMap<int>	 myUniformBlockTable;
    UT_IntArray		 myAttrModeLocs;
    UT_IntArray		 myAttrSamplerLocs;
    
    int			 myBuiltInMap[ RE_UNIFORM_BUILT_IN__count ];
    int64		 myBuiltInUniformSerial;
    int64		 myRenderSerial;

    RE_VertexMap	*myVertexMap;
    UT_IntArray		 myBufferTextures;

    UT_Array<re_NeededLibFuncs>	*myNeededLibraryFuncs;
    int			 myCodeVersion;

    RE_Geometry		*myTransformFeedbackGeometry;
    bool		 myTransformFeedbackGeometryOwned;
    UT_ValArray<re_VertexName *> myTransformFeedbackVertices;

    fpreal		 myShadingRate;
    UT_IntArray		 myTextureUniforms;
    UT_IntArray		 myImageUniforms;

    int			 myShaderSerial;
    int			 myLightsSerial;
    int			 myLightsId;
    int			 myLightMask;

    UT_IntArray		 myDrawBoundTextures; // scope: single draw.
    
public:
    // These are called by RE_Render. Use r->pushShader() or r->bindShader()
    // or r->popShader() instead
    // This should set the shader as current.
    virtual void	 applyShader(RE_Render *r, bool update_re = true) = 0;
    // This should do a cleanup and unset any current shader.
    virtual void	 removeShader(RE_Render *r, bool update_re = true) = 0;
    // This should cleanup after the shader, but leave it set as current.
    virtual void	 cleanup(RE_Render *r) = 0;
    
    // public only for RE_OGLRender.
    static void		 unregisterProgram(int program_object);
    static void		 removeAllTextureRefs(RE_OGLTexture *);
};


class RE_API RE_ShaderStage
{
public:
		RE_ShaderStage(RE_ShaderType stype,
				const char *name = 0);
    virtual    ~RE_ShaderStage();

    /// Returns the amount of main memory (NOT graphics memory!)
    /// owned by this RE_ShaderStage.
    virtual int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += myName.getMemoryUsage(false);
        mem += myFilename.getMemoryUsage(false);
        return mem;
    }

    const char *	getName() const		{ return myName; }
    RE_ShaderType	getShaderType() const	{ return myType; }

    const UT_String    &getFilename() const	{ return myFilename; }
    void		setFilename(const char * fname)
						{ myFilename.harden(fname); }

    // This maintains an internal cache of shader source code; once the shader
    // is loaded, the cached source is used.
    bool		loadShader(RE_Render *r,
				   const char *filename,
				   UT_String *messages = nullptr,
				   bool dump_source_on_error = true,
				   const char *defines = nullptr,
				   int default_code_version
				       = RE_Shader::USE_GLSL_BASE_VERSION);
    
    virtual bool	setSource(RE_Render *r,
				  const char *source_code,
				  int version, 
				  UT_String *messages = nullptr,
				  bool dump_source_on_error = true) = 0;
    virtual bool	getSource(RE_Render *r, UT_String &source);

    void		setID(int id) { myID = id; }
    int			getID() const { return myID; }

    // Returns the version of the shader (100,110,120,130,140,150,330,400,410).
    int			getCodeVersion() const { return myCodeVersion; }
protected:
    void		setCodeVersion(int v) { myCodeVersion=v; }
private:
    
    UT_String		myName;
    UT_String		myFilename;
    RE_ShaderType	myType;
    int			myID;
    int			myCodeVersion;
};

class RE_UniformData // private to RE - no RE_API
{
public:
    RE_UniformData()
	: type(RE_UNIFORM_FLOAT),
	  arraysize(0), topsize(0), topstride(0),
	  uniformblock(-1), dataoffset(0),
	  location(-1), tex_unit(-1), image_unit(-1),
	  value_cached(false)
	{}

    RE_UniformData(const RE_UniformData &data)
	: name(data.name, 1),
	  type(data.type),
	  arraysize(data.arraysize),
	  topsize(data.topsize),
	  topstride(data.topstride),
	  uniformblock(data.uniformblock),
	  dataoffset(data.dataoffset),
	  location(data.location),
	  tex_unit(data.tex_unit),
	  image_unit(data.image_unit),
	  value_cached(false)
	{}

    bool	typeMatch(const RE_UniformData *data) const
	{
	    return (type == data->type &&
		    arraysize == data->arraysize &&
		    dataoffset == data->dataoffset);
	}

    /// Returns the amount of main memory (NOT graphics memory!)
    /// owned by this RE_UniformData.
    int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += name.getMemoryUsage(false);
        return mem;
    }

    enum { cached_data_size = sizeof(UT_Matrix4D) };

    UT_String	    name;
    RE_UniformType  type;
    int		    arraysize;	// array size of member element, ie, int[4]
    int		    topsize;	// array size of buffer variable 
    int		    topstride;  // array stride of buffer variable
    int		    uniformblock;
    int		    dataoffset;
    int		    location;
    int		    tex_unit;   // for normal textures
    int		    image_unit; // for image-load-store images
    bool	    value_cached;
    char	    cached_data[ cached_data_size ];
};

/// --- Inlines

inline bool
RE_Shader::bindInt(RE_Render *r, const char *name, int v)
{
    return bindVariableInt(r, name, &v);
}

inline bool
RE_Shader::bindFloat(RE_Render *r, const char *name, fpreal32 v)
{
    return bindVariable1(r, name, &v);
}

inline bool
RE_Shader::bindDouble(RE_Render *r, const char *name, fpreal64 v)
{
    return bindVariable1(r, name, &v);
}
    
inline bool
RE_Shader::bindVector(RE_Render *r, const char *name, const UT_Vector2F &v)
{
    return bindVariable2(r, name, v.data());
}

inline bool
RE_Shader::bindColor(RE_Render *r, const char *name, const UT_Color &c)
{
    fpreal32 col[3];

    c.getRGB(col, col+1, col+2);
    return bindVariable3(r, name, col);
}
    
inline bool
RE_Shader::bindColor(RE_Render *r, const char *name, const UT_Color &c,
		     fpreal alpha)
{
    fpreal32 col[4];

    c.getRGB(col, col+1, col+2);
    col[3] = alpha;
    return bindVariable4(r, name, col);
}
    

inline bool
RE_Shader::bindVector(RE_Render *r, const char *name, const UT_Vector3F &v)
{
    return bindVariable3(r, name, v.data());
}

inline bool
RE_Shader::bindVector(RE_Render *r, const char *name, const UT_Vector4F &v)
{
    return bindVariable4(r, name, v.data());
}

inline bool
RE_Shader::bindVector(RE_Render *r, const char *name, const UT_Vector2D &v)
{
    return bindVariable2(r, name, v.data());
}

inline bool
RE_Shader::bindVector(RE_Render *r, const char *name, const UT_Vector3D &v)
{
    return bindVariable3(r, name, v.data());
}

inline bool
RE_Shader::bindVector(RE_Render *r, const char *name, const UT_Vector4D &v)
{
    return bindVariable4(r, name, v.data());
}


inline bool
RE_Shader::bindVector(RE_Render *r, const char *name, const UT_Vector2i &v)
{
    return bindVariableInt2(r, name, v.data());
}

inline bool
RE_Shader::bindVector(RE_Render *r, const char *name, const UT_Vector3i &v)
{
    return bindVariableInt3(r, name, v.data());
}

inline bool
RE_Shader::bindVector(RE_Render *r, const char *name, const UT_Vector4i &v)
{
    return bindVariableInt4(r, name, v.data());
}

inline bool
RE_Shader::bindVectors(RE_Render *r,
		       const char *name,
		       const UT_Vector2FArray &vals)
{
    return bindVariable2(r, name, vals(0).data(), vals.entries());
}

    
inline bool
RE_Shader::bindVectors(RE_Render *r, const char *name,
		       const UT_Vector4FArray &vals)
{
    return bindVariable4(r, name, vals(0).data(), vals.entries());
}

inline bool
RE_Shader::bindVectors(RE_Render *r, const char *name,
		       const UT_Vector2DArray &vals)
{
    return bindVariable2(r, name, vals(0).data(), vals.entries());
}

inline bool
RE_Shader::bindVectors(RE_Render *r, const char *name,
		       const UT_Vector4DArray &vals)
{
    return bindVariable4(r, name, vals(0).data(), vals.entries());
}

inline bool
RE_Shader::bindMatrix(RE_Render *r, const char *name, const  UT_Matrix2F &m)
{
    return bindMatrix2(r, name, m.data());
}

inline bool
RE_Shader::bindMatrix(RE_Render *r, const char *name, const UT_Matrix3F &m)
{
    return bindMatrix3(r, name, m.data());
}

inline bool
RE_Shader::bindMatrix(RE_Render *r, const char *name, const UT_Matrix4F &m)
{
    return bindMatrix4(r, name, m.data());
}

inline bool
RE_Shader::bindMatrix(RE_Render *r, const char *name, const UT_Matrix2D &m)
{
    return bindMatrix2(r, name, m.data());
}

inline bool
RE_Shader::bindMatrix(RE_Render *r, const char *name, const UT_Matrix3D &m)
{
    return bindMatrix3(r, name, m.data());
}

inline bool
RE_Shader::bindMatrix(RE_Render *r, const char *name, const UT_Matrix4D &m)
{
    return bindMatrix4(r, name, m.data());
}

inline RE_GenericAttribID
RE_Shader::getShaderAttribGenID(int i) const
{
    return myAttribGenIDs(i);
}

#endif
