// Copyright David Abrahams 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_CONCEPT_DETAIL_CONCEPT_DEF_DWA200651_HPP
# define HBOOST_CONCEPT_DETAIL_CONCEPT_DEF_DWA200651_HPP
# include <hboost/preprocessor/seq/for_each_i.hpp>
# include <hboost/preprocessor/seq/enum.hpp>
# include <hboost/preprocessor/comma_if.hpp>
# include <hboost/preprocessor/cat.hpp>
#endif // HBOOST_CONCEPT_DETAIL_CONCEPT_DEF_DWA200651_HPP

// HBOOST_concept(SomeName, (p1)(p2)...(pN))
//
// Expands to "template <class p1, class p2, ...class pN> struct SomeName"
//
// Also defines an equivalent SomeNameConcept for backward compatibility.
// Maybe in the next release we can kill off the "Concept" suffix for good.
# define HBOOST_concept(name, params)                                            \
    template < HBOOST_PP_SEQ_FOR_EACH_I(HBOOST_CONCEPT_typename,~,params) >       \
    struct name; /* forward declaration */                                      \
                                                                                \
    template < HBOOST_PP_SEQ_FOR_EACH_I(HBOOST_CONCEPT_typename,~,params) >       \
    struct HBOOST_PP_CAT(name,Concept)                                           \
      : name< HBOOST_PP_SEQ_ENUM(params) >                                       \
    {                                                                           \
    };                                                                          \
                                                                                \
    template < HBOOST_PP_SEQ_FOR_EACH_I(HBOOST_CONCEPT_typename,~,params) >       \
    struct name                                                                
    
// Helper for HBOOST_concept, above.
# define HBOOST_CONCEPT_typename(r, ignored, index, t) \
    HBOOST_PP_COMMA_IF(index) typename t

