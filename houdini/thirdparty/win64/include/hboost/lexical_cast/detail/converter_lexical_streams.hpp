// Copyright Kevlin Henney, 2000-2005.
// Copyright Alexander Nasonov, 2006-2010.
// Copyright Antony Polukhin, 2011-2014.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// what:  lexical_cast custom keyword cast
// who:   contributed by Kevlin Henney,
//        enhanced with contributions from Terje Slettebo,
//        with additional fixes and suggestions from Gennaro Prota,
//        Beman Dawes, Dave Abrahams, Daryle Walker, Peter Dimov,
//        Alexander Nasonov, Antony Polukhin, Justin Viiret, Michael Hofmann,
//        Cheng Yang, Matthew Bradbury, David W. Birdsall, Pavel Korzh and other Boosters
// when:  November 2000, March 2003, June 2005, June 2006, March 2011 - 2014

#ifndef HBOOST_LEXICAL_CAST_DETAIL_CONVERTER_LEXICAL_STREAMS_HPP
#define HBOOST_LEXICAL_CAST_DETAIL_CONVERTER_LEXICAL_STREAMS_HPP

#include <hboost/config.hpp>
#ifdef HBOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif


#if defined(HBOOST_NO_STRINGSTREAM) || defined(HBOOST_NO_STD_WSTRING)
#define HBOOST_LCAST_NO_WCHAR_T
#endif

#include <cstddef>
#include <string>
#include <cstring>
#include <cstdio>
#include <hboost/limits.hpp>
#include <hboost/mpl/if.hpp>
#include <hboost/type_traits/is_pointer.hpp>
#include <hboost/static_assert.hpp>
#include <hboost/detail/workaround.hpp>


#ifndef HBOOST_NO_STD_LOCALE
#   include <locale>
#else
#   ifndef HBOOST_LEXICAL_CAST_ASSUME_C_LOCALE
        // Getting error at this point means, that your STL library is old/lame/misconfigured.
        // If nothing can be done with STL library, define HBOOST_LEXICAL_CAST_ASSUME_C_LOCALE,
        // but beware: lexical_cast will understand only 'C' locale delimeters and thousands
        // separators.
#       error "Unable to use <locale> header. Define HBOOST_LEXICAL_CAST_ASSUME_C_LOCALE to force "
#       error "hboost::lexical_cast to use only 'C' locale during conversions."
#   endif
#endif

#ifdef HBOOST_NO_STRINGSTREAM
#include <strstream>
#else
#include <sstream>
#endif

#include <hboost/lexical_cast/detail/lcast_char_constants.hpp>
#include <hboost/lexical_cast/detail/lcast_unsigned_converters.hpp>
#include <hboost/lexical_cast/detail/inf_nan.hpp>

#include <istream>

#ifndef HBOOST_NO_CXX11_HDR_ARRAY
#include <array>
#endif

#include <hboost/array.hpp>
#include <hboost/type_traits/make_unsigned.hpp>
#include <hboost/type_traits/is_integral.hpp>
#include <hboost/type_traits/is_float.hpp>
#include <hboost/range/iterator_range_core.hpp>
#include <hboost/container/container_fwd.hpp>
#include <hboost/integer.hpp>
#include <hboost/detail/basic_pointerbuf.hpp>
#include <hboost/noncopyable.hpp>
#ifndef HBOOST_NO_CWCHAR
#   include <cwchar>
#endif

namespace hboost {

    namespace detail // basic_unlockedbuf
    {
        // acts as a stream buffer which wraps around a pair of pointers
        // and gives acces to internals
        template <class BufferType, class CharT>
        class basic_unlockedbuf : public basic_pointerbuf<CharT, BufferType> {
        public:
           typedef basic_pointerbuf<CharT, BufferType> base_type;
           typedef HBOOST_DEDUCED_TYPENAME base_type::streamsize streamsize;

#ifndef HBOOST_NO_USING_TEMPLATE
            using base_type::pptr;
            using base_type::pbase;
            using base_type::setbuf;
#else
            charT* pptr() const { return base_type::pptr(); }
            charT* pbase() const { return base_type::pbase(); }
            BufferType* setbuf(char_type* s, streamsize n) { return base_type::setbuf(s, n); }
#endif
        };
    }

