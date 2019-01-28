// Copyright David Abrahams 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_CONCEPT_REQUIRES_DWA2006430_HPP
# define HBOOST_CONCEPT_REQUIRES_DWA2006430_HPP

# include <hboost/config.hpp>
# include <hboost/concept/assert.hpp>
# include <hboost/preprocessor/seq/for_each.hpp>

namespace hboost { 

// unaryfunptr_arg_type from parameter/aux_/parenthesized_type.hpp

namespace ccheck_aux {

// A metafunction that transforms void(*)(T) -> T
template <class UnaryFunctionPointer>
struct unaryfunptr_arg_type;

template <class Arg>
struct unaryfunptr_arg_type<void(*)(Arg)>
{
    typedef Arg type;
};

template <>
struct unaryfunptr_arg_type<void(*)(void)>
{
    typedef void type;
};

} // namespace ccheck_aux

// Template for use in handwritten assertions
template <class Model, class More>
struct requires_ : More
{
    HBOOST_CONCEPT_ASSERT((Model));
};

// Template for use by macros, where models must be wrapped in parens.
// This isn't in namespace detail to keep extra cruft out of resulting
// error messages.
template <class ModelFn>
struct _requires_
{
    enum { value = 0 };
    HBOOST_CONCEPT_ASSERT_FN(ModelFn);
};

template <int check, class Result>
struct Requires_ : ::hboost::ccheck_aux::unaryfunptr_arg_type<Result>
{
};

# if HBOOST_WORKAROUND(HBOOST_INTEL_WIN, HBOOST_TESTED_AT(1010))
#  define HBOOST_CONCEPT_REQUIRES_(r,data,t) | (::hboost::_requires_<void(*)t>::value)
# else 
#  define HBOOST_CONCEPT_REQUIRES_(r,data,t) + (::hboost::_requires_<void(*)t>::value)
# endif

#if defined(NDEBUG)

# define HBOOST_CONCEPT_REQUIRES(models, result)                                    \
    typename ::hboost::ccheck_aux::unaryfunptr_arg_type<void(*)result>::type

#elif HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x564))

// Same thing as below without the initial typename
# define HBOOST_CONCEPT_REQUIRES(models, result)                                \
    ::hboost::Requires_<                                                        \
      (0 HBOOST_PP_SEQ_FOR_EACH(HBOOST_CONCEPT_REQUIRES_, ~, models)),           \
      ::hboost::ccheck_aux::unaryfunptr_arg_type<void(*)result>          \
                     >::type

#else

// This just ICEs on MSVC6 :(
# define HBOOST_CONCEPT_REQUIRES(models, result)                                        \
    typename ::hboost::Requires_<                                                       \
      (0 HBOOST_PP_SEQ_FOR_EACH(HBOOST_CONCEPT_REQUIRES_, ~, models)),                   \
      void(*)result                                                                 \
    >::type

#endif 

// C++0x proposed syntax changed.  This supports an older usage
#define HBOOST_CONCEPT_WHERE(models,result) HBOOST_CONCEPT_REQUIRES(models,result)

} // namespace hboost::concept_check

#endif // HBOOST_CONCEPT_REQUIRES_DWA2006430_HPP
