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
 * @author Maximilian Seesslen <src@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <stdint.h>
#include <lepto/list.hpp>
#include <lepto/ring.hpp>


/*--- Declarations ---------------------------------------------------------*/


#if IS_ENABLED( CONFIG_LEPTO_NO_SIGNAL )

#if IS_ENABLED( CONFIG_LEPTO_SIGNAL_FUNCTION ) || IS_ENABLED( CONFIG_LEPTO_SIGNAL_METHOD )
   #erro CONFIG_LEPTO_NO_SIGNAL is set but also CONFIG_LEPTO_SIGNAL_FUNCTION or CONFIG_LEPTO_SIGNAL_METHOD
#endif

#else

// By default, use virtual signals supportunǵ methods but not functions. This
// may lead to CFunctorMethodAsFunction being used. 
#if ! defined ( CONFIG_LEPTO_SIGNAL_FUNCTION ) && ! defined ( CONFIG_LEPTO_SIGNAL_METHOD )
   #undef CONFIG_LEPTO_SIGNAL_FUNCTION
   #define CONFIG_LEPTO_SIGNAL_METHOD        1
#endif

// Only when CONFIG_LEPTO_SIGNAL_METHOD_AS_FUNCTION is defined, functors do not
// need to be virtual. The connect-method is a template and only there the
// class must be known.
#if ( IS_ENABLED( CONFIG_LEPTO_SIGNAL_FUNCTION ) && IS_ENABLED( CONFIG_LEPTO_SIGNAL_METHOD ) ) \
   || ( IS_ENABLED( CONFIG_LEPTO_SIGNAL_METHOD )   && ! IS_ENABLED( CONFIG_LEPTO_SIGNAL_METHOD_AS_FUNCTION ) )
   #define LEPTO_SIGNAL_VIRTUAL              virtual
   #define LEPTO_SIGNAL_DO_VIRTUAL           1
#else
   // Dont use virtual emit methods
   #define LEPTO_SIGNAL_VIRTUAL
   #define LEPTO_SIGNAL_DO_VIRTUAL           0
#endif

#if LEPTO_SIGNAL_DO_VIRTUAL || IS_ENABLED( CONFIG_LEPTO_SIGNAL_FUNCTION ) || IS_ENABLED( CONFIG_LEPTO_SIGNAL_CHAIN )
   #define LEPTO_SIGNAL_FUNCTOR_ALLOCATED    1
#endif

// The template 'sclotClass' can never be part of the signals class declaration
// because the class of the slot is not known at this timepoint of course.
template <typename sigReturn, typename ... sigTypes>
class CSignal;

template <typename sigReturn, typename ... sigTypes>
class CFunctor
{
   friend class CSignal<sigReturn, sigTypes...>;
       typedef const CFunctor<sigReturn, sigTypes...> CConstFunctor;
   
       #if IS_ENABLED( CONFIG_LEPTO_SIGNAL_CHAIN )
       CFunctor *m_next=nullptr;
       #endif

   public:
      #if LEPTO_SIGNAL_FUNCTOR_ALLOCATED
         virtual ~CFunctor()
         {};
      #endif
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
      sigReturn emitSignal( sigTypes ... args ) const //final
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
      slotClass *m_slotObject;
      sigReturn (slotClass::*m_methodPtr)( sigTypes ... args );

      constexpr CFunctorMethod( slotClass *slotObject, sigReturn (slotClass::*methodPtr)( sigTypes ... args ))
         :m_slotObject( slotObject )
         ,m_methodPtr( methodPtr )
      {
      }

      LEPTO_SIGNAL_VIRTUAL
      sigReturn emitSignal( sigTypes ... args ) const //final
      {
         return( (m_slotObject->*(this->m_methodPtr))( args... ) );
      }
};


