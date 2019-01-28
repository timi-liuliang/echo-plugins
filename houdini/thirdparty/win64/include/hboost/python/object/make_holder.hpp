#if !defined(HBOOST_PP_IS_ITERATING)

// Copyright David Abrahams 2001.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

# ifndef HBOOST_MAKE_HOLDER_DWA20011215_HPP
#  define HBOOST_MAKE_HOLDER_DWA20011215_HPP

#  include <hboost/python/detail/prefix.hpp>

#  include <hboost/python/object/instance.hpp>
#  include <hboost/python/converter/registry.hpp>
#if !defined( HBOOST_PYTHON_NO_PY_SIGNATURES) && defined( HBOOST_PYTHON_PY_SIGNATURES_PROPER_INIT_SELF_TYPE)
#  include <hboost/python/detail/python_type.hpp>
#endif

#  include <hboost/python/object/forward.hpp>
#  include <hboost/python/detail/preprocessor.hpp>

#  include <hboost/mpl/next.hpp>
#  include <hboost/mpl/begin_end.hpp>
#  include <hboost/mpl/deref.hpp>

#  include <hboost/preprocessor/iterate.hpp>
#  include <hboost/preprocessor/iteration/local.hpp>
#  include <hboost/preprocessor/repeat.hpp>
#  include <hboost/preprocessor/debug/line.hpp>
#  include <hboost/preprocessor/repetition/enum_trailing_binary_params.hpp>

#  include <cstddef>

namespace hboost { namespace python { namespace objects {

template <int nargs> struct make_holder;

#  define HBOOST_PYTHON_DO_FORWARD_ARG(z, index, _) , f##index(a##index)

// specializations...
#  define HBOOST_PP_ITERATION_PARAMS_1 (3, (0, HBOOST_PYTHON_MAX_ARITY, <hboost/python/object/make_holder.hpp>))
#  include HBOOST_PP_ITERATE()

#  undef HBOOST_PYTHON_DO_FORWARD_ARG

}}} // namespace hboost::python::objects

# endif // HBOOST_MAKE_HOLDER_DWA20011215_HPP

// For gcc 4.4 compatability, we must include the
// HBOOST_PP_ITERATION_DEPTH test inside an #else clause.
#else // HBOOST_PP_IS_ITERATING
#if HBOOST_PP_ITERATION_DEPTH() == 1
# if !(HBOOST_WORKAROUND(__MWERKS__, > 0x3100)                      \
        && HBOOST_WORKAROUND(__MWERKS__, HBOOST_TESTED_AT(0x3201)))
#  line HBOOST_PP_LINE(__LINE__, make_holder.hpp)
# endif 

# define N HBOOST_PP_ITERATION()

template <>
struct make_holder<N>
{
    template <class Holder, class ArgList>
    struct apply
    {
# if N
        // Unrolled iteration through each argument type in ArgList,
        // choosing the type that will be forwarded on to the holder's
        // templated constructor.
        typedef typename mpl::begin<ArgList>::type iter0;
        
#  define HBOOST_PP_LOCAL_MACRO(n)               \
    typedef typename mpl::deref<iter##n>::type t##n;        \
    typedef typename forward<t##n>::type f##n;  \
    typedef typename mpl::next<iter##n>::type   \
        HBOOST_PP_CAT(iter,HBOOST_PP_INC(n)); // Next iterator type
        
#  define HBOOST_PP_LOCAL_LIMITS (0, N-1)
#  include HBOOST_PP_LOCAL_ITERATE()
# endif 
        
        static void execute(
#if !defined( HBOOST_PYTHON_NO_PY_SIGNATURES) && defined( HBOOST_PYTHON_PY_SIGNATURES_PROPER_INIT_SELF_TYPE)
            hboost::python::detail::python_class<HBOOST_DEDUCED_TYPENAME Holder::value_type> *p
#else
            PyObject *p
#endif
            HBOOST_PP_ENUM_TRAILING_BINARY_PARAMS_Z(1, N, t, a))
        {
            typedef instance<Holder> instance_t;
            
            void* memory = Holder::allocate(p, offsetof(instance_t, storage), sizeof(Holder));
            try {
                (new (memory) Holder(
                    p HBOOST_PP_REPEAT_1ST(N, HBOOST_PYTHON_DO_FORWARD_ARG, nil)))->install(p);
            }
            catch(...) {
                Holder::deallocate(p, memory);
                throw;
            }
        }
    };
};

# undef N

#endif // HBOOST_PP_ITERATION_DEPTH()
#endif
