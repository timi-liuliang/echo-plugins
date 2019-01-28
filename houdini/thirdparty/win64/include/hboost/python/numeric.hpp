// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_NUMARRAY_DWA2002922_HPP
# define HBOOST_NUMARRAY_DWA2002922_HPP

# include <hboost/python/detail/prefix.hpp>

# include <hboost/python/tuple.hpp>
# include <hboost/python/str.hpp>
# include <hboost/preprocessor/iteration/local.hpp>
# include <hboost/preprocessor/cat.hpp>
# include <hboost/preprocessor/repetition/enum.hpp>
# include <hboost/preprocessor/repetition/enum_params.hpp>
# include <hboost/preprocessor/repetition/enum_binary_params.hpp>

namespace hboost { namespace python { namespace numeric {

class array;

namespace aux
{
  struct HBOOST_PYTHON_DECL array_base : object
  {
# define HBOOST_PP_LOCAL_MACRO(n)                                \
      array_base(HBOOST_PP_ENUM_PARAMS_Z(1, n, object const& x));
# define HBOOST_PP_LOCAL_LIMITS (1, 7)
# include HBOOST_PP_LOCAL_ITERATE()

      object argmax(long axis=-1);
      object argmin(long axis=-1);
      object argsort(long axis=-1);
      object astype(object const& type = object());
      void byteswap();
      object copy() const;
      object diagonal(long offset = 0, long axis1 = 0, long axis2 = 1) const;
      void info() const;
      bool is_c_array() const;
      bool isbyteswapped() const;
      array new_(object type) const;
      void sort();
      object trace(long offset = 0, long axis1 = 0, long axis2 = 1) const;
      object type() const;
      char typecode() const;

      object factory(
          object const& sequence = object()
        , object const& typecode = object()
        , bool copy = true
        , bool savespace = false
        , object type = object()
        , object shape = object());

      object getflat() const;
      long getrank() const;
      object getshape() const;
      bool isaligned() const;
      bool iscontiguous() const;
      long itemsize() const;
      long nelements() const;
      object nonzero() const;
   
      void put(object const& indices, object const& values);
   
      void ravel();
   
      object repeat(object const& repeats, long axis=0);
   
      void resize(object const& shape);
      
      void setflat(object const& flat);
      void setshape(object const& shape);
   
      void swapaxes(long axis1, long axis2);
   
      object take(object const& sequence, long axis = 0) const;
   
      void tofile(object const& file) const;
   
      str tostring() const;
   
      void transpose(object const& axes = object());
   
      object view() const;

   public: // implementation detail - do not touch.
      HBOOST_PYTHON_FORWARD_OBJECT_CONSTRUCTORS(array_base, object);
  };

  struct HBOOST_PYTHON_DECL array_object_manager_traits
  {
      static bool check(PyObject* obj);
      static detail::new_non_null_reference adopt(PyObject* obj);
      static PyTypeObject const* get_pytype() ;
  };
} // namespace aux

class array : public aux::array_base
{
    typedef aux::array_base base;
 public:

    object astype() { return base::astype(); }
    
    template <class Type>
    object astype(Type const& type_)
    {
        return base::astype(object(type_));
    }

    template <class Type>
    array new_(Type const& type_) const
    {
        return base::new_(object(type_));
    }

    template <class Sequence>
    void resize(Sequence const& x)
    {
        base::resize(object(x));
    }
    
# define HBOOST_PP_LOCAL_MACRO(n)                                \
      void resize(HBOOST_PP_ENUM_PARAMS_Z(1, n, long x))              \
      {                                                         \
          resize(make_tuple(HBOOST_PP_ENUM_PARAMS_Z(1, n, x)));       \
      }
# define HBOOST_PP_LOCAL_LIMITS (1, HBOOST_PYTHON_MAX_ARITY)
# include HBOOST_PP_LOCAL_ITERATE()

    template <class Sequence>
    void setshape(Sequence const& x)
    {
        base::setshape(object(x));
    }
    
# define HBOOST_PP_LOCAL_MACRO(n)                                \
    void setshape(HBOOST_PP_ENUM_PARAMS_Z(1, n, long x))              \
    {                                                           \
        setshape(make_tuple(HBOOST_PP_ENUM_PARAMS_Z(1, n, x)));       \
    }
# define HBOOST_PP_LOCAL_LIMITS (1, HBOOST_PYTHON_MAX_ARITY)
# include HBOOST_PP_LOCAL_ITERATE()

    template <class Indices, class Values>
    void put(Indices const& indices, Values const& values)
    {
        base::put(object(indices), object(values));
    }
    
    template <class Sequence>
    object take(Sequence const& sequence, long axis = 0)
    {
        return base::take(object(sequence), axis);
    }

    template <class File>
    void tofile(File const& f) const
    {
        base::tofile(object(f));
    }

    object factory()
    {
        return base::factory();
    }
    
    template <class Sequence>
    object factory(Sequence const& sequence)
    {
        return base::factory(object(sequence));
    }
    
    template <class Sequence, class Typecode>
    object factory(
        Sequence const& sequence
      , Typecode const& typecode_
      , bool copy = true
      , bool savespace = false
    )
    {
        return base::factory(object(sequence), object(typecode_), copy, savespace);
    }

    template <class Sequence, class Typecode, class Type>
    object factory(
        Sequence const& sequence
      , Typecode const& typecode_
      , bool copy
      , bool savespace
      , Type const& type
    )
    {
        return base::factory(object(sequence), object(typecode_), copy, savespace, object(type));
    }
    
    template <class Sequence, class Typecode, class Type, class Shape>
    object factory(
        Sequence const& sequence
      , Typecode const& typecode_
      , bool copy
      , bool savespace
      , Type const& type
      , Shape const& shape
    )
    {
        return base::factory(object(sequence), object(typecode_), copy, savespace, object(type), object(shape));
    }
    
# define HBOOST_PYTHON_ENUM_AS_OBJECT(z, n, x) object(HBOOST_PP_CAT(x,n))
# define HBOOST_PP_LOCAL_MACRO(n)                                        \
    template <HBOOST_PP_ENUM_PARAMS_Z(1, n, class T)>                    \
    explicit array(HBOOST_PP_ENUM_BINARY_PARAMS_Z(1, n, T, const& x))    \
    : base(HBOOST_PP_ENUM_1(n, HBOOST_PYTHON_ENUM_AS_OBJECT, x))          \
    {}
# define HBOOST_PP_LOCAL_LIMITS (1, 7)
# include HBOOST_PP_LOCAL_ITERATE()
# undef HBOOST_PYTHON_AS_OBJECT

    static HBOOST_PYTHON_DECL void set_module_and_type(char const* package_name = 0, char const* type_attribute_name = 0);
    static HBOOST_PYTHON_DECL std::string get_module_name();

 public: // implementation detail -- for internal use only
    HBOOST_PYTHON_FORWARD_OBJECT_CONSTRUCTORS(array, base);
};

} // namespace hboost::python::numeric

namespace converter
{
  template <>
  struct object_manager_traits< numeric::array >
      : numeric::aux::array_object_manager_traits
  {
      HBOOST_STATIC_CONSTANT(bool, is_specialized = true);
  };
}

}} // namespace hboost::python

#endif // HBOOST_NUMARRAY_DWA2002922_HPP
