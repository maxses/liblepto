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


#include <stdio.h>
#include <lepto/lepto.h>
#include <lepto/log.h>
#include <lepto/eventLoop.hpp>


/*--- Declarations ---------------------------------------------------------*/


#if IS_ENABLED( CONFIG_LEPTO_GLOBAL_EVENT_LOOP )

// Is there a use to make it "__attribute__( (weak) )"
void CEventLoop::globalEventLoop()
{
   CEventLoop* p=m_first;
   while(p)
   {
      //if(p->m_active)
      {
         p->eventLoop();
      }
      p=p->m_next;
   }
   
   return;
}


CEventLoop* CEventLoop::m_first = nullptr;
#else

void CEventLoop::globalEventLoop()
{
   lFatal("NGEL");
}

#endif // ? ! CONFIG_GLOBAL_EVENT_LOOP


/*--- Fin ------------------------------------------------------------------*/
