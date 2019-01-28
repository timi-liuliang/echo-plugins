// (C) Copyright 2005 Matthias Troyer 

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Matthias Troyer

#ifndef HBOOST_MPI_TEXT_SKELETON_OARCHIVE_HPP
#define HBOOST_MPI_TEXT_SKELETON_OARCHIVE_HPP

#include <hboost/archive/detail/auto_link_archive.hpp>
#include <hboost/archive/text_oarchive.hpp>
#include <hboost/mpi/detail/forward_skeleton_oarchive.hpp>
#include <hboost/mpi/detail/ignore_oprimitive.hpp>
#include <hboost/archive/detail/register_archive.hpp>

namespace hboost { namespace mpi {

// an archive that writes a text skeleton into a stream

class text_skeleton_oarchive 
  : public detail::ignore_oprimitive,
    public detail::forward_skeleton_oarchive<text_skeleton_oarchive,hboost::archive::text_oarchive>
{
public:
    text_skeleton_oarchive(std::ostream & s, unsigned int flags = 0) 
     : detail::forward_skeleton_oarchive<text_skeleton_oarchive,hboost::archive::text_oarchive>(skeleton_archive_)
     , skeleton_archive_(s,flags)
    {}

private:
    hboost::archive::text_oarchive skeleton_archive_;
};

namespace detail {

typedef hboost::mpi::detail::forward_skeleton_oarchive<hboost::mpi::text_skeleton_oarchive,hboost::archive::text_oarchive> type3;

}


} } // end namespace hboost::mpi

// required by export
HBOOST_SERIALIZATION_REGISTER_ARCHIVE(hboost::mpi::text_skeleton_oarchive)
HBOOST_SERIALIZATION_REGISTER_ARCHIVE(hboost::mpi::detail::type3)


#endif // HBOOST_MPI_TEXT_SKELETON_OARCHIVE_HPP
