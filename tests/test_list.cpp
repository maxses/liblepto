/**---------------------------------------------------------------------------
 *
 * @file       test_list.cpp
 * @brief      Test CList
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
         int checkCount=0x10; // Addjusted automatically meanwhile; 0xF;
         int offset=0; // Addjusted automatically meanwhile; 1;
      #else
         int checkCount=0x10;
         int offset=0;
      #endif

      for(int i1=0; i1<list.count(); i1++)
      {
         printf( "X %d: %d / %d\n", i1, *list.at(i1), *(list.getEntry(i1)) );
      }
      printf("Front: %d\n", list.frontIndex() );
      printf("Back:  %d\n", list.backIndex() );
      printf("Full:  %s\n", list.isFull() ? "yes" : "no" );
      printf("Max:   %d\n", list.getMaxEntries() );
      printf("Count: %d\n", list.count() );

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

      #if IS_ENABLED( CONFIG_LEPTO_LIST_RESIZABLE )
         list.setResizable(false);
      #endif
      
      list.push_back(10);
      list.push_back(20);
      list.push_back(30);
      
      int total=0;

      REQUIRE( list.begin() != list.end() );
      list.pushable();
      REQUIRE( list.pushable() == false );

      int last=0;
      for(auto e:list)
      {
         total += e;
         REQUIRE( e > last );
         last = e;
      }

      // Spare entry is added automatically now
      #if 0 //LEPTO_RING_SPARE_ENTRIES == 1
         REQUIRE( total == 30 );
      #else
         REQUIRE( total == 60 );
      #endif
      
   }

   SECTION( "Ring" )
   {
      CRing< int > ring( 2 /*+ LEPTO_RING_SPARE_ENTRIES */ );
      //ring.setFatal(false);

      /* - push values to buffer should succeed till buffer is full
       * - Poping entries should return expected vales
       */

      // Just the plain start value
      REQUIRE ( ring.frontEntry() == nullptr );
      REQUIRE ( ring.push_back(0x10) == true );
      REQUIRE ( ring.push_back(0x20) == true );
      #if ! IS_ENABLED( CONFIG_LEPTO_LIST_RESIZABLE )
      REQUIRE ( ring.push_back(0x30) == false );
      #endif
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
      CRing< int > ring( 3 /*+ LEPTO_RING_SPARE_ENTRIES */ );

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

   // This should be Valid
   #if 0
   SECTION( "Overflow" )
   {
      CRing< int > ring( 10 + LEPTO_RING_SPARE_ENTRIES );

      // When top-position is at the very end and the bottom is at the beginning,
      // adding an additional entry must not be allowed. Stack would become
      // "empty" but all entries are marked 'valid'.
      ring.setFrontBack(0, ring.getMaxEntriesDuplicated() - 1 );
      REQUIRE ( ring.tryReserve() == (ringIndex_t)-1 );

      #if ! IS_ENABLED( CONFIG_LEPTO_LIST_RESIZABLE )
         REQUIRE ( ring.push_back( 10 ) == false );
      #endif
   }
   #endif

   SECTION( "Average" )
   {
      #if ! IS_ENABLED( CONFIG_LEPTO_RING_SUPPORT_VOLATILE )
         #error I want to test 'setVolatile()'. Please set CONFIG_LEPTO_RING_SUPPORT_VOLATILE.
      #endif

      CRing<float> ring(4);
      ring.setVolatile(true);

      for(int i1=0; i1<4; i1++)
         ring.push_back( (float)(i1+1) );

      // lHint << "IV: " << ring.average();
      printf("Count: %d\n", ring.count() );
      
      #if 0 // LEPTO_RING_SPARE_ENTRIES == 1
            REQUIRE ( ring.average() == ( ( 2.0f + 3.0f + 4.0f ) / 3.0f ) );
      #else
            REQUIRE ( ring.average() == ( ( 1.0f + 2.0f + 3.0f + 4.0f ) / 4.0f ) );
      #endif
            
      for(int i1=0; i1<5; i1++)
         ring.push_back( (float)(i1+1) );

      // lHint << "IV: " << ring.average();
      // lHint << "SV: " << ( (    2.0+3.0+4.0+5.0 ) / 4.0 );
      #if 0 // LEPTO_RING_SPARE_ENTRIES == 1
         REQUIRE ( ring.count() == 3 );
         REQUIRE ( ring.average() == ( ( 3.0f + 4.0f + 5.0f ) / 3.0f ) );
      #else
         REQUIRE ( ring.count() == 4 );
         REQUIRE ( ring.average() == ( ( 2.0f + 3.0f + 4.0f + 5.0f ) / 4.0f ) );
      #endif
   }
   
   SECTION( "Iterator" )
   {
      CList<int> liste(10);
      CList<int>::CIterator iterator(&liste, 0);
      REQUIRE( iterator.realIndex() == 0 );
      iterator++;
      REQUIRE( iterator.realIndex() == 1 );
   }
   
   SECTION( "C++ iterate" )
   {
      CList<int> list(0);
      int cnt=0;
      for(const int &element: list )
      {
         (void)element;
         cnt++;
      }

      REQUIRE( cnt == 0 );

      bool pushable=
      #if IS_ENABLED( CONFIG_LEPTO_LIST_RESIZABLE )
         true;
      #else
         false;
      #endif

      //REQUIRE(
               list.push_back(0x22);
            //== pushable );

      cnt=0;
      
      for(const int &element: list )
      {
         (void)element;
         cnt++;
      }
      
      // when CONFIG_LEPTO_RING_DEFAULT_SIZE is 0, the list will get vitalized
      // when pushing values. It is still not resizable as long as
      // CONFIG_LEPTO_LIST_RESIZABLE is not set.
      #if IS_ENABLED( CONFIG_LEPTO_LIST_RESIZABLE ) || \
            ( ( CONFIG_LEPTO_RING_DEFAULT_SIZE == 0 ) && IS_ENABLED( CONFIG_LEPTO_RING_VITALIZE ) )
         REQUIRE( cnt == 1 );
      #else
         REQUIRE( cnt == 0 );
      #endif
   }

   SECTION( "Count" )
   {
      CList<int> list1(10);

      #if IS_ENABLED( CONFIG_LEPTO_LIST_RESIZABLE )
         list1.setResizable( true );
      #endif

      for(int i1=0; i1<9; i1++)
      {
         list1.push_back(i1);
      }
      REQUIRE( list1.count() == 9 );

      #if IS_ENABLED( CONFIG_LEPTO_LIST_DOWNSIZE ) && IS_ENABLED( CONFIG_LEPTO_LIST_RESIZABLE )
          list1.push_back( 0xff );

          list1.count();
          REQUIRE( list1.count() == 10 );
      #endif
   }

   // Do not demand CONFIG_LEPTO_LIST_RESIZABLE to be set; non-resizable lists
   // Should also be tested.
   #if 0
      #if ! IS_ENABLED( CONFIG_LEPTO_LIST_RESIZABLE )
         #error "Please set CONFIG_LEPTO_LIST_RESIZABLE, I want to test it"
      #endif
   #endif

   SECTION( "Resize" )
   {
      CList<int> list1(10);
      CList<int> list2(10);

      #if IS_ENABLED( CONFIG_LEPTO_LIST_RESIZABLE )
         list1.setResizable( true );
         list2.setResizable( false );
      #endif

      for(int i1=0; i1<200; i1++)
      {
         list1.push_back(i1);
         list2.push_back(i1);
      }

      #if IS_ENABLED( CONFIG_LEPTO_LIST_RESIZABLE )
         REQUIRE( list1.count() == 200 );
      #else
         REQUIRE( list1.count() == 10 /*- LEPTO_RING_SPARE_ENTRIES */ );
      #endif

      // Must not grow because it was disabled. E.g. for buffer feeded in ISR.
      REQUIRE( list2.count() == 10 /*- LEPTO_RING_SPARE_ENTRIES */ );
   }

   #if 1 // IS_ENABLED( CONFIG_LEPTO_LIST_RESIZABLE )

   SECTION( "Expand shifted" )
   {
      // Prepare an List that the front is quite in the middle and the content
      // overlaps the memory end. Force an reallocation (when enabled) and check
      // that the content is properly moved.

      // Fill '00000'
      CList<char> str(10);
      for(int i1=0; i1<5; i1++)
      {
         str.push_back('0');
      }

      // Append '01234'
      for(int i1=0; i1<5; i1++)
      {
         str.push_back('0'+i1);
      }

      // Remove the first '00000' to
      for(int i1=0; i1<5; i1++)
      {
         str.dropFront();
      }

      // Append '56789'
      for(int i1=0; i1<5; i1++)
      {
         str.push_back('5'+i1);
      }
      #if 1 // LEPTO_RING_SPARE_ENTRIES == 0
         REQUIRE( str.getMaxEntries() == 10 + LEPTO_RING_SPARE_ENTRIES );
      #else
         REQUIRE( str.getMaxEntries() == ( 10 - LEPTO_RING_SPARE_ENTRIES ) + CONFIG_LEPTO_LIST_INCREMENT );
      #endif

      #if IS_ENABLED( CONFIG_LEPTO_LIST_RESIZABLE )
         REQUIRE( str.checkSpace(11) == true );
         printf( "### '%s'\n", str.getBuffers() );
         REQUIRE( memcmp( &str.getBuffers()[0], "01234567890", 10 ) == false );
      #else
         REQUIRE( str.checkSpace(11) == false );
         printf( "### '%s'\n", str.getBuffers() );
         /*
         for(int i1=0; i1<10; i1++)
         {
             printf( "### 0x%02X '%c'\n", str.getBuffers()[i1], str.getBuffers()[i1] );
         }
         */
         #if LEPTO_RING_SPARE_ENTRIES == 0
            REQUIRE( memcmp( &str.getBuffers()[0], "5678901234", 10 ) == false );
         #else
            // When there is a spare entry, the list is automatically bigger
            REQUIRE( memcmp( &str.getBuffers()[0], "67890012345", 10 ) == false );
         #endif
      #endif

   }

   #endif
}


/*--- Fin ------------------------------------------------------------------*/
