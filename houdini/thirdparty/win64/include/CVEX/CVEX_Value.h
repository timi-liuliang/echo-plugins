/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	CVEX_Value.h ( CVEX Library, C++)
 *
 * COMMENTS:	C++ interface to VEX.  This class is used to specify
 *		input/output variable data.
 */

#ifndef __CVEX_Value__
#define __CVEX_Value__

#include "CVEX_API.h"
#include <UT/UT_Array.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_PackedArrayOfArrays.h>
#include <VEX/VEX_PodTypes.h>
#include <VEX/VEX_VexTypes.h>		// for VEX_Type

typedef UT_Array<UT_StringHolder> CVEX_StringArray;

/// The CVEX_Type enum defines the VEX types available to CVEX.
typedef enum {
    CVEX_TYPE_INVALID = -1,	// Invalid type
    CVEX_TYPE_INTEGER,		// Integer
    CVEX_TYPE_FLOAT,		// Float
    CVEX_TYPE_VECTOR2,		// 2-tuple of float
    CVEX_TYPE_VECTOR3,		// 3-tuple of float
    CVEX_TYPE_VECTOR4,		// 4-tuple of float
    CVEX_TYPE_MATRIX2,		// 4-tuple of float
    CVEX_TYPE_MATRIX3,		// 9-tuple of float
    CVEX_TYPE_MATRIX4,		// 16-tuple of float
    CVEX_TYPE_STRING,		// string values
    CVEX_TYPE_INTEGER_ARRAY,	// Integer array
    CVEX_TYPE_FLOAT_ARRAY,	// Float array
    CVEX_TYPE_VECTOR2_ARRAY,	// 2-tuple of float array
    CVEX_TYPE_VECTOR3_ARRAY,	// 3-tuple of float array
    CVEX_TYPE_VECTOR4_ARRAY,	// 4-tuple of float array
    CVEX_TYPE_MATRIX2_ARRAY,	// 4-tuple of float array
    CVEX_TYPE_MATRIX3_ARRAY,	// 9-tuple of float array
    CVEX_TYPE_MATRIX4_ARRAY,	// 16-tuple of float array
    CVEX_TYPE_STRING_ARRAY,	// string array.
} CVEX_Type;

/// @brief A class representing a VEX value.
///
/// The CVEX_Value objects are created internally in the CVEX library.  They
/// provide a mapping from the CVEX library to the underlying VEX
/// representation of objects.
class CVEX_API CVEX_Value {
public:
     CVEX_Value(const UT_StringHolder &name,
     	     CVEX_Type type, VEX_Precision prec,
     	     bool varying);
    ~CVEX_Value();

    /// Query name of VEX value
    const UT_StringHolder &getName() const	{ return myName; }
    /// Query type of VEX value
    CVEX_Type		 getType() const	{ return myType; }
    void		 setType(CVEX_Type type);
    void		 setPrecision(VEX_Precision p) { myPrec = p; }
    /// Query whether the VEX value is an export (or read-only)
    bool		 isExport() const	{ return myExport; }
    /// Query whether the VEX value is uniform or varying
    bool		 isVarying() const	{ return myVarying; }
    /// Query the array size of the VEX value.
    /// @note This is @b not the used for VEX arrays, but rather represents the
    /// number of entries in the varying array.
    int			 getArraySize() const	{ return myArraySize; }

    /// Get the raw data pointer.
    /// For numeric types, the data refers to a raw array of data (depending on
    /// the getType() of the value.
    /// - CVEX_TYPE_INTEGER     @c int
    /// - CVEX_TYPE_FLOAT       @c fpreal32
    /// - CVEX_TYPE_VECTOR2     @c UT_Vector2F
    /// - CVEX_TYPE_VECTOR3     @c UT_Vector3F
    /// - CVEX_TYPE_VECTOR4     @c UT_Vector4F
    /// - CVEX_TYPE_MATRIX2     @c UT_Matrix2F
    /// - CVEX_TYPE_MATRIX3     @c UT_Matrix3F
    /// - CVEX_TYPE_MATRIX4     @c UT_Matrix4F
    /// For CVEX_TYPE_STRING, the data will point to a UT_StringArray.
    void		*getRawData()	{ return myData; }

    bool		 isPackedArray() const { return myIsPackedArray; }

    /// The array size is NOT the size in bytes, but rather the array size of
    /// the input data.  It should either be 1 or N, where N is the number of
    /// values being computed by VEX.
    ///
    /// When passing data for output variables, please check to see whether
    /// the output variable is uniform or varying.  Varying values need to
    /// have a full array of data (including string arrays).
    ///
    /// The CVEX_Value object will hold a pointer to the CVEX_StringArray,
    /// so the data must be persistent.
    bool		 setTypedData(int32 *data, int array_size);
    bool		 setTypedData(fpreal32 *data, int array_size);
    bool		 setTypedData(UT_Vector2F *data, int array_size);
    bool		 setTypedData(UT_Vector3F *data, int array_size);
    bool		 setTypedData(UT_Vector4F *data, int array_size);
    bool		 setTypedData(UT_Matrix2F *data, int array_size);
    bool		 setTypedData(UT_Matrix3F *data, int array_size);
    bool		 setTypedData(UT_Matrix4F *data, int array_size);

