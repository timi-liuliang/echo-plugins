#ifndef HBOOST_BORROWED_PTR_DWA20020601_HPP
# define HBOOST_BORROWED_PTR_DWA20020601_HPP
// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

# include <hboost/config.hpp>
# include <hboost/type.hpp>
# include <hboost/mpl/if.hpp>
# include <hboost/type_traits/object_traits.hpp>
# include <hboost/type_traits/cv_traits.hpp>
# include <hboost/python/tag.hpp>

namespace hboost { namespace python { namespace detail {

template<class T> class borrowed
{ 
    typedef T type;
};

template<typename T>
struct is_borrowed_ptr
{
    HBOOST_STATIC_CONSTANT(bool, value = false); 
};

#  if !defined(__MWERKS__) || __MWERKS__ > 0x3000
template<typename T>
struct is_borrowed_ptr<borrowed<T>*>
{
    HBOOST_STATIC_CONSTANT(bool, value = true);
};

template<typename T>
struct is_borrowed_ptr<borrowed<T> const*>
{
    HBOOST_STATIC_CONSTANT(bool, value = true);
};

template<typename T>
struct is_borrowed_ptr<borrowed<T> volatile*>
{
    HBOOST_STATIC_CONSTANT(bool, value = true);
};

template<typename T>
struct is_borrowed_ptr<borrowed<T> const volatile*>
{
    HBOOST_STATIC_CONSTANT(bool, value = true);
};
#  else
template<typename T>
struct is_borrowed
{
    HBOOST_STATIC_CONSTANT(bool, value = false);
};
template<typename T>
struct is_borrowed<borrowed<T> >
{
    HBOOST_STATIC_CONSTANT(bool, value = true);
};
template<typename T>
struct is_borrowed_ptr<T*>
    : is_borrowed<typename remove_cv<T>::type>
{
};
#  endif 


}

template <class T>
inline T* get_managed_object(detail::borrowed<T> const volatile* p, tag_t)
{
    return (T*)p;
}

}} // namespace hboost::python::detail

#endif // #ifndef HBOOST_BORROWED_PTR_DWA20020601_HPP
