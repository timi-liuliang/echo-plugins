///////////////////////////////////////////////////////////////////////////////
// reference_accumulator.hpp
//
//  Copyright 2005 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_ACCUMULATORS_FRAMEWORK_ACCUMULATORS_REFERENCE_ACCUMULATOR_HPP_EAN_03_23_2006
#define HBOOST_ACCUMULATORS_FRAMEWORK_ACCUMULATORS_REFERENCE_ACCUMULATOR_HPP_EAN_03_23_2006

#include <hboost/ref.hpp>
#include <hboost/mpl/always.hpp>
#include <hboost/parameter/keyword.hpp>
#include <hboost/accumulators/framework/depends_on.hpp> // for feature_tag
#include <hboost/accumulators/framework/accumulator_base.hpp>
#include <hboost/accumulators/framework/extractor.hpp>

namespace hboost { namespace accumulators
{

namespace impl
{
    //////////////////////////////////////////////////////////////////////////
    // reference_accumulator_impl
    //
    template<typename Referent, typename Tag>
    struct reference_accumulator_impl
      : accumulator_base
    {
        typedef Referent &result_type;

        template<typename Args>
        reference_accumulator_impl(Args const &args)
          : ref(args[parameter::keyword<Tag>::get()])
        {
        }

        result_type result(dont_care) const
        {
            return this->ref;
        }

    private:
        reference_wrapper<Referent> ref;
    };
} // namespace impl

namespace tag
{
    //////////////////////////////////////////////////////////////////////////
    // reference_tag
    template<typename Tag>
    struct reference_tag
    {
    };

    //////////////////////////////////////////////////////////////////////////
    // reference
    template<typename Referent, typename Tag>
    struct reference
      : depends_on<>
    {
        /// INTERNAL ONLY
        ///
        typedef mpl::always<accumulators::impl::reference_accumulator_impl<Referent, Tag> > impl;
    };
}

namespace extract
{
    HBOOST_ACCUMULATORS_DEFINE_EXTRACTOR(tag, reference, (typename)(typename))
    HBOOST_ACCUMULATORS_DEFINE_EXTRACTOR(tag, reference_tag, (typename))
}

using extract::reference;
using extract::reference_tag;

// Map all reference<V,T> features to reference_tag<T> so
// that references can be extracted using reference_tag<T>
// without specifying the referent type.
template<typename ValueType, typename Tag>
struct feature_of<tag::reference<ValueType, Tag> >
  : feature_of<tag::reference_tag<Tag> >
{
};

}} // namespace hboost::accumulators

#endif
