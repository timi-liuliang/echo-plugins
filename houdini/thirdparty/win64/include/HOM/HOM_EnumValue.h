/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 *	An enum value object is an immutable object inside a module containing
 *	other enum values.
 */

#ifndef __HOM_EnumValue_h__
#define __HOM_EnumValue_h__

#include "HOM_API.h"
#include "HOM_Defines.h"
#include "HOM_PtrOrNull.h"
#include <SYS/SYS_Math.h>
#include <string>
#include <typeinfo>

SWIGOUT(%rename(EnumValue) HOM_EnumValue;)

class HOM_API HOM_EnumValue
{
public:
    SWIGOUT(%ignore HOM_EnumValue;)
    SWIGOUT(%ignore ~HOM_EnumValue;)

    HOM_EnumValue()
    {
	// This only exists to keep std::map<> happy.
	myEnumClass = &typeid(void);
	myRepr = "";
	myName = "";
	myId = 0;
    }
    
    // The id is a unique id for this enumeration value with its set of values.
    // We use the type info to quickly compare two enum values to make sure
    // they're from the same set of values.  We use repr for the __repr__
    // method (unfortunately, type_info.name() varies between platforms,
    // contains mangling information, and contains the "HOM_" prefix).  We
    // assume that repr is a string constant (we don't store a deep copy).
    // We similarly use name to hold the name of this particular enum value.
    HOM_EnumValue(const std::type_info& enum_class, const char *repr,
		  const char *name, int id)
    : myEnumClass(&enum_class), myRepr(repr), myName(name), myId(id)
    {}

    // Swig doesn't properly let you ignore only one overloaded operator==.
    // So, the methods wrapped by swig for Python are named __eq__.
    bool __eq__(HOM_PtrOrNull<HOM_EnumValue> value)
    { return value.myPointer && *this == *value.myPointer; }

    bool __ne__(HOM_PtrOrNull<HOM_EnumValue> value)
    { return !__eq__(value); }

    std::string __repr__() const
    { return myRepr; }

    int __hash__()
    { return (int)SYSmultiplicative_inthash(myId); }

    std::string name() const
    { return myName; }

    // These methods are used by the implementation and are not wrapped by
    // swig.
    SWIGOUT(%ignore operator==;)
    bool operator==(const HOM_EnumValue &value) const
    { return *myEnumClass == *value.myEnumClass && myId == value.myId; }

    SWIGOUT(%ignore operator!=;)
    bool operator!=(const HOM_EnumValue &value) const
    { return *myEnumClass != *value.myEnumClass || myId != value.myId; }

    SWIGOUT(%ignore enumClass;)
    const std::type_info &enumClass() const
    { return *myEnumClass; }

    SWIGOUT(%ignore id;)
    int id() const
    { return myId; }

    bool operator<(const HOM_EnumValue &value) const
    {
	int	v = strcmp(myName, value.myName);
	return v < 0 || ( v == 0 && myId < value.myId );
    }

    SWIGOUT(%ignore operator=;)
    HOM_EnumValue &operator=(const HOM_EnumValue &that)
    {
	if (&that != this)
	{
	    myEnumClass = that.myEnumClass;
	    myRepr = that.myRepr;
	    myName = that.myName;
	    myId = that.myId;
	}
	return *this;
    }
    

private:
    const std::type_info *myEnumClass;
    const char *myRepr;
    const char *myName;
    int myId;
};


// Use the HOM_DECLARE_ENUM_MODULE macro to declare an enumeration module
// class in the header file.
//     HOM_DEFINE_ENUM_MODULE(enum_module, num_values, values_list)
//     - enum_module is the name of the module (without the HOM_) prefix.
//     - values_list is a comma-separated list of the enumeration values
//       (set this in a define)
//     - num_values is the number of enumeration values
// Use HOM_DEFINE_ENUM_MODULE to define its contents in the .C file.  It
// takes the same parameters.

