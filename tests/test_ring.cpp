/**---------------------------------------------------------------------------
 *
 * @file       test_ring.cpp
 * @brief      Test CRing
 *
 *             All tests are done in test_list.cpp
 *
 * @date       20220619
 * @author     Maximilian Seesslen <mes@seesslen.net>
 * @copyright  SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#if defined ( CATCH_V3 )
   #include <catch2/catch_test_macros.hpp>
#elif defined ( CATCH_V2 )
   #include <catch2/catch.hpp>
#elif defined ( CATCH_V1 )
   #include <catch/catch.hpp>
#else
   #error "Either 'catch' or 'catch2' has to be installed"
#endif

#include <lepto/ring.hpp>

//--- Own ----------------------------

#define TEST_ALL
#define STOP_ON_FAIL

//---Implementation------------------------------------------------------------

#define SPARE_ENTRIES     0

TEST_CASE( "Ring", "[default]" )
{

}


/*--- Fin ------------------------------------------------------------------*/
