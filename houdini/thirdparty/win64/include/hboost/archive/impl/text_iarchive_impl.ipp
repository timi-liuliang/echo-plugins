/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// text_iarchive_impl.ipp:

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

//////////////////////////////////////////////////////////////////////
// implementation of basic_text_iprimitive overrides for the combination
// of template parameters used to implement a text_iprimitive

#include <cstddef> // size_t, NULL
#include <hboost/config.hpp>
#if defined(HBOOST_NO_STDC_NAMESPACE)
namespace std{ 
    using ::size_t; 
} // namespace std
#endif

#include <hboost/detail/workaround.hpp> // RogueWave

#include <hboost/archive/text_iarchive.hpp>

namespace hboost {
namespace archive {

template<class Archive>
HBOOST_ARCHIVE_DECL void
text_iarchive_impl<Archive>::load(char *s)
{
    std::size_t size;
    * this->This() >> size;
    // skip separating space
    is.get();
    // Works on all tested platforms
    is.read(s, size);
    s[size] = '\0';
}

template<class Archive>
HBOOST_ARCHIVE_DECL void
text_iarchive_impl<Archive>::load(std::string &s)
{
    std::size_t size;
    * this->This() >> size;
    // skip separating space
    is.get();
    // borland de-allocator fixup
    #if HBOOST_WORKAROUND(_RWSTD_VER, HBOOST_TESTED_AT(20101))
    if(NULL != s.data())
    #endif
        s.resize(size);
    if(0 < size)
        is.read(&(*s.begin()), size);
}

#ifndef HBOOST_NO_CWCHAR
#ifndef HBOOST_NO_INTRINSIC_WCHAR_T
template<class Archive>
HBOOST_ARCHIVE_DECL void
text_iarchive_impl<Archive>::load(wchar_t *ws)
{
    std::size_t size;
    * this->This() >> size;
    // skip separating space
    is.get();
    is.read((char *)ws, size * sizeof(wchar_t)/sizeof(char));
    ws[size] = L'\0';
}
#endif // HBOOST_NO_INTRINSIC_WCHAR_T

#ifndef HBOOST_NO_STD_WSTRING
template<class Archive>
HBOOST_ARCHIVE_DECL void
text_iarchive_impl<Archive>::load(std::wstring &ws)
{
    std::size_t size;
    * this->This() >> size;
    // borland de-allocator fixup
    #if HBOOST_WORKAROUND(_RWSTD_VER, HBOOST_TESTED_AT(20101))
    if(NULL != ws.data())
    #endif
        ws.resize(size);
    // skip separating space
    is.get();
    is.read((char *)ws.data(), size * sizeof(wchar_t)/sizeof(char));
}

#endif // HBOOST_NO_STD_WSTRING
#endif // HBOOST_NO_CWCHAR

template<class Archive>
HBOOST_ARCHIVE_DECL void
text_iarchive_impl<Archive>::load_override(class_name_type & t){
    basic_text_iarchive<Archive>::load_override(t);
}

template<class Archive>
HBOOST_ARCHIVE_DECL void
text_iarchive_impl<Archive>::init(){
    basic_text_iarchive<Archive>::init();
}

template<class Archive>
HBOOST_ARCHIVE_DECL 
text_iarchive_impl<Archive>::text_iarchive_impl(
    std::istream & is, 
    unsigned int flags
) :
    basic_text_iprimitive<std::istream>(
        is, 
        0 != (flags & no_codecvt)
    ),
    basic_text_iarchive<Archive>(flags)
{
    if(0 == (flags & no_header))
        #if HBOOST_WORKAROUND(__MWERKS__, HBOOST_TESTED_AT(0x3205))
        this->init();
        #else
        this->basic_text_iarchive<Archive>::init();
        #endif
}

} // namespace archive
} // namespace hboost