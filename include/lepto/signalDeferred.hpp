#ifndef LEPTO_SIGNAL_DEFERRED_HPP
#define LEPTO_SIGNAL_DEFERRED_HPP
/**---------------------------------------------------------------------------
 *
 * @file    signalDeferred.hpp
 * @brief   Handle deferred signals
 *
 * Signals can be emited in interrupt handlers but slots are actually called 
 * in applications event loop.
 *
 * @date   20260321
 * @author Maximilian Seesslen <mes@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <lepto/signal.hpp>
#include <lepto/eventLoop.hpp>
#include <lepto/ring.hpp>
#include <lepto/tuple.hpp>


/*--- Declarations ---------------------------------------------------------*/


template <typename sigReturn, typename ... sigTypes>
class CSignalDeferred: public CSignal<sigReturn, sigTypes...>, CEventLoop
{
   private:

      #if LEPTO_SIGNAL_DO_VIRTUAL
         CFunctor<sigReturn, sigTypes...> *functor;
      #elif IS_ENABLED( CONFIG_LEPTO_SIGNAL_FUNCTION )
         const CFunctorFunction<sigReturn, sigTypes...> *functor;
      #elif IS_ENABLED( CONFIG_LEPTO_SIGNAL_METHOD )
         const CFunctorMethod<sigReturn, slotClass, sigTypes...> *functor;
      #else
         #error "Could not check signal configuration"
      #endif

      CRing< STuple<sigTypes...> > p;

   public:

      constexpr CSignalDeferred( int count = 16 )
         :functor{0}
           ,p( count )
      {
         // "count=16" worked ok with button as input. But don't print too much
         // in the slot.
         
         #if IS_ENABLED( CONFIG_LEPTO_GLOBAL_EVENT_LOOP )
         activateEventLoop(true);
         #endif
      };
      ~CSignalDeferred()
      {
      }

      void emitDeferred( sigTypes ... args )
      {
         if( ! p.pushable() )
         {
            lFatal("CNPS");
         }
         p.push_back( STuple<sigTypes ...>( doForward<sigTypes>(args)... ) );
      }

      virtual_eventLoop void eventLoop() override_eventLoop
      {
         STuple<sigTypes ...> *argTuple;
         while( ( argTuple = p.frontEntry() ) )
         {
            callMethodWithTuple( this, &CSignalDeferred::emit, *argTuple );
            p.dropFront();
         }
         
      }
};


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! LEPTO_SIGNAL_DEFERRED_HPP
