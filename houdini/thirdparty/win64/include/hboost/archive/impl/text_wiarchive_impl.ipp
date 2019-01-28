/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// text_text_wiarchive_impl.ipp:

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <cstddef> // size_t, NULL

#include <hboost/config.hpp>
#if defined(HBOOST_NO_STDC_NAMESPACE)
namespace std{ 
    using ::size_t; 
} // namespace std
#endif

#include <hboost/detail/workaround.hpp>  // fixup for RogueWave

#ifndef HBOOST_NO_STD_WSTREAMBUF
#include <hboost/archive/basic_text_iprimitive.hpp>

namespace hboost { 
namespace archive {

//////////////////////////////////////////////////////////////////////
// implementation of wiprimtives functions
//
template<class Archive>
HBOOST_WARCHIVE_DECL void
text_wiarchive_impl<Archive>::load(char *s)
{
    std::size_t size;
    * this->This() >> size;
    // skip separating space
    is.get();
    while(size-- > 0){
        *s++ = is.narrow(is.get(), '\0');
    }
    *s = '\0';
}

template<class Archive>
HBOOST_WARCHIVE_DECL void
text_wiarchive_impl<Archive>::load(std::string &s)
{
    std::size_t size;
    * this->This() >> size;
    // skip separating space
    is.get();
    #if HBOOST_WORKAROUND(_RWSTD_VER, HBOOST_TESTED_AT(20101))
    if(NULL != s.data())
    #endif
        s.resize(0);
    s.reserve(size);
    while(size-- > 0){
        int x = is.narrow(is.get(), '\0');
        s += x;
    }
}

#ifndef HBOOST_NO_INTRINSIC_WCHAR_T
template<class Archive>
HBOOST_WARCHIVE_DECL void
text_wiarchive_impl<Archive>::load(wchar_t *s)
{
    std::size_t size;
    * this->This() >> size;
    // skip separating space
    is.get();
    // Works on all tested platforms
    is.read(s, size);
    s[size] = L'\0';
}
#endif

#ifndef HBOOST_NO_STD_WSTRING
template<class Archive>
HBOOST_WARCHIVE_DECL void
text_wiarchive_impl<Archive>::load(std::wstring &ws)
{
    std::size_t size;
    * this->This() >> size;
    // skip separating space
    is.get();
    // borland complains about resize
    // borland de-allocator fixup
    #if HBOOST_WORKAROUND(_RWSTD_VER, HBOOST_TESTED_AT(20101))
    if(NULL != ws.data())
    #endif
        ws.resize(size);
    // note breaking a rule here - is this a problem on some platform
    is.read(const_cast<wchar_t *>(ws.data()), size);
}
#endif

template<class Archive>
HBOOST_WARCHIVE_DECL 
text_wiarchive_impl<Archive>::text_wiarchive_impl(
    std::wistream & is, 
    unsigned int flags
) :
    basic_text_iprimitive<std::wistream>(
        is, 
        0 != (flags & no_codecvt)
    ),
    basic_text_iarchive<Archive>(flags)
{
    if(0 == (flags & no_header))
        basic_text_iarchive<Archive>::init();
}

} // archive
} // boost

#endif // HBOOST_NO_STD_WSTREAMBUF
