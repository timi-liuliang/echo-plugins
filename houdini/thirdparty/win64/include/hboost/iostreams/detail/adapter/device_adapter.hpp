/*
 * Defines the class template hboost::iostreams::detail::device_adapter,
 * a convenience base class for device adapters.
 *
 * File:        hboost/iostreams/detail/adapter/filter_adapter.hpp
 * Date:        Mon Nov 26 14:35:48 MST 2007
 * 
 * Copyright:   2007-2008 CodeRage, LLC
 * Author:      Jonathan Turkanis
 * Contact:     turkanis at coderage dot com
 *
 * Distributed under the Boost Software License, Version 1.0.(See accompanying 
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)
 * 
 * See http://www.boost.org/libs/iostreams for documentation.
 */

#ifndef HBOOST_IOSTREAMS_DETAIL_DEVICE_ADAPTER_HPP_INCLUDED
#define HBOOST_IOSTREAMS_DETAIL_DEVICE_ADAPTER_HPP_INCLUDED

#include <hboost/iostreams/categories.hpp>
#include <hboost/iostreams/detail/call_traits.hpp>
#include <hboost/iostreams/detail/ios.hpp>
#include <hboost/iostreams/operations.hpp>
#include <hboost/iostreams/traits.hpp>
#include <hboost/static_assert.hpp>

namespace hboost { namespace iostreams { namespace detail {

template<typename T>
class device_adapter {
private:
    typedef typename detail::value_type<T>::type value_type;
    typedef typename detail::param_type<T>::type param_type;
public:
    explicit device_adapter(param_type t) : t_(t) { }
    T& component() { return t_; }

    void close() 
    {
        detail::close_all(t_);
    }

    void close(HBOOST_IOS::openmode which) 
    { 
        iostreams::close(t_, which); 
    }

    bool flush() 
    { 
        return iostreams::flush(t_); 
    }

    template<typename Locale> // Avoid dependency on <locale>
    void imbue(const Locale& loc) { iostreams::imbue(t_, loc); }

    std::streamsize optimal_buffer_size() const 
    { return iostreams::optimal_buffer_size(t_); }
public:
    value_type t_;
};

//----------------------------------------------------------------------------//

} } } // End namespaces detail, iostreams, boost.

#endif // #ifndef HBOOST_IOSTREAMS_DETAIL_DEVICE_ADAPTER_HPP_INCLUDED
