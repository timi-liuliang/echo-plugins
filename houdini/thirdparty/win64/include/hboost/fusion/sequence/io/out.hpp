/*=============================================================================
    Copyright (c) 1999-2003 Jaakko Jarvi
    Copyright (c) 1999-2003 Jeremiah Willcock
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_OUT_05042005_0120)
#define HBOOST_OUT_05042005_0120

#include <hboost/fusion/support/config.hpp>
#include <ostream>
#include <hboost/fusion/sequence/io/detail/out.hpp>
#include <hboost/fusion/support/is_sequence.hpp>
#include <hboost/utility/enable_if.hpp>
#include <hboost/mpl/or.hpp>

namespace hboost { namespace fusion
{
    template <typename Sequence>
    inline std::ostream&
    out(std::ostream& os, Sequence& seq)
    {
        detail::print_sequence(os, seq);
        return os;
    }
    
    namespace operators
    {
        template <typename Sequence>
        inline typename
            hboost::enable_if<
               fusion::traits::is_sequence<Sequence>
              , std::ostream&
            >::type
        operator<<(std::ostream& os, Sequence const& seq)
        {
            return fusion::out(os, seq);
        }
    }
    using operators::operator<<;
}}

#endif
