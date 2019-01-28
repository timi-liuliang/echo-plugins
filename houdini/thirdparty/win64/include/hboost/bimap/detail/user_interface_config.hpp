// Boost.Bimap
//
// Copyright (c) 2006-2007 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file detail/user_interface_config.hpp
/// \brief General configuration directives


#ifndef HBOOST_BIMAP_DETAIL_USER_INTERFACE_CONFIG_HPP
#define HBOOST_BIMAP_DETAIL_USER_INTERFACE_CONFIG_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#ifdef HBOOST_BIMAP_DISABLE_SERIALIZATION
    #define HBOOST_MULTI_INDEX_DISABLE_SERIALIZATION
#endif

#endif // HBOOST_BIMAP_DETAIL_USER_INTERFACE_CONFIG_HPP
