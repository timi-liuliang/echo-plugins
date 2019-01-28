// Copyright (C) 2005 Arkadiy Vertleyb
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_TYPEOF_INTEGRAL_TEMPLATE_PARAM_HPP_INCLUDED
#define HBOOST_TYPEOF_INTEGRAL_TEMPLATE_PARAM_HPP_INCLUDED

#define HBOOST_TYPEOF_unsigned (unsigned)
#define HBOOST_TYPEOF_signed (signed)

#define char_HBOOST_TYPEOF (char)
#define short_HBOOST_TYPEOF (short)
#define int_HBOOST_TYPEOF (int)
#define long_HBOOST_TYPEOF (long)

#define HBOOST_TYPEOF_char_HBOOST_TYPEOF (char)
#define HBOOST_TYPEOF_short_HBOOST_TYPEOF (short)
#define HBOOST_TYPEOF_int_HBOOST_TYPEOF (int)
#define HBOOST_TYPEOF_long_HBOOST_TYPEOF (long)
#define HBOOST_TYPEOF_bool_HBOOST_TYPEOF (bool)
#define HBOOST_TYPEOF_unsigned_HBOOST_TYPEOF (unsigned)
#define HBOOST_TYPEOF_size_t_HBOOST_TYPEOF (size_t)

#define HBOOST_TYPEOF_MAKE_OBJ_char          HBOOST_TYPEOF_INTEGRAL_PARAM(char)
#define HBOOST_TYPEOF_MAKE_OBJ_short         HBOOST_TYPEOF_INTEGRAL_PARAM(short)
#define HBOOST_TYPEOF_MAKE_OBJ_int           HBOOST_TYPEOF_INTEGRAL_PARAM(int)
#define HBOOST_TYPEOF_MAKE_OBJ_long          HBOOST_TYPEOF_INTEGRAL_PARAM(long)
#define HBOOST_TYPEOF_MAKE_OBJ_bool          HBOOST_TYPEOF_INTEGRAL_PARAM(bool)
#define HBOOST_TYPEOF_MAKE_OBJ_unsigned      HBOOST_TYPEOF_INTEGRAL_PARAM(unsigned)
#define HBOOST_TYPEOF_MAKE_OBJ_size_t        HBOOST_TYPEOF_INTEGRAL_PARAM(size_t)
#define HBOOST_TYPEOF_MAKE_OBJ_unsignedchar  HBOOST_TYPEOF_INTEGRAL_PARAM(unsigned char)
#define HBOOST_TYPEOF_MAKE_OBJ_unsignedshort HBOOST_TYPEOF_INTEGRAL_PARAM(unsigned short)
#define HBOOST_TYPEOF_MAKE_OBJ_unsignedint   HBOOST_TYPEOF_INTEGRAL_PARAM(unsigned int)
#define HBOOST_TYPEOF_MAKE_OBJ_unsignedlong  HBOOST_TYPEOF_INTEGRAL_PARAM(unsigned long)
#define HBOOST_TYPEOF_MAKE_OBJ_signedchar    HBOOST_TYPEOF_INTEGRAL_PARAM(signed char)
#define HBOOST_TYPEOF_MAKE_OBJ_signedshort   HBOOST_TYPEOF_INTEGRAL_PARAM(signed short)
#define HBOOST_TYPEOF_MAKE_OBJ_signedint     HBOOST_TYPEOF_INTEGRAL_PARAM(signed int)
#define HBOOST_TYPEOF_MAKE_OBJ_signedlong    HBOOST_TYPEOF_INTEGRAL_PARAM(signed long)
#define HBOOST_TYPEOF_MAKE_OBJ_integral(x)   HBOOST_TYPEOF_INTEGRAL_PARAM(x)

#define HBOOST_TYPEOF_INTEGRAL(X) integral(X) HBOOST_TYPEOF_EAT
#define HBOOST_TYPEOF_EAT_HBOOST_TYPEOF
#define HBOOST_TYPEOF_integral(X) (integral(X))

#define HBOOST_TYPEOF_INTEGRAL_PARAM(Type)\
    (INTEGRAL_PARAM)\
    (Type)

#define HBOOST_TYPEOF_INTEGRAL_PARAM_GETTYPE(Param)\
    HBOOST_PP_SEQ_ELEM(1, Param)

#define HBOOST_TYPEOF_INTEGRAL_PARAM_EXPANDTYPE(Param)\
    HBOOST_TYPEOF_INTEGRAL_PARAM_GETTYPE(Param)

// INTEGRAL_PARAM "virtual functions" implementation

#define HBOOST_TYPEOF_INTEGRAL_PARAM_ENCODE(This, n)\
    typedef typename hboost::type_of::encode_integral<\
        HBOOST_PP_CAT(V, n),\
        HBOOST_TYPEOF_INTEGRAL_PARAM_GETTYPE(This),\
        HBOOST_PP_CAT(P, n)\
    >::type HBOOST_PP_CAT(V, HBOOST_PP_INC(n)); 

#define HBOOST_TYPEOF_INTEGRAL_PARAM_DECODE(This, n)\
    typedef hboost::type_of::decode_integral<HBOOST_TYPEOF_INTEGRAL_PARAM_GETTYPE(This), HBOOST_PP_CAT(iter, n)> HBOOST_PP_CAT(d, n);\
    static const HBOOST_TYPEOF_INTEGRAL_PARAM_GETTYPE(This) HBOOST_PP_CAT(P, n) = HBOOST_PP_CAT(d, n)::value;\
    typedef typename HBOOST_PP_CAT(d, n)::iter HBOOST_PP_CAT(iter, HBOOST_PP_INC(n));

#define HBOOST_TYPEOF_INTEGRAL_PARAM_PLACEHOLDER(Param)\
    (HBOOST_TYPEOF_INTEGRAL_PARAM_GETTYPE(Param))0

#define HBOOST_TYPEOF_INTEGRAL_PARAM_DECLARATION_TYPE(Param)\
    HBOOST_TYPEOF_INTEGRAL_PARAM_GETTYPE(Param)

#define HBOOST_TYPEOF_INTEGRAL_PARAM_PLACEHOLDER_TYPES(Param, n)\
    HBOOST_PP_CAT(T,n)

#define HBOOST_TYPEOF_INTEGRAL_PARAM_ISTEMPLATE 0

#endif//HBOOST_TYPEOF_INTEGRAL_TEMPLATE_PARAM_HPP_INCLUDED
