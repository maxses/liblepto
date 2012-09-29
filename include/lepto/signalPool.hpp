#ifndef LEPTO_SIGNAL_POOL_HPP
#define LEPTO_SIGNAL_POOL_HPP
/**---------------------------------------------------------------------------
 *
 * @file    signalPool.hpp
 * @brief   Pool for C++ signals
 *
 * Signals can be enqueued into a list and at a different time point
 * (e.g. event loop) the connected slots are called.
 *
 * Enqueueing an signal creates an instance of an inherited
 * CPendingSignalBase. This meanss 'maloc' is involved. So this pool
 * should not be used from an ISR. For this usecase use
 * "CPendingSignalPoolStatic"
 *
 * @date   20170127
 * @author Maximilian Seesslen <mes@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <stdint.h>
#include <lepto/list.hpp>
#include <lepto/ring.hpp>


/*--- Declarations ---------------------------------------------------------*/


class CPendingSignalBase
{
public:
   virtual void shot()=0;
   virtual ~CPendingSignalBase() {};
};


template <typename sigReturn, typename sigType1>
class CPendingSignal: public CPendingSignalBase
{
   //CSignalMethod<sigReturn, sigType1> &m_signal;
   CSignal<sigReturn, sigType1> &m_signal;
   sigType1 m_storedArg;

   public:
      CPendingSignal( CSignal<sigReturn, sigType1> &signal, sigType1 arg )
         :m_signal(signal)
         ,m_storedArg( arg )
      {
      };
      virtual void shot() // overload;
      {
         m_signal.emitSignal( m_storedArg );
      };
};


template <typename sigReturn, typename sigType1, typename sigType2>
class CPendingSignal2: public CPendingSignalBase
{
   //CSignalMethod<sigReturn, sigType1> &m_signal;
   CSignal<sigReturn, sigType1, sigType2> &m_signal;
   sigType1 m_storedArg1;
   sigType2 m_storedArg2;

   public:
      CPendingSignal2( CSignal<sigReturn, sigType1, sigType2> &signal, sigType1 arg1, sigType2 arg2 )
         :m_signal(signal)
         ,m_storedArg1( arg1 )
         ,m_storedArg2( arg2 )
      {
      };
      virtual void shot() // overload;
      {
         m_signal.emitSignal( m_storedArg1, m_storedArg2 );
      };
};


template <typename sigReturn>
class CPendingSignal0:public  CPendingSignalBase
{
   CSignal<sigReturn> &m_signal;

   public:
      CPendingSignal0( CSignal<sigReturn> &signal )
         :m_signal(signal)
      {
      };
      virtual void shot() // overload
      {
         m_signal.emitSignal( );
      };
};


class CPendingSignalPool
{
   private:
      CRing<CPendingSignalBase*> pendingSignalList;

   public:
      CPendingSignalPool(int size)
         :pendingSignalList(size)
      {}

      void queueSignal(CPendingSignalBase *signal)
      {
         pendingSignalList << signal;
      };

      void handlePendingSignals()
      {
         CPendingSignalBase *signal;
         while( ( signal=pendingSignalList.pop() ) )
         {
            signal->shot();
            delete(signal);
         }
      };

      template <typename sigReturn, typename sigType1>
      void enqueueSignal( CSignal<sigReturn, sigType1> &signal, sigType1 arg)
      {
         pendingSignalList.push_back( new CPendingSignal( signal, arg ) );
         return;
      };

      template <typename sigReturn, typename sigType1, typename sigType2>
      void enqueueSignal( CSignal<sigReturn, sigType1, sigType2> &signal, sigType1 arg1, sigType2 arg2)
      {
         pendingSignalList.push_back( new CPendingSignal2( signal, arg1, arg2 ) );
         return;
      };

      /*
      template <typename sigReturn, typename ... sigTypes>
      void enqueueSignal( CSignal<sigReturn, sigTypes...> &signal, sigTypes ... args)
      {
         pendingSignalList.push_back( new CPendingSignal( signal, args... ) );
         return;
      };
      */
      template <typename sigReturn>
      void enqueueSignal( CSignal<sigReturn> &signal)
      {
         pendingSignalList.push_back( new CPendingSignal0( signal ) );
         return;
      };
};


/*--- Fin ------------------------------------------------------------------*/
#endif // ! ? LEPTO_SIGNAL_POOL_HPP
