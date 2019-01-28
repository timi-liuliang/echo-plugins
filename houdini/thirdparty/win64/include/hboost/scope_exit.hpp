
// Copyright (C) 2006-2009, 2012 Alexander Nasonov
// Copyright (C) 2012 Lorenzo Caminiti
// Distributed under the Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt)
// Home at http://www.boost.org/libs/scope_exit

#ifndef FILE_hboost_scope_exit_hpp_INCLUDED
#define FILE_hboost_scope_exit_hpp_INCLUDED

#ifndef DOXYGEN

#include <hboost/detail/workaround.hpp>
#include <hboost/mpl/assert.hpp>
#include <hboost/mpl/int.hpp>
#include <hboost/utility/enable_if.hpp>
#include <hboost/function.hpp>
#include <hboost/typeof/typeof.hpp>
#include <hboost/config.hpp>
#include <hboost/preprocessor/cat.hpp>
#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/control/expr_iif.hpp>
#include <hboost/preprocessor/comparison/equal.hpp>
#include <hboost/preprocessor/logical/bitor.hpp>
#include <hboost/preprocessor/logical/bitand.hpp>
#include <hboost/preprocessor/facilities/empty.hpp>
#include <hboost/preprocessor/facilities/is_empty.hpp>
#include <hboost/preprocessor/facilities/identity.hpp>
#include <hboost/preprocessor/punctuation/comma_if.hpp>
#include <hboost/preprocessor/punctuation/paren_if.hpp>
#include <hboost/preprocessor/seq/cat.hpp>
#include <hboost/preprocessor/seq/size.hpp>
#include <hboost/preprocessor/seq/to_tuple.hpp>
#include <hboost/preprocessor/tuple/elem.hpp>
#include <hboost/preprocessor/tuple/eat.hpp>
#include <hboost/preprocessor/tuple/to_list.hpp>
#include <hboost/preprocessor/list/append.hpp>
#include <hboost/preprocessor/list/fold_left.hpp>
#include <hboost/preprocessor/list/enum.hpp>
#include <hboost/preprocessor/list/adt.hpp>
#include <hboost/preprocessor/list/for_each_i.hpp>
#include <hboost/preprocessor/detail/is_unary.hpp>

// PRIVATE/PROTECTED //

// NOTE: AUX prefix and aux namespace mark "private" symbols that shall be used
// only within this library; DETAIL prefix and detail namespace mark "protected"
// symbols that can be used by other Boost libraries but not outside Boost.

// WARNING: HBOOST_SCOPE_EXIT_AUX_GCC also used by some regression test.
#if defined(__GNUC__) && !defined(HBOOST_INTEL)
#   define HBOOST_SCOPE_EXIT_AUX_GCC (__GNUC__ * 100 + __GNUC_MINOR__)
#else
#   define HBOOST_SCOPE_EXIT_AUX_GCC 0
#endif

#if HBOOST_WORKAROUND(HBOOST_SCOPE_EXIT_AUX_GCC, HBOOST_TESTED_AT(413))
#   define HBOOST_SCOPE_EXIT_AUX_TPL_GCC_WORKAROUND_01 1
#else
#   define HBOOST_SCOPE_EXIT_AUX_TPL_GCC_WORKAROUND_01 0
#endif

#if HBOOST_MSVC
#   define HBOOST_SCOPE_EXIT_AUX_TYPEOF_THIS_MSVC_WORKAROUND_01 1
#else
#   define HBOOST_SCOPE_EXIT_AUX_TYPEOF_THIS_MSVC_WORKAROUND_01 0
#endif

// MSVC has problems expanding __LINE__ so use (the non standard) __COUNTER__.
#ifdef HBOOST_MSVC
#   define HBOOST_SCOPE_EXIT_AUX_PP_LINE_COUNTER __COUNTER__
#else
#   define HBOOST_SCOPE_EXIT_AUX_PP_LINE_COUNTER __LINE__
#endif

// Preprocessor "keyword" detection.

// These are not a local macros, do not #undefine them (these are used by the
// ..._BACK macros below).
#define this_HBOOST_SCOPE_EXIT_AUX_PP_KEYWORD_THISUNDERSCORE_IS (1) /* unary */
#define void_HBOOST_SCOPE_EXIT_AUX_PP_KEYWORD_VOID_IS (1) /* unary */

#define HBOOST_SCOPE_EXIT_AUX_PP_KEYWORD_IS_BACK_(token, checking_postfix) \
    HBOOST_PP_IS_UNARY(HBOOST_PP_CAT(token, checking_postfix))

#define HBOOST_SCOPE_EXIT_AUX_PP_KEYWORD_IS_THISUNDERSCORE_BACK(token) \
    HBOOST_SCOPE_EXIT_AUX_PP_KEYWORD_IS_BACK_(token, \
            HBOOST_SCOPE_EXIT_AUX_PP_KEYWORD_THISUNDERSCORE_IS)

#define HBOOST_SCOPE_EXIT_AUX_PP_KEYWORD_IS_VOID_BACK(token) \
    HBOOST_SCOPE_EXIT_AUX_PP_KEYWORD_IS_BACK_(token, \
            _HBOOST_SCOPE_EXIT_AUX_PP_KEYWORD_VOID_IS)

// Preprocessor "void-list".

// NOTE: Empty list must always be represented as void (which is also a way to
// specify no function parameter) and it can never be empty because (1)
// IS_EMPTY(&var) fails (because of the leading non alphanumeric symbol) and
// (2) some compilers (MSVC) fail to correctly pass empty macro parameters
// even if they support variadic macros. Therefore, always using void to
// represent is more portable.

// Argument: (token1)...
#define HBOOST_SCOPE_EXIT_AUX_PP_VOID_LIST_FROM_SEQ_(unused, seq) \
    HBOOST_PP_TUPLE_TO_LIST(HBOOST_PP_SEQ_SIZE(seq), HBOOST_PP_SEQ_TO_TUPLE(seq))

// Token: void | token1
#define HBOOST_SCOPE_EXIT_AUX_PP_VOID_LIST_HANDLE_VOID_( \
        is_void_macro, token) \
    HBOOST_PP_IIF(is_void_macro(token), \
        HBOOST_PP_NIL \
    , \
        (token, HBOOST_PP_NIL) \
    )

// Token: (a)(b)... | empty | void | token
#define HBOOST_SCOPE_EXIT_AUX_PP_VOID_LIST_HANDLE_SEQ_( \
        is_void_macro, token) \
    HBOOST_PP_IIF(HBOOST_PP_IS_UNARY(token), /* unary paren (a)... */ \
        HBOOST_SCOPE_EXIT_AUX_PP_VOID_LIST_FROM_SEQ_ \
    , \
        HBOOST_SCOPE_EXIT_AUX_PP_VOID_LIST_HANDLE_VOID_ \
    )(is_void_macro, token)

#define HBOOST_SCOPE_EXIT_AUX_PP_VOID_LIST_NEVER_(tokens) \
    0 /* void check always returns false */

#ifdef HBOOST_NO_CXX11_VARIADIC_MACROS

#define HBOOST_SCOPE_EXIT_AUX_PP_VOID_LIST_(is_void_macro, seq) \
    HBOOST_SCOPE_EXIT_AUX_PP_VOID_LIST_HANDLE_SEQ_(is_void_macro, seq)

// Expand `void | (a)(b)...` to pp-list `NIL | (a, (b, NIL))`.
#define HBOOST_SCOPE_EXIT_AUX_PP_VOID_LIST(sign) \
    HBOOST_SCOPE_EXIT_AUX_PP_VOID_LIST_( \
            HBOOST_SCOPE_EXIT_AUX_PP_KEYWORD_IS_VOID_BACK, sign)

// Expand `(a)(b)...` to pp-list `(a, (b, NIL))`.
#define HBOOST_SCOPE_EXIT_AUX_PP_NON_VOID_LIST(seq) \
    HBOOST_SCOPE_EXIT_AUX_PP_VOID_LIST_( \
            HBOOST_SCOPE_EXIT_AUX_PP_VOID_LIST_NEVER_, seq)

#else // VARIADICS

// FUTURE: Replace this with HBOOST_PP_VARIADIC_SIZE when and if
// HBOOST_PP_VARIAIDCS detection will match !HBOOST_NO_CXX11_VARIADIC_MACROS (for
// now Boost.Preprocessor and Boost.Config disagree on detecting compiler
// variadic support while this VARIADIC_SIZE works on compilers not detected by
// PP).
#if HBOOST_MSVC
#   define HBOOST_SCOPE_EXIT_AUX_PP_VOID_LIST_VARIADIC_SIZE_(...) \
        HBOOST_PP_CAT(HBOOST_SCOPE_EXIT_AUX_PP_VOID_LIST_VARIADIC_SIZE_I_(__VA_ARGS__, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,),)
#else // MSVC
#   define HBOOST_SCOPE_EXIT_AUX_PP_VOID_LIST_VARIADIC_SIZE_(...) \
        HBOOST_SCOPE_EXIT_AUX_PP_VOID_LIST_VARIADIC_SIZE_I_(__VA_ARGS__, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,)
