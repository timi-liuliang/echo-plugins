///////////////////////////////////////////////////////////////////////////////
/// \file complex.hpp
///
//  Copyright 2005 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_NUMERIC_FUNCTIONAL_COMPLEX_HPP_EAN_01_17_2006
#define HBOOST_NUMERIC_FUNCTIONAL_COMPLEX_HPP_EAN_01_17_2006

#ifdef HBOOST_NUMERIC_FUNCTIONAL_HPP_INCLUDED
# error Include this file before hboost/accumulators/numeric/functional.hpp
#endif

#include <complex>
#include <hboost/mpl/or.hpp>
#include <hboost/type_traits/is_same.hpp>
#include <hboost/utility/enable_if.hpp>
#include <hboost/typeof/std/complex.hpp>
#include <hboost/accumulators/numeric/functional_fwd.hpp>

namespace hboost { namespace numeric { namespace operators
{
    // So that the stats compile when Sample type is std::complex
    template<typename T, typename U>
    typename
        disable_if<
            mpl::or_<is_same<T, U>, is_same<std::complex<T>, U> >
          , std::complex<T>
        >::type
    operator *(std::complex<T> ri, U const &u)
    {
        // BUGBUG promote result to typeof(T()*u) ?
        return ri *= static_cast<T>(u);
    }

    template<typename T, typename U>
    typename
        disable_if<
            mpl::or_<is_same<T, U>, is_same<std::complex<T>, U> >
          , std::complex<T>
        >::type
    operator /(std::complex<T> ri, U const &u)
    {
        // BUGBUG promote result to typeof(T()*u) ?
        return ri /= static_cast<T>(u);
    }

}}} // namespace hboost::numeric::operators

namespace hboost { namespace numeric
{
    namespace detail
    {
        template<typename T>
        struct one_complex
        {
            static std::complex<T> const value;
        };

        template<typename T>
        std::complex<T> const one_complex<T>::value
          = std::complex<T>(numeric::one<T>::value, numeric::one<T>::value);
    }

    /// INTERNAL ONLY
    ///
    template<typename T>
    struct one<std::complex<T> >
      : detail::one_complex<T>
    {
        typedef one type;
        typedef std::complex<T> value_type;
        operator value_type const & () const
        {
            return detail::one_complex<T>::value;
        }
    };

}} // namespace hboost::numeric

#endif
