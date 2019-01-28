/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_Uniform.h (Library, C++)
 *
 * COMMENTS:
 */

#ifndef __RE_Uniform__
#define __RE_Uniform__

#include "RE_API.h"

#include <UT/UT_Color.h>
#include <UT/UT_String.h>
#include <UT/UT_LinkList.h>
#include "RE_Types.h"

class RE_Render;
class RE_Shader;

// The members of this enumeration are used as indices into
// re_BuiltInUniform::theUniforms.  Thus, changing this enumeration requires
// updating re_BuiltInUniform::theUniforms.
enum RE_UniformBuiltIn
{
    RE_UNIFORM_EMISSION = 0,
    RE_UNIFORM_SPECULAR,
    RE_UNIFORM_DIFFUSE,
    RE_UNIFORM_AMBIENT,
    
    RE_UNIFORM_MATERIAL_PASS,
    RE_UNIFORM_ALPHA_PASS,
    RE_UNIFORM_RENDER_PASS,
    RE_UNIFORM_MATERIAL_LAYER,
    RE_UNIFORM_MATERIAL_GROUP,
    RE_UNIFORM_MATERIAL_MASK,
    RE_UNIFORM_ALPHA_CUTOFF,
    RE_UNIFORM_SPLIT_GROUP,
    RE_UNIFORM_ONION_SKIN_GROUP,
    
    RE_UNIFORM_GHOST_COLOR,
    RE_UNIFORM_SELECT_COLOR,
    RE_UNIFORM_SELECT_SECOND_COLOR,
    RE_UNIFORM_SELECT_MODE,
    RE_UNIFORM_SELECT_INSTANCE,
    RE_UNIFORM_FILL_SELECTION,
    RE_UNIFORM_COVERAGE_MASK,

    RE_UNIFORM_LIGHTING_ENABLED,
    RE_UNIFORM_LIGHT_MASK,
    RE_UNIFORM_REFLECT_ORIGIN,
    RE_UNIFORM_SCENE_IOR,
    
    RE_UNIFORM_VIEW_MATRIX,
    RE_UNIFORM_INV_VIEW_MATRIX,
    RE_UNIFORM_OBJECT_MATRIX,
    RE_UNIFORM_INV_OBJECT_MATRIX,
    RE_UNIFORM_NORMAL_MATRIX,
    RE_UNIFORM_PROJECT_MATRIX,
    RE_UNIFORM_INV_PROJECT_MATRIX,
    RE_UNIFORM_SCREEN_SIZE,
    RE_UNIFORM_SAMPLES,
    RE_UNIFORM_IS_ORTHO,
    
    RE_UNIFORM_USE_DEPTH_MAP,
    RE_UNIFORM_DEPTH_MAP,
    RE_UNIFORM_DEPTH_PROJECT,
    RE_UNIFORM_DEPTH_RANGE,
    RE_UNIFORM_DEPTH_OFFSET,

    RE_UNIFORM_WIRE_OVER,
    RE_UNIFORM_WIRE_COLOR,
    RE_UNIFORM_WIRE_THICKNESS,
    RE_UNIFORM_CONST_COLOR,
    RE_UNIFORM_CONST_COLOR_MAP,
    RE_UNIFORM_USE_CONST_COLOR_MAP,

    RE_UNIFORM_DECORATION_SCALE,
    RE_UNIFORM_DECORATION_FONT_PARMS,
    RE_UNIFORM_DECORATION_VISIBILITY,
    RE_UNIFORM_DECORATION_LOCATE,
    RE_UNIFORM_DECORATION_INSTANCE,

    RE_UNIFORM_PRIM_DIVISOR,
    RE_UNIFORM_PRIM_BASE,
    RE_UNIFORM_VERTEX_OFFSET,
    RE_UNIFORM_SELECTION_BASE,
    RE_UNIFORM_INSTANCE_BASE,
    RE_UNIFORM_PRIM_CONSTANT_ID,
    RE_UNIFORM_USE_INSTANCE_PRIM_ID,

    RE_UNIFORM_PICK_BASE_ID,
    RE_UNIFORM_PICK_COMPONENT_ID,
    RE_UNIFORM_PICK_AREA,
    RE_UNIFORM_PICK_HAS_MAP,
    RE_UNIFORM_PICK_MAP,