#endif // MSVC
#define HBOOST_SCOPE_EXIT_AUX_PP_VOID_LIST_VARIADIC_SIZE_I_(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63, size, ...) size

// Argument: token1, ...
#define HBOOST_SCOPE_EXIT_AUX_PP_VOID_LIST_FROM_VARIADIC_(unused, ...) \
    HBOOST_PP_TUPLE_TO_LIST( \
            HBOOST_SCOPE_EXIT_AUX_PP_VOID_LIST_VARIADIC_SIZE_( \
                    __VA_ARGS__), (__VA_ARGS__))

#define HBOOST_SCOPE_EXIT_AUX_PP_VOID_LIST_(is_void_macro, ...) \
    HBOOST_PP_IIF(HBOOST_PP_EQUAL( \
            HBOOST_SCOPE_EXIT_AUX_PP_VOID_LIST_VARIADIC_SIZE_( \
                    __VA_ARGS__), 1), \
        HBOOST_SCOPE_EXIT_AUX_PP_VOID_LIST_HANDLE_SEQ_ \
    , \
        HBOOST_SCOPE_EXIT_AUX_PP_VOID_LIST_FROM_VARIADIC_ \
    )(is_void_macro, __VA_ARGS__)

// Expand `void | (a)(b)... | a, b, ...` to pp-list `NIL | (a, (b, NIL))`.
#define HBOOST_SCOPE_EXIT_AUX_PP_VOID_LIST(...) \
    HBOOST_SCOPE_EXIT_AUX_PP_VOID_LIST_( \
            HBOOST_SCOPE_EXIT_AUX_PP_KEYWORD_IS_VOID_BACK, __VA_ARGS__)

// Expand `(a)(b)... | a, b, ...` to pp-list `(a, (b, NIL))`.
#define HBOOST_SCOPE_EXIT_AUX_PP_NON_VOID_LIST(...) \
    HBOOST_SCOPE_EXIT_AUX_PP_VOID_LIST_( \
            HBOOST_SCOPE_EXIT_AUX_PP_VOID_LIST_NEVER_, __VA_ARGS__)

#endif // VARIADICS

// Steven Watanabe's trick with a modification suggested by Kim Barrett
namespace hboost { namespace scope_exit { namespace detail {

// Type of a local HBOOST_SCOPE_EXIT_AUX_ARGS variable.
// First use in a local scope will declare the HBOOST_SCOPE_EXIT_AUX_ARGS
// variable, subsequent uses will be resolved as two comparisons
// (cmp1 with 0 and cmp2 with HBOOST_SCOPE_EXIT_AUX_ARGS).
template<int Dummy = 0>
struct declared
{
    void* value;
    static int const cmp2 = 0;
    friend void operator>(int, declared const&) {}
};

struct undeclared { declared<> dummy[2]; };

template<int> struct resolve;

template<>
struct resolve<sizeof(declared<>)>
{
    static const int cmp1 = 0;
};

template<>
struct resolve<sizeof(undeclared)>
{
    template<int>
    struct cmp1
    {
        static int const cmp2 = 0;
    };
};

typedef void (*ref_tag)(int&);
typedef void (*val_tag)(int );

template<class T, class Tag> struct member;

template<class T>
struct member<T,ref_tag>
{
    T& value;
#if !HBOOST_SCOPE_EXIT_AUX_TPL_GCC_WORKAROUND_01
    member(T& ref) : value(ref) {}
#endif
};

template<class T>
struct member<T,val_tag>
{
    T value;
#if !HBOOST_SCOPE_EXIT_AUX_TPL_GCC_WORKAROUND_01
    member(T& val) : value(val) {}
#endif
};

template<class T> inline T& deref(T* p, ref_tag) { return *p; }
template<class T> inline T& deref(T& r, val_tag) { return  r; }

template<class T>
struct wrapper
{
    typedef T type;
};

template<class T> wrapper<T> wrap(T&);

} } } // namespace

#include HBOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()
HBOOST_TYPEOF_REGISTER_TEMPLATE(hboost::scope_exit::detail::wrapper, 1)

#define HBOOST_SCOPE_EXIT_AUX_ARGS hboost_scope_exit_aux_args
extern hboost::scope_exit::detail::undeclared HBOOST_SCOPE_EXIT_AUX_ARGS;

#define HBOOST_SCOPE_EXIT_AUX_GUARD(id)   \
    HBOOST_PP_CAT(hboost_se_guard_, id)

#define HBOOST_SCOPE_EXIT_AUX_GUARD_T(id) \
    HBOOST_PP_CAT(hboost_se_guard_t_, id)

#define HBOOST_SCOPE_EXIT_AUX_PARAMS(id)  \
    HBOOST_PP_CAT(hboost_se_params_, id)

#define HBOOST_SCOPE_EXIT_AUX_THIS_T(id)  \
    HBOOST_PP_CAT(hboost_se_this_t_,  id)

#define HBOOST_SCOPE_EXIT_AUX_THIS_CAPTURE_T(id) \
    HBOOST_PP_CAT(hboost_se_this_capture_t_,  id)

#define HBOOST_SCOPE_EXIT_DETAIL_PARAMS_T(id) \
    HBOOST_PP_CAT(hboost_se_params_t_, id)

#define HBOOST_SCOPE_EXIT_DETAIL_TAG(id, i) \
    HBOOST_PP_SEQ_CAT( (hboost_se_tag_)(i)(_)(id) )

#define HBOOST_SCOPE_EXIT_DETAIL_PARAM_THIS(id) \
    HBOOST_PP_SEQ_CAT( (hboost_se_param_this_)(id) )

#define HBOOST_SCOPE_EXIT_DETAIL_PARAM(id, i, var) \
    HBOOST_PP_SEQ_CAT( (hboost_se_param_)(i)(_)(id) )

#define HBOOST_SCOPE_EXIT_DETAIL_PARAM_T(id, i, var) \
    HBOOST_PP_SEQ_CAT( (hboost_se_param_t_)(i)(_)(id) )

#define HBOOST_SCOPE_EXIT_DETAIL_CAPTURE_T(id, i, var) \
    HBOOST_PP_SEQ_CAT( (hboost_se_capture_t_)(i)(_)(id) )

#define HBOOST_SCOPE_EXIT_AUX_WRAPPED(id, i) \
    HBOOST_PP_SEQ_CAT( (hboost_se_wrapped_t_)(i)(_)(id) )

#define HBOOST_SCOPE_EXIT_AUX_DEREF(id, i, var) \
    ::hboost::scope_exit::detail::deref(var, \
            static_cast<HBOOST_SCOPE_EXIT_DETAIL_TAG(id, i)>(0))

#define HBOOST_SCOPE_EXIT_AUX_MEMBER(r, id, i, var) \
    ::hboost::scope_exit::detail::member< \
        HBOOST_SCOPE_EXIT_DETAIL_PARAM_T(id, i, var), \
        HBOOST_SCOPE_EXIT_DETAIL_TAG(id, i) \
    > HBOOST_SCOPE_EXIT_DETAIL_PARAM(id, i, var);

#define HBOOST_SCOPE_EXIT_AUX_ARG_DECL(r, id_ty, i, var) \
    HBOOST_PP_COMMA_IF(i) \
    HBOOST_PP_TUPLE_ELEM(2, 1, id_ty) \
    HBOOST_SCOPE_EXIT_DETAIL_PARAMS_T(HBOOST_PP_TUPLE_ELEM(2, 0, id_ty)):: \
            HBOOST_SCOPE_EXIT_DETAIL_PARAM_T(HBOOST_PP_TUPLE_ELEM(2, 0, id_ty), \
                    i, var) \
    var
 
#define HBOOST_SCOPE_EXIT_AUX_ARG(r, id, i, var) \
    HBOOST_PP_COMMA_IF(i) \
    hboost_se_params_->HBOOST_SCOPE_EXIT_DETAIL_PARAM(id, i, var).value

#define HBOOST_SCOPE_EXIT_DETAIL_TAG_DECL(r, id, i, var) \
    typedef void (*HBOOST_SCOPE_EXIT_DETAIL_TAG(id, i))(int var);

// Adam Butcher's workaround to deduce `this` type on MSVC revision < 10.
// Boost.Typeof for VC71's typeid-based workaround does not work to determine
// `this` type due to error C2355 being incorrectly reported. The typical
// avoidance strategy implemented below is to make an indirect compile-time
// constant by assigning an enum and use that as type-index-- this only works
// with the sizeof() approach and not with the typeid() approach. Lorenzo
// Caminiti extended this approach to work in type-of emulation mode. This code
// is very similar (and somewhat of a duplication) of the code in
// hboost/typeof/msvc/typeof_impl.hpp). However, this code cannot be integrated
// into Boost.Typeof because its final API has to be a `typedef ...` and it
// cannot be a `typeof(...)`.
#if HBOOST_SCOPE_EXIT_AUX_TYPEOF_THIS_MSVC_WORKAROUND_01

#include <hboost/config.hpp>
#include <hboost/detail/workaround.hpp>
#include <hboost/mpl/int.hpp>
#include <hboost/type_traits/is_function.hpp>
#include <hboost/utility/enable_if.hpp>

#if defined(HBOOST_MSVC)
#   include <typeinfo>
#endif

