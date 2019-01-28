// Copyright (C) 2004 Arkadiy Vertleyb
// Copyright (C) 2005 Peder Holt
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_TYPEOF_TEMPLATE_ENCODING_HPP_INCLUDED
#define HBOOST_TYPEOF_TEMPLATE_ENCODING_HPP_INCLUDED

#include <hboost/preprocessor/cat.hpp>
#include <hboost/preprocessor/repetition/enum_trailing.hpp>
#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/detail/is_unary.hpp>
#include <hboost/preprocessor/repetition/repeat.hpp>
#include <hboost/preprocessor/tuple/eat.hpp>
#include <hboost/preprocessor/seq/transform.hpp>
#include <hboost/preprocessor/seq/for_each_i.hpp>
#include <hboost/preprocessor/seq/cat.hpp>

#include <hboost/typeof/encode_decode.hpp>
#include <hboost/typeof/int_encoding.hpp>

#include <hboost/typeof/type_template_param.hpp>
#include <hboost/typeof/integral_template_param.hpp>
#include <hboost/typeof/template_template_param.hpp>

#ifdef __BORLANDC__
#define HBOOST_TYPEOF_QUALIFY(P) self_t::P
#else
#define HBOOST_TYPEOF_QUALIFY(P) P
#endif
// The template parameter description, entered by the user,
// is converted into a polymorphic "object"
// that is used to generate the code responsible for
// encoding/decoding the parameter, etc.

// make sure to cat the sequence first, and only then add the prefix.
#define HBOOST_TYPEOF_MAKE_OBJ(elem) HBOOST_PP_CAT(\
    HBOOST_TYPEOF_MAKE_OBJ,\
    HBOOST_PP_SEQ_CAT((_) HBOOST_TYPEOF_TO_SEQ(elem))\
    )

#define HBOOST_TYPEOF_TO_SEQ(tokens) HBOOST_TYPEOF_ ## tokens ## _HBOOST_TYPEOF

// HBOOST_TYPEOF_REGISTER_TEMPLATE

#define HBOOST_TYPEOF_REGISTER_TEMPLATE_EXPLICIT_ID(Name, Params, Id)\
    HBOOST_TYPEOF_REGISTER_TEMPLATE_IMPL(\
        Name,\
        HBOOST_TYPEOF_MAKE_OBJS(HBOOST_TYPEOF_TOSEQ(Params)),\
        HBOOST_PP_SEQ_SIZE(HBOOST_TYPEOF_TOSEQ(Params)),\
        Id)

#define HBOOST_TYPEOF_REGISTER_TEMPLATE(Name, Params)\
    HBOOST_TYPEOF_REGISTER_TEMPLATE_EXPLICIT_ID(Name, Params, HBOOST_TYPEOF_UNIQUE_ID())

#define HBOOST_TYPEOF_OBJECT_MAKER(s, data, elem)\
    HBOOST_TYPEOF_MAKE_OBJ(elem)

#define HBOOST_TYPEOF_MAKE_OBJS(Params)\
    HBOOST_PP_SEQ_TRANSFORM(HBOOST_TYPEOF_OBJECT_MAKER, ~, Params)

// As suggested by Paul Mensonides:

#define HBOOST_TYPEOF_TOSEQ(x)\
    HBOOST_PP_IIF(\
        HBOOST_PP_IS_UNARY(x),\
        x HBOOST_PP_TUPLE_EAT(3), HBOOST_PP_REPEAT\
    )(x, HBOOST_TYPEOF_TOSEQ_2, ~)

#define HBOOST_TYPEOF_TOSEQ_2(z, n, _) (class)

// HBOOST_TYPEOF_VIRTUAL

#define HBOOST_TYPEOF_CAT_4(a, b, c, d) HBOOST_TYPEOF_CAT_4_I(a, b, c, d)
#define HBOOST_TYPEOF_CAT_4_I(a, b, c, d) a ## b ## c ## d

#define HBOOST_TYPEOF_VIRTUAL(Fun, Obj)\
    HBOOST_TYPEOF_CAT_4(HBOOST_TYPEOF_, HBOOST_PP_SEQ_HEAD(Obj), _, Fun)

// HBOOST_TYPEOF_SEQ_ENUM[_TRAILING][_1]
// Two versions provided due to reentrancy issue

#define HBOOST_TYPEOF_SEQ_EXPAND_ELEMENT(z,n,seq)\
   HBOOST_PP_SEQ_ELEM(0,seq) (z,n,HBOOST_PP_SEQ_ELEM(n,HBOOST_PP_SEQ_ELEM(1,seq)))

#define HBOOST_TYPEOF_SEQ_ENUM(seq,macro)\
    HBOOST_PP_ENUM(HBOOST_PP_SEQ_SIZE(seq),HBOOST_TYPEOF_SEQ_EXPAND_ELEMENT,(macro)(seq))

#define HBOOST_TYPEOF_SEQ_ENUM_TRAILING(seq,macro)\
    HBOOST_PP_ENUM_TRAILING(HBOOST_PP_SEQ_SIZE(seq),HBOOST_TYPEOF_SEQ_EXPAND_ELEMENT,(macro)(seq))

