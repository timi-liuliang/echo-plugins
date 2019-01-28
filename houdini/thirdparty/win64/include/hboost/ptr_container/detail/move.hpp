// (C) Copyright Daniel Wallin 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Contains the definitions of the class template move_source and the function
// template move, which together make move pointers moveable.

#ifndef HBOOST_MOVE_HPP_INCLUDED
#define HBOOST_MOVE_HPP_INCLUDED

namespace hboost { namespace ptr_container_detail {

namespace move_ptrs {

#if HBOOST_WORKAROUND(HBOOST_MSVC, >= 1400)    
#pragma warning(push)    
#pragma warning(disable:4512)  
#endif  
    
template<typename Ptr>
class move_source {
public:
    move_source(Ptr& ptr) : ptr_(ptr) {}
    Ptr& ptr() const { return ptr_; }
private:
    Ptr& ptr_;
    move_source(const Ptr&);
};

#if HBOOST_WORKAROUND(HBOOST_MSVC, >= 1400)    
#pragma warning(pop)    
#endif  

} // End namespace move_ptrs.


template<typename T>
move_ptrs::move_source<T> move(T& x) 
{ return move_ptrs::move_source<T>(x); }

} // namespace 'ptr_container_detail'
} // End namespace hboost.

#endif // #ifndef HBOOST_MOVE_HPP_INCLUDED