namespace hboost { namespace scope_exit { namespace aux {
        namespace msvc_typeof_this {

// compile-time constant code
#if defined(HBOOST_MSVC) && defined(_MSC_EXTENSIONS)

template<int N> struct the_counter;

template<typename T,int N = 5 /* for similarity */>
struct encode_counter {
    __if_exists(the_counter<N + 256>) {
        HBOOST_STATIC_CONSTANT(unsigned,
            count=(encode_counter<T,N + 257>::count));
    }
    __if_not_exists(the_counter<N + 256>) {
        __if_exists(the_counter<N + 64>) {
            HBOOST_STATIC_CONSTANT(unsigned,
                    count=(encode_counter<T,N + 65>::count));
        }
        __if_not_exists(the_counter<N + 64>) {
            __if_exists(the_counter<N + 16>) {
                HBOOST_STATIC_CONSTANT(unsigned,
                        count=(encode_counter<T,N + 17>::count));
            }
            __if_not_exists(the_counter<N + 16>) {
                __if_exists(the_counter<N + 4>) {
                    HBOOST_STATIC_CONSTANT(unsigned,
                            count=(encode_counter<T,N + 5>::count));
                }
                __if_not_exists(the_counter<N + 4>) {
                    __if_exists(the_counter<N>) {
                        HBOOST_STATIC_CONSTANT(unsigned,
                                count=(encode_counter<T,N + 1>::count));
                    }
                    __if_not_exists(the_counter<N>) {
                        HBOOST_STATIC_CONSTANT(unsigned,count=N);
                        typedef the_counter<N> type;
                    }
                }
            }
        }
    }
};

#else // compile-time constant code
    
template<int N> struct encode_counter : encode_counter<N - 1> {};

template<> struct encode_counter<0> {};

#endif // compile-time constant code

#if HBOOST_WORKAROUND(HBOOST_MSVC, >= 1400) // type-of code

struct msvc_extract_type_default_param {};

template<typename ID, typename T = msvc_extract_type_default_param>
struct msvc_extract_type;

template<typename ID>
struct msvc_extract_type<ID, msvc_extract_type_default_param> {
    template<bool>
    struct id2type_impl;

    typedef id2type_impl<true> id2type;
};

template<typename ID, typename T>
struct msvc_extract_type
        : msvc_extract_type<ID, msvc_extract_type_default_param> {
    template<>
    struct id2type_impl<true> { // VC8.0 specific bug-feature.
        typedef T type;
    };

    template<bool>
    struct id2type_impl;

    typedef id2type_impl<true> id2type;
};

template<typename T, typename ID>
struct msvc_register_type : msvc_extract_type<ID, T> {};

#else // type-of code

template<typename ID>
struct msvc_extract_type {
    struct id2type;
};

template<typename T, typename ID>
struct msvc_register_type : msvc_extract_type<ID> {
    typedef msvc_extract_type<ID> base_type;
    struct base_type::id2type { // This uses nice VC6.5 and VC7.1 bug-features.
        typedef T type;
    };
};

#endif // typeof code

template<int Id>
struct msvc_typeid_wrapper {
    typedef typename msvc_extract_type<hboost::mpl::int_<Id>
            >::id2type id2type;
    typedef typename id2type::type type;
};

template<>
struct msvc_typeid_wrapper<4> {
    typedef msvc_typeid_wrapper<4> type;
};

template<typename T>
struct encode_type {
    HBOOST_STATIC_CONSTANT(unsigned, value = encode_counter<T>::count);
    typedef typename msvc_register_type<T,
            hboost::mpl::int_<value> >::id2type type;
    HBOOST_STATIC_CONSTANT(unsigned, next = value + 1);
};

template<class T>
struct sizer {
    typedef char(*type)[encode_type<T>::value];
};

template<typename T>
typename hboost::disable_if<
      typename hboost::is_function<T>::type
    , typename sizer<T>::type
>::type encode_start(T const&);

template<typename T>
typename hboost::enable_if<
      typename hboost::is_function<T>::type
    , typename sizer<T>::type
>::type encode_start(T&);

template<typename Organizer, typename T>
msvc_register_type<T, Organizer> typeof_register_type(const T&,
        Organizer* = 0);

} } } } // namespace

#define HBOOST_SCOPE_EXIT_AUX_TYPEDEF_TYPEOF_THIS_INDEX_(id) \
    HBOOST_PP_CAT(hboost_se_thistype_index_, id)

#define HBOOST_SCOPE_EXIT_DETAIL_TYPEDEF_TYPEOF_THIS(id, ty, new_type) \
    /* unfortunately, we need to go via this enum which causes this to be */ \
    /* a typedef construct and not a typeof (so this code cannot be */ \
    /* integrated into Boost.Typeof) */ \
    enum { \
        HBOOST_SCOPE_EXIT_AUX_TYPEDEF_TYPEOF_THIS_INDEX_(id) = sizeof( \
            *::hboost::scope_exit::aux::msvc_typeof_this::encode_start(this)) \
    }; \
    typedef \
        ty ::hboost::scope_exit::aux::msvc_typeof_this::msvc_typeid_wrapper< \
            HBOOST_SCOPE_EXIT_AUX_TYPEDEF_TYPEOF_THIS_INDEX_(id) \
        >::type \
        new_type \
    ;

#else // TYPEOF_THIS_MSVC_WORKAROUND

#define HBOOST_SCOPE_EXIT_DETAIL_TYPEDEF_TYPEOF_THIS(id, ty, new_type) \
    typedef /* trailing `EMPTY()` handles empty `ty` */ \
        HBOOST_PP_IIF(HBOOST_PP_IS_EMPTY(ty HBOOST_PP_EMPTY()), \
            HBOOST_TYPEOF \
        , \
            HBOOST_TYPEOF_TPL \
        )(this) \
        new_type \
    ;

#endif // TYPEOF_THIS_MSVC_WORKAROUND

#if HBOOST_SCOPE_EXIT_AUX_TPL_GCC_WORKAROUND_01

#define HBOOST_SCOPE_EXIT_AUX_PARAMS_T_CTOR(id, ty, captures, has_this) \
    /* expand to nothing */

#define HBOOST_SCOPE_EXIT_DETAIL_PARAM_INIT(r, id, i, var) \
    HBOOST_PP_COMMA_IF(i) { HBOOST_SCOPE_EXIT_AUX_DEREF(id, i, var) }

#define HBOOST_SCOPE_EXIT_AUX_PARAMS_INIT(id, captures, has_this) \
    HBOOST_PP_EXPR_IIF(HBOOST_PP_BITOR(has_this, \
            HBOOST_PP_LIST_IS_CONS(captures)), \
        = { \
    ) \
    HBOOST_PP_LIST_FOR_EACH_I(HBOOST_SCOPE_EXIT_DETAIL_PARAM_INIT, id, captures) \
    HBOOST_PP_COMMA_IF(HBOOST_PP_BITAND(HBOOST_PP_LIST_IS_CONS(captures), \
            has_this)) \
    HBOOST_PP_EXPR_IIF(has_this, this) /* no extra {...} needed here */ \
    HBOOST_PP_EXPR_IIF(HBOOST_PP_BITOR(has_this, \
            HBOOST_PP_LIST_IS_CONS(captures)), \
        } /* trailing `;` will be added by the caller */ \
    )

#else // TPL_GCC_WORKAROUND

#define HBOOST_SCOPE_EXIT_AUX_CTOR_ARG(r, id, i, var) \
    HBOOST_PP_COMMA_IF(i) \
    HBOOST_SCOPE_EXIT_DETAIL_PARAM_T(id, i, var) & HBOOST_PP_CAT(a, i)

#define HBOOST_SCOPE_EXIT_AUX_MEMBER_INIT(r, id, i, var) \
    HBOOST_PP_COMMA_IF(i) \
    HBOOST_SCOPE_EXIT_DETAIL_PARAM(id, i, var) ( HBOOST_PP_CAT(a, i) )

#define HBOOST_SCOPE_EXIT_AUX_CTOR_ARG_THIS_NAME(id) \
    HBOOST_PP_CAT(hboost_se_this_arg_, id)

#define HBOOST_SCOPE_EXIT_AUX_CTOR_ARG_THIS(id, ty, comma01) \
    HBOOST_PP_COMMA_IF(comma01) \
    ty HBOOST_SCOPE_EXIT_DETAIL_PARAMS_T(id)::HBOOST_SCOPE_EXIT_AUX_THIS_T(id) \
            HBOOST_SCOPE_EXIT_AUX_CTOR_ARG_THIS_NAME(id) /* ptr so no & */

#define HBOOST_SCOPE_EXIT_AUX_MEMBER_THIS_INIT(id, comma01) \
    HBOOST_PP_COMMA_IF(comma01) \
    HBOOST_SCOPE_EXIT_DETAIL_PARAM_THIS(id)( \
            HBOOST_SCOPE_EXIT_AUX_CTOR_ARG_THIS_NAME(id))

