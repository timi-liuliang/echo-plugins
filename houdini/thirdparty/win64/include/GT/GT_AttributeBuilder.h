/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_AttributeBuilder.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_AttributeBuilder__
#define __GT_AttributeBuilder__

#include "GT_API.h"
#include "GT_AttributeList.h"
#include "GT_BuilderStatus.h"
#include "GT_DAConstantValue.h"
#include "GT_DANumeric.h"
#include "GT_DataArray.h"

#include <UT/UT_Array.h>
#include <UT/UT_NonCopyable.h>
#include <UT/UT_Pair.h>
#include <UT/UT_SymbolTable.h>
#include <UT/UT_Variadic.h>

#include <stdarg.h>

typedef UT_Pair<const char *, UT_VariadicPODType>	GT_Attribute;
typedef UT_VariadicT<GT_Attribute>			GT_VariadicAttributes;

class GT_API GT_AttributeBuilder : UT_NonCopyable
{
public:
     GT_AttributeBuilder(GT_Size size);
    ~GT_AttributeBuilder();

    /// The parse method will take a NULL terminated argument list and parse
    /// attributes using a very simple heuristic.  Attributes are specified by
    /// a name/value pair.  The name includes binding class, storage and tuple
    /// size information (possibly type information).  The value is an array
    /// which contains enough data to represent the data.  A copy of the data
    /// is made.
    ///
    /// The name consists of several parts:
    /// <b>Binding Class</b>
    /// The binding class is one of:
    ///	- @c detail, @c constant @n a single value shared over all primitives
    /// - @c primitive, @c uniform @n a value for each primitive
    /// - @c point, @c varying @n a value for each shared point
    /// - @c vertex, @c facevarying @n a value for each vertex
    /// <b> Storage Class </b>
    /// - @c int32, @c int @n A 32 bit integer
    /// - @c int64, @c long @n A 64 bit integer
    /// - @c real16, @c fpreal16, @c float @n A 16 bit float
    /// - @c real32, @c fpreal32, @c float @n A 32 bit float
    /// - @c real64, @c fpreal64, @c double @n A 64 bit float
    /// - @c point16, @c point @n A 16 bit position value (3-tuple)
    /// - @c point32, @c point @n A 32 bit position value (3-tuple)
    /// - @c point64, @n A 64 bit position value (3-tuple)
    /// - @c vector16, @c vector @n A 16 bit direction vector value (3-tuple)
    /// - @c vector32, @c vector @n A 32 bit direction vector value (3-tuple)
    /// - @c vector64, @n A 64 bit direction vector value (3-tuple)
    /// - @c normal16/32/64, @c normal @n A 16/32/64 bit normal vector value (3-tuple)
    /// - @c color16/32/64, @c color @n A 16/32/64 bit color value (3-tuple)
    /// - @c matrix32 @n A 32 bit transformation matrix (16-tuple)
    /// - @c matrix64, @c matrix @n A 64 bit transform matrix (16-tuple)
    /// - @c string @n An array of chars (i.e. const char *)
    /// <b> Array Size </b>
    /// Optionally, an array size can be specified by using <tt>[size]</tt>
    /// notation.  Please note that a "point32[3]" would require 9 fpreal32
    /// values per array entry.
    ///
    /// For example: @code
    ///		parse(vertex, point, primitive, detail,
    ///			"varying point P", P,
    ///			"varying normal N", N,
    ///			"vertex float uv[2]", uv,
    ///			"uniform color Cd", Cd,
    ///			"constant matrix xform", xform);
    /// @endcode

    /// It is possible to pass in the same GT_AttributeBuilder for different
    /// binding classes.  For example, for a GT_PrimSphere, you might
    /// have something like:@code
    ///		parse(detail, detail, detail, detail, ...);
    /// @endcode
    /// which will build all attributes in the same builder (@c detail)

    /// If a variable is specified multiple times, the value will be used for
    /// multi-segment motion blur.  At the current time, all variables which
    /// have multiple segments should have the same number of segments
    /// specified.  It is not necessary to specify segments for all variables,
    /// only ones which change over time.  For example @code
    ///		parse(vertex, point, primitive, detail,
    ///			"vertex point P", P0,	// P at time 0
    ///			"vertex point P", P1,	// P at time 1
    ///			"vertex normal N", N0,	// N at time 0
    ///			"vertex normal N", N1,	// N at time 1
    ///			"vertex float uv[2]", uv,
    ///			"constant matrix xform", xform);
    /// @endcode
    static bool	parse(GT_BuilderStatus &err,
			GT_AttributeBuilder &vertex,
			GT_AttributeBuilder &point,
			GT_AttributeBuilder &primitive, 
			GT_AttributeBuilder &detail,
			const GT_VariadicAttributes &args);

    bool	append(GT_BuilderStatus &status,
			const char *name, const GT_DataArrayHandle &h);

    /// Add a named value to the attribute list
    template <typename T_POD>
    bool	add(GT_BuilderStatus &status, const char *name,
			const T_POD *data, int tuple_size,
			GT_Type typeinfo = GT_TYPE_NONE)
		{
		    GT_DataArray	*array;
		    // Do a quick check to see if the array is constant
		    if (GT_DAConstantValue<T_POD>::isConstantValue(
				    myArraySize, data, tuple_size))
		    {
			array = new GT_DAConstantValue<T_POD>(
					myArraySize, data, tuple_size);
		    }
		    else
		    {
			GT_DANumeric<T_POD>		*narray;
			narray = new GT_DANumeric<T_POD>(myArraySize,
					tuple_size, typeinfo);
			narray->copyFrom(data);
			array = narray;
		    }
		    return append(status, name, GT_DataArrayHandle(array));
		}

    bool	addString(GT_BuilderStatus &status, const char *name,
			const char *const*data, int tuple_size);

    /// Make an attribute list for the attributes defined by parsing
    GT_AttributeListHandle	makeList();

    enum {
	GT_BINDING_VERTEX,	// Vertex/facevarying class
	GT_BINDING_POINT,	// Point/varying class
	GT_BINDING_PRIMITIVE,	// Primitive/uniform binding class
	GT_BINDING_DETAIL	// Detail binding
    };
    /// Guess at the binding, storage and type information.  The values passed
    /// in are not modified.
    static bool	guessTypeInformation(GT_BuilderStatus &err,
				    const char *token,
				    UT_WorkBuffer &name,
				    int &binding_class,
				    GT_Storage &storage,
				    GT_Type &type,
				    GT_Size &tuple_size);

private:
    class MapEntry {
	public:
	    MapEntry()
		: myIndex(-1)
	    {
	    }
	    ~MapEntry() {}
	    UT_Array<GT_DataArrayHandle>	myList;
	    int					myIndex;
    };

    UT_SymbolMap<MapEntry *>	myArrays;
    GT_Size			myArraySize;
    int				mySegments;
};

#endif
