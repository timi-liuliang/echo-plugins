// Copyright (C) 2005 Arkadiy Vertleyb
// Copyright (C) 2005 Peder Holt
//
// Copyright (C) 2006 Tobias Schwinger
//
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_TYPEOF_VECTOR_HPP_INCLUDED

#include <hboost/mpl/int.hpp>
#include <hboost/preprocessor/iteration/self.hpp>

#ifndef HBOOST_TYPEOF_LIMIT_SIZE
#   define HBOOST_TYPEOF_LIMIT_SIZE 50
#endif

//
// To recreate the preprocessed versions of this file preprocess and run
//
//   $(HBOOST_ROOT)/libs/typeof/tools/preprocess.pl
//

#if defined(HBOOST_TYPEOF_PP_INCLUDE_EXTERNAL)

#   undef HBOOST_TYPEOF_PP_INCLUDE_EXTERNAL

#elif !defined(HBOOST_TYPEOF_PREPROCESSING_MODE) && !HBOOST_PP_IS_SELFISH

#   define HBOOST_PP_INDIRECT_SELF <hboost/typeof/vector.hpp>
#   if   HBOOST_TYPEOF_LIMIT_SIZE < 50
#     include HBOOST_PP_INCLUDE_SELF()
#   elif HBOOST_TYPEOF_LIMIT_SIZE < 100
#     include <hboost/typeof/vector50.hpp>
#     define  HBOOST_TYPEOF_PP_START_SIZE 51
#     include HBOOST_PP_INCLUDE_SELF()
#   elif HBOOST_TYPEOF_LIMIT_SIZE < 150
#     include <hboost/typeof/vector100.hpp>
#     define  HBOOST_TYPEOF_PP_START_SIZE 101
#     include HBOOST_PP_INCLUDE_SELF()
#   elif HBOOST_TYPEOF_LIMIT_SIZE < 200
#     include <hboost/typeof/vector150.hpp>
#     define  HBOOST_TYPEOF_PP_START_SIZE 151
#     include HBOOST_PP_INCLUDE_SELF()
#   elif HBOOST_TYPEOF_LIMIT_SIZE <= 250
#     include <hboost/typeof/vector200.hpp>
#     define  HBOOST_TYPEOF_PP_START_SIZE 201
#     include HBOOST_PP_INCLUDE_SELF()
#   else
#     error "HBOOST_TYPEOF_LIMIT_SIZE too high"
#   endif

#else// defined(HBOOST_TYPEOF_PREPROCESSING_MODE) || HBOOST_PP_IS_SELFISH

#   ifndef HBOOST_TYPEOF_PP_NEXT_SIZE
#     define HBOOST_TYPEOF_PP_NEXT_SIZE HBOOST_TYPEOF_LIMIT_SIZE
#   endif
#   ifndef HBOOST_TYPEOF_PP_START_SIZE
#     define HBOOST_TYPEOF_PP_START_SIZE 0
#   endif

#   if HBOOST_TYPEOF_PP_START_SIZE <= HBOOST_TYPEOF_LIMIT_SIZE

#     include <hboost/preprocessor/enum_params.hpp>
#     include <hboost/preprocessor/repeat.hpp>
#     include <hboost/preprocessor/repeat_from_to.hpp>
#     include <hboost/preprocessor/cat.hpp>
#     include <hboost/preprocessor/inc.hpp>
#     include <hboost/preprocessor/dec.hpp>
#     include <hboost/preprocessor/comma_if.hpp>
#     include <hboost/preprocessor/iteration/local.hpp>
#     include <hboost/preprocessor/control/expr_iif.hpp>
#     include <hboost/preprocessor/logical/not.hpp>

// iterator

#     define HBOOST_TYPEOF_spec_iter(n)\
        template<class V>\
        struct v_iter<V, mpl::int_<n> >\
        {\
            typedef typename V::item ## n type;\
            typedef v_iter<V, mpl::int_<n + 1> > next;\
        };

namespace hboost { namespace type_of {