#define HBOOST_SCOPE_EXIT_AUX_PARAMS_T_CTOR(id, ty, captures, has_this) \
    HBOOST_SCOPE_EXIT_DETAIL_PARAMS_T(id)( \
        HBOOST_PP_LIST_FOR_EACH_I(HBOOST_SCOPE_EXIT_AUX_CTOR_ARG, id, captures) \
        HBOOST_PP_IIF(has_this, \
            HBOOST_SCOPE_EXIT_AUX_CTOR_ARG_THIS \
        , \
            HBOOST_PP_TUPLE_EAT(3) \
        )(id, ty, HBOOST_PP_LIST_IS_CONS(captures)) \
    ) \
        HBOOST_PP_EXPR_IIF(HBOOST_PP_BITOR(HBOOST_PP_LIST_IS_CONS(captures), \
                has_this), \
            : \
        ) \
        HBOOST_PP_LIST_FOR_EACH_I(HBOOST_SCOPE_EXIT_AUX_MEMBER_INIT, id, \
                captures) \
        HBOOST_PP_IIF(has_this, \
            HBOOST_SCOPE_EXIT_AUX_MEMBER_THIS_INIT \
        , \
            HBOOST_PP_TUPLE_EAT(2) \
        )(id, HBOOST_PP_LIST_IS_CONS(captures)) \
    {}

#define HBOOST_SCOPE_EXIT_DETAIL_PARAM_INIT(r, id, i, var) \
    HBOOST_PP_COMMA_IF(i) HBOOST_SCOPE_EXIT_AUX_DEREF(id,i,var)

#define HBOOST_SCOPE_EXIT_AUX_PARAMS_INIT(id, captures, has_this) \
    HBOOST_PP_LPAREN_IF(HBOOST_PP_BITOR(has_this, \
            HBOOST_PP_LIST_IS_CONS(captures))) \
    HBOOST_PP_LIST_FOR_EACH_I(HBOOST_SCOPE_EXIT_DETAIL_PARAM_INIT, id, captures) \
    HBOOST_PP_COMMA_IF(HBOOST_PP_BITAND(HBOOST_PP_LIST_IS_CONS(captures), \
            has_this)) \
    HBOOST_PP_EXPR_IIF(has_this, this) \
    HBOOST_PP_RPAREN_IF(HBOOST_PP_BITOR(has_this, \
            HBOOST_PP_LIST_IS_CONS(captures)))

#endif // TPL_GCC_WORKAROUND

#if defined(HBOOST_TYPEOF_EMULATION)

#define HBOOST_SCOPE_EXIT_DETAIL_CAPTURE_DECL(r, id_ty, i, var) \
    struct HBOOST_SCOPE_EXIT_AUX_WRAPPED(HBOOST_PP_TUPLE_ELEM(2, 0, id_ty), i) \
        /* no need to use TYPEOF_TPL here because it's within inheritance */ \
        : HBOOST_TYPEOF(::hboost::scope_exit::detail::wrap( \
                HBOOST_SCOPE_EXIT_AUX_DEREF(HBOOST_PP_TUPLE_ELEM(2, 0, id_ty), \
                        i, var))) \
    {}; \
    typedef HBOOST_PP_TUPLE_ELEM(2, 1, id_ty) \
        HBOOST_SCOPE_EXIT_AUX_WRAPPED(HBOOST_PP_TUPLE_ELEM(2, 0, id_ty), i)::type\
        HBOOST_SCOPE_EXIT_DETAIL_CAPTURE_T(HBOOST_PP_TUPLE_ELEM(2, 0, id_ty), \
                i, var) \
    ;

#elif defined(HBOOST_INTEL)

#define HBOOST_SCOPE_EXIT_DETAIL_CAPTURE_DECL(r, id_ty, i, var) \
    typedef \
        /* no TYPEOF_TPL here because uses TYPEOF_KEYWORD directly */ \
        HBOOST_TYPEOF_KEYWORD(HBOOST_SCOPE_EXIT_AUX_DEREF( \
                HBOOST_PP_TUPLE_ELEM(2, 0, id_ty), i, var)) \
        HBOOST_SCOPE_EXIT_DETAIL_CAPTURE_T(HBOOST_PP_TUPLE_ELEM(2, 0, id_ty), \
                i, var) \
    ;

#else

#define HBOOST_SCOPE_EXIT_DETAIL_CAPTURE_DECL(r, id_ty, i, var) \
    typedef \
        /* no need to use TYPEOF_TPL here because it's a typedef */ \
        HBOOST_TYPEOF(::hboost::scope_exit::detail::wrap( \
                HBOOST_SCOPE_EXIT_AUX_DEREF(HBOOST_PP_TUPLE_ELEM(2, 0, id_ty), \
                        i, var))) \
        HBOOST_SCOPE_EXIT_AUX_WRAPPED(HBOOST_PP_TUPLE_ELEM(2, 0, id_ty), i) \
    ; \
    typedef HBOOST_PP_TUPLE_ELEM(2, 1, id_ty) \
        HBOOST_SCOPE_EXIT_AUX_WRAPPED(HBOOST_PP_TUPLE_ELEM(2, 0, id_ty), i)::type\
        HBOOST_SCOPE_EXIT_DETAIL_CAPTURE_T(HBOOST_PP_TUPLE_ELEM(2, 0, id_ty), \
                i, var) \
    ;

#endif

#define HBOOST_SCOPE_EXIT_DETAIL_PARAM_DECL(r, id_ty, i, var) \
    typedef \
        HBOOST_SCOPE_EXIT_DETAIL_CAPTURE_T(HBOOST_PP_TUPLE_ELEM(2, 0, id_ty), \
                i, var) \
        HBOOST_SCOPE_EXIT_DETAIL_PARAM_T(HBOOST_PP_TUPLE_ELEM(2, 0, id_ty), \
                i, var) \
    ;

// Traits.

#define HBOOST_SCOPE_EXIT_AUX_TRAITS_OP_CAPTURE(d, captures, this01, capture) \
    (HBOOST_PP_LIST_APPEND(captures, (capture, HBOOST_PP_NIL)), this01)

#define HBOOST_SCOPE_EXIT_AUX_TRAITS_OP_THIS(d, captures, this01, this_) \
    (captures, 1 /* has this (note, no error if multiple this_) */)

#define HBOOST_SCOPE_EXIT_AUX_TRAITS_OP(d, captures_this, capture) \
    HBOOST_PP_IIF(HBOOST_SCOPE_EXIT_AUX_PP_KEYWORD_IS_THISUNDERSCORE_BACK(\
            capture), \
        HBOOST_SCOPE_EXIT_AUX_TRAITS_OP_THIS \
    , \
        HBOOST_SCOPE_EXIT_AUX_TRAITS_OP_CAPTURE \
    )(d, HBOOST_PP_TUPLE_ELEM(2, 0, captures_this), \
            HBOOST_PP_TUPLE_ELEM(2, 1, captures_this), capture)

// ref_val: & | =
#define HBOOST_SCOPE_EXIT_AUX_TRAITS_ALL_OP(ref_val, traits) \
    ( \
        HBOOST_PP_LIST_APPEND((ref_val, HBOOST_PP_NIL), \
                HBOOST_SCOPE_EXIT_AUX_TRAITS_CAPTURES(traits)) \
    , \
        HBOOST_SCOPE_EXIT_AUX_TRAITS_HAS_THIS(traits) \
    )

#define HBOOST_SCOPE_EXIT_AUX_TRAITS(captures) \
    HBOOST_PP_LIST_FOLD_LEFT(HBOOST_SCOPE_EXIT_AUX_TRAITS_OP, \
            (HBOOST_PP_NIL, 0), captures)

#define HBOOST_SCOPE_EXIT_AUX_TRAITS_ALL(captures) \
    HBOOST_SCOPE_EXIT_AUX_TRAITS_ALL_OP(HBOOST_PP_LIST_FIRST(captures), \
            HBOOST_SCOPE_EXIT_AUX_TRAITS(HBOOST_PP_LIST_REST(captures)))

#define HBOOST_SCOPE_EXIT_AUX_TRAITS_CAPTURES(traits) \
    HBOOST_PP_TUPLE_ELEM(2, 0, traits)

#define HBOOST_SCOPE_EXIT_AUX_TRAITS_HAS_THIS(traits) \
    HBOOST_PP_TUPLE_ELEM(2, 1, traits)

#ifndef HBOOST_NO_CXX11_LAMBDAS

namespace hboost { namespace scope_exit { namespace aux {

template<typename This = void>
struct guard { // With object `this_` (for backward compatibility).
    explicit guard(This _this) : this_(_this) {}
    ~guard() { if(f_) f_(this_); }
    template<typename Lambda>
    void operator=(Lambda f) { f_ = f; }
private:
    This this_;
    hboost::function<void (This)> f_;
};

template<>
struct guard<void> { // Without object `this_` (could capture `this` directly).
    ~guard() { if(f_) f_(); }
    template<typename Lambda>
    void operator=(Lambda f) { f_ = f; }
private:
    hboost::function<void (void)> f_;
};

} } } // namespace
    
#define HBOOST_SCOPE_EXIT_AUX_LAMBDA_PARAMS(id) \
    HBOOST_PP_CAT(hboost_se_lambda_params_, id)

#define HBOOST_SCOPE_EXIT_AUX_LAMBDA_THIS_CAPTURE_TYPE(id) \
    HBOOST_PP_CAT(hboost_se_lambda_this_t_, id)

