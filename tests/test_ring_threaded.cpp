/**---------------------------------------------------------------------------
 *
 * @file       test_ring_threaded.cpp
 * @brief      test thread safety for CRing::tryReserve()
 *
 * @date       20250801
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
#include <test_ring_threaded.hpp>
#include <QCoreApplication>


/*--- Implementation -------------------------------------------------------*/


TEST_CASE( "Ring threaded", "[default]" )
{
   SECTION( "Ring threaded" )
   {
      for(int i1=0; i1 < 10; i1++)
      {
         int argc=1;
         char* argv[]{ 0 };
         QCoreApplication app( argc, argv );
         CConsumer consumer;

         consumer.start();
         consumer.wait();

         //printf("Loops: %d\n", consumer.getLoops() );
         //printf("Errors: %d\n", consumer.getErrors() );
         REQUIRE( consumer.getErrors() == 0 );
      }
   }
}


/*--- Fin ------------------------------------------------------------------*/