#define HBOOST_TYPEOF_SEQ_EXPAND_ELEMENT_1(z,n,seq)\
    HBOOST_PP_SEQ_ELEM(0,seq) (z,n,HBOOST_PP_SEQ_ELEM(n,HBOOST_PP_SEQ_ELEM(1,seq)))

#define HBOOST_TYPEOF_SEQ_ENUM_1(seq,macro)\
    HBOOST_PP_ENUM(HBOOST_PP_SEQ_SIZE(seq),HBOOST_TYPEOF_SEQ_EXPAND_ELEMENT_1,(macro)(seq))

#define HBOOST_TYPEOF_SEQ_ENUM_TRAILING_1(seq,macro)\
    HBOOST_PP_ENUM_TRAILING(HBOOST_PP_SEQ_SIZE(seq),HBOOST_TYPEOF_SEQ_EXPAND_ELEMENT_1,(macro)(seq))

//

#define HBOOST_TYPEOF_PLACEHOLDER(z, n, elem)\
    HBOOST_TYPEOF_VIRTUAL(PLACEHOLDER, elem)(elem)

#define HBOOST_TYPEOF_PLACEHOLDER_TYPES(z, n, elem)\
    HBOOST_TYPEOF_VIRTUAL(PLACEHOLDER_TYPES, elem)(elem, n)

#define HBOOST_TYPEOF_REGISTER_TEMPLATE_ENCODE_PARAM(r, data, n, elem)\
    HBOOST_TYPEOF_VIRTUAL(ENCODE, elem)(elem, n)

#define HBOOST_TYPEOF_REGISTER_TEMPLATE_DECODE_PARAM(r, data, n, elem)\
    HBOOST_TYPEOF_VIRTUAL(DECODE, elem)(elem, n)

#define HBOOST_TYPEOF_REGISTER_TEMPLATE_PARAM_PAIR(z, n, elem) \
    HBOOST_TYPEOF_VIRTUAL(EXPANDTYPE, elem)(elem) HBOOST_PP_CAT(P, n)

#define HBOOST_TYPEOF_REGISTER_DEFAULT_TEMPLATE_TYPE(Name,Params,ID)\
    Name< HBOOST_PP_ENUM_PARAMS(HBOOST_PP_SEQ_SIZE(Params), P) >

//Since we are creating an internal decode struct, we need to use different template names, T instead of P.
#define HBOOST_TYPEOF_REGISTER_DECODER_TYPE_PARAM_PAIR(z,n,elem) \
    HBOOST_TYPEOF_VIRTUAL(EXPANDTYPE, elem)(elem) HBOOST_PP_CAT(T, n)

//Default template param decoding

#define HBOOST_TYPEOF_TYPEDEF_DECODED_TEMPLATE_TYPE(Name,Params)\
    typedef Name<HBOOST_PP_ENUM_PARAMS(HBOOST_PP_SEQ_SIZE(Params),HBOOST_TYPEOF_QUALIFY(P))> type;

//Branch the decoding
#define HBOOST_TYPEOF_TYPEDEF_DECODED_TYPE(Name,Params)\
    HBOOST_PP_IF(HBOOST_TYPEOF_HAS_TEMPLATES(Params),\
        HBOOST_TYPEOF_TYPEDEF_DECODED_TEMPLATE_TEMPLATE_TYPE,\
        HBOOST_TYPEOF_TYPEDEF_DECODED_TEMPLATE_TYPE)(Name,Params)

#define HBOOST_TYPEOF_REGISTER_TEMPLATE_IMPL(Name, Params, Size, ID)\
    HBOOST_TYPEOF_BEGIN_ENCODE_NS\
    HBOOST_TYPEOF_REGISTER_TEMPLATE_TEMPLATE_IMPL(Name, Params, ID)\
    template<class V\
        HBOOST_TYPEOF_SEQ_ENUM_TRAILING(Params, HBOOST_TYPEOF_REGISTER_TEMPLATE_PARAM_PAIR)\
    >\
    struct encode_type_impl<V, Name<HBOOST_PP_ENUM_PARAMS(Size, P)> >\
    {\
        typedef typename hboost::type_of::push_back<V, hboost::mpl::size_t<ID> >::type V0;\
        HBOOST_PP_SEQ_FOR_EACH_I(HBOOST_TYPEOF_REGISTER_TEMPLATE_ENCODE_PARAM, ~, Params)\
        typedef HBOOST_PP_CAT(V, Size) type;\
    };\
    template<class Iter>\
    struct decode_type_impl<hboost::mpl::size_t<ID>, Iter>\
    {\
        typedef decode_type_impl<hboost::mpl::size_t<ID>, Iter> self_t;\
        typedef hboost::mpl::size_t<ID> self_id;\
        typedef Iter iter0;\
        HBOOST_PP_SEQ_FOR_EACH_I(HBOOST_TYPEOF_REGISTER_TEMPLATE_DECODE_PARAM, ~, Params)\
        HBOOST_TYPEOF_TYPEDEF_DECODED_TYPE(Name, Params)\
        typedef HBOOST_PP_CAT(iter, Size) iter;\
    };\
    HBOOST_TYPEOF_END_ENCODE_NS

#endif//HBOOST_TYPEOF_TEMPLATE_ENCODING_HPP_INCLUDED