    namespace detail
    {
        struct do_not_construct_out_stream_t{};
        
        template <class CharT, class Traits>
        struct out_stream_helper_trait {
#if defined(HBOOST_NO_STRINGSTREAM)
            typedef std::ostrstream                                 out_stream_t;
            typedef void                                            buffer_t;
#elif defined(HBOOST_NO_STD_LOCALE)
            typedef std::ostringstream                              out_stream_t;
            typedef basic_unlockedbuf<std::streambuf, char>         buffer_t;
#else
            typedef std::basic_ostringstream<CharT, Traits> 
                out_stream_t;
            typedef basic_unlockedbuf<std::basic_streambuf<CharT, Traits>, CharT>  
                buffer_t;
#endif
        };   
    }

    namespace detail // optimized stream wrappers
    {
        template< class CharT // a result of widest_char transformation
                , class Traits
                , bool RequiresStringbuffer
                , std::size_t CharacterBufferSize
                >
        class lexical_istream_limited_src: hboost::noncopyable {
            typedef HBOOST_DEDUCED_TYPENAME out_stream_helper_trait<CharT, Traits>::buffer_t
                buffer_t;

            typedef HBOOST_DEDUCED_TYPENAME out_stream_helper_trait<CharT, Traits>::out_stream_t
                out_stream_t;
    
            typedef HBOOST_DEDUCED_TYPENAME hboost::mpl::if_c<
                RequiresStringbuffer,
                out_stream_t,
                do_not_construct_out_stream_t
            >::type deduced_out_stream_t;

            // A string representation of Source is written to `buffer`.
            deduced_out_stream_t out_stream;
            CharT   buffer[CharacterBufferSize];

            // After the `operator <<`  finishes, `[start, finish)` is
            // the range to output by `operator >>` 
            const CharT*  start;
            const CharT*  finish;

        public:
            lexical_istream_limited_src() HBOOST_NOEXCEPT
              : start(buffer)
              , finish(buffer + CharacterBufferSize)
            {}
    
            const CharT* cbegin() const HBOOST_NOEXCEPT {
                return start;
            }

            const CharT* cend() const HBOOST_NOEXCEPT {
                return finish;
            }

        private:
            // Undefined:
            lexical_istream_limited_src(lexical_istream_limited_src const&);
            void operator=(lexical_istream_limited_src const&);

/************************************ HELPER FUNCTIONS FOR OPERATORS << ( ... ) ********************************/
            bool shl_char(CharT ch) HBOOST_NOEXCEPT {
                Traits::assign(buffer[0], ch);
                finish = start + 1;
                return true;
            }

#ifndef HBOOST_LCAST_NO_WCHAR_T
            template <class T>
            bool shl_char(T ch) {
                HBOOST_STATIC_ASSERT_MSG(( sizeof(T) <= sizeof(CharT)) ,
                    "hboost::lexical_cast does not support narrowing of char types."
                    "Use hboost::locale instead" );
#ifndef HBOOST_LEXICAL_CAST_ASSUME_C_LOCALE
                std::locale loc;
                CharT const w = HBOOST_USE_FACET(std::ctype<CharT>, loc).widen(ch);
#else
                CharT const w = static_cast<CharT>(ch);
#endif
                Traits::assign(buffer[0], w);
                finish = start + 1;
                return true;
            }
#endif

            bool shl_char_array(CharT const* str) HBOOST_NOEXCEPT {
                start = str;
                finish = start + Traits::length(str);
                return true;
            }

            template <class T>
            bool shl_char_array(T const* str) {
                HBOOST_STATIC_ASSERT_MSG(( sizeof(T) <= sizeof(CharT)),
                    "hboost::lexical_cast does not support narrowing of char types."
                    "Use hboost::locale instead" );
                return shl_input_streamable(str);
            }
            
