// Boost.Range library
//
//  Copyright Thorsten Ottosen, Neil Groves 2006. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//

#ifndef HBOOST_RANGE_ADAPTOR_COPIED_HPP
#define HBOOST_RANGE_ADAPTOR_COPIED_HPP

#include <hboost/range/adaptor/argument_fwd.hpp>
#include <hboost/range/adaptor/sliced.hpp>
#include <hboost/range/size_type.hpp>
#include <hboost/range/iterator_range.hpp>
#include <hboost/range/concepts.hpp>

namespace hboost
{
    namespace adaptors
    {
        struct copied
        {
            copied(std::size_t t_, std::size_t u_)
                : t(t_), u(u_) {}

            std::size_t t;
            std::size_t u;
        };

        template<class CopyableRandomAccessRange>
        inline CopyableRandomAccessRange
        operator|(const CopyableRandomAccessRange& r, const copied& f)
        {
            HBOOST_RANGE_CONCEPT_ASSERT((
                RandomAccessRangeConcept<const CopyableRandomAccessRange>));

            iterator_range<
                HBOOST_DEDUCED_TYPENAME range_iterator<
                    const CopyableRandomAccessRange
                >::type
            > temp(adaptors::slice(r, f.t, f.u));

            return CopyableRandomAccessRange(temp.begin(), temp.end());
        }

        template<class CopyableRandomAccessRange>
        inline CopyableRandomAccessRange
        copy(const CopyableRandomAccessRange& rng, std::size_t t, std::size_t u)
        {
            HBOOST_RANGE_CONCEPT_ASSERT((
                RandomAccessRangeConcept<const CopyableRandomAccessRange>));

            iterator_range<
                HBOOST_DEDUCED_TYPENAME range_iterator<
                    const CopyableRandomAccessRange
                >::type
            > temp(adaptors::slice(rng, t, u));

            return CopyableRandomAccessRange( temp.begin(), temp.end() );
        }
    } // 'adaptors'

}

#endif // include guard
