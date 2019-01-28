
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

// no include guards, this file is intended for multiple inclusion

#if   HBOOST_FT_ARITY_LOOP_PREFIX

#   ifndef HBOOST_FT_DETAIL_SYNTHESIZE_IMPL_MASTER_HPP_INCLUDED
#   define HBOOST_FT_DETAIL_SYNTHESIZE_IMPL_MASTER_HPP_INCLUDED
#     include <hboost/preprocessor/cat.hpp>
#     include <hboost/preprocessor/arithmetic/dec.hpp>
#     include <hboost/preprocessor/iteration/local.hpp>
#     include <hboost/preprocessor/facilities/empty.hpp>
#     include <hboost/preprocessor/facilities/identity.hpp>
#   endif

#   define HBOOST_FT_type_name type

#   ifdef HBOOST_FT_flags
#     define HBOOST_FT_make_type(flags,cc,arity) HBOOST_FT_make_type_impl(flags,cc,arity)
#     define HBOOST_FT_make_type_impl(flags,cc,arity) make_type_ ## flags ## _ ## cc ## _ ## arity
#   else
HBOOST_PP_EXPAND(#) define HBOOST_FT_make_type(flags,cc,arity) HBOOST_FT_make_type_impl(flags,cc,arity)
HBOOST_PP_EXPAND(#) define HBOOST_FT_make_type_impl(flags,cc,arity) make_type_ ## flags ## _ ## cc ## _ ## arity
#   endif

#   define HBOOST_FT_iter(i) HBOOST_PP_CAT(iter_,i)

#elif HBOOST_FT_ARITY_LOOP_IS_ITERATING

template< HBOOST_FT_tplargs(HBOOST_PP_IDENTITY(typename)) >
struct HBOOST_FT_make_type(HBOOST_FT_flags,HBOOST_FT_cc_id,HBOOST_FT_arity)
{
  typedef HBOOST_FT_type ;
};

template<> 
struct synthesize_impl_o< HBOOST_FT_flags, HBOOST_FT_cc_id, HBOOST_FT_n > 
{ 
  template<typename S> struct synthesize_impl_i
  {
  private:
    typedef typename mpl::begin<S>::type HBOOST_FT_iter(0);
#   if HBOOST_FT_n > 1
#     define HBOOST_PP_LOCAL_MACRO(i) typedef typename mpl::next< \
          HBOOST_FT_iter(HBOOST_PP_DEC(i)) >::type HBOOST_FT_iter(i);
#     define HBOOST_PP_LOCAL_LIMITS (1,HBOOST_FT_n-1)
#     include HBOOST_PP_LOCAL_ITERATE()
#   endif
  public:
    typedef typename detail::HBOOST_FT_make_type(HBOOST_FT_flags,HBOOST_FT_cc_id,HBOOST_FT_arity) 
    < typename mpl::deref< HBOOST_FT_iter(0) >::type 
#   if HBOOST_FT_mfp
    , typename detail::cv_traits< 
          typename mpl::deref< HBOOST_FT_iter(1) >::type >::type
#   endif
#   if HBOOST_FT_n > (HBOOST_FT_mfp+1)
#     define HBOOST_PP_LOCAL_LIMITS (HBOOST_FT_mfp+1,HBOOST_FT_n-1)
#     define HBOOST_PP_LOCAL_MACRO(i) \
        , typename mpl::deref< HBOOST_FT_iter(i) >::type
#     include HBOOST_PP_LOCAL_ITERATE()
#   endif
    >::type type;
  };
};

#elif HBOOST_FT_ARITY_LOOP_SUFFIX

#   ifdef HBOOST_FT_flags
#     undef HBOOST_FT_make_type
#     undef HBOOST_FT_make_type_impl
#   else
HBOOST_PP_EXPAND(#) undef HBOOST_FT_make_type
HBOOST_PP_EXPAND(#) undef HBOOST_FT_make_type_impl
#   endif
#   undef HBOOST_FT_iter
#   undef HBOOST_FT_type_name

#else
#   error "attempt to use arity loop master file without loop"
#endif

