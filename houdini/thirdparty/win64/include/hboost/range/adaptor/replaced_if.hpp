// Boost.Range library
//
//  Copyright Neil Groves 2007. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//

#ifndef HBOOST_RANGE_ADAPTOR_REPLACED_IF_IMPL_HPP_INCLUDED
#define HBOOST_RANGE_ADAPTOR_REPLACED_IF_IMPL_HPP_INCLUDED

#include <hboost/config.hpp>
#include <hboost/range/adaptor/argument_fwd.hpp>
#include <hboost/range/iterator_range.hpp>
#include <hboost/range/begin.hpp>
#include <hboost/range/end.hpp>
#include <hboost/range/value_type.hpp>
#include <hboost/range/concepts.hpp>
#include <hboost/iterator/iterator_adaptor.hpp>
#include <hboost/iterator/transform_iterator.hpp>
#include <hboost/optional/optional.hpp>

namespace hboost
{
    namespace range_detail
    {
        template< class Pred, class Value >
        class replace_value_if
        {
        public:
            typedef const Value& result_type;
            typedef const Value& first_argument_type;

            // Rationale:
            // required to allow the iterator to be default constructible.
            replace_value_if()
            {
            }

            replace_value_if(const Pred& pred, const Value& to)
                : m_impl(data(pred, to))
            {
            }

            const Value& operator()(const Value& x) const
            {
                return m_impl->m_pred(x) ? m_impl->m_to : x;
            }

        private:
            struct data
            {
                data(const Pred& p, const Value& t)
                    : m_pred(p), m_to(t)
                {
                }

                Pred  m_pred;
                Value m_to;
            };
            hboost::optional<data> m_impl;
        };

        template< class Pred, class R >
        class replaced_if_range :
            public hboost::iterator_range<
                hboost::transform_iterator<
                    replace_value_if< Pred, HBOOST_DEDUCED_TYPENAME range_value<R>::type >,
                    HBOOST_DEDUCED_TYPENAME range_iterator<R>::type > >
        {
        private:
            typedef replace_value_if< Pred, HBOOST_DEDUCED_TYPENAME range_value<R>::type > Fn;

            typedef hboost::iterator_range<
                hboost::transform_iterator<
                    replace_value_if< Pred, HBOOST_DEDUCED_TYPENAME range_value<R>::type >,
                    HBOOST_DEDUCED_TYPENAME range_iterator<R>::type > > base_t;

        public:
            typedef HBOOST_DEDUCED_TYPENAME range_value<R>::type value_type;

            replaced_if_range( R& r, const Pred& pred, value_type to )
                : base_t( make_transform_iterator( hboost::begin(r), Fn(pred, to) ),
                          make_transform_iterator( hboost::end(r), Fn(pred, to) ) )
            { }
        };

        template< class Pred, class T >
        class replace_if_holder
        {
        public:
            replace_if_holder( const Pred& pred, const T& to )
                : m_pred(pred), m_to(to)
            { }

            const Pred& pred() const { return m_pred; }
            const T& to() const { return m_to; }

        private:
            Pred m_pred;
            T m_to;
        };

        template< class Pred, class SinglePassRange >
        inline replaced_if_range<Pred, SinglePassRange>
        operator|(
            SinglePassRange& r,
            const replace_if_holder<
                Pred,
                HBOOST_DEDUCED_TYPENAME range_value<SinglePassRange>::type>& f)
        {
            HBOOST_RANGE_CONCEPT_ASSERT((
                SinglePassRangeConcept<SinglePassRange>));

            return replaced_if_range<Pred, SinglePassRange>(
                r, f.pred(), f.to());
        }

        template< class Pred, class SinglePassRange >
        inline replaced_if_range<Pred, const SinglePassRange>
        operator|(
            const SinglePassRange& r,
            const replace_if_holder<
                Pred,
                HBOOST_DEDUCED_TYPENAME range_value<SinglePassRange>::type>& f)
        {
            HBOOST_RANGE_CONCEPT_ASSERT((
                SinglePassRangeConcept<const SinglePassRange>));

            return replaced_if_range<Pred, const SinglePassRange>(
                r, f.pred(), f.to());
        }
    } // 'range_detail'

    using range_detail::replaced_if_range;

    namespace adaptors
    {
        namespace
        {
            const range_detail::forwarder2TU<range_detail::replace_if_holder>
                replaced_if =
                    range_detail::forwarder2TU<range_detail::replace_if_holder>();
        }

        template<class Pred, class SinglePassRange>
        inline replaced_if_range<Pred, SinglePassRange>
        replace_if(SinglePassRange& rng, Pred pred,
                   HBOOST_DEDUCED_TYPENAME range_value<SinglePassRange>::type to)
        {
            HBOOST_RANGE_CONCEPT_ASSERT((
                SinglePassRangeConcept<SinglePassRange>));

            return range_detail::replaced_if_range<Pred, SinglePassRange>(
                rng, pred, to);
        }

        template<class Pred, class SinglePassRange>
        inline replaced_if_range<Pred, const SinglePassRange>
        replace_if(
            const SinglePassRange& rng,
            Pred pred,
            HBOOST_DEDUCED_TYPENAME range_value<const SinglePassRange>::type to)
        {
            HBOOST_RANGE_CONCEPT_ASSERT((
                SinglePassRangeConcept<const SinglePassRange>));

            return range_detail::replaced_if_range<Pred, const SinglePassRange>(
                rng, pred, to);
        }
    } // 'adaptors'

} // 'boost'

#endif // include guard
