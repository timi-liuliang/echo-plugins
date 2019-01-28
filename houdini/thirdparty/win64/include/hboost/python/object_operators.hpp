// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_OBJECT_OPERATORS_DWA2002617_HPP
# define HBOOST_OBJECT_OPERATORS_DWA2002617_HPP

# include <hboost/python/detail/prefix.hpp>

# include <hboost/python/object_core.hpp>
# include <hboost/python/call.hpp>
# include <hboost/iterator/detail/enable_if.hpp>
# include <hboost/mpl/bool.hpp>

# include <hboost/iterator/detail/config_def.hpp>

namespace hboost { namespace python { namespace api {

template <class X>
char is_object_operators_helper(object_operators<X> const*);
    
typedef char (&no_type)[2];
no_type is_object_operators_helper(...);

template <class X> X* make_ptr();

template <class L, class R = L>
struct is_object_operators
{
    enum {
        value 
        = (sizeof(api::is_object_operators_helper(api::make_ptr<L>()))
           + sizeof(api::is_object_operators_helper(api::make_ptr<R>()))
           < 4
        )
    };
    typedef mpl::bool_<value> type;
};

# if !defined(HBOOST_NO_SFINAE) && !defined(HBOOST_NO_IS_CONVERTIBLE)
template <class L, class R, class T>
struct enable_binary
  : hboost::iterators::enable_if<is_object_operators<L,R>, T>
{};
#  define HBOOST_PYTHON_BINARY_RETURN(T) typename enable_binary<L,R,T>::type
# else
#  define HBOOST_PYTHON_BINARY_RETURN(T) T
# endif

template <class U>
object object_operators<U>::operator()() const
{
    object_cref2 f = *static_cast<U const*>(this);
    return call<object>(f.ptr());
}


template <class U>
inline
object_operators<U>::operator bool_type() const
{
    object_cref2 x = *static_cast<U const*>(this);
    int is_true = PyObject_IsTrue(x.ptr());
    if (is_true < 0) throw_error_already_set();
    return is_true ? &object::ptr : 0;
}

template <class U>
inline bool
object_operators<U>::operator!() const
{
    object_cref2 x = *static_cast<U const*>(this);
    int is_true = PyObject_IsTrue(x.ptr());
    if (is_true < 0) throw_error_already_set();
    return !is_true;
}

# define HBOOST_PYTHON_COMPARE_OP(op, opid)                              \
template <class L, class R>                                             \
HBOOST_PYTHON_BINARY_RETURN(object) operator op(L const& l, R const& r)    \
{                                                                       \
    return PyObject_RichCompare(                                    \
        object(l).ptr(), object(r).ptr(), opid);                        \
}
# undef HBOOST_PYTHON_COMPARE_OP
    
# define HBOOST_PYTHON_BINARY_OPERATOR(op)                               \
HBOOST_PYTHON_DECL object operator op(object const& l, object const& r); \
template <class L, class R>                                             \
HBOOST_PYTHON_BINARY_RETURN(object) operator op(L const& l, R const& r)  \
{                                                                       \
    return object(l) op object(r);                                      \
}
HBOOST_PYTHON_BINARY_OPERATOR(>)
HBOOST_PYTHON_BINARY_OPERATOR(>=)
HBOOST_PYTHON_BINARY_OPERATOR(<)
HBOOST_PYTHON_BINARY_OPERATOR(<=)
HBOOST_PYTHON_BINARY_OPERATOR(==)
HBOOST_PYTHON_BINARY_OPERATOR(!=)
HBOOST_PYTHON_BINARY_OPERATOR(+)
HBOOST_PYTHON_BINARY_OPERATOR(-)
HBOOST_PYTHON_BINARY_OPERATOR(*)
HBOOST_PYTHON_BINARY_OPERATOR(/)
HBOOST_PYTHON_BINARY_OPERATOR(%)
HBOOST_PYTHON_BINARY_OPERATOR(<<)
HBOOST_PYTHON_BINARY_OPERATOR(>>)
HBOOST_PYTHON_BINARY_OPERATOR(&)
HBOOST_PYTHON_BINARY_OPERATOR(^)
HBOOST_PYTHON_BINARY_OPERATOR(|)
# undef HBOOST_PYTHON_BINARY_OPERATOR

        
# define HBOOST_PYTHON_INPLACE_OPERATOR(op)                              \
HBOOST_PYTHON_DECL object& operator op(object& l, object const& r);      \
template <class R>                                                      \
object& operator op(object& l, R const& r)                              \
{                                                                       \
    return l op object(r);                                              \
}
HBOOST_PYTHON_INPLACE_OPERATOR(+=)
HBOOST_PYTHON_INPLACE_OPERATOR(-=)
HBOOST_PYTHON_INPLACE_OPERATOR(*=)
HBOOST_PYTHON_INPLACE_OPERATOR(/=)
HBOOST_PYTHON_INPLACE_OPERATOR(%=)
HBOOST_PYTHON_INPLACE_OPERATOR(<<=)
HBOOST_PYTHON_INPLACE_OPERATOR(>>=)
HBOOST_PYTHON_INPLACE_OPERATOR(&=)
HBOOST_PYTHON_INPLACE_OPERATOR(^=)
HBOOST_PYTHON_INPLACE_OPERATOR(|=)
# undef HBOOST_PYTHON_INPLACE_OPERATOR

}}} // namespace hboost::python

#include <hboost/iterator/detail/config_undef.hpp>

#endif // HBOOST_OBJECT_OPERATORS_DWA2002617_HPP
