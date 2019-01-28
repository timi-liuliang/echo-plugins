#ifndef HBOOST_POINTEE_DWA200415_HPP
# define HBOOST_POINTEE_DWA200415_HPP

//
// Copyright David Abrahams 2004. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// typename pointee<P>::type provides the pointee type of P.
//
// For example, it is T for T* and X for shared_ptr<X>.
//
// http://www.boost.org/libs/iterator/doc/pointee.html
//

# include <hboost/detail/is_incrementable.hpp>
# include <hboost/iterator/iterator_traits.hpp>
# include <hboost/type_traits/add_const.hpp>
# include <hboost/type_traits/remove_cv.hpp>
# include <hboost/mpl/if.hpp>
# include <hboost/mpl/eval_if.hpp>

namespace hboost {

namespace detail
{
  template <class P>
  struct smart_ptr_pointee
  {
      typedef typename P::element_type type;
  };

  template <class Iterator>
  struct iterator_pointee
  {
      typedef typename iterator_traits<Iterator>::value_type value_type;

      struct impl
      {
          template <class T>
          static char test(T const&);

          static char (& test(value_type&) )[2];

          static Iterator& x;
      };

      HBOOST_STATIC_CONSTANT(bool, is_constant = sizeof(impl::test(*impl::x)) == 1);

      typedef typename mpl::if_c<
#  if HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x551))
          ::hboost::detail::iterator_pointee<Iterator>::is_constant
#  else
          is_constant
#  endif
        , typename add_const<value_type>::type
        , value_type
      >::type type;
  };
}

template <class P>
struct pointee
  : mpl::eval_if<
        detail::is_incrementable<P>
      , detail::iterator_pointee<P>
      , detail::smart_ptr_pointee<P>
    >
{
};

} // namespace hboost

#endif // HBOOST_POINTEE_DWA200415_HPP