            bool shl_char_array_limited(CharT const* str, std::size_t max_size) HBOOST_NOEXCEPT {
                start = str;
                finish = std::find(start, start + max_size, Traits::to_char_type(0));
                return true;
            }

            template<typename InputStreamable>
            bool shl_input_streamable(InputStreamable& input) {
#if defined(HBOOST_NO_STRINGSTREAM) || defined(HBOOST_NO_STD_LOCALE)
                // If you have compilation error at this point, than your STL library
                // does not support such conversions. Try updating it.
                HBOOST_STATIC_ASSERT((hboost::is_same<char, CharT>::value));
#endif

#ifndef HBOOST_NO_EXCEPTIONS
                out_stream.exceptions(std::ios::badbit);
                try {
#endif
                bool const result = !(out_stream << input).fail();
                const buffer_t* const p = static_cast<buffer_t*>(
                    static_cast<std::basic_streambuf<CharT, Traits>*>(out_stream.rdbuf())
                );
                start = p->pbase();
                finish = p->pptr();
                return result;
#ifndef HBOOST_NO_EXCEPTIONS
                } catch (const ::std::ios_base::failure& /*f*/) {
                    return false;
                }
#endif
            }

            template <class T>
            inline bool shl_unsigned(const T n) {
                CharT* tmp_finish = buffer + CharacterBufferSize;
                start = lcast_put_unsigned<Traits, T, CharT>(n, tmp_finish).convert();
                finish = tmp_finish;
                return true;
            }

            template <class T>
            inline bool shl_signed(const T n) {
                CharT* tmp_finish = buffer + CharacterBufferSize;
                typedef HBOOST_DEDUCED_TYPENAME hboost::make_unsigned<T>::type utype;
                CharT* tmp_start = lcast_put_unsigned<Traits, utype, CharT>(lcast_to_unsigned(n), tmp_finish).convert();
                if (n < 0) {
                    --tmp_start;
                    CharT const minus = lcast_char_constants<CharT>::minus;
                    Traits::assign(*tmp_start, minus);
                }
                start = tmp_start;
                finish = tmp_finish;
                return true;
            }

            template <class T, class SomeCharT>
            bool shl_real_type(const T& val, SomeCharT* /*begin*/) {
                lcast_set_precision(out_stream, &val);
                return shl_input_streamable(val);
            }

