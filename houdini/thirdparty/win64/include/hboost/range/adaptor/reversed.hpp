// Boost.Range library
//
//  Copyright Thorsten Ottosen, Neil Groves 2006 - 2008. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//

#ifndef HBOOST_RANGE_ADAPTOR_REVERSED_HPP
#define HBOOST_RANGE_ADAPTOR_REVERSED_HPP

#include <hboost/range/iterator_range.hpp>
#include <hboost/range/concepts.hpp>
#include <hboost/iterator/reverse_iterator.hpp>

namespace hboost
{
    namespace range_detail
    {
        template< class R >
        struct reversed_range : 
            public hboost::iterator_range< 
                      hboost::reverse_iterator<
                        HBOOST_DEDUCED_TYPENAME range_iterator<R>::type 
                                              >
                                         >
        {
        private:
            typedef hboost::iterator_range< 
                      hboost::reverse_iterator<
                        HBOOST_DEDUCED_TYPENAME range_iterator<R>::type 
                                              >
                                         >
                base;
            
        public:
            typedef hboost::reverse_iterator<HBOOST_DEDUCED_TYPENAME range_iterator<R>::type> iterator;

            explicit reversed_range( R& r ) 
                : base( iterator(hboost::end(r)), iterator(hboost::begin(r)) )
            { }
        };

        struct reverse_forwarder {};
        
        template< class BidirectionalRange >
        inline reversed_range<BidirectionalRange> 
        operator|( BidirectionalRange& r, reverse_forwarder )
        {
            HBOOST_RANGE_CONCEPT_ASSERT((
                BidirectionalRangeConcept<BidirectionalRange>));

            return reversed_range<BidirectionalRange>( r );
        }

        template< class BidirectionalRange >
        inline reversed_range<const BidirectionalRange> 
        operator|( const BidirectionalRange& r, reverse_forwarder )
        {
            HBOOST_RANGE_CONCEPT_ASSERT((
                BidirectionalRangeConcept<const BidirectionalRange>));

            return reversed_range<const BidirectionalRange>( r ); 
        }
        
    } // 'range_detail'
    
    using range_detail::reversed_range;

    namespace adaptors
    { 
        namespace
        {
            const range_detail::reverse_forwarder reversed = 
                                            range_detail::reverse_forwarder();
        }
        
        template<class BidirectionalRange>
        inline reversed_range<BidirectionalRange>
        reverse(BidirectionalRange& rng)
        {
            HBOOST_RANGE_CONCEPT_ASSERT((
                BidirectionalRangeConcept<BidirectionalRange>));

            return reversed_range<BidirectionalRange>(rng);
        }
        
        template<class BidirectionalRange>
        inline reversed_range<const BidirectionalRange>
        reverse(const BidirectionalRange& rng)
        {
            HBOOST_RANGE_CONCEPT_ASSERT((
                BidirectionalRangeConcept<const BidirectionalRange>));

            return reversed_range<const BidirectionalRange>(rng);
        }
    } // 'adaptors'
    
} // 'boost'

#endif
