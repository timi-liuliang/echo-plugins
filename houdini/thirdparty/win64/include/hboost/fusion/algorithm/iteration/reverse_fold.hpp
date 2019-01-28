/*=============================================================================
    Copyright (c) 2010 Christopher Schmidt
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2015 Kohei Takahashi

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef HBOOST_FUSION_ALGORITHM_ITERATION_REVERSE_FOLD_HPP
#define HBOOST_FUSION_ALGORITHM_ITERATION_REVERSE_FOLD_HPP

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/algorithm/iteration/reverse_fold_fwd.hpp>
#include <hboost/config.hpp>
#include <hboost/fusion/sequence/intrinsic/end.hpp>
#include <hboost/fusion/sequence/intrinsic/size.hpp>
#include <hboost/fusion/support/is_segmented.hpp>
#include <hboost/fusion/support/is_sequence.hpp>
#include <hboost/fusion/iterator/deref.hpp>
#include <hboost/fusion/iterator/value_of.hpp>
#include <hboost/fusion/iterator/prior.hpp>
#include <hboost/utility/result_of.hpp>
#include <hboost/core/enable_if.hpp>
#include <hboost/type_traits/add_reference.hpp>

#define HBOOST_FUSION_REVERSE_FOLD

#if !defined(HBOOST_FUSION_DONT_USE_PREPROCESSED_FILES)
#include <hboost/fusion/algorithm/iteration/detail/preprocessed/reverse_fold.hpp>
#else
#if defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "detail/preprocessed/reverse_fold.hpp")
#endif

/*=============================================================================
    Copyright (c) 2009-2010 Christopher Schmidt
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    This is an auto-generated file. Do not edit!
==============================================================================*/

#if defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 1)
#endif

#include <hboost/fusion/algorithm/iteration/detail/fold.hpp>

#if defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif // HBOOST_FUSION_DONT_USE_PREPROCESSED_FILES

#undef HBOOST_FUSION_REVERSE_FOLD

#endif
