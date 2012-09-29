#ifndef LEPTO_SIGNAL_POOL_STATIC_HPP
#define LEPTO_SIGNAL_POOL_STATIC_HPP
/**---------------------------------------------------------------------------
 *
 * @file    signalPoolStatic.hpp
 * @brief   Pool for C++ signals
 *
 * Signals can be enqueued into a list and at a different time point
 * (e.g. event loop) the connected slots are called.
 *
 * Unlike 'CPendingSignalPool' this pool does not create objects for each
 * signal. This pool only supports methods without any argument.
 * The benefit is that no malloc functions are used. Therefore it can be feeded
 * from ISR.
 *
 * @date   20170127
 * @author Maximilian Seesslen <mes@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <lepto/list.hpp>
#include <lepto/ring.hpp>


/*--- Declarations ---------------------------------------------------------*/


class CPendingSignalPoolStatic
{
   private:
      CRing< CSignal<void>* > pendingSignalList;

   public:
      CPendingSignalPoolStatic( int size )
         :pendingSignalList( size )
      {};

      void enqueueSignal( CSignal<void>& signal )
      {
         pendingSignalList << &signal;
      };

      void handlePendingSignals()
      {
         CSignal<void>* signal;
         while( ( signal=pendingSignalList.pop() ) )
         {
            signal->emitSignal();
         }
      };
};


/*--- Fin ------------------------------------------------------------------*/
#endif // ! ? LEPTO_SIGNAL_POOL_STATIC_HPP
