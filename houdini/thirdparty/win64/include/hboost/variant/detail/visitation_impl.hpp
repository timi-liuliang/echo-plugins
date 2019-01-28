//-----------------------------------------------------------------------------
// boost variant/detail/visitation_impl.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2003
// Eric Friedman
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_VARIANT_DETAIL_VISITATION_IMPL_HPP
#define HBOOST_VARIANT_DETAIL_VISITATION_IMPL_HPP

#include "hboost/config.hpp"

#include "hboost/variant/detail/backup_holder.hpp"
#include "hboost/variant/detail/cast_storage.hpp"
#include "hboost/variant/detail/forced_return.hpp"
#include "hboost/variant/detail/generic_result_type.hpp"
#include "hboost/variant/variant_fwd.hpp" // for HBOOST_VARIANT_DO_NOT_USE_VARIADIC_TEMPLATES

#include "hboost/mpl/eval_if.hpp"
#include "hboost/mpl/bool.hpp"
#include "hboost/mpl/identity.hpp"
#include "hboost/mpl/int.hpp"
#include "hboost/mpl/next.hpp"
#include "hboost/mpl/deref.hpp"
#include "hboost/mpl/or.hpp"
#include "hboost/preprocessor/cat.hpp"
#include "hboost/preprocessor/inc.hpp"
#include "hboost/preprocessor/repeat.hpp"
#include "hboost/type_traits/is_same.hpp"
#include "hboost/type_traits/has_nothrow_copy.hpp"
#include "hboost/type_traits/is_nothrow_move_constructible.hpp"

#if HBOOST_WORKAROUND(HBOOST_MSVC, >= 1400) 
# pragma warning (push) 
# pragma warning (disable : 4702) //unreachable code 
#endif

///////////////////////////////////////////////////////////////////////////////
// HBOOST_VARIANT_VISITATION_UNROLLING_LIMIT
//
// Unrolls variant's visitation mechanism to reduce template instantiation
// and potentially increase runtime performance. (TODO: Investigate further.)
//
#if !defined(HBOOST_VARIANT_VISITATION_UNROLLING_LIMIT)

#ifndef HBOOST_VARIANT_DO_NOT_USE_VARIADIC_TEMPLATES
#   include "hboost/mpl/limits/list.hpp"
#   define HBOOST_VARIANT_VISITATION_UNROLLING_LIMIT   \
        HBOOST_MPL_LIMIT_LIST_SIZE
#else
#   define HBOOST_VARIANT_VISITATION_UNROLLING_LIMIT   \
        HBOOST_VARIANT_LIMIT_TYPES
#endif

#endif

namespace hboost {
namespace detail { namespace variant {

///////////////////////////////////////////////////////////////////////////////
// (detail) class apply_visitor_unrolled
//
// Tag type indicates when visitation_impl is unrolled.
//
struct apply_visitor_unrolled {};

///////////////////////////////////////////////////////////////////////////////
// (detail) class template visitation_impl_step
//
// "Never ending" iterator range facilitates visitation_impl unrolling.
//


template <typename Iter, typename LastIter>
struct visitation_impl_step
{
    typedef typename mpl::deref<Iter>::type type;

