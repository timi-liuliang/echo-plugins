///////////////////////////////////////////////////////////////////////////////
/// \file functional.hpp
///
//  Copyright 2005 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_NUMERIC_FUNCTIONAL_HPP_EAN_08_12_2005
#define HBOOST_NUMERIC_FUNCTIONAL_HPP_EAN_08_12_2005

#include <limits>
#include <functional>
#include <hboost/static_assert.hpp>
#include <hboost/mpl/if.hpp>
#include <hboost/mpl/and.hpp>
#include <hboost/type_traits/remove_const.hpp>
#include <hboost/type_traits/add_reference.hpp>
#include <hboost/type_traits/is_empty.hpp>
#include <hboost/type_traits/is_integral.hpp>
#include <hboost/type_traits/is_floating_point.hpp>
#include <hboost/utility/enable_if.hpp>
#include <hboost/typeof/typeof.hpp>
#include <hboost/accumulators/accumulators_fwd.hpp>
#include <hboost/accumulators/numeric/functional_fwd.hpp>
#include <hboost/accumulators/numeric/detail/function1.hpp>
#include <hboost/accumulators/numeric/detail/function2.hpp>
#include <hboost/accumulators/numeric/detail/pod_singleton.hpp>

#ifdef HBOOST_NUMERIC_FUNCTIONAL_STD_VECTOR_SUPPORT
# include <hboost/accumulators/numeric/functional/vector.hpp>
#endif

#ifdef HBOOST_NUMERIC_FUNCTIONAL_STD_VALARRAY_SUPPORT
# include <hboost/accumulators/numeric/functional/valarray.hpp>
#endif

#ifdef HBOOST_NUMERIC_FUNCTIONAL_STD_COMPLEX_SUPPORT
# include <hboost/accumulators/numeric/functional/complex.hpp>
#endif

/// INTERNAL ONLY
///
#define HBOOST_NUMERIC_FUNCTIONAL_HPP_INCLUDED

#ifdef HBOOST_NUMERIC_FUNCTIONAL_DOXYGEN_INVOKED
// Hack to make Doxygen show the inheritance relationships
/// INTERNAL ONLY
///
namespace std
{
    /// INTERNAL ONLY
    ///
    template<class Arg, class Ret> struct unary_function {};
    /// INTERNAL ONLY
    ///
    template<class Left, class Right, class Ret> struct binary_function {};
}
#endif

namespace hboost { namespace numeric
{
    namespace functional
    {
        /// INTERNAL ONLY
        ///
        template<typename A0, typename A1>
        struct are_integral
          : mpl::and_<is_integral<A0>, is_integral<A1> >
        {};

        template<typename Left, typename Right>
        struct left_ref
        {
            typedef Left &type;
        };

        namespace detail
        {
            template<typename T>
            T &lvalue_of();
        }
    }

    // TODO: handle complex weight, valarray, MTL vectors

    /// INTERNAL ONLY
    ///
#define HBOOST_NUMERIC_FUNCTIONAL_DEFINE_UNARY_OP(Name, Op)                                      \
    namespace functional                                                                        \
    {                                                                                           \
        template<typename Arg>                                                                  \
        struct result_of_ ## Name                                                               \
        {                                                                                       \
            HBOOST_TYPEOF_NESTED_TYPEDEF_TPL(                                                    \
                nested                                                                          \
              , Op hboost::numeric::functional::detail::lvalue_of<Arg>()                         \
            )                                                                                   \
            typedef typename nested::type type;                                                 \
        };                                                                                      \
        template<typename Arg, typename EnableIf>                                               \
        struct Name ## _base                                                                    \
          : std::unary_function<                                                                \
                typename remove_const<Arg>::type                                                \
              , typename result_of_ ## Name<Arg>::type                                          \
            >                                                                                   \
        {                                                                                       \
            typename result_of_ ## Name<Arg>::type operator ()(Arg &arg) const                  \
            {                                                                                   \
                return Op arg;                                                                  \
            }                                                                                   \
        };                                                                                      \
        template<typename Arg, typename ArgTag>                                                 \
        struct Name                                                                             \
          : Name ## _base<Arg, void>                                                            \
        {};                                                                                     \
    }                                                                                           \
    namespace op                                                                                \
    {                                                                                           \
        struct Name                                                                             \
          : hboost::detail::function1<functional::Name<_, functional::tag<_> > >                 \
        {};                                                                                     \
    }                                                                                           \
    namespace                                                                                   \
    {                                                                                           \
        op::Name const &Name = hboost::detail::pod_singleton<op::Name>::instance;                \
    }                                                                                           \
    /**/

