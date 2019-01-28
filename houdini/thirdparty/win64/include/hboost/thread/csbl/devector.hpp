// Copyright (C) 2013 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// 2013/10 Vicente J. Botet Escriba
//   Creation.

#ifndef HBOOST_CSBL_DEVECTOR_HPP
#define HBOOST_CSBL_DEVECTOR_HPP

#include <hboost/config.hpp>

#include <hboost/thread/csbl/vector.hpp>
#include <hboost/move/detail/move_helpers.hpp>

namespace hboost
{
  namespace csbl
  {
    template <class T>
    class devector
    {
      typedef csbl::vector<T> vector_type;
      vector_type data_;
      std::size_t front_index_;

      HBOOST_COPYABLE_AND_MOVABLE(devector)

      template <class U>
      void priv_push_back(HBOOST_FWD_REF(U) x)
      { data_.push_back(hboost::forward<U>(x)); }

    public:
      typedef typename vector_type::size_type size_type;
      typedef typename vector_type::reference reference;
      typedef typename vector_type::const_reference const_reference;


      devector() : front_index_(0) {}
      devector(devector const& x) HBOOST_NOEXCEPT
         :  data_(x.data_),
            front_index_(x.front_index_)
      {}
      devector(HBOOST_RV_REF(devector) x) HBOOST_NOEXCEPT
         :  data_(hboost::move(x.data_)),
            front_index_(x.front_index_)
      {}

      devector& operator=(HBOOST_COPY_ASSIGN_REF(devector) x)
      {
         if (&x != this)
         {
           data_ = x.data_;
           front_index_ = x.front_index_;
         }
         return *this;
      }

      devector& operator=(HBOOST_RV_REF(devector) x)
#if defined HBOOST_THREAD_USES_HBOOST_VECTOR
         HBOOST_NOEXCEPT_IF(vector_type::allocator_traits_type::propagate_on_container_move_assignment::value)
#endif
      {
        data_ = hboost::move(x.data_);
        front_index_ = x.front_index_;
        return *this;
      }

      bool empty() const HBOOST_NOEXCEPT
      { return data_.size() == front_index_; }

      size_type size() const HBOOST_NOEXCEPT
      { return data_.size() - front_index_; }

      reference         front() HBOOST_NOEXCEPT
      { return data_[front_index_]; }

      const_reference         front() const HBOOST_NOEXCEPT
      { return data_[front_index_]; }

      reference         back() HBOOST_NOEXCEPT
      { return data_.back(); }

      const_reference         back() const HBOOST_NOEXCEPT
      { return data_.back(); }

      HBOOST_MOVE_CONVERSION_AWARE_CATCH(push_back, T, void, priv_push_back)

      void pop_front()
      {
        ++front_index_;
        if (empty()) {
          data_.clear();
          front_index_=0;
        }
       }

    };
  }
}
#endif // header
