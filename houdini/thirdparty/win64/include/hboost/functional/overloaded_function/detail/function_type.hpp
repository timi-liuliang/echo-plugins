
// Copyright (C) 2009-2012 Lorenzo Caminiti
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt)
// Home at http://www.boost.org/libs/functional/overloaded_function

#ifndef HBOOST_FUNCTIONAL_OVERLOADED_FUNCTION_DETAIL_FUNCTION_TYPE_HPP_
#define HBOOST_FUNCTIONAL_OVERLOADED_FUNCTION_DETAIL_FUNCTION_TYPE_HPP_

#include <hboost/function_types/is_function.hpp>
#include <hboost/function_types/is_function_pointer.hpp>
#include <hboost/function_types/is_function_reference.hpp>
#include <hboost/function_types/function_type.hpp>
#include <hboost/function_types/parameter_types.hpp>
#include <hboost/function_types/result_type.hpp>
#include <hboost/type_traits/remove_pointer.hpp>
#include <hboost/type_traits/remove_reference.hpp>
#include <hboost/function.hpp>
#include <hboost/mpl/if.hpp>
#include <hboost/mpl/identity.hpp>
#include <hboost/mpl/pop_front.hpp>
#include <hboost/mpl/push_front.hpp>
#include <hboost/typeof/typeof.hpp>

// Do not use namespace ::detail because overloaded_function is already a class.
namespace hboost { namespace overloaded_function_detail {

// Requires: F is a monomorphic functor (i.e., has non-template `operator()`).
// Returns: F's function type `result_type (arg1_type, arg2_type, ...)`.
// It does not assume F typedef result_type, arg1_type, ... but needs typeof.
template<typename F>
class functor_type {
    // NOTE: clang does not accept extra parenthesis `&(...)`.
    typedef HBOOST_TYPEOF_TPL(&F::operator()) call_ptr;
public:
    typedef
        typename hboost::function_types::function_type<
            typename hboost::mpl::push_front<
                  typename hboost::mpl::pop_front< // Remove functor type (1st).
                    typename hboost::function_types::parameter_types<
                            call_ptr>::type
                  >::type
                , typename hboost::function_types::result_type<call_ptr>::type
            >::type
        >::type
    type;
};

// NOTE: When using hboost::function in Boost.Typeof emulation mode, the user
// has to register hboost::functionN instead of hboost::function in oder to
// do TYPEOF(F::operator()). That is confusing, so hboost::function is handled
// separately so it does not require any Boost.Typeof registration at all.
template<typename F>
struct functor_type< hboost::function<F> > {
    typedef F type;
};

// Requires: F is a function type, pointer, reference, or monomorphic functor.
// Returns: F's function type `result_type (arg1_type, arg2_type, ...)`.
template<typename F>
struct function_type {
    typedef
        typename hboost::mpl::if_<hboost::function_types::is_function<F>,
            hboost::mpl::identity<F>
        ,
            typename hboost::mpl::if_<hboost::function_types::
                    is_function_pointer<F>,
                hboost::remove_pointer<F>
            ,
                typename hboost::mpl::if_<hboost::function_types::
                        is_function_reference<F>,
                    hboost::remove_reference<F>
                , // Else, requires that F is a functor.
                    functor_type<F>
                >::type
            >::type
        >::type
    ::type type;
};

} } // namespace

#endif // #include guard