    /// INTERNAL ONLY
    ///
#define HBOOST_NUMERIC_FUNCTIONAL_DEFINE_BINARY_OP(Name, Op, RetType)                            \
    namespace functional                                                                        \
    {                                                                                           \
        template<typename Left, typename Right, typename EnableIf>                              \
        struct result_of_ ## Name                                                               \
        {                                                                                       \
            RetType(Left, Op, Right)                                                            \
        };                                                                                      \
        template<typename Left, typename Right, typename EnableIf>                              \
        struct Name ## _base                                                                    \
          : std::binary_function<                                                               \
                typename remove_const<Left>::type                                               \
              , typename remove_const<Right>::type                                              \
              , typename result_of_ ## Name<Left, Right>::type                                  \
            >                                                                                   \
        {                                                                                       \
            typename result_of_ ## Name<Left, Right>::type                                      \
            operator ()(Left &left, Right &right) const                                         \
            {                                                                                   \
                return left Op right;                                                           \
            }                                                                                   \
        };                                                                                      \
        template<typename Left, typename Right, typename LeftTag, typename RightTag>            \
        struct Name                                                                             \
          : Name ## _base<Left, Right, void>                                                    \
        {};                                                                                     \
    }                                                                                           \
    namespace op                                                                                \
    {                                                                                           \
        struct Name                                                                             \
          : hboost::detail::function2<                                                           \
                functional::Name<_1, _2, functional::tag<_1>, functional::tag<_2> >             \
            >                                                                                   \
        {};                                                                                     \
    }                                                                                           \
    namespace                                                                                   \
    {                                                                                           \
        op::Name const &Name = hboost::detail::pod_singleton<op::Name>::instance;                \
    }                                                                                           \
    HBOOST_ACCUMULATORS_IGNORE_GLOBAL(Name)                                                      \
    /**/

    /// INTERNAL ONLY
    ///
#define HBOOST_NUMERIC_FUNCTIONAL_DEDUCED(Left, Op, Right)                                       \
    HBOOST_TYPEOF_NESTED_TYPEDEF_TPL(                                                            \
        nested                                                                                  \
      , hboost::numeric::functional::detail::lvalue_of<Left>() Op                                \
        hboost::numeric::functional::detail::lvalue_of<Right>()                                  \
    )                                                                                           \
    typedef typename nested::type type;                                                         \
    /**/

    /// INTERNAL ONLY
    ///
#define HBOOST_NUMERIC_FUNCTIONAL_LEFT(Left, Op, Right)                                          \
    typedef Left &type;                                                                         \
    /**/