    RE_UNIFORM_COLOR_RAMP,
    RE_UNIFORM_COLOR_RAMP_ENABLE,
    RE_UNIFORM_COLOR_RANGE,
    RE_UNIFORM_COLOR_IGNORE_GEO,

    RE_UNIFORM_UV_FILL_BACKFACES,
    RE_UNIFORM_UV_BACKFACES_COLOR,
    RE_UNIFORM_UV_BACKFACES_SEL_COLOR,
    RE_UNIFORM_UV_FILL_ALPHA,

    RE_UNIFORM_MAX_LOCATED_ID,
    RE_UNIFORM_MIN_LOCATED_ID,
    RE_UNIFORM_USE_LOCATED_MAP,
    RE_UNIFORM_LOCATED_MAP,

    RE_UNIFORM_NUM_BONE_TRANSFORMS,

    RE_UNIFORM_COLOR_XFORM,
    RE_UNIFORM_COLOR_XFORM_REF_VEC,

    RE_UNIFORM_DEPTH_OCCLUSION,

    RE_UNIFORM_VISIBLE_FLAG,

    RE_UNIFORM_BUILT_IN__count
};
#define RE_UNIFORM_CUSTOM	RE_UNIFORM_BUILT_IN__count

// RE_Uniform::typeTypeSizes expects this ordering.  If you modify this
// enumeration, make sure that you also modify 'theTypeSizes' initialization.
enum RE_UniformType
{
    RE_UNIFORM_INVALID = -1,
    RE_UNIFORM_FLOAT = 0,
    RE_UNIFORM_FLOAT_VEC2,
    RE_UNIFORM_FLOAT_VEC3,
    RE_UNIFORM_FLOAT_VEC4,
    RE_UNIFORM_DOUBLE,
    RE_UNIFORM_DOUBLE_VEC2,
    RE_UNIFORM_DOUBLE_VEC3,
    RE_UNIFORM_DOUBLE_VEC4,
    RE_UNIFORM_INT,
    RE_UNIFORM_INT_VEC2,
    RE_UNIFORM_INT_VEC3,
    RE_UNIFORM_INT_VEC4,
    RE_UNIFORM_UNSIGNED_INT,
    RE_UNIFORM_UNSIGNED_INT_VEC2,
    RE_UNIFORM_UNSIGNED_INT_VEC3,
    RE_UNIFORM_UNSIGNED_INT_VEC4,
    RE_UNIFORM_UNSIGNED_INT64,
    RE_UNIFORM_BOOL,
    RE_UNIFORM_BOOL_VEC2,
    RE_UNIFORM_BOOL_VEC3,
    RE_UNIFORM_BOOL_VEC4,
    RE_UNIFORM_FLOAT_MAT2,
    RE_UNIFORM_FLOAT_MAT3,
    RE_UNIFORM_FLOAT_MAT4,
    RE_UNIFORM_DOUBLE_MAT2,
    RE_UNIFORM_DOUBLE_MAT3,
    RE_UNIFORM_DOUBLE_MAT4,
    RE_UNIFORM_SAMPLER_1D,
    RE_UNIFORM_SAMPLER_2D,
    RE_UNIFORM_SAMPLER_3D,
    RE_UNIFORM_SAMPLER_CUBE,
    RE_UNIFORM_SAMPLER_RECTANGLE,
    RE_UNIFORM_SAMPLER_1D_ARRAY,
    RE_UNIFORM_SAMPLER_2D_ARRAY,
    RE_UNIFORM_SAMPLER_2D_MULTISAMPLE,
    RE_UNIFORM_SAMPLER_2D_SHADOW,
    RE_UNIFORM_SAMPLER_BUFFER,
    RE_UNIFORM_INT_SAMPLER_BUFFER,
    RE_UNIFORM_UNSIGNED_INT_SAMPLER_BUFFER,
    RE_UNIFORM_IMAGE_1D,
    RE_UNIFORM_IMAGE_2D,
    RE_UNIFORM_IMAGE_3D,
    RE_UNIFORM_IMAGE_RECTANGLE,
    RE_UNIFORM_IMAGE_CUBE,
    RE_UNIFORM_IMAGE_BUFFER,
    RE_UNIFORM_IMAGE_1D_ARRAY,
    RE_UNIFORM_IMAGE_2D_ARRAY,
    RE_UNIFORM_IMAGE_CUBE_MAP_ARRAY,
    RE_UNIFORM_IMAGE_2D_MULTISAMPLE,
    RE_UNIFORM_IMAGE_2D_MULTISAMPLE_ARRAY,
    RE_UNIFORM_IMAGE_INT_1D,
    RE_UNIFORM_IMAGE_INT_2D,
    RE_UNIFORM_IMAGE_INT_3D,
    RE_UNIFORM_IMAGE_INT_RECTANGLE,
    RE_UNIFORM_IMAGE_INT_CUBE,
    RE_UNIFORM_IMAGE_INT_BUFFER,
    RE_UNIFORM_IMAGE_INT_1D_ARRAY,
    RE_UNIFORM_IMAGE_INT_2D_ARRAY,
    RE_UNIFORM_IMAGE_INT_CUBE_MAP_ARRAY,
    RE_UNIFORM_IMAGE_INT_2D_MULTISAMPLE,
    RE_UNIFORM_IMAGE_INT_2D_MULTISAMPLE_ARRAY,
    RE_UNIFORM_IMAGE_UINT_1D,
    RE_UNIFORM_IMAGE_UINT_2D,
    RE_UNIFORM_IMAGE_UINT_3D,
    RE_UNIFORM_IMAGE_UINT_RECTANGLE,
    RE_UNIFORM_IMAGE_UINT_CUBE,
    RE_UNIFORM_IMAGE_UINT_BUFFER,
    RE_UNIFORM_IMAGE_UINT_1D_ARRAY,
    RE_UNIFORM_IMAGE_UINT_2D_ARRAY,
    RE_UNIFORM_IMAGE_UINT_CUBE_MAP_ARRAY,
    RE_UNIFORM_IMAGE_UINT_2D_MULTISAMPLE,
    RE_UNIFORM_IMAGE_UINT_2D_MULTISAMPLE_ARRAY,
    