// This pair of macros is used to build the class definition for the
// enumeration module.
#define HOM_DECLARE_ENUM_MODULE_START(enum_module) \
    SWIGOUT(%nodefaultctor HOM_ ## enum_module;) \
    SWIGOUT(%rename(enum_module) HOM_ ## enum_module;) \
    class HOM_API HOM_ ## enum_module \
    { \
    public:
#define HOM_DECLARE_ENUM_MODULE_END \
    };

#ifndef SWIG
#include <hboost/preprocessor/cat.hpp>
#include <hboost/preprocessor/arithmetic/dec.hpp>
#include <hboost/preprocessor/tuple/to_seq.hpp>
#include <hboost/preprocessor/tuple/rem.hpp>
#include <hboost/preprocessor/seq/transform.hpp>
#include <hboost/preprocessor/seq/for_each.hpp>
#include <hboost/preprocessor/stringize.hpp>

// For an enum value, we declare a static const HOM_EnumValue member.
// (It will be initialized with the HOM_DEFINE_ENUM_* macros).  For
// each "Foo" enum value we also declare a "Foo_Id" constant.  This constant
// is used so we can use enum values in switch statements in C++.  It doesn't
// need a definition because it's a static const int that's initialized
// inside the class declaration.
//
// r will be the iteration value (plus 1, for some reason).  We use it
// to give each enumeration value a unique id number.
#define HOM_DECLARE_ENUM_VALUE(r, unused_data, elem) \
    static HOM_EnumValue elem; \
    static const int HBOOST_PP_CAT(elem, _Id) = HBOOST_PP_DEC(r);

// Call HOM_DECLARE_ENUM_VALUE for each enumeration value in the sequence.
// The whole thing is wrapped in the class definition.
#define HOM_DECLARE_ENUM_MODULE_FROM_SEQ(enum_module, values_seq) \
    HOM_DECLARE_ENUM_MODULE_START(enum_module) \
    HBOOST_PP_SEQ_FOR_EACH(HOM_DECLARE_ENUM_VALUE, _, values_seq) \
    HOM_DECLARE_ENUM_MODULE_END

// We wrap the comma-separated tuples of values in () to build what the boost
// preprocessor calls a tuple.  We then convert that to a boost sequence
// and call the above macro.  This macro is more convenient to use than the
// one above, though you might need to use the one above if you hit boost's
// maximum tuple size.
#define HOM_DECLARE_ENUM_MODULE(enum_module, num_values, values_tuple) \
    HOM_DECLARE_ENUM_MODULE_FROM_SEQ(enum_module, \
	HBOOST_PP_TUPLE_TO_SEQ(num_values, (values_tuple)))


// For each enum value we provide the definition of the static member variable,
// passing the enum class's type_info, the enum value's repr string, and
// the enum value's id into the constructor.
#define HOM_DEFINE_ENUM_VALUE(r, enum_module, elem) \
    HOM_EnumValue HBOOST_PP_CAT(HOM_, enum_module)::elem \
	(typeid(HBOOST_PP_CAT(HOM_, enum_module)), \
	 HBOOST_PP_STRINGIZE(enum_module) "." HBOOST_PP_STRINGIZE(elem), \
	 HBOOST_PP_STRINGIZE(elem), \
	 HBOOST_PP_DEC(r)); 

// The .C file contains all the definitions for the static member variables.
#define HOM_DEFINE_ENUM_MODULE_FROM_SEQ(enum_module, values_seq) \
    HBOOST_PP_SEQ_FOR_EACH(HOM_DEFINE_ENUM_VALUE, enum_module, values_seq)

#define HOM_DEFINE_ENUM_MODULE(enum_module, num_values, values_tuple) \
    HOM_DEFINE_ENUM_MODULE_FROM_SEQ(enum_module, \
	HBOOST_PP_TUPLE_TO_SEQ(num_values, (values_tuple)))

// Use HOM_DECLARE_ENUM_VALUE2 to declare an enum with a specific value. 
// enum_module: enum module name (ignored)
// enum_elem: enum element name 
// ienum_elem: internal enum element (ignored)
// ienum_elem_name: internal enum element name (ignored)
// enum_value: enum value
// E.g. usage:
// HOM_DECLARE_ENUM_MODULE_START(myEnumModule)
// HOM_DECLARE_ENUM_VALUE2(myEnumModule,myValue,0)
// HOM_DECLARE_ENUM_MODULE_END
#define HOM_DECLARE_ENUM_VALUE2(enum_module, enum_elem, ienum_elem, ienum_elem_name, enum_value) \
    static HOM_EnumValue enum_elem; \
    static const int HBOOST_PP_CAT(enum_elem,_Id) = enum_value;

// Use HOM_DEFINE_ENUM_VALUE2 to define an enum with a specific id number.
// enum_module: enum module name 
// enum_elem: enum element 
// ienum_elem: internal enum element name (ignored)
// ienum_elem_name: internal element name
// enum_value: enum value
#define HOM_DEFINE_ENUM_VALUE2(enum_module, enum_elem, ienum_elem, ienum_elem_name, enum_value) \
    HOM_EnumValue HBOOST_PP_CAT(HOM_,enum_module)::enum_elem \
	((typeid(HBOOST_PP_CAT(HOM_,enum_module))), \
	HBOOST_PP_STRINGIZE(enum_module) "." HBOOST_PP_STRINGIZE(enum_elem), \
	HBOOST_PP_STRINGIZE(enum_elem), \
	enum_value);

#else // SWIG

// Swig can't handle boost's proprocessor macros, so we create a simple
// class for swig to wrap that just contains static HOM_EnumValue members.
// Because values_tuple is a comma-separated list, we don't need any boost
// preprocessor stuff.
#define HOM_DECLARE_ENUM_MODULE(enum_module, num_values, values_tuple) \
    HOM_DECLARE_ENUM_MODULE_START(enum_module) \
    static const HOM_EnumValue values_tuple; \
    HOM_DECLARE_ENUM_MODULE_END

#define HOM_DECLARE_ENUM_VALUE2(enum_module, enum_elem, ienum_elem, ienum_elem_name, enum_value) \
    static const HOM_EnumValue enum_elem;

#endif // SWIG

#endif