#define HBOOST_SCOPE_EXIT_AUX_LAMBDA_THIS_PARAM_TYPE(id) \
    HBOOST_PP_CAT(hboost_se_lambda_this_capture_t_, id)

#define HBOOST_SCOPE_EXIT_AUX_LAMBDA_THIS_TYPE(id, ty) \
    ty HBOOST_SCOPE_EXIT_AUX_LAMBDA_PARAMS(id):: \
            HBOOST_SCOPE_EXIT_AUX_LAMBDA_THIS_PARAM_TYPE(id)

// Precondition: HAS_THIS(traits).
#define HBOOST_SCOPE_EXIT_AUX_LAMBDA_THIS_TYPEDEFS(id, ty, traits) \
    HBOOST_SCOPE_EXIT_DETAIL_TYPEDEF_TYPEOF_THIS(id, ty, \
            HBOOST_SCOPE_EXIT_AUX_LAMBDA_THIS_CAPTURE_TYPE(id)) \
    /* capture type for workaround GCC internal error (even on later C++11) */ \
    struct HBOOST_SCOPE_EXIT_AUX_LAMBDA_PARAMS(id) { \
        typedef HBOOST_SCOPE_EXIT_AUX_LAMBDA_THIS_CAPTURE_TYPE(id) \
                HBOOST_SCOPE_EXIT_AUX_LAMBDA_THIS_PARAM_TYPE(id); \
    };

#define HBOOST_SCOPE_EXIT_AUX_IMPL_LAMBDA(id, ty, traits) \
    HBOOST_PP_IIF(HBOOST_SCOPE_EXIT_AUX_TRAITS_HAS_THIS(traits), \
        /* no need for TYPEDEF THIS MSVC workaround on C++11 */ \
        HBOOST_SCOPE_EXIT_AUX_LAMBDA_THIS_TYPEDEFS \
    , \
        HBOOST_PP_TUPLE_EAT(3) \
    )(id, ty, traits) \
    ::hboost::scope_exit::aux::guard< \
        HBOOST_PP_IIF(HBOOST_SCOPE_EXIT_AUX_TRAITS_HAS_THIS(traits), \
            HBOOST_SCOPE_EXIT_AUX_LAMBDA_THIS_TYPE \
        , \
            HBOOST_PP_TUPLE_EAT(2) \
        )(id, ty) \
    > HBOOST_SCOPE_EXIT_AUX_GUARD(id) \
        HBOOST_PP_EXPR_IIF(HBOOST_SCOPE_EXIT_AUX_TRAITS_HAS_THIS(traits), \
            (this) \
        ) \
    ; \
    HBOOST_SCOPE_EXIT_AUX_GUARD(id) = [ \
        HBOOST_PP_LIST_ENUM(HBOOST_SCOPE_EXIT_AUX_TRAITS_CAPTURES(traits)) \
    ]( \
        HBOOST_PP_IIF(HBOOST_SCOPE_EXIT_AUX_TRAITS_HAS_THIS(traits), \
            HBOOST_SCOPE_EXIT_AUX_LAMBDA_THIS_TYPE \
        , \
            HBOOST_PP_TUPLE_EAT(2) \
        )(id, ty) \
        HBOOST_PP_EXPR_IIF(HBOOST_SCOPE_EXIT_AUX_TRAITS_HAS_THIS(traits), this_) \
    ) mutable /* can change value captures (as with SCOPE_EXIT) */ -> void

#endif // Lambdas.

#if defined(HBOOST_SCOPE_EXIT_CONFIG_USE_LAMBDAS) && \
        !defined(HBOOST_NO_CXX11_LAMBDAS) // Use lambda for SCOPE_EXIT (not just _ALL).

#define HBOOST_SCOPE_EXIT_AUX_IMPL(id, ty, traits) \
    HBOOST_SCOPE_EXIT_AUX_IMPL_LAMBDA(id, ty, traits)

#else // Not using lambdas.

