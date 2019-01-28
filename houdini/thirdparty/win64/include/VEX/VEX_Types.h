/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	VEX_Types.h ( VEX Library, C++)
 *
 * COMMENTS:	Data types for VEX variables
 */

#ifndef __VEX_Types__
#define __VEX_Types__

#include "VEX_API.h"
#include <SYS/SYS_StaticAssert.h>
#include <UT/UT_Vector3.h>
#include <UT/UT_Vector4.h>
#include <UT/UT_Matrix3.h>
#include <UT/UT_Matrix4.h>
#include "VEX_PodTypes.h"
#include "VEX_VexTypes.h"
#include "VEX_RefObject.h"

template <VEX_Precision PREC> class VEX_Value;
template <VEX_Precision PREC> class VEX_Instance;
class VEX_ProcArray;
class VEX_OpCode;
class FS_DiskCache;

// Two way type resolver
//
// VEX_VexTypeResolver: C++ Type -> Vex Type
// VEX_TypeResolver: Vex Type + PREC -> C++ Type
//

template <typename T>
struct VEX_VexTypeResolver;
template <VEX_Type VTYPE, VEX_Precision PREC>
struct VEX_TypeResolver;


#define VEX_TYPERESOLVE_CASE(VexType, CPPType) \
    template <> \
    struct VEX_VexTypeResolver<VEX##CPPType<VEX_32> > \
    { \
    	static constexpr VEX_Type type = VEX_TYPE_##VexType; \
    }; \
    template <> \
    struct VEX_VexTypeResolver<VEX##CPPType<VEX_64> > \
    { \
    	static constexpr VEX_Type type = VEX_TYPE_##VexType; \
    }; \
    template <VEX_Precision PREC> \
    struct VEX_TypeResolver<VEX_TYPE_##VexType, PREC> \
    { \
    	typedef VEX##CPPType<PREC> T; \
    };
    /* end of macro */

VEX_TYPERESOLVE_CASE(FLOAT, float);
VEX_TYPERESOLVE_CASE(INTEGER, int);
VEX_TYPERESOLVE_CASE(VECTOR2, vec2);
VEX_TYPERESOLVE_CASE(VECTOR, vec3);
VEX_TYPERESOLVE_CASE(VECTOR4, vec4);
VEX_TYPERESOLVE_CASE(MATRIX2, mat2);
VEX_TYPERESOLVE_CASE(MATRIX3, mat3);
VEX_TYPERESOLVE_CASE(MATRIX4, mat4);

template <>
struct VEX_VexTypeResolver<UT_StringHolder>
{
    static constexpr VEX_Type type = VEX_TYPE_STRING;
};
template <VEX_Precision PREC>
struct VEX_TypeResolver<VEX_TYPE_STRING, PREC>
{
    typedef UT_StringHolder T;
};
template <>
struct VEX_VexTypeResolver<VEX_RefObject*>
{
    static constexpr VEX_Type type = VEX_TYPE_BSDF;
};
template <VEX_Precision PREC>
struct VEX_TypeResolver<VEX_TYPE_BSDF, PREC>
{
    typedef VEX_RefObject* T;
};

#undef VEX_TYPERESOLVE_CASE


enum VEX_Storage
{
    VEX_STORE_UNDEF,		// Undefined until a later date

    VEX_STORE_PARAMETER,	// Possibly bound to some other source
    VEX_STORE_LOCAL,		// Local variable - storage deferred until bind
    VEX_STORE_CONST,		// Constant
    VEX_STORE_GLOBAL,		// Global variable

    VEX_MAX_STORAGE		// Last entry is a counter only
};

enum VEX_BaseType
{
    VEX_BASETYPE_FLOAT,         // Float
    VEX_BASETYPE_INT,           // Integer
    VEX_BASETYPE_STRING,        // String
    VEX_BASETYPE_BSDF,          // BSDF
};

enum VEX_UIHint
{
    VEX_UI_NONE,		// No ui hint
    VEX_UI_COLOR,		// Parameter represents a color
    VEX_UI_DIRECTION,		//	a direction vector
    VEX_UI_VECTOR,		//	a position vector
    VEX_UI_VECTOR4,		//	a position vector 4
    VEX_UI_UV,			//	a uv position
    VEX_UI_UVW,			//	a uvw position
    VEX_UI_ANGLE,		//	an angle
    VEX_UI_TOGGLE,		//	a boolean toggle
    VEX_UI_BUTTON,		//	a button for executing a callback
    VEX_UI_FILE,		//	a filename
    VEX_UI_IMGFILE,		//	an image file/ramp
    VEX_UI_GEOFILE,		//	a geometry file
    VEX_UI_OPPATH,		//	an operator path
    VEX_UI_OPLIST,		//	a list of operator paths
    VEX_UI_EMBED,		//	an embedded set of parameters
    VEX_MAX_UI
};

class VEX_RWType {
public:
    VEX_RWType()
	: myRead(false)
	, myWrite(false) {}
    VEX_RWType(bool read, bool write)
	: myRead(read)
	, myWrite(write) {}

    void	setRead(bool r = true)	{ myRead = r; }
    void	setWrite(bool w = true)	{ myWrite = w; }
    bool	isRead() const		{ return myRead; }
    bool	isWrite() const		{ return myWrite; }

