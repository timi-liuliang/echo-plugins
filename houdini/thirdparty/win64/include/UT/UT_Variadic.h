/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Variadic.h (UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_Variadic__
#define __UT_Variadic__

#include "UT_API.h"
#include "UT_Array.h"
#include <hboost/variant.hpp>

class UT_JSONWriter;
class UT_String;

/// Build a list of variadic arguments of the given template type
///
/// C-Style variadic arguments are error prone for various reasons:
///	- No type checking
///	- NULL termination issues
///
/// This class provides type-safety and a cleaner interface.  For example, to
/// accept a variable length array of strings: @code
/// typedef UT_VariadicT<const char *>	VariadicStrings;
/// void
/// function(const VariadicStrings &strings)
/// {
///     for (exint i = 0; i < strings.entries(); ++i)
///         cerr << i << " " << strings.item(i) << " " strlen(strings.item(i));
/// }
/// 
/// // Which can be called using
/// function(VariadicStrings() << "hello" << "world");
/// @endcode
template <typename T>
class UT_VariadicT
{
public:
     UT_VariadicT()
     {
     }
     UT_VariadicT(const UT_VariadicT<T> &src)
	 : myItems(src.myItems)
     {
     }
    ~UT_VariadicT()
    {
    }

    UT_VariadicT<T> &operator<<(const T &val)
    {
	myItems.append(&val);
	return *this;
    }

    exint	entries() const	{ return myItems.entries(); }
    const T	&item(exint arg) const	{ return *myItems(arg); }

    void	append(const T &v)	{ myItems.append(&v); }

protected:
    UT_Array<const T *>	myItems;
};

/// Typedef for a POD (int, real or const char *) variadic argument.  This is
/// equivalent to the c-style variadic arguments which can be processed.
typedef hboost::variant<int32, int64, fpreal64,
	    const char *,
	    const char **,
	    const int8 *,
	    const uint8 *,
	    const int16 *,
	    const uint16 *,
	    const int32 *,
	    const int64 *,
	    const fpreal16 *,
	    const fpreal32 *,
	    const fpreal64 *> UT_VariadicPODType;

/// Specialization of UT_VariadicT for simple int/float/string types.  This
/// class supports implicit casting from integer to float types.
///
/// UT_VariadicPOD allows you to easily create a type-safe variadic argument
/// list.  For example, a function that takes variadic args might be something
/// like: @code
///   // Instead of void function(...)
///   void function(const UT_Variadic &vargs)
///    {
///        fpreal           fval;
///        int64            ival;
///        const char      *sval;
///        for (exint i = 0; i < vargs.entries()-1; i += 2)
///        {
///            // First argument is expected to be a string
///            if (!vargs.import(i, sval)) return error("Expected keyword");
///            switch (getType(sval))
///            {
///                case 0:
///                    if (!vargs.import(i+1, ival))
///                        return error("Expected integer value");
///                    break;
///                case 1:
///                    if (!vargs.import(i+1, fval))
///                        return error("Expected real value");
///                    break;
///                case 2:
///                    if (!vargs.import(i+1, sval))
///                        return error("Expected string value");
///                    break;
///            }
///        }
///    }
/// @endcode
///
/// To call a function with variadic arguments is fairly straightforward
/// @code
///     function(UT_Variadic() << "int" << 3
///                            << "real << 3.421
///                            << "string" << "Hello world!");
/// @endcode
///
/// Of course, in the above example, it might be better to use @code
///	typedef UT_Pair<const char *, UT_VariadicPODType> KwPODPair;
///     typedef UT_VariadicT<VariadicPODPair> VariadicKwPODPairs;
/// @endcode
class UT_API UT_VariadicPOD : public UT_VariadicT<UT_VariadicPODType>
{
public:
    typedef UT_VariadicT<UT_VariadicPODType>	Base;

    UT_VariadicPOD()
	: Base()
    {
    }
    UT_VariadicPOD(const UT_VariadicPOD &src)
	: Base(src)
    {
    }
    ~UT_VariadicPOD()
    {
    }

    UT_VariadicPOD	&operator<<(const UT_VariadicPODType &val)
    {
	Base::operator<<(val);
	return *this;
    }

    /// @{
    /// Import the value.  This performs type safety checks.
    /// When importing real values, integer values will be implicitly cast
    /// unless the strict_value option is set.
    bool	import(int arg, int32 &value) const;
    bool	import(int arg, int64 &value) const;
    bool	import(int arg, fpreal32 &value, bool strict_value=false) const;
    bool	import(int arg, fpreal64 &value, bool strict_value=false) const;
    bool	import(int arg, const char *&value) const;
    bool	import(int arg, const int8 *&value) const;
    bool	import(int arg, const uint8 *&value) const;
    bool	import(int arg, const int16 *&value) const;
    bool	import(int arg, const uint16 *&value) const;
    bool	import(int arg, const int32 *&value) const;
    bool	import(int arg, const int64 *&value) const;
    bool	import(int arg, const fpreal16 *&value) const;
    bool	import(int arg, const fpreal32 *&value) const;
    bool	import(int arg, const fpreal64 *&value) const;
    /// @}

    /// @{
    /// Save argument list or dump to stdout
    bool	save(UT_JSONWriter &w) const;
    void	dump() const;
    /// @}
};

#endif