    typedef typename mpl::next<Iter>::type next_iter;
    typedef visitation_impl_step<
          next_iter, LastIter
        > next;
};

template <typename LastIter>
struct visitation_impl_step< LastIter,LastIter >
{
    typedef apply_visitor_unrolled type;
    typedef visitation_impl_step next;
};


///////////////////////////////////////////////////////////////////////////////
// (detail) function template visitation_impl_invoke
//
// Invokes the given visitor on the specified type in the given storage.
//

template <typename Visitor, typename VoidPtrCV, typename T>
inline
    HBOOST_VARIANT_AUX_GENERIC_RESULT_TYPE(typename Visitor::result_type)
visitation_impl_invoke_impl(
      int, Visitor& visitor, VoidPtrCV storage, T*
    , mpl::true_// never_uses_backup
    )
{
    return visitor.internal_visit(
          cast_storage<T>(storage), 1L
        );
}

template <typename Visitor, typename VoidPtrCV, typename T>
inline
    HBOOST_VARIANT_AUX_GENERIC_RESULT_TYPE(typename Visitor::result_type)
visitation_impl_invoke_impl(
      int internal_which, Visitor& visitor, VoidPtrCV storage, T*
    , mpl::false_// never_uses_backup
    )
{
    if (internal_which >= 0)
    {
        return visitor.internal_visit(
              cast_storage<T>(storage), 1L
            );
    }
    else
    {
        return visitor.internal_visit(
              cast_storage< backup_holder<T> >(storage), 1L
            );
    }
}

template <typename Visitor, typename VoidPtrCV, typename T, typename NoBackupFlag>
inline
    HBOOST_VARIANT_AUX_GENERIC_RESULT_TYPE(typename Visitor::result_type)
visitation_impl_invoke(
      int internal_which, Visitor& visitor, VoidPtrCV storage, T* t
    , NoBackupFlag
    , int
    )
{
    typedef typename mpl::or_<
          NoBackupFlag
        , is_nothrow_move_constructible<T>
        , has_nothrow_copy<T>
        >::type never_uses_backup;

    return (visitation_impl_invoke_impl)(
          internal_which, visitor, storage, t
        , never_uses_backup()
        );
}

template <typename Visitor, typename VoidPtrCV, typename NBF>
inline
    HBOOST_VARIANT_AUX_GENERIC_RESULT_TYPE(typename Visitor::result_type)
visitation_impl_invoke(int, Visitor&, VoidPtrCV, apply_visitor_unrolled*, NBF, long)
{
    // should never be here at runtime!
    typedef typename Visitor::result_type result_type;
    return ::hboost::detail::variant::forced_return< result_type >();
}

///////////////////////////////////////////////////////////////////////////////
// (detail) function template visitation_impl
//
// Invokes the given visitor on the type in the given variant storage.
//

template <
      typename W, typename S
    , typename Visitor, typename VPCV
    , typename NBF
    >
inline
    HBOOST_VARIANT_AUX_GENERIC_RESULT_TYPE(typename Visitor::result_type)
visitation_impl(
      int, int, Visitor&, VPCV
    , mpl::true_ // is_apply_visitor_unrolled
    , NBF, W* = 0, S* = 0
    )
{
    // should never be here at runtime!
    typedef typename Visitor::result_type result_type;
    return ::hboost::detail::variant::forced_return< result_type >();
}

template <
      typename Which, typename step0
    , typename Visitor, typename VoidPtrCV
    , typename NoBackupFlag
    >
inline
    HBOOST_VARIANT_AUX_GENERIC_RESULT_TYPE(typename Visitor::result_type)
visitation_impl(
      const int internal_which, const int logical_which
    , Visitor& visitor, VoidPtrCV storage
    , mpl::false_ // is_apply_visitor_unrolled
    , NoBackupFlag no_backup_flag
    , Which* = 0, step0* = 0
    )
{
    // Typedef apply_visitor_unrolled steps and associated types...
#   define HBOOST_VARIANT_AUX_APPLY_VISITOR_STEP_TYPEDEF(z, N, _) \
    typedef typename HBOOST_PP_CAT(step,N)::type HBOOST_PP_CAT(T,N); \
    typedef typename HBOOST_PP_CAT(step,N)::next \
        HBOOST_PP_CAT(step, HBOOST_PP_INC(N)); \
    /**/

    HBOOST_PP_REPEAT(
          HBOOST_VARIANT_VISITATION_UNROLLING_LIMIT
        , HBOOST_VARIANT_AUX_APPLY_VISITOR_STEP_TYPEDEF
        , _
        )

#   undef HBOOST_VARIANT_AUX_APPLY_VISITOR_STEP_TYPEDEF

    // ...switch on the target which-index value...
    switch (logical_which)
    {

    // ...applying the appropriate case:
#   define HBOOST_VARIANT_AUX_APPLY_VISITOR_STEP_CASE(z, N, _) \
    case (Which::value + (N)): \
        return (visitation_impl_invoke)( \
              internal_which, visitor, storage \
            , static_cast<HBOOST_PP_CAT(T,N)*>(0) \
            , no_backup_flag, 1L \
            ); \
    /**/

    HBOOST_PP_REPEAT(
          HBOOST_VARIANT_VISITATION_UNROLLING_LIMIT
        , HBOOST_VARIANT_AUX_APPLY_VISITOR_STEP_CASE
        , _
        )

#   undef HBOOST_VARIANT_AUX_APPLY_VISITOR_STEP_CASE

    default: break;
    }

    // If not handled in this iteration, continue unrolling:
    typedef mpl::int_<
          Which::value + (HBOOST_VARIANT_VISITATION_UNROLLING_LIMIT)
        > next_which;

    typedef HBOOST_PP_CAT(step, HBOOST_VARIANT_VISITATION_UNROLLING_LIMIT)
        next_step;

    typedef typename next_step::type next_type;
    typedef typename is_same< next_type,apply_visitor_unrolled >::type
        is_apply_visitor_unrolled;

    return visitation_impl(
          internal_which, logical_which
        , visitor, storage
        , is_apply_visitor_unrolled()
        , no_backup_flag
        , static_cast<next_which*>(0), static_cast<next_step*>(0)
        );
}

}} // namespace detail::variant
} // namespace hboost

#if HBOOST_WORKAROUND(HBOOST_MSVC, >= 1400)  
# pragma warning(pop)  
#endif 

#endif // HBOOST_VARIANT_DETAIL_VISITATION_IMPL_HPP
