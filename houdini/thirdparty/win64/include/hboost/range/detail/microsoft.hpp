#ifndef HBOOST_RANGE_DETAIL_MICROSOFT_HPP
#define HBOOST_RANGE_DETAIL_MICROSOFT_HPP

// Boost.Range MFC/ATL Extension
//
// Copyright Shunsuke Sogame 2005-2006.
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)




// config
//


#include <hboost/range/iterator.hpp>


#define HBOOST_RANGE_DETAIL_MICROSOFT_RANGE_VERSION_1 1


#if !defined(HBOOST_RANGE_DETAIL_MICROSOFT_RANGE_VERSION_1)
    #define HBOOST_RANGE_DETAIL_MICROSOFT_range_mutable_iterator range_mutable_iterator
    #define HBOOST_RANGE_DETAIL_MICROSOFT_range_begin            range_begin
    #define HBOOST_RANGE_DETAIL_MICROSOFT_range_end              range_end
#else
    #define HBOOST_RANGE_DETAIL_MICROSOFT_range_mutable_iterator range_mutable_iterator
    #define HBOOST_RANGE_DETAIL_MICROSOFT_range_begin            range_begin
    #define HBOOST_RANGE_DETAIL_MICROSOFT_range_end              range_end
#endif




// yet another customization way
//


#include <hboost/iterator/iterator_traits.hpp> // iterator_difference
#include <hboost/mpl/identity.hpp>
#include <hboost/mpl/if.hpp>
#include <hboost/preprocessor/cat.hpp>
#include <hboost/preprocessor/control/iif.hpp>
#include <hboost/preprocessor/comma_if.hpp>
#include <hboost/preprocessor/detail/is_unary.hpp>
#include <hboost/preprocessor/list/for_each.hpp>
#include <hboost/preprocessor/repetition/enum_params.hpp>
#include <hboost/preprocessor/repetition/repeat.hpp>
#include <hboost/preprocessor/seq/for_each_i.hpp>
#include <hboost/preprocessor/seq/size.hpp>
#include <hboost/preprocessor/tuple/eat.hpp>
#include <hboost/range/const_iterator.hpp>
#include <hboost/range/size_type.hpp>
#include <hboost/type_traits/is_const.hpp>
#include <hboost/type_traits/is_same.hpp>
#include <hboost/type_traits/remove_cv.hpp>
#include <hboost/utility/addressof.hpp>
#include <hboost/utility/enable_if.hpp> // disable_if

#if !defined(HBOOST_RANGE_DETAIL_MICROSOFT_RANGE_VERSION_1)
    #include <hboost/range/mutable_iterator.hpp>
#else
    #include <iterator> // distance
    #include <hboost/range/begin.hpp>
    #include <hboost/range/end.hpp>
    #include <hboost/range/iterator.hpp>
#endif


namespace hboost { namespace range_detail_microsoft {


    // customization point
    //

    template< class Tag >
    struct customization;


    template< class T >
    struct customization_tag;


    struct using_type_as_tag
    { };


    // Topic:
    // In fact, it is unnecessary for VC++.
    // VC++'s behavior seems conforming, while GCC fails without this.
    template< class Iterator, class T >
    struct mutable_ :
        disable_if< is_const<T>, Iterator >
    { };


    // helpers
    //

    template< class Tag, class T >
    struct customization_tag_of
    {
        typedef typename mpl::if_< is_same<using_type_as_tag, Tag>,
            T,
            Tag
        >::type type;
    };


    template< class T >
    struct customization_of
    {
        typedef typename remove_cv<T>::type bare_t;
        typedef typename customization_tag<bare_t>::type tag_t;
        typedef customization<tag_t> type;
    };


    template< class T >
    struct mutable_iterator_of
    {
        typedef typename remove_cv<T>::type bare_t;
        typedef typename customization_of<bare_t>::type cust_t;
        typedef typename cust_t::template meta<bare_t>::mutable_iterator type;
    };


    template< class T >
    struct const_iterator_of
    {
        typedef typename remove_cv<T>::type bare_t;
        typedef typename customization_of<bare_t>::type cust_t;
        typedef typename cust_t::template meta<bare_t>::const_iterator type;
    };


    template< class T >
    struct size_type_of
    {
        typedef typename range_detail_microsoft::mutable_iterator_of<T>::type miter_t;
        typedef typename iterator_difference<miter_t>::type type;
    };


