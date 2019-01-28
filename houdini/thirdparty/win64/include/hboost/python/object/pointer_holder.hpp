#if !defined(HBOOST_PP_IS_ITERATING)

// Copyright David Abrahams 2001.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

# ifndef HBOOST_POINTER_HOLDER_DWA20011215_HPP
#  define HBOOST_POINTER_HOLDER_DWA20011215_HPP 

# include <hboost/get_pointer.hpp>
#  include <hboost/type.hpp>

#  include <hboost/python/instance_holder.hpp>
#  include <hboost/python/object/inheritance_query.hpp>
#  include <hboost/python/object/forward.hpp>

#  include <hboost/python/pointee.hpp>
#  include <hboost/python/type_id.hpp>

#  include <hboost/python/detail/wrapper_base.hpp>
#  include <hboost/python/detail/force_instantiate.hpp>
#  include <hboost/python/detail/preprocessor.hpp>


#  include <hboost/mpl/if.hpp>
#  include <hboost/mpl/apply.hpp>

#  include <hboost/preprocessor/comma_if.hpp>
#  include <hboost/preprocessor/iterate.hpp>
#  include <hboost/preprocessor/repeat.hpp>
#  include <hboost/preprocessor/debug/line.hpp>
#  include <hboost/preprocessor/enum_params.hpp>
#  include <hboost/preprocessor/repetition/enum_binary_params.hpp>

#  include <hboost/detail/workaround.hpp>

#  include <hboost/type_traits/remove_const.hpp>

namespace hboost { namespace python {

template <class T> class wrapper;

}}


namespace hboost { namespace python { namespace objects {

#define HBOOST_PYTHON_UNFORWARD_LOCAL(z, n, _) HBOOST_PP_COMMA_IF(n) objects::do_unforward(a##n,0)

template <class Pointer, class Value>
struct pointer_holder : instance_holder
{
    typedef Value value_type;
    
    pointer_holder(Pointer);

    // Forward construction to the held object

#  define HBOOST_PP_ITERATION_PARAMS_1 (4, (0, HBOOST_PYTHON_MAX_ARITY, <hboost/python/object/pointer_holder.hpp>, 1))
#  include HBOOST_PP_ITERATE()

 private: // types
    
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
    Pointer m_p;
};

template <class Pointer, class Value>
struct pointer_holder_back_reference : instance_holder
{
 private:
    typedef typename python::pointee<Pointer>::type held_type;
 public:
    typedef Value value_type;

    // Not sure about this one -- can it work? The source object
    // undoubtedly does not carry the correct back reference pointer.
    pointer_holder_back_reference(Pointer);

    // Forward construction to the held object
#  define HBOOST_PP_ITERATION_PARAMS_1 (4, (0, HBOOST_PYTHON_MAX_ARITY, <hboost/python/object/pointer_holder.hpp>, 2))
#  include HBOOST_PP_ITERATE()

 private: // required holder implementation
    void* holds(type_info, bool null_ptr_only);

 private: // data members
    Pointer m_p;
};

#  undef HBOOST_PYTHON_UNFORWARD_LOCAL

template <class Pointer, class Value>
inline pointer_holder<Pointer,Value>::pointer_holder(Pointer p)
    : m_p(p)
{
}

template <class Pointer, class Value>
inline pointer_holder_back_reference<Pointer,Value>::pointer_holder_back_reference(Pointer p)
    : m_p(p)
{
}

template <class Pointer, class Value>
void* pointer_holder<Pointer, Value>::holds(type_info dst_t, bool null_ptr_only)
{
    typedef typename hboost::remove_const< Value >::type non_const_value;

    if (dst_t == python::type_id<Pointer>()
        && !(null_ptr_only && get_pointer(this->m_p))
    )
        return &this->m_p;

    Value* p0
#  if HBOOST_WORKAROUND(__SUNPRO_CC, HBOOST_TESTED_AT(0x590))
        = static_cast<Value*>( get_pointer(this->m_p) )
#  else 
        = get_pointer(this->m_p)
#  endif
        ;
    non_const_value* p = const_cast<non_const_value*>( p0 );

    if (p == 0)
        return 0;
    
    if (void* wrapped = holds_wrapped(dst_t, p, p))
        return wrapped;
    
    type_info src_t = python::type_id<non_const_value>();
    return src_t == dst_t ? p : find_dynamic_type(p, src_t, dst_t);
}

template <class Pointer, class Value>
void* pointer_holder_back_reference<Pointer, Value>::holds(type_info dst_t, bool null_ptr_only)
{
    if (dst_t == python::type_id<Pointer>()
        && !(null_ptr_only && get_pointer(this->m_p))
    )
        return &this->m_p;

    if (!get_pointer(this->m_p))
        return 0;
    
    Value* p = get_pointer(m_p);
    
    if (dst_t == python::type_id<held_type>())
        return p;

    type_info src_t = python::type_id<Value>();
    return src_t == dst_t ? p : find_dynamic_type(p, src_t, dst_t);
}

}}} // namespace hboost::python::objects

# endif // HBOOST_POINTER_HOLDER_DWA20011215_HPP

/* --------------- pointer_holder --------------- */
// For gcc 4.4 compatability, we must include the
// HBOOST_PP_ITERATION_DEPTH test inside an #else clause.
#else // HBOOST_PP_IS_ITERATING
#if HBOOST_PP_ITERATION_DEPTH() == 1 && HBOOST_PP_ITERATION_FLAGS() == 1
# if !(HBOOST_WORKAROUND(__MWERKS__, > 0x3100)                      \
        && HBOOST_WORKAROUND(__MWERKS__, HBOOST_TESTED_AT(0x3201)))
#  line HBOOST_PP_LINE(__LINE__, pointer_holder.hpp)
# endif

# define N HBOOST_PP_ITERATION()

# if (N != 0)
    template< HBOOST_PP_ENUM_PARAMS_Z(1, N, class A) >
# endif
    pointer_holder(PyObject* self HBOOST_PP_COMMA_IF(N) HBOOST_PP_ENUM_BINARY_PARAMS_Z(1, N, A, a))
        : m_p(new Value(
                HBOOST_PP_REPEAT_1ST(N, HBOOST_PYTHON_UNFORWARD_LOCAL, nil)
            ))
    {
        python::detail::initialize_wrapper(self, get_pointer(this->m_p));
    }

# undef N

/* --------------- pointer_holder_back_reference --------------- */
#elif HBOOST_PP_ITERATION_DEPTH() == 1 && HBOOST_PP_ITERATION_FLAGS() == 2
# if !(HBOOST_WORKAROUND(__MWERKS__, > 0x3100)                      \
        && HBOOST_WORKAROUND(__MWERKS__, HBOOST_TESTED_AT(0x3201)))
#  line HBOOST_PP_LINE(__LINE__, pointer_holder.hpp(pointer_holder_back_reference))
# endif 

# define N HBOOST_PP_ITERATION()

# if (N != 0)
    template < HBOOST_PP_ENUM_PARAMS_Z(1, N, class A) >
# endif
    pointer_holder_back_reference(
        PyObject* p HBOOST_PP_COMMA_IF(N) HBOOST_PP_ENUM_BINARY_PARAMS_Z(1, N, A, a))
        : m_p(new held_type(
                    p HBOOST_PP_COMMA_IF(N) HBOOST_PP_REPEAT_1ST(N, HBOOST_PYTHON_UNFORWARD_LOCAL, nil)
            ))
    {}

# undef N

#endif // HBOOST_PP_ITERATION_DEPTH()
#endif
