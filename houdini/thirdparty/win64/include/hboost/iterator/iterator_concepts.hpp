// (C) Copyright Jeremy Siek 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_ITERATOR_CONCEPTS_HPP
#define HBOOST_ITERATOR_CONCEPTS_HPP

#include <hboost/concept_check.hpp>
#include <hboost/iterator/iterator_categories.hpp>

// Use hboost::detail::iterator_traits to work around some MSVC/Dinkumware problems.
#include <hboost/detail/iterator.hpp>

#include <hboost/type_traits/is_same.hpp>
#include <hboost/type_traits/is_integral.hpp>

#include <hboost/mpl/bool.hpp>
#include <hboost/mpl/if.hpp>
#include <hboost/mpl/and.hpp>
#include <hboost/mpl/or.hpp>

#include <hboost/static_assert.hpp>

// Use hboost/limits to work around missing limits headers on some compilers
#include <hboost/limits.hpp>
#include <hboost/config.hpp>

#include <algorithm>

#include <hboost/concept/detail/concept_def.hpp>

namespace hboost_concepts
{
  // Used a different namespace here (instead of "boost") so that the
  // concept descriptions do not take for granted the names in
  // namespace hboost.

  //===========================================================================
  // Iterator Access Concepts

  HBOOST_concept(ReadableIterator,(Iterator))
    : hboost::Assignable<Iterator>
    , hboost::CopyConstructible<Iterator>

  {
      typedef HBOOST_DEDUCED_TYPENAME hboost::detail::iterator_traits<Iterator>::value_type value_type;
      typedef HBOOST_DEDUCED_TYPENAME hboost::detail::iterator_traits<Iterator>::reference reference;

      HBOOST_CONCEPT_USAGE(ReadableIterator)
      {

          value_type v = *i;
          hboost::ignore_unused_variable_warning(v);
      }
  private:
      Iterator i;
  };

  template <
      typename Iterator
    , typename ValueType = HBOOST_DEDUCED_TYPENAME hboost::detail::iterator_traits<Iterator>::value_type
  >
  struct WritableIterator
    : hboost::CopyConstructible<Iterator>
  {
      HBOOST_CONCEPT_USAGE(WritableIterator)
      {
          *i = v;
      }
  private:
      ValueType v;
      Iterator i;
  };

  template <
      typename Iterator
    , typename ValueType = HBOOST_DEDUCED_TYPENAME hboost::detail::iterator_traits<Iterator>::value_type
  >
  struct WritableIteratorConcept : WritableIterator<Iterator,ValueType> {};

  HBOOST_concept(SwappableIterator,(Iterator))
  {
      HBOOST_CONCEPT_USAGE(SwappableIterator)
      {
          std::iter_swap(i1, i2);
      }
  private:
      Iterator i1;
      Iterator i2;
  };

  HBOOST_concept(LvalueIterator,(Iterator))
  {
      typedef typename hboost::detail::iterator_traits<Iterator>::value_type value_type;

      HBOOST_CONCEPT_USAGE(LvalueIterator)
      {
        value_type& r = const_cast<value_type&>(*i);
        hboost::ignore_unused_variable_warning(r);
      }
  private:
      Iterator i;
  };


  //===========================================================================
  // Iterator Traversal Concepts

  HBOOST_concept(IncrementableIterator,(Iterator))
    : hboost::Assignable<Iterator>
    , hboost::CopyConstructible<Iterator>
  {
      typedef typename hboost::iterator_traversal<Iterator>::type traversal_category;

      HBOOST_CONCEPT_ASSERT((
        hboost::Convertible<
            traversal_category
          , hboost::incrementable_traversal_tag
        >));

      HBOOST_CONCEPT_USAGE(IncrementableIterator)
      {
          ++i;
          (void)i++;
      }
  private:
      Iterator i;
  };

  HBOOST_concept(SinglePassIterator,(Iterator))
    : IncrementableIterator<Iterator>
    , hboost::EqualityComparable<Iterator>

  {
      HBOOST_CONCEPT_ASSERT((
          hboost::Convertible<
             HBOOST_DEDUCED_TYPENAME SinglePassIterator::traversal_category
           , hboost::single_pass_traversal_tag
          > ));
  };