            bool shl_real_type(float val, char* begin) {
                using namespace std;
                const double val_as_double = val;
                finish = start +
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && !defined(__SGI_STL_PORT) && !defined(_STLPORT_VERSION)
                    sprintf_s(begin, CharacterBufferSize,
#else
                    sprintf(begin, 
#endif
                    "%.*g", static_cast<int>(hboost::detail::lcast_get_precision<float>()), val_as_double);
                return finish > start;
            }

            bool shl_real_type(double val, char* begin) {
                using namespace std;
                finish = start +
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && !defined(__SGI_STL_PORT) && !defined(_STLPORT_VERSION)
                    sprintf_s(begin, CharacterBufferSize,
#else
                    sprintf(begin, 
#endif
                    "%.*g", static_cast<int>(hboost::detail::lcast_get_precision<double>()), val);
                return finish > start;
            }

#ifndef __MINGW32__
            bool shl_real_type(long double val, char* begin) {
                using namespace std;
                finish = start +
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && !defined(__SGI_STL_PORT) && !defined(_STLPORT_VERSION)
                    sprintf_s(begin, CharacterBufferSize,
#else
                    sprintf(begin, 
#endif
                    "%.*Lg", static_cast<int>(hboost::detail::lcast_get_precision<long double>()), val );
                return finish > start;
            }
#endif


#if !defined(HBOOST_LCAST_NO_WCHAR_T) && !defined(HBOOST_NO_SWPRINTF) && !defined(__MINGW32__)
            bool shl_real_type(float val, wchar_t* begin) {
                using namespace std;
                const double val_as_double = val;
                finish = start + swprintf(begin, CharacterBufferSize,
                                       L"%.*g",
                                       static_cast<int>(hboost::detail::lcast_get_precision<float >()),
                                       val_as_double );
                return finish > start;
            }

            bool shl_real_type(double val, wchar_t* begin) {
                using namespace std;
                finish = start + swprintf(begin, CharacterBufferSize,
                                          L"%.*g", static_cast<int>(hboost::detail::lcast_get_precision<double >()), val );
                return finish > start;
            }

            bool shl_real_type(long double val, wchar_t* begin) {
                using namespace std;
                finish = start + swprintf(begin, CharacterBufferSize,
                                          L"%.*Lg", static_cast<int>(hboost::detail::lcast_get_precision<long double >()), val );
                return finish > start;
            }
#endif
            template <class T>
            bool shl_real(T val) {
                CharT* tmp_finish = buffer + CharacterBufferSize;
                if (put_inf_nan(buffer, tmp_finish, val)) {
                    finish = tmp_finish;
                    return true;
                }

                return shl_real_type(val, static_cast<CharT*>(buffer));
            }

/************************************ OPERATORS << ( ... ) ********************************/
        public:
            template<class Alloc>
            bool operator<<(std::basic_string<CharT,Traits,Alloc> const& str) HBOOST_NOEXCEPT {
                start = str.data();
                finish = start + str.length();
                return true;
            }

            template<class Alloc>
            bool operator<<(hboost::container::basic_string<CharT,Traits,Alloc> const& str) HBOOST_NOEXCEPT {
                start = str.data();
                finish = start + str.length();
                return true;
            }

            bool operator<<(bool value) HBOOST_NOEXCEPT {
                CharT const czero = lcast_char_constants<CharT>::zero;
                Traits::assign(buffer[0], Traits::to_char_type(czero + value));
                finish = start + 1;
                return true;
            }

            template <class C>
            HBOOST_DEDUCED_TYPENAME hboost::disable_if<hboost::is_const<C>, bool>::type 
            operator<<(const iterator_range<C*>& rng) HBOOST_NOEXCEPT {
                return (*this) << iterator_range<const C*>(rng.begin(), rng.end());
            }
            
            bool operator<<(const iterator_range<const CharT*>& rng) HBOOST_NOEXCEPT {
                start = rng.begin();
                finish = rng.end();
                return true; 
            }

            bool operator<<(const iterator_range<const signed char*>& rng) HBOOST_NOEXCEPT {
                return (*this) << iterator_range<const char*>(
                    reinterpret_cast<const char*>(rng.begin()),
                    reinterpret_cast<const char*>(rng.end())
                );
            }

            bool operator<<(const iterator_range<const unsigned char*>& rng) HBOOST_NOEXCEPT {
                return (*this) << iterator_range<const char*>(
                    reinterpret_cast<const char*>(rng.begin()),
                    reinterpret_cast<const char*>(rng.end())
                );
            }

            bool operator<<(char ch)                    { return shl_char(ch); }
            bool operator<<(unsigned char ch)           { return ((*this) << static_cast<char>(ch)); }
            bool operator<<(signed char ch)             { return ((*this) << static_cast<char>(ch)); }
#if !defined(HBOOST_LCAST_NO_WCHAR_T)
            bool operator<<(wchar_t const* str)         { return shl_char_array(str); }
            bool operator<<(wchar_t * str)              { return shl_char_array(str); }
#ifndef HBOOST_NO_INTRINSIC_WCHAR_T
            bool operator<<(wchar_t ch)                 { return shl_char(ch); }
#endif
#endif
#if !defined(HBOOST_NO_CXX11_CHAR16_T) && !defined(HBOOST_NO_CXX11_UNICODE_LITERALS)
            bool operator<<(char16_t ch)                { return shl_char(ch); }
            bool operator<<(char16_t * str)             { return shl_char_array(str); }
            bool operator<<(char16_t const * str)       { return shl_char_array(str); }
#endif
#if !defined(HBOOST_NO_CXX11_CHAR32_T) && !defined(HBOOST_NO_CXX11_UNICODE_LITERALS)
            bool operator<<(char32_t ch)                { return shl_char(ch); }
            bool operator<<(char32_t * str)             { return shl_char_array(str); }
            bool operator<<(char32_t const * str)       { return shl_char_array(str); }
#endif
            bool operator<<(unsigned char const* ch)    { return ((*this) << reinterpret_cast<char const*>(ch)); }
            bool operator<<(unsigned char * ch)         { return ((*this) << reinterpret_cast<char *>(ch)); }
            bool operator<<(signed char const* ch)      { return ((*this) << reinterpret_cast<char const*>(ch)); }
            bool operator<<(signed char * ch)           { return ((*this) << reinterpret_cast<char *>(ch)); }
            bool operator<<(char const* str)            { return shl_char_array(str); }
            bool operator<<(char* str)                  { return shl_char_array(str); }
            bool operator<<(short n)                    { return shl_signed(n); }
            bool operator<<(int n)                      { return shl_signed(n); }
            bool operator<<(long n)                     { return shl_signed(n); }
            bool operator<<(unsigned short n)           { return shl_unsigned(n); }
            bool operator<<(unsigned int n)             { return shl_unsigned(n); }
            bool operator<<(unsigned long n)            { return shl_unsigned(n); }

#if defined(HBOOST_HAS_LONG_LONG)
            bool operator<<(hboost::ulong_long_type n)   { return shl_unsigned(n); }
            bool operator<<(hboost::long_long_type n)    { return shl_signed(n); }
#elif defined(HBOOST_HAS_MS_INT64)
            bool operator<<(unsigned __int64 n)         { return shl_unsigned(n); }
            bool operator<<(         __int64 n)         { return shl_signed(n); }
#endif

#ifdef HBOOST_HAS_INT128
            bool operator<<(const hboost::uint128_type& n)   { return shl_unsigned(n); }
            bool operator<<(const hboost::int128_type& n)    { return shl_signed(n); }
#endif
            bool operator<<(float val)                  { return shl_real(val); }
            bool operator<<(double val)                 { return shl_real(val); }
            bool operator<<(long double val)            {
#ifndef __MINGW32__
                return shl_real(val);
#else
                return shl_real(static_cast<double>(val));
#endif
            }
            
            // Adding constness to characters. Constness does not change layout
            template <class C, std::size_t N>
            HBOOST_DEDUCED_TYPENAME hboost::disable_if<hboost::is_const<C>, bool>::type
            operator<<(hboost::array<C, N> const& input) HBOOST_NOEXCEPT { 
                HBOOST_STATIC_ASSERT_MSG(
                    (sizeof(hboost::array<const C, N>) == sizeof(hboost::array<C, N>)),
                    "hboost::array<C, N> and hboost::array<const C, N> must have exactly the same layout."
                );
                return ((*this) << reinterpret_cast<hboost::array<const C, N> const& >(input)); 
            }

            template <std::size_t N>
            bool operator<<(hboost::array<const CharT, N> const& input) HBOOST_NOEXCEPT { 
                return shl_char_array_limited(input.begin(), N); 
            }

            template <std::size_t N>
            bool operator<<(hboost::array<const unsigned char, N> const& input) HBOOST_NOEXCEPT { 
                return ((*this) << reinterpret_cast<hboost::array<const char, N> const& >(input)); 
            }

            template <std::size_t N>
            bool operator<<(hboost::array<const signed char, N> const& input) HBOOST_NOEXCEPT { 
                return ((*this) << reinterpret_cast<hboost::array<const char, N> const& >(input)); 
            }
 
#ifndef HBOOST_NO_CXX11_HDR_ARRAY
            // Making a Boost.Array from std::array
            template <class C, std::size_t N>
            bool operator<<(std::array<C, N> const& input) HBOOST_NOEXCEPT { 
                HBOOST_STATIC_ASSERT_MSG(
                    (sizeof(std::array<C, N>) == sizeof(hboost::array<C, N>)),
                    "std::array and hboost::array must have exactly the same layout. "
                    "Bug in implementation of std::array or hboost::array."
                );
                return ((*this) << reinterpret_cast<hboost::array<C, N> const& >(input)); 
            }
#endif
            template <class InStreamable>
            bool operator<<(const InStreamable& input)  { return shl_input_streamable(input); }
        };


        template <class CharT, class Traits>
        class lexical_ostream_limited_src: hboost::noncopyable {
            //`[start, finish)` is the range to output by `operator >>` 
            const CharT*        start;
            const CharT* const  finish;

        public:
            lexical_ostream_limited_src(const CharT* begin, const CharT* end) HBOOST_NOEXCEPT
              : start(begin)
              , finish(end)
            {}

/************************************ HELPER FUNCTIONS FOR OPERATORS >> ( ... ) ********************************/
        private:
            template <typename Type>
            bool shr_unsigned(Type& output) {
                if (start == finish) return false;
                CharT const minus = lcast_char_constants<CharT>::minus;
                CharT const plus = lcast_char_constants<CharT>::plus;
                bool const has_minus = Traits::eq(minus, *start);

                /* We won`t use `start' any more, so no need in decrementing it after */
                if (has_minus || Traits::eq(plus, *start)) {
                    ++start;
                }

                bool const succeed = lcast_ret_unsigned<Traits, Type, CharT>(output, start, finish).convert();

                if (has_minus) {
                    output = static_cast<Type>(0u - output);
                }

                return succeed;
            }

            template <typename Type>
            bool shr_signed(Type& output) {
                if (start == finish) return false;
                CharT const minus = lcast_char_constants<CharT>::minus;
                CharT const plus = lcast_char_constants<CharT>::plus;
                typedef HBOOST_DEDUCED_TYPENAME make_unsigned<Type>::type utype;
                utype out_tmp = 0;
                bool const has_minus = Traits::eq(minus, *start);

                /* We won`t use `start' any more, so no need in decrementing it after */
                if (has_minus || Traits::eq(plus, *start)) {
                    ++start;
                }

                bool succeed = lcast_ret_unsigned<Traits, utype, CharT>(out_tmp, start, finish).convert();
                if (has_minus) {
                    utype const comp_val = (static_cast<utype>(1) << std::numeric_limits<Type>::digits);
                    succeed = succeed && out_tmp<=comp_val;
                    output = static_cast<Type>(0u - out_tmp);
                } else {
                    utype const comp_val = static_cast<utype>((std::numeric_limits<Type>::max)());
                    succeed = succeed && out_tmp<=comp_val;
                    output = static_cast<Type>(out_tmp);
                }
                return succeed;
            }

            template<typename InputStreamable>
            bool shr_using_base_class(InputStreamable& output)
            {
                HBOOST_STATIC_ASSERT_MSG(
                    (!hboost::is_pointer<InputStreamable>::value),
                    "hboost::lexical_cast can not convert to pointers"
                );

#if defined(HBOOST_NO_STRINGSTREAM) || defined(HBOOST_NO_STD_LOCALE)
                HBOOST_STATIC_ASSERT_MSG((hboost::is_same<char, CharT>::value),
                    "hboost::lexical_cast can not convert, because your STL library does not "
                    "support such conversions. Try updating it."
                );
#endif
                typedef HBOOST_DEDUCED_TYPENAME out_stream_helper_trait<CharT, Traits>::buffer_t
                    buffer_t;

#if defined(HBOOST_NO_STRINGSTREAM)
                std::istrstream stream(start, finish - start);
#else

                buffer_t buf;
                // Usually `istream` and `basic_istream` do not modify 
                // content of buffer; `buffer_t` assures that this is true
                buf.setbuf(const_cast<CharT*>(start), finish - start);
#if defined(HBOOST_NO_STD_LOCALE)
                std::istream stream(&buf);
#else
                std::basic_istream<CharT, Traits> stream(&buf);
#endif // HBOOST_NO_STD_LOCALE
#endif // HBOOST_NO_STRINGSTREAM

#ifndef HBOOST_NO_EXCEPTIONS
                stream.exceptions(std::ios::badbit);
                try {
#endif
                stream.unsetf(std::ios::skipws);
                lcast_set_precision(stream, static_cast<InputStreamable*>(0));

                return (stream >> output) 
                    && (stream.get() == Traits::eof());

#ifndef HBOOST_NO_EXCEPTIONS
                } catch (const ::std::ios_base::failure& /*f*/) {
                    return false;
                }
#endif
            }

            template<class T>
            inline bool shr_xchar(T& output) HBOOST_NOEXCEPT {
                HBOOST_STATIC_ASSERT_MSG(( sizeof(CharT) == sizeof(T) ),
                    "hboost::lexical_cast does not support narrowing of character types."
                    "Use hboost::locale instead" );
                bool const ok = (finish - start == 1);
                if (ok) {
                    CharT out;
                    Traits::assign(out, *start);
                    output = static_cast<T>(out);
                }
                return ok;
            }

            template <std::size_t N, class ArrayT>
            bool shr_std_array(ArrayT& output) HBOOST_NOEXCEPT {
                using namespace std;
                const std::size_t size = static_cast<std::size_t>(finish - start);
                if (size > N - 1) { // `-1` because we need to store \0 at the end 
                    return false;
                }

                memcpy(&output[0], start, size * sizeof(CharT));
                output[size] = Traits::to_char_type(0);
                return true;
            }

/************************************ OPERATORS >> ( ... ) ********************************/
        public:
            bool operator>>(unsigned short& output)             { return shr_unsigned(output); }
            bool operator>>(unsigned int& output)               { return shr_unsigned(output); }
            bool operator>>(unsigned long int& output)          { return shr_unsigned(output); }
            bool operator>>(short& output)                      { return shr_signed(output); }
            bool operator>>(int& output)                        { return shr_signed(output); }
            bool operator>>(long int& output)                   { return shr_signed(output); }
#if defined(HBOOST_HAS_LONG_LONG)
            bool operator>>(hboost::ulong_long_type& output)     { return shr_unsigned(output); }
            bool operator>>(hboost::long_long_type& output)      { return shr_signed(output); }
#elif defined(HBOOST_HAS_MS_INT64)
            bool operator>>(unsigned __int64& output)           { return shr_unsigned(output); }
            bool operator>>(__int64& output)                    { return shr_signed(output); }
#endif

#ifdef HBOOST_HAS_INT128
            bool operator>>(hboost::uint128_type& output)        { return shr_unsigned(output); }
            bool operator>>(hboost::int128_type& output)         { return shr_signed(output); }
#endif

            bool operator>>(char& output)                       { return shr_xchar(output); }
            bool operator>>(unsigned char& output)              { return shr_xchar(output); }
            bool operator>>(signed char& output)                { return shr_xchar(output); }
#if !defined(HBOOST_LCAST_NO_WCHAR_T) && !defined(HBOOST_NO_INTRINSIC_WCHAR_T)
            bool operator>>(wchar_t& output)                    { return shr_xchar(output); }
#endif
#if !defined(HBOOST_NO_CXX11_CHAR16_T) && !defined(HBOOST_NO_CXX11_UNICODE_LITERALS)
            bool operator>>(char16_t& output)                   { return shr_xchar(output); }
#endif
#if !defined(HBOOST_NO_CXX11_CHAR32_T) && !defined(HBOOST_NO_CXX11_UNICODE_LITERALS)
            bool operator>>(char32_t& output)                   { return shr_xchar(output); }
#endif
            template<class Alloc>
            bool operator>>(std::basic_string<CharT,Traits,Alloc>& str) { 
                str.assign(start, finish); return true; 
            }

            template<class Alloc>
            bool operator>>(hboost::container::basic_string<CharT,Traits,Alloc>& str) { 
                str.assign(start, finish); return true; 
            }

            template <std::size_t N>
            bool operator>>(hboost::array<CharT, N>& output) HBOOST_NOEXCEPT { 
                return shr_std_array<N>(output); 
            }

            template <std::size_t N>
            bool operator>>(hboost::array<unsigned char, N>& output) HBOOST_NOEXCEPT { 
                return ((*this) >> reinterpret_cast<hboost::array<char, N>& >(output)); 
            }

            template <std::size_t N>
            bool operator>>(hboost::array<signed char, N>& output) HBOOST_NOEXCEPT { 
                return ((*this) >> reinterpret_cast<hboost::array<char, N>& >(output)); 
            }
 
#ifndef HBOOST_NO_CXX11_HDR_ARRAY
            template <class C, std::size_t N>
            bool operator>>(std::array<C, N>& output) HBOOST_NOEXCEPT { 
                HBOOST_STATIC_ASSERT_MSG(
                    (sizeof(hboost::array<C, N>) == sizeof(hboost::array<C, N>)),
                    "std::array<C, N> and hboost::array<C, N> must have exactly the same layout."
                );
                return ((*this) >> reinterpret_cast<hboost::array<C, N>& >(output));
            }
#endif

            bool operator>>(bool& output) HBOOST_NOEXCEPT {
                output = false; // Suppress warning about uninitalized variable

                if (start == finish) return false;
                CharT const zero = lcast_char_constants<CharT>::zero;
                CharT const plus = lcast_char_constants<CharT>::plus;
                CharT const minus = lcast_char_constants<CharT>::minus;

                const CharT* const dec_finish = finish - 1;
                output = Traits::eq(*dec_finish, zero + 1);
                if (!output && !Traits::eq(*dec_finish, zero)) {
                    return false; // Does not ends on '0' or '1'
                }

                if (start == dec_finish) return true;

                // We may have sign at the beginning
                if (Traits::eq(plus, *start) || (Traits::eq(minus, *start) && !output)) {
                    ++ start;
                }

                // Skipping zeros
                while (start != dec_finish) {
                    if (!Traits::eq(zero, *start)) {
                        return false; // Not a zero => error
                    }

                    ++ start;
                }

                return true;
            }

        private:
            // Not optimised converter
            template <class T>
            bool float_types_converter_internal(T& output) {
                if (parse_inf_nan(start, finish, output)) return true;
                bool const return_value = shr_using_base_class(output);

                /* Some compilers and libraries successfully
                 * parse 'inf', 'INFINITY', '1.0E', '1.0E-'...
                 * We are trying to provide a unified behaviour,
                 * so we just forbid such conversions (as some
                 * of the most popular compilers/libraries do)
                 * */
                CharT const minus = lcast_char_constants<CharT>::minus;
                CharT const plus = lcast_char_constants<CharT>::plus;
                CharT const capital_e = lcast_char_constants<CharT>::capital_e;
                CharT const lowercase_e = lcast_char_constants<CharT>::lowercase_e;
                if ( return_value &&
                     (
                        Traits::eq(*(finish-1), lowercase_e)                   // 1.0e
                        || Traits::eq(*(finish-1), capital_e)                  // 1.0E
                        || Traits::eq(*(finish-1), minus)                      // 1.0e- or 1.0E-
                        || Traits::eq(*(finish-1), plus)                       // 1.0e+ or 1.0E+
                     )
                ) return false;

                return return_value;
            }

        public:
            bool operator>>(float& output) { return float_types_converter_internal(output); }
            bool operator>>(double& output) { return float_types_converter_internal(output); }
            bool operator>>(long double& output) { return float_types_converter_internal(output); }

            // Generic istream-based algorithm.
            // lcast_streambuf_for_target<InputStreamable>::value is true.
            template <typename InputStreamable>
            bool operator>>(InputStreamable& output) { 
                return shr_using_base_class(output); 
            }
        };
    }
} // namespace hboost

#undef HBOOST_LCAST_NO_WCHAR_T

#endif // HBOOST_LEXICAL_CAST_DETAIL_CONVERTER_LEXICAL_HPP

