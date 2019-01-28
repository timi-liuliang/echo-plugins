// Copyright (C) 2004, 2005 Arkadiy Vertleyb
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

// Inclusion of this file increments HBOOST_TYPEOF_REGISTRATION_GROUP 
// This method was suggested by Paul Mensonides

#ifdef HBOOST_TYPEOF_EMULATION
#   undef HBOOST_TYPEOF_REGISTRATION_GROUP

#   include <hboost/preprocessor/slot/counter.hpp>
#   include HBOOST_PP_UPDATE_COUNTER()
#   define HBOOST_TYPEOF_REGISTRATION_GROUP HBOOST_PP_COUNTER
#endif
