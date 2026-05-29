/**---------------------------------------------------------------------------
 *
 * @file       test_base64.cpp
 * @brief      Test base64 encoding and decoding
 *
 * @date       20220619
 * @author     Maximilian Seesslen <src@seesslen.net>
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

#include <lepto/base64.h>
#include <lepto/string.hpp>        // strlen

#define TEST_ALL
#define STOP_ON_FAIL


/*--- Implementation -------------------------------------------------------*/


TEST_CASE( "Base64", "[default]" )
{
   const char *data="Hello world! Lori ipsum um die Welt";
   const char *encoded="SGVsbG8gd29ybGQhIExvcmkgaXBzdW0gdW0gZGllIFdlbHQ=";

   SECTION( "CByteArray" )
   {

      {
         CBase64 b64;
         CString encoded;
         CByteArray ba;
         encoded.allocate(1);

         b64.encode( CByteArray( data ), encoded );
         printf("Encoded: %s\n", encoded.data());
         REQUIRE ( encoded == "SGVsbG8gd29ybGQhIExvcmkgaXBzdW0gdW0gZGllIFdlbHQ=" );

         b64.decode( encoded, ba);
         REQUIRE ( memcmp( ba.data(), data, ba.length()) == 0 );
      }
   }

   SECTION( "Fragmented" )
   {
      char dest[100];
      {
         CBase64 b64;
         const char* src=encoded;
         char* dst=dest;
         int destlength=0;
         while( strlen( src ) )
         {
            destlength+=b64.decode( src, MIN(4, strlen(src)), (uint8_t*)dst, 3 );
            src+=4;
            dst+=3;
         }
         REQUIRE ( memcmp( dest, data, strlen(data) + 1 ) == 0 );
         REQUIRE ( destlength == (int)strlen(data) );
      }
   }

   SECTION( "Detect size" )
   {
      CBase64 b64;
      char dest1[100];
      char dest2[100];
      {
         // Zero-length data
         b64.encode( (const uint8_t*)"", 0, (char*)dest1, 100 );
         REQUIRE ( strlen(dest1) == 0 );
         REQUIRE ( b64.decode( dest1, strlen(dest1), (uint8_t*)dest2, 100 ) == 0 );

         // One byte data
         b64.encode( (const uint8_t*)"A", 1, (char*)dest1, 100 );
         REQUIRE ( strlen(dest1) == 4 );
         REQUIRE ( b64.decode( dest1, strlen(dest1), (uint8_t*)dest2, 100 ) == 1 );

         // Two bytes data
         b64.encode( (const uint8_t*)"AB", 2, (char*)dest1, 100 );
         REQUIRE ( strlen(dest1) == 4 );
         REQUIRE ( b64.decode( dest1, strlen(dest1), (uint8_t*)dest2, 100 ) == 2 );

         // Three bytes data
         b64.encode( (const uint8_t*)"ABC", 3, (char*)dest1, 100 );
         REQUIRE ( strlen(dest1) == 4 );
         REQUIRE ( b64.decode( dest1, strlen(dest1), (uint8_t*)dest2, 100 ) == 3 );

         // For bytes data
         b64.encode( (const uint8_t*)"ABCD", 4, (char*)dest1, 100 );
         REQUIRE ( strlen(dest1) == 8 );
         REQUIRE ( b64.decode( dest1, strlen(dest1), (uint8_t*)dest2, 100 ) == 4 );
      }
   }
}


/*--- Fin ------------------------------------------------------------------*/
