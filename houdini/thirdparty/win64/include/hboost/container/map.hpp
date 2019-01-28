//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005-2013. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/container for documentation.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef HBOOST_CONTAINER_MAP_HPP
#define HBOOST_CONTAINER_MAP_HPP

#ifndef HBOOST_CONFIG_HPP
#  include <hboost/config.hpp>
#endif

#if defined(HBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <hboost/container/detail/config_begin.hpp>
#include <hboost/container/detail/workaround.hpp>

// container
#include <hboost/container/container_fwd.hpp>
#include <hboost/container/new_allocator.hpp> //new_allocator
#include <hboost/container/throw_exception.hpp>
// container/detail
#include <hboost/container/detail/mpl.hpp>
#include <hboost/container/detail/tree.hpp>
#include <hboost/container/detail/type_traits.hpp>
#include <hboost/container/detail/value_init.hpp>
#include <hboost/container/detail/pair.hpp>
// move
#include <hboost/move/traits.hpp>
#include <hboost/move/utility_core.hpp>
// move/detail
#if defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)
#include <hboost/move/detail/fwd_macros.hpp>
#endif
#include <hboost/move/detail/move_helpers.hpp>
// intrusive/detail
#include <hboost/intrusive/detail/minimal_pair_header.hpp>      //pair
#include <hboost/intrusive/detail/minimal_less_equal_header.hpp>//less, equal
// other
#include <hboost/static_assert.hpp>
#include <hboost/core/no_exceptions_support.hpp>
// std
#if !defined(HBOOST_NO_CXX11_HDR_INITIALIZER_LIST)
#include <initializer_list>
#endif