    RE_UNIFORM_TYPE__count
};

class RE_UniformStackEntry;

class RE_API RE_Uniform : public UT_LinkNode
{
public:
    // 'size' is the array size.
	     RE_Uniform(const char *name, RE_UniformType type, int size = 1);
	     RE_Uniform(RE_UniformBuiltIn built_in, int size);
    virtual ~RE_Uniform();

    bool		operator==(const RE_Uniform &uniform) const;

    // Removes the array subscript "[]" from the end of 'name' if there is one
    // and stores the result in 'out'.
    static void		 getArrayNameAndIndex(const char *str,
					      UT_String &name_out,
					      int *index_out);

    // Appends an array subscript to the name of this uniform and stores the
    // result in 'out'.  Only call this method for array uniforms.
    void		 getIndexedName(int index, UT_String &out)const;

    const UT_String	&getName() const { return myName; }

    // getSize() returns the array size or 1 if this uniform is not an array.
    // Note that GLSL allows arrays of size 1 so this method cannot be used
    // to determine whether or not this uniform is an array.
    int			 getSize() const { return myArraySize; }
    RE_UniformType	 getType() const { return myType; }
    RE_UniformBuiltIn	 getBuiltInType() const { return myUniformType; }
    int			 getByteSize() const { return myValueBytes; }

    // Get/set the value of this uniform.  If this uniform is an array
    // then 'index' specifies which array element value to return/set;
    // otherwise, its ignored.
    const void		*getValue(int index=0) const;
    bool		 setValue(const void *value, int index=0);
    bool		 setValues(const void *value);

    // Return the color or alpha for (R,G,B,A) color values. Will assert if the
    // uniform is not a color.
    UT_Color		 getColor(int index=0) const;
    fpreal		 getAlpha(int index=0) const;

    UT_Vector2F		 getVector2(int index=0) const;
    UT_Vector3F		 getVector3(int index=0) const;
    UT_Vector4F		 getVector4(int index=0) const;
    
    UT_Matrix3D		 getMatrix3(int index=0) const;
    UT_Matrix4D		 getMatrix4(int index=0) const;

    // Copy the values of 'copy' into this uniform. The type and size must
    // match. Returns true if the value changed.
    bool		 copy(const RE_Uniform *copy);