    HBOOST_NUMERIC_FUNCTIONAL_DEFINE_BINARY_OP(plus, +, HBOOST_NUMERIC_FUNCTIONAL_DEDUCED)
    HBOOST_NUMERIC_FUNCTIONAL_DEFINE_BINARY_OP(minus, -, HBOOST_NUMERIC_FUNCTIONAL_DEDUCED)
    HBOOST_NUMERIC_FUNCTIONAL_DEFINE_BINARY_OP(multiplies, *, HBOOST_NUMERIC_FUNCTIONAL_DEDUCED)
    HBOOST_NUMERIC_FUNCTIONAL_DEFINE_BINARY_OP(divides, /, HBOOST_NUMERIC_FUNCTIONAL_DEDUCED)
    HBOOST_NUMERIC_FUNCTIONAL_DEFINE_BINARY_OP(modulus, %, HBOOST_NUMERIC_FUNCTIONAL_DEDUCED)
    HBOOST_NUMERIC_FUNCTIONAL_DEFINE_BINARY_OP(greater, >, HBOOST_NUMERIC_FUNCTIONAL_DEDUCED)
    HBOOST_NUMERIC_FUNCTIONAL_DEFINE_BINARY_OP(greater_equal, >=, HBOOST_NUMERIC_FUNCTIONAL_DEDUCED)
    HBOOST_NUMERIC_FUNCTIONAL_DEFINE_BINARY_OP(less, <, HBOOST_NUMERIC_FUNCTIONAL_DEDUCED)
    HBOOST_NUMERIC_FUNCTIONAL_DEFINE_BINARY_OP(less_equal, <=, HBOOST_NUMERIC_FUNCTIONAL_DEDUCED)
    HBOOST_NUMERIC_FUNCTIONAL_DEFINE_BINARY_OP(equal_to, ==, HBOOST_NUMERIC_FUNCTIONAL_DEDUCED)
    HBOOST_NUMERIC_FUNCTIONAL_DEFINE_BINARY_OP(not_equal_to, !=, HBOOST_NUMERIC_FUNCTIONAL_DEDUCED)

    HBOOST_NUMERIC_FUNCTIONAL_DEFINE_BINARY_OP(assign, =, HBOOST_NUMERIC_FUNCTIONAL_LEFT)
    HBOOST_NUMERIC_FUNCTIONAL_DEFINE_BINARY_OP(plus_assign, +=, HBOOST_NUMERIC_FUNCTIONAL_LEFT)
    HBOOST_NUMERIC_FUNCTIONAL_DEFINE_BINARY_OP(minus_assign, -=, HBOOST_NUMERIC_FUNCTIONAL_LEFT)
    HBOOST_NUMERIC_FUNCTIONAL_DEFINE_BINARY_OP(multiplies_assign, *=, HBOOST_NUMERIC_FUNCTIONAL_LEFT)
    HBOOST_NUMERIC_FUNCTIONAL_DEFINE_BINARY_OP(divides_assign, /=, HBOOST_NUMERIC_FUNCTIONAL_LEFT)
    HBOOST_NUMERIC_FUNCTIONAL_DEFINE_BINARY_OP(modulus_assign, %=, HBOOST_NUMERIC_FUNCTIONAL_LEFT)

    HBOOST_NUMERIC_FUNCTIONAL_DEFINE_UNARY_OP(unary_plus, +)
    HBOOST_NUMERIC_FUNCTIONAL_DEFINE_UNARY_OP(unary_minus, -)
    HBOOST_NUMERIC_FUNCTIONAL_DEFINE_UNARY_OP(complement, ~)
    HBOOST_NUMERIC_FUNCTIONAL_DEFINE_UNARY_OP(logical_not, !)

#undef HBOOST_NUMERIC_FUNCTIONAL_LEFT
#undef HBOOST_NUMERIC_FUNCTIONAL_DEDUCED
#undef HBOOST_NUMERIC_FUNCTIONAL_DEFINE_UNARY_OP
#undef HBOOST_NUMERIC_FUNCTIONAL_DEFINE_BINARY_OP

    namespace functional
    {
        template<typename Left, typename Right, typename EnableIf>
        struct min_assign_base
          : std::binary_function<Left, Right, void>
        {
            void operator ()(Left &left, Right &right) const
            {
                if(numeric::less(right, left))
                {
                    left = right;
                }
            }
        };

        template<typename Left, typename Right, typename EnableIf>
        struct max_assign_base
          : std::binary_function<Left, Right, void>
        {
            void operator ()(Left &left, Right &right) const
            {
                if(numeric::greater(right, left))
                {
                    left = right;
                }
            }
        };

        template<typename Left, typename Right, typename EnableIf>
        struct fdiv_base
          : functional::divides<Left, Right>
        {};

        // partial specialization that promotes the arguments to double for
        // integral division.
        template<typename Left, typename Right>
        struct fdiv_base<Left, Right, typename enable_if<are_integral<Left, Right> >::type>
          : functional::divides<double const, double const>
        {};

