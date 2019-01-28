/* Copyright 2003-2015 Joaquin M Lopez Munoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/multi_index for library home page.
 */

#ifndef HBOOST_MULTI_INDEX_DETAIL_ARCHIVE_CONSTRUCTED_HPP
#define HBOOST_MULTI_INDEX_DETAIL_ARCHIVE_CONSTRUCTED_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <hboost/config.hpp> /* keep it first to prevent nasty warns in MSVC */
#include <hboost/detail/no_exceptions_support.hpp>
#include <hboost/noncopyable.hpp>
#include <hboost/serialization/serialization.hpp>
#include <hboost/type_traits/aligned_storage.hpp>
#include <hboost/type_traits/alignment_of.hpp> 

namespace hboost{

namespace multi_index{

namespace detail{

/* constructs a stack-based object from a serialization archive */

template<typename T>
struct archive_constructed:private noncopyable
{
  template<class Archive>
  archive_constructed(Archive& ar,const unsigned int version)
  {
    serialization::load_construct_data_adl(ar,&get(),version);
    HBOOST_TRY{
      ar>>get();
    }
    HBOOST_CATCH(...){
      (&get())->~T();
      HBOOST_RETHROW;
    }
    HBOOST_CATCH_END
  }

  template<class Archive>
  archive_constructed(const char* name,Archive& ar,const unsigned int version)
  {
    serialization::load_construct_data_adl(ar,&get(),version);
    HBOOST_TRY{
      ar>>serialization::make_nvp(name,get());
    }
    HBOOST_CATCH(...){
      (&get())->~T();
      HBOOST_RETHROW;
    }
    HBOOST_CATCH_END
  }

  ~archive_constructed()
  {
    (&get())->~T();
  }

  T& get(){return *reinterpret_cast<T*>(&space);}

private:
  typename aligned_storage<sizeof(T),alignment_of<T>::value>::type space;
};

} /* namespace multi_index::detail */

} /* namespace multi_index */

} /* namespace hboost */

#endif
