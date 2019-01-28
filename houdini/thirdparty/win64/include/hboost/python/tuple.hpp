// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef TUPLE_20020706_HPP
#define TUPLE_20020706_HPP

# include <hboost/python/detail/prefix.hpp>

#include <hboost/python/object.hpp>
#include <hboost/python/converter/pytype_object_mgr_traits.hpp>
#include <hboost/preprocessor/enum_params.hpp>
#include <hboost/preprocessor/repetition/enum_binary_params.hpp>

namespace hboost { namespace python {

namespace detail
{
  struct HBOOST_PYTHON_DECL tuple_base : object
  {
   protected:
      tuple_base();
      tuple_base(object_cref sequence);
      
      HBOOST_PYTHON_FORWARD_OBJECT_CONSTRUCTORS(tuple_base, object)

   private:
      static detail::new_reference call(object const&);
  };
}

class tuple : public detail::tuple_base
{
    typedef detail::tuple_base base;
 public:
    tuple() {}

    template <class T>
    explicit tuple(T const& sequence)
        : base(object(sequence))
    {
    }

 public: // implementation detail -- for internal use only
    HBOOST_PYTHON_FORWARD_OBJECT_CONSTRUCTORS(tuple, base)
};

//
// Converter Specializations    // $$$ JDG $$$ moved here to prevent
//                              // G++ bug complaining specialization
                                // provided after instantiation
namespace converter
{
  template <>
  struct object_manager_traits<tuple>
      : pytype_object_manager_traits<&PyTuple_Type,tuple>
  {
  };
}

// for completeness
inline tuple make_tuple() { return tuple(); }

# define HBOOST_PP_ITERATION_PARAMS_1 (3, (1, HBOOST_PYTHON_MAX_ARITY, <hboost/python/detail/make_tuple.hpp>))
# include HBOOST_PP_ITERATE()

}}  // namespace hboost::python

#endif

