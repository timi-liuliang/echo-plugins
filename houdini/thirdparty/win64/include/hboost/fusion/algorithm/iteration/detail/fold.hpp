/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2006 Dan Marsden
    Copyright (c) 2009-2010 Christopher Schmidt
    Copyright (c) 2015 Kohei Takahashi

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#include <hboost/preprocessor/cat.hpp>

#define FUSION_HASH #

#ifdef HBOOST_FUSION_REVERSE_FOLD
#   ifdef HBOOST_FUSION_ITER_FOLD
#       define HBOOST_FUSION_FOLD_NAME reverse_iter_fold
#   else
#       define HBOOST_FUSION_FOLD_NAME reverse_fold
#   endif

#   define HBOOST_FUSION_FOLD_IMPL_FIRST_IT_FUNCTION end
#   define HBOOST_FUSION_FOLD_IMPL_NEXT_IT_FUNCTION prior
#   define HBOOST_FUSION_FOLD_IMPL_FIRST_IT_META_TRANSFORM(IT)                   \
        typename fusion::result_of::prior<IT>::type
#   define HBOOST_FUSION_FOLD_IMPL_FIRST_IT_TRANSFORM(IT) fusion::prior(IT)
#else
#   ifdef HBOOST_FUSION_ITER_FOLD
#       define HBOOST_FUSION_FOLD_NAME iter_fold
#   else
#       define HBOOST_FUSION_FOLD_NAME fold
#   endif

#   define HBOOST_FUSION_FOLD_IMPL_FIRST_IT_FUNCTION begin
#   define HBOOST_FUSION_FOLD_IMPL_NEXT_IT_FUNCTION next
#   define HBOOST_FUSION_FOLD_IMPL_FIRST_IT_META_TRANSFORM(IT) IT
#   define HBOOST_FUSION_FOLD_IMPL_FIRST_IT_TRANSFORM(IT) IT
#endif
#ifdef HBOOST_FUSION_ITER_FOLD
#   define HBOOST_FUSION_FOLD_IMPL_INVOKE_IT_META_TRANSFORM(IT) IT&
#   define HBOOST_FUSION_FOLD_IMPL_INVOKE_IT_TRANSFORM(IT) IT
#else
#   define HBOOST_FUSION_FOLD_IMPL_INVOKE_IT_META_TRANSFORM(IT)                  \
        typename fusion::result_of::deref<IT>::type
#   define HBOOST_FUSION_FOLD_IMPL_INVOKE_IT_TRANSFORM(IT) fusion::deref(IT)
#endif

#if (defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES))
FUSION_HASH if HBOOST_WORKAROUND HBOOST_PREVENT_MACRO_SUBSTITUTION (HBOOST_MSVC, < 1500)
FUSION_HASH     define HBOOST_FUSION_FOLD_IMPL_ENABLER(T) void
FUSION_HASH else
FUSION_HASH     define HBOOST_FUSION_FOLD_IMPL_ENABLER(T) typename T::type
FUSION_HASH endif
#else
#   if HBOOST_WORKAROUND(HBOOST_MSVC, < 1500)
#       define HBOOST_FUSION_FOLD_IMPL_ENABLER(T) void
#   else
#       define HBOOST_FUSION_FOLD_IMPL_ENABLER(T) typename T::type
#   endif
#endif

namespace hboost { namespace fusion
{
    namespace detail
    {
        template<int SeqSize, typename It, typename State, typename F, typename = void
#if defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES)
FUSION_HASH if HBOOST_WORKAROUND HBOOST_PREVENT_MACRO_SUBSTITUTION (HBOOST_MSVC, < 1500)
#endif
#if HBOOST_WORKAROUND(HBOOST_MSVC, < 1500) || \
    (defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES))
          // Dirty hack: those compilers cannot choose exactly one partial specialization.
          , bool = SeqSize == 0
#endif
#if defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES)
FUSION_HASH endif
#endif
        >
        struct HBOOST_PP_CAT(result_of_it_,HBOOST_FUSION_FOLD_NAME)
        {};

