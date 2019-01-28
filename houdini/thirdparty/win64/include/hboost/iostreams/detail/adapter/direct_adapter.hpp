// (C) Copyright 2008 CodeRage, LLC (turkanis at coderage dot com)
// (C) Copyright 2003-2007 Jonathan Turkanis
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

#ifndef HBOOST_IOSTREAMS_DETAIL_DIRECT_ADAPTER_HPP_INCLUDED
#define HBOOST_IOSTREAMS_DETAIL_DIRECT_ADAPTER_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif              

#include <hboost/config.hpp>       // SFINAE, MSVC, put ptrdiff_t in std.
#include <algorithm>              // copy, min.
#include <cstddef>                // ptrdiff_t.
#include <hboost/detail/workaround.hpp>
#include <hboost/iostreams/categories.hpp>
#include <hboost/iostreams/detail/config/limits.hpp>        // forwarding.
#include <hboost/iostreams/detail/config/wide_streams.hpp>  // locale.
#include <hboost/iostreams/detail/double_object.hpp>
#include <hboost/iostreams/detail/error.hpp>
#include <hboost/iostreams/detail/ios.hpp>  // openmode, seekdir, int types.
#include <hboost/iostreams/traits.hpp>      // mode_of, is_direct.
#include <hboost/iostreams/operations.hpp>
#include <hboost/mpl/bool.hpp> 
#include <hboost/mpl/or.hpp> 
#include <hboost/preprocessor/iteration/local.hpp>
#include <hboost/preprocessor/repetition/enum_binary_params.hpp>
#include <hboost/preprocessor/repetition/enum_params.hpp>
#include <hboost/static_assert.hpp>
#include <hboost/throw_exception.hpp>
#include <hboost/type_traits/is_convertible.hpp>

// Must come last.
#include <hboost/iostreams/detail/config/disable_warnings.hpp> // VC7.1

namespace hboost { namespace iostreams { namespace detail {
                    
//------------------Definition of direct_adapter_base-------------------------//

// Put all initialization in base class to faciliate forwarding.
template<typename Direct>
class direct_adapter_base {
public:
    typedef typename char_type_of<Direct>::type  char_type;
    typedef typename mode_of<Direct>::type       mode_type;
    struct category 
        : mode_type,
          device_tag,
          closable_tag
          #ifndef HBOOST_IOSTREAMS_NO_LOCALE
          , localizable_tag
          #endif
        { };
protected:
    explicit direct_adapter_base(const Direct& d);
    typedef is_convertible<category, two_sequence> is_double;
    struct pointers {
        char_type *beg, *ptr, *end;
    };
    void init_input(mpl::true_);
    void init_input(mpl::false_) { }
    void init_output(mpl::true_);
    void init_output(mpl::false_) { }
    double_object<pointers, is_double>  ptrs_;
    Direct                              d_;
};

template<typename Direct>
class direct_adapter : private direct_adapter_base<Direct> {
private:
    typedef direct_adapter_base<Direct>      base_type;
    typedef typename base_type::pointers     pointers;
    typedef typename base_type::is_double    is_double;
    using base_type::ptrs_;
    using base_type::d_;
public:
    typedef typename base_type::char_type    char_type;
    typedef typename base_type::category     category;

        // Constructors

#if !HBOOST_WORKAROUND(HBOOST_MSVC, <= 1310)
    direct_adapter(const Direct& d) : base_type(d) { }   
    direct_adapter(const direct_adapter& d) : base_type(d) { }
# define HBOOST_PP_LOCAL_LIMITS (1, HBOOST_IOSTREAMS_MAX_FORWARDING_ARITY)
#else
    template<typename U>
    struct is_direct
        : mpl::or_< 
              is_same<U, direct_adapter<Direct> >, 
              is_same<U, Direct> 
          >
        { };
    template<typename U>
    direct_adapter(const U& u) 
        : base_type(forward(u, is_direct<U>()))
        { }
# define HBOOST_PP_LOCAL_LIMITS (2, HBOOST_IOSTREAMS_MAX_FORWARDING_ARITY)
#endif

#define HBOOST_PP_LOCAL_MACRO(n) \
    template<HBOOST_PP_ENUM_PARAMS(n, typename P)> \
    direct_adapter(HBOOST_PP_ENUM_BINARY_PARAMS(n, const P, &p)) \
        : base_type(Direct(HBOOST_PP_ENUM_PARAMS(n, p))) \
        { } \
    /**/
#include HBOOST_PP_LOCAL_ITERATE()
#undef HBOOST_PP_LOCAL_MACRO

        // Device interface.

    std::streamsize read(char_type* s, std::streamsize n);
    std::streamsize write(const char_type* s, std::streamsize n);
    std::streampos seek( stream_offset, HBOOST_IOS::seekdir,
                         HBOOST_IOS::openmode = HBOOST_IOS::in | HBOOST_IOS::out );
    void close();
    void close(HBOOST_IOS::openmode which);
#ifndef HBOOST_IOSTREAMS_NO_LOCALE
    void imbue(const std::locale&);
#endif

        // Direct device access.

