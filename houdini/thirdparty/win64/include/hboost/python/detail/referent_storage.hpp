// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_REFERENT_STORAGE_DWA200278_HPP
# define HBOOST_REFERENT_STORAGE_DWA200278_HPP
# include <hboost/mpl/if.hpp>
# include <cstddef>

namespace hboost { namespace python { namespace detail {

struct alignment_dummy;
typedef void (*function_ptr)();
typedef int (alignment_dummy::*member_ptr);
typedef int (alignment_dummy::*member_function_ptr)();

# define HBOOST_PYTHON_ALIGNER(T, n)                     \
        typename mpl::if_c<                             \
           sizeof(T) <= size, T, char>::type t##n

// Storage for size bytes, aligned to all fundamental types no larger than size
template <std::size_t size>
union aligned_storage
{
    HBOOST_PYTHON_ALIGNER(char, 0);
    HBOOST_PYTHON_ALIGNER(short, 1);
    HBOOST_PYTHON_ALIGNER(int, 2);
    HBOOST_PYTHON_ALIGNER(long, 3);
    HBOOST_PYTHON_ALIGNER(float, 4);
    HBOOST_PYTHON_ALIGNER(double, 5);
    HBOOST_PYTHON_ALIGNER(long double, 6);
    HBOOST_PYTHON_ALIGNER(void*, 7);
    HBOOST_PYTHON_ALIGNER(function_ptr, 8);
    HBOOST_PYTHON_ALIGNER(member_ptr, 9);
    HBOOST_PYTHON_ALIGNER(member_function_ptr, 10);
    char bytes[size];
};

# undef HBOOST_PYTHON_ALIGNER

  // Compute the size of T's referent. We wouldn't need this at all,
  // but sizeof() is broken in CodeWarriors <= 8.0
  template <class T> struct referent_size;
  
  
  template <class T>
  struct referent_size<T&>
  {
      HBOOST_STATIC_CONSTANT(
          std::size_t, value = sizeof(T));
  };


// A metafunction returning a POD type which can store U, where T ==
// U&. If T is not a reference type, returns a POD which can store T.
template <class T>
struct referent_storage
{
    typedef aligned_storage<
        ::hboost::python::detail::referent_size<T>::value
    > type;
};

}}} // namespace hboost::python::detail

#endif // HBOOST_REFERENT_STORAGE_DWA200278_HPP