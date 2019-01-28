#if !defined(HBOOST_PP_IS_ITERATING)

// Copyright David Abrahams 2001.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

# ifndef HBOOST_VALUE_HOLDER_DWA20011215_HPP
#  define HBOOST_VALUE_HOLDER_DWA20011215_HPP 

#  include <hboost/python/object/value_holder_fwd.hpp>

#  include <hboost/python/instance_holder.hpp>
#  include <hboost/python/type_id.hpp>
#  include <hboost/python/wrapper.hpp>

#  include <hboost/python/object/inheritance_query.hpp>
#  include <hboost/python/object/forward.hpp>

#  include <hboost/python/detail/force_instantiate.hpp>
#  include <hboost/python/detail/preprocessor.hpp>

#  include <hboost/preprocessor/comma_if.hpp>
#  include <hboost/preprocessor/enum_params.hpp>
#  include <hboost/preprocessor/iterate.hpp>
#  include <hboost/preprocessor/repeat.hpp>
#  include <hboost/preprocessor/debug/line.hpp>

#  include <hboost/preprocessor/repetition/enum_params.hpp>
#  include <hboost/preprocessor/repetition/enum_binary_params.hpp>

#  include <hboost/utility/addressof.hpp>

namespace hboost { namespace python { namespace objects { 

#define HBOOST_PYTHON_UNFORWARD_LOCAL(z, n, _) HBOOST_PP_COMMA_IF(n) objects::do_unforward(a##n,0)

template <class Value>
struct value_holder : instance_holder
{
    typedef Value held_type;
    typedef Value value_type;

    // Forward construction to the held object
#  define HBOOST_PP_ITERATION_PARAMS_1 (4, (0, HBOOST_PYTHON_MAX_ARITY, <hboost/python/object/value_holder.hpp>, 1))
#  include HBOOST_PP_ITERATE()

 private: // required holder implementation
    void* holds(type_info, bool null_ptr_only);
    
    template <class T>
    inline void* holds_wrapped(type_info dst_t, wrapper<T>*,T* p)
    {
        return python::type_id<T>() == dst_t ? p : 0;
    }
    
    inline void* holds_wrapped(type_info, ...)
    {
        return 0;
    }
 private: // data members
    Value m_held;
};

template <class Value, class Held>
struct value_holder_back_reference : instance_holder
{
    typedef Held held_type;
    typedef Value value_type;
    
    // Forward construction to the held object
#  define HBOOST_PP_ITERATION_PARAMS_1 (4, (0, HBOOST_PYTHON_MAX_ARITY, <hboost/python/object/value_holder.hpp>, 2))
#  include HBOOST_PP_ITERATE()

private: // required holder implementation
    void* holds(type_info, bool null_ptr_only);

 private: // data members
    Held m_held;
};

#  undef HBOOST_PYTHON_UNFORWARD_LOCAL

template <class Value>
void* value_holder<Value>::holds(type_info dst_t, bool /*null_ptr_only*/)
{
    if (void* wrapped = holds_wrapped(dst_t, hboost::addressof(m_held), hboost::addressof(m_held)))
        return wrapped;
    
    type_info src_t = python::type_id<Value>();
    return src_t == dst_t ? hboost::addressof(m_held)
        : find_static_type(hboost::addressof(m_held), src_t, dst_t);
}

template <class Value, class Held>
void* value_holder_back_reference<Value,Held>::holds(
    type_info dst_t, bool /*null_ptr_only*/)
{
    type_info src_t = python::type_id<Value>();
    Value* x = &m_held;
    
    if (dst_t == src_t)
        return x;
    else if (dst_t == python::type_id<Held>())
        return &m_held;
    else
        return find_static_type(x, src_t, dst_t);
}

}}} // namespace hboost::python::objects

# endif // HBOOST_VALUE_HOLDER_DWA20011215_HPP

// --------------- value_holder ---------------

// For gcc 4.4 compatability, we must include the
// HBOOST_PP_ITERATION_DEPTH test inside an #else clause.
#else // HBOOST_PP_IS_ITERATING
#if HBOOST_PP_ITERATION_DEPTH() == 1 && HBOOST_PP_ITERATION_FLAGS() == 1
# if !(HBOOST_WORKAROUND(__MWERKS__, > 0x3100)                      \
        && HBOOST_WORKAROUND(__MWERKS__, HBOOST_TESTED_AT(0x3201)))
#  line HBOOST_PP_LINE(__LINE__, value_holder.hpp(value_holder))
# endif

# define N HBOOST_PP_ITERATION()

# if (N != 0)
    template <HBOOST_PP_ENUM_PARAMS_Z(1, N, class A)>
# endif
    value_holder(
      PyObject* self HBOOST_PP_COMMA_IF(N) HBOOST_PP_ENUM_BINARY_PARAMS_Z(1, N, A, a))
        : m_held(
            HBOOST_PP_REPEAT_1ST(N, HBOOST_PYTHON_UNFORWARD_LOCAL, nil)
            )
    {
        python::detail::initialize_wrapper(self, hboost::addressof(this->m_held));
    }

# undef N

// --------------- value_holder_back_reference ---------------

#elif HBOOST_PP_ITERATION_DEPTH() == 1 && HBOOST_PP_ITERATION_FLAGS() == 2
# if !(HBOOST_WORKAROUND(__MWERKS__, > 0x3100)                      \
        && HBOOST_WORKAROUND(__MWERKS__, HBOOST_TESTED_AT(0x3201)))
#  line HBOOST_PP_LINE(__LINE__, value_holder.hpp(value_holder_back_reference))
# endif

# define N HBOOST_PP_ITERATION()

# if (N != 0)
    template <HBOOST_PP_ENUM_PARAMS_Z(1, N, class A)>
# endif
    value_holder_back_reference(
        PyObject* p HBOOST_PP_COMMA_IF(N) HBOOST_PP_ENUM_BINARY_PARAMS_Z(1, N, A, a))
        : m_held(
            p HBOOST_PP_COMMA_IF(N)
            HBOOST_PP_REPEAT_1ST(N, HBOOST_PYTHON_UNFORWARD_LOCAL, nil)
            )
    {
    }

# undef N

#endif // HBOOST_PP_ITERATION_DEPTH()
#endif
