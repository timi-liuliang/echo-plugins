// Copyright (C) 2005 Peder Holt
// Copyright (C) 2005 Arkadiy Vertleyb
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_TYPEOF_TEMPLATE_TEMPLATE_PARAM_HPP_INCLUDED
#define HBOOST_TYPEOF_TEMPLATE_TEMPLATE_PARAM_HPP_INCLUDED

#include <hboost/preprocessor/logical/or.hpp>
#include <hboost/preprocessor/seq/fold_left.hpp>
#include <hboost/preprocessor/seq/enum.hpp>

#define HBOOST_TYPEOF_MAKE_OBJ_template(x)   HBOOST_TYPEOF_TEMPLATE_PARAM(x)
#define HBOOST_TYPEOF_TEMPLATE(X) template(X) HBOOST_TYPEOF_EAT
#define HBOOST_TYPEOF_template(X) (template(X))

#define HBOOST_TYPEOF_TEMPLATE_PARAM(Params)\
    (TEMPLATE_PARAM)\
    (Params)

#define HBOOST_TYPEOF_TEMPLATE_PARAM_GETPARAMS(This)\
    HBOOST_TYPEOF_TOSEQ(HBOOST_PP_SEQ_ELEM(1, This))

//Encode / decode this
#define HBOOST_TYPEOF_TEMPLATE_PARAM_ENCODE(This, n)\
   typedef typename hboost::type_of::encode_template<HBOOST_PP_CAT(V, n),\
       HBOOST_PP_CAT(P, n)<HBOOST_TYPEOF_SEQ_ENUM(HBOOST_TYPEOF_MAKE_OBJS(HBOOST_TYPEOF_TEMPLATE_PARAM_GETPARAMS(This)),HBOOST_TYPEOF_PLACEHOLDER) >\
   >::type HBOOST_PP_CAT(V, HBOOST_PP_INC(n));

#define HBOOST_TYPEOF_TEMPLATE_PARAM_DECODE(This, n)\
   typedef hboost::type_of::decode_template< HBOOST_PP_CAT(iter, n) > HBOOST_PP_CAT(d, n);\
   typedef typename HBOOST_PP_CAT(d, n)::type HBOOST_PP_CAT(P, n);\
   typedef typename HBOOST_PP_CAT(d, n)::iter HBOOST_PP_CAT(iter,HBOOST_PP_INC(n));

// template<class, unsigned int, ...> class
#define HBOOST_TYPEOF_TEMPLATE_PARAM_EXPANDTYPE(This) \
    template <HBOOST_PP_SEQ_ENUM(HBOOST_TYPEOF_TEMPLATE_PARAM_GETPARAMS(This)) > class

#define HBOOST_TYPEOF_TEMPLATE_PARAM_PLACEHOLDER(Param)\
    Nested_Template_Template_Arguments_Not_Supported

//'template<class,int> class' is reduced to 'class'
#define HBOOST_TYPEOF_TEMPLATE_PARAM_DECLARATION_TYPE(Param) class

// T3<int, (unsigned int)0, ...>
#define HBOOST_TYPEOF_TEMPLATE_PARAM_PLACEHOLDER_TYPES(Param, n)\
    HBOOST_PP_CAT(T,n)<HBOOST_TYPEOF_SEQ_ENUM_1(HBOOST_TYPEOF_MAKE_OBJS(HBOOST_TYPEOF_TEMPLATE_PARAM_GETPARAMS(Param)),HBOOST_TYPEOF_PLACEHOLDER) >

#define HBOOST_TYPEOF_TEMPLATE_PARAM_ISTEMPLATE 1

////////////////////////////
// move to encode_decode?

HBOOST_TYPEOF_BEGIN_ENCODE_NS

template<class V, class Type_Not_Registered_With_Typeof_System> struct encode_template_impl;
template<class T, class Iter> struct decode_template_impl;

HBOOST_TYPEOF_END_ENCODE_NS

namespace hboost { namespace type_of {

    template<class V, class T> struct encode_template
        : HBOOST_TYPEOF_ENCODE_NS_QUALIFIER::encode_template_impl<V, T>
    {};

    template<class Iter> struct decode_template
        : HBOOST_TYPEOF_ENCODE_NS_QUALIFIER::decode_template_impl<typename Iter::type, typename Iter::next>
    {};
}}

////////////////////////////
// move to template_encoding.hpp?

//Template template registration
#define HBOOST_TYPEOF_REGISTER_TYPE_FOR_TEMPLATE_TEMPLATE(Name,Params,ID)\
    template<class V\
        HBOOST_TYPEOF_SEQ_ENUM_TRAILING(Params,HBOOST_TYPEOF_REGISTER_TEMPLATE_PARAM_PAIR)\
    >\
    struct encode_template_impl<V,Name<\
        HBOOST_PP_ENUM_PARAMS(\
        HBOOST_PP_SEQ_SIZE(Params),\
        P)> >\
        : hboost::type_of::push_back<V, hboost::mpl::size_t<ID> >\
    {\
    };\
    template<class Iter> struct decode_template_impl<hboost::mpl::size_t<ID>, Iter>\
    {\
        HBOOST_PP_REPEAT(HBOOST_PP_SEQ_SIZE(Params),HBOOST_TYPEOF_TYPEDEF_INT_PN,_)\
        typedef Name<HBOOST_TYPEOF_SEQ_ENUM(Params,HBOOST_TYPEOF_PLACEHOLDER) > type;\
        typedef Iter iter;\
    };

