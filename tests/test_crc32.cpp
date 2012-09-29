/**---------------------------------------------------------------------------
 *
 * @file       test_crc32.cpp
 * @brief      Test crc32 encoding
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

#include <lepto/crc32.h>
#include <string.h>        // strlen


#define TEST_ALL
#define STOP_ON_FAIL


/*--- Implementation -------------------------------------------------------*/


TEST_CASE( "CRC32", "[default]" )
{
   SECTION( "CRC32" )
   {
      const char *data="Hello World!";
      const char *data1="Hello ";
      const char *data2="World!";
      
      {
         crc32_t crc;
         crc=crc32Init();
         crc=crc32Update(crc, data1, strlen(data1));
         crc=crc32Update(crc, data2, strlen(data2));
         crc=crc32Finalize(crc);
         
         REQUIRE ( crc == 0x1C291CA3 );
      }
      
      {
         crc32_t crc;
         crc=crc32MpegInit();
         crc=crc32MpegUpdate(crc, data1, strlen(data1));
         crc=crc32MpegUpdate(crc, data2, strlen(data2));
         crc=crc32MpegFinalize(crc);
         
         REQUIRE ( crc == 0x94E58351 );
      }
      
      {
         crc32_t crc=0;
         crc = calcCrc32( data, strlen(data) );
         
         REQUIRE ( crc ==  0x1C291CA3 );
      }
      
   }
}


/*--- Fin ------------------------------------------------------------------*/