    template< class T > inline
    typename mutable_iterator_of<T>::type
    begin_of(T& x)
    {
        typedef typename customization_of<T>::type cust_t;
        return cust_t().template begin<typename mutable_iterator_of<T>::type>(x);
    }


    template< class T > inline
    typename const_iterator_of<T>::type
    begin_of(T const& x)
    {
        typedef typename customization_of<T>::type cust_t;
        return cust_t().template begin<typename const_iterator_of<T>::type>(x);
    }


    template< class T > inline
    typename mutable_iterator_of<T>::type
    end_of(T& x)
    {
        typedef typename customization_of<T>::type cust_t;
        return cust_t().template end<typename mutable_iterator_of<T>::type>(x);
    }


    template< class T > inline
    typename const_iterator_of<T>::type
    end_of(T const& x)
    {
        typedef typename customization_of<T>::type cust_t;
        return cust_t().template end<typename const_iterator_of<T>::type>(x);
    }


#if defined(HBOOST_RANGE_DETAIL_MICROSOFT_RANGE_VERSION_1)

    template< class T > inline
    typename size_type_of<T>::type
    size_of(T const& x)
    {
        return std::distance(hboost::begin(x), hboost::end(x));
    }

#endif


    template< class Range >
    struct compatible_mutable_iterator : 
        HBOOST_RANGE_DETAIL_MICROSOFT_range_mutable_iterator<Range>
    { };


} } // namespace hboost::range_detail_microsoft


#define HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_namespace_open(NamespaceList) \
    HBOOST_PP_LIST_FOR_EACH(HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_namespace_open_op, ~, NamespaceList) \
/**/

    #define HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_namespace_open_op(r, data, elem) \
        namespace elem { \
    /**/


#define HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_namespace_close(NamespaceList) \
    HBOOST_PP_LIST_FOR_EACH(HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_namespace_close_op, ~, NamespaceList) \
/**/

    #define HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_namespace_close_op(r, data, elem) \
        } \
    /**/


#define HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_namespace_expand_op(r, data, elem) \
    :: elem \
/**/


#define HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TYPE(Tag, NamespaceList, Name) \
    namespace hboost { namespace range_detail_microsoft { \
        HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TYPE_tag(Tag, HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TYPE_fullname(NamespaceList, Name)) \
    } } \
    \
    namespace hboost { \
        HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TYPE_mutable_iterator(HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TYPE_fullname(NamespaceList, Name)) \
        HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TYPE_const_iterator(HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TYPE_fullname(NamespaceList, Name)) \
        HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TYPE_size_type(HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TYPE_fullname(NamespaceList, Name)) \
    } \
    \
    HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_namespace_open(NamespaceList) \
        HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TYPE_begin(HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TYPE_fullname(NamespaceList, Name)) \
        HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TYPE_begin_const(HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TYPE_fullname(NamespaceList, Name)) \
        HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TYPE_end(HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TYPE_fullname(NamespaceList, Name)) \
        HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TYPE_end_const(HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TYPE_fullname(NamespaceList, Name)) \
        HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TYPE_size(HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TYPE_fullname(NamespaceList, Name)) \
    HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_namespace_close(NamespaceList) \
