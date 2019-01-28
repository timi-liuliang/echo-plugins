/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_VECTOR_07072005_1244)
#define HBOOST_FUSION_VECTOR_07072005_1244

#include <hboost/preprocessor/iterate.hpp>
#include <hboost/preprocessor/repetition/enum_params.hpp>
#include <hboost/preprocessor/repetition/enum_binary_params.hpp>
#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/support/void.hpp>
#include <hboost/fusion/support/detail/enabler.hpp>
#include <hboost/fusion/support/is_sequence.hpp>
#include <hboost/fusion/container/vector/vector_fwd.hpp>
#include <hboost/fusion/container/vector/detail/cpp03/vector_n_chooser.hpp>
#include <hboost/fusion/sequence/intrinsic/begin.hpp>
#include <hboost/mpl/at.hpp>
#include <hboost/mpl/bool.hpp>
#include <hboost/type_traits/add_reference.hpp>
#include <hboost/type_traits/add_const.hpp>
#include <hboost/type_traits/is_base_of.hpp>
#include <hboost/detail/workaround.hpp>
#include <hboost/core/enable_if.hpp>

#define FUSION_HASH #

#if HBOOST_WORKAROUND(HBOOST_MSVC, <= 1600)

#define HBOOST_FUSION_VECTOR_COPY_INIT()                                       \
    ctor_helper(rhs, is_base_of<vector, Sequence>())                          \

#define HBOOST_FUSION_VECTOR_CTOR_HELPER()                                     \
    HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED                                  \
    static vector_n const&                                                    \
    ctor_helper(vector const& rhs, mpl::true_)                                \
    {                                                                         \
        return rhs.vec;                                                       \
    }                                                                         \
                                                                              \
    template <typename T>                                                     \
    HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED                                  \
    static T const&                                                           \
    ctor_helper(T const& rhs, mpl::false_)                                    \
    {                                                                         \
        return rhs;                                                           \
    }

#else

#define HBOOST_FUSION_VECTOR_COPY_INIT()                                       \
    rhs                                                                       \

#define HBOOST_FUSION_VECTOR_CTOR_HELPER()

#endif

#if !defined(HBOOST_FUSION_DONT_USE_PREPROCESSED_FILES)
#include <hboost/fusion/container/vector/detail/cpp03/preprocessed/vector.hpp>
#else
#if defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/vvector" FUSION_MAX_VECTOR_SIZE_STR ".hpp")
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
    struct void_;
    struct fusion_sequence_tag;

    template <HBOOST_PP_ENUM_PARAMS(FUSION_MAX_VECTOR_SIZE, typename T)>
    struct vector
        : sequence_base<vector<HBOOST_PP_ENUM_PARAMS(FUSION_MAX_VECTOR_SIZE, T)> >
    {
    private:

        typedef typename detail::vector_n_chooser<
            HBOOST_PP_ENUM_PARAMS(FUSION_MAX_VECTOR_SIZE, T)>::type
        vector_n;

        template <HBOOST_PP_ENUM_PARAMS(FUSION_MAX_VECTOR_SIZE, typename U)>
        friend struct vector;

    public:

        typedef typename vector_n::types types;
        typedef typename vector_n::fusion_tag fusion_tag;
        typedef typename vector_n::tag tag;
        typedef typename vector_n::size size;
        typedef typename vector_n::category category;
        typedef typename vector_n::is_view is_view;

        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        vector()
            : vec() {}

        template <HBOOST_PP_ENUM_PARAMS(FUSION_MAX_VECTOR_SIZE, typename U)>
        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        vector(vector<HBOOST_PP_ENUM_PARAMS(FUSION_MAX_VECTOR_SIZE, U)> const& rhs)
            : vec(rhs.vec) {}

        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        vector(vector const& rhs)
            : vec(rhs.vec) {}

        template <typename Sequence>
        HBOOST_FUSION_GPU_ENABLED
        vector(Sequence const& rhs,
            typename enable_if<traits::is_sequence<Sequence>, detail::enabler_>::type = detail::enabler)
            : vec(HBOOST_FUSION_VECTOR_COPY_INIT()) {}

        //  Expand a couple of forwarding constructors for arguments
        //  of type (T0), (T0, T1), (T0, T1, T2) etc. Example:
        //
        //  vector(
        //      typename detail::call_param<T0>::type arg0
        //    , typename detail::call_param<T1>::type arg1)
        //    : vec(arg0, arg1) {}
        #include <hboost/fusion/container/vector/detail/cpp03/vector_forward_ctor.hpp>

        template <HBOOST_PP_ENUM_PARAMS(FUSION_MAX_VECTOR_SIZE, typename U)>
        HBOOST_CXX14_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        vector&
        operator=(vector<HBOOST_PP_ENUM_PARAMS(FUSION_MAX_VECTOR_SIZE, U)> const& rhs)
        {
            vec = rhs.vec;
            return *this;
        }

        template <typename T>
        HBOOST_CXX14_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        vector&
        operator=(T const& rhs)
        {
            vec = rhs;
            return *this;
        }

        HBOOST_CXX14_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        vector&
        operator=(vector const& rhs)
        {
            vec = rhs.vec;
            return *this;
        }

#if defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES)
FUSION_HASH if !defined(HBOOST_NO_CXX11_RVALUE_REFERENCES)
#endif
#if !defined(HBOOST_NO_CXX11_RVALUE_REFERENCES) || \
    (defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES))
        HBOOST_CXX14_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        vector(vector&& rhs)
            : vec(std::forward<vector_n>(rhs.vec)) {}

        HBOOST_CXX14_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        vector&
        operator=(vector&& rhs)
        {
            vec = std::forward<vector_n>(rhs.vec);
            return *this;
        }

        template <typename T>
        HBOOST_CXX14_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        vector&
        operator=(T&& rhs)
        {
            vec = HBOOST_FUSION_FWD_ELEM(T, rhs);
            return *this;
        }
#endif
#if defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES)
FUSION_HASH endif
#endif

        template <int N>
        HBOOST_CXX14_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        typename add_reference<
            typename mpl::at_c<types, N>::type
        >::type
        at_impl(mpl::int_<N> index)
        {
            return vec.at_impl(index);
        }

        template <int N>
        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        typename add_reference<
            typename add_const<
                typename mpl::at_c<types, N>::type
            >::type
        >::type
        at_impl(mpl::int_<N> index) const
        {
            return vec.at_impl(index);
        }

        template <typename I>
        HBOOST_CXX14_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        typename add_reference<
            typename mpl::at<types, I>::type
        >::type
        at_impl(I /*index*/)
        {
            return vec.at_impl(mpl::int_<I::value>());
        }

        template<typename I>
        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        typename add_reference<
            typename add_const<
                typename mpl::at<types, I>::type
            >::type
        >::type
        at_impl(I /*index*/) const
        {
            return vec.at_impl(mpl::int_<I::value>());
        }

    private:

        HBOOST_FUSION_VECTOR_CTOR_HELPER()
        vector_n vec;
    };
}}

#if defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif // HBOOST_FUSION_DONT_USE_PREPROCESSED_FILES

#undef FUSION_HASH
#endif
