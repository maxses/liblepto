/**---------------------------------------------------------------------------
 *
 * @file       test_crc8.cpp
 * @brief      Test crc8 encoding
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

#include <lepto/crc8.h>
#include <string.h>        // strlen

#define TEST_ALL
#define STOP_ON_FAIL


/*--- Implementation -------------------------------------------------------*/


TEST_CASE( "CRC8", "[default]" )
{
   SECTION( "CRC8" )
   {
      // CRC-8/NRSC-5
      const char *data="Lorem ipsum dolor sit amet, consectetuer adipiscing elit.";
      const char *data1="Lorem ipsum dolor sit amet, ";
      const char *data2="consectetuer adipiscing elit.";

      // There are no accumulating functions like in crc32.
      {
         uint8_t crc=crc8Init();
         crc = crc8Calc( crc, data, strlen(data) );
         
         REQUIRE ( (unsigned int)crc ==  0x85 );
      }

      {
         uint8_t crc=crc8Init();
         crc = crc8Calc( crc, data1, strlen(data1) );
         crc = crc8Calc( crc, data2, strlen(data2) );

         REQUIRE ( (unsigned int)crc ==  0x85 );
      }
      
   }
}


/*--- Fin ------------------------------------------------------------------*/
