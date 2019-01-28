// Copyright (C) 2015-2016 Andrzej Krzemienski.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/optional for documentation.
//
// You are welcome to contact the author at:
//  akrzemi1@gmail.com

#ifndef HBOOST_OPTIONAL_DETAIL_OPTIONAL_REFERENCE_SPEC_AJK_03OCT2015_HPP
#define HBOOST_OPTIONAL_DETAIL_OPTIONAL_REFERENCE_SPEC_AJK_03OCT2015_HPP

#ifdef HBOOST_OPTIONAL_CONFIG_NO_PROPER_ASSIGN_FROM_CONST_INT
#include <hboost/type_traits/is_integral.hpp>
#include <hboost/type_traits/is_const.hpp>
#endif

# if 1

namespace hboost {

namespace detail {

#ifndef HBOOST_OPTIONAL_DETAIL_NO_RVALUE_REFERENCES

template <class From>
void prevent_binding_rvalue()
{
#ifndef HBOOST_OPTIONAL_CONFIG_ALLOW_BINDING_TO_RVALUES
    HBOOST_STATIC_ASSERT_MSG(hboost::is_lvalue_reference<From>::value, 
                            "binding rvalue references to optional lvalue references is disallowed");
#endif    
}

template <class T>
HBOOST_DEDUCED_TYPENAME hboost::remove_reference<T>::type& forward_reference(T&& r)
{
    HBOOST_STATIC_ASSERT_MSG(hboost::is_lvalue_reference<T>::value, 
                            "binding rvalue references to optional lvalue references is disallowed");
    return hboost::forward<T>(r);
}

#endif // HBOOST_OPTIONAL_DETAIL_NO_RVALUE_REFERENCES

template <class From>
void prevent_assignment_from_false_const_integral()
{
#ifndef HBOOST_OPTIONAL_CONFIG_ALLOW_BINDING_TO_RVALUES
#ifdef HBOOST_OPTIONAL_CONFIG_NO_PROPER_ASSIGN_FROM_CONST_INT
    // MSVC compiler without rvalue refernces: we need to disable the asignment from
    // const integral lvalue reference, as it may be an invalid temporary
    HBOOST_STATIC_ASSERT_MSG(!(hboost::is_const<From>::value && hboost::is_integral<From>::value), 
                            "binding const lvalue references to integral types is disabled in this compiler");
#endif
#endif   
}

template <class T>
struct is_optional_
{
  static const bool value = false;
};

template <class U>
struct is_optional_< ::hboost::optional<U> >
{
  static const bool value = true;
};

template <class T>
struct is_no_optional
{
  static const bool value = !is_optional_<HBOOST_DEDUCED_TYPENAME hboost::decay<T>::type>::value;
};

} // namespace detail

template <class T>
class optional<T&> : public optional_detail::optional_tag
{
    T* ptr_;
    
public:
    typedef T& value_type;
    typedef T& reference_type;
    typedef T& reference_const_type;
    typedef T& rval_reference_type;
    typedef T* pointer_type;
    typedef T* pointer_const_type;
    
    optional() HBOOST_NOEXCEPT : ptr_() {}
    optional(none_t) HBOOST_NOEXCEPT : ptr_() {}  

    template <class U>
        explicit optional(const optional<U&>& rhs) HBOOST_NOEXCEPT : ptr_(rhs.ptr_) {}
    optional(const optional& rhs) HBOOST_NOEXCEPT : ptr_(rhs.ptr_) {}
    

    optional& operator=(const optional& rhs) HBOOST_NOEXCEPT { ptr_ = rhs.ptr_; return *this; }
    template <class U>
        optional& operator=(const optional<U&>& rhs) HBOOST_NOEXCEPT { ptr_ = rhs.ptr_; return *this; }
    optional& operator=(none_t) HBOOST_NOEXCEPT { ptr_ = 0; return *this; }
    
    
    void swap(optional& rhs) HBOOST_NOEXCEPT { std::swap(ptr_, rhs.ptr_); }
    T& get() const { HBOOST_ASSERT(ptr_); return   *ptr_; }

    T* get_ptr() const HBOOST_NOEXCEPT { return ptr_; }
    T* operator->() const { HBOOST_ASSERT(ptr_); return ptr_; }
    T& operator*() const { HBOOST_ASSERT(ptr_); return *ptr_; }
    T& value() const { return ptr_ ? *ptr_ : (throw_exception(bad_optional_access()), *ptr_); }
    
    bool operator!() const HBOOST_NOEXCEPT { return ptr_ == 0; }  
    HBOOST_EXPLICIT_OPERATOR_BOOL_NOEXCEPT()
      
    void reset() HBOOST_NOEXCEPT { ptr_ = 0; }

    bool is_initialized() const HBOOST_NOEXCEPT { return ptr_ != 0; }
    
#ifndef HBOOST_OPTIONAL_DETAIL_NO_RVALUE_REFERENCES   
 