        template<typename It, typename State, typename F>
        struct HBOOST_PP_CAT(result_of_it_,HBOOST_FUSION_FOLD_NAME)<0,It,State,F
          , typename hboost::enable_if_has_type<HBOOST_FUSION_FOLD_IMPL_ENABLER(State)>::type
#if defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES)
FUSION_HASH if HBOOST_WORKAROUND HBOOST_PREVENT_MACRO_SUBSTITUTION (HBOOST_MSVC, < 1500)
#endif
#if HBOOST_WORKAROUND(HBOOST_MSVC, < 1500) || \
    (defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES))
          , true
#endif
#if defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES)
FUSION_HASH endif
#endif
          >
        {
            typedef typename State::type type;
        };

        template<int SeqSize, typename It, typename State, typename F>
        struct HBOOST_PP_CAT(result_of_it_,HBOOST_FUSION_FOLD_NAME)<SeqSize,It,State,F
          , typename hboost::enable_if_has_type<
#if defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES)
FUSION_HASH if HBOOST_WORKAROUND HBOOST_PREVENT_MACRO_SUBSTITUTION (HBOOST_MSVC, >= 1500)
#endif
#if HBOOST_WORKAROUND(HBOOST_MSVC, >= 1500) || \
    (defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES))
                // Following SFINAE enables to avoid MSVC 9's partial specialization
                // ambiguous bug but MSVC 8 don't compile, and moreover MSVC 8 style
                // workaround won't work with MSVC 9.
                typename hboost::disable_if_c<SeqSize == 0, State>::type::type
#if defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES)
FUSION_HASH else
                HBOOST_FUSION_FOLD_IMPL_ENABLER(State)
#endif
#else
                HBOOST_FUSION_FOLD_IMPL_ENABLER(State)
#endif
#if defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES)
FUSION_HASH endif
#endif
            >::type
#if defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES)
FUSION_HASH if HBOOST_WORKAROUND HBOOST_PREVENT_MACRO_SUBSTITUTION (HBOOST_MSVC, < 1500)
#endif
#if HBOOST_WORKAROUND(HBOOST_MSVC, < 1500) || \
    (defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES))
          , false
