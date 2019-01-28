/* Copyright 2003-2015 Joaquin M Lopez Munoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/multi_index for library home page.
 */

#ifndef HBOOST_MULTI_INDEX_SEQUENCED_INDEX_HPP
#define HBOOST_MULTI_INDEX_SEQUENCED_INDEX_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <hboost/config.hpp> /* keep it first to prevent nasty warns in MSVC */
#include <hboost/bind.hpp>
#include <hboost/call_traits.hpp>
#include <hboost/detail/allocator_utilities.hpp>
#include <hboost/detail/no_exceptions_support.hpp>
#include <hboost/detail/workaround.hpp>
#include <hboost/foreach_fwd.hpp>
#include <hboost/iterator/reverse_iterator.hpp>
#include <hboost/move/core.hpp>
#include <hboost/move/utility.hpp>
#include <hboost/mpl/bool.hpp>
#include <hboost/mpl/not.hpp>
#include <hboost/mpl/push_front.hpp>
#include <hboost/multi_index/detail/access_specifier.hpp>
#include <hboost/multi_index/detail/bidir_node_iterator.hpp>
#include <hboost/multi_index/detail/do_not_copy_elements_tag.hpp>
#include <hboost/multi_index/detail/index_node_base.hpp>
#include <hboost/multi_index/detail/safe_mode.hpp>
#include <hboost/multi_index/detail/scope_guard.hpp>
#include <hboost/multi_index/detail/seq_index_node.hpp>
#include <hboost/multi_index/detail/seq_index_ops.hpp>
#include <hboost/multi_index/detail/vartempl_support.hpp>
#include <hboost/multi_index/sequenced_index_fwd.hpp>
#include <hboost/tuple/tuple.hpp>
#include <hboost/type_traits/is_integral.hpp>
#include <cstddef>
#include <functional>
#include <utility>

#if !defined(HBOOST_NO_CXX11_HDR_INITIALIZER_LIST)
#include<initializer_list>
#endif

#if !defined(HBOOST_MULTI_INDEX_DISABLE_SERIALIZATION)
#include <hboost/bind.hpp>
#endif

#if defined(HBOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING)
#define HBOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT_OF(x)                    \
  detail::scope_guard HBOOST_JOIN(check_invariant_,__LINE__)=                 \
    detail::make_obj_guard(x,&sequenced_index::check_invariant_);            \
  HBOOST_JOIN(check_invariant_,__LINE__).touch();
#define HBOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT                          \
  HBOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT_OF(*this)
#else
#define HBOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT_OF(x)
#define HBOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT
#endif

