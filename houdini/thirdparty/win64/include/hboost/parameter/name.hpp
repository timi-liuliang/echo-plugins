// Copyright Daniel Wallin 2006. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_PARAMETER_NAME_060806_HPP
# define HBOOST_PARAMETER_NAME_060806_HPP

# include <hboost/parameter/keyword.hpp>
# include <hboost/parameter/value_type.hpp>
# include <hboost/detail/workaround.hpp>
# include <hboost/preprocessor/cat.hpp>
# include <hboost/preprocessor/stringize.hpp>
# include <hboost/preprocessor/control/iif.hpp>
# include <hboost/preprocessor/tuple/eat.hpp>
# include <hboost/preprocessor/tuple/elem.hpp>
# include <hboost/mpl/placeholders.hpp>

# if !defined(HBOOST_NO_SFINAE) \
  && !HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x592))

#  include <hboost/utility/enable_if.hpp>
#  include <hboost/mpl/lambda.hpp>

namespace hboost { namespace parameter { namespace aux {

// Tag type passed to MPL lambda.
struct lambda_tag;

struct name_tag_base
{};

template <class Tag>
struct name_tag
{};

template <class T>
struct is_name_tag
  : mpl::false_
{};

}}} // namespace hboost::parameter::aux

namespace hboost { namespace mpl {

template <class T>
struct lambda<
    T
  , typename hboost::enable_if<
        parameter::aux::is_name_tag<T>, parameter::aux::lambda_tag
    >::type
>
{
    typedef true_ is_le;
    typedef bind3< quote3<parameter::value_type>, arg<2>, T, void> result_;
    typedef result_ type;
};

}} // namespace hboost::mpl

# endif

# if HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x564))
# include <hboost/preprocessor/detail/split.hpp>
// From Paul Mensonides
#  define HBOOST_PARAMETER_IS_BINARY(x) \
    HBOOST_PP_SPLIT(1, HBOOST_PARAMETER_IS_BINARY_C x HBOOST_PP_COMMA() 0) \
    /**/
#  define HBOOST_PARAMETER_IS_BINARY_C(x,y) \
    ~, 1 HBOOST_PP_RPAREN() \
    HBOOST_PP_TUPLE_EAT(2) HBOOST_PP_LPAREN() ~ \
    /**/
# else
#  include <hboost/preprocessor/detail/is_binary.hpp>
#  define HBOOST_PARAMETER_IS_BINARY(x) HBOOST_PP_IS_BINARY(x)
# endif

# define HBOOST_PARAMETER_BASIC_NAME(tag_namespace, tag, name)       \
    namespace tag_namespace                                         \
    {                                                               \
      struct tag                                                    \
      {                                                             \
          static char const* keyword_name()                         \
          {                                                         \
              return HBOOST_PP_STRINGIZE(tag);                       \
          }                                                         \
                                                                    \
          typedef hboost::parameter::value_type<                     \
              hboost::mpl::_2, tag, hboost::parameter::void_          \
          > _;                                                      \
                                                                    \
          typedef hboost::parameter::value_type<                     \
              hboost::mpl::_2, tag, hboost::parameter::void_          \
          > _1;                                                     \
      };                                                            \
    }                                                               \
    namespace                                                       \
    {                                                               \
       ::hboost::parameter::keyword<tag_namespace::tag> const& name  \
       = ::hboost::parameter::keyword<tag_namespace::tag>::instance; \
    }

# define HBOOST_PARAMETER_COMPLEX_NAME_TUPLE1(tag,namespace)         \
    (tag, namespace), ~

# define HBOOST_PARAMETER_COMPLEX_NAME_TUPLE(name)                   \
    HBOOST_PP_TUPLE_ELEM(2, 0, (HBOOST_PARAMETER_COMPLEX_NAME_TUPLE1 name))

# define HBOOST_PARAMETER_COMPLEX_NAME_TAG(name)                     \
    HBOOST_PP_TUPLE_ELEM(2, 0, HBOOST_PARAMETER_COMPLEX_NAME_TUPLE(name))

# define HBOOST_PARAMETER_COMPLEX_NAME_NAMESPACE(name)               \
    HBOOST_PP_TUPLE_ELEM(2, 1, HBOOST_PARAMETER_COMPLEX_NAME_TUPLE(name))

# define HBOOST_PARAMETER_COMPLEX_NAME(name)                         \
    HBOOST_PARAMETER_BASIC_NAME(                                     \
        HBOOST_PARAMETER_COMPLEX_NAME_NAMESPACE(name)                \
      , HBOOST_PP_TUPLE_EAT(2) name                                  \
      , HBOOST_PARAMETER_COMPLEX_NAME_TAG(name)                      \
    )                                                               \
/**/

# define HBOOST_PARAMETER_SIMPLE_NAME(name)                          \
    HBOOST_PARAMETER_BASIC_NAME(tag, name, HBOOST_PP_CAT(_, name))

# define HBOOST_PARAMETER_NAME(name)                                 \
    HBOOST_PP_IIF(                                                   \
        HBOOST_PARAMETER_IS_BINARY(name)                             \
      , HBOOST_PARAMETER_COMPLEX_NAME                                \
      , HBOOST_PARAMETER_SIMPLE_NAME                                 \
    )(name)                                                         \
/**/


# define HBOOST_PARAMETER_TEMPLATE_KEYWORD(name)                     \
    namespace tag                                                   \
    {                                                               \
      struct name;                                                  \
    }                                                               \
    template <class T>                                              \
    struct name                                                     \
      : hboost::parameter::template_keyword<tag::name, T>            \
    {};                                                             \
/**/

#endif // HBOOST_PARAMETER_NAME_060806_HPP

