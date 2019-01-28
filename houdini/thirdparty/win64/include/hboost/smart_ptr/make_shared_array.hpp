/*
 * Copyright (c) 2012-2014 Glen Joseph Fernandes
 * glenfe at live dot com
 *
 * Distributed under the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt
 * or copy at http://boost.org/LICENSE_1_0.txt)
 */
#ifndef HBOOST_SMART_PTR_MAKE_SHARED_ARRAY_HPP
#define HBOOST_SMART_PTR_MAKE_SHARED_ARRAY_HPP

#include <hboost/smart_ptr/detail/array_count_impl.hpp>
#include <hboost/smart_ptr/detail/sp_if_array.hpp>

namespace hboost {
    template<class T>
    inline typename hboost::detail::sp_if_array<T>::type
    make_shared(std::size_t size) {
        typedef typename hboost::detail::array_inner<T>::type T1;
        typedef typename hboost::detail::array_base<T1>::type T2;
        typedef hboost::detail::ms_allocator<T> A1;
        typedef hboost::detail::ms_in_allocator_tag D1;
        std::size_t n1 = size * hboost::detail::array_total<T1>::size;
        T1* p1 = 0;
        T2* p2 = 0;
        D1 d1;
        A1 a1(size, &p2);
        shared_ptr<T> s1(p1, d1, a1);
        A1* a2 = static_cast<A1*>(s1._internal_get_untyped_deleter());
        a2->set(0);
        hboost::detail::ms_init(p2, n1);
        a2->set(p2);
        p1 = reinterpret_cast<T1*>(p2);
        return shared_ptr<T>(s1, p1);
    }

    template<class T>
    inline typename hboost::detail::sp_if_size_array<T>::type
    make_shared() {
        typedef typename hboost::detail::array_inner<T>::type T1;
        typedef typename hboost::detail::array_base<T1>::type T2;
        typedef hboost::detail::ms_allocator<T> A1;
        typedef hboost::detail::ms_in_allocator_tag D1;
        enum {
            N = hboost::detail::array_total<T>::size
        };
        T1* p1 = 0;
        T2* p2 = 0;
        D1 d1;
        A1 a1(&p2);
        shared_ptr<T> s1(p1, d1, a1);
        A1* a2 = static_cast<A1*>(s1._internal_get_untyped_deleter());
        a2->set(0);
        hboost::detail::ms_init(p2, N);
        a2->set(p2);
        p1 = reinterpret_cast<T1*>(p2);
        return shared_ptr<T>(s1, p1);
    }

    template<class T>
    inline typename hboost::detail::sp_if_array<T>::type
    make_shared(std::size_t size,
        const typename hboost::detail::array_inner<T>::type& value) {
        typedef typename hboost::detail::array_inner<T>::type T1;
        typedef typename hboost::detail::array_base<T1>::type T2;
        typedef const T2 T3;
        typedef hboost::detail::ms_allocator<T> A1;
        typedef hboost::detail::ms_in_allocator_tag D1;
        enum {
            M = hboost::detail::array_total<T1>::size
        };
        std::size_t n1 = M * size;
        T1* p1 = 0;
        T2* p2 = 0;
        T3* p3 = reinterpret_cast<T3*>(&value);
        D1 d1;
        A1 a1(size, &p2);
        shared_ptr<T> s1(p1, d1, a1);
        A1* a2 = static_cast<A1*>(s1._internal_get_untyped_deleter());
        a2->set(0);
        hboost::detail::ms_init<T2, M>(p2, n1, p3);
        a2->set(p2);
        p1 = reinterpret_cast<T1*>(p2);
        return shared_ptr<T>(s1, p1);
    }

    template<class T>
    inline typename hboost::detail::sp_if_size_array<T>::type
    make_shared(const typename hboost::detail::array_inner<T>::type& value) {
        typedef typename hboost::detail::array_inner<T>::type T1;
        typedef typename hboost::detail::array_base<T1>::type T2;
        typedef const T2 T3;
        typedef hboost::detail::ms_allocator<T> A1;
        typedef hboost::detail::ms_in_allocator_tag D1;
        enum {
            M = hboost::detail::array_total<T1>::size,
            N = hboost::detail::array_total<T>::size
        };
        T1* p1 = 0;
        T2* p2 = 0;
        T3* p3 = reinterpret_cast<T3*>(&value);
        D1 d1;
        A1 a1(&p2);
        shared_ptr<T> s1(p1, d1, a1);
        A1* a2 = static_cast<A1*>(s1._internal_get_untyped_deleter());
        a2->set(0);
        hboost::detail::ms_init<T2, M>(p2, N, p3);
        a2->set(p2);
        p1 = reinterpret_cast<T1*>(p2);
        return shared_ptr<T>(s1, p1);
    }

    template<class T>
    inline typename hboost::detail::sp_if_array<T>::type
    make_shared_noinit(std::size_t size) {
        typedef typename hboost::detail::array_inner<T>::type T1;
        typedef typename hboost::detail::array_base<T1>::type T2;
        typedef hboost::detail::ms_allocator<T> A1;
        typedef hboost::detail::ms_in_allocator_tag D1;
        std::size_t n1 = size * hboost::detail::array_total<T1>::size;
        T1* p1 = 0;
        T2* p2 = 0;
        D1 d1;
        A1 a1(size, &p2);
        shared_ptr<T> s1(p1, d1, a1);
        A1* a2 = static_cast<A1*>(s1._internal_get_untyped_deleter());
        a2->set(0);
        hboost::detail::ms_noinit(p2, n1);
        a2->set(p2);
        p1 = reinterpret_cast<T1*>(p2);
        return shared_ptr<T>(s1, p1);
    }

    template<class T>
    inline typename hboost::detail::sp_if_size_array<T>::type
    make_shared_noinit() {
        typedef typename hboost::detail::array_inner<T>::type T1;
        typedef typename hboost::detail::array_base<T1>::type T2;
        typedef hboost::detail::ms_allocator<T> A1;
        typedef hboost::detail::ms_in_allocator_tag D1;
        enum {
            N = hboost::detail::array_total<T>::size
        };
        T1* p1 = 0;
        T2* p2 = 0;
        D1 d1;
        A1 a1(&p2);
        shared_ptr<T> s1(p1, d1, a1);
        A1* a2 = static_cast<A1*>(s1._internal_get_untyped_deleter());
        a2->set(0);
        hboost::detail::ms_noinit(p2, N);
        a2->set(p2);
        p1 = reinterpret_cast<T1*>(p2);
        return shared_ptr<T>(s1, p1);
    }
}

#endif