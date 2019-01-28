// Copyright Shreyans Doshi 2017.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_PYTHON_DETAIL_TYPE_TRAITS_HPP
# define HBOOST_PYTHON_DETAIL_TYPE_TRAITS_HPP


#include <hboost/config.hpp>
#ifdef HBOOST_NO_CXX11_HDR_TYPE_TRAITS
# include <hboost/type_traits/transform_traits.hpp>
# include <hboost/type_traits/same_traits.hpp>
# include <hboost/type_traits/cv_traits.hpp>
# include <hboost/type_traits/is_polymorphic.hpp>
# include <hboost/type_traits/composite_traits.hpp>
# include <hboost/type_traits/conversion_traits.hpp>
# include <hboost/type_traits/add_pointer.hpp>
# include <hboost/type_traits/remove_pointer.hpp>
# include <hboost/type_traits/is_void.hpp>
# include <hboost/type_traits/object_traits.hpp>
# include <hboost/type_traits/add_lvalue_reference.hpp>
# include <hboost/type_traits/function_traits.hpp>
# include <hboost/type_traits/is_scalar.hpp>
# include <hboost/type_traits/alignment_traits.hpp>
# include <hboost/mpl/bool.hpp>
#else
# include <type_traits>
#endif

# include <hboost/type_traits/is_base_and_derived.hpp>
# include <hboost/type_traits/alignment_traits.hpp>
# include <hboost/type_traits/has_trivial_copy.hpp>


namespace hboost { namespace python { namespace detail {

#ifdef HBOOST_NO_CXX11_HDR_TYPE_TRAITS
    using hboost::alignment_of;
    using hboost::add_const;
    using hboost::add_cv;
    using hboost::add_lvalue_reference;
    using hboost::add_pointer;

    using hboost::is_array;
    using hboost::is_class;
    using hboost::is_const;
    using hboost::is_convertible;
    using hboost::is_enum;
    using hboost::is_function;
    using hboost::is_integral;
    using hboost::is_lvalue_reference;
    using hboost::is_member_function_pointer;
    using hboost::is_member_pointer;
    using hboost::is_pointer;
    using hboost::is_polymorphic;
    using hboost::is_reference;
    using hboost::is_same;
    using hboost::is_scalar;
    using hboost::is_union;
    using hboost::is_void;
    using hboost::is_volatile;

    using hboost::remove_reference;
    using hboost::remove_pointer;
    using hboost::remove_cv;
    using hboost::remove_const;

    using hboost::mpl::true_;
    using hboost::mpl::false_;
#else
    using std::alignment_of;
    using std::add_const;
    using std::add_cv;
    using std::add_lvalue_reference;
    using std::add_pointer;

    using std::is_array;
    using std::is_class;
    using std::is_const;
    using std::is_convertible;
    using std::is_enum;
    using std::is_function;
    using std::is_integral;
    using std::is_lvalue_reference;
    using std::is_member_function_pointer;
    using std::is_member_pointer;
    using std::is_pointer;
    using std::is_polymorphic;
    using std::is_reference;
    using std::is_same;
    using std::is_scalar;
    using std::is_union;
    using std::is_void;
    using std::is_volatile;

    using std::remove_reference;
    using std::remove_pointer;
    using std::remove_cv;
    using std::remove_const;

    typedef std::integral_constant<bool, true> true_;
    typedef std::integral_constant<bool, false> false_;
#endif
    using hboost::is_base_and_derived;
    using hboost::type_with_alignment;
    using hboost::has_trivial_copy;
}}} // namespace hboost::python::detail


#endif //HBOOST_DETAIL_TYPE_TRAITS_HPP
