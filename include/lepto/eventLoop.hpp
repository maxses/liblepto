#if ! defined LEPTO_EVENT_LOOP
#define LEPTO_EVENT_LOOP
/**---------------------------------------------------------------------------
 *
 * @file       eventLoop.cpp
 * @brief      Handle chain of events
 *
 * @date       20260319
 * @author     Maximilian Seesslen <mes@seesslen.net>
 * @copyright  SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <lepto/lepto.h>
#include <lepto/log.h>

#if IS_ENABLED( CONFIG_LEPTO_GLOBAL_EVENT_LOOP )
   #define virtual_eventLoop virtual
   #define override_eventLoop override
#else
   #define virtual_eventLoop
   #define override_eventLoop
   #define activateEventLoop( a )
   #define deactivateEventLoop( a )
#endif


/*--- Declarations ---------------------------------------------------------*/


class CEventLoop
{
#if IS_ENABLED( CONFIG_LEPTO_GLOBAL_EVENT_LOOP )
   private:
      static CEventLoop* m_first;
      CEventLoop* m_next=nullptr;
      #if IS_ENABLED( CONFIG_LEPTO_EVENT_LOOP_DEACTIVATABLE )
      bool m_active=false;
      #endif

   public:
      CEventLoop()
      {
         if(!m_first)
         {
            m_first=this;
         }
         else
         {
            CEventLoop* p=m_first;
            while( p->m_next)
            {
               p=p->m_next;
            }
            p->m_next=this;
         }
      }
      ~CEventLoop()
      {
         //lFatal("Destruct");
         CEventLoop** p=&m_first;
         while( *p != this  )
         {
            p=&( (*p)->m_next );
            if(!p)
            {
               lFatal("Destruct");
            }
         }
         *p=m_next;
      }
      
      virtual_eventLoop void eventLoop() = 0;
#if 0
      {
         printf("L %p\n", this);
      }
#endif

      virtual_eventLoop void cleanup()
      {
      }
      
      static void globalCleanup(); // __attribute__((error("Dont call this function!")));

      void activateEventLoop( bool active=true )
      {
         #if IS_ENABLED( CONFIG_LEPTO_EVENT_LOOP_DEACTIVATABLE )
            m_active=active;
         #endif
      }
      
      void deactivateEventLoop( bool active=true )
      {
         #if IS_ENABLED( CONFIG_LEPTO_EVENT_LOOP_DEACTIVATABLE )
            activateEventLoop( false );
         #endif
      }
#endif
      
   public:
      static void globalEventLoop(); // __attribute__((error("Dont call this function!")));
};


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! LEPTO_EVENT_LOOP
