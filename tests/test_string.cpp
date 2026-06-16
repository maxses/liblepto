/**---------------------------------------------------------------------------
 *
 * @file       test_string.cpp
 * @brief      Test CString
 *
 * @date       20250915
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

   SECTION( "Copy" )
   {
      CString s1("Hello World");
      CString s2(s1);

      // Strings are equal
      REQUIRE ( strcmp( s1.data(), s2.data() ) == 0 );

      // s2 needs "fresh" allocated memory.
      REQUIRE ( s1.data() != s2.data() );
   }

   SECTION( "Concenate chars" )
   {
      // Keep the string growing

      const char* lorem=
               "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, "
               "sed diam nonumy eirmod tempor invidunt ut labore et dolore "
               "magna aliquyam erat, sed diam voluptua. At vero eos et accusam "
               "et justo duo dolores et ea rebum. Stet clita kasd gubergren, "
               "no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem "
               "ipsum dolor sit amet, consetetur sadipscing elitr, sed diam "
               "nonumy eirmod tempor invidunt ut labore et dolore magna "
               "aliquyam erat, sed diam voluptua. At vero eos et accusam et "
               "justo duo dolores et ea rebum. Stet clita kasd gubergren, no "
               "sea takimata sanctus est Lorem ipsum dolor sit amet.";
      CString s1;

      for(int i1=0; i1<strlen(lorem); i1++)
      {
         s1+=lorem[i1];
      }

      // Strings are equal
      REQUIRE ( s1 == lorem );
   }

   SECTION( "Concenate strings" )
   {
      // Grow the string when the substrings are longer than CONFIG_LEPTO_LIST_INCREMENT

      const char* lorem=
               "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, "
               "sed diam nonumy eirmod tempor invidunt ut labore et dolore "
               "magna aliquyam erat, sed diam voluptua. At vero eos et accusam "
               "et justo duo dolores et ea rebum. Stet clita kasd gubergren, "
               "no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem "
               "ipsum dolor sit amet, consetetur sadipscing elitr, sed diam "
               "nonumy eirmod tempor invidunt ut labore et dolore magna "
               "aliquyam erat, sed diam voluptua. At vero eos et accusam et "
               "justo duo dolores et ea rebum. Stet clita kasd gubergren, no "
               "sea takimata sanctus est Lorem ipsum dolor sit amet.";
      const char* lorem_substrings[]={
               "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, ",
               "sed diam nonumy eirmod tempor invidunt ut labore et dolore ",
               "magna aliquyam erat, sed diam voluptua. At vero eos et accusam ",
               "et justo duo dolores et ea rebum. Stet clita kasd gubergren, ",
               "no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ",
               "ipsum dolor sit amet, consetetur sadipscing elitr, sed diam ",
               "nonumy eirmod tempor invidunt ut labore et dolore magna ",
               "aliquyam erat, sed diam voluptua. At vero eos et accusam et ",
               "justo duo dolores et ea rebum. Stet clita kasd gubergren, no ",
               "sea takimata sanctus est Lorem ipsum dolor sit amet.",
      };
      CString s1;

      for(int i1=0; i1<sizeof(lorem_substrings) / sizeof(lorem_substrings[0]); i1++)
      {
         s1+=lorem_substrings[i1];
      }

      // Strings are equal
      REQUIRE ( s1 == lorem );
   }
}


/*--- Fin ------------------------------------------------------------------*/
