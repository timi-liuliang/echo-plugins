///////////////////////////////////////////////////////////////////////////////
/// \file debug.hpp
/// Utilities for debugging Proto expression trees
//
//  Copyright 2008 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_PROTO_DEBUG_HPP_EAN_12_31_2006
#define HBOOST_PROTO_DEBUG_HPP_EAN_12_31_2006

#include <iostream>
#include <hboost/preprocessor/stringize.hpp>
#include <hboost/ref.hpp>
#include <hboost/mpl/assert.hpp>
#include <hboost/proto/proto_fwd.hpp>
#include <hboost/proto/traits.hpp>
#include <hboost/proto/matches.hpp>
#include <hboost/proto/fusion.hpp>
#include <hboost/fusion/algorithm/iteration/for_each.hpp>
#include <hboost/detail/sp_typeinfo.hpp>

namespace hboost { namespace proto
{
    namespace tagns_ { namespace tag
    {
    #define HBOOST_PROTO_DEFINE_TAG_INSERTION(Tag)                               \
        /** \brief INTERNAL ONLY */                                             \
        inline std::ostream &operator <<(std::ostream &sout, Tag const &)       \
        {                                                                       \
            return sout << HBOOST_PP_STRINGIZE(Tag);                             \
        }                                                                       \
        /**/

        HBOOST_PROTO_DEFINE_TAG_INSERTION(terminal)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(unary_plus)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(negate)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(dereference)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(complement)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(address_of)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(logical_not)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(pre_inc)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(pre_dec)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(post_inc)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(post_dec)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(shift_left)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(shift_right)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(multiplies)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(divides)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(modulus)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(plus)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(minus)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(less)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(greater)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(less_equal)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(greater_equal)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(equal_to)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(not_equal_to)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(logical_or)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(logical_and)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(bitwise_and)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(bitwise_or)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(bitwise_xor)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(comma)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(mem_ptr)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(assign)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(shift_left_assign)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(shift_right_assign)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(multiplies_assign)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(divides_assign)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(modulus_assign)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(plus_assign)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(minus_assign)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(bitwise_and_assign)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(bitwise_or_assign)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(bitwise_xor_assign)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(subscript)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(member)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(if_else_)
        HBOOST_PROTO_DEFINE_TAG_INSERTION(function)

    #undef HBOOST_PROTO_DEFINE_TAG_INSERTION
    }}

    namespace hidden_detail_
    {
        struct ostream_wrapper
        {
            ostream_wrapper(std::ostream &sout)
              : sout_(sout)
            {}

            std::ostream &sout_;

        private:
            ostream_wrapper &operator =(ostream_wrapper const &);
        };

        struct named_any
        {
            template<typename T>
            named_any(T const &)
              : name_(HBOOST_SP_TYPEID(T).name())
            {}

            char const *name_;
        };

        inline std::ostream &operator <<(ostream_wrapper sout_wrap, named_any t)
        {
            return sout_wrap.sout_ << t.name_;
        }
    }

    namespace detail
    {
        struct display_expr_impl
        {
            explicit display_expr_impl(std::ostream &sout, int depth = 0)
              : depth_(depth)
              , first_(true)
              , sout_(sout)
            {}

            template<typename Expr>
            void operator()(Expr const &expr) const
            {
                this->impl(expr, mpl::long_<arity_of<Expr>::value>());
            }

        private:
            display_expr_impl(display_expr_impl const &);
            display_expr_impl &operator =(display_expr_impl const &);

            template<typename Expr>
            void impl(Expr const &expr, mpl::long_<0>) const
            {
                using namespace hidden_detail_;
                typedef typename tag_of<Expr>::type tag;
                this->sout_.width(this->depth_);
                this->sout_ << (this->first_? "" : ", ");
                this->sout_ << tag() << "(" << proto::value(expr) << ")\n";
                this->first_ = false;
            }

            template<typename Expr, typename Arity>
            void impl(Expr const &expr, Arity) const
            {
                using namespace hidden_detail_;
                typedef typename tag_of<Expr>::type tag;
                this->sout_.width(this->depth_);
                this->sout_ << (this->first_? "" : ", ");
                this->sout_ << tag() << "(\n";
                display_expr_impl display(this->sout_, this->depth_ + 4);
                fusion::for_each(expr, display);
                this->sout_.width(this->depth_);
                this->sout_ << "" << ")\n";
                this->first_ = false;
            }

