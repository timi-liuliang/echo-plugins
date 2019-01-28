//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : runtime parameters initialization final step
// ***************************************************************************

#ifndef HBOOST_TEST_UTILS_RUNTIME_FINALIZE_HPP
#define HBOOST_TEST_UTILS_RUNTIME_FINALIZE_HPP

// Boost.Test Runtime parameters
#include <hboost/test/utils/runtime/parameter.hpp>
#include <hboost/test/utils/runtime/argument.hpp>

// Boost.Test
#include <hboost/test/utils/foreach.hpp>

#include <hboost/test/detail/suppress_warnings.hpp>

namespace hboost {
namespace runtime {

inline void
finalize_arguments( parameters_store const& params, runtime::arguments_store& args )
{
    HBOOST_TEST_FOREACH( parameters_store::storage_type::value_type const&, v, params.all() ) {
        basic_param_ptr param = v.second;

        if( !args.has( param->p_name ) ) {
            if( param->p_has_default_value )
                param->produce_default( args );

            if( !args.has( param->p_name ) ) {
                HBOOST_TEST_I_ASSRT( param->p_optional,
                    missing_req_arg( param->p_name ) << "Missing argument for required parameter " << param->p_name << "." );
            }
        }

        if( args.has( param->p_name ) && !!param->p_callback )
            param->p_callback( param->p_name );
    }
}

} // namespace runtime
} // namespace hboost

#include <hboost/test/detail/enable_warnings.hpp>

#endif // HBOOST_TEST_UTILS_RUNTIME_FINALIZE_HPP