    bool		 setTypedData(int64 *data, int array_size);
    bool		 setTypedData(fpreal64 *data, int array_size);
    bool		 setTypedData(UT_Vector2D *data, int array_size);
    bool		 setTypedData(UT_Vector3D *data, int array_size);
    bool		 setTypedData(UT_Vector4D *data, int array_size);
    bool		 setTypedData(UT_Matrix2D *data, int array_size);
    bool		 setTypedData(UT_Matrix3D *data, int array_size);
    bool		 setTypedData(UT_Matrix4D *data, int array_size);

    /// When setting values for string types, you need to pass in a
    /// CVEX_StringArray instead of a void *.  For input variables, the length
    /// of the array will determine whether the parameter is varying or
    /// uniform.
    ///
    /// The CVEX_Value object will hold a pointer to the CVEX_StringArray,
    /// so the data must be persistent.
    /// 
    /// When setting data for outputs, the packed arrays should be empty
    /// and the array size specified.
    bool		 setTypedData(CVEX_StringArray &stringdata);
    bool		 setTypedData(UT_Array<UT_Array<int32> > &intdata);
    bool		 setTypedData(UT_PackedArrayOfArrays<int32> &intdata,
				      int arraysize=0);
    /// This is stored as xyzxyzxyz for vector data.
    bool		 setTypedData(UT_Array<UT_Array<fpreal32> > &fltdata);
    bool		 setTypedData(UT_PackedArrayOfArrays<fpreal32> &fltdata,
				      int arraysize=0);
    bool		 setTypedData(UT_Array<UT_Array<int64> > &intdata);
    bool		 setTypedData(UT_PackedArrayOfArrays<int64> &intdata,
				      int arraysize=0);
    /// This is stored as xyzxyzxyz for vector data.
    bool		 setTypedData(UT_Array<UT_Array<fpreal64> > &fltdata);
    bool		 setTypedData(UT_PackedArrayOfArrays<fpreal64> &fltdata,
				      int arraysize=0);
    bool		 setTypedData(UT_Array<CVEX_StringArray> &stringdata);

    /// Type unsafe way of setting VEX data. Avoid using if possible.
    bool		 setRawData(CVEX_Type type, VEX_Precision, void *data, int array_size);
    

    // Deprecated equivalents. Please avoid, they're error-prone.
    SYS_DEPRECATED(13.0)
    void		*getData()
			 { return getRawData(); }

    SYS_DEPRECATED(13.0)
    bool 		 setData(void *data, int array_size)
			 { return setRawData(myType, VEX_32, data, array_size); }

    SYS_DEPRECATED(13.0)
    bool		 setData(CVEX_StringArray &stringdata)
			 { return setTypedData(stringdata); }

    static int		 getTupleSize(CVEX_Type t);
    static CVEX_Type	 getArrayBaseType(CVEX_Type t);

    /// The IsConstant determines eligibility for constant folding.
    /// It likely implies Uniform.  But it is quite reasonable to have
    /// a Uniform parameter that isn't constant, it is then a demand
    /// that the VEX Assemble does not fold the input regardless of
    /// the initial value during load()
    void		 setIsConstant(bool isconstant)
			 { myIsConstant = isconstant; }
    bool		 isConstant() const
			 { return myIsConstant; }

    /// Given the list of vex parameters, returns the list of cvex parmeters 
    /// that have valid cvex types.
    static VEX_Type	 cvexToVexType(CVEX_Type type, bool &isarray);
    static CVEX_Type	 vexToCVexType(VEX_Type vextype, bool isarray);

private:
    void		 clear();

    void		*myData;
    void		*myStringSet;
    void		*myArrayType;
    UT_StringHolder	 myName;
    CVEX_Type		 myType;	// Data type
    VEX_Type		 myVexType;	// VEX Type (mapped from CVEX_Type)
    VEX_Precision	 myPrec;        // VEX Data precision
    int			 myVexIndex;	// Offset into VEX symbol table
    int			 myArraySize;	// Array size
    bool		 myExport;	// Whether the data is output by VEX
    bool		 myVarying;	// Whether the data is varying
    bool		 myIsArray;	// Am I an array?
    bool		 myIsConstant;	// Am I eligible for constant folding?
    bool		 myIsPackedArray;	// If the array is packed.

    friend class	CVEX_Context;
};

#endif