/**/


    #define HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TYPE_fullname(NamespaceList, Name) \
        HBOOST_PP_LIST_FOR_EACH(HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_namespace_expand_op, ~, NamespaceList) :: Name \
    /**/


    #define HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TYPE_tag(Tag, Fullname) \
        template< > \
        struct customization_tag< Fullname > : \
            customization_tag_of< Tag, Fullname > \
        { }; \
    /**/


    // metafunctions
    //

    #define HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TYPE_mutable_iterator(Fullname) \
        template< > \
        struct HBOOST_RANGE_DETAIL_MICROSOFT_range_mutable_iterator< Fullname > : \
            range_detail_microsoft::mutable_iterator_of< Fullname > \
        { }; \
    /**/


    #define HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TYPE_const_iterator(Fullname) \
        template< > \
        struct range_const_iterator< Fullname > : \
            range_detail_microsoft::const_iterator_of< Fullname > \
        { }; \
    /**/


    #define HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TYPE_size_type(Fullname) \
        template< > \
        struct range_size< Fullname > : \
            range_detail_microsoft::size_type_of< Fullname > \
        { }; \
    /**/


    // functions
    //

    #define HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TYPE_begin(Fullname) \
        inline \
        hboost::range_detail_microsoft::mutable_iterator_of< Fullname >::type \
        HBOOST_RANGE_DETAIL_MICROSOFT_range_begin(Fullname& x) \
        { \
            return hboost::range_detail_microsoft::begin_of(x); \
        } \
    /**/


    #define HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TYPE_begin_const(Fullname) \
        inline \
        hboost::range_detail_microsoft::const_iterator_of< Fullname >::type \
        HBOOST_RANGE_DETAIL_MICROSOFT_range_begin(Fullname const& x) \
        { \
            return hboost::range_detail_microsoft::begin_of(x); \
        } \
    /**/


    #define HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TYPE_end(Fullname) \
        inline \
        hboost::range_detail_microsoft::mutable_iterator_of< Fullname >::type \
        HBOOST_RANGE_DETAIL_MICROSOFT_range_end(Fullname& x) \
        { \
            return hboost::range_detail_microsoft::end_of(x); \
        } \
    /**/


    #define HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TYPE_end_const(Fullname) \
        inline \
        hboost::range_detail_microsoft::const_iterator_of< Fullname >::type \
        HBOOST_RANGE_DETAIL_MICROSOFT_range_end(Fullname const& x) \
        { \
            return hboost::range_detail_microsoft::end_of(x); \
        } \
    /**/


    #if !defined(HBOOST_RANGE_DETAIL_MICROSOFT_RANGE_VERSION_1)

        #define HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TYPE_size(Fullname) \
        /**/

    #else

        #define HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TYPE_size(Fullname) \
            inline \
            hboost::range_detail_microsoft::size_type_of< Fullname >::type \
            hboost_range_size(Fullname const& x) \
            { \
                return hboost::range_detail_microsoft::size_of(x); \
            } \
        /**/

    #endif


#define HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE(Tag, NamespaceList, Name, ParamSeqOrCount) \
    HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_impl( \
        Tag, NamespaceList, Name, \
        HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_to_param_seq(ParamSeqOrCount) \
    ) \
/**/

    #define HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_to_param_seq(ParamSeqOrCount) \
        HBOOST_PP_IIF(HBOOST_PP_IS_UNARY(ParamSeqOrCount), \
            ParamSeqOrCount HBOOST_PP_TUPLE_EAT(3), \
            HBOOST_PP_REPEAT \
        )(ParamSeqOrCount, HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_to_param_seq_op, ~) \
    /**/

        #define HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_to_param_seq_op(z, n, _) \
            (class) \
        /**/


#define HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_impl(Tag, NamespaceList, Name, ParamSeq) \
    namespace hboost { namespace range_detail_microsoft { \
            HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_tag( \
            Tag, \
            HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_params(ParamSeq), \
            HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_fullname(NamespaceList, Name, ParamSeq) \
        ) \
    } } \
    \
    namespace hboost { \
        HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_mutable_iterator( \
            HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_params(ParamSeq), \
            HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_fullname(NamespaceList, Name, ParamSeq) \
        ) \
        HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_const_iterator( \
            HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_params(ParamSeq), \
            HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_fullname(NamespaceList, Name, ParamSeq) \
        ) \
        \
        HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_size_type( \
            HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_params(ParamSeq), \
            HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_fullname(NamespaceList, Name, ParamSeq) \
        ) \
    } \
    \
    HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_namespace_open(NamespaceList) \
        HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_begin( \
            HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_params(ParamSeq), \
            HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_fullname(NamespaceList, Name, ParamSeq) \
        ) \
        HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_begin_const( \
            HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_params(ParamSeq), \
            HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_fullname(NamespaceList, Name, ParamSeq) \
        ) \
        HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_end( \
            HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_params(ParamSeq), \
            HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_fullname(NamespaceList, Name, ParamSeq) \
        ) \
        HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_end_const( \
            HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_params(ParamSeq), \
            HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_fullname(NamespaceList, Name, ParamSeq) \
        ) \
        HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_size( \
            HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_params(ParamSeq), \
            HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_fullname(NamespaceList, Name, ParamSeq) \
        ) \
    HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_namespace_close(NamespaceList) \
