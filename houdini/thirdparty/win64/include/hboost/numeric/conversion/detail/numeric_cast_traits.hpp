//
//! Copyright (c) 2011-2012
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#if !defined(HBOOST_NUMERIC_CONVERSION_DONT_USE_PREPROCESSED_FILES)

    #include <hboost/numeric/conversion/detail/preprocessed/numeric_cast_traits_common.hpp>
	
	#if !defined(HBOOST_NO_LONG_LONG)
        #include <hboost/numeric/conversion/detail/preprocessed/numeric_cast_traits_long_long.hpp>
	#endif
	
#else
#if !HBOOST_PP_IS_ITERATING

    #include <hboost/preprocessor/seq/elem.hpp>
    #include <hboost/preprocessor/seq/size.hpp>
    #include <hboost/preprocessor/iteration/iterate.hpp>
    
    #if defined(__WAVE__) && defined(HBOOST_NUMERIC_CONVERSION_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 2, line: 0, output: "preprocessed/numeric_cast_traits_common.hpp")
    #endif
//
//! Copyright (c) 2011-2012
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
    #if defined(__WAVE__) && defined(HBOOST_NUMERIC_CONVERSION_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 1)
    #endif
	
	//! These are the assumed common built in fundamental types (not typedefs/macros.)
	#define HBOOST_NUMERIC_CONVERSION_BASE_BUILTIN_TYPES() \
        (char)                                            \
        (signed char)                                     \
        (unsigned char)                                   \
        (short)                                           \
        (unsigned short)                                  \
        (int)                                             \
        (unsigned int)                                    \
        (long)                                            \
        (unsigned long)                                   \
        (float)                                           \
        (double)                                          \
        (long double)                                     \
    /***/
	
    #define HBOOST_NUMERIC_CONVERSION_SEQ_A() HBOOST_NUMERIC_CONVERSION_BASE_BUILTIN_TYPES()
	#define HBOOST_NUMERIC_CONVERSION_SEQ_B() HBOOST_NUMERIC_CONVERSION_BASE_BUILTIN_TYPES()

namespace hboost { namespace numeric {

    #define HBOOST_PP_ITERATION_PARAMS_1 (3, (0, HBOOST_PP_DEC(HBOOST_PP_SEQ_SIZE(HBOOST_NUMERIC_CONVERSION_SEQ_A())), <hboost/numeric/conversion/detail/numeric_cast_traits.hpp>))
    #include HBOOST_PP_ITERATE()    

}}//namespace hboost::numeric;

    #if defined(__WAVE__) && defined(HBOOST_NUMERIC_CONVERSION_CREATE_PREPROCESSED_FILES)
        #pragma wave option(output: null)
    #endif   
	
	#if ( defined(__WAVE__) && defined(HBOOST_NUMERIC_CONVERSION_CREATE_PREPROCESSED_FILES) ) || !defined(HBOOST_NO_LONG_LONG)
	
	    #undef HBOOST_NUMERIC_CONVERSION_SEQ_A
	    #undef HBOOST_NUMERIC_CONVERSION_SEQ_B

	    #if defined(__WAVE__) && defined(HBOOST_NUMERIC_CONVERSION_CREATE_PREPROCESSED_FILES)
            #pragma wave option(preserve: 2, line: 0, output: "preprocessed/numeric_cast_traits_long_long.hpp")
        #endif

//
//! Copyright (c) 2011-2012
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
        #if defined(__WAVE__) && defined(HBOOST_NUMERIC_CONVERSION_CREATE_PREPROCESSED_FILES)
            #pragma wave option(preserve: 1)
        #endif

namespace hboost { namespace numeric {

    #define HBOOST_NUMERIC_CONVERSION_SEQ_A() HBOOST_NUMERIC_CONVERSION_BASE_BUILTIN_TYPES()(hboost::long_long_type)(hboost::ulong_long_type)
	#define HBOOST_NUMERIC_CONVERSION_SEQ_B() (hboost::long_long_type)(hboost::ulong_long_type)
    
    #define HBOOST_PP_ITERATION_PARAMS_1 (3, (0, HBOOST_PP_DEC(HBOOST_PP_SEQ_SIZE(HBOOST_NUMERIC_CONVERSION_SEQ_A())), <hboost/numeric/conversion/detail/numeric_cast_traits.hpp>))
    #include HBOOST_PP_ITERATE()    

}}//namespace hboost::numeric;

        #if defined(__WAVE__) && defined(HBOOST_NUMERIC_CONVERSION_CREATE_PREPROCESSED_FILES)
            #pragma wave option(output: null)
        #endif   
	
	#endif
		
    #undef HBOOST_NUMERIC_CONVERSION_BASE_BUILTIN_TYPES
	#undef HBOOST_NUMERIC_CONVERSION_SEQ_A
	#undef HBOOST_NUMERIC_CONVERSION_SEQ_B
    
#elif HBOOST_PP_ITERATION_DEPTH() == 1

    #define HBOOST_PP_ITERATION_PARAMS_2 (3, (0, HBOOST_PP_DEC(HBOOST_PP_SEQ_SIZE(HBOOST_NUMERIC_CONVERSION_SEQ_B())), <hboost/numeric/conversion/detail/numeric_cast_traits.hpp>))
    #include HBOOST_PP_ITERATE()

#elif HBOOST_PP_ITERATION_DEPTH() == 2

    //! Generate default traits for the specified source and target.
    #define HBOOST_NUMERIC_CONVERSION_A HBOOST_PP_FRAME_ITERATION(1)
    #define HBOOST_NUMERIC_CONVERSION_B HBOOST_PP_FRAME_ITERATION(2)

    template <>
    struct numeric_cast_traits
        <
            HBOOST_PP_SEQ_ELEM(HBOOST_NUMERIC_CONVERSION_A, HBOOST_NUMERIC_CONVERSION_SEQ_A())
          , HBOOST_PP_SEQ_ELEM(HBOOST_NUMERIC_CONVERSION_B, HBOOST_NUMERIC_CONVERSION_SEQ_B())
        >
    {
        typedef def_overflow_handler overflow_policy;
        typedef UseInternalRangeChecker range_checking_policy;
        typedef Trunc<HBOOST_PP_SEQ_ELEM(HBOOST_NUMERIC_CONVERSION_B, HBOOST_NUMERIC_CONVERSION_SEQ_B())> rounding_policy;
    };     

    #undef HBOOST_NUMERIC_CONVERSION_A
    #undef HBOOST_NUMERIC_CONVERSION_B

#endif//! Depth 2.
#endif// HBOOST_NUMERIC_CONVERSION_DONT_USE_PREPROCESSED_FILES
