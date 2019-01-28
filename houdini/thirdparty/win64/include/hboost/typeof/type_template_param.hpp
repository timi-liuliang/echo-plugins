// Copyright (C) 2005 Arkadiy Vertleyb
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_TYPEOF_TYPE_TEMPLATE_PARAM_HPP_INCLUDED
#define HBOOST_TYPEOF_TYPE_TEMPLATE_PARAM_HPP_INCLUDED

#define HBOOST_TYPEOF_class_HBOOST_TYPEOF (class)
#define HBOOST_TYPEOF_typename_HBOOST_TYPEOF (typename)

#define HBOOST_TYPEOF_MAKE_OBJ_class HBOOST_TYPEOF_TYPE_PARAM
#define HBOOST_TYPEOF_MAKE_OBJ_typename HBOOST_TYPEOF_TYPE_PARAM

#define HBOOST_TYPEOF_TYPE_PARAM\
    (TYPE_PARAM)

#define HBOOST_TYPEOF_TYPE_PARAM_EXPANDTYPE(Param) class

// TYPE_PARAM "virtual functions" implementation

#define HBOOST_TYPEOF_TYPE_PARAM_ENCODE(This, n)\
    typedef typename hboost::type_of::encode_type<\
        HBOOST_PP_CAT(V, n),\
        HBOOST_PP_CAT(P, n)\
    >::type HBOOST_PP_CAT(V, HBOOST_PP_INC(n)); 

#define HBOOST_TYPEOF_TYPE_PARAM_DECODE(This, n)\
    typedef hboost::type_of::decode_type< HBOOST_PP_CAT(iter, n) > HBOOST_PP_CAT(d, n);\
    typedef typename HBOOST_PP_CAT(d, n)::type HBOOST_PP_CAT(P, n);\
    typedef typename HBOOST_PP_CAT(d, n)::iter HBOOST_PP_CAT(iter, HBOOST_PP_INC(n));

#define HBOOST_TYPEOF_TYPE_PARAM_PLACEHOLDER(Param) int
#define HBOOST_TYPEOF_TYPE_PARAM_DECLARATION_TYPE(Param) class
#define HBOOST_TYPEOF_TYPE_PARAM_PLACEHOLDER_TYPES(Param, n) HBOOST_PP_CAT(T,n)
#define HBOOST_TYPEOF_TYPE_PARAM_ISTEMPLATE 0

#endif//HBOOST_TYPEOF_TYPE_TEMPLATE_PARAM_HPP_INCLUDED
