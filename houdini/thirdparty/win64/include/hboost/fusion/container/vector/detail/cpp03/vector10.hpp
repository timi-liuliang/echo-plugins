/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_VECTOR10_05042005_0257)
#define HBOOST_FUSION_VECTOR10_05042005_0257

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/container/vector/detail/cpp03/vector10_fwd.hpp>
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
#include <hboost/mpl/bool.hpp>
#include <hboost/mpl/at.hpp>
#include <hboost/mpl/vector/vector10.hpp>
#include <hboost/type_traits/is_convertible.hpp>
#include <hboost/utility/enable_if.hpp>

#include <hboost/preprocessor/dec.hpp>
#include <hboost/preprocessor/iteration/iterate.hpp>
#include <hboost/preprocessor/repetition/enum.hpp>
#include <hboost/preprocessor/repetition/enum_shifted.hpp>
#include <hboost/preprocessor/repetition/enum_params.hpp>
#include <hboost/preprocessor/repetition/enum_binary_params.hpp>
#include <hboost/preprocessor/repetition/repeat_from_to.hpp>

namespace hboost { namespace fusion
{
    struct vector_tag;
    struct fusion_sequence_tag;
    struct random_access_traversal_tag;

    template <typename Dummy>
    struct vector0 : sequence_base<vector0<Dummy> >
    {
        typedef mpl::vector0<> types;
        typedef vector_tag fusion_tag;
        typedef fusion_sequence_tag tag; // this gets picked up by MPL
        typedef mpl::false_ is_view;
        typedef random_access_traversal_tag category;
        typedef mpl::int_<0> size;

        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        vector0() HBOOST_NOEXCEPT {}

        template<typename Sequence>
        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        vector0(Sequence const& /*seq*/) HBOOST_NOEXCEPT
        {}
    };
}}

#if !defined(HBOOST_FUSION_DONT_USE_PREPROCESSED_FILES)
#include <hboost/fusion/container/vector/detail/cpp03/preprocessed/vector10.hpp>
#else
#if defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/vector10.hpp")
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
// expand vector1 to vector10
#define HBOOST_PP_FILENAME_1 <hboost/fusion/container/vector/detail/cpp03/vector_n.hpp>
#define HBOOST_PP_ITERATION_LIMITS (1, 10)
#include HBOOST_PP_ITERATE()
#undef FUSION_HASH
}}

#if defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif // HBOOST_FUSION_DONT_USE_PREPROCESSED_FILES

#endif
