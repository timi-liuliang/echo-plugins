//  hboost/detail/lightweight_test_reporter.hpp  ----------------------------------------//

//  Copyright Beman Dawes 2014

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//--------------------------------------------------------------------------------------//
//                                                                                      //
//                         Configuration reporting cpp_main()                           //
//                                                                                      //
//  Displays configuration information, then returns test_main(argc, argv), which       //
//  must be supplied by the user.                                                       //
//                                                                                      //
//  Note: cpp_main(argc, argv) is called from a try block in main(), which is           //
//  supplied by <hboost/detail/lightweight_main.hpp> as is a catch block that reports    //
//  std::exception what().                                                              //
//                                                                                      //
//--------------------------------------------------------------------------------------//

#include <hboost/config.hpp>
#include <hboost/version.hpp>
#include <hboost/detail/lightweight_test.hpp>
#include <hboost/detail/lightweight_main.hpp>
#include <iostream>

int test_main(int argc, char* argv[]);

int cpp_main(int argc, char* argv[])
{
  std::cout << HBOOST_COMPILER
#ifdef __GNUC__
            << ", __GXX_EXPERIMENTAL_CXX0X__ "
# ifdef __GXX_EXPERIMENTAL_CXX0X__
              "defined"
# else
              "not defined"
# endif
#endif
            << "\n"
            << HBOOST_STDLIB << "\n"
            << HBOOST_PLATFORM << "\n"
            << "Boost version " << HBOOST_VERSION / 100000 << '.'
            << HBOOST_VERSION / 100 % 1000 << '.' << HBOOST_VERSION % 100 << "\n";

  std::cout << "Command line: ";
  for (int a = 0; a < argc; ++a)
  {
    std::cout << argv[a];
    if (a != argc - 1)
      std::cout << ' ';
  }
  std::cout << std::endl;

  return test_main(argc, argv);
}