/**/


    #define HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_params(ParamSeq) \
        HBOOST_PP_SEQ_FOR_EACH_I(HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_params_op, ~, ParamSeq) \
    /**/

        #define HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_params_op(r, data, i, elem) \
            HBOOST_PP_COMMA_IF(i) elem HBOOST_PP_CAT(T, i) \
        /**/


    #define HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_fullname(NamespaceList, Name, ParamSeq) \
        HBOOST_PP_LIST_FOR_EACH(HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_namespace_expand_op, ~, NamespaceList) \
        :: Name < HBOOST_PP_ENUM_PARAMS(HBOOST_PP_SEQ_SIZE(ParamSeq), T) > \
    /**/


    #define HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_tag(Tag, Params, Fullname) \
        template< Params > \
        struct customization_tag< Fullname > : \
            customization_tag_of< Tag, Fullname > \
        { }; \
    /**/


    // metafunctions
    //

    #define HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_mutable_iterator(Params, Fullname) \
        template< Params > \
        struct HBOOST_RANGE_DETAIL_MICROSOFT_range_mutable_iterator< Fullname > : \
            range_detail_microsoft::mutable_iterator_of< Fullname > \
        { }; \
    /**/


    #define HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_const_iterator(Params, Fullname) \
        template< Params > \
        struct range_const_iterator< Fullname > : \
            range_detail_microsoft::const_iterator_of< Fullname > \
        { }; \
    /**/


    #define HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_size_type(Params, Fullname) \
        template< Params > \
        struct range_size< Fullname > : \
            range_detail_microsoft::size_type_of< Fullname > \
        { }; \
    /**/


    // functions
    //

    #define HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_begin(Params, Fullname) \
        template< Params > inline \
        typename hboost::range_detail_microsoft::mutable_iterator_of< Fullname >::type \
        HBOOST_RANGE_DETAIL_MICROSOFT_range_begin(Fullname& x) \
        { \
            return hboost::range_detail_microsoft::begin_of(x); \
        } \
    /**/


    #define HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_begin_const(Params, Fullname) \
        template< Params > inline \
        typename hboost::range_detail_microsoft::const_iterator_of< Fullname >::type \
        HBOOST_RANGE_DETAIL_MICROSOFT_range_begin(Fullname const& x) \
        { \
            return hboost::range_detail_microsoft::begin_of(x); \
        } \
    /**/


    #define HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_end(Params, Fullname) \
        template< Params > inline \
        typename hboost::range_detail_microsoft::mutable_iterator_of< Fullname >::type \
        HBOOST_RANGE_DETAIL_MICROSOFT_range_end(Fullname& x) \
        { \
            return hboost::range_detail_microsoft::end_of(x); \
        } \
    /**/


    #define HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_end_const(Params, Fullname) \
        template< Params > inline \
        typename hboost::range_detail_microsoft::const_iterator_of< Fullname >::type \
        HBOOST_RANGE_DETAIL_MICROSOFT_range_end(Fullname const& x) \
        { \
            return hboost::range_detail_microsoft::end_of(x); \
        } \
    /**/


    #if !defined(HBOOST_RANGE_DETAIL_MICROSOFT_RANGE_VERSION_1)

        #define HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_size(Params, Fullname) \
        /**/

    #else

        #define HBOOST_RANGE_DETAIL_MICROSOFT_CUSTOMIZATION_TEMPLATE_size(Params, Fullname) \
            template< Params > inline \
            typename hboost::range_detail_microsoft::size_type_of< Fullname >::type \
            hboost_range_size(Fullname const& x) \
            { \
                return hboost::range_detail_microsoft::size_of(x); \
            } \
        /**/

    #endif




// list_iterator and helpers
//


#include <hboost/assert.hpp>
#include <hboost/iterator/iterator_categories.hpp>
#include <hboost/iterator/iterator_facade.hpp>
#include <hboost/mpl/if.hpp>
#include <hboost/type_traits/is_same.hpp>


// POSITION's header is undocumented, so is NULL.
//
struct __POSITION; // incomplete, but used as just a pointer.
typedef __POSITION *POSITION;


namespace hboost { namespace range_detail_microsoft {


    template<
        class ListT,
        class Value,
        class Reference,
        class Traversal
    >
    struct list_iterator;


    template<
        class ListT,
        class Value,
        class Reference,
        class Traversal
    >
    struct list_iterator_super
    {
        typedef typename mpl::if_< is_same<use_default, Reference>,
            Value&,
            Reference
        >::type ref_t;

        typedef typename mpl::if_< is_same<use_default, Traversal>,
            bidirectional_traversal_tag,
            Traversal
        >::type trv_t;

        typedef iterator_facade<
            list_iterator<ListT, Value, Reference, Traversal>,
            Value,
            trv_t,
            ref_t
        > type;
    };


