// Copyright David Abrahams and Nikolay Mladenov 2003.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_RETURN_ARG_DWA2003719_HPP
# define HBOOST_RETURN_ARG_DWA2003719_HPP
# include <hboost/python/default_call_policies.hpp>
# include <hboost/python/detail/none.hpp>
# include <hboost/python/detail/value_arg.hpp>

#ifndef HBOOST_PYTHON_NO_PY_SIGNATURES
# include <hboost/python/converter/pytype_function.hpp>
#endif

# include <hboost/type_traits/add_reference.hpp>
# include <hboost/type_traits/add_const.hpp>

# include <hboost/mpl/int.hpp>
# include <hboost/mpl/at.hpp>

# include <hboost/static_assert.hpp>
# include <hboost/python/refcount.hpp>

# include <cstddef>

namespace hboost { namespace python { 

namespace detail
{
  template <std::size_t>
  struct return_arg_pos_argument_must_be_positive
# if defined(__GNUC__) || defined(__EDG__)
  {}
# endif
  ;

  struct return_none
  {
      template <class T> struct apply
      {
          struct type
          {
              static bool convertible()
              {
                  return true;
              }
              
              PyObject *operator()( typename value_arg<T>::type ) const
              {
                  return none();
              }
#ifndef HBOOST_PYTHON_NO_PY_SIGNATURES
              PyTypeObject const *get_pytype() const { return converter::expected_pytype_for_arg<T>::get_pytype() ; }
#endif
          };
      };
  };
}
    
template <
    std::size_t arg_pos=1
  , class Base = default_call_policies
> 
struct return_arg : Base
{
 private:
    HBOOST_STATIC_CONSTANT(bool, legal = arg_pos > 0);

 public:
    typedef typename mpl::if_c<
        legal
        , detail::return_none
        , detail::return_arg_pos_argument_must_be_positive<arg_pos>
        // we could default to the base result_converter in case or
        // arg_pos==0 since return arg 0 means return result, but I
        // think it is better to issue an error instead, cause it can
        // lead to confusions
    >::type result_converter;

    template <class ArgumentPackage>
    static PyObject* postcall(ArgumentPackage const& args, PyObject* result)
    {
        // In case of arg_pos == 0 we could simply return Base::postcall,
        // but this is redundant
        HBOOST_STATIC_ASSERT(arg_pos > 0);

        result = Base::postcall(args,result);
        if (!result)
            return 0;
        Py_DECREF(result);
        return incref( detail::get(mpl::int_<arg_pos-1>(),args) );
    }

    template <class Sig> 
    struct extract_return_type : mpl::at_c<Sig, arg_pos>
    {
    };

};

template <
    class Base = default_call_policies
    >
struct return_self 
  : return_arg<1,Base>
{};

}} // namespace hboost::python

#endif // HBOOST_RETURN_ARG_DWA2003719_HPP