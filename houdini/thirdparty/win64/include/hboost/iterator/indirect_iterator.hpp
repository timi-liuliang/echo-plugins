// (C) Copyright David Abrahams 2002.
// (C) Copyright Jeremy Siek    2002.
// (C) Copyright Thomas Witt    2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_INDIRECT_ITERATOR_23022003THW_HPP
#define HBOOST_INDIRECT_ITERATOR_23022003THW_HPP

#include <hboost/iterator.hpp>
#include <hboost/iterator/iterator_adaptor.hpp>

#include <hboost/pointee.hpp>
#include <hboost/indirect_reference.hpp>
#include <hboost/detail/iterator.hpp>

#include <hboost/detail/indirect_traits.hpp>

#include <hboost/type_traits/is_same.hpp>
#include <hboost/type_traits/add_reference.hpp>

#include <hboost/mpl/bool.hpp>
#include <hboost/mpl/identity.hpp>
#include <hboost/mpl/eval_if.hpp>
#include <hboost/mpl/not.hpp>
#include <hboost/mpl/has_xxx.hpp>

#ifdef HBOOST_MPL_CFG_NO_HAS_XXX
# include <hboost/shared_ptr.hpp>
# include <hboost/scoped_ptr.hpp>
# include <hboost/mpl/bool.hpp>
# include <memory>
#endif

#include <hboost/iterator/detail/config_def.hpp> // must be last #include

namespace hboost {
namespace iterators {

  template <class Iter, class Value, class Category, class Reference, class Difference>
  class indirect_iterator;

  namespace detail
  {
    template <class Iter, class Value, class Category, class Reference, class Difference>
    struct indirect_base
    {
        typedef typename hboost::detail::iterator_traits<Iter>::value_type dereferenceable;

        typedef iterator_adaptor<
            indirect_iterator<Iter, Value, Category, Reference, Difference>
          , Iter
          , typename ia_dflt_help<
                Value, pointee<dereferenceable>
            >::type
          , Category
          , typename ia_dflt_help<
                Reference
              , mpl::eval_if<
                    is_same<Value,use_default>
                  , indirect_reference<dereferenceable>
                  , add_reference<Value>
                >
            >::type
          , Difference
        > type;
    };

    template <>
    struct indirect_base<int, int, int, int, int> {};
  } // namespace detail


  template <
      class Iterator
    , class Value = use_default
    , class Category = use_default
    , class Reference = use_default
    , class Difference = use_default
  >
  class indirect_iterator
    : public detail::indirect_base<
        Iterator, Value, Category, Reference, Difference
      >::type
  {
      typedef typename detail::indirect_base<
          Iterator, Value, Category, Reference, Difference
      >::type super_t;

      friend class iterator_core_access;

   public:
      indirect_iterator() {}

      indirect_iterator(Iterator iter)
        : super_t(iter) {}

      template <
          class Iterator2, class Value2, class Category2
        , class Reference2, class Difference2
      >
      indirect_iterator(
          indirect_iterator<
               Iterator2, Value2, Category2, Reference2, Difference2
          > const& y
        , typename enable_if_convertible<Iterator2, Iterator>::type* = 0
      )
        : super_t(y.base())
      {}

  private:
      typename super_t::reference dereference() const
      {
# if HBOOST_WORKAROUND(__BORLANDC__, < 0x5A0 )
          return const_cast<super_t::reference>(**this->base());
# else
          return **this->base();
# endif
      }
  };

  template <class Iter>
  inline
  indirect_iterator<Iter> make_indirect_iterator(Iter x)
  {
    return indirect_iterator<Iter>(x);
  }

  template <class Traits, class Iter>
  inline
  indirect_iterator<Iter,Traits> make_indirect_iterator(Iter x, Traits* = 0)
  {
    return indirect_iterator<Iter, Traits>(x);
  }

} // namespace iterators

using iterators::indirect_iterator;
using iterators::make_indirect_iterator;

} // namespace hboost

#include <hboost/iterator/detail/config_undef.hpp>

#endif // HBOOST_INDIRECT_ITERATOR_23022003THW_HPP