    Direct& operator*() { return d_; }
    Direct* operator->() { return &d_; }
#if HBOOST_WORKAROUND(HBOOST_MSVC, <= 1310)
private:
    template<typename U>
    static Direct forward(const U& u, mpl::true_) { return u; }
    template<typename U>
    static Direct forward(const U& u, mpl::false_) { return Direct(u); }
#endif
};

//--------------Definition of wrap_direct and unwrap_direct-------------------//

template<typename Device>
struct wrap_direct_traits 
    : mpl::if_<
          is_direct<Device>,
          direct_adapter<Device>,
          Device
      >
    { };

template<typename Device>
typename wrap_direct_traits<Device>::type
inline wrap_direct(Device dev) 
{ 
    typedef typename wrap_direct_traits<Device>::type type;
    return type(dev); 
}

template<typename Device>
inline Device& unwrap_direct(Device& d) { return d; }  

template<typename Device>
inline Device& unwrap_direct(direct_adapter<Device>& d) { return *d; }  

//--------------Implementation of direct_adapter_base-------------------------//

template<typename Direct>
direct_adapter_base<Direct>::direct_adapter_base(const Direct& d) : d_(d)
{
    init_input(is_convertible<category, input>());
    init_output(is_convertible<category, output>());
}

template<typename Direct>
void direct_adapter_base<Direct>::init_input(mpl::true_) 
{
    std::pair<char_type*, char_type*> seq = iostreams::input_sequence(d_);
    ptrs_.first().beg = seq.first;
    ptrs_.first().ptr = seq.first;
    ptrs_.first().end = seq.second;
}

template<typename Direct>
void direct_adapter_base<Direct>::init_output(mpl::true_) 
{
    std::pair<char_type*, char_type*> seq = iostreams::output_sequence(d_);
    ptrs_.second().beg = seq.first;
    ptrs_.second().ptr = seq.first;
    ptrs_.second().end = seq.second;
}

//--------------Implementation of direct_adapter------------------------------//

template<typename Direct>
inline std::streamsize direct_adapter<Direct>::read
    (char_type* s, std::streamsize n)
{
    using namespace std;
    pointers& get = ptrs_.first();
    std::streamsize avail = 
        static_cast<std::streamsize>(get.end - get.ptr);
    std::streamsize result = (std::min)(n, avail);
    std::copy(get.ptr, get.ptr + result, s);
    get.ptr += result;
    return result != 0 ? result : -1;
}

template<typename Direct>
inline std::streamsize direct_adapter<Direct>::write
    (const char_type* s, std::streamsize n)
{
    using namespace std;
    pointers& put = ptrs_.second();
    if (n > static_cast<std::streamsize>(put.end - put.ptr))
        hboost::throw_exception(write_area_exhausted());
    std::copy(s, s + n, put.ptr);
    put.ptr += n;
    return n;
}

template<typename Direct>
inline std::streampos direct_adapter<Direct>::seek
    ( stream_offset off, HBOOST_IOS::seekdir way, 
      HBOOST_IOS::openmode which )
{
    using namespace std;
    pointers& get = ptrs_.first();
    pointers& put = ptrs_.second();
    if (way == HBOOST_IOS::cur && get.ptr != put.ptr)
       hboost::throw_exception(bad_seek());
    ptrdiff_t next = 0;
    if ((which & HBOOST_IOS::in) || !is_double::value) {
        if (way == HBOOST_IOS::beg)
            next = off; 
        else if (way == HBOOST_IOS::cur)
            next = get.ptr - get.beg + off; 
        else
            next = get.end - get.beg + off; 
        if (next >= 0 && next <= get.end - get.beg)
            get.ptr = get.beg + next;
        else
            hboost::throw_exception(bad_seek());
    }
    if ((which & HBOOST_IOS::out) && is_double::value) {
        if (way == HBOOST_IOS::beg)
            next = off; 
        else if (way == HBOOST_IOS::cur)
            next = put.ptr - put.beg + off; 
        else
            next = put.end - put.beg + off; 
        if (next >= 0 && next <= put.end - put.beg)
            put.ptr = put.beg + next;
        else
            hboost::throw_exception(bad_seek());
    }
    return offset_to_position(next);
}

template<typename Direct>
void direct_adapter<Direct>::close() 
{ 
    HBOOST_STATIC_ASSERT((!is_convertible<category, two_sequence>::value));
    detail::close_all(d_);
}

template<typename Direct>
void direct_adapter<Direct>::close(HBOOST_IOS::openmode which) 
{ 
    HBOOST_STATIC_ASSERT((is_convertible<category, two_sequence>::value));
    hboost::iostreams::close(d_, which);
}

#ifndef HBOOST_IOSTREAMS_NO_LOCALE
    template<typename Direct>
    void direct_adapter<Direct>::imbue(const std::locale& loc) 
    { hboost::iostreams::imbue(d_, loc); }
#endif

} } } // End namespaces detail, iostreams, boost.

#include <hboost/iostreams/detail/config/enable_warnings.hpp>

#endif // #ifndef HBOOST_IOSTREAMS_DETAIL_DIRECT_ADAPTER_HPP_INCLUDED