// ty: EMPTY() | typename
#define HBOOST_SCOPE_EXIT_AUX_IMPL(id, ty, traits) \
    HBOOST_PP_LIST_FOR_EACH_I(HBOOST_SCOPE_EXIT_DETAIL_TAG_DECL, id, \
            HBOOST_SCOPE_EXIT_AUX_TRAITS_CAPTURES(traits)) \
    HBOOST_PP_LIST_FOR_EACH_I(HBOOST_SCOPE_EXIT_DETAIL_CAPTURE_DECL, (id, ty), \
            HBOOST_SCOPE_EXIT_AUX_TRAITS_CAPTURES(traits)) \
    HBOOST_PP_IIF(HBOOST_SCOPE_EXIT_AUX_TRAITS_HAS_THIS(traits), \
        HBOOST_SCOPE_EXIT_DETAIL_TYPEDEF_TYPEOF_THIS \
    , \
        HBOOST_PP_TUPLE_EAT(3) \
    )(id, ty, HBOOST_SCOPE_EXIT_AUX_THIS_CAPTURE_T(id)) \
    struct HBOOST_SCOPE_EXIT_DETAIL_PARAMS_T(id) { \
        /* interim capture types to workaround internal errors on old GCC */ \
        HBOOST_PP_LIST_FOR_EACH_I(HBOOST_SCOPE_EXIT_DETAIL_PARAM_DECL, (id, ty), \
                HBOOST_SCOPE_EXIT_AUX_TRAITS_CAPTURES(traits)) \
        HBOOST_PP_EXPR_IIF(HBOOST_SCOPE_EXIT_AUX_TRAITS_HAS_THIS(traits), \
            typedef HBOOST_SCOPE_EXIT_AUX_THIS_CAPTURE_T(id) \
                    HBOOST_SCOPE_EXIT_AUX_THIS_T(id) ; \
        ) \
        HBOOST_PP_LIST_FOR_EACH_I(HBOOST_SCOPE_EXIT_AUX_MEMBER, id, \
                HBOOST_SCOPE_EXIT_AUX_TRAITS_CAPTURES(traits)) \
        HBOOST_PP_EXPR_IIF(HBOOST_SCOPE_EXIT_AUX_TRAITS_HAS_THIS(traits), \
            HBOOST_SCOPE_EXIT_AUX_THIS_T(id) \
                    HBOOST_SCOPE_EXIT_DETAIL_PARAM_THIS(id) ; \
        ) \
        HBOOST_SCOPE_EXIT_AUX_PARAMS_T_CTOR(id, ty, \
                HBOOST_SCOPE_EXIT_AUX_TRAITS_CAPTURES(traits), \
                HBOOST_SCOPE_EXIT_AUX_TRAITS_HAS_THIS(traits)) \
    } HBOOST_SCOPE_EXIT_AUX_PARAMS(id) \
        HBOOST_SCOPE_EXIT_AUX_PARAMS_INIT(id, \
                HBOOST_SCOPE_EXIT_AUX_TRAITS_CAPTURES(traits), \
                HBOOST_SCOPE_EXIT_AUX_TRAITS_HAS_THIS(traits)) \
    ; \
    ::hboost::scope_exit::detail::declared< \
        ::hboost::scope_exit::detail::resolve< \
            sizeof(HBOOST_SCOPE_EXIT_AUX_ARGS) \
        >::cmp1<0>::cmp2 \
    > HBOOST_SCOPE_EXIT_AUX_ARGS; \
    HBOOST_SCOPE_EXIT_AUX_ARGS.value = &HBOOST_SCOPE_EXIT_AUX_PARAMS(id); \
    struct HBOOST_SCOPE_EXIT_AUX_GUARD_T(id) { \
        HBOOST_SCOPE_EXIT_DETAIL_PARAMS_T(id)* hboost_se_params_; \
        HBOOST_SCOPE_EXIT_AUX_GUARD_T(id) (void* hboost_se_params) \
            : hboost_se_params_( \
                    (HBOOST_SCOPE_EXIT_DETAIL_PARAMS_T(id)*)hboost_se_params) \
        {} \
        ~HBOOST_SCOPE_EXIT_AUX_GUARD_T(id)() { \
            hboost_se_body( \
                HBOOST_PP_LIST_FOR_EACH_I(HBOOST_SCOPE_EXIT_AUX_ARG, id, \
                        HBOOST_SCOPE_EXIT_AUX_TRAITS_CAPTURES(traits)) \
                HBOOST_PP_COMMA_IF(HBOOST_PP_BITAND(HBOOST_PP_LIST_IS_CONS( \
                        HBOOST_SCOPE_EXIT_AUX_TRAITS_CAPTURES(traits)), \
                        HBOOST_SCOPE_EXIT_AUX_TRAITS_HAS_THIS(traits))) \
                HBOOST_PP_EXPR_IIF(HBOOST_SCOPE_EXIT_AUX_TRAITS_HAS_THIS( \
                        traits), \
                    hboost_se_params_->HBOOST_SCOPE_EXIT_DETAIL_PARAM_THIS(id) \
                ) \
            ); \
        } \
        static void hboost_se_body( \
            HBOOST_PP_LIST_FOR_EACH_I(HBOOST_SCOPE_EXIT_AUX_ARG_DECL, (id, ty), \
                    HBOOST_SCOPE_EXIT_AUX_TRAITS_CAPTURES(traits)) \
            HBOOST_PP_COMMA_IF(HBOOST_PP_BITAND(HBOOST_PP_LIST_IS_CONS( \
                    HBOOST_SCOPE_EXIT_AUX_TRAITS_CAPTURES(traits)), \
                    HBOOST_SCOPE_EXIT_AUX_TRAITS_HAS_THIS(traits))) \
            HBOOST_PP_EXPR_IIF(HBOOST_SCOPE_EXIT_AUX_TRAITS_HAS_THIS(traits), \
                ty HBOOST_SCOPE_EXIT_DETAIL_PARAMS_T(id):: \
                        HBOOST_SCOPE_EXIT_AUX_THIS_T(id) this_ \
            ) \
        )

#endif // Using lambdas.

// PUBLIC //

#if defined(HBOOST_NO_CXX11_VARIADIC_MACROS) // No variadic macros (sequences only).
#   define HBOOST_SCOPE_EXIT_ID(id, void_or_seq) \
        HBOOST_SCOPE_EXIT_AUX_IMPL(id, HBOOST_PP_EMPTY(), \
                HBOOST_SCOPE_EXIT_AUX_TRAITS( \
                        HBOOST_SCOPE_EXIT_AUX_PP_VOID_LIST(void_or_seq)))
#   define HBOOST_SCOPE_EXIT_ID_TPL(id, void_or_seq) \
        HBOOST_SCOPE_EXIT_AUX_IMPL(id, typename, \
                HBOOST_SCOPE_EXIT_AUX_TRAITS( \
                        HBOOST_SCOPE_EXIT_AUX_PP_VOID_LIST(void_or_seq)))
#   define HBOOST_SCOPE_EXIT(void_or_seq) \
        HBOOST_SCOPE_EXIT_ID(HBOOST_SCOPE_EXIT_AUX_PP_LINE_COUNTER, \
                void_or_seq)
#   define HBOOST_SCOPE_EXIT_TPL(void_or_seq) \
        HBOOST_SCOPE_EXIT_ID_TPL(HBOOST_SCOPE_EXIT_AUX_PP_LINE_COUNTER, \
                void_or_seq)
#   if !defined(HBOOST_NO_CXX11_LAMBDAS)
#       define HBOOST_SCOPE_EXIT_ALL_ID(id, seq) \
            HBOOST_SCOPE_EXIT_AUX_IMPL_LAMBDA(id, \
                    /* C++11 allows to use typename outside templates so */ \
                    /* always typename here and no need for ..._ALL_TPL */ \
                    /* (if a C++11 compiler does not implement this use of */ \
                    /* typename, always use `this` instead of `this_`) */ \
                    typename, \
                    HBOOST_SCOPE_EXIT_AUX_TRAITS_ALL( \
                            HBOOST_SCOPE_EXIT_AUX_PP_NON_VOID_LIST(seq)))
#       define HBOOST_SCOPE_EXIT_ALL(seq) \
            HBOOST_SCOPE_EXIT_ALL_ID( \
                    HBOOST_SCOPE_EXIT_AUX_PP_LINE_COUNTER, seq)
#   endif
#else // Variadic macros (both sequences and variadic tuples).
#   define HBOOST_SCOPE_EXIT_ID(id, ...) \
        HBOOST_SCOPE_EXIT_AUX_IMPL(id, HBOOST_PP_EMPTY(), \
                HBOOST_SCOPE_EXIT_AUX_TRAITS( \
                        HBOOST_SCOPE_EXIT_AUX_PP_VOID_LIST(__VA_ARGS__)))
#   define HBOOST_SCOPE_EXIT_ID_TPL(id, ...) \
        HBOOST_SCOPE_EXIT_AUX_IMPL(id, typename, \
                HBOOST_SCOPE_EXIT_AUX_TRAITS( \
                        HBOOST_SCOPE_EXIT_AUX_PP_VOID_LIST(__VA_ARGS__)))
#   define HBOOST_SCOPE_EXIT(...) \
        HBOOST_SCOPE_EXIT_ID(HBOOST_SCOPE_EXIT_AUX_PP_LINE_COUNTER, \
                __VA_ARGS__)
#   define HBOOST_SCOPE_EXIT_TPL(...) \
        HBOOST_SCOPE_EXIT_ID_TPL(HBOOST_SCOPE_EXIT_AUX_PP_LINE_COUNTER, \
                __VA_ARGS__)
#   if !defined(HBOOST_NO_CXX11_LAMBDAS)
#       define HBOOST_SCOPE_EXIT_ALL_ID(id, ...) \
            HBOOST_SCOPE_EXIT_AUX_IMPL_LAMBDA(id, \
                    /* C++11 allows to use typename outside templates so */ \
                    /* always typename here and no need for ..._ALL_TPL */ \
                    /* (if a C++11 compiler does not implement this use of */ \
                    /* typename, always use `this` instead of `this_`) */ \
                    typename, \
                    HBOOST_SCOPE_EXIT_AUX_TRAITS_ALL( \
                            HBOOST_SCOPE_EXIT_AUX_PP_NON_VOID_LIST( \
                                    __VA_ARGS__)))
#       define HBOOST_SCOPE_EXIT_ALL(...) \
            HBOOST_SCOPE_EXIT_ALL_ID( \
                    HBOOST_SCOPE_EXIT_AUX_PP_LINE_COUNTER, __VA_ARGS__)
#   endif
#endif // Variadics.

#if defined(HBOOST_SCOPE_EXIT_CONFIG_USE_LAMBDAS) && \
        !defined(HBOOST_NO_CXX11_LAMBDAS) // Use lambdas for SCOPE_EXIT (not just ALL).
#   define HBOOST_SCOPE_EXIT_END_ID(id) \
        ; /* lambdas ended with just `;` */
#else // Not using lambdas.
#   define HBOOST_SCOPE_EXIT_END_ID(id) \
        } HBOOST_SCOPE_EXIT_AUX_GUARD(id)(HBOOST_SCOPE_EXIT_AUX_ARGS.value);
#endif // Using lambdas.
#define HBOOST_SCOPE_EXIT_END \
    HBOOST_SCOPE_EXIT_END_ID(HBOOST_SCOPE_EXIT_AUX_PP_LINE_COUNTER)

// DOCUMENTATION //

#else // DOXYGEN

/** @file
@brief Scope exits allow to execute arbitrary code when the enclosing scope
exits.
*/

/**
@brief This macro declares a scope exit.

The scope exit declaration schedules the execution of the scope exit body at
the exit of the enclosing scope:

@code
    { // Some local scope.
        ...
        HBOOST_SCOPE_EXIT(capture_list) {
            ... // Body code.
        } HBOOST_SCOPE_EXIT_END
        ...
    }
@endcode

The enclosing scope must be local.
If multiple scope exits are declared within the same enclosing scope, the scope
exit bodies are executed in the reversed order of their declarations.
Note how the end of the scope exit body must be marked by
@RefMacro{HBOOST_SCOPE_EXIT_END}.

@Params
@Param{capture_list,
On compilers that support variadic macros (see also Boost.Config
<c>HBOOST_NO_CXX11_VARIADIC_MACROS</c>)\, the capture list syntax is defined by the
following grammar:
@code
    capture_list:
            void | capture_tuple | capture_sequence
    capture_tuple:
            capture\, capture\, ...
    capture_sequence:
            (capture) (capture) ...
    capture:
            [&]variable | this_
@endcode
On compilers that do not support variadic macros\, <c>capture_tuple</c> cannot
be used:
@code
    capture_list:
            void | capture_sequence
@endcode
Furthermore\, if @RefMacro{HBOOST_SCOPE_EXIT_CONFIG_USE_LAMBDAS} is defined on
C++11 compilers that support lambda functions (i.e.\, Boost.Config's <c>HBOOST_NO_CXX11_LAMBDAS</c> is not defined) then a semicolon <c>;</c> can be used instead of
@RefMacro{HBOOST_SCOPE_EXIT_END} and <c>this</c> can be used instead of
<c>this_</c>:
@code
    capture:
            [&]variable | this_ | this
@endcode

(Lexical conventions: <c>token1 | token2</c> means either <c>token1</c> or
<c>token2</c>; <c>[token]</c> means either <c>token</c> or nothing;
<c>{expression}</c> means the tokens resulting from the expression.)
}
@EndParams

Note that on compilers that support variadic macros (most of moder compliers
and all C++11 compilers), the capture list can be specified as a
comma-separated list of tokens (this is the preferred syntax).
However, on all compilers the same macro @RefMacro{HBOOST_SCOPE_EXIT} also
allows to specify the capture list as a Boost.Preprocessor sequence of tokens
(for supporting compilers without variadic macros and for backward compatibility with older versions of this library).

The name <c>variable</c> of each captured variable must be a valid name in the
enclosing scope and it must appear exactly once in the capture list.
If a capture starts with the ampersand sign <c>&</c>, the corresponding
variable will be available by reference within the scope exit body; otherwise,
a copy of the variable will be made at the point of the scope exit declaration
and that copy will be available inside the scope exit body (in this case, the
variable's type must be <c>CopyConstructible</c>).

From within a member function, the object <c>this</c> can be captured using the
special name <c>this_</c> in both the capture list and the scope exit body
(using <c>this</c> instead of <c>this_</c> in the scope exit body leads to
undefined behaviour).

It is possible to capture no variable by specifying the capture list as
<c>void</c> (regardless of variadic macro support).

Only variables listed in the capture list, static variables, <c>extern</c>
variables, global variables, functions, and enumerations from the enclosing
scope can be used inside the scope exit body.

On various GCC versions the special macro @RefMacro{HBOOST_SCOPE_EXIT_TPL} must
be used instead of @RefMacro{HBOOST_SCOPE_EXIT} within templates (to maximize
portability, it is recommended to always use @RefMacro{HBOOST_SCOPE_EXIT_TPL}
within templates).

On C++11, it is possible capture all variables in scope without listing their
names one-by-one using the macro @RefMacro{HBOOST_SCOPE_EXIT_ALL}.

In general, the special macro @RefMacro{HBOOST_SCOPE_EXIT_ID} must be used
instead of @RefMacro{HBOOST_SCOPE_EXIT} when it is necessary to expand multiple
scope exit declarations on the same line.

@Warning The implementation executes the scope exit body within a destructor
thus the scope exit body must never throw in order to comply with STL exception
safety requirements.

@Note The implementation uses Boost.Typeof to automatically deduce the types of
the captured variables.
In order to compile code in type-of emulation mode, all types must be properly
registered with Boost.Typeof (see the
@RefSect{getting_started, Getting Started} section).

@See @RefSect{tutorial, Tutorial} section,
@RefSect{getting_started, Getting Started} section,
@RefSect{no_variadic_macros, No Variadic Macros} section,
@RefMacro{HBOOST_SCOPE_EXIT_TPL}, @RefMacro{HBOOST_SCOPE_EXIT_ALL},
@RefMacro{HBOOST_SCOPE_EXIT_END}, @RefMacro{HBOOST_SCOPE_EXIT_ID}.
*/
#define HBOOST_SCOPE_EXIT(capture_list)