        template<typename To, typename From, typename EnableIf>
        struct promote_base
          : std::unary_function<From, To>
        {
            To operator ()(From &from) const
            {
                return from;
            }
        };

        template<typename ToFrom>
        struct promote_base<ToFrom, ToFrom, void>
          : std::unary_function<ToFrom, ToFrom>
        {
            ToFrom &operator ()(ToFrom &tofrom)
            {
                return tofrom;
            }
        };

        template<typename Arg, typename EnableIf>
        struct as_min_base
          : std::unary_function<Arg, typename remove_const<Arg>::type>
        {
            HBOOST_STATIC_ASSERT(std::numeric_limits<typename remove_const<Arg>::type>::is_specialized);

            typename remove_const<Arg>::type operator ()(Arg &) const
            {
                return (std::numeric_limits<typename remove_const<Arg>::type>::min)();
            }
        };

        template<typename Arg>
        struct as_min_base<Arg, typename enable_if<is_floating_point<Arg> >::type>
          : std::unary_function<Arg, typename remove_const<Arg>::type>
        {
            HBOOST_STATIC_ASSERT(std::numeric_limits<typename remove_const<Arg>::type>::is_specialized);

            typename remove_const<Arg>::type operator ()(Arg &) const
            {
                return -(std::numeric_limits<typename remove_const<Arg>::type>::max)();
            }
        };

        template<typename Arg, typename EnableIf>
        struct as_max_base
          : std::unary_function<Arg, typename remove_const<Arg>::type>
        {
            HBOOST_STATIC_ASSERT(std::numeric_limits<typename remove_const<Arg>::type>::is_specialized);

            typename remove_const<Arg>::type operator ()(Arg &) const
            {
                return (std::numeric_limits<typename remove_const<Arg>::type>::max)();
            }
        };

        template<typename Arg, typename EnableIf>
        struct as_zero_base
          : std::unary_function<Arg, typename remove_const<Arg>::type>
        {
            typename remove_const<Arg>::type operator ()(Arg &) const
            {
                return numeric::zero<typename remove_const<Arg>::type>::value;
            }
        };

        template<typename Arg, typename EnableIf>
        struct as_one_base
          : std::unary_function<Arg, typename remove_const<Arg>::type>
        {
            typename remove_const<Arg>::type operator ()(Arg &) const
            {
                return numeric::one<typename remove_const<Arg>::type>::value;
            }
        };

        template<typename To, typename From, typename ToTag, typename FromTag>
        struct promote
          : promote_base<To, From, void>
        {};

        template<typename Left, typename Right, typename LeftTag, typename RightTag>
        struct min_assign
          : min_assign_base<Left, Right, void>
        {};

        template<typename Left, typename Right, typename LeftTag, typename RightTag>
        struct max_assign
          : max_assign_base<Left, Right, void>
        {};

        template<typename Left, typename Right, typename LeftTag, typename RightTag>
        struct fdiv
          : fdiv_base<Left, Right, void>
        {};

        /// INTERNAL ONLY 
        /// For back-compat only. Use fdiv.
        template<typename Left, typename Right, typename LeftTag, typename RightTag>
        struct average
          : fdiv<Left, Right, LeftTag, RightTag>
        {};

        template<typename Arg, typename Tag>
        struct as_min
          : as_min_base<Arg, void>
        {};

        template<typename Arg, typename Tag>
        struct as_max
          : as_max_base<Arg, void>
        {};

        template<typename Arg, typename Tag>
        struct as_zero
          : as_zero_base<Arg, void>
        {};

        template<typename Arg, typename Tag>
        struct as_one
          : as_one_base<Arg, void>
        {};
    }

    namespace op
    {
        template<typename To>
        struct promote
          : hboost::detail::function1<functional::promote<To, _, typename functional::tag<To>::type, functional::tag<_> > >
        {};

        struct min_assign
          : hboost::detail::function2<functional::min_assign<_1, _2, functional::tag<_1>, functional::tag<_2> > >
        {};

