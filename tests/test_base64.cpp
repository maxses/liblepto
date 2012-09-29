/**---------------------------------------------------------------------------
 *
 * @file       test_base64.cpp
 * @brief      Test base64 encoding and decoding
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

#include <lepto/base64.h>
#include <lepto/string.hpp>        // strlen

#define TEST_ALL
#define STOP_ON_FAIL


/*--- Implementation -------------------------------------------------------*/


TEST_CASE( "Base64", "[default]" )
{
   SECTION( "Base64" )
   {
      const char *data="Hello world! Lori ipsum um die Welt";
      const char *data1="Hello ";
      const char *data2="World!";
      
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
}


/*--- Fin ------------------------------------------------------------------*/
