// (C) Copyright 2008 CodeRage, LLC (turkanis at coderage dot com)
// (C) Copyright 2003-2007 Jonathan Turkanis
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

#ifndef HBOOST_IOSTREAMS_DETAIL_CHAINBUF_HPP_INCLUDED
#define HBOOST_IOSTREAMS_DETAIL_CHAINBUF_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif      

#include <hboost/config.hpp>                    // HBOOST_MSVC, template friends.
#include <hboost/detail/workaround.hpp>
#include <hboost/iostreams/chain.hpp>
#include <hboost/iostreams/detail/access_control.hpp>
#include <hboost/iostreams/detail/config/wide_streams.hpp>
#include <hboost/iostreams/detail/streambuf.hpp>
#include <hboost/iostreams/detail/streambuf/linked_streambuf.hpp>
#include <hboost/iostreams/detail/translate_int_type.hpp>
#include <hboost/iostreams/traits.hpp>
#include <hboost/noncopyable.hpp>

namespace hboost { namespace iostreams { namespace detail {

//--------------Definition of chainbuf----------------------------------------//

//
// Template name: chainbuf.
// Description: Stream buffer which operates by delegating to the first
//      linked_streambuf in a chain.
// Template parameters:
//      Chain - The chain type.
//
template<typename Chain, typename Mode, typename Access>
class chainbuf
    : public HBOOST_IOSTREAMS_BASIC_STREAMBUF(
                 typename Chain::char_type,
                 typename Chain::traits_type
             ),
      public access_control<typename Chain::client_type, Access>,
      private noncopyable
{
private:
    typedef access_control<chain_client<Chain>, Access>      client_type;
public:
    typedef typename Chain::char_type                        char_type;
    HBOOST_IOSTREAMS_STREAMBUF_TYPEDEFS(typename Chain::traits_type)
protected:
    typedef linked_streambuf<char_type, traits_type>         delegate_type;
    chainbuf() { client_type::set_chain(&chain_); }
    int_type underflow() 
        { sentry t(this); return translate(delegate().underflow()); }
    int_type pbackfail(int_type c)
        { sentry t(this); return translate(delegate().pbackfail(c)); }
    std::streamsize xsgetn(char_type* s, std::streamsize n)
        { sentry t(this); return delegate().xsgetn(s, n); }
    int_type overflow(int_type c)
        { sentry t(this); return translate(delegate().overflow(c)); }
    std::streamsize xsputn(const char_type* s, std::streamsize n)
        { sentry t(this); return delegate().xsputn(s, n); }
    int sync() { sentry t(this); return delegate().sync(); }
    pos_type seekoff( off_type off, HBOOST_IOS::seekdir way,
                      HBOOST_IOS::openmode which =
                          HBOOST_IOS::in | HBOOST_IOS::out )
        { sentry t(this); return delegate().seekoff(off, way, which); }
    pos_type seekpos( pos_type sp,
                      HBOOST_IOS::openmode which =
                          HBOOST_IOS::in | HBOOST_IOS::out )
        { sentry t(this); return delegate().seekpos(sp, which); }
protected:
    typedef HBOOST_IOSTREAMS_BASIC_STREAMBUF(
                 typename Chain::char_type,
                 typename Chain::traits_type
             )                                               base_type;
//#if !HBOOST_WORKAROUND(__GNUC__, == 2)                                 
//    HBOOST_IOSTREAMS_USING_PROTECTED_STREAMBUF_MEMBERS(base_type)
//#endif
private:

    // Translate from std int_type to chain's int_type.
    typedef HBOOST_IOSTREAMS_CHAR_TRAITS(char_type)           std_traits;
    typedef typename Chain::traits_type                      chain_traits;
    static typename chain_traits::int_type 
    translate(typename std_traits::int_type c)
        { return translate_int_type<std_traits, chain_traits>(c); }

    delegate_type& delegate() 
        { return static_cast<delegate_type&>(chain_.front()); }
    void get_pointers()
        {
            this->setg(delegate().eback(), delegate().gptr(), delegate().egptr());
            this->setp(delegate().pbase(), delegate().epptr());
            this->pbump((int) (delegate().pptr() - delegate().pbase()));
        }
    void set_pointers()
        {
            delegate().setg(this->eback(), this->gptr(), this->egptr());
            delegate().setp(this->pbase(), this->epptr());
            delegate().pbump((int) (this->pptr() - this->pbase()));
        }
    struct sentry {
        sentry(chainbuf<Chain, Mode, Access>* buf) : buf_(buf)
            { buf_->set_pointers(); }
        ~sentry() { buf_->get_pointers(); }
        chainbuf<Chain, Mode, Access>* buf_;
    };
    friend struct sentry;
    Chain chain_;
};

} } } // End namespaces detail, iostreams, boost.

#endif // #ifndef HBOOST_IOSTREAMS_DETAIL_CHAINBUF_HPP_INCLUDED