namespace hboost {
namespace container {

#ifdef HBOOST_CONTAINER_DOXYGEN_INVOKED

//! A map is a kind of associative container that supports unique keys (contains at
//! most one of each key value) and provides for fast retrieval of values of another
//! type T based on the keys. The map class supports bidirectional iterators.
//!
//! A map satisfies all of the requirements of a container and of a reversible
//! container and of an associative container. The <code>value_type</code> stored
//! by this container is the value_type is std::pair<const Key, T>.
//!
//! \tparam Key is the key_type of the map
//! \tparam T is the <code>mapped_type</code>
//! \tparam Compare is the ordering function for Keys (e.g. <i>std::less<Key></i>).
//! \tparam Allocator is the allocator to allocate the <code>value_type</code>s
//!   (e.g. <i>allocator< std::pair<const Key, T> > </i>).
//! \tparam MapOptions is an packed option type generated using using hboost::container::tree_assoc_options.
template < class Key, class T, class Compare = std::less<Key>
         , class Allocator = new_allocator< std::pair< const Key, T> >, class MapOptions = tree_assoc_defaults >
#else
template <class Key, class T, class Compare, class Allocator, class MapOptions>
#endif
class map
   ///@cond
   : public container_detail::tree
      < Key, std::pair<const Key, T>
      , container_detail::select1st< std::pair<const Key, T> >
      , Compare, Allocator, MapOptions>
   ///@endcond
{
   #ifndef HBOOST_CONTAINER_DOXYGEN_INVOKED
   private:
   HBOOST_COPYABLE_AND_MOVABLE(map)

   typedef std::pair<const Key, T>  value_type_impl;
   typedef container_detail::tree
      <Key, value_type_impl, container_detail::select1st<value_type_impl>, Compare, Allocator, MapOptions> base_t;
   typedef container_detail::pair <Key, T> movable_value_type_impl;
   typedef container_detail::tree_value_compare
      < Key, value_type_impl, Compare, container_detail::select1st<value_type_impl>
      >  value_compare_impl;
   #endif   //#ifndef HBOOST_CONTAINER_DOXYGEN_INVOKED

   public:
   //////////////////////////////////////////////
   //
   //                    types
   //
   //////////////////////////////////////////////

   typedef Key                                                              key_type;
   typedef ::hboost::container::allocator_traits<Allocator>                  allocator_traits_type;
   typedef T                                                                mapped_type;
   typedef std::pair<const Key, T>                                          value_type;
   typedef typename hboost::container::allocator_traits<Allocator>::pointer          pointer;
   typedef typename hboost::container::allocator_traits<Allocator>::const_pointer    const_pointer;
   typedef typename hboost::container::allocator_traits<Allocator>::reference        reference;
   typedef typename hboost::container::allocator_traits<Allocator>::const_reference  const_reference;
   typedef typename hboost::container::allocator_traits<Allocator>::size_type        size_type;
   typedef typename hboost::container::allocator_traits<Allocator>::difference_type  difference_type;
   typedef Allocator                                                                allocator_type;
   typedef typename HBOOST_CONTAINER_IMPDEF(base_t::stored_allocator_type)           stored_allocator_type;
   typedef HBOOST_CONTAINER_IMPDEF(value_compare_impl)                               value_compare;
   typedef Compare                                                                  key_compare;
   typedef typename HBOOST_CONTAINER_IMPDEF(base_t::iterator)                        iterator;
   typedef typename HBOOST_CONTAINER_IMPDEF(base_t::const_iterator)                  const_iterator;
   typedef typename HBOOST_CONTAINER_IMPDEF(base_t::reverse_iterator)                reverse_iterator;
   typedef typename HBOOST_CONTAINER_IMPDEF(base_t::const_reverse_iterator)          const_reverse_iterator;
   typedef std::pair<key_type, mapped_type>                                         nonconst_value_type;
   typedef HBOOST_CONTAINER_IMPDEF(movable_value_type_impl)                          movable_value_type;

   //////////////////////////////////////////////
   //
   //          construct/copy/destroy
   //
   //////////////////////////////////////////////

   //! <b>Effects</b>: Default constructs an empty map.
   //!
   //! <b>Complexity</b>: Constant.
   map()
      : base_t()
   {
      //A type must be std::pair<CONST Key, T>
      HBOOST_STATIC_ASSERT((container_detail::is_same<std::pair<const Key, T>, typename Allocator::value_type>::value));
   }

   //! <b>Effects</b>: Constructs an empty map using the specified comparison object
   //! and allocator.
   //!
   //! <b>Complexity</b>: Constant.
   explicit map(const Compare& comp, const allocator_type& a = allocator_type())
      : base_t(comp, a)
   {
      //A type must be std::pair<CONST Key, T>
      HBOOST_STATIC_ASSERT((container_detail::is_same<std::pair<const Key, T>, typename Allocator::value_type>::value));
   }

   //! <b>Effects</b>: Constructs an empty map using the specified allocator.
   //!
   //! <b>Complexity</b>: Constant.
   explicit map(const allocator_type& a)
      : base_t(a)
   {
      //A type must be std::pair<CONST Key, T>
      HBOOST_STATIC_ASSERT((container_detail::is_same<std::pair<const Key, T>, typename Allocator::value_type>::value));
   }

   //! <b>Effects</b>: Constructs an empty map using the specified comparison object and
   //! allocator, and inserts elements from the range [first ,last ).
   //!
   //! <b>Complexity</b>: Linear in N if the range [first ,last ) is already sorted using
   //! comp and otherwise N logN, where N is last - first.
   template <class InputIterator>
   map(InputIterator first, InputIterator last, const Compare& comp = Compare(),
         const allocator_type& a = allocator_type())
      : base_t(true, first, last, comp, a)
   {
      //A type must be std::pair<CONST Key, T>
      HBOOST_STATIC_ASSERT((container_detail::is_same<std::pair<const Key, T>, typename Allocator::value_type>::value));
   }

   //! <b>Effects</b>: Constructs an empty map using the specified 
   //! allocator, and inserts elements from the range [first ,last ).
   //!
   //! <b>Complexity</b>: Linear in N if the range [first ,last ) is already sorted using
   //! comp and otherwise N logN, where N is last - first.
   template <class InputIterator>
   map(InputIterator first, InputIterator last, const allocator_type& a)
      : base_t(true, first, last, Compare(), a)
   {
      //A type must be std::pair<CONST Key, T>
      HBOOST_STATIC_ASSERT((container_detail::is_same<std::pair<const Key, T>, typename Allocator::value_type>::value));
   }

   //! <b>Effects</b>: Constructs an empty map using the specified comparison object and
   //! allocator, and inserts elements from the ordered unique range [first ,last). This function
   //! is more efficient than the normal range creation for ordered ranges.
   //!
   //! <b>Requires</b>: [first ,last) must be ordered according to the predicate and must be
   //! unique values.
   //!
   //! <b>Complexity</b>: Linear in N.
   //!
   //! <b>Note</b>: Non-standard extension.
   template <class InputIterator>
   map( ordered_unique_range_t, InputIterator first, InputIterator last
      , const Compare& comp = Compare(), const allocator_type& a = allocator_type())
      : base_t(ordered_range, first, last, comp, a)
   {
      //A type must be std::pair<CONST Key, T>
      HBOOST_STATIC_ASSERT((container_detail::is_same<std::pair<const Key, T>, typename Allocator::value_type>::value));
   }

#if !defined(HBOOST_NO_CXX11_HDR_INITIALIZER_LIST)
   //! <b>Effects</b>: Constructs an empty map using the specified comparison object and
   //! allocator, and inserts elements from the range [il.begin(), il.end()).
   //!
   //! <b>Complexity</b>: Linear in N if the range [first ,last ) is already sorted using
   //! comp and otherwise N logN, where N is il.first() - il.end().
   map(std::initializer_list<value_type> il, const Compare& comp = Compare(), const allocator_type& a = allocator_type())
      : base_t(true, il.begin(), il.end(), comp, a)
   {
      //A type must be std::pair<CONST Key, T>
      HBOOST_STATIC_ASSERT((container_detail::is_same<std::pair<const Key, T>, typename Allocator::value_type>::value));
   }

   //! <b>Effects</b>: Constructs an empty map using the specified
   //! allocator, and inserts elements from the range [il.begin(), il.end()).
   //!
   //! <b>Complexity</b>: Linear in N if the range [first ,last ) is already sorted using
   //! comp and otherwise N logN, where N is il.first() - il.end().
   map(std::initializer_list<value_type> il, const allocator_type& a)
      : base_t(true, il.begin(), il.end(), Compare(), a)
   {
      //A type must be std::pair<CONST Key, T>
      HBOOST_STATIC_ASSERT((container_detail::is_same<std::pair<const Key, T>, typename Allocator::value_type>::value));
   }

   //! <b>Effects</b>: Constructs an empty set using the specified comparison object and
   //! allocator, and inserts elements from the ordered unique range [il.begin(), il.end()). This function
   //! is more efficient than the normal range creation for ordered ranges.
   //!
   //! <b>Requires</b>: [il.begin(), il.end()) must be ordered according to the predicate and must be
   //! unique values.
   //!
   //! <b>Complexity</b>: Linear in N.
   //!
   //! <b>Note</b>: Non-standard extension.
   map(ordered_unique_range_t, std::initializer_list<value_type> il, const Compare& comp = Compare(),
       const allocator_type& a = allocator_type())
      : base_t(ordered_range, il.begin(), il.end(), comp, a)
   {
      //A type must be std::pair<CONST Key, T>
      HBOOST_STATIC_ASSERT((container_detail::is_same<std::pair<const Key, T>, typename Allocator::value_type>::value));
   }
#endif

   //! <b>Effects</b>: Copy constructs a map.
   //!
   //! <b>Complexity</b>: Linear in x.size().
   map(const map& x)
      : base_t(static_cast<const base_t&>(x))
   {
      //A type must be std::pair<CONST Key, T>
      HBOOST_STATIC_ASSERT((container_detail::is_same<std::pair<const Key, T>, typename Allocator::value_type>::value));
   }

   //! <b>Effects</b>: Move constructs a map. Constructs *this using x's resources.
   //!
   //! <b>Complexity</b>: Constant.
   //!
   //! <b>Postcondition</b>: x is emptied.
   map(HBOOST_RV_REF(map) x)
      : base_t(HBOOST_MOVE_BASE(base_t, x))
   {
      //A type must be std::pair<CONST Key, T>
      HBOOST_STATIC_ASSERT((container_detail::is_same<std::pair<const Key, T>, typename Allocator::value_type>::value));
   }

   //! <b>Effects</b>: Copy constructs a map using the specified allocator.
   //!
   //! <b>Complexity</b>: Linear in x.size().
   map(const map& x, const allocator_type &a)
      : base_t(static_cast<const base_t&>(x), a)
   {
      //A type must be std::pair<CONST Key, T>
      HBOOST_STATIC_ASSERT((container_detail::is_same<std::pair<const Key, T>, typename Allocator::value_type>::value));
   }

   //! <b>Effects</b>: Move constructs a map using the specified allocator.
   //!                 Constructs *this using x's resources.
   //!
   //! <b>Complexity</b>: Constant if x == x.get_allocator(), linear otherwise.
   //!
   //! <b>Postcondition</b>: x is emptied.
   map(HBOOST_RV_REF(map) x, const allocator_type &a)
      : base_t(HBOOST_MOVE_BASE(base_t, x), a)
   {
      //A type must be std::pair<CONST Key, T>
      HBOOST_STATIC_ASSERT((container_detail::is_same<std::pair<const Key, T>, typename Allocator::value_type>::value));
   }

   //! <b>Effects</b>: Makes *this a copy of x.
   //!
   //! <b>Complexity</b>: Linear in x.size().
   map& operator=(HBOOST_COPY_ASSIGN_REF(map) x)
   {  return static_cast<map&>(this->base_t::operator=(static_cast<const base_t&>(x)));  }

   //! <b>Effects</b>: this->swap(x.get()).
   //!
   //! <b>Throws</b>: If allocator_traits_type::propagate_on_container_move_assignment
   //!   is false and (allocation throws or value_type's move constructor throws)
   //!
   //! <b>Complexity</b>: Constant if allocator_traits_type::
   //!   propagate_on_container_move_assignment is true or
   //!   this->get>allocator() == x.get_allocator(). Linear otherwise.
   map& operator=(HBOOST_RV_REF(map) x)
      HBOOST_NOEXCEPT_IF(  allocator_traits_type::is_always_equal::value
                                 && hboost::container::container_detail::is_nothrow_move_assignable<Compare>::value )

   {  return static_cast<map&>(this->base_t::operator=(HBOOST_MOVE_BASE(base_t, x)));  }

#if !defined(HBOOST_NO_CXX11_HDR_INITIALIZER_LIST)
   //! <b>Effects</b>: Assign content of il to *this.
   //!
   map& operator=(std::initializer_list<value_type> il)
   {
       this->clear();
       insert(il.begin(), il.end());
       return *this;
   }
#endif

   #if defined(HBOOST_CONTAINER_DOXYGEN_INVOKED)

   //! <b>Effects</b>: Returns a copy of the allocator that
   //!   was passed to the object's constructor.
   //!
   //! <b>Complexity</b>: Constant.
   allocator_type get_allocator() const;

   //! <b>Effects</b>: Returns a reference to the internal allocator.
   //!
   //! <b>Throws</b>: Nothing
   //!
   //! <b>Complexity</b>: Constant.
   //!
   //! <b>Note</b>: Non-standard extension.
   stored_allocator_type &get_stored_allocator() HBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: Returns a reference to the internal allocator.
   //!
   //! <b>Throws</b>: Nothing
   //!
   //! <b>Complexity</b>: Constant.
   //!
   //! <b>Note</b>: Non-standard extension.
   const stored_allocator_type &get_stored_allocator() const HBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: Returns an iterator to the first element contained in the container.
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   iterator begin() HBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: Returns a const_iterator to the first element contained in the container.
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   const_iterator begin() const HBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: Returns a const_iterator to the first element contained in the container.
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   const_iterator cbegin() const HBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: Returns an iterator to the end of the container.
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   iterator end() HBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: Returns a const_iterator to the end of the container.
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   const_iterator end() const HBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: Returns a const_iterator to the end of the container.
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   const_iterator cend() const HBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: Returns a reverse_iterator pointing to the beginning
   //! of the reversed container.
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   reverse_iterator rbegin() HBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: Returns a const_reverse_iterator pointing to the beginning
   //! of the reversed container.
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   const_reverse_iterator rbegin() const HBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: Returns a const_reverse_iterator pointing to the beginning
   //! of the reversed container.
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   const_reverse_iterator crbegin() const HBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: Returns a reverse_iterator pointing to the end
   //! of the reversed container.
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   reverse_iterator rend() HBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: Returns a const_reverse_iterator pointing to the end
   //! of the reversed container.
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   const_reverse_iterator rend() const HBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: Returns a const_reverse_iterator pointing to the end
   //! of the reversed container.
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   const_reverse_iterator crend() const HBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: Returns true if the container contains no elements.
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   bool empty() const HBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: Returns the number of the elements contained in the container.
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   size_type size() const HBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: Returns the largest possible size of the container.
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   size_type max_size() const HBOOST_NOEXCEPT_OR_NOTHROW;

   #endif   //#if defined(HBOOST_CONTAINER_DOXYGEN_INVOKED)

   #if defined(HBOOST_CONTAINER_DOXYGEN_INVOKED)
   //! Effects: If there is no key equivalent to x in the map, inserts
   //! value_type(x, T()) into the map.
   //!
   //! Returns: A reference to the mapped_type corresponding to x in *this.
   //!
   //! Complexity: Logarithmic.
   mapped_type& operator[](const key_type &k);

   //! Effects: If there is no key equivalent to x in the map, inserts
   //! value_type(hboost::move(x), T()) into the map (the key is move-constructed)
   //!
   //! Returns: A reference to the mapped_type corresponding to x in *this.
   //!
   //! Complexity: Logarithmic.
   mapped_type& operator[](key_type &&k);
   #elif defined(HBOOST_MOVE_HELPERS_RETURN_SFINAE_BROKEN)
      //in compilers like GCC 3.4, we can't catch temporaries
      mapped_type& operator[](const key_type &k)         {  return this->priv_subscript(k);  }
      mapped_type& operator[](HBOOST_RV_REF(key_type) k)  {  return this->priv_subscript(::hboost::move(k));  }
   #else
      HBOOST_MOVE_CONVERSION_AWARE_CATCH( operator[] , key_type, mapped_type&, this->priv_subscript)
   #endif

   //! Returns: A reference to the element whose key is equivalent to x.
   //! Throws: An exception object of type out_of_range if no such element is present.
   //! Complexity: logarithmic.
   T& at(const key_type& k)
   {
      iterator i = this->find(k);
      if(i == this->end()){
         throw_out_of_range("map::at key not found");
      }
      return i->second;
   }

   //! Returns: A reference to the element whose key is equivalent to x.
   //! Throws: An exception object of type out_of_range if no such element is present.
   //! Complexity: logarithmic.
   const T& at(const key_type& k) const
   {
      const_iterator i = this->find(k);
      if(i == this->end()){
         throw_out_of_range("map::at key not found");
      }
      return i->second;
   }

   //////////////////////////////////////////////
   //
   //                modifiers
   //
   //////////////////////////////////////////////

   //! <b>Effects</b>: Inserts x if and only if there is no element in the container
   //!   with key equivalent to the key of x.
   //!
   //! <b>Returns</b>: The bool component of the returned pair is true if and only
   //!   if the insertion takes place, and the iterator component of the pair
   //!   points to the element with key equivalent to the key of x.
   //!
   //! <b>Complexity</b>: Logarithmic.
   std::pair<iterator,bool> insert(const value_type& x)
   { return this->base_t::insert_unique(x); }

   //! <b>Effects</b>: Inserts a new value_type created from the pair if and only if
   //! there is no element in the container  with key equivalent to the key of x.
   //!
   //! <b>Returns</b>: The bool component of the returned pair is true if and only
   //!   if the insertion takes place, and the iterator component of the pair
   //!   points to the element with key equivalent to the key of x.
   //!
   //! <b>Complexity</b>: Logarithmic.
   std::pair<iterator,bool> insert(const nonconst_value_type& x)
   { return this->base_t::insert_unique(x); }

   //! <b>Effects</b>: Inserts a new value_type move constructed from the pair if and
   //! only if there is no element in the container with key equivalent to the key of x.
   //!
   //! <b>Returns</b>: The bool component of the returned pair is true if and only
   //!   if the insertion takes place, and the iterator component of the pair
   //!   points to the element with key equivalent to the key of x.
   //!
   //! <b>Complexity</b>: Logarithmic.
   std::pair<iterator,bool> insert(HBOOST_RV_REF(nonconst_value_type) x)
   { return this->base_t::insert_unique(hboost::move(x)); }

   //! <b>Effects</b>: Inserts a new value_type move constructed from the pair if and
   //! only if there is no element in the container with key equivalent to the key of x.
   //!
   //! <b>Returns</b>: The bool component of the returned pair is true if and only
   //!   if the insertion takes place, and the iterator component of the pair
   //!   points to the element with key equivalent to the key of x.
   //!
   //! <b>Complexity</b>: Logarithmic.
   std::pair<iterator,bool> insert(HBOOST_RV_REF(movable_value_type) x)
   { return this->base_t::insert_unique(hboost::move(x)); }

   //! <b>Effects</b>: Move constructs a new value from x if and only if there is
   //!   no element in the container with key equivalent to the key of x.
   //!
   //! <b>Returns</b>: The bool component of the returned pair is true if and only
   //!   if the insertion takes place, and the iterator component of the pair
   //!   points to the element with key equivalent to the key of x.
   //!
   //! <b>Complexity</b>: Logarithmic.
   std::pair<iterator,bool> insert(HBOOST_RV_REF(value_type) x)
   { return this->base_t::insert_unique(hboost::move(x)); }

   //! <b>Effects</b>: Inserts a copy of x in the container if and only if there is
   //!   no element in the container with key equivalent to the key of x.
   //!   p is a hint pointing to where the insert should start to search.
   //!
   //! <b>Returns</b>: An iterator pointing to the element with key equivalent
   //!   to the key of x.
   //!
   //! <b>Complexity</b>: Logarithmic in general, but amortized constant if t
   //!   is inserted right before p.
   iterator insert(const_iterator p, const value_type& x)
   { return this->base_t::insert_unique(p, x); }

   //! <b>Effects</b>: Move constructs a new value from x if and only if there is
   //!   no element in the container with key equivalent to the key of x.
   //!   p is a hint pointing to where the insert should start to search.
   //!
   //! <b>Returns</b>: An iterator pointing to the element with key equivalent
   //!   to the key of x.
   //!
   //! <b>Complexity</b>: Logarithmic in general, but amortized constant if t
   //!   is inserted right before p.
   iterator insert(const_iterator p, HBOOST_RV_REF(nonconst_value_type) x)
   { return this->base_t::insert_unique_convertible(p, hboost::move(x)); }

   //! <b>Effects</b>: Move constructs a new value from x if and only if there is
   //!   no element in the container with key equivalent to the key of x.
   //!   p is a hint pointing to where the insert should start to search.
   //!
   //! <b>Returns</b>: An iterator pointing to the element with key equivalent
   //!   to the key of x.
   //!
   //! <b>Complexity</b>: Logarithmic in general, but amortized constant if t
   //!   is inserted right before p.
   iterator insert(const_iterator p, HBOOST_RV_REF(movable_value_type) x)
   { return this->base_t::insert_unique_convertible(p, hboost::move(x)); }

   //! <b>Effects</b>: Inserts a copy of x in the container.
   //!   p is a hint pointing to where the insert should start to search.
   //!
   //! <b>Returns</b>: An iterator pointing to the element with key equivalent to the key of x.
   //!
   //! <b>Complexity</b>: Logarithmic.
   iterator insert(const_iterator p, const nonconst_value_type& x)
   { return this->base_t::insert_unique_convertible(p, x); }

   //! <b>Effects</b>: Inserts an element move constructed from x in the container.
   //!   p is a hint pointing to where the insert should start to search.
   //!
   //! <b>Returns</b>: An iterator pointing to the element with key equivalent to the key of x.
   //!
   //! <b>Complexity</b>: Logarithmic.
   iterator insert(const_iterator p, HBOOST_RV_REF(value_type) x)
   { return this->base_t::insert_unique(p, hboost::move(x)); }

   //! <b>Requires</b>: first, last are not iterators into *this.
   //!
   //! <b>Effects</b>: inserts each element from the range [first,last) if and only
   //!   if there is no element with key equivalent to the key of that element.
   //!
   //! <b>Complexity</b>: At most N log(size()+N) (N is the distance from first to last)
   template <class InputIterator>
   void insert(InputIterator first, InputIterator last)
   {  this->base_t::insert_unique(first, last);  }

#if !defined(HBOOST_NO_CXX11_HDR_INITIALIZER_LIST)
   //! <b>Effects</b>: inserts each element from the range [il.begin(), il.end()) if and only
   //!   if there is no element with key equivalent to the key of that element.
   //!
   //! <b>Complexity</b>: At most N log(size()+N) (N is the distance from il.begin() to il.end())
   void insert(std::initializer_list<value_type> il)
   {  this->base_t::insert_unique(il.begin(), il.end()); }
#endif

   #if !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES) || defined(HBOOST_CONTAINER_DOXYGEN_INVOKED)

   //! <b>Effects</b>: Inserts an object x of type T constructed with
   //!   std::forward<Args>(args)... in the container if and only if there is
   //!   no element in the container with an equivalent key.
   //!   p is a hint pointing to where the insert should start to search.
   //!
   //! <b>Returns</b>: The bool component of the returned pair is true if and only
   //!   if the insertion takes place, and the iterator component of the pair
   //!   points to the element with key equivalent to the key of x.
   //!
   //! <b>Complexity</b>: Logarithmic in general, but amortized constant if t
   //!   is inserted right before p.
   template <class... Args>
   std::pair<iterator,bool> emplace(HBOOST_FWD_REF(Args)... args)
   {  return this->base_t::emplace_unique(hboost::forward<Args>(args)...); }

   //! <b>Effects</b>: Inserts an object of type T constructed with
   //!   std::forward<Args>(args)... in the container if and only if there is
   //!   no element in the container with an equivalent key.
   //!   p is a hint pointing to where the insert should start to search.
   //!
   //! <b>Returns</b>: An iterator pointing to the element with key equivalent
   //!   to the key of x.
   //!
   //! <b>Complexity</b>: Logarithmic in general, but amortized constant if t
   //!   is inserted right before p.
   template <class... Args>
   iterator emplace_hint(const_iterator p, HBOOST_FWD_REF(Args)... args)
   {  return this->base_t::emplace_hint_unique(p, hboost::forward<Args>(args)...); }

   #else // !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)

   #define HBOOST_CONTAINER_MAP_EMPLACE_CODE(N) \
   HBOOST_MOVE_TMPL_LT##N HBOOST_MOVE_CLASS##N HBOOST_MOVE_GT##N \
   std::pair<iterator,bool> emplace(HBOOST_MOVE_UREF##N)\
   {  return this->base_t::emplace_unique(HBOOST_MOVE_FWD##N);   }\
   \
   HBOOST_MOVE_TMPL_LT##N HBOOST_MOVE_CLASS##N HBOOST_MOVE_GT##N \
   iterator emplace_hint(const_iterator hint HBOOST_MOVE_I##N HBOOST_MOVE_UREF##N)\
   {  return this->base_t::emplace_hint_unique(hint HBOOST_MOVE_I##N HBOOST_MOVE_FWD##N);  }\
   //
   HBOOST_MOVE_ITERATE_0TO9(HBOOST_CONTAINER_MAP_EMPLACE_CODE)
   #undef HBOOST_CONTAINER_MAP_EMPLACE_CODE

   #endif   // !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)

   #if defined(HBOOST_CONTAINER_DOXYGEN_INVOKED)

   //! <b>Effects</b>: Erases the element pointed to by p.
   //!
   //! <b>Returns</b>: Returns an iterator pointing to the element immediately
   //!   following q prior to the element being erased. If no such element exists,
   //!   returns end().
   //!
   //! <b>Complexity</b>: Amortized constant time
   iterator erase(const_iterator p) HBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: Erases all elements in the container with key equivalent to x.
   //!
   //! <b>Returns</b>: Returns the number of erased elements.
   //!
   //! <b>Complexity</b>: log(size()) + count(k)
   size_type erase(const key_type& x) HBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: Erases all the elements in the range [first, last).
   //!
   //! <b>Returns</b>: Returns last.
   //!
   //! <b>Complexity</b>: log(size())+N where N is the distance from first to last.
   iterator erase(const_iterator first, const_iterator last) HBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: Swaps the contents of *this and x.
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   void swap(map& x)
      HBOOST_NOEXCEPT_IF(  allocator_traits_type::is_always_equal::value
                                 && hboost::container::container_detail::is_nothrow_swappable<Compare>::value )

   //! <b>Effects</b>: erase(a.begin(),a.end()).
   //!
   //! <b>Postcondition</b>: size() == 0.
   //!
   //! <b>Complexity</b>: linear in size().
   void clear() HBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: Returns the comparison object out
   //!   of which a was constructed.
   //!
   //! <b>Complexity</b>: Constant.
   key_compare key_comp() const;

   //! <b>Effects</b>: Returns an object of value_compare constructed out
   //!   of the comparison object.
   //!
   //! <b>Complexity</b>: Constant.
   value_compare value_comp() const;

   //! <b>Returns</b>: An iterator pointing to an element with the key
   //!   equivalent to x, or end() if such an element is not found.
   //!
   //! <b>Complexity</b>: Logarithmic.
   iterator find(const key_type& x);

   //! <b>Returns</b>: A const_iterator pointing to an element with the key
   //!   equivalent to x, or end() if such an element is not found.
   //!
   //! <b>Complexity</b>: Logarithmic.
   const_iterator find(const key_type& x) const;

   #endif   //#if defined(HBOOST_CONTAINER_DOXYGEN_INVOKED)

   //! <b>Returns</b>: The number of elements with key equivalent to x.
   //!
   //! <b>Complexity</b>: log(size())+count(k)
   size_type count(const key_type& x) const
   {  return static_cast<size_type>(this->find(x) != this->cend());  }

   #if defined(HBOOST_CONTAINER_DOXYGEN_INVOKED)

   //! <b>Returns</b>: An iterator pointing to the first element with key not less
   //!   than k, or a.end() if such an element is not found.
   //!
   //! <b>Complexity</b>: Logarithmic
   iterator lower_bound(const key_type& x);

   //! <b>Returns</b>: A const iterator pointing to the first element with key not
   //!   less than k, or a.end() if such an element is not found.
   //!
   //! <b>Complexity</b>: Logarithmic
   const_iterator lower_bound(const key_type& x) const;

   //! <b>Returns</b>: An iterator pointing to the first element with key not less
   //!   than x, or end() if such an element is not found.
   //!
   //! <b>Complexity</b>: Logarithmic
   iterator upper_bound(const key_type& x);

   //! <b>Returns</b>: A const iterator pointing to the first element with key not
   //!   less than x, or end() if such an element is not found.
   //!
   //! <b>Complexity</b>: Logarithmic
   const_iterator upper_bound(const key_type& x) const;

   //! <b>Effects</b>: Equivalent to std::make_pair(this->lower_bound(k), this->upper_bound(k)).
   //!
   //! <b>Complexity</b>: Logarithmic
   std::pair<iterator,iterator> equal_range(const key_type& x);

   //! <b>Effects</b>: Equivalent to std::make_pair(this->lower_bound(k), this->upper_bound(k)).
   //!
   //! <b>Complexity</b>: Logarithmic
   std::pair<const_iterator,const_iterator> equal_range(const key_type& x) const;

   //! <b>Effects</b>: Rebalances the tree. It's a no-op for Red-Black and AVL trees.
   //!
   //! <b>Complexity</b>: Linear
   void rebalance();

   //! <b>Effects</b>: Returns true if x and y are equal
   //!
   //! <b>Complexity</b>: Linear to the number of elements in the container.
   friend bool operator==(const map& x, const map& y);

   //! <b>Effects</b>: Returns true if x and y are unequal
   //!
   //! <b>Complexity</b>: Linear to the number of elements in the container.
   friend bool operator!=(const map& x, const map& y);

   //! <b>Effects</b>: Returns true if x is less than y
   //!
   //! <b>Complexity</b>: Linear to the number of elements in the container.
   friend bool operator<(const map& x, const map& y);

   //! <b>Effects</b>: Returns true if x is greater than y
   //!
   //! <b>Complexity</b>: Linear to the number of elements in the container.
   friend bool operator>(const map& x, const map& y);

   //! <b>Effects</b>: Returns true if x is equal or less than y
   //!
   //! <b>Complexity</b>: Linear to the number of elements in the container.
   friend bool operator<=(const map& x, const map& y);

   //! <b>Effects</b>: Returns true if x is equal or greater than y
   //!
   //! <b>Complexity</b>: Linear to the number of elements in the container.
   friend bool operator>=(const map& x, const map& y);

   //! <b>Effects</b>: x.swap(y)
   //!
   //! <b>Complexity</b>: Constant.
   friend void swap(map& x, map& y);

   #endif   //#if defined(HBOOST_CONTAINER_DOXYGEN_INVOKED)

   #ifndef HBOOST_CONTAINER_DOXYGEN_INVOKED
   private:
   template<class KeyConvertible>
   mapped_type& priv_subscript(HBOOST_FWD_REF(KeyConvertible) k)
   {
      return this->insert_from_key(hboost::forward<KeyConvertible>(k))->second;
   }

   #endif   //#ifndef HBOOST_CONTAINER_DOXYGEN_INVOKED
};


#ifndef HBOOST_CONTAINER_DOXYGEN_INVOKED

}  //namespace container {

//!has_trivial_destructor_after_move<> == true_type
//!specialization for optimizations
template <class Key, class T, class Compare, class Allocator>
struct has_trivial_destructor_after_move<hboost::container::map<Key, T, Compare, Allocator> >
{
   typedef typename ::hboost::container::allocator_traits<Allocator>::pointer pointer;
   static const bool value = ::hboost::has_trivial_destructor_after_move<Allocator>::value &&
                             ::hboost::has_trivial_destructor_after_move<pointer>::value &&
                             ::hboost::has_trivial_destructor_after_move<Compare>::value;
};

namespace container {

#endif   //#ifndef HBOOST_CONTAINER_DOXYGEN_INVOKED

#ifdef HBOOST_CONTAINER_DOXYGEN_INVOKED

//! A multimap is a kind of associative container that supports equivalent keys
//! (possibly containing multiple copies of the same key value) and provides for
//! fast retrieval of values of another type T based on the keys. The multimap class
//! supports bidirectional iterators.
//!
//! A multimap satisfies all of the requirements of a container and of a reversible
//! container and of an associative container. The <code>value_type</code> stored
//! by this container is the value_type is std::pair<const Key, T>.
//!
//! \tparam Key is the key_type of the map
//! \tparam Value is the <code>mapped_type</code>
//! \tparam Compare is the ordering function for Keys (e.g. <i>std::less<Key></i>).
//! \tparam Allocator is the allocator to allocate the <code>value_type</code>s
//!   (e.g. <i>allocator< std::pair<const Key, T> > </i>).
//! \tparam MultiMapOptions is an packed option type generated using using hboost::container::tree_assoc_options.
template < class Key, class T, class Compare = std::less<Key>
         , class Allocator = new_allocator< std::pair< const Key, T> >, class MultiMapOptions = tree_assoc_defaults>
#else
template <class Key, class T, class Compare, class Allocator, class MultiMapOptions>
#endif
class multimap
   ///@cond
   : public container_detail::tree
      < Key, std::pair<const Key, T>
      , container_detail::select1st< std::pair<const Key, T> >
      , Compare, Allocator, MultiMapOptions>
   ///@endcond
{
   #ifndef HBOOST_CONTAINER_DOXYGEN_INVOKED
   private:
   HBOOST_COPYABLE_AND_MOVABLE(multimap)

   typedef std::pair<const Key, T>  value_type_impl;
   typedef container_detail::tree
      <Key, value_type_impl, container_detail::select1st<value_type_impl>, Compare, Allocator, MultiMapOptions> base_t;
   typedef container_detail::pair <Key, T> movable_value_type_impl;
   typedef container_detail::tree_value_compare
      < Key, value_type_impl, Compare, container_detail::select1st<value_type_impl>
      >  value_compare_impl;
   #endif   //#ifndef HBOOST_CONTAINER_DOXYGEN_INVOKED

   typedef ::hboost::container::allocator_traits<Allocator>                          allocator_traits_type;

   public:
   //////////////////////////////////////////////
   //
   //                    types
   //
   //////////////////////////////////////////////

   typedef Key                                                                      key_type;
   typedef T                                                                        mapped_type;
   typedef std::pair<const Key, T>                                                  value_type;
   typedef typename hboost::container::allocator_traits<Allocator>::pointer          pointer;
   typedef typename hboost::container::allocator_traits<Allocator>::const_pointer    const_pointer;
   typedef typename hboost::container::allocator_traits<Allocator>::reference        reference;
   typedef typename hboost::container::allocator_traits<Allocator>::const_reference  const_reference;
   typedef typename hboost::container::allocator_traits<Allocator>::size_type        size_type;
   typedef typename hboost::container::allocator_traits<Allocator>::difference_type  difference_type;
   typedef Allocator                                                                allocator_type;
   typedef typename HBOOST_CONTAINER_IMPDEF(base_t::stored_allocator_type)           stored_allocator_type;
   typedef HBOOST_CONTAINER_IMPDEF(value_compare_impl)                               value_compare;
   typedef Compare                                                                  key_compare;
   typedef typename HBOOST_CONTAINER_IMPDEF(base_t::iterator)                        iterator;
   typedef typename HBOOST_CONTAINER_IMPDEF(base_t::const_iterator)                  const_iterator;
   typedef typename HBOOST_CONTAINER_IMPDEF(base_t::reverse_iterator)                reverse_iterator;
   typedef typename HBOOST_CONTAINER_IMPDEF(base_t::const_reverse_iterator)          const_reverse_iterator;
   typedef std::pair<key_type, mapped_type>                                         nonconst_value_type;
   typedef HBOOST_CONTAINER_IMPDEF(movable_value_type_impl)                          movable_value_type;

   //////////////////////////////////////////////
   //
   //          construct/copy/destroy
   //
   //////////////////////////////////////////////

   //! <b>Effects</b>: Default constructs an empty multimap.
   //!
   //! <b>Complexity</b>: Constant.
   multimap()
      : base_t()
   {
      //A type must be std::pair<CONST Key, T>
      HBOOST_STATIC_ASSERT((container_detail::is_same<std::pair<const Key, T>, typename Allocator::value_type>::value));
   }

   //! <b>Effects</b>: Constructs an empty multimap using the specified allocator.
   //!
   //! <b>Complexity</b>: Constant.
   explicit multimap(const Compare& comp, const allocator_type& a = allocator_type())
      : base_t(comp, a)
   {
      //A type must be std::pair<CONST Key, T>
      HBOOST_STATIC_ASSERT((container_detail::is_same<std::pair<const Key, T>, typename Allocator::value_type>::value));
   }

   //! <b>Effects</b>: Constructs an empty multimap using the specified comparison
   //!   object and allocator.
   //!
   //! <b>Complexity</b>: Constant.
   explicit multimap(const allocator_type& a)
      : base_t(a)
   {
      //A type must be std::pair<CONST Key, T>
      HBOOST_STATIC_ASSERT((container_detail::is_same<std::pair<const Key, T>, typename Allocator::value_type>::value));
   }

   //! <b>Effects</b>: Constructs an empty multimap using the specified comparison object
   //!   and allocator, and inserts elements from the range [first ,last ).
   //!
   //! <b>Complexity</b>: Linear in N if the range [first ,last ) is already sorted using
   //! comp and otherwise N logN, where N is last - first.
   template <class InputIterator>
   multimap(InputIterator first, InputIterator last,
            const Compare& comp = Compare(),
            const allocator_type& a = allocator_type())
      : base_t(false, first, last, comp, a)
   {
      //A type must be std::pair<CONST Key, T>
      HBOOST_STATIC_ASSERT((container_detail::is_same<std::pair<const Key, T>, typename Allocator::value_type>::value));
   }

   //! <b>Effects</b>: Constructs an empty multimap using the specified 
   //! allocator, and inserts elements from the range [first ,last ).
   //!
   //! <b>Complexity</b>: Linear in N if the range [first ,last ) is already sorted using
   //! comp and otherwise N logN, where N is last - first.
   template <class InputIterator>
   multimap(InputIterator first, InputIterator last, const allocator_type& a)
      : base_t(false, first, last, Compare(), a)
   {
      //A type must be std::pair<CONST Key, T>
      HBOOST_STATIC_ASSERT((container_detail::is_same<std::pair<const Key, T>, typename Allocator::value_type>::value));
   }

   //! <b>Effects</b>: Constructs an empty multimap using the specified comparison object and
   //! allocator, and inserts elements from the ordered range [first ,last). This function
   //! is more efficient than the normal range creation for ordered ranges.
   //!
   //! <b>Requires</b>: [first ,last) must be ordered according to the predicate.
   //!
   //! <b>Complexity</b>: Linear in N.
   //!
   //! <b>Note</b>: Non-standard extension.
   template <class InputIterator>
   multimap(ordered_range_t, InputIterator first, InputIterator last, const Compare& comp = Compare(),
         const allocator_type& a = allocator_type())
      : base_t(ordered_range, first, last, comp, a)
   {}

#if !defined(HBOOST_NO_CXX11_HDR_INITIALIZER_LIST)
   //! <b>Effects</b>: Constructs an empty multimap using the specified comparison object and
   //! allocator, and inserts elements from the range [il.begin(), il.end()).
   //!
   //! <b>Complexity</b>: Linear in N if the range [first ,last ) is already sorted using
   //! comp and otherwise N logN, where N is il.first() - il.end().
   multimap(std::initializer_list<value_type> il, const Compare& comp = Compare(),
           const allocator_type& a = allocator_type())
      : base_t(false, il.begin(), il.end(), comp, a)
   {
      //A type must be std::pair<CONST Key, T>
      HBOOST_STATIC_ASSERT((container_detail::is_same<std::pair<const Key, T>, typename Allocator::value_type>::value));
   }

   //! <b>Effects</b>: Constructs an empty multimap using the specified
   //! allocator, and inserts elements from the range [il.begin(), il.end()).
   //!
   //! <b>Complexity</b>: Linear in N if the range [first ,last ) is already sorted using
   //! comp and otherwise N logN, where N is il.first() - il.end().
   multimap(std::initializer_list<value_type> il, const allocator_type& a)
      : base_t(false, il.begin(), il.end(), Compare(), a)
   {
      //A type must be std::pair<CONST Key, T>
      HBOOST_STATIC_ASSERT((container_detail::is_same<std::pair<const Key, T>, typename Allocator::value_type>::value));
   }

   //! <b>Effects</b>: Constructs an empty set using the specified comparison object and
   //! allocator, and inserts elements from the ordered range [il.begin(), il.end()). This function
   //! is more efficient than the normal range creation for ordered ranges.
   //!
   //! <b>Requires</b>: [il.begin(), il.end()) must be ordered according to the predicate.
   //!
   //! <b>Complexity</b>: Linear in N.
   //!
   //! <b>Note</b>: Non-standard extension.
   multimap(ordered_range_t, std::initializer_list<value_type> il, const Compare& comp = Compare(),
       const allocator_type& a = allocator_type())
      : base_t(ordered_range, il.begin(), il.end(), comp, a)
   {
      //A type must be std::pair<CONST Key, T>
      HBOOST_STATIC_ASSERT((container_detail::is_same<std::pair<const Key, T>, typename Allocator::value_type>::value));
   }
#endif

   //! <b>Effects</b>: Copy constructs a multimap.
   //!
   //! <b>Complexity</b>: Linear in x.size().
   multimap(const multimap& x)
      : base_t(static_cast<const base_t&>(x))
   {
      //A type must be std::pair<CONST Key, T>
      HBOOST_STATIC_ASSERT((container_detail::is_same<std::pair<const Key, T>, typename Allocator::value_type>::value));
   }

   //! <b>Effects</b>: Move constructs a multimap. Constructs *this using x's resources.
   //!
   //! <b>Complexity</b>: Constant.
   //!
   //! <b>Postcondition</b>: x is emptied.
   multimap(HBOOST_RV_REF(multimap) x)
      : base_t(HBOOST_MOVE_BASE(base_t, x))
   {
      //A type must be std::pair<CONST Key, T>
      HBOOST_STATIC_ASSERT((container_detail::is_same<std::pair<const Key, T>, typename Allocator::value_type>::value));
   }

   //! <b>Effects</b>: Copy constructs a multimap.
   //!
   //! <b>Complexity</b>: Linear in x.size().
   multimap(const multimap& x, const allocator_type &a)
      : base_t(static_cast<const base_t&>(x), a)
   {
      //A type must be std::pair<CONST Key, T>
      HBOOST_STATIC_ASSERT((container_detail::is_same<std::pair<const Key, T>, typename Allocator::value_type>::value));
   }

   //! <b>Effects</b>: Move constructs a multimap using the specified allocator.
   //!                 Constructs *this using x's resources.
   //! <b>Complexity</b>: Constant if a == x.get_allocator(), linear otherwise.
   //!
   //! <b>Postcondition</b>: x is emptied.
   multimap(HBOOST_RV_REF(multimap) x, const allocator_type &a)
      : base_t(HBOOST_MOVE_BASE(base_t, x), a)
   {
      //A type must be std::pair<CONST Key, T>
      HBOOST_STATIC_ASSERT((container_detail::is_same<std::pair<const Key, T>, typename Allocator::value_type>::value));
   }

   //! <b>Effects</b>: Makes *this a copy of x.
   //!
   //! <b>Complexity</b>: Linear in x.size().
   multimap& operator=(HBOOST_COPY_ASSIGN_REF(multimap) x)
   {  return static_cast<multimap&>(this->base_t::operator=(static_cast<const base_t&>(x)));  }

   //! <b>Effects</b>: this->swap(x.get()).
   //!
   //! <b>Complexity</b>: Constant.
   multimap& operator=(HBOOST_RV_REF(multimap) x)
      HBOOST_NOEXCEPT_IF(  allocator_traits_type::is_always_equal::value
                                 && hboost::container::container_detail::is_nothrow_move_assignable<Compare>::value )
   {  return static_cast<multimap&>(this->base_t::operator=(HBOOST_MOVE_BASE(base_t, x)));  }

#if !defined(HBOOST_NO_CXX11_HDR_INITIALIZER_LIST)
   //! <b>Effects</b>: Assign content of il to *this.
   //!
   multimap& operator=(std::initializer_list<value_type> il)
   {
       this->clear();
       insert(il.begin(), il.end());
       return *this;
   }
#endif

   #if defined(HBOOST_CONTAINER_DOXYGEN_INVOKED)

   //! @copydoc ::hboost::container::set::get_allocator()
   allocator_type get_allocator() const;

   //! @copydoc ::hboost::container::set::get_stored_allocator()
   stored_allocator_type &get_stored_allocator();

   //! @copydoc ::hboost::container::set::get_stored_allocator() const
   const stored_allocator_type &get_stored_allocator() const;

   //! @copydoc ::hboost::container::set::begin()
   iterator begin();

   //! @copydoc ::hboost::container::set::begin() const
   const_iterator begin() const;

   //! @copydoc ::hboost::container::set::cbegin() const
   const_iterator cbegin() const;

   //! @copydoc ::hboost::container::set::end()
   iterator end() HBOOST_NOEXCEPT_OR_NOTHROW;

   //! @copydoc ::hboost::container::set::end() const
   const_iterator end() const HBOOST_NOEXCEPT_OR_NOTHROW;

   //! @copydoc ::hboost::container::set::cend() const
   const_iterator cend() const HBOOST_NOEXCEPT_OR_NOTHROW;

   //! @copydoc ::hboost::container::set::rbegin()
   reverse_iterator rbegin() HBOOST_NOEXCEPT_OR_NOTHROW;

   //! @copydoc ::hboost::container::set::rbegin() const
   const_reverse_iterator rbegin() const HBOOST_NOEXCEPT_OR_NOTHROW;

   //! @copydoc ::hboost::container::set::crbegin() const
   const_reverse_iterator crbegin() const HBOOST_NOEXCEPT_OR_NOTHROW;

   //! @copydoc ::hboost::container::set::rend()
   reverse_iterator rend() HBOOST_NOEXCEPT_OR_NOTHROW;

   //! @copydoc ::hboost::container::set::rend() const
   const_reverse_iterator rend() const HBOOST_NOEXCEPT_OR_NOTHROW;

   //! @copydoc ::hboost::container::set::crend() const
   const_reverse_iterator crend() const HBOOST_NOEXCEPT_OR_NOTHROW;

   //! @copydoc ::hboost::container::set::empty() const
   bool empty() const;

   //! @copydoc ::hboost::container::set::size() const
   size_type size() const;

   //! @copydoc ::hboost::container::set::max_size() const
   size_type max_size() const;

   #endif   //#if defined(HBOOST_CONTAINER_DOXYGEN_INVOKED)

   #if !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES) || defined(HBOOST_CONTAINER_DOXYGEN_INVOKED)

   //! <b>Effects</b>: Inserts an object of type T constructed with
   //!   std::forward<Args>(args)... in the container.
   //!   p is a hint pointing to where the insert should start to search.
   //!
   //! <b>Returns</b>: An iterator pointing to the element with key equivalent
   //!   to the key of x.
   //!
   //! <b>Complexity</b>: Logarithmic in general, but amortized constant if t
   //!   is inserted right before p.
   template <class... Args>
   iterator emplace(HBOOST_FWD_REF(Args)... args)
   {  return this->base_t::emplace_equal(hboost::forward<Args>(args)...); }

   //! <b>Effects</b>: Inserts an object of type T constructed with
   //!   std::forward<Args>(args)... in the container.
   //!   p is a hint pointing to where the insert should start to search.
   //!
   //! <b>Returns</b>: An iterator pointing to the element with key equivalent
   //!   to the key of x.
   //!
   //! <b>Complexity</b>: Logarithmic in general, but amortized constant if t
   //!   is inserted right before p.
   template <class... Args>
   iterator emplace_hint(const_iterator p, HBOOST_FWD_REF(Args)... args)
   {  return this->base_t::emplace_hint_equal(p, hboost::forward<Args>(args)...); }

   #else // !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)

   #define HBOOST_CONTAINER_MULTIMAP_EMPLACE_CODE(N) \
   HBOOST_MOVE_TMPL_LT##N HBOOST_MOVE_CLASS##N HBOOST_MOVE_GT##N \
   iterator emplace(HBOOST_MOVE_UREF##N)\
   {  return this->base_t::emplace_equal(HBOOST_MOVE_FWD##N);   }\
   \
   HBOOST_MOVE_TMPL_LT##N HBOOST_MOVE_CLASS##N HBOOST_MOVE_GT##N \
   iterator emplace_hint(const_iterator hint HBOOST_MOVE_I##N HBOOST_MOVE_UREF##N)\
   {  return this->base_t::emplace_hint_equal(hint HBOOST_MOVE_I##N HBOOST_MOVE_FWD##N);  }\
   //
   HBOOST_MOVE_ITERATE_0TO9(HBOOST_CONTAINER_MULTIMAP_EMPLACE_CODE)
   #undef HBOOST_CONTAINER_MULTIMAP_EMPLACE_CODE

   #endif   // !defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)

   //! <b>Effects</b>: Inserts x and returns the iterator pointing to the
   //!   newly inserted element.
   //!
   //! <b>Complexity</b>: Logarithmic.
   iterator insert(const value_type& x)
   { return this->base_t::insert_equal(x); }

   //! <b>Effects</b>: Inserts a new value constructed from x and returns
   //!   the iterator pointing to the newly inserted element.
   //!
   //! <b>Complexity</b>: Logarithmic.
   iterator insert(const nonconst_value_type& x)
   { return this->base_t::insert_equal(x); }

   //! <b>Effects</b>: Inserts a new value move-constructed from x and returns
   //!   the iterator pointing to the newly inserted element.
   //!
   //! <b>Complexity</b>: Logarithmic.
   iterator insert(HBOOST_RV_REF(nonconst_value_type) x)
   { return this->base_t::insert_equal(hboost::move(x)); }

   //! <b>Effects</b>: Inserts a new value move-constructed from x and returns
   //!   the iterator pointing to the newly inserted element.
   //!
   //! <b>Complexity</b>: Logarithmic.
   iterator insert(HBOOST_RV_REF(movable_value_type) x)
   { return this->base_t::insert_equal(hboost::move(x)); }

   //! <b>Effects</b>: Inserts a copy of x in the container.
   //!   p is a hint pointing to where the insert should start to search.
   //!
   //! <b>Returns</b>: An iterator pointing to the element with key equivalent
   //!   to the key of x.
   //!
   //! <b>Complexity</b>: Logarithmic in general, but amortized constant if t
   //!   is inserted right before p.
   iterator insert(const_iterator p, const value_type& x)
   { return this->base_t::insert_equal(p, x); }

   //! <b>Effects</b>: Inserts a new value constructed from x in the container.
   //!   p is a hint pointing to where the insert should start to search.
   //!
   //! <b>Returns</b>: An iterator pointing to the element with key equivalent
   //!   to the key of x.
   //!
   //! <b>Complexity</b>: Logarithmic in general, but amortized constant if t
   //!   is inserted right before p.
   iterator insert(const_iterator p, const nonconst_value_type& x)
   { return this->base_t::insert_equal_convertible(p, x); }

   //! <b>Effects</b>: Inserts a new value move constructed from x in the container.
   //!   p is a hint pointing to where the insert should start to search.
   //!
   //! <b>Returns</b>: An iterator pointing to the element with key equivalent
   //!   to the key of x.
   //!
   //! <b>Complexity</b>: Logarithmic in general, but amortized constant if t
   //!   is inserted right before p.
   iterator insert(const_iterator p, HBOOST_RV_REF(nonconst_value_type) x)
   { return this->base_t::insert_equal_convertible(p, hboost::move(x)); }

   //! <b>Effects</b>: Inserts a new value move constructed from x in the container.
   //!   p is a hint pointing to where the insert should start to search.
   //!
   //! <b>Returns</b>: An iterator pointing to the element with key equivalent
   //!   to the key of x.
   //!
   //! <b>Complexity</b>: Logarithmic in general, but amortized constant if t
   //!   is inserted right before p.
   iterator insert(const_iterator p, HBOOST_RV_REF(movable_value_type) x)
   { return this->base_t::insert_equal_convertible(p, hboost::move(x)); }

   //! <b>Requires</b>: first, last are not iterators into *this.
   //!
   //! <b>Effects</b>: inserts each element from the range [first,last) .
   //!
   //! <b>Complexity</b>: At most N log(size()+N) (N is the distance from first to last)
   template <class InputIterator>
   void insert(InputIterator first, InputIterator last)
   {  this->base_t::insert_equal(first, last); }

#if !defined(HBOOST_NO_CXX11_HDR_INITIALIZER_LIST)
   //! <b>Effects</b>: inserts each element from the range [il.begin(), il.end().
   //!
   //! <b>Complexity</b>: At most N log(size()+N) (N is the distance from il.begin() to il.end())
   void insert(std::initializer_list<value_type> il)
   {  this->base_t::insert_equal(il.begin(), il.end()); }
#endif

   #if defined(HBOOST_CONTAINER_DOXYGEN_INVOKED)

   //! @copydoc ::hboost::container::set::erase(const_iterator)
   iterator erase(const_iterator p);

   //! @copydoc ::hboost::container::set::erase(const key_type&)
   size_type erase(const key_type& x);

   //! @copydoc ::hboost::container::set::erase(const_iterator,const_iterator)
   iterator erase(const_iterator first, const_iterator last);

   //! @copydoc ::hboost::container::set::swap
   void swap(multiset& x)
      HBOOST_NOEXCEPT_IF(  allocator_traits_type::is_always_equal::value
                                 && hboost::container::container_detail::is_nothrow_swappable<Compare>::value );

   //! @copydoc ::hboost::container::set::clear
   void clear() HBOOST_NOEXCEPT_OR_NOTHROW;

   //! @copydoc ::hboost::container::set::key_comp
   key_compare key_comp() const;

   //! @copydoc ::hboost::container::set::value_comp
   value_compare value_comp() const;

   //! <b>Returns</b>: An iterator pointing to an element with the key
   //!   equivalent to x, or end() if such an element is not found.
   //!
   //! <b>Complexity</b>: Logarithmic.
   iterator find(const key_type& x);

   //! <b>Returns</b>: A const iterator pointing to an element with the key
   //!   equivalent to x, or end() if such an element is not found.
   //!
   //! <b>Complexity</b>: Logarithmic.
   const_iterator find(const key_type& x) const;

   //! <b>Returns</b>: The number of elements with key equivalent to x.
   //!
   //! <b>Complexity</b>: log(size())+count(k)
   size_type count(const key_type& x) const;

   //! <b>Returns</b>: An iterator pointing to the first element with key not less
   //!   than k, or a.end() if such an element is not found.
   //!
   //! <b>Complexity</b>: Logarithmic
   iterator lower_bound(const key_type& x);

   //! <b>Returns</b>: A const iterator pointing to the first element with key not
   //!   less than k, or a.end() if such an element is not found.
   //!
   //! <b>Complexity</b>: Logarithmic
   const_iterator lower_bound(const key_type& x) const;

   //! <b>Returns</b>: An iterator pointing to the first element with key not less
   //!   than x, or end() if such an element is not found.
   //!
   //! <b>Complexity</b>: Logarithmic
   iterator upper_bound(const key_type& x);

   //! <b>Returns</b>: A const iterator pointing to the first element with key not
   //!   less than x, or end() if such an element is not found.
   //!
   //! <b>Complexity</b>: Logarithmic
   const_iterator upper_bound(const key_type& x) const;

   //! <b>Effects</b>: Equivalent to std::make_pair(this->lower_bound(k), this->upper_bound(k)).
   //!
   //! <b>Complexity</b>: Logarithmic
   std::pair<iterator,iterator> equal_range(const key_type& x);

   //! <b>Effects</b>: Equivalent to std::make_pair(this->lower_bound(k), this->upper_bound(k)).
   //!
   //! <b>Complexity</b>: Logarithmic
   std::pair<const_iterator,const_iterator> equal_range(const key_type& x) const;

   //! <b>Effects</b>: Rebalances the tree. It's a no-op for Red-Black and AVL trees.
   //!
   //! <b>Complexity</b>: Linear
   void rebalance();

   //! <b>Effects</b>: Returns true if x and y are equal
   //!
   //! <b>Complexity</b>: Linear to the number of elements in the container.
   friend bool operator==(const multimap& x, const multimap& y);

   //! <b>Effects</b>: Returns true if x and y are unequal
   //!
   //! <b>Complexity</b>: Linear to the number of elements in the container.
   friend bool operator!=(const multimap& x, const multimap& y);

   //! <b>Effects</b>: Returns true if x is less than y
   //!
   //! <b>Complexity</b>: Linear to the number of elements in the container.
   friend bool operator<(const multimap& x, const multimap& y);

   //! <b>Effects</b>: Returns true if x is greater than y
   //!
   //! <b>Complexity</b>: Linear to the number of elements in the container.
   friend bool operator>(const multimap& x, const multimap& y);

   //! <b>Effects</b>: Returns true if x is equal or less than y
   //!
   //! <b>Complexity</b>: Linear to the number of elements in the container.
   friend bool operator<=(const multimap& x, const multimap& y);

   //! <b>Effects</b>: Returns true if x is equal or greater than y
   //!
   //! <b>Complexity</b>: Linear to the number of elements in the container.
   friend bool operator>=(const multimap& x, const multimap& y);

   //! <b>Effects</b>: x.swap(y)
   //!
   //! <b>Complexity</b>: Constant.
   friend void swap(multimap& x, multimap& y);

   #endif   //#if defined(HBOOST_CONTAINER_DOXYGEN_INVOKED)
};

#ifndef HBOOST_CONTAINER_DOXYGEN_INVOKED

}  //namespace container {

//!has_trivial_destructor_after_move<> == true_type
//!specialization for optimizations
template <class Key, class T, class Compare, class Allocator>
struct has_trivial_destructor_after_move<hboost::container::multimap<Key, T, Compare, Allocator> >
{
   typedef typename ::hboost::container::allocator_traits<Allocator>::pointer pointer;
   static const bool value = ::hboost::has_trivial_destructor_after_move<Allocator>::value &&
                             ::hboost::has_trivial_destructor_after_move<pointer>::value &&
                             ::hboost::has_trivial_destructor_after_move<Compare>::value;
};

namespace container {

#endif   //#ifndef HBOOST_CONTAINER_DOXYGEN_INVOKED

}}

#include <hboost/container/detail/config_end.hpp>

#endif   // HBOOST_CONTAINER_MAP_HPP

