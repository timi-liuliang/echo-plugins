// Copyright David Abrahams 2003.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_NULLARY_FUNCTION_ADAPTOR_DWA2003824_HPP
# define HBOOST_NULLARY_FUNCTION_ADAPTOR_DWA2003824_HPP

# include <hboost/python/detail/prefix.hpp>
# include <hboost/preprocessor/iteration/local.hpp>
# include <hboost/preprocessor/facilities/intercept.hpp>
# include <hboost/preprocessor/repetition/enum_params.hpp>
# include <hboost/preprocessor/repetition/enum_binary_params.hpp>

namespace hboost { namespace python { namespace detail { 

// nullary_function_adaptor -- a class template which ignores its
// arguments and calls a nullary function instead.  Used for building
// error-reporting functions, c.f. pure_virtual
template <class NullaryFunction>
struct nullary_function_adaptor
{
    nullary_function_adaptor(NullaryFunction fn)
      : m_fn(fn)
    {}

    void operator()() const { m_fn(); }

# define HBOOST_PP_LOCAL_MACRO(i)                                            \
    template <HBOOST_PP_ENUM_PARAMS_Z(1, i, class A)>                        \
    void operator()(                                                        \
        HBOOST_PP_ENUM_BINARY_PARAMS_Z(1, i, A, const& HBOOST_PP_INTERCEPT)   \
    ) const                                                                 \
    {                                                                       \
        m_fn();                                                             \
    }

# define HBOOST_PP_LOCAL_LIMITS (1, HBOOST_PYTHON_MAX_ARITY)
# include HBOOST_PP_LOCAL_ITERATE()
    
 private:
    NullaryFunction m_fn;
};

}}} // namespace hboost::python::detail

#endif // HBOOST_NULLARY_FUNCTION_ADAPTOR_DWA2003824_HPP