            int depth_;
            mutable bool first_;
            std::ostream &sout_;
        };
    }

    namespace functional
    {
        /// \brief Pretty-print a Proto expression tree.
        ///
        /// A PolymorphicFunctionObject which accepts a Proto expression
        /// tree and pretty-prints it to an \c ostream for debugging
        /// purposes.
        struct display_expr
        {
            HBOOST_PROTO_CALLABLE()

            typedef void result_type;

            /// \param sout  The \c ostream to which the expression tree
            ///              will be written.
            /// \param depth The starting indentation depth for this node.
            ///              Children nodes will be displayed at a starting
            ///              depth of <tt>depth+4</tt>.
            explicit display_expr(std::ostream &sout = std::cout, int depth = 0)
              : depth_(depth)
              , sout_(sout)
            {}

            /// \brief Pretty-print the current node in a Proto expression
            /// tree.
            template<typename Expr>
            void operator()(Expr const &expr) const
            {
                detail::display_expr_impl(this->sout_, this->depth_)(expr);
            }

        private:
            int depth_;
            reference_wrapper<std::ostream> sout_;
        };
    }

    /// \brief Pretty-print a Proto expression tree.
    ///
    /// \note Equivalent to <tt>functional::display_expr(0, sout)(expr)</tt>
    /// \param expr The Proto expression tree to pretty-print
    /// \param sout The \c ostream to which the output should be
    ///             written. If not specified, defaults to
    ///             <tt>std::cout</tt>.
    template<typename Expr>
    void display_expr(Expr const &expr, std::ostream &sout)
    {
        functional::display_expr(sout, 0)(expr);
    }

    /// \overload
    ///
    template<typename Expr>
    void display_expr(Expr const &expr)
    {
        functional::display_expr()(expr);
    }

    /// \brief Assert at compile time that a particular expression
    ///        matches the specified grammar.
    ///
    /// \note Equivalent to <tt>HBOOST_MPL_ASSERT((proto::matches\<Expr, Grammar\>))</tt>
    /// \param expr The Proto expression to check againts <tt>Grammar</tt>
    template<typename Grammar, typename Expr>
    void assert_matches(Expr const & /*expr*/)
    {
        HBOOST_MPL_ASSERT((proto::matches<Expr, Grammar>));
    }

    /// \brief Assert at compile time that a particular expression
    ///        does not match the specified grammar.
    ///
    /// \note Equivalent to <tt>HBOOST_MPL_ASSERT_NOT((proto::matches\<Expr, Grammar\>))</tt>
    /// \param expr The Proto expression to check againts <tt>Grammar</tt>
    template<typename Grammar, typename Expr>
    void assert_matches_not(Expr const & /*expr*/)
    {
        HBOOST_MPL_ASSERT_NOT((proto::matches<Expr, Grammar>));
    }

    /// \brief Assert at compile time that a particular expression
    ///        matches the specified grammar.
    ///
    /// \note Equivalent to <tt>proto::assert_matches\<Grammar\>(Expr)</tt>
    /// \param Expr The Proto expression to check againts <tt>Grammar</tt>
    /// \param Grammar The grammar used to validate Expr.
    #define HBOOST_PROTO_ASSERT_MATCHES(Expr, Grammar)                                               \
        (true ? (void)0 : hboost::proto::assert_matches<Grammar>(Expr))

    /// \brief Assert at compile time that a particular expression
    ///        does not match the specified grammar.
    ///
    /// \note Equivalent to <tt>proto::assert_matches_not\<Grammar\>(Expr)</tt>
    /// \param Expr The Proto expression to check againts <tt>Grammar</tt>
    /// \param Grammar The grammar used to validate Expr.
    #define HBOOST_PROTO_ASSERT_MATCHES_NOT(Expr, Grammar)                                           \
        (true ? (void)0 : hboost::proto::assert_matches_not<Grammar>(Expr))

}}

#endif