    template<
        class ListT,
        class Value,
        class Reference = use_default,
        class Traversal = use_default
    >
    struct list_iterator :
        list_iterator_super<ListT, Value, Reference, Traversal>::type
    {
    private:
        typedef list_iterator self_t;
        typedef typename list_iterator_super<ListT, Value, Reference, Traversal>::type super_t;
        typedef typename super_t::reference ref_t;

    public:
        explicit list_iterator()
        { }

        explicit list_iterator(ListT& lst, POSITION pos) :
            m_plst(hboost::addressof(lst)), m_pos(pos)
        { }

    template< class, class, class, class > friend struct list_iterator;
        template< class ListT_, class Value_, class Reference_, class Traversal_>
        list_iterator(list_iterator<ListT_, Value_, Reference_, Traversal_> const& other) :
            m_plst(other.m_plst), m_pos(other.m_pos)
        { }

    private:
        ListT *m_plst;
        POSITION m_pos;

    friend class iterator_core_access;
        ref_t dereference() const
        {
            HBOOST_ASSERT(m_pos != 0 && "out of range");
            return m_plst->GetAt(m_pos);
        }

        // A    B    C    D    x
        // Head           Tail NULL(0)
        //
        void increment()
        {
            HBOOST_ASSERT(m_pos != 0 && "out of range");
            m_plst->GetNext(m_pos);
        }

        void decrement()
        {
            if (m_pos == 0) {
                m_pos = m_plst->GetTailPosition();
                return;
            }

            m_plst->GetPrev(m_pos);
        }

        bool equal(self_t const& other) const
        {
            HBOOST_ASSERT(m_plst == other.m_plst && "iterators incompatible");
            return m_pos == other.m_pos;
        }
    };


    // customization helpers
    //

    struct array_functions
    {
        template< class Iterator, class X >
        Iterator begin(X& x)
        {
            return x.GetData();
        }

        template< class Iterator, class X >
        Iterator end(X& x)
        {
            return begin<Iterator>(x) + x.GetSize();
        }
    };


    struct list_functions
    {
        template< class Iterator, class X >
        Iterator begin(X& x)
        {
            return Iterator(x, x.GetHeadPosition());
        }

        template< class Iterator, class X >
        Iterator end(X& x)
        {
            return Iterator(x, POSITION(0));
        }
    };


} } // namespace hboost::range_detail_microsoft




// test
//


#if defined(HBOOST_RANGE_DETAIL_MICROSOFT_TEST)


#include <algorithm>
#include <iterator>
#include <vector>
#include <hboost/concept_check.hpp>
#include <hboost/next_prior.hpp>
#include <hboost/range/begin.hpp>
#include <hboost/range/concepts.hpp>
#include <hboost/range/const_iterator.hpp>
#include <hboost/range/difference_type.hpp>
#include <hboost/range/distance.hpp>
#include <hboost/range/empty.hpp>
#include <hboost/range/iterator_range.hpp>
#include <hboost/range/mutable_iterator.hpp>
#include <hboost/range/rbegin.hpp>
#include <hboost/range/rend.hpp>
#include <hboost/range/value_type.hpp>
#include <hboost/type_traits/is_same.hpp>


namespace hboost { namespace range_detail_microsoft {


    template< class Range1, class Range2 >
    bool test_equals(Range1 const& rng1, Range2 const& rng2)
    {
        return
            hboost::distance(rng1) == hboost::distance(rng2) &&
            std::equal(hboost::begin(rng1), hboost::end(rng1), hboost::begin(rng2))
        ;
    }


    template< class AssocContainer, class PairT >
    bool test_find_key_and_mapped(AssocContainer const& ac, PairT const& pa)
    {
        typedef typename hboost::range_const_iterator<AssocContainer>::type iter_t;
        for (iter_t it = hboost::const_begin(ac), last = hboost::const_end(ac); it != last; ++it) {
            if (it->first == pa.first && it->second == pa.second)
                return true;
        }

        return false;
    }


    // test functions
    //

    template< class Range >
    bool test_emptiness(Range& )
    {
        bool result = true;

        Range emptyRng;
        result = result && hboost::empty(emptyRng);

        return result;
    }


    template< class Range >
    bool test_trivial(Range& rng)
    {
        bool result = true;

        // convertibility check
        typedef typename range_const_iterator<Range>::type citer_t;
        citer_t cit = hboost::begin(rng);
        (void)cit; // unused

        // mutability check
        typedef typename range_value<Range>::type val_t;
        val_t v = *hboost::begin(rng);
        *hboost::begin(rng) = v;
        result = result && *hboost::begin(rng) == v;

        return result;
    }