    template<class V, class Increase_HBOOST_TYPEOF_LIMIT_SIZE> struct v_iter; // not defined
#     define  HBOOST_PP_LOCAL_MACRO  HBOOST_TYPEOF_spec_iter
#     define  HBOOST_PP_LOCAL_LIMITS \
        (HBOOST_PP_DEC(HBOOST_TYPEOF_PP_START_SIZE), \
         HBOOST_PP_DEC(HBOOST_TYPEOF_LIMIT_SIZE))
#     include HBOOST_PP_LOCAL_ITERATE()

}}

#     undef HBOOST_TYPEOF_spec_iter

// vector

#     define HBOOST_TYPEOF_typedef_item(z, n, _)\
        typedef P ## n item ## n;

#     define HBOOST_TYPEOF_typedef_fake_item(z, n, _)\
        typedef mpl::int_<1> item ## n;

#     define HBOOST_TYPEOF_define_vector(n)\
        template<HBOOST_PP_ENUM_PARAMS(n, class P) HBOOST_PP_EXPR_IIF(HBOOST_PP_NOT(n), class T = void)>\
        struct vector ## n\
        {\
            typedef v_iter<vector ## n<HBOOST_PP_ENUM_PARAMS(n,P)>, hboost::mpl::int_<0> > begin;\
            HBOOST_PP_REPEAT(n, HBOOST_TYPEOF_typedef_item, ~)\
            HBOOST_PP_REPEAT_FROM_TO(n, HBOOST_TYPEOF_PP_NEXT_SIZE, HBOOST_TYPEOF_typedef_fake_item, ~)\
        };

namespace hboost { namespace type_of {

#     define  HBOOST_PP_LOCAL_MACRO  HBOOST_TYPEOF_define_vector
#     define  HBOOST_PP_LOCAL_LIMITS \
        (HBOOST_TYPEOF_PP_START_SIZE,HBOOST_TYPEOF_LIMIT_SIZE)
#     include HBOOST_PP_LOCAL_ITERATE()

}}

#     undef HBOOST_TYPEOF_typedef_item
#     undef HBOOST_TYPEOF_typedef_fake_item
#     undef HBOOST_TYPEOF_define_vector

// push_back

#     define HBOOST_TYPEOF_spec_push_back(n)\
        template<HBOOST_PP_ENUM_PARAMS(n, class P) HBOOST_PP_COMMA_IF(n) class T>\
        struct push_back<HBOOST_PP_CAT(hboost::type_of::vector, n)<HBOOST_PP_ENUM_PARAMS(n, P)>, T>\
        {\
            typedef HBOOST_PP_CAT(hboost::type_of::vector, HBOOST_PP_INC(n))<\
                HBOOST_PP_ENUM_PARAMS(n, P) HBOOST_PP_COMMA_IF(n) T\
            > type;\
        };

namespace hboost { namespace type_of {

#   if   HBOOST_TYPEOF_LIMIT_SIZE < 50
    template<class V, class T> struct push_back {
        typedef V type;
    };
#   endif
    //default behaviour is to let push_back ignore T, and return the input vector.
    //This is to let HBOOST_TYPEOF_NESTED_TYPEDEF work properly with the default vector.
#     define  HBOOST_PP_LOCAL_MACRO  HBOOST_TYPEOF_spec_push_back
#     define  HBOOST_PP_LOCAL_LIMITS \
        (HBOOST_PP_DEC(HBOOST_TYPEOF_PP_START_SIZE), \
         HBOOST_PP_DEC(HBOOST_TYPEOF_LIMIT_SIZE))
#     include HBOOST_PP_LOCAL_ITERATE()

}}

#     undef HBOOST_TYPEOF_spec_push_back

#   endif//HBOOST_TYPEOF_PP_START_SIZE<=HBOOST_TYPEOF_LIMIT_SIZE
#   undef  HBOOST_TYPEOF_PP_START_SIZE
#   undef  HBOOST_TYPEOF_PP_NEXT_SIZE

#endif//HBOOST_TYPEOF_PREPROCESSING_MODE || HBOOST_PP_IS_SELFISH

#define HBOOST_TYPEOF_VECTOR_HPP_INCLUDED
#endif//HBOOST_TYPEOF_VECTOR_HPP_INCLUDED

