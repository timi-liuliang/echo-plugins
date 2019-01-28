// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_PREPROCESSOR_DWA200247_HPP
# define HBOOST_PREPROCESSOR_DWA200247_HPP

# include <hboost/preprocessor/cat.hpp>
# include <hboost/preprocessor/comma_if.hpp>
# include <hboost/preprocessor/repeat.hpp>
# include <hboost/preprocessor/tuple/elem.hpp>

// stuff that should be in the preprocessor library

# define HBOOST_PYTHON_APPLY(x) HBOOST_PP_CAT(HBOOST_PYTHON_APPLY_, x)

# define HBOOST_PYTHON_APPLY_HBOOST_PYTHON_ITEM(v) v
# define HBOOST_PYTHON_APPLY_HBOOST_PYTHON_NIL

// cv-qualifiers

# if !defined(__MWERKS__) || __MWERKS__ > 0x2407
#  define HBOOST_PYTHON_CV_COUNT 4
# else
#  define HBOOST_PYTHON_CV_COUNT 1
# endif

# ifndef HBOOST_PYTHON_MAX_ARITY
#  define HBOOST_PYTHON_MAX_ARITY 15
# endif

# ifndef HBOOST_PYTHON_MAX_BASES
#  define HBOOST_PYTHON_MAX_BASES 10
# endif 

# define HBOOST_PYTHON_CV_QUALIFIER(i)                          \
    HBOOST_PYTHON_APPLY(                                        \
        HBOOST_PP_TUPLE_ELEM(4, i, HBOOST_PYTHON_CV_QUALIFIER_I) \
    )

# define HBOOST_PYTHON_CV_QUALIFIER_I      \
    (                                     \
        HBOOST_PYTHON_NIL,                 \
        HBOOST_PYTHON_ITEM(const),         \
        HBOOST_PYTHON_ITEM(volatile),      \
        HBOOST_PYTHON_ITEM(const volatile) \
    )

// enumerators
# define HBOOST_PYTHON_UNARY_ENUM(c, text) HBOOST_PP_REPEAT(c, HBOOST_PYTHON_UNARY_ENUM_I, text)
# define HBOOST_PYTHON_UNARY_ENUM_I(z, n, text) HBOOST_PP_COMMA_IF(n) text ## n

# define HBOOST_PYTHON_BINARY_ENUM(c, a, b) HBOOST_PP_REPEAT(c, HBOOST_PYTHON_BINARY_ENUM_I, (a, b))
# define HBOOST_PYTHON_BINARY_ENUM_I(z, n, _) HBOOST_PP_COMMA_IF(n) HBOOST_PP_CAT(HBOOST_PP_TUPLE_ELEM(2, 0, _), n) HBOOST_PP_CAT(HBOOST_PP_TUPLE_ELEM(2, 1, _), n)

# define HBOOST_PYTHON_ENUM_WITH_DEFAULT(c, text, def) HBOOST_PP_REPEAT(c, HBOOST_PYTHON_ENUM_WITH_DEFAULT_I, (text, def))
# define HBOOST_PYTHON_ENUM_WITH_DEFAULT_I(z, n, _) HBOOST_PP_COMMA_IF(n) HBOOST_PP_CAT(HBOOST_PP_TUPLE_ELEM(2, 0, _), n) = HBOOST_PP_TUPLE_ELEM(2, 1, _)

// fixed text (no commas)
# define HBOOST_PYTHON_FIXED(z, n, text) text

// flags
# define HBOOST_PYTHON_FUNCTION_POINTER 0x0001
# define HBOOST_PYTHON_POINTER_TO_MEMBER 0x0002

#endif // HBOOST_PREPROCESSOR_DWA200247_HPP
