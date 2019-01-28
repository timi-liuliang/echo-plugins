/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// basic_text_iarchive.ipp:

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.
#include <string>
#include <algorithm>
#include <cstring>

#include <hboost/config.hpp>
#if defined(HBOOST_NO_STDC_NAMESPACE)
namespace std{ 
    using ::memcpy; 
}
#endif

#include <hboost/detail/workaround.hpp>
#include <hboost/serialization/string.hpp>
#include <hboost/archive/basic_text_iarchive.hpp>

namespace hboost {
namespace archive {

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// implementation of text_text_archive

template<class Archive>
HBOOST_ARCHIVE_OR_WARCHIVE_DECL void
basic_text_iarchive<Archive>::load_override(class_name_type & t){
    std::string cn;
    cn.reserve(HBOOST_SERIALIZATION_MAX_KEY_SIZE);
    load_override(cn);
    if(cn.size() > (HBOOST_SERIALIZATION_MAX_KEY_SIZE - 1))
        hboost::serialization::throw_exception(
            archive_exception(archive_exception::invalid_class_name)
        );
    std::memcpy(t, cn.data(), cn.size());
    // borland tweak
    t.t[cn.size()] = '\0';
}

template<class Archive>
HBOOST_ARCHIVE_OR_WARCHIVE_DECL void
basic_text_iarchive<Archive>::init(void){
    // read signature in an archive version independent manner
    std::string file_signature;
    * this->This() >> file_signature;
    if(file_signature != HBOOST_ARCHIVE_SIGNATURE())
        hboost::serialization::throw_exception(
            archive_exception(archive_exception::invalid_signature)
        );

    // make sure the version of the reading archive library can
    // support the format of the archive being read
    library_version_type input_library_version;
    * this->This() >> input_library_version;

    #if HBOOST_WORKAROUND(__MWERKS__, HBOOST_TESTED_AT(0x3205))
    this->set_library_version(input_library_version);
    #else
    detail::basic_iarchive::set_library_version(input_library_version);
    #endif

    // extra little .t is to get around borland quirk
    if(HBOOST_ARCHIVE_VERSION() < input_library_version)
        hboost::serialization::throw_exception(
            archive_exception(archive_exception::unsupported_version)
        );
}

} // namespace archive
} // namespace hboost