    template <class R>
        optional(R&& r, HBOOST_DEDUCED_TYPENAME hboost::enable_if<detail::is_no_optional<R> >::type* = 0) HBOOST_NOEXCEPT
        : ptr_(hboost::addressof(r)) { detail::prevent_binding_rvalue<R>(); }
        
    template <class R>
        optional(bool cond, R&& r, HBOOST_DEDUCED_TYPENAME hboost::enable_if<detail::is_no_optional<R> >::type* = 0) HBOOST_NOEXCEPT
        : ptr_(cond ? hboost::addressof(r) : 0) { detail::prevent_binding_rvalue<R>(); }
        
    template <class R>
        HBOOST_DEDUCED_TYPENAME hboost::enable_if<detail::is_no_optional<R>, optional<T&>&>::type
        operator=(R&& r) HBOOST_NOEXCEPT { detail::prevent_binding_rvalue<R>(); ptr_ = hboost::addressof(r); return *this; }
        
    template <class R>
        void emplace(R&& r, HBOOST_DEDUCED_TYPENAME hboost::enable_if<detail::is_no_optional<R> >::type* = 0) HBOOST_NOEXCEPT
        { detail::prevent_binding_rvalue<R>(); ptr_ = hboost::addressof(r); }
        
    template <class R>
      T& get_value_or(R&& r, HBOOST_DEDUCED_TYPENAME hboost::enable_if<detail::is_no_optional<R> >::type* = 0) const HBOOST_NOEXCEPT
      { detail::prevent_binding_rvalue<R>(); return ptr_ ? *ptr_ : r; }
      
    template <class R>
        T& value_or(R&& r, HBOOST_DEDUCED_TYPENAME hboost::enable_if<detail::is_no_optional<R> >::type* = 0) const HBOOST_NOEXCEPT
        { detail::prevent_binding_rvalue<R>(); return ptr_ ? *ptr_ : r; }
        
    template <class R>
      void reset(R&& r, HBOOST_DEDUCED_TYPENAME hboost::enable_if<detail::is_no_optional<R> >::type* = 0) HBOOST_NOEXCEPT
      { detail::prevent_binding_rvalue<R>(); ptr_ = hboost::addressof(r); }
      
    template <class F>
        T& value_or_eval(F f) const { return ptr_ ? *ptr_ : detail::forward_reference(f()); }
      
#else  // HBOOST_OPTIONAL_DETAIL_NO_RVALUE_REFERENCES

    template <class U>
        optional(U& v, HBOOST_DEDUCED_TYPENAME hboost::enable_if<detail::is_no_optional<U> >::type* = 0) HBOOST_NOEXCEPT : ptr_(hboost::addressof(v)) { }
        
    template <class U>
        optional(bool cond, U& v, HBOOST_DEDUCED_TYPENAME hboost::enable_if<detail::is_no_optional<U> >::type* = 0) HBOOST_NOEXCEPT : ptr_(cond ? hboost::addressof(v) : 0) {}

    template <class U>
        HBOOST_DEDUCED_TYPENAME hboost::enable_if<detail::is_no_optional<U>, optional<T&>&>::type
        operator=(U& v) HBOOST_NOEXCEPT
        {
            detail::prevent_assignment_from_false_const_integral<U>();
            ptr_ = hboost::addressof(v); return *this;
        }

    template <class U>
        void emplace(U& v, HBOOST_DEDUCED_TYPENAME hboost::enable_if<detail::is_no_optional<U> >::type* = 0) HBOOST_NOEXCEPT
        { ptr_ = hboost::addressof(v); }
        
    template <class U>
      T& get_value_or(U& v, HBOOST_DEDUCED_TYPENAME hboost::enable_if<detail::is_no_optional<U> >::type* = 0) const HBOOST_NOEXCEPT
      { return ptr_ ? *ptr_ : v; }
      
    template <class U>
        T& value_or(U& v, HBOOST_DEDUCED_TYPENAME hboost::enable_if<detail::is_no_optional<U> >::type* = 0) const HBOOST_NOEXCEPT
        { return ptr_ ? *ptr_ : v; }
        
    template <class U>
      void reset(U& v, HBOOST_DEDUCED_TYPENAME hboost::enable_if<detail::is_no_optional<U> >::type* = 0) HBOOST_NOEXCEPT
      { ptr_ = hboost::addressof(v); }
      
    template <class F>
      T& value_or_eval(F f) const { return ptr_ ? *ptr_ : f(); }
      
#endif // HBOOST_OPTIONAL_DETAIL_NO_RVALUE_REFERENCES
};

template <class T> 
  void swap ( optional<T&>& x, optional<T&>& y) HBOOST_NOEXCEPT
{
  x.swap(y);
}

} // namespace hboost

#endif // 1/0

#endif // header guard