    template< class Range >
    bool test_forward(Range& rng)
    {
        hboost::function_requires< ForwardRangeConcept<Range> >();

        bool result = (test_trivial)(rng);

        typedef typename range_value<Range>::type val_t;

        std::vector<val_t> saved;
        std::copy(hboost::begin(rng), hboost::end(rng), std::back_inserter(saved));
        std::rotate(hboost::begin(saved), hboost::next(hboost::begin(saved)), hboost::end(saved));

        std::rotate(hboost::begin(rng), hboost::next(hboost::begin(rng)), hboost::end(rng));

        return result && (test_equals)(saved, rng);
    };


    template< class Range >
    bool test_bidirectional(Range& rng)
    {
        hboost::function_requires< BidirectionalRangeConcept<Range> >();

        bool result = (test_forward)(rng);

        typedef typename range_value<Range>::type val_t;

        std::vector<val_t> saved;
        std::copy(hboost::begin(rng), hboost::end(rng), std::back_inserter(saved));

        result = result && (test_equals)(
            hboost::make_iterator_range(hboost::rbegin(saved), hboost::rend(saved)),
            hboost::make_iterator_range(hboost::rbegin(rng), hboost::rend(rng))
        );

        return result;
    }


    template< class Range >
    bool test_random_access(Range& rng)
    {
        hboost::function_requires< RandomAccessRangeConcept<Range> >();

        bool result = (test_bidirectional)(rng);

        typedef typename range_value<Range>::type val_t;

        std::vector<val_t> saved;
        std::copy(hboost::begin(rng), hboost::end(rng), std::back_inserter(saved));
        std::sort(hboost::begin(saved), hboost::end(saved));

        std::random_shuffle(hboost::begin(rng), hboost::end(rng));
        std::sort(hboost::begin(rng), hboost::end(rng));
        result = result && (test_equals)(rng, saved);

        std::random_shuffle(hboost::begin(rng), hboost::end(rng));
        std::stable_sort(hboost::begin(rng), hboost::end(rng));
        result = result && (test_equals)(rng, saved);

        std::random_shuffle(hboost::begin(rng), hboost::end(rng));
        std::partial_sort(hboost::begin(rng), hboost::end(rng), hboost::end(rng));
        result = result && (test_equals)(rng, saved);

        return result;
    }


    // initializer
    //

    template< class ArrayT, class SampleRange >
    bool test_init_array(ArrayT& arr, SampleRange const& sample)
    {
        typedef typename range_const_iterator<SampleRange>::type iter_t;
        typedef typename range_value<SampleRange>::type val_t;

        for (iter_t it = hboost::const_begin(sample), last = hboost::const_end(sample); it != last; ++it) {
            val_t v = *it; // works around ATL3 CSimpleArray
            arr.Add(v);
        }

        return (test_equals)(arr, sample);
    }


    template< class ListT, class SampleRange >
    bool test_init_list(ListT& lst, SampleRange const& sample)
    {
        typedef typename range_const_iterator<SampleRange>::type iter_t;

        for (iter_t it = hboost::const_begin(sample), last = hboost::const_end(sample); it != last; ++it) {
            lst.AddTail(*it);
        }

        return (test_equals)(lst, sample);
    }


    template< class StringT, class SampleRange >
    bool test_init_string(StringT& str, SampleRange const& sample)
    {
        typedef typename range_const_iterator<SampleRange>::type iter_t;
        typedef typename range_value<SampleRange>::type val_t;

        for (iter_t it = hboost::const_begin(sample), last = hboost::const_end(sample); it != last; ++it) {
            str += *it;
        }

        return (test_equals)(str, sample);
    }


    template< class MapT, class SampleMap >
    bool test_init_map(MapT& map, SampleMap const& sample)
    {
        typedef typename range_const_iterator<SampleMap>::type iter_t;

        for (iter_t it = hboost::const_begin(sample), last = hboost::const_end(sample); it != last; ++it) {
            map.SetAt(it->first, it->second);
        }

        return hboost::distance(map) == hboost::distance(sample);
    }


    // metafunction test
    //

    template< class Range, class Iter >
    struct test_mutable_iter :
        hboost::is_same< typename hboost::HBOOST_RANGE_DETAIL_MICROSOFT_range_mutable_iterator<Range>::type, Iter >
    { };


    template< class Range, class Iter >
    struct test_const_iter :
        hboost::is_same< typename hboost::range_const_iterator<Range>::type, Iter >
    { };


} } // namespace hboost::range_detail_microsoft


#endif // defined(HBOOST_RANGE_DETAIL_MICROSOFT_TEST)



#endif
