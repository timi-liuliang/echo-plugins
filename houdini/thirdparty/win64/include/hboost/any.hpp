// See http://www.boost.org/libs/any for Documentation.

#ifndef HBOOST_ANY_INCLUDED
#define HBOOST_ANY_INCLUDED

#if defined(_MSC_VER)
# pragma once
#endif

// what:  variant type hboost::any
// who:   contributed by Kevlin Henney,
//        with features contributed and bugs found by
//        Antony Polukhin, Ed Brey, Mark Rodgers, 
//        Peter Dimov, and James Curran
// when:  July 2001, April 2013 - May 2013

#include <algorithm>

#include "hboost/config.hpp"
#include <hboost/type_index.hpp>
#include <hboost/type_traits/remove_reference.hpp>
#include <hboost/type_traits/decay.hpp>
#include <hboost/type_traits/remove_cv.hpp>
#include <hboost/type_traits/add_reference.hpp>
#include <hboost/type_traits/is_reference.hpp>
#include <hboost/type_traits/is_const.hpp>
#include <hboost/throw_exception.hpp>
#include <hboost/static_assert.hpp>
#include <hboost/utility/enable_if.hpp>
#include <hboost/type_traits/is_same.hpp>
#include <hboost/type_traits/is_const.hpp>
#include <hboost/mpl/if.hpp>

namespace hboost
{
    class any
    {
    public: // structors

        any() HBOOST_NOEXCEPT
          : content(0)
        {
        }

        template<typename ValueType>
        any(const ValueType & value)
          : content(new holder<
                HBOOST_DEDUCED_TYPENAME remove_cv<HBOOST_DEDUCED_TYPENAME decay<const ValueType>::type>::type
            >(value))
        {
        }

        any(const any & other)
          : content(other.content ? other.content->clone() : 0)
        {
        }

#ifndef HBOOST_NO_CXX11_RVALUE_REFERENCES
        // Move constructor
        any(any&& other) HBOOST_NOEXCEPT
          : content(other.content)
        {
            other.content = 0;
        }

        // Perfect forwarding of ValueType
        template<typename ValueType>
        any(ValueType&& value
            , typename hboost::disable_if<hboost::is_same<any&, ValueType> >::type* = 0 // disable if value has type `any&`
            , typename hboost::disable_if<hboost::is_const<ValueType> >::type* = 0) // disable if value has type `const ValueType&&`
          : content(new holder< typename decay<ValueType>::type >(static_cast<ValueType&&>(value)))
        {
        }
#endif

        ~any() HBOOST_NOEXCEPT
        {
            delete content;
        }

    public: // modifiers

        any & swap(any & rhs) HBOOST_NOEXCEPT
        {
            std::swap(content, rhs.content);
            return *this;
        }


#ifdef HBOOST_NO_CXX11_RVALUE_REFERENCES
        template<typename ValueType>
        any & operator=(const ValueType & rhs)
        {
            any(rhs).swap(*this);
            return *this;
        }

        any & operator=(any rhs)
        {
            any(rhs).swap(*this);
            return *this;
        }

#else 
        any & operator=(const any& rhs)
        {
            any(rhs).swap(*this);
            return *this;
        }

        // move assignement
        any & operator=(any&& rhs) HBOOST_NOEXCEPT
        {
            rhs.swap(*this);
            any().swap(rhs);
            return *this;
        }

        // Perfect forwarding of ValueType
        template <class ValueType>
        any & operator=(ValueType&& rhs)
        {
            any(static_cast<ValueType&&>(rhs)).swap(*this);
            return *this;
        }
#endif

    public: // queries

        bool empty() const HBOOST_NOEXCEPT
        {
            return !content;
        }

        void clear() HBOOST_NOEXCEPT
        {
            any().swap(*this);
        }

        const hboost::typeindex::type_info& type() const HBOOST_NOEXCEPT
        {
            return content ? content->type() : hboost::typeindex::type_id<void>().type_info();
        }

#ifndef HBOOST_NO_MEMBER_TEMPLATE_FRIENDS
    private: // types
#else
    public: // types (public so any_cast can be non-friend)
#endif

        class placeholder
        {
        public: // structors

            virtual ~placeholder()
            {
            }

        public: // queries

            virtual const hboost::typeindex::type_info& type() const HBOOST_NOEXCEPT = 0;

            virtual placeholder * clone() const = 0;

        };

        template<typename ValueType>
        class holder : public placeholder
        {
        public: // structors

