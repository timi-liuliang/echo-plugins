#if !defined(HBOOST_PP_IS_ITERATING)

// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

# ifndef HBOOST_SIGNATURE_DWA20021121_HPP
#  define HBOOST_SIGNATURE_DWA20021121_HPP

#  include <hboost/python/type_id.hpp>

#  include <hboost/python/detail/preprocessor.hpp>
#  include <hboost/python/detail/indirect_traits.hpp>
#  include <hboost/python/converter/pytype_function.hpp>

#  include <hboost/preprocessor/iterate.hpp>
#  include <hboost/preprocessor/iteration/local.hpp>

#  include <hboost/mpl/at.hpp>
#  include <hboost/mpl/size.hpp>

namespace hboost { namespace python { namespace detail { 

struct signature_element
{
    char const* basename;
    converter::pytype_function pytype_f;
    bool lvalue;
};

struct py_func_sig_info
{
    signature_element const *signature;
    signature_element const *ret;
};

template <unsigned> struct signature_arity;

#  define HBOOST_PP_ITERATION_PARAMS_1                                            \
        (3, (0, HBOOST_PYTHON_MAX_ARITY + 1, <hboost/python/detail/signature.hpp>))
#  include HBOOST_PP_ITERATE()

// A metafunction returning the base class used for
//
//   signature<class F, class CallPolicies, class Sig>.
//
template <class Sig>
struct signature_base_select
{
    enum { arity = mpl::size<Sig>::value - 1 };
    typedef typename signature_arity<arity>::template impl<Sig> type;
};

template <class Sig>
struct signature
    : signature_base_select<Sig>::type
{
};

}}} // namespace hboost::python::detail

# endif // HBOOST_SIGNATURE_DWA20021121_HPP

#else

# define N HBOOST_PP_ITERATION()

template <>
struct signature_arity<N>
{
    template <class Sig>
    struct impl
    {
        static signature_element const* elements()
        {
            static signature_element const result[N+2] = {
                
#ifndef HBOOST_PYTHON_NO_PY_SIGNATURES
# define HBOOST_PP_LOCAL_MACRO(i)                                                            \
                {                                                                           \
                  type_id<HBOOST_DEDUCED_TYPENAME mpl::at_c<Sig,i>::type>().name()           \
                  , &converter::expected_pytype_for_arg<HBOOST_DEDUCED_TYPENAME mpl::at_c<Sig,i>::type>::get_pytype   \
                  , indirect_traits::is_reference_to_non_const<HBOOST_DEDUCED_TYPENAME mpl::at_c<Sig,i>::type>::value \
                },
#else
# define HBOOST_PP_LOCAL_MACRO(i)                                                            \
                {                                                                           \
                  type_id<HBOOST_DEDUCED_TYPENAME mpl::at_c<Sig,i>::type>().name()           \
                  , 0 \
                  , indirect_traits::is_reference_to_non_const<HBOOST_DEDUCED_TYPENAME mpl::at_c<Sig,i>::type>::value \
                },
#endif
                
# define HBOOST_PP_LOCAL_LIMITS (0, N)
# include HBOOST_PP_LOCAL_ITERATE()
                {0,0,0}
            };
            return result;
        }
    };
};

#endif // HBOOST_PP_IS_ITERATING 


