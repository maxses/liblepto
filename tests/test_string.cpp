/**---------------------------------------------------------------------------
 *
 * @file       test_string.cpp
 * @brief      Test CString
 *
 * @date       20250915
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

#include <lepto/string.hpp>

#define TEST_ALL
#define STOP_ON_FAIL


/*--- Implementation -------------------------------------------------------*/


TEST_CASE( "String", "[default]" )
{
   SECTION( "Manipulate" )
   {
      CString s("Manipulate");

      REQUIRE ( s == "Manipulate" );

      s.remove(-1,1);
      REQUIRE ( s == "Manipulat" );

      s.remove(-3,2);
      REQUIRE ( s == "Maniput" );

      s.remove(3,2);
      printf("%s\n", s.data());
      REQUIRE ( s == "Manut" );

      //s="h";
      s.remove(-4,4);
      printf(">%s\n", s.data());
      REQUIRE ( s == "M" );

      s.remove(-1,1);
      printf(">%s\n", s.data());
      REQUIRE ( s == "" );

      s="Hallo Welt!";
      s.remove(-6, 1000);
      printf(">%s\n", s.data());
      REQUIRE ( s == "Hallo" );

      // Operator =
      CString s1="Hallo";
      s1="Welt";
      REQUIRE ( s1 == "Welt" );

      // Operator +=
      CString s2="Hallo ";
      s2+="Welt!";
      REQUIRE ( s2 == "Hallo Welt!" );

      CString s3="Hallo ";
      CString s4="Welt!";
      CString s5 = s3 + s4;
      REQUIRE ( s5 == "Hallo Welt!" );
   }
}


/*--- Fin ------------------------------------------------------------------*/
