/**---------------------------------------------------------------------------
 *
 * @file       test_ring_threaded.cpp
 * @brief      test thread safety for CRing::tryReserve()
 *
 * @date       20250801
 * @author     Maximilian Seesslen <mes@seesslen.net>
 * @copyright  SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <lepto/lepto.h>
#include <lepto/ring.hpp>
#include <lepto/bufferRing.hpp>
#include <QThread>
#include <QCoreApplication>


#define TEST_ALL
#define STOP_ON_FAIL
#define PRODUCER_LOOPS                 (1000000/2)
//#define PRODUCER_LOOPS               10000
#define PRODUCERS                      4           // Only 4, 6, 8 or 10

// The overflows are gone since m_busyProducing was introduced. Consumer can
// not consume more than once while producer did not finish.
#if IS_ENABLED( CONFIG_LEPTO_RING_DOWNSIZE )
   #define RING_SIZE          ( 64 )
#else
   #define RING_SIZE          ( 64 )
#endif

#if IS_ENABLED( CONFIG_LEPTO_RING_NO_THREADS )
   #error The feature 'CONFIG_LEPTO_RING_NO_THREADS' is enabled. Tests will fail.\
      Please disable the feature for host tests.
#endif

//#define MAY_SLEEP       usleep(1);
#define MAY_SLEEP


/*--- Declaration ----------------------------------------------------------*/


struct SElement
{
   int id;
   int counter;
};


class CProducer: public QThread
{
   Q_OBJECT

   int m_counter;
   int m_id;
   bool& m_pFinished;
   CRing<SElement>& m_ring;

public:
   CProducer( int id, bool& finished, CRing<SElement>& ring )
      :m_counter{ id * 10000 }
      ,m_id(id)
      ,m_pFinished(finished)
      ,m_ring(ring)
   {
   }
   void run() override
   {
      while( m_counter < PRODUCER_LOOPS )
      {
         if( m_ring.pushable() )
         {
            int index;
            if( ( index = m_ring.tryReserve() ) >= 0 )
            {
               //lInfo("PUSH");
               m_counter++;
               *m_ring.reservedEntry( index) = SElement{m_id, m_counter};
               m_ring.pushReserved( index );
            }
         }
         MAY_SLEEP;
      }
      m_pFinished=true;
   }
};


class CConsumer: public QThread
{
   private:
      CRing<SElement> m_ring;
      CProducer m_producers[ PRODUCERS ];
      bool m_finished[ PRODUCERS ];
      int m_counters[ PRODUCERS ];
      int m_loops=0;
      int m_errors=0;
      int m_totalCounter=0;
      
   public:
      CConsumer()
         :m_ring( RING_SIZE )
         ,m_producers{
               {0, m_finished[0], m_ring},
               {1, m_finished[1], m_ring},
               #if PRODUCERS >= 4
                  {2, m_finished[2], m_ring},
                  {3, m_finished[3], m_ring},
               #if PRODUCERS >= 6
                  {4, m_finished[4], m_ring},
                  {5, m_finished[5], m_ring},
               #if PRODUCERS >= 8
                  {6, m_finished[6], m_ring},
                  {7, m_finished[7], m_ring},
               #if PRODUCERS >= 10
                  {8, m_finished[8], m_ring},
                  {9, m_finished[9], m_ring},
               #endif // >= 10
               #endif // >= 8
               #endif // >= 6
               #endif // >= 4
          }
         ,m_finished{0}
         ,m_counters{
               0,
               10000,
               #if PRODUCERS >= 4
                  20000,
                  30000,
               #endif
               #if PRODUCERS >= 8
                  40000,
                  50000,
                  60000,
                  70000,
               #endif
             }
      {
         return;
      }
      void run() override
      {
         for(int i1=0; i1<PRODUCERS; i1++)
         {
            m_producers[i1].start();
         }

         if( allProducerFinished() )
         {
            lWarning("Not plausible");
         }
         int noDAtaCounter=0;
         QElapsedTimer timer;
         timer.start();
         qint64 maxNoData=0;
         
         // Test did not finish yet?
         while( ! allProducerFinished() )
         {
            if( m_ring.isDataAvailable() )
            {
               //lInfo("POP");
               m_totalCounter++;
               maxNoData=qMax( maxNoData,  timer.elapsed() );
               SElement* entry=m_ring.frontEntry();
               int expected=m_counters[entry->id] + 1;
               if( entry->counter != expected )
               {
                  lInfo("Expected: %d; got:%d; producer: %d; total counter: %d", expected
                                 , entry->counter, entry->id, m_totalCounter);
                  for(int i1=0; i1<PRODUCERS; i1++)
                  {
                     lInfo("   [%d]:%d", i1, m_counters[i1]);
                  }
                  m_errors++;
               }
               m_counters[entry->id] = entry->counter;
               m_ring.dropFront();
               m_loops++;
               noDAtaCounter=0;
               timer.restart();
            }
            else
            {
               noDAtaCounter++;
               if( timer.hasExpired( 10 * 1000 ) )
               {
                  lInfo("Pushable: %s", m_ring.pushable() ? "yes": "no");
                  lInfo("Full : %s", m_ring.isFull( m_ring.frontIndex(), m_ring.backIndex() ) ? "yes": "no");
                  lInfo("Bottom: %d", m_ring.frontIndex() );
                  lInfo("Top: %d", m_ring.backIndex() );
                  lInfo("---------");
                  m_ring.dump();
                  
                  lFatal("No data available");
               }
               else
               {
                  
               }
            }
            MAY_SLEEP;
         }
         // Its '0 ms', useless information
         // lInfo("Maximum time without data: %d ms", maxNoData);
         QCoreApplication::exit(0);
         return;
      }
      bool allProducerFinished()
      {
         for(int i1=0; i1<PRODUCERS; i1++)
         {
            if(!m_finished[i1])
            {
               return( false );
            }
         }
         return( true );
      }
      int getLoops()
      {
         return( m_loops );
      }
      int getErrors()
      {
         return( m_errors );
      }
};


/*--- Fin ------------------------------------------------------------------*/
