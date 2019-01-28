/*=============================================================================
    Copyright (c) 2007 Tobias Schwinger
  
    Use modification and distribution are subject to the Boost Software 
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
==============================================================================*/

#ifndef HBOOST_FUNCTIONAL_FACTORY_HPP_INCLUDED
#   ifndef HBOOST_PP_IS_ITERATING

#     include <hboost/preprocessor/iteration/iterate.hpp>
#     include <hboost/preprocessor/repetition/enum_params.hpp>
#     include <hboost/preprocessor/repetition/enum_binary_params.hpp>

#     include <new>
#     include <hboost/pointee.hpp>
#     include <hboost/get_pointer.hpp>
#     include <hboost/non_type.hpp>
#     include <hboost/type_traits/remove_cv.hpp>

#     if defined(HBOOST_FUNCTIONAL_FACTORY_SUPPORT_NONE_T)
#       include <hboost/none_t.hpp>
#     endif

#     ifndef HBOOST_FUNCTIONAL_FACTORY_MAX_ARITY
#       define HBOOST_FUNCTIONAL_FACTORY_MAX_ARITY 10
#     elif HBOOST_FUNCTIONAL_FACTORY_MAX_ARITY < 3
#       undef  HBOOST_FUNCTIONAL_FACTORY_MAX_ARITY
#       define HBOOST_FUNCTIONAL_FACTORY_MAX_ARITY 3
#     endif

namespace hboost
{
    enum factory_alloc_propagation
    {
        factory_alloc_for_pointee_and_deleter,
        factory_passes_alloc_to_smart_pointer
    };

#if defined(HBOOST_FUNCTIONAL_FACTORY_SUPPORT_NONE_T)
    template< typename Pointer, class Allocator = hboost::none_t,
        factory_alloc_propagation AP = factory_alloc_for_pointee_and_deleter >
    class factory;
#else
    template< typename Pointer, class Allocator = void,
        factory_alloc_propagation AP = factory_alloc_for_pointee_and_deleter >
    class factory;
#endif

    //----- ---- --- -- - -  -   -

    template< typename Pointer, factory_alloc_propagation AP >
    class factory<Pointer, void, AP>
    {
      public:
        typedef typename hboost::remove_cv<Pointer>::type result_type;
        typedef typename hboost::pointee<result_type>::type value_type;

        factory()
        { }

#     define HBOOST_PP_FILENAME_1 <hboost/functional/factory.hpp>
#     define HBOOST_PP_ITERATION_LIMITS (0,HBOOST_FUNCTIONAL_FACTORY_MAX_ARITY)
#     include HBOOST_PP_ITERATE()
    };

#if defined(HBOOST_FUNCTIONAL_FACTORY_SUPPORT_NONE_T)
    template< typename Pointer, factory_alloc_propagation AP >
    class factory<Pointer, hboost::none_t, AP>
        : public factory<Pointer, void, AP>
    {};
#endif

    template< class Pointer, class Allocator, factory_alloc_propagation AP >
    class factory
        : private Allocator::template rebind< typename hboost::pointee<
            typename hboost::remove_cv<Pointer>::type >::type >::other
    {
      public:
        typedef typename hboost::remove_cv<Pointer>::type result_type;
        typedef typename hboost::pointee<result_type>::type value_type;

        typedef typename Allocator::template rebind<value_type>::other
            allocator_type;

        explicit factory(allocator_type const & a = allocator_type())
          : allocator_type(a)
        { }

      private:

        struct deleter
            : allocator_type
        {
            inline deleter(allocator_type const& that) 
              : allocator_type(that)
            { }

            allocator_type& get_allocator() const
            {
                return *const_cast<allocator_type*>(
                    static_cast<allocator_type const*>(this));
            }

            void operator()(value_type* ptr) const
            {
                if (!! ptr) ptr->~value_type();
                const_cast<allocator_type*>(static_cast<allocator_type const*>(
                    this))->deallocate(ptr,1);
            }
        };

        inline allocator_type& get_allocator() const
        {
            return *const_cast<allocator_type*>(
                static_cast<allocator_type const*>(this));
        }

        inline result_type make_pointer(value_type* ptr, hboost::non_type<
            factory_alloc_propagation,factory_passes_alloc_to_smart_pointer>)
        const
        {
            return result_type(ptr,deleter(this->get_allocator()));
        }
        inline result_type make_pointer(value_type* ptr, hboost::non_type<
            factory_alloc_propagation,factory_alloc_for_pointee_and_deleter>)
        const
        {
            return result_type(ptr,deleter(this->get_allocator()),
                this->get_allocator());
        }

      public:

#     define HBOOST_TMP_MACRO
#     define HBOOST_PP_FILENAME_1 <hboost/functional/factory.hpp>
#     define HBOOST_PP_ITERATION_LIMITS (0,HBOOST_FUNCTIONAL_FACTORY_MAX_ARITY)
#     include HBOOST_PP_ITERATE()
#     undef HBOOST_TMP_MACRO
    };

    template< typename Pointer, class Allocator, factory_alloc_propagation AP > 
    class factory<Pointer&, Allocator, AP>;
    // forbidden, would create a dangling reference
}

#     define HBOOST_FUNCTIONAL_FACTORY_HPP_INCLUDED
#   else // defined(HBOOST_PP_IS_ITERATING)
#     define N HBOOST_PP_ITERATION()
#     if !defined(HBOOST_TMP_MACRO)
#       if N > 0
    template< HBOOST_PP_ENUM_PARAMS(N, typename T) >
#       endif
    inline result_type operator()(HBOOST_PP_ENUM_BINARY_PARAMS(N,T,& a)) const
    {
        return result_type( new value_type(HBOOST_PP_ENUM_PARAMS(N,a)) );
    }
#     else // defined(HBOOST_TMP_MACRO)
#       if N > 0
    template< HBOOST_PP_ENUM_PARAMS(N, typename T) >
#       endif
    inline result_type operator()(HBOOST_PP_ENUM_BINARY_PARAMS(N,T,& a)) const
    {
        value_type* memory = this->get_allocator().allocate(1);
        try
        { 
            return make_pointer(
                new(memory) value_type(HBOOST_PP_ENUM_PARAMS(N,a)),
                hboost::non_type<factory_alloc_propagation,AP>() );
        }
        catch (...) { this->get_allocator().deallocate(memory,1); throw; }
    }
#     endif
#     undef N
#   endif // defined(HBOOST_PP_IS_ITERATING)

#endif // include guard

