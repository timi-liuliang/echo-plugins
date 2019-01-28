// Copyright Daniel Wallin, David Abrahams 2005. Use, modification and
// distribution is subject to the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_UNWRAP_CV_REFERENCE_050328_HPP
#define HBOOST_UNWRAP_CV_REFERENCE_050328_HPP

#include <hboost/parameter/aux_/yesno.hpp>
#include <hboost/mpl/bool.hpp>
#include <hboost/mpl/identity.hpp>
#include <hboost/mpl/eval_if.hpp>

namespace hboost { template<class T> class reference_wrapper; }

namespace hboost { namespace parameter { namespace aux {

//
// reference_wrapper support -- because of the forwarding problem,
// when passing arguments positionally by non-const reference, we
// ask users of named parameter interfaces to use ref(x) to wrap
// them.
//

// is_cv_reference_wrapper returns mpl::true_ if T is of type
// reference_wrapper<U> cv
template <class U>
yes_tag is_cv_reference_wrapper_check(reference_wrapper<U> const volatile*);
no_tag is_cv_reference_wrapper_check(...);

template <class T>
struct is_cv_reference_wrapper
{
    HBOOST_STATIC_CONSTANT(
        bool, value = (
            sizeof(is_cv_reference_wrapper_check((T*)0)) == sizeof(yes_tag)
        )
    );

    typedef mpl::bool_<
#if HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x564))
        is_cv_reference_wrapper::
#endif 
    value> type;
};

// Needed for unwrap_cv_reference below. T might be const, so
// eval_if might fail because of deriving from T const on EDG.
template <class T>
struct get_type
{
    typedef typename T::type type;
};

#if HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x564))
template <class T, class is_reference_wrapper = typename is_cv_reference_wrapper<T>::type>
struct unwrap_cv_reference
{
    typedef T type;
};

template <class T>
struct unwrap_cv_reference<T const, mpl::false_>
{
    typedef T const type;
};

template <class T>
struct unwrap_cv_reference<T, mpl::true_>
  : T
{};

#else 
// Produces the unwrapped type to hold a reference to in named<>
// Can't use hboost::unwrap_reference<> here because it
// doesn't handle the case where T = reference_wrapper<U> cv
template <class T>
struct unwrap_cv_reference
{
    typedef typename mpl::eval_if<
        is_cv_reference_wrapper<T>
      , get_type<T>
      , mpl::identity<T>
    >::type type;
};
#endif

}}} // namespace hboost::parameter::aux

#endif // HBOOST_UNWRAP_CV_REFERENCE_050328_HPP

