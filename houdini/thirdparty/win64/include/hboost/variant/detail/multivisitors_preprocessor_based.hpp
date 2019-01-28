//  Boost.Varaint
//  Contains multivisitors that are implemented via preprocessor magic
//
//  See http://www.boost.org for most recent version, including documentation.
//
//  Copyright Antony Polukhin, 2013-2014.
//
//  Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt).

#ifndef HBOOST_VARIANT_DETAIL_MULTIVISITORS_PREPROCESSOR_BASED_HPP
#define HBOOST_VARIANT_DETAIL_MULTIVISITORS_PREPROCESSOR_BASED_HPP

#if defined(_MSC_VER)
# pragma once
#endif

#include <hboost/variant.hpp>
#include <hboost/bind.hpp>

#include <hboost/preprocessor/repetition.hpp>
#include <hboost/preprocessor/punctuation/comma_if.hpp>
#include <hboost/preprocessor/arithmetic/add.hpp>
#include <hboost/preprocessor/arithmetic/sub.hpp>

#ifndef HBOOST_VARAINT_MAX_MULTIVIZITOR_PARAMS
#   define HBOOST_VARAINT_MAX_MULTIVIZITOR_PARAMS 4
#endif

namespace hboost { 

namespace detail { namespace variant {

    template <class VisitorT, class Visitable1T, class Visitable2T>
    struct two_variables_holder {
    private:
        VisitorT&       visitor_;
        Visitable1T&    visitable1_;
        Visitable2T&    visitable2_;

        // required to suppress warnings and ensure that we do not copy
        // this visitor
        two_variables_holder& operator=(const two_variables_holder&);

    public:
        typedef HBOOST_DEDUCED_TYPENAME VisitorT::result_type result_type;

        explicit two_variables_holder(VisitorT& visitor, Visitable1T& visitable1, Visitable2T& visitable2) HBOOST_NOEXCEPT 
            : visitor_(visitor)
            , visitable1_(visitable1)
            , visitable2_(visitable2)
        {}

#define HBOOST_VARIANT_OPERATOR_BEG()                            \
    return ::hboost::apply_visitor(                              \
    ::hboost::bind<result_type>(hboost::ref(visitor_), _1, _2     \
    /**/

#define HBOOST_VARIANT_OPERATOR_END()                            \
    ), visitable1_, visitable2_);                               \
    /**/

#define HBOOST_VARANT_VISITORS_VARIABLES_PRINTER(z, n, data)     \
    HBOOST_PP_COMMA() hboost::ref( HBOOST_PP_CAT(vis, n) )         \
    /**/

#define HBOOST_VARIANT_VISIT(z, n, data)                                                     \
    template <HBOOST_PP_ENUM_PARAMS(HBOOST_PP_ADD(n, 1), class VisitableUnwrapped)>           \
    HBOOST_VARIANT_AUX_GENERIC_RESULT_TYPE(result_type) operator()(                          \
        HBOOST_PP_ENUM_BINARY_PARAMS(HBOOST_PP_ADD(n, 1), VisitableUnwrapped, & vis)          \
    ) const                                                                                 \
    {                                                                                       \
        HBOOST_VARIANT_OPERATOR_BEG()                                                        \
        HBOOST_PP_REPEAT(HBOOST_PP_ADD(n, 1), HBOOST_VARANT_VISITORS_VARIABLES_PRINTER, ~)     \
        HBOOST_VARIANT_OPERATOR_END()                                                        \
    }                                                                                       \
    /**/

HBOOST_PP_REPEAT( HBOOST_PP_SUB(HBOOST_VARAINT_MAX_MULTIVIZITOR_PARAMS, 2), HBOOST_VARIANT_VISIT, ~)
#undef HBOOST_VARIANT_OPERATOR_BEG
#undef HBOOST_VARIANT_OPERATOR_END
#undef HBOOST_VARANT_VISITORS_VARIABLES_PRINTER
#undef HBOOST_VARIANT_VISIT

    };

    template <class VisitorT, class Visitable1T, class Visitable2T>
    inline two_variables_holder<VisitorT, Visitable1T, Visitable2T> make_two_variables_holder(
            VisitorT& visitor, Visitable1T& visitable1, Visitable2T& visitable2
        ) HBOOST_NOEXCEPT
    {
        return two_variables_holder<VisitorT, Visitable1T, Visitable2T>(visitor, visitable1, visitable2);
    }

    template <class VisitorT, class Visitable1T, class Visitable2T>
    inline two_variables_holder<const VisitorT, Visitable1T, Visitable2T> make_two_variables_holder(
            const VisitorT& visitor, Visitable1T& visitable1, Visitable2T& visitable2
        ) HBOOST_NOEXCEPT
    {
        return two_variables_holder<const VisitorT, Visitable1T, Visitable2T>(visitor, visitable1, visitable2);
    }

}} // namespace detail::variant

#define HBOOST_VARIANT_APPLY_VISITOR_BEG()                                               \
    return ::hboost::apply_visitor(                                                      \
            hboost::detail::variant::make_two_variables_holder(visitor, var0 , var1),    \
            var2                                                                        \
    /**/

#define HBOOST_VARIANT_APPLY_VISITOR_END()                       \
    );                                                          \
    /**/

#define HBOOST_VARANT_VISITORS_VARIABLES_PRINTER(z, n, data)     \
    HBOOST_PP_COMMA() HBOOST_PP_CAT(var, HBOOST_PP_ADD(n, 3))      \
    /**/

#define HBOOST_VARIANT_VISIT(z, n, data)                                                                 \
    template <class Visitor HBOOST_PP_COMMA() HBOOST_PP_ENUM_PARAMS(HBOOST_PP_ADD(n, 3), class T)>         \
    inline HBOOST_VARIANT_AUX_GENERIC_RESULT_TYPE(HBOOST_DEDUCED_TYPENAME Visitor::result_type) apply_visitor( \
        data HBOOST_PP_COMMA() HBOOST_PP_ENUM_BINARY_PARAMS(HBOOST_PP_ADD(n, 3), T, & var)     \
    )                                                                                                   \
    {                                                                                                   \
        HBOOST_VARIANT_APPLY_VISITOR_BEG()                                                               \
        HBOOST_PP_REPEAT(n, HBOOST_VARANT_VISITORS_VARIABLES_PRINTER, ~)                                  \
        HBOOST_VARIANT_APPLY_VISITOR_END()                                                               \
    }                                                                                                   \
    /**/

HBOOST_PP_REPEAT( HBOOST_PP_SUB(HBOOST_VARAINT_MAX_MULTIVIZITOR_PARAMS, 2), HBOOST_VARIANT_VISIT, const Visitor& visitor)
HBOOST_PP_REPEAT( HBOOST_PP_SUB(HBOOST_VARAINT_MAX_MULTIVIZITOR_PARAMS, 2), HBOOST_VARIANT_VISIT, Visitor& visitor)

#undef HBOOST_VARIANT_APPLY_VISITOR_BEG
#undef HBOOST_VARIANT_APPLY_VISITOR_END
#undef HBOOST_VARANT_VISITORS_VARIABLES_PRINTER
#undef HBOOST_VARIANT_VISIT
    
} // namespace hboost

#endif // HBOOST_VARIANT_DETAIL_MULTIVISITORS_PREPROCESSOR_BASED_HPP

