// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_TYPE_ID_DWA2002517_HPP
# define HBOOST_TYPE_ID_DWA2002517_HPP

# include <hboost/python/detail/prefix.hpp>

# include <hboost/python/detail/msvc_typeinfo.hpp>
# include <hboost/operators.hpp>
# include <typeinfo>
# include <cstring>
# include <ostream>
# include <hboost/static_assert.hpp>
# include <hboost/detail/workaround.hpp>
# include <hboost/type_traits/same_traits.hpp>

#  ifndef HBOOST_PYTHON_HAVE_GCC_CP_DEMANGLE
#   if defined(__GNUC__)                                                \
    && !defined(__EDG_VERSION__)
#    define HBOOST_PYTHON_HAVE_GCC_CP_DEMANGLE
#   endif
#  endif

namespace hboost { namespace python { 

// for this compiler at least, cross-shared-library type_info
// comparisons don't work, so use typeid(x).name() instead. It's not
// yet clear what the best default strategy is.
# if defined(__GNUC__) \
 || defined(_AIX) \
 || (   defined(__sgi) && defined(__host_mips)) \
 || (defined(__hpux) && defined(__HP_aCC)) \
 || (defined(linux) && defined(__INTEL_COMPILER) && defined(__ICC))
#  define HBOOST_PYTHON_TYPE_ID_NAME
# endif 

#ifdef HBOOST_PYTHON_HAVE_GCC_CP_DEMANGLE
// Runtime detection of broken cxxabi::__cxa_demangle versions,
// to avoid #ifdef clutter.
bool cxxabi_cxa_demangle_is_broken();
#define HBOOST_PYTHON_HAVE_CXXABI_CXA_DEMANGLE_IS_BROKEN
#endif

// type ids which represent the same information as std::type_info
// (i.e. the top-level reference and cv-qualifiers are stripped), but
// which works across shared libraries.
struct type_info : private totally_ordered<type_info>
{
    inline type_info(std::type_info const& = typeid(void));
    
    inline bool operator<(type_info const& rhs) const;
    inline bool operator==(type_info const& rhs) const;

    char const* name() const;
    friend HBOOST_PYTHON_DECL std::ostream& operator<<(
        std::ostream&, type_info const&);
    
 private: // data members
#  ifdef HBOOST_PYTHON_TYPE_ID_NAME
    typedef char const* base_id_t;
#  else
    typedef std::type_info const* base_id_t;
#  endif
    
    base_id_t m_base_type;
};


// This macro is obsolete. Port away and remove.
# define HBOOST_PYTHON_EXPLICIT_TT_DEF(T)

template <class T>
inline type_info type_id()
{
    return type_info(
#  if !defined(_MSC_VER)                                       \
      || !HBOOST_WORKAROUND(HBOOST_INTEL_CXX_VERSION, <= 700)
        typeid(T)
#  else // strip the decoration which Intel mistakenly leaves in
        python::detail::msvc_typeid((hboost::type<T>*)0)
#  endif 
        );
}

#  if (defined(__EDG_VERSION__) && __EDG_VERSION__ < 245) \
   || (defined(__sgi) && defined(_COMPILER_VERSION) && _COMPILER_VERSION <= 744)
// Older EDG-based compilers seems to mistakenly distinguish "int" from
// "signed int", etc., but only in typeid() expressions. However
// though int == signed int, the "signed" decoration is propagated
// down into template instantiations. Explicit specialization stops
// that from taking hold.

#   define HBOOST_PYTHON_SIGNED_INTEGRAL_TYPE_ID(T)              \
template <>                                                     \
inline type_info type_id<T>()                                   \
{                                                               \
    return type_info(typeid(T));                                \
}

HBOOST_PYTHON_SIGNED_INTEGRAL_TYPE_ID(short)
HBOOST_PYTHON_SIGNED_INTEGRAL_TYPE_ID(int)
HBOOST_PYTHON_SIGNED_INTEGRAL_TYPE_ID(long)
// using Python's macro instead of Boost's - we don't seem to get the
// config right all the time.
#   ifdef HAVE_LONG_LONG
HBOOST_PYTHON_SIGNED_INTEGRAL_TYPE_ID(long long)
#   endif
#   undef HBOOST_PYTHON_SIGNED_INTEGRAL_TYPE_ID
#  endif

//
inline type_info::type_info(std::type_info const& id)
    : m_base_type(
#  ifdef HBOOST_PYTHON_TYPE_ID_NAME
        id.name()
#  else
        &id
#  endif
        )
{
}

inline bool type_info::operator<(type_info const& rhs) const
{
#  ifdef HBOOST_PYTHON_TYPE_ID_NAME
    return std::strcmp(m_base_type, rhs.m_base_type) < 0;
#  else
    return m_base_type->before(*rhs.m_base_type);
#  endif 
}

inline bool type_info::operator==(type_info const& rhs) const
{
#  ifdef HBOOST_PYTHON_TYPE_ID_NAME
    return !std::strcmp(m_base_type, rhs.m_base_type);
#  else
    return *m_base_type == *rhs.m_base_type;
#  endif 
}

#  ifdef HBOOST_PYTHON_HAVE_GCC_CP_DEMANGLE
namespace detail
{
  HBOOST_PYTHON_DECL char const* gcc_demangle(char const*);
}
#  endif
    
inline char const* type_info::name() const
{
    char const* raw_name
        = m_base_type
#  ifndef HBOOST_PYTHON_TYPE_ID_NAME
          ->name()
#  endif
        ;
    
#  ifdef HBOOST_PYTHON_HAVE_GCC_CP_DEMANGLE
    return detail::gcc_demangle(raw_name);
#  else
    return raw_name;
#  endif 
}


HBOOST_PYTHON_DECL std::ostream& operator<<(std::ostream&, type_info const&);

template<>
inline type_info type_id<void>()
{
    return type_info (typeid (void *));
}
#   ifndef HBOOST_NO_CV_VOID_SPECIALIZATIONS
template<>
inline type_info type_id<const volatile void>()
{
    return type_info (typeid (void *));
}
#  endif

}} // namespace hboost::python

#endif // HBOOST_TYPE_ID_DWA2002517_HPP