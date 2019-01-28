#if !defined(HBOOST_PP_IS_ITERATING)

// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_OVERRIDE_DWA2004721_HPP
# define HBOOST_OVERRIDE_DWA2004721_HPP

# include <hboost/python/detail/prefix.hpp>

# include <hboost/python/converter/return_from_python.hpp>

# include <hboost/python/extract.hpp>
# include <hboost/python/handle.hpp>

#  include <hboost/preprocessor/iterate.hpp>
#  include <hboost/preprocessor/repeat.hpp>
#  include <hboost/preprocessor/debug/line.hpp>
#  include <hboost/preprocessor/repetition/enum_params.hpp>
#  include <hboost/preprocessor/repetition/enum_binary_params.hpp>

#  include <hboost/type.hpp>

namespace hboost { namespace python {

class override;

namespace detail
{
  class wrapper_base;
  
  // The result of calling a method.
  class method_result
  {
   private:
      friend class hboost::python::override;
      explicit method_result(PyObject* x)
        : m_obj(x)
      {}

   public:
      template <class T>
      operator T()
      {
          converter::return_from_python<T> converter;
          return converter(m_obj.release());
      }

#  if HBOOST_WORKAROUND(_MSC_FULL_VER, HBOOST_TESTED_AT(140050215))
      template <class T>
      operator T*()
      {
          converter::return_from_python<T*> converter;
          return converter(m_obj.release());
      }
#  endif 
      
#  if HBOOST_WORKAROUND(HBOOST_MSVC, HBOOST_TESTED_AT(1400)) || HBOOST_WORKAROUND(HBOOST_INTEL_WIN, >= 900)
      // No operator T&
#  else
      
      template <class T>
      operator T&() const
      {
          converter::return_from_python<T&> converter;
          return converter(const_cast<handle<>&>(m_obj).release());
      }
#  endif 

      template <class T>
      T as(type<T>* = 0)
      {
          converter::return_from_python<T> converter;
          return converter(m_obj.release());
      }

      template <class T>
      T unchecked(type<T>* = 0)
      {
          return extract<T>(m_obj.get())();
      }
   private:
      mutable handle<> m_obj;
  };
}

class override : public object
{
 private:
    friend class detail::wrapper_base;
    override(handle<> x)
      : object(x)
    {}
    
 public:
    detail::method_result
    operator()() const
    {
        detail::method_result x(
            PyEval_CallFunction(
                this->ptr()
              , const_cast<char*>("()")
            ));
        return x;
    }

# define HBOOST_PYTHON_fast_arg_to_python_get(z, n, _)   \
    , converter::arg_to_python<A##n>(a##n).get()

# define HBOOST_PP_ITERATION_PARAMS_1 (3, (1, HBOOST_PYTHON_MAX_ARITY, <hboost/python/override.hpp>))
# include HBOOST_PP_ITERATE()

# undef HBOOST_PYTHON_fast_arg_to_python_get
};

}} // namespace hboost::python

#endif // HBOOST_OVERRIDE_DWA2004721_HPP

#else
# if !(HBOOST_WORKAROUND(__MWERKS__, > 0x3100)                      \
        && HBOOST_WORKAROUND(__MWERKS__, HBOOST_TESTED_AT(0x3201)))
#  line HBOOST_PP_LINE(__LINE__, override.hpp)
# endif 

# define N HBOOST_PP_ITERATION()

template <
    HBOOST_PP_ENUM_PARAMS_Z(1, N, class A)
    >
detail::method_result
operator()( HBOOST_PP_ENUM_BINARY_PARAMS_Z(1, N, A, const& a) ) const
{
    detail::method_result x(
        PyEval_CallFunction(
            this->ptr()
          , const_cast<char*>("(" HBOOST_PP_REPEAT_1ST(N, HBOOST_PYTHON_FIXED, "O") ")")
            HBOOST_PP_REPEAT_1ST(N, HBOOST_PYTHON_fast_arg_to_python_get, nil)
        ));
    return x;
}

# undef N
#endif 