  HBOOST_concept(ForwardTraversal,(Iterator))
    : SinglePassIterator<Iterator>
    , hboost::DefaultConstructible<Iterator>
  {
      typedef typename hboost::detail::iterator_traits<Iterator>::difference_type difference_type;

      HBOOST_MPL_ASSERT((hboost::is_integral<difference_type>));
      HBOOST_MPL_ASSERT_RELATION(std::numeric_limits<difference_type>::is_signed, ==, true);

      HBOOST_CONCEPT_ASSERT((
          hboost::Convertible<
             HBOOST_DEDUCED_TYPENAME ForwardTraversal::traversal_category
           , hboost::forward_traversal_tag
          > ));
  };

  HBOOST_concept(BidirectionalTraversal,(Iterator))
    : ForwardTraversal<Iterator>
  {
      HBOOST_CONCEPT_ASSERT((
          hboost::Convertible<
             HBOOST_DEDUCED_TYPENAME BidirectionalTraversal::traversal_category
           , hboost::bidirectional_traversal_tag
          > ));

      HBOOST_CONCEPT_USAGE(BidirectionalTraversal)
      {
          --i;
          (void)i--;
      }
   private:
      Iterator i;
  };

  HBOOST_concept(RandomAccessTraversal,(Iterator))
    : BidirectionalTraversal<Iterator>
  {
      HBOOST_CONCEPT_ASSERT((
          hboost::Convertible<
             HBOOST_DEDUCED_TYPENAME RandomAccessTraversal::traversal_category
           , hboost::random_access_traversal_tag
          > ));

      HBOOST_CONCEPT_USAGE(RandomAccessTraversal)
      {
          i += n;
          i = i + n;
          i = n + i;
          i -= n;
          i = i - n;
          n = i - j;
      }

   private:
      typename BidirectionalTraversal<Iterator>::difference_type n;
      Iterator i, j;
  };

  //===========================================================================
  // Iterator Interoperability

  namespace detail
  {
    template <typename Iterator1, typename Iterator2>
    void interop_single_pass_constraints(Iterator1 const& i1, Iterator2 const& i2)
    {
        bool b;
        b = i1 == i2;
        b = i1 != i2;

        b = i2 == i1;
        b = i2 != i1;
        hboost::ignore_unused_variable_warning(b);
    }

    template <typename Iterator1, typename Iterator2>
    void interop_rand_access_constraints(
        Iterator1 const& i1, Iterator2 const& i2,
        hboost::random_access_traversal_tag, hboost::random_access_traversal_tag)
    {
        bool b;
        typename hboost::detail::iterator_traits<Iterator2>::difference_type n;
        b = i1 <  i2;
        b = i1 <= i2;
        b = i1 >  i2;
        b = i1 >= i2;
        n = i1 -  i2;

        b = i2 <  i1;
        b = i2 <= i1;
        b = i2 >  i1;
        b = i2 >= i1;
        n = i2 -  i1;
        hboost::ignore_unused_variable_warning(b);
        hboost::ignore_unused_variable_warning(n);
    }

    template <typename Iterator1, typename Iterator2>
    void interop_rand_access_constraints(
        Iterator1 const&, Iterator2 const&,
        hboost::single_pass_traversal_tag, hboost::single_pass_traversal_tag)
    { }

  } // namespace detail

  HBOOST_concept(InteroperableIterator,(Iterator)(ConstIterator))
  {
   private:
      typedef typename hboost::iterators::pure_iterator_traversal<Iterator>::type traversal_category;
      typedef typename hboost::iterators::pure_iterator_traversal<ConstIterator>::type const_traversal_category;

   public:
      HBOOST_CONCEPT_ASSERT((SinglePassIterator<Iterator>));
      HBOOST_CONCEPT_ASSERT((SinglePassIterator<ConstIterator>));

      HBOOST_CONCEPT_USAGE(InteroperableIterator)
      {
          detail::interop_single_pass_constraints(i, ci);
          detail::interop_rand_access_constraints(i, ci, traversal_category(), const_traversal_category());

          ci = i;
      }

   private:
      Iterator      i;
      ConstIterator ci;
  };

} // namespace hboost_concepts

#include <hboost/concept/detail/concept_undef.hpp>

#endif // HBOOST_ITERATOR_CONCEPTS_HPP