/**
@brief This macro is a workaround for various versions of GCC to declare scope
exits within templates.

Various versions of the GCC compiler do not compile @RefMacro{HBOOST_SCOPE_EXIT}
inside function templates.
As a workaround, @RefMacro{HBOOST_SCOPE_EXIT_TPL} should be used instead of
@RefMacro{HBOOST_SCOPE_EXIT} in these cases:

@code
    { // Some local scope.
        ...
        HBOOST_SCOPE_EXIT_TPL(capture_list) {
            ... // Body code.
        } HBOOST_SCOPE_EXIT_END
        ...
    }
@endcode

The syntax of @RefMacro{HBOOST_SCOPE_EXIT_TPL} is the exact same as the one of
@RefMacro{HBOOST_SCOPE_EXIT} (see @RefMacro{HBOOST_SCOPE_EXIT} for more
information).

On C++11 compilers, @RefMacro{HBOOST_SCOPE_EXIT_TPL} is not needed because
@RefMacro{HBOOST_SCOPE_EXIT} always compiles on GCC versions that support C++11.
However, @RefMacro{HBOOST_SCOPE_EXIT_TPL} is still provided on C++11 so to write code that is portable between C++03 and C++11 compilers.
It is recommended to always use @RefMacro{HBOOST_SCOPE_EXIT_TPL} within
templates so to maximize portability.

In general, the special macro @RefMacro{HBOOST_SCOPE_EXIT_ID_TPL} must be used
instead of @RefMacro{HBOOST_SCOPE_EXIT_TPL} when it is necessary to expand
multiple scope exit declarations on the same line within templates.

@Note The issue in compiling scope exit declarations that some GCC versions
have is illustrated by the following code (see also
<a href="http://gcc.gnu.org/bugzilla/show_bug.cgi?id=37920">GCC bug 37920</a>):
@code
    template<class T>
    void f(T const& x) {
        int i = 0;
        struct local {
            typedef __typeof__(i) typeof_i;
            typedef __typeof__(x) typeof_x;
        };
        typedef local::typeof_i i_type;
        typedef local::typeof_x x_type;
    }

    int main(void) { f(0); }
@endcode
This can be fixed by adding <c>typename</c> in front of <c>local::typeof_i</c>
and <c>local::typeof_x</c> (which is the approach followed by the
implementation of the @RefMacro{HBOOST_SCOPE_EXIT_TPL} macro).

@Note Although @RefMacro{HBOOST_SCOPE_EXIT_TPL} has the same suffix as
<c>HBOOST_TYPEOF_TPL</c>, it does not follow the Boost.Typeof convention.

@See @RefSect{tutorial, Tutorial} section, @RefMacro{HBOOST_SCOPE_EXIT},
@RefMacro{HBOOST_SCOPE_EXIT_END}, @RefMacro{HBOOST_SCOPE_EXIT_ID_TPL}.
*/
#define HBOOST_SCOPE_EXIT_TPL(capture_list)

/**
@brief This macro allows to expand multiple scope exit declarations on the same
line.

This macro is equivalent to @RefMacro{HBOOST_SCOPE_EXIT} but it can be expanded
multiple times on the same line if different identifiers <c>id</c> are provided
for each expansion (see @RefMacro{HBOOST_SCOPE_EXIT} for more information).

@Params
@Param{id,
A unique identifier token which can be concatenated by the preprocessor
(<c>__LINE__</c>\, <c>scope_exit_number_1_on_line_123</c>\, a combination of
alphanumeric tokens\, etc).
}
@Param{capture_list,
Same as the <c>capture_list</c> parameter of the @RefMacro{HBOOST_SCOPE_EXIT}
macro.
}
@EndParams

@Note This macro can be useful when the scope exit macros are expanded
within user-defined macros (because nested macros expand on the same line).
On some compilers (e.g., MSVC which supports the non standard
<c>__COUNTER__</c> macro) it might not be necessary to use this macro but
the use of this macro is always necessary to ensure portability when expanding
multiple scope exit declarations on the same line.

@See @RefSect{tutorial, Tutorial} section, @RefMacro{HBOOST_SCOPE_EXIT},
@RefMacro{HBOOST_SCOPE_EXIT_END_ID}, @RefMacro{HBOOST_SCOPE_EXIT_ALL_ID},
@RefMacro{HBOOST_SCOPE_EXIT_ID_TPL}.
*/
#define HBOOST_SCOPE_EXIT_ID(id, capture_list)

/**
@brief This macro is required to expand multiple scope exit declarations on the
same line within templates on various versions of GCC.

This macro is equivalent to @RefMacro{HBOOST_SCOPE_EXIT_TPL} but it can be
expanded multiple times on the same line if different identifiers <c>id</c> are
provided for each expansion (see @RefMacro{HBOOST_SCOPE_EXIT_TPL} for more
information).
As with @RefMacro{HBOOST_SCOPE_EXIT_TPL}, it is recommended to always use this
macro when expanding scope exits multiple times on the same line within
templates.

@Params
@Param{id,
A unique identifier token which can be concatenated by the preprocessor
(<c>__LINE__</c>\, <c>scope_exit_number_1_on_line_123</c>\, a combination of
alphanumeric tokens\, etc).
}
@Param{capture_list,
Same as the <c>capture_list</c> parameter of the
@RefMacro{HBOOST_SCOPE_EXIT_TPL} macro.
}
@EndParams

@Note This macro can be useful when the scope exit macros are expanded
within user-defined macros (because nested macros expand on the same line).
On some compilers (e.g., MSVC which supports the non standard
<c>__COUNTER__</c> macro) it might not be necessary to use this macro but
the use of this macro is always necessary to ensure portability when expanding
multiple scope exit declarations on the same line.

@See @RefSect{tutorial, Tutorial} section, @RefMacro{HBOOST_SCOPE_EXIT_TPL},
@RefMacro{HBOOST_SCOPE_EXIT_END_ID}, @RefMacro{HBOOST_SCOPE_EXIT_ID},
@RefMacro{HBOOST_SCOPE_EXIT_ALL_ID}.
*/
#define HBOOST_SCOPE_EXIT_ID_TPL(id, capture_list)

