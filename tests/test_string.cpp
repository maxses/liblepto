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
   SECTION( "tryAppend" )
   {
      CString s(128);
      REQUIRE( s.length() == 0 );
      REQUIRE( s.tryAppend("Hallo") == true );
      REQUIRE( s.length() == 5 );
      REQUIRE( s.tryAppend(" ") == true );
      REQUIRE( s.length() == 6 );
      REQUIRE( s.tryAppend("Welt!") == true );
      //s.length();
      REQUIRE( s.length() == 11 );
      REQUIRE( s == "Hallo Welt!" );
   }

   SECTION( "Manipulate" )
   {
      CString s("Manipulate");

      REQUIRE ( s == "Manipulate" );
      s.length();
      REQUIRE ( s.length() == 10 );

      s.remove(-1,1);
      printf("### %s\n", s.data());
      REQUIRE ( s == "Manipulat" );

      s.remove(-3,2);
      printf( "### %s\n", s.data() );
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

      s="HalloWelt";
      REQUIRE ( s.length() == 9 );
      s.remove(-4, 1000);
      printf(">%s\n", s.data());
      REQUIRE ( s == "Hallo" );

      // Operator =
      CString s1="Hallo";
      s1="Welt";
      REQUIRE ( s1 == "Welt" );

      // Operator +=
      CString s2(15);
      s2="Hallo ";
      s2+="Welt!";
      printf("### s2=%s\n", s2.data());
      REQUIRE ( s2 == "Hallo Welt!" );

      CString s3  { "Hallo " };
      CString s4  { "Welt!" };
      CString s5  { s3 + s4 };

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
      #if ! IS_ENABLED( CONFIG_LEPTO_LIST_RESIZABLE )
         CString s1( 2048 );
      #else
         CString s1;
      #endif

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
               #if 0
               "nonumy eirmod tempor invidunt ut labore et dolore magna "
               "aliquyam erat, sed diam voluptua. At vero eos et accusam et "
               "justo duo dolores et ea rebum. Stet clita kasd gubergren, no "
               "sea takimata sanctus est Lorem ipsum dolor sit amet."
               #endif
               ;
      const char* lorem_substrings[]={
               "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, ",
               "sed diam nonumy eirmod tempor invidunt ut labore et dolore ",
               "magna aliquyam erat, sed diam voluptua. At vero eos et accusam ",
               "et justo duo dolores et ea rebum. Stet clita kasd gubergren, ",
               "no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ",
               "ipsum dolor sit amet, consetetur sadipscing elitr, sed diam ",
               #if 0
               "nonumy eirmod tempor invidunt ut labore et dolore magna ",
               "aliquyam erat, sed diam voluptua. At vero eos et accusam et ",
               "justo duo dolores et ea rebum. Stet clita kasd gubergren, no ",
               "sea takimata sanctus est Lorem ipsum dolor sit amet.",
               #endif
      };
      #if ! IS_ENABLED( CONFIG_LEPTO_LIST_RESIZABLE )
         CString s1(2048);
      #else
         CString s1;
      #endif

      for(int i1=0; i1<sizeof(lorem_substrings) / sizeof(lorem_substrings[0]); i1++)
      {
         s1+=lorem_substrings[i1];
         REQUIRE( s1.length() > 0 );
         printf( "### C: Adding %s : %s (%d, %d, %d)\n", lorem_substrings[i1], s1.data(), s1.backPos(), s1.length(), s1.realEnd() );
      }

      printf( "### s1 B=%s\n", s1.data() );
      // Strings are equal
      REQUIRE ( s1 == lorem );
   }
}


/*--- Fin ------------------------------------------------------------------*/