#define HBOOST_TYPEOF_TYPEDEF_INT_PN(z,n,Params) typedef int HBOOST_PP_CAT(P,n);

#ifdef __BORLANDC__
#define HBOOST_TYPEOF_DECODE_NESTED_TEMPLATE_HELPER_NAME HBOOST_PP_CAT(\
        HBOOST_PP_CAT(\
            HBOOST_PP_CAT(\
                decode_nested_template_helper,\
                HBOOST_TYPEOF_REGISTRATION_GROUP\
            ),0x10000\
        ),__LINE__\
    )
#define HBOOST_TYPEOF_REGISTER_DECODE_NESTED_TEMPLATE_HELPER_IMPL(Name,Params,ID)\
    struct HBOOST_TYPEOF_DECODE_NESTED_TEMPLATE_HELPER_NAME {\
        template<HBOOST_TYPEOF_SEQ_ENUM(Params,HBOOST_TYPEOF_REGISTER_DECLARE_DECODER_TYPE_PARAM_PAIR) >\
        struct decode_params;\
        template<HBOOST_TYPEOF_SEQ_ENUM(Params,HBOOST_TYPEOF_REGISTER_DECODER_TYPE_PARAM_PAIR) >\
        struct decode_params<HBOOST_TYPEOF_SEQ_ENUM(Params,HBOOST_TYPEOF_PLACEHOLDER_TYPES) >\
        {\
            typedef Name<HBOOST_PP_ENUM_PARAMS(HBOOST_PP_SEQ_SIZE(Params),T)> type;\
        };\
    };
//Template template param decoding
#define HBOOST_TYPEOF_TYPEDEF_DECODED_TEMPLATE_TEMPLATE_TYPE(Name,Params)\
    typedef typename HBOOST_TYPEOF_DECODE_NESTED_TEMPLATE_HELPER_NAME::decode_params<HBOOST_PP_ENUM_PARAMS(HBOOST_PP_SEQ_SIZE(Params),P)>::type type;

#else
#define HBOOST_TYPEOF_REGISTER_DECODE_NESTED_TEMPLATE_HELPER_IMPL(Name,Params,ID)

//Template template param decoding
#define HBOOST_TYPEOF_TYPEDEF_DECODED_TEMPLATE_TEMPLATE_TYPE(Name,Params)\
    template<HBOOST_TYPEOF_SEQ_ENUM(Params,HBOOST_TYPEOF_REGISTER_DECLARE_DECODER_TYPE_PARAM_PAIR) >\
    struct decode_params;\
    template<HBOOST_TYPEOF_SEQ_ENUM(Params,HBOOST_TYPEOF_REGISTER_DECODER_TYPE_PARAM_PAIR) >\
    struct decode_params<HBOOST_TYPEOF_SEQ_ENUM(Params,HBOOST_TYPEOF_PLACEHOLDER_TYPES) >\
    {\
        typedef Name<HBOOST_PP_ENUM_PARAMS(HBOOST_PP_SEQ_SIZE(Params),T)> type;\
    };\
    typedef typename decode_params<HBOOST_PP_ENUM_PARAMS(HBOOST_PP_SEQ_SIZE(Params),P)>::type type;
#endif
#define HBOOST_TYPEOF_REGISTER_DECLARE_DECODER_TYPE_PARAM_PAIR(z,n,elem) \
    HBOOST_TYPEOF_VIRTUAL(DECLARATION_TYPE, elem)(elem) HBOOST_PP_CAT(T, n)

// HBOOST_TYPEOF_HAS_TEMPLATES
#define HBOOST_TYPEOF_HAS_TEMPLATES(Params)\
    HBOOST_PP_SEQ_FOLD_LEFT(HBOOST_TYPEOF_HAS_TEMPLATES_OP, 0, Params)

#define HBOOST_TYPEOF_HAS_TEMPLATES_OP(s, state, elem)\
    HBOOST_PP_OR(state, HBOOST_TYPEOF_VIRTUAL(ISTEMPLATE, elem))

//Define template template arguments
#define HBOOST_TYPEOF_REGISTER_TEMPLATE_TEMPLATE_IMPL(Name,Params,ID)\
    HBOOST_PP_IF(HBOOST_TYPEOF_HAS_TEMPLATES(Params),\
        HBOOST_TYPEOF_REGISTER_DECODE_NESTED_TEMPLATE_HELPER_IMPL,\
        HBOOST_TYPEOF_REGISTER_TYPE_FOR_TEMPLATE_TEMPLATE)(Name,Params,ID)

#endif //HBOOST_TYPEOF_TEMPLATE_TEMPLATE_PARAM_HPP_INCLUDED