template <typename sigReturn, typename ... sigTypes>
class CFunctorMethodAsFunction final
      #if LEPTO_SIGNAL_DO_VIRTUAL
          : public CFunctor<sigReturn, sigTypes...>
      #endif
{
      friend class CSignal<sigReturn, sigTypes...>;
      
      #if IS_ENABLED( CONFIG_LEPTO_SIGNAL_CHAIN )
         CFunctorMethodAsFunction *m_next=nullptr;
      #endif

   public:
   void* m_slotObject;
   sigReturn (*m_methodPtr)( void *, sigTypes ... args );
   
   #pragma GCC diagnostic push
   #pragma GCC diagnostic ignored "-Wpmf-conversions"
   
   template <class slotClass>
   constexpr CFunctorMethodAsFunction( slotClass* slotObject, sigReturn (slotClass::*methodPtr)( sigTypes ... args ))
       :m_slotObject( slotObject )
       ,m_methodPtr( (sigReturn (*)( void *, sigTypes ... args ))methodPtr )
   {
   }

   #if 1
   constexpr CFunctorMethodAsFunction( )
       :m_methodPtr( nullptr )
   {
   }
   template <class slotClass>
   void connect( slotClass* _slotObject, sigReturn (slotClass::*_methodPtr)( sigTypes ... args ))
   {
      m_slotObject=_slotObject;
      m_methodPtr=(sigReturn (*)( void *, sigTypes ... args ))_methodPtr;
   }
   #endif
   
   #pragma GCC diagnostic pop
   
   LEPTO_SIGNAL_VIRTUAL
       sigReturn emitSignal( sigTypes ... args ) const //final
   {
      if( m_methodPtr )
      {
         return( (*(this->m_methodPtr))( m_slotObject, args... ) );
      }
      return( (sigReturn)-1 );
   }
   bool isConnected() const
   {
      return( m_methodPtr != nullptr );
   }
   void disconnect()
   {
      m_methodPtr = nullptr;
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


#if LEPTO_SIGNAL_DO_VIRTUAL
   #define CFunctorAbstract CFunctor
   #define CFunctorMethodConcrete   CFunctorMethod
#elif IS_ENABLED( CONFIG_LEPTO_SIGNAL_FUNCTION )
   #define CFunctorAbstract CFunctorFunction
#elif IS_ENABLED( CONFIG_LEPTO_SIGNAL_METHOD )
   #if ! IS_ENABLED( CONFIG_LEPTO_SIGNAL_METHOD_AS_FUNCTION )
      #define CFunctorAbstract CFunctorMethod
      #define CFunctorMethodConcrete CFunctorMethod
   #else
      #define CFunctorAbstract CFunctorMethodAsFunction
      #define CFunctorMethodConcrete CFunctorMethodAsFunction
   #endif
#endif


// The template 'sclotClass' can never be part of the signals class declaration
// because the class of the slot is not known at this timepoint of course.
template <typename sigReturn, typename ... sigTypes>
class CSignal
{
   private:

      #if LEPTO_SIGNAL_DO_VIRTUAL
         // Can not be const when chain mechanism is used.
         CFunctor<sigReturn, sigTypes...> *m_pFunctor;
      #elif IS_ENABLED( CONFIG_LEPTO_SIGNAL_FUNCTION )
         const CFunctorFunction<sigReturn, sigTypes...> *m_pFunctor;
      #elif IS_ENABLED( CONFIG_LEPTO_SIGNAL_METHOD )
         //const CFunctorMethod<sigReturn, slotClass, sigTypes...>* m_pFunctor;
         #if LEPTO_SIGNAL_FUNCTOR_ALLOCATED
            CFunctorMethodConcrete<sigReturn, sigTypes...> *m_pFunctor;
         #else
            #if ! IS_ENABLED( CONFIG_LEPTO_SIGNAL_METHOD_AS_FUNCTION )
               //#error An unalocated functor is not allowed with CFunctorMethod. Define LEPTO_SIGNAL_FUNCTOR_ALLOCATED
            #endif
            CFunctorMethodConcrete<sigReturn, sigTypes...> m_pFunctor;
         #endif
      #else
         #error "Could not check signal configuration"
      #endif

   public:

      constexpr CSignal()
         #if LEPTO_SIGNAL_FUNCTOR_ALLOCATED
           :m_pFunctor( nullptr )
         #else
         #endif
      {
      };
      ~CSignal()
      {
      }

      #if IS_ENABLED( CONFIG_LEPTO_SIGNAL_FUNCTION )
      void connect( sigReturn (*_func)( sigTypes ... args ) )
      {
         m_pFunctor=new CFunctorFunction<sigReturn, sigTypes...>(_func);
      };
      #endif

      #if IS_ENABLED( CONFIG_LEPTO_SIGNAL_METHOD )
      // An connection 'costs' 24 Bytes of RAM
      #if LEPTO_SIGNAL_DO_VIRTUAL || 1
      template <class slotClass >
      #endif
      void connect( slotClass *slotObject, sigReturn (slotClass::*methodPtr)( sigTypes ... args ))
      {
         // if( methodPtr < ( sigReturn (slotClass::*)( sigTypes ... args ) )0x100ul )
         // if( (unsigned long long)(methodPtr) < 0x100 )
         // if( reinterpret_cast<std::intptr_t>(methodPtr) < 0x100 )
         // if( 0 )
         /*
         uintptr_t p=methodPtr;
         if( (uintptr_t)methodPtr < 0x100 )
         {
            lFatal("Inheriting virtual function?");
         }
         */
         #if IS_ENABLED( CONFIG_LEPTO_SIGNAL_CHAIN )
         
            #if LEPTO_SIGNAL_DO_VIRTUAL
               CFunctor<sigReturn, sigTypes...> **pFunctor=&m_pFunctor;
            #elif IS_ENABLED( CONFIG_LEPTO_SIGNAL_FUNCTION )
               CFunctorFunction<sigReturn, sigTypes...>**pFunctor=&m_pFunctor;
            #elif IS_ENABLED( CONFIG_LEPTO_SIGNAL_METHOD )
               CFunctorMethodConcrete<sigReturn, sigTypes...>**pFunctor=&m_pFunctor;
            #else
               #error "Could not check signal configuration"
            #endif
            
            while(*pFunctor)
            {
               pFunctor=&((*pFunctor)->m_next);
            }
            *pFunctor=new CFunctorMethodConcrete(slotObject, methodPtr);
         #else
            #if LEPTO_SIGNAL_FUNCTOR_ALLOCATED
               lAssert( m_pFunctor == nullptr );
            #else
               lAssert( ! m_pFunctor.isConnected() );
            #endif
               
            #if LEPTO_SIGNAL_FUNCTOR_ALLOCATED
               m_pFunctor=new CFunctorMethodConcrete( slotObject, methodPtr );
            #else
               m_pFunctor.connect(slotObject, methodPtr);
            #endif
         #endif
      }
      #endif

      void disconnect()
      {
         #if IS_ENABLED( CONFIG_LEPTO_SIGNAL_CHAIN )
            lFatal("Not implemented");
         #else
            #if LEPTO_SIGNAL_FUNCTOR_ALLOCATED
               if( m_pFunctor )
               {
                  delete m_pFunctor;
                  m_pFunctor = nullptr;
               }
            #else
               m_pFunctor.disconnect();
            #endif
         #endif
      }

      #if 0
      // An connection 'costs' 16 Bytes of RAM;
      void connectCaller( const CFunctor<sigReturn, sigTypes...> *fp )
      {
         functor=fp;
      };
      #endif

      void emitSignal( sigTypes ... args ) const
      {
         #if IS_ENABLED( CONFIG_LEPTO_SIGNAL_CHAIN )
            #if ! LEPTO_SIGNAL_FUNCTOR_ALLOCATED
               #error CONFIG_LEPTO_SIGNAL_CHAIN Only works with allocated functors
            #endif
         
            #if LEPTO_SIGNAL_DO_VIRTUAL
               const CFunctor<sigReturn, sigTypes...> * const*pFunctor=&m_pFunctor;
            #elif IS_ENABLED( CONFIG_LEPTO_SIGNAL_FUNCTION )
               const CFunctorFunction<sigReturn, sigTypes...>* const *pFunctor=&m_pFunctor;
            #elif IS_ENABLED( CONFIG_LEPTO_SIGNAL_METHOD )
               const CFunctorMethodConcrete<sigReturn, sigTypes...>* const *pFunctor=&m_pFunctor;
            #else
               #error "Could not check signal configuration"
            #endif

            while(*pFunctor)
            {
               (*pFunctor)->emitSignal( args ... );
               pFunctor=&((*pFunctor)->m_next);
            }
         #else
            #if LEPTO_SIGNAL_FUNCTOR_ALLOCATED
               if( m_pFunctor )
               {
                  m_pFunctor->emitSignal( args ... );
               }
            #else
               if( m_pFunctor.isConnected() )
               {
                  m_pFunctor.emitSignal( args ... );
               }
            #endif
         #endif
         
         return;
      };
      
      int slotCount( ) const
      {
         int count=0;
#if IS_ENABLED( CONFIG_LEPTO_SIGNAL_CHAIN )
         const CFunctor<sigReturn, sigTypes...> * const*pFunctor=&m_pFunctor;
         while(*pFunctor)
         {
            count++;
            pFunctor=&((*pFunctor)->m_next);
         }
#else
         if( m_pFunctor )
         {
            count++;
         }
#endif
         
         return(count);
      };
      
      /**
       * @brief Emit single signal and return its return value
       * 
       * 'single' is not a typo of 'signal'. This method is needed when return 
       * code matters.
       * 
       * @param args
       * @return 
       */
      sigReturn emitSingle( sigTypes ... args ) const
      {
         #if LEPTO_SIGNAL_FUNCTOR_ALLOCATED
            if( m_pFunctor )
            {
               return( m_pFunctor->emitSignal( args ... ) );
            }
         #else
            if( m_pFunctor.isConnected() )
            {
               return( m_pFunctor.emitSignal( args ... ) );
            }
         #endif
         return( (sigReturn)-1 );
      }

      #if 0
      const CSignal<sigReturn, sigTypes...> *operator =(const CFunctor<sigReturn, sigTypes...> *_functor)
      {
         functor=_functor;
         return(this);
      }
      #endif
};


#endif // ? CONFIG_LEPTO_NO_SIGNAL else


template <typename sigReturn, typename ... sigTypes>
class CSimpleSignal
{
   public:
      void* m_slotObject;
      sigReturn (*m_methodPtr)( void *, sigTypes ... args );

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpmf-conversions"
   
   constexpr CSimpleSignal( )
       :m_methodPtr( nullptr )
   {
   }
   
   template <class slotClass>
   void connect( slotClass* _slotObject, sigReturn (slotClass::*_methodPtr)( sigTypes ... args ))
   {
      m_slotObject=_slotObject;
      m_methodPtr=(sigReturn (*)( void *, sigTypes ... args ))_methodPtr;
   }

#pragma GCC diagnostic pop
   
   sigReturn emitSignal( sigTypes ... args ) const
   {
      if( m_methodPtr )
      {
         return( (*(this->m_methodPtr))( m_slotObject, args... ) );
      }
      return( (sigReturn)-1 );
   }
};


/*--- Fin ------------------------------------------------------------------*/
#endif // ! ? LEPTO_SIGNAL_HPP
