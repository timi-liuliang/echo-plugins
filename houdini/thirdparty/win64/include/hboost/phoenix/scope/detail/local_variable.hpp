/*=============================================================================
    Copyright (c) 2001-2007 Joel de Guzman
    Copyright (c) 2004 Daniel Wallin
    Copyright (c) 2011 Thomas Heller

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef PHOENIX_SCOPE_DETAIL_LOCAL_VARIABLE_HPP
#define PHOENIX_SCOPE_DETAIL_LOCAL_VARIABLE_HPP

#include <hboost/mpl/int.hpp>
#include <hboost/mpl/bool.hpp>
#include <hboost/mpl/eval_if.hpp>
#include <hboost/mpl/identity.hpp>
#include <hboost/fusion/include/at.hpp>
#include <hboost/fusion/include/value_at.hpp>
#include <hboost/preprocessor/enum.hpp>
#include <hboost/preprocessor/repeat.hpp>
#include <hboost/type_traits/remove_reference.hpp>
#include <hboost/type_traits/is_reference.hpp>

#define HBOOST_PHOENIX_MAP_LOCAL_TEMPLATE_PARAM(z, n, data) \
    typename T##n = unused<n>

#define HBOOST_PHOENIX_MAP_LOCAL_DISPATCH(z, n, data)  \
    typedef char(&result##n)[n+2];              \
    static result##n get(T##n*);

namespace hboost { namespace hboostphoenix
{
    template <typename Env, typename OuterEnv, typename Locals, typename Map>
    struct scoped_environment;

    namespace detail
    {
        template <typename Key>
        struct local
        {
            typedef Key key_type;
        };

        namespace result_of
        {
            template <typename Locals, typename Context>
            struct initialize_locals;
            
            template <typename Context>
            struct initialize_locals<vector0<>, Context>
            {
                typedef vector0<> type;
            };

        #define M1(Z, N, D)                                                     \
            typename hboost::hboostphoenix::result_of::eval<                           \
                HBOOST_PP_CAT(A, N)                                              \
              , Context                                                         \
            >::type                                                             \
        /**/

        #define M0(Z, N, D)                                                     \
            template <HBOOST_PHOENIX_typename_A(N), typename Context>            \
            struct initialize_locals<                                           \
                HBOOST_PP_CAT(vector, N)<                                        \
                    HBOOST_PHOENIX_A(N)                                          \
                >                                                               \
              , Context                                                         \
            >                                                                   \
            {                                                                   \
                typedef                                                         \
                    HBOOST_PP_CAT(vector, N)<                                    \
                        HBOOST_PP_ENUM(N, M1, _)                                 \
                    >                                                           \
                    type;                                                       \
            };                                                                  \
        /**/
            HBOOST_PP_REPEAT_FROM_TO(1, HBOOST_PHOENIX_LIMIT, M0, _)
        #undef M0
        }

        template <typename Context>
        vector0<>
        initialize_locals(vector0<> const &, Context const &)
        {
            vector0<> vars;
            return vars;
        }
    #define M2(Z, N, D)                                                         \
        eval(locals. HBOOST_PP_CAT(a, N), ctx)                                   \
    /**/
        
    #define M0(Z, N, D)                                                         \
        template <HBOOST_PHOENIX_typename_A(N), typename Context>                \
        HBOOST_PP_CAT(vector, N)<HBOOST_PP_ENUM(N, M1, _)>                        \
        initialize_locals(                                                      \
            HBOOST_PP_CAT(vector, N)<HBOOST_PHOENIX_A(N)> const & locals          \
          , Context const & ctx                                                 \
        )                                                                       \
        {                                                                       \
            HBOOST_PP_CAT(vector, N)<HBOOST_PP_ENUM(N, M1, _)> vars               \
                = {HBOOST_PP_ENUM(N, M2, _)};                                    \
            return vars;                                                        \
        }                                                                       \
    /**/
        HBOOST_PP_REPEAT_FROM_TO(1, HBOOST_PHOENIX_LIMIT, M0, _)
        #undef M0
        #undef M1
        #undef M2

        template <int N>
        struct unused;

        template <
            HBOOST_PP_ENUM(
                HBOOST_PHOENIX_LOCAL_LIMIT
              , HBOOST_PHOENIX_MAP_LOCAL_TEMPLATE_PARAM
              , _
            )
        >
        struct map_local_index_to_tuple
        {
            typedef char(&not_found)[1];
            static not_found get(...);

            HBOOST_PP_REPEAT(HBOOST_PHOENIX_LOCAL_LIMIT, HBOOST_PHOENIX_MAP_LOCAL_DISPATCH, _)
        };
        
        template<typename T>
        T* generate_pointer();

        template <typename Map, typename Tag>
        struct get_index
        {
            HBOOST_STATIC_CONSTANT(int,
                value = (
                    static_cast<int>((sizeof(Map::get(generate_pointer<Tag>()))) / sizeof(char)) - 2
                ));

            // if value == -1, Tag is not found
            typedef mpl::int_<value> type;
        };

        
        template <typename Local, typename Env>
        struct apply_local;

        template <typename Local, typename Env>
        struct outer_local
        {
            typedef typename
                apply_local<Local, typename Env::outer_env_type>::type
            type;
        };

        template <typename Locals, int Index>
        struct get_local_or_void
        {
            typedef typename
                mpl::eval_if_c<
                    Index < Locals::size_value
                  , fusion::result_of::at_c<Locals, Index>
                  , mpl::identity<fusion::void_>
                >::type
                type;
        };

        template <typename Local, typename Env, int Index>
        struct get_local_from_index
        {
            typedef typename
                mpl::eval_if_c<
                    Index == -1
                  , outer_local<Local, Env>
                  , get_local_or_void<typename Env::locals_type, Index>
                >::type
                type;
        };

        template <typename Local, typename Env>
        struct get_local
        {
            static const int index_value = get_index<typename Env::map_type, Local>::value;

            typedef typename
                get_local_from_index<Local, Env, index_value>::type
            type;
        };

        template <typename Local, typename Env>
        struct apply_local
        {
            // $$$ TODO: static assert that Env is a scoped_environment $$$
            typedef typename get_local<Local, Env>::type type;
        };

        template <typename Key>
        struct eval_local
        {
            template <typename RT, int Index, typename Env>
            static RT
            get(Env const& env, mpl::false_)
            {
                return RT(fusion::at_c<Index>(env.locals));
            }

            template <typename RT, int Index, typename Env>
            static RT
            get(Env const& env, mpl::true_)
            {
                static const int index_value = get_index<typename Env::outer_env_type::map_type, detail::local<Key> >::value;

                return get<RT, index_value>(
                    env.outer_env
                  , mpl::bool_<index_value == -1>());
            }

            template <typename RT, int Index, typename Env>
            static RT
            get(Env const& env)
            {
                return get<RT, Index>(
                    env
                  , mpl::bool_<Index == -1>());
            }
        };
    }
}}

#undef HBOOST_PHOENIX_MAP_LOCAL_TEMPLATE_PARAM
#undef HBOOST_PHOENIX_MAP_LOCAL_DISPATCH

#endif
