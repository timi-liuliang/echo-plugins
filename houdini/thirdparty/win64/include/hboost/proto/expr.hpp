///////////////////////////////////////////////////////////////////////////////
/// \file expr.hpp
/// Contains definition of expr\<\> class template.
//
//  Copyright 2008 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_PROTO_EXPR_HPP_EAN_04_01_2005
#define HBOOST_PROTO_EXPR_HPP_EAN_04_01_2005

#include <hboost/preprocessor/cat.hpp>
#include <hboost/preprocessor/arithmetic/dec.hpp>
#include <hboost/preprocessor/selection/max.hpp>
#include <hboost/preprocessor/iteration/iterate.hpp>
#include <hboost/preprocessor/facilities/intercept.hpp>
#include <hboost/preprocessor/repetition/repeat.hpp>
#include <hboost/preprocessor/repetition/repeat_from_to.hpp>
#include <hboost/preprocessor/repetition/enum_trailing.hpp>
#include <hboost/preprocessor/repetition/enum_params.hpp>
#include <hboost/preprocessor/repetition/enum_binary_params.hpp>
#include <hboost/preprocessor/repetition/enum_trailing_params.hpp>
#include <hboost/preprocessor/repetition/enum_trailing_binary_params.hpp>
#include <hboost/utility/addressof.hpp>
#include <hboost/proto/proto_fwd.hpp>
#include <hboost/proto/args.hpp>
#include <hboost/proto/traits.hpp>

#if defined(_MSC_VER)
# pragma warning(push)
# pragma warning(disable : 4510) // default constructor could not be generated
# pragma warning(disable : 4512) // assignment operator could not be generated
# pragma warning(disable : 4610) // user defined constructor required
# pragma warning(disable : 4714) // function 'xxx' marked as __forceinline not inlined
#endif

namespace hboost { namespace proto
{

    namespace detail
    {
        struct not_a_valid_type
        {
        private:
            not_a_valid_type()
            {}
        };
        
        template<typename Tag, typename Arg>
        struct address_of_hack
        {
            typedef not_a_valid_type type;
        };

        template<typename Expr>
        struct address_of_hack<proto::tag::address_of, Expr &>
        {
            typedef Expr *type;
        };

        template<typename T, typename Expr, typename Arg0>
        HBOOST_FORCEINLINE
        Expr make_terminal(T &t, Expr *, proto::term<Arg0> *)
        {
            Expr that = {t};
            return that;
        }

        template<typename T, typename Expr, typename Arg0, std::size_t N>
        HBOOST_FORCEINLINE
        Expr make_terminal(T (&t)[N], Expr *, proto::term<Arg0[N]> *)
        {
            Expr that;
            for(std::size_t i = 0; i < N; ++i)
            {
                that.child0[i] = t[i];
            }
            return that;
        }

        template<typename T, typename Expr, typename Arg0, std::size_t N>
        HBOOST_FORCEINLINE
        Expr make_terminal(T const(&t)[N], Expr *, proto::term<Arg0[N]> *)
        {
            Expr that;
            for(std::size_t i = 0; i < N; ++i)
            {
                that.child0[i] = t[i];
            }
            return that;
        }

        // Work-around for:
        // https://connect.microsoft.com/VisualStudio/feedback/details/765449/codegen-stack-corruption-using-runtime-checks-when-aggregate-initializing-struct
    #if HBOOST_WORKAROUND(HBOOST_MSVC, HBOOST_TESTED_AT(1700))
        template<typename T, typename Expr, typename C, typename U>
        HBOOST_FORCEINLINE
        Expr make_terminal(T &t, Expr *, proto::term<U C::*> *)
        {
            Expr that;
            that.child0 = t;
            return that;
        }
    #endif

        template<typename T, typename U>
        struct same_cv
        {
            typedef U type;
        };

        template<typename T, typename U>
        struct same_cv<T const, U>
        {
            typedef U const type;
        };
    }

    namespace result_of
    {
        /// \brief A helper metafunction for computing the
        /// return type of \c proto::expr\<\>::operator().
        template<typename Sig, typename This, typename Domain>
        struct funop;

        #include <hboost/proto/detail/funop.hpp>
    }

    namespace exprns_
    {
        // This is where the basic_expr specializations are
        // actually defined:
        #include <hboost/proto/detail/basic_expr.hpp>

        // This is where the expr specialization are
        // actually defined:
        #include <hboost/proto/detail/expr.hpp>
    }

    /// \brief Lets you inherit the interface of an expression
    /// while hiding from Proto the fact that the type is a Proto
    /// expression.
    template<typename Expr>
    struct unexpr
      : Expr
    {
        HBOOST_PROTO_UNEXPR()

        HBOOST_FORCEINLINE
        explicit unexpr(Expr const &e)
          : Expr(e)
        {}
        
        using Expr::operator =;
    };

}}

#if defined(_MSC_VER)
# pragma warning(pop)
#endif

#endif // HBOOST_PROTO_EXPR_HPP_EAN_04_01_2005