            holder(const ValueType & value)
              : held(value)
            {
            }

#ifndef HBOOST_NO_CXX11_RVALUE_REFERENCES
            holder(ValueType&& value)
              : held(static_cast< ValueType&& >(value))
            {
            }
#endif
        public: // queries

            virtual const hboost::typeindex::type_info& type() const HBOOST_NOEXCEPT
            {
                return hboost::typeindex::type_id<ValueType>().type_info();
            }

            virtual placeholder * clone() const
            {
                return new holder(held);
            }

        public: // representation

            ValueType held;

        private: // intentionally left unimplemented
            holder & operator=(const holder &);
        };

#ifndef HBOOST_NO_MEMBER_TEMPLATE_FRIENDS

    private: // representation

        template<typename ValueType>
        friend ValueType * any_cast(any *) HBOOST_NOEXCEPT;

        template<typename ValueType>
        friend ValueType * unsafe_any_cast(any *) HBOOST_NOEXCEPT;

#else

    public: // representation (public so any_cast can be non-friend)

#endif

        placeholder * content;

    };
 
    inline void swap(any & lhs, any & rhs) HBOOST_NOEXCEPT
    {
        lhs.swap(rhs);
    }

    class HBOOST_SYMBOL_VISIBLE bad_any_cast :
#ifndef HBOOST_NO_RTTI
        public std::bad_cast
#else
        public std::exception
#endif
    {
    public:
        virtual const char * what() const HBOOST_NOEXCEPT_OR_NOTHROW
        {
            return "hboost::bad_any_cast: "
                   "failed conversion using hboost::any_cast";
        }
    };

    template<typename ValueType>
    ValueType * any_cast(any * operand) HBOOST_NOEXCEPT
    {
        return operand && operand->type() == hboost::typeindex::type_id<ValueType>()
            ? &static_cast<any::holder<HBOOST_DEDUCED_TYPENAME remove_cv<ValueType>::type> *>(operand->content)->held
            : 0;
    }

    template<typename ValueType>
    inline const ValueType * any_cast(const any * operand) HBOOST_NOEXCEPT
    {
        return any_cast<ValueType>(const_cast<any *>(operand));
    }

    template<typename ValueType>
    ValueType any_cast(any & operand)
    {
        typedef HBOOST_DEDUCED_TYPENAME remove_reference<ValueType>::type nonref;


        nonref * result = any_cast<nonref>(&operand);
        if(!result)
            hboost::throw_exception(bad_any_cast());

        // Attempt to avoid construction of a temporary object in cases when 
        // `ValueType` is not a reference. Example:
        // `static_cast<std::string>(*result);` 
        // which is equal to `std::string(*result);`
        typedef HBOOST_DEDUCED_TYPENAME hboost::mpl::if_<
            hboost::is_reference<ValueType>,
            ValueType,
            HBOOST_DEDUCED_TYPENAME hboost::add_reference<ValueType>::type
        >::type ref_type;

        return static_cast<ref_type>(*result);
    }

    template<typename ValueType>
    inline ValueType any_cast(const any & operand)
    {
        typedef HBOOST_DEDUCED_TYPENAME remove_reference<ValueType>::type nonref;
        return any_cast<const nonref &>(const_cast<any &>(operand));
    }

#ifndef HBOOST_NO_CXX11_RVALUE_REFERENCES
    template<typename ValueType>
    inline ValueType any_cast(any&& operand)
    {
        HBOOST_STATIC_ASSERT_MSG(
            hboost::is_rvalue_reference<ValueType&&>::value /*true if ValueType is rvalue or just a value*/
            || hboost::is_const< typename hboost::remove_reference<ValueType>::type >::value,
            "hboost::any_cast shall not be used for getting nonconst references to temporary objects" 
        );
        return any_cast<ValueType>(operand);
    }
#endif


    // Note: The "unsafe" versions of any_cast are not part of the
    // public interface and may be removed at any time. They are
    // required where we know what type is stored in the any and can't
    // use typeid() comparison, e.g., when our types may travel across
    // different shared libraries.
    template<typename ValueType>
    inline ValueType * unsafe_any_cast(any * operand) HBOOST_NOEXCEPT
    {
        return &static_cast<any::holder<ValueType> *>(operand->content)->held;
    }

    template<typename ValueType>
    inline const ValueType * unsafe_any_cast(const any * operand) HBOOST_NOEXCEPT
    {
        return unsafe_any_cast<ValueType>(const_cast<any *>(operand));
    }
}

// Copyright Kevlin Henney, 2000, 2001, 2002. All rights reserved.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#endif
