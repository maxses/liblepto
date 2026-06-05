/**---------------------------------------------------------------------------
 *
 * @file       test_signal.cpp
 * @brief      Test signals and signal pools
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

#if 0
   #undef CONFIG_LEPTO_SIGNAL_CHAIN
   #define CONFIG_LEPTO_SIGNAL_NO_METHOD_AS_FUNCTION        0
#else
   #define CONFIG_LEPTO_SIGNAL_CHAIN                        1
   #define CONFIG_LEPTO_SIGNAL_NO_METHOD_AS_FUNCTION        1
#endif

#include <lepto/signal.hpp>
#include <lepto/signalPool.hpp>
#include <lepto/signalPoolStatic.hpp>
#include <lepto/signalDeferred.hpp>

#define TEST_ALL
#define STOP_ON_FAIL
#define START_VALUE     555


/*--- Implementation -------------------------------------------------------*/


class C1
{
   int m_counter = START_VALUE;
   int m_last = -1;

   public:
      void _slot1(int dummy, int add)
      {
         (void)dummy;

         REQUIRE( add > m_last );
         m_counter+=add;
         m_last = add;
      }

      void _slot2( int add )
      {
         REQUIRE( add > m_last );
         m_counter+=add;
         m_last = add;
      }

      void _slot3( )
      {
         m_counter++;
      }

      int getCounter()
      {
         return( m_counter );
      }
};

int counter=START_VALUE;

void functionSlot(int add)
{
   counter+=add;
};

int getCounter()
{
   return( counter );
};


TEST_CASE( "Signal", "[default]" )
{
   SECTION( "Signal C++ method" )
   {
      C1 obj;
      
      #if IS_ENABLED( CONFIG_LEPTO_SIGNAL_CHAIN )
         C1 obj2;
      #endif
         
      CSignal<void, int, int>sig;

      sig.connect(&obj, &C1::_slot1);
      
      #if IS_ENABLED( CONFIG_LEPTO_SIGNAL_CHAIN )
         sig.connect(&obj2, &C1::_slot1);
      #endif

      //lHint << "Checking adding to list";
      for(int i1=0; i1<0x10; i1++)
         sig.emitSignal(0, i1);

      // 15 + 14 ... + 3 + 2 + 1
      REQUIRE( obj.getCounter() == 0x78 + START_VALUE );
      
      #if IS_ENABLED( CONFIG_LEPTO_SIGNAL_CHAIN )
         REQUIRE( obj2.getCounter() == 0x78 + START_VALUE );
      #endif
   }

   #if IS_ENABLED( CONFIG_LEPTO_SIGNAL_FUNCTION )
   
   SECTION( "Signal function" )
   {
      CSignal<void, int>sig;

      sig.connect( &functionSlot );

      //lHint << "Checking adding to list";
      for(int i1=0; i1<0x10; i1++)
         sig.emitSignal(i1);

      // 15 + 14 ... + 3 + 2 + 1
      REQUIRE( getCounter() == 0x78 + START_VALUE );
   }

   #endif // ? CONFIG_LEPTO_SIGNAL_FUNCTION
   
   SECTION( "Pending Signal Pool" )
   {
      C1 obj;
      CPendingSignalPool pool(0x11);
      CSignal<void, int> sig;

      sig.connect(&obj, &C1::_slot2);

      //lHint << "Checking adding to list";
      for(int i1=0; i1<0x10; i1++)
      {
         pool.enqueueSignal(sig,i1);
         //sig.emitSignal(i1);
      }

      // Nothing should have happen yet
      REQUIRE( obj.getCounter() == START_VALUE );

      pool.handlePendingSignals();

      // 15 + 14 ... + 3 + 2 + 1
      REQUIRE( obj.getCounter() == 0x78 + START_VALUE );
   }

   SECTION( "Pending Signal Pool Static" )
   {
      C1 obj;
      CPendingSignalPoolStatic pool(0x11);
      CSignal<void> sig;

      sig.connect(&obj, &C1::_slot3);

      //lHint << "Checking adding to list";
      for(int i1=0; i1<0x10; i1++)
      {
         pool.enqueueSignal(sig);
         //sig.emitSignal(i1);
      }

      // Nothing should have happen yet
      REQUIRE( obj.getCounter() == START_VALUE );

      pool.handlePendingSignals();

      REQUIRE( obj.getCounter() == 0x10 + START_VALUE );
   }
   
   SECTION( "Deferred Signal" )
   {
      C1 obj;
      constexpr int sigCount=0x10;
      // "+1" Depending on configuration, 1 element is not usable
      CSignalDeferred<void> sig( sigCount + 1 );
      
      sig.connect(&obj, &C1::_slot3);
      
      //lHint << "Checking adding to list";
      for(int i1=0; i1<sigCount; i1++)
      {
         sig.emitDeferred();
      }
      
      // Nothing should have happen yet
      REQUIRE( obj.getCounter() == START_VALUE );
      
      sig.eventLoop();
      
      REQUIRE( obj.getCounter() == START_VALUE + sigCount );
   }

   SECTION( "Sizes" )
   {
      CSignal<int, int> sig;

      printf( "Size signal: %d\n", (int)sizeof( sig ) );
      printf( "Size functor: %d\n", (int)sizeof( CFunctorMethodAsFunction<int, int> ) );
      #if LEPTO_SIGNAL_DO_VIRTUAL
         printf( "Virtual function is used\n" );
      #else
         printf( "Virtual function is NOT used\n" );
      #endif

      #if IS_ENABLED( CONFIG_LEPTO_SIGNAL_CHAIN )
         printf( "Signal chain is supported\n" );
      #else
         printf( "Signal chain is NOT supported\n" );
      #endif

      #if LEPTO_SIGNAL_FUNCTOR_ALLOCATED
         printf( "Connection is allocated\n" );
      #else
         printf( "Connection is NOT allocated\n" );
      #endif

   }
}


/*--- Fin ------------------------------------------------------------------*/
