// Boost.Range library
//
//  Copyright Thorsten Ottosen, Neil Groves 2006 - 2008. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//

#ifndef HBOOST_RANGE_ADAPTOR_SLICED_HPP
#define HBOOST_RANGE_ADAPTOR_SLICED_HPP

#include <hboost/range/adaptor/argument_fwd.hpp>
#include <hboost/range/size_type.hpp>
#include <hboost/range/iterator_range.hpp>
#include <hboost/range/concepts.hpp>

namespace hboost
{
    namespace adaptors
    {
        struct sliced
        {
            sliced(std::size_t t_, std::size_t u_)
                : t(t_), u(u_) {}
            std::size_t t;
            std::size_t u;
        };

        template< class RandomAccessRange >
        class sliced_range : public hboost::iterator_range< HBOOST_DEDUCED_TYPENAME range_iterator<RandomAccessRange>::type >
        {
            typedef hboost::iterator_range< HBOOST_DEDUCED_TYPENAME range_iterator<RandomAccessRange>::type > base_t;
        public:
            template<typename Rng, typename T, typename U>
            sliced_range(Rng& rng, T t, U u)
                : base_t(hboost::next(hboost::begin(rng), t),
                         hboost::next(hboost::begin(rng), u))
            {
            }
        };

        template< class RandomAccessRange >
        inline sliced_range<RandomAccessRange>
        slice( RandomAccessRange& rng, std::size_t t, std::size_t u )
        {
            HBOOST_RANGE_CONCEPT_ASSERT((
                RandomAccessRangeConcept<RandomAccessRange>));

            HBOOST_ASSERT( t <= u && "error in slice indices" );
            HBOOST_ASSERT( static_cast<std::size_t>(hboost::size(rng)) >= u &&
                          "second slice index out of bounds" );

            return sliced_range<RandomAccessRange>(rng, t, u);
        }

        template< class RandomAccessRange >
        inline iterator_range< HBOOST_DEDUCED_TYPENAME range_iterator<const RandomAccessRange>::type >
        slice( const RandomAccessRange& rng, std::size_t t, std::size_t u )
        {
            HBOOST_RANGE_CONCEPT_ASSERT((
                RandomAccessRangeConcept<const RandomAccessRange>));

            HBOOST_ASSERT( t <= u && "error in slice indices" );
            HBOOST_ASSERT( static_cast<std::size_t>(hboost::size(rng)) >= u &&
                          "second slice index out of bounds" );

            return sliced_range<const RandomAccessRange>(rng, t, u);
        }

        template< class RandomAccessRange >
        inline sliced_range<RandomAccessRange>
        operator|( RandomAccessRange& r, const sliced& f )
        {
            HBOOST_RANGE_CONCEPT_ASSERT((
                RandomAccessRangeConcept<RandomAccessRange>));

            return sliced_range<RandomAccessRange>( r, f.t, f.u );
        }

        template< class RandomAccessRange >
        inline sliced_range<const RandomAccessRange>
        operator|( const RandomAccessRange& r, const sliced& f )
        {
            HBOOST_RANGE_CONCEPT_ASSERT((
                RandomAccessRangeConcept<const RandomAccessRange>));

            return sliced_range<const RandomAccessRange>( r, f.t, f.u );
        }

    } // namespace adaptors
    using adaptors::sliced_range;
} // namespace hboost

#endif