/**
@brief This macro declares a scope exit that captures all variables in scope
(C++11 only).

This macro accepts a capture list starting with either <c>&</c> or <c>=</c> to capture all variables in scope by reference or value respectively (following the same syntax of C++11 lambdas).
A part from that, this macro works like @RefMacro{HBOOST_SCOPE_EXIT} (see @RefMacro{HBOOST_SCOPE_EXIT} for more information):

@code
    { // Some local scope.
        ...
        HBOOST_SCOPE_EXIT_ALL(capture_list) { // C++11 only.
            ... // Body code.
        }; // Use `;` instead of `HBOOST_SCOPE_EXIT_END` (C++11 only).
        ...
    }
@endcode

Note how the end of the scope exit body declared by this macro must be marked
by a semi-column <c>;</c> (and not by @RefMacro{HBOOST_SCOPE_EXIT_END}).

@Warning This macro is only available on C++11 compilers (specifically, on
C++11 compilers that do not define the Boost.Config <c>HBOOST_NO_CXX11_LAMBDAS</c>
macro).
It is not defined on non-C++11 compilers so its use on non-C++11 compilers will generate a compiler error.

@Params
@Param{capture_list,
On compilers that support variadic macros (see also Boost.Config
<c>HBOOST_NO_CXX11_VARIADIC_MACROS</c>)\, the capture list syntax is defined by the
following grammar:
@code
capture_list:
        capture_tuple | capture_sequence
capture_tuple:
        {& | =} [\, capture\, capture\, ...]
capture_sequence:
        {(&) | (=)} [(capture) (capture) ...]
capture:
        [&]variable | this_
@endcode
On compilers that do not support variadic macros\, <c>capture_tuple</c> cannot
be used:
@code
    capture_list:
            void | capture_sequence
@endcode
Furthermore\, on C++11 compilers that support the use of <c>typename</c>
outside templates\, also <c>this</c> can be used to capture the object at member
function scope:
@code
    capture:
            [&]variable | this_ | this
@endcode

(Lexical conventions: <c>token1 | token2</c> means either <c>token1</c> or
<c>token2</c>; <c>[token]</c> means either <c>token</c> or nothing;
<c>{expression}</c> means the token resulting from the expression.)
}
@EndParams

Note that on compilers with variadic macro support (which should be all C++11
compilers), the capture list can be specified as a comma-separated list.
On all compilers, the same macro @RefMacro{HBOOST_SCOPE_EXIT_ALL} also allows to
specify the capture list as a Boost.Preprocessor sequence.

The capture list must always contain at least the leading <c>&</c> or <c>=</c>
so it can never be <c>void</c> (<c>HBOOST_SCOPE_EXIT(void)</c> should be used
to program scope exits with an empty capture list).

In general, the special macro @RefMacro{HBOOST_SCOPE_EXIT_ALL_ID} must be used
instead of @RefMacro{HBOOST_SCOPE_EXIT_ALL} when it is necessary to expand
multiple scope exit declarations on the same line.

@Warning This macro capture list follows the exact same syntax of C++11 lambda
captures which is unfortunately different from the syntax of
@RefMacro{HBOOST_SCOPE_EXIT} captures (unless programmers define the
@RefMacro{HBOOST_SCOPE_EXIT_CONFIG_USE_LAMBDAS} macro).
For example, like C++11 lambda functions, @RefMacro{HBOOST_SCOPE_EXIT_ALL}
requires to capture data members by capturing the object <c>this</c> while
@RefMacro{HBOOST_SCOPE_EXIT} allows to capture data members directly and without
capturing the object.

@Warning The implementation executes the scope exit body within a destructor
thus the scope exit body must never throw in order to comply with STL exception
safety requirements.

@Note This macro can always be used also within templates (so there is no need
for a <c>HBOOST_SCOPE_EXIT_ALL_TPL</c> macro).

@See @RefSect{tutorial, Tutorial} section,
@RefSect{no_variadic_macros, No Variadic Macros} section,
@RefMacro{HBOOST_SCOPE_EXIT}, @RefMacro{HBOOST_SCOPE_EXIT_ALL_ID}.
*/
#define HBOOST_SCOPE_EXIT_ALL(capture_list)

/**
@brief This macro allows to expand on the same line multiple scope exits that
capture all variables in scope (C++11 only).

This macro is equivalent to @RefMacro{HBOOST_SCOPE_EXIT_ALL} but it can be
expanded multiple times on the same line if different identifiers <c>id</c> are
provided for each expansion (see @RefMacro{HBOOST_SCOPE_EXIT_ALL} for more
information).
As with @RefMacro{HBOOST_SCOPE_EXIT_ALL}, this macro is only available on C++11
compilers (specifically, on C++11 compilers that do not define the
Boost.Config <c>HBOOST_NO_CXX11_LAMBDAS</c> macro).

@Params
@Param{id,
A unique identifier token which can be concatenated by the preprocessor
(<c>__LINE__</c>\, <c>scope_exit_number_1_on_line_123</c>\, a combination of
alphanumeric tokens\, etc).
}
@Param{capture_list,
Same as the <c>capture_list</c> parameter of the
@RefMacro{HBOOST_SCOPE_EXIT_ALL} macro.
}
@EndParams

@Note This macro can be useful when the scope exit macros are expanded
within user-defined macros (because nested macros expand on the same line).
On some compilers (e.g., MSVC which supports the non standard
<c>__COUNTER__</c> macro) it might not be necessary to use this macro but
the use of this macro is always necessary to ensure portability when expanding
multiple scope exit declarations on the same line.

@See @RefSect{tutorial, Tutorial} section, @RefMacro{HBOOST_SCOPE_EXIT_ALL},
@RefMacro{HBOOST_SCOPE_EXIT_ID}.
*/
#define HBOOST_SCOPE_EXIT_ALL_ID(id, capture_list)

/**
@brief This macro marks the end of a scope exit body.

This macro must follow the closing curly bracket <c>}</c> that ends the body of
either @RefMacro{HBOOST_SCOPE_EXIT} or @RefMacro{HBOOST_SCOPE_EXIT_TPL}:

@code
    { // Some local scope.
        ...
        HBOOST_SCOPE_EXIT(capture_list) {
            ... // Body code.
        } HBOOST_SCOPE_EXIT_END
        ...
    }
@endcode

In general, the special macro @RefMacro{HBOOST_SCOPE_EXIT_END_ID} must be used
instead of @RefMacro{HBOOST_SCOPE_EXIT_END} when it is necessary to expand
multiple scope exit bodies on the same line.

@Note If programmers define the @RefMacro{HBOOST_SCOPE_EXIT_CONFIG_USE_LAMBDAS}
macro on C++11 compilers, a semicolon <c>;</c> can be used instead of this
macro.
However, to maximize portability, it is recommended to always use
@RefMacro{HBOOST_SCOPE_EXIT_END}.

@See @RefSect{tutorial, Tutorial} section, @RefMacro{HBOOST_SCOPE_EXIT},
@RefMacro{HBOOST_SCOPE_EXIT_TPL}, @RefMacro{HBOOST_SCOPE_EXIT_END_ID}.
*/
#define HBOOST_SCOPE_EXIT_END

/**
@brief This macro allows to terminate multiple scope exit bodies on the same
line.

This macro is equivalent to @RefMacro{HBOOST_SCOPE_EXIT_END} but it can be
expanded multiple times on the same line if different identifiers <c>id</c> are
provided for each expansion (see @RefMacro{HBOOST_SCOPE_EXIT_END} for more
information).

@Params
@Param{id,
A unique identifier token which can be concatenated by the preprocessor
(<c>__LINE__</c>\, <c>scope_exit_number_1_on_line_123</c>\, a combination of
alphanumeric tokens\, etc).
}
@EndParams

@Note This macro can be useful when the scope exit macros are expanded
within user-defined macros (because macros all expand on the same line).
On some compilers (e.g., MSVC which supports the non standard
<c>__COUNTER__</c> macro) it might not be necessary to use this macro but
the use of this macro is always necessary to ensure portability when expanding
multiple scope exit macros on the same line (because this library can only
portably use <c>__LINE__</c> to internally generate unique identifiers).

@See @RefMacro{HBOOST_SCOPE_EXIT_ID}, @RefMacro{HBOOST_SCOPE_EXIT_ID_TPL},
@RefMacro{HBOOST_SCOPE_EXIT_END}.
*/
#define HBOOST_SCOPE_EXIT_END_ID(id)

/**
@brief Force to use C++11 lambda functions to implement scope exits.

If programmers define this configuration macro on a C++11 compiler for which
the Boost.Config macro <c>HBOOST_NO_CXX11_LAMBDAS</c> is not defined, the
@RefMacro{HBOOST_SCOPE_EXIT} and @RefMacro{HBOOST_SCOPE_EXIT_TPL} macros will use
C++11 lambda functions to declare scope exits.
By default this macro is not defined.

@Warning When scope exits are implemented using lambda functions, the syntax of
the capture list follows the exact same syntax of C++11 lambda captures
which is in general different from the legacy capture syntax of this library.
For example, C++11 lambdas require to capture data members by capturing the
object <c>this</c> while this library always allowed to capture data members
directly.
Therefore, when this configuration macro is defined,
@RefMacro{HBOOST_SCOPE_EXIT} and @RefMacro{HBOOST_SCOPE_EXIT_TPL} are no longer
backward compatible (and this is why this macro is not defined by default).

A semicolon <c>;</c> can be used instead of @RefMacro{HBOOST_SCOPE_EXIT_END}
when this configuration macro is defined (but it is recommended to always use
@RefMacro{HBOOST_SCOPE_EXIT_END} so to maximize portability).

@Note This configuration macro does not control the definition of
@RefMacro{HBOOST_SCOPE_EXIT_ALL} which is always and automatically defined on
compilers that support C++11 lambda functions.

@See @RefMacro{HBOOST_SCOPE_EXIT}, @RefMacro{HBOOST_SCOPE_EXIT_TPL},
@RefMacro{HBOOST_SCOPE_EXIT_END}.
*/
#define HBOOST_SCOPE_EXIT_CONFIG_USE_LAMBDAS

#endif // DOXYGEN

#endif // #ifndef FILE_hboost_scope_exit_hpp_INCLUDED

