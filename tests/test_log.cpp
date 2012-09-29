/**---------------------------------------------------------------------------
 *
 * @file       main_log_test.cpp
 * @brief      Test logging
 *
 * Not much to test at the moment. Lepto only prints the logs.
 * TBD: heartbeat
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

#include <lepto/log.h>


/*--- Implementation -------------------------------------------------------*/


#define EV_BOF    0x100


TEST_CASE( "Log", "[default]" )
{
   SECTION( "Output" )
   {
      lWarning( "BOF: %d", 10 );
      lLog( ELogCategory::Warning | EV_BOF, "E-BOF: %d", 10 );
   }
}


/*--- Fin ------------------------------------------------------------------*/
