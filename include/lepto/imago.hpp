#ifndef LEPTO_IMAGO_HPP
#define LEPTO_IMAGO_HPP
/**---------------------------------------------------------------------------
 *
 * @file       imago.hpp
 * @brief      Helps testing temporal sequences off application
 *
 * This can be used for unit tests like CATCH2.
 * The test procedure can wait for something while an custom event
 * loop is kept calling periodically.
 *
 * An example is CANSwitch. When setting an target illumination, after
 * a given time, half the target value has to be reached. After some
 * more time, the full traget  value has to be reached.
 *
 * Some tests may have an big jitter in the result. the jitter may
 * depend on the load of the host machin. When tests are succeeding
 * while developing, they may fail on an buildserver while many other
 * repositories are build in the background. To simulate such load,
 * stress-ng can be used:
 *
 *    ~$ stress-ng -c 64 -l 99
 *
 * @date       20240024
 * @author     Maximilian Seesslen <mes@seesslen.net>
 * @copyright  SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <lepto/log.h>
#include <sys/time.h>


/*--- Declaration ----------------------------------------------------------*/


template<typename MainFunctor>
class CImago
{
   private:
      typedef long long int itimer_t;
      static constexpr itimer_t _MSECS_PER_SEC = 1000;
      static constexpr itimer_t _USECS_PER_MSEC = 1000;
   
      itimer_t m_timer;
      int m_index=0;
      int m_timeoutSeconds=60;
      MainFunctor m_functorEventLoop;

   public:
      CImago(MainFunctor functor)
         :m_functorEventLoop(functor)
      {
      }

      bool waitTime(int seconds)
      {
         m_timer=now();
         while( elapsedSeconds(m_timer) < seconds )
         {
            m_functorEventLoop();
         }
         return( succeed() );
      }

      bool waitTimeMs(int mseconds)
      {
         m_timer=now();
         while( elapsedMSeconds(m_timer) < mseconds )
         {
            m_functorEventLoop();
         }
         return( succeed() );
      }

      template<typename Functor>
      int waitFor(Functor functor)
      {
         step();
         while ( !functor() )
         {
            m_functorEventLoop();
            if( elapsedSeconds(m_timer) > m_timeoutSeconds )
            {
               return(-1);
            }
         }
         return( elapsedSeconds(m_timer) );
      }

      template<typename Functor>
      int waitForEventMSeconds(Functor functor)
      {
         step();
         while ( !functor() )
         {
            m_functorEventLoop();
            if( elapsedSeconds(m_timer) > m_timeoutSeconds )
            {
               return(-1);
            }
         }
         return( elapsedMSeconds(m_timer) );
      }
      template<typename Functor>
      bool waitFor(Functor functor, int timeoutSeconds)
      {
         step();
         while ( !functor() )
         {
            m_functorEventLoop();
            if( elapsedSeconds(m_timer) > timeoutSeconds )
            {
               return( false );
            }
         }
         return( true );
      }
      template<typename Functor>
      bool waitForMS(Functor functor, int timeoutMSeconds)
      {
         step();
         while ( !functor() )
         {
            m_functorEventLoop();
            if( elapsedMSeconds(m_timer) > timeoutMSeconds )
            {
               return( false );
            }
         }
         return( true );
      }
      void loop()
      {
         m_functorEventLoop();
      }
      void step()
      {
         m_timer=now();
         m_index++;
      }
      bool succeed()
      {
         lInfo("Step %d successfull", m_index);
         return(true);
      }
      bool failed()
      {
         lFatal("Failed at step %d"
                , m_index);
         return(false);
      }
      static int approximately( int isValue, int targetValue, int deviation = 25)
      {
         bool match= ( isValue > ( targetValue - deviation ) )
                  && ( isValue < ( targetValue + deviation ) );
         if(match)
         {
            return(targetValue);
         }
         return(isValue);
      }
      static int approximatelyPercent( int isValue, int targetValue, int percent)
      {
         return( approximately( isValue, targetValue, (targetValue*percent)/100 ) );
      }
      static bool matchesJittered( int isValue, int targetValue, int deviation)
      {
         bool match= ( isValue > ( targetValue - deviation ) )
                  && ( isValue < ( targetValue + deviation ) );

         return(match);
      }
      itimer_t now()
      {
         struct timeval tv;
         gettimeofday(&tv,NULL);
         return( ((uint64_t)tv.tv_sec*(uint64_t)_MSECS_PER_SEC)
                        + ( (uint64_t)tv.tv_usec / _USECS_PER_MSEC ) );
      }
      itimer_t elapsedMSeconds( itimer_t ref )
      {
         return( now() - ref );
      }
      int elapsedSeconds( itimer_t ref )
      {
         return( elapsedMSeconds( ref ) / _MSECS_PER_SEC );
      }
      void setTimeoutSeconds(int seconds)
      {
         m_timeoutSeconds=seconds;
      }
};


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! LEPTO_IMAGO_HPP