        struct max_assign
          : hboost::detail::function2<functional::max_assign<_1, _2, functional::tag<_1>, functional::tag<_2> > >
        {};

        struct fdiv
          : hboost::detail::function2<functional::fdiv<_1, _2, functional::tag<_1>, functional::tag<_2> > >
        {};

        /// INTERNAL ONLY
        struct average
          : hboost::detail::function2<functional::fdiv<_1, _2, functional::tag<_1>, functional::tag<_2> > >
        {};

        struct as_min
          : hboost::detail::function1<functional::as_min<_, functional::tag<_> > >
        {};

        struct as_max
          : hboost::detail::function1<functional::as_max<_, functional::tag<_> > >
        {};

        struct as_zero
          : hboost::detail::function1<functional::as_zero<_, functional::tag<_> > >
        {};

        struct as_one
          : hboost::detail::function1<functional::as_one<_, functional::tag<_> > >
        {};
    }

    namespace
    {
        op::min_assign const &min_assign = hboost::detail::pod_singleton<op::min_assign>::instance;
        op::max_assign const &max_assign = hboost::detail::pod_singleton<op::max_assign>::instance;
        op::fdiv const &fdiv = hboost::detail::pod_singleton<op::fdiv>::instance;
        op::fdiv const &average = hboost::detail::pod_singleton<op::fdiv>::instance; ///< INTERNAL ONLY
        op::as_min const &as_min = hboost::detail::pod_singleton<op::as_min>::instance;
        op::as_max const &as_max = hboost::detail::pod_singleton<op::as_max>::instance;
        op::as_zero const &as_zero = hboost::detail::pod_singleton<op::as_zero>::instance;
        op::as_one const &as_one = hboost::detail::pod_singleton<op::as_one>::instance;

        HBOOST_ACCUMULATORS_IGNORE_GLOBAL(min_assign)
        HBOOST_ACCUMULATORS_IGNORE_GLOBAL(max_assign)
        HBOOST_ACCUMULATORS_IGNORE_GLOBAL(fdiv)
        HBOOST_ACCUMULATORS_IGNORE_GLOBAL(average)
        HBOOST_ACCUMULATORS_IGNORE_GLOBAL(as_min)
        HBOOST_ACCUMULATORS_IGNORE_GLOBAL(as_max)
        HBOOST_ACCUMULATORS_IGNORE_GLOBAL(as_zero)
        HBOOST_ACCUMULATORS_IGNORE_GLOBAL(as_one)
    }

    ///////////////////////////////////////////////////////////////////////////////
    // promote
    template<typename To, typename From>
    typename lazy_disable_if<is_const<From>, mpl::if_<is_same<To, From>, To &, To> >::type
    promote(From &from)
    {
        return functional::promote<To, From>()(from);
    }

    template<typename To, typename From>
    typename mpl::if_<is_same<To const, From const>, To const &, To const>::type
    promote(From const &from)
    {
        return functional::promote<To const, From const>()(from);
    }

    template<typename T>
    struct default_
    {
        typedef default_ type;
        typedef T value_type;
        static T const value;

        operator T const & () const
        {
            return default_::value;
        }
    };

    template<typename T>
    T const default_<T>::value = T();

    template<typename T>
    struct one
    {
        typedef one type;
        typedef T value_type;
        static T const value;

        operator T const & () const
        {
            return one::value;
        }
    };

    template<typename T>
    T const one<T>::value = T(1);

    template<typename T>
    struct zero
    {
        typedef zero type;
        typedef T value_type;
        static T const value;

        operator T const & () const
        {
            return zero::value;
        }
    };

    template<typename T>
    T const zero<T>::value = T();

    template<typename T>
    struct one_or_default
      : mpl::if_<is_empty<T>, default_<T>, one<T> >::type
    {};

    template<typename T>
    struct zero_or_default
      : mpl::if_<is_empty<T>, default_<T>, zero<T> >::type
    {};

}} // namespace hboost::numeric

#endif
