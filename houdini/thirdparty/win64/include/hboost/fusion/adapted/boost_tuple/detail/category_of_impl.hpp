/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_CATEGORY_OF_IMPL_09272006_0726)
#define HBOOST_FUSION_CATEGORY_OF_IMPL_09272006_0726

namespace hboost { namespace fusion 
{ 
    struct hboost_tuple_tag;
    struct forward_traversal_tag;

    namespace extension
    {
        template<typename T>
        struct category_of_impl;

        template<>
        struct category_of_impl<hboost_tuple_tag>
        {
            template<typename T>
            struct apply
            {
                typedef forward_traversal_tag type;
            };
        };
    }
}}

#endif
