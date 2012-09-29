#ifndef LEPTO_SIGNAL_HPP
#define LEPTO_SIGNAL_HPP
/**---------------------------------------------------------------------------
 *
 * @file    signal.hpp
 * @brief   C++ signal handling
 *
 * Signals can be connected either to functions or to class methods.
 *
 * Example for function:
 *        CSignal<int, int> mySignal;
 *        mySignal.connect( &mySlot );
 *        mySignal.emitSignal(123);
 *
 * Example for class method:
 *        CSignal<int, int> mySignal;
 *        CMyClass myClassObject;
 *
 *    mySignal.connect( &myClassObject, &CMyClass::mySlot );
 *        mySignal.emitSignal(123);
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


#undef CONFIG_LEPTO_SIGNAL_FUNCTION
#undef CONFIG_LEPTO_SIGNAL_METHOD
#define CONFIG_LEPTO_SIGNAL_FUNCTION      1
#define CONFIG_LEPTO_SIGNAL_METHOD        1


// By default, use virtual signals supportunǵ fungtions and methods
#if ! defined ( CONFIG_LEPTO_SIGNAL_FUNCTION ) && ! defined ( CONFIG_LEPTO_SIGNAL_METHOD )
   #define CONFIG_LEPTO_SIGNAL_FUNCTION      1
   #define CONFIG_LEPTO_SIGNAL_METHOD        1
#endif

#if IS_ENABLED( CONFIG_LEPTO_SIGNAL_FUNCTION ) && IS_ENABLED( CONFIG_LEPTO_SIGNAL_METHOD )
   #define LEPTO_SIGNAL_VIRTUAL              virtual
   #define LEPTO_SIGNAL_DO_VIRTUAL           1
#else
   // Dont use virtual emit methods
   #define LEPTO_SIGNAL_VIRTUAL
   #define LEPTO_SIGNAL_DO_VIRTUAL           0
#endif

template <typename sigReturn, typename ... sigTypes>
class CFunctor
{
   public:
      virtual sigReturn emitSignal( sigTypes ... args ) const = 0;
};


#if IS_ENABLED( CONFIG_LEPTO_SIGNAL_FUNCTION )

/// A function pointer to a single function
template <typename sigReturn, typename ... sigTypes>
class CFunctorFunction
         final
      #if LEPTO_SIGNAL_DO_VIRTUAL
         : public CFunctor<sigReturn, sigTypes...>
      #endif
{
   private:
      sigReturn (*functionPtr)( sigTypes ... args );
   public:
      CFunctorFunction( sigReturn (*_functionPtr)( sigTypes ... args ))
      {
         functionPtr=_functionPtr;
      }

      LEPTO_SIGNAL_VIRTUAL
      sigReturn emitSignal( sigTypes ... args ) const final
      {
         return( functionPtr(args...) );
      }
};

#endif // ? CONFIG_LEPTO_SIGNAL_FUNCTION


#if IS_ENABLED( CONFIG_LEPTO_SIGNAL_METHOD )

/// A function pointer to a class method
template <typename sigReturn, class slotClass, typename ... sigTypes>
class CFunctorMethod final
      #if LEPTO_SIGNAL_DO_VIRTUAL
         : public CFunctor<sigReturn, sigTypes...>
      #endif
{
   public:
      slotClass *slotObject;
      sigReturn (slotClass::*methodPtr)( sigTypes ... args );

      constexpr CFunctorMethod( slotClass *_slotObject, sigReturn (slotClass::*_methodPtr)( sigTypes ... args ))
         :slotObject(_slotObject)
         ,methodPtr(_methodPtr)
      {
      }

      LEPTO_SIGNAL_VIRTUAL
      sigReturn emitSignal( sigTypes ... args ) const final
      {
         return( (slotObject->*(this->methodPtr))( args... ) );
      }
};

#endif // ? CONFIG_LEPTO_SIGNAL_METHOD

#if 0

template <typename sigReturn, class slotClass, typename ... sigTypes>
class CSignalMethod
      #if LEPTO_SIGNAL_DO_VIRTUAL
         : public CFunctor<sigReturn, sigTypes...>
      #endif
{
   public:
      //ret (*func)( types ... args );
      slotClass *slotObject;
      sigReturn (slotClass::*methodPtr)( sigTypes ... args );

      CSignalMethod( )
         :slotObject(nullptr)
         ,methodPtr(nullptr)
      {}

      constexpr CFunctorMethod( slotClass *_slotObject, sigReturn (slotClass::*_methodPtr)( sigTypes ... args ))
         :slotObject(_slotObject)
         ,methodPtr(_methodPtr)
      {
      }

      //CFunctorMethod( slotClass *_slotObject, sigReturn (slotClass::*_methodPtr)( sigTypes ... args ))
      void connect( slotClass *_slotObject, sigReturn (slotClass::*_methodPtr)( sigTypes ... args ))
      {
         methodPtr=_methodPtr;
         slotObject=_slotObject;
      }

      LEPTO_SIGNAL_VIRTUAL
      sigReturn emitSignal( sigTypes ... args )
      {
         return( (slotObject->*(this->methodPtr))( args... ) );
      }
};

#endif // 0


// Size is 4 Bytes
//#if IS_ENABLED( CONFIG_LEPTO_SIGNAL_METHOD ) && ! IS_ENABLED( CONFIG_LEPTO_FUNCTION_FUNCTION )
//template <typename sigReturn, class slotClass, typename ... sigTypes>
//#else
template <typename sigReturn, typename ... sigTypes>
//#endif
class CSignal
{
   private:

      #if LEPTO_SIGNAL_DO_VIRTUAL
         const CFunctor<sigReturn, sigTypes...> *functor;
      #elif IS_ENABLED( CONFIG_LEPTO_SIGNAL_FUNCTION )
         const CFunctorFunction<sigReturn, sigTypes...> *functor;
      #elif IS_ENABLED( CONFIG_LEPTO_SIGNAL_METHOD )
         const CFunctorMethod<sigReturn, slotClass, sigTypes...> *functor;
      #else
         #error "Could not check signal configuration"
      #endif

   public:

      constexpr CSignal()
         :functor{0}
      {
      };
      ~CSignal()
      {
      }

      #if IS_ENABLED( CONFIG_LEPTO_SIGNAL_FUNCTION )
      void connect( sigReturn (*_func)( sigTypes ... args ) )
      {
         functor=new CFunctorFunction<sigReturn, sigTypes...>(_func);
      };
      #endif

      #if IS_ENABLED( CONFIG_LEPTO_SIGNAL_METHOD )
      // An connection 'costs' 24 Bytes of RAM
      template <class slotClass >
      void connect( slotClass *slotObject, sigReturn (slotClass::*_methodPtr)( sigTypes ... args ))
      {
         functor=new CFunctorMethod<sigReturn, slotClass, sigTypes...>(slotObject, _methodPtr);
      };
      #endif

      #if 0
      // An connection 'costs' 16 Bytes of RAM;
      void connectCaller( const CFunctor<sigReturn, sigTypes...> *fp )
      {
         functor=fp;
      };
      #endif

      sigReturn emitSignal( sigTypes ... args ) const
      {
         if(functor)
         {
            return( functor->emitSignal( args ... ) );
         }

         return( (sigReturn)0 );
      };

      #if 0
      const CSignal<sigReturn, sigTypes...> *operator =(const CFunctor<sigReturn, sigTypes...> *_functor)
      {
         functor=_functor;
         return(this);
      }
      #endif
};


/*--- Fin ------------------------------------------------------------------*/
#endif // ! ? LEPTO_SIGNAL_HPP
