/**---------------------------------------------------------------------------
 *
 * @file       test_list.cpp
 * @brief      Test CList
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

#include <lepto/list.hpp>
#include <lepto/ring.hpp>
#include <list>
// Optionally run the test on QList instead of CList
#include <QList>


#define TEST_ALL
#define STOP_ON_FAIL


/*--- Implementation -------------------------------------------------------*/


TEST_CASE( "List", "[default]" )
{
   SECTION( "Volatile" )
   {
      CList<int> list(0x10);
      // std::list<int> list(0x10);
      // QList<int> list(0x10);

      list.setVolatile( true );

      // Fill some data which should disappear
      for(int i1=0; i1<0x5; i1++)
      {
         list << i1;
      }

      //lHint << "Checking adding to list";
      for(int i1=0; i1<0x10; i1++)
         list << i1;

      #if LEPTO_RING_SPARE_ENTRIES == 1
         int checkCount=0xF;
         int offset=1;
      #else
         int checkCount=0x10;
         int offset=0;
      #endif
         
      REQUIRE( list.count() == checkCount );
      for(int i1=0; i1<checkCount; i1++)
      {
         printf( "%d: %d / %d\n", i1, *list.at(i1), *(list.getEntry(i1)) );
         REQUIRE ( *list.at( i1 ) == ( i1 + offset ) );
         REQUIRE ( *list.getEntry( i1 ) == ( i1 + offset ) );
      }

      // 15 + 14 ... + 3 + 2 + 1
      REQUIRE( list.crosssum() == 0x78 );
   }

   SECTION( "Iterate" )
   {
      CRing<int> list(0x3);
      // std::list<int> list(0x3);
      // QList<int> list(0x3);
      
      list.push_back(10);
      list.push_back(20);
      list.push_back(30);
      
      int total=0;

      REQUIRE( list.begin() != list.end() );
      REQUIRE( list.pushable() == false );

      int last=0;
      for(auto e:list)
      {
         total += e;
         REQUIRE( e > last );
         last = e;
      }

      #if LEPTO_RING_SPARE_ENTRIES == 1
         REQUIRE( total == 30 );
      #else
         REQUIRE( total == 60 );
      #endif
      
   }

   SECTION( "Ring" )
   {
      CRing< int > ring( 2 + LEPTO_RING_SPARE_ENTRIES );
      //ring.setFatal(false);

      /* - push values to buffer should succeed till buffer is full
       * - Poping entries should return expected vales
       */

      // Just the plain start value
      REQUIRE ( ring.frontEntry() == nullptr );
      REQUIRE ( ring.push_back(0x10) == true );
      REQUIRE ( ring.push_back(0x20) == true );
      REQUIRE ( ring.push_back(0x30) == false );
      REQUIRE ( *ring.frontEntry() == 0x10 );
      ring.dropFront();
      REQUIRE ( *ring.frontEntry() == 0x20 );
      ring.dropFront();
      REQUIRE ( ring.frontEntry() == nullptr );
   }

   SECTION( "Overlap" )
   {
      CRing< int > ring(4);

      /* - Push some values
       * - Pop an values
       * - Push some values
       * - Drop entries and verify expected content
       */

      // Just the plain start value
      REQUIRE ( ring.frontEntry() == nullptr );
      REQUIRE ( ring.push_back(0x10) == true );
      REQUIRE ( ring.push_back(0x20) == true );
      ring.dropFront();
      REQUIRE ( ring.push_back(0x30) == true );
      REQUIRE ( ring.push_back(0x40) == true );

      REQUIRE ( *ring.frontEntry() == 0x20 );
      ring.dropFront();
      REQUIRE ( *ring.frontEntry() == 0x30 );
      ring.dropFront();
      REQUIRE ( *ring.frontEntry() == 0x40 );
      ring.dropFront();
      REQUIRE ( ring.frontEntry() == nullptr );
      //ring.release();
   }

   SECTION( "Next" )
   {
      CRing< int > ring( 3 + LEPTO_RING_SPARE_ENTRIES );

      /* - Use pushBack() to push some values
       * - Use '<<' to push an entry
       * - Check for expected content
       */

      // Just the plain start value
      ring.push_back(0x10);
      *ring.backEntry()=0x20;
      ring.pushBack();
      ring << 0x30;

      REQUIRE ( ring.backEntry() == nullptr );
      REQUIRE ( ring.pushable() == false );

      REQUIRE ( ring.pop() == 0x10 );
      REQUIRE ( ring.pop() == 0x20 );
      REQUIRE ( ring.pop() == 0x30 );
      REQUIRE ( ring.isDataAvailable() == false );
   }

   SECTION( "Overflow" )
   {
      CRing< int > ring( 10 + LEPTO_RING_SPARE_ENTRIES );

      // When top-position is at the very end and the bottom is at the beginning,
      // adding an additional entry must not be allowed. Stack would become
      // "empty" but all entries are marked 'valid'.
      ring.setBottomTop(0, ring.getMaxEntriesDuplicated() - 1 );
      REQUIRE ( ring.tryReserve() == (ringIndex_t)-1 );
      REQUIRE ( ring.push_back( 10 ) == false );
   }

   SECTION( "Average" )
   {
      #if ! IS_ENABLED( CONFIG_LEPTO_RING_SUPPORT_VOLATILE )
         #error I want to test 'setVolatile()'. Please set CONFIG_LEPTO_RING_SUPPORT_VOLATILE.
      #endif

      CRing<float> ring(4);
      ring.setVolatile(true);

      for(int i1=0; i1<4; i1++)
         ring.push_back(i1+1);

      // lHint << "IV: " << ring.average();
      printf("Count: %d\n", ring.count() );
      
      #if LEPTO_RING_SPARE_ENTRIES == 1
            REQUIRE ( ring.average() == ( ( 2.0 + 3.0 + 4.0) / 3.0) );
      #else
            REQUIRE ( ring.average() == ( (1.0+2.0+3.0+4.0)/4.0) );
      #endif
            
      for(int i1=0; i1<5; i1++)
         ring.push_back(i1+1);

      // lHint << "IV: " << ring.average();
      // lHint << "SV: " << ( (    2.0+3.0+4.0+5.0 ) / 4.0 );
      #if LEPTO_RING_SPARE_ENTRIES == 1
         REQUIRE ( ring.count() == 3 );
         REQUIRE ( ring.average() == ( ( 3.0 + 4.0 + 5.0 ) / 3.0 ) );
      #else
         REQUIRE ( ring.count() == 4 );
         REQUIRE ( ring.average() == ( ( 2.0 + 3.0 + 4.0 + 5.0 ) / 4.0 ) );
      #endif
   }
}


/*--- Fin ------------------------------------------------------------------*/
