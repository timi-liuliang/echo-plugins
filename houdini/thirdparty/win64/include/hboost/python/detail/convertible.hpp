// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_CONVERTIBLE_DWA2002614_HPP
# define HBOOST_CONVERTIBLE_DWA2002614_HPP

# if defined(__EDG_VERSION__) && __EDG_VERSION__ <= 241
#  include <hboost/mpl/if.hpp>
#  include <hboost/type_traits/conversion_traits.hpp>
# endif 

// Supplies a runtime is_convertible check which can be used with tag
// dispatching to work around the Metrowerks Pro7 limitation with hboost::is_convertible
namespace hboost { namespace python { namespace detail { 

typedef char* yes_convertible;
typedef int* no_convertible;

template <class Target>
struct convertible
{
# if !defined(__EDG_VERSION__) || __EDG_VERSION__ > 241 || __EDG_VERSION__ == 238
    static inline no_convertible check(...) { return 0; }
    static inline yes_convertible check(Target) { return 0; }
# else
    template <class X>
    static inline typename mpl::if_c<
        is_convertible<X,Target>::value
        , yes_convertible
        , no_convertible
        >::type check(X const&) { return 0; }
# endif 
};

}}} // namespace hboost::python::detail

#endif // HBOOST_CONVERTIBLE_DWA2002614_HPP