    // Copy the values in data into this uniform. The array must be at least
    // as large as the value returned by getByteSize(). Returns true if the
    // value changed.
    bool		 copyBytes(const void *data);

    // The version is incremented whenever the value of this uniform is
    // changed.  The version starts at 0.
    int			 getVersion() const { return myVersion; }

    void		 setAutoGenerated(bool gen=true)
			    { myAutoGeneratedFlag = gen; }
    bool		 isAutoGenerated() const {return myAutoGeneratedFlag;}

    // Returns an element of RE_UniformBuiltIn if the specified uniform's
    // type, size, and name match the RE_UniformBuiltIn's type, size, and
    // name and the uniform is supported for the specified language; otherwise,
    // returns -1.
    static int			 getBuiltIn(const RE_Uniform *uniform,
    					    int languages =
						RE_SHADER_LANGUAGE_ALL);
    static int			 getBuiltIn(const UT_String &name,
					    RE_UniformType type,
					    int languages =
						RE_SHADER_LANGUAGE_ALL);

    // Static member functions for retrieving information about built-in
    // uniforms.
    static const char		*getBuiltInName(RE_UniformBuiltIn built_in);
    static RE_UniformType	 getBuiltInType(RE_UniformBuiltIn built_in);

    static void			 initBuiltIns();

    // Specify that a given uniform object should represent a given built-in
    // uniform.  After a built-in has been registered, calling getBuiltIn(int)
    // returns the registered uniform object.  By specifying a uniform object
    // when calling unregisterBuilInt(), the built-in will only be unregistered
    // if the currently registered uniform object matches the specified object.
    static void			 registerBuiltIn(RE_Render *r,
						 RE_UniformBuiltIn  built_in,
						 const RE_Uniform &uniform);

    // A convenience version for single-index variables. 'data' must match the
    // uniform's data type.
    static void			 registerBuiltIn(RE_Render *r,
						 RE_UniformBuiltIn built_in,
						 const void *data = NULL);

    // Assign a new value to a previous registered uniform.
    static bool			 assignBuiltIn(RE_Render *r,
					       RE_UniformBuiltIn built_in,
					       const void *data);
    
    // Assign a range of values to a previously registered uniform array.
    // Element zero passed in 'data' always corresponds to element 'start' in
    // the uniform.
    static bool			 assignBuiltInArray(RE_Render *r,
						    RE_UniformBuiltIn built_in,
						    const void *data,
						    int start,
						    int num);
    
    // Remove the uniform. If registerBuiltIn was called, the unform is unbound
    // only. If assignBuiltIn was called, the uniform is unbound and deleted.
    static void			 unregisterBuiltIn(RE_Render *r,
						   RE_UniformBuiltIn built_in);

    void			 print() const;

    // For RE_Shader - caching of uniform index in the uniform.
    void		 storeShaderUniformIndex(const RE_Shader *sh,
						 int index) const;
    int			 getShaderUniformIndex(const RE_Shader *sh) const;

    void		 copyTo(RE_UniformStackEntry &entry) const;
    void		 copyFrom(const RE_UniformStackEntry &entry);


    static int		getVectorSize(RE_UniformType type);
    static bool		isFloat(RE_UniformType type);
    static bool		isInt(RE_UniformType type);
    static bool		isSampler(RE_UniformType type);

private:
    static const int		 theTypeSizes[RE_UNIFORM_TYPE__count];
    UT_String			 myName;
    RE_UniformType		 myType;
    int				 myArraySize;
    int				 myValueBytes;
    int				 myVersion;
    char			*myValues;
    bool			 myAutoGeneratedFlag;
    RE_UniformBuiltIn		 myUniformType;

    // uniform index caching
    mutable int			 myStoredIndex;
    mutable int			 myStoredShaderSerial;
    mutable const void		*myStoredShader; // void *, ptr compare only
};

class RE_UniformStackEntry
{
public:
    RE_UniformStackEntry()
	: myByteSize(0), myArraySize(0), myVersion(0), myData(NULL) {}
    ~RE_UniformStackEntry()
	{
	    if(myData)
		free(myData);
	}

    void *myData;
    int   myVersion;
    int   myArraySize;
    int   myByteSize;
};

#endif

