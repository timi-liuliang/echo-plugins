/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_VECTOR40_05052005_0208)
#define HBOOST_FUSION_VECTOR40_05052005_0208

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/container/vector/detail/cpp03/vector40_fwd.hpp>
#include <hboost/fusion/support/sequence_base.hpp>
#include <hboost/fusion/support/is_sequence.hpp>
#include <hboost/fusion/support/detail/access.hpp>
#include <hboost/fusion/iterator/next.hpp>
#include <hboost/fusion/iterator/deref.hpp>
#include <hboost/fusion/sequence/intrinsic/begin.hpp>
#include <hboost/fusion/container/vector/detail/at_impl.hpp>
#include <hboost/fusion/container/vector/detail/value_at_impl.hpp>
#include <hboost/fusion/container/vector/detail/begin_impl.hpp>
#include <hboost/fusion/container/vector/detail/end_impl.hpp>

#include <hboost/mpl/void.hpp>
#include <hboost/mpl/int.hpp>
#include <hboost/mpl/at.hpp>
#include <hboost/mpl/bool.hpp>
#include <hboost/mpl/vector/vector40.hpp>
#include <hboost/type_traits/is_convertible.hpp>
#include <hboost/utility/enable_if.hpp>

#include <hboost/preprocessor/dec.hpp>
#include <hboost/preprocessor/iteration/iterate.hpp>
#include <hboost/preprocessor/repetition/enum.hpp>
#include <hboost/preprocessor/repetition/enum_shifted.hpp>
#include <hboost/preprocessor/repetition/enum_params.hpp>
#include <hboost/preprocessor/repetition/enum_binary_params.hpp>
#include <hboost/preprocessor/repetition/repeat_from_to.hpp>

#if !defined(HBOOST_FUSION_DONT_USE_PREPROCESSED_FILES)
#include <hboost/fusion/container/vector/detail/cpp03/preprocessed/vector40.hpp>
#else
#if defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/vector40.hpp")
#endif

/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    This is an auto-generated file. Do not edit!
==============================================================================*/

#if defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 1)
#endif

namespace hboost { namespace fusion
{
    struct vector_tag;
    struct fusion_sequence_tag;
    struct random_access_traversal_tag;

#define FUSION_HASH #
// expand vector31 to vector40
#define HBOOST_PP_FILENAME_1 <hboost/fusion/container/vector/detail/cpp03/vector_n.hpp>
#define HBOOST_PP_ITERATION_LIMITS (31, 40)
#include HBOOST_PP_ITERATE()
#undef FUSION_HASH
}}

#if defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif // HBOOST_FUSION_DONT_USE_PREPROCESSED_FILES

#endif

