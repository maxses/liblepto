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

#if IS_ENABLED( CONFIG_LEPTO_GLOBAL_EVENT_LOOP )
   #define virtual_eventLoop virtual
   #define override_eventLoop override
#else
   #define virtual_eventLoop
   #define override_eventLoop
#endif


/*--- Declarations ---------------------------------------------------------*/


class CEventLoop
{
#if IS_ENABLED( CONFIG_LEPTO_GLOBAL_EVENT_LOOP )
   private:
      static CEventLoop* m_first;
      CEventLoop* m_next=nullptr;
      //bool m_active=false;

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

      virtual_eventLoop void eventLoop() = 0;
#if 0
      {
         printf("L %p\n", this);
      }
#endif

      virtual_eventLoop void cleanup()
      {
      }

      void activateEventLoop( bool active=true )
      {
         //m_active=active;
      }

      //static void allEventLoops()
      static void globalCleanup(); // __attribute__((error("Dont call this function!")));
#endif
      
   public:
      static void globalEventLoop(); // __attribute__((error("Dont call this function!")));
};


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! LEPTO_EVENT_LOOP
