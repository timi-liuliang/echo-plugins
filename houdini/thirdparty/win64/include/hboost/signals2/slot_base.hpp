// Boost.Signals2 library

// Copyright Frank Mori Hess 2007-2008.
// Copyright Timmo Stange 2007.
// Copyright Douglas Gregor 2001-2004. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#ifndef HBOOST_SIGNALS2_SLOT_BASE_HPP
#define HBOOST_SIGNALS2_SLOT_BASE_HPP

#include <hboost/shared_ptr.hpp>
#include <hboost/weak_ptr.hpp>
#include <hboost/signals2/detail/foreign_ptr.hpp>
#include <hboost/signals2/expired_slot.hpp>
#include <hboost/signals2/signal_base.hpp>
#include <hboost/variant/apply_visitor.hpp>
#include <hboost/variant/variant.hpp>
#include <vector>

namespace hboost
{
  namespace signals2
  {
    namespace detail
    {
      class tracked_objects_visitor;
      class trackable_pointee;

      typedef hboost::variant<hboost::weak_ptr<trackable_pointee>, hboost::weak_ptr<void>, detail::foreign_void_weak_ptr > void_weak_ptr_variant;
      typedef hboost::variant<hboost::shared_ptr<void>, detail::foreign_void_shared_ptr > void_shared_ptr_variant;
      class lock_weak_ptr_visitor
      {
      public:
        typedef void_shared_ptr_variant result_type;
        template<typename WeakPtr>
        result_type operator()(const WeakPtr &wp) const
        {
          return wp.lock();
        }
        // overload to prevent incrementing use count of shared_ptr associated
        // with signals2::trackable objects
        result_type operator()(const weak_ptr<trackable_pointee> &) const
        {
          return hboost::shared_ptr<void>();
        }
      };
      class expired_weak_ptr_visitor
      {
      public:
        typedef bool result_type;
        template<typename WeakPtr>
        bool operator()(const WeakPtr &wp) const
        {
          return wp.expired();
        }
      };
    }

    class slot_base
    {
    public:
      typedef std::vector<detail::void_weak_ptr_variant> tracked_container_type;
      typedef std::vector<detail::void_shared_ptr_variant> locked_container_type;

      const tracked_container_type& tracked_objects() const {return _tracked_objects;}
      locked_container_type lock() const
      {
        locked_container_type locked_objects;
        tracked_container_type::const_iterator it;
        for(it = tracked_objects().begin(); it != tracked_objects().end(); ++it)
        {
          locked_objects.push_back(apply_visitor(detail::lock_weak_ptr_visitor(), *it));
          if(apply_visitor(detail::expired_weak_ptr_visitor(), *it))
          {
            throw expired_slot();
          }
        }
        return locked_objects;
      }
      bool expired() const
      {
        tracked_container_type::const_iterator it;
        for(it = tracked_objects().begin(); it != tracked_objects().end(); ++it)
        {
          if(apply_visitor(detail::expired_weak_ptr_visitor(), *it)) return true;
        }
        return false;
      }
    protected:
      friend class detail::tracked_objects_visitor;

      void track_signal(const signal_base &signal)
      {
        _tracked_objects.push_back(signal.lock_pimpl());
      }

      tracked_container_type _tracked_objects;
    };
  }
} // end namespace hboost

#endif // HBOOST_SIGNALS2_SLOT_BASE_HPP
