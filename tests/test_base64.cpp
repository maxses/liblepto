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
         CString encodedStr;
         CByteArray ba;
         encodedStr.allocate(1);

         b64.encode( CByteArray( data ), encodedStr );
         printf("Encoded: %s\n", encodedStr.data());
         REQUIRE ( encodedStr == "SGVsbG8gd29ybGQhIExvcmkgaXBzdW0gdW0gZGllIFdlbHQ=" );

         b64.decode( encodedStr, ba);
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

   SECTION( "EEPROM" )
   {
      CBase64 b64;
      char dest1[200];

      // This EEPROM actually failed
      uint8_t eeprom[0x80] {
         0xF7, 0xF4, 0x4A, 0x12, 0x6F, 0x30, 0x1B, 0x68, 0x9E, 0x4C, 0x72, 0x1A, 0x3A, 0x35, 0x46, 0x4C,
         0x20, 0xF5, 0x05, 0xF5, 0x0A, 0x11, 0xE5, 0x3C, 0x70, 0xD3, 0x96, 0x6A, 0x35, 0xE6, 0xF8, 0x36,
         0xE0, 0x8E, 0xAA, 0x80, 0x56, 0x66, 0x1F, 0xE0, 0x79, 0x33, 0xE3, 0x0A, 0x94, 0xC5, 0x10, 0xB3,
         0x69, 0x30, 0x79, 0x5C, 0x78, 0xA7, 0xB4, 0xF6, 0xC8, 0x20, 0x37, 0x74, 0xBD, 0xEB, 0x2E, 0x6B,
         0x2C, 0x1A, 0xF0, 0xB7, 0xEC, 0xD4, 0xCE, 0x45, 0x47, 0x15, 0x87, 0xF2, 0x34, 0x94, 0x0F, 0x05,
         0x1A, 0x7B, 0x06, 0xF1, 0xA8, 0x58, 0x44, 0x8C, 0xB5, 0xF3, 0x3B, 0x62, 0x45, 0xE9, 0xFF, 0xF4,
         0xDA, 0x5F, 0xEE, 0xAC, 0x7F, 0xDF, 0x9A, 0x1D, 0x82, 0x2B, 0x7C, 0x11, 0x24, 0x3B, 0x8C, 0x6F,
         0xF1, 0x37, 0x83, 0xE3, 0x93, 0x18, 0x04, 0x87, 0xCE, 0xA8, 0x8B, 0x1E, 0xB8, 0x58, 0xCE, 0xAD,
      };
      const char* result=
            "9/RKEm8wG2ieTHIaOjVGTCD1BfUKEeU8cNOWajXm+DbgjqqAVmYf4Hkz4wqUxRCzaTB5XHintPbI"
            "IDd0vesuaywa8Lfs1M5FRxWH8jSUDwUaewbxqFhEjLXzO2JF6f/02l/urH/fmh2CK3wRJDuMb/E3"
            "g+OTGASHzqiLHrhYzq0=";

      b64.encode( (const uint8_t*)eeprom, 0x80, (char*)dest1, 200 );
      printf("Encoded: %s\n", dest1);

      REQUIRE ( strcmp( dest1, result) == 0 );
   }

   SECTION( "Decode" )
   {
      CBase64 b64;
      const char* strings[]{
         "9/RKEm8wG2ieTHIaOjVGTD==",
         "IPUF9QoR5Txw05ZqNeb4Nk==",
         "4I6qgFZmH+B5M+MKlMUQsz==",
         "aTB5XHintPbIIDd0vesua2==",
         "LBrwt+zUzkVHFYfyNJQPBU==",
         "GnsG8ahYRIy18ztiRen/9E==",
         "2l/urH/fmh2CK3wRJDuMbz==",
         "8TeD45MYBIfOqIseuFjOrT==",
      };
      char dest1[200];
      
      for( int i1=0; i1< (int)( sizeof(strings) / sizeof(strings[0]) ); i1++ )
      {
         REQUIRE ( b64.decode( strings[i1], strlen(strings[i1]), (uint8_t*)dest1, 200 ) == 0x10 );
      }
   }
   
   SECTION( "Cornercases decode" )
   {
      CBase64 b64;
      const char* strings[]{
         "vjdY+AAEABkASgHQ"
      };
      const uint8_t decoding[] =
         {0xbe, 0x37, 0x58, 0xf8, 0x00, 0x04, 0x00, 0x19, 0x00, 0x4a, 0x01, 0xd0 };
      char dest1[200];
      
      for( int i1=0; i1< (int)( sizeof(strings) / sizeof(strings[0]) ); i1++ )
      {
         REQUIRE ( b64.decode( strings[i1], strlen(strings[i1]), (uint8_t*)dest1, 200 ) == 0xC );
         REQUIRE ( memcmp( decoding, dest1, 0xC ) == 0 );
      }
   }
   
   SECTION( "Cornercases encode" )
   {
      CBase64 b64;
      // The last character was calculated for invalid data
      const char* strings[]{
          "SBAAAQACAQAGAAAA5gcBEQ=="
      };
      const uint8_t decoding[] =
         { 0x48, 0x10, 0x00, 0x01, 0x00, 0x02, 0x01, 0x00, 0x06, 0x00, 0x00, 0x00, 0xe6, 0x07, 0x01, 0x11 };
      char dest1[200];
      
      for( int i1=0; i1< (int)( sizeof(strings) / sizeof(strings[0]) ); i1++ )
      {
         REQUIRE ( b64.encode( decoding, 0x10, (char*)dest1, 200 ) == 0x18 );
         printf("\n\nEncoded A: %s\n", dest1);
         
         REQUIRE ( strcmp( strings[i1], dest1 ) == 0 );
      }
   }
}


/*--- Fin ------------------------------------------------------------------*/
