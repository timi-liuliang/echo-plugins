///////////////////////////////////////////////////////////////////////////////
/// \file regex_domain.hpp
/// Contains the definition of the regex_domain type
//
//  Copyright 2009 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_XPRESSIVE_DETAIL_CORE_REGEX_DOMAIN_HPP_EAN_12_12_2009
#define HBOOST_XPRESSIVE_DETAIL_CORE_REGEX_DOMAIN_HPP_EAN_12_12_2009

#include <hboost/xpressive/xpressive_fwd.hpp>
#include <hboost/proto/traits.hpp>
#include <hboost/proto/domain.hpp>
#include <hboost/proto/generate.hpp>

namespace hboost { namespace xpressive { namespace detail
{
    struct regex_domain
      : proto::domain<proto::default_generator, proto::not_<proto::address_of<proto::_> > >
    {};
}}}

#endif