namespace hboost{

namespace multi_index{

namespace detail{

/* sequenced_index adds a layer of sequenced indexing to a given Super */

template<typename SuperMeta,typename TagList>
class sequenced_index:
  HBOOST_MULTI_INDEX_PROTECTED_IF_MEMBER_TEMPLATE_FRIENDS SuperMeta::type

#if defined(HBOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
  ,public safe_mode::safe_container<
    sequenced_index<SuperMeta,TagList> >
#endif

{ 
#if defined(HBOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING)&&\
    HBOOST_WORKAROUND(__MWERKS__,<=0x3003)
/* The "ISO C++ Template Parser" option in CW8.3 has a problem with the
 * lifetime of const references bound to temporaries --precisely what
 * scopeguards are.
 */

#pragma parse_mfunc_templ off
#endif

  typedef typename SuperMeta::type                    super;

protected:
  typedef sequenced_index_node<
    typename super::node_type>                        node_type;

private:
  typedef typename node_type::impl_type               node_impl_type;
 
public:
  /* types */

  typedef typename node_type::value_type              value_type;
  typedef tuples::null_type                           ctor_args;
  typedef typename super::final_allocator_type        allocator_type;
  typedef typename allocator_type::reference          reference;
  typedef typename allocator_type::const_reference    const_reference;

#if defined(HBOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
  typedef safe_mode::safe_iterator<
    bidir_node_iterator<node_type>,
    sequenced_index>                                  iterator;
#else
  typedef bidir_node_iterator<node_type>              iterator;
#endif

  typedef iterator                                    const_iterator;

  typedef std::size_t                                 size_type;      
  typedef std::ptrdiff_t                              difference_type;
  typedef typename allocator_type::pointer            pointer;
  typedef typename allocator_type::const_pointer      const_pointer;
  typedef typename
    hboost::reverse_iterator<iterator>                 reverse_iterator;
  typedef typename
    hboost::reverse_iterator<const_iterator>           const_reverse_iterator;
  typedef TagList                                     tag_list;

protected:
  typedef typename super::final_node_type     final_node_type;
  typedef tuples::cons<
    ctor_args, 
    typename super::ctor_args_list>           ctor_args_list;
  typedef typename mpl::push_front<
    typename super::index_type_list,
    sequenced_index>::type                    index_type_list;
  typedef typename mpl::push_front<
    typename super::iterator_type_list,
    iterator>::type                           iterator_type_list;
  typedef typename mpl::push_front<
    typename super::const_iterator_type_list,
    const_iterator>::type                     const_iterator_type_list;
  typedef typename super::copy_map_type       copy_map_type;

#if !defined(HBOOST_MULTI_INDEX_DISABLE_SERIALIZATION)
  typedef typename super::index_saver_type    index_saver_type;
  typedef typename super::index_loader_type   index_loader_type;
#endif

private:
#if defined(HBOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
  typedef safe_mode::safe_container<
    sequenced_index>                          safe_super;
#endif

  typedef typename call_traits<value_type>::param_type value_param_type;

  /* Needed to avoid commas in HBOOST_MULTI_INDEX_OVERLOADS_TO_VARTEMPL
   * expansion.
   */

  typedef std::pair<iterator,bool>                     emplace_return_type;

public:

  /* construct/copy/destroy
   * Default and copy ctors are in the protected section as indices are
   * not supposed to be created on their own. No range ctor either.
   */

  sequenced_index<SuperMeta,TagList>& operator=(
    const sequenced_index<SuperMeta,TagList>& x)
  {
    this->final()=x.final();
    return *this;
  }

#if !defined(HBOOST_NO_CXX11_HDR_INITIALIZER_LIST)
  sequenced_index<SuperMeta,TagList>& operator=(
    std::initializer_list<value_type> list)
  {
    this->final()=list;
    return *this;
  }
#endif

  template <class InputIterator>
  void assign(InputIterator first,InputIterator last)
  {
    assign_iter(first,last,mpl::not_<is_integral<InputIterator> >());
  }

#if !defined(HBOOST_NO_CXX11_HDR_INITIALIZER_LIST)
  void assign(std::initializer_list<value_type> list)
  {
    assign(list.begin(),list.end());
  }
#endif

  void assign(size_type n,value_param_type value)
  {
    HBOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    clear();
    for(size_type i=0;i<n;++i)push_back(value);
  }
    
  allocator_type get_allocator()const HBOOST_NOEXCEPT
  {
    return this->final().get_allocator();
  }

  /* iterators */

  iterator  begin()HBOOST_NOEXCEPT
    {return make_iterator(node_type::from_impl(header()->next()));}
  const_iterator begin()const HBOOST_NOEXCEPT
    {return make_iterator(node_type::from_impl(header()->next()));}
  iterator
    end()HBOOST_NOEXCEPT{return make_iterator(header());}
  const_iterator
    end()const HBOOST_NOEXCEPT{return make_iterator(header());}
  reverse_iterator
    rbegin()HBOOST_NOEXCEPT{return hboost::make_reverse_iterator(end());}
  const_reverse_iterator
    rbegin()const HBOOST_NOEXCEPT{return hboost::make_reverse_iterator(end());}
  reverse_iterator
    rend()HBOOST_NOEXCEPT{return hboost::make_reverse_iterator(begin());}
  const_reverse_iterator
    rend()const HBOOST_NOEXCEPT{return hboost::make_reverse_iterator(begin());}
  const_iterator
    cbegin()const HBOOST_NOEXCEPT{return begin();}
  const_iterator
    cend()const HBOOST_NOEXCEPT{return end();}
  const_reverse_iterator
    crbegin()const HBOOST_NOEXCEPT{return rbegin();}
  const_reverse_iterator
    crend()const HBOOST_NOEXCEPT{return rend();}

  iterator iterator_to(const value_type& x)
  {
    return make_iterator(node_from_value<node_type>(&x));
  }

  const_iterator iterator_to(const value_type& x)const
  {
    return make_iterator(node_from_value<node_type>(&x));
  }

  /* capacity */

  bool      empty()const HBOOST_NOEXCEPT{return this->final_empty_();}
  size_type size()const HBOOST_NOEXCEPT{return this->final_size_();}
  size_type max_size()const HBOOST_NOEXCEPT{return this->final_max_size_();}

  void resize(size_type n)
  {
    HBOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    if(n>size()){
      for(size_type m=n-size();m--;)
        this->final_emplace_(HBOOST_MULTI_INDEX_NULL_PARAM_PACK);
    }
    else if(n<size()){for(size_type m=size()-n;m--;)pop_back();}
  }

  void resize(size_type n,value_param_type x)
  {
    HBOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    if(n>size())insert(end(),n-size(),x);
    else if(n<size())for(size_type m=size()-n;m--;)pop_back();
  }

  /* access: no non-const versions provided as sequenced_index
   * handles const elements.
   */

  const_reference front()const{return *begin();}
  const_reference back()const{return *--end();}

  /* modifiers */

  HBOOST_MULTI_INDEX_OVERLOADS_TO_VARTEMPL(
    emplace_return_type,emplace_front,emplace_front_impl)

  std::pair<iterator,bool> push_front(const value_type& x)
                             {return insert(begin(),x);}
  std::pair<iterator,bool> push_front(HBOOST_RV_REF(value_type) x)
                             {return insert(begin(),hboost::move(x));}
  void                     pop_front(){erase(begin());}

  HBOOST_MULTI_INDEX_OVERLOADS_TO_VARTEMPL(
    emplace_return_type,emplace_back,emplace_back_impl)

  std::pair<iterator,bool> push_back(const value_type& x)
                             {return insert(end(),x);}
  std::pair<iterator,bool> push_back(HBOOST_RV_REF(value_type) x)
                             {return insert(end(),hboost::move(x));}
  void                     pop_back(){erase(--end());}

  HBOOST_MULTI_INDEX_OVERLOADS_TO_VARTEMPL_EXTRA_ARG(
    emplace_return_type,emplace,emplace_impl,iterator,position)

  std::pair<iterator,bool> insert(iterator position,const value_type& x)
  {
    HBOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
    HBOOST_MULTI_INDEX_CHECK_IS_OWNER(position,*this);
    HBOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    std::pair<final_node_type*,bool> p=this->final_insert_(x);
    if(p.second&&position.get_node()!=header()){
      relink(position.get_node(),p.first);
    }
    return std::pair<iterator,bool>(make_iterator(p.first),p.second);
  }

  std::pair<iterator,bool> insert(iterator position,HBOOST_RV_REF(value_type) x)
  {
    HBOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
    HBOOST_MULTI_INDEX_CHECK_IS_OWNER(position,*this);
    HBOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    std::pair<final_node_type*,bool> p=this->final_insert_rv_(x);
    if(p.second&&position.get_node()!=header()){
      relink(position.get_node(),p.first);
    }
    return std::pair<iterator,bool>(make_iterator(p.first),p.second);
  }

  void insert(iterator position,size_type n,value_param_type x)
  {
    HBOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
    HBOOST_MULTI_INDEX_CHECK_IS_OWNER(position,*this);
    HBOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    for(size_type i=0;i<n;++i)insert(position,x);
  }
 
  template<typename InputIterator>
  void insert(iterator position,InputIterator first,InputIterator last)
  {
    insert_iter(position,first,last,mpl::not_<is_integral<InputIterator> >());
  }

#if !defined(HBOOST_NO_CXX11_HDR_INITIALIZER_LIST)
  void insert(iterator position,std::initializer_list<value_type> list)
  {
    insert(position,list.begin(),list.end());
  }
#endif

  iterator erase(iterator position)
  {
    HBOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
    HBOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
    HBOOST_MULTI_INDEX_CHECK_IS_OWNER(position,*this);
    HBOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    this->final_erase_(static_cast<final_node_type*>(position++.get_node()));
    return position;
  }
  
  iterator erase(iterator first,iterator last)
  {
    HBOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(first);
    HBOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(last);
    HBOOST_MULTI_INDEX_CHECK_IS_OWNER(first,*this);
    HBOOST_MULTI_INDEX_CHECK_IS_OWNER(last,*this);
    HBOOST_MULTI_INDEX_CHECK_VALID_RANGE(first,last);
    HBOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    while(first!=last){
      first=erase(first);
    }
    return first;
  }

  bool replace(iterator position,const value_type& x)
  {
    HBOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
    HBOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
    HBOOST_MULTI_INDEX_CHECK_IS_OWNER(position,*this);
    HBOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    return this->final_replace_(
      x,static_cast<final_node_type*>(position.get_node()));
  }

  bool replace(iterator position,HBOOST_RV_REF(value_type) x)
  {
    HBOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
    HBOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
    HBOOST_MULTI_INDEX_CHECK_IS_OWNER(position,*this);
    HBOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    return this->final_replace_rv_(
      x,static_cast<final_node_type*>(position.get_node()));
  }

  template<typename Modifier>
  bool modify(iterator position,Modifier mod)
  {
    HBOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
    HBOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
    HBOOST_MULTI_INDEX_CHECK_IS_OWNER(position,*this);
    HBOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;

#if defined(HBOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
    /* MSVC++ 6.0 optimizer on safe mode code chokes if this
     * this is not added. Left it for all compilers as it does no
     * harm.
     */

    position.detach();
#endif

    return this->final_modify_(
      mod,static_cast<final_node_type*>(position.get_node()));
  }

  template<typename Modifier,typename Rollback>
  bool modify(iterator position,Modifier mod,Rollback back_)
  {
    HBOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
    HBOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(position);
    HBOOST_MULTI_INDEX_CHECK_IS_OWNER(position,*this);
    HBOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;

#if defined(HBOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
    /* MSVC++ 6.0 optimizer on safe mode code chokes if this
     * this is not added. Left it for all compilers as it does no
     * harm.
     */

    position.detach();
#endif

    return this->final_modify_(
      mod,back_,static_cast<final_node_type*>(position.get_node()));
  }

  void swap(sequenced_index<SuperMeta,TagList>& x)
  {
    HBOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    HBOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT_OF(x);
    this->final_swap_(x.final());
  }

  void clear()HBOOST_NOEXCEPT
  {
    HBOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    this->final_clear_();
  }

  /* list operations */

  void splice(iterator position,sequenced_index<SuperMeta,TagList>& x)
  {
    HBOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
    HBOOST_MULTI_INDEX_CHECK_IS_OWNER(position,*this);
    HBOOST_MULTI_INDEX_CHECK_DIFFERENT_CONTAINER(*this,x);
    HBOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    iterator first=x.begin(),last=x.end();
    while(first!=last){
      if(insert(position,*first).second)first=x.erase(first);
      else ++first;
    }
  }

  void splice(iterator position,sequenced_index<SuperMeta,TagList>& x,iterator i)
  {
    HBOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
    HBOOST_MULTI_INDEX_CHECK_IS_OWNER(position,*this);
    HBOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(i);
    HBOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(i);
    HBOOST_MULTI_INDEX_CHECK_IS_OWNER(i,x);
    HBOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    if(&x==this){
      if(position!=i)relink(position.get_node(),i.get_node());
    }
    else{
      if(insert(position,*i).second){

#if defined(HBOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
    /* MSVC++ 6.0 optimizer has a hard time with safe mode, and the following
     * workaround is needed. Left it for all compilers as it does no
     * harm.
     */
        i.detach();
        x.erase(x.make_iterator(i.get_node()));
#else
        x.erase(i);
#endif

      }
    }
  }

  void splice(
    iterator position,sequenced_index<SuperMeta,TagList>& x,
    iterator first,iterator last)
  {
    HBOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
    HBOOST_MULTI_INDEX_CHECK_IS_OWNER(position,*this);
    HBOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(first);
    HBOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(last);
    HBOOST_MULTI_INDEX_CHECK_IS_OWNER(first,x);
    HBOOST_MULTI_INDEX_CHECK_IS_OWNER(last,x);
    HBOOST_MULTI_INDEX_CHECK_VALID_RANGE(first,last);
    HBOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    if(&x==this){
      HBOOST_MULTI_INDEX_CHECK_OUTSIDE_RANGE(position,first,last);
      if(position!=last)relink(
        position.get_node(),first.get_node(),last.get_node());
    }
    else{
      while(first!=last){
        if(insert(position,*first).second)first=x.erase(first);
        else ++first;
      }
    }
  }

  void remove(value_param_type value)
  {
    sequenced_index_remove(
      *this,
      ::hboost::bind(std::equal_to<value_type>(),::hboost::arg<1>(),value));
  }

  template<typename Predicate>
  void remove_if(Predicate pred)
  {
    sequenced_index_remove(*this,pred);
  }

  void unique()
  {
    sequenced_index_unique(*this,std::equal_to<value_type>());
  }

  template <class BinaryPredicate>
  void unique(BinaryPredicate binary_pred)
  {
    sequenced_index_unique(*this,binary_pred);
  }

  void merge(sequenced_index<SuperMeta,TagList>& x)
  {
    sequenced_index_merge(*this,x,std::less<value_type>());
  }

  template <typename Compare>
  void merge(sequenced_index<SuperMeta,TagList>& x,Compare comp)
  {
    sequenced_index_merge(*this,x,comp);
  }

  void sort()
  {
    HBOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    sequenced_index_sort(header(),std::less<value_type>());
  }

  template <typename Compare>
  void sort(Compare comp)
  {
    HBOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    sequenced_index_sort(header(),comp);
  }

  void reverse()HBOOST_NOEXCEPT
  {
    HBOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    node_impl_type::reverse(header()->impl());
  }

  /* rearrange operations */

  void relocate(iterator position,iterator i)
  {
    HBOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
    HBOOST_MULTI_INDEX_CHECK_IS_OWNER(position,*this);
    HBOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(i);
    HBOOST_MULTI_INDEX_CHECK_DEREFERENCEABLE_ITERATOR(i);
    HBOOST_MULTI_INDEX_CHECK_IS_OWNER(i,*this);
    HBOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    if(position!=i)relink(position.get_node(),i.get_node());
  }

  void relocate(iterator position,iterator first,iterator last)
  {
    HBOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
    HBOOST_MULTI_INDEX_CHECK_IS_OWNER(position,*this);
    HBOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(first);
    HBOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(last);
    HBOOST_MULTI_INDEX_CHECK_IS_OWNER(first,*this);
    HBOOST_MULTI_INDEX_CHECK_IS_OWNER(last,*this);
    HBOOST_MULTI_INDEX_CHECK_VALID_RANGE(first,last);
    HBOOST_MULTI_INDEX_CHECK_OUTSIDE_RANGE(position,first,last);
    HBOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    if(position!=last)relink(
      position.get_node(),first.get_node(),last.get_node());
  }
    
  template<typename InputIterator>
  void rearrange(InputIterator first)
  {
    HBOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    node_type* pos=header();
    for(size_type s=size();s--;){
      const value_type& v=*first++;
      relink(pos,node_from_value<node_type>(&v));
    }
  }

HBOOST_MULTI_INDEX_PROTECTED_IF_MEMBER_TEMPLATE_FRIENDS:
  sequenced_index(const ctor_args_list& args_list,const allocator_type& al):
    super(args_list.get_tail(),al)
  {
    empty_initialize();
  }

  sequenced_index(const sequenced_index<SuperMeta,TagList>& x):
    super(x)

#if defined(HBOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
    ,safe_super()
#endif

  {
    /* the actual copying takes place in subsequent call to copy_() */
  }

  sequenced_index(
    const sequenced_index<SuperMeta,TagList>& x,do_not_copy_elements_tag):
    super(x,do_not_copy_elements_tag())

#if defined(HBOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
    ,safe_super()
#endif

  {
    empty_initialize();
  }

  ~sequenced_index()
  {
    /* the container is guaranteed to be empty by now */
  }

#if defined(HBOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
  iterator       make_iterator(node_type* node){return iterator(node,this);}
  const_iterator make_iterator(node_type* node)const
    {return const_iterator(node,const_cast<sequenced_index*>(this));}
#else
  iterator       make_iterator(node_type* node){return iterator(node);}
  const_iterator make_iterator(node_type* node)const
                   {return const_iterator(node);}
#endif

  void copy_(
    const sequenced_index<SuperMeta,TagList>& x,const copy_map_type& map)
  {
    node_type* org=x.header();
    node_type* cpy=header();
    do{
      node_type* next_org=node_type::from_impl(org->next());
      node_type* next_cpy=map.find(static_cast<final_node_type*>(next_org));
      cpy->next()=next_cpy->impl();
      next_cpy->prior()=cpy->impl();
      org=next_org;
      cpy=next_cpy;
    }while(org!=x.header());

    super::copy_(x,map);
  }

  template<typename Variant>
  final_node_type* insert_(
    value_param_type v,final_node_type*& x,Variant variant)
  {
    final_node_type* res=super::insert_(v,x,variant);
    if(res==x)link(static_cast<node_type*>(x));
    return res;
  }

  template<typename Variant>
  final_node_type* insert_(
    value_param_type v,node_type* position,final_node_type*& x,Variant variant)
  {
    final_node_type* res=super::insert_(v,position,x,variant);
    if(res==x)link(static_cast<node_type*>(x));
    return res;
  }

  void erase_(node_type* x)
  {
    unlink(x);
    super::erase_(x);

#if defined(HBOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
    detach_iterators(x);
#endif
  }

  void delete_all_nodes_()
  {
    for(node_type* x=node_type::from_impl(header()->next());x!=header();){
      node_type* y=node_type::from_impl(x->next());
      this->final_delete_node_(static_cast<final_node_type*>(x));
      x=y;
    }
  }

  void clear_()
  {
    super::clear_();
    empty_initialize();

#if defined(HBOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
    safe_super::detach_dereferenceable_iterators();
#endif
  }

  void swap_(sequenced_index<SuperMeta,TagList>& x)
  {
#if defined(HBOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
    safe_super::swap(x);
#endif

    super::swap_(x);
  }

  void swap_elements_(sequenced_index<SuperMeta,TagList>& x)
  {
#if defined(HBOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
    safe_super::swap(x);
#endif

    super::swap_elements_(x);
  }

  template<typename Variant>
  bool replace_(value_param_type v,node_type* x,Variant variant)
  {
    return super::replace_(v,x,variant);
  }

  bool modify_(node_type* x)
  {
    HBOOST_TRY{
      if(!super::modify_(x)){
        unlink(x);

#if defined(HBOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
        detach_iterators(x);
#endif

        return false;
      }
      else return true;
    }
    HBOOST_CATCH(...){
      unlink(x);

#if defined(HBOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
      detach_iterators(x);
#endif

      HBOOST_RETHROW;
    }
    HBOOST_CATCH_END
  }

  bool modify_rollback_(node_type* x)
  {
    return super::modify_rollback_(x);
  }

#if !defined(HBOOST_MULTI_INDEX_DISABLE_SERIALIZATION)
  /* serialization */

  template<typename Archive>
  void save_(
    Archive& ar,const unsigned int version,const index_saver_type& sm)const
  {
    sm.save(begin(),end(),ar,version);
    super::save_(ar,version,sm);
  }

  template<typename Archive>
  void load_(
    Archive& ar,const unsigned int version,const index_loader_type& lm)
  {
    lm.load(
      ::hboost::bind(
        &sequenced_index::rearranger,this,::hboost::arg<1>(),::hboost::arg<2>()),
      ar,version);
    super::load_(ar,version,lm);
  }
#endif

#if defined(HBOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING)
  /* invariant stuff */

  bool invariant_()const
  {
    if(size()==0||begin()==end()){
      if(size()!=0||begin()!=end()||
         header()->next()!=header()->impl()||
         header()->prior()!=header()->impl())return false;
    }
    else{
      size_type s=0;
      for(const_iterator it=begin(),it_end=end();it!=it_end;++it,++s){
        if(it.get_node()->next()->prior()!=it.get_node()->impl())return false;
        if(it.get_node()->prior()->next()!=it.get_node()->impl())return false;
      }
      if(s!=size())return false;
    }

    return super::invariant_();
  }

  /* This forwarding function eases things for the hboost::mem_fn construct
   * in HBOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT. Actually,
   * final_check_invariant is already an inherited member function of index.
   */
  void check_invariant_()const{this->final_check_invariant_();}
#endif

private:
  node_type* header()const{return this->final_header();}

  void empty_initialize()
  {
    header()->prior()=header()->next()=header()->impl();
  }

  void link(node_type* x)
  {
    node_impl_type::link(x->impl(),header()->impl());
  };

  static void unlink(node_type* x)
  {
    node_impl_type::unlink(x->impl());
  }

  static void relink(node_type* position,node_type* x)
  {
    node_impl_type::relink(position->impl(),x->impl());
  }

  static void relink(node_type* position,node_type* first,node_type* last)
  {
    node_impl_type::relink(
      position->impl(),first->impl(),last->impl());
  }

#if !defined(HBOOST_MULTI_INDEX_DISABLE_SERIALIZATION)
  void rearranger(node_type* position,node_type *x)
  {
    if(!position)position=header();
    node_type::increment(position);
    if(position!=x)relink(position,x);
  }
#endif

#if defined(HBOOST_MULTI_INDEX_ENABLE_SAFE_MODE)
  void detach_iterators(node_type* x)
  {
    iterator it=make_iterator(x);
    safe_mode::detach_equivalent_iterators(it);
  }
#endif

  template <class InputIterator>
  void assign_iter(InputIterator first,InputIterator last,mpl::true_)
  {
    HBOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    clear();
    for(;first!=last;++first)this->final_insert_ref_(*first);
  }

  void assign_iter(size_type n,value_param_type value,mpl::false_)
  {
    HBOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    clear();
    for(size_type i=0;i<n;++i)push_back(value);
  }

  template<typename InputIterator>
  void insert_iter(
    iterator position,InputIterator first,InputIterator last,mpl::true_)
  {
    HBOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    for(;first!=last;++first){
      std::pair<final_node_type*,bool> p=
        this->final_insert_ref_(*first);
      if(p.second&&position.get_node()!=header()){
        relink(position.get_node(),p.first);
      }
    }
  }

  void insert_iter(
    iterator position,size_type n,value_param_type x,mpl::false_)
  {
    HBOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
    HBOOST_MULTI_INDEX_CHECK_IS_OWNER(position,*this);
    HBOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    for(size_type i=0;i<n;++i)insert(position,x);
  }

  template<HBOOST_MULTI_INDEX_TEMPLATE_PARAM_PACK>
  std::pair<iterator,bool> emplace_front_impl(
    HBOOST_MULTI_INDEX_FUNCTION_PARAM_PACK)
  {
    return emplace_impl(begin(),HBOOST_MULTI_INDEX_FORWARD_PARAM_PACK);
  }

  template<HBOOST_MULTI_INDEX_TEMPLATE_PARAM_PACK>
  std::pair<iterator,bool> emplace_back_impl(
    HBOOST_MULTI_INDEX_FUNCTION_PARAM_PACK)
  {
    return emplace_impl(end(),HBOOST_MULTI_INDEX_FORWARD_PARAM_PACK);
  }

  template<HBOOST_MULTI_INDEX_TEMPLATE_PARAM_PACK>
  std::pair<iterator,bool> emplace_impl(
    iterator position,HBOOST_MULTI_INDEX_FUNCTION_PARAM_PACK)
  {
    HBOOST_MULTI_INDEX_CHECK_VALID_ITERATOR(position);
    HBOOST_MULTI_INDEX_CHECK_IS_OWNER(position,*this);
    HBOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT;
    std::pair<final_node_type*,bool> p=
      this->final_emplace_(HBOOST_MULTI_INDEX_FORWARD_PARAM_PACK);
    if(p.second&&position.get_node()!=header()){
      relink(position.get_node(),p.first);
    }
    return std::pair<iterator,bool>(make_iterator(p.first),p.second);
  }

#if defined(HBOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING)&&\
    HBOOST_WORKAROUND(__MWERKS__,<=0x3003)
#pragma parse_mfunc_templ reset
#endif
};

/* comparison */

template<
  typename SuperMeta1,typename TagList1,
  typename SuperMeta2,typename TagList2
>
bool operator==(
  const sequenced_index<SuperMeta1,TagList1>& x,
  const sequenced_index<SuperMeta2,TagList2>& y)
{
  return x.size()==y.size()&&std::equal(x.begin(),x.end(),y.begin());
}

template<
  typename SuperMeta1,typename TagList1,
  typename SuperMeta2,typename TagList2
>
bool operator<(
  const sequenced_index<SuperMeta1,TagList1>& x,
  const sequenced_index<SuperMeta2,TagList2>& y)
{
  return std::lexicographical_compare(x.begin(),x.end(),y.begin(),y.end());
}

template<
  typename SuperMeta1,typename TagList1,
  typename SuperMeta2,typename TagList2
>
bool operator!=(
  const sequenced_index<SuperMeta1,TagList1>& x,
  const sequenced_index<SuperMeta2,TagList2>& y)
{
  return !(x==y);
}

template<
  typename SuperMeta1,typename TagList1,
  typename SuperMeta2,typename TagList2
>
bool operator>(
  const sequenced_index<SuperMeta1,TagList1>& x,
  const sequenced_index<SuperMeta2,TagList2>& y)
{
  return y<x;
}

template<
  typename SuperMeta1,typename TagList1,
  typename SuperMeta2,typename TagList2
>
bool operator>=(
  const sequenced_index<SuperMeta1,TagList1>& x,
  const sequenced_index<SuperMeta2,TagList2>& y)
{
  return !(x<y);
}

template<
  typename SuperMeta1,typename TagList1,
  typename SuperMeta2,typename TagList2
>
bool operator<=(
  const sequenced_index<SuperMeta1,TagList1>& x,
  const sequenced_index<SuperMeta2,TagList2>& y)
{
  return !(x>y);
}

/*  specialized algorithms */

template<typename SuperMeta,typename TagList>
void swap(
  sequenced_index<SuperMeta,TagList>& x,
  sequenced_index<SuperMeta,TagList>& y)
{
  x.swap(y);
}

} /* namespace multi_index::detail */

/* sequenced index specifier */

template <typename TagList>
struct sequenced
{
  HBOOST_STATIC_ASSERT(detail::is_tag<TagList>::value);

  template<typename Super>
  struct node_class
  {
    typedef detail::sequenced_index_node<Super> type;
  };

  template<typename SuperMeta>
  struct index_class
  {
    typedef detail::sequenced_index<SuperMeta,typename TagList::type> type;
  };
};

} /* namespace multi_index */

} /* namespace hboost */

/* Boost.Foreach compatibility */

template<typename SuperMeta,typename TagList>
inline hboost::mpl::true_* hboost_foreach_is_noncopyable(
  hboost::multi_index::detail::sequenced_index<SuperMeta,TagList>*&,
  hboost_foreach_argument_dependent_lookup_hack)
{
  return 0;
}

#undef HBOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT
#undef HBOOST_MULTI_INDEX_SEQ_INDEX_CHECK_INVARIANT_OF

#endif