    bool	operator==(const VEX_RWType &rhs) const
		{ return myRead == rhs.myRead &&
			 myWrite == rhs.myWrite; }
    bool	operator!=(const VEX_RWType &rhs) const
		{ return !(*this == rhs); }

    const char	*asString() const
		 {
		     if (myRead && myWrite) return "modify";
		     if (myRead) return "read";
		     if (myWrite) return "write";
		     return "unused";
		 }

private:
    bool	myRead;
    bool	myWrite;
};

static const VEX_RWType	 VEX_RW_NONE(false, false);
static const VEX_RWType	 VEX_RW_READ(true, false);
static const VEX_RWType	 VEX_RW_WRITE(false, true);
static const VEX_RWType	 VEX_RW_MODIFY(true, true);

VEX_API const char	*VEXgetType(VEX_Type type);
VEX_API char		 VEXgetMangleType(VEX_Type type);
VEX_API const char	*VEXgetBaseType(VEX_BaseType type);
VEX_API VEX_BaseType	 VEXgetBaseType(VEX_Type type);
VEX_API VEX_Type	 VEXgetType(const char *label);
VEX_API VEX_Type	 VEXgetType(const char *label, bool &isarray);
// Dangerous as there is no unique mapping.
// VEX_API VEX_Type	 VEXgetType(int size);
VEX_API VEX_Type	 VEXgetMangleType(char mangle);
VEX_API const char	*VEXgetStorage(VEX_Storage type);
VEX_API const char	*VEXgetShortStorage(VEX_Storage type);
VEX_API VEX_Storage	 VEXgetStorage(const char *label);
VEX_API const char	*VEXgetUIHint(VEX_UIHint type);
VEX_API VEX_UIHint	 VEXgetUIHint(const char *label);

SYS_FORCE_INLINE constexpr bool
VEXisPrecisioned(VEX_Type type)
{
    switch (type)
    {
    case VEX_TYPE_FLOAT: return true;
    case VEX_TYPE_INTEGER: return true;
    case VEX_TYPE_VECTOR2: return true;
    case VEX_TYPE_VECTOR: return true;
    case VEX_TYPE_VECTOR4: return true;
    case VEX_TYPE_MATRIX2: return true;
    case VEX_TYPE_MATRIX3: return true;
    case VEX_TYPE_MATRIX4: return true;
    default: return false;
    }
    return false;
}


VEX_API FS_DiskCache	&VEXcompileCache();

template <VEX_Precision PREC> static inline constexpr size_t
VEXsizeof(VEX_Type type)
{
    switch (type)
    {
	case VEX_TYPE_VECTOR4:	return sizeof(VEXvec4<PREC>);
	case VEX_TYPE_VECTOR:	return sizeof(VEXvec3<PREC>);
	case VEX_TYPE_VECTOR2:	return sizeof(VEXvec2<PREC>);
	case VEX_TYPE_FLOAT:	return sizeof(VEXfloat<PREC>);
	case VEX_TYPE_INTEGER:	return sizeof(VEXint<PREC>);
	case VEX_TYPE_STRING:	return sizeof(UT_StringHolder);
	case VEX_TYPE_MATRIX2:	return sizeof(VEXmat2<PREC>);
	case VEX_TYPE_MATRIX3:	return sizeof(VEXmat3<PREC>);
	case VEX_TYPE_MATRIX4:	return sizeof(VEXmat4<PREC>);
	case VEX_TYPE_BSDF:	return sizeof(VEX_RefObject *);
	default:		break;
    }
    return 0;
}

VEX_API int		 VEXgetArgCount(VEX_Type type);	  // Tuple size
VEX_API int		 VEXgetFloatCount(VEX_Type type); // Float-tuple size

template <typename T> static inline constexpr int VEXtupleSize()
    { SYS_UNIMPLEMENTED_TEMPLATE(T); return 0; }
template <> inline constexpr int VEXtupleSize<VEXint<VEX_32>>() { return 1; }
template <> inline constexpr int VEXtupleSize<VEXint<VEX_64>>() { return 1; }
template <> inline constexpr int VEXtupleSize<VEXfloat<VEX_32>>() { return 1; }
template <> inline constexpr int VEXtupleSize<VEXfloat<VEX_64>>() { return 1; }
template <> inline constexpr int VEXtupleSize<VEXvec2<VEX_32>>() { return 2; }
template <> inline constexpr int VEXtupleSize<VEXvec2<VEX_64>>() { return 2; }
template <> inline constexpr int VEXtupleSize<VEXvec3<VEX_32>>() { return 3; }
template <> inline constexpr int VEXtupleSize<VEXvec3<VEX_64>>() { return 3; }
template <> inline constexpr int VEXtupleSize<VEXvec4<VEX_32>>() { return 4; }
template <> inline constexpr int VEXtupleSize<VEXvec4<VEX_64>>() { return 4; }
template <> inline constexpr int VEXtupleSize<VEXmat2<VEX_32>>() { return 4; }
template <> inline constexpr int VEXtupleSize<VEXmat2<VEX_64>>() { return 4; }
template <> inline constexpr int VEXtupleSize<VEXmat3<VEX_32>>() { return 9; }
template <> inline constexpr int VEXtupleSize<VEXmat3<VEX_64>>() { return 9; }
template <> inline constexpr int VEXtupleSize<VEXmat4<VEX_32>>() { return 16; }
template <> inline constexpr int VEXtupleSize<VEXmat4<VEX_64>>() { return 16; }

#endif