#endif
#if defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES)
FUSION_HASH endif
#endif
          >
          : HBOOST_PP_CAT(result_of_it_,HBOOST_FUSION_FOLD_NAME)<
                SeqSize-1
              , typename result_of::HBOOST_FUSION_FOLD_IMPL_NEXT_IT_FUNCTION<It>::type
              , hboost::result_of<
                    F(
                        typename add_reference<typename State::type>::type,
                        HBOOST_FUSION_FOLD_IMPL_INVOKE_IT_META_TRANSFORM(It const)
                    )
                >
              , F
            >
        {};

        template<typename It, typename State, typename F>
        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        inline typename HBOOST_PP_CAT(result_of_it_,HBOOST_FUSION_FOLD_NAME)<
            0
          , It
          , State
          , F
        >::type
        HBOOST_PP_CAT(it_,HBOOST_FUSION_FOLD_NAME)(mpl::int_<0>, It const&, typename State::type state, F&)
        {
            return state;
        }

        template<typename It, typename State, typename F, int SeqSize>
        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        inline typename lazy_enable_if_c<
            SeqSize != 0
          , HBOOST_PP_CAT(result_of_it_,HBOOST_FUSION_FOLD_NAME)<
                SeqSize
              , It
              , State
              , F
            >
        >::type
        HBOOST_PP_CAT(it_,HBOOST_FUSION_FOLD_NAME)(mpl::int_<SeqSize>, It const& it, typename State::type state, F& f)
        {
            return HBOOST_PP_CAT(it_,HBOOST_FUSION_FOLD_NAME)<
                typename result_of::HBOOST_FUSION_FOLD_IMPL_NEXT_IT_FUNCTION<It>::type
              , hboost::result_of<
                    F(
                        typename add_reference<typename State::type>::type,
                        HBOOST_FUSION_FOLD_IMPL_INVOKE_IT_META_TRANSFORM(It const)
                    )
                >
              , F
            >(
                mpl::int_<SeqSize-1>()
              , fusion::HBOOST_FUSION_FOLD_IMPL_NEXT_IT_FUNCTION(it)
              , f(state, HBOOST_FUSION_FOLD_IMPL_INVOKE_IT_TRANSFORM(it))
              , f
            );
        }

        template<typename Seq, typename State, typename F
          , bool = traits::is_sequence<Seq>::value
          , bool = traits::is_segmented<Seq>::value>
        struct HBOOST_PP_CAT(result_of_,HBOOST_FUSION_FOLD_NAME)
        {};

        template<typename Seq, typename State, typename F>
        struct HBOOST_PP_CAT(result_of_,HBOOST_FUSION_FOLD_NAME)<Seq, State, F, true, false>
          : HBOOST_PP_CAT(result_of_it_,HBOOST_FUSION_FOLD_NAME)<
                result_of::size<Seq>::value
              , HBOOST_FUSION_FOLD_IMPL_FIRST_IT_META_TRANSFORM(
                    typename result_of::HBOOST_FUSION_FOLD_IMPL_FIRST_IT_FUNCTION<Seq>::type
                )
              , add_reference<State>
              , F
            >
        {};

        template<typename Seq, typename State, typename F>
        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        inline typename HBOOST_PP_CAT(result_of_,HBOOST_FUSION_FOLD_NAME)<Seq, State, F>::type
        HBOOST_FUSION_FOLD_NAME(Seq& seq, State& state, F& f)
        {
            return HBOOST_PP_CAT(it_,HBOOST_FUSION_FOLD_NAME)<
                HBOOST_FUSION_FOLD_IMPL_FIRST_IT_META_TRANSFORM(
                    typename result_of::HBOOST_FUSION_FOLD_IMPL_FIRST_IT_FUNCTION<Seq>::type
                )
              , add_reference<State>
              , F
            >(
                typename result_of::size<Seq>::type()
              , HBOOST_FUSION_FOLD_IMPL_FIRST_IT_TRANSFORM(
                    fusion::HBOOST_FUSION_FOLD_IMPL_FIRST_IT_FUNCTION(seq)
                )
              , state
              , f
            );
        }
    }

    namespace result_of
    {
        template<typename Seq, typename State, typename F>
        struct HBOOST_FUSION_FOLD_NAME
          : detail::HBOOST_PP_CAT(result_of_,HBOOST_FUSION_FOLD_NAME)<Seq, State, F>
        {};
    }

    template<typename Seq, typename State, typename F>
    HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline typename result_of::HBOOST_FUSION_FOLD_NAME<
        Seq
      , State const
      , F
    >::type
    HBOOST_FUSION_FOLD_NAME(Seq& seq, State const& state, F f)
    {
        return detail::HBOOST_FUSION_FOLD_NAME<Seq, State const, F>(seq, state, f);
    }

    template<typename Seq, typename State, typename F>
    HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline typename result_of::HBOOST_FUSION_FOLD_NAME<
        Seq const
      , State const
      , F
    >::type
    HBOOST_FUSION_FOLD_NAME(Seq const& seq, State const& state, F f)
    {
        return detail::HBOOST_FUSION_FOLD_NAME<Seq const, State const, F>(seq, state, f);
    }

    template<typename Seq, typename State, typename F>
    HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline typename result_of::HBOOST_FUSION_FOLD_NAME<
        Seq
      , State
      , F
    >::type
    HBOOST_FUSION_FOLD_NAME(Seq& seq, State& state, F f)
    {
        return detail::HBOOST_FUSION_FOLD_NAME<Seq, State, F>(seq, state, f);
    }

    template<typename Seq, typename State, typename F>
    HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline typename result_of::HBOOST_FUSION_FOLD_NAME<
        Seq const
      , State
      , F
    >::type
    HBOOST_FUSION_FOLD_NAME(Seq const& seq, State& state, F f)
    {
        return detail::HBOOST_FUSION_FOLD_NAME<Seq const, State, F>(seq, state, f);
    }
}}

#undef HBOOST_FUSION_FOLD_NAME
#undef HBOOST_FUSION_FOLD_IMPL_ENABLER
#undef HBOOST_FUSION_FOLD_IMPL_FIRST_IT_FUNCTION
#undef HBOOST_FUSION_FOLD_IMPL_NEXT_IT_FUNCTION
#undef HBOOST_FUSION_FOLD_IMPL_FIRST_IT_META_TRANSFORM
#undef HBOOST_FUSION_FOLD_IMPL_FIRST_IT_TRANSFORM
#undef HBOOST_FUSION_FOLD_IMPL_INVOKE_IT_META_TRANSFORM
#undef HBOOST_FUSION_FOLD_IMPL_INVOKE_IT_TRANSFORM
#undef FUSION_HASH
